/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : Vx.x.x
* Date               :
* Description        : Main program body
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
#include "menu.h"
/* Private variables ---------------------------------------------------------*/
#define DLY_100US  2000
Int32U CriticalSecCntr;
/* Private functions ---------------------------------------------------------*/
void Joystick_Init (void);
void SetupClock (void);

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void main(void)
{

  ENTR_CRT_SECTION();
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  EXT_CRT_SECTION();

  /* GLCD init */
  GLCD_PowerUpInit((pu8)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);
  DelayResolution100us(5000);

  GLCD_PowerUpInit((pu8)STM32_Logo.pPicStream);
  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_TextSetPos(4,6);
  GLCD_print("STM32F107VC-SK");
  GLCD_Backlight(BACKLIGHT_ON);

  /* Menu init */
  Menu_Init();
  /* Joystick init */
  Joystick_Init();

  DisplayMenu();

/* Infinite loop */
  while (1)
  {
  }	
}


/*******************************************************************************
* Function Name  : Joystick_Init
* Description    : Joystick_Init.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Joystick_Init (void)
{

  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure PB.08 .. PB.15 as Output push-pull */
  GPIO_InitTypeDef GPIO_InitStructure;

  // GPIO enable clock and release Reset
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOA
                         | RCC_APB2Periph_GPIOB
                         | RCC_APB2Periph_GPIOC
                         | RCC_APB2Periph_GPIOD
                         | RCC_APB2Periph_GPIOE, DISABLE);
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA
                         | RCC_APB2Periph_GPIOB
                         | RCC_APB2Periph_GPIOC
                         | RCC_APB2Periph_GPIOD
                         | RCC_APB2Periph_GPIOE, ENABLE);


  /* Configure PA0 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PE1 (USER button) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Configure PC13 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PC2, PC3 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PE7, PE12 et PE13 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  | GPIO_Pin_12 | GPIO_Pin_13 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* SEL Button : Joystick center button*/
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource12);
  EXTI_InitStructure.EXTI_Line = EXTI_Line12;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* UP Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* DOWN Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource13);
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Deinitializes the NVIC */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the EXTI15_10 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  EXTI_ClearITPendingBit(EXTI_Line12);
  EXTI_ClearITPendingBit(EXTI_Line13);
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the EXTI3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  EXTI_ClearITPendingBit(EXTI_Line3);
  NVIC_Init(&NVIC_InitStructure);

  EXTI_ClearITPendingBit(EXTI_Line12);
  EXTI_ClearITPendingBit(EXTI_Line13);
  EXTI_ClearITPendingBit(EXTI_Line3);

  NVIC_Init(&NVIC_InitStructure);

  IntExtOnOffConfig(ENABLE);
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: u32 Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(u32 Delay)
{
  for(; Delay; Delay--)
  {
    for(volatile u32 j = DLY_100US; j; j--)
    {
    }
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
