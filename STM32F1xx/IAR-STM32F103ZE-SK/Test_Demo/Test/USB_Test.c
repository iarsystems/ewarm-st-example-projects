/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : USB_Test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : USB_Test program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : USB_Test
* Description    : USB_Test program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_Test(void)
{
  /* Disable the JoyStick interrupts */
  IntExtOnOffConfig(DISABLE);
  
  /* Display Test name */   
  printf ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  printf( "USB Test\n ");
  
  GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
  GLCD_TextSetPos(1,3);
  printf ("Insert the USB cable");
  GLCD_TextSetPos(1,4);
  printf ("on USB-B connector");
  GLCD_TextSetPos(1,5);
  printf ("This Test adds an");
  GLCD_TextSetPos(1,6);
  printf ("USB audio device with"); 
  GLCD_TextSetPos(1,7); 
  printf("one output.");
  GLCD_TextSetPos(1,8);
  printf("It also validates");
  GLCD_TextSetPos(1,9);
  printf("the HEADPHONE");
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  // Init Audio
  UsbAudioClassInit();

 // EXT_CRT_SECTION();

  // Soft connection enable
  USB_ConnectRes(TRUE);

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

