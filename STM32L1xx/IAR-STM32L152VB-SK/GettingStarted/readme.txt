########################################################################
#
#                           GettingStarted.eww
#
# $Revision: 1477 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the IAR-STM32L152-SK evaluation board. It shows
  basic use of I/O, timer and the interrupt controllers.
   Displays running lights on the LEDs.

COMPATIBILITY
=============
   The GettingStarted example project is compatible with IAR-STM32L152-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   The GettingStarted application is downloaded to embedded RAM or
  flash memory depending on the selected configuration and executed.

   Make sure that the following jumpers are correctly configured on the
  IAR-STM32L152-SK evaluation board:

  Jumpers:
   PWR_SEL - depending of power source
   3.3V_MCU_E - filled
   B0_0/B0_1  - B0_0
   B1_0/B1_1  - B1_0

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
