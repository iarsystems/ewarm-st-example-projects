/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : mod_lcd6610.c
 *    Description : MOD-LCD6610 Graphic Library
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 2946 $
 **************************************************************************/

#include <stdlib.h>
#include "mod_lcd6610.h"

#define MOD_LCD6610_data           ((MOD_LCD6610_data_t*)conn->aux)
#define MOD_LCD6610_SPI_CS_LOW()    (MOD_LCD6610_data->port_cs)->BSRRH = MOD_LCD6610_data->pin_cs
#define MOD_LCD6610_SPI_CS_HIGH()   (MOD_LCD6610_data->port_cs)->BSRRL = MOD_LCD6610_data->pin_cs

#define MOD_LCD6610_SPI_SCK_LOW()   (MOD_LCD6610_data->port_sck)->BSRRH = MOD_LCD6610_data->pin_sck
#define MOD_LCD6610_SPI_SCK_HIGH()  (MOD_LCD6610_data->port_sck)->BSRRL = MOD_LCD6610_data->pin_sck

#define MOD_LCD6610_SPI_MOSI_LOW()  (MOD_LCD6610_data->port_mosi)->BSRRH = MOD_LCD6610_data->pin_mosi
#define MOD_LCD6610_SPI_MOSI_HIGH() (MOD_LCD6610_data->port_mosi)->BSRRL = MOD_LCD6610_data->pin_mosi

/*** MOD_LCD6610 Instance parameters ***/

MODInfo_t MOD_LCD6610_Instance = {
  .hasUART = 0,
  .hasI2C  = 0,
  .hasSPI  = 0
};

MODInfo_t * MOD_LCD6610 = &MOD_LCD6610_Instance;

GPIOParameters_t  mod_lcd6610_gpiopar = {{
{ ptOUTPUT, psNORMAL },
{ ptOUTPUT, psNORMAL },
{ ptINPUT,  psNORMAL },
{ ptINPUT,  psNORMAL },
{ ptINPUT,  psPULLUP },
{ ptOUTPUT, psNORMAL },
{ ptOUTPUT, psNORMAL },
{ ptOUTPUT, psNORMAL }
}};

ParametersGroupInfo_t MOD_LCD6610_PG = {
&mod_lcd6610_gpiopar,
#if defined __UEXT_UART__
NULL,
#endif
#if defined __UEXT_I2C__
NULL,
#endif
#if defined __UEXT_SPI__
NULL
#endif
};

/*************************************************************************
 * Function Name: MOD_LCD6610_transfer
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint8_t data - byte to send
 *                uint8_t type - type of transfer (command/data)
 * Return:        none
 *
 * Description: Perform low level SPI transfer of single byte
 *************************************************************************/
static inline void MOD_LCD6610_transfer(ConnectionHandler_t * conn, uint8_t data, uint8_t type)
{
  /* Send Data/Command bit */
  MOD_LCD6610_SPI_SCK_LOW();
  if(type)
    MOD_LCD6610_SPI_MOSI_HIGH();
  else
    MOD_LCD6610_SPI_MOSI_LOW();
  MOD_LCD6610_SPI_SCK_HIGH();
  /* Send data */
  for(int mask = 1<<7; mask; mask >>= 1)
  {
    MOD_LCD6610_SPI_SCK_LOW();
    if(data&mask)
      MOD_LCD6610_SPI_MOSI_HIGH();
    else
      MOD_LCD6610_SPI_MOSI_LOW();
    MOD_LCD6610_SPI_SCK_HIGH();
  }
}

#define MOD_LCD6610_assertCS()           MOD_LCD6610_SPI_CS_LOW()
#define MOD_LCD6610_releaseCS()          MOD_LCD6610_SPI_CS_HIGH()

/*************************************************************************
 * Function Name: MOD_LCD6610_lowLevelMapping
 * Parameters:    ConnectionHandler_t *conn - connection handler
 * Return:        uint32_t - result of operation
 *                MOD_LCD6610_RESULT_OK - operation completed successfully
 *                MOD_LCD6610_RESULT_MEM_ERROR - not enough memory
 *
 * Description: Perform low level mapping of pins needed for the SPI transfer
 *************************************************************************/
