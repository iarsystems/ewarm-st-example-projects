/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : CAN_Test.c
* Author             : (c) Copyright IAR Systems 2008
* Version            : V1.0.0
* Date               : 12/17/2008
* Description        : CAN_Test program body
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
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
  #define GPIO_CAN                   GPIOB
  #define RCC_APB2Periph_GPIO_CAN    RCC_APB2Periph_GPIOB
  #define GPIO_Pin_RX                GPIO_Pin_8
  #define GPIO_Pin_TX                GPIO_Pin_9
  #define GPIO_Remap_CAN             GPIO_Remap1_CAN1

#define Key_Pressed 0x01
#define Key_NoPressed 0x00


/* Private functions ---------------------------------------------------------*/
void LED_Display_All(u8 Ledstatus);
void NVIC_Configuration(void);
u32 CAN_Config(void);
void Init_RxMes(CanRxMsg *RxMessage);
u8 Key_Status(void);
TestStatus CAN1_Polling(void);
TestStatus CAN2_Polling(void);
u32 TestCAN(u32 Repeat);
/* Private variables ---------------------------------------------------------*/
u8 Key_Pressed_Number = 0x0;
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg TxMessage;
u32 i;
/*******************************************************************************
* Function Name  : CAN_Test
* Description    : CAN_Test program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_Test(void)
{
  vu32 ret = 0; /* for return of the interrupt handling */
  volatile TestStatus TestRx;

  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure LED pins PC6..9 as outputs push-pull, max speed 50 MHz */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |  GPIO_Pin_7  |
                                GPIO_Pin_8 | GPIO_Pin_9;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* turn off all leds*/
  GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);
  GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);
  GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
  GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);

  /* Display Test name */
  GLCD_print ("\f");
  GLCD_SetFont(&Terminal_9_12_6,0xA00,0xFFF);
  GLCD_TextSetPos(6,1);
  GLCD_print( "CAN Test\n ");

  /* CAN configuration */
  ret=CAN_Config();

  if (ret == FAILED)
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(0,5);
    GLCD_print ("Initialisation Error!   ");
    return;
  }

  GLCD_TextSetPos(1,3);
  GLCD_print ("Please connect the\n");
  GLCD_TextSetPos(1,4);
  GLCD_print ("two CAN ...\n");

  GLCD_SetFont(&Terminal_9_12_6,0x80,0xFFF);
  GLCD_TextSetPos(0,9);
  GLCD_print( "Press User key to exit\n ");

  GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
  GLCD_TextSetPos(2,6);
  GLCD_print ("CAN1 <- X -> CAN2");
  TestRx=FAILED;

  /* Run Test and wait for connexion */
  while((ReadKey() != User) && (TestRx==FAILED))
    {
      CAN_Config();
      TestRx=(TestStatus)TestCAN(1000);
    }

  if (TestRx == FAILED)
  {
    GLCD_SetFont(&Terminal_9_12_6,0x100,0xFFF);
    GLCD_TextSetPos(3,7);
    GLCD_print ("Connexion Error!       ");
    ReturnFunc();
    /* Enable EXTI for the menu navigation keys  */
    IntExtOnOffConfig(ENABLE);
    return;
  }
  else
  {
    GLCD_SetFont(&Terminal_9_12_6,0x110,0xFFF);
    GLCD_TextSetPos(2,6);
    GLCD_print (" CAN1 <----> CAN2");
    GLCD_TextSetPos(1,7);
    GLCD_print ("Connexion Established ");
    GLCD_TextSetPos(2,8);
    GLCD_print ("Transfert Complete! ");
  }

  while (ReadKey() != User);

  /* turn off all leds*/
  GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);
  GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);
  GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
  GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);

  ReturnFunc();

  /* Enable EXTI for the menu navigation keys  */
  IntExtOnOffConfig(ENABLE);

}
/**
  * @brief  Configures the NVIC for CAN and joystick
  * @param  None
  * @retval : None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  check if joystick is pressed or not
  * @param  None
  * @retval :
  *   1: if Up is pressed
  *   2: if Down is pressed
  *   3: if Left is pressed
  *   4: if Right is pressed
  */
u8 Key_Status(void)
{
  if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))== 0)
    return (Key_Pressed);
  else
    return (Key_NoPressed);
}

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval : None
  */
