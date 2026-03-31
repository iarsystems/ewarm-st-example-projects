/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Accelerometer_Test.c
* Author             : (c) Copyright IAR Systems 2008
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : Accelerometer_Test program body
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
#include "main.h"
#include "i2c2_drv.h"
#include "accl_drv.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* LED Definitions */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Accelerometer_Test
* Description    : Accelerometer_Test program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Accelerometer_Test(void)
{
s16 X, Y, Z;
s32 DegShow = 0, DegShow_h = 1;
f32 Deg;
  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);

  /* Display Test name */
  printf ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  printf( "Accelerometer Test\n ");
  /*I2C2 init*/
  I2C2_Init();

  /* Init Accl sensor*/
  if(FALSE == Accl_Init())
  {
    /* Initialization fault*/
    GLCD_TextSetPos(0,0);
    printf("\fLIS3LV020 Init.\r\nfault\r\n");
    while(1);
  }
  while(ReadKey() != User)
    {
   GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
   GLCD_TextSetPos(1,3);
   printf("Move board vertically");
   GLCD_TextSetPos(1,4);
   printf("to display its ");
   GLCD_TextSetPos(1,5);
   printf ("position %% Horizontal");
    /* Get new sample of the axes*/
    if(Accl_Get(&X,&Y,&Z))
    {
      /* normalization and calculate angle between the board and the horizontal plain*/
      if(Y > 1023)
      {
        Y = 1023;
      }
      else if (Y < -1023)
      {
        Y = -1023;
      }
      Deg = (f32)Y/1023.0;
      Deg = asin(Deg);
      if (Deg >= 0)
      {
        if (Z < 0)
        {
          Deg = 3.14 - Deg;
        }
      }
      else
      {
        if (Z < 0)
        {
          Deg = -3.14 - Deg;
        }
      }
      DegShow = (s32)((Deg*180.0)/(3.14));
      if(DegShow != DegShow_h)
      {
        DegShow_h = DegShow;
        GLCD_TextSetPos(1,7);
        printf("Position:%d Deg\r\n",DegShow);
       }
    }

    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,9);
    printf( "Press User key to exit\n ");
    }

  ReturnFunc();
  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
