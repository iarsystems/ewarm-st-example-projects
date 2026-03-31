/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : drv_glcd.c
 *    Description : STM32F4xx LCD-TFT controller driver
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
 *    $Revision: 3189 $
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include "includes.h"

#define C_GLCD_CLK_PER_LINE     (C_GLCD_H_SIZE + C_GLCD_H_PULSE + C_GLCD_H_FRONT_PORCH + C_GLCD_H_BACK_PORCH)
#define C_GLCD_LINES_PER_FRAME  (C_GLCD_V_SIZE + C_GLCD_V_PULSE + C_GLCD_V_FRONT_PORCH + C_GLCD_V_BACK_PORCH)
#define C_GLCD_PIX_CLK          (9.1MHZ)

extern Int32U SDRAM_VRAM_BASE_ADDR[];
extern Int32U SDRAM_VRAM2_BASE_ADDR[];

#define LCD_LAYERS_COUNT   2

#define LCD_VRAM_BASE_ADDR ((Int32U)SDRAM_VRAM_BASE_ADDR)
#define LCD_VRAM2_BASE_ADDR ((Int32U)SDRAM_VRAM2_BASE_ADDR)

static pFontType_t pCurrFont[LAYERS_COUNT] = { NULL, NULL };
static LdcPixel_t TextColour[LAYERS_COUNT];
static LdcPixel_t TextBackgndColour[LAYERS_COUNT];

static uint32_t TextX_Pos[LAYERS_COUNT] = { 0, 0 };
static uint32_t TextY_Pos[LAYERS_COUNT] = { 0, 0 };

static uint32_t XL_Win[LAYERS_COUNT] = { 0, 0 };
static uint32_t YU_Win[LAYERS_COUNT] = { 0, 0 };
static uint32_t XR_Win[LAYERS_COUNT] = { C_GLCD_H_SIZE-1, C_GLCD_LAYER2_H_SIZE-1 };
static uint32_t YD_Win[LAYERS_COUNT] = { C_GLCD_V_SIZE-1, C_GLCD_LAYER2_V_SIZE-1 };

static uint32_t TabSize[LAYERS_COUNT] = { TEXT_DEF_TAB_SIZE, TEXT_DEF_TAB_SIZE };

static uint32_t WindY_Size, WindX_Size;
static uint32_t CurrY_Size, CurrX_Size;
static uint32_t * pWind;
static uint32_t * pPix;

#define EXP_COEF -0.011

uint32_t testtbl[8] = {0x00000000, 0x00000001, 0x00000101,
                       0x00010001, 0x00000100, 0x00010100,
                       0x00010000, 0x00010101};

/*************************************************************************
 * Function Name: GLCD_Init
 * Parameters: pPic_t pPic, const uint32_t * pPallete
 *
 * Return: none
 *
 * Description: GLCD controller init
 *
 *************************************************************************/
