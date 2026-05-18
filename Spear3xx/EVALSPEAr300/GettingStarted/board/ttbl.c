/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : ttbl.c
 *    Description : MMU Translation tables
 *
 *    History :
 *    1. Date        : December 9, 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *        The physical memory map is:
 *                                                                      Instruction cache
 *                                                                        Data cache
 *                                                                          User Access
 *                                                                            Supervisor Access
 *                                                                              Read Only
 *  Segment                     Description     Size                    I D U S R
 *  0x0000 0000 - 0x03FF FFFF   DDR2            64 Mbyte                * * * * -
 *  0x0400 0000 - 0x0FFF FFFF   DDR2            64 Mbyte                - - * * -
 *  0x5000 0000 - 0xAFFF FFFF   RAS           1536 Mbyte                - - * * -
 *  0xD000 0000 - 0xD7FF FFFF   Low speed      128 Mbytes               - - * * -
 *  0xD900 0000 - 0xD97F FFFF   App subsystem    8 Mbytes               - - * * -
 *  0xE000 0000 - 0xE7FF FFFF   High speed     128 Mbytes               - - * * -
 *  0xF000 0000 - 0xF7FF FFFF   CPU system     128 Mbytes               - - * * -
 *  0xF800 0000 - 0xFEFF FFFF   Basic system   112 Mbytes               - - * * -
 *  0xFF00 0000 - 0xFFFF FFFF   iROM            16 Mbytes               * * * * -
 *
 *  Domain 0 - 01 Client
 *  Domain 1 - 11 Manager
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "ttbl.h"

#pragma segment="MMU_TT"

#pragma location="MMU_TT"
#pragma data_alignment=16384
__no_init Int32U L1Table[L1_ENTRIES_NUMB];

extern __arm void __vector(void);

const TtSectionBlock_t TtSB[] =
{
  // L1
  // 64 MB RW section section cached
  L1_SECTIONS_ENTRY(  64,0x00000000,(Int32U)__vector   ,3,0,1,1),
  // 64 MB RW section section no cached
  L1_SECTIONS_ENTRY(  64,0x04000000,0x04000000         ,3,0,0,0),
  // 1536 MB section section no cached (RAS)
  L1_SECTIONS_ENTRY(1536,0x50000000,0x50000000         ,1,0,0,0),
  // 128 MB section section no cached (Low speed)
  L1_SECTIONS_ENTRY( 128,0xD0000000,0xD0000000         ,1,0,0,0),
  // 8 MB section section no cached (App sub system)
  L1_SECTIONS_ENTRY(   8,0xD9000000,0xD9000000         ,1,0,0,0),
  // 128 MB section section no cached (High Speed)
  L1_SECTIONS_ENTRY( 128,0xE0000000,0xE0000000         ,1,0,0,0),
  // 128 MB section section no cached (CPU system)
  L1_SECTIONS_ENTRY( 128,0xF0000000,0xF0000000         ,1,0,0,0),
  // 112 MB section section no cached (Basic system)
  L1_SECTIONS_ENTRY( 128,0xF8000000,0xF8000000         ,1,0,0,0),
  // 16 MB section section no cached (iRoom)
  L1_SECTIONS_ENTRY(  16,0xFF000000,0xFF000000         ,3,0,1,0),
  TSB_INVALID,
};

const TtTableBlock_t TtTB[] =
{
  {L1Table,TableL1},
  TTB_INVALID
};
