/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : mod_lcd6610.h
 *    Description : MOD-LCD6610 Graphic Library header
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 4285 $
 **************************************************************************/

#ifndef __MOD_LCD6610_H
#define __MOD_LCD6610_H

#include "../framework/platform/platform_config.h"
#include "../framework/font.h"
#include "../framework/bitmap.h"
#include "../framework/connect.h"

#define MOD_LCD6610_PIN_RES  pinTX
#define MOD_LCD6610_PIN_BL   pinRX
#define MOD_LCD6610_PIN_MISO pinMISO
#define MOD_LCD6610_PIN_MOSI pinMOSI
#define MOD_LCD6610_PIN_SCK  pinSCK
#define MOD_LCD6610_PIN_CS   pinSSEL

#define MOD_LCD6610_RESULT_OK         0
#define MOD_LCD6610_RESULT_COMM_ERROR 1
#define MOD_LCD6610_RESULT_MEM_ERROR  2

#define MOD_LCD6610_COLS              132
#define MOD_LCD6610_ROWS              132

#define MOD_LCD6610_CHAR_SPACING      1
#define MOD_LCD6610_LINE_SPACING      1

#define MOD_LCD6610_VARIANT_EPSON     0
#define MOD_LCD6610_VARIANT_PHILIPS   1

#define MOD_LCD6610_VARIANT           MOD_LCD6610_VARIANT_EPSON

#if (MOD_LCD6610_VARIANT == MOD_LCD6610_VARIANT_EPSON)

/* S1D15G00 COMMANDS */

#define MOD_LCD6610_COMMAND0_DISON     0xAF // Display on
#define MOD_LCD6610_COMMAND0_DISOFF    0xAE // Display off
#define MOD_LCD6610_COMMAND0_DISNOR    0xA6 // Normal display
#define MOD_LCD6610_COMMAND0_DISINV    0xA7 // Inverse display
#define MOD_LCD6610_COMMAND0_COMSCN    0xBB // Common scan direction
#define MOD_LCD6610_COMMAND0_DISCTL    0xCA // Display control
#define MOD_LCD6610_COMMAND0_SLPIN     0x95  // Sleep in
#define MOD_LCD6610_COMMAND0_SLPOUT    0x94 // Sleep out
#define MOD_LCD6610_COMMAND0_PASET     0x75 // Page address set
#define MOD_LCD6610_COMMAND0_CASET     0x15 // Column address set
#define MOD_LCD6610_COMMAND0_DATCTL    0xBC // Data scan direction, etc.
#define MOD_LCD6610_COMMAND0_RGBSET8   0xCE // 256-color position set
#define MOD_LCD6610_COMMAND0_RAMWR     0x5C // Writing to memory
#define MOD_LCD6610_COMMAND0_RAMRD     0x5D // Reading from memory
#define MOD_LCD6610_COMMAND0_PTLIN     0xA8 // Partial display in
#define MOD_LCD6610_COMMAND0_PTLOUT    0xA9 // Partial display out
#define MOD_LCD6610_COMMAND0_RMWIN     0xE0 // Read and modify write
#define MOD_LCD6610_COMMAND0_RMWOUT    0xEE // End
#define MOD_LCD6610_COMMAND0_ASCSET    0xAA // Area scroll set
#define MOD_LCD6610_COMMAND0_SCSTART   0xAB // Scroll start set
#define MOD_LCD6610_COMMAND0_OSCON     0xD1 // Internal oscillation on
#define MOD_LCD6610_COMMAND0_OSCOFF    0xD2 // Internal oscillation off
#define MOD_LCD6610_COMMAND0_PWRCTR    0x20 // Power control
#define MOD_LCD6610_COMMAND0_VOLCTR    0x81 // Electronic volume control
#define MOD_LCD6610_COMMAND0_VOLUP     0xD6 // Increment electronic control by 1
#define MOD_LCD6610_COMMAND0_VOLDOWN   0xD7 // Decrement electronic control by 1
#define MOD_LCD6610_COMMAND0_TMPGRD    0x82 // Temperature gradient set
#define MOD_LCD6610_COMMAND0_EPCTIN    0xCD // Control EEPROM
#define MOD_LCD6610_COMMAND0_EPCOUT    0xCC // Cancel EEPROM control
#define MOD_LCD6610_COMMAND0_EPMWR     0xFC // Write into EEPROM
#define MOD_LCD6610_COMMAND0_EPMRD     0xFD // Read from EEPROM
#define MOD_LCD6610_COMMAND0_EPSRRD1   0x7C // Read register 1
#define MOD_LCD6610_COMMAND0_EPSRRD2   0x7D // Read register 2
#define MOD_LCD6610_COMMAND0_NOP       0x25 // NOP instruction

