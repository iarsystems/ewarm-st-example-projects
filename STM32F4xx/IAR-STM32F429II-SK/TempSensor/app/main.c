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
 *    1. Date        : June 2011
 *       Author      : Stoyan Choynev
 *       Description :
 *
 *    2. Date        : October 2013
 *       Author      : Atanas Uzunov
 *       Description : Updated to support the STM32F429II-SK board.
 *                     Added Celsius to Fahrenheit conversion, temperature
 *                     reading indicator, USER button handler and PNG support.
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32F429II-SK board. It shows basic use of the
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

#define DLY_100US               1665     // value @ 168MHz
#define READTEMP_0_5S_RELOAD    49
#define LEDOFF_0_1S_RELOAD      9

extern uint8_t __resources_bin[];

#define RES_IAR_LOGO         (__resources_bin)
#define RES_IAR_LOGO_SIZE     15546

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

// variable for critical section entry control
uint32_t CriticalSecCntr;

volatile uint32_t flags0=0;

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

volatile uint8_t LedOffTimer=0, ReadTempTimer=0;
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
pPic_t pPic;
float Temp = 0.0, TempHold = 0.0;
Boolean Alarm;

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

  /* SysTick Config*/
  if(SysTick_Config(SystemCoreClock/100))
  {
    /* Capture error */
    while (1);
  }

  /* I2C2 init */
  I2C2_Init();

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
  GLCD_Init (pPic, NULL);
  GLCD_Ctrl (TRUE);
  STM_BacklightOn();
  GLCD_SetFont(&Terminal_18_24_12, GLCD_COLOR_BLACK, GLCD_COLOR_IAR_YELLOW, Layer1);
  GLCD_SetWindow(95,210,470,270,Layer1);
  GLCD_TextSetPos(0,0,Layer1);

  /* Init Temperature sensor */
  if(   FALSE == TempSensor_Init()
     || FALSE == TempSensorShutdown(FALSE)
     || FALSE == TempSensor_Conf(34.0,33.0,TEMP_SENSOR_COMP_MODE,2))
  {
    /* Initialization fault */
    GLCD_TextSetPos(0,0,Layer1);
    GLCD_print(Layer1,"STCN75 init fault!");
    while(1);
  }

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
      if(TempSensorGetTemp(&Temp,&Alarm))
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
