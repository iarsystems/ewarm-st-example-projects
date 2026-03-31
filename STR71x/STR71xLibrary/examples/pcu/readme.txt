*** This README.TXT file comes with the STR71x Software Library ***

*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************

Example features
================
 This example highlights one of the features of the PCU which is to put
 the chip in stop mode and use the wakeup pin to resume program execution.

 This example demonstrates the following features of the PCU :

  - Initialization,
  - External wakeup line configuration,
  - Entering stop mode,
  - Exit from stop mode using a wakeup line.

Directory contents
==================
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program
 
Hardware implementation:
=======================
 This example works as standalone on the STR711/STR712 IAR boards and
 extra hardware implementation is not required to run the example.

How to use it
=============
 In order to make the program work, you must do the following :

   - Put the Jumper Dis/Led on the position 1-2(only for STR711 IAR Board) 
  -  run the program,
  - Once the program runs and the CPU switches into STOP mode, you can wake-up it by
    applying a falling edge on the P0.15 pin (or pressing the wakeup button)

(C) COPYRIGHT 2003 STMicroelectronics
