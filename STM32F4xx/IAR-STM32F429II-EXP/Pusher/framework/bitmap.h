/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : bitmap.h
 *    Description : Bitmap definitions
 *
 *    History :
 *    1. Date        : 31, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __BITMAP_H
#define __BITMAP_H

#include <stdint.h>
#include "arm_comm.h"

typedef struct _BitmapColorType_t {
  uint32_t  H_Size;
  uint32_t  V_Size;
  uint32_t  BitsPP;
  uint32_t  BytesPP;
  uint32_t * pPalette;
  uint32_t * pPicStream;
  uint8_t *  pPicDesc;
} BitmapColorType_t, *pBitmapColorType_t;

typedef struct {
  Int32U H_Size;
  Int32U V_Size;
  pInt8U pBitmapStream;
} BitmapMonoType_t, *pBitmapMonoType_t;

#endif /* __BITMAP_H */
