/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : snake_hw.h
 *    Description : Snake-game hardware related functions header
 *
 *    History :
 *    1. Date        : 28, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 4202 $
 **************************************************************************/

#ifndef __SNAKE_HW_H
#define __SNAKE_HW_H

#include <stdint.h>
#include "../modules/mod_lcd6610.h"
#include "../modules/mod_joystick.h"

#define SNAKE_SCREEN_MAX_X        131
#define SNAKE_SCREEN_MAX_Y        131

#define SNAKE_READCTRL_RES_OK     0
#define SNAKE_READCTRL_RES_ERROR  1

#define SNAKE_READCTRL_TRIES      200

#define SNAKE_READCTRL_LO_LIMIT   0
#define SNAKE_READCTRL_HI_LIMIT   255
#define SNAKE_READCTRL_THRESHOLD  10

typedef struct
{
  uint8_t direction;
  uint8_t key0;
  uint8_t key1;
} ControlsData_t;

/*************************************************************************
 * Function Name: Snake_hwInit
 * Parameters:    ConnectionHandler_t dispHandler - display handler
 *                ConnectionHandler_t ctrlHandler - joystick handler
 * Return:        none
 *
 * Description: Initialize hardware needed for the game engine.
 *************************************************************************/
void Snake_hwInit(ConnectionHandler_t * dispHandler, ConnectionHandler_t * ctrlHandler);

/*************************************************************************
 * Function Name: Snake_drawBitmap
 * Parameters:    BitmapColorType_t bmp - bitmap to display
 *                uint32_t x - x coord. in screen pixels
 *                uint32_t y - y coord. in screen pixels
 * Return:        none
 *
 * Description: Draws bitmap on the screen
 *************************************************************************/
void Snake_drawBitmap(BitmapColorType_t * bmp, uint32_t x, uint32_t y);

/*************************************************************************
 * Function Name: Snake_printScore
 * Parameters:    uint16_t score - score to display
 * Return:        none
 *
 * Description: Displays game score on the screen
 *************************************************************************/
void Snake_printScore(uint16_t score);

/*************************************************************************
 * Function Name: Snake_printMessage
 * Parameters:    uint8_t totalLines
 *                uint8_t currentLine
                  const char * message
 * Return:        none
 *
 * Description: Displays message on "currentLine". It places a block of
 *              "totalLines" at the middle of the screen. Actually - little
 *              lower to fit in the middle between the walls.
 *************************************************************************/
void Snake_printMessage(uint8_t totalLines, uint8_t currentLine, const char * message);

/*************************************************************************
 * Function Name: Snake_printLevel
 * Parameters:    uint16_t level - level to display
 * Return:        none
 *
 * Description: Displays the game level on the screen
 *************************************************************************/
void Snake_printLevel(uint16_t level);

/*************************************************************************
 * Function Name: Snake_clearScreen
 * Parameters: none
 * Return: none
 *
 * Description: Clears the whole screen.
 *************************************************************************/
void Snake_clearScreen(void);

/*************************************************************************
 * Function Name: Snake_readControls
 * Parameters:    ControlsData_t * controls - pointer to controls data
 *                                            structure
 * Return:        uint32_t - result of reading:
 *                           SNAKE_READCTRL_RES_OK - OK
 *                           SNAKE_READCTRL_RES_ERROR - error
 *
 * Description: Reads control(joystick).
 *************************************************************************/
uint32_t Snake_readControls(ControlsData_t * controls);

#endif
