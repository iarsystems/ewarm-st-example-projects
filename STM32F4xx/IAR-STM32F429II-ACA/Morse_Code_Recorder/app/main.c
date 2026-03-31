/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2014
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : October, 2014
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F429II-ACA board. It shows basic
 * use of the parallel I/O, the timers, the interrupt controller, the DAC module
 * and the SPI module. The example implements simple, event driven state machine.
 *
 * Sine wave tone (1kHz) is genearted via the onboard speaker, when BUT1 is
 * pressed. Sequence of tones (dashes and dots) and pauses, called morse code,
 * can be recorded, when pressing the BUT3. Recording can be stopped by pressing
 * again the BUT3.
 * The playback of the recorded sequence can be started by pressing BUT2. It can
 * be stopped by pressing BUT2 again.
 *
 * The recorded morse code sequence is saved in the external SPI flash memory.
 *
 * The following LEDs turn on, when the application is in the following states:
 *
 * LED7 - sine wave tone is generated via the speaker.
 * LED5 - recording of the morse code is in progress.
 * LED8 - playback of the saved morse code is in progress.
 *
 * COMPATIBILITY
 * =============
 *  The Morse_Code_Recorder example project is compatible with IAR-STM32F429II-ACA
 * evaluation board.
 *  By default, the project is configured to use the I-Jet JTAG/SWD interface.
 *
 * CONFIGURATION
 * =============
 * Flash Debug - The progam is loaded to the internal flash.
 *
 *  Make sure that the following jumpers are correctly configured on the
 * IAR-STM32F429II-ACA evaluation board:
 *
 *  Jumpers:
 *   B0_1/B0_0  - B0_0
 *   B1_1/B1_0  - B1_0
 *   LEDS_PWR_E - closed
 *   DAC2_E     - closed
 *   SPEAKER_E  - closed
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define BT_TONE         (1<<0)
#define BT_PLAY         (1<<1)
#define BT_REC          (1<<2)

#define LED_TONE        LED7
#define LED_PLAY        LED8
#define LED_REC         LED5

#define SPK_ZERO_OFFSET 2047
#define SINE_WAVE_GAIN  1900

static volatile SoundState_TypeDef sound_state;
static volatile uint32_t      buttons_numb_timer;
/* message chunks saving buffer */
static volatile Chunk_TypeDef chunks[5000];
static uint32_t sinePtr;
/* application state and event variables */
static MCREvent_TypeDef event;
static MCRState_TypeDef state;

/* variable for critical section entry control */
uint32_t CriticalSecCntr;

typedef int16_t AudioChannelSample16b_t;

volatile AudioChannelSample16b_t * SineData;

#define PI                    3.14159265
#define SINE_BUFFER_SIZE      441

#define MMESSAGE_SPI_ADDR0    (0x00000000)

#define InitPlayerSoftStart() sinePtr = 0;

/*************************************************************************
 * Function Name: GenerateSineWave
 * Parameters: AudioChannelSample16b_t * data, size_t size, uint32_t amplitude
 *
 * Return: none
 *
 * Description: Generates 1kHz sine wave array.
 *
 *************************************************************************/
void GenerateSineWave(AudioChannelSample16b_t * data, size_t size, uint32_t amplitude)
{
  for (int i = 0; i<size; i++)
  {
    *(data+i) = (AudioChannelSample16b_t)(sin(10 * (2 * PI) * i / size) * amplitude);
  }
}

/*************************************************************************
 * Function Name: GetSineSample
 * Parameters: none
 *
 * Return: AudioChannelSample16b_t
 *
 * Description: Returns next sample from the sine wave array.
 *
 *************************************************************************/
AudioChannelSample16b_t GetSineSample(void){
  AudioChannelSample16b_t sample;
  sample = *( SineData + sinePtr );
  if( ++sinePtr >= SINE_BUFFER_SIZE ) sinePtr = 0;
  return sample;
}

/*************************************************************************
 * Function Name: Sound_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Sound System interrupt handler (TIM4 update interrupt)
 *
 *************************************************************************/
