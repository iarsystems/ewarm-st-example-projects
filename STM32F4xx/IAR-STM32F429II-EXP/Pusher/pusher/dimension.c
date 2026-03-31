/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : dimension.c
 *      Description : dimenstion_t manipulation functions.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#include "dimension.h"

/*******************************************************************************
 ** Function Name : set_dimension
 ** Parameters    : dim - Dimension to change.
 **                 w   - New width.
 **                 h   - New height.
 ** Returns       : none.
 **
 ** Description   : Sets a new width and height to a dimension.
 ******************************************************************************/
void set_dimension(dimension_t *dim, uint8_t w, uint8_t h)
{
	dim->w = w;
	dim->h = h;
}

/*******************************************************************************
 ** Function Name : resize_dimension
 ** Parameters    : dim - Dimension to resize.
 **                 dw   - Resize width with.
 **                 dh   - Resize height with.
 ** Returns       : none.
 **
 ** Description   : Resizes a dimension.
 ******************************************************************************/
void resize_dimension(dimension_t *dim, int8_t dw, int8_t dh)
{
	dim->w += dw;
	dim->h += dh;
}
