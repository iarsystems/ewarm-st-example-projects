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
 *    1. Date        : October 10, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR STR912-SK evaluation boards. It implements a MP3
 * player .
 *
 * Jumpers:
 *	POWER_SELECT    - depend of power source
 *	USB_DIS         - 2-3
 *	RST-TRST        - Unfilled
 *	U2_RX_E         - Filled
 *	Jumper array J1 - 2-3 (near prototype area)
 *	Jumper array J2,J5,J6,J7,J8 - Unfilled
 *	Jumper array J16 and J15 - 2-3 (near prototype area) when J-Trace is used
 *
 *  AN_TR - volume control
 *
 *    $Revision: 39 $
 *
 **************************************************************************/
#include    "includes.h"
#include    "91x_conf.h"
#include    "91x_lib.h"

#define TICK_PER_SEC  50

#define LOGO_DLY      (2   * TICK_PER_SEC)  // 2 sec
#define REPEAT_DLY    (0.5 * TICK_PER_SEC)  // 0.5 sec

#define MP3_DMA_INTR_PRIORITY   12
#define MP3_TMR_INTR_PRIORITY   13
#define SYS_TMR_INTR_PRIORITY   14
#define DLY_TMR_INTR_PRIORITY   15

#define PLAY_CH           1
#define WIDE_STEREO_CH    2

const Int8S PlayChar[] =
{
  0x10,
  0x18,
  0x1C,
  0x1E,
  0x1C,
  0x18,
  0x10,
  0x00,
};

const Int8S WideStereoChar[] =
{
  0x0E,
  0x11,
  0x00,
  0x0E,
  0x11,
  0x00,
  0x04,
  0x00,
};

typedef struct _DirTree_t
{
  char DirName[LIST_MAXLENFILENAME];
  struct _DirTree_t *pPrev;
} DirTree_t,*pDirTree_t;

typedef enum _SoundEffect_t
{
  NoSoundEffect = 0, WideStereoEffect, LouldEffect
} SoundEffect_t;

volatile Int32U DlyCount;

Int32U  SysTmrPeriodHold,SysTmrPeriodHold1;

volatile Boolean TickSysFlag;

EmbeddedFileSystem efsl;
EmbeddedFile filer;
DirList list;

const DirTree_t RootDir = {"/",NULL};

Int32U Mp3Init[2];

const Int16U Adc2Vol[] =
{
  0xF0F0, // 0
  0xC0C0, // 1
  0xA0A0, // 2
  0x7070, // 3
  0x6868, // 4
  0x6060, // 5
  0x5858, // 6
  0x5050, // 7
  0x4848, // 8
  0x4040, // 9
  0x3838, // 10
  0x3030, // 11
  0x2828, // 12
  0x2020, // 13
  0x1C1C, // 14
  0x1818, // 15
  0x1414, // 16
  0x1010, // 17
  0x0D0D, // 18
  0x0C0C, // 19
  0x0B0B, // 20
  0x0A0A, // 21
  0x0909, // 22
  0x0808, // 23
  0x0707, // 24
  0x0606, // 25
  0x0505, // 26
  0x0404, // 27
  0x0303, // 28
  0x0202, // 29
  0x0101, // 30
  0x0000, // 31
};

__no_init Int32U Mp3Buffer[512*2/sizeof(Int32U)];

/*************************************************************************
 * Function Name: Tim0Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 2 interrupt handler
 *
 *************************************************************************/
void Tim2Handler (void)
{
  // Update OC1R
  TIM2->OC1R += 100;
  if(DlyCount)
  {
    --DlyCount;
  }
  // Clear TIM2 flag OC1
  TIM_ClearFlag(TIM2,TIM_FLAG_OC1);
}

/*************************************************************************
 * Function Name: Tim1Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 3 interrupt handler
 *
 *************************************************************************/
void Tim3Handler (void)
{
  // Update OC1R
  TIM3->OC1R += SysTmrPeriodHold;
  TickSysFlag = TRUE;
  // Clear TIM3 flag OC1
  TIM_ClearFlag(TIM3,TIM_FLAG_OC1);
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
  SCU_BRCLKDivisorConfig(SCU_BRCLK_Div1);
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

  // Clear TIM2 counter
  TIM_CounterCmd(TIM2, TIM_CLEAR);
  TIM2->OC1R = 100;
  // Clear TIM2 flag OC1
  TIM_ClearFlag(TIM2,TIM_FLAG_OC1);
  // Enable TIM2 OC1 interrupt
  TIM_ITConfig(TIM2, TIM_IT_OC1, ENABLE);
  // Enable TIM2 counter
  TIM_CounterCmd(TIM2, TIM_START);

  while(DlyCount);

  // Disable TIM2 OC1 interrupt
  TIM_ITConfig(TIM2, TIM_IT_OC1, DISABLE);
  // Disable TIM2 counter
  TIM_CounterCmd(TIM2, TIM_STOP);
}

