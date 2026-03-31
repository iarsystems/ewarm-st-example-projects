/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 19, July 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F103ZE-SK board. It shows basic
 * use of parallel I/O, timer, interrupt controller, interface to
 * a Nokia Graphic color LCD module, step motor and 3D accelerometer sensor.
 *  It try to lock position motor's rotor pointer regardless of the board
 * position.
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US  450

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

Int32U CriticalSecCntr;


/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
  }
}


/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
void main(void)
{
Int16S X, Y, Z;
Int32S DegShow = 0, DegShow_h = 1, Delta;
Flo32 Deg;
Flo32 MotorDeg = 0.0;
Boolean NewData = FALSE;
Boolean Dir;
Int32U Steps;

  ENTR_CRT_SECTION();
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  // NVIC init
#ifndef  EMB_FLASH
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  // Step motor init
  StepMotorInit();
  // I2C2 init
  I2C2_Init();

  EXT_CRT_SECTION();

  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);
  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(10,104,131,131);

  // Init Accl sensor
  if(FALSE == Accl_Init())
  {
    // Initialization fault
    GLCD_TextSetPos(0,0);
    GLCD_print("\fLIS3LV020 Init.\r\nfault\r\n");
    while(1);
  }

  while(1)
  {
    // Get new sample of the axes
    if(Accl_Get(&X,&Y,&Z))
    {
      // normalization and calculate angle between the board and the horizontal plain
      if(Y > 1023)
      {
        Y = 1023;
      }
      else if (Y < -1023)
      {
        Y = -1023;
      }
      Deg = (Flo32)Y/1023.0;
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
      DegShow = (Int32S)((Deg*180.0)/(3.14));
      if(DegShow != DegShow_h)
      {
        DegShow_h = DegShow;
        GLCD_TextSetPos(0,0);
        GLCD_print("\fPosition : %d Deg\r\n",DegShow);
        NewData = TRUE;
      }
      else if (StepMotorDone())
      {
        // reduce power consumption
        StepMotorStop();
      }
    }

    if (   StepMotorDone()
        && NewData)
    {
      // Apply compensation
      NewData = FALSE;
      Delta = (Int32S)MotorDeg;
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

      Steps = StepMotorDeg2Steps ((Flo32)Delta);
      if(Steps)
      {
        StepMotorSet(1000,!Dir);
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
  }
}
