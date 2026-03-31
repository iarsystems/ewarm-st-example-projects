/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : 03, June 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32L152VB-EXP evaluation board. It shows
 * an implementation of the classic Snake game using the new UEXT framework.
 *  Connect MOD-LCD6610 to the UXT1 and JOYSTICK to the UXT2 connector.
 * Run the project and follow the onscreen instructions. Control the snake
 * using the joystick and try to eat the "apples" on the screen. After eating
 * some apples, the exit door appears on one of the walls. Trying not to
 * hit the walls or snake itself, exit from the door to enter the next level.
 *
 * COMPATIBILITY
 * =============
 *  The Snake project is compatible with IAR-STM32L152VB-EXP
 * evaluation board. By default, the project is configured to use the
 * J-Link JTAG/SWD interface.
 *
 * CONFIGURATION
 * =============
 *  Flash Debug - The Progam is loaded to internal Flash.
 *
 *  Make sure that the following jumpers are correctly configured on the
 * IAR-STM32L152VB-EXP evaluation board:
 *
 * Jumpers:
 *   PWR_LED_E  - filled
 *   SCL2/SCL1  - SCL2
 *   SDA2/SDA1  - SDA2
 *   B0_0/B0_1  - B0_0
 *   B1_0/B1_1  - B1_0
 *
 * GETTING STARTED
 * ===============
 * 1) Build and download the example.
 *
 * 2) Run the example.
 *
 *    $Revision: 39 $
 **************************************************************************/

#include <intrinsics.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "framework/uext.h"
#include "framework/mod.h"
#include "framework/connect.h"
#include "board/board_def.h"

#include "snake/snake_hw.h"
#include "snake/snake_core.h"

uint32_t CriticalSecCntr;

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
ConnectionHandler_t * connHandlerDisplay;
ConnectionHandler_t * connHandlerJoystick;

void main(void)
{
  __disable_interrupt();

  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  /* NVIC init */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  ENTR_CRT_SECTION();
  /* SysTick Config*/
  if(SysTick_Config(SystemCoreClock/10000))
  {
    /* Error occured */
    while (1);
  }
  EXT_CRT_SECTION();

  /* Connect Modules and get handlers */
  connHandlerDisplay  = ConnectModule(MOD_LCD6610,  UEXT1, &MOD_LCD6610_PG);
  connHandlerJoystick = ConnectModule(MOD_JOYSTICK, UEXT2, &MOD_JOYSTICK_PG);

  while(1)
  {
    /* Init hardware interface of the Snake-Game */
    Snake_hwInit(connHandlerDisplay, connHandlerJoystick);
    /* Run the game */
    Snake_game();
  }
}
