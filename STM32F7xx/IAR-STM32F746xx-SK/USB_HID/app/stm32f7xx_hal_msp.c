/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : stm32f7xx_hal_msp.c
 *    Description : HAL Low level hardware initialization
 *
 *    History :
 *    1. Date        : June 2015
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *
 **************************************************************************/
#include "iar_stm32f746xx_sk.h"

/*************************************************************************
 * Function Name: HAL_UART_MspInit
 * Parameters: UART_HandleTypeDef *huart
 *
 * Return: none
 *
 * Description: Low level UART initialization
 *************************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock */
  COM1_TX_GPIO_CLK_ENABLE();
  COM1_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of COM1 clocks */
  RCC_PeriphClkInit.PeriphClockSelection = COM1_PERIPH_CLK;
  if(huart->Instance == USART1)
    RCC_PeriphClkInit.Usart1ClockSelection = COM1_CLKSOURCE;
  else
  if(huart->Instance == USART2)
    RCC_PeriphClkInit.Usart2ClockSelection = COM1_CLKSOURCE;
  else
  if(huart->Instance == USART3)
    RCC_PeriphClkInit.Usart3ClockSelection = COM1_CLKSOURCE;
  else
  if(huart->Instance == UART4)
    RCC_PeriphClkInit.Uart4ClockSelection = COM1_CLKSOURCE;
  else
  if(huart->Instance == UART5)
    RCC_PeriphClkInit.Uart5ClockSelection = COM1_CLKSOURCE;
  else
  if(huart->Instance == USART6)
    RCC_PeriphClkInit.Usart6ClockSelection = COM1_CLKSOURCE;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USARTx clock */
  COM1_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = COM1_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = COM1_TX_AF;

  HAL_GPIO_Init(COM1_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = COM1_RX_PIN;
  GPIO_InitStruct.Alternate = COM1_RX_AF;

  HAL_GPIO_Init(COM1_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USARTx */
  //HAL_NVIC_SetPriority(COM1_IRQn, 0, 1);
  //HAL_NVIC_EnableIRQ(COM1_IRQn);
}

/*************************************************************************
 * Function Name: HAL_UART_MspDeInit
 * Parameters: UART_HandleTypeDef *huart
 *
 * Return: none
 *
 * Description: Low level UART deinitialization
 *************************************************************************/
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  COM1_FORCE_RESET();
  COM1_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(COM1_TX_GPIO_PORT, COM1_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(COM1_RX_GPIO_PORT, COM1_RX_PIN);

  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(COM1_IRQn);
}

/*************************************************************************
 * Function Name: HAL_RTC_MspInit
 * Parameters: RTC_HandleTypeDef *hrtc
 *
 * Return: none
 *
 * Description: Low level RTC initialization
 *************************************************************************/
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Enables the PWR Clock and Enables access to the backup domain ###################################*/
  /* To change the source clock of the RTC feature (LSE, LSI), You have to:
     - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
     - Enable write access using HAL_PWR_EnableBkUpAccess() function before to
       configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and
       __HAL_RCC_BACKUPRESET_RELEASE().
     - Configure the needed RTC clock source */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /*##-2- Configue LSE/LSI as RTC clock soucre ###############################*/
#ifdef RTC_CLOCK_SOURCE_LSE
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    printf("HAL_RCC_OscConfig() fail!\r\n");
    while(1);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    printf("HAL_RCCEx_PeriphCLKConfig() fail!\r\n");
    while(1);
  }
#elif defined (RTC_CLOCK_SOURCE_LSI)
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    printf("HAL_RCC_OscConfig() fail!\r\n");
    while(1);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    printf("HAL_RCCEx_PeriphCLKConfig() fail!\r\n");
    while(1);
  }
