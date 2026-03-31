#include <intrinsics.h>

#include "stm32f4xx_tim.h"

#include "init.h"
#include "framework/font.h"
#include "framework/mod.h"
#include "framework/connect.h"
#include "board/board_def.h"
#include "modules/mod_lcd6610.h"
#include "modules/mod_joystick.h"

/* Dimensions of the screen that we will paint to. */
#define SCREEN_X1 0
/* Haven't tried 131 pixel, it could show, but you never know. */
#define SCREEN_X2 130
/* For some reason the two first rows of pixels don't show... */
#define SCREEN_Y1 2
#define SCREEN_Y2 130

extern FontType_t Console_5_7;
extern void SystemInit(void);

void init_hw()
{
	__disable_interrupt();

	SystemInit();

	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	__enable_interrupt();
}

void enable_interrupt(IRQn_Type interrupt,
                      uint8_t preemption_prio,
                      uint8_t sub_prio)
{
	__disable_interrupt();

	NVIC_InitTypeDef interr;
	interr.NVIC_IRQChannel = interrupt;
	interr.NVIC_IRQChannelPreemptionPriority = preemption_prio;
	interr.NVIC_IRQChannelSubPriority = sub_prio;
	interr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&interr);

	__enable_interrupt();
}

void enable_timer(TIM_TypeDef * timer,
                  uint32_t enable_reg,
                  uint16_t prescaler,
                  uint32_t period)
{
	TIM_TimeBaseInitTypeDef timer_init_struct;

	RCC_APB1PeriphClockCmd(enable_reg, ENABLE);
	RCC_APB1PeriphResetCmd(enable_reg, DISABLE);

	timer_init_struct.TIM_Prescaler = prescaler;
	timer_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init_struct.TIM_Period = period;
	timer_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(timer, &timer_init_struct);

	TIM_ClearITPendingBit(timer, TIM_FLAG_Update);
	TIM_ITConfig(timer, TIM_FLAG_Update, ENABLE);

	TIM_Cmd(timer, ENABLE);
}

ConnectionHandler_t* init_display(const UEXTInfo_t *uext,
                                  uint16_t background_fill_color,
                                  uint16_t text_background_color,
                                  uint16_t text_color)
{
	ConnectionHandler_t *disp = ConnectModule(MOD_LCD6610,
                                              uext,
                                              &MOD_LCD6610_PG);

	MOD_LCD6610_init(disp);
	MOD_LCD6610_setFont(disp, &Console_5_7);
	MOD_LCD6610_setWindow(disp,
	                      SCREEN_X1, SCREEN_Y1,
	                      SCREEN_X2, SCREEN_Y2);
	MOD_LCD6610_fillWindow(disp, background_fill_color);
	MOD_LCD6610_setTextColor(disp, text_color);
	MOD_LCD6610_setTextBackground(disp, text_background_color);

	return disp;
}

ConnectionHandler_t* init_joystick(const UEXTInfo_t *uext)
{
	ConnectionHandler_t *conn = ConnectModule(MOD_JOYSTICK,
	                                          uext,
	                                          &MOD_JOYSTICK_PG);

	if (MOD_JOYSTICK_init(conn) == MOD_JOYSTICK_RESULT_OK) {
		return conn;
	}

	return NULL;
}
