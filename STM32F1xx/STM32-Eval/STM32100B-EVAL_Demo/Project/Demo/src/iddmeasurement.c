/**
  ******************************************************************************
  * @file     iddmeasurement.c
  * @author   MCD Application Team
  * @version  V1.0.0
  * @date     03/01/2010
  * @brief    This file includes the idd measurement driver for the STM3210B-EVAL 
  *           demonstration.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32100B_EVAL_Demo
  * @{
  */

/** @defgroup IDDMEASUREMENT
  * @brief    This file includes the idd measurement driver for the STM3210B-EVAL 
  *           demonstration.
  * @{
  */

/** @defgroup IDDMEASUREMENT_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup IDDMEASUREMENT_Private_Defines
  * @{
  */
#define EVAL_RESISTOR_LCD_REG_71         125      /* 1.25 ohm  */
#define EVAL_RESISTOR_LCD_REG_72         100000   /* 1000 ohm  */
#define EVAL_RESISTOR_STT5PF20V          8        /* 0.085 ohm  */
#define EVAL_RESISTOR_RATIO              100      /* LCD_REG_71 and LCD_REG_72 are multiplied by 100 */
#define EVAL_MAX9938_GAIN                50       /* Ampli-op gain = 50 */
#define ADC_CONVERT_RATIO                806      /* (3300mV / 0xFFF)* 1000 */

/**
  * @}
  */

/** @defgroup IDDMEASUREMENT_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup IDDMEASUREMENT_Private_Variables
  * @{
  */
uint16_t ADCdata = 0;
uint32_t VDDmV = 0;
uint32_t IDDrunuA = 0;
/**
  * @}
  */

/** @defgroup IDDMEASUREMENT_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup IDDMEASUREMENT_Private_Functions
  * @{
  */
  
/**
  * @brief  IDD run mode measurement program.
  * @param  None
  * @retval None
  */
void IDD_Measurement_Run(void)
{
  __IO uint8_t current1000, current100, current10, current1, current01, current001 = 0;
  uint8_t pressedkey, str[20];
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;

  /* Disable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(DISABLE);

  while (Menu_ReadKey() != NOKEY)
  {}

  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_2, "      Run Mode      ");
  LCD_DisplayStringLine(LCD_LINE_5, "   To exit press    ");
  LCD_DisplayStringLine(LCD_LINE_6, "      Joystick      ");
  RCC_ADCCLKConfig(RCC_PCLK2_Div2);

  /* Configure PC.05 (ADC Channel14) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel15 configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

  /* Check which key is pressed */
  pressedkey = Menu_ReadKey();

  while (pressedkey == NOKEY)
  {
    /* Check which key is pressed */
    pressedkey = Menu_ReadKey();

    /* Wait until ADC Channel 15 end of conversion */
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
    {}

    ADCdata = ADC_GetConversionValue(ADC1);

    /* Calculate voltage value*/
    VDDmV = (uint32_t)((uint32_t)ADCdata * (uint32_t)ADC_CONVERT_RATIO) / 1000;

    /* calculate the current consumption */
    IDDrunuA = (uint32_t) ((uint32_t)(((VDDmV * 1000 * EVAL_RESISTOR_RATIO) / EVAL_MAX9938_GAIN))\
                           / (uint32_t)(EVAL_RESISTOR_LCD_REG_71));

    /* x  current value*/
    current1000 = (uint8_t)(IDDrunuA / 100000);
    /* Hundreds current value */
    current100 = (uint8_t)((IDDrunuA % 100000) / 10000);
    /* Tens current value */
    current10 = (uint8_t)((IDDrunuA % 10000) / 1000);
    /* 1mA current value */
    current1 = (uint8_t)((IDDrunuA % 1000) / 100);
    /* 0.1 mA current value */
    current01 = (uint8_t)((IDDrunuA % 100 ) / 10);
    /* 0.01 mA current value */
    current001 =  (uint8_t)(IDDrunuA % 10);

    sprintf ((char*)str, "   IDD: %d%d%d,%d%d%dmA   ", current1000, current100, current10, current1, current01, current001); /** Format the string */
    LCD_DisplayStringLine(LCD_LINE_3, str);

  }

  /* Disable ADC1 */
  ADC_Cmd(ADC1, DISABLE);

  /* Stop ADC1 Software Conversion */
  ADC_SoftwareStartConvCmd(ADC1, DISABLE);

  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);

  /* Display the previous menu */
  Menu_DisplayMenu();

  /* Enable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(ENABLE);
}

/**
  * @brief  IDD sleep mode measurement program.
  * @param  None
  * @retval None
  */
