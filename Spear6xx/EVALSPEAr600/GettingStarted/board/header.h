/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : header.h
 *    Description : Second level bootloader heder include file
 *
 *    History :
 *    1. Date        : August 23, 2011
 *       Author      : Stanimir Bonev
 *       Description :
 *
 *    $Revision: 39 $
 **************************************************************************/

#include <stdint.h>

#ifndef __HEADER_H
#define __HEADER_H

#define IH_NMLEN                32      /* Image Name Length */
#define IH_MAGIC        0x27051956      /* Image Magic Number */


#define SWAP_LONG(x) \
        ((uint32_t)( \
                (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) | \
                (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) | \
                (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) | \
                (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24) ))



#define     ntohl(a)       SWAP_LONG(a)

typedef  struct image_header {
        uint32_t        ih_magic;       /* Image Header Magic Number    */
        uint32_t        ih_hcrc;        /* Image Header CRC Checksum    */
        uint32_t        ih_time;        /* Image Creation Timestamp     */
        uint32_t        ih_size;        /* Image Data Size              */
        uint32_t        ih_load;        /* Data  Load  Address          */
        uint32_t        ih_ep;          /* Entry Point Address          */
        uint32_t        ih_dcrc;        /* Image Data CRC Checksum      */
        uint8_t         ih_os;          /* Operating System             */
        uint8_t         ih_arch;        /* CPU architecture             */
        uint8_t         ih_type;        /* Image Type                   */
        uint8_t         ih_comp;        /* Compression Type             */
        uint8_t         ih_name[IH_NMLEN];      /* Image Name           */
} image_header_t;

#endif // __HEADER_H

