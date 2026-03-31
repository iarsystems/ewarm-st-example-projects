/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   	 : buttons.c
 *    Description    :
 *
 *    History :
 *    1. Date        : June 5, 2008
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
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_11;
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
Int8U Tmp = 0;
  Tmp |= (GPIO_ReadBit(GPIO1,GPIO_Pin_5 ) == Bit_SET)?BUTT_LEFT:0;
  Tmp |= (GPIO_ReadBit(GPIO1,GPIO_Pin_7 ) == Bit_SET)?BUTT_UP:0;
  Tmp |= (GPIO_ReadBit(GPIO1,GPIO_Pin_9 ) == Bit_SET)?BUTT_DOWN:0;
  Tmp |= (GPIO_ReadBit(GPIO1,GPIO_Pin_11) == Bit_SET)?BUTT_RIGHT:0;
  return (Tmp);
}
