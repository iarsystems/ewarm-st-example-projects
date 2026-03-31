/******************** (C) COPYRIGHT 2003-2004 STMicroelectronics ********************
* File Name          : LPmode.c
* Author             : MCD APPLICATION TEAM
* Date First Issued  : DECEMBER 2004
* Description        : Power management routines
********************************************************************************
* History:
* 12/10/2004 : Created
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



/********************************************************************************
* Function Name  : SLOW
* Description    : configures the RCLK as CLK2 , CLK2/16 or RTC_clock , PLL1 off
* Input          : clock : CLK2, CLK2/16 or RTC_CLK
* Return         : None
********************************************************************************/

void SLOW(SlowClock_Typedef clock)
{
   switch (clock)
   {
     case CLK2: RCCU_RCLKSourceConfig(RCCU_CLOCK2); 		break; //select CLK2 as RCLK , PLL1 off
     case CLK2_16: RCCU_RCLKSourceConfig(RCCU_CLOCK2_16); 	break; //select CLK2_16 as RCLK ,  PLL1 off
     default:
     {
      RCCU_RCLKSourceConfig(RCCU_RTC_CLOCK);                   //change the system clock to RTC
      RCCU->PLL1CR|=0x7;                                       //switch off PLL1
      break;
     }
   }
}

/*******************************************************************************
* Function Name  : WFI
* Description    : enters the MCU in WFI mode
* Input          : None
* Return         : None
*******************************************************************************/

void WFI(void)
{
     RCCU->SMR &=0xFE ;          // reset bit WFI in the RCCU_SMR register
}

/*******************************************************************************
* Function Name  : LPWFI
* Description    : enters the MCU in Low Power WFI mode
* Input1         : clock in WFI mode: RTC_CLK or CLK2_16
* Input2         : DISABLE : stops the MVR during LPWFI mode
* Return         : None
*******************************************************************************/

void LPWFI(SlowClock_Typedef clock,FunctionalState VRstate)
{	
   FLASHR->CR0|=0x8000;          // disable the flash during the LPWFI
   if (clock == RTC_CLK)
   RCCU->CCR  |=0x2;             //select the 32KHz as peripherl clock during LPWFI
   else
   RCCU->CCR  &=~0x2;            //select CLK2/15 as peripherals clock during LPWFI
   RCCU->CCR  |=0x1;             //set bit LOPWFI for selecting LPWFI mode
   if (VRstate == DISABLE )
   {
    PCU->PWRCR  |=0x8000;        //disable MVR during WFI mode
    PCU->PWRCR  |=0x0010;
   }
    RCCU->SMR &=0xFE;            //enter the LPWFI mode
}

/*******************************************************************************
* Function Name  : STOP
* Description    : enters the MCU in STOP mode
* Input1         : DISABLE : puts the FLASH in power down during the STOP mode
* Input2         : DISABLE : stops the MVR during the STOP mode
* Return         : None
*******************************************************************************/

void STOP(FunctionalState Flashstate, FunctionalState VRstate)
{
   if (VRstate == DISABLE)
   {
    PCU->PWRCR |=0x8000;    	 // disable MVR during STOP mode
    PCU->PWRCR |=0x0010;
    }
   if (Flashstate == DISABLE)
    FLASHR->CR0 |=0x8000;        // stop the flash during stop mode
    XTI->CTRL |=0x03;            //set bit ID1St and WKUP-int bits in XTI_CTRL register
   do
   {
    XTI->CTRL &=0x03;        	 //Reset the STOP bit in XTI_CTRL register
    RCCU->CFR &=0xBFFF;          //Reset the STOP_I bit in the CLK_FLAG register
    // Write '1' to Stop Bit
    XTI->CTRL |= 0x04;
    // Write '0' to Stop Bit
    XTI->CTRL &= 0x03;
    // Write '1' to Stop Bit
    XTI->CTRL |= 0x04;

    asm(              // some dummy instructions after the STOP sequence
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP");

    }
   while(!(RCCU->CFR&0x4000)||(XTI->CTRL&0x4)==1);   // assure the STOP mode was really entered
                                                     // STOP_I =1  STOP =0
}

/*******************************************************************************
* Function Name  : STANDBY
* Description    : enters the MCU in STOP mode
* Input          : DISABLE : stops the LPVR during STANDBY mode
* Return         : None
*******************************************************************************/

void STANDBY(FunctionalState VRstate)
{	
   if (VRstate==DISABLE)
   {
    PCU->PWRCR |= 0x8000;       //disable the Low power voltage regulator
    PCU->PWRCR |= 0x0020;
   }
   PCU->PWRCR |= 0x8000;        // enter in Standby mode
   PCU->PWRCR |= 0x0040;
}


