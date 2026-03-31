/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : lsm6ds3_drv.h
 *    Description : Accelerometer and Gyroscope sensor LSM6DS3 driver include file
 *
 *    History :
 *    1. Date        : May, 2015
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: #1 $
 **************************************************************************/
#ifndef __LSM6DS3_DRV_H
#define __LSM6DS3_DRV_H

#define ACC_GYRO_SENSOR_ADDR     (0x6B<<1)

#define CTRL1_ODR_PD             (0x0 << 4)
#define CTRL1_ODR_13HZ           (0x1 << 4)
#define CTRL1_ODR_26HZ           (0x2 << 4)
#define CTRL1_ODR_52HZ           (0x3 << 4)
#define CTRL1_ODR_104HZ          (0x4 << 4)
#define CTRL1_ODR_208HZ          (0x5 << 4)
#define CTRL1_ODR_416HZ          (0x6 << 4)
#define CTRL1_ODR_833HZ          (0x7 << 4)
#define CTRL1_ODR_1660HZ         (0x8 << 4)
#define CTRL1_ODR_3330HZ         (0x9 << 4)
#define CTRL1_ODR_6660HZ         (0xA << 4)

#define CTRL1_FS_2g              (0x0 << 2)
#define CTRL1_FS_16g             (0x1 << 2)
#define CTRL1_FS_4g              (0x2 << 2)
#define CTRL1_FS_8g              (0x4 << 2)

#define CTRL1_BW_400HZ           (0x0 << 0)
#define CTRL1_BW_200HZ           (0x1 << 0)
#define CTRL1_BW_100HZ           (0x2 << 0)
#define CTRL1_BW_50HZ            (0x3 << 0)

/*************************************************************************
 * Function Name: LSM6DS3_Init
 * Parameters: I2C_HandleTypeDef * hi2c
 *
 * Return: uint32_t
 *
 * Description: Init Accelerometer and Gyroscope sensor
 *************************************************************************/
uint32_t LSM6DS3_Init(I2C_HandleTypeDef * hi2c, uint8_t ctrl1_xl);

/*************************************************************************
 * Function Name: LSM6DS3_Get
 * Parameters: I2C_HandleTypeDef * hi2c, int16_t * pX, int16_t * pY, int16_t * pZ
 *
 * Return: uint32_t
 *
 * Description: Read X,Y,Z - Acc and Gyro data, and Temp
 *************************************************************************/
uint32_t LSM6DS3_Get(I2C_HandleTypeDef * hi2c, int16_t * pAX, int16_t * pAY, int16_t * pAZ, int16_t * pGX, int16_t * pGY, int16_t * pGZ, int16_t * pTemp);

/*************************************************************************
 * Function Name: LSM6DS3_AccReady
 * Parameters: none
 *
 * Return: uint32_t
 *
 * Description: Check if AccReady INT is set
 *************************************************************************/
uint32_t LSM6DS3_AccReady(void);

/*************************************************************************
 * Function Name: LSM6DS3_GyroReady
 * Parameters: none
 *
 * Return: uint32_t
 *
 * Description: Check if GyroReady INT is set
 *************************************************************************/
uint32_t LSM6DS3_GyroReady(void);

#endif // __LSM6DS3_DRV_H
