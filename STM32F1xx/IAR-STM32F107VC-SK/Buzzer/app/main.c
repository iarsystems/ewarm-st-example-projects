/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 9, September 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F107VC-SK board. It shows how
 * to make the buzzer make a sound. The Buzzer demo application is
 * downloaded to Embedded Flash memory on the evaluation board.
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   JTAG_SEL  - open (on board j-link)
 *   B0_1/B0_0 - B0_0
 *   B1_1/B1_0 - B1_0
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US  450

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

#define BUZZER_MASK           GPIO_Pin_13
#define BUZZER_PORT           GPIOD

Int32U CriticalSecCntr;

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
  }
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
void main(void)
{
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
GPIO_InitTypeDef GPIO_InitStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
Buttons_t ButtCtrl;

  ENTR_CRT_SECTION();
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  EXT_CRT_SECTION();

  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);
  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(10,104,131,131);

  // Buzzer init
  // GPIO enable clock and release Reset
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOD 
                         | RCC_APB2Periph_AFIO, DISABLE);
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOD 
                         | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  BUZZER_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

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

  while(1)
  {
    ButtCtrl = GetButtons();

    if (ButtCtrl.Tamper)
    {
      GLCD_Backlight(BACKLIGHT_OFF);
      // Map TIM4 OC2 to PD13
      GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
      TIM4->CCR2 = 0x80;
    }
    else
    {
      GLCD_Backlight(BACKLIGHT_ON);

      GPIO_PinRemapConfig(GPIO_Remap_TIM4,DISABLE);
    }
  }
}