void GLCD_Init (pPic_t pPic, const uint32_t * pPallete)
{
uint32_t * pDst = (uint32_t *) LCD_VRAM_BASE_ADDR;
GPIO_InitTypeDef GPIO_InitStruct;
LTDC_InitTypeDef LTDC_InitStruct;
LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;

  /* Pin Configure */
  /* Enable GPIOs clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG |
                         RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI , ENABLE);

  /* Configure LCD_G7*/
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_3;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOD, &GPIO_InitStruct);
  /* Configure LCD_G7*/
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_LTDC);

  /* Configure LCD_B0,LCD_G0,LCD_G1, */
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5  | GPIO_Pin_6;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOE, &GPIO_InitStruct);
  /* Configure LCD_B0,LCD_G0,LCD_G1, */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_LTDC);

  /* Configure LCD_DE */
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_10;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOF, &GPIO_InitStruct);
  /* Configure LCD_DE */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);

  /* Configure LCD_R7,LCD_CLK,LCD_B2,LCD_B3,LCD_B1 */
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_6  /*| GPIO_Pin_7 */ |
                              GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOG, &GPIO_InitStruct);
  /* Configure LCD_CLK */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7  ;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOG, &GPIO_InitStruct);
  /* Configure LCD_R7,LCD_CLK,LCD_B2,LCD_B3,LCD_B1 */
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_LTDC);

  /* Configure LCD_R0,LCD_R1,LCD_R2,LCD_R3,LCD_R4,LCD_R5,LCD_R6
               LCD_G2,LCD_G3,LCD_G4 */
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_2  | GPIO_Pin_3  | GPIO_Pin_8  |
                              GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |
                              GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                              GPIO_Pin_15 ;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOH, &GPIO_InitStruct);
  /* Configure LCD_R0,LCD_R1,LCD_R2,LCD_R3,LCD_R4,LCD_R5,LCD_R6
               LCD_G2,LCD_G3,LCD_G4 */
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource2, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource8, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource9, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource12, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource13, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource14, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource15, GPIO_AF_LTDC);

  /* Configure LCD_G5,LCD_G6,LCD_B4,LCD_B5,LCD_B6,LCD_B7 */
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 |
                              GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOI, &GPIO_InitStruct);
  /* Configure LCD_G5,LCD_G6,LCD_B4,LCD_B5,LCD_B6,LCD_B7 */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource4, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_LTDC);

  /* Init GLCD cotroller */
  /* Clock init PLLSAI R output @ 10MHz
     LTDC Clock @ 5MHz */
  RCC_PLLSAIConfig(72, 0, 4);
  RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div2);
  RCC_PLLSAICmd(ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

  for(uint32_t i = 0; 10000 > i; i++);

  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;
  LTDC_InitStruct.LTDC_HorizontalSync = C_GLCD_H_PULSE - 1;
  LTDC_InitStruct.LTDC_VerticalSync = C_GLCD_V_PULSE - 1;
  LTDC_InitStruct.LTDC_AccumulatedHBP = C_GLCD_H_PULSE + C_GLCD_H_BACK_PORCH - 1;
  LTDC_InitStruct.LTDC_AccumulatedVBP = C_GLCD_V_PULSE + C_GLCD_V_BACK_PORCH - 1;
  LTDC_InitStruct.LTDC_AccumulatedActiveW = C_GLCD_H_PULSE + C_GLCD_H_BACK_PORCH \
                                            + C_GLCD_H_SIZE - 1;
  LTDC_InitStruct.LTDC_AccumulatedActiveH = C_GLCD_V_PULSE + C_GLCD_V_BACK_PORCH \
                                          + C_GLCD_V_SIZE - 1;
  LTDC_InitStruct.LTDC_TotalWidth = C_GLCD_H_PULSE + C_GLCD_H_BACK_PORCH \
                                  + C_GLCD_H_SIZE + C_GLCD_H_FRONT_PORCH - 1;
  LTDC_InitStruct.LTDC_TotalHeigh = C_GLCD_V_PULSE + C_GLCD_V_BACK_PORCH \
                                  + C_GLCD_V_SIZE + C_GLCD_V_FRONT_PORCH - 1;

  LTDC_InitStruct.LTDC_BackgroundRedValue = 0x00;
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0x00;
  LTDC_InitStruct.LTDC_BackgroundBlueValue = 0x00;
  LTDC_Init(&LTDC_InitStruct);

  LTDC_Layer_InitStruct.LTDC_HorizontalStart = C_GLCD_H_PULSE + C_GLCD_H_BACK_PORCH ;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = C_GLCD_H_PULSE + C_GLCD_H_BACK_PORCH \
                                            + C_GLCD_H_SIZE - 1;
  LTDC_Layer_InitStruct.LTDC_VarticalStart = C_GLCD_V_PULSE + C_GLCD_V_BACK_PORCH ;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = C_GLCD_V_PULSE + C_GLCD_V_BACK_PORCH \
                                          + C_GLCD_V_SIZE - 1;
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_ARGB8888;
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 0xFF;
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0x00;
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0x00;
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0x00;
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0x00;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_VRAM_BASE_ADDR;
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = C_GLCD_H_SIZE*4+3;
  LTDC_Layer_InitStruct.LTDC_CFBPitch = C_GLCD_H_SIZE*4;
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = C_GLCD_V_SIZE;
  LTDC_LayerInit(LTDC_Layer1,&LTDC_Layer_InitStruct);

  LTDC_LayerCmd(LTDC_Layer1,ENABLE);
  LTDC_ReloadConfig(LTDC_IMReload);

  if(NULL != pPallete)
  {
    GLCD_SetPallete(pPallete, Layer1);
  }

  if (NULL == pPic)
  {
    uint32_t pix = 0;
    /* Set test picture */
    for(uint32_t v = 0; (C_GLCD_V_SIZE) > v; v++)
    {
      pDst = (uint32_t *)LCD_VRAM_BASE_ADDR + v*C_GLCD_H_SIZE;
      uint32_t i = 0;

      for(uint32_t n = 0; 8 > n; n++)
      {
        pix = testtbl[n]*0xFF/*(uint32_t)(275*(1-exp(EXP_COEF*v)))*/;
        for(/*empty*/ ; i < C_GLCD_H_SIZE*(n+1)/8;i++)
        {
           *(pDst+i) = pix;
        }
      }
    }
  }
  else
  {
    // set display memory

   GLCD_ShowPic(0,0,pPic,0,Layer1);
   /*
    for(uint32_t i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE) > i; i++)
    {
      *pDst++ = *pPain++;
    }*/
  }

  for(volatile uint32_t i = C_GLCD_ENA_DIS_DLY; i; i--);
}

