########################################################################
#
#                           MassStorage.eww
#
# $Revision: 1474 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the IAR STM32-SK board.
   It implements a MMC/SD card drive. The first free drive letters will be
  used. For example, if your PC configuration includes two hard disk partitions
  (in C:\ and D:\) and a CD-ROM drive (in E:\), the memory card drive will
  appear as F:\.
   The LCD backlight will indicate drive activity.

   The first time a USB device is connected to your computer, Windows will
  automatically load the proper device driver. In this case, the USB mass
  storage device driver will be loaded.

COMPATIBILITY
=============
  The MassStorage example project is compatible with, and works as a
  standalone project, on the STM32-SK evaluation board. By default,
  the project is configured to use the J-Link JTAG/SWD interface.

CONFIGURATION
=============
   The MassStorage application is downloaded to the flash memory on the
  evaluation board and executed. Also note that, during board operation,
  the LCD backlight will indicate drive activity.

   Make sure that the following jumpers are correctly configured on the
  IAR STM32-SK evaluation board:

   Jumpers:
  PWR_SEL - depending of power source

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.

  3) Use a USB cable to connect the evaluation board to your PC.
