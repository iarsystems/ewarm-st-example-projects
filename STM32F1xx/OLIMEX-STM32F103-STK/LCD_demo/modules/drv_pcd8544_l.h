/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_pcd8544_l.h
 *    Description : STM32-103STK PCD8544 low level driver include file
 *
 *    History :
 *    1. Date        : December 28, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 4269 $
 **************************************************************************/
#include "includes.h"

#ifndef  __DRV_PCD8544_L_H
#define  __DRV_PCD8544_L_H

#define LCD_RST                   GPIO_Pin_7
#define LCD_RST_PORT              GPIOC

#define LCD_CS                    GPIO_Pin_10
#define LCD_CS_PORT               GPIOC

#define LCD_DC                    GPIO_Pin_2
#define LCD_DC_PORT               GPIOB

#define PCD8544_RST_L()           GPIO_WriteBit(LCD_RST_PORT,LCD_RST,Bit_RESET)
#define PCD8544_RST_H()           GPIO_WriteBit(LCD_RST_PORT,LCD_RST,Bit_SET  )

#define PCD8544_CS_L()            GPIO_WriteBit(LCD_CS_PORT ,LCD_CS, Bit_RESET)
#define PCD8544_CS_H()            GPIO_WriteBit(LCD_CS_PORT ,LCD_CS, Bit_SET  )

#define PCD8544_DATA()            GPIO_WriteBit(LCD_DC_PORT ,LCD_DC ,Bit_SET  )
#define PCD8544_CMD()             GPIO_WriteBit(LCD_DC_PORT ,LCD_DC ,Bit_RESET)

void PCD8544_Init(void);
void PCD8544_WrData(Int8U Data);
void PCD8544_WrCmd(Int8U Command);

#endif  /* __DRV_PCD8544_L_H */
