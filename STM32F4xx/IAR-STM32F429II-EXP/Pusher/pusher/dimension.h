/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : dimension.h
 *      Description : dimension_t declaration.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef DIMENSION_H_
#define DIMENSION_H_

#include <stdint.h>

/*******************************************************************************
 ** Struct name : dimension_t
 **
 ** Description : Holds a dimension.
 ******************************************************************************/
typedef struct {
	uint8_t w;
	uint8_t h;
} dimension_t;

void set_dimension(dimension_t *dim, uint8_t w, uint8_t h);
void resize_dimension(dimension_t *dim, int8_t dw, int8_t dh);

#endif /* DIMENSION_H_ */
