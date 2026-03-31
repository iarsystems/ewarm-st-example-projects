/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : temp_measure.c
 *    Description : Temperature measure functions
 *
 *    History :
 *    1. Date        : Nov 2, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

/** include files **/
#include "temp_measure.h"

Int32U DataBuf[16];
Int32U Result;
Int8U  DataInd;

/*************************************************************************
 * Function Name: InitTermMeasure
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init NTC analog channel
 *
 *************************************************************************/
void InitTermMeasure (void)
{
ADC_InitTypeDef ADC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

DataInd = 0;
  Result  = 0;
  for(int i = 0; i< 16; ++i)
  {
    DataBuf[i] = 0;
  }

  /* GPIO Initialization */
  /* GPIO3 Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_GPIO3 , ENABLE);
  /* GPIO3 Configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_HI_AIN_TRI;
  GPIO_InitStructure.GPIO_Pins = 0x2;
  GPIO_Init (GPIO3, &GPIO_InitStructure);

  /* ADC Clock Enable */
  CFG_PeripheralClockConfig(CFG_CLK_ADC, ENABLE);
  /* ADC Configuration */
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Calibration = ADC_Calibration_ON;
  ADC_InitStructure.ADC_CalibAverage = ADC_CalibAverage_Enable;
  ADC_InitStructure.ADC_AutoClockOff = ADC_AutoClockOff_Disable ;
  ADC_InitStructure.ADC_ConversionMode = ADC_ConversionMode_Scan ;
  ADC_InitStructure.ADC_SamplingPrescaler = 0x7;
  ADC_InitStructure.ADC_ConversionPrescaler = 0x0;
  ADC_InitStructure.ADC_FirstChannel = ADC_CHANNEL1 ;
  ADC_InitStructure.ADC_ChannelNumber = 1;
  ADC_Init (&ADC_InitStructure);

  /* Enable ADC */
  ADC_Cmd(ENABLE);

  for(int i = 16; i ; --i)
  {
    MeasureTerm();
  }
}

/*************************************************************************
 * Function Name: MeasureTerm
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Measure from NTC channel and filtering the result
 *
 *************************************************************************/
Int32U MeasureTerm (void)
{
Int32U * pData = &DataBuf[DataInd++&0xF];

// Start measure
  ADC_ConversionCmd (ADC_ConversionStart);
  while(ADC_FlagStatus(ADC_IT_EOC) == RESET);
  ADC_FlagClear( ADC_IT_EOC);
  Result -= *pData;
  *pData = ADC_GetConversionValue(ADC_CHANNEL1);
  Result += *pData;
  return(Result >> 4);
}

