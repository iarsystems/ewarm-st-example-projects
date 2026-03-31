########################################################################
#
#                           RfVirtualCom.eww
#
# $Revision: 1589 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the Olimex STM32-103STK board.
   It implements wireless point to point USB CDC (Communication Device Class)
  device and installs it like a Virtual COM port.
   The connection is realized between a device with Node A configuration and
  a device with Node B configuration.
  
  Note:
	 Before you can use the following example the proper .inf file needs to be 
	installed for your version of windows. Please look in this example project 
	folder a subfolder \VirCOM_Driver_"X"\ . In this you will see the proper .inf file 
	that must be installed when Windows tries to install the generic CDC class driver. 
	Please note that "X" denotes your particular operating system. If it does 
	not install properly, you may have to go into your device manager and look 
	under "other devices" and manually point the driver to the proper .inf file 
	for your operating system. When you have properly installed the driver you 
	will see an entry in your Device Manager|Ports that says IAR Virtual Com Port, 
	"your board"(where "your board is the particular ST board you are using). 

COMPATIBILITY
=============
   The RF USB CDC example project is compatible with the Olimex STM32-103STK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
   The RF USB CDC application is downloaded to the Embedded Flash memory
  on the evaluation board and executed.

GETTING STARTED
===============
  1) Select Node configuration

  2) Build and download the example.

  3) Run the example.
