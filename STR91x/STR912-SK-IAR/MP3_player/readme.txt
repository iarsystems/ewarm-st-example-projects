########################################################################
#
#                    MP3_player.eww
#
#                                                   $Revision: 1550 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the IAR-STR912-SK evaluation board and MOD-MP3 module.
  It implements a MP3 player. The MOD-MP3 module must be connected to the
  UEXT connector of the STR912-SK board. The player can read MMC/SD media
  formatted with FAT12/16/32 file system.

 COMPATIBILITY
=============
   The project is compatible with the IAR-STR912-SK evaluation board and MOD-MP3
  module.
   The project is by default configured to use the J-Link JTAG interface.

CONFIGURATION
=============
  The application is downloaded to the flash memory.

  Jumpers:
   POWER_SELECT    - depends on the power source
   USB_DIS         - 2-3
   RST-TRST        - Unfilled
   U2_RX_E         - Filled
   Jumper array J1 - 2-3 (near prototype area)
   Jumper array J2,J5,J6,J7,J8 - Unfilled
   Jumper array J16 and J15    - 2-3 (near prototype area) when J-Trace is used

  Buttons:
   B1 - Next a file/dir (select mode)
   B1 - Sound effects None, Wide Stereo, Loudness (play mode)
   B2 - Enter/exit form directory or Play/Stop

  Potentiometers:
   AN_TR - volume control

GETTING STARTED
===============
  1) Connect the J-Link.
  2) Power the board.
  3) Build and download the example.
  4) Run the example.
