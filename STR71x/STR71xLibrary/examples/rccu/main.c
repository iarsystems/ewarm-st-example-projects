 /******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file presents a basic example to use the RCCU
*                      software library
********************************************************************************
* History:
* 24/05/05 : V3.0
* 30/11/04 : V2.0
* 16/05/03 : Created
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "71x_lib.h"
u32 MCLKval;
u32 APB1CLKval;
u32 APB2CLKval;
int main(void)
{
  #ifdef DEBUG
  debug();
  #endif

  /* RCCU peripheral configuration ------------------------------------------*/
  RCCU_Div2Config ( DISABLE);
  /*  Configure FCLK = RCLK /2 */
  RCCU_FCLKConfig ( RCCU_RCLK_2 );

  /*  Configure PCLK = RCLK /4 */
  RCCU_PCLKConfig (RCCU_RCLK_4);

  /*  Configure MCLK clock for the CPU, RCCU_DEFAULT = RCLK /1 */
  RCCU_MCLKConfig (RCCU_DEFAULT);

  /*  Configure the PLL1 ( * 12 , / 2 ) */
  RCCU_PLL1Config (RCCU_PLL1_Mul_12, RCCU_Div_2) ;

  /*  Wait PLL to lock */
  while(RCCU_FlagStatus(RCCU_PLL1_LOCK)==RESET);

  /*  Select PLL1_Output as RCLK clock */
  RCCU_RCLKSourceConfig (RCCU_PLL1_Output) ;

  MCLKval=RCCU_FrequencyValue (RCCU_MCLK);
  APB1CLKval=RCCU_FrequencyValue (RCCU_FCLK);
  APB2CLKval=RCCU_FrequencyValue (RCCU_PCLK);

  /*  At this step the MCLK = 24 MHz, APB1 clock = 12 MHz and APB2 = 6MHz
     with an external oscilator equal to 4MHz */

  while(1);
}
