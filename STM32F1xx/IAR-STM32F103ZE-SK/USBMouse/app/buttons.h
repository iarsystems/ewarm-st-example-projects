/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name      : buttons.h
 *    Description    : Buttons include header
 *
 *    History :
 *    1. Date        : 29, July 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include "includes.h"

#ifndef  __BUTTONS_H
#define  __BUTTONS_H

#define B1_MASK         GPIO_Pin_8
#define B1_PORT         GPIOG

#define B2_MASK         GPIO_Pin_0
#define B2_PORT         GPIOA

#define TAMPER_MASK     GPIO_Pin_13
#define TAMPER_PORT     GPIOC

#define JS_PORT         GPIOG
#define JS_RIGHT_MASK   GPIO_Pin_7
#define JS_LEFT_MASK    GPIO_Pin_12
#define JS_UP_MASK      GPIO_Pin_14
#define JS_DOWN_MASK    GPIO_Pin_13
#define JS_CENTER_MASK  GPIO_Pin_15

typedef union _Buttons_t
{
  Int32U Data;
  struct
  {
    Int32U JsUp     : 1;
    Int32U JsDown   : 1;
    Int32U JsRight  : 1;
    Int32U JsLeft   : 1;
    Int32U JsCenter : 1;
    Int32U B1       : 1;
    Int32U B2       : 1;
    Int32U Tamper   : 1;
    Int32U          :24;
  };
} Buttons_t, *pButtons_t;

/*************************************************************************
 * Function Name: ButtonsInit
 * Parameters: none
 * Return: none
 * Description: Init buttons
 *
 *************************************************************************/
void ButtonsInit (void);

/*************************************************************************
 * Function Name: GetButtons
 * Parameters: none
 * Return: Buttons_t
 * Description: Return current buttons states
 *
 *************************************************************************/
Buttons_t GetButtons (void);

#endif  /* __BUTTONS_H */
