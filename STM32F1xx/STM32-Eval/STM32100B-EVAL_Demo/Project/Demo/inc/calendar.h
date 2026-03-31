/**
 *******************************************************************************
 * @file     calendar.h
 * @author   MCD Application Team
 * @version  V1.0.0
 * @date     03/01/2010
 * @brief    This file contains all the functions prototypes for the calendar 
 *           firmware driver.
 *******************************************************************************
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
#ifndef __CALENDAR_H
#define __CALENDAR_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32100B_EVAL_Demo
  * @{
  */

/** @addtogroup CALENDAR
  * @{
  */

/** @defgroup CALENDAR_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup CALENDAR_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup CALENDAR_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup CALENDAR_Exported_Functions
  * @{
  */
uint8_t Calendar_ReadDigit(uint8_t ColBegin, uint8_t CountBegin, uint8_t ValueMax, uint8_t ValueMin);
void Calendar_Init(void);
uint32_t Calendar_TimeRegulate(void);
void Calendar_TimeAdjust(void);
void Calendar_TimeShow(void);
void Calendar_TimeDisplay(uint32_t TimeVar);
void Calendar_DateRegulate(void);
void Calendar_DateAdjust(void);
void Calendar_DateDisplay(uint16_t nYear, uint8_t nMonth, uint8_t nDay);
void Calendar_DateShow(void);
void Calendar_DateUpdate(void);
uint32_t Calendar_AlarmRegulate(void);
void Calendar_AlarmAdjust(void);
void Calendar_AlarmPreAdjust(void);
void Calendar_AlarmDisplay(uint32_t AlarmVar);
void Calendar_AlarmShow(void);

#ifdef __cplusplus
}
#endif

#endif /* __CALENDAR_H */
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
