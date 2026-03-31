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
 *    1. Date        : 28, July 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *   This example project shows how to use the IAR Embedded Workbench
 *  for ARM to develop code for the IAR-STM32F107VC-SK board. It shows basic
 *  use of parallel I/O, timer, interrupt controller, interface to
 *  a Nokia Graphic color LCD module and I2C temperature sensor.
 *   It show temperature and alarm pin state. The alarm activate when temperature is
 *  over 31.0 C and deactivate when it drop below 30.0 C.
 *
 *  Jumpers:
 *   PWR_SEL   - depending of power source
 *   JTAG_SEL  - open (on board j-link)
 *   B0_1/B0_0 - B0_0
 *   B1_1/B1_0 - B1_0
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US       450
#define LED1_MASK       GPIO_Pin_6
#define LEDS_PORT       GPIOC

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
GPIO_InitTypeDef GPIO_InitStructure;
Flo32 Temp, TempHold = 200.0;
Boolean Alarm;

  ENTR_CRT_SECTION();
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  // SysTick end of count event each 0.5s with input clock equal to 9MHz (HCLK/8, default)
  SysTick_Config(4500000);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  // I2C1 init
  I2C1_Init();

  // Status LED port init
  // GPIO enable clock and release Reset
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOC, DISABLE);

  GPIO_InitStructure.GPIO_Pin =  LED1_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LEDS_PORT, &GPIO_InitStructure);

  GPIO_WriteBit(LEDS_PORT,LED1_MASK,Bit_RESET);

  EXT_CRT_SECTION();

  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);
  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(10,104,131,131);

  // Init Temperature sensor
  if(   FALSE == TempSensor_Init()
     || FALSE == TempSensorShutdown(FALSE)
     || FALSE == TempSensor_Conf(31.0,30.0,TEMP_SENOSR_COMP_MODE,2))
  {
    // Initialization fault
    GLCD_TextSetPos(0,0);
    GLCD_print("\fSTCN75 Init.\r\nfault\r\n");
    while(1);
  }

  while(1)
  {
    if (SysTickFl)
    {
      SysTickFl = FALSE;
      // Get new sample of the temperature
      if(TempSensorGetTemp(&Temp,&Alarm))
      {
        if(Alarm)
        {
          // turn on Status LED1
          GPIO_WriteBit(LEDS_PORT,LED1_MASK,Bit_SET);
        }
        else
        {
          // turn off Status LED1
          GPIO_WriteBit(LEDS_PORT,LED1_MASK,Bit_RESET);
        }
        if(TempHold != Temp)
        {
          TempHold = Temp;
          GLCD_TextSetPos(0,0);
          GLCD_print("\fTemperature: %3.1fC",Temp);
        }
      }
      else
      {
        // Initialization fault
        GLCD_TextSetPos(0,0);
        GLCD_print("\fSTCN75 reading\r\nfault\r\n");
        while(1);
      }
    }
  }
}
