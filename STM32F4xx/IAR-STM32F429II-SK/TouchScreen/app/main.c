/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2016
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : June, 2016
 *       Author      : Atanas Uzunov
 *       Description : Initial revision.
 *
 * This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32F429II-SK board. It shows basic use of the
 * parallel I/O, the interrupt controller, the LCD controller, ADC module, 
 * and TIM5 module.
 * The example demostrates how to use the touchscreen driver.
 * Use a pen(stylus)-like object to draw on the screen using red color.
 *
 * The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/
 *
 * Make sure that the following jumpers are correctly configured on the
 * IAR-STM32F429II-SK evaluation board:
 *
 * Jumpers:
 *  PWR_SEL - depending of power source
 *  B0_1/B0_0 - B0_0
 *  B1_1/B1_0 - B1_0
 *
 *    $Revision: 3190 $
 **************************************************************************/
#include "includes.h"

extern uint8_t __resources_bin[];

#define RES_IAR_LOGO         (__resources_bin)
#define RES_IAR_LOGO_SIZE     15546

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

// variable for critical section entry control
uint32_t CriticalSecCntr;

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
uint32_t cursor_x, cursor_y;
pPic_t pPic;
ToushRes_t XY_Touch;
Boolean Touch = FALSE;
lcd_32bitpixel_t Pixel;

 /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  ENTR_CRT_SECTION();

  /* Enable GPIOs clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                         RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                         RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

  /* Init LCD backlight port */
  STM_BacklightInit();

  /* I2C2 init */
  I2C2_Init();

  /* LED ports init */
  STM_LEDInit(LED1);
  STM_LEDInit(LED2);
  STM_LEDInit(LED3);
  STM_LEDInit(LED4);

  /* Turn off LEDs */
  STM_LEDOff(LED1);
  STM_LEDOff(LED2);
  STM_LEDOff(LED3);
  STM_LEDOff(LED4);

  /* USER Button init */
  STM_ButtonInit(BUTTON_USER,BUTTON_MODE_GPIO);

  EXT_CRT_SECTION();

  /* SDRAM init */
  SDRAM_Init();

  /* GLCD init */
  GLCD_Ctrl(FALSE);

  /* Load/decode PNG picture from png file to SDRAM */
  if(NULL == (pPic = GLCD_LoadPNG(RES_IAR_LOGO, RES_IAR_LOGO_SIZE)))
  {
    while(1);
  }

  /* Init LCD and copy picture to VRAM */
  GLCD_Init(pPic, NULL);
  GLCD_Ctrl(TRUE);
  STM_BacklightOn();
  GLCD_SetFont(&Terminal_18_24_12, GLCD_COLOR_BLACK, GLCD_COLOR_IAR_YELLOW, Layer1);
  GLCD_SetWindow(100,200,470,270,Layer1);
  GLCD_TextSetPos(0,0,Layer1);
  GLCD_print(Layer1,"Use a stylus-like object\r\n to draw on the screen.");

  /* Init drawing color */
  Pixel.red = 0xFF;
  Pixel.green = 0x00;
  Pixel.blue = 0x00;

  /* Init the touch screen */
  TouchScrInit();

  while(1)
  {
    /* Check if data is read by the touch screen driver */
    if(TouchGet(&XY_Touch))
    {
      cursor_x = XY_Touch.X;
      cursor_y = XY_Touch.Y;
      GLCD_PutPixel(cursor_x, cursor_y, &Pixel, Layer1);
      if(cursor_x > 0) GLCD_PutPixel(cursor_x-1, cursor_y, &Pixel, Layer1);
      if(cursor_x < C_GLCD_H_SIZE-1) GLCD_PutPixel(cursor_x+1, cursor_y, &Pixel, Layer1);
      if(cursor_y > 0) GLCD_PutPixel(cursor_x, cursor_y-1, &Pixel, Layer1);
      if(cursor_y < C_GLCD_V_SIZE-1) GLCD_PutPixel(cursor_x, cursor_y+1, &Pixel, Layer1);
      if (FALSE == Touch)
      {
        Touch = TRUE;
      }
    }
    else if(Touch)
    {
      Touch = FALSE;
    }
  }
}
