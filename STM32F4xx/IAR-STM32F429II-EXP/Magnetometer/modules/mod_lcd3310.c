/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : mod_lcd3310.c
 *    Description : MOD-LCD3310 library
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 4278 $
 **************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "mod_lcd3310.h"

/* MOD_LCD3310 Instance parameters */

MODInfo_t MOD_LCD3310_Instance = {
  .hasUART = 0,
  .hasI2C  = 0,
  .hasSPI  = 1
};

MODInfo_t * MOD_LCD3310  = &MOD_LCD3310_Instance;

GPIOParameters_t mod_lcd3310_gpiopar = {{
{ ptINPUT,  psNORMAL },
{ ptINPUT,  psNORMAL },
{ ptOUTPUT, psNORMAL },
{ ptOUTPUT, psNORMAL },
{ ptFUNC,   psPULLUP },
{ ptFUNC,   psNORMAL },
{ ptFUNC,   psNORMAL },
{ ptOUTPUT, psNORMAL }
}};

SPIParameters_t mod_lcd3310_spipar   = { 2000000, ( SPI_CONFIG_CPOL | SPI_CONFIG_CPHA | SPI_CONFIG_SOFTSSEL ) };

ParametersGroupInfo_t MOD_LCD3310_PG = {
&mod_lcd3310_gpiopar,
#if defined __UEXT_UART__
NULL,
#endif
#if defined __UEXT_I2C__
NULL,
#endif
#if defined __UEXT_SPI__
&mod_lcd3310_spipar
#endif
};

#define MOD_LCD3310_data ((MOD_LCD3310_data_t*)(conn->aux))

/*************************************************************************
 * Function Name: MOD_LCD3310_sendCmd
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t cmd - command to send
 * Return:        none
 *
 * Description: Sends "cmd" command to the display.
 *************************************************************************/
static void MOD_LCD3310_sendCmd(ConnectionHandler_t * conn, uint8_t cmd)
{
  conn->uext->SPI->assertSSEL(conn->uext->SPI, conn->uext->GPIO);
  conn->uext->GPIO->writeBit(conn->uext->GPIO, MOD_LCD3310_PIN_DC, 0);
  conn->uext->SPI->send(conn->uext->SPI, &cmd, 1);
  while(conn->uext->SPI->isBusy(conn->uext->SPI));
  conn->uext->SPI->releaseSSEL(conn->uext->SPI, conn->uext->GPIO);
}

/*************************************************************************
 * Function Name: MOD_LCD3310_sendData
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t * data - data to send
 *                uint16_t len - count of bytes to send
 * Return:        none
 *
 * Description: Sends "len" bytes of data to the display.
 *************************************************************************/
static void MOD_LCD3310_sendData(ConnectionHandler_t * conn, uint8_t *data, uint16_t len)
{
  conn->uext->SPI->assertSSEL(conn->uext->SPI, conn->uext->GPIO);
  conn->uext->GPIO->writeBit(conn->uext->GPIO, MOD_LCD3310_PIN_DC, 1);
  conn->uext->SPI->send(conn->uext->SPI, data, len);
  while(conn->uext->SPI->isBusy(conn->uext->SPI));
  conn->uext->SPI->releaseSSEL(conn->uext->SPI, conn->uext->GPIO);
}

/*************************************************************************
 * Function Name: MOD_LCD3310_init
 * Parameters:    ConnectionHandler_t * conn - connection handler
 * Return:        uint32_t - result of operation
 *                MOD_LCD3310_RESULT_OK - operation completes successfully
 *                MOD_LCD3310_RESULT_MEM_ERROR - not enough heap memory
 *
 * Description: Initialize the MOD-LCD3310 display module.
 *************************************************************************/
