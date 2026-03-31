/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : map.h
 *      Description : map_t and mapt_level_t declarations.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef MAP_H_
#define MAP_H_

#include <stdint.h>
#include <stdbool.h>

#include "tile.h"
#include "dimension.h"
#include "entity.h"

#define MAP_SIZE_W 14 /*< Column count of tiles. */
#define MAP_SIZE_H 12 /*< Row count of tiles. */

struct entity;

/*******************************************************************************
 ** Enum name   : map_level_t
 **
 ** Description : Holds levels.
 ******************************************************************************/
typedef enum {
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
	LEVEL_COUNT
} map_level_t;

/*******************************************************************************
 ** Struct name : map_t
 **
 ** Description : Holds a level map with some basic data for rendering.
 ******************************************************************************/
typedef struct map{
	tile_t *map_data; /*< Holds a map. */
	dimension_t size; /*< Tile width and height count. */
	dimension_t tile_size; /*< Pixel size of tiles. */
} map_t;

void load_level(map_level_t level);
bool is_solid(uint8_t x, uint8_t y, bool real_coord);
map_t* get_map();

struct entity * get_player();
struct entity * get_boxes();
uint8_t get_box_count();
struct entity * get_goal();
struct entity * get_entities();

#endif /* MAP_H_ */
