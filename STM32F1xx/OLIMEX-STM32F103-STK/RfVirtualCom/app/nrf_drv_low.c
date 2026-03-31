/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : nrf_drv_low.c
 *    Description : nRF24L01 low level driver
 *
 *    History :
 *    1. Date        : September 2007
 *       Author      : Stanimir Bonev
 *       Description : Initial Revision
 *    2. Date        : 20.12.2007
 *       Author      : Stoyan Choynev
 *       Description : Ported for Olimex STM32F103-STK
 *    $Revision: 39 $
 **************************************************************************/

/** include files **/
#include "nrf_drv_low.h"
/** local definitions **/

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/

/*************************************************************************
 * Function Name: nrf24l_init
 * Parameters: None
 *
 * Return: None
 *
 * Description: Configure NFR interface (SPI1 and GPIOC Pin 8). Configure
 *              NRF External Interrupt
 *
 *************************************************************************/
void nrf24l_init(void)
{
  SPI_InitTypeDef   SPI_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;

  /* Enable GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
  /* Enable SPI1 clock  */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);

  /* Configure SPI1 pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*Configure CS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_4 );

  /*Enable CPIOC clock*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, DISABLE);

  /* Configure NRF_CE */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init (GPIOC, &GPIO_InitStructure);
  /*Reset NRF_CE pin*/
	RF24L_CE_L();

  /* SPI1 Configure */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  RF24L_CS_H();                   //CS High
  SPI_SSOutputCmd(SPI1, ENABLE);  //Enable NSS Output
  /* SPI1 enable */
  SPI_Cmd(SPI1, ENABLE);

  /* Configure NRF_IRQ Pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init (GPIOC, &GPIO_InitStructure);

  /* Continue NRF_IRQ pin as EXTI9 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);

  /*Continue EXTI9*/
  EXTI_ClearITPendingBit(EXTI_Line9);

  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* NVIC */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NRF_IRQ_PreemptioinPriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = NRF_IRQ_SubPrioriry;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

/*************************************************************************
 * Function Name: SpiNrfSendData
 * Parameters: Byte to send.
 *
 * Return:  The value of the received Byte
 *
 * Description: Sends and receives a Byte from the SPI1.
 *
 *************************************************************************/
BYTE SpiNrfSendData (BYTE Byte)
{
  /* Send Byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, Byte );
  /* Wait transfer to be finished*/
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));
  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}

/** private functions **/
