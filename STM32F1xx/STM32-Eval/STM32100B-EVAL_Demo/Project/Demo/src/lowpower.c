/**
  ******************************************************************************
  * @file     lowpower.c
  * @author   MCD Application Team
  * @version  V1.0.0
  * @date     03/01/2010
  * @brief    This file includes the low power driver for the STM32100B-EVAL 
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

/** @defgroup LOWPOWER
  * @brief    This file includes the low power driver for the STM32100B-EVAL 
  *           demonstration.
  * @{
  */

/** @defgroup LOWPOWER_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Private_Variables
  * @{
  */
static __IO uint8_t STOPModeStatus = 0;
static uint32_t GPIOA_CRL = 0, GPIOA_CRH = 0, GPIOB_CRL = 0x300, GPIOB_CRH = 0xBBB00000,
                GPIOC_CRL = 0, GPIOC_CRH = 0, GPIOD_CRL = 0, GPIOD_CRH = 0x04040004,
                GPIOE_CRL = 0x44, GPIOE_CRH = 0;
/**
  * @}
  */

/** @defgroup LOWPOWER_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Private_Functions
  * @{
  */

/**
  * @brief  Initializes Low Power application.
  * @param  None
  * @retval None
  */
void LowPower_Init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* Enable WakeUp pin */
  PWR_WakeUpPinCmd(ENABLE);
  /* Enable Clock Security System(CSS) */
  RCC_ClockSecuritySystemCmd(ENABLE);
  /* Enable Debug feature in low power modes (Sleep, STOP and STANDBY) */
  DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY, ENABLE);
}


/**
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
void LowPower_SYSCLKConfig_STOP(void)
{
  ErrorStatus HSEStartUpStatus;
  
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  
  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }
}

/**
  * @brief  Enters MCU in STOP mode. The wake-up from STOP mode is performed by 
  *         an external interrupt.
  * @param  None
  * @retval None
  */
void LowPower_EnterSTOPMode_EXTI(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  STOPModeStatus = 0;

  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  /* Disable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(DISABLE);
  /* Configure the EXTI Line 9 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  /* Enable the EXTI9_5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  LCD_DisplayStringLine(LCD_LINE_4, "  MCU in STOP Mode  ");
  LCD_DisplayStringLine(LCD_LINE_5, "To exit press Key   ");
  LCD_DisplayStringLine(LCD_LINE_6, "push button         ");
  /* Clear the RTC Alarm flag */
  RTC_ClearFlag(RTC_FLAG_ALR);

  /* Save the GPIO pins current configuration then put all GPIO pins in Analog
     Input mode ...*/
  LowPower_SaveGPIOConfig();
  /* ... and keep PB.09 configuration which will be used as EXTI Line9 source */
  GPIOB->CRH = 0x40;
  /* ... and keep PE.07 configuration which will be used as EXTI Line9 source */
  GPIOE->CRL = 0x80000000;
  GPIOE->BSRR = GPIO_Pin_7;
  /* Request to enter STOP mode with regulator in low power */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

  /* Restore the GPIO Configurations*/
  LowPower_RestoreGPIOConfig();
  /* At this stage the system has resumed from STOP mode ************************/
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);
  /* Enable the EXTI9_5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* Configures system clock after wake-up from STOP: enable HSE, PLL and select PLL
     as system clock source (HSE and PLL are disabled in STOP mode) */
  LowPower_SYSCLKConfig_STOP();
  if (STOPModeStatus != RESET)
  {
    LCD_DisplayStringLine(LCD_LINE_4, "      STOP Mode     ");
    LCD_DisplayStringLine(LCD_LINE_5, "Wake-Up by RTC Alarm");
    LCD_DisplayStringLine(LCD_LINE_6, "Press JoyStick to   ");
    LCD_DisplayStringLine(LCD_LINE_7, "continue...         ");
  }
  else
  {
    LCD_DisplayStringLine(LCD_LINE_4, "      STOP Mode     ");
    LCD_DisplayStringLine(LCD_LINE_5, "WakeUp by Key Button");
    LCD_DisplayStringLine(LCD_LINE_6, "Press JoyStick to   ");
    LCD_DisplayStringLine(LCD_LINE_7, "continue...         ");
  }

  while (Menu_ReadKey() == NOKEY)
  {}
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  /* Display the previous menu */
  Menu_DisplayMenu();
  /* Enable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(ENABLE);
}

/**
  * @brief  Enters MCU in STOP mode. The wake-up from STOP mode is performed by 
  *         an RTC Alarm.
  * @param  None
  * @retval None
  */
