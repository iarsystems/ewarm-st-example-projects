/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : pusher.c
 *      Description : Game logic and presentations functions.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#include "pusher.h"
#include "map.h"
#include "entity.h"
#include "stm32f4xx.h"
#include "controller.h"
#include "player.h"
#include "render.h"
#include "goal.h"

#include "stm32f4xx_tim.h"

#include <stdio.h>

/* Timer handler functions. */
void game_loop_timer();
void time_loop_timer();

static map_level_t current_level = LEVEL_1;

static bool completed = false;
static bool load_new_level = true;

static ConnectionHandler_t *joystick;
static volatile uint32_t timer = 0;

/* Tick flags. */
static volatile bool time_tick = false;
static volatile bool game_tick = false;

/* Conversion functions. */
char* itoa(int value, char* result, int base);

/* Game state. */
typedef enum {
	HELLO_SCREEN,
	GAME,
	THE_END
} state_t;

/* Current game state. */
static state_t state = HELLO_SCREEN;

/*******************************************************************************
 ** Function Name : game_loop_timer
 ** Parameters    : none.
 ** Returns       : none.
 **
 ** Description   : Ticks the game loop timer.
 ******************************************************************************/
void game_loop_timer()
{
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    game_tick = true;
}

/*******************************************************************************
 ** Function Name : time_loop_timer
 ** Parameters    : none.
 ** Returns       : none.
 **
 ** Description   : Ticks the time counter.
 ******************************************************************************/
void time_loop_timer()
{
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    time_tick = true;
    timer += 2;
}

/*******************************************************************************
 ** Function Name : itoa
 ** Parameters    : value  - Value to convert.
 **                 result - Resulting conversion.
 **                 base   - Base to convert to.
 ** Returns       : Pointer to result.
 **
 ** Description   : Converts an integer value to string.
 ******************************************************************************/
char* itoa(int value, char* result, int base) {
    if (base < 2 || base > 36) {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) {
        *ptr++ = '-';
    }

    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

/*******************************************************************************
 ** Function Name : init_pusher
 ** Parameters    : j - Connection to the joystick.
 ** Returns       : none.
 **
 ** Description   : Initializes the game.
 ******************************************************************************/
void init_pusher(ConnectionHandler_t *j)
{
	joystick = j;
}

/*******************************************************************************
 ** Function Name : level_completed
 ** Parameters    : none.
 ** Returns       : none.
 **
 ** Description   : Is called when the current level is completed.
 ******************************************************************************/
void level_completed()
{
	if (!completed) {
		if (current_level + 1 == LEVEL_COUNT) {
			completed = true;
		} else {
			current_level += 1;
			load_new_level = true;
		}
	}
}

void convert_time(char *print)
{
	int
		ms,
		time[3],
		total,
		j = 0;

	char
		ret[3];

	total = timer;
	ms = total % 10;
	total -= ms;
	time[0] = total / (10*60*60);
	total -= time[0] * (10*60*60);
	time[1] = total / (10*60);
	total -= time[1] * (10*60);
	time[2] = total / 10;

	for (int i = 0; i < 3; i++) {
		itoa(time[i], ret, 10);
		if (time[i] < 10) {
			print[j] = '0';
			print[j+1] = ret[0];
		} else {
			print[j] = ret[0];
			print[j+1] = ret[1];
		}
		print[j+2] = ':';
		j+=3;
	}
	print[8] = '.';
	itoa(ms, ret, 10);
	print[9] = ret[0];
}

void hello_screen()
{
	render_clear_screen(0x0000);
	uint8_t i = 131 / 2;

	render_text("Pusher", i - (3*7) , 2);
	render_text("How to play:", i - (6*7), 2 + 9*2);
	render_text("Use joystick to move.", 2, 2 + 9*3);
	render_text("z to restart level.", 2, 2 + 9*4);
	render_text("c to restart game.", 2,2 + 9*5);

	render_text("Press any button", i - (8*7), 9*7);
	render_text("to continue.", i - (6*7), 9*8);
}

void game_completed()
{
	char print[11];
	convert_time(print);
	render_clear_screen(0x0000);
	render_text("Your time: ", 2, 6 * 9);
	render_text(print, 2 + 9 * 7, 6 * 9);

	while(1){}
}

void check_joystick(controller_t *cont)
{
    controller_t tmp;
    if (!read_controller(joystick, &tmp)) {
        while (1) {}
    }
    if (!(tmp.stick_x < 235 && tmp.stick_x > 220 &&
                tmp.stick_y < 235 && tmp.stick_y > 220)) {
        cont->stick_x = tmp.stick_x;
        cont->stick_y = tmp.stick_y;
    }
    if (tmp.z_btn_pressed != tmp.c_btn_pressed) {
        cont->z_btn_pressed = tmp.z_btn_pressed;
        cont->c_btn_pressed = tmp.c_btn_pressed;
    }
}

/*******************************************************************************
 ** Function Name : run_pusher
 ** Parameters    : none.
 ** Returns       : none.
 **
 ** Description   : Main loop of the game.
 ******************************************************************************/
void run_pusher()
{
	bool
		run = true; /*< Should main loop be running.? */

	controller_t cont;
	map_t *map; /*< Map of the current game. */
	entity_t *entities; /*< All entities. */
	char print[11]; /*< Holds the string representing how log the current game
	                  < has taken. */

	timer = 0;

    hello_screen();

	while (run) {
		switch (state) {
		case HELLO_SCREEN:
			/* Wait until a button has been pressed befor exiting loop. */
            if (game_tick) {
                game_tick = false;

                check_joystick(&cont);

                if (cont.z_btn_pressed || cont.c_btn_pressed) {
                    state = GAME;
                }
                reset_controller(&cont);
            }

			break;
		case GAME:
			/* Check if we need to load a new level. */
			if (load_new_level) {
				load_new_level = false;
				load_level(current_level);
				render_clear_screen(0x0000);
				render_text("Pusher v0.1", 2, 2);
				map = get_map();
				render_map(map);
				entities = get_entities();
			}

			/* When the current game time has changed we need to update it on
			   the screen. */
			if (time_tick) {
				time_tick = false;
				convert_time(print);
				render_text(print, 7 * 10, 2);
			}

			/* Change state when all the levels have been played. */
			if (completed) {
				state = THE_END;
			}

            if (game_tick) {
                game_tick = false;

                check_joystick(&cont);

				/* Resets the current level. */
				if (cont.z_btn_pressed) {
					load_new_level = true;
					reset_box_count();
                    reset_controller(&cont);
					continue;
				}

				/* Resets to first level. */
				if (cont.c_btn_pressed) {
					load_new_level = true;
					current_level = LEVEL_1;
					reset_box_count();
                    reset_controller(&cont);
					continue;
				}

				/* Send the controller to the player to process. */
				player_control(&entities[0], &cont);

                /* Reset controller for next loop. */
                reset_controller(&cont);

				/* Render tile where the player is currently located
				   before we step him. We don't want a trail made by
				   the player right? */
				render_tile(&map->map_data[entities[0].location.x +
				                           (entities[0].location.y *
				                            map->size.w)]);

				/* Here we step all entities, that includes the player, boxes
				   and goal. */
				step_entities(map, entities, get_box_count() + 2);

				/* Just render all entities. We can optimize this if we need
				   and/or want to do it. */
				render_entities(entities, get_box_count() + 2);
			}
			break;

		case THE_END:
			game_completed();
			break;
		}
	}
}
