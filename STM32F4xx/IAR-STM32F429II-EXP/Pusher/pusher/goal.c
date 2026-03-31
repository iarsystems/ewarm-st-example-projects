/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : goal.c
 *      Description : Goal entity_t step routine handlers.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#include "goal.h"
#include "map.h"
#include "pusher.h"

uint8_t box_comp_count = 0;

void goal_step_routine(map_t *map,
                       entity_t *goal,
                       entity_t *all_ent,
                       uint8_t count);

/*******************************************************************************
 ** Function Name : goal_step_routine
 ** Parameters    : map  - Map to step the goal on.
 **                 goal - Goal to step.
 **                 all_ent - All entities.
 **                 count   - Entity count.
 ** Returns       : none.
 **
 ** Description   : Logic for the goal.
 ******************************************************************************/
void goal_step_routine(map_t *map,
                       entity_t *goal,
                       entity_t *all_ent,
                       uint8_t count)
{
	for (int i = 0; i < count; i++) {
		if (all_ent[i].graf != BOX) {
			continue;
		}
		/* Check if any box is on top of the the goal. */
		if (all_ent[i].location.x == goal->location.x &&
		    all_ent[i].location.y == goal->location.y) {
			set_vector(&all_ent[i].location, box_comp_count, 12);
			box_comp_count++;
		}
    }
	/* If we have moved all the boxes to the goal, we need to notify it
	   somehow. */
	if (box_comp_count == get_box_count()) {
		level_completed();
		box_comp_count = 0;
	}
}

/*******************************************************************************
 ** Function Name : init_goal
 ** Parameters    : goal - Goal to initialize.
 ** Returns       : none.
 **
 ** Description   : Initializes a goal.
 ******************************************************************************/
void init_goal(entity_t *goal)
{
	goal->health = 255;
	goal->step_routine = &goal_step_routine;
	goal->graf = GOAL;
	set_vector(&goal->location, 0, 0);
	set_dimension(&goal->size, 9, 9);
	goal->dir = NONE;
}

/*******************************************************************************
 ** Function Name : reset_box_count
 ** Parameters    : none.
 ** Returns       : none.
 **
 ** Description   : Resets count of boxes that have been completed.
 ******************************************************************************/
void reset_box_count()
{
	box_comp_count = 0;
}
