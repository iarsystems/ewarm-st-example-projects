/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : assets.h
 *      Description : Access functions for graphics assets for the game Pusher.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef ASSETS_H_
#define ASSETS_H_

#include "framework/bitmap.h"

#define TILE_HEIGHT 9
#define TILE_WIDTH 9

typedef enum bmp_asset {
	AIR,
	ROCK,
	PLAYER,
	BOX,
	GOAL,
	OTHER,

	BMP_ASSET_COUNT /* Keep this last. */
} bmp_asset_t;

void init_assets();

BitmapColorType_t * get_bmp_asset(bmp_asset_t asset);

#endif //ASSETS_H_
