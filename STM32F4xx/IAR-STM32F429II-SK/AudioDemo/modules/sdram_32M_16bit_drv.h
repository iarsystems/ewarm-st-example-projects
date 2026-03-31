/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : sdram_32M_16bit_drv.h
 *    Description : SDRAM driver K4S561632C-TC(L)75 4MBx16bitx4
 *
 *    History :
 *    1. Date        : 29, October 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __SDRAM_32M_16BIT_DRV_H
#define __SDRAM_32M_16BIT_DRV_H

#define SDRAM_SIZE 0x02000000UL

void SDRAM_Init(void);

#endif // __SDRAM_32M_16BIT_DRV_H