u32 CAN_Config(void)
{
CAN_InitTypeDef CAN_InitStruct;
GPIO_InitTypeDef GPIO_InitStructure;
CAN_FilterInitTypeDef CAN_FilterInitStructure;

  // CAN init
  // Enable GPIOD clock
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, DISABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,ENABLE);

  // Configure PD1 as CAN TX
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // Configure PD0 as CAN RX
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // Configure PB6 as CAN TX
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Configure PB5 as CAN RX
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  AFIO->MAPR |= (1ul<<22) | (3ul<<13);

  CAN_DeInit(CAN1);
 CAN_DeInit(CAN2);
  /*
  CAN_ITConfig(  CAN_IT_TME  | CAN_IT_FMP0
               | CAN_IT_FF0  | CAN_IT_FOV0
               | CAN_IT_FMP1 | CAN_IT_FF1
               | CAN_IT_FOV1 | CAN_IT_EWG
               | CAN_IT_EPV  | CAN_IT_LEC  | CAN_IT_BOF
               | CAN_IT_ERR  | CAN_IT_WKU  | CAN_IT_SLK,
                 DISABLE);
  */

  CAN_InitStruct.CAN_TTCM = DISABLE;
  CAN_InitStruct.CAN_ABOM = DISABLE;
  CAN_InitStruct.CAN_AWUM = DISABLE;
  CAN_InitStruct.CAN_NART = ENABLE;
  CAN_InitStruct.CAN_RFLM = ENABLE;
  CAN_InitStruct.CAN_TXFP = ENABLE;

  CAN_InitStruct.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStruct.CAN_SJW  = CAN_SJW_3tq;
  CAN_InitStruct.CAN_BS1  = CAN_BS1_6tq;
  CAN_InitStruct.CAN_BS2  = CAN_BS2_5tq;
  CAN_InitStruct.CAN_Prescaler = 3;

  if(CAN_Init(CAN2,&CAN_InitStruct) == CANINITFAILED)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,DISABLE);
    return(0);
  }

  if(CAN_Init(CAN1,&CAN_InitStruct) == CANINITFAILED)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,DISABLE);
    return(0);
  }

  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = DISABLE;

  for(u32 i = 0; i < 14; ++i)
  {
    CAN_FilterInitStructure.CAN_FilterNumber = i;
    CAN_FilterInit(&CAN_FilterInitStructure);
  }
  return(1);
}

/**
  * @brief  Initializes a Rx Message.
  * @param CanRxMsg *RxMessage
  * @retval : None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  u8 i = 0;

  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (i = 0;i < 8;i++)
    RxMessage->Data[i] = 0x00;
}

TestStatus CAN1_Polling(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CanTxMsg TxMessage;
  CanRxMsg RxMessage;
  u32 i = 0;
  u8 TransmitMailbox = 0;

  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
 CAN_InitStructure.CAN_TTCM=DISABLE;
  CAN_InitStructure.CAN_ABOM=DISABLE;
  CAN_InitStructure.CAN_AWUM=DISABLE;
  CAN_InitStructure.CAN_NART=DISABLE;
  CAN_InitStructure.CAN_RFLM=DISABLE;
  CAN_InitStructure.CAN_TXFP=DISABLE;
  CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
  CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
  CAN_InitStructure.CAN_Prescaler=5;
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN filter init */
  /*CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);*/


  /*---------------------*/

  /*CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x4440;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x4440;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInit(&CAN_FilterInitStructure);
  CAN_FilterInitStructure.CAN_FilterNumber = 2;
  CAN_FilterInit(&CAN_FilterInitStructure);

  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x2220;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x2220;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
  CAN_FilterInitStructure.CAN_FilterNumber = 1;
  CAN_FilterInit(&CAN_FilterInitStructure);
  CAN_FilterInitStructure.CAN_FilterNumber = 3;
  CAN_FilterInit(&CAN_FilterInitStructure);*/

  /*---------------------*/

  /* transmit */
  TxMessage.StdId=0x11;
  TxMessage.RTR=CAN_RTR_DATA;
  TxMessage.IDE=CAN_ID_STD;
  TxMessage.DLC=2;
  TxMessage.Data[0]=0xCA;
  TxMessage.Data[1]=0xFE;

  TransmitMailbox=CAN_Transmit(CAN1, &TxMessage);
  i = 0;
  while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK) && (i != 0xFF))
  {
    i++;
  }

  i = 0;
  while((CAN_MessagePending(CAN1, CAN_FIFO0) < 1) && (i != 0xFF))
  {
    i++;
  }

  /* receive */
  RxMessage.StdId=0x00;
  RxMessage.IDE=CAN_ID_STD;
  RxMessage.DLC=0;
  RxMessage.Data[0]=0x00;
  RxMessage.Data[1]=0x00;
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

  if (RxMessage.StdId!=0x11)
  {
    return FAILED;
  }

  if (RxMessage.IDE!=CAN_ID_STD)
  {
    return FAILED;
  }

  if (RxMessage.DLC!=2)
  {
    return FAILED;
  }

  if ((RxMessage.Data[0]<<8|RxMessage.Data[1])!=0xCAFE)
  {
    return FAILED;
  }

  return PASSED; /* Test Passed */
}


