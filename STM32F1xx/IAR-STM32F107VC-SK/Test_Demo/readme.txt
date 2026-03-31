/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Tools Support Team
* Version            : V1.0.0
* Date               : 07/01/2009
* Description        : Description of IAR "STM32F107VCT6-SK Demonstration software.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED 
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*******************************************************************************/

Introduction
=============

This manual describes the demonstration software running on the IAR 
STM32F107VCT6-SK, which you can use to evaluate the capabilities of the microcontroller
and the on-board peripherals.

  
Demonstration Package contents
===============================

The demonstration software package contains the following subfolders:

- Test folder:
The Test folder contains STM32F107VCT6-SK peripheral tests that make up the core 
of the demonstration. These tests demonstrate some capabilities of the 
microcontroller.
  
- library folder
The STM32F10x_StdPeriph_Driver folder contains all the subdirectories and files that make 
up the core of the STM32F10xxx Firmware library:
  - inc subfolder contains the firmware library header files
  - src subfolder contains the firmware library source files
  
- Drivers folder
The drivers folder contains all  source and header files that provides a set of 
functions needed to drive some STM32F107VC6-SK components (LCD,stepper motor, Temperature sensor...)

- project folder
project folder contains project for the EWARM toolchain (V5.40) that 
compiles all the demonstration files.

Demonstration description 
==========================

1. After power ON(*), there would be the "STM32F107VCT6-SK" message and IAR and ST logos 
displayed on LCD.

2. After few seconds, The main Menu is displayed. You can switch between
General purpose peripheral tests or Communication peripheral tests using
Joystick key (J).

The selected item's color is blue. you can use Joystick key (J) to navigate on different 
menu and sub-menu items.

3. If you choose Gen. purpose Periph (pressing Joystick key (J)), you will enter
periph test sub-menu in which you can test STM32F107VCT6 IAR SK components:

    - Stepper Motor     : Showing how the motor can be driven (rotate).
    
    - Accelerometer     : Displaying the board position regardless of horizontal.
    
    - Potentiometer     : Changing the GLCD contrast by moving the potentiometer AN-TR.
    
    - Buzzer            : Generating Beep signal on the buzzer using 8KHz PWM signal .
    
    - Audio In/Out      : Recording 5 second of speech from the microphone and then play 
                          it through th headphone.
    
The continue... sub menu allows you to test remaining peripherals due to the 
GLCD line numbers limit. Choosing the Continue... allows you testing:
    
    - LED               : Turning on Leds (STAT1..STAT4) and switch off Leds one by one.
    
    - Temperature Sensor: Using I2C temperature sensor to show temperature. 
    
    - Key Button        : Pressing Tamper Button allow display of "Tamper key pressed"  
                          message display and LED (STAT1) is ON.      
    
3. If you choose Communication Periph (pressing Joystick key (J)), you can test:
    
    - USART1          : Using the USART1 to communicate with the hyperterminal with interactive
                        messages on LCD.  
    - USART2          : Using the USART2 to communicate with the hyperterminal with interactive
                        messages on LCD.    
    - CAN             : Performing a communication and data transfert between CAN1 an CAN2.  
    
    - Ethernet        : Testing the Auto-Negotiation and  Loop-back mode of the STE101P transceiver 
                        and the different baudrate and duplex mode of supported.              
                 
NOTE: 

* Refer to IAR documentations on Power Supply ways.
** After running tests you can press User key to exit, this allows you to return
to Periph tests sub menu.
*** Choosing Return allows go back to main Menu  

How to use it
============= 
 - Open project at project\EWARMv5\project.eww  
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Debug
 - Run program: Debug->Go(F5)
******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE******

