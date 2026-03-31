/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : October, 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    1. Date        : August, 2015
 *       Author      : Atanas Uzunov
 *       Description : Adapted for IAR-STM32F746xx-SK. Changed resources
 *                     load/link location to QSPI.
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F746xx-SK board. It shows basic
 * use of the parallel I/O, the timer, the interrupt controller, the LCD
 * controller and the I2S(SPI) module.
 *  The I2S module is connected to external Wolfson WM8731 audio codec, and is
 * configured for 44.1kHz / Stereo mode/playback. User can choose from 4
 * possible inputs (tone generator, microphone, line-in, and wav file playback).
 * Also it is possible to set up/down/mute the volume.
 *
 *  The background file and the audio file are combined into the
 * "res.bin" file with the command:
 *
 *  copy /b iar_globe_logo.png+iar_disco_2b_mixdown_stereo.wav res.bin
 *
 *  The resources are loaded to the onboard QSPI flash memory.
 *
 *  The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/
 *
 *  The music piece used in this example is composed and recorded by some
 * developers at IAR Systems.
 *
 * COMPATIBILITY
 * =============
 *  The audio demo example project is compatible with IAR-STM32F746xx-SK
 * evaluation board. By default, the project is configured to use the
 * I-Jet JTAG/SWD interface.
 *
 * CONFIGURATION
 * =============
 * Flash Debug - The Progam is loaded to internal Flash.
 *
 *  Make sure that the following jumpers are correctly configured on the
 * IAR-STM32F746xx-SK evaluation board:
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   B0_1/B0_0       - B0_0
 *   RST_E           - open
 *   RXD3(BOOT)/RXD2 - RXD2
 *   TXD3(BOOT)/TXD2 - TXD2
 *
 *    $Revision: 3190 $
 **************************************************************************/
#include "includes.h"

#define SOUND_VOL_STEPS       10
#define SOUND_VOL_MIN         (WM8731_LHO_LHPVOL_Min-1)
#define SOUND_VOL_MAX         (WM8731_LHO_LHPVOL_Max)
#define SOUND_VOL_STEP       ((SOUND_VOL_MAX-SOUND_VOL_MIN)/SOUND_VOL_STEPS)
#define WM8731_VOL_CONV(x)    (SOUND_VOL_MIN + SOUND_VOL_STEP*x)

#define SINE_BUFFER_SIZE      441

#define PI                    3.14159265

#define REC_BUFFER_TIME       1

#define REC_BUFFER_SIZE       (44100*REC_BUFFER_TIME)

#define REC_BUFFER_ALLOW_PLAY (REC_BUFFER_SIZE-100)

extern uint8_t __resources_bin[];

#define RES_IAR_LOGO         (__resources_bin)
#define RES_IAR_LOGO_SIZE     47892

#define RES_WAV_FILE         (__resources_bin + RES_IAR_LOGO_SIZE)
#define RES_WAV_FILE_SIZE     1411244

#define RES_WAV_DATA_SIZE    *((uint32_t*)(RES_WAV_FILE + 40))
#define RES_WAV_DATA         (RES_WAV_FILE + 44)

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

static volatile uint32_t TimingDelay;

/* variable for critical section entry control */
uint32_t CriticalSecCntr;

QSPI_HandleTypeDef QSPIHandle;
UART_HandleTypeDef UartHandle;
I2S_HandleTypeDef  I2S_IN_Handle;
I2S_HandleTypeDef  I2S_OUT_Handle;
I2C_HandleTypeDef  I2cHandle;

volatile AudioChannelSample16b_t * SineData;
volatile AudioSample16b_t * RecData;
volatile PlaybackState_t playState;
volatile uint32_t recPtr, playPtr;
volatile Boolean allowPlay;

/*************************************************************************
 * Function Name: getJoystick
 * Parameters: none
 *
 * Return: none
 *
 * Description: Returns the new changed joystick state
 *************************************************************************/
