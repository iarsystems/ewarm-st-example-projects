/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : September, 22 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 * Jumpers:
 * 	USB_E/USB_P70   - USB_P70 (2-3)
 * 	RST-TRST        - unfilled
 * 	L15/ETM_P66,
 *	L16/ETM_P67     - 2-3 (near prototype area) when jtrace is used
 *
 *   This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for the IAR-STR912-SK evaluation board. It implements a
 *  MMC/SD card drive. The first free drive letters will be used. For example,
 *  if your PC configuration includes two hard disk partitions (in C:\ and D:\)
 *  and a CD-ROM drive (in E:\), the memory card drive will appear as F:\.
 *   The MassStorage example project works standalone on the IAR-STR912-SK board.
 *   The LCD backlight will indicate drive activity.
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"
#define TICK_PER_SECOND       2
#define UPDATE_SHOW_DLY       ((Int32U)(0.5*TICK_PER_SECOND))
#define STARTUP_SHOW_DLY      ((Int32U)(5 *TICK_PER_SECOND))

const Int8U HexToCharStr [] = "0123456789ABCDEF";
volatile Boolean TickSysFlag;
volatile Int32U DlyCount;

#pragma data_alignment=4
Int8U Lun0Buffer[2048];

/*************************************************************************
 * Function Name: Tim0Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void Tim0Handler (void)
{
  // Clear TIM0 counter
  TIM_CounterCmd(TIM0, TIM_CLEAR);
  if(DlyCount)
  {
    --DlyCount;
  }
  // Clear TIM0 flag OC1
  TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
}

/*************************************************************************
 * Function Name: Tim1Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 1 interrupt handler
 *
 *************************************************************************/
void Tim1Handler (void)
{
  // Clear TIM1 counter
  TIM_CounterCmd(TIM1, TIM_CLEAR);
  TickSysFlag = TRUE;
  // Clear TIM1 flag OC1
  TIM_ClearFlag(TIM1,TIM_FLAG_OC1);
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init MCU clock
 *
 *************************************************************************/
void InitClock (void)
{
  // Clock
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);       // master clk - OSC clk
  // Flash controller init
  SCU_FMICLKDivisorConfig(SCU_FMICLK_Div1);
  FMI_Config(FMI_READ_WAIT_STATE_2,FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE,\
             FMI_LVD_ENABLE,FMI_FREQ_HIGH);
  // Set clks dividers
  SCU_RCLKDivisorConfig(SCU_RCLK_Div1);
  SCU_HCLKDivisorConfig(SCU_HCLK_Div1);
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1);
  // Init PLL = 48 MHz
  SCU_PLLFactorsConfig(192,25,3);
  // PLL Enabled
  SCU_PLLCmd(ENABLE);
  // Switch clk MCLK = PLL
  SCU_MCLKSourceConfig(SCU_MCLK_PLL);
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Delay
 *
 * Return: none
 *
 * Description: Delay 100us * arg
 *
 *************************************************************************/
void DelayResolution100us(Int32U Delay)
{
  DlyCount = Delay;

  // Clear TIM0 counter
  TIM_CounterCmd(TIM0, TIM_CLEAR);
  // Clear TIM0 flag OC1
  TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
  // Enable TIM0 OC1 interrupt
  TIM_ITConfig(TIM0, TIM_IT_OC1, ENABLE);
  // Enable TIM0 counter
  TIM_CounterCmd(TIM0, TIM_START);

  while(DlyCount);

  // Disable TIM0 OC1 interrupt
  TIM_ITConfig(TIM0, TIM_IT_OC1, DISABLE);
  // Disable TIM0 counter
  TIM_CounterCmd(TIM0, TIM_STOP);
}

/*************************************************************************
 * Function Name: InitDlyTimer
 * Parameters: Int32U IntrPriority
 *
 * Return: none
 *
 * Description: Init Delay Timer (TIM 0)
 *
 *************************************************************************/
void InitDlyTimer (Int32U IntrPriority)
{
TIM_InitTypeDef TIM_InitStructure;

  // Enable TIM0 clocks
  SCU_APBPeriphClockConfig(__TIM01, ENABLE);
  // Release TIM0 reset
  SCU_APBPeriphReset(__TIM01,DISABLE);

  // Timer 0
  // TIM Configuration in Output Compare Timing Mode period 100us
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB; // assign PCLK to TIM_Clk
  TIM_InitStructure.TIM_Prescaler = 48 - 1;         // 1us resolution
  TIM_InitStructure.TIM_Pulse_Length_1 = 100;       // 100 us period
  TIM_Init(TIM0, &TIM_InitStructure);

  // VIC configuration
  VIC_Config(TIM0_ITLine, VIC_IRQ, IntrPriority);
  VIC_ITCmd(TIM0_ITLine, ENABLE);
}

/*************************************************************************
 * Function Name: InitSysTimer
 * Parameters: Int32U IntrPriority
 *
 * Return: none
 *
 * Description: Init Delay Timer (TIM 1)
 *
 *************************************************************************/
