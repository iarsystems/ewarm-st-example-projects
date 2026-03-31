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
 *    1. Date        : October 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    2. Date        : June 2015
 *       Author      : Atanas Uzunov
 *       Description : Adapted for ST HAL drivers
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F746xx-SK board. It shows basic
 * use of the parallel I/O, the timer, the interrupt, LCD and the USB_HS
 * controllers.
 *  Connect the board to the PC through the USB-OTG port and use the joystick
 * and the USER button to control the mouse pointer on the PC screen.
 * The LED4 shows the USB Device connect status, and the LED1 will turns on when
 * there is joystick or USER button activity.
 *  Additional info about usb hid status, x-axis, y-axis and the buttons +
 * "IAR Academy" logo is shown on the LCD display.
 *
 * The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/
 *
 * COMPATIBILITY
 * =============
 *  The USB HID example project is compatible with IAR-STM32F746xx-SK evaluation
 * board. By default, the project is configured to use the J-Link JTAG/SWD
 * interface.
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
 *    $Revision: 3189 $
 **************************************************************************/
#include "includes.h"

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

extern uint8_t __resources_bin[];

#define RES_IAR_LOGO         (__resources_bin)
#define RES_IAR_LOGO_SIZE     56259

USBD_HandleTypeDef USBD_Device;
TIM_HandleTypeDef Tim5Handle;
UART_HandleTypeDef UartHandle;

#define MOUSE_DELTA           4

/* States of the X-axis, Y-axis and button*/
typedef enum
{
  xstInactive,
  xstIdle,
  xstLeft,
  xstRight
} MouseXActionState_t;

typedef enum
{
  ystInactive,
  ystIdle,
  ystUp,
  ystDown
} MouseYActionState_t;

typedef enum
{
  bstInactive,
  bstIdle,
  bstClick
} MouseBActionState_t;

volatile MouseXActionState_t x_state = xstInactive;
volatile MouseYActionState_t y_state = ystInactive;
volatile MouseBActionState_t click_state = bstInactive;
volatile Boolean hid_active_state = FALSE;

/*************************************************************************
 * Function Name: Tim5Start
 * Parameters: uint32_t Tick - ticks per second
 *
 * Return: none
 *
 * Description: Starts and configures the Tim5.
 *************************************************************************/
void Tim5Start(uint32_t Tick)
{
  __HAL_RCC_TIM5_CLK_ENABLE();

  /* Configure TIM5 */
  Tim5Handle.Instance = TIM5;
  /* If APB1 PRESC == 1 , TIM clocks are x1 else x2 */
  Tim5Handle.Init.Period            = HAL_RCC_GetPCLK1Freq()*2/Tick;
  Tim5Handle.Init.Prescaler         = 0;
  Tim5Handle.Init.ClockDivision     = 0;
  Tim5Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  Tim5Handle.Init.RepetitionCounter = 0;

  if (HAL_TIM_Base_Init(&Tim5Handle) != HAL_OK)
  {
    /* Initialization Error */
    printf("TIM5 init fail!\r\n");
  }

  __HAL_TIM_CLEAR_FLAG(&Tim5Handle, TIM_FLAG_UPDATE);

  HAL_TIM_Base_Start(&Tim5Handle);
}

/*************************************************************************
 * Function Name: Tim5Stop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Stops the Tim5.
 *************************************************************************/
void Tim5Stop(void)
{
  HAL_TIM_Base_Stop(&Tim5Handle);
  HAL_TIM_Base_DeInit(&Tim5Handle);
}

/*************************************************************************
 * Function Name: Tim5IsOvf
 * Parameters: none
 *
 * Return: none
 *
 * Description: Check for Tim5 Ovf.
 *************************************************************************/
uint32_t Tim5IsOvf(void)
{
  if (__HAL_TIM_GET_FLAG(&Tim5Handle, TIM_FLAG_UPDATE))
  {
    __HAL_TIM_CLEAR_FLAG(&Tim5Handle, TIM_FLAG_UPDATE);
    return 1;
  }
  else
    return 0;
}

/*************************************************************************
 * Function Name: LCD_LOG_Status
 * Parameters: char * msg, LdcPixel_t color
 *
 * Return: none
 *
 * Description: Write USB status message.
 *
 *************************************************************************/
