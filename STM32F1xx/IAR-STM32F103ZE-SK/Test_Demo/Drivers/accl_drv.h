/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : accl_drv.h
 *    Description : Accelerometer sensor LIS3L020 driver include file
 *
 *    History :
 *    1. Date        : July 24, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 4262 $
 **************************************************************************/
#include "includes.h"

#ifndef __ACCL_DRV_H
#define __ACCL_DRV_H

#ifdef  ACCL_DRV_GLOBAL
#define ACCL_DRV_EXTERN
#else
#define ACCL_DRV_EXTERN  extern
#endif

#define ACC_SENSOR_ADDR     0x3A

/*************************************************************************
 * Function Name: Accl_Init
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Init Accelerometer sensor
 *
 *************************************************************************/
Boolean Accl_Init (void);

/*************************************************************************
 * Function Name: Accl_Get
 * Parameters: ps16 pX, ps16 pY, ps16 pZ
 *
 * Return: Boolean
 *
 * Description: Read X,Y,Z data
 *
 *************************************************************************/
Boolean Accl_Get (ps16 pX, ps16 pY, ps16 pZ);

#endif // __ACCL_DRV_H
