/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : stm32.c
 *    Description : board support module
 *
 *    History :
 *    1. Date        : 07, March 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include "stm32.h"

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT,
                                 LED4_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN,
                                 LED4_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK,
                                 LED4_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {WAKEUP_BUTTON_GPIO_PORT, USER_BUTTON_GPIO_PORT};

const uint16_t BUTTON_PIN[BUTTONn] = {WAKEUP_BUTTON_PIN, USER_BUTTON_PIN};

const uint32_t BUTTON_CLK[BUTTONn] = {WAKEUP_BUTTON_GPIO_CLK, USER_BUTTON_GPIO_CLK};

const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {WAKEUP_BUTTON_EXTI_LINE,
                                            USER_BUTTON_EXTI_LINE};

const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {WAKEUP_BUTTON_EXTI_PORT_SOURCE,
                                              USER_BUTTON_EXTI_PORT_SOURCE};
								
const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {WAKEUP_BUTTON_EXTI_PIN_SOURCE,
                                             USER_BUTTON_EXTI_PIN_SOURCE};

const uint16_t BUTTON_IRQn[BUTTONn] = {WAKEUP_BUTTON_EXTI_IRQn, USER_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {STM32_COM1, STM32_COM2};

GPIO_TypeDef* COM_TX_PORT[COMn] = {STM32_COM1_TX_GPIO_PORT, STM32_COM2_TX_GPIO_PORT};

GPIO_TypeDef* COM_RX_PORT[COMn] = {STM32_COM1_RX_GPIO_PORT, STM32_COM2_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {STM32_COM1_CLK, STM32_COM2_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {STM32_COM1_TX_GPIO_CLK, STM32_COM2_TX_GPIO_CLK};

const uint32_t COM_RX_PORT_CLK[COMn] = {STM32_COM1_RX_GPIO_CLK, STM32_COM2_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {STM32_COM1_TX_PIN, STM32_COM2_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {STM32_COM1_RX_PIN, STM32_COM2_RX_PIN};

const uint16_t COM_TX_PIN_SOURCE[COMn] = {STM32_COM1_TX_SOURCE, STM32_COM2_TX_SOURCE};

const uint16_t COM_RX_PIN_SOURCE[COMn] = {STM32_COM1_RX_SOURCE, STM32_COM2_RX_SOURCE};

const uint16_t COM_TX_AF[COMn] = {STM32_COM1_TX_AF, STM32_COM2_TX_AF};

const uint16_t COM_RX_AF[COMn] = {STM32_COM1_RX_AF, STM32_COM2_RX_AF};

const uint32_t COM_PWR_PORT_CLK[COMn] = {STM32_COM1_PWR_GPIO_CLK, STM32_COM2_PWR_GPIO_CLK};

GPIO_TypeDef* COM_PWR_PORT[COMn] = {STM32_COM1_PWR_GPIO_PORT, STM32_COM2_PWR_GPIO_PORT};

const uint16_t COM_PWR_PIN[COMn] = {STM32_COM1_PWR_PIN, STM32_COM2_PWR_PIN};

const uint32_t COM_RTS_PORT_CLK[COMn] = {STM32_COM1_RTS_GPIO_CLK, STM32_COM2_RTS_GPIO_CLK};

GPIO_TypeDef* COM_RTS_PORT[COMn] = {STM32_COM1_RTS_GPIO_PORT, STM32_COM2_RTS_GPIO_PORT};

const uint16_t COM_RTS_PIN[COMn] = {STM32_COM1_RTS_PIN, STM32_COM2_RTS_PIN};

const uint16_t COM_RTS_PIN_SOURCE[COMn] = {STM32_COM1_RTS_SOURCE, STM32_COM2_RTS_SOURCE};

const uint16_t COM_RTS_AF[COMn] = {STM32_COM1_RTS_AF, STM32_COM2_RTS_AF};

const uint32_t COM_CTS_PORT_CLK[COMn] = {STM32_COM1_CTS_GPIO_CLK, STM32_COM2_CTS_GPIO_CLK};

GPIO_TypeDef* COM_CTS_PORT[COMn] = {STM32_COM1_CTS_GPIO_PORT, STM32_COM2_CTS_GPIO_PORT};

const uint16_t COM_CTS_PIN[COMn] = {STM32_COM1_CTS_PIN, STM32_COM2_CTS_PIN};

const uint16_t COM_CTS_PIN_SOURCE[COMn] = {STM32_COM1_CTS_SOURCE, STM32_COM2_CTS_SOURCE};

const uint16_t COM_CTS_AF[COMn] = {STM32_COM1_CTS_AF, STM32_COM2_CTS_AF};

void STM32_LEDInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHBPeriphClockCmd(GPIO_CLK[Led], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
  STM32_LEDOff(Led);
}

void STM32_LEDOn(Led_TypeDef Led)
{
  if(  (LED1 == Led)
     ||(LED2 == Led))
  {
    GPIO_PORT[Led]->BSRRL = GPIO_PIN[Led];
  }
  else
  {
    GPIO_PORT[Led]->BSRRH = GPIO_PIN[Led];
  }
}

void STM32_LEDOff(Led_TypeDef Led)
{
  if(  (LED1 == Led)
     ||(LED2 == Led))
  {
    GPIO_PORT[Led]->BSRRH = GPIO_PIN[Led];
  }
  else
  {
    GPIO_PORT[Led]->BSRRL = GPIO_PIN[Led];
  }
}

void STM32_LEDToggle(Led_TypeDef Led)
{
  GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}

void STM32_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the BUTTON Clock */
  RCC_AHBPeriphClockCmd(BUTTON_CLK[Button], ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure Button pin as input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
  GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);


  if (Button_Mode == BUTTON_MODE_EXTI)
  {
    /* Connect Button EXTI Line to Button GPIO Pin */
    SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

    /* Configure Button EXTI line */
    EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
  }
}

uint32_t STM32_PBGetState(Button_TypeDef Button)
{
  return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

void STM32_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct, bool PowerState)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);
	
  /* Enable UART clock */
  RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);

  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

	if(COM_PWR_PORT[COM])
	{
  	/* Configure USART PWR as output push-pull */
	  RCC_AHBPeriphClockCmd(COM_PWR_PORT_CLK[COM], ENABLE);
	  GPIO_InitStructure.GPIO_Pin = COM_PWR_PIN[COM];
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(COM_PWR_PORT[COM], &GPIO_InitStructure);
	  STM32_COM_PowerCtrl(COM,PowerState);
	}
	
  if(   (USART_HardwareFlowControl_RTS & USART_InitStruct->USART_HardwareFlowControl)
     && (COM_RTS_PORT[COM]))
  {
	  RCC_AHBPeriphClockCmd(COM_RTS_PORT_CLK[COM], ENABLE);
    GPIO_PinAFConfig(COM_RTS_PORT[COM], COM_RTS_PIN_SOURCE[COM], COM_RTS_AF[COM]);
	  GPIO_InitStructure.GPIO_Pin = COM_RTS_PIN[COM];
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(COM_RTS_PORT[COM], &GPIO_InitStructure);
  }

  if(   (USART_HardwareFlowControl_CTS & USART_InitStruct->USART_HardwareFlowControl)
     && (COM_CTS_PORT[COM]))
  {
	  RCC_AHBPeriphClockCmd(COM_CTS_PORT_CLK[COM], ENABLE);
    GPIO_PinAFConfig(COM_CTS_PORT[COM], COM_CTS_PIN_SOURCE[COM], COM_CTS_AF[COM]);
	  GPIO_InitStructure.GPIO_Pin = COM_CTS_PIN[COM];
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(COM_CTS_PORT[COM], &GPIO_InitStructure);
  }

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStruct);

  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
}

void STM32_COM_PowerCtrl(COM_TypeDef COM, bool State)
{
	if(COM_PWR_PORT[COM])
	{
    if(State)
    {
      GPIO_ResetBits(COM_PWR_PORT[COM], COM_PWR_PIN[COM]);
    }
    else
    {
      GPIO_SetBits(COM_PWR_PORT[COM], COM_PWR_PIN[COM]);
    }
	}
}

void STM32_AN_TR_Init(bool State)
{
GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  STM32_AN_TR_PowerCtrl(State);
  
}

void STM32_AN_TR_PowerCtrl(bool State)
{
  if(State)
  {
    GPIO_ResetBits(GPIOD, GPIO_Pin_1);
  }
  else
  {
    GPIO_SetBits(GPIOD, GPIO_Pin_1);
  }
}
