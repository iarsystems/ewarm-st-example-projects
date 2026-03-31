/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 9, March 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32L152-SK evaluation board. It shows
 * basic use of I/O, DMA, ADC and LCD controller.
 *  The example change LCD contrast depending of AN_TR trimmer position.
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   3.3V_MCU_E - filled
 *   B0_0/B0_1  - B0_0
 *   B1_0/B1_1  - B1_0
 *
 *    $Revision: 39 $
 **************************************************************************/

#include <intrinsics.h>
#include "stm32l1xx.h"
#include "stm32.h"
#include "stm32_glass_lcd.h"

#define SCROLL_SPEED  5000 /* Low value gives higher speed */
#define SCROLL_NUM    8

#define ADC1_DR_Address    ((uint32_t)0x40012458)
#define ADC_StepValue       512  /* Eight intervals with ADC_StepValue range */

const uint32_t LCDContrastArray[8]= {LCD_Contrast_Level_0, LCD_Contrast_Level_1, LCD_Contrast_Level_2,
                                     LCD_Contrast_Level_3, LCD_Contrast_Level_4, LCD_Contrast_Level_5,
                                     LCD_Contrast_Level_6, LCD_Contrast_Level_7};

uint8_t LCD_String[29] = " IAR SYSTEMS STM32L152VB-SK ";

volatile uint16_t ADC_ConvertedValue;

/*************************************************************************
 * Function Name: LCD_GLASS_DisplayContrast
 * Parameters: uint8_t value
 *
 * Return: none
 *
 * Description: onfigures the LCD Contrast and  Display Message on the LCD GLASS.
 *
 *************************************************************************/
void  LCD_GLASS_DisplayContrast(uint8_t value)
{
  /* Configures the LCD Contrast Maximum
     Voltage according to the value variable */
  LCD_ContrastConfig(LCDContrastArray[value]);

  switch (value)
  {
    case 0 :
      /* Display the "CONT2V6" message on the LCD GLASS */
      LCD_GLASS_DisplayString("CONT2V6");
      break;
    case 1 :
      /* Display the "CONT2V7" message on the LCD GLASS */
      LCD_GLASS_DisplayString("CONT2V7");
      break;
    case 2 :
      /* Display the "CONT2V8" message on the LCD GLASS */
      LCD_GLASS_DisplayString("CONT2V8");
      break;
    case 3 :
      /* Display the "CONT2V9" message on the LCD GLASS */
      LCD_GLASS_DisplayString("CONT2V9");
      break;
    case 4 :
      /* Display the "CONT3V0" message on the LCD GLASS */
      LCD_GLASS_DisplayString("CONT3V0");
      break;
    case 5 :
      /* Display the "CONT3V1" message on the LCD GLASS */
      LCD_GLASS_DisplayString("CONT3V1");
      break;
    case 6 :
      /* Display the "CONT3V2" message on the LCD GLASS */
      LCD_GLASS_DisplayString("CONT3V2");
      break;
    default :
      /* Display the "CONT3V3" message on the LCD GLASS */
      LCD_GLASS_DisplayString("CONT3V3");
      break;
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
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
uint32_t contrast;

  __disable_interrupt();

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

  /* GPIO init*/
  STM32_AN_TR_Init(TRUE);

  /* DMA1 channel1 configuration ----------------------------------------------*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /* ADC1 configuration ------------------------------------------------------*/
  /* Enable ADC1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* Enable the HSI */
  RCC_HSICmd(ENABLE);

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = 0;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel18 configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_22, 1, ADC_SampleTime_4Cycles);

  /* Enable the request after last transfer for DMA Circular mode */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConv(ADC1);

  /* RCC configuration*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to the RTC */
  PWR_RTCAccessCmd(ENABLE);

  /* Reset Backup Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);

  /*!< LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);

  /*!< Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  /*!< LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* LCD GLASS Initialization */
  LCD_GLASS_Init();

  __enable_interrupt();

  while(1)
  {
    /* Calculate the Contrast range value */
    contrast = (uint8_t) (ADC_ConvertedValue / ADC_StepValue);

    /* Configure the LCD Contrast and Display message on the LCD Glass */
    LCD_GLASS_DisplayContrast(contrast);
  }
}
