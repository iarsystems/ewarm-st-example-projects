/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : drv_vs1002.c
 *    Description : VS1002 driver
 *
 *    History :
 *    1. Date        : October 10, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
**************************************************************************/
#include "drv_vs1002.h"

#define MP3_VER(var)       ((var)?var:MP3_Pass)
#define MP3_SHORT(var)     ((var)?MP3_Fault:MP3_Pass)

#define MP3_RET(var) return((MP3_STATUS_VERBOSE?MP3_VER(var):MP3_SHORT(var)))

#define MP3_BOOT_SEL  GPIO_Pin_1
#define MP3_RST       GPIO_Pin_1
#define MP3_DATA_RQ   GPIO_Pin_0

#define MP3_CS        GPIO_Pin_0

#define MP3_MOSI      GPIO_Pin_6
#define MP3_MISO      GPIO_Pin_5
#define MP3_SCLK      GPIO_Pin_4

#define TMR_PERIOD(Val) (8000000UL*MP3_STREAM_SIZE*8/(Val*1000UL))

const Int32U ISO11172_Bitrate[] = {
  0,
  TMR_PERIOD( 32),TMR_PERIOD( 40),TMR_PERIOD( 48),TMR_PERIOD( 56),TMR_PERIOD( 64),
  TMR_PERIOD( 80),TMR_PERIOD( 96),TMR_PERIOD(112),TMR_PERIOD(128),TMR_PERIOD(160),
  TMR_PERIOD(192),TMR_PERIOD(224),TMR_PERIOD(256),TMR_PERIOD(320),
  0
};

const Int32U MPG2_Bitrate[] = {
  0,
  TMR_PERIOD(  8),TMR_PERIOD( 16),TMR_PERIOD( 24),TMR_PERIOD( 32),TMR_PERIOD( 40),
  TMR_PERIOD( 48),TMR_PERIOD( 56),TMR_PERIOD( 64),TMR_PERIOD( 80),TMR_PERIOD( 96),
  TMR_PERIOD(112),TMR_PERIOD(128),TMR_PERIOD(144),TMR_PERIOD(160),
  0
};

volatile Mp3TransferStatus_t Mp3TransferStatus; // data transfer state

Boolean PlayFile = 0;     // Play in progress
Int32U Mp2DmaTmp;         // local buffer for transfer without incitement

Int32U DmaPeriodHold;     // Sample period of a steam transfer
volatile pInt8U pMp3Data; // Stream transfer buffer
volatile Int32S Mp3Size;  // Stream transfer remaining bytes
Boolean AddInc = 0;       // disable/enable address increment

/*************************************************************************
 * Function Name: MP3_Reset
 * Parameters: Boolean Select
 * Return: none
 *
 * Description: MP3 reset and disable booting from external EEPROM
 * Select = true  - Reset the VS1002
 * Select = false - Release reset of the VS1002
 *
 *************************************************************************/
static inline
void MP3_Reset (Boolean Select)
{
  // Set the Boot select low
  GPIO_WriteBit(GPIO2,MP3_BOOT_SEL,Bit_RESET);
  GPIO_WriteBit(GPIO3,MP3_RST,Select?Bit_RESET:Bit_SET);
}

/*************************************************************************
 * Function Name: MP3_DReq
 * Parameters: none
 * Return: Boolean
 *
 * Description: Return state of the DREQ line
 *
 *************************************************************************/
static inline
Boolean MP3_DReq (void)
{
  return(GPIO_ReadBit(GPIO3,MP3_DATA_RQ) == Bit_SET);
}

/*************************************************************************
 * Function Name: MP3_ChipSelect
 * Parameters: Boolean Select
 * Return: none
 *
 * Description: MP3 chip select control
 * Select = true  - Chip is enable - command
 * Select = false - Chip is disable - data
 *
 *************************************************************************/
static inline
void MP3_ChipSelect (Boolean Select)
{
  GPIO_WriteBit(GPIO2,MP3_CS,Select?Bit_RESET:Bit_SET);
}

/*************************************************************************
 * Function Name: MP3_SetClockFreq
 * Parameters: Int32U Frequency
 * Return: Int32U
 *
 * Description: Set SPI ckl frequency
 *
 *************************************************************************/
