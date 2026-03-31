/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : render.h
 *      Description : Render functions declaration.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef RENDER_H_
#define RENDER_H_

#include "framework/connect.h"
#include <stdint.h>
#include "tile.h"
#include "assets.h"
#include "map.h"
#include "entity.h"

void init_render(ConnectionHandler_t *display, uint8_t off_x, uint8_t off_y);

void render(bmp_asset_t type,
            uint8_t x,
            uint8_t y);

void render_map(map_t *map);
void render_tile(tile_t *tile);
void render_entities(entity_t *ent, uint8_t count);
void render_text(char *text, int16_t x, int16_t y);
void render_clear_screen(uint16_t color);

#endif /* RENDER_H_ */
