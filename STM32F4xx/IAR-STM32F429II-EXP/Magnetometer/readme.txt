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
 to develop code for the IAR-STM32F429II-EXP evaluation board. It shows
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
  The Magnetometer project is compatible with IAR-STM32F429II-EXP
 evaluation board. By default, the project is configured to use the
 J-Link JTAG/SWD interface.

CONFIGURATION
=============
  Flash Debug - The Progam is loaded to internal Flash.

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
