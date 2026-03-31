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
 *    1. Date        : 30, July 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *   This example project shows how to use the IAR Embedded Workbench
 *  for ARM to develop code for the IAR-STM32F103ZE-SK board.
 *   It implements a MMC/SD card drive. The first free drive letters will be
 *  used. For example, if your PC configuration includes two hard disk partitions
 *  (in C:\ and D:\) and a CD-ROM drive (in E:\), the memory card drive will
 *  appear as F:\.
 *  The LCD backlight will indicate drive activity.
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US       450
#define UPDATE_SHOW_DLY 5

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

Int32U CriticalSecCntr;
volatile Boolean SysTickFl;

#pragma data_alignment=4
__no_init Int8U Lun0Buffer[512];

/*************************************************************************
 * Function Name: TickHandler
 * Parameters: void
 * Return: void
 *
 * Description:
 *		
 *************************************************************************/
void TickHandler(void)
{
  SysTickFl = TRUE;
}

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
Int32U Dly = 0;
DiskStatusCode_t StatusHold = (DiskStatusCode_t) -1;
Int64U Size,Tmp;

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

  // SysTick end of count event each 0.1s with input clock equal to 9MHz (HCLK/8, default)
  SysTick_Config(900000);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  // Init SCSI module
  ScsiInit();

  // LUNs Init
  LunInit(SD_DISK_LUN,SdDiskInit,SdGetDiskCtrlBkl,SdDiskIO);

  EXT_CRT_SECTION();

  // Init SD card driver
  // SCSI init push init SD driver message for an each LUN
  for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
  {
    // Implement LUNs messages
    if(LunImp(i))
    {
      GLCD_Backlight(BACKLIGHT_ON);
    }
    else
    {
      GLCD_Backlight(BACKLIGHT_OFF+0x10);
    }
  }

  // Soft connection enable
  USB_ConnectRes(TRUE);

  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);

  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(10,107,131,131);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fMass storage device\r");

  GLCD_SetWindow(10,119,131,131);
  GLCD_TextSetPos(0,0);

  while(1)
  {
    for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
    {
      // Implement LUNs messages
      if(LunImp(i))
      {
        GLCD_Backlight(BACKLIGHT_ON);
      }
      else
      {
        GLCD_Backlight(BACKLIGHT_OFF+0x10);
      }
    }
    if (SysTickFl)
    {
      SysTickFl = FALSE;
      // Update MMC/SD card status
      SdStatusUpdate();
      if(Dly-- == 0)
      {
        // LCD show
        Dly = UPDATE_SHOW_DLY;
        // Current state of MMC/SD show
        pDiskCtrlBlk_t pMMCDiskCtrlBlk = SdGetDiskCtrlBkl();
        if(StatusHold != pMMCDiskCtrlBlk->DiskStatus)
        {

          StatusHold = pMMCDiskCtrlBlk->DiskStatus;
          switch (pMMCDiskCtrlBlk->DiskStatus)
          {
          case DiskCommandPass:
            // Calculate MMC/SD size [MB]
            Size  = (Int64U)pMMCDiskCtrlBlk->BlockNumb;
            Size *= pMMCDiskCtrlBlk->BlockSize;
            Tmp  = Size/1000000LL;
            Tmp += ((Size%1000000LL)<1000000LL/2)?0:1;
            if(pMMCDiskCtrlBlk->DiskType == DiskMMC)
            {
              GLCD_print("MMC Card - %lld MB\r",Tmp);
            }
            else
            {
              GLCD_print("SD Card - %lld MB\r",Tmp);
            }
            break;
          default:
            GLCD_print("Pls, Insert Card\r");
          }
        }
      }
    }
  }
}
