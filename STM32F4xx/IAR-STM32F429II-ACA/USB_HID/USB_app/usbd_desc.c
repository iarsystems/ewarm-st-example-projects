/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : usbd_desc.c
 *    Description : USB Device Descriptor for USB On-The-Go Host and
 *                  Device Library by ST.
 *
 *    History :
 *    1. Date        : October 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_req.h"
#include "usbd_conf.h"
#include "usb_regs.h"

#define USBD_VID                      0xFFFF
#define USBD_PID                      0x0065

#define USBD_LANGID_STRING            0x409
#define USBD_MANUFACTURER_STRING      "IAR Systems"

#define USBD_PRODUCT_STRING_HS        "HID Mouse High-Speed"
#define USBD_SN_STRING_HS             "000000000001"

#define USBD_PRODUCT_STRING_FS        "HID Mouse Full-Speed"
#define USBD_SN_STRING_FS             "000000000002"

#define USBD_CONFIGURATION_STRING_HS  "HID Config"
#define USBD_INTERFACE_STRING_HS      "HID Interface"

#define USBD_CONFIGURATION_STRING_FS  "HID Config"
#define USBD_INTERFACE_STRING_FS      "HID Interface"


USBD_DEVICE HID_Mouse_descriptor =
{
  USBD_USR_DeviceDescriptor,
  USBD_USR_LangIDStrDescriptor, 
  USBD_USR_ManufacturerStrDescriptor,
  USBD_USR_ProductStrDescriptor,
  USBD_USR_SNStrDescriptor,
  USBD_USR_ConfigStrDescriptor,
  USBD_USR_InterfaceStrDescriptor
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#pragma data_alignment=4   
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB Standard Device Descriptor */
uint8_t USBD_DeviceDescriptor[USB_DEVICE_DESC_SIZE] =
{
  0x12,                       /*bLength */
  USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
  0x00,                       /*bcdUSB */
  0x02,
  0x00,                       /*bDeviceClass*/
  0x00,                       /*bDeviceSubClass*/
  0x00,                       /*bDeviceProtocol*/
  USB_OTG_MAX_EP0_SIZE,       /*bMaxPacketSize*/
  LOBYTE(USBD_VID),           /*idVendor*/
  HIBYTE(USBD_VID),           /*idVendor*/
  LOBYTE(USBD_PID),           /*idVendor*/
  HIBYTE(USBD_PID),           /*idVendor*/
  0x00,                       /*bcdDevice rel. 2.00*/
  0x02,
  USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
  USBD_IDX_PRODUCT_STR,       /*Index of product string*/
  USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
  USBD_CFG_MAX_NUM            /*bNumConfigurations*/
}; /* USB_DeviceDescriptor */

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#pragma data_alignment=4   
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB Standard Device Descriptor */
uint8_t USBD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED 
#pragma data_alignment=4   
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB Standard Device Descriptor */
uint8_t USBD_LangIDDesc[USB_LANGID_STRING_SIZE] =
{
  USB_LANGID_STRING_SIZE,         
  USB_DESC_TYPE_STRING,       
  LOBYTE(USBD_LANGID_STRING),
  HIBYTE(USBD_LANGID_STRING), 
};

/*************************************************************************
 * Function Name: USBD_USR_DeviceDescriptor
 * Parameters: uint8_t speed - device speed
 *             uint16_t * length - pointer to decriptor length
 *
 * Return: * uint8_t - pointer to descriptor buffer
 *
 * Description: Returns the device descriptor
 *
 *************************************************************************/
uint8_t * USBD_USR_DeviceDescriptor( uint8_t speed , uint16_t *length)
{
  *length = sizeof(USBD_DeviceDescriptor);
  return USBD_DeviceDescriptor;
}

/*************************************************************************
 * Function Name: USBD_USR_LangIDStrDescriptor
 * Parameters: uint8_t speed - device speed
 *             uint16_t * length - pointer to decriptor length
 *
 * Return: * uint8_t - pointer to descriptor buffer
 *
 * Description: Returns the device descriptor
 *
 *************************************************************************/
uint8_t *  USBD_USR_LangIDStrDescriptor( uint8_t speed , uint16_t *length)
{
  *length =  sizeof(USBD_LangIDDesc);  
  return USBD_LangIDDesc;
}

/*************************************************************************
 * Function Name: USBD_USR_ProductStrDescriptor
 * Parameters: uint8_t speed - device speed
 *             uint16_t * length - pointer to decriptor length
 *
 * Return: * uint8_t - pointer to descriptor buffer
 *
 * Description: Returns the device descriptor
 *
 *************************************************************************/
uint8_t * USBD_USR_ProductStrDescriptor( uint8_t speed , uint16_t *length)
{
  if(0 == speed)
  {   
    USBD_GetString (USBD_PRODUCT_STRING_HS, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString (USBD_PRODUCT_STRING_FS, USBD_StrDesc, length);    
  }
  return USBD_StrDesc;
}

/*************************************************************************
 * Function Name: USBD_USR_ManufacturerStrDescriptor
 * Parameters: uint8_t speed - device speed
 *             uint16_t * length - pointer to decriptor length
 *
 * Return: * uint8_t - pointer to descriptor buffer
 *
 * Description: Returns the device descriptor
 *
 *************************************************************************/
uint8_t * USBD_USR_ManufacturerStrDescriptor( uint8_t speed , uint16_t *length)
{
  USBD_GetString (USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

/*************************************************************************
 * Function Name: USBD_USR_SNStrDescriptor
 * Parameters: uint8_t speed - device speed
 *             uint16_t * length - pointer to decriptor length
 *
 * Return: * uint8_t - pointer to descriptor buffer
 *
 * Description: Returns the device descriptor
 *
 *************************************************************************/
uint8_t * USBD_USR_SNStrDescriptor( uint8_t speed , uint16_t *length)
{
  if(USB_OTG_SPEED_HIGH == speed)
  {    
    USBD_GetString (USBD_SN_STRING_HS, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString (USBD_SN_STRING_FS, USBD_StrDesc, length);    
  }
  return USBD_StrDesc;
}

/*************************************************************************
 * Function Name: USBD_USR_ConfigStrDescriptor
 * Parameters: uint8_t speed - device speed
 *             uint16_t * length - pointer to decriptor length
 *
 * Return: * uint8_t - pointer to descriptor buffer
 *
 * Description: Returns the device descriptor
 *
 *************************************************************************/
uint8_t * USBD_USR_ConfigStrDescriptor( uint8_t speed , uint16_t *length)
{
  if(USB_OTG_SPEED_HIGH == speed)
  {  
    USBD_GetString (USBD_CONFIGURATION_STRING_HS, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString (USBD_CONFIGURATION_STRING_FS, USBD_StrDesc, length); 
  }
  return USBD_StrDesc;  
}


/*************************************************************************
 * Function Name: USBD_USR_InterfaceStrDescriptor
 * Parameters: uint8_t speed - device speed
 *             uint16_t * length - pointer to decriptor length
 *
 * Return: * uint8_t - pointer to descriptor buffer
 *
 * Description: Returns the device descriptor
 *
 *************************************************************************/
uint8_t * USBD_USR_InterfaceStrDescriptor( uint8_t speed , uint16_t *length)
{
  if(0 == speed)
  {
    USBD_GetString (USBD_INTERFACE_STRING_HS, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString (USBD_INTERFACE_STRING_FS, USBD_StrDesc, length);
  }
  return USBD_StrDesc;  
}
