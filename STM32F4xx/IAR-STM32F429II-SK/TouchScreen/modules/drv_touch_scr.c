/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : drv_touch_scr.c
 *    Description : Touch screen driver module
 *
 *    History :
 *    1. Date        : August, 8 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : July, 24 2013
 *       Author      : Atanas Uzunov
 *       Description : Adapted for IAR-STM32F429II-SK
 *
 *  Touch screen sampling algorithm
 *   1. Set Y1, Y2 to output 1, Set X2 - ADC input, X1 - input with pull-down
 *   2. Wait setup delay.
 *   3. Check X1 state - if high, the screen is touched, set Y1 to low and wait
 *      setup delay else wait until interrupt by rising edge from X1.
 *      When interrupt arises, set Y1 to low and wait setup delay.
 *   4. Start conversion
 *   5. After TS_SAMPLES change Y1 high, Y2 low and calculate result 1023 - ADC.
 *   6. After TS_SAMPLES change Y1 to ADC input, Y2 input without pulls X1 and X2
 *      output in low and high level.
 *   7. Same like X.
 *   8. Apply sample delay and return back to point 1.
 *
 *    $Revision: #6 $
 **************************************************************************/
#include <assert.h>
#include <stdint.h>
#include "drv_touch_scr.h"
#include "drv_glcd.h"
#include "iar_stm32f429ii_sk.h"

typedef enum _TouchScrState_t
{
  TS_INTR_SETUP_DLY = 0, TS_WAIT_FOR_TOUCH,
  TS_X1_SETUP_DLY, TS_X1_MEASURE,
  TS_X2_SETUP_DLY, TS_X2_MEASURE,
  TS_Y1_SETUP_DLY, TS_Y1_MEASURE,
  TS_Y2_SETUP_DLY, TS_Y2_MEASURE,
} TouchScrState_t;

#define LEFT_UP_X      77 * TS_SAMPLES * 2
#define RIGHT_UP_X    952 * TS_SAMPLES * 2

#define LEFT_UP_Y     131 * TS_SAMPLES * 2
#define RIGHT_UP_Y    883 * TS_SAMPLES * 2

static volatile uint32_t Touch;
static volatile uint32_t X,Y;
static volatile uint32_t X_temp,Y_temp;
static volatile uint32_t Touch_temp;

static volatile TouchScrState_t State;
static volatile uint32_t Samples;

static RCC_ClocksTypeDef RCC_Clocks;

/*************************************************************************
 * Function Name: TIM5_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Sample timer interrupt handler
 *************************************************************************/
void TIM5_IRQHandler (void)
{
NVIC_InitTypeDef        NVIC_InitStructure;
GPIO_InitTypeDef        GPIO_InitStructure;

  TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
  TIM_Cmd(TIM5, DISABLE);

  switch(State)
  {
  case TS_X1_SETUP_DLY:
  case TS_X2_SETUP_DLY:
    ++State;
    ADC_SoftwareStartConv(ADC1);
    break;
  case TS_Y1_SETUP_DLY:
  case TS_Y2_SETUP_DLY:
    ++State;
    ADC_SoftwareStartConv(ADC1);
    break;
  case TS_INTR_SETUP_DLY:
    ++State;
    EXTI_ClearITPendingBit(TS_X1_EXTI_LINE);
    if(Bit_RESET == GPIO_ReadInputDataBit(TS_X1_PORT, TS_X1_PIN))
    {
      Touch_temp = Touch = FALSE;
      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
      NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
    }
    else
    {
      /* Update X and Y */
      if(Touch_temp)
      {
        X = X_temp;
        Y = Y_temp;
      }

      Touch = Touch_temp;

      /* Y1 = 0, Y2 = 1 */
      GPIO_ResetBits(TS_Y1_PORT,TS_Y1_PIN);
      GPIO_SetBits(TS_Y2_PORT,TS_Y2_PIN);
      /* Disable X1 pull down */
      GPIO_InitStructure.GPIO_Pin = TS_X1_PIN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(TS_X1_PORT, &GPIO_InitStructure);
      /* Reset sample counter */
      Samples = 0;
      /* Clear accumulators */
      X_temp = Y_temp = 0;
      /* Init setup delay */
      if(Touch)
      {
        TIM_SetCompare1(TIM5, ((RCC_Clocks.PCLK1_Frequency / 1)/1000000ul) * TS_SETUP_DLY);
      }
      else
      {
        TIM_SetCompare1(TIM5, ((RCC_Clocks.PCLK1_Frequency / 1)/1000000ul) * TS_INIT_DLY);
      }
      State = TS_X1_SETUP_DLY;
      TIM_SetCounter(TIM5, 0);
      TIM_Cmd(TIM5, ENABLE);
    }
    break;
  default:
    assert(0);
    break;
  }
}

