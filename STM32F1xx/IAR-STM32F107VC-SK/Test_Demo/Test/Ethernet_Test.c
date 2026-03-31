/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Ethernet_Test.c
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : USB_Test program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32_eth.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define PHY_ADDRESS      0x15 /* Relative to IAR SK Board*/
#define DP83848_PHY        /* Ethernet pins mapping */
//#define MII_MODE          /* MII mode for Barracuda STM3210C-EVAL Board (MB784) (check jumpers setting) */
#define RMII_MODE       /* RMII mode for Barracuda STM3210C-EVAL Board (MB784) (check jumpers setting) */

#define ETH_RXBUFNB        2//8
#define ETH_TXBUFNB        2

/* ETHERNET errors */
#define  ETH_ERROR              ((u32)0)
#define  ETH_SUCCESS            ((u32)1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ETH_InitTypeDef ETH_InitStructure;

/* Ethernet Rx & Tx DMA Descriptors */
ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB];
/* Ethernet buffers */

u8 Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];
u8 Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE];
/* Private function prototypes -----------------------------------------------*/

void Ethernet_Config(void);
void GPIO_Conf(void);
u32 Modes_Test(u32 Speed, u32 Duplex);
u32 Auto_Negotiation_Test(void);
u32 Loop_Back_Test(void);
extern void Joystick_Init (void);
extern void uIPMain(void);
u32 RTH_CONF(void);

/* Private variables ---------------------------------------------------------*/
u32 Value = 0;
u32 address = 0;
u32 STE100P_BCR,STE100P_BSR;
u32 PR20,PR19;

u8 data[100];
u8 Send_buf[100]="Test 11";
u8 Send_buf2[100]="Test 22";
u32 size;

/* MAC address */
u8 AddressPtr[6] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};

/*******************************************************************************
* Function Name  : Ethernet_Test
* Description    : Ethernet_Test program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Ethernet_Test(void)
{
  u32 Ret;
  vu32 PR20;
  /* Disable the JoyStick interrupts */
  IntExtOnOffConfig(DISABLE);

  /* Display Test name */
  GLCD_print ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(4,1);
  GLCD_print( "Ethernet Test\n ");

  GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
  GLCD_TextSetPos(1,3);
  GLCD_print ("Initialisation...    ");

  GPIO_Conf();
  Ethernet_Config();

  GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
  GLCD_TextSetPos(1,3);
  GLCD_print ("Auto-Neg Test...    ");

  /* Test of the transceiver's Auto Negotiation function */
  Ret=Auto_Negotiation_Test();
  if (Ret == ETH_ERROR)
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,3);
    GLCD_print ("..Error!       ");
  }
  else
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,3);
    GLCD_print ("..OK           ");
  }

  GLCD_TextSetPos(1,4);
  GLCD_print ("Loopback Test...    ");

  /* Transceiver's Loop-Back Mode  */
  Ret=Loop_Back_Test();

  if (Ret == ETH_ERROR)
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,4);
     GLCD_print ("..Error!       ");
  }
  else
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,4);
    GLCD_print ("..OK           ");
  }

  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
  /* Test ALL Ethernet modes */

  GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
  GLCD_TextSetPos(1,5);
  GLCD_print ("10M/Half Dup...  ");
  Ret = Modes_Test(ETH_Speed_10M,ETH_Mode_HalfDuplex);
  if (Ret == ETH_ERROR)
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,5);
    GLCD_print ("..Error!       ");
  }
  else
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,5);
    GLCD_print ("..OK           ");
  }

  GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
  GLCD_TextSetPos(1,6);
  GLCD_print ("10M/Full Dup...  ");
  Ret = Modes_Test(ETH_Speed_10M,ETH_Mode_FullDuplex);
  if (Ret == ETH_ERROR)
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,6);
    GLCD_print ("..Error!       ");
  }
  else
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,6);
    GLCD_print ("..OK           ");
  }

  GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
  GLCD_TextSetPos(1,7);
  GLCD_print ("100M/Half Dup...  ");
  Ret = Modes_Test(ETH_Speed_100M,ETH_Mode_HalfDuplex);
  if (Ret == ETH_ERROR)
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,7);
    GLCD_print ("..Error!       ");
  }
  else
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,7);
    GLCD_print ("..OK           ");
  }

  GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
  GLCD_TextSetPos(1,8);
  GLCD_print ("100M/Full Dup...  ");
  Ret = Modes_Test(ETH_Speed_100M,ETH_Mode_FullDuplex);
  if (Ret == ETH_ERROR)
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,8);
    GLCD_print ("..Error!       ");
  }
  else
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(14,8);
    GLCD_print ("..OK           ");
  }

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

  /* Enable MAC and DMA transmission and reception */
  ETH_Start();

  STE100P_BSR = ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BSR);
  PR20= ETH_ReadPHYRegister(PHY_ADDRESS, 20);
  PR19= ETH_ReadPHYRegister(PHY_ADDRESS, 19);

 /* Test  Connection between two board */

 //while(size==0)
