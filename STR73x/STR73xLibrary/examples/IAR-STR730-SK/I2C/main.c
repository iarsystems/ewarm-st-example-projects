/******************** (C) COPYRIGHT 2005 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 09/27/2005 :  V1.0
* Description        : Main program body
**********************************************************************************
* History:
* 09/27/2005 :  V1.0
**********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*********************************************************************************/

/* Includes---------------------------------------------------------------------*/
  #include "73x_lib.h"

/* Include of other module interface headers -----------------------------------*/
/* Local includes --------------------------------------------------------------*/
/* Private typedef -------------------------------------------------------------*/
/* Private define --------------------------------------------------------------*/
#define BUFFER_SIZE 4
#define I2C1_SLAVE_ADDRESS  0x03A0
typedef enum { FAIL = 0, PASS = !FAIL } Status;

/* Private macro ---------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------*/
u8 Tx_Idx=0,Rx_Idx=0;
u8 I2C0_Buffer_Tx[BUFFER_SIZE], I2C1_Buffer_Rx[BUFFER_SIZE];
u8 I2C0_Buffer_Rx[BUFFER_SIZE], I2C1_Buffer_Tx[BUFFER_SIZE];
Status  TransferStatus1, TransferStatus2;

/*-----------------Define Peripherals Init Structure ---------------------------*/
GPIO_InitTypeDef  GPIO2_InitStructure;
GPIO_InitTypeDef  GPIO4_InitStructure;
I2C_InitTypeDef   I2C0_InitStructure;
I2C_InitTypeDef   I2C1_InitStructure;
CMU_InitTypeDef   CMU_InitStructure;

/* Private function prototypes -------------------------------------------------*/
/* Interface functions ---------------------------------------------------------*/
/* Private functions -----------------------------------------------------------*/
void Fill_Buffer(u8 *Buffer, u8 Offset);
Status Buffercmp(u8* pBuffer, u8* pBuffer1, u16 BufferLength);

