########################################################################
#
#                           MassStorage.eww
#
# $Revision: 1535 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the IAR-STR750-SK evaluation board. It implements a 
  MMC/SD card drive. The first free drive letters will be used. For example, 
  if your PC configuration includes two hard disk partitions (in C:\ and D:\) 
  and a CD-ROM drive (in E:\), the memory card drive will appear as F:\. 
  The MassStorage example project works standalone on the IAR-STR750-SK board.

   The first time a USB device is connected to your computer, Windows will
  automatically load the proper device driver. In this case, the USB mass
  storage device driver will be loaded.

  Note that during board operation, the LCD backlight will indicate drive activity.

COMPATIBILITY
=============
  The project is compatible with the IAR STR750-SK board.
  The project is by default configured to use the J-Link JTAG interface.

CONFIGURATION
=============
   The MassStorage application is downloaded to the flash memory on the
  evaluation board and executed.

   Make sure that the following jumpers are correctly configured on the
  IAR-STR750-SK evaluation board:

    PWR_SEL         	- depends on the power source
    VRE             	- OFF
    BOOT0           	- 0
    BOOT1           	- 0
    SPI1_NSS/AIN5   	- SPI1_NSS
    Jumper array J5 	- 2-3 (near prototype area)
    Jumper array J6 	- 2-3 (near prototype area)

GETTING STARTED
===============
  1) Connect the J-Link.
  2) Power the board.
  3) Build and download the example.
  4) Run the example.
  5) Use a USB cable to connect the evaluation board to your PC.
  
     The MassStorage application is downloaded to the flash memory on the
     evaluation board and executed. Also note that, during board operation,
     the LCD backlight will indicate drive activity.
