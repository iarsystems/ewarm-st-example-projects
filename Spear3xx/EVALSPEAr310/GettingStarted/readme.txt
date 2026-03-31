########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 1535 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use IAR Embedded Workbench for ARM
  to develop code for a SPEAR310 board. It shows basic use of I/O, system
  initialization PLL, DDR2, VIC, MMU, BS_GPIO and timer.

  It toggles port BS_PORT bit 0-3 (J25 pin 2 - pin 5), 15 times per second.

COMPATIBILITY
=============
   The example project is compatible with an EVALSPEAR310 evaluation board.
  By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============
   Use the default jumper and switch settings on the board. For more information,
  read the User manual for EVALSPEAR310 board.

GETTING STARTED
===============
  1) Build and download the example.
  2) Run the example.
