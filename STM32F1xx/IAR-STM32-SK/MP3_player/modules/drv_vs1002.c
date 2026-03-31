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

#define MP3_BOOT_SEL  GPIO_Pin_7  // PB7
#define MP3_RST       GPIO_Pin_10 // PA10
#define MP3_DATA_RQ   GPIO_Pin_9  // PA9

#define MP3_CS        GPIO_Pin_6  // PB6

#define MP3_MOSI      GPIO_Pin_7  // PA7
#define MP3_MISO      GPIO_Pin_6  // PA6
#define MP3_SCLK      GPIO_Pin_5  // Pa5

Boolean PlayFile = 0;     // Play in progress

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
  GPIO_WriteBit(GPIOB,MP3_BOOT_SEL,Bit_RESET);
  GPIO_WriteBit(GPIOA,MP3_RST,Select?Bit_RESET:Bit_SET);
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
  return(GPIO_ReadInputDataBit(GPIOA,MP3_DATA_RQ) == Bit_SET);
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
  GPIO_WriteBit(GPIOB,MP3_CS,Select?Bit_RESET:Bit_SET);
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
Int32U DivVal = 0;
RCC_ClocksTypeDef Clk;

  RCC_GetClocksFreq(&Clk);

  while((Frequency * Div) <=  Clk.PCLK2_Frequency)
  {
    Div <<= 1;
    if (++DivVal == 7)
    {
      break;
    }
  }

  SPI1->CR1 = (SPI1->CR1 & ~(0x7 << 3)) | ((DivVal&0x7) << 3);

  // Return real frequency
  return(Clk.PCLK2_Frequency/Div);
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
  SPI_I2S_SendData(SPI1, ch);
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  return(SPI_I2S_ReceiveData(SPI1));
}

/*************************************************************************
 * Function Name: Mp3ModuleInit
 * Parameters: none
 * Return: MP3_Status_t
 *          MP3_Pass, MP3_Fault or MP3_WrongRev, MP3_NotComm
 *
 * Description: Initialize MP3 module (VS1002)
 *
 *************************************************************************/
static inline
MP3_Status_t Mp3ModuleInit (void)
{
SPI_InitTypeDef   SPI_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;
Int32U i;

  // Initialize SPI1, Timer1 and IO
  // Enable SPI1, Timer1 and GPIO clocks
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA
                        | RCC_APB2Periph_GPIOB
                        | RCC_APB2Periph_AFIO
                        | RCC_APB2Periph_TIM1
                        | RCC_APB2Periph_SPI1
                        , ENABLE);
  // Deinitializes the SPI1
  SPI_I2S_DeInit(SPI1);
  // Deinitializes the Timer1
  TIM_DeInit(TIM1);
  // Release reset of GPIOB, GPIOC
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOA
                         | RCC_APB2Periph_GPIOB
                         | RCC_APB2Periph_AFIO
                         , DISABLE);

  // Configure Boot and chip select pins
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = MP3_BOOT_SEL | MP3_CS;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Configure MP3 Reset pin
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = MP3_RST;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure data request pin
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = MP3_DATA_RQ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure SPI1_CLK, SPI1_MOSI and SPI_MISO pins
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = MP3_SCLK | MP3_MOSI | MP3_MISO;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Disable SPI1 remap
  GPIO_PinRemapConfig(GPIO_Remap_SPI1,DISABLE);

  // SPI init
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  // Disable DMA
  SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,DISABLE);
  SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);

  // Clock Freq <= 2MHz
  MP3_SetClockFreq(MP3_CLK_FREQ);

  // Enable SPI2
  SPI_Cmd(SPI1, ENABLE);

  // Deselect chip
  MP3_ChipSelect(0);

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
     (MP3_PLUS_V_ENA?1UL<<7:0); // +V mode
  Mp3SendCmd(Mp3CmdSetModeReg,&i);

  PlayFile = 0;

  MP3_RET(MP3_Pass);
}

/*************************************************************************
 * Function Name: Mp3Transmit
 * Parameters: pInt32U pData , Int32U Size,
 *             Boolean StreamMode, Boolean SrcAddInc
 * Return: none
 *
 * Description: Transmit data
 *
 *************************************************************************/
static
void Mp3Transmit(pInt32U pData , Int32U Size, Boolean StreamMode, Boolean SrcAddInc)
{
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
}

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

  switch (Cmd)
  {
  case Mp3CmdPowerUp:
    *pData = Mp3ModuleInit();
    break;
  case Mp3CmdPowerDown:
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
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_ReadCmd);
    MP3_TranserByte(MP3_STATUS);
    MP3_TranserByte(0);
    Tmp = MP3_TranserByte(0);
    *pData = (Tmp>>4) & 0x7;
    MP3_ChipSelect(0);
    break;
  case Mp3CmdSetClkReg:
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_CLOCKF);
    MP3_TranserByte(*pData>>8);
    MP3_TranserByte(*pData   );
    MP3_ChipSelect(0);
    break;
  case Mp3CmdSetModeReg:
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_MODE);
    MP3_TranserByte(*pData>>8);
    MP3_TranserByte(*pData   );
    MP3_ChipSelect(0);
    break;
  case Mp3CmdSetVol:
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_VOL);
    MP3_TranserByte(*pData>>8);
    MP3_TranserByte(*pData   );
    MP3_ChipSelect(0);
    break;
  case Mp3CmdTstSin:
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
  case Mp3CmdPlay:
    // Wait until data transfer is active
    pMp3Stream = (pMp3Stream_t)pData;
    if (pMp3Stream->Size)
    {
      // for synchronization
      MP3_ChipSelect(1);
      if(!PlayFile)
      {
        Size = VS1002_BUFFER_SIZE/2;
        Size = MIN(pMp3Stream->Size,Size);
        PlayFile = 1;
        if((pMp3Stream->Size -= Size) == 0)
        {
          break;
        }
      }
      Mp3Transmit(pMp3Stream->pStream,pMp3Stream->Size,TRUE,TRUE);
      break;
    }
  case Mp3CmdPlayStop:
    MP3_ChipSelect(1);
    // Init Transfer of 2048 zero (clear on-chip buffer)
    Tmp = 0;
    Mp3Transmit(&Tmp,2048,FALSE,FALSE);
    PlayFile = 0;
    break;
  case Mp3CmdWideStereo:
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
    MP3_ChipSelect(1);
    MP3_TranserByte(MP3_WriteCmd);
    MP3_TranserByte(MP3_BASS);
    MP3_TranserByte(Tmp>>8);
    MP3_TranserByte(Tmp   );
    MP3_ChipSelect(0);
    break;
  }
}