TestStatus CAN2_Polling(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CanTxMsg TxMessage;
  CanRxMsg RxMessage;
  u32 i = 0;
  u8 TransmitMailbox = 0;

  /* CAN register init */
 /* CAN_DeInit(CAN2);
  CAN_StructInit(&CAN_InitStructure);*/

  /* CAN cell init */
 /* CAN_InitStructure.CAN_TTCM=DISABLE;
  CAN_InitStructure.CAN_ABOM=DISABLE;
  CAN_InitStructure.CAN_AWUM=DISABLE;
  CAN_InitStructure.CAN_NART=DISABLE;
  CAN_InitStructure.CAN_RFLM=DISABLE;
  CAN_InitStructure.CAN_TXFP=DISABLE;
  CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
  CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
  CAN_InitStructure.CAN_Prescaler=5;
  CAN_Init(CAN2, &CAN_InitStructure);*/

  /* CAN filter init */
  /*CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);*/

  /* transmit */
  TxMessage.StdId=0x11;
  TxMessage.RTR=CAN_RTR_DATA;
  TxMessage.IDE=CAN_ID_STD;
  TxMessage.DLC=2;
  TxMessage.Data[0]=0xCA;
  TxMessage.Data[1]=0xFE;

  TransmitMailbox=CAN_Transmit(CAN2, &TxMessage);
  i = 0;
  while((CAN_TransmitStatus(CAN2, TransmitMailbox) != CANTXOK) && (i != 0xFF))
  {
    i++;
  }

  i = 0;
  while((CAN_MessagePending(CAN2, CAN_FIFO1) < 1) && (i != 0xFF))
  {
    i++;
  }

  /* receive */
  RxMessage.StdId=0x00;
  RxMessage.IDE=CAN_ID_STD;
  RxMessage.DLC=0;
  RxMessage.Data[0]=0x00;
  RxMessage.Data[1]=0x00;
  CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);

  if (RxMessage.StdId!=0x11)
  {
    return FAILED;
  }

  if (RxMessage.IDE!=CAN_ID_STD)
  {
    return FAILED;
  }

  if (RxMessage.DLC!=2)
  {
    return FAILED;
  }

  if ((RxMessage.Data[0]<<8|RxMessage.Data[1])!=0xCAFE)
  {
    return FAILED;
  }

  return PASSED; /* Test Passed */
}

/*************************************************************************
 * Function Name: TestCAN
 * Parameters: none
 * Return: Int32U
 *
 * Description: CAN test subroutine
 *
 *************************************************************************/
CanTxMsg CanTxMsg1;
CanRxMsg CanRxMsg1;
u32 R;

