/******************** (C) COPYRIGHT 2003-2004 STMicroelectronics ***************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 12/10/2004
* Description        : STR71x Low Power Wait For Interrupt Mode test for the STR71x Power
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


void Test_LPWFImode (void);      //enter in Low power WFI mode


void delay(void)              //delay
{
   u32 i;
   for(i=0; i<0x2FFFF; i++);
}

void Test_LPWFImode (void)
{
/* MCU in Low power Wait For Interrupt mode
  The selected clock for this mode is the RTC 32KHz
  The MVR is stopped during entering the LPWFI mode
  The Flash is in power down
  The Wakeup is done by the RTC Alarm (after a delay of 10s)
  or WAKEUP pin P0.15 */

   u32 i;

   /* Configure wake up by RTC */
   RTC_PrescalerConfig (0x0008000);                      // Configure RTC prescaler for 1 second increments
   RTC_FlagClear (RTC_AIR);                              // Clear pending interrupts flags
   RTC_FlagClear (RTC_GIR);
   RTC_ITConfig(RTC_GIT, ENABLE);                        // Enable Global interrupt
   RTC_ITConfig(RTC_AIT, ENABLE);                        // Enable Alarm interrupt
   RTC_CounterConfig(0);
   RTC_AlarmConfig(10);                                  //alarm after 10 s              	        					

   /* Configure external interrupt */
   XTI_Init();                                           //initialize the XTI
   XTI_LineModeConfig(XTI_Line15,  XTI_RisingEdge);      //Configure the line 15 mode ,select Rising edge trig
   XTI_LineConfig(XTI_Line15, ENABLE);                   //Enable line 15 WAKEUP pin or RTC Alarm
   XTI_ModeConfig(XTI_WakeUpInterrupt, ENABLE);          //Enable Wake-Up mode in the XTI

   /* Configure interrupt controler */
   EIC_IRQChannelPriorityConfig(RTC_IRQChannel, 2);      // set the interrupt priority for RTC
   EIC_IRQChannelConfig(RTC_IRQChannel, ENABLE);         // Enable RTC IRQ channel
   EIC_IRQChannelPriorityConfig(XTI_IRQChannel,1);       // set the interrupt priority for external interrupt
   EIC_IRQChannelConfig(XTI_IRQChannel,ENABLE);          // Enable external interrupt.
   EIC_IRQConfig(ENABLE);                                // Enable the IRQ

   SLOW(RTC_CLK);                                        // Enter Slow mode with 32 KHz as sys clock
   for(i=0; i<0x100; i++);                               // delay before stopping the 16 MHz osc
   LPWFI(RTC_CLK,DISABLE);                               // Enter in LPWFI ( LPWFI clock = 32KHz RTC Clock, MVR disabled)
   for(i=0; i<0x100; i++);                               // delay before restoring the CLK clock as system clock
   RCCU_PLL1Config(RCCU_PLL1_Mul_16, RCCU_Div_4);        // restore the  PLL
   RCCU_RCLKSourceConfig(RCCU_PLL1_Output);              // Set the RCLK to the PLL1 output
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

  while(counter)                                //pin P1.8 toggling before entering low power mode
  {
    GPIO_BitWrite (GPIO1, 8, 0);                //Set P1.8
    delay();                                    //delay
    GPIO_BitWrite (GPIO1, 8, 1);                //Clear P1.8
    delay();                                    //delay
    counter--;
  }

  Test_LPWFImode();                                //enter in Low power WFI mode

  while(1)                                      //pin P1.8 toggling after exit from
                                                //LPWFI mode
  {
    GPIO_BitWrite (GPIO1, 8, 0);                //Set P1.8
    delay();                                    //delay
    GPIO_BitWrite (GPIO1, 8, 1);                //Clear P1.8
    delay();                                    //delay
  }

}


