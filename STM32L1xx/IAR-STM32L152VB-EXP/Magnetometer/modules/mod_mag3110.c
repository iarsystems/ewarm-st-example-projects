/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : mod_mag3110.c
 *    Description : MOD-MAG3110 library
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 2662 $
 **************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "mod_mag3110.h"

/* MOD_MAG3110 Instance parameters */

MODInfo_t MOD_MAG3110_Instance = {
  .hasUART = 0,
  .hasI2C  = 1,
  .hasSPI  = 0
};

MODInfo_t * MOD_MAG3110  = &MOD_MAG3110_Instance;

GPIOParameters_t mod_mag3110_gpiopar = {{
{ ptINPUT,  psNORMAL },
{ ptINPUT,  psPULLDN },
{ ptFUNC,   psNORMAL },
{ ptFUNC,   psNORMAL },
{ ptINPUT,  psNORMAL },
{ ptINPUT,  psNORMAL },
{ ptINPUT,  psNORMAL },
{ ptINPUT,  psNORMAL }
}};

I2CParameters_t mod_mag3110_i2cpar   = { 400000 };

ParametersGroupInfo_t MOD_MAG3110_PG = {
&mod_mag3110_gpiopar,
#if defined __UEXT_UART__
NULL,
#endif
#if defined __UEXT_I2C__
&mod_mag3110_i2cpar,
#endif
#if defined __UEXT_SPI__
NULL
#endif
};

/*************************************************************************
 * Function Name: MOD_MAG3110_readData
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t addr - start address
 *                uint8_t * buffer - read buffer
 *                uint32_t len - count of bytes to be read
 * Return:        uint32_t - operation result
 *                MOD_MAG3110_RESULT_OK - operation completed successfully
 *                MOD_MAG3110_RESULT_COMM_ERROR - communication error occured
 *
 * Description: Read "len" bytes of data from MAG3110 memory to "buffer",
 *              starting at address "addr".
 *************************************************************************/
uint32_t MOD_MAG3110_readData(ConnectionHandler_t * conn, uint8_t addr, uint8_t * buffer, uint32_t len)
{
uint8_t * addrbuf = &addr;
  if (conn->uext->I2C->send(conn->uext->I2C, MOD_MAG3110_ADDR, addrbuf, 1, I2C_SKIP_STOP) != I2C_RESULT_OK) return MOD_MAG3110_RESULT_COMM_ERROR;
  if (conn->uext->I2C->receive(conn->uext->I2C, MOD_MAG3110_ADDR, buffer, len, I2C_SEND_STOP, I2C_SKIP_BUSY) != I2C_RESULT_OK) return MOD_MAG3110_RESULT_COMM_ERROR;
  return MOD_MAG3110_RESULT_OK;
}

/*************************************************************************
 * Function Name: MOD_MAG3110_writeData
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t addr - start address
 *                uint8_t * buffer - data to write
 *                uint32_t len - count of bytes to be written
 * Return:        uint32_t - operation result
 *                MOD_MAG3110_RESULT_OK - operation completed successfully
 *                MOD_MAG3110_RESULT_COMM_ERROR - communication error occured
 *
 * Description: Write "len" bytes of data from "buffer" to MAG3110 memory,
 *              starting at address "addr".
 *************************************************************************/
uint32_t MOD_MAG3110_writeData(ConnectionHandler_t * conn, uint8_t addr, uint8_t * buffer, uint32_t len)
{
uint32_t res;
uint8_t * addrbuf = malloc(len+1);
  if (addrbuf == NULL) return MOD_MAG3110_RESULT_MEM_ERROR;
  *addrbuf = addr;
  memcpy(addrbuf+1, buffer, len);
  res = conn->uext->I2C->send(conn->uext->I2C, MOD_MAG3110_ADDR, addrbuf, len+1, I2C_SEND_STOP);
  free(addrbuf);
  if (res != I2C_RESULT_OK) return MOD_MAG3110_RESULT_COMM_ERROR;
  return MOD_MAG3110_RESULT_OK;
}

/*************************************************************************
 * Function Name: MOD_MAG3110_isInt
 * Parameters:    ConnectionHandler_t * conn - connection handler
 * Return:        uint8_t - operation result
 *                1 - interrupt pending
 *                0 - no interrupt occured
 *
 * Description: Check if there is pending interrupt from the MOD-MAG3110
 *              device.
 *************************************************************************/
uint8_t MOD_MAG3110_isInt(ConnectionHandler_t * conn)
{
  return conn->uext->GPIO->readBit(conn->uext->GPIO, pinRX);
}

/*************************************************************************
 * Function Name: MOD_MAG3110_getStatus
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t * status - device status
 * Return:        uint32_t - operation result
 *                MOD_MAG3110_RESULT_OK - operation completed successfully
 *                MOD_MAG3110_RESULT_COMM_ERROR - communication error occured
 *
 * Description: Read MOD-MAG3110 DR_STATUS register.
 *************************************************************************/
