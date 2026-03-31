/**
 *******************************************************************************
 * @file     stm32f10x_it.h
 * @author   MCD Application Team
 * @version  V1.0.0
 * @date     03/01/2010
 * @brief    This file contains the headers of the interrupt handlers.
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
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32100B_EVAL_Demo
  * @{
  */

/** @addtogroup IT
  * @{
  */

/** @defgroup IT_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup IT_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup IT_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup IT_Exported_Functions
  * @{
  */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void RTC_IRQHandler(void);
void CEC_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void TIM1_UP_TIM16_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void RTCAlarm_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void I2C1_ER_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IT_H */
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