/*************************************************************************
 * Function Name: GLCD_Layer2_Init
 * Parameters: pPic_t *pPic, const uint32_t * pPallete
 *
 * Return: none
 *
 * Description: GLCD controller init
 *
 *************************************************************************/
void GLCD_Layer2_Init(pPic_t pPic, const uint32_t * pPallete)
{
LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;
uint32_t * pData = (uint32_t *)LCD_VRAM2_BASE_ADDR;

  memset(pData,0x00,C_GLCD_LAYER2_H_SIZE*C_GLCD_LAYER2_V_SIZE*4);

  LTDC_Layer_InitStruct.LTDC_HorizontalStart = C_GLCD_H_PULSE + C_GLCD_H_BACK_PORCH ;

  LTDC_Layer_InitStruct.LTDC_HorizontalStop = C_GLCD_H_PULSE + C_GLCD_H_BACK_PORCH \
                                            + C_GLCD_LAYER2_H_SIZE - 1;

  LTDC_Layer_InitStruct.LTDC_VarticalStart = C_GLCD_V_PULSE + C_GLCD_V_BACK_PORCH ;

  LTDC_Layer_InitStruct.LTDC_VerticalStop = C_GLCD_V_PULSE + C_GLCD_V_BACK_PORCH \
                                          + C_GLCD_LAYER2_V_SIZE - 1;

  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_ARGB8888;
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 0xFF;
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0x00;
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0x00;
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0x00;
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0x00;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_VRAM2_BASE_ADDR;
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = C_GLCD_LAYER2_H_SIZE*4+3;
  LTDC_Layer_InitStruct.LTDC_CFBPitch = C_GLCD_LAYER2_H_SIZE*4;
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = C_GLCD_LAYER2_V_SIZE;
  LTDC_LayerInit(LTDC_Layer2,&LTDC_Layer_InitStruct);

  LTDC_LayerCmd(LTDC_Layer2,ENABLE);
  LTDC_ReloadConfig(LTDC_VBReload);

  if(NULL != pPallete)
  {
    GLCD_SetPallete(pPallete, Layer2);
  }

  if (NULL != pPic)
  {
    GLCD_ShowPic(0,0,pPic,0,Layer2);
  }

}


/*************************************************************************
 * Function Name: GLCD_SetPallete
 * Parameters: const uint32_t * pPallete, Layer_t Layer
 *
 * Return: none
 *
 * Description: GLCD init colour pallete
 *
 *************************************************************************/
void GLCD_SetPallete(const uint32_t * pPallete, Layer_t Layer)
{
}

/*************************************************************************
 * Function Name: GLCD_Ctrl
 * Parameters: Boolean bEna
 *
 * Return: none
 *
 * Description: GLCD enable disable sequence
 *
 *************************************************************************/