static inline
Int32U MP3_SetClockFreq (Int32U Frequency)
{
Int32U Div = 2;
Int32U Pclk;
  Pclk = SCU_GetMCLKFreqValue()*1000;
  if(!(SCU->CLKCNTR & 1UL << 9))
  {
    Pclk >>= 1; // /2
  }

  while((Frequency * Div) <=  Pclk)
  {
    Div += 2;
  }

  if (Div > 254)
  {
    Div = 254;
  }

  SSP1->PR = Div;

  // Return real frequency
  return(Pclk/Div);
}

/*************************************************************************
 * Function Name: MP3_TranserByte
 * Parameters: Int8U ch
 * Return: Int16U
 *
 * Description: Read 8 bits from SPI
 *
 *************************************************************************/
static
Int16U MP3_TranserByte (Int8U ch)
{
  SSP_SendData(SSP1, ch);
  while(SSP_GetFlagStatus(SSP1, SSP_FLAG_RxFifoNotEmpty) == RESET);
  return(SSP_ReceiveData(SSP1));
}


/*************************************************************************
 * Function Name: MP3_RxFifoDrain
 * Parameters: void
 *
 * Return: void
 *
 * Description: Drain the RX FIFO
 *
 *************************************************************************/
static inline
void MP3_RxFifoDrain (void)
{
  while (SSP_GetFlagStatus(SSP1, SSP_FLAG_RxFifoNotEmpty) == SET)
  {
    Int32U Dummy = SSP_ReceiveData(SSP1);
  }
}

/*************************************************************************
 * Function Name: MP3_WaitTxFifoEmptying
 * Parameters: void
 *
 * Return: void
 *
 * Description: Wait emptying of the TX FIFO
 *
 *************************************************************************/
static inline
void MP3_WaitTxFifoEmptying (void)
{
  while (SSP_GetFlagStatus(SSP1, SSP_FLAG_TxFifoEmpty) == RESET);
}

/*************************************************************************
 * Function Name: Mp3ModuleInit
 * Parameters: Int32U DmaIntrPriority, Int32U DmaTimerIntrPriority
 * Return: MP3_Status_t
 *          MP3_Pass, MP3_Fault or MP3_WrongRev, MP3_NotComm
 *
 * Description: Initialize MP3 module (VS1002)
 *
 *************************************************************************/
