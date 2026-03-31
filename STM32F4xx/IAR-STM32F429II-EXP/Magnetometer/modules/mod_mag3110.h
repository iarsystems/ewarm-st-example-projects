/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : mod_mag3110.h
 *    Description : MOD-MAG3110 library header
 *
 *    History :
 *    1. Date        : 30, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 2662 $
 **************************************************************************/

#ifndef __MOD_MAG3110_H
#define __MOD_MAG3110_H

#include "../framework/font.h"
#include "../framework/bitmap.h"
#include "../framework/connect.h"

#define MOD_MAG3110_RESULT_OK         0
#define MOD_MAG3110_RESULT_COMM_ERROR 1
#define MOD_MAG3110_RESULT_MEM_ERROR  2

#define MOD_MAG3110_DR_STATUS    0x00
#define MOD_MAG3110_CTRL_REG1    0x10
#define MOD_MAG3110_CTRL_REG2    0x11

#define MOD_MAG3110_DR_STATUS_ZYXDR         0x08
#define MOD_MAG3110_CTRL_REG2_AUTO_MRST_EN  0x80

#define MOD_MAG3110_CTRL_REG1_DR_MASK       0x7
#define MOD_MAG3110_CTRL_REG1_DR_SHIFT      5

#define MOD_MAG3110_CTRL_REG1_OS_MASK       0x3
#define MOD_MAG3110_CTRL_REG1_OS_SHIFT      3

#define MOD_MAG3110_CTRL_REG1_AC            0x01
#define MOD_MAG3110_CTRL_REG1_TM            0x02
#define MOD_MAG3110_CTRL_REG1_FR            0x04

#define MOD_MAG3110_ADDR   0x0E<<1

typedef enum
{
  os16,
  os32,
  os64,
  os128,
  NUMBER_OF_MAG_OS
} MOD_MAG3110_OS_e;

typedef struct MOD_MAG3110_Data {
  int16_t  X;
  int16_t  Y;
  int16_t  Z;
  int8_t   T;
} MOD_MAG3110_Data_t;

extern MODInfo_t * MOD_MAG3110;

extern ParametersGroupInfo_t MOD_MAG3110_PG;

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
uint32_t MOD_MAG3110_readData(ConnectionHandler_t * conn, uint8_t addr, uint8_t * buffer, uint32_t len);

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
uint32_t MOD_MAG3110_writeData(ConnectionHandler_t * conn, uint8_t addr, uint8_t * buffer, uint32_t len);

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
uint8_t MOD_MAG3110_isInt(ConnectionHandler_t * conn);

/*************************************************************************
 * Function Name: MOD_MAG3110_isInt
 * Parameters:    ConnectionHandler_t * conn - connection handler
 *                uint8_t * status - device status
 * Return:        uint8_t - operation result
 *                1 - interrupt pending
 *                0 - no interrupt occured
 *
 * Description: Read MOD-MAG3110 DR_STATUS register.
 *************************************************************************/
uint8_t MOD_MAG3110_getStatus(ConnectionHandler_t * conn, uint8_t * status);

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
uint32_t MOD_MAG3110_getXYZ(ConnectionHandler_t * conn, MOD_MAG3110_Data_t * magData);

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
uint32_t MOD_MAG3110_init(ConnectionHandler_t * conn, uint8_t dr, MOD_MAG3110_OS_e os);

#endif /* __MOD_MAG3110_H */
