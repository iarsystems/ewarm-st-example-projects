/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : LED_Test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : LED_Test program body
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
/* LED Definitions */
#define LED_MSK         0x00000FF0  /* PB8..15*/
#define LED_On(led)     GPIOF->ODR |=  led
#define LED_Off(led)    GPIOF->ODR &= ~led
#define LED_Toggle(led) GPIOF->ODR ^=  led
#define LED_Output(led) GPIOF->ODR = (GPIOB->ODR & ~LED_MSK) | led

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : LED_Test
* Description    : LED_Test program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LED_Test(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
  unsigned long n;

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);

  /* Display Test name */
  printf ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  printf( "LED Test\n ");

  /* Configure PF7..10 as outputs push-pull, max speed 50 MHz */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |  GPIO_Pin_8  |
                                GPIO_Pin_9 | GPIO_Pin_10;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  LED_On (LED_MSK);
  GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
  GLCD_TextSetPos(1,3);
  printf( "Turn On LEDs \n ");
  /* Turn On  all LEDs */
  DelayResolution100us(Dly-5000);

  for (n = 0; n < 4; n++)
   {
  /* Turn Off one LED  */
    LED_Off(0x40 << n);
    GLCD_TextSetPos(1,3);
    printf( "Turn Off each LED\n ");
    DelayResolution100us(Dly-5000);
   }
    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,9);
    printf( "Press User key to exit\n ");
  while(ReadKey() != User)
    {
    }

  ReturnFunc();

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);

}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
