
#include "usbd_msc_bot.h"
#include "usbd_msc.h"
#include "usbd_msc_scsi.h"
#include "usbd_ioreq.h"
#include "usb_device.h"




static void MSC_BOT_SendData(USBD_HandleTypeDef *pdev, uint8_t *pbuf, uint32_t len);
static void MSC_BOT_CBW_Decode(USBD_HandleTypeDef *pdev);
static void MSC_BOT_Abort(USBD_HandleTypeDef *pdev);
/**
  * @}
  */


/** @defgroup MSC_BOT_Private_Functions
  * @{
  */


/**
  * @brief  MSC_BOT_Init
  *         Initialize the BOT Process
  * @param  pdev: device instance
  * @retval None
  */
void MSC_BOT_Init(USBD_HandleTypeDef *pdev)
{
  USBD_CDCMSC_HandleTypeDef *hmsc = (USBD_CDCMSC_HandleTypeDef *)pdev->pClassDataMSC;

  if (hmsc == NULL)
  {
    return;
  }

  hmsc->bot_state = USBD_BOT_IDLE;
  hmsc->bot_status = USBD_BOT_STATUS_NORMAL;

  hmsc->scsi_sense_tail = 0U;
  hmsc->scsi_sense_head = 0U;
  hmsc->scsi_medium_state = SCSI_MEDIUM_UNLOCKED;

  ((USBD_StorageTypeDef *)pdev->pUserDataMSC)->Init(0U);

  (void)USBD_LL_FlushEP(pdev, MSC_OUT_EP);
  (void)USBD_LL_FlushEP(pdev, MSC_IN_EP);

  /* Prepare EP to Receive First BOT Cmd */
  (void)USBD_LL_PrepareReceive(pdev, MSC_OUT_EP, (uint8_t *)&hmsc->cbw,
                               USBD_BOT_CBW_LENGTH);
}

/**
  * @brief  MSC_BOT_Reset
  *         Reset the BOT Machine
  * @param  pdev: device instance
  * @retval  None
  */
void MSC_BOT_Reset(USBD_HandleTypeDef *pdev)
{
  USBD_CDCMSC_HandleTypeDef *hmsc = (USBD_CDCMSC_HandleTypeDef *)pdev->pClassDataMSC;

  if (hmsc == NULL)
  {
    return;
  }

  hmsc->bot_state  = USBD_BOT_IDLE;
  hmsc->bot_status = USBD_BOT_STATUS_RECOVERY;

  (void)USBD_LL_ClearStallEP(pdev, MSC_IN_EP);
  (void)USBD_LL_ClearStallEP(pdev, MSC_OUT_EP);

  /* Prepare EP to Receive First BOT Cmd */
  (void)USBD_LL_PrepareReceive(pdev, MSC_OUT_EP, (uint8_t *)&hmsc->cbw,
                               USBD_BOT_CBW_LENGTH);
}

/**
  * @brief  MSC_BOT_DeInit
  *         DeInitialize the BOT Machine
  * @param  pdev: device instance
  * @retval None
  */
void MSC_BOT_DeInit(USBD_HandleTypeDef  *pdev)
{
  USBD_CDCMSC_HandleTypeDef *hmsc = (USBD_CDCMSC_HandleTypeDef *)pdev->pClassDataMSC;

  if (hmsc != NULL)
  {
    hmsc->bot_state = USBD_BOT_IDLE;
  }
}

/**
  * @brief  MSC_BOT_DataIn
  *         Handle BOT IN data stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval None
  */
