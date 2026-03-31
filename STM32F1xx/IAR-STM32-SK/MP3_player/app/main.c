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
 * to develop code for the IAR STM32-SK evaluation boards and MOD-MP3 module.
 *  It implements a MP3 player. The MOD-MP3 module must be connected to the
 * UEXT connector of the IAR STM32-SK board. The player can read MMC/SD media
 * formatted with FAT12/16/32 file system.
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *
 *  Buttons:
 *   B1 - Next a file/dir (select mode)
 *   B1 - Sound effects None, Wide Stereo, Loudness (play mode)
 *   B2 - Enter/exit form directory or Play/Stop
 *  Potentiometers:
 *   AN_TR - volume control
 *
 *    $Revision: 1475 $
 *
 **************************************************************************/
#include    "includes.h"

#define TICK_PER_SEC  25
#define DLY_100US     450

#define LOGO_DLY              (2   * TICK_PER_SEC)  // 2 sec
#define REPEAT_DLY    (Int32U)(0.5 * TICK_PER_SEC)  // 0.5 sec

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
Int32U CriticalSecCntr;

/*************************************************************************
 * Function Name: Tim3Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 3 interrupt handler
 *
 *************************************************************************/
void Tim3Handler (void)
{
  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
  TickSysFlag = TRUE;
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
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
NVIC_InitTypeDef NVIC_InitStructure;

  // Timer3 Init
  // Enable Timer3 clock and release reset
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3,DISABLE);

  // Set timer period 100 msec
  TIM_TimeBaseInitStruct.TIM_Prescaler = 720;  // 10us resolution
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = 100000/TICK_PER_SEC;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);

  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
  // Enable update interrupt
  TIM_ITConfig(TIM3,TIM_FLAG_Update,ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // Enable timer counting
  TIM_Cmd(TIM3,ENABLE);
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
GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef   ADC_InitStructure;

  // ADC init
  // ADC Deinit
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  ADC_DeInit(ADC1);

  // RC5 - analog input
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init (GPIOC, &GPIO_InitStructure);

  // ADC Structure Initialization
  ADC_StructInit(&ADC_InitStructure);

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  // Enable the ADC
  ADC_Cmd(ADC1, ENABLE);

  // ADC calibration
  // Enable ADC1 reset calibration register
  ADC_ResetCalibration(ADC1);
  // Check the end of ADC1 reset calibration register
  while(ADC_GetResetCalibrationStatus(ADC1) == SET);

  // Start ADC1 calibration
  ADC_StartCalibration(ADC1);
  // Check the end of ADC1 calibration
  while(ADC_GetCalibrationStatus(ADC1) == SET);

  // Configure channel
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5);

  // Start the conversion
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

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

  ENTR_CRT_SECTION();

  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  // NVIC init
#ifndef  EMB_FLASH
  // Set the Vector Table base location at 0x20000000
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  // VECT_TAB_FLASH
  // Set the Vector Table base location at 0x08000000
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

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
  SdDiskInit();

  // Init keys
  KeyInit();

  // Volume control init
  AdcInit();

  // init MP3 module

  do
  {
    Mp3SendCmd(Mp3CmdPowerUp,(pInt32U)&MP3_Status);
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
      KeyImpl();
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
      if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)
      {
        // Get the conversion value
        Tmp = (ADC_GetConversionValue(ADC1) >> 7) & 0x1F;
        Tmp = Adc2Vol[Tmp];
        if(Volume != Tmp)
        {
          // Update volume value and set flag (bit31)
          Volume = Tmp | 0x80000000;
        }
        // Start conversion
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
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
          SdStatusUpdate();
        }
        pDiskCtrlBlk_t pSD_DiskStatus = SdGetDiskCtrlBkl();
        if(pSD_DiskStatus->DiskStatus == DiskCommandPass)
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
    // Wait until data was send
    if(Volume & 0x80000000)
    {
      // Set the new volume value
      Mp3SendCmd(Mp3CmdSetVol,&Volume);
      Volume &= ~0x80000000;
    }
  }
}
