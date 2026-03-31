/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : font.h
 *    Description : font define file
 *
 *    History :
 *    1. Date        : January 2, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __FONT_H
#define __FONT_H

#include "arm_comm.h"

typedef struct _FontType_t {
  Int32U H_Size;
  Int32U V_Size;
  Int32U CharacterOffset;
  Int32U CharactersNuber;
  pInt8U pFontStream;
  pInt8U pFontDesc;
} FontType_t, *pFontType_t;

#endif /* __FONT_H */
