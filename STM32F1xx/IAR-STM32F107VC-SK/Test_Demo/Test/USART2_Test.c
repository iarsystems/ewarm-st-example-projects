/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : USART2_Test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : USART2_Test program body
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : USART2_Test
* Description    : USART2_Test program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_Test(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 USART_InitTypeDef USART_InitStructure;

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);

  /* Display Test name */
  GLCD_print ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  GLCD_print( "RS232-2 Test\n ");

    // Release reset and enable clock
    USART_DeInit(USART3);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // GPIO Init
    // Enable GPIO clock and release reset
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,
                           ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,
                           DISABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);

    // Assign PD8 to UART3 (Tx)
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // Assign PD6=9 to UART3 (Rx)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* USART3 configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled
        - Receive and transmit enabled
  */

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);

  /* Enable the USART3 */
  USART_Cmd(USART3, ENABLE);

  GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
  /* Print messages on GLCD*/
   GLCD_TextSetPos(1,2);
  GLCD_print ("Hyperterminal config:\n");
  GLCD_TextSetPos(1,3);
  GLCD_print ("- BaudRate = 115200 baud\n");
  GLCD_TextSetPos(1,4);
  GLCD_print ("- Word Length = 8 Bits\n");
  GLCD_TextSetPos(1,5);
  GLCD_print ("- One Stop Bit\n");
  GLCD_TextSetPos(1,6);
  GLCD_print ("- No parity\n");
  GLCD_TextSetPos(1,7);
  GLCD_print ("Insert RS232 cable\n");
  GLCD_TextSetPos(0,9);
  GLCD_print ("Print K in hyperterminal\n");

  /* UART Loopback Test (fit loopback on HW) */
  ser_printf(USART3,'K');
  if (ser_scanf(USART3) != 'K')
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(1,8);
    GLCD_print ("RS232-2 Error!         ");
  }
  else

  {
    GLCD_SetFont(&Terminal_9_12_6,0x10,0xFFF);
    GLCD_TextSetPos(1,8);
    GLCD_print ("RS232-2 OK!         ");
  }
    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,10);
    GLCD_print( "Press User key to exit\n ");
  while(ReadKey() != User)
    {
    }

  ReturnFunc();

  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
