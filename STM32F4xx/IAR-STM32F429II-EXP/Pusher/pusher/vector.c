/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : vector.c
 *      Description : vector_t handleing functions.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#include "vector.h"

/*******************************************************************************
 ** Function Name : set_vector
 ** Parameters    : vect - Vector to set.
 **                 x    - Location on the x-axis.
 **                 y    - Location on the y-axis.
 ** Returns       : none.
 **
 ** Description   : Sets a vect to location (x, y).
 ******************************************************************************/
void set_vector(vector_t *vect, int8_t x, int8_t y)
{
	vect->x = x;
	vect->y = y;
}

/*******************************************************************************
 ** Function Name : move_vector
 ** Parameters    : vect - Vector to set.
 **                 dx   - How much on the x-axis.
 **                 dy   - How much on the y-axis.
 ** Returns       : none.
 **
 ** Description   : Moves the location of a vector by dx, dy.
 ******************************************************************************/
void move_vector(vector_t *vect, int8_t dx, int8_t dy)
{
	vect->x += dx;
	vect->y += dy;
}
