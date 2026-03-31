/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 04/06/2003
* Description        : Short Example program using the GPIO 2 and Timer 1.
*                      The purpose of this program is to configure all the
*                      GPIO 2 pins ports as Push-Pull output, and
*                      the Timer 1 to generate overflow interrupt.
********************************************************************************
* History:
* 24/05/05 : V3.0
* 30/11/04 : V2.0
* 16/05/03 : Created
*******************************************************************************/
#include "71x_map.h"

int main(void)

{

  EIC->SIR[19] |= 0x00000001; /* Configure the channel 19 priority */
  EIC->IER |= 1<<19; /* Enable interrupt on channel 19 */

  EIC->ICR |= 0x00000001; /* Enable interrupts */


/* Configure GPIO2 in Push-Pull output */
  GPIO2->PC0 = 0xFFFF;
  GPIO2->PC1 = 0x0000 ;
  GPIO2->PC2 = 0xFFFF ;
  GPIO2->PD = 0x0800  ; /*  reset pins */


  /*  TIM1 configuration */
  TIM1->CR2 &= ~0x2000;
  TIM1->CR2  = 0x2009;
  TIM1->CR1  = 0x8000;
  TIM1->CNTR = 0xFFFF;

  while(1);  // infinite loop

}
/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