static inline
MP3_Status_t Mp3ModuleInit (Int32U DmaIntrPriority, Int32U DmaTimerIntrPriority)
{
SSP_InitTypeDef   SSP_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;
Int32U i;

  // Initialize DMA
  // Enable DMA clock
  SCU_AHBPeriphClockConfig(__DMA , ENABLE);
  // Reinitializes the DMA
  SCU_AHBPeriphReset(__DMA, ENABLE);
  // Release DMA reset
  SCU_AHBPeriphReset(__DMA, DISABLE);
  // Enable DMA module
  DMA_Cmd(ENABLE);
  DMA_SyncConfig(DMA_SSP1_RX_Mask | DMA_SSP1_TX_Mask, DISABLE);
  // Clear pending interrupt
  DMA_ClearIT(Channel6,DMA_TCC);
  DMA_ClearIT(Channel6,DMA_EC);

  // VIC configuration
  VIC_Config(DMA_ITLine, VIC_IRQ, DmaIntrPriority);
  VIC_ITCmd(DMA_ITLine, ENABLE);

  Mp3TransferStatus = Mp3NotDataTransfer;

  // Initialize SPI1 and IO
  // Enable GPIO, SPI1 and Timer0/1 clock
  SCU_APBPeriphClockConfig(__GPIO2 | __GPIO3 | __SSP1 | __TIM01, ENABLE);
  // Reinitialize the SPI1
  SCU_APBPeriphReset(__SSP1, ENABLE);
  // Release GPIO, SPI1 and Timer 0/1 reset
  SCU_APBPeriphReset(__GPIO2 | __GPIO3 | __SSP1 | __TIM01, DISABLE);
  // APB peripheral clock disabled during ARM debug state
  SCU_APBPeriphDebugConfig(__TIM01,DISABLE);

  // Configure Boot select pin
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = MP3_BOOT_SEL;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt1;
  GPIO_Init (GPIO2, &GPIO_InitStructure);

  // Configure MP3 Reset pin
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = MP3_RST;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt1;
  GPIO_Init (GPIO3, &GPIO_InitStructure);

  // Configure MP3 CS pin
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = MP3_CS;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt1;
  GPIO_Init (GPIO2, &GPIO_InitStructure);

  // Configure data request pin
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Pin = MP3_DATA_RQ;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_Init (GPIO3, &GPIO_InitStructure);

  // Configure SSP1_CLK, SSP1_MOSI pins
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = MP3_MOSI | MP3_SCLK;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
  GPIO_Init (GPIO3, &GPIO_InitStructure);

  // Configure SSP1_nSS pin
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt1;
  GPIO_Init (GPIO3, &GPIO_InitStructure);
  GPIO_WriteBit(GPIO3,GPIO_Pin_7,Bit_SET);

  // Configure SSP1_MISO pin
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Pin = MP3_MISO;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Enable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_Init (GPIO3, &GPIO_InitStructure);

  // Spi init
  SSP_StructInit(&SSP_InitStructure);
  SSP_InitStructure.SSP_FrameFormat = SSP_FrameFormat_Motorola;
  SSP_InitStructure.SSP_Mode = SSP_Mode_Master;
  SSP_InitStructure.SSP_CPOL = SSP_CPOL_Low;
  SSP_InitStructure.SSP_CPHA = SSP_CPHA_1Edge;
  SSP_InitStructure.SSP_DataSize = SSP_DataSize_8b;
  SSP_InitStructure.SSP_ClockRate = 0;
  SSP_InitStructure.SSP_ClockPrescaler = 2;
  SSP_Init(SSP1, &SSP_InitStructure);

  // Disable DMA
  SSP_DMACmd(SSP1,SSP_DMA_Transmit | SSP_DMA_Receive,DISABLE);

  // Clock Freq <= 2MHz
  MP3_SetClockFreq(MP3_CLK_FREQ);

  // SSP1 enable
  SSP_Cmd(SSP1, ENABLE);

  // Deselect chip
  MP3_ChipSelect(0);

  // Timer 0
  // VIC configuration
  VIC_Config(TIM0_ITLine, VIC_IRQ, DmaTimerIntrPriority);
  VIC_ITCmd(TIM0_ITLine, ENABLE);

  // Deinitialization MP3 module (VS1002)
  MP3_Reset(1);
  DelayResolution100us(1);
  MP3_Reset(0);

  // Wait XRESET inactive to DREQ low (<= 400us)
  for(i = 8;i;--i)
  {
    if (!MP3_DReq())
    {
      break;
    }
    DelayResolution100us(1);
  }
  if (!i)
  {
    MP3_RET(MP3_NotComm);
  }

  // Wait XRESET inactive to software ready (<= 4ms)
  for(i = 80;i;--i)
  {
    if (MP3_DReq())
    {
      break;
    }
    DelayResolution100us(1);
  }
  if (!i)
  {
    MP3_RET(MP3_NotComm);
  }

  // Wait 100ms
  DelayResolution100us(1000);

  // Get the chip ID
  Mp3SendCmd(Mp3CmdGetRevision,&i);
  if (i != MP3_VS1002_REV)
  {
    MP3_RET(MP3_WrongRev);
  }
  // Init Clk
  i = 0x9800; // 12.288MHz * 2
  Mp3SendCmd(Mp3CmdSetClkReg,&i);
  // Init Mode
  // VS1002 native SPI modes, Share SPI chip select
  i = 0x0C00 |
     (MP3_STREAM_MODE_ENA?1UL<<6:0) |  // Stream mode mode
     (MP3_PLUS_V_ENA?1UL<<7:0); // +V mode
  Mp3SendCmd(Mp3CmdSetModeReg,&i);

  PlayFile = 0;

  MP3_RET(MP3_Pass);
}

/*************************************************************************
 * Function Name: Mp3InitDmaTransmit
 * Parameters: pInt32U pData , Int32U Size, Boolean StreamMode,
 *             Boolean SrcAddInc, pDmaLli_t DmaLli
 * Return: none
 *
 * Description: Init DMA transmit by SPI1
 *
 *************************************************************************/
