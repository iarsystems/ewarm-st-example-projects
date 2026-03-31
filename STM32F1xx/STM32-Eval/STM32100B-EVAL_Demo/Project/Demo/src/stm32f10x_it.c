/**
  ******************************************************************************
  * @file     stm32f10x_it.c
  * @author   MCD Application Team
  * @version  V1.0.0
  * @date     03/01/2010
  * @brief    Main Interrupt Service Routines.
  *           This file provides template for all exceptions handler and 
  *           peripherals interrupt service routine.
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

/** @defgroup IT
  * @brief Main Interrupt Service Routines.
  *        This file provides template for all exceptions handler and peripherals 
  *        interrupt service routine.
  * @{
  */

/** @defgroup IT_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup IT_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup IT_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup IT_Private_Variables
  * @{
  */
__IO uint8_t Index = 0;
__IO uint32_t wavecounter = 0;
static __IO uint8_t AlarmStatus = 0;
static __IO uint8_t LedCounter = 0;
uint8_t Wavebuffer[512];
extern uint8_t Wavebuffer2[];
extern FILEINFO fiwave;
uint32_t var, dmaindex = 0;
extern uint8_t sector[SECTOR_SIZE];
extern VOLINFO  vi;
extern DIRINFO  di;
extern uint8_t Pressed_Key_Button;
extern __IO uint8_t SMbusAlertOccurred, CEC_Menu;
/**
  * @}
  */


/** @defgroup IT_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */


/** @defgroup IT_Private_Functions
  * @{
  */
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
  uint32_t tmp = 318, index = 0;
  
  /* Disable LCD Window mode */
  LCD_WindowModeDisable();
  
  /* If HSE is not detected at program startup or HSE clock failed during program execution */
  if ((RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) || (RCC_GetITStatus(RCC_IT_CSS) != RESET))
  {
    /* Clear the LCD */
    LCD_Clear(LCD_COLOR_WHITE);
    /* Set the LCD Back Color */
    LCD_SetBackColor(LCD_COLOR_BLUE);
    /* Set the LCD Text Color */
    LCD_SetTextColor(LCD_COLOR_WHITE);
    /* Display " No Clock Detected  " message */
    LCD_DisplayStringLine(LCD_LINE_0, "No HSE Clock         ");
    LCD_DisplayStringLine(LCD_LINE_1, "Detected. STANDBY    ");
    LCD_DisplayStringLine(LCD_LINE_2, "mode in few seconds. ");

    LCD_DisplayStringLine(LCD_LINE_5, "If HSE Clock         ");
    LCD_DisplayStringLine(LCD_LINE_6, "recovers before the  ");
    LCD_DisplayStringLine(LCD_LINE_7, "time out, a System   ");
    LCD_DisplayStringLine(LCD_LINE_8, "Reset is generated.  ");
    LCD_ClearLine(LCD_LINE_9);
    /* Clear Clock Security System interrupt pending bit */
    RCC_ClearITPendingBit(RCC_IT_CSS);
    GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);
    LCD_ClearLine(LCD_LINE_4);
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_WHITE);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_RED);
    LCD_DrawRect(71, 319, 25, 320);
    LCD_SetBackColor(LCD_COLOR_GREEN);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    /* Wait till HSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {
      if (index == 0x3FFFF)
      {
        LCD_DisplayChar(LCD_LINE_3, tmp, 0x20);
        tmp -= 16;
        index = 0;
      }
      index++;
      /* Enters the system in STANDBY mode */
      if (tmp < 16)
      {
        LCD_SetBackColor(LCD_COLOR_BLUE);
        LCD_ClearLine(LCD_LINE_3);
        LCD_ClearLine(LCD_LINE_4);
        LCD_ClearLine(LCD_LINE_5);
        LCD_ClearLine(LCD_LINE_6);
        LCD_DisplayStringLine(LCD_LINE_7, " MCU in STANDBY Mode");
        LCD_DisplayStringLine(LCD_LINE_8, "To exit press Wakeup");
        /* Request to enter STANDBY mode */
        PWR_EnterSTANDBYMode();
      }
    }

    /* Generate a system reset */
    NVIC_SystemReset();
  }
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}


