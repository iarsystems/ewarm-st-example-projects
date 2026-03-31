/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : box.c
 *      Description : Box initialization function and game step routines.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/
#include "box.h"
#include "assets.h"
#include "map.h"
#include <stdlib.h>

inline entity_t * find_player(entity_t *all_ent, uint8_t count);

inline bool box_exists_at(entity_t *all_ents,
                          uint8_t count,
                          uint8_t x,
                          uint8_t y);

void box_step_routine(map_t *map,
                      entity_t *box,
                      entity_t *all_ent,
                      uint8_t count);

/*******************************************************************************
 ** Function Name : find_player
 ** Parameters    : all_ent - All entities to search for player.
 **                 cout    - Count of entities.
 ** Returns       : Pointer to player entity if found, else false.
 **
 ** Description   : Searches for a player entity in a give entity array.
 ******************************************************************************/
inline entity_t * find_player(entity_t *all_ent, uint8_t count)
{
	for (int i = 0; i < count; i++) {
		if (all_ent[i].graf == PLAYER) {
			return &all_ent[i];
		}
	}

	return NULL;
}

/*******************************************************************************
 ** Function Name : box_exists_at
 ** Parameters    : all_ents - All entities to search in.
 **                 count    - Count of entities.
 **                 x        - Location to find entity on.
 **                 y        - Location to find entity on.
 ** Returns       : True if there is a box on the given location, else false.
 **
 ** Description   : Checks if there is a box on the given location.
 ******************************************************************************/
inline bool box_exists_at(entity_t *all_ents,
                          uint8_t count,
                          uint8_t x,
                          uint8_t y)
{
	for (int i = 0; i < count; i++) {
		if (all_ents[i].graf == BOX &&
		    all_ents[i].location.x == x &&
		    all_ents[i].location.y == y) {
			return true;
		}
	}

	return false;
}

/*******************************************************************************
 ** Function Name : box_step_routine
 ** Parameters    : map - Map to step a box on.
 **                 box - Box to step.
 **                 all_ent - All other entities on map.
 **                 count   - Entity count on map.
 ** Returns       : none
 **
 ** Description   : Logic for different events that can happen to the box.
 ******************************************************************************/
void box_step_routine(map_t *map,
                      entity_t *box,
                      entity_t *all_ent,
                      uint8_t count)
{
	entity_t *player = find_player(all_ent, count);
	if (player == NULL) {
		while(1){}
	}
	/* Check if player is on the current box. */
	if (player->location.x == box->location.x &&
	    player->location.y == box->location.y) {
		/* Need to check if box can move in a certain direction. If not
		   move the player from where he came from. */
		switch (player->dir) {
		case LEFT:
			if (!is_solid(box->location.x - 1,
			              box->location.y,
			              false)
			    &&
			    !box_exists_at(all_ent, count,
			                   box->location.x - 1,
			                   box->location.y)) {
				move_vector(&box->location, -1, 0);
			} else {
				move_vector(&player->location, 1, 0);
			}
			break;
		case RIGHT:
			if (!is_solid(box->location.x + 1,
			              box->location.y,
			              false)
			    &&
			    !box_exists_at(all_ent, count,
			                   box->location.x + 1,
			                   box->location.y)) {
				move_vector(&box->location, 1, 0);
			} else {
				move_vector(&player->location, -1, 0);
			}
			break;
		case DOWN:
			if (!is_solid(box->location.x,
			              box->location.y + 1,
			              false)
			    &&
			    !box_exists_at(all_ent, count,
			                   box->location.x,
			                   box->location.y + 1)) {
				move_vector(&box->location, 0, 1);
			} else {
				move_vector(&player->location, 0, -1);
			}
			break;
		case UP:
			if (!is_solid(box->location.x,
			              box->location.y - 1,
			              false)
			    &&
			    !box_exists_at(all_ent, count,
			                   box->location.x,
			                   box->location.y - 1)) {
				move_vector(&box->location, 0, -1);
			} else {
				move_vector(&player->location, 0, 1);
			}
			break;
		}
	}
}

/*******************************************************************************
 ** Function Name : init_box
 ** Parameters    : box - Box to initialize.
 ** Returns       : none.
 **
 ** Description   : Initializes a box.
 ******************************************************************************/
void init_box(entity_t *box)
{
	box->health = 255;
	box->step_routine = &box_step_routine;
	box->graf = BOX;
	set_vector(&box->location, 0, 0);
	set_dimension(&box->size, 9, 9);
	box->dir = NONE;
}
