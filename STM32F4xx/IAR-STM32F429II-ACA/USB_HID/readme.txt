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
  for ARM to develop code for the IAR-STM32F429II-ACA board. It shows basic
  use of the parallel I/O, the timer, the interrupt, the ADC and the USB HS
  controllers.
   Connect the board to the PC through the USB-OTG port and use the BUT1,
  BUT2,BUT3 and the AN_TR to control the mouse pointer on the PC screen.
  The LED1 will show the USB Device connect status.

COMPATIBILITY
=============
   The USB HID example project is compatible with IAR-STM32F429II-ACA evaluation
  evaluation board. By default, the project is configured to use the I-Jet JTAG/SWD
  interface.

CONFIGURATION
=============
  Flash Debug - The Progam is loaded to internal Flash.

  Make sure that the following jumpers are correctly configured on the
  IAR-STM32F429II-ACA evaluation board:

   Jumpers:
    B0_1/B0_0  - B0_0
    B1_1/B1_0  - B1_0
    LEDS_PWR_E - closed
    TRIM_E     - closed

GETTING STARTED
===============
  1) Build and download the example.
  2) Run the example.