void MSC_BOT_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  UNUSED(epnum);

  USBD_CDCMSC_HandleTypeDef *hmsc = (USBD_CDCMSC_HandleTypeDef *)pdev->pClassDataMSC;

  if (hmsc == NULL)
  {
    return;
  }

  switch (hmsc->bot_state)
  {
    case USBD_BOT_DATA_IN:
      if (SCSI_ProcessCmd(pdev, hmsc->cbw.bLUN, &hmsc->cbw.CB[0]) < 0)
      {
        MSC_BOT_SendCSW(pdev, USBD_CSW_CMD_FAILED);
      }
      break;

    case USBD_BOT_SEND_DATA:
    case USBD_BOT_LAST_DATA_IN:
      MSC_BOT_SendCSW(pdev, USBD_CSW_CMD_PASSED);
      break;

    default:
      break;
  }
}
/**
  * @brief  MSC_BOT_DataOut
  *         Process MSC OUT data
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval None
  */
void MSC_BOT_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  UNUSED(epnum);

  USBD_CDCMSC_HandleTypeDef *hmsc = (USBD_CDCMSC_HandleTypeDef *)pdev->pClassDataMSC;

  if (hmsc == NULL)
  {
    return;
  }

  switch (hmsc->bot_state)
  {
    case USBD_BOT_IDLE:
      MSC_BOT_CBW_Decode(pdev);
      break;

    case USBD_BOT_DATA_OUT:
      if (SCSI_ProcessCmd(pdev, hmsc->cbw.bLUN, &hmsc->cbw.CB[0]) < 0)
      {
        MSC_BOT_SendCSW(pdev, USBD_CSW_CMD_FAILED);
      }
      break;

    default:
      break;
  }
}

/**
  * @brief  MSC_BOT_CBW_Decode
  *         Decode the CBW command and set the BOT state machine accordingly
  * @param  pdev: device instance
  * @retval None
  */
static void  MSC_BOT_CBW_Decode(USBD_HandleTypeDef *pdev)
{
  USBD_CDCMSC_HandleTypeDef *hmsc = (USBD_CDCMSC_HandleTypeDef *)pdev->pClassDataMSC;

  if (hmsc == NULL)
  {
    return;
  }

  hmsc->csw.dTag = hmsc->cbw.dTag;
  hmsc->csw.dDataResidue = hmsc->cbw.dDataLength;

  if ((USBD_LL_GetRxDataSize(pdev, MSC_OUT_EP) != USBD_BOT_CBW_LENGTH) ||
      (hmsc->cbw.dSignature != USBD_BOT_CBW_SIGNATURE) ||
      (hmsc->cbw.bLUN > 1U) || (hmsc->cbw.bCBLength < 1U) ||
      (hmsc->cbw.bCBLength > 16U))
  {
    SCSI_SenseCode(pdev, hmsc->cbw.bLUN, ILLEGAL_REQUEST, INVALID_CDB);

    hmsc->bot_status = USBD_BOT_STATUS_ERROR;
    MSC_BOT_Abort(pdev);
  }
  else
  {
    if (SCSI_ProcessCmd(pdev, hmsc->cbw.bLUN, &hmsc->cbw.CB[0]) < 0)
    {
      if (hmsc->bot_state == USBD_BOT_NO_DATA)
      {
        MSC_BOT_SendCSW(pdev, USBD_CSW_CMD_FAILED);
      }
      else
      {
        MSC_BOT_Abort(pdev);
      }
    }
    /* Burst xfer handled internally */
    else if ((hmsc->bot_state != USBD_BOT_DATA_IN) &&
             (hmsc->bot_state != USBD_BOT_DATA_OUT) &&
             (hmsc->bot_state != USBD_BOT_LAST_DATA_IN))
    {
      if (hmsc->bot_data_length > 0U)
      {
        MSC_BOT_SendData(pdev, hmsc->bot_data, hmsc->bot_data_length);
      }
      else if (hmsc->bot_data_length == 0U)
      {
        MSC_BOT_SendCSW(pdev, USBD_CSW_CMD_PASSED);
      }
      else
      {
        MSC_BOT_Abort(pdev);
      }
    }
    else
    {
      return;
    }
  }
}

/**
  * @brief  MSC_BOT_SendData
  *         Send the requested data
  * @param  pdev: device instance
  * @param  buf: pointer to data buffer
  * @param  len: Data Length
  * @retval None
  */