uint32_t MOD_LCD3310_init(ConnectionHandler_t * conn)
{
MOD_LCD3310_data_t * data;
  data = malloc(sizeof(MOD_LCD3310_data_t));
  if (data != NULL)
  {
    /* Send #RESET pulse wider than 3uS */
    conn->uext->GPIO->writeBit(conn->uext->GPIO, MOD_LCD3310_PIN_RES, 0);
    for(uint32_t dly=0; dly < 100; dly++);
    conn->uext->GPIO->writeBit(conn->uext->GPIO, MOD_LCD3310_PIN_RES, 1);
    /* Wait at least 3uS */
    for(uint32_t dly=0; dly < 100; dly++);
    /* Configure LCD */
    MOD_LCD3310_sendCmd(conn, 0x21);    /* LCD Extended commands         */
    MOD_LCD3310_sendCmd(conn, 0x04 | ((MOD_LCD3310_START_LINE_ADDR >> 6) & 0x01)); /* Start line - S6 bit      */
    MOD_LCD3310_sendCmd(conn, 0x40 | ( MOD_LCD3310_START_LINE_ADDR       & 0x3F)); /* Start line - S[5:0] bits */
    MOD_LCD3310_sendCmd(conn, 0x13);    /* LCD Bias = 7 (Duty 1:40,1:36) */
    MOD_LCD3310_sendCmd(conn, 0x08);    /* LCD Disp.config DO=0          */
    MOD_LCD3310_sendCmd(conn, 0xF0);    /* LCD Vop (contrast)            */
    MOD_LCD3310_sendCmd(conn, 0x20);    /* LCD Standard Commands, Horizontal Addr. Mode */
    MOD_LCD3310_sendCmd(conn, 0x0C);    /* LCD in normal mode            */
    conn->aux = data;
    /* Set default char spacing */
    MOD_LCD3310_setCharSpacing(conn, MOD_LCD3310_CHAR_SPACING);
    MOD_LCD3310_clear(conn);
  }
  else
  {
    return MOD_LCD3310_RESULT_MEM_ERROR;
  }
  return MOD_LCD3310_RESULT_OK;
}

/*************************************************************************
 * Function Name: MOD_LCD3310_update
 * Parameters:    ConnectionHandler_t * conn - connection handler
 * Return:        none
 *
 * Description: Draw the current display buffer (MOD_LCD3310_data->pixelData).
 *************************************************************************/
void MOD_LCD3310_update(ConnectionHandler_t * conn)
{
  for (uint8_t row=0; row<(MOD_LCD3310_ROWS/8); row++)
  {
    MOD_LCD3310_sendCmd(conn, 0x40 | row);
    MOD_LCD3310_sendCmd(conn, 0x80);
    MOD_LCD3310_sendData(conn, MOD_LCD3310_data->pixelData+(MOD_LCD3310_COLS*row), MOD_LCD3310_COLS);
  }
}

/*************************************************************************
 * Function Name: MOD_LCD3310_clear
 * Parameters:    ConnectionHandler_t * conn - connection handler
 * Return:        none
 *
 * Description: Clears the screen.
 *************************************************************************/
void MOD_LCD3310_clear(ConnectionHandler_t * conn)
{
  memset(MOD_LCD3310_data->pixelData, 0x00, MOD_LCD3310_COLS*MOD_LCD3310_ROWS/8);
  MOD_LCD3310_update(conn);
}

/*************************************************************************
 * Function Name: MOD_LCD3310_displayDirect
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                const char chr - coordinate (in pixels)
 * Return:        none
 *
 * Description: Outputs a raw data stream to the display. Every byte
 *              represents 8 vertical dots.
 *************************************************************************/
void MOD_LCD3310_displayDirect(ConnectionHandler_t * conn, uint8_t *data, uint8_t x, uint8_t y, int16_t len)
{
uint8_t currentValidCols, currentMaxCols;
  for (uint8_t row = y; (row < (MOD_LCD3310_ROWS/8)) && (len > 0); row++)
  {
    currentMaxCols = MOD_LCD3310_COLS-x;
    currentValidCols = (len>currentMaxCols)?currentMaxCols:len;
    MOD_LCD3310_sendCmd(conn, 0x80 | x);
    MOD_LCD3310_sendCmd(conn, 0x40 | row);
    MOD_LCD3310_sendData(conn, data, currentValidCols);
    data += currentValidCols;
    len -= currentValidCols;
    if(x) x=0;
  }
}