/*************************************************************************
 * Function Name: EXTI9_5_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: On touch interrupt handler
 *************************************************************************/
void EXTI9_5_IRQHandler(void)
{
NVIC_InitTypeDef        NVIC_InitStructure;
GPIO_InitTypeDef        GPIO_InitStructure;

  /* Disable and clear interrupt */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* Init ADC measure setup delay */
  if(TS_WAIT_FOR_TOUCH == State)
  {
    /* Y1 = 0, Y2 = 1 */
    GPIO_ResetBits(TS_Y1_PORT,TS_Y1_PIN);
    GPIO_SetBits(TS_Y2_PORT,TS_Y2_PIN);
    /* Disable X1 pull down */
    GPIO_InitStructure.GPIO_Pin = TS_X1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(TS_X1_PORT, &GPIO_InitStructure);
    /* Reset sample counter */
    Samples = 0;
    /* Clear accumulators */
    X_temp = Y_temp = 0;
    /* Init setup delay */
    if(Touch)
    {
      TIM_SetCompare1(TIM5, ((RCC_Clocks.PCLK1_Frequency / 1)/1000000ul) * TS_SETUP_DLY);
    }
    else
    {
      TIM_SetCompare1(TIM5, ((RCC_Clocks.PCLK1_Frequency / 1)/1000000ul) * TS_INIT_DLY);
    }
    State = TS_X1_SETUP_DLY;
    TIM_SetCounter(TIM5, 0);
    TIM_Cmd(TIM5, ENABLE);
  }
  else
  {
    assert(0);
  }
}

/*************************************************************************
 * Function Name: ADC_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: End of conversion - interrupt handler
 *************************************************************************/
