/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : step_motor_drv_2.c
 *    Description : Step Motor Driver 2
 *
 *    History :
 *    1. Date        : May 20, 2009
 *       Author      : Stoyan Choynev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "step_motor_drv_2.h"
#include "Sin_Table.h"

#define MOTOR_TIM         TIM1
#define MOTOR_ENA_A_PIN   GPIO_Pin_7
#define MOTOR_ENA_B_PIN   GPIO_Pin_10
#define MOTOR_ENA_A_PORT  GPIOB
#define MOTOR_ENA_B_PORT  GPIOD

#define MOTOR_SET_PHASE1(phase) TIM_SetCompare1(MOTOR_TIM,phase)
#define MOTOR_SET_PHASE2(phase) TIM_SetCompare2(MOTOR_TIM,phase)

#define RCC_APB2PERIPH_MOTOR_TIM  RCC_APB2Periph_TIM1
#define RCC_APB2PERIPH_MOTOR_ENA_A_PORT  RCC_APB2Periph_GPIOB
#define RCC_APB2PERIPH_MOTOR_ENA_B_PORT  RCC_APB2Periph_GPIOD


static Int16U SinePhase = 0;
static Int16S SineFreq = 1;
static Int32U PwmFreq;

/*************************************************************************
 * Function Name: StepMotorIntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Step motor interrupt handler
 *
 *************************************************************************/
void StepMotorIntrHandler(void)
{
  TIM_ClearITPendingBit(MOTOR_TIM, TIM_IT_Update);

  SinePhase += SineFreq;

  MOTOR_SET_PHASE1(1024 + Sin_Table[(Int16U)(SinePhase)>>6]);
  MOTOR_SET_PHASE2(1024 + Sin_Table[(Int16U)(SinePhase+65536/4)>>6]);

}

/*************************************************************************
 * Function Name: StepMotorInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Step motor
 *
 *************************************************************************/
void StepMotorInit (void)
{
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
RCC_ClocksTypeDef RCC_Clocks;
NVIC_InitTypeDef NVIC_InitStructure;

  // Enable clock
  RCC_APB2PeriphResetCmd( RCC_APB2Periph_AFIO | RCC_APB2PERIPH_MOTOR_TIM | RCC_APB2Periph_GPIOE
                        | RCC_APB2PERIPH_MOTOR_ENA_A_PORT | RCC_APB2PERIPH_MOTOR_ENA_B_PORT,
                          DISABLE);

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2PERIPH_MOTOR_TIM | RCC_APB2Periph_GPIOE
                        | RCC_APB2PERIPH_MOTOR_ENA_A_PORT | RCC_APB2PERIPH_MOTOR_ENA_B_PORT,
                          ENABLE);

  GPIO_InitStructure.GPIO_Pin =  MOTOR_ENA_A_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_ENA_A_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  MOTOR_ENA_B_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_ENA_B_PORT, &GPIO_InitStructure);

  StepMotorStop();

  /* Init PWM TIM1 */
  /* PWM (TIM1/CH1P) PE9 */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* PWM (TIM1/CH1N) PE8 */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* PWM (TIM1/CH2P) PE11 */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  /* PWM (TIM1/CH2N) PE10 */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Map TIM1 OC1/OC2 */
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);

  // Enable Timer1 clock and release reset
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1,DISABLE);

  TIM_InternalClockConfig(TIM1);

  RCC_GetClocksFreq(&RCC_Clocks);

  Int32U Tim1Clock = RCC_Clocks.PCLK2_Frequency ;

  if(RCC_Clocks.HCLK_Frequency != RCC_Clocks.PCLK2_Frequency)
  {
    Tim1Clock = 2*RCC_Clocks.PCLK2_Frequency ;
  }


  // Time base configuration
  TIM_TimeBaseStructure.TIM_Prescaler = ((Tim1Clock + (PWM_FREQ*PWM_RES)/2)/(PWM_FREQ*PWM_RES))-1;
  PwmFreq = Tim1Clock/((TIM_TimeBaseStructure.TIM_Prescaler+1)*PWM_RES); /*This is there real PWM Freq*/
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = PWM_RES; /*PWM resolution*/
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

  // Channel 1 Configuration in TOGGLE mode
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
  TIM_OC1Init(TIM1,&TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
  // Channel 2 Configuration in TOGGLE mode
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
  // Double buffered
  TIM_ARRPreloadConfig(TIM1,DISABLE);


  /* Enable the TIM1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

  TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

/*************************************************************************
 * Function Name: StepMotorRun
 * Parameters: none
 *
 * Return: none
 *
 * Description: Step motor run
 *
 *************************************************************************/
void StepMotorRun ()
{
  GPIO_WriteBit(MOTOR_ENA_A_PORT,MOTOR_ENA_A_PIN,Bit_SET);
  GPIO_WriteBit(MOTOR_ENA_B_PORT,MOTOR_ENA_B_PIN,Bit_SET);
  // TIM1 counter enable
  TIM_Cmd(TIM1,ENABLE);

}

/*************************************************************************
 * Function Name: StepMotorStop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Step motor stop
 *
 *************************************************************************/
void StepMotorStop (void)
{
  // TIM1 counter disable
  TIM_Cmd(TIM1,DISABLE);
  /**/
  GPIO_WriteBit(MOTOR_ENA_A_PORT,MOTOR_ENA_A_PIN,Bit_RESET);
  GPIO_WriteBit(MOTOR_ENA_B_PORT,MOTOR_ENA_B_PIN,Bit_RESET);
}

/*************************************************************************
 * Function Name: StepMotorSetRPM
 * Parameters: float  Rpm - Rotaion per minute
 *
 * Return: none
 *
 * Description: Set Stepper motor Rotation per minute (RPM)
 *              The sign shows direction
 *
 *************************************************************************/
void StepMotorSetRPM (float Rpm)
{
  SineFreq = (Int32S)((Rpm*65536)/(12*PwmFreq));
}