void InitSysTimer (Int32U IntrPriority)
{
TIM_InitTypeDef TIM_InitStructure;

  // Enable TIM0 clocks
  SCU_APBPeriphClockConfig(__TIM01, ENABLE);
  // Release TIM0 reset
  SCU_APBPeriphReset(__TIM01,DISABLE);

  // Timer 1
  // TIM Configuration in Output Compare Timing Mode period 200 msec
  SCU_TIMPresConfig(SCU_TIM01,((48000-1)&~1UL));    // Timer resolution 1ms
  SCU_TIMCLKSourceConfig(SCU_TIM01,SCU_TIMCLK_INT); // Timer01 Clock form the prescaler
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_SCU; // assign Presclater clk to TIM_Clk
  TIM_InitStructure.TIM_Pulse_Length_1 = 200;       // 200 ms period
  TIM_Init(TIM1, &TIM_InitStructure);
  TIM_CounterCmd(TIM1, TIM_CLEAR);                  // clear the timer 1 counter

  // VIC configuration
  VIC_Config(TIM1_ITLine, VIC_IRQ, IntrPriority);
  VIC_ITCmd(TIM1_ITLine, ENABLE);

  // Clear TIM1 flag OC1
  TIM_ClearFlag(TIM1,TIM_FLAG_OC1);
  // Enable TIM1 OC1 interrupt
  TIM_ITConfig(TIM1, TIM_IT_OC1, ENABLE);
  // Enable TIM1 counter
  TIM_CounterCmd(TIM1, TIM_START);
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
extern void MmcSendBlock (pInt8U pData, Int32U Size);
int main()
{
Int32U Dly;
Int8U Message[17];
Int32U Tmp, Size;
DiskStatusCode_t StatusHold = (DiskStatusCode_t) -1;
Int32U USB_StateHold = (Int32U)-1;

  // MCU clock init
  InitClock();
  // Enable the clock for the VIC
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);
  // VIC Deinitialization
  VIC_DeInit();
  // Delay timer init
  InitDlyTimer(5);
  // Delay timer init
  InitSysTimer(4);
  // Init USB
  USB_Init(0,1,UsbClassBotConfigure);
  // LUNs Init
  LunInit(MMC_DISK_LUN,MmcDiskInit,MmcDiskInfo,MmcDiskStatus,MmcDiskIO);
  // Init SCSI module
  ScsiInit();

  // Soft connection enable
  USB_ConnectRes(TRUE);
  // Enable interrupts
  __enable_interrupt();

  // LCD Powerup init
  HD44780_PowerUpInit();
  // Show message on LCD
  HD44780_StrShow(1, 1,  "IAR Systems ARM ");
  HD44780_StrShow(1, 2,  "USB Mass Storage");

  // Set the startup message time out
  Dly = STARTUP_SHOW_DLY;

  while(1)
  {
    for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
    {
      // Implement LUNs messages
      if(LunImp(i))
      {
        LCD_LIGHT_ON();
      }
      else
      {
        LCD_LIGHT_OFF();
      }
    }
    if (TickSysFlag)
    {
      TickSysFlag = 0;
      // Update MMC/SD card status
      MmcStatusUpdate();
      if(Dly-- == 0)
      {
        // LCD show
        Dly = UPDATE_SHOW_DLY;
        Tmp = UsbCoreReq(UsbCoreReqDevState);
        if(USB_StateHold != Tmp)
        {
          // Current state of USB show
          if((USB_StateHold = Tmp) == UsbDevStatusConfigured)
          {
            if(UsbCoreReq(UsbCoreReqDevSusState))
            {
              strcpy((char*)Message,"USB - Suspend   ");
            }
            else
            {
              strcpy((char*)Message,"USB - Connect   ");
            }
          }
          else
          {
            strcpy((char*)Message,"Pls, Insert USB ");
          }
          HD44780_StrShow(1, 1, (pInt8S)Message);
        }
        // Current state of MMC/SD show
        pDiskCtrlBlk_t pMMCDiskCtrlBlk = MmcDiskStatus();
        if(StatusHold != pMMCDiskCtrlBlk->DiskStatus)
        {
          StatusHold = pMMCDiskCtrlBlk->DiskStatus;
          switch (pMMCDiskCtrlBlk->DiskStatus)
          {
          case DiskCommandPass:
            // Calculate MMC/SD size [MB]
            Size = (pMMCDiskCtrlBlk->BlockNumb * pMMCDiskCtrlBlk->BlockSize);
            Tmp  = Size/1000000;
            Tmp += ((Size%1000000)<1000000/2)?0:1;

            switch(pMMCDiskCtrlBlk->DiskType)
            {
            case DiskMMC:
              sprintf((char*)Message,"MMC Card - %dMB",Tmp);
              break;
            case DiskSD:
              sprintf((char*)Message,"SD Card - %dMB",Tmp);
              break;
            default:
              sprintf((char*)Message,"Card - %dMB",Tmp);
            }
            for(Int32U i = strlen((char*)Message); i < 16; ++i)
            {
              Message[i] = ' ';
            }
            Message[strlen((char*)Message)+1] = 0;
            break;
          default:
            strcpy((char*)Message,"Pls, Insert Card");
          }
          HD44780_StrShow(1, 2, (pInt8S)Message);
        }
      }
    }
  }
}