/*************************************************************************
 * Function Name: InitDlyTimer
 * Parameters: Int32U IntrPriority
 *
 * Return: none
 *
 * Description: Init Delay Timer (TIM 2)
 *
 *************************************************************************/
void InitDlyTimer (Int32U IntrPriority)
{
TIM_InitTypeDef TIM_InitStructure;

  // Enable TIM23 clocks
  SCU_APBPeriphClockConfig(__TIM23, ENABLE);
  // Release TIM23 reset
  SCU_APBPeriphReset(__TIM23,DISABLE);
  // APB peripheral clock disabled during ARM debug state
  SCU_APBPeriphDebugConfig(__TIM23,DISABLE);

  // Timer 2
  // TIM Configuration in Output Compare Timing Mode period 100us
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB; // assign PCLK to TIM_Clk
  TIM_InitStructure.TIM_Prescaler = 48 - 1;         // 1us resolution
  TIM_InitStructure.TIM_Pulse_Length_1 = 100;       // 100 us period
  TIM_Init(TIM2, &TIM_InitStructure);

  // VIC configuration
  VIC_Config(TIM2_ITLine, VIC_IRQ, IntrPriority);
  VIC_ITCmd(TIM2_ITLine, ENABLE);
}

/*************************************************************************
 * Function Name: InitSystemTimer
 * Parameters: Int32U IntrPriority
 *
 * Return: none
 *
 * Description: Init System Timer (TIM 3)
 *
 *************************************************************************/
void InitSystemTimer (Int32U IntrPriority)
{
TIM_InitTypeDef TIM_InitStructure;

  // Enable TIM23 clocks
  SCU_APBPeriphClockConfig(__TIM23, ENABLE);
  // Release TIM23 reset
  SCU_APBPeriphReset(__TIM23,DISABLE);
  // APB peripheral clock disabled during ARM debug state
  SCU_APBPeriphDebugConfig(__TIM23,DISABLE);

  // Timer 3
  // TIM Configuration in Output Compare Timing Mode period 100 msec
  SCU_TIMPresConfig(SCU_TIM23,((48000-1)&~1UL));    // Timer resolution 1ms
  SCU_TIMCLKSourceConfig(SCU_TIM23,SCU_TIMCLK_INT); // Timer01 Clock form the prescaler
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_SCU; // assign Prescaler clk to TIM_Clk
  SysTmrPeriodHold = 1000/TICK_PER_SEC;             // 20 ms period
  TIM_InitStructure.TIM_Pulse_Length_1 = SysTmrPeriodHold;

  TIM_Init(TIM3, &TIM_InitStructure);
  TIM_CounterCmd(TIM3, TIM_CLEAR);                  // clear the timer 3 counter

  // VIC configuration
  VIC_Config(TIM3_ITLine, VIC_IRQ, IntrPriority);
  VIC_ITCmd(TIM3_ITLine, ENABLE);

  // Clear TIM3 flag OC1
  TIM_ClearFlag(TIM3,TIM_FLAG_OC1);
  // Enable TIM3 OC1 interrupt
  TIM_ITConfig(TIM3, TIM_IT_OC1, ENABLE);
  // Enable TIM3 counter
  TIM_CounterCmd(TIM3, TIM_START);
}

/*************************************************************************
 * Function Name: AdcInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Initialize ADC channel 5
 *
 *************************************************************************/
