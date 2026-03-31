/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : October 4, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the STR910 Evaluation board (MB460). It implements USB HID mouse.
 *  The joystick control up, down, left, right and left mouse button, and key is
 * the right mouse button.
 *  The setting of the jumpers is regarding STR910 Evaluation board user manual.
 *
 *
 *    $Revision: 39 $
 *
 **************************************************************************/
#include "includes.h"
#include <intrinsics.h>

#define TICK_PER_SEC      (50UL)

volatile Boolean TickSysFlag;
/*************************************************************************
 * Function Name: Tim0Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void Tim0Handler (void)
{
  // Clear TIM0 counter
  TIM_CounterCmd(TIM0, TIM_CLEAR);
  TickSysFlag = TRUE;
  // Clear TIM0 flag OC1
  TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init MCU clock
 *
 *************************************************************************/
void InitClock (void)
{
  // Clock
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);       // master clk - OSC clk
  // Flash controller init
  SCU_FMICLKDivisorConfig(SCU_FMICLK_Div1);
  FMI_Config(FMI_READ_WAIT_STATE_2,FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE,\
             FMI_LVD_ENABLE,FMI_FREQ_HIGH);
  // Set clks dividers
  SCU_RCLKDivisorConfig(SCU_RCLK_Div1);
  SCU_HCLKDivisorConfig(SCU_HCLK_Div1);
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1);
  // Init PLL = 48 MHz
  SCU_PLLFactorsConfig(192,25,3);
  // PLL Enabled
  SCU_PLLCmd(ENABLE);
  // Switch clk MCLK = PLL
  SCU_MCLKSourceConfig(SCU_MCLK_PLL);
}

/*************************************************************************
 * Function Name: InitSysTimer
 * Parameters: Int32U Priority
 *
 * Return: none
 *
 * Description: Init System Timer (TIM 0)
 *
 *************************************************************************/
void InitSysTimer (Int32U Priority)
{
TIM_InitTypeDef TIM_InitStructure;

  // Enable TIM0 clocks
  SCU_APBPeriphClockConfig(__TIM01, ENABLE);
  // Release TIM0 reset
  SCU_APBPeriphReset(__TIM01,DISABLE);

  // Timer 0
  // TIM Configuration in Output Compare Timing Mode period 200 msec
  SCU_TIMPresConfig(SCU_TIM01,((48000-1)&~1UL));    // Timer resolution 1ms
  SCU_TIMCLKSourceConfig(SCU_TIM01,SCU_TIMCLK_INT); // Timer01 Clock form the prescaler
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_SCU; // assign Prescaler clk to TIM_Clk
  TIM_InitStructure.TIM_Pulse_Length_1 = \
                               1000/TICK_PER_SEC;   // 20 ms period
  TIM_Init(TIM0, &TIM_InitStructure);
  TIM_CounterCmd(TIM0, TIM_CLEAR);                  // clear the timer 1 counter

  // VIC configuration
  VIC_Config(TIM0_ITLine, VIC_IRQ, Priority);
  VIC_ITCmd(TIM0_ITLine, ENABLE);

  // Clear TIM0 flag OC1
  TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
  // Enable TIM0 OC1 interrupt
  TIM_ITConfig(TIM0, TIM_IT_OC1, ENABLE);
  // Enable TIM0 counter
  TIM_CounterCmd(TIM0, TIM_START);
}


/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
int  main (void)
{
Int8S X = 0, Y = 0;
Int8U Buttons = 0, ButtonsHold = 0, ButtHold;

  InitClock();
  // Enable the clock for the VIC
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);

  // VIC Deinitialization
  VIC_DeInit();

  // SysTimer init
  InitSysTimer(1);

  // Buttons Init
  ButtonsInit();

  // Initialize USB
  USB_Init(3,2,UsbClassHidConfigure);
  // HID Init
  HidInit();

  // Enable the Interrupt controller to manage IRQ channel
  __enable_interrupt();

  while(1)
  {
    // Wait Timer Interrupt
    if(TickSysFlag)
    {
      TickSysFlag = FALSE;
      if(UsbCoreReq(UsbCoreReqDevState) == UsbDevStatusConfigured)
      {
        // Get Buttons and Joystick state
        ButtHold = GetButtons();
        if (ButtHold & JS_SELECT)
        {
          Buttons |= 0x1;
        }

        if (ButtHold & KEY_PB3)
        {
          Buttons |= 0x2;
        }

        if(ButtHold & JS_DOWN)
        {
          Y = +5;
        }

        if(ButtHold & JS_LEFT)
        {
          X = -5;
        }

        if(ButtHold & JS_RIGHT)
        {
          X = +5;
        }
        if(ButtHold & JS_UP)
        {
          Y = -5;
        }
      }
      /* Send new offsets and Buttons */
      if(Y || X  || Buttons || (ButtonsHold ^ Buttons))
      {
        UsbWakeUp();  // wake-up
        if(HidSendReport(Buttons,X,Y))
        {
          ButtonsHold = Buttons;
          Y = X = Buttons = 0;
        }
      }
    }
  }
}
