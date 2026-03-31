/**
  ******************************************************************************
  * @file    IOExpander/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    06/19/2009
  * @brief   Main program body
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

#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();  

  /* SysTick end of count event each 10ms with input clock equal to 9 MHz (HCLK/8, default) */
  SysTick_Config(SystemCoreClock / 100);

  /* Initialize LEDs and push-buttons mounted on STM3210X-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  STM_EVAL_PBInit(Button_WAKEUP, BUTTON_MODE);
  STM_EVAL_PBInit(Button_TAMPER, BUTTON_MODE);
  STM_EVAL_PBInit(Button_KEY, BUTTON_MODE);

  /* Initialize the LCD */
  STM3210C_LCD_Init();
 
  /* Clear the LCD */ 
  LCD_Clear(White);
  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);    
 
  LCD_DisplayStringLine(Line0, "   STM3210C-EVAL    ");
  LCD_DisplayStringLine(Line1, "  Example on how to ");
  LCD_DisplayStringLine(Line2, " use the IO Expander");
  
  /* Configure the IO Expander */
  if (IOE_Config() == IOE_OK)
  {
    LCD_DisplayStringLine(Line4, "   IO Expander OK   ");
  }
  else
  {
    LCD_DisplayStringLine(Line4, "IO Expander FAILED ");
    LCD_DisplayStringLine(Line5, " Please Reset the  ");
    LCD_DisplayStringLine(Line6, "   board and start ");
    LCD_DisplayStringLine(Line7, "    again          ");
    while(1);
  }

  /* Leds Control blocks */
  LCD_SetTextColor(Blue);
  LCD_DrawRect(180, 310, 40, 60);
  LCD_SetTextColor(Red);
  LCD_DrawRect(180, 230, 40, 60);
  LCD_SetTextColor(Yellow);
  LCD_DrawRect(180, 150, 40, 60);
  LCD_SetTextColor(Green);
  LCD_DrawRect(180, 70, 40, 60);

#ifdef IOE_INTERRUPT_MODE
  /* Enable the Touch Screen and Joystick interrupts */
  IOE_ITConfig(IOE_ITSRC_JOYSTICK | IOE_ITSRC_TSC); 
#endif /* IOE_INTERRUPT_MODE */
  
  
  while(1)
  {
#ifdef IOE_POLLING_MODE
    static JOY_State_TypeDef JoyState = JOY_NONE;
    static TS_STATE* TS_State;
    
    /* Get the Joytick State */
    JoyState = IOE_JoyStickGetState();
   
    switch (JoyState)
    {
      case JOY_NONE:
        LCD_DisplayStringLine(Line5, "JOY:     ----        ");
        break;
      case JOY_UP:
        LCD_DisplayStringLine(Line5, "JOY:     UP         ");
        break;     
      case JOY_DOWN:
        LCD_DisplayStringLine(Line5, "JOY:    DOWN        ");
        break;          
      case JOY_LEFT:
        LCD_DisplayStringLine(Line5, "JOY:    LEFT        ");
        break;         
      case JOY_RIGHT:
        LCD_DisplayStringLine(Line5, "JOY:    RIGHT        ");
        break;                 
      case JOY_CENTER:
        LCD_DisplayStringLine(Line5, "JOY:   CENTER       ");
        break; 
      default:
        LCD_DisplayStringLine(Line5, "JOY:   ERROR      ");
        break;         
    }

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
#endif /* IOE_POLLING_MODE */  
    
#ifdef BUTTON_MODE_GPIO
    /* Insert 10 ms delay */
    Delay(1);
    
    if (STM_EVAL_PBGetState(Button_KEY) == 0)
    {
      /* Toggle LD1 */
      STM_EVAL_LEDToggle(LED1);

      LCD_DisplayStringLine(Line4, "Pol:   KEY Pressed  ");
    }

    if (STM_EVAL_PBGetState(Button_TAMPER) == 0)
    {
      /* Toggle LD2 */
      STM_EVAL_LEDToggle(LED2);

      LCD_DisplayStringLine(Line4, "Pol: TAMPER Pressed ");
    }

    if (STM_EVAL_PBGetState(Button_WAKEUP) != 0)
    {
      /* Toggle LD3 */
      STM_EVAL_LEDToggle(LED3);
      LCD_DisplayStringLine(Line4, "Pol: WAKEUP Pressed ");
    }
#endif
  }
}
																 
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

   LCD_DisplayStringLine(Line0, "assert_param error!!");

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
