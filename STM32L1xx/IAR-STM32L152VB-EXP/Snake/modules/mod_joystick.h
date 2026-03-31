/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : mod_joystick.h
 *    Description : MOD-JOYSTICK Library header
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __MOD_JOYSTICK_H
#define __MOD_JOYSTICK_H

#include "../framework/connect.h"
#include "../framework/mod.h"
#include "../framework/uext.h"

#define MOD_JOYSTICK_RESULT_OK         0
#define MOD_JOYSTICK_RESULT_COMM_ERROR 1
#define MOD_JOYSTICK_RESULT_MEM_ERROR  2
#define MOD_JOYSTICK_RESULT_HW_ERROR   3

#define MOD_JOYSTICK_ADDRESS           0x52<<1

#define MOD_JOYSTICK_READ_TIMEOUT      100

typedef struct {
  uint8_t StickX;
  uint8_t StickY;
  uint16_t AccelX;
  uint16_t AccelY;
  uint16_t AccelZ;
  uint8_t ButtonZ;
  uint8_t ButtonC;
} MOD_JOYSTICK_Data_t;

extern MODInfo_t * MOD_JOYSTICK;

extern ParametersGroupInfo_t MOD_JOYSTICK_PG;

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
uint32_t MOD_JOYSTICK_readData(ConnectionHandler_t * conn, uint8_t address, uint8_t * pBuffer, uint32_t length);

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
uint32_t MOD_JOYSTICK_writeData(ConnectionHandler_t * conn, uint8_t address, uint8_t * pBuffer, uint32_t length);

/*************************************************************************
 * Function Name: MOD_JOYSTICK_init
 * Parameters:    ConnectionHandler_t *conn - connection handler
 * Return:        uint32_t - result of operation
 *                MOD_JOYSTICK_RESULT_OK - operation was ok
 *                MOD_JOYSTICK_RESULT_COMM_ERROR - communication error occured
 *
 * Description: Initialize the joystick controller
 *************************************************************************/
uint32_t MOD_JOYSTICK_init(ConnectionHandler_t * conn);

/*************************************************************************
 * Function Name: MOD_JOYSTICK_readJoystick
 * Parameters:    ConnectionHandler_t *conn - connection handler
 *                MOD_JOYSTICK_Data_t * jData - joystick's data structure
 * Return:        uint32_t - result of operation
 *                MOD_JOYSTICK_RESULT_OK - operation was ok
 *                MOD_JOYSTICK_RESULT_COMM_ERROR - communication error occured
 *
 * Description: Reads the joystick.
 *************************************************************************/
uint32_t MOD_JOYSTICK_readJoystick(ConnectionHandler_t * conn, MOD_JOYSTICK_Data_t * jData);

#endif