#define DEBOUNCE_THRESHOLD 3
JoystickState_t getJoystick(void)
{
static JoystickState_t oldState = joyIdle;
JoystickState_t newState;
static uint32_t deb_counter = DEBOUNCE_THRESHOLD;

  if (GPIO_PIN_SET == STM_ButtonGetState(BUTTON_UP))
    newState = joyUp;
  else
  if (GPIO_PIN_SET == STM_ButtonGetState(BUTTON_DOWN))
    newState = joyDown;
  else
  if (GPIO_PIN_SET == STM_ButtonGetState(BUTTON_LEFT))
    newState = joyLeft;
  else
  if (GPIO_PIN_SET == STM_ButtonGetState(BUTTON_RIGHT))
    newState = joyRight;
  else
  if ((GPIO_PIN_SET == STM_ButtonGetState(BUTTON_SEL)) || (GPIO_PIN_RESET == STM_ButtonGetState(BUTTON_USER)))
    newState = joyCenter;
  else
    newState = joyIdle;

  if (newState == oldState)
  {
    if (deb_counter) deb_counter--;
    else return newState;
  }
  else
  {
    deb_counter = DEBOUNCE_THRESHOLD;
    oldState = newState;
  }
  return joyIdle;
}

/*************************************************************************
 * Function Name: GenerateSineWave
 * Parameters: none
 *
 * Return: none
 *
 * Description: Generates 1 kHz sine wave array
 *************************************************************************/
void GenerateSineWave(void)
{
  for (uint32_t i = 0; i<SINE_BUFFER_SIZE; i++) *(SineData+i) = (AudioChannelSample16b_t)(sin(10 * (2 * PI) * i / SINE_BUFFER_SIZE) * 28000);
}

/*************************************************************************
 * Function Name: GetSineSample
 * Parameters: none
 *
 * Return: AudioChannelSample16b_t
 *
 * Description: Return the next audio sample from an array containing
 *              1 kHz sine wave
 *************************************************************************/
AudioChannelSample16b_t GetSineSample(void){
  static uint32_t sinePtr=0;
  static AudioChannelSample16b_t sample;
  sample  = *(SineData + sinePtr);
  if (!__HAL_I2S_GET_FLAG(&I2S_OUT_Handle, I2S_FLAG_CHSIDE))
  {
    /* If both channels sended */
    if (++sinePtr >= SINE_BUFFER_SIZE) sinePtr = 0;
  }
  return sample;
}

/*************************************************************************
 * Function Name: GetRecSample
 * Parameters: none
 *
 * Return: AudioChannelSample16b_t
 *
 * Description: Return the next audio sample from the recording array
 *************************************************************************/
AudioChannelSample16b_t GetRecSample(void){
  AudioChannelSample16b_t sample;
  if (__HAL_I2S_GET_FLAG(&I2S_IN_Handle, I2S_FLAG_CHSIDE))
  {
    /* Right Channel */
    sample = (RecData + playPtr)->rightChannel;
  }
  else
  {
    /* Left Channel */
    sample = (RecData + playPtr)->leftChannel;
    /* Both channels fetched, increment pointer */
    if (++playPtr >= REC_BUFFER_SIZE) playPtr = 0;
  }
  return sample;
}

/*************************************************************************
 * Function Name: SaveRecSample
 * Parameters: AudioChannelSample16b_t sample
 *
 * Return: uint32_t - pointer in the used circular buffer
 *
 * Description: Save an audio sample to the recording array
 *************************************************************************/
uint32_t SaveRecSample(AudioChannelSample16b_t sample){
  if (__HAL_I2S_GET_FLAG(&I2S_IN_Handle, I2S_FLAG_CHSIDE))
  {
    /* Save Right Channel */
    (RecData + recPtr)->rightChannel = sample;
  }
  else
  {
    /* Save Left Channel */
    (RecData + recPtr)->leftChannel = sample;
    if (++recPtr >= REC_BUFFER_SIZE) recPtr = 0;
  }
  return recPtr;
}