void GLCD_Ctrl (Boolean bEna)
{
  if (bEna)
  {
    LTDC_Cmd(ENABLE);
  }
  else
  {
    LTDC_Cmd(DISABLE);
  }
}

/*************************************************************************
 * Function Name: GLCD_SetFont
 * Parameters: pFontType_t pFont, LdcPixel_t Color
 *              LdcPixel_t BackgndColor, Layer_t Layer
 *
 * Return: none
 *
 * Description: Set current font, font color and background color
 *
 *************************************************************************/
void GLCD_SetFont(pFontType_t pFont, LdcPixel_t Color, LdcPixel_t BackgndColor, Layer_t Layer)
{
  pCurrFont[Layer] = pFont;
  TextColour[Layer] = Color;
  TextBackgndColour[Layer] = BackgndColor;
}

/*************************************************************************
 * Function Name: GLCD_SetWindow
 * Parameters: uint32_t X_Left, uint32_t Y_Up,
 *             uint32_t X_Right, uint32_t Y_Down, Layer_t Layer
 *
 * Return: none
 *
 * Description: Set draw window XY coordinate in pixels
 *
 *************************************************************************/
void GLCD_SetWindow(uint32_t X_Left, uint32_t Y_Up,
                    uint32_t X_Right, uint32_t Y_Down, Layer_t Layer)
{
  if(Layer1 == Layer)
  {
    assert(X_Right < C_GLCD_H_SIZE);
    assert(Y_Down < C_GLCD_V_SIZE);
  }
  else
  if(Layer2 == Layer)
  {
    assert(X_Right < C_GLCD_LAYER2_H_SIZE);
    assert(Y_Down < C_GLCD_LAYER2_V_SIZE);
  }
  assert(X_Left < X_Right);
  assert(Y_Up < Y_Down);
  XL_Win[Layer] = X_Left;
  YU_Win[Layer] = Y_Up;
  XR_Win[Layer] = X_Right;
  YD_Win[Layer] = Y_Down;
}

/*************************************************************************
 * Function Name: GLCD_TextSetPos
 * Parameters: uint32_t X_UpLeft, uint32_t Y_UpLeft,
 *             uint32_t X_DownLeft, uint32_t Y_DownLeft, Layer_t Layer
 *
 * Return: none
 *
 * Description: Set text X,Y coordinate in characters
 *
 *************************************************************************/
void GLCD_TextSetPos(uint32_t X, uint32_t Y, Layer_t Layer)
{
  TextX_Pos[Layer] = X;
  TextY_Pos[Layer] = Y;
}

/*************************************************************************
 * Function Name: GLCD_TextSetTabSize
 * Parameters: uint32_t Size
 *
 * Return: none
 *
 * Description: Set text tab size in characters
 *
 *************************************************************************/
void GLCD_TextSetTabSize(uint32_t Size, Layer_t Layer)
{
  TabSize[Layer] = Size;
}

/*************************************************************************
 * Function Name: LCD_SET_WINDOW
 * Parameters: uint32_t X_Left, uint32_t X_Right,
 *             uint32_t Y_Up, uint32_t Y_Down, Layer_t Layer
 *
 * Return: none
 *
 * Description: Set LCD Window
 *
 *************************************************************************/
static
void LCD_SET_WINDOW (uint32_t X_Left, uint32_t X_Right,
                     uint32_t Y_Up, uint32_t Y_Down, Layer_t Layer)
{
  if(Layer1 == Layer)
    pPix = pWind = ((uint32_t *)LCD_VRAM_BASE_ADDR) + X_Left + (Y_Up*C_GLCD_H_SIZE);
  else if(Layer2 == Layer)
    pPix = pWind = ((uint32_t *)LCD_VRAM2_BASE_ADDR) + X_Left + (Y_Up*C_GLCD_LAYER2_H_SIZE);
  WindX_Size = X_Right - X_Left;
  WindY_Size = Y_Down - Y_Up;
  CurrX_Size = CurrY_Size = 0;
}

/*************************************************************************
 * Function Name: LCD_SET_WINDOW
 * Parameters: uint32_t Pixel, Layer_t Layer
 *
 * Return: none
 *
 * Description: Write pixelin the video memory
 *
 *************************************************************************/