u32 TestCAN(u32 Repeat)
{
CAN_FilterInitTypeDef CAN_FilterInitStructure;
CanTxMsg CanTxMsg;
CanRxMsg CanRxMsg;
u32 TO;

  CAN1->FMR |= 1;
  CAN1->FMR  = 2<<8;

  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x4440;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x4440;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInit(&CAN_FilterInitStructure);
  CAN_FilterInitStructure.CAN_FilterNumber = 2;
  CAN_FilterInit(&CAN_FilterInitStructure);

  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x2220;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x2220;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
  CAN_FilterInitStructure.CAN_FilterNumber = 1;
  CAN_FilterInit(&CAN_FilterInitStructure);
  CAN_FilterInitStructure.CAN_FilterNumber = 3;
  CAN_FilterInit(&CAN_FilterInitStructure);

  Repeat<<=1;

  for(;Repeat;Repeat--)
  {

    CanTxMsg.ExtId = 0x000;
    CanTxMsg.IDE = DISABLE;
    CanTxMsg.RTR = DISABLE;
    CanTxMsg.DLC   = 8;
    CanTxMsg.StdId = 0x111;
    CanTxMsg.Data[0] = 0x11 ^ (Repeat         & 0xFF);
    CanTxMsg.Data[1] = 0x22 ^ ((Repeat >> 8 ) & 0xFF);
    CanTxMsg.Data[2] = 0x33 ^ ((Repeat >> 16) & 0xFF);
    CanTxMsg.Data[3] = 0x44 ^ ((Repeat >> 24) & 0xFF);
    CanTxMsg.Data[4] = 0x55 ^ (Repeat         & 0xFF);
    CanTxMsg.Data[5] = 0x66 ^ ((Repeat >> 8 ) & 0xFF);
    CanTxMsg.Data[6] = 0x77 ^ ((Repeat >> 16) & 0xFF);
    CanTxMsg.Data[7] = 0x88 ^ ((Repeat >> 24) & 0xFF);
    CAN_Transmit(CAN1,&CanTxMsg);


    /* CAN1 ----> CAN2 */
    TO = 1000;
    while(!CAN_MessagePending(CAN2,1) && (--TO));

    if(!TO)
    {
      R = Repeat;
      CanTxMsg1 = CanTxMsg; CanRxMsg1 = CanRxMsg;
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,DISABLE);
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,DISABLE);
      return(0);
    }

    CAN_Receive(CAN2,1,&CanRxMsg);
    for(int i = 0; i < CanRxMsg.DLC; i++)
    {
      if(CanTxMsg.Data[i] != CanRxMsg.Data[i])
      {
        CanTxMsg1 = CanTxMsg; CanRxMsg1 = CanRxMsg;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,DISABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,DISABLE);
        return(0);
      }
      LED_Display_All(CanRxMsg.Data[i] & 7);
    }

    --Repeat;
    CanTxMsg.ExtId = 0x000;
    CanTxMsg.IDE = DISABLE;
    CanTxMsg.RTR = DISABLE;
    CanTxMsg.DLC   = 8;
    CanTxMsg.StdId = 0x111;
    CanTxMsg.Data[0] = 0x11 ^ (Repeat         & 0xFF);
    CanTxMsg.Data[1] = 0x22 ^ ((Repeat >> 8 ) & 0xFF);
    CanTxMsg.Data[2] = 0x33 ^ ((Repeat >> 16) & 0xFF);
    CanTxMsg.Data[3] = 0x44 ^ ((Repeat >> 24) & 0xFF);
    CanTxMsg.Data[4] = 0x55 ^ (Repeat         & 0xFF);
    CanTxMsg.Data[5] = 0x66 ^ ((Repeat >> 8 ) & 0xFF);
    CanTxMsg.Data[6] = 0x77 ^ ((Repeat >> 16) & 0xFF);
    CanTxMsg.Data[7] = 0x88 ^ ((Repeat >> 24) & 0xFF);
    CAN_Transmit(CAN2,&CanTxMsg);

    TO = 1000;
    while(!CAN_MessagePending(CAN1,1) && (--TO));

    if(!TO)
    {
      R = Repeat;
      CanTxMsg1 = CanTxMsg; CanRxMsg1 = CanRxMsg;
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,DISABLE);
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,DISABLE);
      return(0);
    }

    CAN_Receive(CAN1,1,&CanRxMsg);
    for(int i = 0; i < CanRxMsg.DLC; i++)
    {
      if(CanTxMsg.Data[i] != CanRxMsg.Data[i])
      {
        CanTxMsg1 = CanTxMsg; CanRxMsg1 = CanRxMsg;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,DISABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,DISABLE);
        return(0);
      }
      LED_Display_All(CanRxMsg.Data[i] & 7);
    }
  }
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,DISABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,DISABLE);
  return(1);
}
/**
  * @brief  Turn ON/OFF the dedicate led
  * @param Ledstatus: Led number from 0 to 3
  * @retval : None
  */
void LED_Display_All(u8 Ledstatus)
{
  /* turn off all leds*/
  /*GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);
  GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);
  GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
  GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);*/

  switch(Ledstatus)
  {
   case(1): GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);
            break;

   case(2): GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_SET);
            break;

   case(3): GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
            break;

   case(4): GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);
            break;
  default:
        break;
   }
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
