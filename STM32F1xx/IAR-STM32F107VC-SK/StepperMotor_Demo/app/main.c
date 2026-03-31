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
 *    1. Date        : 20, May 2009
 *       Author      : Stoyan Choynev
 *       Description : Create
 *
      This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F107VC-SK board. It shows basic
 * use of parallel I/O, timer, interrupt controller, interface to
 * a Nokia Graphic color LCD module and step motor.
 * The stepper motor speed and direction is controlled by the trimer.
 *
 * Jumpers:
 *   PWR_SEL   - depending of power source
 *   JTAG_SEL  - open (on board j-link)
 *   B0_1/B0_0 - B0_0
 *   B1_1/B1_0 - B1_0
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US       450

#define TRIM_MASK       GPIO_Pin_10
#define TRIM_CH         ADC_Channel_10
#define TRIM_PORT       GPIOC

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

Int32U CriticalSecCntr;
volatile Boolean SysTickFl;

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
GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
float AdcData;

  ENTR_CRT_SECTION();
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  // SysTick end of count event each 0.2s with input clock equal to 9MHz (HCLK/8, default)
  SysTick_Config(1800000);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  // Stepper motor init
  StepMotorInit();

  EXT_CRT_SECTION();
  // AN_TR port and ADC init
  // Enable ADC1 and GPIOC clock
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, DISABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  TRIM_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TRIM_PORT, &GPIO_InitStructure);

  // ADC1 configuration
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  // ADC1 regular channel10 configuration
  ADC_RegularChannelConfig(ADC1, TRIM_CH, 1, ADC_SampleTime_55Cycles5);

  // Disable ADC1 DMA
  ADC_DMACmd(ADC1, DISABLE);

  // Enable ADC1
  ADC_Cmd(ADC1, ENABLE);

  // Enable ADC1 reset calibaration register
  ADC_ResetCalibration(ADC1);
  // Check the end of ADC1 reset calibration register
  while(ADC_GetResetCalibrationStatus(ADC1));

  // Start ADC1 calibaration
  ADC_StartCalibration(ADC1);
  // Check the end of ADC1 calibration
  while(ADC_GetCalibrationStatus(ADC1));

  // Start ADC1 Software Conversion
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  // GLCD init
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);
  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(10,104,131,131);

  StepMotorRun();

  while(1)
  {
    if (SysTickFl)
    {
      SysTickFl = FALSE;
      // Get new sample of the ADC
      AdcData = ADC_GetConversionValue(ADC1);
      ADC_SoftwareStartConvCmd(ADC1, ENABLE);

      AdcData = ((AdcData /((1UL<<12)/2)-1)*RPM_MAX);
      StepMotorSetRPM (AdcData);
      GLCD_print("\fRPM: %d\r",(Int32S)AdcData);
    }
  }
}
