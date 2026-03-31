
/* Includes ------------------------------------------------------------------*/
#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

__IO uint32_t  EthInitStatus = 0;
__IO uint8_t EthLinkStatus = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*************************************************************************
 * Function Name: Ethernet_Configure
 * Parameters: void
 * Return: uint32_t (configuration result)
 *
 * Description: Configures the Ethernet Module
 *
 *************************************************************************/
uint32_t Ethernet_Configure(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
ETH_InitTypeDef ETH_InitStructure;

  // ETHERNET pins configuration
  // AF Output Push Pull:
  //- ETH_RMII_MDIO:    PA2
  //- ETH_RMII_MDC:     PC1
  //- ETH_RMII_MDINT:   PA3
  //- ETH_RMII_TX_EN:   PB11
  //- ETH_RMII_TXD0:    PG13
  //- ETH_RMII_TXD1:    PG14
  //- ETH_RMII_PPS_OUT: PB5
  //- ETH_RMII_REF_CLK: PA1
  //- ETH_RMII_CRS_DV:  PA7
  //- ETH_RMII_RXD0:    PC4
  //- ETH_RMII_RXD1:    PC5

  // Configure PA1, PA2, PA3, PA7 as alternate function
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

  // Configure PB11 as alternate function push-pull
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_ETH);

  // Configure PG13, PG14 as alternate function push-pull
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);

  // Configure PC1, PC4, PC5 as alternate function
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);

  // Enable SYSCFG clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // Set RMII mode
  SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);

  // Reset ETHERNET on AHB Bus
  ETH_DeInit();

  // Software reset
  ETH_SoftwareReset();

  // Wait for software reset
  while(ETH_GetSoftwareResetStatus()==SET);

  // ETHERNET Configuration
  // Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter
  ETH_StructInit(&ETH_InitStructure);

  // Fill ETH_InitStructure parametrs
  //------------------------   MAC   -----------------------------------
  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
  ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
  ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
  ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
  ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
  ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
  ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Enable;
  ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
  ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
  ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
#ifdef CHECKSUM_BY_HARDWARE
  ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif
  //------------------------   DMA   -----------------------------------
  // When we use the Checksum offload feature, we need to enable the Store and Forward mode:
  // the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum,
  // if the checksum is OK the DMA can handle the frame otherwise the frame is dropped

  ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
  ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
  ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;
  ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
  ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
  ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
  ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
  ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
  ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
  ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
  ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

  // Find PHY Address
  unsigned int PhyAddr;
  for(PhyAddr = 0; 32 > PhyAddr; PhyAddr++)
  {
    union {
      uint32_t    HI_LO;
      struct
      {
        uint16_t  LO;
        uint16_t  HI;
      };
    } PHYID;

    PHYID.HI = ETH_ReadPHYRegister(PhyAddr,2);
    PHYID.LO = ETH_ReadPHYRegister(PhyAddr,3) & 0xFFF0;

    if((0x00061c50 == PHYID.HI_LO) || (0x02038460 == PHYID.HI_LO) || (0x0007C0F0 == PHYID.HI_LO)) break; // 0x0007C0F0 - PHYID of LAN8710A
  }

  // if PHY address not found
  if(32 < PhyAddr)
  {
    return ETHERNET_PHY_ERROR;
  }

  // Initialize Ethernet
  if(0 == ETH_Init(&ETH_InitStructure, PhyAddr))
  {
    return ETHERNET_INIT_ERROR;
  }
  return ETHERNET_SUCCESS;
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
