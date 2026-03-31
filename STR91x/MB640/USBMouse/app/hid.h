/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name      : hid.h
 *    Description    : Common HID device definitions
 *
 *    History :
 *    1. Date        : December 19, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "arm_comm.h"

#ifndef __HID_H
#define __HID_H

typedef enum _HidRequest_t
{
  HID_GET_REPORT = 1, HID_GET_IDLE, HID_GET_PROTOCOL,
  HID_SET_REPORT = 9, HID_SET_IDLE, HID_SET_PROTOCOL,
} HidRequest_t;

typedef enum _HidSubclassCodes_t
{
  NoSubclass = 0, BootInterfaceSubclass
} HidSubclassCodes_t;

typedef enum _HidProtocolCodes_t
{
  None = 0, Keyboard, Mouse
} HidProtocolCodes_t;

typedef enum _HidClassDescriptorTypes_t
{
  Hid = 0x21, HidReport, HidPhysical,
} HidClassDescriptorTypes_t;

typedef enum _MouseProtocol_t
{
  BootProtocol = 0, ReportProtocol
} MouseProtocol_t;

#pragma pack(1)

typedef struct _UsbHidDescriptor_t
{
  Int8U       bLength;
  Int8U       bDescriptorType;
  Int8U       bcdHID[2];
  Int8U       bCountryCode;
  Int8U       bNumDescriptors;
  Int8U       bDescriptorHidType;
  Int16U      wDescriptorLength;
} UsbHidDescriptor_t, * pUsbHidDescriptor_t;
#pragma pack()

#endif //__HID_H
