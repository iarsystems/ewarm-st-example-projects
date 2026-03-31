########################################################################
#
#                           Magnetometer.eww
#
# $Revision: 39 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM
 to develop code for the IAR-STM32L152VB-EXP evaluation board. It shows
 a simple magnetometer application using the new UEXT framework.
  Connect MOD-LCD3310 to the UXT1 and MOD-MAG3110(MOD-MAG) to the UXT2
 connector. The application will display current temperature and current
 magnetic field intensity in three axises (X, Y, Z).
  Immediately after starting the application, it should be calibrated (the
 "calibrating..." message is shown on the display).
 Please make a full rotations of the magnetometer sensor around each axis
 (X, Y and Z), until the displaying of the values starts. Magnetic field
 intensity is displayed in relative values (as outputted from the sensor)
 with addition of the calibrated offset.

COMPATIBILITY
=============
  The Magnetometer project is compatible with IAR-STM32L152VB-EXP
 evaluation board. By default, the project is configured to use the
 J-Link JTAG/SWD interface.

CONFIGURATION
=============
  Make sure that the following jumpers are correctly configured on the
 IAR-STM32L152VB-EXP evaluation board:

  Jumpers:
    PWR_LED_E  - filled
    SCL2/SCL1  - SCL2
    SDA2/SDA1  - SDA2
    B0_0/B0_1  - B0_0
    B1_0/B1_1  - B1_0

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
