
#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage_if.h"
#include "usbd_cdc_if.h"



static uint8_t  USBD_MSC_CDC_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  USBD_MSC_CDC_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  USBD_MSC_CDC_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t  USBD_MSC_CDC_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  USBD_MSC_CDC_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  USBD_MSC_CDC_EP0_RxReady(USBD_HandleTypeDef *pdev);
uint8_t *USBD_MSC_CDC_GetFSCfgDesc(uint16_t *length);
uint8_t *USBD_MSC_CDC_GetHSCfgDesc(uint16_t *length);
uint8_t *USBD_MSC_CDC_GetOtherSpeedCfgDesc(uint16_t *length);
uint8_t *USBD_MSC_CDC_GetDeviceQualifierDescriptor(uint16_t *length);


USBD_HandleTypeDef hUsbDeviceFS;

USBD_ClassTypeDef  USBD_MSC_CDC =
{
	USBD_MSC_CDC_Init,
	USBD_MSC_CDC_DeInit,
	USBD_MSC_CDC_Setup,
	NULL, /*EP0_TxSent*/
	USBD_MSC_CDC_EP0_RxReady, /*EP0_RxReady*/
	USBD_MSC_CDC_DataIn,
	USBD_MSC_CDC_DataOut,
	NULL, /*SOF */
	NULL,
	NULL,
	USBD_MSC_CDC_GetHSCfgDesc,
	USBD_MSC_CDC_GetFSCfgDesc,
	USBD_MSC_CDC_GetOtherSpeedCfgDesc,
	USBD_MSC_CDC_GetDeviceQualifierDescriptor,
};






void MX_USB_DEVICE_Init(void)
{
	USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
	USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC_CDC);

	USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
	USBD_MSC_RegisterStorage(&hUsbDeviceFS, &USBD_Storage_Interface_fops_FS);
	USBD_Start(&hUsbDeviceFS);
}