static
void Mp3InitDmaTransmit(pInt32U pData , Int32U Size, Boolean StreamMode,
                        Boolean SrcAddInc, pDmaLli_t DmaLli)
{
#if MP3_STREAM_MODE_ENA > 0
DMA_InitTypeDef DMA_InitStructure;
  MP3_ChipSelect(0);
  if (StreamMode)
  {
    AddInc = SrcAddInc;
    pMp3Data = (pInt8U)pData;
    Mp3Size = Size;
    Mp3TransferStatus = Mp3DataTransferProgress;
  }
  else
  {
    // set the flag DMA Transfer in progress
    Mp3TransferStatus = Mp3DataTransferProgress;
    // Enable SPI1 DMA transfer
    SSP_DMACmd(SSP1,SSP_DMA_Transmit,ENABLE);
    // Initialize DMA
    DMA_InitStructure.DMA_Channel_SrcAdd = (Int32U)pData;
    DMA_InitStructure.DMA_Channel_DesAdd = (Int32U)&SSP1->DR;
    DMA_InitStructure.DMA_Channel_LLstItm = (Int32U)DmaLli;
    DMA_InitStructure.DMA_Channel_DesWidth = DMA_DesWidth_Byte;
    DMA_InitStructure.DMA_Channel_SrcWidth = DMA_SrcWidth_Byte;
    DMA_InitStructure.DMA_Channel_DesBstSize = DMA_DesBst_4Data;
    DMA_InitStructure.DMA_Channel_SrcBstSize = DMA_SrcBst_1Data;
    // Transferred_byte = DMA_Channel_TrsfSize * SrcWidth [bytes],
    // when DMA controlling data flow
    DMA_InitStructure.DMA_Channel_TrsfSize = Size;

    DMA_InitStructure.DMA_Channel_FlowCntrl = DMA_FlowCntrl1_DMA;
    DMA_InitStructure.DMA_Channel_Src = 0x0;

    DMA_InitStructure.DMA_Channel_Des = DMA_DES_SSP1_TX;

    // the DMA Terminal Count interrupt enable
    DMA_ITConfig(DMA_Channel6,ENABLE);
    // Disable destination adders increment
    DMA_ChannelDESIncConfig(DMA_Channel6,DISABLE);
    // Disable/Enable source adders increment
    DMA_ChannelSRCIncConfig(DMA_Channel6,(SrcAddInc?ENABLE:DISABLE));
    // Unmask all interrupts
    DMA_ITMaskConfig(DMA_Channel6,DMA_ITMask_ALL,ENABLE);
    // Init channel
    DMA_Init(DMA_Channel6,&DMA_InitStructure);

    // Enable channel
    DMA_ChannelCmd(DMA_Channel6,ENABLE);
  }
#else
Int32U Tmp;
Int32S iSize = Size;
  MP3_ChipSelect(0);

  while(iSize > 0)
  {
    if(StreamMode)
      while(!MP3_DReq());
    for(int i = 8; i; --i)
    {
      Tmp = *pData;
      MP3_TranserByte(Tmp    );
      MP3_TranserByte(Tmp>> 8);
      MP3_TranserByte(Tmp>>16);
      MP3_TranserByte(Tmp>>24);
      iSize -= 4;
      if(!iSize)
      {
        return;
      }
      if(SrcAddInc)
      {
        ++pData;
      }
    }
  }
#endif // MP3_STREAM_MODE_ENA > 0
}

#if MP3_STREAM_MODE_ENA > 0
/*************************************************************************
 * Function Name: MP3_DmaHandler
 * Parameters: Int32U IntrType - Error / Terminal Count
 * Return: none
 *
 * Description: Interrupt handler of the MP3 DMA.
 *
 *************************************************************************/
void MP3_DmaHandler(Int32U IntrType)
{
  if (IntrType)
  {
    // Terminal Count
    DMA_ClearIT(Channel6,DMA_TCC);
    Mp3TransferStatus = Mp3DataTransferComplete;
  }
  else
  {
    // Error
    DMA_ClearIT(Channel6,DMA_TCC);
    DMA_ClearIT(Channel6,DMA_EC);
    Mp3TransferStatus = Mp3DataTransferCompleteWithError;
    DMA_ChannelCmd(DMA_Channel6,DISABLE);
  }
}

