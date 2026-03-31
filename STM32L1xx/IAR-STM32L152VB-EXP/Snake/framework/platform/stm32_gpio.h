/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : stm32_gpio.h
 *    Description : STM32 GPIO library
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __STM32_GPIO_H
#define __STM32_GPIO_H

#include "..\uext.h"
#include "..\mod.h"

#include "platform_config.h"

/*************************************************************************
 * Function Name: STM32_GPIO_init
 * Parameters:    const UEXTInfo_t * uext - pointer to the parent UEXT object
 *                GPIOParameters_t * parameters - set of GPIO parameters
 * Return:        none
 *
 * Description: Initialize GPIO module towards the MODPIN object array.
 *************************************************************************/
void STM32_GPIO_init(const UEXTInfo_t * uext, GPIOParameters_t * parameters);

/*************************************************************************
 * Function Name: STM32_GPIO_readBit
 * Parameters:    const GPIOInfo_t * gpio - pointer to the GPIO object
 *                IFPINS_e pin - single pin from the IFPINS object
 * Return:        uint8_t - the input (GPIO) port pin value
 *
 * Description: Read single pin(bit) of the GPIO object.
 *************************************************************************/
uint8_t STM32_GPIO_readBit(const struct GPIOInfo * gpio, IFPINS_e pin);

/*************************************************************************
 * Function Name: STM32_GPIO_writeBit
 * Parameters:    const GPIOInfo_t * gpio - pointer to the GPIO object
 *                IFPINS_e pin - single pin from the IFPINS object
 *                uint8_t state - state to set (0, 1)
 * Return:        uint8_t - the input (GPIO) port pin value
 *
 * Description: Sets single pin(bit) of the GPIO object to "state"
 *************************************************************************/
void STM32_GPIO_writeBit(const GPIOInfo_t * gpio, IFPINS_e pin, uint8_t state);

/*************************************************************************
 * Function Name: STM32_GPIO_toggleBit
 * Parameters:    const GPIOInfo_t * gpio - pointer to the GPIO object
 *                IFPINS_e pin - single pin from the IFPINS object
 * Return:        uint8_t - the input (GPIO) port pin value
 *
 * Description: Toggles the state of the single pin(bit) of the GPIO object.
 *************************************************************************/
void STM32_GPIO_toggleBit(const struct GPIOInfo * gpio, IFPINS_e pin);

/*************************************************************************
 * Function Name: STM32_GPIO_getPortBase
 * Parameters:    uint8_t portNumber - port number
 * Return:        uint32_t - GPIO base address of the port
 *
 * Description: Returns GPIO base address of the given port.
 *************************************************************************/
uint32_t STM32_GPIO_getPortBase(uint8_t portNumber);

#endif
