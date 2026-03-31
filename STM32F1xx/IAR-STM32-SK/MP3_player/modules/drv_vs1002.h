/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : drv_vs1002.h
 *    Description : VS1002 Driver include file
 *
 *    History :
 *    1. Date        : October 10, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"
#include "drv_vs1002_cnfg.h"

#ifndef  __DRV_VS1002_H
#define  __DRV_VS1002_H

#define MP3_ReadCmd     0x03
#define MP3_WriteCmd    0x02

#define MP3_VS1002_REV  2

#define VS1002_BUFFER_SIZE  2048

typedef enum _Dma_BurstSize_t
{
  Dma_1_Transfer = 0, Dma_4_Transfer, Dma_8_Transfer, Dma_16_Transfer,
  Dma_32_Transfer, Dma_64_Transfer, Dma_128_Transfer, Dma_254_Transfer
} Dma_BurstSize_t;

typedef enum _Dma_Width_t
{
  Dma_Byte = 0, Dma_HalfWord, Dma_Word
} Dma_Width_t;

typedef struct _DmaLli_t
{
  Int32U DMA_SrcAddr;
  Int32U DMA_DstAddr;
  Int32U DMA_NextLli;
  union
  {
    Int32U DMA_Ctrl;
    struct
    {
      Int32U TransferSize :12;
      Int32U SBSize       : 3;
      Int32U DBSize       : 3;
      Int32U SWidth       : 3;
      Int32U DWidth       : 3;
      Int32U              : 2;
      Int32U SInc         : 1;
      Int32U DInc         : 1;
      Int32U Prot0        : 1;
      Int32U Prot1        : 1;
      Int32U Prot2        : 1;
      Int32U TermCntIE    : 1;
    };
  };
} DmaLli_t, *pDmaLli_t;

//#define MP3_VOL_STEP    2     // 0.5 dB

typedef enum _MP3_RegAddr_t
{
  MP3_MODE = 0, MP3_STATUS, MP3_BASS, MP3_CLOCKF, MP3_DECODE_TIME, MP3_AUDATA,
  MP3_WRAM, MP3_WRAMADDR, MP3_HDAT0, MP3_HDAT1, MP3_AIADDR, MP3_VOL,
  MP3_AICTRL0,MP3_AICTRL1, MP3_AICTRL2, MP3_AICTRL3
} MP3_RegAddr_t;

typedef enum _MP3_Status_t
{
  MP3_Pass = 0,
  MP3_Fault,
  MP3_WrongRev,
  MP3_NotComm,
} MP3_Status_t;

typedef enum _MP3_Cmd_t
{
  Mp3CmdPowerUp = 0, Mp3CmdPowerDown, Mp3CmdGetRevision , Mp3CmdSetClkReg,
  Mp3CmdSetModeReg, Mp3CmdSetVol, Mp3CmdTstSin, Mp3CmdPlay,
  Mp3CmdPlayStop, Mp3CmdWideStereo, Mp3CmdLoudness
} MP3_Cmd_t;

typedef enum Mp3TransferStatus_t
{
  Mp3NotDataTransfer = 0, Mp3DataTransferProgress,
  Mp3DataTransferComplete, Mp3DataTransferCompleteWithError,
} Mp3TransferStatus_t;

typedef enum _Mp3PlaySpeed_t
{
  Mp3PlayNorm = 0, Mp3PlayIncreaseSpeed, Mp3PlayDecreaseSpeed,
} Mp3PlaySpeed_t;

typedef struct _Mp3Stream_t
{
  pInt32U pStream;
  Int32U Size;
  Mp3PlaySpeed_t PlaySpeed;
} Mp3Stream_t, *pMp3Stream_t;

typedef enum _Mp3StreamId_t
{
  MPG_2_5_1_4a = 0, MPG_2_5_1_4b, MPG_2_0_1_2, ISO_11172_3_1_0
} Mp3StreamId_t;

extern void DelayResolution100us(Int32U Dly);
#define Mp3SpiBufferDrainDly() for(volatile Int32U drain_dly = 15; drain_dly; --drain_dly)

/*************************************************************************
 * Function Name: Mp3SendCmd
 * Parameters: MP3_Cmd_t Cmd, pInt32U pData
 * Return: none
 *
 * Description: Send command to the MP3 module (VS1002)
 *
 *************************************************************************/
void Mp3SendCmd (MP3_Cmd_t Cmd, pInt32U pData);

#endif  /* __DRV_VS1002_H */
