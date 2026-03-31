/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 9, March 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32L152-SK evaluation board. It shows
 * basic use of I/O, timer and the interrupt controller.
 *  Displays running lights on the LEDs.
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   3.3V_MCU_E - filled
 *   B0_0/B0_1  - B0_0
 *   B1_0/B1_1  - B1_0
 *
 *    $Revision: 4202 $
 **************************************************************************/

#include <intrinsics.h>
#include "stm32l1xx.h"
#include "stm32.h"

static volatile uint32_t s_Tick;

void LEDsSet (unsigned int);

/*************************************************************************
 * Function Name: Timer2IntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 2 interrupt handler
 *
 *************************************************************************/
void Timer2IntrHandler (void)
{
  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
  s_Tick = 1;
}

/*************************************************************************
 * Function Name: LEDsSet
 * Parameters: unsigned int State
 *
 * Return: none
 *
 * Description: Set LEDS State
 *
 *************************************************************************/
void LEDsSet (unsigned int State)
{

  if(State & 1)
  {
    STM32_LEDOn(LED1);
  }
  else
  {
    STM32_LEDOff(LED1);
  }
  if(State & 2)
  {
    STM32_LEDOn(LED2);
  }
  else
  {
    STM32_LEDOff(LED2);
  }
  if(State & 4)
  {
    STM32_LEDOn(LED3);
  }
  else
  {
    STM32_LEDOff(LED3);
  }
  if(State & 8)
  {
    STM32_LEDOn(LED4);
  }
  else
  {
    STM32_LEDOff(LED4);
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
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

  __disable_interrupt();

  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  // NVIC init
#ifndef  EMB_FLASH
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  // LEDs Init
  STM32_LEDInit(LED1);
  STM32_LEDInit(LED2);
  STM32_LEDInit(LED3);
  STM32_LEDInit(LED4);

  // Timer2 Init
  // Enable Timer1 clock and release reset
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2,DISABLE);

  // Set timer period 0.1 sec
  TIM_TimeBaseInitStruct.TIM_Prescaler = 320;  // 10us resolution
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = 10000;  // 100 ms
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);

  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
  // Enable update interrupt
  TIM_ITConfig(TIM2,TIM_FLAG_Update,ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // Enable timer counting
  TIM_Cmd(TIM2,ENABLE);

  __enable_interrupt();

  while(1)
  {
    // LEDs pattern 0000
    while(!s_Tick);
    s_Tick = 0;
    LEDsSet(0x1);
    // LEDs pattern 0001
    while(!s_Tick);
    s_Tick = 0;
    LEDsSet(0x3);
    // LEDs pattern 0011
    while(!s_Tick);
    s_Tick = 0;
    LEDsSet(0x7);
    // LEDs pattern 0111
    while(!s_Tick);
    s_Tick = 0;
    LEDsSet(0xF);
    // LEDs pattern 1111
    while(!s_Tick);
    s_Tick = 0;
    LEDsSet(0xE);
    // LEDs pattern 1110
    while(!s_Tick);
    s_Tick = 0;
    LEDsSet(0xC);
    // LEDs pattern 1100
    while(!s_Tick);
    s_Tick = 0;
    LEDsSet(0x8);
    // LEDs pattern 1000
    while(!s_Tick);
    s_Tick = 0;
    LEDsSet(0x0);
  }
}
