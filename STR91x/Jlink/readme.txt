J-Link STR9 Commander (Command line tool)
=========================================

J-Link Commander (arm\bin\JLinkSTR91x.exe) is a tool that can be used to
configure STR91x cores.
It permits some STR9 specific commands like setting the flash configuration
register and erasing the flash.
This tool can be used to erase the flash of the controller even if a program
is in flash which causes the ARM core to stall.
The tool is described in arm\doc\JLinkARM.pdf.


Programming the STR9 configuration register from C-SPY
======================================================

In most cases it is more efficient to use the J-Link STR9 commander as described above.

Loading the macro file str91x-isc.mac in the C-SPY debugger makes
it possible to control some of the STR91x low level configuration.
The low level configuration can only be controlled using low-level JTAG commands.

The J-Link must be configured for fixed speed at 400 kHz.

Note that the debugger will lose the connection with the ARM core
when executing any of these commands. To recover, the debugger session
must be terminated and the board must be power cycled.

The functions should be called from the C-SPY debugger quick watch window.
All output will go the the debug log window.
The following commands are available:

str910_conf()
Read the configuration register.

str910_erase_all()
Erase the whole flash.
Note that this operation takes some time (about 10 seconds).

str910_bank0()
Set the configuration register to bank 0.

str910_bank1()
Set the configuration register to bank 1.

