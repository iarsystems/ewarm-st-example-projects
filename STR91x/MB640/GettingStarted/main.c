/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        :
 *       Author      : Stanimir Bonev
 *       Description : October 5 2006
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the STR910 Evaluation board. It shows basic use of I/O,
 * timer and the interrupt controllers. It starts by blinking LD2, LD3, LD4 and
 * LD5. The Left and right directions of joystick change blinking speed and
 * select change light pattern of the leds.
 *  The jumpers settings are regarding STR910 Evaluation board user manual.
 *
 * Buttons:
 *  Left direction of the joystick  - decrease speed
 *  Right direction of the joystick - increase speed
 *  Select of the joystick          - change pattern
 *
 *    $Revision: 39 $
 **************************************************************************/
#include <intrinsics.h>
#include <stdio.h>

#include "91x_conf.h"
#include "91x_lib.h"

// Timer Ticks per second
#define TICK_PER_SEC    (10UL)

// Timer Delta period [ms]
#define DELTA_PER       (50UL)

#define TIM1_MAX_PER    (1000/ 5)
#define TIM1_MIN_PER    (1000/20)

// Leds bits definitions
#define LED_LD2         GPIO_Pin_0
#define LED_LD3         GPIO_Pin_1
#define LED_LD4         GPIO_Pin_2
#define LED_LD5         GPIO_Pin_3

// Joystick definitions
#define JS_LEFT         (2UL)
#define JS_RIGHT        (3UL)
#define JS_UP           (4UL)
#define JS_DOWN         (1UL)
#define JS_SELECT       (0UL)

#define JS_INTR         GPIO_Pin_4
#define JS_A0           GPIO_Pin_5
#define JS_A1           GPIO_Pin_6
#define JS_A2           GPIO_Pin_7

typedef struct LedPattern_t
{
  unsigned int Pattern;
  struct LedPattern_t const * pNextPattern;
}LedPattern_t;

const LedPattern_t LedPatterns10,LedPatterns11;
const LedPattern_t LedPatterns12,LedPatterns13;

const LedPattern_t LedPatterns20,LedPatterns21;
const LedPattern_t LedPatterns22,LedPatterns23;
const LedPattern_t LedPatterns24,LedPatterns25;

const LedPattern_t LedPatterns30,LedPatterns31;
const LedPattern_t LedPatterns32,LedPatterns33;
const LedPattern_t LedPatterns34,LedPatterns35;
const LedPattern_t LedPatterns36,LedPatterns37;

const LedPattern_t LedPatterns10 = {0x00000001,&LedPatterns11};
const LedPattern_t LedPatterns11 = {0x00000002,&LedPatterns12};
const LedPattern_t LedPatterns12 = {0x00000004,&LedPatterns13};
const LedPattern_t LedPatterns13 = {0x00000008,&LedPatterns10};

const LedPattern_t LedPatterns20 = {0x00000001,&LedPatterns21};
const LedPattern_t LedPatterns21 = {0x00000003,&LedPatterns22};
const LedPattern_t LedPatterns22 = {0x00000006,&LedPatterns23};
const LedPattern_t LedPatterns23 = {0x0000000C,&LedPatterns24};
const LedPattern_t LedPatterns24 = {0x00000008,&LedPatterns20};

const LedPattern_t LedPatterns30 = {0x00000000,&LedPatterns31};
const LedPattern_t LedPatterns31 = {0x00000001,&LedPatterns32};
const LedPattern_t LedPatterns32 = {0x00000003,&LedPatterns33};
const LedPattern_t LedPatterns33 = {0x00000007,&LedPatterns34};
const LedPattern_t LedPatterns34 = {0x0000000F,&LedPatterns35};
const LedPattern_t LedPatterns35 = {0x00000007,&LedPatterns36};
const LedPattern_t LedPatterns36 = {0x00000003,&LedPatterns37};
const LedPattern_t LedPatterns37 = {0x00000001,&LedPatterns30};

const LedPattern_t * const pLedPatterns[] =
{
  &LedPatterns10,
  &LedPatterns20,
  &LedPatterns30,
};

int PatternInd;
const LedPattern_t * pNextPattern;

volatile unsigned int Tim0Per;

/*************************************************************************
 * Function Name: Tim0Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void Tim0Handler (void)
{
unsigned int temp = Tim0Per;     // avoid warning
  TIM0->OC1R += temp;
  pNextPattern = pNextPattern->pNextPattern;
  GPIO_Write(GPIO9,~pNextPattern->Pattern);
  // Clear TIM0 flag OC1
  TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
}

/*************************************************************************
 * Function Name: JS_IntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Joystick interrupt handler
 *
 *************************************************************************/
