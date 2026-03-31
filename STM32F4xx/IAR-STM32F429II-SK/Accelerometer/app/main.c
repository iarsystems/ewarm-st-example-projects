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
 *    1. Date        : August 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F429II-SK board. It shows basic
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
 *  The accelerometer sensor example project is compatible with IAR-STM32F429II-SK
 * evaluation board. By default, the project is configured to use the
 * J-Link JTAG/SWD interface.
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
 *    $Revision: 3186 $
 **************************************************************************/
#include "includes.h"

#define TICK_PER_SEC          30

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

static volatile uint32_t TimingDelay;

/* variable for critical section entry control */
uint32_t CriticalSecCntr;

extern uint8_t __resources_bin[];

#define RES_IAR_LOGO         (__resources_bin)
#define RES_IAR_LOGO_SIZE     15546

#define RES_CURSOR           (__resources_bin + RES_IAR_LOGO_SIZE)
#define RES_CURSOR_SIZE       2678

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: uint32_t Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(uint32_t Dly)
{
  TimingDelay = Dly;

  while(TimingDelay != 0);
}

/*************************************************************************
 * Function Name: TimingDelay_Decrement
 * Parameters: none
 *
 * Return: none
 *
 * Description: SysTick Handler function
 *
 *************************************************************************/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
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
int16_t cursor_x;
int16_t cursor_y;
int16_t X, Y, Z;
pPic_t pPic;
pPic_t pCursor;

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

  /* SysTick Config */
  if(SysTick_Config(SystemCoreClock/10000))
  {
    /* Capture error */
    while (1);
  }

  EXT_CRT_SECTION();

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

  /* Accelerometer init */
  I2C2_Init();
  if(FALSE == Accl_Init())
  {
    /* Capture error */
    while(1);
  }

  /* Init LED 1 */
  STM_LEDInit(LED1);

  while(1)
  {
    /* Delay */
    DelayResolution100us(10000/TICK_PER_SEC);

    /* Toggle LED1 */
    STM_LEDToggle(LED1);

    /* Read data from accelerometer */
    Accl_Get(&X,&Y,&Z);

    /*Calculate the new cursor position*/
    cursor_x += X>>6;
    cursor_y += Y>>6;

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
