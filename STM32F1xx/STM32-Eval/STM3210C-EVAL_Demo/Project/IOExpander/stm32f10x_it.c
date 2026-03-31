/**
  ******************************************************************************
  * @file    IOExpander/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    06/19/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

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
  {
  }
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
  {
  }
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
  {
  }
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
  {
  }
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
  * @brief  This function handles PendSVC exception.
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
  TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(WAKEUP_BUTTON_EXTI_LINE) != RESET)
  {
    /* Toggle LD3 */
    STM_EVAL_LEDToggle(LED3);

    LCD_DisplayStringLine(Line4, "IT:  WAKEUP Pressed ");

    EXTI_ClearITPendingBit(WAKEUP_BUTTON_EXTI_LINE);
  }
}

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE) != RESET)
  {
    /* Toggle LD1 */
    STM_EVAL_LEDToggle(LED1);

    LCD_DisplayStringLine(Line4, "IT:   KEY Pressed   ");
	
    EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
  }
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(TAMPER_BUTTON_EXTI_LINE) != RESET)
  {
    /* Toggle LD2 */
    STM_EVAL_LEDToggle(LED2);

    LCD_DisplayStringLine(Line4, "IT: TAMPER Pressed  ");
   
    EXTI_ClearITPendingBit(TAMPER_BUTTON_EXTI_LINE);
  }

  if(EXTI_GetITStatus(EXTI_LINE_IOE_ITLINE) != RESET)
  {
    
#ifdef IOE_INTERRUPT_MODE   
    static JOY_State_TypeDef JoyState = JOY_NONE;
    static TS_STATE* TS_State;
    
    /* Check if the interrupt source is the Touch Screen */
    if (IOE_GetGITStatus(IOE_1_ADDR, IOE_TS_IT) & IOE_TS_IT)
    {
      /* Update the structure with the current position */
      TS_State = IOE_TS_GetState();  
      
      if ((TS_State->TouchDetected) && (TS_State->Y < 220) && (TS_State->Y > 180))
      {
        if ((TS_State->X > 10) && (TS_State->X < 70))
        {
          LCD_DisplayStringLine(Line6, " LD4                ");
          STM_EVAL_LEDOn(LED4);
        }
        else if ((TS_State->X > 90) && (TS_State->X < 150))
        {
          LCD_DisplayStringLine(Line6, "      LD3           ");
          STM_EVAL_LEDOn(LED3);
        }
        else if ((TS_State->X > 170) && (TS_State->X < 230))
        {
          LCD_DisplayStringLine(Line6, "           LD2      ");
          STM_EVAL_LEDOn(LED2);
        }     
        else if ((TS_State->X > 250) && (TS_State->X < 310))
        {
          LCD_DisplayStringLine(Line6, "                LD1 ");
          STM_EVAL_LEDOn(LED1);
        }
      }
      else
      {
        STM_EVAL_LEDOff(LED1);
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED4);
      }    
      
      /* Clear the interrupt pending bits */    
      IOE_ClearGITPending(IOE_1_ADDR, IOE_TS_IT);      
    }
    else if (IOE_GetGITStatus(IOE_2_ADDR, IOE_GIT_GPIO))
    {
      /* Get the Joytick State */
      JoyState = IOE_JoyStickGetState();
      
      switch (JoyState)
      {
      case JOY_NONE:
        LCD_DisplayStringLine(Line5, "JOY: IT  ----        ");
        break;
      case JOY_UP:
        LCD_DisplayStringLine(Line5, "JOY: IT  UP         ");
        break;     
      case JOY_DOWN:
        LCD_DisplayStringLine(Line5, "JOY: IT DOWN        ");
        break;          
      case JOY_LEFT:
        LCD_DisplayStringLine(Line5, "JOY: IT LEFT        ");
        break;         
      case JOY_RIGHT:
        LCD_DisplayStringLine(Line5, "JOY: IT  RIGHT        ");
        break;                 
      case JOY_CENTER:
        LCD_DisplayStringLine(Line5, "JOY: IT CENTER       ");
        break; 
      default:
        LCD_DisplayStringLine(Line5, "JOY: IT ERROR      ");
        break;         
      }   
      
      /* Clear the interrupt pending bits */    
      IOE_ClearGITPending(IOE_2_ADDR, IOE_GIT_GPIO);
      IOE_ClearIOITPending(IOE_2_ADDR, IOE_JOY_IT);     
    }
    else
    {
      IOE_ClearGITPending(IOE_1_ADDR, ALL_IT);
      IOE_ClearGITPending(IOE_2_ADDR, ALL_IT);
    }
#endif /* IOE_INTERRUPT_MODE */
    
    EXTI_ClearITPendingBit(EXTI_LINE_IOE_ITLINE);
  }  
}

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
