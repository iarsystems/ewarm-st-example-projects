/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : stm32_spi.h
 *    Description : STM32 SPI library header
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __STM32_SPI_H
#define __STM32_SPI_H

#include "..\uext.h"
#include "..\mod.h"

#include "platform_config.h"

#define SPI_XFER_TIMEOUT    1000

/*************************************************************************
 * Function Name: STM32_SPI_init
 * Parameters:    const SPIInfo_t * spi - pointer to the SPI object
 *                SPIParameters_t * parameters - pointer to the SPI
 *                                               Parameters object
 * Return:        uint32_t - operation result
 *                SPI_RESULT_OK - operation completed successfully
 *                SPI_RESULT_INITED - SPI was already inited
 *
 * Description: Initialize the SPI object with the given parameters.
 *************************************************************************/
uint32_t STM32_SPI_init(const SPIInfo_t * spi, SPIParameters_t * parameters);

/*************************************************************************
 * Function Name: STM32_SPI_assertSSEL
 * Parameters:    const SPIInfo_t * spi - pointer to the SPI object
 *                GPIOInfo_t * gpio - pointer to the GPIO object
 * Return:        none
 *
 * Description: Asserts the SSEL pin.
 *************************************************************************/
void STM32_SPI_assertSSEL(const SPIInfo_t * spi, const struct GPIOInfo *gpio);

/*************************************************************************
 * Function Name: STM32_SPI_releaseSSEL
 * Parameters:    const SPIInfo_t * spi - pointer to the SPI object
 *                GPIOInfo_t * gpio - pointer to the GPIO object
 * Return:        none
 *
 * Description: Releases the SSEL pin.
 *************************************************************************/
void STM32_SPI_releaseSSEL(const SPIInfo_t * spi, const struct GPIOInfo *gpio);

/*************************************************************************
 * Function Name: STM32_SPI_send
 * Parameters:    const SPIInfo_t * spi - pointer to the SPI object
 *                uint8_t * buffer - buffer to send
 *                uint32_t len - count of bytes to send
 * Return:        uint32_t - operation result
 *                SPI_RESULT_OK - operation completed successfully
 *                SPI_RESULT_TIMEOUT - timeout error
 *
 * Description: Sends "len" bytes from "buffer" over SPI.
 *************************************************************************/
uint32_t STM32_SPI_send(const SPIInfo_t * spi, uint8_t *buffer, uint32_t len);

/*************************************************************************
 * Function Name: STM32_SPI_isBusy
 * Parameters:    const SPIInfo_t * spi - pointer to the SPI object
 * Return:        uint8_t - operation result
 *                0 - not busy
 *                other value - SPI is busy
 *
 * Description: Checks if SPI module is busy.
 *************************************************************************/
uint8_t STM32_SPI_isBusy(const SPIInfo_t * spi);

/*************************************************************************
 * Function Name: STM32_SPI_receive
 * Parameters:    const SPIInfo_t * spi - pointer to the SPI object
 *                uint8_t * buffer - buffer for receiving
 *                uint32_t len - count of bytes to receive
 * Return:        uint32_t - operation result
 *                SPI_RESULT_OK - operation completed successfully
 *                SPI_RESULT_TIMEOUT - timeout error
 *
 * Description: Receives "len" bytes to "buffer" from SPI device.
 *************************************************************************/
uint32_t STM32_SPI_receive(const SPIInfo_t * spi, uint8_t *buffer, uint32_t len);

#endif  /* __STM32_SPI_H */
