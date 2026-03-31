/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : keys.c
 *    Description : keys module
 *
 *    History :
 *    1. Date        : October 10, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 *
 **************************************************************************/
#include    "keys.h"

Key_t Key;

/*************************************************************************
 * Function Name: KeyInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init keys
 *
 *************************************************************************/
void KeyInit(void)
{
GPIO_InitTypeDef  GPIO_InitStructure;

  // Enable GPIO clocks
  SCU_APBPeriphClockConfig(__GPIO7 , ENABLE);
  // Release GPIO reset
  SCU_APBPeriphReset(__GPIO7, DISABLE);

  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_Init(GPIO7, &GPIO_InitStructure);

  Key.Data = 0;
}

/*************************************************************************
 * Function Name: GetKeys
 * Parameters: none
 *
 * Return: none
 *
 * Description: Return pressed keys
 *
 *************************************************************************/
Key_t GetKeys (void)
{
  Key_t KeyHold = Key;
  Key.PlayStop =  Key.Next = 0;
  return(KeyHold);
}

/*************************************************************************
 * Function Name: GenreteKeyPress
 * Parameters: Int32U Keys
 *
 * Return: none
 *
 * Description: Generate key press
 *
 *************************************************************************/
void GenreteKeyPress (Int32U Keys)
{
  Key.Data |= Keys & (KeyPlayStopMask | KeyNextMask);
}

/*************************************************************************
 * Function Name: KeyImpl
 * Parameters: none
 *
 * Return: none
 *
 * Description: Scan keys
 *
 *************************************************************************/
void KeyImpl (void)
{
static Int32U KeyPlayStop_Cnt = 0;
static Int32U KeyNext_Cnt = 0;

  if (GPIO_ReadBit(GPIO7,GPIO_Pin_5) == Bit_RESET)
  {
    if(KeyNext_Cnt <= KEY_PRESS_DLY)
    {
      ++KeyNext_Cnt;
    }
  }
  else
  {
    KeyNext_Cnt = 0;
  }

  if (GPIO_ReadBit(GPIO7,GPIO_Pin_6) == Bit_RESET)
  {
    if(KeyPlayStop_Cnt <= KEY_PRESS_DLY)
    {
      ++KeyPlayStop_Cnt;
    }
  }
  else
  {
    KeyPlayStop_Cnt = 0;
  }

  if(KeyNext_Cnt == KEY_PRESS_DLY)
  {
    Key.Next = 1;
  }

  if(KeyPlayStop_Cnt == KEY_PRESS_DLY)
  {
    Key.PlayStop = 1;
  }

}