static uint32_t MOD_LCD6610_lowLevelMapping(ConnectionHandler_t * conn)
{
  conn->aux = malloc(sizeof(MOD_LCD6610_data_t));
  if (conn->aux != NULL)
  {
    MOD_LCD6610_data->port_cs   = (GPIO_TypeDef*)conn->uext->GPIO->getPortBase(conn->uext->GPIO->portNumber[MOD_LCD6610_PIN_CS]);
    MOD_LCD6610_data->port_sck  = (GPIO_TypeDef*)conn->uext->GPIO->getPortBase(conn->uext->GPIO->portNumber[MOD_LCD6610_PIN_SCK]);
    MOD_LCD6610_data->port_mosi = (GPIO_TypeDef*)conn->uext->GPIO->getPortBase(conn->uext->GPIO->portNumber[MOD_LCD6610_PIN_MOSI]);
    MOD_LCD6610_data->pin_cs   = 1 << conn->uext->GPIO->pinNumber[MOD_LCD6610_PIN_CS];
    MOD_LCD6610_data->pin_sck  = 1 << conn->uext->GPIO->pinNumber[MOD_LCD6610_PIN_SCK];
    MOD_LCD6610_data->pin_mosi = 1 << conn->uext->GPIO->pinNumber[MOD_LCD6610_PIN_MOSI];
    MOD_LCD6610_data->oddPixel = 0;
    MOD_LCD6610_data->newStream = 1;
  }
  else return MOD_LCD6610_RESULT_MEM_ERROR;
  return MOD_LCD6610_RESULT_OK;
}

/*************************************************************************
 * Function Name: MOD_LCD6610_init
 * Parameters:    ConnectionHandler_t *conn - connection handler
 * Return:        uint32_t - result of operation
 *                MOD_LCD6610_RESULT_OK - operation completed successfully
 *                MOD_LCD6610_RESULT_MEM_ERROR - not enough memory
 *
 * Description: Initialize LCD6610 display
 *              Only tested with MOD_LCD6610_VARIANT_EPSON
 *************************************************************************/
uint32_t MOD_LCD6610_init(ConnectionHandler_t * conn)
{
uint32_t res;
  /* Low level pin and port assignment */
  res = MOD_LCD6610_lowLevelMapping(conn);
  if (res) return res;
  /* Initial pin states */
  conn->uext->GPIO->writeBit(conn->uext->GPIO, MOD_LCD6610_PIN_CS,  1);
  conn->uext->GPIO->writeBit(conn->uext->GPIO, MOD_LCD6610_PIN_RES, 1);
  /* Send RESET pulse wider than 10uS */
  conn->uext->GPIO->writeBit(conn->uext->GPIO, MOD_LCD6610_PIN_RES, 0);
  for(uint32_t dly=0; dly < 200; dly++);
  conn->uext->GPIO->writeBit(conn->uext->GPIO, MOD_LCD6610_PIN_RES, 1);
  /* Wait at least 5ms */
  for(uint32_t dly=0; dly < 25000; dly++);
  /* Turn backlight on */
  conn->uext->GPIO->writeBit(conn->uext->GPIO, MOD_LCD6610_PIN_BL, 1);

#if (MOD_LCD6610_VARIANT == MOD_LCD6610_VARIANT_EPSON)

  /* Display control */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_DISCTL);
  MOD_LCD6610_sendData(conn, 0x00); /* P1: 0x00 */
  MOD_LCD6610_sendData(conn, 0x20); /* P2: 0x20 = nlines/4 - 1 = 132/4 - 1 = 32) */
  MOD_LCD6610_sendData(conn, 0x0C); /* P3: 0x0C */
  MOD_LCD6610_releaseCS();
  /* COM control */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_COMSCN);
  MOD_LCD6610_sendData(conn, 0x01); /* P1: 0x01 = Scan 1->80, 160<-81 */
  MOD_LCD6610_releaseCS();
  /* Turn on internal oscillator */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_OSCON);
  MOD_LCD6610_releaseCS();
  /* Sleep out */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_SLPOUT);
  MOD_LCD6610_releaseCS();
  /* Power control */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_PWRCTR);
  MOD_LCD6610_sendData(conn, 0x0F); /* reference voltage regulator on, circuit voltage follower on, BOOST ON */
  MOD_LCD6610_releaseCS();
  /* Inverse display */
  MOD_LCD6610_assertCS();
  //MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_DISINV);
  MOD_LCD6610_releaseCS();
  /* Data control */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_DATCTL);
  MOD_LCD6610_sendData(conn, 0x04); /* P1: 0x04 = page address inverted, col address normal, address scan in page/row direction */
  MOD_LCD6610_sendData(conn, 0x00); /* P2: 0x00 = RGB sequence (default value) */
  MOD_LCD6610_sendData(conn, 0x00); /* P3: 0x00 */
  MOD_LCD6610_releaseCS();
  /* Voltage control (contrast) */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_VOLCTR);
  MOD_LCD6610_sendData(conn, 0x24); /* P1 = 36 volume value (adjust this setting for your display 0 .. 63) */
  MOD_LCD6610_sendData(conn, 3);    /* P2 = 3 resistance ratio (determined by experiment)  */
  MOD_LCD6610_releaseCS();
  /* Wait to stabilize */
  for(uint32_t dly=0; dly < 1000; dly++);
  /* Turn on the display */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_DISON);
  MOD_LCD6610_releaseCS();

