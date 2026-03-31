/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : October, 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F429II-SK board. It shows basic
 * use of the parallel I/O, the timer, the interrupt controller, the LCD
 * controller and the I2S(SPI) module.
 *  The I2S module is connected to external Wolfson WM8731 audio codec, and is
 * configured for 44.1kHz / Stereo mode/playback. User can choose from 4
 * possible inputs (tone generator, microphone, line-in, and wav file playback)
 * Also it is possible to set up/down/mute the volume.
 *
 *  The background file and the audio file are combined into the
 * "res.bin" file with the command:
 *
 *  copy /b iar_globe_logo.png+iar_disco_2b_mixdown_stereo.wav res.bin
 *
 * The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/
 *
 *  The music piece used in this example is composed and recorded by some
 * developers at IAR Systems.
 *
 * COMPATIBILITY
 * =============
 *  The audio demo example project is compatible with IAR-STM32F429II-SK
 * evaluation board. By default, the project is configured to use the
 * J-Link JTAG/SWD interface.
 *
 * CONFIGURATION
 * =============
 * Flash Debug - The Progam is loaded to internal Flash.
 *
 *  Make sure that the following jumpers are correctly configured on the
 * IAR-STM32F429II-SK evaluation board:
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   B0_1/B0_0 - B0_0
 *   B1_1/B1_0 - B1_0
 *
 *
 *    $Revision: 3190 $
 **************************************************************************/
#include "includes.h"

#define TICK_PER_SEC          30

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

#define TICK_DLY              100 /* tick at every 10ms */

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

static volatile uint32_t TimingDelay;

/* variable for critical section entry control */
uint32_t CriticalSecCntr;

volatile AudioChannelSample16b_t * SineData;
volatile AudioSample16b_t * RecData;
volatile PlaybackState_t playState;
volatile uint32_t recPtr, playPtr;
volatile Boolean allowPlay;
volatile Boolean flTick;
uint32_t intrTickTimer;

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: uint32_t Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(uint32_t Dly)
{
  TimingDelay = Dly;

  while(TimingDelay != 0);
}

/*************************************************************************
 * Function Name: SysTickHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: SysTick Handler function @ every 100us
 *
 *************************************************************************/
void SysTickHandler(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }

  if (++intrTickTimer >= TICK_DLY)
  {
    flTick = TRUE;
    intrTickTimer = 0;
  }
}

/*************************************************************************
 * Function Name: getJoystick
 * Parameters: none
 *
 * Return: none
 *
 * Description: Returns the new changed joystick state
 *
 *************************************************************************/
