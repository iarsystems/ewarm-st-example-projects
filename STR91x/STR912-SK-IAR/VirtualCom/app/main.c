/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : October 2, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR STR912-SK evaluation boards. It implements USB
 * CDC (Communication Device Class) device and install it like a Virtual COM
 * port. The UART0 is used for physical implementation of the RS232 port.
 *
 * Jumpers:
 *	POWER_SELECT    - depend of power source
 *	USB_DIS         - 2-3
 *	RST-TRST        - Absence
 *	Jumper array J5 - 2-3
 *	Jumper array J16 and J15 - 2-3 (near prototype area) when jtrace is used
 *
 *    $Revision: 39 $
 *
 **************************************************************************/
#include    "includes.h"
#include    "91x_conf.h"
#include    "91x_lib.h"


volatile Int32U DlyCount;
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
  // Clear TIM0 counter
  TIM_CounterCmd(TIM0, TIM_CLEAR);
  if(DlyCount)
  {
    --DlyCount;
  }
  // Clear TIM0 flag OC1
  TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
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
 * Function Name: DelayResolution100us
 * Parameters: Int32U Delay
 *
 * Return: none
 *
 * Description: Delay 100us * arg
 *
 *************************************************************************/
void DelayResolution100us(Int32U Delay)
{
  DlyCount = Delay;

  // Clear TIM0 counter
  TIM_CounterCmd(TIM0, TIM_CLEAR);
  // Clear TIM0 flag OC1
  TIM_ClearFlag(TIM0,TIM_FLAG_OC1);
  // Enable TIM0 OC1 interrupt
  TIM_ITConfig(TIM0, TIM_IT_OC1, ENABLE);
  // Enable TIM0 counter
  TIM_CounterCmd(TIM0, TIM_START);

  while(DlyCount);

  // Disable TIM0 OC1 interrupt
  TIM_ITConfig(TIM0, TIM_IT_OC1, DISABLE);
  // Disable TIM0 counter
  TIM_CounterCmd(TIM0, TIM_STOP);
}

/*************************************************************************
 * Function Name: InitDlyTimer
 * Parameters: Int32U IntrPriority
 *
 * Return: none
 *
 * Description: Init Delay Timer (TIM 0)
 *
 *************************************************************************/
void InitDlyTimer (Int32U IntrPriority)
{
TIM_InitTypeDef TIM_InitStructure;

  // Enable TIM0 clocks
  SCU_APBPeriphClockConfig(__TIM01, ENABLE);
  // Release TIM0 reset
  SCU_APBPeriphReset(__TIM01,DISABLE);

  // Timer 0
  // TIM Configuration in Output Compare Timing Mode period 100us
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;   // OUTPUT COMPARE CHANNEL 1 Mode
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;     // OCMP1 pin is disabled
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB; // assign PCLK to TIM_Clk
  TIM_InitStructure.TIM_Prescaler = 48 - 1;         // 1us resolution
  TIM_InitStructure.TIM_Pulse_Length_1 = 100;       // 100 us period
  TIM_Init(TIM0, &TIM_InitStructure);

  // VIC configuration
  VIC_Config(TIM0_ITLine, VIC_IRQ, IntrPriority);
  VIC_ITCmd(TIM0_ITLine, ENABLE);
}


