/******************** (C) COPYRIGHT 2005 STMicroelectronics **********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 09/27/2005 :  V1.0
* Description        : Description of the GPIO example
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
This example guides the user to :
- configure GPIO0 as output push-pull
- write on the GPIO0 port

It is made of 3 steps :
1) GPIO0 configuration in output push-pull mode to use STR731 IAR board's leds

2) Drive leds connected to the GPIO0 pins:
    - Set led connected to PIN0
    - Reset led connected to PIN0
    - Set GPIO0 LSB
    - Set GPIO0 MSB
    - Reset all GPIO0 pins
    - Set all GPIO0 pins

3) Make a loop on all GPIO0 connected leds
   - Set  GPIO0 pins one by one in a sequence. 

Directory contents
==================
73x_conf.h  Library Configuration file
73x_it.c    Interrupt handlers
main.c      Main program

How to use it
=============
In order to make the program work, you must do the following :
- Run the example
- Check the toggling of GPIO0 pins.

******************* (C) COPYRIGHT 2005 STMicroelectronics *****END OF FILE****
