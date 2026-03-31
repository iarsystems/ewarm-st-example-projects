/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name      : buttons.c
 *    Description    : Buttons module
 *
 *    History :
 *    1. Date        : 29, July 2008
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
 * Description: Init buttons
 *
 *************************************************************************/
void ButtonsInit (void)
{
GPIO_InitTypeDef GPIO_InitStructure;

  // GPIO enable clock and release Reset
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOA
                         | RCC_APB2Periph_GPIOC
                         | RCC_APB2Periph_GPIOG, DISABLE);
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA
                         | RCC_APB2Periph_GPIOC
                         | RCC_APB2Periph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  B1_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(B1_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  B2_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(B2_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  TAMPER_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TAMPER_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = JS_RIGHT_MASK | JS_LEFT_MASK | JS_UP_MASK |\
                                JS_DOWN_MASK | JS_CENTER_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(JS_PORT, &GPIO_InitStructure);
}

/*************************************************************************
 * Function Name: GetButtons
 * Parameters: none
 * Return: Buttons_t
 * Description: Return current buttons states
 *
 *************************************************************************/
Buttons_t GetButtons (void)
{
Buttons_t Buttons = {0};
  if(!(B1_PORT->IDR & B1_MASK))
  {
    Buttons.B1 = 1;
  }
  if(B2_PORT->IDR & B2_MASK)
  {
    Buttons.B2 = 1;
  }
  if(!(TAMPER_PORT->IDR & TAMPER_MASK))
  {
    Buttons.Tamper = 1;
  }
  if(JS_PORT->IDR & JS_RIGHT_MASK)
  {
    Buttons.JsRight = 1;
  }
  if(JS_PORT->IDR & JS_LEFT_MASK)
  {
    Buttons.JsLeft = 1;
  }
  if(JS_PORT->IDR & JS_UP_MASK)
  {
    Buttons.JsUp = 1;
  }
  if(JS_PORT->IDR & JS_DOWN_MASK)
  {
    Buttons.JsDown = 1;
  }
  if(JS_PORT->IDR & JS_CENTER_MASK)
  {
    Buttons.JsCenter = 1;
  }
  return(Buttons);
}
