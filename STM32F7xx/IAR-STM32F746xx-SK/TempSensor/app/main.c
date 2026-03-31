/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2015
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : June, 2011
 *       Author      : Stoyan Choynev
 *       Description :
 *
 *    2. Date        : June, 2015
 *       Author      : Atanas Uzunov
 *       Description : Updated to support the STM32F746xx-SK board.
 *                     Added Celsius to Fahrenheit conversion, temperature
 *                     reading indicator, USER button handler and PNG support.
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32F746xx-SK board. It shows basic use of the
 * parallel I/O,the SysTick timer,the interrupt controller,the LCD controller
 * and the I2C temperature sensor.
 *  Measured/readed board temperature is shown on the LCD display below the IAR logo.
 * Each time the temperature is measured/readed, the ReadTemp LED (LED1) lights up
 * for 100ms. The alarm (LED3) activates when temperature is over 34.0°C and deactivates
 * when it drop below 33.0°C. User can swith the measurement units (Celsius/Fahrenheit)
 * alternatively, by pressing the USER button at the board.
 *
 * The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/
 *
 *  Make sure that the following jumpers are correctly configured on the
 * IAR-STM32F746xx-SK evaluation board:
 *
 * Jumpers:
 *  PWR_SEL - depending of power source
 *  B0_1/B0_0 - B0_0
 *  B1_1/B1_0 - B1_0
 *
 *    $Revision: 3190 $
 **************************************************************************/
#include "includes.h"

#define READTEMP_0_5S_RELOAD    499
#define LEDOFF_0_1S_RELOAD      99

extern uint8_t __resources_bin[];

#define RES_IAR_LOGO         (__resources_bin)
#define RES_IAR_LOGO_SIZE     15546

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

volatile uint32_t flags0;

UART_HandleTypeDef UartHandle;

I2C_HandleTypeDef I2cHandle;

#define flTickLedOff               (flags0 &   ( 1UL << 0 ))
#define flTickLedOffSet()          (flags0 |=  ( 1UL << 0 ))
#define flTickLedOffClear()        (flags0 &= ~( 1UL << 0 ))

#define flTickReadTemp             (flags0 &   ( 1UL << 1 ))
#define flTickReadTempSet()        (flags0 |=  ( 1UL << 1 ))
#define flTickReadTempClear()      (flags0 &= ~( 1UL << 1 ))

#define flUserButtonChanged        (flags0 &   ( 1UL << 2 ))
#define flUserButtonChangedSet()   (flags0 |=  ( 1UL << 2 ))
#define flUserButtonChangedClear() (flags0 &= ~( 1UL << 2 ))

#define flUpdateDisplay            (flags0 &   ( 1UL << 3 ))
#define flUpdateDisplaySet()       (flags0 |=  ( 1UL << 3 ))
#define flUpdateDisplayClear()     (flags0 &= ~( 1UL << 3 ))

#define flFahrenheit               (flags0 &   ( 1UL << 4 ))
#define flFahrenheitToggle()       (flags0 ^=  ( 1UL << 4 ))

volatile uint32_t LedOffTimer=0, ReadTempTimer=0;
volatile uint8_t ButtonState;
uint8_t LastButtonState;

#define USER_BUTTON_PRESSED    (ButtonState == 1)
#define USER_BUTTON_RELEASED   (ButtonState == 0)

/*************************************************************************
 * Function Name: UserButtonHandler
 * Parameters: void
 * Return: void
 *
 * Description: Reads USER button and sets flUserButtonChanged if there is
 *              change in the button state.
 *
 *************************************************************************/
