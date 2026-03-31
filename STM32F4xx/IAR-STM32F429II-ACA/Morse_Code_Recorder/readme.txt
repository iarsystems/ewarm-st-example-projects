########################################################################
#
#                        Morse_Code_Recorder.eww
#
# $Revision: 39 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR-STM32F429II-ACA board. It shows basic
  use of the parallel I/O, the timers, the interrupt controller, the DAC module
  and the SPI module. The example implements simple, event driven state machine.

  Sine wave tone (1kHz) is genearted via the onboard speaker, when BUT1 is
  pressed. Sequence of tones (dashes and dots) and pauses, called morse code,
  can be recorded, when pressing the BUT3. Recording can be stopped by pressing
  again the BUT3.
  The playback of the recorded sequence can be started by pressing BUT2. It can
  be stopped by pressing BUT2 again.

  The recorded morse code sequence is saved in the external SPI flash memory.

  The letters in the Morse code alphabet are represented by sequence of "dots"
  and "dashes", where dot is short generated tone (short press of the tone button),
  and "dash" is long generated tone (long press of the tone button).

  More info about Morse codes you can find on the internet (for ex. Wikipedia
  website).

  The following LEDs turn on, when the application is in the following states:

  LED7 - sine wave tone is generated via the speaker.
  LED5 - recording of the morse code is in progress.
  LED8 - playback of the saved morse code is in progress.

COMPATIBILITY
=============
   The Morse_Code_Recorder example project is compatible with IAR-STM32F429II-ACA
  evaluation board.
   By default, the project is configured to use the I-Jet JTAG/SWD interface.

CONFIGURATION
=============
  Flash Debug - The progam is loaded to the internal flash.

  Make sure that the following jumpers are correctly configured on the
  IAR-STM32F429II-ACA evaluation board:

   Jumpers:
    B0_1/B0_0  - B0_0
    B1_1/B1_0  - B1_0
    LEDS_PWR_E - closed
    DAC2_E     - closed
    SPEAKER_E  - closed

GETTING STARTED
===============
  1) Build and download the example.
  2) Run the example.
