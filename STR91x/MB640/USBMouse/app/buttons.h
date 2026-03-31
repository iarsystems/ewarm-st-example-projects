/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   	 : buttons.h
 *    Description    : buttons header file
 *
 *    History :
 *    1. Date        : August 10, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#ifndef  __BUTTONS_H
#define  __BUTTONS_H

// Joystick definitions

#define  JS_SELECT       1
#define  JS_DOWN         2
#define  JS_LEFT         4
#define  JS_RIGHT        8
#define  JS_UP          16
#define  KEY_PB3        32

/*************************************************************************
 * Function Name: ButtonsInit
 * Parameters: none
 * Return: none
 * Description: Init Joystick and button
 *
 *************************************************************************/
void ButtonsInit (void);

/*************************************************************************
 * Function Name: GetButtonsEvent
 * Parameters: none
 * Return: Int8U
 * Description: Return buttons
 *
 *************************************************************************/
Int8U GetButtons (void);

#endif  /* __BUTTONS_H */

