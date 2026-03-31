/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : nrf_drv_low.h
 *    Description : nRF24L01 low level driver include file
 *
 *    History :
 *    1. Date        : September 2007
 *       Author      : Stanimir Bonev
 *       Description : Initial Revision
 *    2. Date        : 20.12.2007
 *       Author      : Stoyan Choynev
 *       Description : Ported for Olimex STM32F103-STK
 *    $Revision: 39 $
 **************************************************************************/

/** include files **/
#include "includes.h"

/** definitions **/

#ifndef NRF_DRV_LOW_H
#define NRF_DRV_LOW_H

#ifdef RNF_DRV_LOW_GLOBAL
#define RNF_DRV_LOW_EXTER
#else
#define RNF_DRV_LOW_EXTER  extern
#endif

#ifndef BYTE
#define BYTE    Int8U
#endif

#ifndef WORD
#define WORD    Int16U
#endif

#ifndef DWORD
#define DWORD   Int32U
#endif

#ifndef LONG
#define LONG    Int32U
#endif

#ifndef BOOL
#define BOOL    Boolean
#endif

#define NRF24L_DISABLE_DBG_MESSAGES
//#define NRF24L_DISABLE_ERROR_MESSAGES

#define NRF_IRQ_PreemptioinPriority 2
#define NRF_IRQ_SubPrioriry         0

#define NRF_TX_PULSE_DLY      100  // >10us

#define RNF_MAX_PAYLOAD				32

#define RF24L_CE_L()      GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define RF24L_CE_H()	    GPIO_SetBits(GPIOC, GPIO_Pin_8)

#define RF24L_GET_CE()    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)

#define RF24L_CS_L()      GPIO_ResetBits(GPIOA, GPIO_Pin_4 );
#define RF24L_CS_H()      GPIO_SetBits(GPIOA, GPIO_Pin_4 );

/** default settings **/

/** public data **/

/** public functions **/
BYTE SpiNrfSendData (BYTE Byte);
void nrf24l_init(void);




#endif // NRF_DRV_LOW_H
