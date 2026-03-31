/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Data        : 2 Nov. 2005
 *       Author      : SBB
 *       Description : Create
 * Jumpers:
 *  EXT/JLINK - depend of the power source
 *  M0.L/M0.H - 2-3
 *  M1.L/M1.H - 2-3
 *  R-T       - Absence
 *
 *    $Revision: 39 $
**************************************************************************/
#include <intrinsics.h>
#include "73x_lib.h"

extern void ARMIRQ_Enable (void);   /* Imported from 73x_init.s Assembler file */
extern void ARMFIQ_Enable (void);   /* Imported from 73x_init.s Assembler file */
extern void ARMIRQ_Disable (void);  /* Imported from 73x_init.s Assembler file */
extern void ARMFIQ_Disable (void);  /* Imported from 73x_init.s Assembler file */

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: Main routine
 *
 *************************************************************************/
int main( void )
{
CMU_InitTypeDef    CMU_InitStructure;
PRCCU_InitTypeDef  PRCCU_InitStructure;
EIC_InitTypeDef    EIC_InitStructure;
GPIO_InitTypeDef   GPIO_InitStructure;
TIM_InitTypeDef    TIM_InitStructure;

  /* disable global interrupts */
  ARMIRQ_Disable ();  /* Imported from 73x_init.s Assembler file */
  ARMFIQ_Disable ();  /* Imported from 73x_init.s Assembler file */

  /* Clock set */
  /* CMU Initialization */
  CMU_DeInit();
  CMU_StructInit(&CMU_InitStructure);

  /* Enable the external oscillator */
  CMU_InitStructure.CMU_CKSEL0 = CMU_CKSEL0_CKOSC;
  CMU_InitStructure.CMU_CKSEL2 = CMU_CKSEL2_CKOSC;
  CMU_Init(&CMU_InitStructure);

  /* PRCCU Initialization */
  PRCCU_DeInit();
  PRCCU_StructInit(&PRCCU_InitStructure);
  /* Set PLL frequency OSC*16/2*2 = 16MHz */
  PRCCU_InitStructure.PRCCU_DIV2        = ENABLE;
  PRCCU_InitStructure.PRCCU_MCLKSRC_SRC = PRCCU_MCLKSRC_PLL;
  PRCCU_InitStructure.PRCCU_PLLDIV      = PRCCU_PLLDIV_2;
  PRCCU_InitStructure.PRCCU_PLLMUL      = PRCCU_PLLMUL_16;
  PRCCU_InitStructure.PRCCU_FREEN       = DISABLE;
  PRCCU_Init(&PRCCU_InitStructure);

  /* GPIO Initialization */
  /* GPIO0 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_GPIO0 , ENABLE);
  /* GPIO0 Configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_InitStructure.GPIO_Pins = LED_MASK_P0 ;
  GPIO_Init (GPIO0, &GPIO_InitStructure);
  GPIO_WordWrite(GPIO0,1);

  /* EIC Initialization */
  /* EIC Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_EIC, ENABLE);
  EIC_DeInit();
  /* EIC Config */
  EIC_StructInit(&EIC_InitStructure);
  EIC_Init (&EIC_InitStructure);
  EIC_IRQChannelConfig( TIM1_IRQChannel , ENABLE );
  EIC_IRQChannelPriorityConfig( TIM1_IRQChannel, 1);

  /* TIM1 Initialization */
  /* TIM1 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_TIM1, ENABLE);
  TIM_DeInit(TIM1);
  /* TIM1 Config */
  TIM_CounterCmd(TIM1,TIM_STOP);
  TIM_CounterCmd(TIM1,TIM_CLEAR);
  TIM_StructInit(&TIM_InitStructure);
  TIM_InitStructure.TIM_Mode            = TIM_OCM_CHANNELA;
  TIM_InitStructure.TIM_OCA_Modes       = TIM_Timing;
  TIM_InitStructure.TIM_Clock_Source    = TIM_CLK_INTERNAL;
  TIM_InitStructure.TIM_Pulse_Length_A  = TIMING;
  TIM_InitStructure.TIM_Prescaler       = 0xFF;
  TIM_Init(TIM1,&TIM_InitStructure);
  /* TIM1 Output compare A interrupt enable */
  TIM_ITConfig(TIM1,TIM_IT_OCA,ENABLE);

  /* enable global interrupts */
  EIC_IRQCmd(ENABLE);
  ARMIRQ_Enable();

  /* start timer */
  TIM_CounterCmd(TIM1,TIM_START);

  while( 1 )
  {
  }
}

