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
 *    1. Date        : July, 2 2010
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
 *  0x0000_0000 - 0x03FF_3FFF   DDR2            64 Mbyte                * * * * -
 *  0x0400_0000 - 0x07FF_3FFF   DDR2            64 Mbyte                - - * * -
 *  0x4000 0000 - 0x43FF 3FFF   EMI Reg         64 Mbyte                - - * * -
 *  0x4400 0000 - 0x47FF 3FFF   EMI Per0-3      64 Mbyte                * * * * -
 *  0x7000 0000 - 0x7FFF FFF0   SDIO           256 Mbyte                - - * * -
 *  0x8000 0000 - 0x8000 3FFF   BOOT Mem        16 Kbytes               * - * * *
 *  0x9000 0000 - 0x9FFF_FFFF   CLCD           256 Mbytes               - - * * -
 *  0xA000 0000 - 0xA0FF_FFFF   PPC             16 Mbytes               - - * * -
 *  0xA100 0000 - 0xA1FF_FFFF   CAN0            16 Mbytes               - - * * -
 *  0xA200 0000 - 0xA2FF_FFFF   CAN1            16 Mbytes               - - * * -
 *  0xA300 0000 - 0xA3FF_FFFF   UART1           16 Mbytes               - - * * -
 *  0xA400 0000 - 0xA4FF_FFFF   UART2           16 Mbytes               - - * * -
 *  0xA500 0000 - 0xA5FF_FFFF   SPI1            16 Mbytes               - - * * -
 *  0xA600 0000 - 0xA6FF_FFFF   SPI2            16 Mbytes               - - * * -
 *  0xA700 0000 - 0xA7FF_FFFF   I2C             16 Mbytes               - - * * -
 *  0xA800 0000 - 0xA8FF_FFFF   QUAD PWM        16 Mbytes               - - * * -
 *  0xA900 0000 - 0xA9CF_FFFF   GPIO            13 Mbytes               - - * * -
 *  0xAA00 0000 - 0xAAFF_FFFF   SMII0           16 Mbytes               - - * * -
 *  0xAB00 0000 - 0xABFF_FFFF   SMII1/MII       16 Mbytes               - - * * -
 *  0xB300 0000 - 0xBFFF FFFF   AHBif          208 Mbytes               - - * * -
 *  0xD000 0000 - 0xD007 FFFF   UART1          512 Kbytes               - - * * -
 *  0xD008 0000 - 0xD00F FFFF   ADC            512 Kbytes               - - * * -
 *  0xD010 0000 - 0xD017 FFFF   SSP            512 Kbytes               - - * * -
 *  0xD018 0000 - 0xD01F FFFF   I2C            512 Kbytes               - - * * -
 *  0xD080 0000 - 0xD0FF FFFF   JPEG codec      16 Mbytes               - - * * -
 *  0xD100 0000 - 0xD17F FFFF   UART1            8 Mbytes               - - * * -
 *  0xD280 0000 - 0xD7FF FFFF   SRAM            88 Mbytes               - - * * -
 *  0xD900 0000 - 0xD97F FFFF   C3(AHB)          8 Mbytes               - - * * -
 *  0xE080 0000 - 0xE0FF FFFF   Ethernet ctrl    8 Mbytes               - - * * -
 *  0xE100 0000 - 0xE00F FFFF   USB2.0 device    1 Mbytes               - - * * -
 *  0xE110 0000 - 0xE11F FFFF   USB2.0 device    1 Mbytes               - - * * -
 *  0xE120 0000 - 0xE12F FFFF   USB2.0 device    1 Mbytes               - - * * -
 *  0xE180 0000 - 0xE18F FFFF   USB2.0 EHCI 0/1  1 Mbytes               - - * * -
 *  0xE190 0000 - 0xE19F FFFF   USB2.0 OHCI 0    1 Mbytes               - - * * -
 *  0xE210 0000 - 0xE21F FFFF   USB2.0 OHCI 1    1 Mbytes               - - * * -
 *  0xE280 0000 - 0xE28F FFFF   ML USB ARB       1 Mbytes               - - * * -
 *  0xF000 0000 - 0xF00F FFFF   Timer            1 Mbytes               - - * * -
 *  0xF110 0000 - 0xF11F FFFF   ITC primary      1 Mbytes               - - * * -
 *  0xF800 0000 - 0xFBFF FFFF   Serial Flash    64 Mbytes               - - * * -
 *  0xFC00 0000 - 0xFC1F FFFF   Serial Flash     2 Mbytes               - - * * -
 *  0xFC40 0000 - 0xFC5F FFFF   DMA controller   2 Mbytes               - - * * -
 *  0xFC60 0000 - 0xFC7F FFFF   SDRAM controller 2 Mbytes               - - * * -
 *  0xFC80 0000 - 0xFC87 FFFF   Timer 1        512 Kbytes               - - * * -
 *  0xFC88 0000 - 0xFC8F FFFF   WDT            512 Kbytes               - - * * -
 *  0xFC90 0000 - 0xFC97 FFFF   RTC            512 Kbytes               - - * * -
 *  0xFC98 0000 - 0xFC9F FFFF   GPIO           512 Kbytes               - - * * -
 *  0xFCA0 0000 - 0xFCA7 FFFF   SC             512 Kbytes               - - * * -
 *  0xFCA8 0000 - 0xFCAF FFFF   Misc           512 Kbytes               - - * * -
 *  0xFCB0 0000 - 0xFCB7 0000   Timer 2        512 Kbytes               - - * * -
 *  0xFF00 0000 - 0xFFFF FFFF   iROM            16 Mbytes               * - * * *
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

