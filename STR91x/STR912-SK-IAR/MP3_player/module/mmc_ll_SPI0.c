/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : mmc_ll_SPI0.h
 *    Description : Low level MMC SPI diver
 *
 *    History :
 *    1. Date        : September 22, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : November 28, 2006
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *        Support for a DMA transfer.
 *
 *    $Revision: 39 $
 **************************************************************************/
extern void DelayResolution100us(Int32U Delay);

#define SD_CP      GPIO_Pin_1
#define SD_WP      GPIO_Pin_0
#define SD_CS      GPIO_Pin_3

//#define SD_CS      GPIO_Pin_7

#define SD_SCLK    GPIO_Pin_4
#define SD_MOSI    GPIO_Pin_5
#define SD_MISO    GPIO_Pin_6

#if SPI_DMA_ENA > 0
typedef enum _SPI_TransferDir_t {
  SPI_RECEIVE = 0, SPI_TRANSMIT
} SPI_TransferDir_t;

volatile Boolean TransferStatus;     // data transfer state

/*************************************************************************
 * Function Name: SPI0_DmaHandler
 * Parameters: none
 * Return: none
 *
 * Description: Interrupt handler of the SPI0 DMA.
 *
 *************************************************************************/
void SPI0_DmaHandler(void)
{
  DMA_ClearIT(Channel4,DMA_TCC);
  DMA_ClearIT(Channel4,DMA_EC);
  DMA_ClearIT(Channel5,DMA_TCC);
  DMA_ClearIT(Channel5,DMA_EC);
  TransferStatus = FALSE;
  DMA_ChannelCmd(DMA_Channel4,DISABLE);
  SSP_DMACmd(SSP0,SSP_DMA_Transmit,DISABLE);
}

/*************************************************************************
 * Function Name: SPI0_DmaTransfer
 * Parameters: pInt8U pData,Int32U Size, SPI_TransferDir_t SPI_TransferDir
 * Return: none
 *
 * Description: DMA transfer
 *
 *************************************************************************/
void SPI0_DmaTransfer(pInt8U pData,Int32U Size, SPI_TransferDir_t SPI_TransferDir)
{
DMA_InitTypeDef DMA_InitStructure;
Int32U Dummy = 0xFF;
  // set the flag DMA Transfer in progress
  TransferStatus = TRUE;
  // Enable SPI1 DMA transfer
  SSP_DMACmd(SSP0,SSP_DMA_Transmit,ENABLE);
  SSP_DMACmd(SSP0,SSP_DMA_Receive,ENABLE);

  // Initialize DMA Tx channel
  DMA_InitStructure.DMA_Channel_SrcAdd = (SPI_TransferDir == SPI_TRANSMIT)?(Int32U)pData:(Int32U)&Dummy;
  DMA_InitStructure.DMA_Channel_DesAdd = (Int32U)&SSP0->DR;
  DMA_InitStructure.DMA_Channel_LLstItm = 0;
  DMA_InitStructure.DMA_Channel_DesWidth = DMA_DesWidth_Byte;
  DMA_InitStructure.DMA_Channel_SrcWidth = DMA_SrcWidth_Byte;
  DMA_InitStructure.DMA_Channel_DesBstSize = DMA_DesBst_1Data;
  DMA_InitStructure.DMA_Channel_SrcBstSize = DMA_SrcBst_1Data;

  // Transferred_byte = DMA_Channel_TrsfSize * SrcWidth [bytes],
  // when DMA controlling data flow
  DMA_InitStructure.DMA_Channel_TrsfSize = Size;

  DMA_InitStructure.DMA_Channel_FlowCntrl = DMA_FlowCntrl1_DMA;
  DMA_InitStructure.DMA_Channel_Src = 0x0;
  DMA_InitStructure.DMA_Channel_Des = DMA_DES_SSP0_TX;

  // the DMA Terminal Count interrupt enable
  DMA_ITConfig(DMA_Channel4,ENABLE);
  // Disable destination adders increment
  DMA_ChannelDESIncConfig(DMA_Channel4,DISABLE);
  // Disable/Enable source adders increment
  DMA_ChannelSRCIncConfig(DMA_Channel4,SPI_TransferDir == SPI_TRANSMIT?ENABLE:DISABLE);
  // Unmask all interrupts
  DMA_ITMaskConfig(DMA_Channel4,DMA_ITMask_ALL,ENABLE);
  // Init channel
  DMA_Init(DMA_Channel4,&DMA_InitStructure);

  // Initialize DMA Rx channel
  DMA_InitStructure.DMA_Channel_SrcAdd = (Int32U)&SSP0->DR;
  DMA_InitStructure.DMA_Channel_DesAdd = (SPI_TransferDir == SPI_RECEIVE)?(Int32U)pData:(Int32U)&Dummy;
  DMA_InitStructure.DMA_Channel_LLstItm = 0;
  DMA_InitStructure.DMA_Channel_DesWidth = DMA_DesWidth_Byte;
  DMA_InitStructure.DMA_Channel_SrcWidth = DMA_SrcWidth_Byte;
  DMA_InitStructure.DMA_Channel_DesBstSize = DMA_DesBst_1Data;
  DMA_InitStructure.DMA_Channel_SrcBstSize = DMA_SrcBst_1Data;
  DMA_InitStructure.DMA_Channel_FlowCntrl = DMA_FlowCntrl_Perip2;

  DMA_InitStructure.DMA_Channel_Src = DMA_SRC_SSP0_RX;
  DMA_InitStructure.DMA_Channel_Des = 0;

  // the DMA Terminal Count interrupt enable
  DMA_ITConfig(DMA_Channel5,ENABLE);
  // Disable/Enable destination adders increment
  DMA_ChannelDESIncConfig(DMA_Channel5,SPI_TransferDir == SPI_RECEIVE?ENABLE:DISABLE);
  // Disable source adders increment
  DMA_ChannelSRCIncConfig(DMA_Channel5,DISABLE);
  // Mask all interrupts
  DMA_ITMaskConfig(DMA_Channel5,DMA_ITMask_ALL,DISABLE);
  // Init channel
  DMA_Init(DMA_Channel5,&DMA_InitStructure);
  // Enable channel
  DMA_ChannelCmd(DMA_Channel5,ENABLE);
  DMA_ChannelCmd(DMA_Channel4,ENABLE);
  while(TransferStatus);
  // wait until SPI transmit fifo isn't empty
  while(SSP_GetFlagStatus(SSP0, SSP_FLAG_TxFifoEmpty)==RESET);
  // wait until SPI receive fifo isn't empty
  while(SSP_GetFlagStatus(SSP0, SSP_FLAG_RxFifoNotEmpty)==SET);
  // disable DMA channels
  DMA_ChannelCmd(DMA_Channel5,DISABLE);
  SSP_DMACmd(SSP0,SSP_DMA_Receive,DISABLE);
}
#endif // SPI_DMA_ENA > 0