static void  MSC_BOT_SendData(USBD_HandleTypeDef *pdev, uint8_t *pbuf, uint32_t len)
{
  USBD_CDCMSC_HandleTypeDef *hmsc = (USBD_CDCMSC_HandleTypeDef *)pdev->pClassDataMSC;

  uint32_t length = MIN(hmsc->cbw.dDataLength, len);

  if (hmsc == NULL)
  {
    return;
  }

  hmsc->csw.dDataResidue -= len;
  hmsc->csw.bStatus = USBD_CSW_CMD_PASSED;
  hmsc->bot_state = USBD_BOT_SEND_DATA;

  (void)USBD_LL_Transmit(pdev, MSC_IN_EP, pbuf, length);
}

/**
  * @brief  MSC_BOT_SendCSW
  *         Send the Command Status Wrapper
  * @param  pdev: device instance
  * @param  status : CSW status
  * @retval None
  */
void  MSC_BOT_SendCSW(USBD_HandleTypeDef *pdev, uint8_t CSW_Status)
{
  USBD_CDCMSC_HandleTypeDef *hmsc = (USBD_CDCMSC_HandleTypeDef *)pdev->pClassDataMSC;

  if (hmsc == NULL)
  {
    return;
  }

  hmsc->csw.dSignature = USBD_BOT_CSW_SIGNATURE;
  hmsc->csw.bStatus = CSW_Status;
  hmsc->bot_state = USBD_BOT_IDLE;

  (void)USBD_LL_Transmit(pdev, MSC_IN_EP, (uint8_t *)&hmsc->csw,
                         USBD_BOT_CSW_LENGTH);

  /* Prepare EP to Receive next Cmd */
  (void)USBD_LL_PrepareReceive(pdev, MSC_OUT_EP, (uint8_t *)&hmsc->cbw,
                               USBD_BOT_CBW_LENGTH);
}

/**
  * @brief  MSC_BOT_Abort
  *         Abort the current transfer
  * @param  pdev: device instance
  * @retval status
  */

static void  MSC_BOT_Abort(USBD_HandleTypeDef *pdev)
{
  USBD_CDCMSC_HandleTypeDef *hmsc = (USBD_CDCMSC_HandleTypeDef *)pdev->pClassDataMSC;

  if (hmsc == NULL)
  {
    return;
  }

  if ((hmsc->cbw.bmFlags == 0U) &&
      (hmsc->cbw.dDataLength != 0U) &&
      (hmsc->bot_status == USBD_BOT_STATUS_NORMAL))
  {
    (void)USBD_LL_StallEP(pdev, MSC_OUT_EP);
  }

  (void)USBD_LL_StallEP(pdev, MSC_IN_EP);

  if (hmsc->bot_status == USBD_BOT_STATUS_ERROR)
  {
    (void)USBD_LL_StallEP(pdev, MSC_IN_EP);
    (void)USBD_LL_StallEP(pdev, MSC_OUT_EP);
  }
}

/**
  * @brief  MSC_BOT_CplClrFeature
  *         Complete the clear feature request
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval None
  */

void  MSC_BOT_CplClrFeature(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_CDCMSC_HandleTypeDef *hmsc = (USBD_CDCMSC_HandleTypeDef *)pdev->pClassDataMSC;

  if (hmsc == NULL)
  {
    return;
  }

  if (hmsc->bot_status == USBD_BOT_STATUS_ERROR) /* Bad CBW Signature */
  {
    (void)USBD_LL_StallEP(pdev, MSC_IN_EP);
    (void)USBD_LL_StallEP(pdev, MSC_OUT_EP);
  }
  else if (((epnum & 0x80U) == 0x80U) && (hmsc->bot_status != USBD_BOT_STATUS_RECOVERY))
  {
    MSC_BOT_SendCSW(pdev, USBD_CSW_CMD_FAILED);
  }
  else
  {
    return;
  }
}
/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
