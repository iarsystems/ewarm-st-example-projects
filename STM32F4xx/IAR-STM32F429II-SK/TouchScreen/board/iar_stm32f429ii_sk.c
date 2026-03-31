/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : iar_stm32f429ii_sk.c
 *    Description : IAR-STM32F429II-SK board support package
 *
 *    History :
 *    1. Date        : 01 July 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include "iar_stm32f429ii_sk.h"


GPIO_TypeDef* GPIO_PORT[LEDn]  = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT, LED4_GPIO_PORT};

const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};

const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK, LED4_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn]  = {USER_BUTTON_GPIO_PORT,RIGHT_BUTTON_GPIO_PORT, LEFT_BUTTON_GPIO_PORT,
                                       UP_BUTTON_GPIO_PORT,DOWN_BUTTON_GPIO_PORT, SEL_BUTTON_GPIO_PORT};

const uint16_t BUTTON_PIN[BUTTONn] = {USER_BUTTON_PIN,RIGHT_BUTTON_PIN, LEFT_BUTTON_PIN,
                                      UP_BUTTON_PIN,DOWN_BUTTON_PIN, SEL_BUTTON_PIN};

const uint32_t BUTTON_CLK[BUTTONn] = {USER_BUTTON_GPIO_CLK,RIGHT_BUTTON_GPIO_CLK, LEFT_BUTTON_GPIO_CLK,
                                      UP_BUTTON_GPIO_CLK,DOWN_BUTTON_GPIO_CLK, SEL_BUTTON_GPIO_CLK};


const uint16_t BUTTON_EXTI_LINE[BUTTONn]   = {USER_BUTTON_EXTI_LINE,
                                              RIGHT_BUTTON_EXTI_LINE,
                                              LEFT_BUTTON_EXTI_LINE,
                                              UP_BUTTON_EXTI_LINE,
                                              DOWN_BUTTON_EXTI_LINE,
                                              SEL_BUTTON_EXTI_LINE};

const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {USER_BUTTON_EXTI_PORT_SOURCE,
                                              RIGHT_BUTTON_EXTI_PORT_SOURCE,
                                              LEFT_BUTTON_EXTI_PORT_SOURCE,
                                              UP_BUTTON_EXTI_PORT_SOURCE,
                                              DOWN_BUTTON_EXTI_PORT_SOURCE,
                                              SEL_BUTTON_EXTI_PORT_SOURCE};

const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {USER_BUTTON_EXTI_PIN_SOURCE,
                                             RIGHT_BUTTON_EXTI_PIN_SOURCE,
                                             LEFT_BUTTON_EXTI_PIN_SOURCE,
                                             UP_BUTTON_EXTI_PIN_SOURCE,
                                             DOWN_BUTTON_EXTI_PIN_SOURCE,
                                             SEL_BUTTON_EXTI_PIN_SOURCE};

