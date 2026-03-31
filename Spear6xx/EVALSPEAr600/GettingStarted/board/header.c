/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : bootloader.c
 *    Description : bootloader for SPEAR600 board
 *
 *
 *    History :
 *    1. Date        : August 23, 2011
 *       Author      : Stanimir Bonev
 *       Description : initial vestion
 *
 *    $Revision: 39 $
 **************************************************************************/
/** include files **/
#include "header.h"

/** local definitions **/
#pragma section =".boot_header"

extern __arm void __boot_entry(void);
extern __arm void __region_RAM_bootload_start__();

// ih_hcrc, ih_size, ih_dcrc field - updated by flashloader,
// ih_magic, ih_time, ih_load, ih_ep - change endian from litte to big by flashloader
// use --patch_disable option will disable update of ih_hcrc, ih_size, ih_dcrc field
// note : reserved vector (0x14, SPEAR600_cstartup) hold application image size
// vector 14 will be updated from flashloader
// all other field should be in bigendian.
__root const image_header_t Bootlader_header @ ".boot_header" =
{
  .ih_magic = IH_MAGIC,                                 /* Image Header Magic Number    */
  .ih_hcrc  = 0xFFFFFFFF,                               /* Image Header CRC Checksum    */
  .ih_time  = 12345678,                                 /* Image Creation Timestamp     */
  .ih_size  = 0xFFFFFFFF,                               /* Image Data Size              */
  .ih_load  = (uint32_t)__region_RAM_bootload_start__,  /* Data  Load  Address          */
  .ih_ep    = (uint32_t)__boot_entry,                   /* Entry Point Address          */
  .ih_dcrc  = 0xFFFFFFFF,                               /* Image Data CRC Checksum      */
  .ih_os    = 5,                                        /* Operating System             */
  .ih_arch  = 2,                                        /* CPU architecture             */
  .ih_type  = 2,                                        /* Image Type                   */
  .ih_comp  = 0,                                        /* Compression Type             */
  .ih_name  = "XLOADER",                                /* Image Name                   */
};
