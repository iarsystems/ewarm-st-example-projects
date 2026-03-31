/**
  ******************************************************************************
  * @file     main.c
  * @author   MCD Application Team
  * @version  V1.0.0
  * @date     03/01/2010
  * @brief    Main program body
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

/** @defgroup MAIN
  * @brief    Main program body
  * @{
  */

/** @defgroup MAIN_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Private_Variables
  * @{
  */
static __IO uint32_t TimingDelay = 0;
static __IO uint32_t LedShowStatus = 0;

/**
  * @}
  */

/** @defgroup MAIN_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Private_Functions
  * @{
  */

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Initialize the Demo */
  Demo_Init();

  while (1)
  {}
}

/**
  * @brief  Initializes the demonstration application.
  * @param  None
  * @retval None
  */
void Demo_Init(void)
{
  /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

  /* TIM1 and ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_ADC1, ENABLE);

  /* SPI2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  /* TIM6 and DAC clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 | RCC_APB1Periph_DAC, ENABLE);

  /*------------------- Resources Initialization -----------------------------*/

  /* GPIO Configuration */
  Demo_GPIOConfig();

  /* Interrupt Configuration */
  Demo_InterruptConfig();

  /* Configure the systick */
  Demo_SysTickConfig();

  /*------------------- Drivers Initialization -------------------------------*/
  /* Initialize the LEDs toogling */
  Demo_LedShowInit();

  /* Initialize the Low Power application */
  LowPower_Init();

  /* Initialize the LCD */
  STM32100B_LCD_Init();

  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);

  /* If HSE is not detected at program startup */
  if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {
    /* Generate NMI exception */
    SCB->ICSR |= SCB_ICSR_NMIPENDSET;
  }

  /* Checks the availability of the bitmap files */
  Demo_CheckBitmapFilesStatus();

  /* Initialize Wave player application */
  WavePlayer_Init();

  /* Display the STM32 introduction */
  Menu_STM32Intro();

  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);

  /* Initialize the Calendar */
  Calendar_Init();

  /* Initialize the Thermometer application */
  LM75_Init();

  /* Enable Leds toggling */
  Demo_LedShow(ENABLE);

  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);

  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);

  /* Initialize the Menu */
  Menu_Init();
  
  /* Display the main menu icons */
  Menu_ShowMenuIcons();
}

/**
  * @brief  Configures the used IRQ Channels and sets their priority.
  * @param  None
  * @retval None
  */
void Demo_InterruptConfig(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Set the Vector Table base address at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);

  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the EXTI9_5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the EXTI15_10 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* Enable the TIM6 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the TIM1 UP Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM16_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* Enable the DMA1_Channel3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the EXTI LCD_LINE_17 Interrupt */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable the CEC global Interrupt (with higher priority) */
  NVIC_InitStructure.NVIC_IRQChannel = CEC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Re-configure and enable I2C1 error interrupt to have the higher priority */
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure a SysTick Base time to 10 ms.
  * @param  None
  * @retval None
  */
