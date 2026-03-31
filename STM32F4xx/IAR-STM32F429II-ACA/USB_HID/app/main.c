/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2014
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : October, 2014
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 * DESCRIPTION
 * ===========
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR-STM32F429II-ACA board. It shows basic
 * use of the parallel I/O, the timer, the interrupt, the ADC and the USB HS
 * controllers.
 *  Connect the board to the PC through the USB-OTG port and use the BUT1,
 * BUT2,BUT3 and the AN_TR to control the mouse pointer on the PC screen.
 * The LED1 will show the USB Device connect status.
 *
 * COMPATIBILITY
 * =============
 *  The USB HID example project is compatible with IAR-STM32F429II-ACA evaluation
 * board. By default, the project is configured to use the I-Jet JTAG/SWD
 * interface.
 *
 * CONFIGURATION
 * =============
 * Flash Debug - The Progam is loaded to internal Flash.
 *
 *  Make sure that the following jumpers are correctly configured on the
 * IAR-STM32F429II-ACA evaluation board:
 *
 *  Jumpers:
 *   B0_1/B0_0  - B0_0
 *   B1_1/B1_0  - B1_0
 *   LEDS_PWR_E - closed
 *   TRIM_E     - closed
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#define DELAY_10MS_RELOAD     100

/* variable for critical section entry control */
uint32_t CriticalSecCntr;

/* tick handler variables */
static volatile Boolean  flTick10ms;
static uint32_t          TimingDelay10ms;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#pragma data_alignment=4
#endif
USB_OTG_CORE_HANDLE  USB_OTG_dev;

#define MOUSE_DELTA           4

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
  if (TimingDelay10ms) TimingDelay10ms--;
  else {
    TimingDelay10ms = DELAY_10MS_RELOAD;
    flTick10ms = TRUE;
  }
}

/*************************************************************************
 * Function Name: USB_Mouse
 * Parameters: none
 *
 * Return: none
 *
 * Description: Reads buttons and AN_TR states and sends HID reports to Host
 *
 *************************************************************************/
void USB_Mouse(void)
{
ADC_CommonInitTypeDef ADC_CommonInitStruct;
uint8_t buf[4] = {0, 0, 0, 0};

#define Buttons buf[0]
#define X       buf[1]
#define Y       buf[2]

uint8_t ButtonsHold = 0;
int32_t TrimHold, TrimVal;

  ENTR_CRT_SECTION();

  /* SysTick Config */
  if(SysTick_Config(SystemCoreClock/10000))
  {
    /* Capture error */
    while (1);
  }

  EXT_CRT_SECTION();

  /* Enable periph clock */
  RCC_APB2PeriphClockCmd(TRIMER_PERIPH_CLK, ENABLE);

  /* ADC Common init */
  ADC_CommonStructInit(&ADC_CommonInitStruct);
  ADC_CommonInit(&ADC_CommonInitStruct);

  /* ADC2 regular channel (AN_TR) configuration */
  ADC_RegularChannelConfig(TRIMER_ADC, TRIMER_CHANNEL, 1, ADC_SampleTime_56Cycles);

  /* Disable ADC DMA */
  ADC_DMACmd(TRIMER_ADC, DISABLE);

  /* Enable ADC */
  ADC_Cmd(TRIMER_ADC, ENABLE);

  /* Start ADC Software Conversion */
  ADC_SoftwareStartConv(TRIMER_ADC);

  /* Wait for conversion completion */
  while(RESET == ADC_GetFlagStatus(TRIMER_ADC, ADC_FLAG_EOC));

  /* Read and store TRIM value */
  TrimHold = ADC_GetConversionValue(TRIMER_ADC)>>2;

  /* Start ADC Software Conversion again */
  ADC_SoftwareStartConv(TRIMER_ADC);

  while(1)
  {
    if(flTick10ms)
    {
      flTick10ms = FALSE;

      /* Set X-axis state */
      if(Bit_RESET == STM_ButtonGetState(BUT2))
      {
        X += MOUSE_DELTA;
      }
      else if(Bit_RESET == STM_ButtonGetState(BUT1))
      {
        X -= MOUSE_DELTA;
      }

      /* Check for ADC completion */
      if(SET == ADC_GetFlagStatus(TRIMER_ADC, ADC_FLAG_EOC))
      {
        /* Read TRIM channel of ADC */
        TrimVal = ADC_GetConversionValue(TRIMER_ADC)>>2;
        /* Set Y-axis state */
        Y = TrimVal - TrimHold;
        TrimHold = TrimVal;
        /* Start conversion again */
        ADC_SoftwareStartConv(TRIMER_ADC);
      }

      /* Set button state */
      if(Bit_RESET == STM_ButtonGetState(BUT3))
      {
        Buttons |= 1;
      }
      else
      {
        Buttons &= ~1;
      }

      /* If there is position change or button pressed */
      if(Y || X  || Buttons || (ButtonsHold ^ Buttons))
      {
        /* Send report */
        USBD_HID_SendReport (&USB_OTG_dev,
                             buf,
                             4);
        /* Store button state */
        ButtonsHold = Buttons;
        Y = X = Buttons = 0;
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
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  /* Init + turn off LEDs */
  for(Led_TypeDef i = LED1; i <= LED8; i++ )
  {
    STM_LEDInit(i);
    STM_LEDOff(i);
  }

  /* Init buttons */
  STM_ButtonInit(BUT1, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUT2, BUTTON_MODE_GPIO);
  STM_ButtonInit(BUT3, BUTTON_MODE_GPIO);

  /* TRIM init */
  STM_TrimInit();

  /* USB Device init */
  USBD_Init(&USB_OTG_dev,
            USB_OTG_HS_CORE_ID,
            &HID_Mouse_descriptor,
            &USBD_HID_cb,
            &USR_cb);

  /* Start USB Mouse */
  USB_Mouse();
}
