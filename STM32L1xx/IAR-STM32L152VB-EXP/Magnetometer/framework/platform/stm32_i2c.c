/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : stm32_i2c.c
 *    Description : STM32 I2C library
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 4202 $
 **************************************************************************/

#include "stm32_i2c.h"

/*************************************************************************
 * Function Name: STM32_I2C_init
 * Parameters:    const I2CInfo_t * i2c - pointer to the I2C object
 *                I2CParameters_t * parameters - set of I2C parameters
 * Return:        uint32_t - result of operation
 *                I2C_RESULT_OK - operation completed successfully
 *                I2C_RESULT_INITED - I2C module was already initialized
 *
 * Description: Initialize I2C module with the given I2C parameters.
 *************************************************************************/
uint32_t STM32_I2C_init(const I2CInfo_t * i2c, I2CParameters_t * parameters)
{
I2C_InitTypeDef  I2C_InitStructure;

  /* Check if the hardware is already initialized by this framework */
  if (i2c->i2cHwIf->isConfigured) return I2C_RESULT_INITED;

  /* DeInit the I2C hardware */
  I2C_DeInit((I2C_TypeDef*)I2C_BASE);

#if defined I2C1_BASE
  if ((uint32_t)I2C_BASE == I2C1_BASE)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
  }
#endif
#if defined I2C2_BASE
  if ((uint32_t)I2C_BASE == I2C2_BASE)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
  }
#endif
#if defined I2C3_BASE
  if ((uint32_t)I2C_BASE == I2C3_BASE)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C3, DISABLE);
  }
#endif

  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = parameters->speed;

  /* I2C Peripheral Enable */
  I2C_Cmd((I2C_TypeDef*)I2C_BASE, ENABLE);

  /* Apply I2C configuration after enabling it */
  I2C_Init((I2C_TypeDef*)I2C_BASE, &I2C_InitStructure);

  /* Mark HW interface as configured */
  i2c->i2cHwIf->isConfigured = 1;

  return I2C_RESULT_OK;
}

/*************************************************************************
 * Function Name: STM32_I2C_send
 * Parameters:    const I2CInfo_t * i2c - pointer to the I2C object
 *                uint8_t address - address of the I2C device
 *                uint8_t * buffer - buffer of data to be sent
 *                uint32_t len - count of bytes to send
 *                uint8_t stop - generate stop condition (1)
 *                               or do not (0)
 * Return:        uint32_t - result of operation
 *                I2C_RESULT_OK - operation completed successfully
 *                I2C_RESULT_ERR_BUSY - i2c busy error
 *                I2C_RESULT_ERR_SB - start bit error
 *                I2C_RESULT_ERR_ADDR - address send error
 *                I2C_RESULT_ERR_TX - error while transmitting data
 *
 * Description: Send "len" bytes of data, to i2c device.
 *************************************************************************/
uint32_t STM32_I2C_send(const I2CInfo_t * i2c, uint8_t address, uint8_t * buffer, uint32_t len, uint8_t stop)
{
uint32_t I2C_Timeout;
  /* Test on BUSY Flag */
  I2C_Timeout = I2C_LONG_TIMEOUT;
  while (I2C_GetFlagStatus((I2C_TypeDef*)I2C_BASE,I2C_FLAG_BUSY))
  {
    if((I2C_Timeout--) == 0) return I2C_RESULT_ERR_BUSY;
  }
  /* Enable the I2C peripheral */
  I2C_GenerateSTART((I2C_TypeDef*)I2C_BASE, ENABLE);

  /* Test on SB flag */
  I2C_Timeout = I2C_SHORT_TIMEOUT;
  while (I2C_GetFlagStatus((I2C_TypeDef*)I2C_BASE,I2C_FLAG_SB) == RESET)
  {
    if((I2C_Timeout--) == 0) return I2C_RESULT_ERR_SB;
  }

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress((I2C_TypeDef*)I2C_BASE, address, I2C_Direction_Transmitter);

  /* Test on ADDR flag */
  I2C_Timeout = I2C_SHORT_TIMEOUT;
  while (!I2C_CheckEvent((I2C_TypeDef*)I2C_BASE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2C_Timeout--) == 0)
    {
      I2C_GenerateSTOP((I2C_TypeDef*)I2C_BASE, ENABLE);
      return I2C_RESULT_ERR_ADDR;
    }
  }

  while(len--)
  {
    /* Transmit data */
    I2C_SendData((I2C_TypeDef*)I2C_BASE, *buffer++);

    /* Test on TXE Flag (data sent) */
    I2C_Timeout = I2C_LONG_TIMEOUT;
    while ((!I2C_GetFlagStatus((I2C_TypeDef*)I2C_BASE,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus((I2C_TypeDef*)I2C_BASE,I2C_FLAG_BTF)))
    {
      if((I2C_Timeout--) == 0)
      {
        I2C_GenerateSTOP((I2C_TypeDef*)I2C_BASE, ENABLE);
        return I2C_RESULT_ERR_TX;
      }
    }
  }
  /* Wait until BTF Flag is set before generating STOP */
  I2C_Timeout = I2C_LONG_TIMEOUT;
  while (!I2C_GetFlagStatus((I2C_TypeDef*)I2C_BASE,I2C_FLAG_BTF))
  {
    if((I2C_Timeout--) == 0)
    {
      I2C_GenerateSTOP((I2C_TypeDef*)I2C_BASE, ENABLE);
      return I2C_RESULT_ERR_TX;
    }
  }
  /* Send STOP Condition */
  if (stop) I2C_GenerateSTOP((I2C_TypeDef*)I2C_BASE, ENABLE);

  return I2C_RESULT_OK;
}