/*************************************************************************
 * Function Name: GetWavSample
 * Parameters: none
 *
 * Return: AudioChannelSample16b_t
 *
 * Description: Fetch next audio sample from the WAV data
 *************************************************************************/
AudioChannelSample16b_t GetWavSample(void){
  AudioChannelSample16b_t sample;
  if (__HAL_I2S_GET_FLAG(&I2S_OUT_Handle, I2S_FLAG_CHSIDE))
  {
    /* Right Channel */
    sample = ((AudioSample16b_t*)RES_WAV_DATA+playPtr)->rightChannel;
  }
  else
  {
    /* Left Channel */
    sample = ((AudioSample16b_t*)RES_WAV_DATA+playPtr)->leftChannel;
    if (++playPtr >= RES_WAV_DATA_SIZE/sizeof(AudioSample16b_t)) playPtr = 0;
  }
  return sample;
}

/*************************************************************************
 * Function Name: I2S_OUT_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: I2S Output IRQ Handler
 *************************************************************************/
void I2S_OUT_Handler(void)
{
  /* Handle TX Buffer empty interrupt */
  if (SET == __HAL_I2S_GET_FLAG(&I2S_OUT_Handle, I2S_FLAG_TXE))
  {
    if(stGenerateSine == playState)
    {
      (&I2S_OUT_Handle)->Instance->DR = GetSineSample();
    }
    else if((stLoopbackMic == playState) || (stLoopbackLineIn == playState))
    {
      /* If playback is allowed, fetch the next audio sample, otherwise send 0 */
      (&I2S_OUT_Handle)->Instance->DR = (allowPlay)?GetRecSample():0;
    }
    else if(stPlayWave == playState)
    {
      /* Fetch the next wav audio sample */
      (&I2S_OUT_Handle)->Instance->DR = GetWavSample();
    }
  }
}

/*************************************************************************
 * Function Name: I2S_IN_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: I2S Input IRQ Handler
 *************************************************************************/
void I2S_IN_Handler(void)
{
  /* Handle RX Buffer not empty interrupt */
  if (SET == __HAL_I2S_GET_FLAG(&I2S_IN_Handle, I2S_FLAG_RXNE))
  {
    if((stLoopbackMic == playState) || (stLoopbackLineIn == playState))
    {
      /* Save the received sample. If enough samples are saved, allow the playback */
      if (SaveRecSample((&I2S_IN_Handle)->Instance->DR) > REC_BUFFER_ALLOW_PLAY) allowPlay = TRUE;
    }
    else
    {
      /* Dummy Read of input buffer to clear the interrupt flag */
      (&I2S_IN_Handle)->Instance->DR;
    }
  }
}

/*************************************************************************
 * Function Name: InitPlaybackState
 * Parameters: PlaybackState_t state
 *
 * Return: none
 *
 * Description: Initialize the playback state
 *************************************************************************/
void InitPlaybackState(PlaybackState_t state)
{
  switch(state)
  {
    case stGenerateSine:
      WM8731_Mic_Mute(&I2cHandle, TRUE);
      WM8731_LineIn_Mute(&I2cHandle, TRUE);
      break;

    case stLoopbackMic:
      WM8731_Mic_Mute(&I2cHandle, FALSE);
      WM8731_LineIn_Mute(&I2cHandle, TRUE);
      WM8731_InputSelectMic(&I2cHandle);
      allowPlay = 0;
      playPtr = 0;
      recPtr = 0;
      break;

    case stLoopbackLineIn:
      WM8731_Mic_Mute(&I2cHandle, TRUE);
      WM8731_LineIn_Mute(&I2cHandle, FALSE);
      WM8731_LineIn_Volume(&I2cHandle, WM8731_RLIN_RINVOL_Max);
      WM8731_InputSelectLineIn(&I2cHandle);
      allowPlay = 0;
      playPtr = 0;
      recPtr = 0;
      break;

    case stPlayWave:
      WM8731_Mic_Mute(&I2cHandle, TRUE);
      WM8731_LineIn_Mute(&I2cHandle, TRUE);
      playPtr = 0;
      break;

    default:break;
  }
}