#elif (MOD_LCD6610_VARIANT == MOD_LCD6610_VARIANT_PHILIPS)

  /* Sleep out */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND1_SLEEPOUT);
  MOD_LCD6610_releaseCS();
  /* Inverse the display */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND1_INVON);
  MOD_LCD6610_releaseCS();
  /* Color interface pixel format */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND1_COLMOD);
  MOD_LCD6610_sendData(conn, 0x03); /* 12-bits per pixel */
  MOD_LCD6610_releaseCS();
  /* Memory access controller */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND1_COLMOD);
  MOD_LCD6610_sendData(conn, 0xC8); /* C0 - mirror x and y, reverse RGB */
  MOD_LCD6610_releaseCS();
  /* Write contrast */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND1_SETCON);
  MOD_LCD6610_sendData(conn, 0x30);
  MOD_LCD6610_releaseCS();
  /* Wait to stabilize */
  for(uint32_t dly=0; dly < 1000; dly++);
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND1_DISPON);
  MOD_LCD6610_releaseCS();

#endif

  return MOD_LCD6610_RESULT_OK;
}

/*************************************************************************
 * Function Name: MOD_LCD6610_setWindow
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                int16_t x1 - upper left x coordinate of the window area
 *                int16_t y1 - upper left y coordinate of the window area
 *                int16_t x2 - lower right x coordinate of the window area
 *                int16_t y2 - lower right y coordinate of the window area
 * Return:        none
 *
 * Description: Set window area on the display for drawing.
 *************************************************************************/
void MOD_LCD6610_setWindow(ConnectionHandler_t * conn, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  MOD_LCD6610_data->winX1 = x1;
  MOD_LCD6610_data->winY1 = y1;
  MOD_LCD6610_data->winX2 = x2;
  MOD_LCD6610_data->winY2 = y2;
}

/*************************************************************************
 * Function Name: MOD_LCD6610_setTextPosition
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                int16_t x - upper left x coordinate of the text area
 *                int16_t y - upper left y coordinate of the text area
 * Return:        none
 *
 * Description: Set start position of the text that will be written.
 *************************************************************************/
void MOD_LCD6610_setTextPosition(ConnectionHandler_t * conn, int16_t x, int16_t y)
{
  MOD_LCD6610_data->textX = x;
  MOD_LCD6610_data->textY = y;
}

/*************************************************************************
 * Function Name: MOD_LCD6610_sendBlock
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint8_t * pData - pointer to data buffer to be sent
 *                uint16_t size - count of bytes to be sent
 * Return:        none
 *
 * Description: Sends a block of data bytes to LCD6610 controller
 *************************************************************************/