const uint16_t BUTTON_IRQn[BUTTONn] = {USER_BUTTON_EXTI_IRQn,RIGHT_BUTTON_EXTI_IRQn,
                                       LEFT_BUTTON_EXTI_IRQn, UP_BUTTON_EXTI_IRQn,
                                       DOWN_BUTTON_EXTI_IRQn, SEL_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {COM1_BASE};

GPIO_TypeDef* COM_TX_PORT[COMn] = {COM1_TX_GPIO_PORT};

GPIO_TypeDef* COM_RX_PORT[COMn] = {COM1_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {COM1_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {COM1_TX_GPIO_CLK};

const uint32_t COM_RX_PORT_CLK[COMn] = {COM1_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {COM1_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {COM1_RX_PIN};

const uint16_t COM_TX_PIN_SOURCE[COMn] = {COM1_TX_SOURCE};

const uint16_t COM_RX_PIN_SOURCE[COMn] = {COM1_RX_SOURCE};

const uint16_t COM_TX_AF[COMn] = {COM1_TX_AF};

const uint16_t COM_RX_AF[COMn] = {COM1_RX_AF};

/*************************************************************************
 * Function Name: STM_GPIOReset
 * Parameters: none
 *
 * Return: none
 *
 * Description: Reset GPIOs
 *
 *************************************************************************/
void STM_GPIOReset(void)
{
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
  GPIO_DeInit(GPIOE);
  GPIO_DeInit(GPIOF);
  GPIO_DeInit(GPIOG);
  GPIO_DeInit(GPIOH);
  GPIO_DeInit(GPIOI);
}

/*************************************************************************
 * Function Name: STM_LEDInit
 * Parameters: Led_TypeDef Led
 *
 * Return:
 *
 * Description: Init given onboard LED
 *
 *************************************************************************/
void STM_LEDInit(Led_TypeDef Led)
{
GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(GPIO_CLK[Led], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

/*************************************************************************
 * Function Name: STM_LEDOn
 * Parameters: Led_TypeDef Led
 *
 * Return: none
 *
 * Description: Turn on the given onboard LED
 *
 *************************************************************************/
void STM_LEDOn(Led_TypeDef Led)
{
  GPIO_ResetBits(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/*************************************************************************
 * Function Name: STM_LEDOff
 * Parameters: Led_TypeDef Led
 *
 * Return: none
 *
 * Description: Turn off the given onboard LED
 *
 *************************************************************************/
void STM_LEDOff(Led_TypeDef Led)
{
  GPIO_SetBits(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/*************************************************************************
 * Function Name: STM_LEDToggle
 * Parameters: Led_TypeDef Led
 *
 * Return: none
 *
 * Description: Toggle the given onboard LED
 *
 *************************************************************************/
void STM_LEDToggle(Led_TypeDef Led)
{
  GPIO_ToggleBits(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/*************************************************************************
 * Function Name: STM_ButtonInit
 * Parameters: Button_TypeDef Button, ButtonMode_TypeDef Button_Mode
 *
 * Return: none
 *
 * Description: Configures onboard button.
 *
 *************************************************************************/
void STM_ButtonInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the BUTTON Clock */
  RCC_AHB1PeriphClockCmd(BUTTON_CLK[Button], ENABLE);
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
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
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

/*************************************************************************
 * Function Name: STM_ButtonGetState
 * Parameters: Button_TypeDef Button
 *
 * Return: none
 *
 * Description: Reads the button state.
 *
 *************************************************************************/
uint32_t STM_ButtonGetState(Button_TypeDef Button)
{
  return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/*************************************************************************
 * Function Name: STM_COMInit
 * Parameters: COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct
 *
 * Return: none
 *
 * Description: Initialize an onchip USART
 *
 *************************************************************************/
void STM_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

  if (COM == COM1)
  {
    /* Enable UART clock */
    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }

  /* Connect port pin to USARTx_Tx*/
  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

  /* Connect port pin to USARTx_Rx*/
  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStruct);

  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
}

/*************************************************************************
 * Function Name: STM_I2S2Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init I2S module
 *
 *************************************************************************/
void STM_I2S2Init(void)
{
I2S_InitTypeDef I2S_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOs clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                         RCC_AHB1Periph_GPIOI, ENABLE);

  /* Enable I2S2 APB1 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  /* Deinitialize SPI2 peripheral */
  SPI_I2S_DeInit(SPI2);
  SPI_I2S_DeInit(I2S2ext);

  /* Configure PI2 and PI3*/ // I2S2EXT_SD, I2S2_SD
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOI, &GPIO_InitStructure);

  /* Configure PI2 and PI3 to I2S2 */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource2,(uint8_t)0x06); // I2S2ext_SD
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource3, GPIO_AF_SPI2);

  /* Configure PB9 and PB10*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Connect PB9 and PB10 I2S2 module*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9,  GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);

  /* Configure PC6*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Connect PC6 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_SPI2);

  /* I2S peripheral configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_44k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_Init(SPI2, &I2S_InitStructure);

  I2S_InitStructure.I2S_Mode = I2S_Mode_SlaveRx;
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_44k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_Init(I2S2ext, &I2S_InitStructure);

  /* Enable the SPI2/I2S2 peripheral */
  I2S_Cmd(SPI2, ENABLE);
  I2S_Cmd(I2S2ext, ENABLE);
}

/*************************************************************************
 * Function Name: STM_BacklightInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init LCD backlight pin
 *
 *************************************************************************/
void STM_BacklightInit(void)
{
GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = LCD_BACKLIGHT_PIN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(LCD_BACKLIGHT_PORT, &GPIO_InitStruct);
  GPIO_SetBits(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
}

/*************************************************************************
 * Function Name: STM_BacklightOff
 * Parameters: none
 *
 * Return: none
 *
 * Description: Turn off the LCD backlight
 *
 *************************************************************************/
void STM_BacklightOff(void)
{
  GPIO_SetBits(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
}

/*************************************************************************
 * Function Name: STM_BacklightOn
 * Parameters: none
 *
 * Return: none
 *
 * Description: Turn on the LCD backlight
 *
 *************************************************************************/
void STM_BacklightOn(void)
{
  GPIO_ResetBits(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
}
