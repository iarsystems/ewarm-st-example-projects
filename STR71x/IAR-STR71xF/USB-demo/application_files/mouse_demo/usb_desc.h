/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : MCD Application Team
* Date First Issued  : 27/10/2003
* Description        : Descriptor for USB Mouse
*
********************************************************************************/


#define	MOUSE_SIZ_DEVICE_DESC	18
#define	MOUSE_SIZ_CONFIG_DESC	34
#define	MOUSE_SIZ_STRING_DESC	152

#define	MOUSE_SIZ_REPORT_DESC	0x4A
#define MOUSE_OFF_HID_DESC	18
#define MOUSE_SIZ_HID_DESC	9

extern const BYTE MOUSE_DeviceDescriptor[MOUSE_SIZ_DEVICE_DESC];
extern const BYTE MOUSE_ConfigDescriptor[MOUSE_SIZ_CONFIG_DESC];
extern const BYTE MOUSE_StringDescriptor[MOUSE_SIZ_STRING_DESC];

extern const BYTE MOUSE_ReportDescriptor[MOUSE_SIZ_REPORT_DESC];


