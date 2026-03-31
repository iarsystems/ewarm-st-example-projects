/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : sdram_32M_16bit_drv.c
 *    Description : SDRAM driver K4S561632C-TC(L)75 4MBx16bitx4
 *
 *    History :
 *    1. Date        : July, 2013
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 3186 $
 **************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "arm_comm.h"
#include "iar_stm32f429ii_sk.h"
#include "sdram_32M_16bit_drv.h"
#include <arm_itm.h>

#define MEM_CAS 2
#define SDRAM_PERIOD          12
#define P2C(Period)           (((Period<SDRAM_PERIOD)?0:(Int32U)((Flo32)Period/SDRAM_PERIOD))+1)

#define SDRAM_REFRESH         7813
#define SDRAM_TRP             20
#define SDRAM_TRAS            45
#define SDRAM_TWR             5
#define SDRAM_TRC             65
#define SDRAM_TRCD            20
#define SDRAM_TXSR            67
#define SDRAM_TMRD            3


void Delayus(uint32_t time){
RCC_ClocksTypeDef RCC_Clocks;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;

  /* Enable TIM2 clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  RCC_GetClocksFreq(&RCC_Clocks);

  /* Init TIM2 */
  TIM_DeInit(TIM2);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_Pulse = ((RCC_Clocks.PCLK1_Frequency / 1)/1000000ul) * time;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  /* TIM2 Interrupts enable */
  TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);

  while(RESET == TIM_GetITStatus(TIM2, TIM_IT_CC1));

  TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
}

/*************************************************************************
 * Function Name: SDRAM_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: SDRAM controller and memory init
 *
 *************************************************************************/