void UserButtonHandler(void)
{
  if (!flUserButtonChanged)
  {
    ButtonState = 0;
    ButtonState = (STM_ButtonGetState(BUTTON_USER) ? 0 : 1);
    if (ButtonState != LastButtonState) flUserButtonChangedSet();
    LastButtonState = ButtonState;
  }
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
  if(!LedOffTimer--)
  {
    flTickLedOffSet();
    LedOffTimer = LEDOFF_0_1S_RELOAD;
  }
  if(!ReadTempTimer--)
  {
    flTickReadTempSet();
    ReadTempTimer = READTEMP_0_5S_RELOAD;
  }
  UserButtonHandler();
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

  /* Select PLLSAI output as USB clock source, LTDC(R) = 9.6MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48 | RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 8;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
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

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
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
float Temp = 0.0, TempHold = 0.0;
Boolean Alarm;

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

  /* Init I2C2 bus */
  I2cHandle.Instance             = I2C2;
  I2cHandle.Init.Timing          = 0x40913227;
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
    while(1);
  }

  /* Status LEDS port init */
  STM_LEDInit(LED1); // ReadTemp LED
  STM_LEDInit(LED2);
  STM_LEDInit(LED3); // Alarm LED
  STM_LEDInit(LED4);

  /* Turn off LEDs */
  STM_LEDOff(LED1);
  STM_LEDOff(LED2);
  STM_LEDOff(LED3);
  STM_LEDOff(LED4);

  /* USER Button init */
  STM_ButtonInit(BUTTON_USER,BUTTON_MODE_GPIO);

  /* GLCD init */
  GLCD_Ctrl(FALSE);

  /* Load/decode PNG picture from png file to SDRAM */
  if(NULL == (pPic = GLCD_LoadPNG(RES_IAR_LOGO, RES_IAR_LOGO_SIZE)))
  {
    while(1);
  }

  /* Init LCD and copy picture to VRAM */
  GLCD_Init (pPic, NULL);
  GLCD_Ctrl (TRUE);
  STM_BacklightOn();
  GLCD_SetFont(&Terminal_18_24_12, GLCD_COLOR_BLACK, GLCD_COLOR_IAR_YELLOW, Layer1);
  GLCD_SetWindow(95,210,470,270,Layer1);
  GLCD_TextSetPos(0,0,Layer1);

  /* Init Temperature sensor */
  if(   FALSE == TempSensor_Init(&I2cHandle)
     || FALSE == TempSensorShutdown(&I2cHandle, FALSE)
     || FALSE == TempSensor_Conf(&I2cHandle, 34.0,33.0,TEMP_SENSOR_COMP_MODE,2))
  {
    /* Initialization fault */
    GLCD_TextSetPos(0,0,Layer1);
    GLCD_print(Layer1,"STCN75 init fault!");
    while(1);
  }

  flags0 = 0;

  while(1)
  {
    /* If there is a change in the state of USER button */
    if (flUserButtonChanged)
    {
      if (USER_BUTTON_PRESSED)
      {
        /* Toggle Fahrenheit/Celsius */
        flFahrenheitToggle();
        /* Display should be updated */
        flUpdateDisplaySet();
      }
      flUserButtonChangedClear();
    }

    if (flTickReadTemp)
    {
      flTickReadTempClear();
      flTickLedOffClear();
      STM_LEDOn(LED1);
      /* Get new sample of the temperature */
      if(TempSensorGetTemp(&I2cHandle, &Temp, &Alarm))
      {
        if(Alarm)
        {
          /* Turn on Alarm LED */
          STM_LEDOn(LED3);
        }
        else
        {
          /* Turn off Alarm LED */
          STM_LEDOff(LED3);
        }

        /* If the current temperature sample differs from the previous one */
        if(TempHold != Temp)
        {
          TempHold = Temp;
          /* Display should be updated */
          flUpdateDisplaySet();
        }
      }
      else
      {
        /* Initialization fault */
        GLCD_TextSetPos(0,0,Layer1);
        GLCD_print(Layer1,"STCN75 reading fault!");
        while(1);
      }
    }
    else if (flTickLedOff)
    {
      flTickLedOffClear();
      /* turns off ReadTemp LED */
      STM_LEDOff(LED1);
    }

    /* if display should be updated */
    if(flUpdateDisplay)
    {
      GLCD_TextSetPos(0,0,Layer1);
      GLCD_print(Layer1,"Board Temperature: ");
      if(flFahrenheit)
      {
        /* Print temperature in Fahrenheit */
        GLCD_print(Layer1,"%3.1f \370F    ",((9.0/5.0)*Temp + 32.0));
      }
      else
      {
        /* Print temperature in Celsius */
        GLCD_print(Layer1,"%3.1f \370C    ",Temp);
      }
      flUpdateDisplayClear();
    }
  }
}
