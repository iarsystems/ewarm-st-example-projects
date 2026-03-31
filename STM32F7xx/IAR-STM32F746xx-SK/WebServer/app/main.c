/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    1. Date        : September 2011
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    2. Date        : August 2015
 *       Author      : Atanas Uzunov
 *       Description : Ported to STM32F746xx-SK board
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F746xx-SK board. It is an
 * embedded Web Server based on lwIP TCP/IP stack v1.3.2 and features
 * three HTML pages that form a compact and interactive Web Server to
 * interact with IAR-STM32F746xx-SK board.
 *  You will be able to address the IAR-STM32F746xx-SK board as a web
 * page using your web browser, as well as controlling the four LEDs on
 * the board through the browser and finally you will be able to get
 * continuously, each second, the ADC2 Channel 6 converted value and
 * the board temperature, both displayed on the web page of your browser.
 * The project can be configured to use Static IP or DHCP (see main.h).
 * The IP address that is selected will be shown on the onboard LCD display.
 * If the DHCP procedure is not successful, the selected static IP will
 * be 192.168.0.200
 *
 * The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/
 *
 *  Make sure that the following jumpers are correctly configured on the
 *  IAR-STM32F746xx-SK evaluation board:
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   B0_1/B0_0 - B0_0
 *   B1_1/B1_0 - B1_0
 *
 *    $Revision: 3190 $
 **************************************************************************/
#include "includes.h"

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

// variable for critical section entry control
uint32_t CriticalSecCntr;

// time counter variable - increments by 1 every 1 ms
volatile uint32_t timeCounter = 0;

I2C_HandleTypeDef I2cHandle;
ETH_HandleTypeDef EthHandle;
ADC_HandleTypeDef ADC2Handler;
UART_HandleTypeDef UartHandle;

extern uint8_t __resources_bin[];

#define RES_IAR_LOGO         (__resources_bin)
#define RES_IAR_LOGO_SIZE     15546

#pragma data_alignment=4
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB];

/* Ethernet Driver Receive buffers  */
#pragma data_alignment=4
uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE];

/* Ethernet Driver Transmit buffers */
#pragma data_alignment=4
uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE];

uint8_t macaddr[6] = {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5};

/*************************************************************************
 * Function Name: ADC_Configure
 * Parameters: void
 * Return: void
 *
 * Description: Configures the ADC Module
 *
 *************************************************************************/
void ADC_Configure(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
ADC_ChannelConfTypeDef ChanConfig;

  // ADC2 pin configuration
  TRIMER_CLK_ENABLE();

  GPIO_InitStructure.Pin = TRIMER_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(TRIMER_PORT, &GPIO_InitStructure);

  __HAL_RCC_ADC2_CLK_ENABLE();

  // ADC2 configuration
  ADC2Handler.Instance = ADC2;
  ADC2Handler.Init.Resolution = ADC_RESOLUTION_12B;
  ADC2Handler.Init.ScanConvMode = DISABLE;
  ADC2Handler.Init.ContinuousConvMode = DISABLE;
  ADC2Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  ADC2Handler.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
  ADC2Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  ADC2Handler.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&ADC2Handler);

  // ADC2 regular channel6 configuration
  ChanConfig.Channel = TRIMER_CHANNEL;
  ChanConfig.Rank = 1;
  ChanConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
  ChanConfig.Offset = 0;
  HAL_ADC_ConfigChannel(&ADC2Handler, &ChanConfig);

  // Start ADC2 Software Conversion
  HAL_ADC_Start(&ADC2Handler);
}

/*************************************************************************
 * Function Name: SysTickHandler
 * Parameters: void
 * Return: void
 *
 * Description: SysTick interrupt handler
 *
 *************************************************************************/
void SysTickHandler(void)
{
  timeCounter++;
}


/*************************************************************************
 * Function Name: SysTickHandler
 * Parameters: void
 * Return: void
 *
 * Description: SysTick interrupt handler
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 200000000
 *            HCLK(Hz)                       = 200000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 400
 *            PLL_P                          = 2
 *            PLL_Q                          = 8
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 6
 *************************************************************************/
