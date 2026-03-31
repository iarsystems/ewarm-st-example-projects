########################################################################
#
#                           Accelerometer.eww
#
# $Revision: 39 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR-STM32F429II-SK board. It shows basic
  use of the parallel I/O, the timer, the interrupt controller, the LCD
  controller and the 3D accelerometer sensor. The IAR logo and a "magnifying
  glass" are displayed on the LCD. The magnifying glass moves as the board
  position is changed.

   The background and the magnifying glass image files are combined into the
  "images.bin" file with the command:

  copy /b iar_logo.png+edit-find.png images.bin

  The project uses "edit-find" icon from http://openiconlibrary.sourceforge.net/

  The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/

COMPATIBILITY
=============
   The accelerometer sensor example project is compatible with IAR-STM32F429II-SK
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
