/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : mod_joystick.c
 *    Description : MOD-JOYSTICK Library
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "mod_joystick.h"

#define MOD_JOYSTICK_ADDRESS           0x52<<1

/*** Joystick instance parameters ***/

MODInfo_t MOD_JOYSTICK_Instance = {
  .hasUART = 0,
  .hasI2C  = 1,
  .hasSPI  = 0  
};

MODInfo_t * MOD_JOYSTICK = &MOD_JOYSTICK_Instance;

GPIOParameters_t mod_joystick_gpiopar = {{
{ ptINPUT,  psNORMAL },
{ ptINPUT,  psNORMAL },
{ ptFUNC,   psNORMAL },
{ ptFUNC,   psNORMAL },
{ ptINPUT,  psNORMAL },
{ ptINPUT,  psNORMAL },
{ ptINPUT,  psNORMAL },
{ ptINPUT,  psNORMAL }
}};

I2CParameters_t mod_joystick_i2cPar    = { 100000 };  /* I2C Speed = 100kHz*/

ParametersGroupInfo_t MOD_JOYSTICK_PG  = { 
&mod_joystick_gpiopar,
#if defined __UEXT_UART__
NULL,
#endif
#if defined __UEXT_I2C__
&mod_joystick_i2cPar,
#endif
#if defined __UEXT_SPI__
NULL
#endif
};

/*************************************************************************
 * Function Name: MOD_JOYSTICK_readData
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint8_t address - start address of the data to be read
 *                uint8_t * pBuffer - buffer for storing the readed values
 *                uint32_t length - number of bytes to read
 * Return:        uint32_t - result of operation
 *                MOD_JOYSTICK_RESULT_OK - operation was ok
 *                MOD_JOYSTICK_RESULT_COMM_ERROR - communication error occured
 *
 * Description: Reads "length" bytes of data from the joystick memory
 *************************************************************************/
uint32_t MOD_JOYSTICK_readData(ConnectionHandler_t * conn, uint8_t address, uint8_t * pBuffer, uint32_t length)
{
uint8_t * pAddress = &address;

  if (conn->uext->I2C->send(conn->uext->I2C, MOD_JOYSTICK_ADDRESS, pAddress, 1, I2C_SEND_STOP) != I2C_RESULT_OK) return MOD_JOYSTICK_RESULT_COMM_ERROR;
  if (conn->uext->I2C->receive(conn->uext->I2C, MOD_JOYSTICK_ADDRESS, pBuffer, length, I2C_SEND_STOP, I2C_CHECK_BUSY) != I2C_RESULT_OK) return MOD_JOYSTICK_RESULT_COMM_ERROR;
  return MOD_JOYSTICK_RESULT_OK;
}

/*************************************************************************
 * Function Name: MOD_JOYSTICK_writeData
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                uint8_t address - start address where data should be written
 *                uint8_t * pBuffer - buffer to write
 *                uint32_t length - number of bytes to read
 * Return:        uint32_t - result of operation
 *                MOD_JOYSTICK_RESULT_OK - operation was ok
 *                MOD_JOYSTICK_RESULT_COMM_ERROR - communication error occured
 *
 * Description: Writes "length" bytes to the joystick memory
 *************************************************************************/
uint32_t MOD_JOYSTICK_writeData(ConnectionHandler_t * conn, uint8_t address, uint8_t * pBuffer, uint32_t length)
{
uint32_t res;
uint8_t * pWrBuffer = malloc(length+1);

  if (pWrBuffer == NULL) return MOD_JOYSTICK_RESULT_MEM_ERROR;
  *pWrBuffer = address;
  memcpy(pWrBuffer+1, pBuffer, length);
  res = conn->uext->I2C->send(conn->uext->I2C, MOD_JOYSTICK_ADDRESS, pWrBuffer, length+1, I2C_SEND_STOP);
  free(pWrBuffer);
  if (res != I2C_RESULT_OK) return MOD_JOYSTICK_RESULT_COMM_ERROR;
  return MOD_JOYSTICK_RESULT_OK;
}

/*************************************************************************
 * Function Name: MOD_JOYSTICK_init
 * Parameters:    ConnectionHandler_t *conn - connection handler
 * Return:        uint32_t - result of operation
 *                MOD_JOYSTICK_RESULT_OK - operation was ok
 *                MOD_JOYSTICK_RESULT_COMM_ERROR - communication error occured
 *
 * Description: Initializes the joystick controller
 *************************************************************************/
uint32_t MOD_JOYSTICK_init(ConnectionHandler_t * conn)
{
uint8_t buffer[1];
uint32_t result;

  buffer[0] = 0x55;
  result = MOD_JOYSTICK_writeData(conn, 0xF0, buffer, 1);
  if (result != MOD_JOYSTICK_RESULT_OK) return result;

  buffer[0] = 0x00;
  result = MOD_JOYSTICK_writeData(conn, 0xFB, buffer, 1);
  if (result != MOD_JOYSTICK_RESULT_OK) return result;

  return MOD_JOYSTICK_RESULT_OK;
}

/*************************************************************************
 * Function Name: MOD_JOYSTICK_readJoystick()
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                MOD_JOYSTICK_Data_t * jData - joystick's data structure
 * Return:        uint32_t - result of operation
 *                MOD_JOYSTICK_RESULT_OK - operation was ok
 *                MOD_JOYSTICK_RESULT_COMM_ERROR - communication error occured
 *
 * Description: Reads the joystick.
 *************************************************************************/
uint32_t MOD_JOYSTICK_readJoystick(ConnectionHandler_t * conn, MOD_JOYSTICK_Data_t * jData)
{
uint8_t buffer[6];
uint8_t timeout = MOD_JOYSTICK_READ_TIMEOUT;

  /* Read joystick */
  while(MOD_JOYSTICK_readData(conn, 0x00, buffer, 6) != MOD_JOYSTICK_RESULT_OK)
  {
    if (timeout) timeout--;
    else return MOD_JOYSTICK_RESULT_COMM_ERROR;
  }

  if ((buffer[0] == 0xFF) && (buffer[1] == 0xFF) && (buffer[2] == 0xFF) && (buffer[3] == 0xFF) && (buffer[4] == 0xFF))
  {
    return MOD_JOYSTICK_RESULT_HW_ERROR;
  }

  if (jData)
  {
    /* Decode joystick's position */
    jData->StickX = buffer[0];
    jData->StickY = buffer[1];

    /* Decode joystick's accellerometer */
    jData->AccelX = (((uint16_t)buffer[2]) << 2) | ((buffer[5]>>2) & 0x03);
    jData->AccelY = (((uint16_t)buffer[3]) << 2) | ((buffer[5]>>4) & 0x03);
    jData->AccelZ = (((uint16_t)buffer[4]) << 2) | ((buffer[5]>>6) & 0x03);

    /* Decode joystick's buttons */
    jData->ButtonZ = !(buffer[5] & 0x01); /* Z */
    jData->ButtonC = !(buffer[5] & 0x02); /* C */
  }
  return MOD_JOYSTICK_RESULT_OK;
}