static
void LCD_WRITE_PIXEL (uint32_t Pixel, Layer_t Layer)
{
  *pPix++ = Pixel;
  if (++CurrX_Size > WindX_Size)
  {
    CurrX_Size = 0;
    if(++CurrY_Size > WindY_Size)
    {
      CurrY_Size = 0;
    }
    if (Layer1 == Layer)
      pPix = pWind + CurrY_Size * C_GLCD_H_SIZE;
    else if (Layer2 == Layer)
      pPix = pWind + CurrY_Size * C_GLCD_LAYER2_H_SIZE;
  }
}

/*************************************************************************
 * Function Name: GLCD_TextCalcWindow
 * Parameters: uint32_t * pXL, uint32_t * pXR,
 *             uint32_t * pYU, uint32_t * pYD,
 *             uint32_t * pH_Size, uint32_t * pV_Size, Layer_t Layer
 *
 * Return: Boolean
 *          FALSE - out of window coordinate aren't valid
 *          TRUE  - the returned coordinate are valid
 *
 * Description: Calculate character window
 *
 *************************************************************************/
static
Boolean GLCD_TextCalcWindow (uint32_t * pXL, uint32_t * pXR,
                             uint32_t * pYU, uint32_t * pYD,
                             uint32_t * pH_Size, uint32_t * pV_Size, Layer_t Layer)
{
  *pH_Size = pCurrFont[Layer]->H_Size;
  *pV_Size = pCurrFont[Layer]->V_Size;
  *pXL = XL_Win[Layer] + (TextX_Pos[Layer]*pCurrFont[Layer]->H_Size);
  if(*pXL > XR_Win[Layer])
  {
    return(FALSE);
  }
  *pYU = YU_Win[Layer] + (TextY_Pos[Layer]*pCurrFont[Layer]->V_Size);
  if(*pYU > YD_Win[Layer])
  {
    return(FALSE);
  }

  *pXR   = XL_Win[Layer] + ((TextX_Pos[Layer]+1)*pCurrFont[Layer]->H_Size) - 1;
  if(*pXR > XR_Win[Layer])
  {
    *pH_Size -= *pXR - XR_Win[Layer];
    *pXR = XR_Win[Layer];
  }

  *pYD = YU_Win[Layer] + ((TextY_Pos[Layer]+1)*pCurrFont[Layer]->V_Size) - 1;
  if(*pYD > YD_Win[Layer])
  {
    *pV_Size -= *pYD - YD_Win[Layer];
    *pYD = YD_Win[Layer];
  }

  return(TRUE);
}

/*************************************************************************
 * Function Name: putchar
 * Parameters: int c, Layer_t Layer
 *
 * Return: none
 *
 * Description: Put char function
 *
 *************************************************************************/
