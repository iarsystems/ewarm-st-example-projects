########################################################################
#
#                                lcd.eww
#
# $Revision: 1535 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM to
 develop code for the IAR STR750-SK board. It shows basic use of parallel I/O,
 timer, interrupt controller, ADC and interface to a LCD HD44780 compatible
 module. It shows the position of the potentiometer in percents.

COMPATIBILITY
=============
 The project is compatible with the IAR STR750-SK board
 The project is by default configured to use the J-Link JTAG interface.

CONFIGURATION
=============
 Jumpers:
  PWR_SEL   - depends on the power source
  VRE       - OFF
  BOOT0     - 0
  BOOT1     - 0

GETTING STARTED
===============
  1) Connect the J-Link.
  2) Power the board.
  3) Build and download the example.
  4) Run the example.