#elif (MOD_LCD6610_VARIANT == MOD_LCD6610_VARIANT_PHILIPS)

/* PCF8833 COMMANDS */

#define MOD_LCD6610_COMMAND1_NOP       0x00 // nop
#define MOD_LCD6610_COMMAND1_SWRESET   0x01 // software reset
#define MOD_LCD6610_COMMAND1_BSTROFF   0x02 // booster voltage OFF
#define MOD_LCD6610_COMMAND1_BSTRON    0x03 // booster voltage ON
#define MOD_LCD6610_COMMAND1_RDDIDIF   0x04 // read display identification
#define MOD_LCD6610_COMMAND1_RDDST     0x09 // read display status
#define MOD_LCD6610_COMMAND1_SLEEPIN   0x10 // sleep in
#define MOD_LCD6610_COMMAND1_SLEEPOUT  0x11 // sleep out
#define MOD_LCD6610_COMMAND1_PTLON     0x12 // partial display mode
#define MOD_LCD6610_COMMAND1_NORON     0x13 // display normal mode
#define MOD_LCD6610_COMMAND1_INVOFF    0x20 // inversion OFF
#define MOD_LCD6610_COMMAND1_INVON     0x21 // inversion ON
#define MOD_LCD6610_COMMAND1_DALO      0x22 // all pixel OFF
#define MOD_LCD6610_COMMAND1_DAL       0x23 // all pixel ON
#define MOD_LCD6610_COMMAND1_SETCON    0x25 // write contrast
#define MOD_LCD6610_COMMAND1_DISPOFF   0x28 // display OFF
#define MOD_LCD6610_COMMAND1_DISPON    0x29 // display ON
#define MOD_LCD6610_COMMAND1_CASET     0x2A // column address set
#define MOD_LCD6610_COMMAND1_PASET     0x2B // page address set
#define MOD_LCD6610_COMMAND1_RAMWR     0x2C // memory write
#define MOD_LCD6610_COMMAND1_RGBSET    0x2D // colour set
#define MOD_LCD6610_COMMAND1_PTLAR     0x30 // partial area
#define MOD_LCD6610_COMMAND1_VSCRDEF   0x33 // vertical scrolling definition
#define MOD_LCD6610_COMMAND1_TEOFF     0x34 // test mode
#define MOD_LCD6610_COMMAND1_TEON      0x35 // test mode
#define MOD_LCD6610_COMMAND1_MADCTL    0x36 // memory access control
#define MOD_LCD6610_COMMAND1_SEP       0x37 // vertical scrolling start address
#define MOD_LCD6610_COMMAND1_IDMOFF    0x38 // idle mode OFF
#define MOD_LCD6610_COMMAND1_IDMON     0x39 // idle mode ON
#define MOD_LCD6610_COMMAND1_COLMOD    0x3A // interface pixel format
#define MOD_LCD6610_COMMAND1_SETVOP    0xB0 // set Vop
#define MOD_LCD6610_COMMAND1_BRS       0xB4 // bottom row swap
#define MOD_LCD6610_COMMAND1_TRS       0xB6 // top row swap
#define MOD_LCD6610_COMMAND1_DISCTR    0xB9 // display control
#define MOD_LCD6610_COMMAND1_DOR       0xBA // data order
#define MOD_LCD6610_COMMAND1_TCDFE     0xBD // enable/disable DF temperature compensation
#define MOD_LCD6610_COMMAND1_TCVOPE    0xBF // enable/disable Vop temp comp
#define MOD_LCD6610_COMMAND1_EC        0xC0 // internal or external oscillator
#define MOD_LCD6610_COMMAND1_SETMUL    0xC2 // set multiplication factor
#define MOD_LCD6610_COMMAND1_TCVOPAB   0xC3 // set TCVOP slopes A and B
#define MOD_LCD6610_COMMAND1_TCVOPCD   0xC4 // set TCVOP slopes c and d
#define MOD_LCD6610_COMMAND1_TCDF      0xC5 // set divider frequency
#define MOD_LCD6610_COMMAND1_DF8COLOR  0xC6 // set divider frequency 8-color mode
#define MOD_LCD6610_COMMAND1_SETBS     0xC7 // set bias system
#define MOD_LCD6610_COMMAND1_RDTEMP    0xC8 // temperature read back
#define MOD_LCD6610_COMMAND1_NLI       0xC9 // n-line inversion #define RDID1 0xDA // read ID1
#define MOD_LCD6610_COMMAND1_RDID2     0xDB // read ID2
#define MOD_LCD6610_COMMAND1_RDID3     0xDC // read ID3

#endif

