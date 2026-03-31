/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : main.c
 *    Description : Options word manipulation project
 *
 *    History :
 *    1. Date        : January 7, 2013
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 * Programming the STM32F4xx Options word from C-SPY
 * ======================================================
 *
 * Loading the macro file stm32f4xx_opt_word.mac in the C-SPY debugger makes
 * it possible to control some of the STM32F4xx low level configuration.
 *
 * The functions should be called from the C-SPY debugger quick watch window.
 * All output will go the debug log window.
 *
 * NOTE: In some case after execution of macro function the flash memory will
 * be erased.
 *
 * The following commands are available:
 *
 * stm32f4xx_read_opt()
 * Read the Options word.
 *
 * stm32f4xx_read_lock()
 * Enable read protection of memories (debug features limited) - level 1
 *
 * stm32f4xx_full_lock()
 * Enable chip protection (debug and boot from RAM features disabled) - level 2
 * Note: Memory read protection Level 2 is an irreversible operation. The JTAG
 *  port is permanently disabled when Level 2 is active (acting as a JTAG fuse).
 *
 * stm32f4xx_read_unlock()
 * Disable read protection - level 0. This function will erase flash memory.
 * Note: The mass erase of the flash memory takes a few seconds
 *
 * stm32f4xx_iwdt_enable()
 * Enable independent wdt. This function Will erase flash memory, if flash
 * memory not read protected.
 *
 * stm32f4xx_iwdt_disable()
 * Disable independent wdt. This function Will erase flash memory, if flash
 * memory not read protected.
 *
 * stm32f4xx_rst_stop_enable()
 * Enable reset generation when entering Stop mode. This function will erase
 * flash memory, if flash memory not read protected.
 *
 * stm32f4xx_rst_stop_disable()
 * Disable reset generation when entering Stop mode. This function will erase
 * flash memory, if flash memory not read protected.
 *
 * stm32f4xx_rst_stdby_enable()
 * Enable reset generation when entering STDBY mode. This function will erase
 * flash memory, if flash memory not read protected.
 *
 * stm32f4xx_stdby_disable()
 * Disable reset generation when entering STDBY mode. This function will erase
 * flash memory, if flash memory not read protected.
 *
 * stm32f4xx_write_protect(0xxAAA)
 * Enable/disable writes protection.
 *  0xAAA - WRP
 *  Bits 12-15 not used
 *  0 - protected sectors
 *  1 - unprotected sectors
 * Note: When the memory read protection level is selected (RDP level = 1), it
 * is not possible to program or erase Flash memory sector i if the CPU debug
 * features are connected (JTAG or single wire) or boot code is being executed
 * from RAM, even if nWRPi = 1.
 *
 * stm32f4xx_bor_level(0xA)
 * Enable/disable writes protection.
 *  0xA - BOR level
 *  Bits 7-2 not used
 *  00: BOR Level 3 (VBOR3). reset threshold level from 2.70 to 3.60 V
 *  01: BOR Level 2 (VBOR2). reset threshold level from 2.40 to 2.70 V
 *  10: BOR Level 1 (VBOR1). reset threshold level from 2.10 to 2.40 V
 *  11: BOR off (VBOR0), reset threshold level from 1.8 to 2.10 V
 *
 *    $Revision: 39 $
 **************************************************************************/

int main()
{
  return 0;
}
