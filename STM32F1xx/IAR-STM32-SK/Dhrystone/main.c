/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 14, May 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32-SK evaluation board.
 *  It runs dhrysrone test bench. The results are showed in SPY Terminal I/O
 * window (Veiw->Terminal I/O).
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *
 *    $Revision: 39 $
 **************************************************************************/
#include <intrinsics.h>
#include "stm32f10x.h"

extern  void dystone (void);

/*************************************************************************
 * Function Name: GetTimersTick
 * Parameters: none
 *
 * Return: unsigned long
 *
 * Description: Return count of both cascade conected timers
 *
 *************************************************************************/
unsigned long GetTimersTick (void)
{
unsigned long count;
  TIM_Cmd(TIM2, DISABLE);
  TIM_Cmd(TIM3, DISABLE);
  count  = TIM_GetCounter(TIM2);
  count |= ((unsigned long)TIM_GetCounter(TIM3)) << 16;
  TIM_Cmd(TIM3, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  return(count);
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
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

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

  // Timers Init
  // TIM2 and TIM3 clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3,
                         ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3,
                         DISABLE);

  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Select the Master Slave Mode */
  TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

  /* Master Mode selection */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

  /* Slave Mode selection: TIM3 */
  TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_External1);
  TIM_SelectInputTrigger(TIM3, TIM_TS_ITR1);

  /* Select the Master Slave Mode */
  TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);

  /* Master Mode selection: TIM3 */
  TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);

  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);
  TIM_Cmd(TIM2, ENABLE);

  __enable_interrupt();

  dystone();

  while(1)
  {
  }
}
