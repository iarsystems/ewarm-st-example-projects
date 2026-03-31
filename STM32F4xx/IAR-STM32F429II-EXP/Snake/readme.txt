########################################################################
#
#                             Snake.eww
#
# $Revision: 39 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for the IAR-STM32F429II-EXP evaluation board. It shows
 an implementation of the classic Snake game using the new UEXT framework.
  Connect MOD-LCD6610 to the UXT1 and JOYSTICK to the UXT2 connector.
 Run the project and follow the onscreen instructions. Control the snake
 using the joystick and try to eat the "apples" on the screen. After eating
 some apples, the exit door appears on one of the walls. Trying not to
 hit the walls or snake itself, exit from the door to enter the next level.

COMPATIBILITY
=============
  The Snake project is compatible with IAR-STM32F429II-EXP
 evaluation board. By default, the project is configured to use the
 J-Link JTAG/SWD interface.

CONFIGURATION
=============
  Flash Debug - The Progam is loaded to internal Flash.

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
