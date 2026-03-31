/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : lsm6ds3_drv.c
 *    Description : Accelerometer and Gyroscope sensor LSM6DS3 driver source file
 *
 *    History :
 *    1. Date        : May, 2015
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: #1 $
 **************************************************************************/
#include "arm_comm.h"
#include "iar_stm32f746xx_sk.h"
#include "lsm6ds3_drv.h"

typedef enum _LSM6DS3_regs_t
{
  FUNC_CFG_ACCESS = 0x01,
  SENSOR_SYNC_TIME_FRAME = 0x04,
  FIFO_CTRL1 = 0x06, FIFO_CTRL2, FIFO_CTRL3, FIFO_CTRL4, FIFO_CTRL5,
  ORIENT_CFG_G,
  INT1_CTRL = 0x0D, INT2_CTRL,
  WHO_AM_I,
  CTRL1_XL, CTRL2_G, CTRL3_C, CTRL4_C, CTRL5_C, CTRL6_C, CTRL7_G, CTRL8_XL,
  CTRL9_XL, CTRL10_C, MASTER_CONFIG, WAKE_UP_SRC, TAP_SRC, D6D_SRC,
  STATUS_REG,
  OUT_TEMP_L = 0x20, OUT_TEMP_H, OUTX_L_G, OUTX_H_G, OUTY_L_G, OUTY_H_G,
  OUTZ_L_G, OUTZ_H_G, OUTX_L_XL, OUTX_H_XL, OUTY_L_XL, OUTY_H_XL, OUTZ_L_XL,
  OUTZ_H_XL, SENSORHUB1_REG, SENSORHUB2_REG, SENSORHUB3_REG, SENSORHUB4_REG,
  SENSORHUB5_REG, SENSORHUB6_REG, SENSORHUB7_REG, SENSORHUB8_REG,
  SENSORHUB9_REG, SENSORHUB10_REG, SENSORHUB11_REG, SENSORHUB12_REG,
  FIFO_STATUS1, FIFO_STATUS2, FIFO_STATUS3, FIFO_STATUS4,
  FIFO_DATA_OUT_L, FIFO_DATA_OUT_H, TIMESTAMP0_REG, TIMESTAMP1_REG,
  TIMESTAMP2_REG,
  STEP_TIMESTAMP_L = 0x49, STEP_TIMESTAMP_H, STEP_COUNTER_L, STEP_COUNTER_H,
  SENSORHUB13_REG, SENSORHUB14_REG, SENSORHUB15_REG, SENSORHUB16_REG,
  SENSORHUB17_REG, SENSORHUB18_REG, FUNC_SRC,
  TAP_CFG = 0x58, TAP_THS_6D, INT_DUR2, WAKE_UP_THS, WAKE_UP_DUR, FREE_FALL,
  MD1_CFG, MD2_CFG,
  OUT_MAG_RAW_X_L = 0x66, OUT_MAG_RAW_X_H, OUT_MAG_RAW_Y_L, OUT_MAG_RAW_Y_H,
  OUT_MAG_RAW_Z_L, OUT_MAG_RAW_Z_H
} LSM6DS3_regs_t;

/*************************************************************************
 * Function Name: LSM6DS3_Init
 * Parameters: I2C_HandleTypeDef * hi2c
 *
 * Return: uint32_t
 *
 * Description: Init Accelerometer and Gyroscope sensor
 *************************************************************************/
