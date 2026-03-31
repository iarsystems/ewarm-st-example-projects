########################################################################
#
#                           LCD.eww
#
# $Revision: 1465 $
#
########################################################################

DESCRIPTION
===========
    This example project shows how to use the IAR Embedded Workbench for ARM
   to develop code for the IAR-STM32F207ZG-SK board. It shows basic use of
   the I/O, the timer, the interrupt controller and the LDC module for graphic
   and text drawing.

   IAR logo is displayed on the LCD and after 3 second a test screen is drawn.

COMPATIBILITY
=============
   The LDC example project is compatible with IAR-STM32F207ZG-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
  SRAM Debug - The Progam is loaded to internal RAM.
  Flash Debug - The Progam is loaded to internal Flash.

  Make sure that the following jumpers are correctly configured on the
  IAR-STM32F207ZG-SK evaluation board:

   Jumpers:
    PWR_SEL - depending of power source
    B0_1/B0_0 - B0_0
    B1_1/B1_0 - B1_0

  The LCD and the Trace module have shared pins (LCD_RST and TRACED2).
  If you need to use the LCD and the Trace simultaneously please set 
  the Trace port width to 2 in the ETM settings. 

GETTING STARTED
===============
  1) Power the board and connect the J-Link.

  2) Select a configuration

  3) Build and download the example.

  4) Run the example.
