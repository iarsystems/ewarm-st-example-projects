########################################################################
#
#                           Buzzer.eww
#
# $Revision: 1471 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR-STM32F107VC-SK board. It shows how
  to make the buzzer make a sound.

   The buzzer will make a sound when you press the button labeled tamper.

COMPATIBILITY
=============
   The Buzzer example project is compatible with IAR-STM32F107VC-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   The Buzzer demo application is downloaded to Embedded Flash memory on
  the evaluation board.

   Make sure that the following jumpers are correctly configured on the
  IAR-STM32F107VC-SK evaluation board:

   Jumpers:
    PWR_SEL   - depending of power source
    JTAG_SEL  - open (on board j-link)
    B0_1/B0_0 - B0_0
    B1_1/B1_0 - B1_0

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
