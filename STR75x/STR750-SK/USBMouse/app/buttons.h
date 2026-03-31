/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   	 : buttons.h
 *    Description    : buttons header file
 *
 *    History :
 *    1. Date        : June 5, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include <includes.h>

#ifndef  __BUTTONS_H
#define  __BUTTONS_H

// Joystick definitions

#define  BUTT_DOWN         2
#define  BUTT_LEFT         4
#define  BUTT_RIGHT        8
#define  BUTT_UP          16

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

