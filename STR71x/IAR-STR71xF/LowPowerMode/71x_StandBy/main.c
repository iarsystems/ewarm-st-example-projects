/******************** (C) COPYRIGHT 2003-2004 STMicroelectronics ***************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 12/10/2004
* Description        : STR71x Low Power STANDBY Mode test for the STR71x Power
*
********************************************************************************
* History:
* 24/05/05 : V3.0
* 30/11/04 : V2.0
* 16/05/03 : Created
********************************************************************************
*THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A
RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH
SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN
IN CONNECTION WITH THEIR PRODUCTS.
********************************************************************************/



#include "71x_lib.h"
#include "lpmode.h"

void Test_STANDBYmode (void);      //enter in STANDBY mode


void delay(u32 value)              //delay
{
   u32 i;
   for(i=0; i<value; i++);
}

void Test_STANDBYmode (void)
{
 //MCU in STANDBY mode
 //The wake up is done by the RTC alarm interrupt (after 10s)
 //or by the Wakeup pin


   //Configure the RTC alarm to wakeup the MCU after 10s
   RTC_PrescalerConfig (0x0008000);                      //Configure RTC prescaler for 1 second increments
   RTC_FlagClear (RTC_AIR);                              //Clear pending interrupts flags
   RTC_FlagClear (RTC_GIR);
   RTC_ITConfig(RTC_GIT, ENABLE);                        //Enable Global interrupt
   RTC_ITConfig(RTC_AIT, ENABLE);                        //Enable Alarm interrupt
   RTC_CounterConfig(0);
   RTC_AlarmConfig(2);                                  //Wakeup after 10s : wakeup = MCU RESET

   STANDBY(ENABLE);                                      //Enter Standby (LPVR Enabled)

}


u32 counter;

int main(void)
{

  #ifdef DEBUG
  debug();
  #endif

  counter = 10;

  /* Set system, peripheral clocks */
  RCCU_Div2Config(ENABLE);                     // CK/2 Enabled
  RCCU_PLL1Config(RCCU_PLL1_Mul_16, RCCU_Div_4);    // Configure PLL with mult factor = 16, div factor = 4 (x4)
  RCCU_FCLKConfig(RCCU_DEFAULT);               // Set the APB1 clock to default (/1)
  RCCU_PCLKConfig(RCCU_DEFAULT);               // Set the APB2 clock to default (/1)
  RCCU_RCLKSourceConfig(RCCU_PLL1_Output);     // Set the RCLK to the PLL output
  PCU->PLL2CR|=0x0007;                         // stop the PLL2 (DX1,DX2,DX0 =1)
  RCCU->PER &=~0x001F;                         // stop unused periph clocks in PRCCU_PER register

  /* GPIO configuration & initialization*/
  GPIO_WordWrite (GPIO0,0x0);
  GPIO_WordWrite (GPIO1,0x0);
  GPIO_WordWrite (GPIO2,0x0);
  GPIO_Config (GPIO0 ,0xFFFF, GPIO_OUT_PP);    //configure all GPIO as pushpull output
  GPIO_Config (GPIO1, 0xFFFF, GPIO_OUT_PP);
  GPIO_Config (GPIO2, 0xFFFF, GPIO_OUT_PP);

  while(counter)                                //pin P1.8 toggling before entering STANDBY mode
  {
    GPIO_BitWrite (GPIO1, 8, 1);                //Set P1.8
    delay(0x2FFFF);                             //delay
    GPIO_BitWrite (GPIO1, 8, 0);                //Clear P1.8
    delay(0x2FFFF);                             //delay
    counter--;
  }

  Test_STANDBYmode();                                //enter in STANDBY mode

  while(1)                                      //This part of code should not be reached.
                                                //There is a RESET after STANDBY mode.
  {
    GPIO_BitWrite (GPIO1, 8, 1);                //Set P1.8
    delay(0x17FFF);                             //Smaller delay
    GPIO_BitWrite (GPIO1, 8, 0);                //Clear P1.8
    delay(0x17FFF);                             //Smaller delay
  }

}


