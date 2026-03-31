########################################################################
#
#                           GetingStarted.eww
#
# $Revision: 1550 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the STR910 Evaluation board. It shows basic use of I/O,
  timer and the interrupt controllers. It starts by blinking LD2, LD3, LD4 and
  LD5. The Left and right directions of joystick change blinking speed and
  select change light pattern of the leds.

  Buttons:
   Left direction of the joystick  - decrease speed
   Right direction of the joystick - increase speed
   Select of the joystick          - change pattern

COMPATIBILITY
=============
   The GetingStarted example project is compatible with STR910 Evaluation
  board. By default, the project is configured to use the J-Link JTAG interface.

CONFIGURATION
=============
  The application is downloaded to the flash memory.

  Make sure that the jumpers settings are regarding STR910 Evaluation board
  user manual.

GETTING STARTED
===============
  1) Connect the J-Link.
  2) Power the board.
  3) Build and download the example.
  4) Run the example.