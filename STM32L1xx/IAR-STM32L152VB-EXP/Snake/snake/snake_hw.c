/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : snake_hw.c
 *    Description : Snake-game hardware related functions
 *
 *    History :
 *    1. Date        : 28, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "snake_hw.h"
#include "snake_core.h"

ConnectionHandler_t * displayHandler;
ConnectionHandler_t * controlHandler;

extern const FontType_t Console_5_7;

/*************************************************************************
 * Function Name: Snake_hwInit
 * Parameters:    ConnectionHandler_t dispHandler - display handler
 *                ConnectionHandler_t ctrlHandler - joystick handler
 * Return:        none
 *
 * Description: Initialize hardware needed for the game engine.
 *************************************************************************/
void Snake_hwInit(ConnectionHandler_t * dispHandler, ConnectionHandler_t * ctrlHandler)
{
  displayHandler = dispHandler;
  controlHandler = ctrlHandler;
  /* Init Display */
  MOD_LCD6610_init(displayHandler);
  MOD_LCD6610_setFont(displayHandler, &Console_5_7);
  MOD_LCD6610_setWindow(displayHandler, 0, 0, SNAKE_SCREEN_MAX_X, SNAKE_SCREEN_MAX_Y);
  MOD_LCD6610_fillWindow(displayHandler, 0x000);
  MOD_LCD6610_setTextColor(displayHandler, 0x0FFF);
  MOD_LCD6610_setTextBackground(displayHandler, 0x0000);
  /* Init Joystick */
  MOD_JOYSTICK_init(controlHandler);
}

/*************************************************************************
 * Function Name: Snake_drawBitmap
 * Parameters:    BitmapColorType_t bmp - bitmap to display
 *                uint32_t x - x coord. in screen pixels
 *                uint32_t y - y coord. in screen pixels
 * Return:        none
 *
 * Description: Draws bitmap on the screen
 *************************************************************************/
void Snake_drawBitmap(BitmapColorType_t * bmp, uint32_t x, uint32_t y)
{
  MOD_LCD6610_drawBitmap(displayHandler, bmp, x, y);
}

/*************************************************************************
 * Function Name: Snake_printScore
 * Parameters:    uint16_t score - score to display
 * Return:        none
 *
 * Description: Displays game score on the screen
 *************************************************************************/
void Snake_printScore(uint16_t score)
{
char str_sc[11];

  MOD_LCD6610_setTextPosition(displayHandler, 70, 3);
  sprintf(str_sc, "Score:%04i",score);
  MOD_LCD6610_writeString(displayHandler, str_sc);
}

/*************************************************************************
 * Function Name: Snake_printLevel
 * Parameters:    uint16_t level - level to display
 * Return:        none
 *
 * Description: Displays the game level on the screen
 *************************************************************************/
void Snake_printLevel(uint16_t level)
{
char str_sc[11];

  MOD_LCD6610_setTextPosition(displayHandler, 1, 3);
  sprintf(str_sc, "Level:%i",level);
  MOD_LCD6610_writeString(displayHandler, str_sc);
}

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
void Snake_printMessage(uint8_t totalLines, uint8_t currentLine, const char * message)
{
uint8_t x = (132 - strlen(message)*(Console_5_7.H_Size+1)) / 2;
  MOD_LCD6610_setTextPosition(displayHandler, x, (80-(totalLines*10))/2 + 30 + currentLine*10);
  MOD_LCD6610_writeString(displayHandler, message);
}

/*************************************************************************
 * Function Name: Snake_clearScreen
 * Parameters: none
 * Return: none
 *
 * Description: Clears the whole screen.
 *************************************************************************/
void Snake_clearScreen(void)
{
  MOD_LCD6610_setWindow(displayHandler, 0, 0, SNAKE_SCREEN_MAX_X, SNAKE_SCREEN_MAX_Y);
  MOD_LCD6610_fillWindow(displayHandler, 0x000);
}

/*************************************************************************
 * Function Name: Snake_readControls
 * Parameters:    ControlsData_t * controls - pointer to controls data
 *                                            structure
 * Return:        uint32_t - result of reading:
 *                           SNAKE_READCTRL_RES_OK - OK
 *                           SNAKE_READCTRL_RES_ERROR - error
 *
 * Description: Reads controls(joystick).
 *************************************************************************/
uint32_t Snake_readControls(ControlsData_t * controls)
{
MOD_JOYSTICK_Data_t jData;

  if (MOD_JOYSTICK_readJoystick(controlHandler, &jData) != MOD_JOYSTICK_RESULT_OK) return SNAKE_READCTRL_RES_ERROR;

  if (controls != NULL)
  {
    /* Decode joystick's position */
    if (jData.StickX < (SNAKE_READCTRL_LO_LIMIT + SNAKE_READCTRL_THRESHOLD))
      controls->direction = SNAKE_DIR_LEFT;
    else
    if (jData.StickX > (SNAKE_READCTRL_HI_LIMIT - SNAKE_READCTRL_THRESHOLD))
      controls->direction = SNAKE_DIR_RIGHT;
    else
    if (jData.StickY < (SNAKE_READCTRL_LO_LIMIT + SNAKE_READCTRL_THRESHOLD))
      controls->direction = SNAKE_DIR_DOWN;
    else
    if (jData.StickY > (SNAKE_READCTRL_HI_LIMIT - SNAKE_READCTRL_THRESHOLD))
      controls->direction = SNAKE_DIR_UP;
    else
      controls->direction = SNAKE_DIR_NONE;
    /* Decode joystick's keys */
    controls->key0 = jData.ButtonZ;
    controls->key1 = jData.ButtonC;
  }
  return SNAKE_READCTRL_RES_OK;
}