/*************************************************************************
 * Function Name: STM32_I2C_receive
 * Parameters:    const I2CInfo_t * i2c - pointer to the I2C object
 *                uint8_t address - address of the I2C device
 *                uint8_t * buffer - buffer to store data
 *                uint32_t len - count of bytes to read
 *                uint8_t stop - generate stop condition (1)
 *                               or do not (0)
 *                uint8_t checkbusy - check busy status before receiving (1, 0)
 * Return:        uint32_t - result of operation
 *                I2C_RESULT_OK - operation completed successfully
 *                I2C_RESULT_ERR_BUSY - i2c busy error
 *                I2C_RESULT_ERR_SB - start bit error
 *                I2C_RESULT_ERR_ADDR - address send error
 *                I2C_RESULT_ERR_RX - error while receiving data
 *
 * Description: Receives "len" bytes of data, from i2c device.
 *************************************************************************/
uint32_t STM32_I2C_receive(const I2CInfo_t * i2c, uint8_t address, uint8_t * buffer, uint32_t len, uint8_t stop, uint8_t checkbusy)
{
uint32_t I2C_Timeout;
  /* Test on BUSY Flag */
  if (checkbusy)
  {
    I2C_Timeout = I2C_LONG_TIMEOUT;
    while (I2C_GetFlagStatus((I2C_TypeDef*)I2C_BASE,I2C_FLAG_BUSY))
    {
      if((I2C_Timeout--) == 0) return I2C_RESULT_ERR_BUSY;
    }
  }

  /* Enable the I2C peripheral */
  I2C_GenerateSTART((I2C_TypeDef*)I2C_BASE, ENABLE);

  /* Test on SB flag */
  I2C_Timeout = I2C_SHORT_TIMEOUT;
  while (I2C_GetFlagStatus((I2C_TypeDef*)I2C_BASE,I2C_FLAG_SB) == RESET)
  {
    if((I2C_Timeout--) == 0) return I2C_RESULT_ERR_SB;
  }

  /* Send slave address for read */
  I2C_Send7bitAddress((I2C_TypeDef*)I2C_BASE, address, I2C_Direction_Receiver);

  /* Set ACK for received bytes ??? */
  I2C_AcknowledgeConfig((I2C_TypeDef*)I2C_BASE, ENABLE);

  /* Test on ADDR flag */
  I2C_Timeout = I2C_SHORT_TIMEOUT;
  while (!I2C_CheckEvent((I2C_TypeDef*)I2C_BASE, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    if((I2C_Timeout--) == 0)
    {
      I2C_GenerateSTOP((I2C_TypeDef*)I2C_BASE, ENABLE);
      return I2C_RESULT_ERR_ADDR;
    }
  }

  while(len--)
  {
    /* If last byte send NACK */
    if (!len) I2C_AcknowledgeConfig((I2C_TypeDef*)I2C_BASE, DISABLE);
    /* Test on RxNE flag */
    I2C_Timeout = I2C_LONG_TIMEOUT;
    while (!I2C_CheckEvent((I2C_TypeDef*)I2C_BASE, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
      if((I2C_Timeout--) == 0)
      {
        I2C_GenerateSTOP((I2C_TypeDef*)I2C_BASE, ENABLE);
        return I2C_RESULT_ERR_RX;
      }
    }
    *buffer++ = I2C_ReceiveData((I2C_TypeDef*)I2C_BASE);
  }
  /* Send STOP Condition */
  if (stop) I2C_GenerateSTOP((I2C_TypeDef*)I2C_BASE, ENABLE);

  return I2C_RESULT_OK;
}