//{
 /* Delay */
  for(int i = PHY_ConfigDelay; i != 0; i--);

  ETH_HandleTxPkt(Send_buf2 ,100);
  size = ETH_HandleRxPkt((u8 *)data);

  if (size!=0)
  {
    if ((data[5]=='1') && (ReadKey() != User))
      GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_SET);
    else if (data[5]=='2')
      GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET);
    else
      size=0;
  }
//}
  GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
  GLCD_TextSetPos(0,10);
  GLCD_print( "Press User key to exit\n ");

  /* Run uIP Demo */
  uIPMain();

  ReturnFunc();

  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
}


/*******************************************************************************
* Function Name  : Modes_Test
* Description    : Test the specified modes for the ethernet
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u32 Auto_Negotiation_Test(void)
{
  vu32 RetVal;

  /* advertise only 10Mbits modes   */
  //RetVal = ETH_ReadPHYRegister(PHY_ADDRESS, 4);
  //RetVal &= ~0x180;
  //ETH_WritePHYRegister(PHY_ADDRESS, 4,RetVal);
  //RetVal = ETH_ReadPHYRegister(PHY_ADDRESS, 4);

  ETH_InitStructure.ETH_AutoNegotiation =ETH_AutoNegotiation_Enable;
  ETH_InitStructure.ETH_LoopbackMode =ETH_LoopbackMode_Disable;

  /* Configure ETHERNET */
  RetVal = ETH_Init(&ETH_InitStructure, PHY_ADDRESS);
  STE100P_BCR = ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BCR);

  ETH_Start();

  if ((ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BSR) & 4)==0)
    return ETH_ERROR;
  else
    return ETH_SUCCESS;

}

/*******************************************************************************
* Function Name  : Modes_Test
* Description    : Test the specified modes for the ethernet
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u32 Modes_Test(u32 Speed, u32 Duplex)
{
  vu32 RetVal;

  ETH_InitStructure.ETH_AutoNegotiation =ETH_AutoNegotiation_Disable;
  ETH_InitStructure.ETH_LoopbackMode =ETH_LoopbackMode_Disable;
  ETH_InitStructure.ETH_Speed = Speed;
  ETH_InitStructure.ETH_Mode = Duplex;

  /* Configure ETHERNET */
  RetVal = ETH_Init(&ETH_InitStructure, PHY_ADDRESS);
  STE100P_BCR = ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BCR);

  return RetVal;
}
/*******************************************************************************
* Function Name  : Loop_Back_Test
* Description    : Test the Loop-Back Mode for ethernet
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u32 Loop_Back_Test(void)
{
u32 timeout=0;
u32 size=0;

  ETH_InitStructure.ETH_AutoNegotiation =ETH_AutoNegotiation_Disable;

  /* Configure ETHERNET */
  ETH_Init(&ETH_InitStructure, PHY_ADDRESS);

  /* Enable Loop-Back Mode */
  if (!ETH_PHYLoopBackCmd(PHY_ADDRESS,ENABLE))
    return ETH_ERROR;

  STE100P_BCR = ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BCR);

  /* Enable MAC and DMA transmission and reception */
  ETH_Start();

  /* Delay */
  for(int i = PHY_ConfigDelay; i != 0; i--);

  /* Send a packet */
  ETH_HandleTxPkt(Send_buf ,100);

  /* Receives a packet */
  do
  {  ETH_HandleTxPkt(Send_buf ,100);
     timeout++;
     size = ETH_HandleRxPkt((u8 *)data);
  } while ((size==0) && (timeout < 0x00FFFF));

  /* Disable Loop-Back Mode */
  ETH_PHYLoopBackCmd(PHY_ADDRESS,DISABLE);
	
  if ((size>0) )//&& (data[5]=='1'))
  {
    GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_SET);
	return ETH_SUCCESS;
  }
  else
    return ETH_ERROR;

}

