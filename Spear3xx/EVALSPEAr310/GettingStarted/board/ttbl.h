/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : ttbl.h
 *    Description : MMU Translation tables
 *
 *    History :
 *    1. Date        : December 13, 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "arm926ej_cp15_drv.h"

#ifndef __TTBL_H
#define __TTBL_H

extern Int32U L1Table[L1_ENTRIES_NUMB];
extern Int32U L2Fine[L2_FP_ENTRIES_NUMB];
extern const TtSectionBlock_t TtSB[];
extern const TtTableBlock_t TtTB[];

#endif // __TTBL_H
