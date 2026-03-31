/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file gives an example of the use of the One Pulse
*                      Mode.
********************************************************************************
* History:
* 13/01/2006 : V3.1
* 24/05/2005 : V3.0
* 30/11/2004 : V2.0
* 16/05/2003 : Created
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
 The timer is configured to generate a pulse on the output compare A pin with
 configurable fixed length timer period. The pulse generation is activated by an external
 rising edge on the input capture A.

                      ______        __        ______
                     |      |      |  |      |      |
Input Capture A _____|      |______|  |______|      |___
                      ___           ___       ___
                     |   |         |   |     |   |
Output CompareA  ____|   |_________|   |_____|   |______
                     <--->
                  Same Pulse Length
*/

#define T1_OCMP_A 0x80
#define T1_ICAP_A 0x10

int main(void)
{
  #ifdef DEBUG
  debug ();
  #endif

  /* ---------------------------------------------------------------------------
      Initialization
     --------------------------------------------------------------------------- */
  /*  Configure the GPIO1 Port

   Timer 1 Output Compare function pin (P1.7) configured as Alternate
   function pushpull. */
  GPIO_Config (GPIO1 , T1_OCMP_A, GPIO_AF_PP );

  /*  Timer 1 Input Capture A (P1.4) configured as input TTL state */
  GPIO_Config (GPIO1 , T1_ICAP_A , GPIO_IN_TRI_TTL );

    /* ---------------------------------------------------------------------------
     Configure the TIM1
     --------------------------------------------------------------------------- */
  /*  Initialize the Timer 1 registers to reset values */
  TIM_Init ( TIM1 );

  /*  Configure the TIM Prescaler */
  TIM_PrescalerConfig ( TIM1, 0x1F );

  /*  Configure the TIM1 Output Compare Function : */
  /*    The OCMPA pin is configured to toggle from Low level to High level after
     0x3000 timer ticks. */
  TIM_OPModeConfig ( TIM1, 0xF, TIM_HIGH, TIM_LOW, TIM_RISING );
  /*                   |      |      |         |        |
              TIM 1 <--+      |      |         |        +--> Activation edge on ICAP_A
                              |      |         |
             Pulse Length  <--+      |         +--> Level After The pulse
                                     |
       Level During the Pulse    <---+                                             */


  /*  Start the Counter */
  TIM_CounterConfig ( TIM1, TIM_START );

  /*  Infinite loop */
  while (1);
}