void LCD_LOG_Status(char * msg, LdcPixel_t color)
{
  GLCD_SetFont(&Terminal_9_12_6,color,GLCD_COLOR_WHITE,Layer1);
  GLCD_SetWindow(12,50,100,62,Layer1);
  GLCD_print(Layer1,"\f%s",msg);
}

/*************************************************************************
 * Function Name: LCD_LOG_ActionX
 * Parameters: char * msg, LdcPixel_t color
 *
 * Return: none
 *
 * Description: Write the x-axis state message on the LCD.
 *
 *************************************************************************/
void LCD_LOG_ActionX(char * msg, LdcPixel_t color)
{
  GLCD_SetFont(&Terminal_9_12_6,color,GLCD_COLOR_WHITE,Layer1);
  GLCD_SetWindow(12,64,100,76,Layer1);
  GLCD_print(Layer1,"\fX:%s",msg);
}

/*************************************************************************
 * Function Name: LCD_LOG_ActionY
 * Parameters: char * msg, LdcPixel_t color
 *
 * Return: none
 *
 * Description: Write the y-axis state message on the LCD.
 *
 *************************************************************************/
void LCD_LOG_ActionY(char * msg, LdcPixel_t color)
{
  GLCD_SetFont(&Terminal_9_12_6,color,GLCD_COLOR_WHITE,Layer1);
  GLCD_SetWindow(12,78,100,90,Layer1);
  GLCD_print(Layer1,"\fY:%s",msg);
}

/*************************************************************************
 * Function Name: LCD_LOG_ActionB
 * Parameters: char * msg, LdcPixel_t color
 *
 * Return: none
 *
 * Description: Write the button state message on the LCD.
 *
 *************************************************************************/
void LCD_LOG_ActionB(char * msg, LdcPixel_t color)
{
  GLCD_SetFont(&Terminal_9_12_6,color,GLCD_COLOR_WHITE,Layer1);
  GLCD_SetWindow(12,92,100,104,Layer1);
  GLCD_print(Layer1,"\fB:%s",msg);
}

/*************************************************************************
 * Function Name: USB_Mouse_Disconnect_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Callback function on device disconnection
 *
 *************************************************************************/
void USB_Mouse_Disconnect_Handler(void)
{
  x_state = xstInactive;
  y_state = ystInactive;
  click_state = bstInactive;
  hid_active_state = FALSE;
}

/*************************************************************************
 * Function Name: USB_Mouse_Connect_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Callback function on device connection
 *
 *************************************************************************/
void USB_Mouse_Connect_Handler(void)
{
  hid_active_state = TRUE;
}

/*************************************************************************
 * Function Name: USB_Mouse
 * Parameters: none
 *
 * Return: none
 *
 * Description: Read joystick state and sends HID reports to Host
 *
 *************************************************************************/
