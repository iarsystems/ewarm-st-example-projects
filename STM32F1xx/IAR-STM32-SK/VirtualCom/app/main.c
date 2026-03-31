/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 19, July 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench
 * for ARM to develop code for the IAR STM32-SK board.
 *  It implements USB CDC (Communication Device Class) device and installs
 * it like a Virtual COM port. UART3 is used for physical implementation
 * of the RS232 port.
 *
 *  Jumpers:
 *   PWR_SEL - depending of power source
 *
 *    $Revision: 1475 $
 **************************************************************************/
#include "includes.h"

#define UART  UART_3

#define DLY_100US  450

Int32U CriticalSecCntr;

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
Int8U Buffer[100];
pInt8U pBuffer;
Int32U Size,TranSize;
Boolean UpdateLineState = TRUE;

#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
CDC_LineCoding_t CDC_LineCoding;
USART_InitTypeDef UartLineCoding;
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0
#if CDC_DEVICE_SUPPORT_LINE_STATE > 0
UartLineEvents_t      UartLineEvents;
SerialState_t   SerialState = {0};
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0

  ENTR_CRT_SECTION();

  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  // NVIC init
#ifndef  EMB_FLASH
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  // Enable timer counting
  TIM_Cmd(TIM1,ENABLE);

  // Init UART 3
  UartInit(UART,3);

  // CDC USB
  UsbCdcInit();

  // Soft connection enable
  USB_ConnectRes(TRUE);

  EXT_CRT_SECTION();

  // LCD Init
  HD44780_PowerUpInit();

  // Backlight On
  LCD_LIGHT_ON();
  // Show messages on LCD
  HD44780_StrShow(1, 1,  "  IAR Systems   ");
  HD44780_StrShow(1, 2,  "Virtual COM Port");

  SerialState.bRxCarrier = 1;
  SerialState.bTxCarrier = 1;
  while(1)
  {
    if (IsUsbCdcConfigure())
    {
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
          TranSize = UartWrite(UART,pBuffer,Size);
        }
        while(Size != TranSize);
      }

      // Data from UART
      Size = UartRead(UART,Buffer,sizeof(Buffer)-1);
      if(Size)
      {
    #ifdef DATA_LOGGING
        Buffer[Size] = 0;
        printf("< %s\n",Buffer);
    #endif  // DATA_LOGGING
        while(!UsbCdcWrite(Buffer,Size));
      }
      // Get line and modem events from UART
    #if CDC_DEVICE_SUPPORT_LINE_STATE > 0
      // Get line events - BI, FE, PE, OE
      UartLineEvents = UartGetUartLineEvents(UART);
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
    // UART line coding - Baud rate, number of the stop bits,
    // number of bits of the data word and parity type
    #if CDC_DEVICE_SUPPORT_LINE_CODING > 0
      if(UsbCdcIsNewLineCodingSettings())
      {
        CDC_LineCoding = UsbCdcGetLineCodingSettings();
        // Update the baud rate
        UartLineCoding.USART_BaudRate = CDC_LineCoding.dwDTERate;
        // Update the stop bits number
        switch(CDC_LineCoding.bCharFormat)
        {
        case 0:
          UartLineCoding.USART_StopBits = USART_StopBits_1;
          break;
        case 1:
          UartLineCoding.USART_StopBits = USART_StopBits_1_5;
          break;
        case 2:
          UartLineCoding.USART_StopBits = USART_StopBits_2;
          break;
        }
        // Update the parity type
        switch(CDC_LineCoding.bParityType)
        {
        case ODDPARITY:
          UartLineCoding.USART_Parity = USART_Parity_Odd;
          break;
        case EVENPARITY:
          UartLineCoding.USART_Parity = USART_Parity_Even;
          break;
        default:
          UartLineCoding.USART_Parity = USART_Parity_No;
        }
        // Update the word width
        UartLineCoding.USART_WordLength = USART_WordLength_8b;
        // Set UART line coding
        UartSetLineCoding(UART,UartLineCoding);
      }
    #endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0
      // Get line and modem events from USB
    #if CDC_DEVICE_SUPPORT_BREAK > 0
      // Break event
      UartSetUartLineState(UART,UsbCdcGetBreakState());
    #endif // CDC_DEVICE_SUPPORT_BREAK > 0
    }
  }
}