/***  **********************************************************************
 * Function Name: UI_UpdateInput
 * Parameters: PlaybackState_t state
 *
 * Return: none
 *
 * Description: Set the Input label on the LCD
 *************************************************************************/
void UI_UpdateInput(PlaybackState_t state)
{
  GLCD_SetWindow(30,245,269,255,Layer1);
  GLCD_TextSetPos(0,0,Layer1);
  GLCD_print(Layer1, "\f INPUT: ");
  switch(state)
  {
    case stGenerateSine:
      GLCD_print(Layer1, "1kHz SINE WAVE");
      break;
    case stLoopbackMic:
      GLCD_print(Layer1, "MICROPHONE (DELAY-EFFECT)");
      break;
    case stLoopbackLineIn:
      GLCD_print(Layer1, "LINE-IN (DELAY-EFFECT)");
      break;
    case stPlayWave:
      GLCD_print(Layer1, "WAVE FILE LOOPED PLAYBACK");
      break;
  }
}

/*************************************************************************
 * Function Name: UI_UpdateVolume
 * Parameters: uint8_t volume
 *             Boolean muted
 *
 * Return: none
 *
 * Description: Set the Volume label on the LCD
 *************************************************************************/
void UI_UpdateVolume(uint8_t volume, Boolean muted)
{
  GLCD_SetWindow(30,230,269,240,Layer1);
  GLCD_TextSetPos(0,0,Layer1);
  GLCD_print(Layer1, "\fVOLUME: %i%", volume*10);
  if(muted) GLCD_print(Layer1, " (MUTE)");
}

/*************************************************************************
 * Function Name: UI_UpdateHelp
 * Parameters: none
 *
 * Return: none
 *
 * Description: Set the Help instructions on the LCD
 *************************************************************************/
void UI_UpdateHelp(void)
{
  GLCD_SetWindow(340,190,479,200,Layer1);
  GLCD_TextSetPos(0,0,Layer1);
  GLCD_print(Layer1, "\fJOYSTICK CONTROLS\r\n\r\n");
  GLCD_SetWindow(320,210,479,271,Layer1);
  GLCD_print(Layer1, "\f        UP: Volume Up\r\n");
  GLCD_print(Layer1, "      DOWN: Volume Down\r\n");
  GLCD_print(Layer1, "LEFT/RIGHT: Input Select\r\n");
  GLCD_print(Layer1, "    CENTER: Mute On/Off");
}

/*************************************************************************
 * Function Name: QSPI_WriteEnable
 * Parameters: QSPI_HandleTypeDef *hqspi
 *
 * Return: none
 *
 * Description: This function send a Write Enable and wait it is effective.
 *************************************************************************/
static void QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi)
{
QSPI_CommandTypeDef     sCommand;
QSPI_AutoPollingTypeDef sConfig;

  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = WRITE_ENABLE_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    while(1);
  }

  /* Configure automatic polling mode to wait for write enabling ---- */
  sConfig.Match           = 0x02;
  sConfig.Mask            = 0x02;
  sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
  sConfig.StatusBytesSize = 1;
  sConfig.Interval        = 0x10;
  sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  sCommand.Instruction    = READ_STATUS1_REG_CMD;
  sCommand.DataMode       = QSPI_DATA_1_LINE;

  if (HAL_QSPI_AutoPolling(hqspi, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    while(1);
  }
}

/*************************************************************************
 * Function Name: QSPI_QuadModeEnable
 * Parameters: QSPI_HandleTypeDef *hqspi
 *
 * Return: none
 *
 * Description: This function configures the SPI memory in QSPI mode.
 *************************************************************************/