void Demo_SysTickConfig(void)
{
  /* Setup SysTick Timer for 10 msec interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
    while (1);
  }

  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/**
  * @brief  Enables or disables EXTI for the menu navigation keys :
  *         EXTI lines 8, 12 and 14 which correpond respectively
  *         to "UP", "SEL" and "DOWN".
  * @param  NewState: New state of the navigation keys. This parameter
  *                   can be: ENABLE or DISABLE.
  * @retval None
  */
void Demo_IntExtOnOffConfig(FunctionalState NewState)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  /* Initializes the EXTI_InitStructure */
  EXTI_StructInit(&EXTI_InitStructure);
  /* Disable the EXTI line 8, 12 and 14 on falling edge */
  if (NewState == DISABLE)
  {
    EXTI_InitStructure.EXTI_Line = EXTI_Line8 | EXTI_Line12 | EXTI_Line14;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
  /* Enable the EXTI line 8, 12 and 14 on falling edge */
  else
  {
    /* Clear the the EXTI line 8, 12 and 14 interrupt pending bit */
    EXTI_ClearITPendingBit(EXTI_Line8 | EXTI_Line12 | EXTI_Line14);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line8 | EXTI_Line12 | EXTI_Line14;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
}

/**
  * @brief  Configures the different GPIO ports pins.
  * @param  None
  * @retval None
  */
void Demo_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure PD.08, PD.12 and PD.14 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_12 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Configure PE.00 and PE.01 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Configure PB.09 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* RIGHT Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);
  /* LEFT Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource1);
  /* DOWN Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource14);
  /* UP Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8);
  /* SEL Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource12);
  /* KEY Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length (time base 10 ms).
  * @retval None
  */
void Demo_Delay(__IO uint32_t nCount)
{
  TimingDelay = nCount;
  /* Enable the SysTick Counter */
  SysTick->CTRL |= SysTick_CTRL_ENABLE;
  while (TimingDelay != 0);
  /* Disable the SysTick Counter */
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
  /* Clear the SysTick Counter */
  SysTick->VAL = (uint32_t)0x0;
}

/**
  * @brief  Inserts a delay time while no joystick RIGHT, LEFT and SEL
  *         are pressed.
  * @param  nTount: specifies the delay time length (time base 10 ms).
  * @retval Pressed Key. This value can be: NOKEY, RIGHT, LEFT or SEL.
  */
uint32_t Demo_DelayJoyStick(__IO uint32_t nTime)
{
  __IO uint32_t keystate = 0;
  TimingDelay = nTime;

  while ((TimingDelay != 0) && (keystate != RIGHT) && (keystate != LEFT) && (keystate != SEL))
  {
    keystate = Menu_ReadKey();
  }

  return keystate;
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void Demo_DecrementTimingDelay(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

/**
  * @brief  Configure the leds pins as output pushpull: LED1, LED2, LED3
  *         and LED4
  * @param  None
  * @retval None
  */
void Demo_LedShowInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);
  /* PC.06, PC.07, PC.08 and PC.09 as output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 719;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0x0AD4;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  /* Channel 1, 2, 3 and 4 Configuration in Timing mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 0x0;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
}


/**
  * @brief  Enables or disables LED1, LED2, LED3 and LED4 toggling.
  * @param  None
  * @retval None
  */
void Demo_LedShow(FunctionalState NewState)
{
  /* Enable LEDs toggling */
  if (NewState == ENABLE)
  {
    LedShowStatus = 1;
    /* Enable the TIM Update Interrupt */
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    /* TIM counter enable */
    TIM_Cmd(TIM1, ENABLE);
  }
  /* Disable LEDs toggling */
  else
  {
    LedShowStatus = 0;
    /* Disable the TIM Update Interrupt */
    TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
    /* TIM counter disable */
    TIM_Cmd(TIM1, DISABLE);
  }
}

/**
  * @brief  Get the LedShowStatus value.
  * @param  None
  * @retval LedShowStatus Value.
  */
uint32_t Demo_Get_LedShowStatus(void)
{
  return LedShowStatus;
}

/**
  * @brief  Checks the bitmap files availability and display a warning
  *         message if these files doesn't exit.
  * @param  None
  * @retval None
  */
void Demo_CheckBitmapFilesStatus(void)
{
  /* Checks if the Bitmap files are loaded */
  if (Menu_CheckBitmapFiles() != 0)
  {
    /* Set the LCD Back Color */
    LCD_SetBackColor(LCD_COLOR_BLUE);
    /* Set the LCD Text Color */
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_0, "      Warning       ");
    LCD_DisplayStringLine(LCD_LINE_1, "No loaded Bitmap    ");
    LCD_DisplayStringLine(LCD_LINE_2, "files. Demo can't be");
    LCD_DisplayStringLine(LCD_LINE_3, "executed.           ");
    LCD_DisplayStringLine(LCD_LINE_4, "Please be sure that ");
    LCD_DisplayStringLine(LCD_LINE_5, "all files are       ");
    LCD_DisplayStringLine(LCD_LINE_6, "correctly programmed");
    LCD_DisplayStringLine(LCD_LINE_7, "in the MicroSD Card ");
    LCD_DisplayStringLine(LCD_LINE_8, "and restart the Demo");
    LCD_DisplayStringLine(LCD_LINE_9, "                    ");

    /* Deinitializes the RCC */
    RCC_DeInit();

    /* Demo Can't Start */
    while (1)
    {}
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_0, "   Assert Function  ");
  /* Infinite loop */
  while (1)
  {}
}
#endif
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
