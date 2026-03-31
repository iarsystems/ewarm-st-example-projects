/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    1. Date        : September 2011
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    2. Date        : August 2013
 *       Author      : Atanas Uzunov
 *       Description : Ported to STM32F429II-SK board
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F429II-SK board. It is an
 * embedded Web Server based on lwIP TCP/IP stack v1.3.2 and features
 * three HTML pages that form a compact and interactive Web Server to
 * interact with IAR-STM32F429II-SK board.
 *  You will be able to address the IAR-STM32F429II-SK board as a web
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
 *  IAR-STM32F429II-SK evaluation board:
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   B0_1/B0_0 - B0_0
 *   B1_1/B1_0 - B1_0
 *
 *    $Revision: 3190 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US           1665     // value @ 168MHz

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

// variable for critical section entry control
uint32_t CriticalSecCntr;

// time counter variable - increments by 1 every 1 ms
volatile uint32_t timeCounter = 0;

extern uint8_t __resources_bin[];

#define RES_IAR_LOGO         (__resources_bin)
#define RES_IAR_LOGO_SIZE     15546

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
ADC_InitTypeDef       ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC2 clock */
  RCC_AHB1PeriphClockCmd(TRIMER_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

  /* Configure TRIMER_PORT/TRIMER_PIN (ADC2 Channel6) as analog input */
  GPIO_InitStructure.GPIO_Pin = TRIMER_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TRIMER_PORT, &GPIO_InitStructure);

  /* ADC Common Initialization */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div6;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC2 Configuration */
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC2, &ADC_InitStructure);

  /* ADC2 regular channel6 configuration */
  ADC_RegularChannelConfig(ADC2, TRIMER_CHANNEL, 1, ADC_SampleTime_56Cycles);

  /* Enable ADC2 */
  ADC_Cmd(ADC2, ENABLE);

  /* ADC2 Software Start Conversion */
  ADC_SoftwareStartConv(ADC2);
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
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
  }
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
uint32_t ethResult;
pPic_t pPic;

 /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  ENTR_CRT_SECTION();

  /* Enable ETHERNET and GPIOs clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |
                         RCC_AHB1Periph_ETH_MAC_Rx | RCC_AHB1Periph_ETH_MAC_PTP |
                         RCC_AHB1Periph_GPIOA |
                         RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                         RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

  /* Init LCD backlight port */
  STM_BacklightInit();

  /* SysTick Config */
  if(SysTick_Config(SystemCoreClock/1000))
  {
    /* Capture error */
    while (1);
  }

  EXT_CRT_SECTION();

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

  /* SDRAM init */
  SDRAM_Init();

  /* Load/decode picture from png file to SDRAM */
  if(NULL == (pPic = GLCD_LoadPNG(RES_IAR_LOGO, RES_IAR_LOGO_SIZE)))
  {
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
  I2C2_Init();

  /* STCN75 init */
  if(FALSE == TempSensor_Init()
  || FALSE == TempSensorShutdown(FALSE))
  {
    GLCD_print(Layer1,"STCN75 Init fault\n\r");
    while(1);
  }

  /* Ethernet init */
  ethResult = Ethernet_Configure();
  switch(ethResult)
  {
    case ETHERNET_SUCCESS :
      GLCD_print(Layer1,"lwIP WEB Server\n\r");
      break;

    case ETHERNET_PHY_ERROR :
      GLCD_print(Layer1,"Eth PHY Error\r");
      while(1);

    case ETHERNET_INIT_ERROR :
      GLCD_print(Layer1,"Eth Init Error\r");
      while(1);

    default : break;
  }

  /* Initilaize the LwIP stack */
  LwIP_Init();

#ifndef USE_DHCP
  /* Show the IP address on LCD */
char s[20];
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
    /* check if any packet received */
    if (ETH_CheckFrameReceived())
    {
      /* process received ethernet packet */
      LwIP_Pkt_Handle();
    }
    /* handle periodic timers for LwIP */
    LwIP_Periodic_Handle(timeCounter);
  }
}
