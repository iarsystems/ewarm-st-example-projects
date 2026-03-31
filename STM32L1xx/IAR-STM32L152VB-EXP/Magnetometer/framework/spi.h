/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : spi.h
 *    Description : SPI object definitions
 *
 *    History :
 *    1. Date        : 31, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>

#define SPI_RESULT_OK      0
#define SPI_RESULT_ERROR   1
#define SPI_RESULT_TIMEOUT 2
#define SPI_RESULT_INITED  3

#define SPI_CONFIG_CPHA     0x0001 
#define SPI_CONFIG_CPOL     0x0002
#define SPI_CONFIG_SOFTSSEL 0x0004
#define SPI_CONFIG_LSBFIRST 0x0008

typedef struct SPIParameters {
  uint32_t speed;
  uint32_t config;
} SPIParameters_t;

typedef struct SPIHWInterface {
  const uint32_t * registerBase;
        uint8_t    isConfigured;
} SPIHWInterface_t;

typedef struct SPIInfo {
  uint32_t (*init)(const struct SPIInfo *spi, SPIParameters_t * parameters);
  void     (*assertSSEL)(const struct SPIInfo *spi, const struct GPIOInfo *gpio);
  void     (*releaseSSEL)(const struct SPIInfo *spi, const struct GPIOInfo *gpio);
  uint8_t  (*isBusy)(const struct SPIInfo *spi);
  uint32_t (*send)(const struct SPIInfo *spi, uint8_t *buffer, uint32_t len);
  uint32_t (*receive)(const struct SPIInfo *spi, uint8_t *buffer, uint32_t len);
  SPIHWInterface_t * spiHwIf;
} SPIInfo_t;

#endif /* __SPI_H */