#else
#error Please select the RTC Clock source inside the main.h file
#endif /*RTC_CLOCK_SOURCE_LSI*/

  /*##-2- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

  /*##-4- Configure the NVIC for RTC Alarm ###################################*/
  //HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0x0F, 0);
  //HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/*************************************************************************
 * Function Name: HAL_I2C_MspInit
 * Parameters: I2C_HandleTypeDef *hi2c
 *
 * Return: none
 *
 * Description: Low level I2C initialization
 *************************************************************************/
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO SCL/SDA clock */
  I2C2_SCL_CLK_ENABLE();
  I2C2_SDA_CLK_ENABLE();
  /* Enable I2Cx clock */
  I2C2_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* I2C SCL GPIO pin configuration  */
  GPIO_InitStruct.Pin       = I2C2_SCL_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = I2C2_SCL_AF;
  HAL_GPIO_Init(I2C2_SCL_PORT, &GPIO_InitStruct);

  /* I2C SDA GPIO pin configuration  */
  GPIO_InitStruct.Pin = I2C2_SDA_PIN;
  GPIO_InitStruct.Alternate = I2C2_SDA_AF;
  HAL_GPIO_Init(I2C2_SDA_PORT, &GPIO_InitStruct);
}

/*************************************************************************
 * Function Name: HAL_I2C_MspDeInit
 * Parameters: I2C_HandleTypeDef *hi2c
 *
 * Return: none
 *
 * Description: Low level I2C deinitialization
 *************************************************************************/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;

  /*##-1- Reset peripherals ##################################################*/
  I2C2_FORCE_RESET();
  I2C2_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure I2C Tx as alternate function  */
  HAL_GPIO_DeInit(I2C2_SCL_PORT, I2C2_SCL_PIN);
  /* Configure I2C Rx as alternate function  */
  HAL_GPIO_DeInit(I2C2_SDA_PORT, I2C2_SDA_PIN);
}

/*************************************************************************
 * Function Name: HAL_ETH_MspInit
 * Parameters: ETH_HandleTypeDef *heth
 *
 * Return: none
 *
 * Description: Low level ETH initialization
 *************************************************************************/
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
GPIO_InitTypeDef GPIO_InitStructure;

  __ETHMAC_CLK_ENABLE();
  __ETHMACTX_CLK_ENABLE();
  __ETHMACRX_CLK_ENABLE();

  //  ETHERNET pins configuration
  //  AF Output Push Pull:
  //- ETH_RMII_MDIO: PA2
  //- ETH_RMII_MDC: PC1
  //- ETH_RMII_MDINT: PA3
  //- ETH_RMII_TX_EN: PB11
  //- ETH_RMII_TXD0: PG13
  //- ETH_RMII_TXD1: PG14
  //- ETH_RMII_PPS_OUT: PB5
  //- ETH_RMII_REF_CLK: PA1
  //- ETH_RMII_CRS_DV: PA7
  //- ETH_RMII_RXD0: PC4
  //- ETH_RMII_RXD1: PC5

  // Configure PA1, PA2, PA3, PA7 as alternate function
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_7;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure PB11 as alternate function push-pull
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStructure.Pin =  GPIO_PIN_11;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Configure PG13, PG14 as alternate function push-pull
  __HAL_RCC_GPIOG_CLK_ENABLE();
  GPIO_InitStructure.Pin =  GPIO_PIN_13 | GPIO_PIN_14;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  // Configure PC1, PC4, PC5 as alternate function
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*************************************************************************
 * Function Name: HAL_ETH_MspDeInit
 * Parameters: ETH_HandleTypeDef *heth
 *
 * Return: none
 *
 * Description: Low level ETH deinitialization
 *************************************************************************/
void HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth)
{
  if (heth->Instance == ETH)
  {
    __HAL_RCC_ETHMAC_FORCE_RESET();
    __HAL_RCC_ETHMAC_RELEASE_RESET();

    HAL_GPIO_DeInit(GPIOA, (GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_7));
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);
    HAL_GPIO_DeInit(GPIOG, (GPIO_PIN_13 | GPIO_PIN_14));
    HAL_GPIO_DeInit(GPIOC, (GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5));
  }
}

