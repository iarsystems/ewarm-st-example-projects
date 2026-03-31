/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : STR912_enet.c
 *    Description : MAC/DMA Controller with DMA (ENET) driver
 *
 *    History :
 *    1. Date        : October 4, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "STR912_enet.h"

#pragma data_alignment=16
EnetDmaDesc_t EnetDmaRx;

#pragma data_alignment=16
EnetDmaDesc_t EnetDmaTx;

#pragma data_alignment=4
Int8U RxBuff[1520];
#pragma data_alignment=4
Int8U TxBuff[1520];

/*******************************************************************************
* Function Name  : ENET_RxDscrInit
* Description    : Initializes the Rx ENET descriptor chain. Single Descriptor
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void ENET_RxDscrInit(void)
{
  /* Initialization */
  /* Assign temp Rx array to the ENET buffer */
  EnetDmaRx.Rx.pBuffer = (pInt32U)RxBuff;

  /* Initialize RX ENET Status and control */
  EnetDmaRx.Rx.EnetRxCR.Data = 0;

  /* Initialize the next descriptor- In our case its single descriptor */
  EnetDmaRx.Rx.EnetDmaNextDesc = &EnetDmaRx;

  /* Setting the RX NEXT Descriptor Register inside the ENET */
  ENET_RXNDAR = (Int32U)&EnetDmaRx;

  /* Set the max packet size  */
  EnetDmaRx.Rx.EnetRxCR.DMA_XFERCOUNT = EMAC_MAX_PACKET_SIZE;

  /* Setting the VALID bit */
  EnetDmaRx.Rx.EnetRxSR.Valid = 1;

}

/*******************************************************************************
* Function Name  : ENET_TxDscrInit
* Description    : Initializes the Tx ENET descriptor chain with single descriptor
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void ENET_TxDscrInit(void)
{

  /* ENET Start Address */
  EnetDmaTx.Tx.pBuffer = (pInt32U)TxBuff;

  /* Next Descriptor Address */
  EnetDmaTx.Tx.EnetDmaNextDesc = &EnetDmaTx;

  /* Initialize ENET status and control */
  EnetDmaTx.Tx.EnetTxCR.Data = 0;

  /* Tx next set to Tx descriptor base */
  ENET_TXNDAR = (Int32U)&EnetDmaTx;

  /* Enable next enable */
  ENET_TXNDAR_bit.NPOL_EN = 1;

}

/*************************************************************************
 * Function Name:
 * Parameters: None
 *
 * Return: None
 *
 * Description: Init  MAC/DMA Controller
 *
 *************************************************************************/
