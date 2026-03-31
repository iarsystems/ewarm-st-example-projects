/**
 *******************************************************************************
 * @file     iddmeasurement.h
 * @author   MCD Application Team
 * @version  V1.0.0
 * @date     03/01/2010
 * @brief    This file contains all the functions prototypes for the idd measurement
 *           firmware driver.
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
#ifndef __IDDMEASUREMENT_H
#define __IDDMEASUREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32100B_EVAL_Demo
  * @{
  */

/** @addtogroup IDD_MEASUREMENT
  * @{
  */

/** @defgroup IDD_MEASUREMENT_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup IDD_MEASUREMENT_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup IDD_MEASUREMENT_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup IDD_MEASUREMENT_Exported_Functions
  * @{
  */

void IDD_Measurement_Run(void);
void IDD_Measurement_Stop(void);
void IDD_Measurement_Sleep(void);

#ifdef __cplusplus
}
#endif

#endif /* __IDDMEASUREMENT_H */
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
