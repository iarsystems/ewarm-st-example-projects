/**
  ******************************************************************************
  * @file    stm32l152_eval_glass_lcd.h
  * @author  MCD Application Team
  * @version V4.4.0RC1
  * @date    07/02/2010
  * @brief   Header file for stm32l152_eval_glass_lcd.c module.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L152_EVAL_GLASS_LCD_H
#define __STM32L152_EVAL_GLASS_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

typedef enum
{
  POINT_OFF = 0,
  POINT_ON = 1
}Point_Typedef;

typedef enum
{
  SEMICOL_OFF = 0,
  SEMICOL_ON = 1
}Semicol_Typedef;

#define SPEC_SEG_PLYS           (1UL<<0)
#define SPEC_SEG_MINUS          (1UL<<1)
#define SPEC_SEG_ARROW_RIGHT    (1UL<<2)
#define SPEC_SEG_ARROW_LEFT     (1UL<<3)
#define SPEC_SEG_ARROW_UP       (1UL<<4)
#define SPEC_SEG_ARROW_DOWN     (1UL<<5)
#define SPEC_SEG_BATT           (1UL<<6)
#define SPEC_SEG_BATT_0         (1UL<<7)
#define SPEC_SEG_BATT_1         (1UL<<8)
#define SPEC_SEG_BATT_2         (1UL<<9)
#define SPEC_SEG_BARR_R         (1UL<<10)
#define SPEC_SEG_BARR_L         (1UL<<11)
#define SPEC_SEG_BARR_0         (1UL<<12)
#define SPEC_SEG_BARR_1         (1UL<<13)
#define SPEC_SEG_BARR_2         (1UL<<14)
#define SPEC_SEG_BARR_3         (1UL<<15)
#define SPEC_SEG_BARR_4         (1UL<<16)
#define SPEC_SEG_BARR_5         (1UL<<17)
#define SPEC_SEG_BARR_6         (1UL<<18)
#define SPEC_SEG_BARR_7         (1UL<<19)
#define SPEC_SEG_BARR_8         (1UL<<20)
#define SPEC_SEG_BARR_9         (1UL<<21)
#define SPEC_SEG_MICRO          (1UL<<22)
#define SPEC_SEG_MIL            (1UL<<23)
#define SPEC_SEG_H              (1UL<<24)
#define SPEC_SEG_F              (1UL<<25)
#define SPEC_SEG_OLIMEX         (1UL<<26)

void LCD_GLASS_Init(void);
void LCD_GLASS_DisplayChar(uint8_t* ch, Point_Typedef point, Semicol_Typedef semicol,uint8_t position);
void LCD_GLASS_DisplayString(uint8_t* ptr);
void LCD_GLASS_DisplayDigit(uint8_t* dig, Point_Typedef point, Semicol_Typedef semicol,uint8_t position);
void LCD_GLASS_DisplaySpecial(uint32_t spec);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32L152_EVAL_GLASS_LCD_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
