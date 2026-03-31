/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : sdram_32M_16bit_drv.c
 *    Description : SDRAM driver K4S561632C-TC(L)75 4MBx16bitx4
 *
 *    History :
 *    1. Date        : 29, October 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    2. Date        : May, 2015
 *       Author      : Atanas Uzunov
 *       Description : Adapted for the new STM32F7xx HAL libraries
 *
 *    $Revision: #4 $
 **************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "arm_comm.h"
#include "iar_stm32f746xx_sk.h"
#include "sdram_32M_16bit_drv.h"

extern uint32_t SDRAM_BASE_ADDR;

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
TIM_HandleTypeDef Tim2Handler;
TIM_OC_InitTypeDef sConfig;

  /* Enable TIM2 clocks */
  __HAL_RCC_TIM2_CLK_ENABLE();

  /* Configure TIM2 */
  Tim2Handler.Instance = TIM2;
  /* If APB1 PRESC == 1 , TIM clocks are x1 else x2 */
  Tim2Handler.Init.Period            = 0xFFFFFFFF;
  Tim2Handler.Init.Prescaler         = 0;
  Tim2Handler.Init.ClockDivision     = 0;
  Tim2Handler.Init.CounterMode       = TIM_COUNTERMODE_UP;
  __HAL_UNLOCK(&Tim2Handler);
  if (HAL_TIM_OC_Init(&Tim2Handler) != HAL_OK)
  {
    /* Initialization Error */
    printf("TIM2 init fail!\r\n");
  }

  /* TIM2 Timing Mode configuration: Channel1 */
  sConfig.OCMode = TIM_OCMODE_TIMING;
  sConfig.Pulse = ((HAL_RCC_GetPCLK1Freq()*2 / 1)/1000000ul) * time;
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;

  HAL_TIM_OC_ConfigChannel(&Tim2Handler, &sConfig, TIM_CHANNEL_1);

  __HAL_TIM_SET_COUNTER(&Tim2Handler, 0);

  /* TIM2 Start OC */
  HAL_TIM_OC_Start(&Tim2Handler, TIM_CHANNEL_1);

  while(RESET == __HAL_TIM_GET_FLAG(&Tim2Handler, TIM_FLAG_CC1));

  __HAL_TIM_CLEAR_IT(&Tim2Handler, TIM_FLAG_CC1);

  HAL_TIM_OC_DeInit(&Tim2Handler);
}

#define PRINTF printf

/*************************************************************************
 * Function Name: SDRAM_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: SDRAM controller and memory init
 *
 *************************************************************************/
