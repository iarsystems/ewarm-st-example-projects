########################################################################
#
#                           LCD_Demo.eww
#
# $Revision: 3916 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for the IAR-STM32F103ZE-SK board. It shows basic use of 
 the I/O, the timer, the interrupt controllers and the LCD module for graphic 
 and text drawing.
  It starts by show the IAR logo on the LCD and after 5 second draw a test screen.
  Controls:
 AN_TR                    – contrast / backlight
 USER (B1 Left button)    - adjust backlight
 WKUP (B2 Right button)   - adjust LCD contrast


COMPATIBILITY
=============
   The temperature sensor example project is compatible with IAR-STM32F103ZE-SK
  evaluation board. By default, the project is configured to use the 
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
  The LCD demo application is downloaded to Embedded Flash memory 
 on the evaluation board and executed.

  Make sure that the following jumpers are correctly configured on the
 IAR-STM32F103ZE-SK evaluation board:

Jumpers:
  PWR_SEL - depending of power source

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