void MOD_LCD6610_sendBlock(ConnectionHandler_t * conn, uint8_t * pData, uint16_t size)
{
  while(size--)
  {
    MOD_LCD6610_sendData(conn, *pData++);
  }
}

/*************************************************************************
 * Function Name: MOD_LCD6610_setFont
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                const FontType_t * font - font to be set as default font
 * Return:        none
 *
 * Description: Sets default font used for text output.
 *************************************************************************/
void MOD_LCD6610_setFont(ConnectionHandler_t * conn, const FontType_t * font)
{
  MOD_LCD6610_data->font = font;
}

/*************************************************************************
 * Function Name: MOD_LCD6610_setTextColor
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint16_t color - text color, format (0x0RGB)
 * Return:        none
 *
 * Description: Sets default text color used for text output.
 *************************************************************************/
void MOD_LCD6610_setTextColor(ConnectionHandler_t * conn, uint16_t color)
{
  MOD_LCD6610_data->textColor = color;
}

/*************************************************************************
 * Function Name: MOD_LCD6610_setTextBackground
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint16_t background - text background color, format (0x0RGB)
 * Return:        none
 *
 * Description: Sets default background color used for text output.
 *************************************************************************/
void MOD_LCD6610_setTextBackground(ConnectionHandler_t * conn, uint16_t background)
{
  MOD_LCD6610_data->textBackground = background;
}

/*************************************************************************
 * Function Name: MOD_LCD6610_putPixel
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint16_t pixel - color of the pixel, format (0x0RGB)
 * Return:        none
 *
 * Description: Write a single pixel to the LCD display at current position.
 *************************************************************************/
void MOD_LCD6610_putPixel(ConnectionHandler_t * conn, uint16_t pixel)
{
uint32_t pixel32 = pixel;

  if(MOD_LCD6610_data->newStream)
  {
    /* Saves the first pixel in the window stream, in case the pixels are
       odd count. The pixel is flushed out after the last pixel in the stream.
       This workarounds the problem with odd count of pixels. */
    MOD_LCD6610_data->firstPixel = pixel;
  }

  if(!MOD_LCD6610_data->oddPixel)
  {
    MOD_LCD6610_data->holdPixel  = (pixel32>>4)&0x0000FF;
    MOD_LCD6610_data->holdPixel |= (pixel32<<12)&0x00F000;
  }
  else
  {
    MOD_LCD6610_data->holdPixel |= (pixel32)&0x000F00;
    MOD_LCD6610_data->holdPixel |= (pixel32<<16)&0xFF0000;
    MOD_LCD6610_sendBlock(conn, (uint8_t*)&(MOD_LCD6610_data->holdPixel), 3);
  }
  /* Toggle odd flag */
  MOD_LCD6610_data->oddPixel ^= 0x01;
  MOD_LCD6610_data->newStream = 0;
}

/*************************************************************************
 * Function Name: MOD_LCD6610_flushPixels
 * Parameters:    ConnectionHandler_t *conn - connection handler
 * Return:        none
 *
 * Description: If the count of outputted pixels is odd, this function writes
 *              again the first pixel in the window area, to finish the data transfer.
 *************************************************************************/
void MOD_LCD6610_flushPixels(ConnectionHandler_t * conn)
{
  if(MOD_LCD6610_data->oddPixel)
  {
    MOD_LCD6610_data->holdPixel |= (MOD_LCD6610_data->firstPixel)&0x000F00;
    MOD_LCD6610_data->holdPixel |= (MOD_LCD6610_data->firstPixel<<16)&0xFF0000;
    MOD_LCD6610_sendBlock(conn, (uint8_t*)&(MOD_LCD6610_data->holdPixel), 3);
    MOD_LCD6610_data->oddPixel = 0;
  }
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_NOP);
  MOD_LCD6610_data->newStream = 1;
}