void LowPower_EnterSTOPMode_RTCAlarm(void)
{
  uint32_t tmp = 0;
  
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
  /* Disable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(DISABLE);
  
  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
    LCD_DisplayStringLine(LCD_LINE_1, "Time and Date are   ");
    LCD_DisplayStringLine(LCD_LINE_2, "not configured,     ");
    LCD_DisplayStringLine(LCD_LINE_3, "please go to the    ");
    LCD_DisplayStringLine(LCD_LINE_4, "calendar menu and   ");
    LCD_DisplayStringLine(LCD_LINE_5, "set the time and    ");
    LCD_DisplayStringLine(LCD_LINE_6, "date parameters.    ");
    LCD_DisplayStringLine(LCD_LINE_7, "Press JoyStick to   ");
    LCD_DisplayStringLine(LCD_LINE_8, "continue...         ");
    while (Menu_ReadKey() == NOKEY)
    {}
    /* Clear the LCD */
    LCD_Clear(LCD_COLOR_WHITE);
    /* Display the previous menu */
    Menu_DisplayMenu();
    /* Enable the JoyStick interrupts */
    Demo_IntExtOnOffConfig(ENABLE);
    return;
  }
  tmp = RTC_GetCounter();
  /* Save the Alarm value in the Backup register */
  BKP_WriteBackupRegister(BKP_DR6, (tmp & 0x0000FFFF));
  BKP_WriteBackupRegister(BKP_DR7, (tmp >> 16));

  Calendar_AlarmPreAdjust();
  LCD_ClearLine(LCD_LINE_8);
  LCD_DisplayStringLine(LCD_LINE_6, "  MCU in STOP Mode  ");
  LCD_DisplayStringLine(LCD_LINE_7, " Wait For RTC Alarm ");

  /* Save the GPIO pins current configuration then put all GPIO pins in Analog Input mode */
  LowPower_SaveGPIOConfig();

  /* Request to enter STOP mode with regulator in low power */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
  /* Restore the GPIO Configurations*/
  LowPower_RestoreGPIOConfig();

  /* Configures system clock after wake-up from STOP: enable HSE, PLL and select PLL
     as system clock source (HSE and PLL are disabled in STOP mode) */
  LowPower_SYSCLKConfig_STOP();
  LCD_DisplayStringLine(LCD_LINE_4, "      STOP Mode     ");
  LCD_DisplayStringLine(LCD_LINE_5, "Wake-Up by RTC Alarm");
  LCD_DisplayStringLine(LCD_LINE_6, "Press JoyStick to   ");
  LCD_DisplayStringLine(LCD_LINE_7, "continue...         ");
  while (Menu_ReadKey() == NOKEY)
  {}
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  /* Display the previous menu */
  Menu_DisplayMenu();
  /* Enable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(ENABLE);
}

/**
  * @brief  Enters MCU in STANDBY mode. The wake-up from STANDBY mode is performed 
  *         when a rising edge is detected on WKP_STDBY pin.
  * @param  None
  * @retval None
  */
void LowPower_EnterSTANDBYMode_WAKEUP(void)
{
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
  LCD_DisplayStringLine(LCD_LINE_7, " MCU in STANDBY Mode");
  LCD_DisplayStringLine(LCD_LINE_8, "To exit press Wakeup");
  
  /* Request to enter STANDBY mode (Wake Up flag is cleared in PWR_EnterSTANDBYMode function) */
  PWR_EnterSTANDBYMode();
}

