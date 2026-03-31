/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : stm32_spi.c
 *    Description : STM32 SPI library
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include "stm32_spi.h"

#define SPI_BASE (spi->spiHwIf->registerBase)
#define SPI_ADDR (parameters->address)

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
uint32_t STM32_SPI_init(const SPIInfo_t * spi, SPIParameters_t * parameters)
{
SPI_InitTypeDef   SPI_InitStructure;
RCC_ClocksTypeDef clocks;
uint32_t          pclkFreq;
uint16_t          divider = 2;

  /* Check if the hardware is already initialized by this framework */
  if (spi->spiHwIf->isConfigured) return SPI_RESULT_INITED;

  /* DeInit the SPI hardware */
  SPI_I2S_DeInit((SPI_TypeDef*)SPI_BASE);

  /* Get clocks values */
  RCC_GetClocksFreq(&clocks);

#if defined SPI1_BASE
  if((uint32_t)SPI_BASE == SPI1_BASE)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
    pclkFreq = clocks.PCLK2_Frequency;
  }
#endif
#if defined SPI2_BASE
  if((uint32_t)SPI_BASE == SPI2_BASE)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
    pclkFreq = clocks.PCLK1_Frequency;
  }
#endif
#if defined SPI3_BASE
  if((uint32_t)SPI_BASE == SPI3_BASE)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);
    pclkFreq = clocks.PCLK1_Frequency;
  }
#endif
#if defined SPI4_BASE
  if((uint32_t)SPI_BASE == SPI4_BASE)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI4, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI4, DISABLE);
    pclkFreq = clocks.PCLK2_Frequency;
  }
#endif
#if defined SPI5_BASE
  if((uint32_t)SPI_BASE == SPI5_BASE)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI5, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI5, DISABLE);
    pclkFreq = clocks.PCLK2_Frequency;
  }
#endif
#if defined SPI6_BASE
  if((uint32_t)SPI_BASE == SPI6_BASE)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI6, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI6, DISABLE);
    pclkFreq = clocks.PCLK2_Frequency;
  }
#endif

  /* SPI Config */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = (parameters->config & SPI_CONFIG_CPOL)?SPI_CPOL_High:SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = (parameters->config & SPI_CONFIG_CPHA)?SPI_CPHA_2Edge:SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS  = (parameters->config & SPI_CONFIG_SOFTSSEL)?SPI_NSS_Soft:SPI_NSS_Hard;

  /* SPI baud rate calculate */
  while((parameters->speed * divider) < pclkFreq)
  {
    divider <<= 1;
    if (divider == 256) break;
  }

  switch(divider)
  {
    case   2: SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; break;
    case   4: SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; break;
    case   8: SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; break;
    case  16: SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; break;
    case  32: SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; break;
    case  64: SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; break;
    case 128: SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; break;
    default :
    case 256: SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; break;
  }
  SPI_InitStructure.SPI_FirstBit = (parameters->config & SPI_CONFIG_LSBFIRST)?SPI_FirstBit_LSB:SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;

  SPI_Init((SPI_TypeDef*)SPI_BASE, &SPI_InitStructure);

  /* Enable SPI */
  SPI_Cmd((SPI_TypeDef*)SPI_BASE, ENABLE);

  /* Enable/Disable output of SSEL signal */
  if (parameters->config & SPI_CONFIG_SOFTSSEL)
  {
    SPI_SSOutputCmd((SPI_TypeDef*)SPI_BASE, DISABLE);
  }
  else
  {
    SPI_SSOutputCmd((SPI_TypeDef*)SPI_BASE, ENABLE);
  }

  /* Mark hardware as configured */
  spi->spiHwIf->isConfigured = 1;

  return SPI_RESULT_OK;
}

/*************************************************************************
 * Function Name: STM32_SPI_assertSSEL
 * Parameters:    const SPIInfo_t * spi - pointer to the SPI object
 *                GPIOInfo_t * gpio - pointer to the GPIO object
 * Return:        none
 *
 * Description: Asserts the SSEL pin.
 *************************************************************************/
void STM32_SPI_assertSSEL(const SPIInfo_t * spi, const struct GPIOInfo *gpio)
{
  gpio->writeBit(gpio, pinSSEL, 0);
}

/*************************************************************************
 * Function Name: STM32_SPI_releaseSSEL
 * Parameters:    const SPIInfo_t * spi - pointer to the SPI object
 *                GPIOInfo_t * gpio - pointer to the GPIO object
 * Return:        none
 *
 * Description: Releases the SSEL pin.
 *************************************************************************/
void STM32_SPI_releaseSSEL(const SPIInfo_t *spi, const struct GPIOInfo *gpio)
{
  gpio->writeBit(gpio, pinSSEL, 1);
}

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
uint32_t STM32_SPI_send(const SPIInfo_t * spi, uint8_t *buffer, uint32_t len)
{
uint32_t to;
  while(len--)
  {
    to=SPI_XFER_TIMEOUT;
    SPI_I2S_SendData((SPI_TypeDef*)SPI_BASE, *buffer++);
    while (!SPI_I2S_GetFlagStatus((SPI_TypeDef*)SPI_BASE, SPI_I2S_FLAG_TXE)) { if (!to--) return SPI_RESULT_TIMEOUT; }
  }
  return SPI_RESULT_OK;
}

/*************************************************************************
 * Function Name: STM32_SPI_isBusy
 * Parameters:    const SPIInfo_t * spi - pointer to the SPI object
 * Return:        uint8_t - operation result
 *                0 - not busy
 *                other value - SPI is busy
 *
 * Description: Checks if SPI module is busy.
 *************************************************************************/
uint8_t STM32_SPI_isBusy(const SPIInfo_t * spi)
{
  return SPI_I2S_GetFlagStatus((SPI_TypeDef*)SPI_BASE, SPI_I2S_FLAG_BSY);
}

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
uint32_t STM32_SPI_receive(const SPIInfo_t * spi, uint8_t *buffer, uint32_t len)
{
uint32_t to; 
  while(len--)
  {
    to=SPI_XFER_TIMEOUT;
    *buffer++ = SPI_I2S_ReceiveData((SPI_TypeDef*)SPI_BASE);
    while (!SPI_I2S_GetFlagStatus((SPI_TypeDef*)SPI_BASE, SPI_I2S_FLAG_RXNE)) { if (!to--) return SPI_RESULT_TIMEOUT; }
  }
  return SPI_RESULT_OK;
}