uint32_t LSM6DS3_Init(I2C_HandleTypeDef * hi2c, uint8_t ctrl1_xl)
{
GPIO_InitTypeDef GPIO_InitStructure;
uint8_t Data[2];

  // Enable ACC_GYRO_INT1 port
  ACC_GYRO_INT1_CLK_ENABLE();
  GPIO_InitStructure.Pin = ACC_GYRO_INT1_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(ACC_GYRO_INT1_PORT, &GPIO_InitStructure);

  // Enable ACC_GYRO_INT2 port
  ACC_GYRO_INT2_CLK_ENABLE();
  GPIO_InitStructure.Pin = ACC_GYRO_INT2_PIN;
  HAL_GPIO_Init(ACC_GYRO_INT2_PORT, &GPIO_InitStructure);

  if(!hi2c)
  {
    return(FALSE);
  }

  // Get LSM6DS3 sensor ID
  Data[0] = WHO_AM_I;
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, ACC_GYRO_SENSOR_ADDR, Data, 1, 2000))
  {
    return(FALSE);
  }
  if(HAL_OK != HAL_I2C_Master_Receive(hi2c, ACC_GYRO_SENSOR_ADDR, Data, 1, 2000))
  {
    return(FALSE);
  }
  if(0x69 != Data[0])
  {
    return(FALSE);
  }

  // Sensor configure

  Data[0] = CTRL3_C;
  Data[1] = 0x01; // Software reset
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, ACC_GYRO_SENSOR_ADDR, Data, 2, 2000))
  {
    return(FALSE);
  }

  HAL_Delay(100);

  Data[0] = CTRL3_C;
  Data[1] = 0x44; // Enable BDU (block data update) and IF_INC (serial addr auto incr.)
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, ACC_GYRO_SENSOR_ADDR, Data, 2, 2000))
  {
    return(FALSE);
  }

  Data[0] = INT1_CTRL;
  Data[1] = 0x01; // enable accelerometer data ready int on INT1
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, ACC_GYRO_SENSOR_ADDR, Data, 2, 2000))
  {
    return(FALSE);
  }

  Data[0] = INT2_CTRL;
  Data[1] = 0x02; // enable gyroscope data ready int on INT2
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, ACC_GYRO_SENSOR_ADDR, Data, 2, 2000))
  {
    return(FALSE);
  }

  Data[0] = CTRL1_XL;
  Data[1] = ctrl1_xl; // set accelerometer configuration
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, ACC_GYRO_SENSOR_ADDR, Data, 2, 2000))
  {
    return(FALSE);
  }

  Data[0] = CTRL2_G;
  Data[1] = 0x10 | 0x00; // output rate:13Hz, full scale:245dps
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, ACC_GYRO_SENSOR_ADDR, Data, 2, 2000))
  {
    return(FALSE);
  }

  return(TRUE);
}

/*************************************************************************
 * Function Name: LSM6DS3_Get
 * Parameters: int16_t * pX, int16_t * pY, int16_t * pZ
 *
 * Return: uint32_t
 *
 * Description: Read X,Y,Z - Acc and Gyro data, and Temp
 *************************************************************************/
uint32_t LSM6DS3_Get(I2C_HandleTypeDef * hi2c, int16_t * pAX, int16_t * pAY, int16_t * pAZ, int16_t * pGX, int16_t * pGY, int16_t * pGZ, int16_t * pTemp)
{
#pragma pack(1)
union
{
  uint8_t Data[14];
  struct
  {
    int16_t Temp;
    int16_t GyroX;
    int16_t GyroY;
    int16_t GyroZ;
    int16_t AccX;
    int16_t AccY;
    int16_t AccZ;
  };
} OutData;
#pragma pack()

  // Set start read address
  OutData.Data[0] = OUT_TEMP_L;
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, ACC_GYRO_SENSOR_ADDR, OutData.Data, 1, 2000))
  {
    return(FALSE);
  }

  // Read all output data
  if(HAL_OK != HAL_I2C_Master_Receive(hi2c, ACC_GYRO_SENSOR_ADDR, OutData.Data, 14, 2000))
  {
    return(FALSE);
  }

  *pTemp = OutData.Temp;
  *pAX = OutData.AccX;
  *pAY = OutData.AccY;
  *pAZ = OutData.AccZ;
  *pGX = OutData.GyroX;
  *pGY = OutData.GyroY;
  *pGZ = OutData.GyroZ;

  return(TRUE);
}

/*************************************************************************
 * Function Name: LSM6DS3_AccReady
 * Parameters: none
 *
 * Return: uint32_t
 *
 * Description: Check if AccReady INT is set
 *************************************************************************/
uint32_t LSM6DS3_AccReady(void)
{
  if (HAL_GPIO_ReadPin(ACC_GYRO_INT1_PORT, ACC_GYRO_INT1_PIN))
   return TRUE;
  else
   return FALSE;
}

/*************************************************************************
 * Function Name: LSM6DS3_GyroReady
 * Parameters: none
 *
 * Return: uint32_t
 *
 * Description: Check if GyroReady INT is set
 *************************************************************************/
uint32_t LSM6DS3_GyroReady(void)
{
  if (HAL_GPIO_ReadPin(ACC_GYRO_INT2_PORT, ACC_GYRO_INT2_PIN))
   return TRUE;
  else
   return FALSE;
}