/*************************************************************************
 * Function Name: Tim0Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void Tim0Handler (void)
{
  if (Mp3TransferStatus == Mp3DataTransferProgress)
  {
    for(Int32U i = 0; i < MP3_STREAM_SIZE; ++i)
    {
      SSP1->DR = *(pMp3Data+i);

    }
    if(AddInc)
    {
      pMp3Data += MP3_STREAM_SIZE;
    }
    if((Mp3Size -= MP3_STREAM_SIZE) <= 0)
    {
      Mp3TransferStatus = Mp3DataTransferComplete;
      if(!PlayFile)
      {
        // Disable TIM0 OC1 interrupt
        TIM0->CR2 &= ~TIM_IT_OC1;
        // Disable TIM0 counter
        TIM0->CR1 &= 0x7FFF;
      }
    }
  }
  // Update OC1R
  TIM0->OC1R += DmaPeriodHold;
  // Clear TIM0 flag OC1
  TIM0->SR &= ~TIM_FLAG_OC1;
}
#endif // MP3_STREAM_MODE_ENA > 0

/*************************************************************************
 * Function Name: Mp3SendCmd
 * Parameters: MP3_Cmd_t Cmd, pInt32U pData
 * Return: none
 *
 * Description: Send command to the MP3 module (VS1002)
 *
 *************************************************************************/