static void QSPI_QuadModeEnable(QSPI_HandleTypeDef *hqspi)
{
QSPI_CommandTypeDef sCommand;
uint8_t reg[2];

  /* Read Configuration register SR-2 ------------------------------- */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = READ_STATUS2_REG_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_1_LINE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
  sCommand.NbData            = 1;

  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    while(1);
  }

  if (HAL_QSPI_Receive(hqspi, &reg[1], HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    while(1);
  }

  /* Return if QE bit is already set */
  if (reg[1] & 0x02) return;

  /* else */

  /* Read Configuration register SR-1 ------------------------------- */
  sCommand.Instruction       = READ_STATUS1_REG_CMD;

  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    while(1);
  }

  if (HAL_QSPI_Receive(hqspi, &reg[0], HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    while(1);
  }

  /* Enable write operations ---------------------------------------- */
  QSPI_WriteEnable(hqspi);

  /* Write Configuration register SR-1 and SR-2 (QE bit set in SR-2)  */
  sCommand.Instruction       = WRITE_STATUS_REG_CMD;
  sCommand.NbData            = 2;
  reg[1] = 0x02;

  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    while(1);
  }

  if (HAL_QSPI_Transmit(hqspi, &reg[0], HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    while(1);
  }
}

/*************************************************************************
 * Function Name: QSPI_AutoPollingMemReady
 * Parameters: QSPI_HandleTypeDef *hqspi
 *
 * Return: none
 *
 * Description: This function read the SR-1 of the memory and wait the EOP.
 *************************************************************************/
static void QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi)
{
QSPI_CommandTypeDef     sCommand;
QSPI_AutoPollingTypeDef sConfig;

  /* Configure automatic polling mode to wait for memory ready ------ */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = READ_STATUS1_REG_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_1_LINE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  sConfig.Match           = 0x00;
  sConfig.Mask            = 0x01;
  sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
  sConfig.StatusBytesSize = 1;
  sConfig.Interval        = 0x10;
  sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_QSPI_AutoPolling(hqspi, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    while(1);
  }
}

/*************************************************************************
 * Function Name: SysTickHandler
 * Parameters: void
 * Return: void
 *
 * Description: SysTick interrupt handler
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 200000000
 *            HCLK(Hz)                       = 200000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 400
 *            PLL_P                          = 2
 *            PLL_Q                          = 8
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 6
 *************************************************************************/
void SystemClock_Config(void)
{
RCC_ClkInitTypeDef RCC_ClkInitStruct;
RCC_OscInitTypeDef RCC_OscInitStruct;
HAL_StatusTypeDef ret = HAL_OK;
RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    /* catch error */
    while(1);
  }

  /* Activate the OverDrive to reach the 200 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    /* catch error */
    while(1);
  }

  /* Select PLLSAI output as USB clock source, LTDC(R) = 9.6MHz
     Select PLLI2S output as I2S clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48 | RCC_PERIPHCLK_LTDC | RCC_PERIPHCLK_I2S | RCC_PERIPHCLK_PLLI2S;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.I2sClockSelection = RCC_I2SCLKSOURCE_PLLI2S;

  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 8;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
  PeriphClkInitStruct.PLLSAIDivQ = 0;

  PeriphClkInitStruct.PLLI2S.PLLI2SN = 271;
  PeriphClkInitStruct.PLLI2S.PLLI2SQ = 4;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  PeriphClkInitStruct.PLLI2S.PLLI2SP = 1;
  PeriphClkInitStruct.PLLI2SDivQ = 1;

  ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  if(ret != HAL_OK)
  {
    /* catch error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if(ret != HAL_OK)
  {
    /* catch error */
    while(1);
  }

}

