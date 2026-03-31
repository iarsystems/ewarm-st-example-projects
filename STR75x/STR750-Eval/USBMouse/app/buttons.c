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
  // Enable GPIO clock
  MRCC_PeripheralClockConfig(MRCC_Peripheral_GPIO, ENABLE);
  // Release GPIO reset
  MRCC_PeripheralSWResetConfig(MRCC_Peripheral_GPIO,DISABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_8;
  GPIO_Init(GPIO0, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_Init(GPIO1, &GPIO_InitStructure);
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
Int8U Tmp;
  Tmp  = (GPIO_ReadBit(GPIO0,GPIO_Pin_2 ) == Bit_RESET)?JS_SELECT:0;
  Tmp |= (GPIO_ReadBit(GPIO0,GPIO_Pin_8 ) == Bit_RESET)?JS_UP:0;
  Tmp |= (GPIO_ReadBit(GPIO1,GPIO_Pin_5 ) == Bit_RESET)?KEY_PB3:0;
  Tmp |= (GPIO_ReadBit(GPIO1,GPIO_Pin_7 ) == Bit_RESET)?JS_DOWN:0;
  Tmp |= (GPIO_ReadBit(GPIO1,GPIO_Pin_12) == Bit_RESET)?JS_LEFT:0;
  Tmp |= (GPIO_ReadBit(GPIO1,GPIO_Pin_13) == Bit_RESET)?JS_RIGHT:0;
  return (Tmp);
}
