/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : mod_lcd3310.h
 *    Description : MOD-LCD3310 library header
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 2673 $
 **************************************************************************/

#ifndef __MOD_LCD3310_H
#define __MOD_LCD3310_H

#include "../framework/font.h"
#include "../framework/bitmap.h"
#include "../framework/connect.h"

#define MOD_LCD3310_RESULT_OK         0
#define MOD_LCD3310_RESULT_COMM_ERROR 1
#define MOD_LCD3310_RESULT_MEM_ERROR  2

#define MOD_LCD3310_START_LINE_ADDR  (66-2)
#define MOD_LCD3310_COLS 84
#define MOD_LCD3310_ROWS 48

#define MOD_LCD3310_CHAR_SPACING 1

#define MOD_LCD3310_PIN_RES pinSCL
#define MOD_LCD3310_PIN_DC  pinSDA
#define MOD_LCD3310_PIN_CS  pinSSEL

extern MODInfo_t * MOD_LCD3310;

extern ParametersGroupInfo_t MOD_LCD3310_PG;

typedef struct
{
  const FontType_t * font;
  uint8_t pixelData[MOD_LCD3310_COLS*MOD_LCD3310_ROWS/8];
  uint8_t graphX, graphY, textX, textY;
  uint8_t charSpacing;
} MOD_LCD3310_data_t;

/*************************************************************************
 * Function Name: MOD_LCD3310_init
 * Parameters:    ConnectionHandler_t * conn - connection handler
 * Return:        uint32_t - result of operation
 *                MOD_LCD3310_RESULT_OK - operation completes successfully
 *                MOD_LCD3310_RESULT_MEM_ERROR - not enough heap memory
 *
 * Description: Initialize the MOD-LCD3310 display module.
 *************************************************************************/
uint32_t MOD_LCD3310_init(ConnectionHandler_t * conn);

/*************************************************************************
 * Function Name: MOD_LCD3310_update
 * Parameters:    ConnectionHandler_t * conn - connection handler
 * Return:        none
 *
 * Description: Draw the current display buffer (MOD_LCD3310_data->pixelData).
 *************************************************************************/
void MOD_LCD3310_update(ConnectionHandler_t * conn);

/*************************************************************************
 * Function Name: MOD_LCD3310_clear
 * Parameters:    ConnectionHandler_t * conn - connection handler
 * Return:        none
 *
 * Description: Clears the screen.
 *************************************************************************/
void MOD_LCD3310_clear(ConnectionHandler_t * conn);

/*************************************************************************
 * Function Name: MOD_LCD3310_displayDirect
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                const char chr - coordinate (in pixels)
 * Return:        none
 *
 * Description: Outputs a raw data stream to the display. Every byte
 *              represents 8 vertical dots.
 *************************************************************************/
void MOD_LCD3310_displayDirect(ConnectionHandler_t * conn, uint8_t *buffer, uint8_t x, uint8_t y, int16_t len);

/*************************************************************************
 * Function Name: MOD_LCD3310_setFont
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                const FontType_t * font - font to be set
 * Return:        none
 *
 * Description: Sets the current font.
 *************************************************************************/
void MOD_LCD3310_setFont(ConnectionHandler_t * conn, const FontType_t * font);

/*************************************************************************
 * Function Name: MOD_LCD3310_putChar
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                const char chr - coordinate (in pixels)
 *                const FontType_t * font - font to use
 * Return:        none
 *
 * Description: Outputs a single character to the display.
 *************************************************************************/
void MOD_LCD3310_putChar(ConnectionHandler_t * conn, const char chr, const FontType_t * font);

/*************************************************************************
 * Function Name: MOD_LCD3310_writeString
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                const char *str - string
 * Return:        none
 *
 * Description: Outputs a string to the display at the current text position.
 *************************************************************************/
void MOD_LCD3310_writeString(ConnectionHandler_t * conn, const char *str);

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
void MOD_LCD3310_setTextPosition(ConnectionHandler_t * conn, uint8_t x, uint8_t y);

/*************************************************************************
 * Function Name: MOD_LCD3310_setCharSpacing
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t cs - char spacing (in pixels)
 * Return:        none
 *
 * Description: Sets the current char spacing.
 *************************************************************************/
void MOD_LCD3310_setCharSpacing(ConnectionHandler_t * conn, uint8_t cs);

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
void MOD_LCD3310_putPixel(ConnectionHandler_t * conn, uint8_t x, uint8_t y, uint8_t state);

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
void MOD_LCD3310_hLine(ConnectionHandler_t * conn, uint8_t x1, uint8_t x2, uint8_t y, uint8_t state);

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
void MOD_LCD3310_vLine(ConnectionHandler_t * conn, uint8_t y1, uint8_t y2, uint8_t x, uint8_t state);

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
void MOD_LCD3310_line(ConnectionHandler_t * conn, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t state);

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
void MOD_LCD3310_rectangle(ConnectionHandler_t * conn, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t state);

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
void MOD_LCD3310_filledRectangle(ConnectionHandler_t * conn, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t state);

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
void MOD_LCD3310_ellipse(ConnectionHandler_t * conn, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t state);

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
void MOD_LCD3310_filledEllipse(ConnectionHandler_t * conn, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t state);

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
void MOD_LCD3310_drawBitmap(ConnectionHandler_t * conn, uint8_t x, uint8_t y, const BitmapMonoType_t * bitmap);

#endif /* __MOD_LCD3310_H */
