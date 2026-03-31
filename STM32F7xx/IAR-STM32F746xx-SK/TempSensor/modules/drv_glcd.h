/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : drv_glcd.h
 *    Description : STM32F4xx LCD-TFT controller driver include file
 *
 *    History :
 *    1. Date        : 6, March 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    2. Date        : August 2013
 *       Author      : Atanas Uzunov
 *       Author      : Stoyan Choynev
 *       Description : Adapted for STM32F4xx LCD-TFT controller
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "arm_comm.h"
#include "bmp.h"
#include "bmp32.h"

#ifndef __GLCD_DRV_H
#define __GLCD_DRV_H

typedef struct
{
  uint8_t   blue;
  uint8_t   green;
  uint8_t   red;
  uint8_t   alpha;
} lcd_32bitpixel_t;

typedef struct _Pic_t {
  uint32_t  H_Size;
  uint32_t  V_Size;
  uint32_t  BitsPP;
  uint32_t  BytesPP;
  uint32_t * pPalette;
  lcd_32bitpixel_t * pPicStream;
  uint8_t *  pPicDesc;
} Pic_t, *pPic_t;

typedef struct _FontType_t {
  uint32_t H_Size;
  uint32_t V_Size;
  uint32_t CharacterOffset;
  uint32_t CharactersNuber;
  uint8_t * pFontStream;
  uint8_t * pFontDesc;
} FontType_t, *pFontType_t;

typedef enum {
  Layer1=0,
  Layer2,
  NumberOfLayers
} Layer_t;

typedef uint32_t LdcPixel_t, *pLdcPixel_t;

#define C_GLCD_REFRESH_FREQ     (50HZ)
#define C_GLCD_H_SIZE           480
#define C_GLCD_H_PULSE          45
#define C_GLCD_H_FRONT_PORCH    45
#define C_GLCD_H_BACK_PORCH     45
#define C_GLCD_V_SIZE           272
#define C_GLCD_V_PULSE          16
#define C_GLCD_V_FRONT_PORCH    16
#define C_GLCD_V_BACK_PORCH     16

#define LAYERS_COUNT            NumberOfLayers

#define C_GLCD_LAYER2_H_SIZE    48
#define C_GLCD_LAYER2_V_SIZE    48

#define C_GLCD_PWR_ENA_DIS_DLY  50000
#define C_GLCD_ENA_DIS_DLY      50000

#define TEXT_DEF_TAB_SIZE       5

#define TEXT_BEL1_FUNC()

#define MAX_GLCD_STR_SIZE       256

#define GLCD_COLOR_BLUE         0xFF0000FF
#define GLCD_COLOR_RED          0xFFFF0000
#define GLCD_COLOR_GREEN        0xFF00FF00
#define GLCD_COLOR_BLACK        0xFF000000
#define GLCD_COLOR_WHITE        0xFFFFFFFF
#define GLCD_COLOR_GREY         0xFF808080
#define GLCD_COLOR_IAR_YELLOW   0xFFFFDD00

void GLCD_Init(pPic_t pPic, const uint32_t * pPallete);
void GLCD_SetPallete (const uint32_t * pPallete, Layer_t Layer);
void GLCD_Ctrl (Boolean bEna);
void GLCD_SetFont(pFontType_t pFont, LdcPixel_t Color, LdcPixel_t BackgndColor, Layer_t Layer);
void GLCD_SetWindow(uint32_t X_Left, uint32_t Y_Up,
                    uint32_t X_Right, uint32_t Y_Down, Layer_t Layer);
void GLCD_TextSetPos(uint32_t X, uint32_t Y, Layer_t Layer);
void GLCD_TextSetTabSize(uint32_t Size, Layer_t Layer);
static void LCD_SET_WINDOW (uint32_t X_Left, uint32_t X_Right,
                            uint32_t Y_Up, uint32_t Y_Down, Layer_t Layer);
static void LCD_WRITE_PIXEL (uint32_t Pixel, Layer_t Layer);
static Boolean GLCD_TextCalcWindow (uint32_t * pXL, uint32_t * pXR,
                                    uint32_t * pYU, uint32_t * pYD,
                                    uint32_t * pH_Size, uint32_t * pV_Size, Layer_t Layer);
pPic_t GLCD_LoadPic (bmp_t * pBmp);
pPic_t GLCD_LoadPic32 (bmp32_t * pBmp);
void GLCD_ShowPic (Int32S X, Int32S Y, pPic_t pPic, uint32_t Mask, Layer_t Layer);
void GLCD_FreePic (pPic_t pPic);
void GLCD_print (Layer_t Layer, const char *fmt, ...);
void GLCD_PutPixel(Int32U X, Int32U Y, lcd_32bitpixel_t * pPixel, Layer_t Layer);

void GLCD_Layer2_Init();
void GLCD_Layer2_Position(Int32U X, Int32U Y);

#endif // __GLCD_DRV_H