/*************************************************************************
 * Function Name: MOD_LCD3310_setFont
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                const FontType_t * font - font to be set
 * Return:        none
 *
 * Description: Sets the current font.
 *************************************************************************/
void MOD_LCD3310_setFont(ConnectionHandler_t * conn, const FontType_t * font)
{
  MOD_LCD3310_data->font = font;
}

/*************************************************************************
 * Function Name: MOD_LCD3310_putChar
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                const char chr - coordinate (in pixels)
 *                const FontType_t * font - font to use
 * Return:        none
 *
 * Description: Outputs a single character to the display.
 *************************************************************************/
void MOD_LCD3310_putChar(ConnectionHandler_t * conn, const char chr, const FontType_t * font)
{
int32_t ch;
uint8_t *pCharData;
  ch = chr - font->CharacterOffset;
  pCharData = font->pFontStream + (ch * font->H_Size);
  MOD_LCD3310_sendData(conn, pCharData, font->H_Size);
  memcpy(&(MOD_LCD3310_data->pixelData[MOD_LCD3310_data->textY*MOD_LCD3310_COLS+MOD_LCD3310_data->textX]), pCharData, font->H_Size);
}

/*************************************************************************
 * Function Name: MOD_LCD3310_updateTextPosition
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x - x coordinate (in pixels)
 *                uint8_t y - y coordinate (in lines)
 * Return:        none
 *
 * Description: Sends coordinates to the display. 1 line = 8 horizontal
 *              dot lines.
 *************************************************************************/
static void MOD_LCD3310_updateTextPosition(ConnectionHandler_t * conn)
{
  MOD_LCD3310_sendCmd(conn, 0x80 | MOD_LCD3310_data->textX);
  MOD_LCD3310_sendCmd(conn, 0x40 | MOD_LCD3310_data->textY);
}

/*************************************************************************
 * Function Name: MOD_LCD3310_setTextPosition
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x - x coordinate (in pixels)
 *                uint8_t y - y coordinate (in lines)
 * Return:        none
 *
 * Description: Sets the current text position. 1 line = 8 horizontal
 *              dot lines.
 *************************************************************************/
void MOD_LCD3310_setTextPosition(ConnectionHandler_t * conn, uint8_t x, uint8_t y)
{
  MOD_LCD3310_data->textX = x;
  MOD_LCD3310_data->textY = y;
  MOD_LCD3310_updateTextPosition(conn);
}

/*************************************************************************
 * Function Name: MOD_LCD3310_setCharSpacing
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t cs - char spacing (in pixels)
 * Return:        none
 *
 * Description: Sets the current char spacing.
 *************************************************************************/
void MOD_LCD3310_setCharSpacing(ConnectionHandler_t * conn, uint8_t cs)
{
  MOD_LCD3310_data->charSpacing = cs;
}

/*************************************************************************
 * Function Name: MOD_LCD3310_setGraphXY
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x - x coordinate
 *                uint8_t y - y coordinate
 * Return:        none
 *
 * Description: Sets the current graphic position.
 *************************************************************************/
void MOD_LCD3310_setGraphXY(ConnectionHandler_t * conn, uint8_t x, uint8_t y)
{
  MOD_LCD3310_data->graphX = x;
  MOD_LCD3310_data->graphY = y;
}

/*************************************************************************
 * Function Name: MOD_LCD3310_writeString
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                const char *str - string
 * Return:        none
 *
 * Description: Outputs a string to the display at the current text position.
 *************************************************************************/
