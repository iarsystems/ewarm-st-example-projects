*** This README.TXT file comes with the STR71x Software Library ***

*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************

Example description
===================
This example demonstrates the following features of the GPIO :
- configuration of GPIO1 (P1.8) as output push-pull,
- read/write access to the I/O ports

Hardware implementation:
=======================
 This example works as standalone on the STR711/STR712 IAR boards and
 extra hardware implementation is not required to run the example.
 
 Directory contents
==================
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program

How to use it
=============
In order to make the program work, you have to do the following :
 
 - Put the Jumper Dis/Led on the position 1-2(only for STR711 IAR Board)
 - Run program

Check the toggling of GPIO1 P1.8 pin.

(C) COPYRIGHT 2003 STMicroelectronics
