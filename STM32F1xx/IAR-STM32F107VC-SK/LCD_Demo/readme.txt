########################################################################
#
#                           LCD_Demo.eww
#
# $Revision: 1471 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the IAR-STM32F107VC-SK board. It shows basic use of
  the I/O, the timer, the interrupt controller and the LDC module for graphic
  and text drawing.
   It starts by show the IAR logo on the LCD and after 5 second draw a test screen.
   Controls:
  AN_TR                    - contrast / backlight
  Left Joystik position    - adjust backlight
  Right Joystik position   - adjust LCD contrast

COMPATIBILITY
=============
    The LCD demo example project is compatible with IAR-STM32F107VC-SK
  evaluation board. By default, the project is configured to use the on board
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   The temperature sensor demo application is downloaded to Embedded Flash memory
  on the evaluation board and executed.

    Make sure that the following jumpers are correctly configured on the
  AR-STM32F107VC-SK evaluation board:

  Jumpers:
  PWR_SEL   - depending of power source
  JTAG_SEL  - open (on board j-link)
  B0_1/B0_0 - B0_0
  B1_1/B1_0 - B1_0

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