/*************************************************************************
 * Function Name: CPU_CACHE_Enable
 * Parameters: none
 *
 * Return: none
 *
 * Description: Enables the CPU cache
 *************************************************************************/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
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
int main(void)
{
pPic_t pPic;
Boolean muted = FALSE;
uint8_t volume;
JoystickState_t joystickState, joystickPrevState = joyIdle;

QSPI_CommandTypeDef      sCommand;
QSPI_MemoryMappedTypeDef sMemMappedCfg;

  /* Enable Cache */
  CPU_CACHE_Enable();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
  */
  HAL_Init();

  /* Configure MPU */
  MPU_RegionConfig();

  /* Configure the system clock to 200 MHz, LTDC to 9.6 MHz */
  SystemClock_Config();

  /* LCD backlight disable */
  STM_BacklightInit();

  /* SDRAM init */
  SDRAM_Init();

  /* Initialize QuadSPI structures ------------------------------------------- */
  QSPIHandle.Instance = QUADSPI;

  /* ClockPrescaler set to 2, so QSPI clock = 200 MHz / (2+1) = 66 MHz */
  QSPIHandle.Init.ClockPrescaler     = 2;
  QSPIHandle.Init.FifoThreshold      = 4;
  QSPIHandle.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_NONE;
  QSPIHandle.Init.FlashSize          = QSPI_FLASH_SIZE;
  QSPIHandle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  QSPIHandle.Init.ClockMode          = QSPI_CLOCK_MODE_0;

  /* Initialize QuadSPI ------------------------------------------------ */
  HAL_QSPI_DeInit(&QSPIHandle);
  if (HAL_QSPI_Init(&QSPIHandle) != HAL_OK)
  {
    while(1);
  }

  /* Wait until flash memory is ready */
  QSPI_AutoPollingMemReady(&QSPIHandle);

  /* Set flash memory to QSPI mode */
  QSPI_QuadModeEnable(&QSPIHandle);

  /* Wait until flash memory is ready */
  QSPI_AutoPollingMemReady(&QSPIHandle);

  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
  sCommand.DataMode          = QSPI_DATA_4_LINES;
  sCommand.Instruction       = QUAD_OUT_FAST_READ_CMD;
  sCommand.DummyCycles       = 8;

  sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;

  /* Set QSPI controller to Memory Mapped mode */
  if (HAL_QSPI_MemoryMapped(&QSPIHandle, &sCommand, &sMemMappedCfg) != HAL_OK)
  {
    while(1);
  }

  /* Initialize buttons */
  STM_ButtonInit(BUTTON_USER, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUTTON_LEFT, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUTTON_SEL, BUTTON_MODE_GPIO);

  /* Init I2S peripheral */
  if (STM_I2SInit(&I2S_OUT_Handle, &I2S_IN_Handle))
  {
    printf("I2S init fail!\r\n");
    return 1;
  };

  /* Disable I2S interrupts until everything is configured */
  HAL_NVIC_DisableIRQ(I2S_OUT_IRQ);
  HAL_NVIC_SetPriority(I2S_OUT_IRQ, 0, 0);

  HAL_NVIC_DisableIRQ(I2S_IN_IRQ);
  HAL_NVIC_SetPriority(I2S_IN_IRQ, 1, 0);

  /* Enable the I2S Module Interrupts */
  __HAL_I2S_ENABLE_IT(&I2S_OUT_Handle, I2S_IT_TXE);
  __HAL_I2S_ENABLE_IT(&I2S_IN_Handle, I2S_IT_RXNE);

  /* Reserve space for playback buffers */
  SineData = malloc(SINE_BUFFER_SIZE*sizeof(AudioChannelSample16b_t));
  RecData = malloc(REC_BUFFER_SIZE*sizeof(AudioSample16b_t));
  if((SineData == NULL) || (RecData == NULL))
  {
    while(1);
  }

  /* Load/decode picture from png file to SDRAM */
  if(NULL == (pPic = GLCD_LoadPNG(RES_IAR_LOGO, RES_IAR_LOGO_SIZE)))
  {
    while(1);
  }

  /* Init LCD and copy picture to VRAM */
  GLCD_Init (pPic, NULL);
  GLCD_Ctrl (TRUE);
  GLCD_SetFont(&Terminal_18_24_12, GLCD_COLOR_BLACK, GLCD_COLOR_IAR_YELLOW, Layer1);
  GLCD_SetWindow(30,20,469,60,Layer1);
  GLCD_TextSetPos(3,0,Layer1);
  GLCD_print(Layer1, "IAR-STM32F746xx-SK AUDIO DEMO");
  GLCD_SetFont(&Terminal_9_12_6, GLCD_COLOR_BLACK, GLCD_COLOR_IAR_YELLOW, Layer1);
  STM_BacklightOn();

  I2cHandle.Instance             = I2C2;
  I2cHandle.Init.Timing          = 0x40912732;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress1     = 0x00;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress2     = 0x00;
  I2cHandle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  I2cHandle.State                = HAL_I2C_STATE_RESET;

  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Audio Codec init */
  if(WM8731_RESULT_OK != WM8731_Init(&I2cHandle))
  {
    /* Initialization Error */
    while(1);
  }

  /* Generate Sine Wave Array */
  GenerateSineWave();

  /* Draw Help messages */
  UI_UpdateHelp();

  /* Set default volume */
  volume = 5;
  WM8731_Headphone_Volume(&I2cHandle, WM8731_VOL_CONV(volume));
  UI_UpdateVolume(volume,muted);

  /* Set default state*/
  playState = stPlayWave;

  /* Init default state*/
  InitPlaybackState(playState);
  UI_UpdateInput(playState);

  /* Enable I2S interrupts */
  HAL_NVIC_EnableIRQ(I2S_OUT_IRQ);
  HAL_NVIC_EnableIRQ(I2S_IN_IRQ);

  while(1)
  {
    HAL_Delay(10);

    joystickState = getJoystick();

    if (joystickPrevState != joystickState)
    switch(joystickState)
    {
      /* Joystick UP pressed */
      case joyUp:
        if (WM8731_VOL_CONV(volume) < SOUND_VOL_MAX) volume++;
        if (!muted) if (WM8731_Headphone_Volume(&I2cHandle, WM8731_VOL_CONV(volume))) GLCD_print(Layer1, "WM8731 Error!\r\n");
        UI_UpdateVolume(volume,muted);
        break;
      /* Joystick DOWN pressed */
      case joyDown:
        if (WM8731_VOL_CONV(volume) > SOUND_VOL_MIN) volume--;
        if (!muted) if (WM8731_Headphone_Volume(&I2cHandle, WM8731_VOL_CONV(volume))) GLCD_print(Layer1, "WM8731 Error!\r\n");
        UI_UpdateVolume(volume,muted);
        break;
      /* Joystick LEFT pressed */
      case joyLeft:
        HAL_NVIC_DisableIRQ(I2S_OUT_IRQ);
        HAL_NVIC_DisableIRQ(I2S_IN_IRQ);
        if (playState) playState--;
        InitPlaybackState(playState);
        HAL_NVIC_EnableIRQ(I2S_OUT_IRQ);
        HAL_NVIC_EnableIRQ(I2S_IN_IRQ);
        UI_UpdateInput(playState);
        break;
      /* Joystick RIGHT pressed */
      case joyRight:
        HAL_NVIC_DisableIRQ(I2S_OUT_IRQ);
        HAL_NVIC_DisableIRQ(I2S_IN_IRQ);
        if (playState < NumberOfPlaybackStates-1) playState++;
        InitPlaybackState(playState);
        HAL_NVIC_EnableIRQ(I2S_OUT_IRQ);
        HAL_NVIC_EnableIRQ(I2S_IN_IRQ);
        UI_UpdateInput(playState);
        break;
      /* Joystick CENTER/SET pressed */
      case joyCenter:
        if(muted)
        {
          WM8731_Headphone_Volume(&I2cHandle, WM8731_VOL_CONV(volume));
          muted = FALSE;
        }
        else
        {
          WM8731_Headphone_Volume(&I2cHandle, SOUND_VOL_MIN);
          muted = TRUE;
        }
        UI_UpdateVolume(volume,muted);
        break;
      /* Joystick idle */
      case joyIdle:
      default:
        break;
    }
    joystickPrevState = joystickState;
  }
}