/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
__ALIGN_BEGIN uint8_t USBD_MSC_CDC_CfgHSDesc[98]  __ALIGN_END =
{
	// Дескриптор конфигурации
	0x09,        //bLength
	0x02,        //bDescriptorType
	98,        //wTotalLength   (128 bytes)
	0x00,
	0x03,        //bNumInterfaces
	0x01,        //bConfigurationValue
	0x00,        //iConfiguration
	0x80,        //bmAttributes   (Bus-powered Device)
	0xFA,        //bMaxPower      (500 mA)

	// Дескриптор интерфейса 2 Mass Storage
	0x09,        //bLength
	0x04,        //bDescriptorType
	0x01,        //bInterfaceNumber
	0x00,        //bAlternateSetting
	0x02,        //bNumEndPoints
	0x08,        //bInterfaceClass      (Mass Storage Device Class)
	0x06,        //bInterfaceSubClass   (Transparent SCSI subclass)
	0x50,        //bInterfaceProtocol   (Bulk only transport)
	0x05,        //iInterface   "ST-Link mass storage"

	//Дескриптор 1 конечной точки интерфейса 2
	0x07,        //bLength
	0x05,        //bDescriptorType
	0x83,        //bEndpointAddress  (IN endpoint 1)
	0x02,        //bmAttributes      (Transfer: Bulk / Synch: None / Usage: Data)
	0x40,        //wMaxPacketSize    (64 bytes)
	0x00,
	0x00,        //bInterval

	//Дескриптор 2 конечной точки интерфейса 2
	0x07,        //bLength
	0x05,        //bDescriptorType
	0x03,        //bEndpointAddress  (OUT endpoint 1)
	0x02,        //bmAttributes      (Transfer: Bulk / Synch: None / Usage: Data)
	0x40,        //wMaxPacketSize    (64 bytes)
	0x00,
	0x00,        //bInterval

	// Дескриптор ассоциации интерфейсов
	0x08,        //bLength
	0x0B,        //bDescriptorType
	0x02,        //bFirstInterface
	0x02,        //bInterfaceCount
	0x02,        //bFunctionClass      (Communication Device Class)
	0x02,        //bFunctionSubClass   (Abstract Control Model - ACM)
	0x01,        //bFunctionProtocol   (ITU-T V.250)
	0x06,        //iFunction   "ST-Link VCP Ctrl"

	// Дескриптор инрефейса 3 CDC
	0x09,        //bLength
	0x04,        //bDescriptorType
	0x02,        //bInterfaceNumber
	0x00,        //bAlternateSetting
	0x01,        //bNumEndPoints
	0x02,        //bInterfaceClass      (Communication Device Class)
	0x02,        //bInterfaceSubClass   (Abstract Control Model - ACM)
	0x01,        //bInterfaceProtocol   (ITU-T V.250)
	0x06,        //iInterface   "ST-Link VCP Ctrl"

	//CDC Header Functional Descriptor:
	0x05,        //bFunctionalLength
	0x24,        //bDescriptorType
	0x00,        //bDescriptorSubtype
	0x10,        //bcdCDC
	0x01,

	//CDC Call Management Functional Descriptor:
	0x05,        //bFunctionalLength
	0x24,        //bDescriptorType
	0x01,        //bDescriptorSubtype
	0x00,        //bmCapabilities
	0x03,        //bDataInterface

	//CDC Abstract Control Management Functional Descriptor:
	0x04,        //bFunctionalLength
	0x24,        //bDescriptorType
	0x02,        //bDescriptorSubtype
	0x06,        //bmCapabilities

	//CDC Union Functional Descriptor:
	0x05,        //bFunctionalLength
	0x24,        //bDescriptorType
	0x06,        //bDescriptorSubtype
	0x02,        //bControlInterface
	0x03,        //bSubordinateInterface(0)

	//Endpoint Descriptor:
	0x07,        //bLength
	0x05,        //bDescriptorType
	0x84,        //bEndpointAddress  (IN endpoint 4)
	0x03,        //bmAttributes      (Transfer: Interrupt / Synch: None / Usage: Data)
	0x08,        //wMaxPacketSize    (1 x 8 bytes)
	0x00,
	0x10,        //bInterval         (16 frames)

	// Interface Descriptor:
	0x09,        //bLength
	0x04,        //bDescriptorType
	0x03,        //bInterfaceNumber
	0x00,        //bAlternateSetting
	0x02,        //bNumEndPoints
	0x0A,        //bInterfaceClass      (CDC Data)
	0x00,        //bInterfaceSubClass
	0x00,        //bInterfaceProtocol
	0x07,        //iInterface   "ST-Link VCP Data"

	// Endpoint Descriptor:
	0x07,        //bLength
	0x05,        //bDescriptorType
	0x05,        //bEndpointAddress  (OUT endpoint 5)
	0x02,        //bmAttributes      (Transfer: Bulk / Synch: None / Usage: Data)
	0x10,        //wMaxPacketSize    (16 bytes)
	0x00,
	0x00,        //bInterval

	// Endpoint Descriptor:
	0x07,        //bLength
	0x05,        //bDescriptorType
	0x85,        //bEndpointAddress  (IN endpoint 5)
	0x02,        //bmAttributes      (Transfer: Bulk / Synch: None / Usage: Data)
	0x10,        //wMaxPacketSize    (16 bytes)
	0x00,
	0x00,        //bInterval
};





