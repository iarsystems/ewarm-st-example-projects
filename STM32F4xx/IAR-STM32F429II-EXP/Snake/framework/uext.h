/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : uext.h
 *    Description : UEXT object definitions
 *
 *    History :
 *    1. Date        : 31, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __UEXT_H
#define __UEXT_H

#include "fw_config.h"

#include "gpio.h"
#include "spi.h"
#include "i2c.h"

typedef struct UEXTInfo {
#if defined __UEXT_UART__
  const UARTInfo_t * UART;
#endif
#if defined __UEXT_I2C__
  const I2CInfo_t  * I2C;
#endif
#if defined __UEXT_SPI__
  const SPIInfo_t  * SPI;
#endif
  const GPIOInfo_t * GPIO;
} UEXTInfo_t;

#endif /* __UEXT_H */
