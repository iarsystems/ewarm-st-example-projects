/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Buzzer_Test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : Main program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

/* Private function prototypes -----------------------------------------------*/
void GPIO_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Buzzer_Test
* Description    : Buzzer_Test program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Buzzer_Test(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);

  /* Display Test name */
  GLCD_print ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  GLCD_print( "Buzzer Test\n ");

  /* GPIO Configuration */
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, DISABLE);
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

  /*GPIOC Configuration: TIM4 channel 2 as alternate function push-pull */
  GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // Init PWM TIM4
  // Enable Timer4 clock and release reset
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4,DISABLE);
  TIM_InternalClockConfig(TIM4);

  GPIO_PinRemapConfig(GPIO_Remap_TIM4,DISABLE);

  // Time base configuration
  TIM_TimeBaseStructure.TIM_Prescaler = 110;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0xFF; // 8 bit resolution
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

  // Channel 2 Configuration in PWM mode
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0x00;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC2Init(TIM4,&TIM_OCInitStructure);
  // Double buffered
  TIM_ARRPreloadConfig(TIM4,ENABLE);
  // TIM4 counter enable
  TIM_Cmd(TIM4,ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
  TIM4->CCR2 = 0x80;

    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,9);
    GLCD_print( "Press User key to exit\n ");

  while(ReadKey() != User)
    {
    }

  GPIO_PinRemapConfig(GPIO_Remap_TIM4, DISABLE);

  ReturnFunc();

  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
