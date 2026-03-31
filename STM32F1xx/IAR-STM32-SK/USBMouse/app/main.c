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
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR STM32-SK board. It implements USB
 * HID mouse. When the host installs the needed driver, the mouse cursor
 * begins to move in a rectangle shape. The WAKE-UP button is used for
 * USB resume when the device is suspended.
 *
 *  Controls:
 *   WAKE-UP - USB resume, when device is suspended
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *
 *    $Revision: 1475 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US  450

volatile Boolean Update = TRUE;
Int32U CriticalSecCntr;

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
  Update = TRUE;
}

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
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef TIM1_TimeBaseInitStruct;
GPIO_InitTypeDef GPIO_InitStructure;

Int32U State, Count;

  ENTR_CRT_SECTION();

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

  // Init USB wakeup button (WAKE_UP (PA0))
  // Enable GPIO clock and release reset
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,
                         ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA,
                         DISABLE);

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  // Timer1 Init
  // Enable Timer1 clock and release reset
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1,DISABLE);

  // Set timer period 0.01 sec
  TIM1_TimeBaseInitStruct.TIM_Prescaler = 720;  // 10us resolution
  TIM1_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM1_TimeBaseInitStruct.TIM_Period = 1000;  // 10 ms
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

  // HID USB
  HidInit();
  // Soft connection enable
  USB_ConnectRes(TRUE);

  EXT_CRT_SECTION();

  // LCD Init
  HD44780_PowerUpInit();

  // Show message on the LCD
  HD44780_StrShow(1, 1,  "  IAR Systems   ");
  HD44780_StrShow(1, 2,  "   HID Class    ");
  LCD_LIGHT_ON();
  while(1)
  {
    if(Update)
    {
      Update = FALSE;
      if(UsbCoreReq(UsbCoreReqDevState) == UsbDevStatusConfigured &&
         !UsbCoreReq(UsbCoreReqDevSusState))
      {
        // Mouse pointer
        switch(State)
        {
        case 0:
          HidMouseSendReport(-2,0,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 1:
          HidMouseSendReport(0,2,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 2:
          HidMouseSendReport(2,0,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 3:
          HidMouseSendReport(0,-2,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        default:
          State = 0;
          Count = 100;
        }
      }
      else
      {
        // Reset state
        State = 0;
        Count = 100;
      }
      // USB Wakeup
      if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == Bit_SET)
      {
        UsbWakeUp();
      }
    }
  }
}
