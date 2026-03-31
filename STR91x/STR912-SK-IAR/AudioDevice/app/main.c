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
 *    1. Date        : February 28, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 * Jumpers:
 *  POWER_SELECT  - depend of power source
 *  SEL_AUDIO_OUT - 1-2 Audio jack / 2-3 Built in speaker
 *  VREF_3.3_E    - fitted
 *  USB_DIS       - 2-3
 *
 * The example implements USB audio device with one output and one input.
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

volatile Int32U DlyCount;
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
  if(DlyCount)
  {
    --DlyCount;
  }
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
 * Function Name: DelayResolution100us
 * Parameters: Int32U Delay
 *
 * Return: none
 *
 * Description: Delay 100us * arg
 *
 *************************************************************************/
void DelayResolution100us(Int32U Delay)
{
  DlyCount = Delay;

  // Clear TIM0 counter
  TIM_CounterCmd(TIM0, TIM_CLEAR);
  // Clear TIM0 flag OC1
  TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
  // Enable TIM0 OC1 interrupt
  TIM_ITConfig(TIM0, TIM_IT_OC1, ENABLE);
  // Enable TIM0 counter
  TIM_CounterCmd(TIM0, TIM_START);

  while(DlyCount);

  // Disable TIM0 OC1 interrupt
  TIM_ITConfig(TIM0, TIM_IT_OC1, DISABLE);
  // Disable TIM0 counter
  TIM_CounterCmd(TIM0, TIM_STOP);
}

/*************************************************************************
 * Function Name: InitDlyTimer
 * Parameters: Int32U IntrPriority
 *
 * Return: none
 *
 * Description: Init Delay Timer (TIM 0)
 *
 *************************************************************************/
void InitDlyTimer (Int32U IntrPriority)
{
TIM_InitTypeDef TIM_InitStructure;

  // Enable TIM0 clocks
  SCU_APBPeriphClockConfig(__TIM01, ENABLE);
  // Release TIM0 reset
  SCU_APBPeriphReset(__TIM01,DISABLE);

  // Timer 0
  // TIM Configuration in Output Compare Timing Mode period 100us
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB; // assign PCLK to TIM_Clk
  TIM_InitStructure.TIM_Prescaler = 48 - 1;         // 1us resolution
  TIM_InitStructure.TIM_Pulse_Length_1 = 100;       // 100 us period
  TIM_Init(TIM0, &TIM_InitStructure);

  // VIC configuration
  VIC_Config(TIM0_ITLine, VIC_IRQ, IntrPriority);
  VIC_ITCmd(TIM0_ITLine, ENABLE);
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
void main()
{
  InitClock();
  // Enable the clock for the VIC
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);
  // VIC Deinitialization
  VIC_DeInit();

  // Delay timer init
  InitDlyTimer(2);

  // Initialize USB
  // Init USB
  USB_Init(3,4,UsbClassAudioConfigure);
  // Init Audio
  AudioClassInit(1);

  // Enable the Interrupt controller to manage IRQ channel
  __enable_interrupt();

  // Soft connection enable
  USB_ConnectRes(TRUE);

  // LCD Init
  HD44780_PowerUpInit();
  // Show messages on LCD
  HD44780_StrShow(1, 1,  " IAR Systems    ");
  HD44780_StrShow(1, 2,  "Audio Class Dev ");

  while(1)
  {
    if((UsbCoreReq(UsbCoreReqDevState) == UsbDevStatusConfigured) &&
       !UsbCoreReq(UsbCoreReqDevSusState))
    {
      LCD_LIGHT_ON();
    }
    else
    {
      LCD_LIGHT_OFF();
    }
  }
}
