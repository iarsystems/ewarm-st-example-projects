/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : platform_config.h
* Author             : MCD Application Team
* Version            : V0.0.2
* Date               : 03/05/2009
* Description        : Evaluation board specific configuration file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
#if !defined (USE_STM3210D_EVAL) 
 #define USE_STM3210D_EVAL
#endif

/* Define the STM32F10x hardware depending on the used evaluation board */
#ifdef USE_STM3210D_EVAL
  #define LED_GPIO_PORT             GPIOD
  #define LED1_GPIO_PORT            GPIOE
  #define LED1                      GPIO_Pin_14
  #define LED2                      GPIO_Pin_13
  #define LED3                      GPIO_Pin_3
  #define LED4                      GPIO_Pin_4
  #define KEY_BUTTON_PORT           GPIOB
  #define KEY_BUTTON_PIN            GPIO_Pin_9
  #define KEY_BUTTON_EXTI_LINE      EXTI_Line9
  #define KEY_BUTTON_PORT_SOURCE	GPIO_PortSourceGPIOB
  #define KEY_BUTTON_PIN_SOURCE		GPIO_PinSource9
  #define LED_KEY_GPIO_CLOCK        RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE
  #define RCC_APB2Periph_GPIO       RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |\
                                    RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
  #define GPIO_NUMBER               5 
#endif /* USE_STM3210D_EVAL */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
