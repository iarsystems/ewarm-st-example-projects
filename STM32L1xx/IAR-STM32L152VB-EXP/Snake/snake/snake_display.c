/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : snake_display.c
 *    Description : Snake-game display module
 *
 *    History :
 *    1. Date        : 28, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include <stdint.h>
#include "snake_hw.h"
#include "snake_core.h"
#include "snake_data.h"
#include "snake_display.h"

/*************************************************************************
 * Function Name: Snake_drawBackground
 * Parameters:    none
 * Return:        none
 *
 * Description: Draws the playground.
 *************************************************************************/
void Snake_drawBackground(void)
{
  for(int y=SNAKE_MIN_Y; y<=SNAKE_MAX_Y; y++)
  for(int x=SNAKE_MIN_X; x<=SNAKE_MAX_X; x++)
    Snake_drawBitmap((BitmapColorType_t*)&Bgr, x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
}

/*************************************************************************
 * Function Name: Snake_drawWalls
 * Parameters:    none
 * Return:        none
 *
 * Description: Draws walls around the playground.
 *************************************************************************/
void Snake_drawWalls(void)
{
  for(int x=SNAKE_MIN_X-1; x<=SNAKE_MAX_X+1; x++)
  for(int y=SNAKE_MIN_Y-1; y<=SNAKE_MAX_Y+1; y++)
  {
    if((y == SNAKE_MIN_Y-1) || (y == SNAKE_MAX_Y+1) || (x == SNAKE_MIN_X-1) || (x == SNAKE_MAX_X+1))
    {
      Snake_drawBitmap((BitmapColorType_t*)&Wall, x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
    }
  }
}

/*************************************************************************
 * Function Name: Snake_drawFeed
 * Parameters:    none
 * Return:        none
 *
 * Description: Draws already generated feed, on the playground.
 *************************************************************************/
void Snake_drawFeed(void)
{
uint8_t x;
uint8_t y;

  for(int i = 0; i < feedCount; i++)
  {
    x = (feedPlaces[i] >> 4) & 0x0F;
    y =  feedPlaces[i] & 0x0F;
    Snake_drawBitmap((BitmapColorType_t*)&Feed, x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
  }
}

/*************************************************************************
 * Function Name: Snake_drawExit
 * Parameters:    none
 * Return:        none
 *
 * Description: Draws an already genearted exit door, on the walls.
 *************************************************************************/
void Snake_drawExit(void)
{
uint8_t x = (exitDoor >> 4) & 0x0F;
uint8_t y =  exitDoor & 0x0F;

  Snake_drawBitmap((BitmapColorType_t*)&Bgr, x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
}

/*************************************************************************
 * Function Name: Snake_drawSegment
 * Parameters:    uint8_t pos - position on the playground.
 *                uint8_t type - type of the snake's segment.
 * Return:        none
 *
 * Description: Draws one snake's segment, depending on it's type.
 *************************************************************************/
void Snake_drawSegment(uint8_t pos, uint8_t type)
{
uint8_t x = (pos >> 4) & 0x0F;
uint8_t y =  pos & 0x0F;

  switch(type)
  {
    case SNAKE_SEG_H:
      Snake_drawBitmap((BitmapColorType_t*)&SnakeH,  x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_V:
      Snake_drawBitmap((BitmapColorType_t*)&SnakeV,  x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_LU:
      Snake_drawBitmap((BitmapColorType_t*)&SnakeLU, x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_RU:
      Snake_drawBitmap((BitmapColorType_t*)&SnakeRU, x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_LD:
      Snake_drawBitmap((BitmapColorType_t*)&SnakeLD, x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_RD:
      Snake_drawBitmap((BitmapColorType_t*)&SnakeRD, x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_HU:
      Snake_drawBitmap((BitmapColorType_t*)&HeadU,   x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_HD:
      Snake_drawBitmap((BitmapColorType_t*)&HeadD,   x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_HL:
      Snake_drawBitmap((BitmapColorType_t*)&HeadL,   x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_HR:
      Snake_drawBitmap((BitmapColorType_t*)&HeadR,   x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_HH:
      Snake_drawBitmap((BitmapColorType_t*)&SnakeHit,x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_TU:
      Snake_drawBitmap((BitmapColorType_t*)&TailU,   x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_TD:
      Snake_drawBitmap((BitmapColorType_t*)&TailD,   x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_TL:
      Snake_drawBitmap((BitmapColorType_t*)&TailL,   x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_TR:
      Snake_drawBitmap((BitmapColorType_t*)&TailR,   x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
    case SNAKE_SEG_NO:
    case SNAKE_SEG_INVALID:
    default:
      Snake_drawBitmap((BitmapColorType_t*)&Bgr,     x*SNAKE_SEG_WIDTH, y*SNAKE_SEG_HEIGHT);
      break;
  }
}
