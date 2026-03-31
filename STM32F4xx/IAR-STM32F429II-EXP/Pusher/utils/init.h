#ifndef INIT_H_
#define INIT_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "framework/connect.h"
#include "framework/uext.h"

/*******************************************************************************
 ** Function Name : init_hw
 ** Parameters    : none
 ** Returns       : none
 **
 ** Description   : Does basic initialization of hardware.
 ******************************************************************************/
void init_hw();

/*******************************************************************************
 ** Function Name : enable_interrupt
 ** Parameters    : interrupt      - interrupt type.
 **                 preemtion_prio - Prieemtion priority.
 **                 sub_prio       - Sub priority.
 ** Returns       : none
 **
 ** Description   : Enables an interrupt.
 ******************************************************************************/
void enable_interrupt(IRQn_Type interrupt,
                      uint8_t preemption_prio,
                      uint8_t sub_prio);

/*******************************************************************************
 ** Function Name : enabler_timer
 ** Parameters    : timer      - Timer to enable.
 **                 enable_reg - Enable register.
 **                 prescaler  - Prescaler for timer.
 **                 period     - Period for timer.
 ** Returns       : none
 **
 ** Description   : Enables interrupt timer.
 ******************************************************************************/
void enable_timer(TIM_TypeDef * timer,
                  uint32_t enable_reg,
                  uint16_t prescaler,
                  uint32_t period);

/*******************************************************************************
 ** Function Name : init_display
 ** Parameters    : background_fill_color - Color to fill background with.
 **                 text_background_color - Color to have as background for
 **                                         text rendering.
 **                 text_color            - Color for text.
 ** Returns       : An initialized ConnectionHandler_t pointer for the color
 **                 display.
 **
 ** Description   : Initializes a display, sets font usage and clears the
 **                 screen.
 ******************************************************************************/
ConnectionHandler_t* init_display(const UEXTInfo_t *uext,
                                  uint16_t background_fill_color,
                                  uint16_t text_background_color,
                                  uint16_t text_color);

/*******************************************************************************
 ** Function Name : init_joystick
 ** Parameters    : uext - Whitch UEXT port to use.
 ** Returns       : A connection handler to the joystick, if something went
 **                 wrong, NULL will be returned.
 **
 ** Description   : Initializes the connection to a joystick.
 ******************************************************************************/
ConnectionHandler_t* init_joystick(const UEXTInfo_t *uext);

#endif
