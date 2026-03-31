
Programming the STM32 Option byte from C-SPY
======================================================

Loading the macro file stm32_opt_bytes.mac in the C-SPY debugger makes
it possible to control some of the STM32 low level configuration.
The low level configuration can only be controlled using low-level JTAG commands.

The functions should be called from the C-SPY debugger quick watch window.
All output will go the debug log window.

NOTE: In some case after execution of macro function the flash memory will be erased.

The following commands are available:

stm32_read_opt()
Read the Option byte and write protect registers.

stm32_read_lock()
Enable read protection

stm32_read_unlock()
Disable read protection. This function Will erase flash memory.

stm32_iwdt_enable()
Enable independent wdt. This function Will erase flash memory, if flash memory not read protected.

stm32_iwdt_disable()
Disable independent wdt. This function Will erase flash memory, if flash memory not read protected.

stm32_rst_stop_enable()
Enable reset generation when entering Stop mode. This function will erase flash memory, if flash memory not read protected.

stm32_rst_stop_disable()
Disable reset generation when entering Stop mode. This function will erase flash memory, if flash memory not read protected.

stm32_rst_stdby_enable()
Enable reset generation when entering STDBY mode. This function will erase flash memory, if flash memory not read protected.

stm32_rst_stdby_disable()
Disable reset generation when entering STDBY mode. This function will erase flash memory, if flash memory not read protected.

stm32_write_protect(0xAABBCCDD)
Enable/disable writes protection. This function will erase flash memory, if flash memory not read protected.
 0xAA - WRP3
 0xBB - WRP2
 0xCC - WRP1
 0xDD - WRP0
 0 - protected sectors
 1 - unprotected sectors

stm32_data(0xAABB)
Set user data bytes. This function will erase flash memory, if flash memory not read protected.
 0xAA - Data0
 0xBB - Data1