int GLCD_putchar (int c, Layer_t Layer)
{
uint8_t * pSrc;
uint32_t H_Line;
uint32_t xl,xr,yu,yd,Temp,V_Size, H_Size, SrcInc = 1;
uint32_t WhiteSpaceNumb;
  if(pCurrFont[Layer] == NULL)
  {
    return(EOF);
  }
  H_Line = (pCurrFont[Layer]->H_Size / 8) + ((pCurrFont[Layer]->H_Size % 8)?1:0);
  switch(c)
  {
  case '\n':  // go to begin of next line (NewLine)
    ++TextY_Pos[Layer];
    break;
  case '\r':  // go to begin of this line (Carriage Return)
    // clear from current position to end of line
    while(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size,Layer))
    {
      LCD_SET_WINDOW(xl,xr,yu,yd,Layer);
      for(uint32_t i = 0; i < V_Size; ++i)
      {
        for(uint32_t j = 0; j < H_Size; ++j)
        {
          LCD_WRITE_PIXEL(TextBackgndColour[Layer],Layer);
        }
      }
      ++TextX_Pos[Layer];
    }
    TextX_Pos[Layer] = 0;
    break;
  case '\b': // go back one position (BackSpace)
    if(TextX_Pos[Layer])
    {
      --TextX_Pos[Layer];
      // del current position
      if(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size,Layer))
      {
        LCD_SET_WINDOW(xl,xr,yu,yd,Layer);
        for(uint32_t i = 0; i < V_Size; ++i)
        {
          for(uint32_t j = 0; j < H_Size; ++j)
          {
            LCD_WRITE_PIXEL(TextBackgndColour[Layer],Layer);
          }
        }
      }
    }
    break;
  case '\t':  // go to next Horizontal Tab stop
    WhiteSpaceNumb = TabSize[Layer] - (TextX_Pos[Layer]%TabSize[Layer]);
    for(uint32_t k = 0; k < WhiteSpaceNumb; ++k)
    {
      if(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size,Layer))
      {
        LCD_SET_WINDOW(xl,xr,yu,yd,Layer);
        for(uint32_t i = 0; i < V_Size; ++i)
        {
          for(uint32_t j = 0; j < H_Size; ++j)
          {
            LCD_WRITE_PIXEL(TextBackgndColour[Layer],Layer);
          }
        }
        ++TextX_Pos[Layer];
      }
      else
      {
        break;
      }
    }
    break;
  case '\f':  // go to top of page (Form Feed)
    // clear entire window
    H_Size = XR_Win[Layer] - XL_Win[Layer];
    V_Size = YD_Win[Layer] - YU_Win[Layer];
    // set character window X left, Y right
    LCD_SET_WINDOW(XL_Win[Layer],XR_Win[Layer],YU_Win[Layer],YD_Win[Layer],Layer);
    // Fill window with background font color
    for(uint32_t i = 0; i <= V_Size; ++i)
    {
      for(uint32_t j = 0; j <= H_Size; ++j)
      {
        LCD_WRITE_PIXEL(TextBackgndColour[Layer],Layer);
      }
    }

    TextX_Pos[Layer] = TextY_Pos[Layer] = 0;
    break;
  case '\a':  // signal an alert (BELl)
    TEXT_BEL1_FUNC();
    break;
  default:
    // Calculate the current character base address from stream
    // and the character position
    if((c <  pCurrFont[Layer]->CharacterOffset) &&
       (c >= pCurrFont[Layer]->CharactersNuber))
    {
      c = 0;
    }
    else
    {
      c -= pCurrFont[Layer]->CharacterOffset;
    }
    pSrc = pCurrFont[Layer]->pFontStream + (H_Line * pCurrFont[Layer]->V_Size * c);
    // Calculate character window and fit it in the text window
    if(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size,Layer))
    {
      // set character window X left, Y right
      LCD_SET_WINDOW(xl,xr,yu,yd,Layer);
      // Send char data
      for(uint32_t i = 0; i < V_Size; ++i)
      {
        SrcInc = H_Line;
        for(uint32_t j = 0; j < H_Size; ++j)
        {
          Temp = (*pSrc & (1UL << (j&0x7)))?TextColour[Layer]:TextBackgndColour[Layer];
          LCD_WRITE_PIXEL(Temp,Layer);
          if((j&0x7) == 7)
          {
            ++pSrc;
            --SrcInc;
          }
        }
        // next line of character
        pSrc += SrcInc;
      }
    }
    ++TextX_Pos[Layer];
  }
  return(c);
}

/*************************************************************************
 * Function Name: GLCD_print
 * Parameters: Layer_t Layer, char *s, const char *fmt, ...
 *
 * Return: none
 *
 * Description: Print formated string on the LCD
 *
 *************************************************************************/
void GLCD_print (Layer_t Layer,const char *fmt, ...)
{
va_list ap;
char s[MAX_GLCD_STR_SIZE];
char *p_char = s;
  va_start(ap, fmt);
  vsprintf(s, fmt,ap);
  va_end(ap);
  while(0 != *p_char)
  {
    if(-1 == GLCD_putchar(*p_char++, Layer))
    {
      break;
    }
  }
}

#define min(a,b)  (a<b?a:b)
/*************************************************************************
 * Function Name: GLCD_LoadPic
 * Parameters: bmp_t * pBmp
 *
 * Return: pPic_t
 *
 * Description: Read bmp pic and place it in SDRAM (currently supports
 *              only 24bits BMP)
 *
 *************************************************************************/
