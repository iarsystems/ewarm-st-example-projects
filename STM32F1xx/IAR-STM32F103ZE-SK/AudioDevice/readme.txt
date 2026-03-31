########################################################################
#
#                           AudioDevice.eww
#
# $Revision: 1469 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR-STM32F103ZE-SK board.
   It implements usb audio device with one output.

COMPATIBILITY
=============
   The audio device example project is compatible with IAR-STM32F103ZE-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   The first time the device is connected to the computer, Windows will load the
  driver for identified device. The USB Audio Device driver will be automatically loaded.s

   Make sure that the following jumpers are correctly configured on the
  IAR-STM32F103ZE-SK evaluation board:

   Jumpers:
  PWR_SEL - depending of power source


GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.

  3) Use the USB cable to connect the PC to USB of the Board
