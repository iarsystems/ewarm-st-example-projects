########################################################################
#
#                           AudioDemo.eww
#
# $Revision: 39 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR-STM32F746xx-SK board. It shows basic
  use of the parallel I/O, the timer, the interrupt controller, the LCD
  controller and the I2S(SPI) module.
   The I2S module is connected to external Wolfson WM8731 audio codec, and is
  configured for 44.1kHz / Stereo mode/playback. User can choose from 4
  possible inputs (tone generator, microphone, line-in, and wav file playback).
  Also it is possible to set up/down/mute the volume.

   The background file and the audio file are combined into the
  "res.bin" file with the command:

  copy /b iar_globe_logo.png+iar_disco_2b_mixdown_stereo.wav res.bin

  The resources are loaded to the onboard QSPI flash memory.

  The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/

   The music piece used in this example is composed and recorded by some
  developers at IAR Systems.

COMPATIBILITY
=============
   The audio demo example project is compatible with IAR-STM32F746xx-SK
  evaluation board. By default, the project is configured to use the
  I-Jet JTAG/SWD interface.

CONFIGURATION
=============
  Flash Debug - The Progam is loaded to internal Flash.

  Make sure that the following jumpers are correctly configured on the
  IAR-STM32F746xx-SK evaluation board:

   Jumpers:
    PWR_SEL         - depending of power source
    B0_1/B0_0       - B0_0
    RST_E           - open
    RXD3(BOOT)/RXD2 - RXD2
    TXD3(BOOT)/TXD2 - TXD2

GETTING STARTED
===============
  1) Plug stereo headphones to the board. /!\ Take care of yours ears. /!\

  2) Build and download the example.

  3) Run the example.
