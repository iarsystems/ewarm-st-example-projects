/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : mod.h
 *    Description : MOD object definitions
 *
 *    History :
 *    1. Date        : 31, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __MOD_H
#define __MOD_H

#include "ifpins.h"

typedef struct MODInfo {
  uint32_t hasUART;
  uint32_t hasI2C;
  uint32_t hasSPI;
} MODInfo_t;  

#endif /* __MOD_H */