void SDRAM_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
FMC_SDRAM_CommandTypeDef command;
SDRAM_HandleTypeDef      hsdram;
FMC_SDRAM_TimingTypeDef  SDRAM_Timing;

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Reset the FMC */
  __HAL_RCC_FMC_FORCE_RESET();
  __HAL_RCC_FMC_RELEASE_RESET();

  /* Enable gpio clocks */
  FMC_PORT1_CLK_ENABLE();
  FMC_PORT2_CLK_ENABLE();
  FMC_PORT3_CLK_ENABLE();
  FMC_PORT4_CLK_ENABLE();
  FMC_PORT5_CLK_ENABLE();

  /* Assign pins to EMC module */
  GPIO_InitStructure.Pin       = FMC_A0_PIN | FMC_A1_PIN  | FMC_A2_PIN | FMC_A3_PIN | FMC_A4_PIN
                               | FMC_A5_PIN | FMC_RAS_PIN | FMC_A6_PIN | FMC_A7_PIN
                               | FMC_A8_PIN | FMC_A9_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(FMC_PORT1, &GPIO_InitStructure);

  GPIO_InitStructure.Pin       = FMC_A10_PIN | FMC_A11_PIN | FMC_A12_PIN | FMC_BA0_PIN
                               | FMC_BA1_PIN | FMC_CLK_PIN | FMC_CAS_PIN;
  HAL_GPIO_Init(FMC_PORT2, &GPIO_InitStructure);

  GPIO_InitStructure.Pin       = FMC_BL0_PIN | FMC_BL1_PIN | FMC_D4_PIN | FMC_D5_PIN
                               | FMC_D6_PIN  | FMC_D7_PIN  | FMC_D8_PIN | FMC_D9_PIN
                               | FMC_D10_PIN | FMC_D11_PIN | FMC_D12_PIN;
  HAL_GPIO_Init(FMC_PORT3, &GPIO_InitStructure);

  GPIO_InitStructure.Pin =       FMC_D0_PIN | FMC_D1_PIN | FMC_D2_PIN | FMC_D3_PIN | FMC_D13_PIN | FMC_D14_PIN
                               | FMC_D15_PIN;
  HAL_GPIO_Init(FMC_PORT4, &GPIO_InitStructure);

  GPIO_InitStructure.Pin       = FMC_NWE_PIN | FMC_NE0_PIN | FMC_CKE0_PIN ;
  HAL_GPIO_Init(FMC_PORT5, &GPIO_InitStructure);

  hsdram.Instance = FMC_SDRAM_DEVICE;

  SDRAM_Timing.LoadToActiveDelay    = SDRAM_TMRD;
  SDRAM_Timing.ExitSelfRefreshDelay = P2C(SDRAM_TXSR);
  SDRAM_Timing.SelfRefreshTime      = P2C(SDRAM_TRAS);
  SDRAM_Timing.RowCycleDelay        = P2C(SDRAM_TRC);
  SDRAM_Timing.WriteRecoveryTime    = SDRAM_TWR;
  SDRAM_Timing.RPDelay              = P2C(SDRAM_TRP);
  SDRAM_Timing.RCDDelay             = P2C(SDRAM_TRCD);

  hsdram.Init.SDBank             = FMC_SDRAM_BANK1;
  hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram.Init.CASLatency         = (MEM_CAS == 2)?FMC_SDRAM_CAS_LATENCY_2:FMC_SDRAM_CAS_LATENCY_3;
  hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_3;
  hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
  hsdram.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;

  /* Initialize the SDRAM controller */
  if(HAL_SDRAM_Init(&hsdram, &SDRAM_Timing) != HAL_OK)
  {
    /* Catch error */
    printf("SDRAM init fail!\r\n");
  }

  /* Start delivering the clock to the memory */
  command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  HAL_SDRAM_SendCommand(&hsdram, &command, 100);

  Delayus(100);

  /* PALL */
  command.CommandMode = FMC_SDRAM_CMD_PALL;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  HAL_SDRAM_SendCommand(&hsdram, &command, 100);
  Delayus(100);

  /* Auto refresh */
  command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  command.AutoRefreshNumber = 7; // 8 cycles
  HAL_SDRAM_SendCommand(&hsdram, &command, 100);

  /* Set SDRAM reg */
  command.ModeRegisterDefinition = (1<<0) // Burst Length = 1
                                 | (0<<3) // BT - Sequential
                                 | (MEM_CAS<<4) // CAS = MEM_CAS
                                 | (0<<7) // Op Mode = 0
                                 | (1<<9) // WB - Programmed Burst Length
                                 ;
  command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  HAL_SDRAM_SendCommand(&hsdram, &command, 100);

  /* Program the refresh rate */
  HAL_SDRAM_ProgramRefreshRate(&hsdram, P2C(SDRAM_REFRESH));

  /* Norm operation */
  command.CommandMode = FMC_SDRAM_CMD_NORMAL_MODE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  HAL_SDRAM_SendCommand(&hsdram, &command, 100);
  Delayus(200);
  /*
  while(1)
  {
    uint16_t * addr = (uint16_t *)&SDRAM_BASE_ADDR;
    for(int i = 0 ; i < 0x800000; i++)
    {
      *addr++ = 0x5555;
      *addr++ = 0xAAAA;
    }
  }
  */
}
