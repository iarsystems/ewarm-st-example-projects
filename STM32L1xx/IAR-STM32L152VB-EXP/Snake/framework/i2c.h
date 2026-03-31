/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : i2c.h
 *    Description : I2C object definitions
 *
 *    History :
 *    1. Date        : 31, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>

#define I2C_RESULT_OK       0
#define I2C_RESULT_ERROR    1
#define I2C_RESULT_TIMEOUT  2
#define I2C_RESULT_INITED   3
#define I2C_RESULT_ERR_BUSY 10
#define I2C_RESULT_ERR_SB   11
#define I2C_RESULT_ERR_ADDR 12
#define I2C_RESULT_ERR_TX   13
#define I2C_RESULT_ERR_RX   14

#define I2C_SKIP_STOP       0
#define I2C_SEND_STOP       1

#define I2C_SKIP_BUSY       0
#define I2C_CHECK_BUSY      1

typedef struct I2CParameters {
  uint32_t speed;
} I2CParameters_t;

typedef struct I2CHWInterface {
  const uint32_t * registerBase;
        uint8_t    isConfigured;
} I2CHWInterface_t;

typedef struct I2CInfo {
  uint32_t (*init)(const struct I2CInfo *i2c, I2CParameters_t * parameters);
  uint32_t (*send)(const struct I2CInfo *i2c, uint8_t address, uint8_t *buffer, uint32_t len, uint8_t stop);
  uint32_t (*receive)(const struct I2CInfo *i2c, uint8_t address, uint8_t *buffer, uint32_t len, uint8_t stop, uint8_t checkbusy);
  I2CHWInterface_t * i2cHwIf;
} I2CInfo_t;

#endif /* __I2C_H */
