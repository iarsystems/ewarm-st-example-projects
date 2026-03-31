/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : gpio.h
 *    Description : GPIO object definitions
 *
 *    History :
 *    1. Date        : 31, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>

#include "ifpins.h"
#include "mod.h"

struct UEXTInfo;

typedef struct GPIOPINInfo {
  IFPINS_PinType_e   pinType;
  IFPINS_PullState_e pullState;
} GPIOPINInfo_t;

typedef struct GPIOParameters {
  GPIOPINInfo_t pin[NUMBER_OF_IF_PINS];
} GPIOParameters_t;

typedef struct GPIOInfo {
  uint8_t portNumber[NUMBER_OF_IF_PINS];
  uint8_t pinNumber[NUMBER_OF_IF_PINS];
  void (*init)(const struct UEXTInfo *uext, GPIOParameters_t * parameters);
  uint8_t (*readBit)(const struct GPIOInfo *gpio, IFPINS_e pin);
  void (*writeBit)(const struct GPIOInfo *gpio, IFPINS_e pin, uint8_t state);
  void (*toggleBit)(const struct GPIOInfo *gpio, IFPINS_e pin);
  uint32_t (*getPortBase)(uint8_t portNumber);
} GPIOInfo_t;

#endif /* __GPIO_H */