void Sound_Handler(void)
{
static uint32_t __sound_state;
AudioChannelSample16b_t sample;
  if(SET == TIM_GetITStatus(TIM4, TIM_IT_Update))
  {
    if ( __sound_state )
    {
      sample = GetSineSample();
      /* Ensure a soft stop */
      if( ( sndStop == sound_state ) && ( abs(sample) < 5 ) )
      {
        __sound_state = 0;
        STM_LEDOff( LED_TONE );
      }
    }
    else
    {
      sample = 0;
      if( ( sndStart == sound_state ) )
      {
        /* Ensure a soft start */
        InitPlayerSoftStart();
        __sound_state = 1;
        STM_LEDOn( LED_TONE );
      }
    }
    /* Write DAC output holding register */
    DAC_SetChannel2Data(DAC_Align_12b_R, SPK_ZERO_OFFSET + sample);
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    /* Ensure interrupt flag bit is cleared */
    __DSB();
  }
}

/*************************************************************************
 * Function Name: SystemTimer_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: System Timer Handler function
 *
 *************************************************************************/
void SystemTimer_Handler()
{
  if( buttons_numb_timer != 0x00 )
  {
    buttons_numb_timer--;
  }
}

/*************************************************************************
 * Function Name: SoundSystemInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init modules needed for sound generating
 *
 *************************************************************************/