typedef struct {
     GPIO_TypeDef * port_cs;
     GPIO_TypeDef * port_sck;
     GPIO_TypeDef * port_mosi;
         uint32_t   pin_cs;
         uint32_t   pin_sck;
         uint32_t   pin_mosi;
          int16_t   winX1;
          int16_t   winX2;
          int16_t   winY1;
          int16_t   winY2;
          int16_t   textX;
          int16_t   textY;
const FontType_t * font;
         uint16_t   textColor;
         uint16_t   textBackground;
         uint32_t   holdPixel;
          uint8_t   oddPixel;
          uint8_t   newStream;
         uint16_t   firstPixel;
} MOD_LCD6610_data_t;

extern MODInfo_t * MOD_LCD6610;

extern ParametersGroupInfo_t MOD_LCD6610_PG;

#define MOD_LCD6610_sendCmd(conn, cmd)   MOD_LCD6610_transfer(conn, cmd,  0);

#define MOD_LCD6610_sendData(conn, data) MOD_LCD6610_transfer(conn, data, 1);

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
uint32_t MOD_LCD6610_init(ConnectionHandler_t * conn);

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
void MOD_LCD6610_setWindow(ConnectionHandler_t * conn, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

/*************************************************************************
 * Function Name: MOD_LCD6610_setTextPosition
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                int16_t x - upper left x coordinate of the text area
 *                int16_t y - upper left y coordinate of the text area
 * Return:        none
 *
 * Description: Set start position of the text that will be written.
 *************************************************************************/
void MOD_LCD6610_setTextPosition(ConnectionHandler_t * conn, int16_t x, int16_t y);

/*************************************************************************
 * Function Name: MOD_LCD6610_sendBlock
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint8_t * pData - pointer to data buffer to be sent
 *                uint16_t size - count of bytes to be sent
 * Return:        none
 *
 * Description: Sends a block of data bytes to LCD6610 controller
 *************************************************************************/
void MOD_LCD6610_sendBlock(ConnectionHandler_t * conn, uint8_t * pData, uint16_t size);

/*************************************************************************
 * Function Name: MOD_LCD6610_setFont
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                const FontType_t * font - font to be set as default font
 * Return:        none
 *
 * Description: Sets default font used for text output.
 *************************************************************************/
void MOD_LCD6610_setFont(ConnectionHandler_t * conn, const FontType_t * font);

/*************************************************************************
 * Function Name: MOD_LCD6610_setTextColor
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint16_t color - text color, format (0x0RGB)
 * Return:        none
 *
 * Description: Sets default text color used for text output.
 *************************************************************************/
void MOD_LCD6610_setTextColor(ConnectionHandler_t * conn, uint16_t color);

/*************************************************************************
 * Function Name: MOD_LCD6610_setTextBackground
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint16_t background - text background color, format (0x0RGB)
 * Return:        none
 *
 * Description: Sets default background color used for text output.
 *************************************************************************/
void MOD_LCD6610_setTextBackground(ConnectionHandler_t * conn, uint16_t background);

/*************************************************************************
 * Function Name: MOD_LCD6610_putPixel
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint16_t pixel - color of the pixel, format (0x0RGB)
 * Return:        none
 *
 * Description: Write a single pixel to the LCD display at current position.
 *************************************************************************/
void MOD_LCD6610_putPixel(ConnectionHandler_t * conn, uint16_t pixel);

/*************************************************************************
 * Function Name: MOD_LCD6610_flushPixels
 * Parameters:    ConnectionHandler_t *conn - connection handler
 * Return:        none
 *
 * Description: If the count of outputted pixels is odd, this function writes
 *              again the first pixel in the window area, to finish the data transfer.
 *************************************************************************/
void MOD_LCD6610_flushPixels(ConnectionHandler_t * conn);

/*************************************************************************
 * Function Name: MOD_LCD6610_fillWindow
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint16_t color - color of the window area, format (0x0RGB)
 * Return:        none
 *
 * Description: Fills the window area with a color.
 *************************************************************************/
void MOD_LCD6610_fillWindow(ConnectionHandler_t * conn, uint16_t color);

/*************************************************************************
 * Function Name: MOD_LCD6610_putChar
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                const char chr - char to be outputted
 * Return:        none
 *
 * Description: Outputs single character to the current text position.
 *************************************************************************/
void MOD_LCD6610_putChar(ConnectionHandler_t * conn, const char chr);

/*************************************************************************
 * Function Name: MOD_LCD6610_writeString
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                const char *str - pointer to text string
 * Return:        none
 *
 * Description: Outputs a text string to the current text position.
 *************************************************************************/
void MOD_LCD6610_writeString(ConnectionHandler_t * conn, const char *str);

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
void MOD_LCD6610_drawBitmap(ConnectionHandler_t * conn, const BitmapColorType_t * bitmap, uint16_t x, uint16_t y);

#endif /* __MOD_LCD6610_H */