int main()
{

#ifdef DEBUG
    debug();
#endif

/*--------------------------------------GPIO4--------------------------------------------*/
  /* GPIO4 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_GPIO4, ENABLE);
  /* GPIO4 Configuration */
  GPIO_DeInit (GPIO4);
  GPIO_StructInit(&GPIO4_InitStructure);
  GPIO4_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO4_InitStructure.GPIO_Pins = GPIO_PIN_6 | GPIO_PIN_7 ;
  GPIO_Init (GPIO4, &GPIO4_InitStructure);

/*--------------------------------------GPIO2--------------------------------------------*/
  /* GPIO2 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_GPIO2, ENABLE);
  /* GPIO2 Configuration */
  GPIO_DeInit (GPIO2);
  GPIO_StructInit(&GPIO2_InitStructure);
  GPIO2_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO2_InitStructure.GPIO_Pins = GPIO_PIN_14 | GPIO_PIN_15 ;
  GPIO_Init (GPIO2, &GPIO2_InitStructure);

/*---------------------------------------CMU---------------------------------------------*/
  CMU_DeInit( );
  CMU_StructInit(&CMU_InitStructure);
  CMU_InitStructure.CMU_CKSEL0 = CMU_CKSEL0_CKOSC;
  CMU_Init(&CMU_InitStructure);
  /* Disable DIV2 */
  PRCCU->CFR &= 0x7FFF;

/*----------------------------I2C0 -------------------------------------*/
  /* I2C0 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_I2C0, ENABLE);
  /* I2C0 Configuration */
  I2C_DeInit(I2C0);
  I2C_StructInit(&I2C0_InitStructure);
  I2C0_InitStructure.I2C_GeneralCall = I2C_GeneralCallDisable;
  I2C0_InitStructure.I2C_Ack = I2C_AckEnable;
  I2C0_InitStructure.I2C_CLKSpeed = 10000;
  I2C0_InitStructure.I2C_OwnAddress = 0xA0;

/*----------------------------I2C1 -------------------------------------*/
  /* I2C1 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_I2C1, ENABLE);
  /* I2C1 Configuration */
  I2C_DeInit(I2C1);
  I2C_StructInit(&I2C1_InitStructure);
  I2C1_InitStructure.I2C_GeneralCall = I2C_GeneralCallDisable;
  I2C1_InitStructure.I2C_Ack = I2C_AckEnable;
  I2C1_InitStructure.I2C_CLKSpeed = 10000;
  I2C1_InitStructure.I2C_OwnAddress = 0x03A0;

/*-------------------------------------EIC------------------------------------------*/
  /* EIC Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_EIC, ENABLE);
  /* EIC Config */
  EIC_IRQChannelPriorityConfig( I2C1ITDDC_IRQChannel, 1);
  EIC_IRQChannelConfig( I2C1ITDDC_IRQChannel, ENABLE );
  EIC_IRQCmd(ENABLE);

  /*--------------------------------------------------*/
  /* Transmission Phase-------------------------------*/
  /*--------------------------------------------------*/

  /* Enable I2C0 and I2C1 */
  I2C_Cmd (I2C0, ENABLE);
  I2C_Cmd (I2C1, ENABLE);
  /* Initialize I2C0 and I2C1 */
  I2C_Init(I2C0, &I2C0_InitStructure);
  I2C_Init(I2C1, &I2C1_InitStructure);
  /* Fill buffer to send */
  Fill_Buffer(I2C0_Buffer_Tx,1);
  /* I2C1 Interrupt Config */
  I2C_ITConfig (I2C1,ENABLE);
  /* Send STRAT condition */
  I2C_STARTGenerate (I2C0, ENABLE);
  /* Test on EV5 and clear it */
  while( ! I2C_EventCheck(I2C0, I2C_EVENT_MASTER_MODE_SELECT) );  // EV5
  /* Send I2C1 Address for write */
  I2C_AddressSend (I2C0, I2C1_SLAVE_ADDRESS, I2C_MODE_ADDRESS7, I2C_MODE_TRANSMITTER);
  /* Test on EV6 and clear it */
  while( ! I2C_EventCheck(I2C0, I2C_EVENT_MASTER_MODE_SELECTED) );  // EV6
  /* Clear EV6 by set again the PE bit */
  I2C_Cmd (I2C0, ENABLE);
  /* Send Buffer Data */
  I2C_BufferSend (I2C0, I2C0_Buffer_Tx, BUFFER_SIZE);
  /* Send STOP Condition */
  I2C_STOPGenerate (I2C0, ENABLE);
  /* Disable I2C0 and I2C1 */
  I2C_Cmd(I2C0, DISABLE);
  I2C_Cmd(I2C1, DISABLE);

  /*--------------------------------------------------*/
  /* Check the I2C1_Buffer_Rx received Buffer---------*/
  /*--------------------------------------------------*/

  /* Check if the data read by I2C1 is correct */
  TransferStatus1 = Buffercmp(I2C0_Buffer_Tx, I2C1_Buffer_Rx, BUFFER_SIZE);
  /* TransferStatus1 = PASS, if the data received by I2C1 is correct */
  /* TransferStatus1 = FAIL, if the data received by I2C1 is corrupted */

  /*--------------------------------------------------*/
  /* Reception Phase----------------------------------*/
  /*--------------------------------------------------*/

  /* Reset index counter for reception */
  Tx_Idx=Rx_Idx=0;
  /* Enable I2C0 and I2C1 */
  I2C_Cmd (I2C0, ENABLE);
  I2C_Cmd (I2C1, ENABLE);
  /* Initialize I2C0 and I2C1 */
  I2C_Init(I2C0, &I2C0_InitStructure);
  I2C_Init(I2C1, &I2C1_InitStructure);
  /* Fill buffer to send */
  Fill_Buffer(I2C1_Buffer_Tx,5);
  /* I2C1 Interrupt Config */
  I2C_ITConfig (I2C1,ENABLE);
  /* Send STRAT condition */
  I2C_STARTGenerate (I2C0, ENABLE);
  /* Test on EV5 and clear it */
  while( ! I2C_EventCheck(I2C0, I2C_EVENT_MASTER_MODE_SELECT) );  // EV5
  /* Send EEPROM Address for write */
  I2C_AddressSend (I2C0, I2C1_SLAVE_ADDRESS, I2C_MODE_ADDRESS7, I2C_MODE_RECEIVER);
  /* Test on EV6 and clear it */
  while( ! I2C_EventCheck(I2C0, I2C_EVENT_MASTER_MODE_SELECTED) );  // EV6
  /* Clear EV6 by set again the PE bit */
  I2C_Cmd (I2C0, ENABLE);
  /* Receive Buffer Data */
  I2C_BufferReceive (I2C0, I2C0_Buffer_Rx, BUFFER_SIZE);
  /* Send STOP Condition */
  I2C_STOPGenerate (I2C0, ENABLE);
  /* Disable I2C0 and I2C1 */
  I2C_Cmd(I2C0, DISABLE);
  I2C_Cmd(I2C1, DISABLE);

  /*--------------------------------------------------*/
  /* Check the I2C0_Buffer_Rx received Buffer---------*/
  /*--------------------------------------------------*/

  /* Check if the data read by I2C0 is correct */
  TransferStatus2 = Buffercmp(I2C1_Buffer_Tx, I2C0_Buffer_Rx, BUFFER_SIZE);
  /* TransferStatus2 = PASS, if the data received by I2C0 is correct */
  /* TransferStatus2 = FAIL, if the data received by I2C0 is corrupted */


while(1);
}

/* Private functions -------------------------------------------------------- */

/*******************************************************************************
Function name : void Fill_Buffer(U8 *Buffer)
Description   : Fill the gloal buffer
Input1        : [u8] the Buffer to fill
Input2        : [u8] offset value to fill Buffer data
Output param  : None
*******************************************************************************/
void Fill_Buffer(u8  *Buffer, u8 Offset)
{
  u8 IndexTmp;

  for( IndexTmp = 0; IndexTmp < BUFFER_SIZE; IndexTmp++ )
  {
    Buffer[IndexTmp] =IndexTmp + Offset;
  }
}

/*******************************************************************************
* Function Name  : Buffercmp
* Description    : Compares two buffers.
* Input          : - pBuffer, pBuffer1: buffers to be compared.
*                : - BufferLength: buffer's length
* Output         : None
* Return         : PASS: pBuffer identical to pBuffer1
*                  FAIL: pBuffer differs from pBuffer1
*******************************************************************************/
Status Buffercmp(u8* pBuffer, u8* pBuffer1, u16 BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer != *pBuffer1)
    {
      return FAIL;
    }

    pBuffer++;
    pBuffer1++;
  }
  return PASS;
}

/******************* (C) COPYRIGHT 2005 STMicroelectronics *****END OF FILE****/

