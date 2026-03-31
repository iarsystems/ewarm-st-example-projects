########################################################################
#
#                           LCD_Demo.eww
#
# $Revision: 1474 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR STM32-SK board. It shows basic
  use of the parallel I/O, the timer, the interrupt controller, the ADC and the
  interface to LCD HD44780 compatible module.
   It shows the position of the on-board potentiometer in percents.

COMPATIBILITY
=============
   The LCD_Demo example project is compatible with IAR-STM32-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   The LCD Demo application is downloaded to the Embedded RAM or
  Embedded Flash memory depending of configurations on the evaluation
  board and executed.
   Make sure that the following jumpers are correctly configured on the
  IAR STM32-SK evaluation board:

   Jumpers:
  PWR_SEL - depending of power source

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
