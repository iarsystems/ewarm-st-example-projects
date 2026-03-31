/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : main.c
 *      Description : Initializes hardware and game and runs it.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#include <intrinsics.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "framework/uext.h"
#include "framework/mod.h"
#include "framework/connect.h"
#include "board/board_def.h"
#include "modules/mod_lcd6610.h"

#include "utils/init.h"
#include "pusher/pusher.h"
#include "pusher/render.h"
#include "pusher/map.h"
#include "pusher/player.h"
#include "pusher/entity.h"
#include "pusher/controller.h"
#include "pusher/box.h"

ConnectionHandler_t *display_handler;
ConnectionHandler_t *joystick_handler;

void init_stuff()
{
	init_hw();
	enable_timer(TIM2, RCC_APB1Periph_TIM2, 3200, 2000);
	enable_timer(TIM3, RCC_APB1Periph_TIM3, 3200, 2000);

	enable_interrupt(TIM2_IRQn, 7, 0);
	enable_interrupt(TIM3_IRQn, 7, 0);
	init_assets();

	display_handler = init_display(UEXT2, 0x000, 0x000, 0x0FFF);
	joystick_handler = init_joystick(UEXT1);
	init_render(display_handler, 2, 11);

	init_pusher(joystick_handler);
}

void main()
{
	init_stuff();

	run_pusher();

	while (1) {}
}
