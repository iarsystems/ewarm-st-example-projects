********************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
********************************************************************************

Description
===========

This software is provided for testing the various STR71x low power modes.

*Low Power Wait For Interrupt (LPWFI) Mode
*STOP mode
*STANDBY mode


How to use it
=============

LPWFI mode:
LED stops blinking during LPWFI mode. Push the wake-up button to go out of LPWFI
mode, or you can wait for 10 seconds for the RTC alarm to wake up the STR71xF 
and program continues.

STOP mode:
LED stops blinking during STOP mode. Push the wake-up button to go out of STOP 
mode and program continues.

STANDBY mode:
LED stops blinking during STANDBY mode. Push the wake-up button to go out of 
STANDBY mode, or you can wait for 10 seconds for the RTC alarm to wake up the 
STR71xF and the program resets.


Jumper setting
==============

Do not put the STANDBY jumper.
Put the jumper DIS/LED on the position 1-2 (LED) for STR711 demo board.

(C) COPYRIGHT 2003 STMicroelectronics

