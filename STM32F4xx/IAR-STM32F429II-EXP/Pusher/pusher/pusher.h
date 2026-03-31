/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : pusher.h
 *      Description : Functions to initialize the game and then run it.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef PUSHER_H_
#define PUSHER_H_

#include <stdbool.h>
#include "framework/connect.h"

void init_pusher(ConnectionHandler_t *j);
void level_completed();
void run_pusher();

#endif /* PUSHER_H_ */
