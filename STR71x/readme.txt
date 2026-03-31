
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


DESCRIPTION
===========
This repository is a set of examples for STR71X microcontrollers that show how 
to use the IAR Embedded Workbench for ARM.

GettingStarted\Led_Dimmer project gives an example of ADC and I/O use.
LowPowerMode workspace include examples for Low power Wait For Interrupt (LPWFI), 
STANDBY and STOP mode management.
All examples use the STR71xLibrary interface. This is a set of C source files with 
functions to easily configure and access STR71x peripherals. 
STR71xLibrary repository also include a set of examples for each peripheral.
STR710-EVAL is a project dedicated to the STR7-Eval board.
USB-demo is a virtual USB mouse- The cursor moves depending on the command 
received through UART.


COMPATIBILITY
=============
ST\STR71x\STR71xLibrary\examples\ dedicated to the STR711/STR712 IAR boards
ST\STR71x\STR71xLibrary\examples\can project can't be used with STR711 (No CAN) .
ST\STR71x\USB-demo project can't be used with STR712 (No USB).


GETTING STARTED
===============
Double click on the *.eww file to start IAR Embedded Workbench for ARM and automatically
open the workspace including projects.Or, start the IAR Embedded Workbench for ARM.
Select File->Open->Workspace...
Open the workspace file *.eww (For example: ST\STR71x\GettingStarted\Led_Dimmer.eww)

To select the project you want to use, click on the appropriate tab or select the project 
on the Overview window (tab on the left), right click and select "set as active".
The active project is displayed in bold.
Make the project (project>make or shortcut F7) and start debug (project>debug or shortcut CTRL+D).
By default, the project is configured to use the J-Link JTAG interface.
Verify that J-Link is connected both to the PC (USB cable) and target board (JTAG flat cable). 
Start program (debug>GO or shortcut F5).

  
CONFIGURATION
=============
Refer to readme.txt file for each example for information and jumper settings.


(C) COPYRIGHT 2003 STMicroelectronics
