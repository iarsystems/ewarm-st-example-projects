/**
  ******************************************************************************
  * @file    stm32_eval.h
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   Header file for stm32_eval.c module.
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
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_EVAL_H
#define __STM32_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup Utilities
  * @{
  */ 
  
/** @addtogroup STM3210E_EVAL
  * @{
  */ 


/** @defgroup STM3210E_EVAL_Exported_Types
  * @{
  */
/**
  * @}
  */ 



/** @defgroup STM3210E_EVAL_Exported_Constants
  * @{
  */ 
/** @addtogroup STM3210E_EVAL_LED
  * @{
  */
#define LEDn                        4
#define LED1_GPIO_PORT              GPIOC
#define LED1_GPIO_CLK               RCC_APB2Periph_GPIOC  
#define LED1_GPIO_PIN               GPIO_Pin_6
  
#define LED2_GPIO_PORT              GPIOC
#define LED2_GPIO_CLK               RCC_APB2Periph_GPIOC  
#define LED2_GPIO_PIN               GPIO_Pin_7
  
#define LED3_GPIO_PORT              GPIOC
#define LED3_GPIO_CLK               RCC_APB2Periph_GPIOC  
#define LED3_GPIO_PIN               GPIO_Pin_8
  
#define LED4_GPIO_PORT              GPIOC
#define LED4_GPIO_CLK               RCC_APB2Periph_GPIOC  
#define LED4_GPIO_PIN               GPIO_Pin_9

/**
  * @}
  */
  
/** @addtogroup STM3210E_EVAL_BUTTON
  * @{
  */  
#define BUTTONn                     8

/**
 * @brief Wakeup push-button
 */
#define WAKEUP_BUTTON_PORT          GPIOA
#define WAKEUP_BUTTON_CLK           RCC_APB2Periph_GPIOA
#define WAKEUP_BUTTON_PIN           GPIO_Pin_0
#define WAKEUP_BUTTON_EXTI_LINE     EXTI_Line0
#define WAKEUP_BUTTON_PORT_SOURCE   GPIO_PortSourceGPIOA
#define WAKEUP_BUTTON_PIN_SOURCE    GPIO_PinSource0
#define WAKEUP_BUTTON_IRQn          EXTI0_IRQn 
/**
 * @brief Tamper push-button
 */
#define TAMPER_BUTTON_PORT          GPIOC
#define TAMPER_BUTTON_CLK           RCC_APB2Periph_GPIOC
#define TAMPER_BUTTON_PIN           GPIO_Pin_13
#define TAMPER_BUTTON_EXTI_LINE     EXTI_Line13
#define TAMPER_BUTTON_PORT_SOURCE   GPIO_PortSourceGPIOC
#define TAMPER_BUTTON_PIN_SOURCE    GPIO_PinSource13
#define TAMPER_BUTTON_IRQn          EXTI15_10_IRQn 
/**
 * @brief Key push-button
 */
#define KEY_BUTTON_PORT             GPIOE
#define KEY_BUTTON_CLK              RCC_APB2Periph_GPIOG
#define KEY_BUTTON_PIN              GPIO_Pin_1
#define KEY_BUTTON_EXTI_LINE        EXTI_Line1
#define KEY_BUTTON_PORT_SOURCE      GPIO_PortSourceGPIOG
#define KEY_BUTTON_PIN_SOURCE       GPIO_PinSource8
#define KEY_BUTTON_IRQn             EXTI1_IRQn
/**
 * @brief Joystick Right push-button
 */
#define RIGHT_BUTTON_PORT           GPIOC
#define RIGHT_BUTTON_CLK            RCC_APB2Periph_GPIOC
#define RIGHT_BUTTON_PIN            GPIO_Pin_2
#define RIGHT_BUTTON_EXTI_LINE      EXTI_Line2
#define RIGHT_BUTTON_PORT_SOURCE    GPIO_PortSourceGPIOC
#define RIGHT_BUTTON_PIN_SOURCE     GPIO_PinSource2
#define RIGHT_BUTTON_IRQn           EXTI2_IRQn
/**
 * @brief Joystick Left push-button
 */    
