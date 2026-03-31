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
 *    1. Date        : May, 2015
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F746xx-SK board. It shows basic
 * use of the parallel I/O, the timer, the interrupt controller, the LCD
 * controller and the 3D accelerometer sensor. The IAR logo and a "magnifying
 * glass" are displayed on the LCD. The magnifying glass moves as the board
 * position is changed.
 *
 *  The background and the magnifying glass image files are combined into the
 * "images.bin" file with the command:
 *
 * copy /b iar_logo.png+edit-find.png images.bin
 *
 * The project uses "edit-find" icon from http://openiconlibrary.sourceforge.net/
 *
 * The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/
 *
 * COMPATIBILITY
 * =============
 *  The accelerometer sensor example project is compatible with IAR-STM32F746xx-SK
 * evaluation board. By default, the project is configured to use the
 * I-Jet JTAG/SWD interface.
 *
 * CONFIGURATION
 * =============
 * Flash Debug - The Progam is loaded to internal Flash.
 *
 *  Make sure that the following jumpers are correctly configured on the
 * IAR-STM32F746xx-SK evaluation board:
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   B0_1/B0_0 - B0_0
 *   B1_1/B1_0 - B1_0
 *
 *
 *    $Revision: 3186 $
 **************************************************************************/
#include "includes.h"

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

UART_HandleTypeDef UartHandle;

I2C_HandleTypeDef I2cHandle;

/* variable for critical section entry control */
uint32_t CriticalSecCntr;

extern uint8_t __resources_bin[];

#define RES_IAR_LOGO         (__resources_bin)
#define RES_IAR_LOGO_SIZE     15546

#define RES_CURSOR           (__resources_bin + RES_IAR_LOGO_SIZE)
#define RES_CURSOR_SIZE       2678

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
int main(void)
{
int16_t cursor_x;
int16_t cursor_y;
int16_t X, Y, Z, DM;
pPic_t pPic;
pPic_t pCursor;

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

  /* Load/decode picture from png file to SDRAM */
  if(NULL == (pPic = GLCD_LoadPNG(RES_IAR_LOGO, RES_IAR_LOGO_SIZE)))
  {
    while(1);
  }

  /* Load/decode picture from png file to SDRAM */
  if(NULL == (pCursor = GLCD_LoadPNG(RES_CURSOR, RES_CURSOR_SIZE)))
  {
    while(1);
  }

  /* Init LCD and copy picture to VRAM */
  GLCD_Init(pPic, NULL);
  GLCD_Ctrl(TRUE);
  GLCD_SetFont(&Terminal_9_12_6, GLCD_COLOR_BLACK, GLCD_COLOR_IAR_YELLOW, Layer1);
  GLCD_SetWindow(20,20,160,160,Layer1);
  GLCD_TextSetPos(0,0,Layer1);

  cursor_x = (C_GLCD_H_SIZE - pCursor->H_Size)/2;
  cursor_y = (C_GLCD_V_SIZE - pCursor->V_Size)/2;

  /* Init Cursor Layer */
  GLCD_Layer2_Init(pCursor, NULL);

  /* Set cursor position */
  GLCD_Layer2_Position(cursor_x, cursor_y);

  /* Turn backlight on */
  STM_BacklightOn();

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
    //printf("I2C2 init fail!\r\n");
    return 1;
  }

  /* Accelerometer init */
  if(FALSE == LSM6DS3_Init(&I2cHandle, ( CTRL1_ODR_52HZ | CTRL1_FS_2g | CTRL1_BW_400HZ )))
  {
    /* Capture error */
    while(1);
  }

  /* Init LED 1 */
  STM_LEDInit(LED1);

  while(1)
  {
    if(LSM6DS3_AccReady())
    {
      /* Toggle LED1 */
      STM_LEDToggle(LED1);

      /* Read data from accelerometer */
      LSM6DS3_Get(&I2cHandle, &X, &Y, &Z, &DM, &DM, &DM, &DM);

      /* Calculate the new cursor position */
      cursor_x += X>>8;
      cursor_y += Y>>8;

      /* Check X boundaries */
      if((int16_t)(C_GLCD_H_SIZE - pCursor->H_Size ) < cursor_x)
      {
        cursor_x = C_GLCD_H_SIZE - pCursor->H_Size;
      }

      if(0 > cursor_x)
      {
        cursor_x = 0;
      }

      /* Check Y boundaries */
      if((int16_t)(C_GLCD_V_SIZE - pCursor->V_Size) < cursor_y)
      {
        cursor_y = (C_GLCD_V_SIZE - pCursor->V_Size);
      }

      if(0 > cursor_y)
      {
        cursor_y = 0;
      }

      /* Set new cursor position */
      GLCD_Layer2_Position(cursor_x, cursor_y);
    }
  }
}
