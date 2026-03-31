/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : drv_hd44780_l.h
 *    Description : STR731 HD44780 low level I/O function driver
 *
 *    History :
 *    1. Date        : June 3, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#ifndef  __DRV_HD44780_L_H
#define  __DRV_HD44780_L_H

#define LCD_LIGHT                 GPIO_PIN_0
#define LCD_LIGHT_PORT            GPIO6

#define LCD_RS                    GPIO_PIN_12
#define LCD_RS_PORT               GPIO1

#define LCD_E                     GPIO_PIN_13
#define LCD_E_PORT                GPIO1

#define LCD_RW                    GPIO_PIN_7
#define LCD_RW_PORT               GPIO1

#define LCD_DATA                  0xFUL
#define LCD_DATA_PORT             GPIO1
#define LCD_DATA_SHIFT            8

#define LCD_LIGHT_ON()            GPIO_BitWrite(LCD_LIGHT_PORT,LCD_LIGHT,Bit_SET)
#define LCD_LIGHT_OFF()           GPIO_BitWrite(LCD_LIGHT_PORT,LCD_LIGHT,Bit_RESET)

/* Delay macro */
#define HD44780_BUS_DLY()         for(volatile int dly = 30;dly;--dly)

void HD44780_IO_Init (void);
void HD44780SetRS (Boolean Data);
void HD44780SetE (Boolean Data);
void HD44780WrIO (Int32U);
#if HD4780_WR > 0
void HD44780SetRW (Boolean Data);
Int8U HD44780RdIO (void);
#endif

#endif  /* __DRV_HD44780_L_H */
