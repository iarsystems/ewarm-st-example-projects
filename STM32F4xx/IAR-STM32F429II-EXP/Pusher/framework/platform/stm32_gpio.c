/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : stm32_gpio.c
 *    Description : STM32 GPIO library
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include "stm32_gpio.h"

/*************************************************************************
 * Function Name: STM32_GPIO_getPortBase
 * Parameters:    uint8_t portNumber - port number
 * Return:        uint32_t - GPIO base address of the port
 *
 * Description: Returns GPIO base address of the given port.
 *************************************************************************/
uint32_t STM32_GPIO_getPortBase(uint8_t portNumber)
{
uint32_t GPIO_BASE;
  /* Find PORT_BASE address */
  switch (portNumber)
  {
    case 0: GPIO_BASE = GPIOA_BASE;
            break;
    case 1: GPIO_BASE = GPIOB_BASE;
            break;
    case 2: GPIO_BASE = GPIOC_BASE;
            break;
    case 3: GPIO_BASE = GPIOD_BASE;
            break;
    case 4: GPIO_BASE = GPIOE_BASE;
            break;
    case 5: GPIO_BASE = GPIOF_BASE;
            break;
    case 6: GPIO_BASE = GPIOG_BASE;
            break;
    case 7: GPIO_BASE = GPIOH_BASE;
            break;
   default: GPIO_BASE = 0;
            break;
  }
  return GPIO_BASE;
}

/*************************************************************************
 * Function Name: STM32_GPIO_init
 * Parameters:    const UEXTInfo_t * uext - pointer to the parent UEXT object
 *                GPIOParameters_t * parameters - set of GPIO parameters
 * Return:        none
 *
 * Description: Initialize GPIO module towards the MODPIN object array.
 *************************************************************************/
void STM32_GPIO_init(const UEXTInfo_t * uext, GPIOParameters_t * parameters)
{
GPIO_InitTypeDef  GPIO_InitStructure;
uint32_t GPIO_BASE;
uint32_t i;

  for(i=0; i<NUMBER_OF_IF_PINS; i++)
  {
    /* Enable GPIO clocks */
#if defined (STM32L1XX_MD) || defined (STM32L1XX_MDP) || defined (STM32L1XX_HD)
    RCC_AHBPeriphClockCmd((1 << uext->GPIO->portNumber[i]), ENABLE);
#elif defined (STM32F40XX) || defined (STM32F427X) || defined (STM32F429X)
    RCC_AHB1PeriphClockCmd((1 << uext->GPIO->portNumber[i]), ENABLE);
#else
    RCC_AHBPeriphClockCmd((1 << uext->GPIO->portNumber[i]), ENABLE);
#endif
    /* Set pin number */
    GPIO_InitStructure.GPIO_Pin = (1 << uext->GPIO->pinNumber[i]);

    /* Get GPIO_BASE address */
    GPIO_BASE = STM32_GPIO_getPortBase(uext->GPIO->portNumber[i]);

    /* Configure direction type */
    switch (parameters->pin[i].pinType)
    {
     case ptFUNC:
                     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
#if defined __UEXT_I2C__
                     /* Connect pin to I2C_SCL/I2C_SDA if needed*/
                     if ((pinSCL == i) || (pinSDA == i))
                     {
#if defined I2C1_BASE
                       if ((uint32_t)uext->I2C->i2cHwIf->registerBase == I2C1_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_I2C1);
                       }
#endif
#if defined I2C2_BASE
                       if ((uint32_t)uext->I2C->i2cHwIf->registerBase == I2C2_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_I2C2);
                       }
#endif
#if defined I2C3_BASE
                       if ((uint32_t)uext->I2C->i2cHwIf->registerBase == I2C3_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_I2C3);
                       }
#endif
                       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
                       GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
                     }
#endif
#if defined __UEXT_SPI__
                     /* Connect pin to SPI_MISO/SPI_MOSI/SPI_SCK/SPI_SSEL if needed */
                     if ((pinMISO == i) || (pinMOSI == i) || (pinSCK == i) || (pinSSEL == i))
                     {
#if defined SPI1_BASE
                       if ((uint32_t)uext->SPI->spiHwIf->registerBase == SPI1_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_SPI1);
                       }
#endif
#if defined SPI2_BASE
                       if ((uint32_t)uext->SPI->spiHwIf->registerBase == SPI2_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_SPI2);
                       }
#endif
#if defined SPI3_BASE
                       if ((uint32_t)uext->SPI->spiHwIf->registerBase == SPI3_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_SPI3);
                       }
#endif
#if defined SPI4_BASE
                       if ((uint32_t)uext->SPI->spiHwIf->registerBase == SPI4_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_SPI4);
                       }
#endif
#if defined SPI5_BASE
                       if ((uint32_t)uext->SPI->spiHwIf->registerBase == SPI5_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_SPI5);
                       }
#endif
#if defined SPI6_BASE
                       if ((uint32_t)uext->SPI->spiHwIf->registerBase == SPI6_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_SPI6);
                       }
