########################################################################
#
#                        Cap_Buttons.eww
#
# $Revision: 39 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR-STM32F429II-ACA board. It shows basic
  use of the parallel I/O, EXTI module, the timers and the interrupt controller.
   It lights up LED1 and LED3, according the state of the capacitive buttons.
  If you touch the CAP_BUT2, the LED1 lights up, and if you touch CAP_BUT1,
  the LED3 lights up. The example measures the time needed for charging each
  capacitive button. If a button is touched the capacitance changes, so 
  the charging time also changes.

COMPATIBILITY
=============
   The Cap_Buttons example project is compatible with IAR-STM32F429II-ACA
  evaluation board.
   By default, the project is configured to use the I-Jet JTAG/SWD interface.

CONFIGURATION
=============
  Flash Debug - The progam is loaded to the internal flash.

  Make sure that the following jumpers are correctly configured on the
  IAR-STM32F429II-ACA evaluation board:

   Jumpers:
    B0_1/B0_0  - B0_0
    B1_1/B1_0  - B1_0
    LEDS_PWR_E - closed

GETTING STARTED
===============
  1) Build and download the example.
  2) Run the example.
