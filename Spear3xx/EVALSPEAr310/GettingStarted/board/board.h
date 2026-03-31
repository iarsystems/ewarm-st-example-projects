/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2010
 **
 **    $Revision: 39 $
 **
 ***************************************************************************/
#include <st/iospear310.h>
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

/* OSC [MHz] */
#define FOSC1               24MHZ
#define FOSC2               327680HZ

/* Core clk [MHz]  333 MHz */
#define MPLL_CLK            333MHZ
#define HCLK                166MHZ
#define PCLK                83MHZ


#define LED0                (1UL<< 0) // BS GPIO 0
#define LED1                (1UL<< 1) // BS GPIO 1
#define LED2                (1UL<< 2) // BS GPIO 2
#define LED3                (1UL<< 3) // BS GPIO 3

#define LED_MASK            (LED0 | LED1 | LED2 | LED3)
#define LED_PORT            GPIODATA
#define LED_DIR             GPIODIR


#define LED0_ON()           (LED_PORT |=  LED0)
#define LED0_OFF()          (LED_PORT &= ~LED0)
#define LED1_ON()           (LED_PORT |=  LED1)
#define LED1_OFF()          (LED_PORT &= ~LED1)
#define LED2_ON()           (LED_PORT |=  LED2)
#define LED2_OFF()          (LED_PORT &= ~LED2)
#define LED3_ON()           (LED_PORT |=  LED3)
#define LED3_OFF()          (LED_PORT &= ~LED3)

#endif /* __BOARD_H */
