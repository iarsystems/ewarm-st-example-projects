/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : clock-arch.c
 *    Description : Implementation of architecture-specific clock functionality
 *
 *    History :
 *    1. Date        : October 4, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
**************************************************************************/
#include "clock-arch.h"

volatile clock_time_t Ticks;
unsigned int Tim0Per;

/*************************************************************************
 * Function Name: Tim1Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: TIM1 interrupt handler
 *
 *************************************************************************/
void Tim1Handler (void)
{
  // Clear TIM1 counter
  TIM_CounterCmd(TIM1, TIM_CLEAR);
  ++Ticks;
  // Clear TIM1 flag OC1
  TIM_ClearFlag(TIM1,TIM_FLAG_OC1);
}

/*************************************************************************
 * Function Name: clock_init
 * Parameters: Int32U IntrPriority
 *
 * Return: none
 *
 * Description: Timer init
 *
 *************************************************************************/
void clock_init(Int32U IntrPriority)
{
TIM_InitTypeDef TIM_InitStructure;

  Ticks = 0;
  // Enable TIM0 clocks
  SCU_APBPeriphClockConfig(__TIM01, ENABLE);
  // Release TIM0 reset
  SCU_APBPeriphReset(__TIM01,DISABLE);

  // Timer 1
  // TIM Configuration in Output Compare Timing Mode period 200 msec
  SCU_TIMPresConfig(SCU_TIM01,((48000-1)&~1UL));    // Timer resolution 1ms
  SCU_TIMCLKSourceConfig(SCU_TIM01,SCU_TIMCLK_INT); // Timer01 Clock form the prescaler
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_SCU; // assign Prescaler clk to TIM_Clk
  TIM_InitStructure.TIM_Pulse_Length_1 =\
                            1000/TICK_PER_SEC;      // set period
  TIM_Init(TIM1, &TIM_InitStructure);
  TIM_CounterCmd(TIM1, TIM_CLEAR);                  // clear the timer 1 counter

  // VIC configuration
  VIC_Config(TIM1_ITLine, VIC_IRQ, IntrPriority);
  VIC_ITCmd(TIM1_ITLine, ENABLE);

  // Clear TIM1 flag OC1
  TIM_ClearFlag(TIM1,TIM_FLAG_OC1);
  // Enable TIM1 OC1 interrupt
  TIM_ITConfig(TIM1, TIM_IT_OC1, ENABLE);
  // Enable TIM1 counter
  TIM_CounterCmd(TIM1, TIM_START);
}

/*************************************************************************
 * Function Name: clock_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: The current clock time, measured in system ticks
 *
 *************************************************************************/
clock_time_t clock_time(void)
{
  return(Ticks);
}
