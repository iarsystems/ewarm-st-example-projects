########################################################################
#
#                         FlashingLight.eww
#
# $Revision: 1535 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the IAR STR730-SK evaluation board based on STR730F.
  It shows basic use of parallel I/O, timer and the interrupt controller.

   It starts by showing two patterns on the LEDs separated by ~1/16 second.
  After another period the LEDs continuously displays a moving pattern.

COMPATIBILITY
=============
  The project is compatible with the IAR STR730-SK evaluation board.
  The project is by default configured to use the J-Link JTAG interface.

CONFIGURATION
=============
   The application is downloaded to flash or RAM depending on the selected
  configuration.

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
  4) Run the example.
