/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : controller.h
 *      Description : controller_t declaration and usage functions.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <stdint.h>
#include <stdbool.h>
#include "connect.h"

/*******************************************************************************
 ** Struct name : controller_t
 **
 ** Description : Holds the states for joystick and buttons.
 ******************************************************************************/
typedef struct {
	uint8_t stick_x;
	uint8_t stick_y;
	bool z_btn_pressed;
	bool c_btn_pressed;
} controller_t;

bool read_controller(ConnectionHandler_t *joy, controller_t *cont);

void reset_controller(controller_t *cont);

#endif
