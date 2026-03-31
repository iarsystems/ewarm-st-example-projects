/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : player.c
 *      Description : Player entity_t step and initialization routines.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#include "player.h"
#include "vector.h"
#include "tile.h"
#include "assets.h"
#include "rect.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void player_step_routine(map_t *map,
                         entity_t *player,
                         entity_t *all_ent,
                         uint8_t count);

/*******************************************************************************
 ** Function Name : player_set_routine
 ** Parameters    : map     - Map the player is on.
 **                 player  - Player entity.
 **                 all_ent - All entities on the map the player is on,
 **                           including the player.
 **                 count   - Count of all entities.
 ** Returns       : none
 **
 ** Description   : The logic that happens every step for the player entity.
 ******************************************************************************/
void player_step_routine(map_t *map,
                         entity_t *player,
                         entity_t *all_ent,
                         uint8_t count)
{
	/* Check so that the player doesn't go into a wall. */
	switch (player->dir) {
	case LEFT:
		if (!is_solid(player->location.x - 1, player->location.y, false)) {
			move_vector(&player->location, -1, 0);
		}
		break;
	case RIGHT:
		if (!is_solid(player->location.x + 1, player->location.y, false)) {
			move_vector(&player->location, 1, 0);
		}
		break;
	case UP:
		if (!is_solid(player->location.x, player->location.y - 1, false)) {
			move_vector(&player->location, 0, -1);
		}
		break;
	case DOWN:
		if (!is_solid(player->location.x, player->location.y + 1, false)) {
			move_vector(&player->location, 0, 1);
		}
		break;
	default:
        break;
	}
}


/*******************************************************************************
 ** Function Name : init_player
 ** Parameters    : player - Entity that will be a player.
 ** Returns       : none
 **
 ** Description   : Initializes a player entity.
 ******************************************************************************/
void init_player(entity_t *player)
{
	player->health = 255;
	player->step_routine = &player_step_routine;
	player->graf = PLAYER;
	set_vector(&player->location, 0, 0);
	set_dimension(&player->size, 9, 9);
	player->dir = NONE;
}

/*******************************************************************************
 ** Function Name : player_control
 ** Parameters    : player - The player.
 ** Returns       : none.
 **
 ** Description   : Handle different events from the controller.
 ******************************************************************************/
void player_control(entity_t *player, controller_t *cont)
{
	/* Check what direction the controller joystick is directed and set that to
	   player. */
	if (cont->stick_x < 30) player->dir = LEFT;
	else if (cont->stick_x > 220) player->dir = RIGHT;
	else if (cont->stick_y < 30) player->dir = DOWN;
	else if (cont->stick_y > 220) player->dir = UP;
	else player->dir = NONE;
}
