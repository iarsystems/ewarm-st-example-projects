/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : snake_display.h
 *    Description : Snake-game dipslay module
 *
 *    History :
 *    1. Date        : 28, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __SNAKE_DISPLAY_H
#define __SNAKE_DISPLAY_H

#define SNAKE_MIN_X           1
#define SNAKE_MIN_Y           2
#define SNAKE_MAX_X           9
#define SNAKE_MAX_Y           9

#define SNAKE_SEG_NO          0
#define SNAKE_SEG_H           1
#define SNAKE_SEG_V           2
#define SNAKE_SEG_LU          3
#define SNAKE_SEG_RU          4
#define SNAKE_SEG_LD          5
#define SNAKE_SEG_RD          6
#define SNAKE_SEG_HU          7
#define SNAKE_SEG_HD          8
#define SNAKE_SEG_HL          9
#define SNAKE_SEG_HR          10
#define SNAKE_SEG_HH          11
#define SNAKE_SEG_TU          12
#define SNAKE_SEG_TD          13
#define SNAKE_SEG_TL          14
#define SNAKE_SEG_TR          15
#define SNAKE_SEG_INVALID     100

#define SNAKE_SEG_WIDTH       12
#define SNAKE_SEG_HEIGHT      12

/*************************************************************************
 * Function Name: Snake_drawBackground
 * Parameters:    none
 * Return:        none
 *
 * Description: Draws the playground.
 *************************************************************************/
void Snake_drawBackground(void);

/*************************************************************************
 * Function Name: Snake_drawWalls
 * Parameters:    none
 * Return:        none
 *
 * Description: Draws walls around the playground.
 *************************************************************************/
void Snake_drawWalls(void);

/*************************************************************************
 * Function Name: Snake_drawFeed
 * Parameters:    none
 * Return:        none
 *
 * Description: Draws already generated feed, on the playground.
 *************************************************************************/
void Snake_drawFeed(void);

/*************************************************************************
 * Function Name: Snake_drawExit
 * Parameters:    none
 * Return:        none
 *
 * Description: Draws an already genearted exit door, on the walls.
 *************************************************************************/
void Snake_drawExit(void);

/*************************************************************************
 * Function Name: Snake_drawSegment
 * Parameters:    uint8_t pos - position on the playground.
 *                uint8_t type - type of the snake's segment.
 * Return:        none
 *
 * Description: Draws one snake's segment, depending on it's type.
 *************************************************************************/
void Snake_drawSegment(uint8_t pos, uint8_t state);

#endif
