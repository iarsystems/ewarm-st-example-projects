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
GPIO_InitTypeDef GPIO_InitStructure;

  // Enable GPIO clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,
                         ENABLE);
  // Release GPIO reset
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,
                         DISABLE);

  // B2 (PB5)
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOB,&GPIO_InitStructure);

  // B1(PC13)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_Init(GPIOC,&GPIO_InitStructure);

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

  if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)  == Bit_SET)
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

  if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == Bit_SET)
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

