/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 19, July 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32-SK evaluation board. It shows
 * basic use of I/O, timer and the interrupt controllers.
 *  Displays running lights on the board LED's.
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *
 *    $Revision: 39 $
 **************************************************************************/
#include <intrinsics.h>
#include "stm32f10x.h"

volatile unsigned int LedState = 0; // LED is ON when corresponding bit is 1

void LEDsSet (unsigned int);

/*************************************************************************
 * Function Name: Timer1IntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 1 interrupt handler
 *
 *************************************************************************/
void Timer1IntrHandler (void)
{
  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);
  if(((LedState <<= 1) > 0x8000) || (LedState == 0))
  {
    LedState = 1;
  }
  LEDsSet(LedState);
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
  GPIO_WriteBit(GPIOA,GPIO_Pin_4 ,(State & (1<<0 ))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOA,GPIO_Pin_5 ,(State & (1<<1 ))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOA,GPIO_Pin_6 ,(State & (1<<2 ))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOA,GPIO_Pin_7 ,(State & (1<<3 ))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOA,GPIO_Pin_9 ,(State & (1<<4 ))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOA,GPIO_Pin_10,(State & (1<<5 ))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOB,GPIO_Pin_6 ,(State & (1<<6 ))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOB,GPIO_Pin_7 ,(State & (1<<7 ))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOB,GPIO_Pin_10,(State & (1<<8 ))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOB,GPIO_Pin_11,(State & (1<<9 ))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOB,GPIO_Pin_12,(State & (1<<10))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOB,GPIO_Pin_13,(State & (1<<11))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOB,GPIO_Pin_14,(State & (1<<12))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOB,GPIO_Pin_15,(State & (1<<13))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOC,GPIO_Pin_6 ,(State & (1<<14))?Bit_RESET:Bit_SET);
  GPIO_WriteBit(GPIOC,GPIO_Pin_7 ,(State & (1<<15))?Bit_RESET:Bit_SET);
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
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef TIM1_TimeBaseInitStruct;

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

  // GPIO Init
  // Enable GPIO clock and release reset
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC,
                         ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC,
                         DISABLE);

  // Assign PA4, PA5, PA6, PA7, PA9, PA10, PB6, PB7, PB10, PB11, PB12, PB13,
  // PB14, PB15, PC6, PC7 to LEDs
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |
                                GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_12|
                                GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_13|
                                GPIO_Pin_14| GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  LEDsSet(LedState);

  // Timer1 Init
  // Enable Timer1 clock and release reset
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1,DISABLE);

  // Set timer period 0.1 sec
  TIM1_TimeBaseInitStruct.TIM_Prescaler = 720;  // 10us resolution
  TIM1_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM1_TimeBaseInitStruct.TIM_Period = 10000;  // 100 ms
  TIM1_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM1_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseInitStruct);

  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);
  // Enable update interrupt
  TIM_ITConfig(TIM1,TIM_FLAG_Update,ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // Enable timer counting
  TIM_Cmd(TIM1,ENABLE);

  __enable_interrupt();

  while(1)
  {
  }
}