pPic_t GLCD_LoadPic (bmp_t * pBmp)
{

lcd_32bitpixel_t * pData;
bmp_24bitpixel_t * pSrc;
uint32_t PicLineSize;
pPic_t pPic;
uint32_t X_size;
uint32_t Y_size;

  if(NULL == (pPic = malloc(1*sizeof(Pic_t))))
  {
    return NULL;
  }

  pPic->H_Size = pBmp->dip_header.bmp_width;
  pPic->V_Size = pBmp->dip_header.bmp_height;
  pPic->BitsPP = pBmp->dip_header.bpp;
  pPic->BytesPP = sizeof(lcd_32bitpixel_t);

  pPic->pPalette = NULL;
  if(NULL ==  (pPic->pPicStream = malloc(pPic->H_Size*pPic->V_Size*sizeof(lcd_32bitpixel_t))))
  {
    free(pPic);
    return NULL;
  }
  pPic->pPicDesc = NULL;

  pData =  pPic->pPicStream;

  /*BMP Padding data*/
  PicLineSize = pBmp->dip_header.bmp_data_size/pBmp->dip_header.bmp_height;
  pSrc = (bmp_24bitpixel_t *)((uint32_t)pBmp + pBmp->pixarray_offset);
  pSrc = (bmp_24bitpixel_t *)((uint32_t)pSrc +(pBmp->dip_header.bmp_height-1)*PicLineSize);

  X_size = pPic->H_Size;
  Y_size = pPic->V_Size;


  for(uint32_t i = 0; i < Y_size; i++)
  {
    for(uint32_t j = 0; j < X_size; j++,pData++,pSrc++)
    {
      pData->red = pSrc->red;
      pData->green = pSrc->green;
      pData->blue = pSrc->blue;
    }
    pSrc -= X_size;
    pSrc = (bmp_24bitpixel_t *)((uint32_t)pSrc - PicLineSize);
  }

  return pPic;
}

/*************************************************************************
 * Function Name: GLCD_LoadPic32
 * Parameters: bmp32_t * pBmp
 *
 * Return: pPic_t
 *
 * Description: Read 32-bit bmp pic and place it in SDRAM
 *
 *************************************************************************/
pPic_t GLCD_LoadPic32 (bmp32_t * pBmp)
{

lcd_32bitpixel_t * pData;
bmp_32bitpixel_t * pSrc;
uint32_t PicLineSize;
pPic_t pPic;
uint32_t X_size;
uint32_t Y_size;

  if(NULL == (pPic = malloc(1*sizeof(Pic_t))))
  {
    return NULL;
  }

  pPic->H_Size = pBmp->dip_header.bmp_width;
  pPic->V_Size = pBmp->dip_header.bmp_height;
  pPic->BitsPP = pBmp->dip_header.bpp;
  pPic->BytesPP = sizeof(lcd_32bitpixel_t);

  pPic->pPalette = NULL;
  if(NULL ==  (pPic->pPicStream = malloc(pPic->H_Size*pPic->V_Size*sizeof(lcd_32bitpixel_t))))
  {
    free(pPic);
    return NULL;
  }
  pPic->pPicDesc = NULL;

  pData =  pPic->pPicStream;

  /*BMP Padding data*/
  PicLineSize = pBmp->dip_header.bmp_data_size/pBmp->dip_header.bmp_height;
  pSrc = (bmp_32bitpixel_t *)((uint32_t)pBmp + pBmp->pixarray_offset);
  pSrc = (bmp_32bitpixel_t *)((uint32_t)pSrc +(pBmp->dip_header.bmp_height-1)*PicLineSize);

  X_size = pPic->H_Size;
  Y_size = pPic->V_Size;


  for(uint32_t i = 0; i < Y_size; i++)
  {
    for(uint32_t j = 0; j < X_size; j++,pData++,pSrc++)
    {
      pData->red = pSrc->red;
      pData->green = pSrc->green;
      pData->blue = pSrc->blue;
      pData->alpha = pSrc->alpha;
    }
    pSrc -= X_size;
    pSrc = (bmp_32bitpixel_t *)((uint32_t)pSrc - PicLineSize);
  }

  return pPic;
}