#define LEFT_BUTTON_PORT            GPIOE
#define LEFT_BUTTON_CLK             RCC_APB2Periph_GPIOE
#define LEFT_BUTTON_PIN             GPIO_Pin_7
#define LEFT_BUTTON_EXTI_LINE       EXTI_Line7
#define LEFT_BUTTON_PORT_SOURCE     GPIO_PortSourceGPIOE
#define LEFT_BUTTON_PIN_SOURCE      GPIO_PinSource7
#define LEFT_BUTTON_IRQn            EXTI9_5_IRQn  
/**
 * @brief Joystick Up push-button
 */
#define UP_BUTTON_PORT              GPIOC
#define UP_BUTTON_CLK               RCC_APB2Periph_GPIOC
#define UP_BUTTON_PIN               GPIO_Pin_3
#define UP_BUTTON_EXTI_LINE         EXTI_Line3
#define UP_BUTTON_PORT_SOURCE       GPIO_PortSourceGPIOC
#define UP_BUTTON_PIN_SOURCE        GPIO_PinSource3
#define UP_BUTTON_IRQn              EXTI3_IRQn  
/**
 * @brief Joystick Down push-button
 */   
#define DOWN_BUTTON_PORT            GPIOE
#define DOWN_BUTTON_CLK             RCC_APB2Periph_GPIOE
#define DOWN_BUTTON_PIN             GPIO_Pin_13
#define DOWN_BUTTON_EXTI_LINE       EXTI_Line13
#define DOWN_BUTTON_PORT_SOURCE     GPIO_PortSourceGPIOE
#define DOWN_BUTTON_PIN_SOURCE      GPIO_PinSource13
#define DOWN_BUTTON_IRQn            EXTI15_10_IRQn  
/**
 * @brief Joystick Sel push-button
 */  
#define SEL_BUTTON_PORT             GPIOE
#define SEL_BUTTON_CLK              RCC_APB2Periph_GPIOE
#define SEL_BUTTON_PIN              GPIO_Pin_12
#define SEL_BUTTON_EXTI_LINE        EXTI_Line12
#define SEL_BUTTON_PORT_SOURCE      GPIO_PortSourceGPIOE
#define SEL_BUTTON_PIN_SOURCE       GPIO_PinSource12
#define SEL_BUTTON_IRQn             EXTI15_10_IRQn          
/**
  * @}
  */ 

/** @addtogroup STM3210E_EVAL_COM
  * @{
  */
#define COMn                        2

/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define EVAL_COM1                   USART2
#define EVAL_COM1_GPIO              GPIOD
#define EVAL_COM1_CLK               RCC_APB1Periph_USART2
#define EVAL_COM1_GPIO_CLK          RCC_APB2Periph_GPIOD
#define EVAL_COM1_RxPin             GPIO_Pin_6
#define EVAL_COM1_TxPin             GPIO_Pin_5

/**
 * @brief Definition for COM port2, connected to USART2
 */ 
#define EVAL_COM2                   USART3
#define EVAL_COM2_GPIO              GPIOD
#define EVAL_COM2_CLK               RCC_APB1Periph_USART3
#define EVAL_COM2_GPIO_CLK          RCC_APB2Periph_GPIOD
#define EVAL_COM2_RxPin             GPIO_Pin_9
#define EVAL_COM2_TxPin             GPIO_Pin_8


typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
} Led_TypeDef;

typedef enum 
{  
  Button_WAKEUP = 0,
  Button_TAMPER = 1,
  Button_KEY = 2,
  Button_RIGHT = 3,
  Button_LEFT = 4,
  Button_UP = 5,
  Button_DOWN = 6,
  Button_SEL = 7
} Button_TypeDef;

typedef enum 
{  
  Mode_GPIO = 0,
  Mode_EXTI = 1
} Button_Mode_TypeDef;

typedef enum 
{ 
  JOY_NONE = 0,
  JOY_CENTER = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOY_State_TypeDef;

typedef enum 
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;
 
/**
  * @}
  */ 

/** @defgroup STM32_EVAL_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32_EVAL_Exported_Functions
  * @{
  */ 
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(Button_TypeDef Button, Button_Mode_TypeDef Button_Mode);
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button);
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);

#ifdef __cplusplus
}
#endif


#endif /* __STM32_EVAL_H */
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

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