void Ethernet_Config(void)
{
    /* Enable ETHERNET clock  */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx |
                        RCC_AHBPeriph_ETH_MAC_Rx, DISABLE);

  /* Reset ETHERNET on AHB Bus */
  ETH_DeInit();
  /* Select RMII Interface */
  GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_RMII);

  /* ETHERNET pins remapp in STM3210C-EVAL board: RX_DV and RxD[3:0] */
  GPIO_PinRemapConfig(GPIO_Remap_ETH, DISABLE);


  /* Enable ETHERNET clock  */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx |
                        RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);

  /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |	RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);


 /* Reset ETHERNET on AHB Bus */
  ETH_DeInit();

  /* Software reset */
  ETH_SoftwareReset();
  /* Wait for software reset */
  while(ETH_GetSoftwareResetStatus()==SET);

  /* ETHERNET Configuration ------------------------------------------------------*/
  /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
  ETH_StructInit(&ETH_InitStructure);

  /* Fill ETH_InitStructure parametrs */
  /*------------------------   MAC   -----------------------------------*/
  ETH_InitStructure.ETH_AutoNegotiation =ETH_AutoNegotiation_Disable;
  ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
  ETH_InitStructure.ETH_LoopbackMode =ETH_LoopbackMode_Disable;
  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
  ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
  ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
  ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Enable;
  ETH_InitStructure.ETH_BroadcastFramesReception =ETH_BroadcastFramesReception_Disable;
  ETH_InitStructure.ETH_PromiscuousMode =ETH_PromiscuousMode_Disable;
  ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
  ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
  /* Configure ETHERNET */

  Value = ETH_Init(&ETH_InitStructure, PHY_ADDRESS);
  STE100P_BCR = ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BCR);

  /* Initialize Tx Descriptors list: Chain Mode */
  ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
  /* Initialize Rx Descriptor s list: Chain Mode  */
  ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

}

void GPIO_Conf(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

/* ETHERNET pins configuration */
/* AF Output Push Pull:
- ETH_RMII_MDIO:  PA2
- ETH_RMII_MDC:   PC1
- ETH_RMII_TX_EN: PB11
- ETH_RMII_TXD0:  PB12
- ETH_RMII_TXD1:  PB13
- ETH_RMII_PPS_OUT: PB5 ??
*/

  /* Configure PA2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PC1 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PB11, PB12 and PB13 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

/**************************************************************/
/*               For Remapped Ethernet pins                   */
/*************************************************************/

/* Input (Reset Value):

- ETH_RMII_REF_CLK: PA1
- ETH_RMII_CRS_DV : PA7
- ETH_RMII_RXD0   : PC4
- ETH_RMII_RXD1   : PC5
- ETH_RMII_MDINT   :PE14
 */

  /* Configure PA1, PA7 as input */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PC4, PC5 as input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4,GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PE14 as input */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /****************************************************************/
  /*                 MCO = PLL3Output = 50MHz                     */
  /****************************************************************/
  /* MCO pin configuration------------------------------------------------- */
  /* Configure MCO (PA8) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* set PLL3 clock output to 50MHz (25MHz /5 *10 =50MHz) */
  RCC_PLL3Config(RCC_PLL3Mul_10);
  /* Enable PLL3 */
  RCC_PLL3Cmd(ENABLE);
  /* Wait till PLL3 is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET)
  {}

  /* Get clock PLL3 clock on PA8 pin */
  RCC_MCOConfig(RCC_MCO_PLL3CLK);

}

u32 RTH_CONF(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // GPIOs init
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOA
                         | RCC_APB2Periph_GPIOB
                         | RCC_APB2Periph_GPIOC
                         | RCC_APB2Periph_AFIO
                         | RCC_APB2Periph_GPIOE, DISABLE);
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA
                         | RCC_APB2Periph_GPIOB
                         | RCC_APB2Periph_GPIOC
                         | RCC_APB2Periph_AFIO
                         | RCC_APB2Periph_GPIOE, ENABLE);

  // RMII
  AFIO->MAPR |=   1UL << 23;
  // no remap
  AFIO->MAPR &= ~(1UL << 21);
  // MDINT
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  // REF_CLK, CRS_DV
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // RXD0, RXD1
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // MDIO
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // TX_EN, TXD0, TXD1
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // MDC
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Enbale Ehrenet clk and relese reset
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETHR_MAC,ENABLE);
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETHR_RXEN,ENABLE);
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETHR_TXEN,ENABLE);
 /* Enable ETHERNET clock  */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx |
                        RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);

 // ETH_DMA->DMABMR |= 1;
 // while(ETH_DMA->DMABMR && 1);

  /* Software reset */
  ETH_SoftwareReset();
  /* Wait for software reset */
  while(ETH_GetSoftwareResetStatus()==SET);

  // MDC interface init
  ETH->MACMIIAR = 0;

    /* Initialize Tx Descriptors list: Chain Mode */
  ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
  /* Initialize Rx Descriptor s list: Chain Mode  */
  ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

  /* Check Phy ID */
  u16 Id1 = ETH_ReadPHYRegister(PHY_ADDRESS, 2);
  u16 Id2 = ETH_ReadPHYRegister(PHY_ADDRESS, 3);

   if(  (0x0006 == Id1)||(0x1C52 == Id2))
  {
    return 1;
  }
return 0;
}
