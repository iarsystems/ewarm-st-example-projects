 /*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : bmp32.h
 *    Description : BitMap Header file. Support of 32-bit BMP
 *
 *    History :
 *    1. Date        : August 2013
 *       Author      : Atanas Uzunov
 *       Description : Initial revision.
 *
 *    $Revision: 39 $
 **************************************************************************/
#include <stdint.h>

#ifndef __BMP32_H
#define __BMP32_H

#pragma pack(1)

typedef struct
{
  uint32_t  info_header_size;
  uint32_t  bmp_width;
  uint32_t  bmp_height;
  uint16_t  planes;
  uint16_t  bpp;
  uint32_t  compression;
  uint32_t  bmp_data_size;
  uint32_t  h_res;
  uint32_t  v_res;
  uint32_t  palette_colors;
  uint32_t  imp_colors;
  uint32_t  red_channel_bitmask;
  uint32_t  green_channel_bitmask;
  uint32_t  blue_channel_bitmask;
  uint32_t  alpha_channel_bitmask;
  uint32_t  color_space_type;
  uint8_t   color_space_endpoints[36];
  uint32_t  intent;
  uint32_t  icc_profile_data;
  uint32_t  icc_profile_size;
  uint32_t  reserved;
} dip_header_v5_t;

typedef struct
{
  char      str[2];
  uint32_t  bmp_size;
  uint8_t   :8;
  uint8_t   :8;
  uint8_t   :8;
  uint8_t   :8;
  uint32_t  pixarray_offset;
  dip_header_v5_t dip_header;
} bmp32_t;

typedef struct
{
  uint8_t   blue;
  uint8_t   green;
  uint8_t   red;
  uint8_t   alpha;
} bmp_32bitpixel_t;

#pragma pack()

#endif // __BMP32_H
