/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : October 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F429II-SK board. It shows basic
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
 *  The USB HID example project is compatible with IAR-STM32F429II-SK evaluation
 * board. By default, the project is configured to use the J-Link JTAG/SWD
 * interface.
 *
 * CONFIGURATION
 * =============
 * Flash Debug - The Progam is loaded to internal Flash.
 *
 *  Make sure that the following jumpers are correctly configured on the
 * IAR-STM32F429II-SK evaluation board:
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

/* variable for critical section entry control */
uint32_t CriticalSecCntr;

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

extern uint8_t __resources_bin[];

#define RES_IAR_LOGO         (__resources_bin)
#define RES_IAR_LOGO_SIZE     56259

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#pragma data_alignment=4
#endif
USB_OTG_CORE_HANDLE  USB_OTG_dev;

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

/*************************************************************************
 * Function Name: SysTickStart
 * Parameters: uint32_t Tick
 *
 * Return: none
 *
 * Description: SysTick timer configuration
 *
 *************************************************************************/
void SysTickStart(uint32_t Tick)
{
RCC_ClocksTypeDef Clocks;
volatile uint32_t dummy;

  RCC_GetClocksFreq(&Clocks);

  dummy = SysTick->CTRL;

  /* SysTick uses extclock source which is HCLK/8 */
  SysTick->LOAD = (Clocks.HCLK_Frequency/8)/Tick;

  SysTick->CTRL = 1;
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
MouseXActionState_t x_state = xstInactive;
MouseYActionState_t y_state = ystInactive;
MouseBActionState_t click_state = bstInactive;
Boolean hid_active_state = FALSE;
uint8_t buf[4] = {0, 0, 0, 0};

#define Buttons buf[0]
#define X       buf[1]
#define Y       buf[2]

uint8_t ButtonsHold = 0;

  ENTR_CRT_SECTION();
  /* SysTick 100 times per second */
  SysTickStart(100);
  EXT_CRT_SECTION();

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

  while('\x1B' !=  getchar())
  {
    if (USB_OTG_CONFIGURED != USB_OTG_dev.dev.device_status) {
      if(hid_active_state)
      {
        x_state = xstInactive;
        y_state = ystInactive;
        click_state = bstInactive;
        LCD_LOG_ActionX("Inactive",GLCD_COLOR_GREY);
        LCD_LOG_ActionY("Inactive",GLCD_COLOR_GREY);
        LCD_LOG_ActionB("Inactive",GLCD_COLOR_GREY);
      }
      hid_active_state = FALSE;
      continue;
    }
    hid_active_state = TRUE;
    if( SysTick->CTRL & (1<<16))
    {
      /* Check states of joystyck buttons */
      /* Set X-axis state */
      if(Bit_SET == STM_ButtonGetState(BUTTON_RIGHT))
      {
        X += MOUSE_DELTA;
        if (xstRight != x_state)
        {
          x_state = xstRight;
          LCD_LOG_ActionX("Right",GLCD_COLOR_BLACK);
        }
      }
      else if(Bit_SET == STM_ButtonGetState(BUTTON_LEFT))
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
      if(Bit_SET == STM_ButtonGetState(BUTTON_DOWN))
      {
        Y += MOUSE_DELTA;
        if (ystDown != y_state)
        {
          y_state = ystDown;
          LCD_LOG_ActionY("Down",GLCD_COLOR_BLACK);
        }
      }
      else if(Bit_SET == STM_ButtonGetState(BUTTON_UP))
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
      if((Bit_SET == STM_ButtonGetState(BUTTON_SEL)) || (Bit_RESET == STM_ButtonGetState(BUTTON_USER)))
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
        USBD_HID_SendReport (&USB_OTG_dev,
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
USART_InitTypeDef USART_InitStructure;

  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  /* Enable GPIOs clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                         RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                         RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG |
                         RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI, ENABLE);

  EXT_CRT_SECTION();

  /* Init LCD backlight port */
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

  /* USARTx configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  STM_COMInit(COM1, &USART_InitStructure);

  USBD_Init(&USB_OTG_dev,
            USB_OTG_HS_CORE_ID,
            &HID_Joystick_descriptor,
            &USBD_HID_cb,
            &USR_cb);

  USB_Mouse();

  return 0;
}