void Mp3SendCmd (MP3_Cmd_t Cmd, pInt32U pData)
{
Int32U Tmp,Size;
pMp3Stream_t pMp3Stream;

#if MP3_STREAM_MODE_ENA > 0
static Int32U Period, MinPeriod, MaxPeriod, PeriodDelta;
Int32U StreamID;
TIM_InitTypeDef TIM_InitStructure;
#endif // MP3_STREAM_MODE_ENA > 0
  switch (Cmd)
  {
  case Mp3CmdPowerUp:
    *pData = Mp3ModuleInit(*pData,*(pData+1));
    break;
  case Mp3CmdPowerDown:
    // Wait until data transfer is active
    while(Mp3TransferStatus == Mp3DataTransferProgress);
    // Disable SPI1 DMA
    SSP_DMACmd(SSP1,SSP_DMA_Transmit | SSP_DMA_Receive,DISABLE);
    MP3_RxFifoDrain();
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_ReadCmd);
    MP3_TranserByte(MP3_STATUS);
    Tmp = (MP3_TranserByte(0) << 8) & 0xFF00;
    Tmp|=  MP3_TranserByte(0)       & 0x00FF;
    MP3_ChipSelect(0);
    // Set SS APDOWN2 bit (3)
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_STATUS);
    MP3_TranserByte(Tmp >> 8);
    MP3_TranserByte(Tmp | (1UL<<3));
    MP3_ChipSelect(0);
    // Wait 10ms
    DelayResolution100us(100);
    // Reset VS1002
    MP3_Reset(1);
    break;
  case Mp3CmdGetRevision:
    // Wait until data transfer is active
    while(Mp3TransferStatus == Mp3DataTransferProgress);
    // Disable SPI1 DMA
    SSP_DMACmd(SSP1,SSP_DMA_Transmit | SSP_DMA_Receive,DISABLE);
    MP3_RxFifoDrain();
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_ReadCmd);
    MP3_TranserByte(MP3_STATUS);
    MP3_TranserByte(0);
    Tmp = MP3_TranserByte(0);
    *pData = (Tmp>>4) & 0x7;
    MP3_ChipSelect(0);
    break;
  case Mp3CmdSetClkReg:
    // Wait until data transfer is active
    while(Mp3TransferStatus == Mp3DataTransferProgress);
    // Disable SPI1 DMA
    SSP_DMACmd(SSP1,SSP_DMA_Transmit | SSP_DMA_Receive,DISABLE);
    MP3_WaitTxFifoEmptying();
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_CLOCKF);
    MP3_TranserByte(*pData>>8);
    MP3_TranserByte(*pData   );
    MP3_ChipSelect(0);
    break;
  case Mp3CmdSetModeReg:
    // Wait until data transfer is active
    while(Mp3TransferStatus == Mp3DataTransferProgress);
    // Disable SPI1 DMA
    SSP_DMACmd(SSP1,SSP_DMA_Transmit | SSP_DMA_Receive,DISABLE);
    MP3_WaitTxFifoEmptying();
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_MODE);
    MP3_TranserByte(*pData>>8);
    MP3_TranserByte(*pData   );
    MP3_ChipSelect(0);
    break;
  case Mp3CmdSetVol:
    // Wait until data transfer is active
    while(Mp3TransferStatus == Mp3DataTransferProgress);
    // Disable SPI1 DMA
    SSP_DMACmd(SSP1,SSP_DMA_Transmit | SSP_DMA_Receive,DISABLE);
    MP3_WaitTxFifoEmptying();
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_VOL);
    MP3_TranserByte(*pData>>8);
    MP3_TranserByte(*pData   );
    MP3_ChipSelect(0);
    break;
  case Mp3CmdTstSin:
    // Wait until data transfer is active
    while(Mp3TransferStatus == Mp3DataTransferProgress);
    // Disable SPI1 DMA
    SSP_DMACmd(SSP1,SSP_DMA_Transmit | SSP_DMA_Receive,DISABLE);
    MP3_RxFifoDrain();
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_ReadCmd);
    MP3_TranserByte(MP3_MODE);
    Tmp = (MP3_TranserByte(0) << 8) & 0xFF00;
    Tmp|=  MP3_TranserByte(0)       & 0x00FF;
    MP3_ChipSelect(0);
    // Set test bit
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_MODE);
    MP3_TranserByte(Tmp >> 8);
    MP3_TranserByte(Tmp | (1UL << 5));
    MP3_ChipSelect(0);
    if(*pData)
    {
      // Send test command sequence
      MP3_TranserByte(0x53);
      MP3_TranserByte(0xEF);
      MP3_TranserByte(0x6E);
      MP3_TranserByte(*pData);
      MP3_TranserByte(0x00);
      MP3_TranserByte(0x00);
      MP3_TranserByte(0x00);
      MP3_TranserByte(0x00);
    }
    else
    {
      // Send exit form the test command sequence
      MP3_TranserByte(0x45);
      MP3_TranserByte(0x78);
      MP3_TranserByte(0x69);
      MP3_TranserByte(0x74);
      MP3_TranserByte(0x00);
      MP3_TranserByte(0x00);
      MP3_TranserByte(0x00);
      MP3_TranserByte(0x00);
    }
    DelayResolution100us(1000);
    // Clear test bit
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_MODE);
    MP3_TranserByte(Tmp>>8);
    MP3_TranserByte(Tmp   );
    MP3_ChipSelect(0);
    break;
  case Mp3CmdPlayStatus:
    *pData = Mp3TransferStatus;
    break;
  case Mp3CmdPlay:
    // Wait until data transfer is active
    while(Mp3TransferStatus == Mp3DataTransferProgress);
    pMp3Stream = (pMp3Stream_t)pData;
    if (pMp3Stream->Size)
    {
      MP3_WaitTxFifoEmptying();
      // for synchronization
      MP3_ChipSelect(1);
      if(!PlayFile)
      {
        Size = VS1002_BUFFER_SIZE/2;
        Size = MIN(pMp3Stream->Size,Size);
        Mp3InitDmaTransmit(pMp3Stream->pStream,Size,FALSE,TRUE,NULL);
        // Wait until DMA transfer is active and player recognize MP3 file type
        while(Mp3TransferStatus == Mp3DataTransferProgress);
    #if MP3_STREAM_MODE_ENA > 0
        // Disable SPI1 DMA
        SSP_DMACmd(SSP1,SSP_DMA_Transmit | SSP_DMA_Receive,DISABLE);
        MP3_RxFifoDrain();

        // Init transfer timer depending of the file bitrate
        MP3_ChipSelect(1);
        MP3_TranserByte(MP3_ReadCmd);
        MP3_TranserByte(MP3_HDAT1);
        Tmp = (MP3_TranserByte(0) << 8) & 0xFF00;
        Tmp|=  MP3_TranserByte(0)       & 0x00FF;
        MP3_ChipSelect(0);

        if((Tmp & 0xFFE0) != 0xFFE0)
        {
          // invalid stream
          pMp3Stream->pStream = NULL;
          return;
        }

        StreamID = (Tmp>>3) & 0x3;

        MP3_ChipSelect(1);
        MP3_TranserByte(MP3_ReadCmd);
        MP3_TranserByte(MP3_HDAT0);
        Tmp = (MP3_TranserByte(0) << 8) & 0xFF00;
        Tmp|=  MP3_TranserByte(0)       & 0x00FF;
        MP3_ChipSelect(0);

        Tmp >>= 12;
        Tmp  &= 0xF;

        if(StreamID == ISO_11172_3_1_0)
        {
          Period = ISO11172_Bitrate[Tmp];
        }
        else
        {
          Period = MPG2_Bitrate[Tmp];
        }

        if(!Period)
        {
          // invalid stream bitrate
          pMp3Stream->pStream = NULL;
          return;
        }
        Tmp = Period/20;
        MinPeriod = Period - Tmp; // 95%
        MaxPeriod = Period + Tmp; // 105%
        PeriodDelta = Tmp/32;
        DmaPeriodHold = Period;

        // Timer 0
        // TIM Configuration in Output Compare Timing Mode
        TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
        TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
        TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB; // assign PCLK to TIM_Clk
        TIM_InitStructure.TIM_Prescaler = (48/8) - 1;
        TIM_InitStructure.TIM_Pulse_Length_1 = Period;
        TIM_Init(TIM0, &TIM_InitStructure);
        TIM_CounterCmd(TIM0, TIM_CLEAR);                  // clear the timer 0 counter

        // Clear TIM0 flag OC1
        TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
        // Enable TIM0 OC1 interrupt
        TIM_ITConfig(TIM0, TIM_IT_OC1, ENABLE);

        TIM_DMACmd(TIM0,ENABLE);

        // Enable TIM0 counter
        TIM_CounterCmd(TIM0, TIM_START);
    #endif // MP3_STREAM_MODE_ENA > 0

        PlayFile = 1;
        if((pMp3Stream->Size -= Size) == 0)
        {
          break;
        }
      }
    #if MP3_STREAM_MODE_ENA > 0
      // Correct stream sample rate
      switch(pMp3Stream->PlaySpeed)
      {
      case Mp3PlayIncreaseSpeed:
        if(DmaPeriodHold < MaxPeriod)
        {
          DmaPeriodHold += PeriodDelta;
        }
        break;
      case Mp3PlayDecreaseSpeed:
        if(DmaPeriodHold > MinPeriod)
        {
          DmaPeriodHold -= PeriodDelta;
        }
        break;
      default:
        if(DmaPeriodHold < Period)
        {
          DmaPeriodHold += PeriodDelta;
        }
        else if (DmaPeriodHold > Period)
        {
          DmaPeriodHold -= PeriodDelta;
        }
      }
    #endif  // STREAM_MODE_ENA > 0
      Mp3InitDmaTransmit(pMp3Stream->pStream,pMp3Stream->Size,TRUE,TRUE,NULL);
      break;
    }
  case Mp3CmdPlayStop:
    // Wait until data transfer is active
    while(Mp3TransferStatus == Mp3DataTransferProgress);
    // Disable TIM0 OC1 interrupt
    TIM_ITConfig(TIM0, TIM_IT_OC1, DISABLE);
    // Disable TIM0 counter
    TIM_CounterCmd(TIM0, TIM_STOP);
    MP3_WaitTxFifoEmptying();
    MP3_ChipSelect(1);
    // Init Transfer of 2048 zero (clear on-chip buffer)
    Mp2DmaTmp = 0;
    Mp3InitDmaTransmit(&Mp2DmaTmp,2048,FALSE,FALSE,NULL);
    PlayFile = 0;
    break;
  case Mp3CmdWideStereo:
    // Wait until data transfer is active
    while(Mp3TransferStatus == Mp3DataTransferProgress);
    // Disable SPI1 DMA
    SSP_DMACmd(SSP1,SSP_DMA_Transmit | SSP_DMA_Receive,DISABLE);
    MP3_RxFifoDrain();
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_ReadCmd);
    MP3_TranserByte(MP3_MODE);
    Tmp = (MP3_TranserByte(0) << 8) & 0xFF00;
    Tmp|=  MP3_TranserByte(0)       & 0x00FF;
    MP3_ChipSelect(0);
    if(*pData)
    {
      Tmp |= 1;
    }
    else
    {
      Tmp &= ~1;
    }
    // Set test bit
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_MODE);
    MP3_TranserByte(Tmp >> 8);
    MP3_TranserByte(Tmp | (1UL << 5));
    MP3_ChipSelect(0);
    break;
  case Mp3CmdLoudness:
    Tmp = *pData?0x8A:0;
    // Wait until data transfer is active
    while(Mp3TransferStatus == Mp3DataTransferProgress);
    // Disable SPI1 DMA
    SSP_DMACmd(SSP1,SSP_DMA_Transmit | SSP_DMA_Receive,DISABLE);
    MP3_WaitTxFifoEmptying();
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_BASS);
    MP3_TranserByte(Tmp>>8);
    MP3_TranserByte(Tmp   );
    MP3_ChipSelect(0);
    break;
  }
}

