/**
  ******************************************************************************
  * @file    Lib_DEBUG/lib_dbg.c 
  * @author  MCD Application Team
  * @version V1.0.0RC1
  * @date    07/02/2010
  * @brief   This file provides all peripherals pointers initialization.
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

/* Includes ------------------------------------------------------------------*/
#include "lib_dbg.h"

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup Lib_DEBUG
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TypeDef             *TIM2_DBG;
TIM_TypeDef             *TIM3_DBG;
TIM_TypeDef             *TIM4_DBG;
TIM_TypeDef             *TIM6_DBG;
TIM_TypeDef             *TIM7_DBG;
TIM_TypeDef             *TIM9_DBG;
TIM_TypeDef             *TIM10_DBG;
TIM_TypeDef             *TIM11_DBG;
RTC_TypeDef             *RTC_DBG;
WWDG_TypeDef            *WWDG_DBG;
IWDG_TypeDef            *IWDG_DBG;
SPI_TypeDef             *SPI2_DBG;
USART_TypeDef           *USART1_DBG;
USART_TypeDef           *USART2_DBG;
USART_TypeDef           *USART3_DBG;
I2C_TypeDef             *I2C1_DBG;
I2C_TypeDef             *I2C2_DBG;
PWR_TypeDef             *PWR_DBG;
DAC_TypeDef             *DAC_DBG;
EXTI_TypeDef            *EXTI_DBG;
GPIO_TypeDef            *GPIOA_DBG;
GPIO_TypeDef            *GPIOB_DBG;
GPIO_TypeDef            *GPIOC_DBG;
GPIO_TypeDef            *GPIOD_DBG;
GPIO_TypeDef            *GPIOE_DBG;
GPIO_TypeDef            *GPIOH_DBG;
ADC_TypeDef             *ADC1_DBG;
SPI_TypeDef             *SPI1_DBG;
DMA_TypeDef             *DMA1_DBG;
DMA_Channel_TypeDef     *DMA1_Channel1_DBG;
DMA_Channel_TypeDef     *DMA1_Channel2_DBG;
DMA_Channel_TypeDef     *DMA1_Channel3_DBG;
DMA_Channel_TypeDef     *DMA1_Channel4_DBG;
DMA_Channel_TypeDef     *DMA1_Channel5_DBG;
DMA_Channel_TypeDef     *DMA1_Channel6_DBG;
DMA_Channel_TypeDef     *DMA1_Channel7_DBG;
RCC_TypeDef             *RCC_DBG;
CRC_TypeDef             *CRC_DBG;
FLASH_TypeDef           *FLASH_DBG;
OB_TypeDef              *OB_DBG;  
DBGMCU_TypeDef          *DBGMCU_DBG;
SysTick_Type            *SysTick_DBG;
NVIC_Type               *NVIC_DBG;
SCB_Type                *SCB_DBG;
COMP_TypeDef            *COMP_DBG;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function initialize peripherals pointers.
  * @param  None
  * @retval None
  */
void debug(void)
{
/************************************* ADC ************************************/
  ADC1_DBG = (ADC_TypeDef *)  ADC1_BASE;
  
/************************************* CRC ************************************/
  CRC_DBG = (CRC_TypeDef *)  CRC_BASE;

/************************************* DAC ************************************/
  DAC_DBG = (DAC_TypeDef *)  DAC_BASE;

/************************************* DBGMCU**********************************/
  DBGMCU_DBG = (DBGMCU_TypeDef *)  DBGMCU_BASE;


/************************************* DMA ************************************/
  DMA1_DBG = (DMA_TypeDef *)  DMA1_BASE;

  DMA1_Channel1_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel1_BASE;

  DMA1_Channel2_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel2_BASE;

  DMA1_Channel3_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel3_BASE;

  DMA1_Channel4_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel4_BASE;

  DMA1_Channel5_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel5_BASE;

  DMA1_Channel6_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel6_BASE;

  DMA1_Channel7_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel7_BASE;

/************************************* EXTI ***********************************/
  EXTI_DBG = (EXTI_TypeDef *)  EXTI_BASE;

/************************************* FLASH and Option Bytes *****************/
  FLASH_DBG = (FLASH_TypeDef *)  FLASH_R_BASE;
  OB_DBG = (OB_TypeDef *)        OB_BASE;

/************************************* GPIO ***********************************/
  GPIOA_DBG = (GPIO_TypeDef *)  GPIOA_BASE;

  GPIOB_DBG = (GPIO_TypeDef *)  GPIOB_BASE;

  GPIOC_DBG = (GPIO_TypeDef *)  GPIOC_BASE;

  GPIOD_DBG = (GPIO_TypeDef *)  GPIOD_BASE;

  GPIOE_DBG = (GPIO_TypeDef *)  GPIOE_BASE;

  GPIOH_DBG = (GPIO_TypeDef *)  GPIOH_BASE;

/************************************* I2C ************************************/
  I2C1_DBG = (I2C_TypeDef *)  I2C1_BASE;

  I2C2_DBG = (I2C_TypeDef *)  I2C2_BASE;

/************************************* IWDG ***********************************/
  IWDG_DBG = (IWDG_TypeDef *) IWDG_BASE;

/************************************* NVIC ***********************************/
  NVIC_DBG = (NVIC_Type *)  NVIC_BASE;
  SCB_DBG = (SCB_Type *)  SCB_BASE;

/************************************* PWR ************************************/
  PWR_DBG = (PWR_TypeDef *)  PWR_BASE;

/************************************* RCC ************************************/
  RCC_DBG = (RCC_TypeDef *)  RCC_BASE;

/************************************* RTC ************************************/
  RTC_DBG = (RTC_TypeDef *)  RTC_BASE;

/************************************* SPI ************************************/
  SPI1_DBG = (SPI_TypeDef *)  SPI1_BASE;

  SPI2_DBG = (SPI_TypeDef *)  SPI2_BASE;

/************************************* SysTick ********************************/
  SysTick_DBG = (SysTick_Type *)  SysTick_BASE;

/************************************* TIM ************************************/
  TIM2_DBG = (TIM_TypeDef *)  TIM2_BASE;

  TIM3_DBG = (TIM_TypeDef *)  TIM3_BASE;

  TIM4_DBG = (TIM_TypeDef *)  TIM4_BASE;

  TIM6_DBG = (TIM_TypeDef *)  TIM6_BASE;

  TIM7_DBG = (TIM_TypeDef *)  TIM7_BASE;

  TIM9_DBG = (TIM_TypeDef *)  TIM9_BASE;

  TIM10_DBG = (TIM_TypeDef *)  TIM10_BASE;

  TIM11_DBG = (TIM_TypeDef *)  TIM11_BASE;      

/************************************* USART **********************************/
  USART1_DBG = (USART_TypeDef *) USART1_BASE;

  USART2_DBG = (USART_TypeDef *) USART2_BASE;

  USART3_DBG = (USART_TypeDef *) USART3_BASE;


/************************************* WWDG ***********************************/
  WWDG_DBG = (WWDG_TypeDef *)  WWDG_BASE;
}

/**
  * @}
  */

/**
  * @}
  */
  
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
