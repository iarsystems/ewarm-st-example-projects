/******************** (C) COPYRIGHT 2003-2004 STMicroelectronics ***************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 12/10/2004
* Description        : STR71x STOP Mode test for the STR71x Power
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

void Test_STOPmode (void);      //enter in STOP mode

void delay(void)              //delay
{
   u32 i;
   for(i=0; i<0x2FFFF; i++);
}

void Test_STOPmode (void)
{
//MCU in STOP mode
//The Flash is in power down during this mode
//The MVR is stopped
//P1.10 used for stopping the oscillator (with tristate pin (STR712 only))
//The Wakeup is done by pin P0.15 rising edge configured as wake up line

   u32 i;
   XTI_Init();                                           //initialize the XTI
   XTI_LineModeConfig(XTI_Line15,  XTI_RisingEdge);       //Configure the line15 mode,select Rising edge trig
   XTI_LineConfig(XTI_Line15, ENABLE);                    //Enable line 8 Wake-Up interrupts
   XTI_ModeConfig(XTI_WakeUp, ENABLE);                   //Enable Wake-Up mode in the XTI

   //change the system clock to the RTC 32KHz Or the Free runnig mode clock

   //SLOW(RTC_CLK);                                      //change the system clock to RTC
   //change the system clock to the Free runnig mode with Freq =~ 1 MHz /8
   RCCU->CFR    &=~0x1;                                  //PLL not sys clock
   RCCU->PLL1CR |= 0x7;                                  //PLL off DX[2:0] = '111'
   RCCU->PLL1CR |= 0x30;                                 //MX[1:0] = '11'
   RCCU->PLL1CR &=~0x40;                                 //REF_RANGE = 0
   RCCU->PLL1CR |=0x80;                                  //FREEN =1
   RCCU->CFR    |=0x1;                                   //PLL as system clock
   PCU->PWRCR |= 0x8000;
   PCU->PWRCR  |=0x0080;                                 //32KHz could be disabled if RTC
                                                         //clock not used (Free runnig mode clock used)

   for(i=0; i<0x100; i++);                               //Delay before stopping the oscillator
   GPIO_BitWrite (GPIO1,10,0);                           //STOP the 16MHz oscillator using pin P1.10 (STR712 only)
   STOP(DISABLE,DISABLE);                                //enter in STOP mode ( flash disabled, MVR stopped)
   GPIO_BitWrite (GPIO1,10,1);                           //reenable the oscillator using pin P1.10 (STR712 only)
   for(i=0; i<0x100; i++);                               //delay
   RCCU_PLL1Config(RCCU_PLL1_Mul_16, RCCU_Div_4);             //restore the  PLL
   RCCU_RCLKSourceConfig(RCCU_PLL1_Output);              //Set the RCLK to the PLL1 output
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
    GPIO_BitWrite (GPIO1, 8, 1);                //Set P1.8
    delay();                                    //delay
    GPIO_BitWrite (GPIO1, 8, 0);                //Clear P1.8
    delay();                                    //delay
    counter--;
  }

  Test_STOPmode();                                //enter in STOP mode

  while(1)                                      //pin P1.8 toggling after exit from
                                                //STOP mode
  {
    GPIO_BitWrite (GPIO1, 8, 1);                //Set P1.8
    delay();                                    //delay
    GPIO_BitWrite (GPIO1, 8, 0);                //Clear P1.8
    delay();                                    //delay
  }

}