#define DEBOUNCE_THRESHOLD 3
JoystickState_t getJoystick(void)
{
static JoystickState_t oldState = joyIdle;
JoystickState_t newState;
static uint32_t deb_counter = DEBOUNCE_THRESHOLD;

  if (Bit_SET == STM_ButtonGetState(BUTTON_UP))
    newState = joyUp;
  else
  if (Bit_SET == STM_ButtonGetState(BUTTON_DOWN))
    newState = joyDown;
  else
  if (Bit_SET == STM_ButtonGetState(BUTTON_LEFT))
    newState = joyLeft;
  else
  if (Bit_SET == STM_ButtonGetState(BUTTON_RIGHT))
    newState = joyRight;
  else
  if (Bit_SET == STM_ButtonGetState(BUTTON_SEL))
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
 *
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
 *
 *************************************************************************/
AudioChannelSample16b_t GetSineSample(void){
  static uint32_t sinePtr=0;
  static AudioChannelSample16b_t sample;
  sample  = *(SineData + sinePtr);
  if (!SPI_I2S_GetFlagStatus(SPI2, I2S_FLAG_CHSIDE))
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
 *
 *************************************************************************/
AudioChannelSample16b_t GetRecSample(void){
  AudioChannelSample16b_t sample;
  if (SPI_I2S_GetFlagStatus(SPI2, I2S_FLAG_CHSIDE))
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
 *
 *************************************************************************/
uint32_t SaveRecSample(AudioChannelSample16b_t sample){
  if (SPI_I2S_GetFlagStatus(I2S2ext, I2S_FLAG_CHSIDE))
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
 *
 *************************************************************************/
AudioChannelSample16b_t GetWavSample(void){
  AudioChannelSample16b_t sample;
  if (SPI_I2S_GetFlagStatus(SPI2, I2S_FLAG_CHSIDE))
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
 * Function Name: I2S2Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: I2S2 IRQ Handler
 *
 *************************************************************************/
void I2S2Handler(void)
{
  /* Handle TX Buffer empty interrupt */
  if (SET == SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE))
  {
    if(stGenerateSine == playState)
    {
      SPI_I2S_SendData(SPI2,GetSineSample());
    }
    else if((stLoopbackMic == playState) || (stLoopbackLineIn == playState))
    {
      /* If playback is allowed, fetch the next audio sample, otherwise send 0 */
      SPI_I2S_SendData(SPI2,(allowPlay)?GetRecSample():0);
    }
    else if(stPlayWave == playState)
    {
      SPI_I2S_SendData(SPI2,GetWavSample());
    }
  }
  /* Handle RX Buffer not empty interrupt */
  if (SET == SPI_I2S_GetFlagStatus(I2S2ext, SPI_I2S_FLAG_RXNE))
  {
    if((stLoopbackMic == playState) || (stLoopbackLineIn == playState))
    {
      /* Save the received sample. If enough samples are saved, allow the playback */
      if (SaveRecSample(SPI_I2S_ReceiveData(I2S2ext)) > REC_BUFFER_ALLOW_PLAY) allowPlay = TRUE;
    }
    else
    {
      /* Dummy Read of input buffer to clear the interrupt flag */
      SPI_I2S_ReceiveData(I2S2ext);
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
 *
 *************************************************************************/
void InitPlaybackState(PlaybackState_t state)
{
  switch(state)
  {
    case stGenerateSine:
      WM8731_Mic_Mute(TRUE);
      WM8731_LineIn_Mute(TRUE);
      break;

    case stLoopbackMic:
      WM8731_Mic_Mute(FALSE);
      WM8731_LineIn_Mute(TRUE);
      WM8731_InputSelectMic();
      allowPlay = 0;
      playPtr = 0;
      recPtr = 0;
      break;

    case stLoopbackLineIn:
      WM8731_Mic_Mute(TRUE);
      WM8731_LineIn_Mute(FALSE);
      WM8731_LineIn_Volume(WM8731_RLIN_RINVOL_Max);
      WM8731_InputSelectLineIn();
      allowPlay = 0;
      playPtr = 0;
      recPtr = 0;
      break;

    case stPlayWave:
      WM8731_Mic_Mute(TRUE);
      WM8731_LineIn_Mute(TRUE);
      playPtr = 0;
      break;

    default:break;
  }
}

/*************************************************************************
 * Function Name: UI_UpdateInput
 * Parameters: PlaybackState_t state
 *
 * Return: none
 *
 * Description: Set the Input label on the LCD
 *
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
 *
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
 *
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

  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  ENTR_CRT_SECTION();

  /* Enable GPIOs clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                         RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                         RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG |
                         RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI, ENABLE);

  /* Init LCD backlight port */
  STM_BacklightInit();

  /*PLLI2S configure*/
  RCC_PLLI2SConfig(271,4,2);
  /*Enable PLLI2S*/
  RCC_PLLI2SCmd(ENABLE);
  /*Wait PLLI2S Lock*/
  while(RESET == RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY));
  /*PLLI2S is I2S clock source*/
  RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);

  /* Init I2S2 peripheral */
  STM_I2S2Init();

  /* Enable the I2S2 Interrupts */
  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, ENABLE);
  SPI_I2S_ITConfig(I2S2ext, SPI_I2S_IT_RXNE, ENABLE);

  /* SysTick Config */
  if(SysTick_Config(SystemCoreClock/10000))
  {
    /* Capture error */
    while (1);
  }

  EXT_CRT_SECTION();

  /* SDRAM init */
  SDRAM_Init();

  /* Reserve space for playback buffers */
  SineData = malloc(SINE_BUFFER_SIZE*sizeof(AudioChannelSample16b_t));
  RecData = malloc(REC_BUFFER_SIZE*sizeof(AudioSample16b_t));
  if((SineData == NULL) || (RecData == NULL))
  {
    while(1);
  }

  /* Disable I2S interrupts until everything is configured */
  NVIC_DisableIRQ (SPI2_IRQn);
  NVIC_SetPriority (SPI2_IRQn, 15);

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
  GLCD_print(Layer1, "IAR-STM32F429II-SK AUDIO DEMO");
  GLCD_SetFont(&Terminal_9_12_6, GLCD_COLOR_BLACK, GLCD_COLOR_IAR_YELLOW, Layer1);
  STM_BacklightOn();

  __enable_interrupt();

  /* Audio Codec init */
  I2C2_Init();
  if(WM8731_RESULT_OK != WM8731_Init())
  {
    /* Codec error */
    while(1);
  }

  /* Generate Sine Wave Array */
  GenerateSineWave();

  /* Draw Help messages */
  UI_UpdateHelp();

  /* Set default volume */
  volume = 5;
  WM8731_Headphone_Volume(WM8731_VOL_CONV(volume));
  UI_UpdateVolume(volume,muted);

  /* Set default state*/
  playState = stPlayWave;

  /* Init default state*/
  InitPlaybackState(playState);
  UI_UpdateInput(playState);

  NVIC_EnableIRQ (SPI2_IRQn);

  while(1)
  {
    if (!flTick) continue;

    flTick = FALSE;

    joystickState = getJoystick();

    if (joystickPrevState != joystickState)
    switch(joystickState)
    {
      /* Joystick UP pressed */
      case joyUp:
        if (WM8731_VOL_CONV(volume) < SOUND_VOL_MAX) volume++;
        if (!muted) if (WM8731_Headphone_Volume(WM8731_VOL_CONV(volume))) GLCD_print(Layer1, "WM8731 Error!\r\n");
        UI_UpdateVolume(volume,muted);
        break;
      /* Joystick DOWN pressed */
      case joyDown:
        if (WM8731_VOL_CONV(volume) > SOUND_VOL_MIN) volume--;
        if (!muted) if (WM8731_Headphone_Volume(WM8731_VOL_CONV(volume))) GLCD_print(Layer1, "WM8731 Error!\r\n");
        UI_UpdateVolume(volume,muted);
        break;
      /* Joystick LEFT pressed */
      case joyLeft:
        NVIC_DisableIRQ(SPI2_IRQn);
        if (playState) playState--;
        InitPlaybackState(playState);
        NVIC_EnableIRQ(SPI2_IRQn);
        UI_UpdateInput(playState);
        break;
      /* Joystick RIGHT pressed */
      case joyRight:
        NVIC_DisableIRQ(SPI2_IRQn);
        if (playState < NumberOfPlaybackStates-1) playState++;
        InitPlaybackState(playState);
        NVIC_EnableIRQ(SPI2_IRQn);
        UI_UpdateInput(playState);
        break;
      /* Joystick CENTER/SET pressed */
      case joyCenter:
        if(muted)
        {
          WM8731_Headphone_Volume(WM8731_VOL_CONV(volume));
          muted = FALSE;
        }
        else
        {
          WM8731_Headphone_Volume(SOUND_VOL_MIN);
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