void SDRAM_Init (void)
{
GPIO_InitTypeDef GPIO_InitStructure;
FMC_SDRAMCommandTypeDef FMC_SDRAMCommandStructure;
FMC_SDRAMInitTypeDef FMC_SDRAMInitStructure;
FMC_SDRAMTimingInitTypeDef FMC_SDRAMTimingInitStructure;

/* Enable FMC clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);

  /* Reset the FMC */
  RCC_AHB3PeriphResetCmd(RCC_AHB3Periph_FMC , ENABLE);
  RCC_AHB3PeriphResetCmd(RCC_AHB3Periph_FMC , DISABLE);

  /* Assign pins to EMC module */
  GPIO_InitStructure.GPIO_Pin = FMC_A0_PIN | FMC_A1_PIN  | FMC_A2_PIN | FMC_A3_PIN | FMC_A4_PIN
                              | FMC_A5_PIN | FMC_RAS_PIN | FMC_A6_PIN | FMC_A7_PIN
                              | FMC_A8_PIN | FMC_A9_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(FMC_PORT1, &GPIO_InitStructure);

  GPIO_PinAFConfig(FMC_PORT1, FMC_A1_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT1, FMC_A2_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT1, FMC_A3_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT1, FMC_A4_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT1, FMC_A5_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT1, FMC_RAS_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT1, FMC_A6_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT1, FMC_A7_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT1, FMC_A8_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT1, FMC_A9_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT1, FMC_A0_SOURCE, GPIO_AF_FMC);


  GPIO_InitStructure.GPIO_Pin = FMC_A10_PIN | FMC_A11_PIN | FMC_A12_PIN | FMC_BA0_PIN
                              | FMC_BA1_PIN | FMC_CLK_PIN | FMC_CAS_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(FMC_PORT2, &GPIO_InitStructure);

  GPIO_PinAFConfig(FMC_PORT2, FMC_A10_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT2, FMC_A11_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT2, FMC_A12_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT2, FMC_BA0_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT2, FMC_BA1_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT2, FMC_CLK_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT2, FMC_CAS_SOURCE, GPIO_AF_FMC);


  GPIO_InitStructure.GPIO_Pin = FMC_BL0_PIN | FMC_BL1_PIN | FMC_D4_PIN | FMC_D5_PIN
                              | FMC_D6_PIN  | FMC_D7_PIN  | FMC_D8_PIN | FMC_D9_PIN
                              | FMC_D10_PIN | FMC_D11_PIN | FMC_D12_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(FMC_PORT3, &GPIO_InitStructure);

  GPIO_PinAFConfig(FMC_PORT3, FMC_BL0_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT3, FMC_BL1_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT3, FMC_D4_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT3, FMC_D5_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT3, FMC_D6_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT3, FMC_D7_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT3, FMC_D8_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT3, FMC_D9_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT3, FMC_D10_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT3, FMC_D11_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT3, FMC_D12_SOURCE, GPIO_AF_FMC);

  GPIO_InitStructure.GPIO_Pin = FMC_D0_PIN | FMC_D1_PIN | FMC_D2_PIN | FMC_D3_PIN | FMC_D13_PIN | FMC_D14_PIN
                              | FMC_D15_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(FMC_PORT4, &GPIO_InitStructure);

  GPIO_PinAFConfig(FMC_PORT4, FMC_D0_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT4, FMC_D1_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT4, FMC_D2_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT4, FMC_D3_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT4, FMC_D13_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT4, FMC_D14_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT4, FMC_D15_SOURCE, GPIO_AF_FMC);

  GPIO_InitStructure.GPIO_Pin = FMC_NWE_PIN | FMC_NE0_PIN | FMC_CKE0_PIN ;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(FMC_PORT5, &GPIO_InitStructure);

  GPIO_PinAFConfig(FMC_PORT5, FMC_NWE_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT5, FMC_NE0_SOURCE, GPIO_AF_FMC);
  GPIO_PinAFConfig(FMC_PORT5, FMC_CKE0_SOURCE, GPIO_AF_FMC);

  FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay = SDRAM_TMRD;
  FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = P2C(SDRAM_TXSR);
  FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime = P2C(SDRAM_TRAS);
  FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay = P2C(SDRAM_TRC);
  FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime = SDRAM_TWR;
  FMC_SDRAMTimingInitStructure.FMC_RPDelay = P2C(SDRAM_TRP);
  FMC_SDRAMTimingInitStructure.FMC_RCDDelay = P2C(SDRAM_TRCD);

  FMC_SDRAMInitStructure.FMC_Bank = FMC_Bank1_SDRAM;
  FMC_SDRAMInitStructure.FMC_ColumnBitsNumber = FMC_ColumnBits_Number_9b;
  FMC_SDRAMInitStructure.FMC_RowBitsNumber = FMC_RowBits_Number_13b;
  FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth = FMC_SDMemory_Width_16b;
  FMC_SDRAMInitStructure.FMC_InternalBankNumber = FMC_InternalBank_Number_4;
  FMC_SDRAMInitStructure.FMC_CASLatency = (MEM_CAS == 2)?FMC_CAS_Latency_2:FMC_CAS_Latency_3;
  FMC_SDRAMInitStructure.FMC_WriteProtection = FMC_Write_Protection_Disable;
  FMC_SDRAMInitStructure.FMC_SDClockPeriod = FMC_SDClock_Period_3;
  FMC_SDRAMInitStructure.FMC_ReadBurst = FMC_Read_Burst_Enable;
  FMC_SDRAMInitStructure.FMC_ReadPipeDelay = FMC_ReadPipe_Delay_0;
  FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct = &FMC_SDRAMTimingInitStructure;

  FMC_SDRAMDeInit(FMC_Bank1_SDRAM);
  FMC_SDRAMInit(&FMC_SDRAMInitStructure);

  // start delivering the clock to the memory
  FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_CLK_Enabled;
  FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank1;
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

  Delayus(100);

  // PALL
  FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_PALL;
  FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank1;
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
  Delayus(100);

  // Auto refresh
  FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_AutoRefresh;
  FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank1;
  FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 7; // 8 cycles
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

  // set SDRAM reg
  FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = (1<<0) // Burst Length = 1
                                                       | (0<<3) // BT - Sequential
                                                       | (MEM_CAS<<4) // CAS = MEM_CAS
                                                       | (0<<7) // Op Mode = 0
                                                       | (1<<9) // WB - Programmed Burst Length
                                                       ;
  FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_LoadMode;
  FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank1;
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

  // Program the refresh rate
  FMC_SetRefreshCount(P2C(SDRAM_REFRESH));

  // norm operation
  FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_normal;
  FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank1;
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
  Delayus(200);
}
