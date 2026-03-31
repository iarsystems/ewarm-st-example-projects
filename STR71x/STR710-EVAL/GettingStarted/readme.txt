########################################################################
#
#                          STR710-EVAL.eww
#
# $Revision: 1535 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM
  to develop code for the ST STR710-EVAL (MB393) evaluation board based on STR710.
  It shows basic use of parallel I/O, timer and the interrupt controller.

  It blinks the LED's next to the JTAG connector.

COMPATIBILITY
=============
  The project is compatible with the STR710-Eval board( MB393 ).
  The project is by default configured to use the J-Link JTAG interface.

CONFIGURATION
=============
  The application is downloaded to flash or RAM depending on the selected configuration.

  For the flash configuration make sure that
  - the board switches SW13, SW14 and SW15 is configured with the
    flash mapped at 0h, see the evaluation board documentation for details.
    SW13 towards CPU
    SW14 towards CPU
    SW15 opposite CPU

  For the RAM configuration make sure that
  - the board switches SW13, SW14 and SW15 is configured with the
    RAM mapped at 0h, see the evaluation board documentation for details.
    SW13 opposite CPU
    SW14 towards CPU
    SW15 opposite CPU

GETTING STARTED
===============
  1) Build and download the example.
  2) Run the example.
