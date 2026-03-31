/**
  ******************************************************************************
  * @file    CortexM3/MPU/main.h 
  * @author  MCD Application Team
  * @version V1.0.0RC1
  * @date    07/02/2010
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
#define RAM_ADDRESS_START                        (0x20000000UL)
#define RAM_SIZE                                 (0x19UL << 0UL)
#define PERIPH_ADDRESS_START                     (0x40000000)
#define PERIPH_SIZE                              (0x39UL << 0UL)
#define FLASH_ADDRESS_START                      (0x08000000)
#define FLASH_SIZE                               (0x27UL << 0UL)
#define portMPU_REGION_READ_WRITE                (0x03UL << (MPU_RASR_ATTRS_Pos+8))
#define portMPU_REGION_PRIVILEGED_READ_ONLY      (0x05UL << (MPU_RASR_ATTRS_Pos+8))
#define portMPU_REGION_READ_ONLY                 (0x06UL << (MPU_RASR_ATTRS_Pos+8))
#define portMPU_REGION_PRIVILEGED_READ_WRITE     (0x01UL << (MPU_RASR_ATTRS_Pos+8))
#define RAM_REGION_NUMBER                        (0x0ul << MPU_RNR_REGION_Pos)
#define FLASH_REGION_NUMBER                      (0x1ul << MPU_RNR_REGION_Pos)
#define PERIPH_REGION_NUMBER                     (0x2ul << MPU_RNR_REGION_Pos)

/* Exported functions ------------------------------------------------------- */
void accesspermission(void);
void overlap(void); 
void subregion(void); 
void background(void);
void nmi_hardfault(void);

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
