/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : connect.c
 *    Description : UEXT to MOD connect utility
 *
 *    History :
 *    1. Date        : 31, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include <stdlib.h>
#include <stdint.h>

#include "uext.h"
#include "mod.h"
#include "connect.h"

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
ConnectionHandler_t * ConnectModule(const MODInfo_t * mod, const UEXTInfo_t * uext, ParametersGroupInfo_t * parGroup)
{
  ConnectionHandler_t * conn;
  conn = malloc(sizeof(ConnectionHandler_t));
  if (conn != NULL)
  {
    conn->mod = mod;
    conn->uext = uext;
    conn->uext->GPIO->init(conn->uext, parGroup->gpioParameters);
#if defined __UEXT_UART__
    if (mod->hasUART) conn->uext->UART->init(conn->uext->UART, par->uartParameters);
#endif
#if defined __UEXT_I2C__
    if (mod->hasI2C) conn->uext->I2C->init(conn->uext->I2C, parGroup->i2cParameters);
#endif
#if defined __UEXT_SPI__
    if (mod->hasSPI) conn->uext->SPI->init(conn->uext->SPI, parGroup->spiParameters);
#endif
  }
  return conn;
}
