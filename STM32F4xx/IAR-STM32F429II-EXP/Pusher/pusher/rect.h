/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : rect.h
 *      Description : rect_t declaration.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef RECT_H_
#define RECT_H_

#include <stdint.h>

/*******************************************************************************
 ** Struct name : rect_t
 **
 ** Description : A rectangle.
 ******************************************************************************/
typedef struct {
	uint8_t left;   /*< Left x coordinate.   */
	uint8_t top;    /*< Top y coordinate.    */
	uint8_t right;  /*< Right x coordinate.  */
	uint8_t bottom; /*< Bottom y coordinate. */
} rect_t;

void set_rect_location(rect_t *rect, uint8_t x, uint8_t y);
void move_rect_location(rect_t *rect, uint8_t dx, uint8_t dy);
uint8_t get_rect_width(rect_t *rect);
uint8_t get_rect_height(rect_t *rect);

#endif /* RECT_H_ */
