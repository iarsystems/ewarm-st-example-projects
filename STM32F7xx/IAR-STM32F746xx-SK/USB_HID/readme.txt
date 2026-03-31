########################################################################
#
#                           USB_HID.eww
#
# $Revision: 39 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR-STM32F746xx-SK board. It shows basic
  use of the parallel I/O, the timer, the interrupt, LCD and the USB_HS
  controllers.
   Connect the board to the PC through the USB-OTG port and use the joystick
  and the USER button to control the mouse pointer on the PC screen.
  The LED4 shows the USB Device connect status, and the LED1 will turns on when
  there is joystick or USER button activity.
   Additional info about usb hid status, x-axis, y-axis and the buttons +
  "IAR Academy" logo is shown on the LCD display.

  The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/

COMPATIBILITY
=============
   The USB HID example project is compatible with IAR-STM32F746xx-SK evaluation
  evaluation board. By default, the project is configured to use the I-Jet JTAG/SWD
  interface.

CONFIGURATION
=============
  Flash Debug - The Progam is loaded to internal Flash.

  Make sure that the following jumpers are correctly configured on the
  IAR-STM32F746xx-SK evaluation board:

   Jumpers:
    PWR_SEL         - depending of power source
    B0_1/B0_0       - B0_0
    RST_E           - open
    RXD3(BOOT)/RXD2 - RXD2
    TXD3(BOOT)/TXD2 - TXD2

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