void USB_Mouse(void)
{
Boolean hid_active_state_old;

uint8_t buf[4] = {0, 0, 0, 0};

#define Buttons buf[0]
#define X       buf[1]
#define Y       buf[2]

uint8_t ButtonsHold = 0;

  /* Timer5 - tick 100 times per second */
  Tim5Start(100);

  /*Init Buttons*/
  STM_ButtonInit(BUTTON_USER, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUTTON_LEFT, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUTTON_SEL, BUTTON_MODE_GPIO);

  LCD_LOG_ActionX("Inactive",GLCD_COLOR_GREY);
  LCD_LOG_ActionY("Inactive",GLCD_COLOR_GREY);
  LCD_LOG_ActionB("Inactive",GLCD_COLOR_GREY);
  hid_active_state_old = FALSE;

  while(1)
  {
    if(hid_active_state_old != hid_active_state)
    {
      if (!hid_active_state)
      {
        LCD_LOG_ActionX("Inactive",GLCD_COLOR_GREY);
        LCD_LOG_ActionY("Inactive",GLCD_COLOR_GREY);
        LCD_LOG_ActionB("Inactive",GLCD_COLOR_GREY);
      }
      hid_active_state_old = hid_active_state;
    }
    /* If device is not connected skip processing */
    if (hid_active_state != TRUE) continue;
    if( Tim5IsOvf() )
    {
      /* Check states of joystyck buttons */
      /* Set X-axis state */
      if(GPIO_PIN_SET == STM_ButtonGetState(BUTTON_RIGHT))
      {
        X += MOUSE_DELTA;
        if (xstRight != x_state)
        {
          x_state = xstRight;
          LCD_LOG_ActionX("Right",GLCD_COLOR_BLACK);
        }
      }
      else if(GPIO_PIN_SET == STM_ButtonGetState(BUTTON_LEFT))
      {
        X -= MOUSE_DELTA;
        if (xstLeft != x_state)
        {
          x_state = xstLeft;
          LCD_LOG_ActionX("Left",GLCD_COLOR_BLACK);
        }
      }
      else
      {
        if (xstIdle != x_state)
        {
          x_state = xstIdle;
          LCD_LOG_ActionX("Idle",GLCD_COLOR_BLACK);
        }
      }
      /* Set Y-axis state */
      if(GPIO_PIN_SET == STM_ButtonGetState(BUTTON_DOWN))
      {
        Y += MOUSE_DELTA;
        if (ystDown != y_state)
        {
          y_state = ystDown;
          LCD_LOG_ActionY("Down",GLCD_COLOR_BLACK);
        }
      }
      else if(GPIO_PIN_SET == STM_ButtonGetState(BUTTON_UP))
      {
        Y -= MOUSE_DELTA;
        if (ystUp != y_state)
        {
          y_state = ystUp;
          LCD_LOG_ActionY("Up",GLCD_COLOR_BLACK);
        }
      }
      else
      {
        if (ystIdle != y_state)
        {
          y_state = ystIdle;
          LCD_LOG_ActionY("Idle",GLCD_COLOR_BLACK);
        }
      }
      /* Set Button state */
      if((GPIO_PIN_SET == STM_ButtonGetState(BUTTON_SEL)) || (GPIO_PIN_RESET == STM_ButtonGetState(BUTTON_USER)))
      {
        Buttons |= 1;
        if (bstClick != click_state)
        {
          click_state = bstClick;
          LCD_LOG_ActionB("Click",GLCD_COLOR_BLACK);
        }
      }
      else
      {
        Buttons &= ~1;
        if (bstIdle != click_state)
        {
          click_state = bstIdle;
          LCD_LOG_ActionB("Idle",GLCD_COLOR_BLACK);
        }
      }
      /* If there is joystick or button pressed */
      if(Y || X  || Buttons || (ButtonsHold ^ Buttons))
      {
        /* Send report */
        USBD_HID_SendReport (&USBD_Device,
                             buf,
                             4);
        ButtonsHold = Buttons;
        Y = X = Buttons = 0;
        STM_LEDOn(LED1);
      }
      else
      {
        STM_LEDOff(LED1);
      }
    }
  }
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
int main(void)
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

  /* Load/decode PNG picture from png file to SDRAM */
  if(NULL == (pPic = GLCD_LoadPNG(RES_IAR_LOGO, RES_IAR_LOGO_SIZE)))
  {
    while(1);
  }

  /* Init LCD and copy picture to VRAM */
  GLCD_Init(NULL, NULL);
  GLCD_SetWindow(0,0,479,271,Layer1);
  GLCD_SetFont(&Terminal_18_24_12,GLCD_COLOR_BLACK,GLCD_COLOR_WHITE,Layer1);
  GLCD_print(Layer1,"\f");
  GLCD_TextSetPos(1,1,Layer1);
  GLCD_print(Layer1,"USB HID");
  GLCD_ShowPic(102,0,pPic,0,Layer1);
  GLCD_Ctrl(TRUE);
  STM_BacklightOn();

  /* Init onboard LEDs */
  STM_LEDInit(LED1);
  STM_LEDInit(LED2);
  STM_LEDInit(LED3);
  STM_LEDInit(LED4);

  STM_LEDOff(LED1);
  STM_LEDOff(LED2);
  STM_LEDOff(LED3);
  STM_LEDOff(LED4);

  /* Init Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);

  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_HID_CLASS);

  /* Start Device Process */
  USBD_Start(&USBD_Device);

  /* Run Application */
  USB_Mouse();

  return 0;
}
