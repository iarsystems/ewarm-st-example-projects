/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : 10, June 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32F429II-EXP evaluation board. It shows
 * a simple magnetometer application using the new UEXT framework.
 *  Connect MOD-LCD3310 to the UXT1 and MOD-MAG3110(MOD-MAG) to the UXT2
 * connector. The application will display current temperature and current
 * magnetic field intensity in three axises (X, Y, Z).
 * Immediately after starting the application, it should be calibrated (the
 * "calibrating..." message is shown on the display).
 * Please make a full rotations of the magnetometer sensor around each axis
 * (X, Y and Z), until the displaying of the values starts. Magnetic field
 * intensity is displayed in relative values (as outputted from the sensor)
 * with addition of the calibrated offset.
 *
 * COMPATIBILITY
 * =============
 *  The Magnetometer project is compatible with IAR-STM32F429II-EXP
 * evaluation board. By default, the project is configured to use the
 * J-Link JTAG/SWD interface.
 *
 * CONFIGURATION
 * =============
 *  Flash Debug - The Progam is loaded to internal Flash.
 *
 * GETTING STARTED
 * ===============
 * 1) Build and download the example.
 *
 * 2) Run the example.
 *
 *    $Revision: 3160 $
 **************************************************************************/

#include <intrinsics.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "framework/platform/platform_config.h"
#include "framework/uext.h"
#include "framework/mod.h"
#include "framework/connect.h"
#include "board/board_def.h"

#include "mod_lcd3310.h"
#include "mod_mag3110.h"

uint32_t CriticalSecCntr;

extern const FontType_t Console_5_7;

/* CALIBRATION_THRESHOLD */
/* the higher value - the more precise calibration, also more time for calibration */
/* recommended values 700..800 */

#define CALIBRATION_THRESHOLD 740

/* TEMP_OFFSET */
/* temperature offset value should be tuned for each MOD_MAG3110 sensor */

#define TEMP_OFFSET 26

const unsigned char CustomSymbolsStream[] =
{
   0x00, 0x06, 0x09, 0x09, 0x06  // degree symbol
};

const FontType_t CustomSymbols =
{
  5,
  7,
  0x00,
  1,
  (pInt8U)CustomSymbolsStream,
  "CustomSymbols (5x7)"
};

/*************************************************************************
 * Function Name: Magnetometer_showI2CError
 * Parameters:    ConnectionHandler_t * conn - connection handler for display
 *
 * Return:        none
 *
 * Description: Shows an I2C error message on the display.
 *************************************************************************/
void Magnetometer_showI2CError(ConnectionHandler_t * conn)
{
  MOD_LCD3310_setTextPosition(conn,  0, 3);
  MOD_LCD3310_writeString(conn, "              ");
  MOD_LCD3310_setTextPosition(conn,  0, 4);
  MOD_LCD3310_writeString(conn, "I2C read error");
  MOD_LCD3310_setTextPosition(conn,  0, 5);
  MOD_LCD3310_writeString(conn, "              ");
}

/*************************************************************************
 * Function Name: Magnetometer_drawBarGraph
 * Parameters:    ConnectionHandler_t * conn - connection handler for display
 *                int16_t magValue - magnetometer axis value
 *                uint8_t x - x coordinate for the bar graph
 *                uint8_t y - y coordinate for the bar graph
 *                uint8_t width - bargraph max width
 *
 * Return:        none
 *
 * Description: Builds graphic representation of mag. value in bar style.
 *************************************************************************/
void Magnetometer_drawBarGraph(ConnectionHandler_t * conn, int16_t magValue, uint8_t x, uint8_t y, uint8_t width)
{
int8_t xd;
int16_t div = 900 / width;
uint8_t cen = width/2;

  if (magValue < 0)
    xd = x + cen - (abs(magValue) / div);
  else
    xd = x + cen + (magValue / div);

  if (xd > x + width - 1) xd = x + width - 1;
  if (xd < x) xd = x;

   if (magValue < 0)
   {
     MOD_LCD3310_filledRectangle(conn,       x, y,      xd-1, y+6, 0);
     MOD_LCD3310_filledRectangle(conn,      xd, y,     x+cen, y+6, 1);
     MOD_LCD3310_filledRectangle(conn, x+cen+1, y, x+width-1, y+6, 0);
   }
   else
   {
     MOD_LCD3310_filledRectangle(conn,       x, y,   x+cen-1, y+6, 0);
     MOD_LCD3310_filledRectangle(conn,   x+cen, y,        xd, y+6, 1);
     MOD_LCD3310_filledRectangle(conn,    xd+1, y, x+width-1, y+6, 0);
   }
}

