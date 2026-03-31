/***************************************************************************
 **
 **    Boart specific header file
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2005
 **
 **    $Revision: 39 $
 **
 ***************************************************************************/
#include "includes.h"

#ifndef __BOARD_H
#define __BOARD_H

// Osc Frequncy
#define OSC_FREQ        (4 MHZ)

// ICLK Frequency
#define ICLK_FREQ       ((OSC_FREQ*16)/4)

// RTC_CLK Frequency
#define RTC_FREQ        (OSC_FREQ/(4*64))

// Tikcs Per second
#define TICK_PER_SECOND 50

#endif // __BOARD_H