/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}


/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  /* Decrement the TimingDelay variable */
  Demo_DecrementTimingDelay();
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/
/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
  /* If counter is equal to 86339: one day was elapsed */
  if ((RTC_GetCounter() / 3600 == 23) && (((RTC_GetCounter() % 3600) / 60) == 59) &&
      (((RTC_GetCounter() % 3600) % 60) == 59)) /* 23*3600 + 59*60 + 59 = 86339 */
  {
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Reset counter value */
    RTC_SetCounter(0x0);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Increment the date */
    Calendar_DateUpdate();
  }
  /* Clear the RTC Second Interrupt pending bit */
  RTC_ClearITPendingBit(RTC_IT_SEC);
}

/**
  * @brief  This function handles External lines 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    /* Clear the EXTI Line 0 */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

/**
  * @brief  This function handles DMA1 Channel3 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel3_IRQHandler(void)
{
  if (dmaindex == 0)
  {
    DMA1_Channel3->CCR = 0x0;
    DMA1_Channel3->CNDTR = 0x200;
    DMA1_Channel3->CPAR = 0x40007410;
    DMA1_Channel3->CMAR = (uint32_t) & Wavebuffer2;
    DMA1_Channel3->CCR = 0x2093;
    DFS_ReadFile(&fiwave, sector, Wavebuffer, &var, SECTOR_SIZE);
    dmaindex++;
  }
  else
  {
    DMA1_Channel3->CCR = 0x0;
    DMA1_Channel3->CNDTR = 0x200;
    DMA1_Channel3->CPAR = 0x40007410;
    DMA1_Channel3->CMAR = (uint32_t) & Wavebuffer;
    DMA1_Channel3->CCR = 0x2093;
    DFS_ReadFile(&fiwave, sector, Wavebuffer2, &var, SECTOR_SIZE);
    dmaindex = 0;
  }
  DMA1->IFCR = DMA1_IT_TC3;
}

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
    if(!CEC_Menu)
	{
	  Menu_UpFunc();
	}    
    /* Clear the EXTI Line 8 */
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
  if (EXTI_GetITStatus(EXTI_Line9) != RESET)
  {
    Pressed_Key_Button = 1;
    /* Clear the EXTI Line 9 */
    EXTI_ClearITPendingBit(EXTI_Line9);
  }
  if (EXTI_GetITStatus(EXTI_Line7) != RESET)
  {
    /* Restore the GPIO Configurations*/
    LowPower_RestoreGPIOConfig();

    /* Configures system clock after wake-up from STOP: enable HSE, PLL and select PLL
     as system clock source (HSE and PLL are disabled in STOP mode) */
    LowPower_SYSCLKConfig_STOP();

    /* Disable TIM6 */
    TIM_Cmd(TIM6, DISABLE);

    LCD_Clear(LCD_COLOR_WHITE);
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_BLUE);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_RED);
    LCD_DisplayStringLine(LCD_LINE_2, "Err: SDCard Removed ");
    LCD_DisplayStringLine(LCD_LINE_3, "Please check SD Card");
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_5, "Press JoyStick Up to");
    LCD_DisplayStringLine(LCD_LINE_6, "Restart the Demo... ");
 	
    /* Wait until no key is pressed */
    while (Menu_ReadKey() != UP)
    {
    }
    /* Generate System Reset to load the new option byte values */
    NVIC_SystemReset();
  }
}

/**
  * @brief  This function handles TIM1 overflow and update and TIM16 global 
  *         interrupts requests.
  * @param  None
  * @retval None
  */