void IDD_Measurement_Sleep(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  __IO uint32_t current1000, current100, current10, current1, current01, current001, i = 0;
  uint32_t result;
  uint8_t str[20];
  ADC_InitTypeDef ADC_InitStructure;

  /* Disable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(DISABLE);

  while (Menu_ReadKey() != NOKEY)
  {}

  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_2, "    SLEEP Mode      ");

  RCC_ADCCLKConfig(RCC_PCLK2_Div2);

  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel15 configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable the TIM1 UP Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM16_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* SPI2 TIM6 and DAC Periph clock disable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB1Periph_TIM6 |
                         RCC_APB1Periph_DAC, DISABLE);
  /* Disable GPIOA, GPIOB, GPIOC, TIM1 and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC, DISABLE);
  /* Disable the SRAM and FLITF clock in Stop mode */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM | RCC_AHBPeriph_FLITF, DISABLE);

  /* Mode: SLEEP + Entry with WFI */
  __WFI();

  /* Enable the TIM1 UP Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM16_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  /* initialize result*/
  result = 0;

  for (i = 4; i > 0; i--)
  {
    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    /* Wait until ADC Channel 15 end of conversion */
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
    {}
    /* read ADC convertion result */
    ADCdata = ADC_GetConversionValue(ADC1);
    result += ADCdata;
  }
  result >>= 2;

  /* SPI2 TIM6 and DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB1Periph_TIM6 |
                         RCC_APB1Periph_DAC, ENABLE);
  /* ENABLE GPIOA, GPIOB, GPIOC, TIM1 and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO |
                         RCC_APB2Periph_TIM1, ENABLE);
  /* Disable the SRAM and FLITF clock in Stop mode */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM | RCC_AHBPeriph_FLITF, ENABLE);

  /* Calculate voltage value*/
  VDDmV = (uint32_t)((uint32_t)result * (uint32_t)ADC_CONVERT_RATIO) / 1000;

  /* calculate the current consumption */
  IDDrunuA = (uint32_t) ((uint32_t)(((VDDmV * 1000 * EVAL_RESISTOR_RATIO) / EVAL_MAX9938_GAIN))\
                         / (uint32_t)(EVAL_RESISTOR_LCD_REG_71));
  /* x  current value*/
  current1000 = (uint8_t)(IDDrunuA / 100000);
  /* Hundreds current value */
  current100 = (uint8_t)((IDDrunuA % 100000) / 10000);
  /* Tens current value */
  current10 = (uint8_t)((IDDrunuA % 10000) / 1000);
  /* 1mA current value */
  current1 = (uint8_t)((IDDrunuA % 1000) / 100);
  /* 0.1 mA current value */
  current01 = (uint8_t)((IDDrunuA % 100 ) / 10);
  /* 0.01 mA current value */
  current001 =  (uint8_t)(IDDrunuA % 10);

  sprintf ((char*)str, "   IDD: %d%d%d,%d%d%dmA   ", current1000, current100, current10, current1, current01, current001); /** Format the string */
  LCD_DisplayStringLine(LCD_LINE_3, str);
  LCD_DisplayStringLine(LCD_LINE_5, "   To exit press    ");
  LCD_DisplayStringLine(LCD_LINE_6, "      Joystick      ");

  while (Menu_ReadKey() == NOKEY)
  {}

  /* Disable ADC1 */
  ADC_Cmd(ADC1, DISABLE);
  /* Stop ADC1 Software Conversion */
  ADC_SoftwareStartConvCmd(ADC1, DISABLE);

  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  /* Display the previous menu */
  Menu_DisplayMenu();
  /* Enable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(ENABLE);
}
/**
  * @brief  IDD stop mode measurement program.
  * @param  None
  * @retval None
  */