void JS_IntrHandler (void)
{
  if(WIU_GetFlagStatus(WIU_Line28))
  {
    // Clear WIU line 28 pending interrupt flag
    WIU_ClearITPendingBit(WIU_Line28);
    unsigned int Key = GPIO_Read(GPIO7) >> 5;
    switch(Key)
    {
    case JS_SELECT: // change pattern
      if(++PatternInd >= sizeof(pLedPatterns)/sizeof(LedPattern_t *))
      {
        PatternInd = 0;
      }
      pNextPattern = pLedPatterns[PatternInd];
      break;
    case JS_LEFT:   // decrease speed
      if(Tim0Per < TIM1_MAX_PER)
      {
        Tim0Per += DELTA_PER;
      }
      break;
    case JS_RIGHT:  // increase speed
      if(Tim0Per > TIM1_MIN_PER)
      {
        Tim0Per -= DELTA_PER;
      }
    }
  }
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init MCU clock
 *
 *************************************************************************/
void InitClock (void)
{
  // Clock
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);       // master clk - OSC clk
  // Flash controller init
  SCU_FMICLKDivisorConfig(SCU_FMICLK_Div1);
  FMI_Config(FMI_READ_WAIT_STATE_2,FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE,\
             FMI_LVD_ENABLE,FMI_FREQ_HIGH);
  // Set clks dividers
  SCU_RCLKDivisorConfig(SCU_RCLK_Div1);
  SCU_HCLKDivisorConfig(SCU_HCLK_Div1);
  SCU_PCLKDivisorConfig(SCU_PCLK_Div1);
  // Init PLL = 48 MHz
  SCU_PLLFactorsConfig(192,25,3);
  // PLL Enabled
  SCU_PLLCmd(ENABLE);
  // Switch clk MCLK = PLL
  SCU_MCLKSourceConfig(SCU_MCLK_PLL);
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

  
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_InitTypeDef TIM_InitStructure;
  WIU_InitTypeDef WIU_InitStructure;
  
  // Init Clks
  InitClock();
  // Enable the clock for the VIC
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);

  // VIC Deinitialization
  VIC_DeInit();

  // Init IO
  // Enable GPIO7, GPIO9, WIU and TIM01 clocks
  SCU_APBPeriphClockConfig(__GPIO7 | __GPIO9 | __WIU | __TIM01, ENABLE);
  // Release GPIO7, GPIO9, WIU and TIM01 reset
  SCU_APBPeriphReset(__GPIO7 | __GPIO9 | __WIU | __TIM01,DISABLE);

  // GPIO9 - GPIO
  SCU->GPIOEMI &= ~1;
  // GPIO9 Push-Pull outputs
  SCU->GPIOTYPE[9] &=  ~(LED_LD2 | LED_LD3 | LED_LD4 | LED_LD5);
  GPIO9->DDR |= LED_LD2 | LED_LD3 | LED_LD4 | LED_LD5;
  // Set Leds off (LD2 - GPIO_90, LD3 - GPIO_91, LD4 - GPIO_92, LD5 - GPIO_93)
  PatternInd = 0;
  pNextPattern = pLedPatterns[0];
  GPIO_Write(GPIO9,pLedPatterns[0]->Pattern);

  // Init Joystick
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_InitStructure.GPIO_Pin = JS_A0 | JS_A1 | JS_A2 | JS_INTR;
  GPIO_Init(GPIO7, &GPIO_InitStructure);

  //Enable the WIU & Clear the WIU line 28 pending bit 
  WIU_Cmd(ENABLE );
  WIU_ClearITPendingBit(WIU_Line28);


  WIU_InitStructure.WIU_Line = WIU_Line28 ;
  WIU_InitStructure.WIU_TriggerEdge = WIU_FallingEdge ;
  WIU_Init(&WIU_InitStructure);

  //Select WIU line 28 as VIC1.13 interrupt source 
  SCU_WakeUpLineConfig(28);
  
  
  
  
  
  // VIC configuration
  VIC_Config(EXTIT3_ITLine, VIC_IRQ, 1);
  VIC_ITCmd(EXTIT3_ITLine, ENABLE);

  // Timer 0
  SCU_APBPeriphDebugConfig(__TIM01,DISABLE);        // APB peripheral clock disabled during ARM debug state
  // TIM Configuration in Output Compare Timing Mode period 100 msec
  SCU_TIMPresConfig(SCU_TIM01,((48000-1)&~1UL));    // Timer resolution 1ms
  SCU_TIMCLKSourceConfig(SCU_TIM01,SCU_TIMCLK_INT); // Timer01 Clock form the prescaler
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_SCU; // assign Prescaler clk to TIM_Clk
  Tim0Per = 1000/TICK_PER_SEC;                      // 100 ms period
  TIM_InitStructure.TIM_Pulse_Length_1 = Tim0Per;

  TIM_Init(TIM0, &TIM_InitStructure);
  TIM_CounterCmd(TIM0, TIM_CLEAR);                  // clear the timer 1 counter

  // VIC configuration
  VIC_Config(TIM0_ITLine, VIC_IRQ, 1);
  VIC_ITCmd(TIM0_ITLine, ENABLE);

  // Clear TIM0 flag OC1
  TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
  // Enable TIM0 OC1 interrupt
  TIM_ITConfig(TIM0, TIM_IT_OC1, ENABLE);
  // Enable TIM0 counter
  TIM_CounterCmd(TIM0, TIM_START);

  __enable_interrupt();
  // main loop
  while(1)
  {};
} // main(void)
