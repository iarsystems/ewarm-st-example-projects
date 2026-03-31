/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : map.c
 *      Description : Holds level maps and handler functions for map_t.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#include "map.h"
#include "assets.h"
#include "player.h"
#include "box.h"
#include "goal.h"
#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>

inline void init_tile(tile_t *tile,
                      bmp_asset_t type,
                      uint8_t healt,
                      bool solid);

/* The different level maps are described here. */
static uint8_t level_1[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1,
	1, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

static uint8_t level_1_box_count = 3;

static uint8_t level_2[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1,
	1, 0, 0, 3, 3, 1, 0, 0, 1, 1, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 4, 1, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

static uint8_t level_2_box_count = 5;

static uint8_t level_3[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3, 1, 1, 1,
	1, 0, 3, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
	1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
	1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1,
	1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 2, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

static uint8_t level_3_box_count = 4;

/* End of level maps. */

static entity_t *entities = NULL;
static uint8_t box_count = 0;

static tile_t map_data[MAP_SIZE_W * MAP_SIZE_H]; /*< Hold the current map. */

/*< The map object. */
static map_t map = {map_data,
                    {MAP_SIZE_W, MAP_SIZE_H},
                    {TILE_WIDTH, TILE_HEIGHT}};

/*******************************************************************************
 ** Function Name : init_tile
 ** Parameters    : tile   - To initialize.
 **                 type   - To which type.
 **                 health - Tile health.
 **                 solid  - If the tile is solid or not.
 ** Returns       : none.
 **
 ** Description   : Initializes a tile.
 ******************************************************************************/
inline void init_tile(tile_t *tile,
                      bmp_asset_t type,
                      uint8_t health,
                      bool solid)
{
	tile->type = type;
	tile->health = health;
	tile->solid = solid;
}

/*******************************************************************************
 ** Function Name : load_level
 ** Parameters    : level - Level to load.
 ** Returns       : none.
 **
 ** Description   : Loads a level with all its assets. But you still have to
 **                 initialize all assets before calling this.
 ******************************************************************************/
void load_level(map_level_t level)
{
	uint8_t
		x = 0,
		y = 0,
		pos = 0,
		count = map.size.w * map.size.h,
		box_location = 2;

	uint8_t
		*load_map = NULL;

    size_t
	    entity_size = sizeof(entity_t);

    box_count = 0;

    if (entities != NULL) {
	    free (entities);
    }

    /* Selects which level to load. */
	switch (level) {
	case LEVEL_1:
		load_map = level_1;
		entities = (entity_t *)malloc(entity_size * (level_1_box_count + 2));
		break;
	case LEVEL_2:
		load_map = level_2;
		entities = (entity_t *)malloc(entity_size * (level_2_box_count + 2));
		break;
	case LEVEL_3:
		load_map = level_3;
		entities = (entity_t *)malloc(entity_size * (level_3_box_count + 2));
		break;
	}

	/* Initializes all tiles and entities. */
	for (int i = 0; i < count; i++) {
		x = i % map.size.w;
		y = i / map.size.w;
		pos = y * map.size.w + x;

		/* Check what kind of tile to initialize. */
		switch (load_map[pos]) {
		case 0: init_tile(&map_data[pos], AIR, 255, false); break;
		case 1: init_tile(&map_data[pos], ROCK, 255, true); break;
		case 2:
			init_tile(&map_data[pos], AIR, 255, false);

			init_player(&entities[0]);
			set_vector(&entities[0].location, x, y);
			break;
		case 3:
			init_tile(&map_data[pos], AIR, 255, false);

			init_box(&entities[box_location]);
			set_vector(&entities[box_location].location, x, y);
			box_count++;
			box_location++;
			break;
		case 4:
			init_tile(&map_data[pos], AIR, 255, false);

			init_goal(&entities[1]);
			set_vector(&entities[1].location, x, y);
			break;
		default: init_tile(&map_data[pos], OTHER, 0, false);
		}

		set_vector(&map_data[pos].location, x, y);
	}
}

/*******************************************************************************
 ** Function Name : is_solid
 ** Parameters    : x          - Location on x.
 **                 y          - Location on y.
 **                 real_coord - Use real coordinates or tile coordinates.
 ** Returns       : False if tile found is solid, else false.
 **
 ** Description   : Checks if a tile on the given location is solid or not.
 ******************************************************************************/
bool is_solid(uint8_t x, uint8_t y, bool real_coord)
{
	if (real_coord) {
		/* Convert from real coordinates to coordinates in map array. */
		x /= map.tile_size.w;
		y /= map.tile_size.h;
	}

	if (x < map.size.w && y < map.size.h) {
		return map.map_data[x + y * map.size.w].solid;
	}

	return false;
}

/*******************************************************************************
 ** Function Name : get_map
 ** Parameters    : none.
 ** Returns       : Pointer to the current map.
 **
 ** Description   : none.
 ******************************************************************************/
map_t* get_map()
{
	return &map;
}

/*******************************************************************************
 ** Function Name : get_player
 ** Parameters    : none.
 ** Returns       : Player on current map.
 **
 ** Description   : none.
 ******************************************************************************/
entity_t * get_player()
{
	return &entities[0];
}

/*******************************************************************************
 ** Function Name : get_boxes
 ** Parameters    : none.
 ** Returns       : Pointer to first box.
 **
 ** Description   : Returns pointer to the first box in the array, use
 **                 get_box_count to see how many boxes exists in the array.
 ******************************************************************************/
entity_t * get_boxes()
{
	return &entities[2];
}

/*******************************************************************************
 ** Function Name : get_box_count
 ** Parameters    : none.
 ** Returns       : Count of boxes for the current map.
 **
 ** Description   : none.
 ******************************************************************************/
uint8_t get_box_count()
{
	return box_count;
}

/*******************************************************************************
 ** Function Name : get_goal
 ** Parameters    : none.
 ** Returns       : Goal on current map.
 **
 ** Description   : none.
 ******************************************************************************/
entity_t * get_goal()
{
	return &entities[1];
}

/*******************************************************************************
 ** Function Name : get_entities
 ** Parameters    : none.
 ** Returns       : All entities on current map.
 **
 ** Description   : none.
 ******************************************************************************/
entity_t * get_entities()
{
	return entities;
}
