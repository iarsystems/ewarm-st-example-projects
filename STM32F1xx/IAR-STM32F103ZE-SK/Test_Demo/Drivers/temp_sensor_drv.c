/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : temp_sensor_drv.h
 *    Description : Temperature sensor STCN75 driver
 *
 *    History :
 *    1. Date        : July 28, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#define TEMP_SENSOR_DRV_GLOBAL
#include "temp_sensor_drv.h"
#include "i2c2_drv.h"
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
 *
 *************************************************************************/
Boolean TempSensor_Init (void)
{
GPIO_InitTypeDef GPIO_InitStructure;
u8 Data[3];

  // Enable GPIOG port
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG , DISABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);
  // Init temperature sensor interrupt signal
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  if(FALSE == I2C2_Open())
  {
    return(FALSE);
  }
  // Init Temperature sensor to default
  Data[0] = CONF_REG;
  Data[1] = 0;
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR, Data, 2))
  {
    I2C2_Close();
    return(FALSE);
  }

  // THYS 75C
  Data[0] = THYS_REG;
  Data[1] = 0x4B;
  Data[2] = 0x00;
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR, Data, 3))
  {
    I2C2_Close();
    return(FALSE);
  }

  // TOS 80C
  Data[0] = TOS_REG;
  Data[1] = 0x50;
  Data[2] = 0x00;
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR, Data, 3))
  {
    I2C2_Close();
    return(FALSE);
  }

  I2C2_Close();
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
Boolean TempSensorShutdown (Boolean Shutdown)
{
u8 Data[2];
  if(FALSE == I2C2_Open())
  {
    return(FALSE);
  }
  Data[0] = CONF_REG;
  // Read config reg
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR, Data, 1))
  {
    I2C2_Close();
    return(FALSE);
  }
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR | 0x1, &Data[1], 1))
  {
    I2C2_Close();
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
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR, Data,2))
  {
    I2C2_Close();
    return(FALSE);
  }
  I2C2_Close();
  return(TRUE);
}

/*************************************************************************
 * Function Name: TempSensor_Conf
 * Parameters: f32 TOS, f32 THYS, Boolean Mode, u32 FaultTol
 *
 * Return: Boolean
 *
 * Description: Temperature sensor config
 *
 *************************************************************************/
Boolean TempSensor_Conf (f32 TOS, f32 THYS, Boolean Mode,
                         u32 FaultTol)
{
u8 Data[3];
s16 Temp;

  if(FALSE == I2C2_Open())
  {
    return(FALSE);
  }
  // Configure the fault tolerance
  Data[0] = CONF_REG;
  // Read config reg
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR, Data, 1))
  {
    I2C2_Close();
    return(FALSE);
  }
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR | 0x1, &Data[1], 1))
  {
    I2C2_Close();
    return(FALSE);
  }
  Data[1] &= ~0x18;
  Data[1] |= (FaultTol & 3) << 3;

  // Write config reg
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR, Data,2))
  {
    I2C2_Close();
    return(FALSE);
  }
  // Set Tos
  Temp = (s16)(TOS * 256.0);
  Data[0] = TOS_REG;
  Data[1] = Temp>>8;
  Data[2] = Temp;
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR, Data, 3))
  {
    I2C2_Close();
    return(FALSE);
  }
  // Set Thys
  Temp = (s16)(THYS * 256.0);
  Data[0] = THYS_REG;
  Data[1] = Temp>>8;
  Data[2] = Temp;
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR, Data, 3))
  {
    I2C2_Close();
    return(FALSE);
  }
  I2C2_Close();
  return(TRUE);
}

/*************************************************************************
 * Function Name: TempSensorGetTemp
 * Parameters: pf32 pTemp, pBoolean pAlarm
 *
 * Return: Boolean
 *
 * Description: Temperature sensor get temperature and alarm state
 *
 *************************************************************************/
Boolean TempSensorGetTemp (pf32 pTemp, pBoolean pAlarm)
{
u8 Data[2];
s16 Temp;

  if(FALSE == I2C2_Open())
  {
    return(FALSE);
  }

  if(Bit_SET == GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6))
  {
    *pAlarm = FALSE;
  }
  else
  {
    *pAlarm = TRUE;
  }

  // Configure the fault tolerance
  Data[0] = TEMP_REG;
  // Read config reg
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR, Data, 1))
  {
    I2C2_Close();
    return(FALSE);
  }
  if(FALSE == I2C2_DataTransfer(TEMP_SENSOR_SENSOR_ADDR | 0x1, Data, 2))
  {
    I2C2_Close();
    return(FALSE);
  }
  I2C2_Close();
  Temp = (Data[0] << 8) | Data[1];

  *pTemp = (f32)Temp;
  *pTemp /= 256.0;
  return(TRUE);
}