void tapdev_init(void)
{
volatile Int32U i;
GPIO_InitTypeDef  GPIO_InitStructure;

  EnetDmaTx.Tx.EnetTxSR.Valid = 0;
  EnetDmaRx.Rx.EnetRxSR.Valid = 0;
  // Init Sys
  // Reset
  SCU_AHBPeriphReset(__ENET,ENABLE);
  // Clock enable
  SCU_AHBPeriphClockConfig(__ENET,ENABLE);
  // Reset release
  SCU_AHBPeriphReset(__ENET,DISABLE);

  // Assign pins to the MII interface
  // Enable GPIO clocks
  SCU_APBPeriphClockConfig(__GPIO0 | __GPIO1 | __GPIO5, ENABLE);
  // Release GPIO reset
  SCU_APBPeriphReset(__GPIO0 | __GPIO1 | __GPIO5, DISABLE);

  // GPIO0 All pins Default Input function
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIO0, &GPIO_InitStructure);

  // GPIO1 1,2,3,4,7 Push-Pull outputs  Alt func 2
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |\
                                GPIO_Pin_4 | GPIO_Pin_7;
  GPIO_Init(GPIO1, &GPIO_InitStructure);

  // GPIO1 0,5,6 inputs
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_Init(GPIO1, &GPIO_InitStructure);

  // GPIO5 2,3 Push-Pull outputs  Alt func 2
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_Init(GPIO5, &GPIO_InitStructure);

  // MII_PHYCLK Enable
  SCU_PHYCLKConfig(ENABLE);

  ENET_SCR_bit.SRESET     = 1;    // MAC DMA Software reset

  ENET_SCR_bit.SRESET = 0;        // MAC DMA Software reset release
  ENET_CCR_bit.SEL_CLK =0;        // Clock configuration

  // MAC Control Register
  ENET_MCR_bit.RA = 0;            // Receive filtering depend of Address filtering mode
  ENET_MCR_bit.EN = 0;            // little endian mode
  ENET_MCR_bit.PS = 0;            // Prescaler bits
  ENET_MCR_bit.RCFA = 0;          // bits transmission order right to left
  ENET_MCR_bit.DRO  = 0;          // Enable the MAC controller to receive all the incoming packets
  ENET_MCR_bit.LM   = 0;          // Loopback Mode - Normal mode
  ENET_MCR_bit.FDM  = 1;          // Full duplex mode
  ENET_MCR_bit.AFM  = 0;          // MAC address perfect filtering for physical and multicast addresses
  ENET_MCR_bit.PWF  = 0;          // Wrong frames are filtered
  ENET_MCR_bit.VFM  = 0;          // VLAN Filtering Mode
  ENET_MCR_bit.ELC  = 0;          // Disable Late Collision
  ENET_MCR_bit.DBF  = 0;          // Broadcast frame reception enabled
  ENET_MCR_bit.DPR  = 0;          // Enable Packet Retry
  ENET_MCR_bit.RVFF = 1;          // Enable VCI Rx Frame filtering
  ENET_MCR_bit.APR  = 1;          // Automatic Pad Removal
  ENET_MCR_bit.BL   = 1;          // 01: #bits used from LFSR to initialize the slot-time counter = 8
  ENET_MCR_bit.DCE  = 1;          // Deferral Check Enable
  ENET_MCR_bit.RVBE = 1;          // Reception VCI Burst Enable
  ENET_MCR_bit.RCFA = 0;          // Reverse Control Frame Address disable
  // DMA Status/Control Register
  ENET_SCR_bit.RX_MAX_BURST_SIZE = 0; // 16-beat incrementing burst (INCR16)
  ENET_SCR_bit.RX_MAX_BURST_SIZE = 3; // Single transfers only (SINGLE)
  // Set MAC Physical address
  *(pInt32U)&ENET_MAL = (Int32U)(UIP_ETHADDR3 << 24) +\
             (UIP_ETHADDR2 << 16) +\
             (UIP_ETHADDR1 << 8)  +\
             UIP_ETHADDR0;
  ENET_MAH = (UIP_ETHADDR5 << 8) + UIP_ETHADDR4;
  // Put the PHY in reset mode
  ENET_MIIWriteRegister(MII_PHY_ADDR,MAC_MII_REG_XCR, 0x8000);
  // Delay to assure PHY reset
  for(i=0; i<0xFFFFF; i++);
  // Set PHY operation mode
  ENET_MIIWriteRegister(MII_PHY_ADDR,MAC_MII_REG_XCR, PHY_OPR_MODE);

  // Init Rx and Tx DMA descriptors
  ENET_TxDscrInit();
  ENET_RxDscrInit();

  // Disable DMA
  ENET_RXSTR_bit.DMA_EN = 0; // RX DMA fetching descriptors
  ENET_TXSTR_bit.DMA_EN = 0; // TX DMA fetching descriptors

  /* Reset all interrupts */
  ENET_ISR = 0xFFFFFFFF;

  /* Setup Descriptor Fetch ENET_PhyDelay for Receive Block */
  ENET_RXSTR_bit.DFETCH_DLY = 0x0100;

  /* Setup Descriptor Fetch ENET_PhyDelay for Transmit Block */
  ENET_TXSTR_bit.DFETCH_DLY = 0x0100;

  /* Set Tx underrun bit */
  ENET_TXSTR_bit.UNDER_RUN = 1;

  /* Clear the interrupts */
  ENET_IER = 0x0;

  // Enable
  ENET_MCR |= 0xc;                // Reception and Transmission Enable
  ENET_RXSTR_bit.START_FETCH = 1; // Start RX DMA fetching descriptors
}

