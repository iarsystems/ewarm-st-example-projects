/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Potentiometer_Test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : Potentiometer_Test program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((u32)0x4001244C)
#define TRIM_MASK       GPIO_Pin_4
#define TRIM_CH         ADC_Channel_14
#define TRIM_PORT       GPIOC
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef   ADC_InitStructure;
DMA_InitTypeDef   DMA_InitStructure;
vu16 ADC_RegularConvertedValueTab[64], ADC_InjectedConvertedValueTab[32];
volatile unsigned long ADCConvertedValue;
volatile Boolean CntrSel = FALSE;

/* Private function prototypes -----------------------------------------------*/
void DrawTable (void);
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Potentiometer_Test
* Description    : Potentiometer_Test program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Potentiometer_Test(void)
{

ADC_InitTypeDef ADC_InitStructure;
Boolean SelHold;
u32 AdcData;

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);

  /* Display Test name */
  printf ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  printf( "Potentiometer Test\n ");

  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration*/
  ADC_RegularChannelConfig(ADC1, TRIM_CH, 1, ADC_SampleTime_55Cycles5);

  /* Disable ADC1 DMA */
  ADC_DMACmd(ADC1, DISABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */
  ADC_ResetCalibration(ADC1);

  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);

  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
  GLCD_TextSetPos(1,3);
  printf("Move the potentiometer");
  GLCD_TextSetPos(1,4);
  printf ("to adjust the contrast.");

  DrawTable();

  while(ReadKey() != User)
    {
      /* Get new sample of the ADC */
      AdcData = ADC_GetConversionValue(ADC1) >> 6;
      ADC_SoftwareStartConvCmd(ADC1, ENABLE);
      if(SelHold)
      {
        /* Contract adj */
        AdcData += 0x38;
        GLCD_SendCmd(SETCON,(pu8)&AdcData,0);
      }
      else
      {
        /* Backlight adj */
        AdcData /= 2;
        AdcData += 0x40;
        GLCD_Backlight(AdcData);
      }
    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,9);
    printf( "Press User key to exit\n ");
    }

  ReturnFunc();

  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
}

/*************************************************************************
 * Function Name: DrawTable
 * Parameters: none
 *
 * Return: none
 *
 * Description: Show color table on the GCLCD
 *
 *************************************************************************/
void DrawTable (void)
{
u32 x,y,j,k;

// xxxxRRRRGGGGBBBB
const u32 TableColor [] =
{
  0xFFF, 0xAAA, 0x888, 0x666, 0x444, 0x000,   // Grey
  0x00F, 0x00A, 0x008, 0x006, 0x004, 0x001,   // Blue
  0xF00, 0xA00, 0x800, 0x600, 0x400, 0x100,   // Red
  0x0F0, 0x0A0, 0x080, 0x060, 0x040, 0x010,   // Green

};

//  for(y = 0; y < 4; ++y)
//  {

y=3;
    for(x = 0; x < 6; ++x)
    {
      // set rectangle
      k = (x*22) | ((((x+1)*22)-1)<<8);
      GLCD_SendCmd(CASET,(pu8)&k,0);
      k = (y*29) | ((((y+1)*29)-1)<<8);
      GLCD_SendCmd(RASET,(pu8)&k,0);
      // fill rectangle
      for(j = 0; j < (22*16); ++j)
      {
        GLCD_SendCmd(RAMWR,(pu8)&TableColor[x+(y*6)],2);
      }
//    }
  }
}


/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
