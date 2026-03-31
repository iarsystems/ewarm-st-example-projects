/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Tem_Sensor_Test.c
* Author             : (c) Copyright IAR Systems 2008
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : Temp_Sensor_Test program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "temp_sensor_drv.h"
#include "i2c2_drv.h"
#include "main.h"
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Temp_Sensor_Test
* Description    : Temp_Sensor_Test program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Temp_Sensor_Test(void)
{

  /* GPIO configuration */
GPIO_InitTypeDef GPIO_InitStructure;
f32 Temp, TempHold = 200.0;
Boolean Alarm;

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);

  /* Display Test name */
  printf ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  printf( "Temp Sensor Test\n ");

  /* GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* SysTick end of count event each 0.5s with input clock equal to 9MHz (HCLK/8, default)*/
  SysTick_Config(4500000);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  /* I2C2 init */
  I2C2_Init();

  /* Init Temperature sensor */
  if(   FALSE == TempSensor_Init()
     || FALSE == TempSensorShutdown(FALSE)
     || FALSE == TempSensor_Conf(31.0,30.0,TEMP_SENOSR_COMP_MODE,2))
  {
    /* Initialization fault */
    GLCD_TextSetPos(4,4);
    printf("Temp.Sensor Init.\r\nFailed\r\n");
    while(1);
  }
  while(ReadKey() != User)
    {
      if(TempSensorGetTemp(&Temp,&Alarm))
      {
        if(Alarm)
        {
          /* turn on Status LED1 */
          GPIO_WriteBit(GPIOF,GPIO_Pin_7,Bit_RESET);
        }
        else
        {
          /* turn off Status LED1 */
          GPIO_WriteBit(GPIOF,GPIO_Pin_7,Bit_SET);
        }
        if(TempHold != Temp)
        {
          TempHold = Temp;
          GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
          GLCD_TextSetPos(1,4);
          printf("Temperature: %3.1fC",Temp);
        }
     }
      else
      {
        /* Initialization fault */
        GLCD_TextSetPos(4,4);
        printf("\fTemp.Sensor reading\r\nFailed\r\n");
        while(1);
      }
    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,9);
    printf( "Press User key to exit\n ");
    }



  ReturnFunc();

  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);

  /* TransferStatus2 = PASSED, if the specified sector part is erased */
  /* TransferStatus2 = FAILED, if the specified sector part is not well erased  */
}



/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