void MOD_LCD3310_writeString(ConnectionHandler_t * conn, const char *str)
{
uint8_t space = 0;

  while(*str)
  {
    MOD_LCD3310_updateTextPosition(conn);
    if (*str == 0x0D)
    {
      /* Carriage return */
      MOD_LCD3310_data->textX = 0;
    }
    else if (*str == 0x0A)
    {
      /* Line feed */
      MOD_LCD3310_data->textY++;
    }
    else
    {
      /* Any other character */
      MOD_LCD3310_putChar(conn, *str, MOD_LCD3310_data->font);
      MOD_LCD3310_data->textX += MOD_LCD3310_data->font->H_Size;
      /* Draw spacing */
      for(uint8_t sp = 0; sp < MOD_LCD3310_data->charSpacing; sp++)
      {
        if((MOD_LCD3310_data->textX + sp) < MOD_LCD3310_COLS)
        {
          MOD_LCD3310_sendData(conn, &space, 1);
          MOD_LCD3310_data->pixelData[MOD_LCD3310_data->textY*MOD_LCD3310_COLS+MOD_LCD3310_data->textX + sp] = space;
        }
      }
      /* Update X and Y values */
      if ((MOD_LCD3310_data->textX += MOD_LCD3310_data->charSpacing) > (MOD_LCD3310_COLS - MOD_LCD3310_data->font->H_Size))
      {
        MOD_LCD3310_data->textX = 0;
        if ((++MOD_LCD3310_data->textY) > (MOD_LCD3310_ROWS/MOD_LCD3310_data->font->V_Size - 1)) MOD_LCD3310_data->textY = 0;
      }
    }
    str++;
  }
}

/*************************************************************************
 * Function Name: MOD_LCD3310_putPixel
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x - x coordinate of the pixel
 *                uint8_t y - y coordinate of the pixel
 *                uint8_t state - pixel state (0:off, 1:on)
 * Return:        none
 *
 * Description: Draws a single pixel at (x,y)
 *************************************************************************/
void MOD_LCD3310_putPixel(ConnectionHandler_t * conn, uint8_t x, uint8_t y, uint8_t state)
{
uint8_t yl,yb;
  if (x > (MOD_LCD3310_COLS-1)) return;
  if (y > (MOD_LCD3310_ROWS-1)) return;
  MOD_LCD3310_setGraphXY(conn, x, y);
  yl = y/8;
  yb = y%8;
  if(state)
    MOD_LCD3310_data->pixelData[yl*MOD_LCD3310_COLS+x] |=  (1<<yb);
  else
    MOD_LCD3310_data->pixelData[yl*MOD_LCD3310_COLS+x] &= ~(1<<yb);
}

/*************************************************************************
 * Function Name: MOD_LCD3310_hLine
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x1 - x1 start coordinate of the line
 *                uint8_t x2 - x2 end coordinate of the line
 *                uint8_t y - y coordinate of the line
 *                uint8_t state - pixel state (0:off, 1:on)
 * Return:        none
 *
 * Description: Draws a horizontal line from (x1,y) to (x2,y)
 *************************************************************************/
void MOD_LCD3310_hLine(ConnectionHandler_t * conn, uint8_t x1, uint8_t x2, uint8_t y, uint8_t state)
{
  if (x1 > (MOD_LCD3310_COLS-1)) return;
  if (x2 > (MOD_LCD3310_COLS-1)) return;
  for(uint8_t x = x1; x <= x2; x++) MOD_LCD3310_putPixel(conn, x, y, state);
}

/*************************************************************************
 * Function Name: MOD_LCD3310_vLine
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x - x coordinate of the line
 *                uint8_t y1 - y1 start coordinate of the line
 *                uint8_t y2 - y2 end coordinate of the line
 *                uint8_t state - pixel state (0:off, 1:on)
 * Return:        none
 *
 * Description: Draws a vertical line from (x,y1) to (x,y2)
 *************************************************************************/
void MOD_LCD3310_vLine(ConnectionHandler_t * conn, uint8_t y1, uint8_t y2, uint8_t x, uint8_t state)
{
  if (y1 > (MOD_LCD3310_ROWS-1)) return;
  if (y2 > (MOD_LCD3310_ROWS-1)) return;
  for(uint8_t y = y1; y <= y2; y++) MOD_LCD3310_putPixel(conn, x, y, state);
}