/*************************************************************************
 * Function Name: MmcChipSelect
 * Parameters: Boolean Select
 * Return: none
 *
 * Description: Mmc Chip select control
 * Select = true  - Chip is enable
 * Select = false - Chip is disable
 *
 *************************************************************************/
void MmcChipSelect (Boolean Select)
{
  GPIO_WriteBit(GPIO9,SD_CS,Select?Bit_RESET:Bit_SET);
}
/*************************************************************************
 * Function Name: MmcPresent
 * Parameters: none
 * Return: Boolean - true cart present
 *                 - false cart no present
 *
 * Description: Mmc present check
 *
 *************************************************************************/
inline
Boolean MmcPresent (void)
{
  return(GPIO_ReadBit(GPIO9,SD_CP) == Bit_RESET);
}

/*************************************************************************
 * Function Name: MmcWriteProtect
 * Parameters: none
 * Return: Boolean - true cart is protected
 *                 - false cart no protected
 *
 * Description: Mmc Write protect check
 *
 *************************************************************************/
inline
Boolean MmcWriteProtect (void)
{
  return(GPIO_ReadBit(GPIO9,SD_WP) == Bit_SET);
}

/*************************************************************************
 * Function Name: MmcSetClockFreq
 * Parameters: Int32U Frequency
 * Return: Int32U
 *
 * Description: Set SPI ckl frequency
 *
 *************************************************************************/
Int32U MmcSetClockFreq (Int32U Frequency)
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

  SSP0->PR = Div;

  // Return real frequency
  return(Pclk/Div);
}

/*************************************************************************
 * Function Name: MmcInit
 * Parameters: none
 * Return: none
 *
 * Description: Init SPI, Cart Present, Write Protect and Chip select pins
 *
 *************************************************************************/