#endif
#if defined (STM32L1XX_MD) || defined (STM32L1XX_MDP) || defined (STM32L1XX_HD)
                       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
#elif defined (STM32F40XX) || defined (STM32F427X) || defined (STM32F429X)
                       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
#else
                       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
#endif
                       GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
                     }
#endif
#if defined __UEXT_UART__
                     /* Connect pin to UART_RX/UART/TX if needed */
                     if ((pinTX == i) || (pinRX == i))
                     {
#if defined USART1_BASE
                       if ((uint32_t)uext->UART->uartHwIf->registerBase == USART1_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_USART1);
                       }
#endif
#if defined USART2_BASE
                       if ((uint32_t)uext->UART->uartHwIf->registerBase == USART2_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_USART2);
                       }
#endif
#if defined USART3_BASE
                       if ((uint32_t)uext->UART->uartHwIf->registerBase == USART3_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_USART3);
                       }
#endif
#if defined UART4_BASE
                       if ((uint32_t)uext->UART->uartHwIf->registerBase == UART4_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_UART4);
                       }
#endif
#if defined UART5_BASE
                       if ((uint32_t)uext->UART->uartHwIf->registerBase == UART5_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_UART5);
                       }
#endif
#if defined USART6_BASE
                       if ((uint32_t)uext->UART->uartHwIf->registerBase == USART6_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_USART6);
                       }
#endif
#if defined UART7_BASE
                       if ((uint32_t)uext->UART->uartHwIf->registerBase == UART7_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_UART7);
                       }
#endif
#if defined UART8_BASE
                       if ((uint32_t)uext->UART->uartHwIf->registerBase == UART8_BASE)
                       {
                         GPIO_PinAFConfig((GPIO_TypeDef*)GPIO_BASE, uext->GPIO->pinNumber[i], GPIO_AF_UART8);
                       }
#endif
                       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
                       GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
                     }
#endif
                     break;
     case ptOUTPUT:
                     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
#if defined (STM32L1XX_MD) || defined (STM32L1XX_MDP) || defined (STM32L1XX_HD)
                     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
#elif defined (STM32F40XX) || defined (STM32F427X) || defined (STM32F429X)
                     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
#else
                     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
#endif
                     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
                     break;
     case ptINPUT:
     default:
                     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
                     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
                     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
                     break;
    };

    switch (parameters->pin[i].pullState)
    {
     case psPULLUP:
                     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
                     break;
     case psPULLDN:
                     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
                     break;
     case psNORMAL:
     default:
                     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
                     break;
    };

    /* Init GPIO pin */
    GPIO_Init((GPIO_TypeDef*)GPIO_BASE, &GPIO_InitStructure);
  }
}

/*************************************************************************
 * Function Name: STM32_GPIO_readBit
 * Parameters:    const GPIOInfo_t * gpio - pointer to the GPIO object
 *                IFPINS_e pin - single pin from the IFPINS object
 * Return:        uint8_t - the input (GPIO) port pin value
 *
 * Description: Read single pin(bit) of the GPIO object.
 *************************************************************************/
uint8_t STM32_GPIO_readBit(const GPIOInfo_t * gpio, IFPINS_e pin)
{
uint32_t GPIO_BASE;

  GPIO_BASE = STM32_GPIO_getPortBase(gpio->portNumber[pin]);

  return GPIO_ReadInputDataBit((GPIO_TypeDef*)GPIO_BASE, (1 << gpio->pinNumber[pin]));
}

/*************************************************************************
 * Function Name: STM32_GPIO_writeBit
 * Parameters:    const GPIOInfo_t * gpio - pointer to the GPIO object
 *                IFPINS_e pin - single pin from the IFPINS object
 *                uint8_t state - state to set (0, 1)
 * Return:        uint8_t - the input (GPIO) port pin value
 *
 * Description: Sets single pin(bit) of the GPIO object to "state"
 *************************************************************************/
void STM32_GPIO_writeBit(const GPIOInfo_t * gpio, IFPINS_e pin, uint8_t state)
{
uint32_t GPIO_BASE;

  GPIO_BASE = STM32_GPIO_getPortBase(gpio->portNumber[pin]);

  GPIO_WriteBit((GPIO_TypeDef*)GPIO_BASE, (1 << gpio->pinNumber[pin]), (state)?Bit_SET:Bit_RESET);
}

/*************************************************************************
 * Function Name: STM32_GPIO_toggleBit
 * Parameters:    const GPIOInfo_t * gpio - pointer to the GPIO object
 *                IFPINS_e pin - single pin from the IFPINS object
 * Return:        uint8_t - the input (GPIO) port pin value
 *
 * Description: Toggles the state of the single pin(bit) of the GPIO object.
 *************************************************************************/
void STM32_GPIO_toggleBit(const struct GPIOInfo *gpio, IFPINS_e pin)
{
uint32_t GPIO_BASE;

  GPIO_BASE = STM32_GPIO_getPortBase(gpio->portNumber[pin]);

  GPIO_ToggleBits((GPIO_TypeDef*)GPIO_BASE, (1 << gpio->pinNumber[pin]));
}
