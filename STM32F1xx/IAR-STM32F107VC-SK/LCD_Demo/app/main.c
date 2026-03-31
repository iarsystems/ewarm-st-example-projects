/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 8, September 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *   This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for the IAR-STM32F107VC-SK board. It shows basic use of
 *  the I/O, the timer, the interrupt controllers and the LDC module for graphic
 *  and text drawing.
 *   It starts by show the IAR logo on the LCD and after 5 second draw a test
 *  screen.
 *
 *   Controls:
 *  AN_TR                    - contrast / backlight
 *  Left Joystik position    - adjust backlight
 *  Right Joystik position   - adjust LCD contrast
 *
 *  Jumpers:
 *   PWR_SEL   - depending of power source
 *   JTAG_SEL  - open (on board j-link)
 *   B0_1/B0_0 - B0_0
 *   B1_1/B1_0 - B1_0
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US       450

#define JS_RIGHT_MASK   GPIO_Pin_2
#define JS_RIGHT_PORT   GPIOC
#define JS_LEFT_MASK    GPIO_Pin_7
#define JS_LEFT_PORT    GPIOE

#define TRIM_MASK       GPIO_Pin_0
#define TRIM_CH         ADC_Channel_10
#define TRIM_PORT       GPIOC

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

Int32U CriticalSecCntr;
volatile Boolean SysTickFl;
volatile Boolean CntrSel = FALSE;

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
  if(!(JS_RIGHT_MASK & JS_RIGHT_PORT->IDR))
  {
    CntrSel = TRUE;
  }
  else if (!(JS_LEFT_MASK & JS_LEFT_PORT->IDR))
  {
    CntrSel = FALSE;
  }
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
Int32U x,y,j,k;

// xxxxRRRRGGGGBBBB
const Int32U TableColor [] =
{
  0xFFF, 0xAAA, 0x888, 0x666, 0x444, 0x000,   // Grey
  0x00F, 0x00A, 0x008, 0x006, 0x004, 0x001,   // Red
  0x0F0, 0x0A0, 0x080, 0x060, 0x040, 0x010,   // Green
  0xF00, 0xA00, 0x800, 0x600, 0x400, 0x100,   // Blue
};

  for(y = 0; y < 4; ++y)
  {
    for(x = 0; x < 6; ++x)
    {
      // set rectangle
      k = (x*22) | ((((x+1)*22)-1)<<8);
      GLCD_SendCmd(CASET,(pInt8U)&k,0);
      k = (y*29) | ((((y+1)*29)-1)<<8);
      GLCD_SendCmd(RASET,(pInt8U)&k,0);
      // fill rectangle
      for(j = 0; j < (22*29); ++j)
      {
        GLCD_SendCmd(RAMWR,(pInt8U)&TableColor[x+(y*6)],2);
      }
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
Boolean SelHold;
Int32U AdcData;

  ENTR_CRT_SECTION();

  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);


  // SysTick end of count event each 0.1s with input clock equal to 9MHz (HCLK/8, default)
  SysTick_Config(900000);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  // Buttons port init
  // GPIO enable clock and release Reset
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOC
                         | RCC_APB2Periph_GPIOE, DISABLE);
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOC
                         | RCC_APB2Periph_GPIOE, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  JS_RIGHT_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(JS_RIGHT_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  JS_LEFT_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(JS_LEFT_PORT, &GPIO_InitStructure);

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

  // ADC1 regular channel14 configuration
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
  DelayResolution100us(30000);
  DrawTable();

  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(10,116,131,131);
  GLCD_TextSetPos(0,0);

  if(CntrSel)
  {
    SelHold = TRUE;
    GLCD_print("\fContrast adj.\r");
  }
  else
  {
    SelHold = FALSE;
    GLCD_print("\fBacklight adj.\r");
  }

  while(1)
  {
    if (SysTickFl)
    {
      SysTickFl = FALSE;
      // Get new sample of the ADC
      AdcData = ADC_GetConversionValue(ADC1) >> 6;
      ADC_SoftwareStartConvCmd(ADC1, ENABLE);
      if(SelHold)
      {
        // Contract adj
        AdcData += 0x38;
        GLCD_SendCmd(SETCON,(pInt8U)&AdcData,0);
      }
      else
      {
        // Backlight adj
        AdcData /= 2;
        AdcData += 0x40;
        GLCD_Backlight(AdcData);
      }
    }
    if(SelHold != CntrSel)
    {
      SelHold ^= 1;
      if(SelHold)
      {
        GLCD_print("\fContrast adj.\r");
      }
      else
      {
        GLCD_print("\fBacklight adj.\r");
      }
    }
  }
}
