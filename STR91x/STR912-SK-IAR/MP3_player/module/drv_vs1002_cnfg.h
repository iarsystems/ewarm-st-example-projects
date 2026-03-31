/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : drv_vs1002_cnfg.h
 *    Description : VS1002 Driver configuration file
 *
 *    History :
 *    1. Date        : October 10, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef  __DRV_VS1002_CNFG_H
#define  __DRV_VS1002_CNFG_H

#define MP3_STATUS_VERBOSE  1
#define MP3_CLK_FREQ        2900000 // [Hz]
#define MP3_STREAM_MODE_ENA 1
#define MP3_PLUS_V_ENA      1
// This must be <= 8, because the SPI FIFO size is 8 entities
#define MP3_STREAM_SIZE     8

#endif  /* __DRV_VS1002_CNFG_H */
