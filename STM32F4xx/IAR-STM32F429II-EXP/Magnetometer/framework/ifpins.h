/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : ifpins.h
 *    Description : Interface Pins (IFPINS) object definitions
 *
 *    History :
 *    1. Date        : 03, June 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __IFPINS_H
#define __IFPINS_H

#include <stdint.h>

typedef enum {
  pinTX,
  pinRX,
  pinSCL,
  pinSDA,
  pinMISO,
  pinMOSI,
  pinSCK,
  pinSSEL,
  NUMBER_OF_IF_PINS
} IFPINS_e;

typedef enum {
  psNORMAL,
  psPULLUP,
  psPULLDN
} IFPINS_PullState_e;

typedef enum {
  ptINPUT,
  ptOUTPUT,
  ptFUNC
} IFPINS_PinType_e;

#endif /* __IFPINS_H */
