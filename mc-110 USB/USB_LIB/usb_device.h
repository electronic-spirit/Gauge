
#ifndef __USB_DEVICE__H__
#define __USB_DEVICE__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "usbd_def.h"
#include "usbd_cdc.h"
#include  "usbd_msc_bot.h"
#include "usbd_msc_scsi.h"


#define MSC_interface					1
#define CDC_cmd_interface				2
#define CDC_interface					3

#define MSC_OUT_EP                      0x01
#define MSC_IN_EP                       0x81
#define CDC_CMD_EP                      0x82
#define CDC_OUT_EP                      0x03
#define CDC_IN_EP                       0x83

#define USB_MSC_CDC_CONFIG_DESC_SIZ       98




 typedef struct
 {
	uint32_t                 max_lun;
	uint32_t                 interface;
	uint8_t                  bot_state;
	uint8_t                  bot_status;
	uint32_t                 bot_data_length;
	uint8_t                  bot_data[MSC_MEDIA_PACKET];
	USBD_MSC_BOT_CBWTypeDef  cbw;
	USBD_MSC_BOT_CSWTypeDef  csw;

	USBD_SCSI_SenseTypeDef   scsi_sense [SENSE_LIST_DEEPTH];
	uint8_t                  scsi_sense_head;
	uint8_t                  scsi_sense_tail;
	uint8_t                  scsi_medium_state;

	uint16_t                 scsi_blk_size;
	uint32_t                 scsi_blk_nbr;

	uint32_t                 scsi_blk_addr;
	uint32_t                 scsi_blk_len;

	uint32_t data[CDC_DATA_HS_MAX_PACKET_SIZE / 4U];      /* Force 32bits alignment */
	uint8_t  CmdOpCode;
	uint8_t  CmdLength;
	uint8_t  *RxBuffer;
	uint8_t  *TxBuffer;
	uint32_t RxLength;
	uint32_t TxLength;

	__IO uint32_t TxState;
	__IO uint32_t RxState;
 } USBD_CDCMSC_HandleTypeDef;





/** USB Device initialization function. */
void MX_USB_DEVICE_Init(void);













#ifdef __cplusplus
}
#endif

#endif /* __USB_DEVICE__H__ */


