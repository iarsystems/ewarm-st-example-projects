########################################################################
#
#                           USBMouse.eww
#
# $Revision: 1474 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR STM32-SK board. It implements USB
  HID mouse. When the host installs the needed driver, the mouse cursor
  begins to move in a rectangle shape. The WAKE-UP button is used for USB
  resume when the device is suspended.

   Controls:
    WAKE-UP - USB resume, when device is suspended

COMPATIBILITY
=============
   The USBMouse example project is compatible with IAR-STM32-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   The USBMouse application is downloaded to the Embedded RAM or
  Embedded Flash memory depending of configurations on the evaluation
  board and executed.

   Make sure that the following jumpers are correctly configured on the
  IAR STM32-SK evaluation board:

   Jumpers:
  PWR_SEL - depending of power source

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