/*************************************************************************
 * Function Name: MOD_LCD3310_line
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x1 - x1 coordinate of the upper left corner of the rectangle
 *                uint8_t y1 - y1 coordinate of the upper left corner of the rectangle
 *                uint8_t x2 - x2 coordinate of the lower right corner of the rectangle
 *                uint8_t y2 - y2 coordinate of the lower right corner of the rectangle
 *                uint8_t state - pixel state (0:off, 1:on)
 * Return:        none
 *
 * Description: Draws a rectangle with coordinates (x1,y1,x2,y2) on the screen
 *************************************************************************/
void MOD_LCD3310_line(ConnectionHandler_t * conn, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t state)
{
uint8_t x,y;
float d;
  if((x2-x1)<(y2-y1))
  {
    if(y2==y1) d = 0;
    else d = (float)(x2-x1)/(float)(y2-y1);
    for(y = y1; y<=y2; y++)
    {
      x = x1 + (int)(d*(y - y1));
      MOD_LCD3310_putPixel(conn, x, y, state);
    }
  }
  else
  {
    if(x2==x1) d = 0;
    else d = (float)(y2-y1)/(float)(x2-x1);
    for(x = x1; x<=x2; x++)
    {
      y = y1 + (int)(d*(x - x1));
      MOD_LCD3310_putPixel(conn, x, y, state);
    }
  }
}

/*************************************************************************
 * Function Name: MOD_LCD3310_rectangle
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x1 - x1 coordinate of the upper left corner of the rectangle
 *                uint8_t y1 - y1 coordinate of the upper left corner of the rectangle
 *                uint8_t x2 - x2 coordinate of the lower right corner of the rectangle
 *                uint8_t y2 - y2 coordinate of the lower right corner of the rectangle
 *                uint8_t state - pixel state (0:off, 1:on)
 * Return:        none
 *
 * Description: Draws a rectangle with coordinates (x1,y1,x2,y2) on the screen
 *************************************************************************/
void MOD_LCD3310_rectangle(ConnectionHandler_t * conn, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t state)
{
  MOD_LCD3310_hLine(conn, x1, x2, y1, state);
  MOD_LCD3310_vLine(conn, y1, y2, x2, state);
  MOD_LCD3310_hLine(conn, x1, x2, y2, state);
  MOD_LCD3310_vLine(conn, y1, y2, x1, state);
}

/*************************************************************************
 * Function Name: MOD_LCD3310_filledRectangle
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x1 - x1 coordinate of the upper left corner of the rectangle
 *                uint8_t y1 - y1 coordinate of the upper left corner of the rectangle
 *                uint8_t x2 - x2 coordinate of the lower right corner of the rectangle
 *                uint8_t y2 - y2 coordinate of the lower right corner of the rectangle
 *                uint8_t state - pixel state (0:off, 1:on)
 * Return:        none
 *
 * Description: Draws a filled rectangle with coordinates (x1,y1,x2,y2) on the screen
 *************************************************************************/
void MOD_LCD3310_filledRectangle(ConnectionHandler_t * conn, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t state)
{
  if (y1 > (MOD_LCD3310_ROWS-1)) return;
  if (y2 > (MOD_LCD3310_ROWS-1)) return;
  for(uint8_t y = y1; y <= y2; y++) MOD_LCD3310_hLine(conn, x1, x2, y, state);
}

/*************************************************************************
 * Function Name: MOD_LCD3310_ellipse
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x1 - x1 coordinate of the upper left corner of window
 *                uint8_t y1 - y1 coordinate of the upper left corner of window
 *                uint8_t x2 - x2 coordinate of the lower right corner of window
 *                uint8_t y2 - y2 coordinate of the lower right corner of window
 *                uint8_t state - pixel state (0:off, 1:on)
 * Return:        none
 *
 * Description: Draws an ellipse in a window with coordinates
 *               (x1,y1,x2,y2)
 *************************************************************************/
