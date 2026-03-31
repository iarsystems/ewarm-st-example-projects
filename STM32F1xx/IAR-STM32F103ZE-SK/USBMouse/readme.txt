########################################################################
#
#                           USBMouse.eww
#
# $Revision: 1469 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR-STM32F103ZE-SK board. It implements
  USB HID mouse. Any button will resume USB if device is suspended.
   The joystick control up, down, left, right and left mouse button, USER
  is the left mouse button too and WKUP is the right mouse button.
  Controls:
   Joystick    - left, right, up, down movement, center left button
   USER        - left button
   WKUP        - right button

COMPATIBILITY
=============
    The USB mouse example project is compatible with IAR-STM32F103ZE-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   The USB Mouse application is downloaded to Embedded Flash memory
  on the evaluation board and executed.

   Make sure that the following jumpers are correctly configured on the
  IAR-STM32F103ZE-SK evaluation board:

Jumpers:
  PWR_SEL - depending of power source

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
