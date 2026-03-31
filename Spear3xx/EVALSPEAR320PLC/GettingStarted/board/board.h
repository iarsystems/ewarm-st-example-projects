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
#include <st/iospear320.h>
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


#define LED0                (1UL<< 15) // PL_GPIO47
#define LED1                (1UL<< 17) // PL_GPIO49
#define LED2                (1UL<< 26) // PL_GPIO58
#define LED3                (1UL<<  0) // PL_GPIO64

#define LED_MASK1           (LED0 | LED1 | LED2)
#define LED_MASK2           (LED3)
#define LED_SEL_PORT1       RAS_GPIO_SELECT1
#define LED_SEL_PORT2       RAS_GPIO_SELECT2
#define LED_PORT1           RAS_GPIO_OUT1
#define LED_PORT2           RAS_GPIO_OUT2
#define LED_DIR1            RAS_GPIO_EN1
#define LED_DIR2            RAS_GPIO_EN1

#define LED0_ON()           (LED_PORT1 |=  LED0)
#define LED0_OFF()          (LED_PORT1 &= ~LED0)
#define LED1_ON()           (LED_PORT1 |=  LED1)
#define LED1_OFF()          (LED_PORT1 &= ~LED1)
#define LED2_ON()           (LED_PORT1 |=  LED2)
#define LED2_OFF()          (LED_PORT1 &= ~LED2)
#define LED3_ON()           (LED_PORT2 |=  LED3)
#define LED3_OFF()          (LED_PORT2 &= ~LED3)

#endif /* __BOARD_H */
