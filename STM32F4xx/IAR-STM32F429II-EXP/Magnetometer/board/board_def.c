/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : board_def.c
 *    Description : STM32F429II-EXP board definitions
 *
 *    History :
 *    1. Date        : 18, June 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include "../framework/fw_config.h"
#if defined __UEXT_I2C__
  #include "../framework/platform/stm32_i2c.h"
#endif
#if defined __UEXT_SPI__
  #include "../framework/platform/stm32_spi.h"
#endif
#if defined __UEXT_UART__
  #include "../framework/platform/stm32_uart.h"
#endif
#include "../framework/platform/stm32_gpio.h"
#include "../framework/uext.h"

#if defined __UEXT_I2C__
/* Define I2C hardware interface */
I2CHWInterface_t i2c1HwIf = {
  .registerBase = (uint32_t*)I2C1_BASE,
  .isConfigured = 0,
};

I2CHWInterface_t i2c2HwIf = {
  .registerBase = (uint32_t*)I2C2_BASE,
  .isConfigured = 0,
};

I2CHWInterface_t i2c3HwIf = {
  .registerBase = (uint32_t*)I2C3_BASE,
  .isConfigured = 0,
};

/* Define I2C UEXT interface */
const I2CInfo_t UEXT1_I2C = {
  .init = STM32_I2C_init,
  .send = STM32_I2C_send,
  .receive = STM32_I2C_receive,
  .i2cHwIf = &i2c1HwIf
};

const I2CInfo_t UEXT2_I2C = {
  .init = STM32_I2C_init,
  .send = STM32_I2C_send,
  .receive = STM32_I2C_receive,
  .i2cHwIf = &i2c2HwIf
};

const I2CInfo_t UEXT3_I2C = {
  .init = STM32_I2C_init,
  .send = STM32_I2C_send,
  .receive = STM32_I2C_receive,
  .i2cHwIf = &i2c3HwIf
};
#endif

#if defined __UEXT_SPI__
/* Define SPI hardware interface */
SPIHWInterface_t spi1HwIf = {
  .registerBase = (uint32_t*)SPI1_BASE,
  .isConfigured = 0,
};

SPIHWInterface_t spi2HwIf = {
  .registerBase = (uint32_t*)SPI2_BASE,
  .isConfigured = 0,
};

SPIHWInterface_t spi3HwIf = {
  .registerBase = (uint32_t*)SPI6_BASE,
  .isConfigured = 0,
};

/* Define SPI UEXT interface */
const SPIInfo_t UEXT1_SPI = {
  .init = STM32_SPI_init,
  .assertSSEL = STM32_SPI_assertSSEL,
  .releaseSSEL = STM32_SPI_releaseSSEL,
  .isBusy = STM32_SPI_isBusy,
  .send = STM32_SPI_send,
  .receive = STM32_SPI_receive,
  .spiHwIf = &spi1HwIf
};

const SPIInfo_t UEXT2_SPI = {
  .init = STM32_SPI_init,
  .assertSSEL = STM32_SPI_assertSSEL,
  .releaseSSEL = STM32_SPI_releaseSSEL,
  .isBusy = STM32_SPI_isBusy,
  .send = STM32_SPI_send,
  .receive = STM32_SPI_receive,
  .spiHwIf = &spi2HwIf
};

const SPIInfo_t UEXT3_SPI = {
  .init = STM32_SPI_init,
  .assertSSEL = STM32_SPI_assertSSEL,
  .releaseSSEL = STM32_SPI_releaseSSEL,
  .isBusy = STM32_SPI_isBusy,
  .send = STM32_SPI_send,
  .receive = STM32_SPI_receive,
  .spiHwIf = &spi3HwIf
};
#endif

/* Define GPIO UEXT interface */
const GPIOInfo_t UEXT1_GPIO = {
  .portNumber = {  1, 1, 1, 1, 0, 0, 0, 0 },
  .pinNumber =  {  6, 7, 8, 9, 6, 7, 5, 4 },
  .init = STM32_GPIO_init,
  .readBit = STM32_GPIO_readBit,
  .writeBit = STM32_GPIO_writeBit,
  .toggleBit = STM32_GPIO_toggleBit,
  .getPortBase = STM32_GPIO_getPortBase
};

const GPIOInfo_t UEXT2_GPIO = {
  .portNumber = { 0, 0,  1,  1,  1,  1,  1,  1 },
  .pinNumber =  { 2, 3, 10, 11, 14, 15, 13, 12 },
  .init = STM32_GPIO_init,
  .readBit = STM32_GPIO_readBit,
  .writeBit = STM32_GPIO_writeBit,
  .toggleBit = STM32_GPIO_toggleBit,
  .getPortBase = STM32_GPIO_getPortBase
};

const GPIOInfo_t UEXT3_GPIO = {
  .portNumber = {  2,  2, 0, 2,  6,  6,  6, 6 },
  .pinNumber =  { 10, 11, 8, 9, 12, 14, 13, 8 },
  .init = STM32_GPIO_init,
  .readBit = STM32_GPIO_readBit,
  .writeBit = STM32_GPIO_writeBit,
  .toggleBit = STM32_GPIO_toggleBit,
  .getPortBase = STM32_GPIO_getPortBase
};

/* Define UEXT instances */
const UEXTInfo_t UEXT1Instance = {
#if defined __UEXT_I2C__
  .I2C  = &UEXT1_I2C,
#endif
#if defined __UEXT_SPI__
  .SPI  = &UEXT1_SPI,
#endif
  .GPIO = &UEXT1_GPIO
};

const UEXTInfo_t UEXT2Instance = {
#if defined __UEXT_I2C__
  .I2C  = &UEXT2_I2C,
#endif
#if defined __UEXT_SPI__
  .SPI  = &UEXT2_SPI,
#endif
  .GPIO = &UEXT2_GPIO
};

const UEXTInfo_t UEXT3Instance = {
#if defined __UEXT_I2C__
  .I2C  = &UEXT3_I2C,
#endif
#if defined __UEXT_SPI__
  .SPI  = &UEXT3_SPI,
#endif
  .GPIO = &UEXT3_GPIO
};

const UEXTInfo_t * UEXT1 = &UEXT1Instance;
const UEXTInfo_t * UEXT2 = &UEXT2Instance;
const UEXTInfo_t * UEXT3 = &UEXT3Instance;
