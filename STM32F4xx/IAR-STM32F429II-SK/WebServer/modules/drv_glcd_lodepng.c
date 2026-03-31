/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : drv_glcd_lodepng.c
 *    Description : drv_glcd wrapper for LodePNG library
 *
 *    History :
 *    1. Date        : October, 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "arm_comm.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "lodepng.h"
#include "drv_glcd.h"

/*************************************************************************
 * Function Name: LoadPNG
 * Parameters: uint8_t* pPngData, uint32_t PngSize
 *
 * Return: pPic_t
 *
 * Description: Loads and converts a PNG image into ARGB8888 LCD data
 *
 *************************************************************************/
pPic_t GLCD_LoadPNG(uint8_t* pPngData, uint32_t PngSize)
{
pPic_t pPic;
LodePNGState state;
uint32_t width;
uint32_t height;
uint32_t pixels_count;
uint32_t error;
uint8_t * image;
lcd_32bitpixel_t pixel;

  /* Load PNG image */
  lodepng_state_init(&state);
  /* Default format of decoded image is RGBA */
  error = lodepng_decode(&image, &width, &height, &state, pPngData, PngSize);
  if (error) return NULL;

  /* Create Pic_t object */
  pPic = (pPic_t)malloc(sizeof(Pic_t));
  if (NULL == pPic) return NULL;
  pPic->H_Size = width;
  pPic->V_Size = height;
  pPic->BitsPP = 32;
  pPic->BytesPP = 4;
  pPic->pPalette = NULL;
  pPic->pPicStream = (lcd_32bitpixel_t*)image;
  pPic->pPicDesc = NULL;

  /* Convert image from RGBA to ARGB */
  pixels_count = width*height;
  for (int i=0; i<pixels_count; i++)
  {
    pixel = *((lcd_32bitpixel_t*)image+i);
    ((lcd_32bitpixel_t*)image+i)->alpha = pixel.alpha;
    ((lcd_32bitpixel_t*)image+i)->red   = pixel.blue;
    ((lcd_32bitpixel_t*)image+i)->green = pixel.green;
    ((lcd_32bitpixel_t*)image+i)->blue  = pixel.red;
  }

  return pPic;
}
