/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : stm32.h
 *    Description : board support include file
 *
 *    History :
 *    1. Date        : 07, March 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include <stdbool.h>
#include "stm32l1xx.h"
#include "arm_comm.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_H
#define STM32_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
} Led_TypeDef;

typedef enum
{
  BUTTON_WAKEUP = 0,
  BUTTON_USER = 1,
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;

#define LEDn                            4

#define LED1_PIN                        GPIO_Pin_4
#define LED1_GPIO_PORT                  GPIOA
#define LED1_GPIO_CLK                   RCC_AHBPeriph_GPIOA

#define LED2_PIN                        GPIO_Pin_5
#define LED2_GPIO_PORT                  GPIOA
#define LED2_GPIO_CLK                   RCC_AHBPeriph_GPIOA

#define LED3_PIN                        GPIO_Pin_10
#define LED3_GPIO_PORT                  GPIOE
#define LED3_GPIO_CLK                   RCC_AHBPeriph_GPIOE

#define LED4_PIN                        GPIO_Pin_11
#define LED4_GPIO_PORT                  GPIOE
#define LED4_GPIO_CLK                   RCC_AHBPeriph_GPIOE

#define BUTTONn                         2

#define USER_BUTTON_PIN                 GPIO_Pin_13
#define USER_BUTTON_GPIO_PORT           GPIOC
#define USER_BUTTON_GPIO_CLK            RCC_AHBPeriph_GPIOC
#define USER_BUTTON_EXTI_LINE           EXTI_Line13
#define USER_BUTTON_EXTI_PORT_SOURCE    EXTI_PortSourceGPIOC
#define USER_BUTTON_EXTI_PIN_SOURCE     EXTI_PinSource13
#define USER_BUTTON_EXTI_IRQn           EXTI15_10_IRQn

#define WAKEUP_BUTTON_PIN               GPIO_Pin_0
#define WAKEUP_BUTTON_GPIO_PORT         GPIOA
#define WAKEUP_BUTTON_GPIO_CLK          RCC_AHBPeriph_GPIOA
#define WAKEUP_BUTTON_EXTI_LINE         EXTI_Line0
#define WAKEUP_BUTTON_EXTI_PORT_SOURCE  EXTI_PortSourceGPIOA
#define WAKEUP_BUTTON_EXTI_PIN_SOURCE   EXTI_PinSource0
#define WAKEUP_BUTTON_EXTI_IRQn         EXTI0_IRQn

#define COMn                            2

#define STM32_COM1                      USART2
#define STM32_COM1_CLK                  RCC_APB1Periph_USART2
#define STM32_COM1_TX_PIN               GPIO_Pin_5
#define STM32_COM1_TX_GPIO_PORT         GPIOD
#define STM32_COM1_TX_GPIO_CLK          RCC_AHBPeriph_GPIOD
#define STM32_COM1_TX_SOURCE            GPIO_PinSource5
#define STM32_COM1_TX_AF                GPIO_AF_USART2
#define STM32_COM1_RX_PIN               GPIO_Pin_6
#define STM32_COM1_RX_GPIO_PORT         GPIOD
#define STM32_COM1_RX_GPIO_CLK          RCC_AHBPeriph_GPIOD
#define STM32_COM1_RX_SOURCE            GPIO_PinSource6
#define STM32_COM1_RX_AF                GPIO_AF_USART2
#define STM32_COM1_IRQn                 USART2_IRQn
#define STM32_COM1_PWR_GPIO_CLK         RCC_AHBPeriph_GPIOD
#define STM32_COM1_PWR_GPIO_PORT        GPIOD
#define STM32_COM1_PWR_PIN              GPIO_Pin_0
#define STM32_COM1_RTS_GPIO_CLK         RCC_AHBPeriph_GPIOD
#define STM32_COM1_RTS_GPIO_PORT        GPIOD
#define STM32_COM1_RTS_PIN              GPIO_Pin_4
#define STM32_COM1_RTS_SOURCE           GPIO_PinSource4
#define STM32_COM1_RTS_AF               GPIO_AF_USART2
#define STM32_COM1_CTS_GPIO_CLK         RCC_AHBPeriph_GPIOD
#define STM32_COM1_CTS_GPIO_PORT        GPIOD
#define STM32_COM1_CTS_PIN              GPIO_Pin_3
#define STM32_COM1_CTS_SOURCE           GPIO_PinSource3
#define STM32_COM1_CTS_AF               GPIO_AF_USART2

#define STM32_COM2                      USART3
#define STM32_COM2_CLK                  RCC_APB1Periph_USART3
#define STM32_COM2_TX_PIN               GPIO_Pin_10
#define STM32_COM2_TX_GPIO_PORT         GPIOC
#define STM32_COM2_TX_GPIO_CLK          RCC_AHBPeriph_GPIOC
#define STM32_COM2_TX_SOURCE            GPIO_PinSource10
#define STM32_COM2_TX_AF                GPIO_AF_USART3
#define STM32_COM2_RX_PIN               GPIO_Pin_11
#define STM32_COM2_RX_GPIO_PORT         GPIOC
#define STM32_COM2_RX_GPIO_CLK          RCC_AHBPeriph_GPIOC
#define STM32_COM2_RX_SOURCE            GPIO_PinSource11
#define STM32_COM2_RX_AF                GPIO_AF_USART3
#define STM32_COM2_IRQn                 USART3_IRQn
#define STM32_COM2_PWR_GPIO_CLK         0
#define STM32_COM2_PWR_GPIO_PORT        NULL
#define STM32_COM2_PWR_PIN              0
#define STM32_COM2_RTS_GPIO_CLK         0
#define STM32_COM2_RTS_GPIO_PORT        NULL
#define STM32_COM2_RTS_PIN              0
#define STM32_COM2_RTS_SOURCE           0
#define STM32_COM2_RTS_AF               0
#define STM32_COM2_CTS_GPIO_CLK         0
#define STM32_COM2_CTS_GPIO_PORT        NULL
#define STM32_COM2_CTS_PIN              0
#define STM32_COM2_CTS_SOURCE           0
#define STM32_COM2_CTS_AF               0

void STM32_LEDInit(Led_TypeDef Led);
void STM32_LEDOn(Led_TypeDef Led);
void STM32_LEDOff(Led_TypeDef Led);
void STM32_LEDToggle(Led_TypeDef Led);
void STM32_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM32_PBGetState(Button_TypeDef Button);
void STM32_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct, bool PowerState);
void STM32_COM_PowerCtrl(COM_TypeDef COM, bool State);
void STM32_AN_TR_Init(bool State);
void STM32_AN_TR_PowerCtrl(bool State);

#ifdef __cplusplus
}
#endif

#endif /* STM32_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
