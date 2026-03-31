/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : i2c2_drv.c
 *    Description : I2C2 Master mode driver
 *
 *    History :
 *    1. Date        : September 8, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#define I2C2_DRV_GLOBAL

#include "arm_comm.h"
#include "iar_stm32f429ii_sk.h"
#include "i2c2_drv.h"

static Boolean s_I2C_NotUsed;
static volatile Boolean s_Done, s_Error;
static Int8U s_SlaveAddr;
static volatile Int32U s_Size;
static pInt8U s_pData;

/*************************************************************************
 * Function Name: I2C2_ErrIntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: I2C2 error interrupt handler
 *
 *************************************************************************/
void I2C2_ER_IRQHandler(void)
{
  if(I2C_EVENT_SLAVE_ACK_FAILURE & I2C_GetLastEvent(I2C2))
  {
    // Generate Stop condition (return back to slave mode)
    I2C_GenerateSTOP(I2C2,ENABLE);
    I2C_ClearFlag(I2C2,I2C_FLAG_AF);
  }
  s_Done = TRUE;
  s_Error = TRUE;
}

/*************************************************************************
 * Function Name: I2C2_EvnIntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: I2C2 event interrupt handler
 *
 *************************************************************************/
void I2C2_EV_IRQHandler(void)
{
  switch (I2C_GetLastEvent(I2C2))
  {
  case I2C_EVENT_MASTER_MODE_SELECT:
    I2C_SendData(I2C2,s_SlaveAddr);
    if(0 == s_Size)
    {
      I2C_GenerateSTOP(I2C2,ENABLE);
      s_Done = TRUE;
    }
    break;
  case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
  case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
    if(0 == s_Size)
    {
      s_Done = TRUE;
    }
    else
    {
      --s_Size;
      I2C_SendData(I2C2,*s_pData++);
      if(0 == s_Size)
      {
        I2C_GenerateSTOP(I2C2,ENABLE);
      }
    }
    break;
  case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
    if (1 == s_Size)
    {
      I2C_AcknowledgeConfig(I2C2,DISABLE);
      I2C_GenerateSTOP(I2C2,ENABLE);
    }
    break;
  case I2C_EVENT_MASTER_BYTE_RECEIVED:
    if (1 == --s_Size)
    {
      I2C_GenerateSTOP(I2C2,ENABLE);
      I2C_AcknowledgeConfig(I2C2,DISABLE);
    }
    *s_pData++ = I2C_ReceiveData(I2C2);
    if (0 == s_Size)
    {
      s_Done = TRUE;
    }
    break;
  }
}

/*************************************************************************
 * Function Name: I2C2_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init I2C2 interface
 *
 *************************************************************************/
void I2C2_Init (void)
{
I2C_InitTypeDef  I2C_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

  s_I2C_NotUsed = TRUE;

  // Enable clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
  
  // Reset GPIO
  RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOH, DISABLE);

  I2C_DeInit(I2C2);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource4, GPIO_AF_I2C2);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource5, GPIO_AF_I2C2);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOH, &GPIO_InitStructure);

  // Enable clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

  // Reset I2C2
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
  
  // I2C configuration
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_OwnAddress1 = 0xAA;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C2_SPEED;

  // I2C2 Peripheral Enable
  I2C_Cmd(I2C2, ENABLE);

  // Apply I2C2 configuration after enabling it
  I2C_Init(I2C2, &I2C_InitStructure);

  // Enable the I2C2 Events Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = I2C2_INTR_PRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = I2C2_INTR_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // Enable the I2C2 Errors Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = I2C2_INTR_PRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = I2C2_INTR_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // Enable interrupts from I2C2 module
  I2C_ITConfig(I2C2, I2C_IT_BUF | I2C_IT_EVT | I2C_IT_ERR, ENABLE);
}

/*************************************************************************
 * Function Name: I2C2_Open
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Init I2C2 interface open
 *
 *************************************************************************/
Boolean I2C2_Open (void)
{
  return(0 != AtomicExchange(FALSE, &s_I2C_NotUsed));
}

/*************************************************************************
 * Function Name: I2C2_Close
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init I2C2 interface release
 *
 *************************************************************************/
void I2C2_Close (void)
{
  AtomicExchange(TRUE, &s_I2C_NotUsed);
}

/*************************************************************************
 * Function Name: I2C2_DataTransfer
 * Parameters: Int8U SlaveAddr, pInt8U pData, Int32U Size
 *
 * Return: none
 *
 * Description: I2C2 transfer data to/from slave
 *
 *************************************************************************/
Boolean I2C2_DataTransfer (Int8U SlaveAddr, pInt8U pData, Int32U Size)
{
  s_Error = FALSE;
  s_Done  = FALSE;
  s_SlaveAddr = SlaveAddr;
  s_Size = Size;
  s_pData = pData;

  I2C_AcknowledgeConfig(I2C2,ENABLE);
  I2C_GenerateSTART(I2C2,ENABLE);

  while(FALSE == s_Done);

  return(!s_Error);
}

