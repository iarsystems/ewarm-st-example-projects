########################################################################
#
#                           Acc_demo.eww
#
# $Revision: 1469 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR-STM32F103ZE-SK board. It shows basic
  use of the parallel I/O, the timer,the interrupt controller,the interface to
  Nokia Graphic color LCD module,the step motor and the 3D accelerometer sensor.
   It locks the position of the motor rotor pointer regardless of the board position.

COMPATIBILITY
=============
   The accelerometer sensor example project is compatible with IAR-STM32F103ZE-SK
  evaluation board. By default, the project is configured to use the 
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   The accelerometer sensor demo application is downloaded to Embedded Flash memory 
  on the evaluation board and executed.

   The stepper motor uses 600-700 mAmperes, you have to use external power
  (5VDC, any polarity) for this example to work correctly.
  Make sure that the following jumpers are correctly configured on the
  IAR-STM32F103ZE-SK evaluation board:

   Jumpers:
  PWR_SEL: +5V_EXT
        
GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
