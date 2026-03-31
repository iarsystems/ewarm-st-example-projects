########################################################################
#
#                           TempSensor.eww
#
# $Revision: 39 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the IAR-STM32F429II-SK board. It shows basic use of the 
  parallel I/O,the SysTick timer,the interrupt controller,the LCD controller
  and the I2C temperature sensor.
   Measured/readed board temperature is shown on the LCD display below the IAR logo.
  Each time the temperature is measured/readed, the ReadTemp LED (LED1) lights up 
  for 100ms. The alarm (LED3) activates when temperature is over 34.0°C and deactivates 
  when it drop below 33.0°C. User can swith the measurement units (Celsius/Fahrenheit) 
  alternatively, by pressing the USER button at the board.

  The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/

COMPATIBILITY
=============
   The temperature sensor example project is compatible with IAR-STM32F429II-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
  Flash Debug - The Progam is loaded to internal Flash.

  Make sure that the following jumpers are correctly configured on the
  IAR-STM32F429II-SK evaluation board:

   Jumpers:
    PWR_SEL - depending of power source
    B0_1/B0_0 - B0_0
    B1_1/B1_0 - B1_0

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
