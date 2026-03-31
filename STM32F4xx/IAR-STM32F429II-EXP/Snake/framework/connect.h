/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : connect.h
 *    Description : UEXT to MOD connect utility header
 *
 *    History :
 *    1. Date        : 31, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __CONNECT_H
#define __CONNECT_H

#include "uext.h"
#include "mod.h"

typedef struct ConnectionHandler {
  const MODInfo_t  * mod;
  const UEXTInfo_t * uext;
              void * aux;
} ConnectionHandler_t;

typedef struct ParametersGroupInfo {
  GPIOParameters_t  * gpioParameters;
#if defined __UEXT_UART__
  UARTParameters_t * uartParameters;
#endif
#if defined __UEXT_I2C__
  I2CParameters_t * i2cParameters;
#endif  
#if defined __UEXT_SPI__
  SPIParameters_t * spiParameters;
#endif
} ParametersGroupInfo_t;

/*************************************************************************
 * Function Name: ConnectModule
 * Parameters:    const MODInfo_t * mod - pointer to the MOD object
 *                const UEXTInfo_t * uext - pointer to the UEXT object
 *                ParametersGroupInfo_t * parGroup - Parameters Group Object
 * Return:        ConnectionHandler_t - produced connection handler on success
 *                NULL - if an error occured
 *
 * Description: Connects a MOD object with an UEXT object.
 *************************************************************************/
ConnectionHandler_t * ConnectModule(const MODInfo_t * mod, const UEXTInfo_t * uext, ParametersGroupInfo_t * parGroup);

#endif /* __CONNECT_H */