/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
int main (void)
{
Int8U Buffer[100];
pInt8U pBuffer;
Int32U Size,TranSize;
Boolean UpdateLineState = TRUE;
Boolean CdcConfigureStateHold;


#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
CDC_LineCoding_t CDC_LineCoding;
UART_InitTypeDef UartLineCoding;
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0

#if CDC_DEVICE_SUPPORT_LINE_STATE > 0

UartLineEvents_t      UartLineEvents;

SerialState_t   SerialState = {0};
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0

  InitClock();
  // Enable the clock for the VIC
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);
  // VIC Deinitialization
  VIC_DeInit();
  // Delay timer init
  InitDlyTimer(1);
  // Init UART 0
  UartInit(UART_0,3);

  // Initialize USB
  // Init USB
  USB_Init(4,5,UsbCdcConfigure);
  // Init CDC
  UsbCdcInit(2);
  // Soft connection enable
  USB_ConnectRes(TRUE);

  // Enable the Interrupt controller to manage IRQ channel
  __enable_interrupt();

  // LCD Powerup init
  HD44780_PowerUpInit();
  // Backlight On
  LCD_LIGHT_ON();
  // Show messages on LCD
  HD44780_StrShow(1, 1,  "  IAR Systems   ");

  SerialState.bRxCarrier = 1;
  SerialState.bTxCarrier = 1;

  CdcConfigureStateHold = !UsbCdcIsCdcConfigure();

  while(1)
  {
    if (UsbCdcIsCdcConfigure())
    {
      if(CdcConfigureStateHold == FALSE)
      {
        HD44780_StrShow(1, 2,  "Virtual COM Port");
        CdcConfigureStateHold = TRUE;
      }
      // Data from USB
      Size = UsbCdcRead(Buffer,sizeof(Buffer)-1);
      if(Size)
      {
#ifdef DATA_LOGGING
        Buffer[Size] = 0;
        printf("> %s\n",Buffer);
#endif // DATA_LOGGING
        TranSize = 0;
        pBuffer = Buffer;
        do
        {
          Size -= TranSize;
          pBuffer += TranSize;
          TranSize = UartWrite(UART_0,pBuffer,Size);
        }
        while(Size != TranSize);
      }

      // Data from UART0
      Size = UartRead(UART_0,Buffer,sizeof(Buffer)-1);
      if(Size)
      {
#ifdef DATA_LOGGING
        Buffer[Size] = 0;
        printf("< %s\n",Buffer);
#endif  // DATA_LOGGING
        while(!UsbCdcWrite(Buffer,Size));
      }

      // Get line and modem events from UART 0
#if CDC_DEVICE_SUPPORT_LINE_STATE > 0
      // Get line events - BI, FE, PE, OE
      UartLineEvents = UartGetUartLineEvents(UART_0);
      if(UartLineEvents.Data | UpdateLineState)
      {
        UpdateLineState = FALSE;
        // Line events report BI, PE, FE and OE
        SerialState.bBreak = UartLineEvents.bBI;
        SerialState.bFraming = UartLineEvents.bFE;
        SerialState.bOverRun = UartLineEvents.bOE;
        SerialState.bParity = UartLineEvents.bPE;
        // Send events
        UsbCdcReportSerialCommState(SerialState);
      }
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0

    }
    else
    {
      if(CdcConfigureStateHold == TRUE)
      {
        HD44780_StrShow(1, 2,  " Put USB cable  ");
        CdcConfigureStateHold = FALSE;
      }
    }
    // UART0 line coding - Baud rate, number of the stop bits,
    // number of bits of the data word and parity type
#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
    if(UsbCdcIsNewLineCodingSettings())
    {
      CDC_LineCoding = UsbCdcGetLineCodingSettings();
      // Update the baud rate
      UartLineCoding.UART_BaudRate = CDC_LineCoding.dwDTERate;
      // Update the stop bits number
      UartLineCoding.UART_StopBits = CDC_LineCoding.bCharFormat?UART_StopBits_2:UART_StopBits_1;
      // Update the parity type
      switch(CDC_LineCoding.bParityType)
      {
      case ODDPARITY:
        UartLineCoding.UART_Parity = UART_Parity_Odd;
        break;
      case EVENPARITY:
        UartLineCoding.UART_Parity = UART_Parity_Even;
        break;
      case MARKPARITY:
        UartLineCoding.UART_Parity = UART_Parity_EvenStick;
        break;
      case SPACEPARITY:
        UartLineCoding.UART_Parity = UART_Parity_OddStick;
        break;
      default:
        UartLineCoding.UART_Parity = UART_Parity_No;
      }
      // Update the word width
      switch( CDC_LineCoding.bDataBits)
      {
      case 5:
        UartLineCoding.UART_WordLength = UART_WordLength_5D;
        break;
      case 6:
        UartLineCoding.UART_WordLength = UART_WordLength_6D;
        break;
      case 7:
        UartLineCoding.UART_WordLength = UART_WordLength_7D;
        break;
      default:
        UartLineCoding.UART_WordLength = UART_WordLength_8D;
      }
      // Set UART line coding
      UartSetLineCoding(UART_0,UartLineCoding);
    }
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0

    // Get line and modem events from USB
#if CDC_DEVICE_SUPPORT_BREAK > 0
    // Break event
    UartSetUartLineState(UART_0,UsbCdcGetBreakState());
#endif // CDC_DEVICE_SUPPORT_BREAK > 0
  }
}