__ALIGN_BEGIN uint8_t USBD_MSC_CDC_CfgFSDesc[98]  __ALIGN_END =
{
	// Дескриптор конфигурации
	0x09,        //bLength
	0x02,        //bDescriptorType
	98,        //wTotalLength   (128 bytes)
	0x00,
	0x03,        //bNumInterfaces
	0x01,        //bConfigurationValue
	0x00,        //iConfiguration
	0x80,        //bmAttributes   (Bus-powered Device)
	0xFA,        //bMaxPower      (500 mA)

	// Дескриптор интерфейса 2 Mass Storage
	0x09,        //bLength
	0x04,        //bDescriptorType
	MSC_interface,        //bInterfaceNumber
	0x00,        //bAlternateSetting
	0x02,        //bNumEndPoints
	0x08,        //bInterfaceClass      (Mass Storage Device Class)
	0x06,        //bInterfaceSubClass   (Transparent SCSI subclass)
	0x50,        //bInterfaceProtocol   (Bulk only transport)
	0x05,        //iInterface   "ST-Link mass storage"

	//Дескриптор 1 конечной точки интерфейса 2
	0x07,        //bLength
	0x05,        //bDescriptorType
	MSC_IN_EP,        //bEndpointAddress  (IN endpoint 1)
	0x02,        //bmAttributes      (Transfer: Bulk / Synch: None / Usage: Data)
	0x40,        //wMaxPacketSize    (64 bytes)
	0x00,
	0x00,        //bInterval

	//Дескриптор 2 конечной точки интерфейса 2
	0x07,        //bLength
	0x05,        //bDescriptorType
	MSC_OUT_EP,        //bEndpointAddress  (OUT endpoint 1)
	0x02,        //bmAttributes      (Transfer: Bulk / Synch: None / Usage: Data)
	0x40,        //wMaxPacketSize    (64 bytes)
	0x00,
	0x00,        //bInterval

	// Дескриптор ассоциации интерфейсов
	0x08,        //bLength
	0x0B,        //bDescriptorType
	CDC_cmd_interface,        //bFirstInterface
	0x02,        //bInterfaceCount
	0x02,        //bFunctionClass      (Communication Device Class)
	0x02,        //bFunctionSubClass   (Abstract Control Model - ACM)
	0x01,        //bFunctionProtocol   (ITU-T V.250)
	0x06,        //iFunction   "ST-Link VCP Ctrl"

	// Дескриптор инрефейса 3 CDC
	0x09,        //bLength
	0x04,        //bDescriptorType
	CDC_cmd_interface,        //bInterfaceNumber
	0x00,        //bAlternateSetting
	0x01,        //bNumEndPoints
	0x02,        //bInterfaceClass      (Communication Device Class)
	0x02,        //bInterfaceSubClass   (Abstract Control Model - ACM)
	0x01,        //bInterfaceProtocol   (ITU-T V.250)
	0x06,        //iInterface   "ST-Link VCP Ctrl"

	//CDC Header Functional Descriptor:
	0x05,        //bFunctionalLength
	0x24,        //bDescriptorType
	0x00,        //bDescriptorSubtype
	0x10,        //bcdCDC
	0x01,

	//CDC Call Management Functional Descriptor:
	0x05,        //bFunctionalLength
	0x24,        //bDescriptorType
	0x01,        //bDescriptorSubtype
	0x00,        //bmCapabilities
	CDC_interface,        //bDataInterface

	//CDC Abstract Control Management Functional Descriptor:
	0x04,        //bFunctionalLength
	0x24,        //bDescriptorType
	0x02,        //bDescriptorSubtype
	0x06,        //bmCapabilities

	//CDC Union Functional Descriptor:
	0x05,        //bFunctionalLength
	0x24,        //bDescriptorType
	0x06,        //bDescriptorSubtype
	CDC_cmd_interface,        //bControlInterface
	CDC_interface,        //bSubordinateInterface(0)

	//Endpoint Descriptor:
	0x07,        //bLength
	0x05,        //bDescriptorType
	CDC_CMD_EP,        //bEndpointAddress  (IN endpoint 4)
	0x03,        //bmAttributes      (Transfer: Interrupt / Synch: None / Usage: Data)
	0x08,        //wMaxPacketSize    (1 x 8 bytes)
	0x00,
	0x10,        //bInterval         (16 frames)

	// Interface Descriptor:
	0x09,        //bLength
	0x04,        //bDescriptorType
	CDC_interface,        //bInterfaceNumber
	0x00,        //bAlternateSetting
	0x02,        //bNumEndPoints
	0x0A,        //bInterfaceClass      (CDC Data)
	0x00,        //bInterfaceSubClass
	0x00,        //bInterfaceProtocol
	0x07,        //iInterface   "ST-Link VCP Data"

	// Endpoint Descriptor:
	0x07,        //bLength
	0x05,        //bDescriptorType
	CDC_OUT_EP,        //bEndpointAddress  (OUT endpoint 5)
	0x02,        //bmAttributes      (Transfer: Bulk / Synch: None / Usage: Data)
	0x40,        //wMaxPacketSize    (16 bytes)
	0x00,
	0x00,        //bInterval

	// Endpoint Descriptor:
	0x07,        //bLength
	0x05,        //bDescriptorType
	CDC_IN_EP,        //bEndpointAddress  (IN endpoint 5)
	0x02,        //bmAttributes      (Transfer: Bulk / Synch: None / Usage: Data)
	0x40,        //wMaxPacketSize    (16 bytes)
	0x00,
	0x00,        //bInterval
};


