/******************** (C) COPYRIGHT 2005 STMicroelectronics **********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 09/27/2005 :  V1.0
* Description        : Description of the  TB example
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

This example guides the user to  generate an interrupt each 250ms using
the Timer Base peripheral :

- Setup of the MCLK (Main system Clock) to 32Mhz
- Configuration of the Timer base (TB2):
  + Selection of the internal Clock,
  + Setup of the prescaler & the preload values to have a period of 250ms,
  + Enable of the TB interrupt.

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
