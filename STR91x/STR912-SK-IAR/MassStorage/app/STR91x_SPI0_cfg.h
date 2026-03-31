/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : STR91x_SPI0_cfg.h
 *    Description : SPI 0 configuration file
 *
 *    History :
 *    1. Date        : November 28, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include "includes.h"

#ifndef __STR91X_SPI0_CFG_H
#define __STR91X_SPI0_CFG_H

// Uncoment MmcDiskInfo function
#define MMC_DISK_INFO
// enable block DMA transfer
#define SPI_DMA_ENA       1
// DMA handler interrupt priority level
#define SPI_DMA_INTR_PRIO 2

#endif //__STR91X_SPI0_CFG_H