/*************************************************************************
 * Function Name: GLCD_FreePic
 * Parameters:
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
void GLCD_FreePic (pPic_t pPic)
{
  free(pPic->pPicStream);
  free(pPic);
}

/*************************************************************************
 * Function Name: GLCD_ShowPic
 * Parameters: uint32_t X_Left, uint32_t Y_Up,  pPic_t pPic, uint32_t Mask
 *
 * Return: none
 *
 * Description: Load a picture to VRAM memory area
 *
 *************************************************************************/
void GLCD_ShowPic (Int32S X, Int32S Y, pPic_t pPic, uint32_t Mask, Layer_t Layer)
{
uint32_t * pData;
uint32_t * pSrc = (uint32_t *)(pPic->pPicStream);
uint32_t X_size;
uint32_t Y_size;
uint32_t Layer_H_Size;
uint32_t Layer_V_Size;

  if(Layer1 == Layer)
  {
    pData = (uint32_t *)LCD_VRAM_BASE_ADDR;
    Layer_H_Size = C_GLCD_H_SIZE;
    Layer_V_Size = C_GLCD_V_SIZE;
  }
  else if(Layer2 == Layer)
  {
    pData = (uint32_t *)LCD_VRAM2_BASE_ADDR;
    Layer_H_Size = C_GLCD_LAYER2_H_SIZE;
    Layer_V_Size = C_GLCD_LAYER2_V_SIZE;
  }
  else return;

  if(X >= 0)
  {
    pData += X;
    X_size = min(pPic->H_Size,Layer_H_Size-X);
  }
  else
  {
    pSrc -= X;
    X_size = min(pPic->H_Size+X,Layer_H_Size);
  }

  if(Y >=0)
  {
    pData += (Y * Layer_H_Size);
    Y_size = min(pPic->V_Size,Layer_V_Size-Y);
  }
  else
  {
    pSrc -= Y*pPic->H_Size;
    Y_size = min(pPic->V_Size+Y,Layer_V_Size);
  }

  for(uint32_t i = 0; i < Y_size; i++)
  {
    for(uint32_t j = 0; j < X_size; j++,pData++,pSrc++)
    {
      *pData = *pSrc^Mask;
    }
    pData += Layer_H_Size-X_size;
    pSrc += pPic->H_Size-X_size;
  }
}

/*************************************************************************
 * Function Name: GLCD_PutPixel
 * Parameters: uint32_t X, uint32_t Y,  lcd_24bitpixel_t * pPixel, Layer_t Layer
 *
 * Return: none
 *
 * Description: Draws a single pixel to the screen
 *
 *************************************************************************/
void GLCD_PutPixel(Int32U X, Int32U Y, lcd_32bitpixel_t * pPixel, Layer_t Layer)
{
uint32_t * pData;
uint32_t Layer_H_Size;
uint32_t Layer_V_Size;

  if(Layer1 == Layer)
  {
    pData = (uint32_t *)LCD_VRAM_BASE_ADDR;
    Layer_H_Size = C_GLCD_H_SIZE;
    Layer_V_Size = C_GLCD_V_SIZE;
  }
  else if(Layer2 == Layer)
  {
    pData = (uint32_t *)LCD_VRAM2_BASE_ADDR;
    Layer_H_Size = C_GLCD_LAYER2_H_SIZE;
    Layer_V_Size = C_GLCD_LAYER2_V_SIZE;
  }
  else return;

  if((X <= Layer_H_Size) && (Y <= Layer_V_Size))
  {
    pData += (Y * Layer_H_Size);
    pData += X;
    memcpy(pData,pPixel,sizeof(lcd_32bitpixel_t));
  }
}

/*************************************************************************
 * Function Name: GLCD_Layer2_Position
 * Parameters: uint32_t X, uint32_t Y
 *
 * Return: none
 *
 * Description: Sets Layer2 position
 *
 *************************************************************************/
void GLCD_Layer2_Position(Int32U X, Int32U Y)
{
  LTDC_LayerPosition(LTDC_Layer2,X,Y);
  LTDC_ReloadConfig(LTDC_VBReload);
}
