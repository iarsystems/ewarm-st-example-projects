/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : snake_core.h
 *    Description : Snake-game core header
 *
 *    History :
 *    1. Date        : 28, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __SNAKE_CORE_H
#define __SNAKE_CORE_H

#define SNAKE_MAX_SEGS        72

#define SNAKE_POS_TRAIL       0
#define SNAKE_POS_TAIL        1

#define SNAKE_MOVE_OK         0
#define SNAKE_MOVE_HIT        1
#define SNAKE_MOVE_INVALID    2
#define SNAKE_MOVE_FEED       3
#define SNAKE_MOVE_EXIT       4

#define SNAKE_DIR_UP          0
#define SNAKE_DIR_DOWN        1
#define SNAKE_DIR_RIGHT       2
#define SNAKE_DIR_LEFT        3
#define SNAKE_DIR_NONE        4

#define SNAKE_GROW_SIZE       2

#define SNAKE_PART_HEAD       0x01
#define SNAKE_PART_TAIL       0x02
#define SNAKE_PART_BODY       0x04
#define SNAKE_PART_ALL       (SNAKE_PART_HEAD | SNAKE_PART_TAIL | SNAKE_PART_BODY)

#define SNAKE_MAX_LEVELS      9

#define SNAKE_GO_DELAY_L1     6000
#define SNAKE_GO_DELAY_L2     5000
#define SNAKE_GO_DELAY_L3     4500
#define SNAKE_GO_DELAY_L4     4000
#define SNAKE_GO_DELAY_L5     3500
#define SNAKE_GO_DELAY_L6     3000
#define SNAKE_GO_DELAY_L7     2500
#define SNAKE_GO_DELAY_L8     2000
#define SNAKE_GO_DELAY_L9     1500

#define SNAKE_MAX_FEED_PLACES 2

#define SNAKE_INIT_POS_X      2
#define SNAKE_INIT_POS_Y      SNAKE_MAX_Y-2

#define SNAKE_FEED_TO_EXIT    12

#define SNAKE_EXIT_COMPLETED  0
#define SNAKE_EXIT_FAILED     1
#define SNAKE_EXIT_EXCEPTION  100

#define SNAKE_CTRL_READ_TRIES 200
/* mod_joystick should be read in interval multiple by 2ms */
#define SNAKE_CTRL_READ_DELAY 20

/* exported data */
extern uint8_t feedCount;
extern uint8_t exitDoor;
extern uint8_t feedPlaces[];

typedef struct {

  uint32_t snakeGoValue;

} SnakeGameLevel_t;

/*************************************************************************
 * Function Name: Snake_Heartbeat
 * Parameters:    none
 * Return:        none
 *
 * Description: Snake's heartbeat. This function should be called every 100us
 *              from an interrupt handler.
 *************************************************************************/
void Snake_Heartbeat(void);

/*************************************************************************
 * Function Name: Snake_game
 * Parameters:    none
 * Return:        uint32_t - type of game exit
 *                SNAKE_EXIT_COMPLETED - all levels completed
 *                SNAKE_EXIT_FAILED - game failed
 *                SNAKE_EXIT_EXCEPTION - unexpected error occured
 *
 * Description: Main game function. Must be called from the user app.
 *************************************************************************/
uint32_t Snake_game(void);

#endif
