/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : rect.c
 *      Description : rect_t handler functions.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#include "rect.h"

/*******************************************************************************
 ** Function Name : set_rect_location
 ** Parameters    : rect - Set location on.
 **                 x    - New x location.
 **                 y    - New y location.
 ** Returns       : none.
 **
 ** Description   : Set a new location for a rect.
 ******************************************************************************/
void set_rect_location(rect_t *rect, uint8_t x, uint8_t y)
{
	uint8_t s = get_rect_width(rect);
	rect->left = x;
	rect->right = x + s;
	s = get_rect_height(rect);
	rect->top = y;
	rect->bottom = y + s;
}

/*******************************************************************************
 ** Function Name : move_rect_location
 ** Parameters    : rect - Move location on.
 **                 dx   - Move by on x-axis.
 **                 dy   - Move by on y-axis.
 ** Returns       : none.
 **
 ** Description   : Move a rect location.
 ******************************************************************************/
void move_rect_location(rect_t *rect, uint8_t dx, uint8_t dy)
{
	rect->left += dx;
	rect->right += dx;
	rect->top += dy;
	rect->bottom += dy;
}

/*******************************************************************************
 ** Function Name : get_rect_width
 ** Parameters    : rect - To get width from.
 ** Returns       : Width of the rect.
 **
 ** Description   : Calculates the width of a rect.
 ******************************************************************************/
uint8_t get_rect_width(rect_t *rect)
{
	return rect->right - rect->left;
}

/*******************************************************************************
 ** Function Name : get_rect_height
 ** Parameters    : rect - To get height from.
 ** Returns       : Height of the rect.
 **
 ** Description   : Calculates the height of a rect.
 ******************************************************************************/
uint8_t get_rect_height(rect_t *rect)
{
	return rect->bottom - rect->top;
}
