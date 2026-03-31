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
 *    1. Date        : 19, July 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F103ZE-SK board. It shows how
 * to make the buzzer make a sound. The Buzzer demo application is
 * downloaded to Embedded Flash memory on the evaluation board.
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US  450

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

#define BUZZER_MASK           GPIO_Pin_7
#define BUZZER_PORT           GPIOC

Int32U CriticalSecCntr;

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
GPIO_InitTypeDef GPIO_InitStructure;
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

  EXT_CRT_SECTION();

  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);
  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(10,104,131,131);

  // Buzzer init
  // GPIO enable clock and release Reset
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOC, DISABLE);
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  BUZZER_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

  while(1)
  {
    ButtCtrl = GetButtons();

    if (ButtCtrl.Tamper)
    {
      GLCD_Backlight(BACKLIGHT_OFF);
      // Map TIM3 OC2 to PC7
      GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
      TIM3->CCR2 = 80;
    }
    else
    {
      GLCD_Backlight(BACKLIGHT_ON);
      // Unmap TIM3 OC2 from PC7
      GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    }
  }
}
