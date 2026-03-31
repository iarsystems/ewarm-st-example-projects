/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : tile.h
 *      Description : tile_t declaration.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef TILE_H_
#define TILE_H_

#include "assets.h"
#include <stdint.h>
#include <stdbool.h>
#include "vector.h"
#include "dimension.h"

/*******************************************************************************
 ** Struct name : tile_t
 **
 ** Description : Represents a map tile.
 ******************************************************************************/
typedef struct {
	bmp_asset_t type; /*< Tile grafics and type. */
	uint8_t health; /*< Health of tile. */
	bool solid; /*< Is the tile solid. */

	vector_t location; /*< Locaiton if tile. */
} tile_t;

#endif
