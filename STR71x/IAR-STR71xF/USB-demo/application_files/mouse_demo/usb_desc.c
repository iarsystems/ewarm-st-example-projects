 /******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Date First Issued  : 27/10/2003
* Description        : Descriptors for USB Mouse
*
********************************************************************************/

#include "USB_lib.h"
#include "USB_desc.h"

const BYTE MOUSE_ReportDescriptor[MOUSE_SIZ_REPORT_DESC] = {
	0x05,	/* Usage Page(Generic Desktop) */
	0x01,
	0x09,	/* Usage(Mouse) */
	0x02,
	0xA1,	/* Collection(Logical) */
	0x01,
	0x09,	/* Usage(Pointer) */
	0x01,
/* 8 */
	0xA1,	/* Collection(Linked) */
	0x00,
	0x05,	/* Usage Page(Buttons) */
	0x09,
	0x19,	/* Usage Minimum(1) */
	0x01,
	0x29,	/* Usage Maximum(3) */
	0x03,
/* 16	*/
	0x15,	/* Logical Minimum(0) */
	0x00,
	0x25,	/* Logical Maximum(1) */
	0x01,
	0x95,	/* Report Count(3) */
	0x03,
	0x75,	/* Report Size(1) */
	0x01,
/* 24	*/
	0x81,	/* Input(Variable) */
	0x02,
	0x95,	/* Report Count(1) */
	0x01,
	0x75,	/* Report Size(5) */
	0x05,
	0x81,	/* Input(Constant,Array) */
	0x01,
/* 32 */
	0x05,	/* Usage Page(Generic Desktop) */
	0x01,
	0x09,	/* Usage(X axis) */
	0x30,
	0x09,	/* Usage(Y axis) */
	0x31,
	0x09,	/* Usage(Wheel) */
	0x38,
/* 40	*/
	0x15,   /* Logical Minimum(-127) */
	0x81,
	0x25,	/* Logical Maximum(127) */
	0x7F,
	0x75,	/* Report Size(8) */
	0x08,
	0x95,	/* Report Count(3) */
	0x03,
/* 48	*/
	0x81,	/* Input(Variable, Relative) */
	0x06,
	0xC0,	/* End Collection */
	0x09,
	0x3c,
	0x05,
	0xff,
	0x09,
/* 56	*/
	0x01,
	0x15,
	0x00,
	0x25,
	0x01,
	0x75,
	0x01,
	0x95,
/* 64	*/
	0x02,
	0xb1,
	0x22,
	0x75,
	0x06,
	0x95,
	0x01,
	0xb1,
/* 72	*/
	0x01,
	0xc0
}; /* MOUSE_ReportDescriptor */

const BYTE MOUSE_DeviceDescriptor[MOUSE_SIZ_DEVICE_DESC] = {	/* ST mouse */
	0x12,	/* bLength */
	0x01,	/* bDescriptorType */
	0x00,	/* bcdUSB */
	0x01,
	0x00,	/* bDeviceClass */
	0x00,	/* bDeviceSubClass */
	0x00,	/* bDeviceProtocol */
	0x08,	/* bMaxPacketSize0 */
	0x83,	/* idVendor	(1155) */
	0x04,
	0x01,	/* idProduct */
	0x00,
	0x04,	/* bcdDevice rel. 1.04 */
	0x01,
	4,		/* Index of string descriptor */
			/*		describing manufacturer */
	42,		/* Index of string descriptor */
			/*		describing product */
	0, /*98,*/		/* Index of string descriptor */
			/*		describing the device's */
			/*		serial number */
	0x01	/* bNumConfigurations */
}; /* MOUSE_DeviceDescriptor */

