/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : vector.h
 *      Description : vector_t declaration.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdint.h>

/*******************************************************************************
 ** Struct name : vector_t
 **
 ** Description : A vector.
 ******************************************************************************/
typedef struct {
	int8_t x;
	int8_t y;
} vector_t;

void set_vector(vector_t *vect, int8_t x, int8_t y);
void move_vector(vector_t *vect, int8_t dx, int8_t dy);

#endif /* VECTOR_H_ */
