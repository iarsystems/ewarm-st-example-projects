/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : types.h
 *      Description : direction_t declaration.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef TYPES_H_
#define TYPES_H_

/*******************************************************************************
 ** Enum name   : direction_t
 **
 ** Description : A direction, can describe a direction in 9 different
 **               directions.
 ******************************************************************************/
typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT,
	NONE
} direction_t;

#endif
