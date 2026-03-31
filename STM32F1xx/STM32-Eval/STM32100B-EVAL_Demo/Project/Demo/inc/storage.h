/**
 *******************************************************************************
 * @file     storage.h
 * @author   MCD Application Team
 * @version  V1.0.0
 * @date     03/01/2010
 * @brief    This file contains all the functions prototypes for the storage
 *           firmware driver.
 *******************************************************************************
 * @copy
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STORAGE_H
#define __STORAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32100B_EVAL_Demo
  * @{
  */

/** @addtogroup STORAGE
  * @{
  */

/** @defgroup STORAGE_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup STORAGE_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup STORAGE_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STORAGE_Exported_Functions
  * @{
  */
uint32_t Storage_Init (void);
uint32_t Storage_OpenReadFile(uint8_t *DirName, uint8_t *FileName, uint32_t BufferAddress, uint32_t *FileLen);
uint32_t Storage_OpenReadFileWave(uint8_t *DirName, uint8_t *FileName, uint8_t* BufferAddress, uint32_t *FileLen);
uint32_t Storage_GetDirectoryFiles (char* Files[]);
uint32_t Storage_GetDirectoryWaveFiles (char* Files[]);
uint32_t Storage_CheckBitmapFile(uint8_t *DirName, uint8_t *FileName, uint32_t BufferAddress, uint32_t *FileLen);

/* DosFs Low Layer functions */
uint32_t DFS_ReadSector(uint8_t unit, uint8_t *buffer, uint32_t sector, uint32_t count);
uint32_t DFS_WriteSector(uint8_t unit, uint8_t *buffer, uint32_t sector, uint32_t count);

#ifdef __cplusplus
}
#endif

#endif /* __STORAGE_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