/*************************************************************************
 * Function Name: HAL_SD_MspInit
 * Parameters: SD_HandleTypeDef *hsd
 *
 * Return: none
 *
 * Description: Low level SD initialization
 *************************************************************************/
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
GPIO_InitTypeDef  GPIO_InitStructure;

  // Enable GPIO clocks
  SD_CP_CLK_ENABLE();
  SD_WP_CLK_ENABLE();
  SD_CMD_CLK_ENABLE();
  SD_D0_CLK_ENABLE();
  SD_D1_CLK_ENABLE();
  SD_D2_CLK_ENABLE();
  SD_D3_CLK_ENABLE();
  SD_CLK_CLK_ENABLE();

  // Enable SDMMC clock
  __HAL_RCC_SDMMC1_CLK_ENABLE();

  // Init CP pin
  GPIO_InitStructure.Pin = SD_CP_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(SD_CP_PORT, &GPIO_InitStructure);

  // Init WP pin
  GPIO_InitStructure.Pin = SD_WP_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(SD_WP_PORT, &GPIO_InitStructure);

  // Init D0 pin
  GPIO_InitStructure.Pin = SD_D0_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Alternate = GPIO_AF12_SDMMC;
  HAL_GPIO_Init(SD_D0_PORT, &GPIO_InitStructure);

  // Init D1 pin
  GPIO_InitStructure.Pin = SD_D1_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Alternate = GPIO_AF12_SDMMC;
  HAL_GPIO_Init(SD_D1_PORT, &GPIO_InitStructure);

  // Init D2 pin
  GPIO_InitStructure.Pin = SD_D2_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Alternate = GPIO_AF12_SDMMC;
  HAL_GPIO_Init(SD_D2_PORT, &GPIO_InitStructure);

  // Init D3 pin
  GPIO_InitStructure.Pin = SD_D3_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Alternate = GPIO_AF12_SDMMC;
  HAL_GPIO_Init(SD_D3_PORT, &GPIO_InitStructure);

  // Init CLK pin
  GPIO_InitStructure.Pin = SD_CLK_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Alternate = GPIO_AF12_SDMMC;
  HAL_GPIO_Init(SD_CLK_PORT, &GPIO_InitStructure);

  // Init CMD pin
  GPIO_InitStructure.Pin = SD_CMD_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Alternate = GPIO_AF12_SDMMC;
  HAL_GPIO_Init(SD_CMD_PORT, &GPIO_InitStructure);
}

/*************************************************************************
 * Function Name: HAL_SD_MspDeInit
 * Parameters: SD_HandleTypeDef *hsd
 *
 * Return: none
 *
 * Description: Low level SD deinitialization
 *************************************************************************/
void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
  __HAL_RCC_SDMMC1_CLK_DISABLE();
}

/**
  * @brief QSPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for requests by peripheral
  *           - NVIC configuration for DMA and QSPI interrupts
  * @param hqspi: QSPI handle pointer
  * @retval None
  */
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
GPIO_InitTypeDef GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable the QuadSPI memory interface clock */
  QSPI_FLASH_CLK_ENABLE();
  /* Reset the QuadSPI memory interface */
  QSPI_FLASH_FORCE_RESET();
  QSPI_FLASH_RELEASE_RESET();
  /* Enable GPIO clocks */
  QSPI_FLASH_GPIO_CS_CLK_ENABLE();
  QSPI_FLASH_GPIO_IO_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* QSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_FLASH_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(QSPI_FLASH_CS_PORT, &GPIO_InitStruct);

  /* QSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_FLASH_CLK_PIN;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_FLASH_CLK_PORT, &GPIO_InitStruct);

  /* QSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_FLASH_IO0_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(QSPI_FLASH_IO_PORT, &GPIO_InitStruct);

  /* QSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_FLASH_IO1_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(QSPI_FLASH_IO_PORT, &GPIO_InitStruct);

  /* QSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_FLASH_IO2_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_FLASH_IO_PORT, &GPIO_InitStruct);

  /* QSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = QSPI_FLASH_IO3_PIN;
  GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_FLASH_IO_PORT, &GPIO_InitStruct);

}

/**
  * @brief QSPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hqspi: QSPI handle pointer
  * @retval None
  */
void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi)
{
  /* De-Configure QSPI pins */
  HAL_GPIO_DeInit(QSPI_FLASH_CS_PORT,  QSPI_FLASH_CS_PIN);
  HAL_GPIO_DeInit(QSPI_FLASH_CLK_PORT, QSPI_FLASH_CLK_PIN);
  HAL_GPIO_DeInit(QSPI_FLASH_IO_PORT,  QSPI_FLASH_IO0_PIN);
  HAL_GPIO_DeInit(QSPI_FLASH_IO_PORT,  QSPI_FLASH_IO1_PIN);
  HAL_GPIO_DeInit(QSPI_FLASH_IO_PORT,  QSPI_FLASH_IO2_PIN);
  HAL_GPIO_DeInit(QSPI_FLASH_IO_PORT,  QSPI_FLASH_IO3_PIN);

  /*##-3- Reset peripherals ##################################################*/
  /* Reset the QuadSPI memory interface */
  QSPI_FLASH_FORCE_RESET();
  QSPI_FLASH_RELEASE_RESET();

  /* Disable the QuadSPI memory interface clock */
  QSPI_FLASH_CLK_DISABLE();
}

