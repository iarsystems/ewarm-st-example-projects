/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : entity.c
 *      Description : entity_t handler function.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#include "entity.h"
#include "tile.h"

/*******************************************************************************
 ** Function Name : step_entities
 ** Parameters    : map      - Map where the entities are on.
 **                 entity_t - Array of all entities.
 **                 count    - Count of number of entities.
 ** Returns       : none
 **
 ** Description   : Does a single step of all entities.
 ******************************************************************************/
void step_entities(map_t *map, entity_t *ent, uint8_t count)
{
	for (int i = 0; i < count; i++) {
		(*(ent[i].step_routine))(map, &ent[i], ent, count);
	}
}