/*************************************************************************
 * Function Name: tapdev_read
 * Parameters:
 * Return:
 *
 * Description: Read data for MAC/DMA Controller
 *
 *************************************************************************/
Int32U tapdev_read(void * pPacket)
{
  Int32U size;
  Int32U value;

  /*check for validity*/
  if(!EnetDmaRx.Rx.EnetRxSR.Valid)
  {
    /*Get the size of the packet*/
    size = ((EnetDmaRx.Rx.EnetRxSR.FrameLength) - 4);
    //MEMCOPY_L2S_BY4((u8*)ppkt, RxBuff, size); /*optimized memcopy function*/
    memcpy(pPacket, RxBuff, size);   //string.h library*/
  }
  else
  {
    return(ENET_NOK);
  }
  /* Give the buffer back to ENET */
  EnetDmaRx.Rx.EnetRxSR.Valid = 1;
  /* Start the receive operation */
  ENET_RXSTR_bit.START_FETCH = 1;
  /* Return no error */
  return size;
}

/*************************************************************************
 * Function Name: tapdev_send
 * Parameters:
 * Return:
 *
 * Description: Send data to MAC/DMA Controller
 *
 *************************************************************************/
void tapdev_send(void *pPacket, Int32U size)
{
  /* Copy the  application buffer to the driver buffer
     Using this MEMCOPY_L2L_BY4 makes the copy routine faster
     than memcpy */
  //MEMCOPY_L2S_BY4((u8*)TxBuff, (u8*)ppkt, size);
  memcpy(TxBuff, pPacket, size);

  /* Assign ENET address to Temp Tx Array */
  EnetDmaTx.Tx.pBuffer = (pInt32U)TxBuff;

  /* Setting the Frame Length*/
  EnetDmaTx.Tx.EnetTxCR.DMA_XFERCOUNT = (size&0xFFF);

  /* Start the ENET by setting the VALID bit in dmaPackStatus of current descr*/
  EnetDmaTx.Tx.EnetTxSR.Valid = 1;

  /* Start the receive operation */
  ENET_TXSTR_bit.START_FETCH = 1;
}

/*************************************************************************
 * Function Name: ENET_MIIWriteRegister
 * Parameters: Int8U DevId, Int8U RegAddr, Int32U Value
 * Return: none
 *
 * Description: Writes a value on the PHY registers
 *
 *************************************************************************/
static void ENET_MIIWriteRegister (Int8U DevId, Int8U RegAddr, Int32U Value)
{
   while(ENET_MIIA_bit.BUSY);
   ENET_MIID = Value;
   ENET_MIIA_bit.PADDR = DevId;   // set the MII Physical address
   ENET_MIIA_bit.RADDR = RegAddr; // set the MII register address
   ENET_MIIA_bit.WR = 1;          // set write operation
   ENET_MIIA_bit.BUSY = 1;        // start write operation
   // wait until write complete
   while(ENET_MIIA_bit.BUSY);
}

/*************************************************************************
 * Function Name: ENET_MIIReadRegister
 * Parameters: Int8U DevId, Int8U RegAddr, Int32U Value
 * Return: Int32U
 *
 * Description: Read a value from the PHY registers
 *
 *************************************************************************/
static Int32U ENET_MIIReadRegister (Int8U DevId, Int8U RegAddr)
{
   while(ENET_MIIA_bit.BUSY);
   ENET_MIIA_bit.PADDR = DevId;   // set the MII Physical address
   ENET_MIIA_bit.RADDR = RegAddr; // set the MII register address
   ENET_MIIA_bit.WR = 0;          // set read operation
   ENET_MIIA_bit.BUSY = 1;        // start write operation
   // wait until read complete
   while(ENET_MIIA_bit.BUSY);
   return(ENET_MIID);
}