const BYTE MOUSE_ConfigDescriptor[MOUSE_SIZ_CONFIG_DESC] = {
	0x09,	/* bLength: Configuation Descriptor size */
	0x02,	/* bDescriptorType: Configuration */
	MOUSE_SIZ_CONFIG_DESC,
			/* wTotalLength: Bytes returned */
	0x00,
	0x01,	/* bNumInterfaces: 1 interface */
	0x01,	/* bConfigurationValue: */
			/*	Configuration value */
	0x00,	/* iConfiguration: */
			/*	Index of string descriptor */
			/*	describing the configuration */
	0xA0,	/* bmAttributes: */
			/*	bus powered, remote-wakeup */
	0x32,	/* MaxPower 100 mA: this current is used for detecting Vbus */

/******************** Descriptor of mouse interface ********************/
/* 09 */
	0x09,	/* bLength: Interface Descriptor size */
	0x04,	/* bDescriptorType: */
			/*	Interface descriptor type */
	0x00,	/* bInterfaceNumber: Number of Interface */
	0x00,	/* bAlternateSetting: Alternate setting */
	0x01,	/* bNumEndpoints*/
	0x03,	/* bInterfaceClass: HID */
	0x01,	/* bInterfaceSubClass : 1=BOOT, 0=no boot*/
	0x02,	/* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
	0,	    /* iInterface: */
			/*	Index of string descriptor */
/******************** Descriptor of Mouse HID ********************/
/* 18 */
	0x09,	/* bLength: HID Descriptor size */
	0x21,	/* bDescriptorType: HID */
	0x00,	/* bcdHID: HID Class Spec release number */
	0x01,
	0x00,	/* bCountryCode: Hardware target country */
	0x01,	/* bNumDescriptors: */
			/*	Number of HID class descriptors */
			/*	to follow */
	0x22,	/* bDescriptorType */
	MOUSE_SIZ_REPORT_DESC,
			/* wItemLength: Total length of Report descriptor */
	0x00,
/******************** Descriptor of mouse endpoint ********************/
/* 27 */
	0x07,	/* bLength: Endpoint Descriptor size */
	0x05,	/* bDescriptorType: */
			/*	Endpoint descriptor type */
	0x81,	/* bEndpointAddress: */
			/*	Endpoint Address (IN) */
	0x03,	/* bmAttributes: Interrupt endpoint */
	0x04,	/* wMaxPacketSize: 4 Byte max  */
	0x00,
	0x20,	/* bInterval: Polling Interval (32 ms) */
/* 34 */
}; /* MOUSE_ConfigDescriptor */

const BYTE MOUSE_StringDescriptor[MOUSE_SIZ_STRING_DESC] = {
	0x04,
	0x03,
	0x09,
	0x04,		/* LangID = 0x0409: U.S. English */
/* 4 */
	38,			/* Size of manufaturer string */
	0x03,		/* bDescriptorType = String descriptor */
	/* Manufacturer: "STMicroelectronics" */
	'S',0, 'T',0, 'M',0, 'i',0, 'c',0, 'r',0, 'o',0, 'e',0,
	'l',0, 'e',0, 'c',0, 't',0, 'r',0, 'o',0, 'n',0, 'i',0,
	'c',0, 's',0,
/* 42 */
	56,
	0x03,
	/* Product name: "ARM7   : USB Mouse Tutorial" */
	'A',0, 'R',0, 'M',0, '7',0, ' ',0, ' ',0, ' ',0, ':',0,
	' ',0, 'U',0, 'S',0, 'B',0, ' ',0, 'M',0, 'o',0, 'u',0,
	's',0, 'e',0, ' ',0, 'T',0, 'u',0, 't',0, 'o',0, 'r',0,
	'i',0, 'a',0, 'l',0,
/* 98 */
	28,
	0x03,
	/* Serial number: "20072000-0001" */
	'2',0, '0',0, '0',0, '7',0, '2',0, '0',0, '0',0, '0',0,
	'-',0, '0',0, '0',0, '0',0, '1',0,
/* 126 */
	26,
	0x03,
	/* Interface 0: "ST USB Mouse" */
	'S',0, 'T',0, ' ',0, 'U',0, 'S',0, 'B',0, ' ',0, 'M',0,
	'o',0, 'u',0, 's',0, 'e',0,
/* 152 */
}; /* MOUSE_StringDescriptor */

