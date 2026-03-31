/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file gives an example on the use of the Output
*                      compare feature.
********************************************************************************
* History:
* 13/01/2006 : V3.1
* 24/05/05   : V3.0
* 30/11/04   : V2.0
* 16/05/03   : Created
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "71x_lib.h"

/*
 This Device is configured to Toggle the TIM1 Output compare A pin from low
 level to high level after 0xF000 timer period. The GPIO pin is also toggled from
 low level to high level and may be used as a reference to verify the delay of the
 Output compare function.

                                    __________________________
                                    |                          TIM1 OCMPA
            ________________________|
                          ____________________________________
                         |                                     GPIO1 pin
            _____________|
                         <---------->
                         Output Compare Delay
*/

#define T1_OCMP_A 0x80

int main(void)
{
  #ifdef DEBUG
  debug();
  #endif

  /* ---------------------------------------------------------------------------
      Initialization
     --------------------------------------------------------------------------- */
  /*  Configure the GPIO Port */

  /*  Timer 1 Output Compare function pin (P1.7) configured as Alternate
      function pushpull */
  GPIO_Config (GPIO1, T1_OCMP_A, GPIO_AF_PP );

  /* P1.8 configured as output push pull */
  GPIO_Config (GPIO1, 0x0100, GPIO_OUT_PP );

  /*  Initialise the GPIO1 port to low level */
   GPIO_BitWrite(GPIO1,8,1);

  /* ---------------------------------------------------------------------------
      Configure the EIC Timer1 IRQ channel
     --------------------------------------------------------------------------- */

  /*  Enable the IRQ0 for timer 1 */
  EIC_IRQChannelConfig( T1TIMI_IRQChannel, ENABLE );
  EIC_IRQChannelPriorityConfig( T1TIMI_IRQChannel, 1);
  EIC_IRQConfig( ENABLE );

  /* ---------------------------------------------------------------------------
      Configure the TIM1
     --------------------------------------------------------------------------- */
  /*  Initialize the Timer 1 registers to reset values */
  TIM_Init ( TIM1 );

  /*  Configure the TIM Prescaler */
  TIM_PrescalerConfig ( TIM1, 0x1F );

  /*  Enable the Output Compare for the TIM1 peripheral */
   TIM_ITConfig ( TIM1, TIM_OCA_IT, ENABLE );

  /*  Configure the TIM1 Output Compare Function */
  
  TIM_OCMPModeConfig ( TIM1, TIM_CHANNEL_A, 0xF000, TIM_WAVE, TIM_HIGH );
  /*                     |       |             |        |         |
                TIM 1 <--+       |             |        |         +--> Toggle To
                                 |             |        |              High Level
      TIM 1 Output compare       |             |        |
         channelA             <--+             |        +--> OCMP function used for
                                               |             external wave generation.
       Delay before toggling <-----------------+                                       */

  /* P1.8 Forced To high Level */
 GPIO_BitWrite(GPIO1,8,0);

  /*  Infinite loop */
  while (1);
}
