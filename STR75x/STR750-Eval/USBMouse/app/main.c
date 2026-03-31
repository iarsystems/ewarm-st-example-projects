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
 *    1. Date        : August 10, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the STR750 Evaluation board (MB469). It implements USB HID mouse.
 *  The joystick control up, down, left, right and left mouse button, and key is
 * the right mouse button.
 *  The setting of the jumpers is regarding STR750 Evaluation board user manual.
 *
 *
 *    $Revision: 39 $
 *
 **************************************************************************/
#include    "includes.h"

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
  TickSysFlag = TRUE;
  // Clear the Update pending Bit
  TIM_ClearITPendingBit(TIM0, TIM_IT_Update);
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
  // MRCC system reset(for debug purpose)
  MRCC_DeInit();
  // Wait for OSC4M start-up
  MRCC_WaitForOSC4MStartUp();
  // Set HCLK
  MRCC_HCLKConfig(MRCC_CKSYS_Div2);
  // Set CKTIM
  MRCC_CKTIMConfig(MRCC_HCLK_Div1);
  // Set PCLK
  MRCC_PCLKConfig(MRCC_CKTIM_Div2);
  // Set CKSYS
  MRCC_CKSYSConfig(MRCC_CKSYS_OSC4MPLL, MRCC_PLL_Mul_16);
  // GPIO pins optimized for 3V3 operation
  MRCC_IOVoltageRangeConfig(MRCC_IOVoltageRange_3V3);
}

/*************************************************************************
 * Function Name: InitSysTimer
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init System Timer (TIM 0)
 *
 *************************************************************************/
void InitSysTimer (void)
{
MRCC_ClocksTypeDef MRCC_Clocks;
TIM_InitTypeDef TIM_InitStructure;
EIC_IRQInitTypeDef EIC_IRQInitStructure;

  // Enable TIM0 clocks
  MRCC_PeripheralClockConfig(MRCC_Peripheral_TIM0, ENABLE);
  // Release TIM0 reset
  MRCC_PeripheralSWResetConfig(MRCC_Peripheral_TIM0,DISABLE);

  // TIM Configuration in Output Compare Timing Mode
  // Calculate counter value
  MRCC_GetClocksStatus(&MRCC_Clocks);

  TIM_InitStructure.TIM_Period = (MRCC_Clocks.CKTIM_Frequency / (277*TICK_PER_SEC));
  TIM_InitStructure.TIM_Prescaler = 277-1;
  TIM_InitStructure.TIM_Mode = TIM_Mode_OCTiming;
  TIM_InitStructure.TIM_ClockSource = TIM_ClockSource_Internal;
  TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_InitStructure.TIM_Channel = TIM_Channel_2;
  TIM_Init(TIM0, &TIM_InitStructure);

  // Enable and configure the priority of the TIM0 Update IRQ Channel
  EIC_IRQInitStructure.EIC_IRQChannel = TIM0_UP_IRQChannel;
  EIC_IRQInitStructure.EIC_IRQChannelPriority = 1;
  EIC_IRQInitStructure.EIC_IRQChannelCmd = ENABLE;
  EIC_IRQInit(&EIC_IRQInitStructure);

  // Clear TIM0 flags
  TIM_ClearFlag(TIM0, TIM_FLAG_OC1| TIM_FLAG_OC2| TIM_FLAG_Update);
  // Enable TIM0 Update interrupt
  TIM_ITConfig(TIM0, TIM_IT_Update, ENABLE);
  // Enable TIM0 counter
  TIM_Cmd(TIM0, ENABLE);
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
int main (void)
{
Int8S X = 0, Y = 0;
Int8U Buttons = 0, ButtonsHold = 0, ButtHold;

  // Set Burst mode
  CFG_FLASHBurstConfig(CFG_FLASHBurst_Enable);

  // MCU clock init
  InitClock();

  // EIC init
  EIC_DeInit();

  // SysTimer init
  InitSysTimer();

  // Buttons Init
  ButtonsInit();

  // HID USB
  HidInit();
  // Enable the Interrupt controller to manage IRQ channel
  EIC_IRQCmd(ENABLE);
  __enable_interrupt();

  // Soft connection enable
  USB_ConnectRes(TRUE);

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
        HidMouseSendReport(X,Y,Buttons);
        ButtonsHold = Buttons;
        Y = X = Buttons = 0;
      }
    }
  }
}