void AdcInit(void)
{
ADC_InitTypeDef   ADC_InitStructure;

  // Enable the clock for the ADC and GPIO4
  SCU_APBPeriphClockConfig(__ADC, ENABLE);
  // ADC Deinitialization
  ADC_DeInit();
  // Configure the GPIO4 pin 5 as analog input
  GPIO_ANAPinConfig(GPIO_ANAChannel5, ENABLE);
  /* ADC Structure Initialization */
  ADC_StructInit(&ADC_InitStructure);

  // Configure the ADC in continuous mode conversion
  ADC_InitStructure.ADC_Channel_5_Mode = ADC_NoThreshold_Conversion;
  ADC_InitStructure.ADC_Select_Channel = ADC_Channel_5;
  ADC_InitStructure.ADC_Scan_Mode = DISABLE;
  ADC_InitStructure.ADC_Conversion_Mode = ADC_Continuous_Mode;

  // Enable the ADC
  ADC_Cmd(ENABLE);

  // Prescaler configure
  ADC_PrescalerConfig(0x0);

  // Configure the ADC
  ADC_Init(&ADC_InitStructure);

  // Start the conversion
  ADC_ConversionCmd(ADC_Conversion_Start);

}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
int main (void)
{
Int32U ShowDly;
Int32U Tmp;
esint8 FatOpen = -1;
pDirTree_t pCurrDir = (pDirTree_t)&RootDir, pDirTemp;
Int8U Deep = 0;
Boolean Play = 0, Dir,PervDir;
Key_t PressedKey;
File Mp3File;
Boolean FileOpen;
Boolean FillBlockOffset, DrainBlockOffset;
Int32U FillSize = 0, DrainSizeHold = 0;
Mp3Stream_t Mp3Stream;
char* pStr;
Int32U Volume = 0;
MP3_Status_t MP3_Status;
SoundEffect_t SoundEffect = NoSoundEffect;
Boolean  Line2Update = 0;
Int8S Line2[17];

  // Init Pll
  InitClock();

  // Enable the clock for the VIC
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);
  // VIC Deinitialization
  VIC_DeInit();

  // Delay timer init
  InitDlyTimer(DLY_TMR_INTR_PRIORITY);

  // System timer init
  InitSystemTimer(SYS_TMR_INTR_PRIORITY);

  // Enable the Interrupt controller to manage IRQ channel
  __enable_interrupt();

  // LCD Powerup init
  HD44780_PowerUpInit();
  HD44780_WrCGRAM(PlayChar,PLAY_CH);
  HD44780_WrCGRAM(WideStereoChar,WIDE_STEREO_CH);

  // Backlight On
  LCD_LIGHT_ON();

  // Init MMC/SD driver
  MmcDiskInit();

  // Init keys
  KeyInit();

  // Volume control init
  AdcInit();

  // init MP3 module
  do
  {
    Mp3Init[0] = MP3_DMA_INTR_PRIORITY;
    Mp3Init[1] = MP3_TMR_INTR_PRIORITY;
    Mp3SendCmd(Mp3CmdPowerUp,Mp3Init);
    MP3_Status = (MP3_Status_t)Mp3Init[0];
    switch(MP3_Status)
    {
    case MP3_Pass:
      HD44780_StrShow(1, 1,  "  IAR Systems   ");
      HD44780_StrShow(1, 2,  "  MP3 Player    ");
      break;
    case MP3_WrongRev:
      HD44780_StrShow(1, 1,  "  Unsupported   ");
      HD44780_StrShow(1, 2,  "  MP3 module    ");
      break;
    default:
      HD44780_StrShow(1, 1,  "  Can't find    ");
      HD44780_StrShow(1, 2,  "  MP3 module    ");
    }
  }
  while(MP3_Status != MP3_Pass);

  ShowDly = LOGO_DLY;

  while(1)
  {

    if(TickSysFlag)
    {
      TickSysFlag = FALSE;
      // Display messages
      if(Line2Update)
      {
        Line2Update = 0;
        if(!FatOpen)
        {
          // show special symbols PLAY, WideStereo, Loudness
          Line2[13] = ' ';
          Line2[14] = ' ';
          Line2[15] = ' ';
          switch(SoundEffect)
          {
          case WideStereoEffect:
            Line2[14] = WIDE_STEREO_CH;
            break;
          case LouldEffect:
            Line2[13] = 'L';
            Line2[14] = 'D';
          }
          if(Play)
          {
            Line2[15] = PLAY_CH;
          }
        }
        HD44780_StrShow(1, 2,  Line2);
      }

      // Implement the volume control
      if(ADC_GetFlagStatus(ADC_FLAG_ECV) == SET)
      {
        // Get the conversion value
        Tmp = ((ADC_GetConversionValue(ADC_Channel_5) >> 5) & 0x1F);
        Tmp = Adc2Vol[Tmp];
        if(Volume != Tmp)
        {
          // Update volume value and set flag (bit31)
          Volume = Tmp | 0x80000000;
        }
        // Clear the end of conversion flag
        ADC_ClearFlag(ADC_FLAG_ECV);
      }

      if(ShowDly)
      {
        --ShowDly;
      }
      else
      {
        ShowDly = REPEAT_DLY;
        // Update MMC/SD card status
        if (!Play)
        {
          MmcStatusUpdate();
        }
        pDiskCtrlBlk_t pMMCDiskStatus = MmcDiskStatus();
        if(pMMCDiskStatus->DiskStatus == DiskCommandPass)
        {
          if (FatOpen)
          {
            FatOpen = efs_init(&efsl,NULL);
            if (FatOpen == 0)
            {
              // free memory if this is necessary
              while (Deep)
              {
                pDirTemp = pCurrDir;
                pCurrDir = pCurrDir->pPrev;
                free(pDirTemp);
                --Deep;
              }
              // Open the root directory
              ls_openDir( &list, &(efsl.myFs) , pCurrDir->DirName);
              FileOpen = 0;
              GenreteKeyPress(KeyNextMask);
            }
            else
            {
              // Not valid file system
              strcpy((char *)Line2, "Pls, Insert Card");
              Line2Update = 1;
            }
          }
        }
        else
        {
          // Can't find a card
          strcpy((char *)Line2, "Pls, Insert Card");
          Line2Update = 1;
          FatOpen = -1;
        }
      }
    }

    // User Interface
    KeyImpl();
    PressedKey = GetKeys();

    if (!FatOpen)
    {
      // valid a file systems
      if (Play)
      {
        // Play a file
        if (PressedKey.PlayStop)
        {
          Play = 0;
          FatOpen = file_fclose(&Mp3File);
          FileOpen = 0;
          FillSize = 0;
          // stop play
          Mp3SendCmd(Mp3CmdPlayStop,&Tmp);
          Line2[15] = ' ';
          Line2Update = 1;
        }
        else if (PressedKey.Next)
        {
          // change sound effects
          ++SoundEffect;
          switch(SoundEffect)
          {
          case WideStereoEffect:
            Tmp = 1;
            Mp3SendCmd(Mp3CmdWideStereo,&Tmp);
            break;
          case LouldEffect:
            Tmp = 0;
            Mp3SendCmd(Mp3CmdWideStereo,&Tmp);
            Tmp = 1;
            Mp3SendCmd(Mp3CmdLoudness,&Tmp);
            break;
          default:
            Tmp = 0;
            Mp3SendCmd(Mp3CmdWideStereo,&Tmp);
            Mp3SendCmd(Mp3CmdLoudness,&Tmp);
            SoundEffect = NoSoundEffect;
          }
          Line2Update = 1;
        }
        else
        {
          if(!FileOpen && !FillSize)
          {
            // file open
            if((FatOpen = file_fopen(&Mp3File,&(efsl.myFs),(char *)list.currentEntry.FileName,MODE_READ)) == 0)
            {
              // Fill a buffer and send the first chunk to the MP3 decoder
              FileOpen = 1;
              FillSize = 0;
              FillBlockOffset = 0;
              DrainBlockOffset = 0;
              DrainSizeHold = 0;
              Line2[15] = PLAY_CH;
              Line2Update = 1;
              if((Mp3Stream.Size = file_read(&Mp3File,sizeof(Mp3Buffer),(pInt8U)Mp3Buffer)) != 0)
              {
                Mp3Stream.pStream = Mp3Buffer;
                Mp3Stream.PlaySpeed = Mp3PlayNorm;
                Mp3SendCmd(Mp3CmdPlay,(pInt32U)&Mp3Stream);
              }
            }
          }
          else
          {
            // Fill a buffer
            if (FileOpen && (FillSize < 1024))
            {
              Tmp = file_read(&Mp3File,512,(pInt8U)(FillBlockOffset?(Mp3Buffer+(512/sizeof(Mp3Buffer[0]))):Mp3Buffer));
              FillSize += Tmp;
              if(Tmp == 512)
              {
                FillBlockOffset ^= 1;
              }
              else
              {
                // End of file
                FileOpen = 0;
                file_fclose(&Mp3File);
                // find next MP3 file
                while(1)
                {
                  // open next MP3 file
                  if (ls_getNext(&list) == 0)
                  {
                    if (!(list.currentEntry.Attribute & (ATTR_VOLUME_ID | ATTR_DIRECTORY | ATTR_SYSTEM | ATTR_HIDDEN )))
                    {

                      // MP3 file is found
                      list.currentEntry.FileName[12-1] = '\0';
                      // prepare file name
                      if(!strcmp((char const *)&list.currentEntry.FileName[12-1-3],"MP3"))
                      {
                        pStr = (char*)strchr((const char*)list.currentEntry.FileName, ' ');
                        if(pStr != NULL)
                        {
                          *pStr = 0;
                        }
                        else
                        {
                          pStr = (char*)list.currentEntry.FileName;
                          list.currentEntry.FileName[12-1-3] = 0;
                        }
                        strcat(pStr, ".MP3");
                        strcpy((char*)Line2,(const char*)list.currentEntry.FileName);
                        while(strlen((const char *)Line2) < 16)
                        {
                          strcat((char *)Line2, " ");
                        }
                        break;
                      }
                    }
                  }
                  else
                  {
                    list.cEntry = 0xFFFF;
                    list.rEntry = 0;
                  }
                }
              }
            }
            // Drain a buffer
            Mp3SendCmd(Mp3CmdPlayStatus,&Tmp);
            // Wait until data was send
            if(Tmp != Mp3DataTransferProgress)
            {
              FillSize -= DrainSizeHold;
              if(FillSize)
              {
                DrainSizeHold = MIN(FillSize,512);
                // calculate the offset
                Mp3Stream.pStream   = DrainBlockOffset?(Mp3Buffer+(512/sizeof(Mp3Buffer[0]))):Mp3Buffer;
                Mp3Stream.Size      = DrainSizeHold;
                Mp3Stream.PlaySpeed = Mp3PlayNorm;
                Mp3SendCmd(Mp3CmdPlay,(pInt32U)&Mp3Stream);
                DrainBlockOffset ^= 1;
              }
              else
              {
                // The buffer is empty when end of file is reached or some error is appear
                Mp3SendCmd(Mp3CmdPlayStop,&Tmp);
              }
            }

          }
        }
      }
      else
      {
        // Select a file
        if (PressedKey.PlayStop)
        {
          if (Dir)
          {
            pDirTemp = malloc(sizeof(DirTree_t));
            pDirTemp->pPrev = pCurrDir;
            pCurrDir = pDirTemp;
            strcpy(pCurrDir->DirName,(const char*)list.currentEntry.FileName);
            ls_openDir( &list, &(efsl.myFs) , pCurrDir->DirName);
            // enter in dir
            GenreteKeyPress(KeyNextMask);
            ++Deep;
          }
          else if (PervDir)
          {
            pDirTemp = pCurrDir;
            pCurrDir = pDirTemp->pPrev;
            free(pDirTemp);
            ls_openDir( &list, &(efsl.myFs) , pCurrDir->DirName);
            // exit from dir
            GenreteKeyPress(KeyNextMask);
            --Deep;
          }
          else
          {
            // Play file
            Play = 1;
          }
        }
        else if (PressedKey.Next)
        {
          PervDir = Dir = 0;
          while(1)
          {
            // open next directory or mp3 file
            if (ls_getNext(&list) == 0)
            {
              if (list.currentEntry.Attribute & ATTR_DIRECTORY)
              {
                // Directory is found
                Dir = 1;
                list.currentEntry.FileName[12-1] = '\0';
                strcpy((char*)Line2,(const char*)list.currentEntry.FileName);
                break;
              }
              else if (!(list.currentEntry.Attribute & (ATTR_VOLUME_ID | ATTR_SYSTEM | ATTR_HIDDEN)))
              {
                // MP3 file is found
                list.currentEntry.FileName[12-1] = '\0';

                if(!strcmp((char const *)&list.currentEntry.FileName[12-1-3],"MP3"))
                {
                  pStr = (char*)strchr((const char*)list.currentEntry.FileName, ' ');
                  if(pStr != NULL)
                  {
                    *pStr = 0;
                  }
                  else
                  {
                    pStr = (char*)list.currentEntry.FileName;
                    list.currentEntry.FileName[12-1-3] = 0;
                  }
                  strcat(pStr, ".MP3");
                  strcpy((char*)Line2,(const char*)list.currentEntry.FileName);
                  break;
                }
              }
            }
            else
            {
              if (Deep)
              {
                // show up dir entry
                strcpy((char*)Line2,"../             ");
                // return to begin of the list of the current open directory
                PervDir = 1;
              }
              else
              {
                // if media is empty
                strcpy((char*)Line2,"Root/            ");
                GenreteKeyPress(KeyNextMask);
              }
              list.cEntry = 0xFFFF;
              list.rEntry = 0;
              break;
            }
          }
          while(strlen((const char *)Line2) < 16)
          {
            strcat((char *)Line2, " ");
          }
          Line2Update = 1;
        }
      }
    }
    else
    {
      if(Play)
      {
        Mp3SendCmd(Mp3CmdPlayStop,&Tmp);
      }
      Dir = PervDir = Play = 0;
    }

    // Volume control
    Mp3SendCmd(Mp3CmdPlayStatus,&Tmp);
    // Wait until data was send
    if((Tmp != Mp3DataTransferProgress) &&
       (Volume & 0x80000000))
    {
      // Set the new volume value
      Mp3SendCmd(Mp3CmdSetVol,&Volume);
      Volume &= ~0x80000000;
    }
  }
}
