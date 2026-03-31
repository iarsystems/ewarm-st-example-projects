/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : usbd_desc.h
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
#ifndef __USB_DESC_H
#define __USB_DESC_H

#include "usbd_def.h"

#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05
#define USB_DEVICE_DESC_SIZE                      18
#define USB_LANGID_STRING_SIZE                     4

extern  uint8_t USBD_DeviceDescriptor[USB_DEVICE_DESC_SIZE];
extern  uint8_t USBD_StrDesc[USB_MAX_STR_DESC_SIZ]; 
extern  uint8_t USBD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC];
extern  uint8_t USBD_LangIDDesc[USB_LANGID_STRING_SIZE];
extern  USBD_DEVICE HID_Joystick_descriptor; 

uint8_t * USBD_USR_DeviceDescriptor( uint8_t speed , uint16_t *length);
uint8_t * USBD_USR_LangIDStrDescriptor( uint8_t speed , uint16_t *length);
uint8_t * USBD_USR_ManufacturerStrDescriptor ( uint8_t speed , uint16_t *length);
uint8_t * USBD_USR_ProductStrDescriptor( uint8_t speed , uint16_t *length);
uint8_t * USBD_USR_SNStrDescriptor( uint8_t speed , uint16_t *length);
uint8_t * USBD_USR_ConfigStrDescriptor( uint8_t speed , uint16_t *length);
uint8_t * USBD_USR_InterfaceStrDescriptor( uint8_t speed , uint16_t *length);

#endif /* __USBD_DESC_H */
