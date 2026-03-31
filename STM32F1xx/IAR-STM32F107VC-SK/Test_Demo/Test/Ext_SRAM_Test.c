/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Ext_SRAM_Test.c
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : Ext_SRAM_Test program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "fsmc_sram.h"
#include "stm32f10x_lib.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE    1024
#define WRITE_ADDR     0x8000
//#define Dly            8000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u16 TxBuffer[BUFFER_SIZE];
u16 RxBuffer[BUFFER_SIZE];

u32 WriteReadStatus = 0, Index = 0;

/* Private function prototypes -----------------------------------------------*/
void Fill_Buffer2(u16 *pBuffer, u16 BufferLenght, u32 Offset);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Ext_SRAM_Test
* Description    : Ext_SRAM_Test program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Ext_SRAM_Test(void)
{
GPIO_InitTypeDef GPIO_InitStructure;

  /* Disable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(DISABLE);
  
  /* Display Test name */
  printf ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(3,1);
  printf( "Ext. SRAM Test\n ");
  
  /* GPIO Configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |  GPIO_Pin_8  |
                                GPIO_Pin_9 | GPIO_Pin_10 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* Configure FSMC Bank1 NOR/SRAM3 */
  GLCD_SetFont(&Terminal_9_12_6,0x1,0xFFF);
  FSMC_SRAM_Init();
  GLCD_TextSetPos(1,3);
  printf ("SRAM Initialization..\n");

  /* Write data to FSMC SRAM memory */
  /* Fill the buffer to send */
  Fill_Buffer2(TxBuffer, BUFFER_SIZE, 0x3212);
  FSMC_SRAM_WriteBuffer(TxBuffer, WRITE_ADDR, BUFFER_SIZE);


  /* Read data from FSMC SRAM memory */
  FSMC_SRAM_ReadBuffer(RxBuffer, WRITE_ADDR, BUFFER_SIZE);  

  /* Read back SRAM memory and check content correctness */   
  for (Index = 0x00; (Index < BUFFER_SIZE) && (WriteReadStatus == 0); Index++)
  {
    if (RxBuffer[Index] != TxBuffer[Index])
    {
      WriteReadStatus = Index + 1;
    }
  }
  GLCD_TextSetPos(1,5);
  printf ("Write data at 0x8000       \n");
  DelayResolution100us(Dly);
  if (WriteReadStatus == 0)
  {/* OK */
    GLCD_SetFont(&Terminal_9_12_6,0x40,0xFFF);
    GLCD_TextSetPos(1,6);
    printf ("Write operation OK!       \n");
  }
  else
  { /* KO */
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(1,6);
    printf ("Write operation FAILED!       \n");
  }
    GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
    GLCD_TextSetPos(0,9);
    printf( "Press User key to exit\n "); 
  while(ReadKey() != User)
    {
    }
   
  ReturnFunc();
  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);
  
}

/*******************************************************************************
* Function name : Fill_Buffer
* Description   : Fill the global buffer
* Input         : - pBuffer: pointer on the Buffer to fill
*                 - BufferSize: size of the buffer to fill
*                 - Offset: first value to fill on the Buffer
* Output param  : None
*******************************************************************************/
void Fill_Buffer2(u16 *pBuffer, u16 BufferLenght, u32 Offset)
{
  u16 IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
