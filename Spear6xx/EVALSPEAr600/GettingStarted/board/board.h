/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2011
 **
 **    $Revision: 39 $
 **
 ***************************************************************************/
#include <st/iospear600.h>
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

/* OSC [MHz] */
#define FOSC1               30MHZ
#define FOSC2               327680HZ

/* Core clk [MHz]  333 MHz */
#define MPLL_CLK            333MHZ
#define HCLK                166MHZ
#define PCLK                83MHZ


#define LED0                (1UL<<4) // BS GPIO 4
#define LED1                (1UL<<5) // BS GPIO 5

#define LED_MASK            (LED0 | LED1)
#define LED_PORT            GPIODATA
#define LED_DIR             GPIODIR


#define LED0_ON()           (LED_PORT |=  LED0)
#define LED0_OFF()          (LED_PORT &= ~LED0)
#define LED1_ON()           (LED_PORT |=  LED1)
#define LED1_OFF()          (LED_PORT &= ~LED1)

#endif /* __BOARD_H */
