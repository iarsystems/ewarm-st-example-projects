
/************************  STR9 Configurations bits ****************************

 !!! NOTE
 All C-SPY debugger callable function are found at the end!
 The description of the functions are also found at the end.

 Known problems

 1. Use 400 kHz fixed J-Link speed. Higher speeds and adaptive clocking will fail.

 2. Turbo mode have to be enabled, so the functions will enable turbo mode.
   There is no reason why these simple operations should need turbo mode,
   but it simply doesn't work with the normal scan chain structure.
   This has the drawback that the board must be power cycled to get back
   to a normal state where C-SPY can debug the ARM core. A hardware reset
   does not seem to be enough.

*******************************************************************************/


/*******************************************************************************

    High level functions:


   1. str910_conf()  :Read the configuration register
   
   2. str910_erase_all() :Erase the whole flash
                    Note that this operation takes some time (about 10 seconds)

   3. str910_bank0(): Set the configuration register to bank 0

   4. str910_bank1(): Set the configuration register to bank 1

--------------------------------------------------------------------------------
*       These functions should be called from the C-SPY debugger
*       quick watch window.
*       All output will go the the debug log window.
*
*       Note that the debugger will lose the connection with the ARM core
*       when executing any of these commands. To recover, the debugger session
*       must be terminated and the board must be power cycled.
*       
*       
*******************************************************************************/

int main(void)
{    
 
}
 
