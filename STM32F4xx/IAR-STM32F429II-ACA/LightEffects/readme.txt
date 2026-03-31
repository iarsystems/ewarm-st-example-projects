########################################################################
#
#                           LightEffects.eww
#
# $Revision: 39 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR-STM32F429II-ACA board. It shows basic
  use of the parallel I/O, the SysTick timer and the interrupt controller.
   The LEDs will blink in various ways, depending on the onboard switches
  positions.

COMPATIBILITY
=============
   The LightEffects example project is compatible with IAR-STM32F429II-ACA
  evaluation board. By default, the project is configured to use the
  I-Jet JTAG/SWD interface.

CONFIGURATION
=============
  Flash Debug - The Progam is loaded to internal Flash.

  Make sure that the following jumpers are correctly configured on the
  IAR-STM32F429II-ACA evaluation board:

   Jumpers:
    B0_1/B0_0 - B0_0
    B1_1/B1_0 - B1_0
    LEDS_PWR_E - closed

GETTING STARTED
===============
  1) Build and download the example.
  2) Run the example.
