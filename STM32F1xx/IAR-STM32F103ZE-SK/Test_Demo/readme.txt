/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : Description of IAR STM32F103ZE-SK Demonstration software.
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
STM32F103ZE-SK, which you can use to evaluate the capabilities of the microcontroller
and the on-board peripherals.

  
Demonstration Package contents
===============================

The demonstration software package contains the following subfolders:

- Test folder:
The Test folder contains STM32F103ZE-SK peripheral tests that make up the core 
of the demonstration. These tests demonstrate some capabilities of the 
microcontroller.
  
- library folder
The FWlib folder contains all the subdirectories and files that make up the core of the
STM32F10xxx Firmware library:
  - inc subfolder contains the firmware library header files
  - src subfolder contains the firmware library source files
  
- Drivers folder
The drivers folder contains all  source and header files that provides a set of 
functions needed to drive some STM32F103ZE-SK components (LCD, External SRAM...)

- project folder
project folder contains project for the EWARM toolchain (V5.20 and later) that 
compiles all the demonstration files.

Demonstration description 
==========================

1. After power ON(*), there would be the "STM32F103ZE-SK" message and a IAR logo 
displayed on LCD.

2. After few seconds, The principle Menu is displayed. You can switch between
tests of General purpose peripheral tests or Communication peripheral tests using
Joystick key (J).

The selected item's color is blue. you use Joystick  key (J) to navigate on different 
menu and sub-menu items.

3. If you choose Gen. purpose Periph (pressing Joystick key (J)), you will enter
periph test sub menu in which you can test STM32F103ZE- IAR SK components.

    - Ext SRAM: Use the FSMC firmware library and an associate driver to 
               communicate with a 16-Bit SRAM memory. Write into all memory,
               Read and verify the contents.
    - Stepper Motor: Show haw the motor can be driven (rotate).
    - Accelerometer: Lock the board position regardless of horizontal.
    - Potentiometer: Change the GLCD contrast by moving the potentiometer AN-TR.
    - SD Card: Use the SDIO firmware library and an associate driver to perform 
               read/write operations on the SD Card memory. 
    - Buzzer: Output 8KHz PWM signal on the buzzer.

The continue... sub menu allows you to test remaining peripherals due to the 
GLCD line numbers limit. Choosing the Continue... allows you testing:
    
    - LED: Turn on Leds (STAT1..STAT4) and switch off Leds one by one.
    - Temperature Sensor: USE of I2C temperature sensor to show temperature. 
    - Key Button: Pressing Tamper Button allow display of "Tamper key pressed"  
                  message display and LED (STAT1) is ON.      
    
3. If you choose Communication Periph (pressing Joystick key (J)), you can test:
    
    - USB: ADD USB audio device with one output. 
    - USART1: Use the USART1 to communicate with the hyperterminal with interactive
              messages on LCD.  
    - USART2: Use the USART2 to communicate with the hyperterminal with interactive
              messages on LCD.                 
                 
NOTE: 

* Refer to IAR documentations on Power Supply ways.
** After running tests you can press User key to exit, this allows you to return
to Periph tests sub menu.
*** Choosing Return allows go back to main Menu  

How to use it
============= 
- Open project at project\EWARMv5\project.eww  
- Link all compiled files and load your image into Flash
- Run the demonstation software

******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE******
