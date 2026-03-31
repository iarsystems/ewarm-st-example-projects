/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 19, July 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR STM32-SK board. It shows basic
 * use of parallel I/O, timer, interrupt controller, ADC and interface to
 * a LCD HD44780 compatible module.
 *  It show the position of the potentiometer in percents (ADC Channel 15).
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US  450

Int32U CriticalSecCntr;
volatile Boolean Update = TRUE;

/*************************************************************************
 * Function Name: Timer1IntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 1 interrupt handler
 *
 *************************************************************************/
void Timer1IntrHandler (void)
{
  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);
  Update = TRUE;
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
 * Function Name: InitADC1
 * Parameters: none
 * Return: none
 *
 * Description: ADC Init subroutine
 *
 *************************************************************************/
void InitADC1(void)
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
  // Enable ADC1 reset calibaration register
  ADC_ResetCalibration(ADC1);
  // Check the end of ADC1 reset calibration register
  while(ADC_GetResetCalibrationStatus(ADC1) == SET);

  // Start ADC1 calibaration
  ADC_StartCalibration(ADC1);
  // Check the end of ADC1 calibration
  while(ADC_GetCalibrationStatus(ADC1) == SET);

}

/*************************************************************************
 * Function Name: GetADC1Channel
 * Parameters: Int8U channel
 * Return: Int16U
 *
 * Description: ADC Convert
 *
 *************************************************************************/
Int16U GetADC1Channel(Int8U chanel)
{
  // Configure channel
  ADC_RegularChannelConfig(ADC1, chanel, 1, ADC_SampleTime_55Cycles5);

  // Start the conversion
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

  // Wait until conversion completion
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

  // Get the conversion value
  return ADC_GetConversionValue(ADC1);
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
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef TIM1_TimeBaseInitStruct;
char Str[17];
Int32U Pos;

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

  // ADC1 Init
  InitADC1();

  // Timer1 Init
  // Enable Timer1 clock and release reset
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1,DISABLE);

  // Set timer period 0.2 sec
  TIM1_TimeBaseInitStruct.TIM_Prescaler = 720;  // 10us resolution
  TIM1_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM1_TimeBaseInitStruct.TIM_Period = 20000;  // 200 ms
  TIM1_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM1_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseInitStruct);

  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);
  // Enable update interrupt
  TIM_ITConfig(TIM1,TIM_FLAG_Update,ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // Enable timer counting
  TIM_Cmd(TIM1,ENABLE);

  EXT_CRT_SECTION();

  // LCD Init
  HD44780_PowerUpInit();

  // Show message on the LCD
  HD44780_StrShow(1, 1,  "  IAR Systems   ");
  HD44780_StrShow(1, 2,  "    STM32-SK    ");
  LCD_LIGHT_ON();
  DelayResolution100us(30000);
  while(1)
  {
    if(Update)
    {
      Update = FALSE;
      // Calculate position of the potentiometer in percents
      Pos = (GetADC1Channel(ADC_Channel_15)*100)/4095;
      sprintf(Str,"AN_TR:%3d%%      ",Pos);
      HD44780_StrShow(1, 2,(pInt8S)Str);
    }
  }
}