void MmcInit (void)
{
SSP_InitTypeDef   SSP_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;

  // Enable GPIO clocks
  SCU_APBPeriphClockConfig(__GPIO5 | __GPIO9 | __SSP0, ENABLE);
  // Deinitializes the SPI0
  SCU_APBPeriphReset(__SSP0, ENABLE);
  // Release GPIO and SPI0 reset
  SCU_APBPeriphReset(__GPIO5 | __GPIO9 | __SSP0, DISABLE);

  // Configure SSP0_CLK, SSP0_MOSI, SSP0_nCS1, Card Present and Write Protect pins
  SCU->GPIOEMI &= ~1;
  SCU->GPIOTYPE[9] &=  ~(GPIO_Pin_2 | GPIO_Pin_3);
  GPIO9->DDR |= GPIO_Pin_2 | GPIO_Pin_3;
  GPIO9->DDR &= ~(SD_CP | SD_WP);

  // Set the NSS high
  GPIO_WriteBit(GPIO9,GPIO_Pin_2,Bit_SET);

  // Configure SSP0_CLK, SSP0_MOSI, SSP0_nSS pins
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = SD_SCLK | SD_MOSI | SD_CS;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
  GPIO_Init (GPIO5, &GPIO_InitStructure);

  // Configure SSP0_MISO pin GPIO5.6
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Pin = SD_MISO;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Enable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_Init (GPIO5, &GPIO_InitStructure);

  // Chip select
  MmcChipSelect(0);

  // Spi init
  SSP_StructInit(&SSP_InitStructure);

  SSP_InitStructure.SSP_FrameFormat = SSP_FrameFormat_Motorola;
  SSP_InitStructure.SSP_Mode = SSP_Mode_Master;
  SSP_InitStructure.SSP_CPOL = SSP_CPOL_Low;
  SSP_InitStructure.SSP_CPHA = SSP_CPHA_1Edge;
  SSP_InitStructure.SSP_DataSize = SSP_DataSize_8b;
  SSP_InitStructure.SSP_ClockRate = 0;
  SSP_InitStructure.SSP_ClockPrescaler = 2;
  SSP_Init(SSP0, &SSP_InitStructure);

  // SSP0 enable
  SSP_Cmd(SSP0, ENABLE);

  // Clock Freq. Identification Mode < 400kHz
  MmcSetClockFreq(IdentificationModeClock);
#if SPI_DMA_ENA > 0
  // Enable DMA clock
  SCU_AHBPeriphClockConfig(__DMA , ENABLE);
  // Reinitializes the DMA
  SCU_AHBPeriphReset(__DMA, ENABLE);
  // Release DMA reset
  SCU_AHBPeriphReset(__DMA, DISABLE);
  // Enable DMA module
  DMA_Cmd(ENABLE);
  DMA_SyncConfig(DMA_SSP0_RX_Mask | DMA_SSP0_TX_Mask, DISABLE);
  // Clear pending interrupt
  DMA_ClearIT(Channel4,DMA_TCC);
  DMA_ClearIT(Channel4,DMA_EC);
  DMA_ClearIT(Channel5,DMA_TCC);
  DMA_ClearIT(Channel5,DMA_EC);

  // VIC configuration
  VIC_Config(DMA_ITLine, VIC_IRQ, SPI_DMA_INTR_PRIO);
  VIC_ITCmd(DMA_ITLine, ENABLE);
#endif // SPI_DMA_ENA > 0
}

/*************************************************************************
 * Function Name: MmcTranserByte
 * Parameters: Int8U ch
 * Return: Int8U
 *
 * Description: Transfer byte by SPI
 *
 *************************************************************************/
Int8U MmcTranserByte (Int8U ch)
{
  SSP_SendData(SSP0, ch);
  while(SSP_GetFlagStatus(SSP0, SSP_FLAG_RxFifoNotEmpty) == RESET);
  return(SSP_ReceiveData(SSP0));
}

/*************************************************************************
 * Function Name: MmcSendBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: none
 *
 * Description: Send block by SPI
 *
 *************************************************************************/
void MmcSendBlock (pInt8U pData, Int32U Size)
{
#if SPI_DMA_ENA > 0
  SPI0_DmaTransfer(pData,Size,SPI_TRANSMIT);
#else
Int32U OutCount = Size;
  while (OutCount--)
  {
    SSP_SendData(SSP0, *pData++);
    while(SSP_GetFlagStatus(SSP0, SSP_FLAG_RxFifoNotEmpty)==RESET);
    volatile Int32U Dummy = SSP_ReceiveData(SSP0);
  }
#endif // SPI_DMA_ENA > 0
}

/*************************************************************************
 * Function Name: MmcReceiveBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: none
 *
 * Description: Read block by SPI
 *
 *************************************************************************/
void MmcReceiveBlock (pInt8U pData, Int32U Size)
{
#if SPI_DMA_ENA > 0
  SPI0_DmaTransfer(pData,Size,SPI_RECEIVE);
#else
Int32U InCount = Size;
  while (InCount--)
  {
    SSP_SendData(SSP0, 0xFF);
    while(SSP_GetFlagStatus(SSP0, SSP_FLAG_RxFifoNotEmpty)==RESET);
    *pData++ = SSP_ReceiveData(SSP0);
  }
#endif // SPI_DMA_ENA > 0
}

/*************************************************************************
 * Function Name: MmcDly_1ms
 * Parameters: Int32U Delay
 * Return: none
 *
 * Description: Delay [msec]
 *
 *************************************************************************/
void MmcDly_1ms (Int32U Delay)
{
  for(;Delay;--Delay)
  {
    DelayResolution100us(10);
  }
}

