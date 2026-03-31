/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : temp_sensor_drv.h
 *    Description : Temperature sensor STCN75 driver
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
 *    $Revision: #2 $
 **************************************************************************/
#include "arm_comm.h"
#include "iar_stm32f746xx_sk.h"
#include "temp_sensor_drv.h"

typedef enum _temp_sensor_regs_t
{
  TEMP_REG = 0, CONF_REG, THYS_REG, TOS_REG
} temp_sensor_regs_t;

/*************************************************************************
 * Function Name: TempSensor_Init
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Init Temperature sensor
 *************************************************************************/
Boolean TempSensor_Init(I2C_HandleTypeDef * hi2c)
{
GPIO_InitTypeDef GPIO_InitStructure;
Int8U Data[3];

  // Enable TEMP_SENSOR_INT port
  TEMP_SENSOR_INT_CLK_ENABLE();

  // Init temperature sensor interrupt signal
  GPIO_InitStructure.Pin = TEMP_SENSOR_INT_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(TEMP_SENSOR_INT_PORT, &GPIO_InitStructure);

  if(!hi2c)
  {
    return(FALSE);
  }

  // Init Temperature sensor to default
  Data[0] = CONF_REG;
  Data[1] = 0;
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, TEMP_SENSOR_SENSOR_ADDR, Data, 2, 1000))
  {
    return(FALSE);
  }

  // THYS 75C
  Data[0] = THYS_REG;
  Data[1] = 0x4B;
  Data[2] = 0x00;
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, TEMP_SENSOR_SENSOR_ADDR, Data, 3, 1000))
  {
    return(FALSE);
  }

  // TOS 80C
  Data[0] = TOS_REG;
  Data[1] = 0x50;
  Data[2] = 0x00;
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, TEMP_SENSOR_SENSOR_ADDR, Data, 3, 1000))
  {
    return(FALSE);
  }

  return(TRUE);
}

/*************************************************************************
 * Function Name: TempSensorShutdown
 * Parameters: Boolean Shutdown
 *
 * Return: Boolean
 *
 * Description: Enable/Disable sensor shutdown
 *
 *************************************************************************/
Boolean TempSensorShutdown(I2C_HandleTypeDef * hi2c, Boolean Shutdown)
{
Int8U Data[2];

  if(!hi2c)
  {
    return(FALSE);
  }

  // Read config reg
  Data[0] = CONF_REG;
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, TEMP_SENSOR_SENSOR_ADDR, Data, 1, 1000))
  {
    return(FALSE);
  }
  if(HAL_OK != HAL_I2C_Master_Receive(hi2c, TEMP_SENSOR_SENSOR_ADDR, &Data[1], 1, 1000))
  {
    return(FALSE);
  }

  if (Shutdown)
  {
    Data[1] |= 1U << 0;
  }
  else
  {
    Data[1] &= ~(1U << 0);
  }

  // Write config reg
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, TEMP_SENSOR_SENSOR_ADDR, Data, 2, 1000))
  {
    return(FALSE);
  }

  return(TRUE);
}

/*************************************************************************
 * Function Name: TempSensor_Conf
 * Parameters: Flo32 TOS, Flo32 THYS, Boolean Mode, Int32U FaultTol
 *
 * Return: Boolean
 *
 * Description: Temperature sensor config
 *
 *************************************************************************/
Boolean TempSensor_Conf(I2C_HandleTypeDef * hi2c, Flo32 TOS, Flo32 THYS,
                        Boolean Mode, Int32U FaultTol)
{
Int8U Data[3];
Int16S Temp;

  if(!hi2c)
  {
    return(FALSE);
  }

  // Configure the fault tolerance
  // Read config reg
  Data[0] = CONF_REG;
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, TEMP_SENSOR_SENSOR_ADDR, Data, 1, 1000))
  {
    return(FALSE);
  }
  if(HAL_OK != HAL_I2C_Master_Receive(hi2c, TEMP_SENSOR_SENSOR_ADDR, &Data[1], 1, 1000))
  {
    return(FALSE);
  }

  Data[1] &= ~0x18;
  Data[1] |= (FaultTol & 3) << 3;

  // Write config reg
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, TEMP_SENSOR_SENSOR_ADDR, Data, 2, 1000))
  {
    return(FALSE);
  }

  // Set Tos
  Temp = (Int16S)(TOS * 256.0);
  Data[0] = TOS_REG;
  Data[1] = Temp>>8;
  Data[2] = Temp;
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, TEMP_SENSOR_SENSOR_ADDR, Data, 3, 1000))
  {
    return(FALSE);
  }

  // Set Thys
  Temp = (Int16S)(THYS * 256.0);
  Data[0] = THYS_REG;
  Data[1] = Temp>>8;
  Data[2] = Temp;
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, TEMP_SENSOR_SENSOR_ADDR, Data, 3, 1000))
  {
    return(FALSE);
  }

  return(TRUE);
}

/*************************************************************************
 * Function Name: TempSensorGetTemp
 * Parameters: pFlo32 pTemp, pBoolean pAlarm
 *
 * Return: Boolean
 *
 * Description: Temperature sensor get temperature and alarm state
 *
 *************************************************************************/
Boolean TempSensorGetTemp(I2C_HandleTypeDef * hi2c, pFlo32 pTemp, pBoolean pAlarm)
{
Int8U Data[2];
Int16S Temp;

  if(!hi2c)
  {
    return(FALSE);
  }

  if(GPIO_PIN_SET == HAL_GPIO_ReadPin(TEMP_SENSOR_INT_PORT, TEMP_SENSOR_INT_PIN))
  {
    *pAlarm = FALSE;
  }
  else
  {
    *pAlarm = TRUE;
  }

  // Configure the fault tolerance
  // Read config reg
  Data[0] = TEMP_REG;
  if(HAL_OK != HAL_I2C_Master_Transmit(hi2c, TEMP_SENSOR_SENSOR_ADDR, Data, 1, 1000))
  {
    return(FALSE);
  }
  if(HAL_OK != HAL_I2C_Master_Receive(hi2c, TEMP_SENSOR_SENSOR_ADDR, Data, 2, 1000))
  {
    return(FALSE);
  }

  Temp = (Data[0] << 8) | Data[1];
  *pTemp = (Flo32)Temp;
  *pTemp /= 256.0;

  return(TRUE);
}
