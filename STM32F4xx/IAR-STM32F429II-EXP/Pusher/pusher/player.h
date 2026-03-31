/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : player.h
 *      Description : Player entity_t handler functions.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef PLAYER_H_
#define PLAYER_H_

#include "map.h"
#include "entity.h"
#include "controller.h"

void init_player(entity_t *player);

void player_control(entity_t *player, controller_t *cont);

#endif
