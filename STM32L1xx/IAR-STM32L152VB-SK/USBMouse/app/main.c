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
 *    1. Date        : 07, March 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *   This example project shows how to use the IAR Embedded Workbench
 *  for ARM to develop code for the IAR-STM32L152-SK board. It implements USB
 *  HID mouse. 
 *   The USER and WKUP buttons control the horizontal movement and the AN_TR controls
 *  the vertical one.
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   3.3V_MCU_E - filled
 *   B0_0/B0_1  - B0_0
 *   B1_0/B1_1  - B1_0
 *
 *    $Revision: 1477 $
 **************************************************************************/
#include "includes.h"

Int32U CriticalSecCntr;
volatile Boolean SysTickFl;

#define ADC1_DR_Address    &ADC1->DR

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
//Int32U  count;
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
volatile uint16_t ADC_ConvertedValue;
uint16_t ADC_Hold;

  ENTR_CRT_SECTION();
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  /* NVIC init */
#ifndef  EMB_FLASH
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* SysTick end of count event each 0.01s with input clock equal to 9MHz (HCLK/8, default) */
  SysTick_Config(90000);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  /* Init LEDs */
  STM32_LEDInit(LED1);
  STM32_LEDInit(LED2);
  STM32_LEDInit(LED3);
  STM32_LEDInit(LED4);

  /* V - move input init */
  /* GPIO init*/
  STM32_AN_TR_Init(TRUE);

  /* DMA1 channel1 configuration ----------------------------------------------*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
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
  
  /* H - move input init */
  STM32_PBInit(BUTTON_WAKEUP,BUTTON_MODE_GPIO);
  STM32_PBInit(BUTTON_USER,BUTTON_MODE_GPIO);
  
  /* HID USB */
  HidInit();
  EXT_CRT_SECTION();

  /* Soft connection enable */
  USB_ConnectRes(TRUE);
  ADC_Hold = ADC_ConvertedValue;
  while(1)
  {
    if (SysTickFl)
    {
      SysTickFl = FALSE;
      if(UsbDevStatusConfigured == UsbCoreReq(UsbCoreReqDevState))
      {
        if(!UsbCoreReq(UsbCoreReqDevSusState))
        {
          Int8S h = 0, v = 0;
          int tmp;
          if(STM32_PBGetState(BUTTON_WAKEUP))
          {
            h += 5;
          }
          else if (!STM32_PBGetState(BUTTON_USER))
          {
            h -= 5;
          }
      
          tmp = ADC_Hold - ADC_ConvertedValue;
          tmp >>= 3;
          ADC_Hold = ADC_ConvertedValue;
          if(tmp > 127)
          {
            tmp = 127;
          }
          else if (tmp < -128)
          {
            tmp = -128;
          }
          v = tmp;
          if(h || v)
          {
            UsbWakeUp();
            STM32_LEDOn(LED1);
            HidMouseSendReport(h,v,0);
          }
          else
          {
            STM32_LEDOff(LED1);
          }
        }
      }
    }
  }
}
