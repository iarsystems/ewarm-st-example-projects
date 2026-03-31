/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : iar_stm32f746xx_sk.c
 *    Description : IAR-STM32F746xx-SK board support package
 *
 *    History :
 *    1. Date        : May, 2015
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: #2 $
 **************************************************************************/

#include "iar_stm32f746xx_sk.h"


GPIO_TypeDef* GPIO_PORT[LEDn]  = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT, LED4_GPIO_PORT};

const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};

GPIO_TypeDef* BUTTON_PORT[BUTTONn]  = {USER_BUTTON_GPIO_PORT,RIGHT_BUTTON_GPIO_PORT, LEFT_BUTTON_GPIO_PORT,
                                       UP_BUTTON_GPIO_PORT, DOWN_BUTTON_GPIO_PORT, SEL_BUTTON_GPIO_PORT};

const uint16_t BUTTON_PIN[BUTTONn] = {USER_BUTTON_PIN,RIGHT_BUTTON_PIN, LEFT_BUTTON_PIN,
                                      UP_BUTTON_PIN,DOWN_BUTTON_PIN, SEL_BUTTON_PIN};

const IRQn_Type BUTTON_IRQn[BUTTONn] = {USER_BUTTON_EXTI_IRQn, RIGHT_BUTTON_EXTI_IRQn,
                                        LEFT_BUTTON_EXTI_IRQn, UP_BUTTON_EXTI_IRQn,
                                        DOWN_BUTTON_EXTI_IRQn, SEL_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {COM1_BASE};

GPIO_TypeDef* COM_TX_PORT[COMn] = {COM1_TX_GPIO_PORT};

GPIO_TypeDef* COM_RX_PORT[COMn] = {COM1_RX_GPIO_PORT};

const uint16_t COM_TX_PIN[COMn] = {COM1_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {COM1_RX_PIN};


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
  for(GPIO_TypeDef *gpio = GPIOA; gpio <= GPIOI; gpio++)
  {
    for(uint32_t pin=0; pin <= 16; pin++)
    {
      HAL_GPIO_DeInit(gpio, pin);
    }
  }
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
  switch(Led)
  {
    case LED1: LED1_GPIO_CLK_ENABLE(); break;
    case LED2: LED2_GPIO_CLK_ENABLE(); break;
    case LED3: LED3_GPIO_CLK_ENABLE(); break;
    case LED4: LED4_GPIO_CLK_ENABLE(); break;
    default: return;
  }

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.Pin = GPIO_PIN[Led];
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
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
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
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
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
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
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
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

  /* Enable the BUTTON Clock */
  switch(Button)
  {
    case BUTTON_USER:  USER_BUTTON_GPIO_CLK_ENABLE(); break;
    case BUTTON_RIGHT: RIGHT_BUTTON_GPIO_CLK_ENABLE(); break;
    case BUTTON_LEFT:  LEFT_BUTTON_GPIO_CLK_ENABLE(); break;
    case BUTTON_UP:    UP_BUTTON_GPIO_CLK_ENABLE(); break;
    case BUTTON_DOWN:  DOWN_BUTTON_GPIO_CLK_ENABLE(); break;
    case BUTTON_SEL:   SEL_BUTTON_GPIO_CLK_ENABLE(); break;
  }

  /* Configure Button pin as input */
  GPIO_InitStructure.Mode  = (Button_Mode == BUTTON_MODE_EXTI)?GPIO_MODE_IT_RISING:GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Pin   = BUTTON_PIN[Button];
  GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

  if (Button_Mode == BUTTON_MODE_EXTI)
  {
    /* Enable and set EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(BUTTON_IRQn[Button], 2, 0);
    HAL_NVIC_EnableIRQ(BUTTON_IRQn[Button]);
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
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
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
uint32_t STM_COMInit(UART_HandleTypeDef * pUartHandle, USART_TypeDef * COM_BASE)
{
  /* USARTx configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  pUartHandle->Instance        = COM_BASE;

  HAL_UART_DeInit(pUartHandle);

  pUartHandle->Init.BaudRate   = 115200;
  pUartHandle->Init.WordLength = UART_WORDLENGTH_8B;
  pUartHandle->Init.StopBits   = UART_STOPBITS_1;
  pUartHandle->Init.Parity     = UART_PARITY_NONE;
  pUartHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  pUartHandle->Init.Mode       = UART_MODE_TX_RX;
  pUartHandle->State           = HAL_UART_STATE_RESET;

  if(HAL_UART_Init(pUartHandle) != HAL_OK)
  {
    /* Initialization Error */
    return -1;
  }
  return 0;
}

/*************************************************************************
 * Function Name: STM_I2S2Init
 * Parameters: I2S_HandleTypeDef * hi2s_out, I2S_HandleTypeDef * hi2s_in
 *
 * Return: uint32_t
 *
 * Description: Init I2S module
 *
 *************************************************************************/
uint32_t STM_I2SInit(I2S_HandleTypeDef * hi2s_out, I2S_HandleTypeDef * hi2s_in)
{
  /* I2S OUT peripheral configuration */
  hi2s_out->Instance = SPI2;
  hi2s_out->Init.Mode = I2S_MODE_MASTER_TX;
  hi2s_out->Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s_out->Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s_out->Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s_out->Init.AudioFreq = I2S_AUDIOFREQ_44K;
  hi2s_out->Init.CPOL = I2S_CPOL_LOW;
  hi2s_out->Init.ClockSource = I2S_CLOCK_SYSCLK;
  hi2s_out->State = HAL_I2S_STATE_RESET;
  if (HAL_OK != HAL_I2S_Init(hi2s_out))
  {
    /* Initialization Error */
    return -1;
  }
  __HAL_I2S_ENABLE(hi2s_out);

  /* I2S IN peripheral configuration */
  hi2s_in->Instance = SPI1;
  hi2s_in->Init.Mode = I2S_MODE_SLAVE_RX;
  hi2s_in->Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s_in->Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s_in->Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
  hi2s_in->Init.AudioFreq = I2S_AUDIOFREQ_44K;
  hi2s_in->Init.CPOL = I2S_CPOL_LOW;
  hi2s_in->Init.ClockSource = I2S_CLOCK_SYSCLK;
  hi2s_in->State = HAL_I2S_STATE_RESET;
  if (HAL_OK != HAL_I2S_Init(hi2s_in))
  {
    /* Initialization Error */
    return -1;
  }
  __HAL_I2S_ENABLE(hi2s_in);
  return 0;
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

  /* Enable Balcklight port clock */
  LCD_BACKLIGHT_CLK_ENABLE();

  /* Configure Backlight pin */
  GPIO_InitStruct.Pin = LCD_BACKLIGHT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(LCD_BACKLIGHT_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, GPIO_PIN_SET);
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
  HAL_GPIO_WritePin(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, GPIO_PIN_SET);
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
  HAL_GPIO_WritePin(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, GPIO_PIN_RESET);
}
