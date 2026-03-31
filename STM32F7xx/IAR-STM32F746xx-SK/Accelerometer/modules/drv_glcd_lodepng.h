/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : drv_glcd_lodepng.h
 *    Description : drv_glcd wrapper header for LodePNG library
 *
 *    History :
 *    1. Date        : October, 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: #1 $
 **************************************************************************/
#ifndef __DRV_GLCD_LODEPNG_H
#define __DRV_GLCD_LODEPNG_H

/*************************************************************************
 * Function Name: LoadPNG
 * Parameters: uint8_t* pPngData, uint32_t PngSize
 *
 * Return: pPic_t
 *
 * Description: Loads and converts a PNG image into ARGB8888 LCD data
 *
 *************************************************************************/
pPic_t GLCD_LoadPNG(uint8_t* pPngData, uint32_t PngSize);

#endif
