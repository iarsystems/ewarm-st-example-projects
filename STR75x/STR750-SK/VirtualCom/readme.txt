########################################################################
#
#                            VirtualCom.eww
#
#  $Revision: 1722 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench for ARM 
  to develop code for the IAR STR750-SK evaluation boards. It implements USB 
  CDC (Communication Device Class) device and install it like a Virtual COM 
  port. The UART1 is used for physical implementation of the RS232 port.
  This example can work standalone on the IAR STR750-SK board.
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
  The project is compatible with the IAR STR750-SK evaluation board.
  The project is by default configured to use the J-Link JTAG interface.

CONFIGURATION
=============
  The application is downloaded to the flash.

  Set the jumpers:

  PWR_SEL   - depends on the power source
  VRE       - OFF
  BOOT0     - 0
  BOOT1     - 0

GETTING STARTED
===============
  1) Connect the J-Link.
  2) Power the board.
  3) Build and download the example.
  4) Run the example.
  5) Use an USB cable to connect the PC to the USB of the board
