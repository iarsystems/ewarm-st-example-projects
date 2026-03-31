/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : temp_sensor_drv.h
 *    Description : Temperature sensor STCN75 driver include file
 *
 *    History :
 *    1. Date        : July 28, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    2. Date        : May, 2015
 *       Author      : Atanas Uzunov
 *       Description : Adapted for STM32F7xx HAL libraries
 *
 *    $Revision: #1 $
 **************************************************************************/
#ifndef __TEMP_SENSOR_DRV_H
#define __TEMP_SENSOR_DRV_H

#define TEMP_SENSOR_SENSOR_ADDR     (0x4B<<1)
#define TEMP_SENSOR_COMP_MODE       FALSE
#define TEMP_SENSOR_INTR_MODE       TRUE

/*************************************************************************
 * Function Name: TempSensor_Init
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Init Temperature sensor
 *
 *************************************************************************/
Boolean TempSensor_Init(I2C_HandleTypeDef * hi2c);

/*************************************************************************
 * Function Name: TempSensorShutdown
 * Parameters: Boolean Shutdown
 *
 * Return: Boolean
 *
 * Description: Enable/Disable sensor shutdown
 *
 *************************************************************************/
Boolean TempSensorShutdown(I2C_HandleTypeDef * hi2c, Boolean Shutdown);

/*************************************************************************
 * Function Name: TempSensor_Conf
 * Parameters: Flo32 TOS, Flo32 THYS, Boolean Mode, Int32U FaultTol
 *
 * Return: Boolean
 *
 * Description: Temperature sensor config
 *
 *************************************************************************/
Boolean TempSensor_Conf(I2C_HandleTypeDef * hi2c, Flo32 TOS, Flo32 THYS, Boolean Mode,
                        Int32U FaultTol);

/*************************************************************************
 * Function Name: TempSensorGetTemp
 * Parameters: pFlo32 pTemp, pBoolean pAlarm
 *
 * Return: Boolean
 *
 * Description: Temperature sensor get temperature and alarm state
 *
 *************************************************************************/
Boolean TempSensorGetTemp(I2C_HandleTypeDef * hi2c, pFlo32 pTemp, pBoolean pAlarm);

#endif // __TEMP_SENSOR_DRV_H
