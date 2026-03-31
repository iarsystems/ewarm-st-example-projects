/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : usb_des.h
 *    Description : Define standard USB descriptors module
 *
 *    History :
 *    1. Date        : August 10, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#ifndef __USB_DESC_H
#define __USB_DESC_H

#define UsbConfigPower_mA(mA)   ((mA)/2)
#define UsbEpOut(addr)          ((addr) | 0x00)
#define UsbEpIn(addr)           ((addr) | 0x80)

#define UsbDescLengthOffset     0
#define UsbDescDescriptorOffset 1

typedef enum _UsbDescriptorType_t
{
  UsbDescriptorDevice = 1, UsbDescriptorConfiguration, UsbDescriptorString,
  UsbDescriptorInterface, UsbDescriptorEp, UsbDescriptorDeviceQualifier,
  UsbDescriptorOtherSeedConfiguration, UsbDescriptorInterfacePower
} UsbDescriptorType_t;

typedef enum _UsbStringsIndex_t
{
  iLanguagesStr = 0, iManufacturerStr, iProductStr, iSerialNumberStr,
  iConfigurationStr,
  iInterface,
} UsbStringsIndex_t;

typedef enum _UsbDeviceClassses_t
{
/* USB Device Classes */
  UsbDeviceClassReserved = 0, UsbDeviceClassAudio,
  UsbDeviceClassCommunications,UsbDeviceClassHumanInterface,
  UsbDeviceClassMonitor,UsbDeviceClassPhysicalInterface,
  UsbDeviceClassPower,UsbDeviceClassPrinter,
  UsbDeviceClassStorage,UsbDeviceClassHub,
  UsbDeviceClassVendorSpecific = 0xFF
} UsbDeviceClassses_t;

typedef union _CnfgArtr_t
{
  u8 Data;
  struct
  {
    u8 Reserved      : 5;
    u8 RemoteWakeup  : 1;
    u8 SelfPowered   : 1;
    u8 MustBeOne     : 1;
  };
} CnfgArtr_t, * pCnfgArtr_t;

#define USBPowerMask          0x40
#define USBRemoteWakeUpMask   0x20

typedef enum _UsbConfigurationAttr_t
{
  UsbRemoteWakeup = 0x20, UsbSelfPowered = 0x40,  UsbConfigurationCommmonAttr = 0x80
} UsbConfigurationAttr_t;

typedef enum _UsbEpTransferType_t
{
  UsbEpTransferControl = 0, UsbEpTransferIsochronous,
  UsbEpTransferBulk,UsbEpTransferInterrupt
}UsbEpTransferType_t;

typedef enum _UsbEpSynchronizationType_t
{
  UsbEpSynchNoSynchronization = 0, UsbEpSynchAsynchronous = 1<<2,
  UsbEpSynchAdaptive = 2<<2, UsbEpSynchSynchronous = 3<<2
}UsbEpSynchronizationType_t;

typedef enum _UsbEpUsageType_t
{
  UsbEpUsageData = 0, UsbEpUsageFeedback = 1<<4,
  UsbEpUsageImplicit = 2<<4, UsbEpUsageReserved = 3<<4
}UsbEpUsageType_t;

typedef union _EpAttr_t
{
  u8 Data;
  struct
  {
    u8 TransferType        : 2;
    u8 SynchronizationType : 2;
    u8 UsageType           : 2;
  };
} EpAttr_t, * pEpAttr_t;

#pragma pack(1)
typedef struct _UsbStandardDeviceDescriptorStr_t
{
  u8       bLength;
  u8       bDescriptorType;
  u16      bcdUSB;
  u8       bDeviceClass;
  u8       bDeviceSubClass;
  u8       bDeviceProtocol;
  u8       bMaxPacketSize0;
  u16      idVendor;
  u16      idProduct;
  u16      bcdDevice;
  u8       iManufacturer;
  u8       iProduct;
  u8       iSerialNumber;
  u8       bNumConfigurations;
} UsbStandardDeviceDescriptorStr_t, * pUsbStandardDeviceDescriptorStr_t;

typedef struct _UsbStandardConfigurationDescriptor_t
{
  u8       bLength;
  u8       bDescriptorType;
  u16      wTotalLength;
  u8       bNumInterfaces;
  u8       bConfigurationValue;
  u8       iConfiguration;
  CnfgArtr_t  bmAttributes;
  u8       bMaxPower;
} UsbStandardConfigurationDescriptor_t, * pUsbStandardConfigurationDescriptor_t;

typedef struct _UsbStandardInterfaceDescriptor_t
{
  u8       bLength;
  u8       bDescriptorType;
  u8       bInterfaceNumber;
  u8       bAlternateSetting;
  u8       bNumEndpoints;
  u8       bInterfaceClass;
  u8       bInterfaceSubClass;
  u8       bInterfaceProtocol;
  u8       iInterface;
} UsbStandardInterfaceDescriptor_t, * pUsbStandardInterfaceDescriptor_t;

typedef struct _UsbStandardEpDescriptor_t
{
  u8       bLength;
  u8       bDescriptorType;
  u8       bEndpointAddress;
  EpAttr_t    bmAttributes;
  u16      wMaxPacketSize;
  u8       bInterval;
} UsbStandardEpDescriptor_t, * pUsbStandardEpDescriptor_t;

#if USB_HIGH_SPEED > 0
typedef struct _UsbStandardDeviceQualifierDescriptor_t
{
  u8       bLength;
  u8       bDescriptorType;
  u16      bcdUSB;
  u8       bDeviceClass;
  u8       bDeviceSubClass;
  u8       bDeviceProtocol;
  u8       bMaxPacketSize0;
  u8       bNumConfigurations;
  u8       bReserve;
} UsbStandardDeviceQualifierDescriptor_t, *pUsbStandardDeviceQualifierDescriptor_t;
#endif // USB_HIGH_SPEED > 0
#pragma pack()

#endif //__USB_DESC_H