void TIM1_UP_TIM16_IRQHandler(void)
{
  /* Clear the TIM1 Update pending bit */
  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
  
  if (AlarmStatus == 1)
  {
    if ((LedCounter & 0x01) == 0)
    {
      GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
    }
    else if ((LedCounter & 0x01) == 0x01)
    {
      GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
    }
    LedCounter++;
    if (LedCounter == 255)
    {
      AlarmStatus = 0;
      LedCounter = 0;
    }
  }
  else
  {
    /* If LedShowStatus is TRUE: enable leds toggling */
    if (Demo_Get_LedShowStatus() != 0)
    {
      switch (Index)
      {
          /* LD1 turned on, LD4 turned off */
        case 0:
        {
          GPIO_ResetBits(GPIOC, GPIO_Pin_9);
          GPIO_SetBits(GPIOC, GPIO_Pin_6);
          Index++;
          break;
        }
        /* LD2 turned on, LD1 turned off */
        case 1:
        {
          GPIO_ResetBits(GPIOC, GPIO_Pin_6);
          GPIO_SetBits(GPIOC, GPIO_Pin_7);
          Index++;
          break;
        }
        /* LD3 turned on, LD2 turned off */
        case 2:
        {
          GPIO_ResetBits(GPIOC, GPIO_Pin_7);
          GPIO_SetBits(GPIOC, GPIO_Pin_8);
          Index++;
          break;
        }
        /* LD4 turned on, LD3 turned off */
        case 3:
        {
          GPIO_ResetBits(GPIOC, GPIO_Pin_8);
          GPIO_SetBits(GPIOC, GPIO_Pin_9);
          Index++;
          break;
        }
        default:
          break;
      }
      /* Reset Index to replay leds switch on sequence  */
      if (Index == 4)
      {
        Index = 0;
      }
    }
  }
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line12) != RESET)
  {
    Menu_SelFunc();
    /* Clear the EXTI Line 12 */
    EXTI_ClearITPendingBit(EXTI_Line12);
  }
  if (EXTI_GetITStatus(EXTI_Line14) != RESET)
  {
    Menu_DownFunc();
    /* Clear the EXTI Line 14 */
    EXTI_ClearITPendingBit(EXTI_Line14);
  }
}

/**
  * @brief  This function handles I2C2 Error interrupt request.
  * @param  None
  * @retval None
  */
void I2C1_ER_IRQHandler(void)
{
  /* Check on I2C1 SMBALERT flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_SMBALERT))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_SMBALERT);
    SMbusAlertOccurred++;
  }
  /* Check on I2C1 Time out flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_TIMEOUT))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_TIMEOUT);
  }
  /* Check on I2C1 Arbitration Lost flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_ARLO))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_ARLO);
  } 

  /* Check on I2C1 PEC error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_PECERR))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_PECERR);
  } 
  /* Check on I2C1 Overrun/Underrun error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_OVR))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_OVR);
  } 
  /* Check on I2C1 Acknowledge failure error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_AF))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
  }
  /* Check on I2C1 Bus error flag and clear it */
  if (I2C_GetITStatus(I2C1, I2C_IT_BERR))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_BERR);
  }   
}

/**
  * @brief  This function handles RTC Alarm interrupt request.
  * @param  None
  * @retval None
  */
void RTCAlarm_IRQHandler(void)
{
  /* Clear the Alarm Pending Bit */
  RTC_ClearITPendingBit(RTC_IT_ALR);

  AlarmStatus = 1;
  LowPower_Set_STOPModeStatus();
  /* Clear the EXTI Line 17/ */
  EXTI_ClearITPendingBit(EXTI_Line17);
}

/**
  * @brief  This function handles CEC global interrupt request.
  * @param  None
  * @retval None
  */
void CEC_IRQHandler(void)
{
  /* Process the CEC Interrupts */
  HDMI_CEC_ProcessIRQSrc();
}

/**
  * @brief  This function handles TIM6 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM6_DAC_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  {
    /* Set DAC Channel1 DHR register */
    DAC_SetChannel1Data(DAC_Align_8b_R, Wavebuffer[wavecounter++]);
    if (wavecounter == 511)
    {
      wavecounter = 0;
      DFS_ReadFile(&fiwave, sector, Wavebuffer, &var, SECTOR_SIZE);
    }

    /* If we reach the WaveDataLength of the wave to play */
    if (Decrement_WaveDataLength() == 0)
    {
      /* Stop wave playing */
      WavePlayer_Stop();
    }
    /* Clear TIM6 update interrupt */
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  }
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