void SystemClock_Config(void)
{
RCC_ClkInitTypeDef RCC_ClkInitStruct;
RCC_OscInitTypeDef RCC_OscInitStruct;
HAL_StatusTypeDef ret = HAL_OK;
RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    /* catch error */
    while(1);
  }

  /* Activate the OverDrive to reach the 200 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    /* catch error */
    while(1);
  }

  /* Select PLLSAI output as USB clock source, LTDC = 12MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48 | RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 8;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
  PeriphClkInitStruct.PLLSAIDivQ = 0;

  ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  if(ret != HAL_OK)
  {
    /* catch error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if(ret != HAL_OK)
  {
    /* catch error */
    while(1);
  }

}

/*************************************************************************
 * Function Name: CPU_CACHE_Enable
 * Parameters: none
 *
 * Return: none
 *
 * Description: Enables the CPU cache
 *************************************************************************/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

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
pPic_t pPic;

  /* Enable Cache */
  CPU_CACHE_Enable();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
  */
  HAL_Init();

  /* Configure MPU */
  MPU_RegionConfig();

  /* Configure the system clock to 200 MHz, USB to 48MHz, LTDC to 24MHz */
  SystemClock_Config();

  /* LCD backlight disable */
  STM_BacklightInit();

  /* SDRAM init */
  SDRAM_Init();

  /* Status LEDs port init */
  STM_LEDInit(LED1);
  STM_LEDInit(LED2);
  STM_LEDInit(LED3);
  STM_LEDInit(LED4);

  /* Turn off LEDs */
  STM_LEDOff(LED1);
  STM_LEDOff(LED2);
  STM_LEDOff(LED3);
  STM_LEDOff(LED4);

  /* Load/decode picture from png file to SDRAM */
  if(NULL == (pPic = GLCD_LoadPNG(RES_IAR_LOGO, RES_IAR_LOGO_SIZE)))
  {
    /* catch error */
    while(1);
  }

  /* Init LCD and copy picture to VRAM */
  GLCD_Init(pPic, NULL);
  GLCD_Ctrl(TRUE);
  GLCD_SetFont(&Terminal_9_12_6, GLCD_COLOR_BLACK, GLCD_COLOR_IAR_YELLOW,Layer1);
  GLCD_SetWindow(20,20,160,160,Layer1);
  GLCD_TextSetPos(0,0,Layer1);
  STM_BacklightOn();

  /* ADC init */
  ADC_Configure();

  /* I2C init */
  I2cHandle.Instance             = I2C2;
  I2cHandle.Init.Timing          = 0x40912732;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress1     = 0x00;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress2     = 0x00;
  I2cHandle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  I2cHandle.State                = HAL_I2C_STATE_RESET;

  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    GLCD_print(Layer1,"I2C2 init fail!\r\n");
  }

  /* STCN75 init */
  if(FALSE == TempSensor_Init(&I2cHandle)
  || FALSE == TempSensorShutdown(&I2cHandle, FALSE))
  {
    GLCD_print(Layer1,"STCN75 Init fault!\n\r");
    while(1);
  }

  EthHandle.Instance = ETH;
  EthHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
  EthHandle.Init.Speed = ETH_SPEED_100M;
  EthHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
  EthHandle.Init.MACAddr = &macaddr[0];
  EthHandle.Init.RxMode = ETH_RXPOLLING_MODE;
  EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
  EthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
  EthHandle.Init.PhyAddress = 0;
  EthHandle.State = HAL_ETH_STATE_RESET;

  if (HAL_OK != HAL_ETH_Init(&EthHandle))
  {
    GLCD_print(Layer1,"ETH init fail!\r\n");
    while(1);
  }

  /* Initialize Tx Descriptors list: Chain Mode */
  HAL_ETH_DMATxDescListInit(&EthHandle, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
  /* Initialize Rx Descriptors list: Chain Mode  */
  HAL_ETH_DMARxDescListInit(&EthHandle, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

  /* Initilaize the LwIP stack */
  LwIP_Init();

#ifndef USE_DHCP
  /* Show the IP address on LCD */
char s[25];
  sprintf(s,"IP:%d.%d.%d.%d", \
           IP_ADDR0, IP_ADDR1,\
           IP_ADDR2, IP_ADDR3);
  GLCD_print(Layer1,s);
#else
  GLCD_print(Layer1,"Obtaining IP...");
#endif

  /* Http web server init */
  httpd_init();

  /* Main loop */
  while (1)
  {
    // check if any packet received
    // process received ethernet packet
    LwIP_Pkt_Handle();
    // handle periodic timers for LwIP
    LwIP_Periodic_Handle(timeCounter);
  }
}
