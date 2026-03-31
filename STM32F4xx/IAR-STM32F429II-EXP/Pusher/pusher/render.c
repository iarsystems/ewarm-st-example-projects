/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : render.c 
 *      Description : Pusher render functions.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#include "render.h"
#include "modules/mod_lcd6610.h"

static ConnectionHandler_t *disp;
static uint8_t x_offset;
static uint8_t y_offset;

/*******************************************************************************
 ** Function Name : render_title
 ** Parameters    : display - Display connection.
 **                 off_x   - Offset to be used when rendering game stuff.
 **                 off_y   - Offset to be used when rendering game stuff.
 ** Return        : none.
 **
 ** Description   : Initialises the game renderer.
 ******************************************************************************/
void init_render(ConnectionHandler_t *display, uint8_t off_x, uint8_t off_y)
{
	disp = display;
	x_offset = off_x;
	y_offset = off_y;
}

/*******************************************************************************
 ** Function Name : render
 ** Parameters    : type - What type of asset to draw.
 **                 x    - Location on the x-axis.
 **                 y    - Location on the y-axis.
 ** Return        : none.
 **
 ** Description   : Renders an asset to the 6610 display.
 ******************************************************************************/
void render(bmp_asset_t type,
            uint8_t x,
            uint8_t y)
{
	MOD_LCD6610_drawBitmap(disp, get_bmp_asset(type), x, y);
}

/*******************************************************************************
 ** Function Name : render_map
 ** Parameters    : map      - Map to render.
 **                 x_offset - Offset on the x-axis.
 **                 y_offset - Offset on the y-axis.
 ** Return        : none.
 **
 ** Description   : Renders a map to the display.
 ******************************************************************************/
void render_map(map_t *map)
{
	uint8_t count = map->size.w * map->size.h;
	tile_t *map_data = map->map_data;

	for (uint8_t i = 0; i < count; i++) {
		render(map_data->type,
		       (map_data->location.x * map->tile_size.w)  + x_offset,
		       (map_data->location.y * map->tile_size.h) + y_offset);

		map_data++;
	}
}

/*******************************************************************************
 ** Function Name : render_tile
 ** Parameters    : tile - Tile to render.
 ** Returns       : none
 **
 ** Description   : As the name suggest, renders a tile, with the offset.
 ******************************************************************************/
void render_tile(tile_t *tile)
{
	render(tile->type,
           (tile->location.x * 9) + x_offset,
           (tile->location.y * 9) + y_offset);
}

/*******************************************************************************
 ** Function Name : render_entities
 ** Parameters    : ent   - Entities to render.
 **                 count - Number of entities to render.
 ** Return        : none.
 **
 ** Description   : Renders entities to the 6610 display.
 ******************************************************************************/
void render_entities(entity_t *ent, uint8_t count)
{
	for (int i = 0; i < count; i++) {
		render(ent->graf,
		       (ent->location.x * ent->size.w) + x_offset,
		       (ent->location.y * ent->size.h) + y_offset);
		ent++;
	}
}

/*******************************************************************************
 ** Function Name : render_text
 ** Parameters    : text - Text to render.
 **                 x    - x location.
 **                 y    - y location.
 ** Returns       : none.
 **
 ** Description   : Renders a text to a given location.
 ******************************************************************************/
void render_text(char *text, int16_t x, int16_t y)
{
	MOD_LCD6610_setTextPosition(disp, x, y);
	MOD_LCD6610_writeString(disp, text);
}

/*******************************************************************************
 ** Function Name : render_clear_screen
 ** Parameters    : color - Color the clear screen with.
 ** Returns       : none.
 **
 ** Description   : Clears the screen.
 ******************************************************************************/
void render_clear_screen(uint16_t color)
{
	MOD_LCD6610_fillWindow(disp, color);
}

