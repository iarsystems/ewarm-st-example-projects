########################################################################
#
#                               AMP.eww
#
# $Revision: 1535 $
#
########################################################################

DESCRIPTION
===========
  The example projects in this workspace show how to use IAR Embedded Workbench
  for ARM to develop code for the SR5E1-EVBE7000P board and how to debug it
  in asymmetric multi-core setup. It shows basic use of I/O, system initialization
  and timer.
  The example project for Core1 blinks the on-board user LED_0, and the one for
  Core2 blinks the on-board LED_1.

COMPATIBILITY
=============
  The example projects are compatible with the SR5E1-EVBE7000P evaluation board.
  By default, the projects are configured to use the I-Jet SWD interface.

CONFIGURATION
=============
  Use the default jumper and switch settings on the board.
  For more information, read the user manual of SR5E1-EVBE7000P board.

GETTING STARTED
===============
  1) Make Core1 project active and select the desired target configuration -
     RAM Debug or Flash Debug.
  2) Batch build the application images for both Core2 and Core1 - press F8,
     select the desired batch and build it.
  3) While the Core1 project is active, press Download and Debug.
     This operation will start a multi-core debug session and will download
     both Core1 and Core2 application images.
  4) Run the application.

  NOTE: In order to properly reset the device during a debug session
  using the C-Spy toolbar's reset button, please select "System" reset
  in the button's drop-down list.
