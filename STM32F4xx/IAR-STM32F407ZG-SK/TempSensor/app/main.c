/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : June 2011
 *       Author      : Stoyan Choynev
 *       Description :
 *
 *    2. Date        : October 2011
 *       Author      : Atanas Uzunov
 *       Description : Updated to support the STM32F407ZG-SK board.
                       Added Celsius to Fahrenheit conversion, temperature
 *                     reading indicator and USER button handler.
 *
 *   This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR-STM32F407ZG-SK board. It shows basic
 * use of the parallel I/O,the SysTick timer,the interrupt controller,the interface to
 * Nokia Graphic color LCD module and the I2C temperature sensor.
 *   Measured/readed temperature is shown on the LCD display below the IAR logo. Each time the
 * temperature is measured/readed, the LED1 lights up for 100ms. User can
 * toggle the measurement units (Celsius/Fahrenheit) alternatively, by pressing USER
 * button at the board.
 *
 *  SRAM Debug - The Progam is loaded to internal RAM.
 *  Flash Debug - The Progam is loaded to internal Flash.
 *
 *  Make sure that the following jumpers are correctly configured on the
 *  IAR-STM32F407ZG-SK evaluation board:
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *   B0_1/B0_0 - B0_0
 *   B1_1/B1_0 - B1_0
 *   STAT1_E - on
 *   STAT2_E - on
 *   STAT3_E - on
 *   STAT4_E - on
 *
 *    $Revision: 1467 $
 **************************************************************************/
#include "includes.h"

#define READTEMP_RELOAD    5000  /* 0.5s  */
#define LEDOFF_RELOAD      1000  /* 0.1s  */
#define KEYBOARD_RELOAD     500  /* 0.05s */

extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

/* variable for critical section entry control */
uint32_t CriticalSecCntr;

volatile uint32_t flags0=0;

#define flTickLedOff               (flags0 &   ( 1 << 0 ))
#define flTickLedOffSet()          (flags0 |=  ( 1 << 0 ))
#define flTickLedOffClear()        (flags0 &= ~( 1 << 0 ))

#define flTickReadTemp             (flags0 &   ( 1 << 1 ))
#define flTickReadTempSet()        (flags0 |=  ( 1 << 1 ))
#define flTickReadTempClear()      (flags0 &= ~( 1 << 1 ))

#define flUserButtonChanged        (flags0 &   ( 1 << 2 ))
#define flUserButtonChangedSet()   (flags0 |=  ( 1 << 2 ))
#define flUserButtonChangedClear() (flags0 &= ~( 1 << 2 ))

#define flUpdateDisplay            (flags0 &   ( 1 << 3 ))
#define flUpdateDisplaySet()       (flags0 |=  ( 1 << 3 ))
#define flUpdateDisplayClear()     (flags0 &= ~( 1 << 3 ))

#define flFahrenheit               (flags0 &   ( 1 << 4 ))
#define flFahrenheitToggle()       (flags0 ^=  ( 1 << 4 ))

volatile uint16_t LedOffTimer=0, ReadTempTimer=0, ReadKeyboardTimer=0;
volatile uint8_t ButtonState;
volatile uint32_t TimingDelay;
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
  if (TimingDelay) TimingDelay--;
  if(++LedOffTimer >= LEDOFF_RELOAD)
  {
    flTickLedOffSet();
    LedOffTimer = 0;
  }
  if(++ReadTempTimer >= READTEMP_RELOAD)
  {
    flTickReadTempSet();
    ReadTempTimer = 0;
  }
  if(++ReadKeyboardTimer >= KEYBOARD_RELOAD)
  {
    UserButtonHandler();
    ReadKeyboardTimer = 0;
  }
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: uint32_t Dly
 *
 * Return: none
 *
 * Description: Delay = (arg * 100us); Max error: 100us
 *
 *************************************************************************/
void DelayResolution100us(uint32_t Dly)
{
  TimingDelay = Dly;

  while(TimingDelay != 0);
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
float Temp, TempHold = 200.0;
Boolean Alarm;

 /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  ENTR_CRT_SECTION();

  /* SysTick Config*/
  if(SysTick_Config(SystemCoreClock/10000))
  {
    /* Capture error */
    while (1);
  }

  /* I2C1 init */
  I2C1_Init();

  /* Status LEDS port init */
  STM_LEDInit(LED1); /* ReadTemp LED */
  STM_LEDInit(LED3); /* Alarm LED    */

  /* USER Button init */
  STM_ButtonInit(BUTTON_USER,BUTTON_MODE_GPIO);

  EXT_CRT_SECTION();

  /* GLCD init */
  GLCD_PowerUpInit((pInt8U)IAR_Logo.pPicStream);
  GLCD_Backlight(BACKLIGHT_ON);
  GLCD_SetFont(&Terminal_9_12_6,0x000F00,0x00FF0);
  GLCD_SetWindow(9,109,131,131);

  /* Init Temperature sensor */
  if(   FALSE == TempSensor_Init()
     || FALSE == TempSensorShutdown(FALSE)
     || FALSE == TempSensor_Conf(31.0,30.0,TEMP_SENOSR_COMP_MODE,2))
  {
    /* Initialization fault */
    GLCD_TextSetPos(0,0);
    GLCD_print("\fSTCN75 Init.\r\nfault\r\n");
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
      LedOffTimer = 0;
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
        GLCD_TextSetPos(0,0);
        GLCD_print("\fSTCN75 reading\r\nfault\r\n");
        while(1);
      }
    }
    else if (flTickLedOff)
    {
      flTickLedOffClear();
      /* Turns off ReadTemp LED */
      STM_LEDOff(LED1);
    }

    /* If display should be updated */
    if(flUpdateDisplay)
    {
      GLCD_TextSetPos(0,0);
      if(flFahrenheit)
      {
        /* Print temperature in Fahrenheit */
        GLCD_print("\fTemperature: %3.1f\370F",((9.0/5.0)*Temp + 32.0));
      }
      else
      {
        /* Print temperature in Celsius */
        GLCD_print("\fTemperature: %3.1f\370C",Temp);
      }
      flUpdateDisplayClear();
    }
  }
}
