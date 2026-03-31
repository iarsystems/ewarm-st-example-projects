/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : i2c2_drv.h
 *    Description : I2C2 Master mode driver include file
 *
 *    History :
 *    1. Date        : September 8, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 4202 $
 **************************************************************************/
#ifndef __I2C2_DRV_H
#define __I2C2_DRV_H

#ifdef  I2C2_DRV_GLOBAL
#define I2C2_DRV_EXTERN
#else
#define I2C2_DRV_EXTERN  extern
#endif

#define I2C2_SPEED          400000
#define I2C2_INTR_SUBPRIO   1
#define I2C2_INTR_PRIO      0

/*************************************************************************
 * Function Name: I2C2_ErrIntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: I2C2 error interrupt handler
 *
 *************************************************************************/
void I2C2_ErrIntrHandler(void);

/*************************************************************************
 * Function Name: I2C2_EvnIntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: I2C2 event interrupt handler
 *
 *************************************************************************/
void I2C2_EvnIntrHandler(void);

/*************************************************************************
 * Function Name: I2C2_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init I2C2 interface
 *
 *************************************************************************/
void I2C2_Init (void);

/*************************************************************************
 * Function Name: I2C2_Open
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Init I2C2 interface open
 *
 *************************************************************************/
Boolean I2C2_Open (void);

/*************************************************************************
 * Function Name: I2C2_Close
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init I2C2 interface release
 *
 *************************************************************************/
void I2C2_Close (void);

/*************************************************************************
 * Function Name: I2C2_DataTransfer
 * Parameters: Int8U SlaveAddr, pInt8U pData, Int32U Size
 *
 * Return: none
 *
 * Description: I2C2 transfer data to/from slave
 *
 *************************************************************************/
Boolean I2C2_DataTransfer (Int8U SlaveAddr, pInt8U pData, Int32U Size);

#endif // __I2C1_DRV_H
