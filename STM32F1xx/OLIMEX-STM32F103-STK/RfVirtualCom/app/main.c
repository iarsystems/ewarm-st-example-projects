/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : 20, December 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *   This example project shows how to use the IAR Embedded Workbench
 *  for ARM to develop code for the Olimex STM32-103STK board.
 *   It implements wireless point to point USB CDC (Communication Device Class)
 *  device and installs it like a Virtual COM port.
 *   The connection is realized between a device with Node A configuration and
 *  a device with Node B configuration.
 *
 *    $Revision: 1473 $
 **************************************************************************/
#include "includes.h"

#define LED_ON()          GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_RESET)
#define LED_OFF()         GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_SET)

#define RING_BUFFER_SIZE  150

Int32U CriticalSecCntr;

RingBuffer_t RfRxRingBuffer;
RingBuffer_t RfTxRingBuffer;
Int8U RxBuff[RING_BUFFER_SIZE];
Int8U TxBuff[RING_BUFFER_SIZE];

extern void RfInit(void);
extern void StartRx(void);

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
void main(void)
{
Int32U Size;
GPIO_InitTypeDef GPIO_InitStructure;
pInt8U pBuffer;
Boolean bComm;

  ENTR_CRT_SECTION();

  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  // Init LED Output
  RCC_APB2PeriphResetCmd(  RCC_APB2Periph_GPIOC, DISABLE);
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOC, ENABLE);

  // Configure PC12 STAT LED & LCD LE PIN.
  GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_SET);

  GPIO_WriteBit(GPIOC,GPIO_Pin_10,Bit_SET);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // NVIC init
#ifndef  EMB_FLASH
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  // Enable timer counting
  TIM_Cmd(TIM1,ENABLE);

  // CDC USB
  UsbCdcInit();

  // Soft connection enable
  USB_ConnectRes(TRUE);

  // Init Ring Buffers
  RingBuff_Init(&RfRxRingBuffer,RxBuff,RING_BUFFER_SIZE);
  RingBuff_Init(&RfTxRingBuffer,TxBuff,RING_BUFFER_SIZE);

  RfInit();

  EXT_CRT_SECTION();

  while(1)
  {
    if (IsUsbCdcConfigure())
    {
      bComm = FALSE;
      // Get free size of the buffer
      pBuffer = RingBuff_GetFreeLinBuff(&RfTxRingBuffer,&Size);
      if(Size)
      {
        // Read data from USB
        Size = UsbCdcRead(pBuffer,Size);
        if(Size)
        {
          LED_ON();
          bComm = TRUE;
        }
        RingBuff_Allocate(&RfTxRingBuffer,Size);
      }

      // Get filled size of the buffer
      pBuffer = RingBuff_GetUsedLinBuff(&RfRxRingBuffer,&Size);
      if(Size)
      {
        // Send data from radio
        LED_ON();
        bComm = TRUE;
        UsbCdcWrite(pBuffer,Size);
        RingBuff_Free(&RfRxRingBuffer,Size);
      }

      if (!bComm)
      {
        LED_OFF();
      }
    }
    else
    {
      LED_OFF();
    }
  }
}
