/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : 28, December 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the Olimex STM32-103STK board.
 *  It shows basic use of parallel I/O, SPI, timer and interface to
 * a LCD PCD8544 compatible module.
 *  It downloads a picture with the IAR logo and implements a putchar
 * function.
 *
 *    $Revision: 1473 $
 **************************************************************************/
#include "includes.h"

extern const Int8U Logo[];

#define LED_ON()          GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_RESET)
#define LED_OFF()         GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_SET)


/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
void main(void)
{
GPIO_InitTypeDef GPIO_InitStructure;


  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  // Init LED Output
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOC, DISABLE);
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOC, ENABLE);

  // Configure PC12 STAT LED.
  GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_SET);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // NVIC init
#ifndef  EMB_FLASH
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  PCD8544_PowerUp(Logo);
  PCD8544_StrShow(0,5,"  IAR Systems");
  while(1)
  {
  }
}
