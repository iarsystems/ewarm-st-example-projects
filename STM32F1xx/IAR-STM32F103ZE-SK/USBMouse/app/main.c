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
 *    1. Date        : 29, July 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *   This example project shows how to use the IAR Embedded Workbench
 *  for ARM to develop code for the IAR-STM32F103ZE-SK board. It implements USB
 *  HID mouse. The any button will resume USB if device is suspended.
 *   The joystick control up, down, left, right and left mouse button, USER
 *  is the left mouse button too and WKUP is the right mouse button.
 *  Controls:
 *   Joystick    - left, right, up, down movement, center left button
 *   USER        - left button
 *   WKUP        - right button
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US  450
#define MOUSE_DELTA     4

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

Int32U CriticalSecCntr;
volatile Boolean SysTickFl;

/*************************************************************************
 * Function Name: TickHandler
 * Parameters: void
 * Return: void
 *
 * Description:
 *		
 *************************************************************************/
void TickHandler(void)
{
  SysTickFl = TRUE;
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
Int8S X = 0, Y = 0;
Int8U Buttons = 0, ButtonsHold = 0;
Buttons_t ButtCtrl;

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

  // SysTick end of count event each 0.01s with input clock equal to 9MHz (HCLK/8, default)
  SysTick_Config(90000);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  // HID USB
  HidInit();
  EXT_CRT_SECTION();

  // Soft connection enable
  USB_ConnectRes(TRUE);

  // Init Buttons
  ButtonsInit();
  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);

  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(20,116,131,131);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fHID Class Mouse\r");

  while(1)
  {
    if (SysTickFl)
    {
      SysTickFl = FALSE;
      if(UsbCoreReq(UsbCoreReqDevState) == UsbDevStatusConfigured)
      {
        // Buttons and Joystick processing
        ButtCtrl = GetButtons();
        if(ButtCtrl.JsUp)
        {
          Y -= MOUSE_DELTA;
        }
        else if(ButtCtrl.JsDown)
        {
          Y += MOUSE_DELTA;
        }

        if(ButtCtrl.JsRight)
        {
          X += MOUSE_DELTA;
        }
        else if(ButtCtrl.JsLeft)
        {
          X -= MOUSE_DELTA;
        }

        if(ButtCtrl.JsCenter || ButtCtrl.B1)
        {
          Buttons |= 1;
        }
        else
        {
          Buttons &= ~1;
        }

        if(ButtCtrl.B2)
        {
          Buttons |= 2;
        }
        else
        {
          Buttons &=~2;
        }

        if(Y || X  || Buttons || (ButtonsHold ^ Buttons))
        {
          UsbWakeUp();  // wake-up
          // Send report
          HidMouseSendReport(X,Y,Buttons);
          ButtonsHold = Buttons;
          Y = X = Buttons = 0;
        }
      }
    }
  }
}