/*************************************************************************
 * Function Name: main
 * Parameters:    none
 *
 * Return:        none
 *
 * Description: main
 *************************************************************************/
void main(void)
{
ConnectionHandler_t * displayHandler;
ConnectionHandler_t * magHandler;
MOD_MAG3110_Data_t magData;
int16_t x_min, x_max, y_min, y_max, z_min, z_max;
int16_t x_cen, y_cen, z_cen;
uint8_t count = 0;
uint8_t calibrated = 0;
uint8_t dr_status;
char str[10];

  __disable_interrupt();

  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  /* NVIC init */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  ENTR_CRT_SECTION();
  /* SysTick Config*/
  if(SysTick_Config(SystemCoreClock/10000))
  {
    /* Error occured */
    while (1);
  }
  EXT_CRT_SECTION();

  /* Connect Modules and get handlers */
  displayHandler = ConnectModule(MOD_LCD3310, UEXT1, &MOD_LCD3310_PG);
  magHandler     = ConnectModule(MOD_MAG3110, UEXT2, &MOD_MAG3110_PG);

  /* Display and Magnetometer sensor initialization */
  if (MOD_MAG3110_init(magHandler, 0, os128) != MOD_MAG3110_RESULT_OK) 
  {
    /* catch error */
    while(1);
  }
  if (MOD_LCD3310_init(displayHandler) != MOD_LCD3310_RESULT_OK)
  {
    /* catch error */
    while(1);
  }
  MOD_LCD3310_setFont(displayHandler, &Console_5_7);
  MOD_LCD3310_clear(displayHandler);

  /* check if the measurement values are ready */
  do
  {
    if( MOD_MAG3110_getStatus(magHandler, &dr_status) != MOD_MAG3110_RESULT_OK )
    {
      Magnetometer_showI2CError(displayHandler);
    }
  }
  while(!(dr_status & MOD_MAG3110_DR_STATUS_ZYXDR));

  /* get data from the sensor */
  if( MOD_MAG3110_getXYZ(magHandler, &magData) != MOD_MAG3110_RESULT_OK)
  {
    Magnetometer_showI2CError(displayHandler);
  }
  x_min = x_max = magData.X;
  y_min = y_max = magData.Y;
  z_min = z_max = magData.Z;

  /* Output title on the display */
  MOD_LCD3310_setTextPosition(displayHandler,  0, 0);
  MOD_LCD3310_writeString(displayHandler, " MAGNETOMETER ");
  MOD_LCD3310_setTextPosition(displayHandler,  9, 1);
  MOD_LCD3310_writeString(displayHandler, "IAR Systems");

  MOD_LCD3310_setTextPosition(displayHandler,  0, 3);
  MOD_LCD3310_writeString(displayHandler, "calibrating...");

  /* calibration routine */
  while(!calibrated)
  {
    /* check if the measurement values are ready */
    do
    {
      if( MOD_MAG3110_getStatus(magHandler, &dr_status) != MOD_MAG3110_RESULT_OK )
      {
        Magnetometer_showI2CError(displayHandler);
      }
    }
    while(!(dr_status & MOD_MAG3110_DR_STATUS_ZYXDR));

    /* read XYZ data */
    if( MOD_MAG3110_getXYZ(magHandler, &magData) != MOD_MAG3110_RESULT_OK)
    {
      Magnetometer_showI2CError(displayHandler);
    }
    else
    {
      /* detect min and max values */
      if(magData.X < x_min) x_min = magData.X;
      if(magData.X > x_max) x_max = magData.X;
      if(magData.Y < y_min) y_min = magData.Y;
      if(magData.Y > y_max) y_max = magData.Y;
      if(magData.Z < z_min) z_min = magData.Z;
      if(magData.Z > z_max) z_max = magData.Z;

      /* calculate range center */
      x_cen = (x_min+x_max) / 2;
      y_cen = (y_min+y_max) / 2;
      z_cen = (z_min+z_max) / 2;

      /* check if ranges are wide enough */
      if(((x_max-x_min) > CALIBRATION_THRESHOLD) &&
         ((y_max-y_min) > CALIBRATION_THRESHOLD) &&
         ((z_max-z_min) > CALIBRATION_THRESHOLD))
        calibrated = 1;
    }
  }

  /* display static labels */
  MOD_LCD3310_setTextPosition(displayHandler,  0, 3);
  MOD_LCD3310_writeString(displayHandler, "              ");

  MOD_LCD3310_setTextPosition(displayHandler,  0, 3);
  MOD_LCD3310_writeString(displayHandler, "-");
  MOD_LCD3310_setTextPosition(displayHandler, 36, 3);
  MOD_LCD3310_writeString(displayHandler, "+");

  MOD_LCD3310_setTextPosition(displayHandler,  0, 4);
  MOD_LCD3310_writeString(displayHandler, "-");
  MOD_LCD3310_setTextPosition(displayHandler, 36, 4);
  MOD_LCD3310_writeString(displayHandler, "+");

  MOD_LCD3310_setTextPosition(displayHandler,  0, 5);
  MOD_LCD3310_writeString(displayHandler, "-");
  MOD_LCD3310_setTextPosition(displayHandler, 36, 5);
  MOD_LCD3310_writeString(displayHandler, "+");

  /* main measure loop */
  while(1)
  {
    /* check if the measurement values are ready */
    do
    {
      if( MOD_MAG3110_getStatus(magHandler, &dr_status) != MOD_MAG3110_RESULT_OK )
      {
        Magnetometer_showI2CError(displayHandler);
      }
    }
    while(!(dr_status & MOD_MAG3110_DR_STATUS_ZYXDR));

    /* read XYZ data */
    if( MOD_MAG3110_getXYZ(magHandler, &magData) != MOD_MAG3110_RESULT_OK)
    {
      Magnetometer_showI2CError(displayHandler);
    }
    else
    {
      /* apply the offset values */
      magData.X -= x_cen;
      magData.Y -= y_cen;
      magData.Z -= z_cen;
      magData.T += TEMP_OFFSET;

      /* draw bar-graphs to buffer then update to display */
      Magnetometer_drawBarGraph(displayHandler, magData.X, 6, 24, 29);
      Magnetometer_drawBarGraph(displayHandler, magData.Y, 6, 32, 29);
      Magnetometer_drawBarGraph(displayHandler, magData.Z, 6, 40, 29);
      MOD_LCD3310_update(displayHandler);

      if(!(++count%3))
      {
        /* print magnetometer values */
        MOD_LCD3310_setTextPosition(displayHandler, 24,2);
        sprintf(str,"t:%3i",magData.T);
        MOD_LCD3310_writeString(displayHandler, str);
        MOD_LCD3310_setTextPosition(displayHandler, 54,2);
        MOD_LCD3310_putChar(displayHandler, 0, &CustomSymbols);
        MOD_LCD3310_setTextPosition(displayHandler, 60,2);
        MOD_LCD3310_writeString(displayHandler, "C");

        MOD_LCD3310_setTextPosition(displayHandler, 48,3);
        sprintf(str,"X:%-4i",magData.X);
        MOD_LCD3310_writeString(displayHandler, str);

        MOD_LCD3310_setTextPosition(displayHandler, 48,4);
        sprintf(str,"Y:%-4i",magData.Y);
        MOD_LCD3310_writeString(displayHandler, str);

        MOD_LCD3310_setTextPosition(displayHandler, 48,5);
        sprintf(str,"Z:%-4i",magData.Z);
        MOD_LCD3310_writeString(displayHandler, str);
      }
    }
  }
}