__ALIGN_BEGIN uint8_t USBD_MSC_CDC_OtherSpeedCfgDesc[USB_MSC_CDC_CONFIG_DESC_SIZ]   __ALIGN_END  =
{
	0x09,   /* bLength: Configuation Descriptor size */
	USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION,
	USB_MSC_CDC_CONFIG_DESC_SIZ,

	0x00,
	0x01,   /* bNumInterfaces: 1 interface */
	0x01,   /* bConfigurationValue: */
	0x04,   /* iConfiguration: */
	0xC0,   /* bmAttributes: */
	0x32,   /* MaxPower 100 mA */

	/********************  Mass Storage interface ********************/
	0x09,   /* bLength: Interface Descriptor size */
	0x04,   /* bDescriptorType: */
	0x00,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x02,   /* bNumEndpoints*/
	0x08,   /* bInterfaceClass: MSC Class */
	0x06,   /* bInterfaceSubClass : SCSI transparent command set*/
	0x50,   /* nInterfaceProtocol */
	0x05,          /* iInterface: */
	/********************  Mass Storage Endpoints ********************/
	0x07,   /*Endpoint descriptor length = 7*/
	0x05,   /*Endpoint descriptor type */
	MSC_IN_EP,   /*Endpoint address (IN, address 1) */
	0x02,   /*Bulk endpoint type */
	0x40,
	0x00,
	0x00,   /*Polling interval in milliseconds */

	0x07,   /*Endpoint descriptor length = 7 */
	0x05,   /*Endpoint descriptor type */
	MSC_OUT_EP,   /*Endpoint address (OUT, address 1) */
	0x02,   /*Bulk endpoint type */
	0x40,
	0x00,
	0x00     /*Polling interval in milliseconds*/
};



/* USB Standard Device Descriptor */
__ALIGN_BEGIN  uint8_t USBD_MSC_CDC_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC]  __ALIGN_END =
{
	USB_LEN_DEV_QUALIFIER_DESC,
	USB_DESC_TYPE_DEVICE_QUALIFIER,
	0x00,
	0x02,
	0x00,
	0x00,
	0x00,
	MSC_MAX_FS_PACKET,
	0x01, //0x01
	0x00,
};



uint8_t *USBD_MSC_CDC_GetDeviceQualifierDescriptor(uint16_t *length)
{
  *length = sizeof(USBD_MSC_CDC_DeviceQualifierDesc);
  return USBD_MSC_CDC_DeviceQualifierDesc;
}


uint8_t *USBD_MSC_CDC_GetOtherSpeedCfgDesc(uint16_t *length)
{
  *length = sizeof(USBD_MSC_CDC_OtherSpeedCfgDesc);
  return USBD_MSC_CDC_OtherSpeedCfgDesc;
}

uint8_t *USBD_MSC_CDC_GetHSCfgDesc(uint16_t *length)
{
  *length = sizeof(USBD_MSC_CDC_CfgHSDesc);
  return USBD_MSC_CDC_CfgHSDesc;
}


uint8_t *USBD_MSC_CDC_GetFSCfgDesc(uint16_t *length)
{
  *length = sizeof(USBD_MSC_CDC_CfgFSDesc);
  return USBD_MSC_CDC_CfgFSDesc;
}


// Инициализация композитного устройства
static uint8_t  USBD_MSC_CDC_Init (USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	/* MSC initialization */
	uint8_t ret = USBD_MSC_Init (pdev, cfgidx);
	if(ret != USBD_OK)
			return ret;

	/* CDC initialization */
	ret = USBD_CDC_Init (pdev, cfgidx);
	if(ret != USBD_OK)
			return ret;

	return USBD_OK;
}


// Деинициализация композитного устройства
static uint8_t  USBD_MSC_CDC_DeInit (USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
       	/* MSC De-initialization */
       	USBD_MSC_DeInit(pdev, cfgidx);

       	/* CDC De-initialization */
       	USBD_CDC_DeInit(pdev, cfgidx);

       	return USBD_OK;
}



// Setup для композитного устройства
static uint8_t  USBD_MSC_CDC_Setup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	// Route requests to MSC interface or its endpoints to MSC class implementaion
	if(((req->bmRequest & USB_REQ_RECIPIENT_MASK) == USB_REQ_RECIPIENT_INTERFACE && req->wIndex == MSC_OUT_EP) ||
		((req->bmRequest & USB_REQ_RECIPIENT_MASK) == USB_REQ_RECIPIENT_ENDPOINT && ((req->wIndex & 0x7F) == MSC_OUT_EP)))
	{
		return USBD_MSC_Setup(pdev, req);
	}

	return USBD_CDC_Setup(pdev, req);
}


// Data IN для композитного устройства
static uint8_t  USBD_MSC_CDC_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	if(epnum == MSC_OUT_EP)
		return USBD_MSC_DataIn(pdev, epnum);

	return USBD_CDC_DataIn(pdev, epnum);
}


// Data OUT для композитного устройства
static uint8_t  USBD_MSC_CDC_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	if(epnum == MSC_OUT_EP)
		return USBD_MSC_DataOut(pdev, epnum);

	return USBD_CDC_DataOut(pdev, epnum);
}


// Обработчик приема EP0
static uint8_t  USBD_MSC_CDC_EP0_RxReady (USBD_HandleTypeDef *pdev)
{
    	return USBD_CDC_EP0_RxReady(pdev);
}


