########################################################################
#
#                           VirtualCom.eww
#
# $Revision: 1589 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
  for ARM to develop code for the IAR STM32-SK board.
   It implements USB CDC (Communication Device Class) device and installs
  it like a Virtual COM port. UART3 is used for physical implementation
  of the RS232 port.
   With this example project, you can echo a character between the RS232 
  Physical interface and the Virtual Com Port USB interface. You will need to 
  open two separate (Tera Term or HyperTerm) windows. One for the virtual 
  interface and another for the UART. Set both sessions to 8-N-1 No flow control. 
  Make sure that the virtual com port occupies a port between 1-8. You may have 
  to change this in the Device Manager option under the advanced tab.
  
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
   The USB CDC example project is compatible with the IAR-STM32-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
  The USB CDC application is downloaded to the Embedded Flash memory
  on the evaluation board and executed.
   Make sure that the following jumpers are correctly configured on the
  IAR STM32-SK evaluation board:

   Jumpers:
  PWR_SEL - depending of power source

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