void MOD_LCD3310_ellipse(ConnectionHandler_t * conn, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t state)
{
  float xr = (float)(x2-x1)/2;
  float yr = (float)(y2-y1)/2;
  float xc = x1 + xr;
  float yc = y1 + yr;
  float f;
  for(int8_t x = x1; x <= x2; x++ )
  for(int8_t y = y1; y <= y2; y++ )
  {
    f = ((x-xc)*(x-xc))/(xr*xr) + ((y-yc)*(y-yc))/(yr*yr);
    if( (f > 0.96) && (f < 1.04) ) MOD_LCD3310_putPixel(conn, x, y, state);
  }
}

/*************************************************************************
 * Function Name: MOD_LCD3310_filledEllipse
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x1 - x1 coordinate of the upper left corner of window
 *                uint8_t y1 - y1 coordinate of the upper left corner of window
 *                uint8_t x2 - x2 coordinate of the lower right corner of window
 *                uint8_t y2 - y2 coordinate of the lower right corner of window
 *                uint8_t state - pixel state (0:off, 1:on)
 * Return:        none
 *
 * Description: Draws a filled ellipse in a window with coordinates
 *               (x1,y1,x2,y2)
 *************************************************************************/
void MOD_LCD3310_filledEllipse(ConnectionHandler_t * conn, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t state)
{
  float xr = (float)(x2-x1)/2;
  float yr = (float)(y2-y1)/2;
  float xc = x1 + xr;
  float yc = y1 + yr;
  float f;
  for(int8_t x = x1; x <= x2; x++ )
  for(int8_t y = y1; y <= y2; y++ )
  {
    f = ((x-xc)*(x-xc))/(xr*xr) + ((y-yc)*(y-yc))/(yr*yr);
    if( f < 1 ) MOD_LCD3310_putPixel(conn, x, y, state);
  }
}

/*************************************************************************
 * Function Name: MOD_LCD3310_drawBitmap
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t x - x coordinate of the upper left corner of bitmap
 *                uint8_t y - y coordinate of the upper left corner of bitmap
 *                const BitmapMonoType_t * bitmap - bitmap to draw
 * Return:        none
 *
 * Description: Draws a bitmap at (x,y) position on the screen
 *************************************************************************/
void MOD_LCD3310_drawBitmap(ConnectionHandler_t * conn, uint8_t x, uint8_t y, const BitmapMonoType_t * bitmap)
{
  uint8_t vOffset = y%8;
  uint8_t vlStart = y/8;
  uint8_t vbEnd = (y + bitmap->V_Size)%8;
  uint8_t vlEnd = (y + bitmap->V_Size)/8;
  int16_t ybs = vOffset, ybe = 8;
  uint8_t pmask;
  uint16_t pixpnt = 0;
  for(uint8_t yl = vlStart; (yl <= vlEnd) && (yl < MOD_LCD3310_ROWS/8); yl++, ybs = 0)
  {
    /* if last line of bitmap, set bitmap ending bit */
    if(yl == vlEnd) ybe = vbEnd;
    /* create mask */
    pmask = -(1 << ybs);
    pmask &= (1 << ybe) - 1;
    /* horizontal line process */
    for(uint8_t xp = x; xp < (x + bitmap->H_Size); xp++, pixpnt++)
    {
      if (xp >= MOD_LCD3310_COLS) continue;
      uint8_t * pbyte = &MOD_LCD3310_data->pixelData[yl*MOD_LCD3310_COLS + xp];
      /* clear bitmap bits */
      *pbyte &= ~pmask;
      /* if bitmap start line or not bitmap end line */
      if ((yl < vlEnd) || (yl == vlStart)) *pbyte |= bitmap->pBitmapStream[pixpnt] << vOffset;
      /* if not bitmap start line */
      if(yl > vlStart) *pbyte |= bitmap->pBitmapStream[pixpnt - (bitmap->H_Size)] >> (8 - vOffset);
    }
  }
}