void SoundSystemInit(void)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
GPIO_InitTypeDef GPIO_InitStructure;
DAC_InitTypeDef DAC_InitStructure;
RCC_ClocksTypeDef Clocks;

  /* Enable DAC clock */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_DAC, ENABLE );
  /* Release DAC from reset state */
  DAC_DeInit();
  /* Init DAC out pin */
  GPIO_InitStructure.GPIO_Pin = SPEAKER_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SPEAKER_GPIO_PORT, &GPIO_InitStructure);
  /* Configure DAC */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);
  /* Enable DAC */
  DAC_Cmd(DAC_Channel_2, ENABLE);

  RCC_GetClocksFreq(&Clocks);
  /* Enable TIM4 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  /* Release TIM4 from reset state */
  TIM_DeInit(TIM4);
  /* Init TIM4 */
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  /* APB1 prescaler is > 1, so timer clock is multiplied by 2 */
  TIM_TimeBaseInitStruct.TIM_Period = ((Clocks.PCLK1_Frequency*2)/44100);
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
  /* Select TIM4 update event to be trigger output */
  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);
  /* Enable TIM4 interrupt */
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  NVIC_EnableIRQ(TIM4_IRQn);
  NVIC_SetPriority(TIM4_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
  /* Load first sample to DAC */
  DAC_SetChannel2Data(DAC_Align_12b_R, SPK_ZERO_OFFSET);
  /* Playback is stopped */
  sound_state = sndStop;
  /* Enable TIM4 */
  TIM_Cmd(TIM4, ENABLE);
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
uint16_t chunk_index;
uint32_t buttons_hold, buttons;
uint32_t start_counter;
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  ENTR_CRT_SECTION();

  /* SysTick Config */
  if( SysTick_Config( SystemCoreClock/10000 ) )
  {
    /* Catch an error */
    while (1);
  }

  /* Reserve space for sine wave array */
  SineData = malloc(SINE_BUFFER_SIZE*sizeof(AudioChannelSample16b_t));

  if( NULL == SineData )
  {
    /* Catch a low memory error */
    while (1);
  }

  /* Generate sine wave, with the given amplitude */
  GenerateSineWave( (AudioChannelSample16b_t *)SineData, SINE_BUFFER_SIZE, SINE_WAVE_GAIN );

  /* Initialize sound system */
  SoundSystemInit();

  /* Initialize the pause and tone measurement timer (TIM5) */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  TIM_DeInit(TIM5);
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInitStruct.TIM_Period = 0xFFFFFFFF;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);
  TIM_Cmd(TIM5, ENABLE);

  EXT_CRT_SECTION();

  /* Init buttons */
  STM_ButtonInit( BUT1, BUTTON_MODE_GPIO );
  STM_ButtonInit( BUT2, BUTTON_MODE_GPIO );
  STM_ButtonInit( BUT3, BUTTON_MODE_GPIO );

  /* Init + turn off all LEDs */
  for(Led_TypeDef i = LED1; i <= LED8; i++ )
  {
    STM_LEDInit( i );
    STM_LEDOff( i );
  }

  /* Init and unprotect flash memory */
  STM_SPIFlashInit();
  AT25_UnprotectFlash();

  /* Main loop */
  while(1)
  {
    /* Clear the event variable */
    event = evNone;

    /* Buttons handler, generates a buttons-event */
    if( !buttons_numb_timer )
    {
      buttons = ( (( RESET == STM_ButtonGetState( BUT1 )?1:0 ) << 0 ) |
                  (( RESET == STM_ButtonGetState( BUT2 )?1:0 ) << 1 ) |
                  (( RESET == STM_ButtonGetState( BUT3 )?1:0 ) << 2 ));

      /* if there is a change in the buttons state */
      if( buttons_hold != buttons )
      {
        /* generate an appropriate event */
        switch( buttons )
        {
          case BT_TONE: event = evButtonTonePressed; break;
          case BT_PLAY: event = evButtonPlayPressed; break;
          case BT_REC:  event = evButtonRecPressed; break;
          case 0:       event = evButtonsReleased; break;
          default:      event = evNone; break;
        }
        buttons_numb_timer = 300;
        /* saves the current buttons state */
        buttons_hold = buttons;
      }
    }

    /* State processing */
    switch( state )
    {
      /* Idle state, do nothing, wait for button press */
      case stIdle:
        STM_LEDOff( LED_PLAY );
        STM_LEDOff( LED_REC );
        sound_state = sndStop;
        if( evButtonTonePressed == event )
        {
          state = stDoToneInit;
        }
        if( evButtonRecPressed == event )
        {
          state = stRecordingInit;
        }
        if( evButtonPlayPressed == event )
        {
          state = stPlaybackInit;
        }
        break;

      /* DoTone state and init state, produces tone output */
      case stDoToneInit:
        sound_state = sndStart;
        state = stDoTone;
        break;

      case stDoTone:
        if( evButtonsReleased == event )
        {
          state = stIdle;
        }
        break;

      /* Recording init state, prepares for recording */
      case stRecordingInit:
        STM_LEDOn( LED_REC );
        chunk_index = 0;
        state = stRecPauseInit;
        break;

      /* Record Pause state and init state, records one pause chunk */
      case stRecPauseInit:
        sound_state = sndStop;
        start_counter = TIM_GetCounter(TIM5);
        chunks[chunk_index].type = ctPause;
        state = stRecPause;
        break;

      case stRecPause:
        if( evButtonTonePressed == event )
        {
          chunks[chunk_index++].time = TIM_GetCounter(TIM5) - start_counter;
          state = stRecToneInit;
        }
        if( evButtonRecPressed == event )
        {
          chunks[chunk_index++].time = TIM_GetCounter(TIM5) - start_counter;
          chunks[chunk_index].type = ctEnd;
          state = stRecSave;
        }
        break;

      /* Record Tone state and init state, records one tone chunk */
      case stRecToneInit:
        sound_state = sndStart;
        start_counter = TIM_GetCounter( TIM5 );
        chunks[chunk_index].type = ctTone;
        state = stRecTone;
        break;

      case stRecTone:
        if( evButtonsReleased == event )
        {
          chunks[chunk_index].time = TIM_GetCounter( TIM5 ) - start_counter;
          chunk_index++;
          state = stRecPauseInit;
        }
        break;

      /* Record Save state, saves the recorded set of chunks to the SPI flash memory */
      case stRecSave:
        AT25_WriteData( MMESSAGE_SPI_ADDR0,(unsigned char *)chunks, (chunk_index+1)*sizeof(Chunk_TypeDef) );
        state = stIdle;
        break;

      /* Playback init state, prepare for playback of saved chunks, loads them from SPI flash memory */
      case stPlaybackInit:
        STM_LEDOn( LED_PLAY );
        chunk_index = 0;
        AT25_ReadData( MMESSAGE_SPI_ADDR0,(unsigned char *)chunks, sizeof(chunks) );
        state = stPlayChunkInit;
        break;

      /* Play Chunk state and init state, plays one chunk */
      case stPlayChunkInit:
        if( ctPause == chunks[chunk_index].type )
        {
          sound_state = sndStop;
          TIM_SetCounter( TIM5, 0 );
          state = stPlayChunk;
        }
        else if( ctTone == chunks[chunk_index].type )
        {
          sound_state = sndStart;
          TIM_SetCounter( TIM5,0 );
          state = stPlayChunk;
        }
        else
        {
          state = stIdle;
        }
        break;

      case stPlayChunk:
        if( TIM_GetCounter( TIM5 ) > chunks[chunk_index].time )
        {
          chunk_index++;
          state = stPlayChunkInit;
        }
        if( evButtonPlayPressed == event )
        {
          state = stIdle;
        }
        break;
    }
  }
}