void IDD_Measurement_Stop(void)
{
  __IO uint32_t  current1, current01, current001, i = 0;
  uint32_t result;
  uint8_t str[20];
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Disable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(DISABLE);

  while (Menu_ReadKey() != NOKEY)
  {}

  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_2, "     STOP Mode      ");

  RCC_ADCCLKConfig(RCC_PCLK2_Div2);

  /* Configure PC.05 (ADC Channel15) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel15 configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5);

  /* Configure PA.0 (LP_WAKEUP) as output push-pull -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  /* Disable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(DISABLE);

  /* Configure the EXTI Line 0 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable the EXTI0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Clear the RTC Alarm flag */
  RTC_ClearFlag(RTC_FLAG_ALR);

  /* Save the GPIO pins current configuration then put all GPIO pins in Analog
     Input mode ...*/
  LowPower_SaveGPIOConfig();
  /* ... and keep PA.00 configuration which will be used as EXTI LCD_LINE_0 source */
  GPIOA->CRL = 0x4;

  /* Configure PE.14 (IDD_CNT_EN) as output push-pull -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Clear PE.14 (IDD_CNT_EN) */
  GPIO_ResetBits(GPIOE, GPIO_Pin_14);

  /* Disable the SRAM and FLITF clock in Stop mode */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM | RCC_AHBPeriph_FLITF, DISABLE);

  /* Request to enter STOP mode with regulator in low power */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

  /* Restore the GPIO Configurations*/
  LowPower_RestoreGPIOConfig();

  /* Configures system clock after wake-up from STOP: enable HSE, PLL and select PLL
     as system clock source (HSE and PLL are disabled in STOP mode) */
  LowPower_SYSCLKConfig_STOP();

  /* Reset the counter by programming IDD_CNT_EN High in less than 70ms after
  the wakeup to avoid 1Kohm to be connected later on VDD_MCU */
  GPIO_SetBits(GPIOE, GPIO_Pin_14);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  for (i = 0; i < 25; i++);
  /* initialize result*/
  result = 0;

  for (i = 4; i > 0; i--)
  {
    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    /* Wait until ADC Channel 15 end of conversion */
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
    {}
    /* read ADC convertion result */
    ADCdata = ADC_GetConversionValue(ADC1);
    result += ADCdata;
  }
  result >>= 2;

  /* At this stage the system has resumed from STOP mode ************************/
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable the EXTI0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the SRAM and FLITF clock in Stop mode */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM | RCC_AHBPeriph_FLITF, ENABLE);

  /* Calculate voltage value*/
  VDDmV = (uint32_t)((uint32_t)result * (uint32_t)ADC_CONVERT_RATIO) / 1000;

  /* calculate the current consumption */
  IDDrunuA = (uint32_t) ((uint32_t)(((VDDmV * 1000 * EVAL_RESISTOR_RATIO) / EVAL_MAX9938_GAIN))\
                         / (uint32_t)(EVAL_RESISTOR_LCD_REG_71 + EVAL_RESISTOR_LCD_REG_72 + EVAL_RESISTOR_STT5PF20V));
  /* 1mA current value */
  current1 = (uint8_t)((IDDrunuA % 1000) / 100);
  /* 0.1 mA current value */
  current01 = (uint8_t)((IDDrunuA % 100 ) / 10);
  /* 0.01 mA current value */
  current001 =  (uint8_t)(IDDrunuA % 10);

  sprintf ((char*)str, "     IDD: %d%d%duA     ", current1, current01, current001); /** Format the string */
  LCD_DisplayStringLine(LCD_LINE_3, str);
  LCD_DisplayStringLine(LCD_LINE_5, "   To exit press    ");
  LCD_DisplayStringLine(LCD_LINE_6, "      Joystick      ");


  while (Menu_ReadKey() == NOKEY)
  {}

  /* Disable ADC1 */
  ADC_Cmd(ADC1, DISABLE);

  /* Stop ADC1 Software Conversion */
  ADC_SoftwareStartConvCmd(ADC1, DISABLE);

  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  /* Display the previous menu */
  Menu_DisplayMenu();
  /* Enable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(ENABLE);
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