void ADC_IRQHandler(void)
{
GPIO_InitTypeDef   GPIO_InitStructure;
uint32_t Data;

  ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  Data = ADC_GetConversionValue(ADC1);

  switch(State)
  {
  case TS_X1_MEASURE:
    Y_temp += Data;
    if(++Samples >= TS_SAMPLES)
    {
      Samples = 0;
      State = TS_X2_SETUP_DLY;
      /* Y2 = 0, Y1 = 1; */
      GPIO_SetBits(TS_Y1_PORT,TS_Y1_PIN);
      GPIO_ResetBits(TS_Y2_PORT,TS_Y2_PIN);
      /* Init setup delay */
      TIM_SetCompare1(TIM5, ((RCC_Clocks.PCLK1_Frequency / 1)/1000000ul) * TS_SETUP_DLY);
      TIM_SetCounter(TIM5, 0);
      TIM_Cmd(TIM5, ENABLE);
    }
    else
    {
      ADC_SoftwareStartConv(ADC1);
    }
    break;

  case TS_X2_MEASURE:
    Y_temp += 1023UL - Data;
    if(++Samples >= TS_SAMPLES)
    {
      Samples = 0;
      State = TS_Y1_SETUP_DLY;

      /* X1 = 0, X2 = 1, no pulls */
      GPIO_InitStructure.GPIO_Pin = TS_X2_PIN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(TS_X2_PORT, &GPIO_InitStructure);

      GPIO_InitStructure.GPIO_Pin = TS_X1_PIN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(TS_X1_PORT, &GPIO_InitStructure);

      GPIO_ResetBits(TS_X1_PORT,TS_X1_PIN);
      GPIO_SetBits(TS_X2_PORT,TS_X2_PIN);

      /* Y2 - ADC1, Y1 - input */
      GPIO_InitStructure.GPIO_Pin = TS_Y1_PIN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(TS_Y1_PORT, &GPIO_InitStructure);

      GPIO_InitStructure.GPIO_Pin = TS_Y2_PIN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(TS_Y2_PORT, &GPIO_InitStructure);

      /* ADC1 regular channel1 configuration */
      ADC_RegularChannelConfig(ADC1, TS_Y2_CHANNEL, 1, ADC_SampleTime_56Cycles);

      /* Clear pending interrupt */
      ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);

      /* Init setup delay */
      TIM_SetCompare1(TIM5, ((RCC_Clocks.PCLK1_Frequency / 1)/1000000ul) * TS_SETUP_DLY);
      TIM_SetCounter(TIM5, 0);
      TIM_Cmd(TIM5, ENABLE);
    }
    else
    {
      ADC_SoftwareStartConv(ADC1);
    }
    break;

  case TS_Y1_MEASURE:
    X_temp += 1023UL - Data;
    if(++Samples >= TS_SAMPLES)
    {
      Samples = 0;
      State = TS_Y2_SETUP_DLY;
      /* X2 = 0, X1 = 1 */
      GPIO_SetBits(TS_X1_PORT,TS_X1_PIN);
      GPIO_ResetBits(TS_X2_PORT,TS_X2_PIN);
      /* Init setup delay */
      TIM_SetCompare1(TIM5, ((RCC_Clocks.PCLK1_Frequency / 1)/1000000ul) * TS_SETUP_DLY);
      TIM_SetCounter(TIM5, 0);
      TIM_Cmd(TIM5, ENABLE);
    }
    else
    {
      ADC_SoftwareStartConv(ADC1);
    }
    break;

  case TS_Y2_MEASURE:
    X_temp += Data;

    if(++Samples >= TS_SAMPLES)
    {
      State = TS_INTR_SETUP_DLY;

      /* Y1 = 1, Y2 = 1 */
      GPIO_InitStructure.GPIO_Pin = TS_Y1_PIN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(TS_Y1_PORT, &GPIO_InitStructure);
      GPIO_SetBits(TS_Y1_PORT,TS_Y1_PIN);

      GPIO_InitStructure.GPIO_Pin = TS_Y2_PIN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(TS_Y2_PORT, &GPIO_InitStructure);
      GPIO_SetBits(TS_Y2_PORT,TS_Y2_PIN);

      /* X2 - ADC, X1 input with pull down */
      GPIO_InitStructure.GPIO_Pin = TS_X1_PIN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
      GPIO_Init(TS_X1_PORT, &GPIO_InitStructure);

      GPIO_InitStructure.GPIO_Pin = TS_X2_PIN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(TS_X2_PORT, &GPIO_InitStructure);

      /* ADC1 regular channel0 configuration */
      ADC_RegularChannelConfig(ADC1, TS_X2_CHANNEL, 1, ADC_SampleTime_56Cycles);

      /* Clear pending interrupt */
      ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);

      /* Init setup delay */
      TIM_SetCompare1(TIM5, ((RCC_Clocks.PCLK1_Frequency / 1)/1000000ul) * TS_SETUP_DLY);
      TIM_SetCounter(TIM5, 0);
      TIM_Cmd(TIM5, ENABLE);
      Touch_temp = TRUE;
    }
    else
    {
      ADC_SoftwareStartConv(ADC1);
    }
    break;
  default:
    assert(0);
  }
}

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Touch screen
 *************************************************************************/
