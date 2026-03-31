/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : mpu.c
 *    Description : Cortex-M MPU initialization
 *
 *    History :
 *    1. Date        : May, 2015
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: #1 $
 **************************************************************************/
#include "iar_stm32f746xx_sk.h"
#include "mpu.h"

/*************************************************************************
 * Function Name: MPU_RegionConfig
 * Parameters: none
 *
 * Return: none
 *
 * Description: Simple MPU Configuration
 *************************************************************************/
void MPU_RegionConfig(void)
{
MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable MPU */
  HAL_MPU_Disable();

  /* Configure RAM region as Region 0, 8kB of size and R/W region */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = RAM_ADDRESS_START;
  MPU_InitStruct.Size = RAM_SIZE;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = RAM_REGION_NUMBER;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* Configure FLASH region as REGION 1, 1MB of size */
  MPU_InitStruct.BaseAddress = FLASH_ADDRESS_START;
  MPU_InitStruct.Size = FLASH_SIZE;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.Number = FLASH_REGION_NUMBER;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure Peripheral1 region as REGION 2, 0.5GB of size, R/W and Execute
  Never region */
  MPU_InitStruct.BaseAddress = PERIPH1_ADDRESS_START;
  MPU_InitStruct.Size = PERIPH1_SIZE;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.Number = PERIPH1_REGION_NUMBER;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure Peripheral2 region as REGION 3, 0.25GB of size, R/W and Execute
  Never region */
  MPU_InitStruct.BaseAddress = PERIPH2_ADDRESS_START;
  MPU_InitStruct.Size = PERIPH2_SIZE;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.Number = PERIPH2_REGION_NUMBER;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct); 
  
  /* Configure SDRAM region as REGION 4, 0.25GB of size, R/W */
  MPU_InitStruct.BaseAddress = SDR_ADDRESS_START;
  MPU_InitStruct.Size = SDR_SIZE;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.Number = SDR_REGION_NUMBER;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  //MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure FLASH region as REGION 5, 256MB of size */
  MPU_InitStruct.BaseAddress = QSPI_ADDRESS_START;
  MPU_InitStruct.Size = QSPI_SIZE;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.Number = FLASH_REGION_NUMBER;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable MPU */
  HAL_MPU_Enable(MPU_HFNMI_PRIVDEF);
}
