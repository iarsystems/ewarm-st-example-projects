/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : stm32_i2c.h
 *    Description : STM32 I2C library header
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __STM32_I2C_H
#define __STM32_I2C_H

#define I2C_BASE (i2c->i2cHwIf->registerBase)
#define I2C_ADDR (parameters->address)

#define I2C_SHORT_TIMEOUT         ((uint32_t)0x1000)
#define I2C_LONG_TIMEOUT          ((uint32_t)(10 * I2C_SHORT_TIMEOUT))

#include "..\i2c.h"

#include "platform_config.h"

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
uint32_t STM32_I2C_init(const I2CInfo_t * i2c, I2CParameters_t * parameters);

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
uint32_t STM32_I2C_send(const I2CInfo_t * i2c, uint8_t address, uint8_t * buffer, uint32_t len, uint8_t stop);

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
uint32_t STM32_I2C_receive(const I2CInfo_t * i2c, uint8_t address, uint8_t * buffer, uint32_t len, uint8_t stop, uint8_t checkbusy);

#endif /* __STM32_I2C_H */
