/**
  ******************************************************************************
  * @file    SAI/SAI_AudioPlayback/Src/stm32h7xx_hal_msp.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    21-April-2017
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup SAI_AudioPlayback
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern SAI_HandleTypeDef SaiInputHandle;
extern SAI_HandleTypeDef SaiOutputHandle;
DMA_HandleTypeDef        SaiInputDmaHandle;
DMA_HandleTypeDef        SaiOutputDmaHandle;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  SAI MSP Init.
  * @param  hsai : pointer to a SAI_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */
void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  GPIO_InitTypeDef  GPIO_Init;

  if(hsai->Instance == AUDIO_IN_SAI_PDMx)
  {
    /* Enable SAI clock */
    AUDIO_IN_SAI_PDMx_CLK_ENABLE();

    AUDIO_IN_SAI_PDMx_CLK_IN_ENABLE();
    AUDIO_IN_SAI_PDMx_DATA_IN_ENABLE();

    GPIO_Init.Pin = AUDIO_IN_SAI_PDMx_CLK_IN_PIN;
    GPIO_Init.Mode = GPIO_MODE_AF_PP;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = AUDIO_IN_SAI_PDMx_DATA_CLK_AF;
    HAL_GPIO_Init(AUDIO_IN_SAI_PDMx_CLK_IN_PORT, &GPIO_Init);

    GPIO_Init.Pull = GPIO_PULLUP;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_Init.Pin = AUDIO_IN_SAI_PDMx_DATA_IN_PIN;
    HAL_GPIO_Init(AUDIO_IN_SAI_PDMx_DATA_IN_PORT, &GPIO_Init);

    AUDIO_IN_SAI_PDMx_FS_SCK_ENABLE();

    /* CODEC_SAI pins configuration: FS, SCK, MCK and SD pins */
    GPIO_Init.Pin = AUDIO_IN_SAI_PDMx_FS_PIN | AUDIO_IN_SAI_PDMx_SCK_PIN;
    GPIO_Init.Mode = GPIO_MODE_AF_PP;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = AUDIO_IN_SAI_PDMx_FS_SCK_AF;
    HAL_GPIO_Init(AUDIO_IN_SAI_PDMx_FS_SCK_GPIO_PORT, &GPIO_Init);

    /* Enable the DMA clock */
    AUDIO_IN_SAI_PDMx_DMAx_CLK_ENABLE();

    /* Configure the hdma_sai_rx handle parameters */
    SaiInputDmaHandle.Init.Request             = AUDIO_IN_SAI_PDMx_DMAx_REQUEST;
    SaiInputDmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    SaiInputDmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
    SaiInputDmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
    SaiInputDmaHandle.Init.PeriphDataAlignment = AUDIO_IN_SAI_PDMx_DMAx_PERIPH_DATA_SIZE;
    SaiInputDmaHandle.Init.MemDataAlignment    = AUDIO_IN_SAI_PDMx_DMAx_MEM_DATA_SIZE;
    SaiInputDmaHandle.Init.Mode                = DMA_CIRCULAR;
    SaiInputDmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;
    SaiInputDmaHandle.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    SaiInputDmaHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    SaiInputDmaHandle.Init.MemBurst            = DMA_MBURST_SINGLE;
    SaiInputDmaHandle.Init.PeriphBurst         = DMA_MBURST_SINGLE;

    SaiInputDmaHandle.Instance = AUDIO_IN_SAI_PDMx_DMAx_STREAM;

    /* Associate the DMA handle */
    __HAL_LINKDMA(&SaiInputHandle, hdmarx, SaiInputDmaHandle);

    /* Deinitialize the Stream for new transfer */
    HAL_DMA_DeInit(&SaiInputDmaHandle);

    /* Configure the DMA Stream */
    HAL_DMA_Init(&SaiInputDmaHandle);

    /* SAI DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(AUDIO_IN_SAI_PDMx_DMAx_IRQ, AUDIO_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(AUDIO_IN_SAI_PDMx_DMAx_IRQ);
  }

  if(hsai->Instance == AUDIO_OUT_SAIx)
  {
    /* Enable SAI clock */
    AUDIO_OUT_SAIx_CLK_ENABLE();

    /* Enable GPIO clock */
    AUDIO_OUT_SAIx_SD_FS_CLK_ENABLE();

    /* CODEC_SAI pins configuration: FS, SCK, MCK and SD pins */
    GPIO_Init.Pin = AUDIO_OUT_SAIx_FS_PIN | AUDIO_OUT_SAIx_SCK_PIN | AUDIO_OUT_SAIx_SD_PIN;
    GPIO_Init.Mode = GPIO_MODE_AF_PP;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Init.Alternate = AUDIO_OUT_SAIx_AF;
    HAL_GPIO_Init(AUDIO_OUT_SAIx_SD_FS_SCK_GPIO_PORT, &GPIO_Init);

    /* Enable the DMA clock */
    AUDIO_OUT_SAIx_DMAx_CLK_ENABLE();

    /* Configure the hdma_sai_tx handle parameters */
    SaiOutputDmaHandle.Init.Request             = AUDIO_OUT_SAIx_DMAx_REQUEST;
    SaiOutputDmaHandle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    SaiOutputDmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
    SaiOutputDmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
    SaiOutputDmaHandle.Init.PeriphDataAlignment = AUDIO_OUT_SAIx_DMAx_PERIPH_DATA_SIZE;
    SaiOutputDmaHandle.Init.MemDataAlignment    = AUDIO_OUT_SAIx_DMAx_MEM_DATA_SIZE;
    SaiOutputDmaHandle.Init.Mode                = DMA_CIRCULAR;
    SaiOutputDmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;
    SaiOutputDmaHandle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
    SaiOutputDmaHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    SaiOutputDmaHandle.Init.MemBurst            = DMA_MBURST_SINGLE;
    SaiOutputDmaHandle.Init.PeriphBurst         = DMA_PBURST_SINGLE;

    SaiOutputDmaHandle.Instance = AUDIO_OUT_SAIx_DMAx_STREAM;

    /* Associate the DMA handle */
    __HAL_LINKDMA(&SaiOutputHandle, hdmatx, SaiOutputDmaHandle);

    /* Deinitialize the Stream for new transfer */
    HAL_DMA_DeInit(&SaiOutputDmaHandle);

    /* Configure the DMA Stream */
    HAL_DMA_Init(&SaiOutputDmaHandle);

    /* SAI DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(AUDIO_OUT_SAIx_DMAx_IRQ, AUDIO_IRQ_PREPRIO, 0);
    HAL_NVIC_EnableIRQ(AUDIO_OUT_SAIx_DMAx_IRQ);
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
