########################################################################
#
#                           Temp_demo.eww
#
# $Revision: 1469 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR-STM32F103ZE-SK board. It shows basic
  use of the parallel I/O,the timer,the interrupt controller,the interface to
  Nokia Graphic color LCD module and the I2C temperature sensor.
   It show temperature and alarm pin state. The alarm activate when temperature
  is over 31.0 C and deactivate when it drop below 30.0 C.

COMPATIBILITY
=============
   The temperature sensor example project is compatible with IAR-STM32F103ZE-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   The temperature sensor demo application is downloaded to Embedded Flash memory
  on the evaluation board and executed.

   Make sure that the following jumpers are correctly configured on the
  IAR-STM32F103ZE-SK evaluation board:

   Jumpers:
  PWR_SEL - depending of power source

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
