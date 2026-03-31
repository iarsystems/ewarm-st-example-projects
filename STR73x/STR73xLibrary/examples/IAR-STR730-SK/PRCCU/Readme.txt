/******************** (C) COPYRIGHT 2005 STMicroelectronics **********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 09/27/2005 :  V1.0
* Description        : Description of the PRCCU example
**********************************************************************************
* History:
* 09/27/2005 :  V1.0
*********************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************

Example description
===================
 This example shows how to use the PRCCU peripheral in order to configure the
 system clocks and to enter/exit from LPWFI (Low Power Wait For Interrupt) mode.

It is made of 5 steps :
1) GPIO configuration
   - All GPIO0 pins as Output push-pull (Leds)
   - GPIO2: Pins P2.12 and P2.13 as Input Tri-TTL
   
2) EIC configuration
   - configure EXTIT14 and EXTIT15 channels priority, 
   - Configure External IT Trigger for EXTIT14 and EXTIT15 channels,
   - Enable EXTIT14 and EXTIT15 interrupts,
   - Enable Interrupts.
   
3) CMU Clock input selection
         + Select the Main oscillator=4MHz as CMU input clock
	 
4) PRCCU MCLK configuration 
         + Configure the DIV2, PLL multiplier, PLLdivider, Free running mode for PLL (MCLK=12MHz)

5) Start GPIO0 toggling  with a MCLK frequency = 12MHz
  - To enter in LPWFI mode press B3 button
  - The system enters in LPWFI mode 
         + PLL is stopped and CK2_16 divider is enabled: MCLK=2MHz/16=125KHz
	 + CPU is stopped while all enabled peripherals still running under MCLK=125KHz
  - To exit LPWFI mode press B4 button
         + PLL still stopped and CK2_16 divider disabled: MCLK=CLOCK2=2MHz
         + In the Interrupt handler the MCLK frequency is again reconfigured to 12MHz as in step (4)

  - To re-enter in LPWFI mode press B3 button (The same procedure could be done)	 

Note : This example is running on STR730 IAR  board and uses "B3" and "B4" push-buttons
       which are connected respectively to pins GPIO2.12/INT14 and GPIO2.13/INT15.
	 
Directory contents
==================
73x_conf.h  Library Configuration file
73x_it.c    Interrupt handlers
main.c      Main program

How to use it
=============
In order to make the program work, you must do the following :

- Run the example


******************* (C) COPYRIGHT 2005 STMicroelectronics *****END OF FILE****
