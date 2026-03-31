/**
 *******************************************************************************
 * @file     lowpower.h
 * @author   MCD Application Team
 * @version  V1.0.0
 * @date     03/01/2010
 * @brief    This file contains all the functions prototypes for the low power
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
#ifndef __LOWPOWER_H
#define __LOWPOWER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32100B_EVAL_Demo
  * @{
  */

/** @addtogroup LOWPOWER
  * @{
  */

/** @defgroup LOWPOWER_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Exported_Functions
  * @{
  */
void LowPower_Init(void);
void LowPower_SYSCLKConfig_STOP(void);
void LowPower_EnterSTOPMode_EXTI(void);
void LowPower_EnterSTOPMode_RTCAlarm(void);
void LowPower_EnterSTANDBYMode_WAKEUP(void);
void LowPower_EnterSTANDBYMode_RTCAlarm(void);
void LowPower_Set_STOPModeStatus(void);
void LowPower_SaveGPIOConfig(void);
void LowPower_RestoreGPIOConfig(void);

#ifdef __cplusplus
}
#endif

#endif /* __LOWPOWER_H */
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
