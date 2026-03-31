/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Stepper_Motor_Test.c
* Author             : (c) Copyright IAR Systems 2008
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : Stepper_Motor_Test program body
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
#include "step_motor_drv.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* LED Definitions */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Stepper_Motor_Test
* Description    : Stepper_Motor_Test.c
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Stepper_Motor_Test(void)
{
s32 DegShow = 0, Delta;
f32 MotorDeg = 360.0;
Boolean Dir;
u32 Steps;
  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);

    /* Display Test name */
  GLCD_print ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  GLCD_print( "Stepper motor Test\n ");

  GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
  GLCD_TextSetPos(0,4);
  GLCD_print("Plug the stepper motor");
  GLCD_TextSetPos(0,5);
  GLCD_print ("into MT1...");
  /* Step motor init */
  StepMotorInit();

  if (StepMotorDone())
        /* reduce power consumption */
        //StepMotorStop();

  if (    StepMotorDone() )
    {
      /* Apply compensation */
      Delta = (s32)MotorDeg;
      Delta = DegShow - Delta;
      if(Dir = Delta < 0)
      {
        Delta = -Delta;
      }
      if(Delta > 180)
      {
        Dir = !Dir;
        do
        {
          Delta = 360 - Delta;
          if (Delta < 0)
          {
            Delta = -Delta;
          }
        }
        while(Delta >= 360);
      }

      Steps = StepMotorDeg2Steps ((f32)Delta);
      Steps = 3000;
       if(Steps)
      {
        StepMotorSet(4000,!Dir);
        StepMotorRun(Steps);
        if(Dir)
        {
          MotorDeg -= StepMotorSteps2Deg(Steps);
        }
        else
        {
          MotorDeg += StepMotorSteps2Deg(Steps);
        }
        if(MotorDeg > 180.0)
        {
          MotorDeg = MotorDeg - 360;
        }
        else if(MotorDeg < -180.0)
        {
          MotorDeg = MotorDeg + 360;
        }
      }
    }


    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,9);
    GLCD_print( "Press User key to exit\n ");
  while(ReadKey() != User)
    {
    }
  StepMotorStop();
  ReturnFunc();
  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