/*************************************************************************
 * Function Name: MOD_LCD6610_fillWindow
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint16_t color - color of the window area, format (0x0RGB)
 * Return:        none
 *
 * Description: Fills the window area with a color.
 *************************************************************************/
void MOD_LCD6610_fillWindow(ConnectionHandler_t * conn, uint16_t color)
{
uint16_t pixels = (MOD_LCD6610_data->winX2 - MOD_LCD6610_data->winX1 + 1) * (MOD_LCD6610_data->winY2 - MOD_LCD6610_data->winY1 + 1);

  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_CASET);
  MOD_LCD6610_sendData(conn, MOD_LCD6610_data->winX1);
  MOD_LCD6610_sendData(conn, MOD_LCD6610_data->winX2);
  MOD_LCD6610_releaseCS();

  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_PASET);
  MOD_LCD6610_sendData(conn, MOD_LCD6610_data->winY1);
  MOD_LCD6610_sendData(conn, MOD_LCD6610_data->winY2);
  MOD_LCD6610_releaseCS();

  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_RAMWR);
  for(uint16_t i=0; i<pixels; i++) MOD_LCD6610_putPixel(conn, color);
  MOD_LCD6610_flushPixels(conn);
  MOD_LCD6610_releaseCS();
}

/*************************************************************************
 * Function Name: MOD_LCD6610_putChar
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                const char chr - char to be outputted
 * Return:        none
 *
 * Description: Outputs single character to the current text position.
 *************************************************************************/
void MOD_LCD6610_putChar(ConnectionHandler_t * conn, const char chr)
{
int32_t ch;
uint8_t * pCharData;

  ch = chr - MOD_LCD6610_data->font->CharacterOffset;

  /* Data control */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_DATCTL);
  MOD_LCD6610_sendData(conn, 0x04); /* P1: 0x00 = page address normal, col address normal, address scan in row/page direction */
  MOD_LCD6610_sendData(conn, 0x00); /* P2: 0x00 = RGB sequence (default value) */
  MOD_LCD6610_sendData(conn, 0x00); /* P3: 0x00 = Color type */
  MOD_LCD6610_releaseCS();

  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_CASET);
  MOD_LCD6610_sendData(conn, MOD_LCD6610_data->textX + MOD_LCD6610_data->winX1);
  MOD_LCD6610_sendData(conn, MOD_LCD6610_data->textX + MOD_LCD6610_data->winX1 + MOD_LCD6610_data->font->H_Size-1);
  MOD_LCD6610_releaseCS();

  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_PASET);
  MOD_LCD6610_sendData(conn, MOD_LCD6610_data->textY + MOD_LCD6610_data->winY1);
  MOD_LCD6610_sendData(conn, MOD_LCD6610_data->textY + MOD_LCD6610_data->winY1 + MOD_LCD6610_data->font->V_Size-1);
  MOD_LCD6610_releaseCS();

  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_RAMWR);
  for(uint16_t x=0; x<MOD_LCD6610_data->font->H_Size; x++)
  {
    pCharData = MOD_LCD6610_data->font->pFontStream + (ch*MOD_LCD6610_data->font->H_Size) + x;
    for(uint16_t y=0; y<MOD_LCD6610_data->font->V_Size; y++)
    {
      if((*pCharData>>y)&0x01)
        MOD_LCD6610_putPixel(conn, MOD_LCD6610_data->textColor);
      else
        MOD_LCD6610_putPixel(conn, MOD_LCD6610_data->textBackground);
    }
  }
  MOD_LCD6610_flushPixels(conn);
  MOD_LCD6610_releaseCS();
}

/*************************************************************************
 * Function Name: MOD_LCD6610_writeString
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                const char *str - pointer to text string
 * Return:        none
 *
 * Description: Outputs a text string to the current text position.
 *************************************************************************/
