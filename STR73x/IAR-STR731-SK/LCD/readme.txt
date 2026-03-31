########################################################################
#
#                          STR731_LCD.eww
#
# $Revision: 1535 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM to
  develop code for the IAR STR731-SK evaluation boards. It shows basic use of
  I/O, the timer, LCD module, ADC and the interrupt controller.

COMPATIBILITY
=============
  The project is compatible with the IAR STR731-SK evaluation board.
  The project is by default configured to use the J-Link JTAG interface.

CONFIGURATION
=============
   The application is downloaded to the flash memory.

  Jumpers:
   EXT/JLINK - the correct jumper position depends on the power source used
   M0.L/M0.H - 2-3 (M0.L)
   M1.L/M1.H - 2-3 (M1.L)
   R-T       - OFF

GETTING STARTED
===============
  1) Connect the J-Link.
  2) Power the board.
  3) Build and download the example.
  4) Run the example..
