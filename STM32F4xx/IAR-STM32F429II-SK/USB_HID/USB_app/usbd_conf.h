/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : usbd_conf.h
 *    Description : USB Device Configuration file for USB On-The-Go Host and
 *                  Device Library by ST.
 *
 *    History :
 *    1. Date        : October 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

#include "usb_conf.h"


#define USBD_CFG_MAX_NUM           1
#define USBD_ITF_MAX_NUM           1

#define USB_MAX_STR_DESC_SIZ       64

#define USBD_SELF_POWERED

#define USBD_DYNAMIC_DESCRIPTOR_CHANGE_ENABLED

/* HID Layer parameters */

#define HID_IN_EP                    0x81
#define HID_OUT_EP                   0x01

#define HID_IN_PACKET                4
#define HID_OUT_PACKET               4


#endif //__USBD_CONF__H__
