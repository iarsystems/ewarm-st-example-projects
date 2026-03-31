/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2014
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : October, 2014
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F429II-ACA board. It shows basic
 * use of the parallel I/O, EXTI module, the timers and the interrupt controller.
 *  It lights up LED1 and LED3, according the state of the capacitive buttons.
 * If you touch the CAP_BUT2, the LED1 lights up, and if you touch CAP_BUT1,
 * the LED3 lights up. The example measures the time needed for charging each
 * capacitive button. If a button is touched the capacitance changes, so 
 * the charging time also changes.
 *
 * COMPATIBILITY
 * =============
 *  The Cap_Buttons example project is compatible with IAR-STM32F429II-ACA
 * evaluation board.
 *  By default, the project is configured to use the I-Jet JTAG/SWD interface.
 *
 * CONFIGURATION
 * =============
 * Flash Debug - The progam is loaded to the internal flash.
 *
 *  Make sure that the following jumpers are correctly configured on the
 * IAR-STM32F429II-ACA evaluation board:
 *
 *  Jumpers:
 *   B0_1/B0_0  - B0_0
 *   B1_1/B1_0  - B1_0
 *   LEDS_PWR_E - closed
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define CAP_BUT1_BIT    (1 << CAP_BUT1)
#define CAP_BUT2_BIT    (1 << CAP_BUT2)

/* cap buttons relative threshold value (lower - increase sensitivity) */
#define CAP_BUT_THRESHOLD 20

/* variable for critical section entry control */
uint32_t CriticalSecCntr;

/* timer capture variables */
volatile uint16_t timer1_int, timer2_int;

/*************************************************************************
 * Function Name: CapButtons_InterruptHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPIO interrupt handler.
 *
 *************************************************************************/
void CapButtons_InterruptHandler(void)
{
  /* If CAP_BUT1 becomes high state, capture timer */
  if(EXTI_GetFlagStatus(CAP_BUT1_EXTI_LINE) == SET)
  {
    timer1_int = TIM_GetCounter(TIM3);
    EXTI_ClearITPendingBit(CAP_BUT1_EXTI_LINE);
  }
  /* If CAP_BUT2 becomes high state, capture timer */
  if(EXTI_GetFlagStatus(CAP_BUT2_EXTI_LINE) == SET)
  {
    timer2_int = TIM_GetCounter(TIM3);
    EXTI_ClearITPendingBit(CAP_BUT2_EXTI_LINE);
  }
  __DSB();
}

/*************************************************************************
 * Function Name: CapButtons_TimerSetup
 * Parameters: none
 *
 * Return: void
 *
 * Description: Prepares timer for use with capacitive sensor buttons.
 *
 *************************************************************************/
void CapButtons_TimerSetup(void)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

  /* Initialize capacitive buttons charging timer (TIM3) */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  TIM_DeInit(TIM3);
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInitStruct.TIM_Period = 0xFFFFFFFF;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
  TIM_Cmd(TIM3, ENABLE);
}

/*************************************************************************
 * Function Name: CapButtons_LoopHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Capacitive buttons reader.
 *
 *************************************************************************/
void CapButtons_LoopHandler(uint32_t * cap_buttons)
{
static CapButtonsState_TypeDef  state;
static uint16_t timer_hold;
static uint16_t cap_but1_zero = 0x7FFF, cap_but2_zero = 0x7FFF;
uint16_t cap_but1_value, cap_but2_value;

  switch(state)
  {
    /* Discharges the first capacitive button */
    case stCapBut1Discharge:
      STM_CapBut_Discharge(CAP_BUT1);
      state = stCapBut1PrepareAndStart;
      break;

    /* Configures GPIOs needed for measurement, and start charging */
    case stCapBut1PrepareAndStart:
      timer1_int = 0;
      STM_CapBut_Prepare(CAP_BUT1);
      timer_hold = TIM_GetCounter(TIM3);
      STM_CapBut_Charge(CAP_BUT1);
      state = stCapBut1Charging;
      break;

    /* Charging state, looks for charging completion */
    case stCapBut1Charging:
      /* if charging completes the timer1_int will be set in interrupt handler with value > 0 */
      if(timer1_int)
      {
        /* Calculates actual timer cycles needed for charging */
        cap_but1_value = timer1_int - timer_hold;
        if( cap_but1_value > ( cap_but1_zero + ( cap_but1_zero * CAP_BUT_THRESHOLD / 100 ) ) )
        {
          *cap_buttons |= CAP_BUT1_BIT;
        }
        else
        {
          *cap_buttons &= ~CAP_BUT1_BIT;
        }
        /* Updates calibration value if needed, assumes that not touched button have lower charging time */
        if( cap_but1_zero > cap_but1_value ) cap_but1_zero = cap_but1_value;
        state = stCapBut2Discharge;
      }
      break;

    /* Discharges the second capacitive button */
    case stCapBut2Discharge:
      STM_CapBut_Discharge(CAP_BUT2);
      state = stCapBut2PrepareAndStart;
      break;

    /* Configures GPIOs needed for measurement, and start charging */
    case stCapBut2PrepareAndStart:
      timer2_int = 0;
      STM_CapBut_Prepare(CAP_BUT2);
      timer_hold = TIM_GetCounter(TIM3);
      STM_CapBut_Charge(CAP_BUT2);
      state = stCapBut2Charging;
      break;

    /* Charging state, looks for charging completion */
    case stCapBut2Charging:
      if(timer2_int)
      {
        /* Calculates actual timer cycles needed for charging */
        cap_but2_value = timer2_int - timer_hold;
        if( cap_but2_value > ( cap_but2_zero + ( cap_but2_zero * CAP_BUT_THRESHOLD / 100 ) ) )
        {
          *cap_buttons |= CAP_BUT2_BIT;
        }
        else
        {
          *cap_buttons &= ~CAP_BUT2_BIT;
        }
        /* Updates calibration value if needed, assumes that not touched button have lower charging time */
        if( cap_but2_zero > cap_but2_value ) cap_but2_zero = cap_but2_value;
        state = stCapBut1Discharge;
      }
      break;
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
int main(void)
{
uint32_t cap_buttons;

  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  ENTR_CRT_SECTION();

  /* Init timer needed for capacitive buttons operations */
  CapButtons_TimerSetup();

  /* Init each capacitive button GPIO and EXTI */
  STM_CapBut_Setup(CAP_BUT1);
  STM_CapBut_Setup(CAP_BUT2);

  EXT_CRT_SECTION();

  /* Init + turn off all LEDs */
  for(Led_TypeDef i = LED1; i <= LED8; i++ )
  {
    STM_LEDInit( i );
    STM_LEDOff( i );
  }

  /* Main loop */
  while(1)
  {
    /* Updates the capacitive buttons reading state, and cap_buttons value */
    CapButtons_LoopHandler(&cap_buttons);

    if (cap_buttons & CAP_BUT1_BIT)
     STM_LEDOn(LED3);
    else
     STM_LEDOff(LED3);

    if (cap_buttons & CAP_BUT2_BIT)
     STM_LEDOn(LED1);
    else
     STM_LEDOff(LED1);
  }
}