void TouchScrInit(void)
{
ADC_InitTypeDef         ADC_InitStructure;
ADC_CommonInitTypeDef   ADC_CommonInitStruct;
GPIO_InitTypeDef        GPIO_InitStructure;
NVIC_InitTypeDef        NVIC_InitStructure;
EXTI_InitTypeDef        EXTI_InitStructure;
TIM_TypeDef             TIM_Struct;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;

  /* Disable ADC, TIM5 and EXTI9_5 interrupts if any enabled */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Init variables */
  Touch_temp = Touch = FALSE;
  X = Y = 0;
  State = TS_INTR_SETUP_DLY;

  /* Init GPIOs */
  RCC_AHB1PeriphClockCmd(TS_X1_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(TS_X2_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(TS_Y1_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(TS_Y2_CLK, ENABLE);

  /* Y1 = 1 */
  GPIO_InitStructure.GPIO_Pin = TS_Y1_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(TS_Y1_PORT, &GPIO_InitStructure);
  GPIO_SetBits(TS_Y1_PORT,TS_Y1_PIN);

  /* Y2 = 1 */
  GPIO_InitStructure.GPIO_Pin = TS_Y2_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(TS_Y2_PORT, &GPIO_InitStructure);
  GPIO_SetBits(TS_Y2_PORT,TS_Y2_PIN);

  /* X1 - IN - Pull Down */
  GPIO_InitStructure.GPIO_Pin = TS_X1_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(TS_X1_PORT, &GPIO_InitStructure);

  /* X2 - ADC1 */
  GPIO_InitStructure.GPIO_Pin = TS_X2_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(TS_X2_PORT, &GPIO_InitStructure);

  /* ADC1 configuration (X2) */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  ADC_CommonStructInit(&ADC_CommonInitStruct);
  ADC_CommonInit(&ADC_CommonInitStruct);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel configuration */
  ADC_RegularChannelConfig(ADC1, TS_X2_CHANNEL, 1, ADC_SampleTime_56Cycles);

  /* Disable ADC1 DMA */
  ADC_DMACmd(ADC1, DISABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Clear any pending interrupts */
  ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);

  /* Enable ADC1 Interrupt */
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

  /* Allow ports to settle */
  for(volatile int x=0; x<1000; x++);

  /* Enable X1 interrupt on touch */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(TS_X1_EXTI_PORT, TS_X1_EXTI_PIN);
  EXTI_InitStructure.EXTI_Line = TS_X1_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable ADC interrupts */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable TIM5 clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

  RCC_GetClocksFreq(&RCC_Clocks);

  /* Init Timer5 */
  TIM_DeInit(TIM5);
  TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  /* TIM5 Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_Pulse = ((RCC_Clocks.PCLK1_Frequency / 1)/1000000ul) * TS_SETUP_DLY;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);

  /* TIM Interrupts enable */
  TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);

  /* NVIC TIM5 Interrupt enable */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_SetCounter(TIM5, 0);

  /* TIM5 enable counter */
  TIM_Cmd(TIM5, ENABLE);
}

/*************************************************************************
 * Function Name: TouchScrGetStatus
 * Parameters: ToushRes_t * pData X,Y data
 *
 * Return: uint32_t 0 - untouched
 *                  1 - touched
 *
 * Description: Return current state of the touch screen
 *************************************************************************/
uint32_t TouchGet (ToushRes_t * pData)
{
uint32_t  TouchResReadyHold = Touch;
uint32_t X_coordinate, Y_coordinate;

  if(TouchResReadyHold)
  {
    X_coordinate = X;
    Y_coordinate = Y;

    /* Calculate X coordinate in pixels */
    if (X_coordinate <= LEFT_UP_X)
    {
      X_coordinate = 0;
    }
    else if (X_coordinate >= RIGHT_UP_X)
    {
      X_coordinate = IMAGE_WIDTH;
    }
    else
    {
      X_coordinate = ((X_coordinate - LEFT_UP_X) * IMAGE_WIDTH)/(RIGHT_UP_X-LEFT_UP_X);
    }

    /* Calculate Y coordinate in pixels */
    if (Y_coordinate <= LEFT_UP_Y)
    {
      Y_coordinate = IMAGE_HEIGHT;
    }
    else if (Y_coordinate >= RIGHT_UP_Y)
    {
      Y_coordinate = 0;
    }
    else
    {
      Y_coordinate = ((RIGHT_UP_Y-Y_coordinate) * IMAGE_HEIGHT)/(RIGHT_UP_Y-LEFT_UP_Y);
    }

    pData->X = X_coordinate;
    pData->Y = Y_coordinate;
  }
  return(TouchResReadyHold);
}
