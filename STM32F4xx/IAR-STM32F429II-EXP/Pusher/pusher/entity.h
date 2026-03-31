/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : entity.h
 *      Description : entity_t declaration.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#ifndef ENTITY_H_
#define ENTITY_H_

#include <stdint.h>
#include <stdbool.h>
#include "map.h"
#include "assets.h"
#include "rect.h"
#include "vector.h"
#include "controller.h"
#include "dimension.h"
#include "types.h"

struct map;

/*******************************************************************************
 ** Struct name : entity_t
 **
 ** Description : Represents an entity in the game.
 ******************************************************************************/
typedef struct entity {
	uint8_t health; /*< Health of the entity. */
	vector_t location; /*< Location of the entity. */
	dimension_t size; /*< Size of graphics. */
	direction_t dir; /*< Direction the entity is facing. */
	bmp_asset_t graf; /*< What type of graphics the entity has. */

/*******************************************************************************
 ** Function Name : step_routine
 ** Parameters    : map_t *    - Map it happens on.
 **                 entity_t * - The entity that is stepped.
 **                 entity_t * - All other entities on the map, the entity that
 **                              is being processed is also present.
 **                 uint8_t *  - Total entity count.
 ** Returns       : none
 **
 ** Description   : Logic routine for a entity.
 ******************************************************************************/
	void (*step_routine)(struct map *,
	                     struct entity *,
	                     struct entity *,
	                     uint8_t);
} entity_t;

void step_entities(struct map *map, entity_t *ent, uint8_t count);

#endif /* ENTITY_H_ */
