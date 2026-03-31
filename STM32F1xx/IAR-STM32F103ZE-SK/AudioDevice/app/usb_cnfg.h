/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : usb_cnfg.h
 *    Description : USB config file
 *
 *    History :
 *    1. Date        : July 30, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include <includes.h>

#ifndef __USB_CNFG_H
#define __USB_CNFG_H

/* The Board */
#define IAR_STM32F103ZE_SK

/* USB High Speed support*/
#define USB_HIGH_SPEED                  0

/* USB interrupt priority */
#define USB_INTR_HIGH_PRIORITY          2
#define USB_INTR_LOW_PRIORITY           3

/* USB Events */
#define USB_SOF_EVENT                   1
#define USB_ERROR_EVENT                 0   // for debug
#define USB_HIGH_PRIORITY_EVENT         1   // ISO and Double buffered bulk
#define USB_PMAOVR_EVENT                0   // for debug

/* USB Clock settings */
#define USB_DIVIDER                     RCC_USBCLKSource_PLLCLK_1Div5 // when PLL clk 72MHz

/* Device power atrb  */
#define USB_SELF_POWERED                0
#define USB_REMOTE_WAKEUP               0

/* Max Interfaces number*/
#define USB_MAX_INTERFACE               3

/* Endpoint definitions */
#define MaxIndOfRealizeEp               ENP1_OUT   // be careful this is very important const
#define Ep0MaxSize                      8

#define SpkEp         				          ENP1_OUT
#define SpkEpMaxSize   				          (SampRerFrame * SubFrameSize)
#define SpkDDInd                        0

/* Class defenitions*/
#define USB_INTR_AUDIO_SAMP_PRIORITY    1

#define USB_CTRL_INTERFACE              0
#define USB_SPK_INTERFACE               1

#define SubFrameSize                    2
#define SampRerFrame                    48
#define SampFreq                        (SampRerFrame KHZ)


/* Other defenitions */

#endif //__USB_CNFG_H