#pragma location="MMU_TT"
#pragma data_alignment=4096
__no_init Int32U L2Fine1[L2_FP_ENTRIES_NUMB];

extern __arm void __vector(void);

const TtSectionBlock_t TtSB[] =
{
  // L1
  // 64 MB RW section section cached
  L1_SECTIONS_ENTRY(     64,0x00000000,(Int32U)__vector   ,3,1,1,1),
  // 64 MB RW section section no cached
  L1_SECTIONS_ENTRY(     64,0x04000000,0x04000000         ,3,1,0,0),
  // 64 MB section section no cached (EMI Reg)
  L1_SECTIONS_ENTRY(     64,0x40000000,0x40000000         ,1,0,0,0),
  // 64 MB section section no cached (EMI Per 0- 3)
  L1_SECTIONS_ENTRY(     64,0x44000000,0x44000000         ,1,0,1,1),
  // 256 MB section section no cached (SDIO)
  L1_SECTIONS_ENTRY(    256,0x70000000,0x70000000         ,1,0,0,0),
  //   1 MB Fine table 1 0x8000 0000 (Boot ROM)
  L1_FINE_PAGE_ENTRY(     1,0x80000000,(Int32U)L2Fine1       ,  0    ),
  // 413 MB section section no cached (Reconfigurable array subsystem)
  L1_SECTIONS_ENTRY(    413,0x90000000,0x90000000         ,1,0,0,0),
  // 32 MB section section no cached (Reconfigurable array subsystem)
  L1_SECTIONS_ENTRY(     32,0xAA000000,0xAA000000         ,1,0,0,0),
  // 208 MB section section no cached (AHBif)
  L1_SECTIONS_ENTRY(    208,0xB3000000,0xB3000000         ,1,0,0,0),
  // 2 MB section section no cached (Low speed subsystem)
  L1_SECTIONS_ENTRY(      2,0xD0000000,0xD0000000         ,1,0,0,0),
  // 24 MB section section no cached (JPEG codec, UART1)
  L1_SECTIONS_ENTRY(     24,0xD0800000,0xD0800000         ,1,0,0,0),
  // 88 MB section section no cached (SRAM)
  L1_SECTIONS_ENTRY(     88,0xD2800000,0xD2800000         ,1,0,0,0),
  // 8 MB section section no cached (C3)
  L1_SECTIONS_ENTRY(      8,0xD9000000,0xD9000000         ,1,0,0,0),
  // 11 MB section section no cached (High speed subsystem)
  L1_SECTIONS_ENTRY(     11,0xE0800000,0xE0800000         ,1,0,0,0),
  // 2 MB section section no cached (High speed subsystem)
  L1_SECTIONS_ENTRY(      2,0xE1800000,0xE1800000         ,1,0,0,0),
  // 1 MB section section no cached (USB2.0 OHCI 1)
  L1_SECTIONS_ENTRY(      1,0xE2100000,0xE2100000         ,1,0,0,0),
  // 1 MB section section no cached (ML USB ARB)
  L1_SECTIONS_ENTRY(      1,0xE2800000,0xE2800000         ,1,0,0,0),
  // 1 MB section section no cached (Timer)
  L1_SECTIONS_ENTRY(      1,0xF0000000,0xF0000000         ,1,0,0,0),
  // 1 MB section section no cached (ITC primary)
  L1_SECTIONS_ENTRY(      1,0xF1100000,0xF1100000         ,1,0,0,0),
  // 66 MB section section no cached (Serial Flash)
  L1_SECTIONS_ENTRY(     66,0xF8000000,0xF8000000         ,1,0,0,0),
  // 10 MB section section no cached (Basic subsystem)
  L1_SECTIONS_ENTRY(     10,0xFC400000,0xFC400000         ,1,0,0,0),
  // 16 MB section section no cached (iRoom)
  L1_SECTIONS_ENTRY(     16,0xFF000000,0xFF000000         ,3,1,1,0),
  TSB_INVALID,

  // L2 fine table1
  // 16KB  16 tiny pages ReadOnly section cached
  L2_FT_TINY_PAGE_ENTRY( 16,0x80000000,0x80000000         ,3       ,1,0),
  TSB_INVALID,
};

const TtTableBlock_t TtTB[] =
{
  {L1Table,TableL1},
  {L2Fine1  ,TableL2_Fine  },
  TTB_INVALID
};
