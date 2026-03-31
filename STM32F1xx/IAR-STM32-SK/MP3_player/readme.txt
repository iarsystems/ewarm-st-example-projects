########################################################################
#
#                           MP3_player.eww
#
# $Revision: 1474 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the IAR STM32-SK evaluation boards and MOD-MP3 module.
   It implements a MP3 player. The MOD-MP3 module must be connected to the
  UEXT connector of the IAR STM32-SK board. The player can read MMC/SD media
  formatted with FAT12/16/32 file system.

COMPATIBILITY
=============
   The project is compatible with the IAR IAR STM32-SK evaluation board and
  MOD-MP3 module. The project is by default configured to use the J-Link
  JTAG interface.

CONFIGURATION
=============
  The application is downloaded to the flash memory.

  Jumpers:
   PWR_SEL - depending of power source

  Buttons:
   B1 - Next a file/dir (select mode)
   B1 - Sound effects None, Wide Stereo, Loudness (play mode)
   B2 - Enter/exit form directory or Play/Stop

 Potentiometers:
   AN_TR - volume control

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