uint8_t MOD_MAG3110_getStatus(ConnectionHandler_t * conn, uint8_t * status)
{
uint8_t buf[1];
uint32_t res; 

  res = MOD_MAG3110_readData(conn, MOD_MAG3110_DR_STATUS, buf, 1);
  if(res != MOD_MAG3110_RESULT_OK) return res;
  else
  {
    * status = buf[0];
  }
  return MOD_MAG3110_RESULT_OK;
}

/*************************************************************************
 * Function Name: MOD_MAG3110_getXYZ
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                MOD_MAG3110_Data_t * magData - structure to store results
 * Return:        uint32_t - operation result
 *                MOD_MAG3110_RESULT_OK - operation completed successfully
 *                MOD_MAG3110_RESULT_COMM_ERROR - communication error occured
 *
 * Description: Read MOD-MAG3110 measured values into "magData" structure.
 *************************************************************************/
uint32_t MOD_MAG3110_getXYZ(ConnectionHandler_t * conn, MOD_MAG3110_Data_t * magData)
{
uint8_t buf[16];
uint32_t res; 

  res = MOD_MAG3110_readData(conn, 0, buf, 16);
  if(res != MOD_MAG3110_RESULT_OK) return res;
  else
  {
    magData->X = (buf[1]<<8 | buf[2]);
    magData->Y = (buf[3]<<8 | buf[4]);
    magData->Z = (buf[5]<<8 | buf[6]);
    magData->T = (int8_t)buf[15];
  }
  return MOD_MAG3110_RESULT_OK;
}

/*************************************************************************
 * Function Name: MOD_MAG3110_init
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t dr - data rate
 *                MOD_MAG3110_OS_e os - over-sampling ratio
 * Return:        uint32_t - operation result
 *                MOD_MAG3110_RESULT_OK - operation completed successfully
 *                MOD_MAG3110_RESULT_COMM_ERROR - communication error occured
 *
 * Description: Initialize the MOD-MAG3110 device with the given os ratio.
 *************************************************************************/
uint32_t MOD_MAG3110_init(ConnectionHandler_t * conn, uint8_t dr, MOD_MAG3110_OS_e os)
{
uint8_t buffer[6];

  if(MOD_MAG3110_readData(conn, MOD_MAG3110_CTRL_REG1, buffer, 1) != MOD_MAG3110_RESULT_OK) return MOD_MAG3110_RESULT_COMM_ERROR;
  buffer[0] &= ~MOD_MAG3110_CTRL_REG1_AC;
  if(MOD_MAG3110_writeData(conn, MOD_MAG3110_CTRL_REG1, buffer, 1) != MOD_MAG3110_RESULT_OK) return MOD_MAG3110_RESULT_COMM_ERROR;

  if(MOD_MAG3110_readData(conn, MOD_MAG3110_CTRL_REG2, buffer, 1) != MOD_MAG3110_RESULT_OK) return MOD_MAG3110_RESULT_COMM_ERROR;
  buffer[0] |= MOD_MAG3110_CTRL_REG2_AUTO_MRST_EN;
  if(MOD_MAG3110_writeData(conn, MOD_MAG3110_CTRL_REG2, buffer, 1) != MOD_MAG3110_RESULT_OK) return MOD_MAG3110_RESULT_COMM_ERROR;

  if(MOD_MAG3110_readData(conn, MOD_MAG3110_CTRL_REG1, buffer, 1) != MOD_MAG3110_RESULT_OK) return MOD_MAG3110_RESULT_COMM_ERROR;
  buffer[0] &= ~(MOD_MAG3110_CTRL_REG1_DR_MASK << MOD_MAG3110_CTRL_REG1_DR_SHIFT);
  buffer[0] &= ~(MOD_MAG3110_CTRL_REG1_OS_MASK << MOD_MAG3110_CTRL_REG1_OS_SHIFT);
  buffer[0] |=  (MOD_MAG3110_CTRL_REG1_DR_MASK & dr) << MOD_MAG3110_CTRL_REG1_DR_SHIFT;
  buffer[0] |=  (MOD_MAG3110_CTRL_REG1_OS_MASK & os) << MOD_MAG3110_CTRL_REG1_OS_SHIFT;
  buffer[0] |=   MOD_MAG3110_CTRL_REG1_AC;
  if(MOD_MAG3110_writeData(conn, MOD_MAG3110_CTRL_REG1, buffer, 1) != MOD_MAG3110_RESULT_OK) return MOD_MAG3110_RESULT_COMM_ERROR;
  /* Dummy read of XYZ to clear int/ready flags */
  MOD_MAG3110_readData(conn, 0x01, buffer, 6);
  return MOD_MAG3110_RESULT_OK;
}

