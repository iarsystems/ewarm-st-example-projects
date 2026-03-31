/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   	 : buttons.c
 *    Description    :
 *
 *    History :
 *    1. Date        : February 10, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "buttons.h"

/*************************************************************************
 * Function Name: ButtonsInit
 * Parameters: none
 * Return: none
 * Description: Init Joystick and button
 *
 *************************************************************************/
void ButtonsInit (void)
{
GPIO_InitTypeDef  GPIO_InitStructure;
  // Enable GPIO7 clocks
  SCU_APBPeriphClockConfig(__GPIO7, ENABLE);
  // Release GPIO7 reset
  SCU_APBPeriphReset(__GPIO7,DISABLE);

  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIO7, &GPIO_InitStructure);

}

/*************************************************************************
 * Function Name: GetButtonsEvent
 * Parameters: none
 * Return: Int8U
 * Description: Return buttons
 *
 *************************************************************************/
Int8U GetButtons (void)
{
Int8U Tmp = GPIO_Read(GPIO7) >> 5;
  switch (Tmp)
  {
  case 0:
    return(JS_SELECT);
  case 1:
    return(JS_DOWN);
  case 2:
    return(JS_LEFT);
  case 3:
    return(JS_RIGHT);
  case 4:
    return(JS_UP);
  case 5:
    return(KEY_PB3);
  }
  return (0);
}
