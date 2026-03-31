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
  to develop code for a SPEAR600 board. It shows basic use of I/O, system
  initialization PLL, DDR2, VIC, MMU, GPIO and timers.
   Set the jumpers and the switches regarding the EVALSPEAR600 User's Manual.
  It toggles port APP_PORT bit 4-5 (LEDs D5, D6) 15 times per second.

COMPATIBILITY
=============
   The example project is compatible with an EVALSPEAR600 evaluation board.
  By default,  he project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============
   Use the default jumper and switch settings on the board. For more information,
  read the User manual for EVALSPEAR600 board.
   The Getting Started example contains two configurations.

  DDR2 Debug: The example is loaded into external DDR2 memory
  Serial NOR Debug: The example is loaded into external DDR2 from serial NOR (25P64)

GETTING STARTED
===============
  1) Build and download the example.
  2) Run the example.
