/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : str91x_usb_cfg.h
 *    Description : USB definitions
 *
 *    History :
 *    1. Data        : February 10, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include "includes.h"

#ifndef __STR91X_USB_CFG_H
#define __STR91X_USB_CFG_H

#define USB_EXT_CLK               0   // External 48MHz clock

#define USB_CONN_CTRL             1   // Enable software control of USB connection (wake-up resistor)

/* USB Events */
#define USB_SOF_EVENT             1
#define USB_ERROR_EVENT           0   // for debbug
#define USB_HIGH_PRIORITY_EVENT   1   // ISO and Double buffered bulk
#define USB_WKUP_EVENT            0
#define USB_DOVR_EVENT            0   // for speed up retransfer

/* Endpoint definitions */
#define Ep0MaxSize        				8

#define SpkEp         				    UsbEp1Out
#define SpkEpSlot      				    EP_SLOT1
#define SpkEpMaxSize   				   (SampRerFrame * SubFrameSize)
#define SpkDDInd                  0

#define MicEp         				    UsbEp2In
#define MicEpSlot      				    EP_SLOT2
#define MicEpMaxSize   				   (SampRerFrame * SubFrameSize * 2)
#define MicDDInd                  1

#define MaxIndOfRealizeEp         2

/* USB DMA defenitions */
#define USB_DMA                   1
#define USB_DMA_RX_MAX            1
#define USB_DMA_TX_MAX            1

/* Othe defenitions */

#endif //__STR91X_USB_CFG_H