void MOD_LCD6610_writeString(ConnectionHandler_t * conn, const char *str)
{
  while(*str)
  {
    if (*str == 0x0D)
    {
      /* Carriage return */
      MOD_LCD6610_data->textX = 0;
    }
    else if (*str == 0x0A)
    {
      /* Line feed */
      MOD_LCD6610_data->textY += MOD_LCD6610_data->font->V_Size + MOD_LCD6610_LINE_SPACING;
      if((MOD_LCD6610_data->textY + MOD_LCD6610_data->winY1) > (MOD_LCD6610_data->winY2 - MOD_LCD6610_data->font->V_Size + 1))
        MOD_LCD6610_data->textY = 0;
    }
    else
    {
      /* Any other character */
      MOD_LCD6610_putChar(conn, *str);
      /* Update X and Y values */
      if (((MOD_LCD6610_data->textX += MOD_LCD6610_data->font->H_Size + MOD_LCD6610_CHAR_SPACING) + MOD_LCD6610_data->winX1) > (MOD_LCD6610_data->winX2 - MOD_LCD6610_data->font->H_Size))
      {
        MOD_LCD6610_data->textX = 0;
        if (((MOD_LCD6610_data->textY += MOD_LCD6610_data->font->V_Size + MOD_LCD6610_LINE_SPACING) + MOD_LCD6610_data->winY1) > (MOD_LCD6610_data->winY2 - MOD_LCD6610_data->font->V_Size + 1))
          MOD_LCD6610_data->textY = 0;
      }
    }
    str++;
  }
}

/*************************************************************************
 * Function Name: MOD_LCD6610_drawBitmap
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                BitmapColorType_t * bitmap - pointer to the bitmap
 *                uint16_t x - x coordinate of bitmap (upper left)
 *                uint16_t y - y coordinate of bitmap (upper left)
 * Return:        none
 *
 * Description: Outputs a bitmap to the display. Coordinates are in the current
 *              window area.
 *************************************************************************/
void MOD_LCD6610_drawBitmap(ConnectionHandler_t * conn, const BitmapColorType_t * bitmap, uint16_t x, uint16_t y)
{
uint32_t pixel=0;
uint16_t bmpX1, bmpX2, bmpY1, bmpY2;

  bmpX1 = MOD_LCD6610_data->winX1 + x;
  bmpX2 = MOD_LCD6610_data->winX1 + x + bitmap->H_Size - 1;
  if (bmpX2 > MOD_LCD6610_data->winX2) bmpX2 = MOD_LCD6610_data->winX2;
  bmpY1 = MOD_LCD6610_data->winY1 + y;
  bmpY2 = MOD_LCD6610_data->winY1 + y + bitmap->V_Size - 1;
  if (bmpY2 > MOD_LCD6610_data->winY2) bmpY2 = MOD_LCD6610_data->winY2;

  /* Data control */
  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_DATCTL);
  MOD_LCD6610_sendData(conn, 0x00); /* P1: 0x00 = page address normal, col address normal, address scan in page/row direction */
  MOD_LCD6610_sendData(conn, 0x00); /* P2: 0x00 = RGB sequence (default value) */
  MOD_LCD6610_sendData(conn, 0x00); /* P3: 0x02 = Grayscale -> 16 (selects 12-bit color, type A) */
  MOD_LCD6610_releaseCS();

  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_CASET);
  MOD_LCD6610_sendData(conn, bmpX1);
  MOD_LCD6610_sendData(conn, bmpX2);
  MOD_LCD6610_releaseCS();

  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_PASET);
  MOD_LCD6610_sendData(conn, bmpY1);
  MOD_LCD6610_sendData(conn, bmpY2);
  MOD_LCD6610_releaseCS();

  MOD_LCD6610_assertCS();
  MOD_LCD6610_sendCmd(conn, MOD_LCD6610_COMMAND0_RAMWR);
  for(uint16_t yn=0; yn < bitmap->V_Size; yn++)
  {
    for(uint16_t xn=0; xn < bitmap->H_Size; xn++, pixel++)
    {
      if (((bmpX1+xn) <= bmpX2) && ((bmpY1+yn) <= bmpY2))
        MOD_LCD6610_putPixel(conn, *((uint16_t*)(bitmap->pPicStream) + pixel));
    }
  }
  MOD_LCD6610_flushPixels(conn);
  MOD_LCD6610_releaseCS();
}
