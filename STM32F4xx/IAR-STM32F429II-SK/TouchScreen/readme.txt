########################################################################
#
#                           TouchScreen.eww
#
# $Revision: 39 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the IAR-STM32F429II-SK board. It shows basic use of the
  parallel I/O, the interrupt controller, the LCD controller, ADC module, 
  and TIM5 module.
  The example demostrates how to use the touchscreen driver.
  Use a pen(stylus)-like object to draw on the screen using red color.

  The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/

COMPATIBILITY
=============
  The TouchScreen Demo project is compatible with IAR-STM32F429II-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
  Flash Debug - The Progam is loaded to internal Flash.

  Make sure that the following jumpers are correctly configured on the
  IAR-STM32F429II-SK evaluation board:

   Jumpers:
    PWR_SEL - depending of power source
    B0_1/B0_0 - B0_0
    B1_1/B1_0 - B1_0

GETTING STARTED
===============
  1) Build and download the example.
  2) Run the example.