/**
  * @brief  Enters MCU in STANDBY mode. The wake-up from STANDBY mode is performed 
  *         by an RTC Alarm event.
  * @param  None
  * @retval None
  */
void LowPower_EnterSTANDBYMode_RTCAlarm(void)
{
  uint32_t tmp = 0;
  
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  /* Disable the JoyStick interrupts */
  Demo_IntExtOnOffConfig(DISABLE);
  
  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
    LCD_DisplayStringLine(LCD_LINE_1, "Time and Date are   ");
    LCD_DisplayStringLine(LCD_LINE_2, "not configured,     ");
    LCD_DisplayStringLine(LCD_LINE_3, "please go to the    ");
    LCD_DisplayStringLine(LCD_LINE_4, "calendar menu and   ");
    LCD_DisplayStringLine(LCD_LINE_5, "set the time and    ");
    LCD_DisplayStringLine(LCD_LINE_6, "date parameters.    ");
    LCD_DisplayStringLine(LCD_LINE_7, "Press JoyStick to   ");
    LCD_DisplayStringLine(LCD_LINE_8, "continue...         ");
    while (Menu_ReadKey() == NOKEY)
    {}
    /* Clear the LCD */
    LCD_Clear(LCD_COLOR_WHITE);
    /* Display the previous menu */
    Menu_DisplayMenu();
    /* Enable the JoyStick interrupts */
    Demo_IntExtOnOffConfig(ENABLE);
    return;
  }

  tmp = RTC_GetCounter();
  /* Save the Alarm value in the Backup register */
  BKP_WriteBackupRegister(BKP_DR6, (tmp & 0x0000FFFF));
  BKP_WriteBackupRegister(BKP_DR7, (tmp >> 16));
  Calendar_AlarmPreAdjust();
  LCD_DisplayStringLine(LCD_LINE_7, " MCU in STANDBY Mode");
  LCD_DisplayStringLine(LCD_LINE_8, " Wait For RTC Alarm ");
  /* Request to enter STANDBY mode (Wake Up flag is cleared in PWR_EnterSTANDBYMode function) */
  PWR_EnterSTANDBYMode();
}

/**
  * @brief  Sets STOPModeStatus variable.
  * @param  None
  * @retval None
  */
void LowPower_Set_STOPModeStatus(void)
{
  STOPModeStatus = 1;
}

/**
  * @brief  Save all GPIOs Configurations.
  * @param  None
  * @retval None
  */
void LowPower_SaveGPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIOA_CRL = GPIOA->CRL;
  GPIOA_CRH = GPIOA->CRH;

  GPIOB_CRL = GPIOB->CRL;
  GPIOB_CRH = GPIOB->CRH;

  GPIOC_CRL = GPIOC->CRL;
  GPIOC_CRH = GPIOC->CRH;

  GPIOD_CRL = GPIOD->CRL;
  GPIOD_CRH = GPIOD->CRH;

  GPIOE_CRL = GPIOE->CRL;
  GPIOE_CRH = GPIOE->CRH;

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/**
  * @brief  Restores all GPIOs Configurations.
  * @param  None
  * @retval None
  */
void LowPower_RestoreGPIOConfig(void)
{
  GPIOA->CRL = GPIOA_CRL;
  GPIOA->CRH = GPIOA_CRH;

  GPIOB->CRL = GPIOB_CRL;
  GPIOB->CRH = GPIOB_CRH;

  GPIOC->CRL = GPIOC_CRL;
  GPIOC->CRH = GPIOC_CRH;

  GPIOD->CRL = GPIOD_CRL;
  GPIOD->CRH = GPIOD_CRH;

  GPIOE->CRL = GPIOE_CRL;
  GPIOE->CRH = GPIOE_CRH;
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
