/**
 *******************************************************************************
 * @file     cec_display.h
 * @author   MCD Application Team
 * @version  V1.0.0
 * @date     03/01/2010
 * @brief    Header file for cec_display.c file.
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
#ifndef __CEC_DISPLAY_H
#define __CEC_DISPLAY_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32100B_EVAL_Demo
  * @{
  */

/** @addtogroup CEC_DISPLAY
  * @{
  */

/** @defgroup CEC_DISPLAY_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup CEC_DISPLAY_Exported_Constants
  * @{
  */
/**
  * @brief LCD COLUMNs
  */
#define LCD_COLUMN_0        320
#define LCD_COLUMN_1        304
#define LCD_COLUMN_2        288
#define LCD_COLUMN_3        272
#define LCD_COLUMN_4        256
#define LCD_COLUMN_5        240
#define LCD_COLUMN_6        224
#define LCD_COLUMN_7        208
#define LCD_COLUMN_8        192
#define LCD_COLUMN_9        176
#define LCD_COLUMN_10       160
#define LCD_COLUMN_11       144
#define LCD_COLUMN_12       128
#define LCD_COLUMN_13       112
#define LCD_COLUMN_14        96
#define LCD_COLUMN_15        80
#define LCD_COLUMN_16        64
#define LCD_COLUMN_17        48
#define LCD_COLUMN_18        32
#define LCD_COLUMN_19        16  
/**
  * @}
  */

/** @defgroup CEC_DISPLAY_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup CEC_DISPLAY_Exported_Functions
  * @{
  */
void CEC_Display_DeInit(void);
void CEC_Display_Init(void);
void CEC_Display_SelectDeviceType(void);
uint8_t CEC_Display_CECAvailableCommands(void);
uint8_t CEC_Display_SelectFollowerAddress(void);
void CEC_Display_ReceiveInfoDisplay(void);
void CEC_Display_DisplayHexToChar(uint8_t Data, uint16_t Line, uint16_t column);
void CEC_Display(uint8_t Line, uint8_t *ptr);
uint8_t CEC_Display_HexToChar(uint8_t Data);
void CEC_Display_DisplayBuffer(uint8_t *DataBuffer, uint8_t BufferSize, uint16_t Line);
void CEC_Display_LCD_DisplayString(uint8_t Row, uint16_t Column, uint8_t *ptr);

#ifdef __cplusplus
}
#endif

#endif /* __CEC_DISPLAY_H */

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