/**
  * @brief I2S MSP Init
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
GPIO_InitTypeDef GPIO_InitStructure;

  if (I2S_IN_MODULE == hi2s->Instance)
  {
    /* Enable GPIOs clock */
    I2S_IN_CK_CLK_ENABLE();
    I2S_IN_WS_CLK_ENABLE();
    I2S_IN_SD_CLK_ENABLE();

    /* Enable I2S_IN clock */
    I2S_IN_CLK_ENABLE();

    /* Release I2S_IN reset */
    I2S_IN_RELEASE_RESET();

    /* Configure I2S_IN_SD */
    GPIO_InitStructure.Pin = I2S_IN_SD_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
    GPIO_InitStructure.Alternate = I2S_IN_SD_AF;
    HAL_GPIO_Init(I2S_IN_SD_PORT, &GPIO_InitStructure);

    /* Configure I2S_IN_CK */
    GPIO_InitStructure.Pin = I2S_IN_CK_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
    GPIO_InitStructure.Alternate = I2S_IN_CK_AF;
    HAL_GPIO_Init(I2S_IN_CK_PORT, &GPIO_InitStructure);

    /* Configure I2S_IN_WS */
    GPIO_InitStructure.Pin = I2S_IN_WS_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
    GPIO_InitStructure.Alternate = I2S_IN_WS_AF;
    HAL_GPIO_Init(I2S_IN_WS_PORT, &GPIO_InitStructure);
  }

  if (I2S_OUT_MODULE == hi2s->Instance)
  {
    /* Enable GPIOs clock */
    I2S_OUT_CK_CLK_ENABLE();
    I2S_OUT_WS_CLK_ENABLE();
    I2S_OUT_SD_CLK_ENABLE();

    /* Enable I2S_OUT clock */
    I2S_OUT_CLK_ENABLE();

    /* Release I2S_OUT reset */
    I2S_OUT_RELEASE_RESET();

    /* Configure I2S_OUT_SD */
    GPIO_InitStructure.Pin = I2S_OUT_SD_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
    GPIO_InitStructure.Alternate = I2S_OUT_SD_AF;
    HAL_GPIO_Init(I2S_OUT_SD_PORT, &GPIO_InitStructure);

    /* Configure I2S_OUT_CK */
    GPIO_InitStructure.Pin = I2S_OUT_CK_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
    GPIO_InitStructure.Alternate = I2S_OUT_CK_AF;
    HAL_GPIO_Init(I2S_OUT_CK_PORT, &GPIO_InitStructure);

    /* Configure I2S_OUT_WS */
    GPIO_InitStructure.Pin = I2S_OUT_WS_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
    GPIO_InitStructure.Alternate = I2S_OUT_WS_AF;
    HAL_GPIO_Init(I2S_OUT_WS_PORT, &GPIO_InitStructure);
  }

  /* Enable MCK GPIO clock */
  I2S_MCK_CLK_ENABLE();

  /* Configure I2S_MCK */
  GPIO_InitStructure.Pin = I2S_MCK_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
  GPIO_InitStructure.Alternate = I2S_MCK_AF;
  HAL_GPIO_Init(I2S_MCK_PORT, &GPIO_InitStructure);
}

/**
  * @brief I2S MSP DeInit
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s)
{
  if (I2S_IN_MODULE == hi2s->Instance)
  {
    /* Disable I2S_IN clock */
    I2S_IN_CLK_DISABLE();

    /* Assert I2S_IN reset */
    I2S_IN_FORCE_RESET();
  }

  if (I2S_OUT_MODULE == hi2s->Instance)
  {
    /* Disable I2S_OUT clock */
    I2S_OUT_CLK_DISABLE();

    /* Assert I2S_OUT reset */
    I2S_OUT_FORCE_RESET();
  }
}
