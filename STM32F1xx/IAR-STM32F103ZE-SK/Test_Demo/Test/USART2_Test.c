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
  printf ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  printf( "RS232-2 Test\n ");

  /* Configure USART2 RTS (PA1) and USART2 Tx (PA2) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART2 CTS (PA0) and USART2 Rx (PA3) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* USART2 configured as follow:
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
  USART_Init(USART2, &USART_InitStructure);

  /* Enable the USART2 */
  USART_Cmd(USART2, ENABLE);

  GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
  /* Print messages on GLCD*/
   GLCD_TextSetPos(1,2);
  printf ("Hyperterminal config:\n");
  GLCD_TextSetPos(1,3);
  printf ("- BaudRate = 115200 baud\n");
  GLCD_TextSetPos(1,4);
  printf ("- Word Length = 8 Bits\n");
  GLCD_TextSetPos(1,5);
  printf ("- One Stop Bit\n");
  GLCD_TextSetPos(1,6);
  printf ("- No parity\n");
  GLCD_TextSetPos(1,7);
  printf ("Insert RS232 cable\n");
  GLCD_TextSetPos(0,9);
  printf ("Print K in hyperterminal\n");
  /* UART Loopback Test (fit loopback on HW) */
  ser_printf(USART2,'K');
  if (ser_scanf(USART2) != 'K')
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(1,8);
    printf ("RS232-2 Error!         ");
  }
  else

  {
    GLCD_SetFont(&Terminal_9_12_6,0x10,0xFFF);
    GLCD_TextSetPos(1,8);
    printf ("RS232-2 OK!         ");
  }
    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,10);
    printf( "Press User key to exit\n ");
  while(ReadKey() != User)
    {
    }

  ReturnFunc();

  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
