/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 27/10/2003
* Description        : USB Mouse demo main file
********************************************************************************/
#include <stdio.h>

#include "71x_lib.h"

#include "USB_lib.h"
#include "USB_conf.h"
#include "USB_prop.h"
#include "USB_pwr.h"


// UART pin numbers depending on the chip package
#define UART0_Rx_Pin (1<<8)   // TQFP 64: pin N° 63 , TQFP 144 pin N° 143
#define UART0_Tx_Pin (1<<9)   // TQFP 64: pin N° 64 , TQFP 144 pin N° 144

#define UART1_Rx_Pin (1<<10)  // TQFP 64: pin N° 1  , TQFP 144 pin N° 1
#define UART1_Tx_Pin (1<<11)  // TQFP 64: pin N° 2  , TQFP 144 pin N° 3

#define UART2_Rx_Pin (1<<13)  // TQFP 64: pin N° 5  , TQFP 144 pin N° 9
#define UART2_Tx_Pin (1<<14)  // TQFP 64: pin N° 6  , TQFP 144 pin N° 10

#define UART3_Rx_Pin (1<<1)   // TQFP 64: pin N° 52 , TQFP 144 pin N° 123
#define UART3_Tx_Pin (1<<0)   // TQFP 64: pin N° 53 , TQFP 144 pin N° 124


// select which UART to use
#ifdef USE_UART0
  #define UARTx         UART0
  #define UARTx_Rx_Pin  UART0_Rx_Pin
  #define UARTx_Tx_Pin  UART0_Tx_Pin
#endif

#ifdef USE_UART1
  #define UARTx         UART1
  #define UARTx_Rx_Pin  UART1_Rx_Pin
  #define UARTx_Tx_Pin  UART1_Tx_Pin
#endif

#ifdef USE_UART2
  #define UARTx         UART2
  #define UARTx_Rx_Pin  UART2_Rx_Pin
  #define UARTx_Tx_Pin  UART2_Tx_Pin
#endif

#ifdef USE_UART3
  #define UARTx         UART3
  #define UARTx_Rx_Pin  UART3_Rx_Pin
  #define UARTx_Tx_Pin  UART3_Tx_Pin
#endif

#ifndef UARTx
  #error You must define USE_UART{0..3} in 71x_conf.h
#endif


// directions of transfer for CopyBuffer
#define USR_TO_PMA 0
#define PMA_TO_USR 1

// key codes for direction
#define KEY_LEFT	'A'
#define KEY_RIGHT	'D'
#define KEY_UP		'W'
#define KEY_DOWN	'X'

#define KEY_left	'a'
#define KEY_right	'd'
#define KEY_up		'w'
#define KEY_down	'x'

// cursor step
#define CURSOR_STEP	5

extern int getRx1Char(unsigned char *);

extern BOOL fCellSuspended;


/*******************************************************************************
* Function Name : gInit
* Description   : Hardware initialisation
* Input         : None
* Output        : None
* Return value  : None
*******************************************************************************/
void gInit(void)
{
  wInterrupt_Mask = IMR_MSK;

} /* gInit */


/*******************************************************************************
* Function Name : CopyBuffer
* Description   : Transfers a packet of data from/to PMA to/from user memory
* Input 1       : iTrDirection: USR_TO_PMA or PMA_TO_USR
* Input 2       : EpNum: endpoint number
* Input 3       : Usr_buffer: a pointer to user memory
* Input 4       : Nbytes: number of bytes to transfer
* Output        : None
* Return value  : None
*******************************************************************************/
void CopyBuffer(int iTrDirection, BYTE EpNum, BYTE *Usr_buffer, int Nbytes)
{
  DWORD *pTxBuff;
  WORD wTra;
  BYTE *pbTra;
  int i;

  if (iTrDirection == USR_TO_PMA)
  {
    pTxBuff = (DWORD *)(PMAAddr + (BYTE *)(GetEPTxAddr(EpNum)*2));
   	for(i = 0; i < Nbytes;)
   	{
	   	pbTra = (BYTE *)&wTra;
   		*pbTra++ = *Usr_buffer++;
   		i++;
   		if (i < Nbytes) /* check for ODD transfers */
	   		*pbTra = *Usr_buffer++;
	   	else
	   		*pbTra = 0;
   		*pTxBuff++ = wTra;
	   	i++;
	}
  }

} /* CopyBuffer */


/*******************************************************************************
* Function Name : Mouse_Send
* Description   : Decodes commands arriving from serial and
*                 prepares buffer to be sent containing mouse event infos
* Input         : Keys: keys received from terminal
* Output        : None
* Return value  : None
*******************************************************************************/
void Mouse_Send(BYTE Keys)
{
  BYTE Mouse_Buffer[3] = {0,0,0};
  char X = 0, Y = 0;

  switch (Keys)
  {
	case KEY_LEFT:
	case KEY_left:
	  X -= CURSOR_STEP;
	  break;
	case KEY_RIGHT:
	case KEY_right:
	  X += CURSOR_STEP;
	  break;
	case KEY_UP:
	case KEY_up:
	  Y -= CURSOR_STEP;
	  break;
	case KEY_DOWN:
	case KEY_down:
	  Y += CURSOR_STEP;
	  break;
	default:
	  return;
  }

  /* prepare buffer to send */
  Mouse_Buffer[1] = X;
  Mouse_Buffer[2] = Y;
  CopyBuffer(USR_TO_PMA, ENDP1, Mouse_Buffer, 3);

  /* enable endpoint for transmission */
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxCount(ENDP1, 4);
  SetEPTxValid(ENDP1);

} /* Mouse_Send */


/*******************************************************************************
* Function Name : Keys_Read
* Description   : Checks if a new character from terminal has arrived
* Input         : None
* Output        : None
* Return value  : None
*******************************************************************************/
void Keys_Read(void)
{
  u8 cRcvd;
  int UART_STATUS;


// Disable the Reception mode
  UART_RxConfig(UARTx, DISABLE);

  while (!(UART_FlagStatus(UARTx) & UART_TxEmpty));

  UART_FifoReset(UARTx, UART_TxFIFO);   // Reset the TxFIFO

  UART_RxConfig(UARTx, ENABLE);   // Enable Rx

  UART_STATUS = UART_DataReceive(UARTx, &cRcvd, 1, 0xFF);

  UART_RxConfig(UARTx, DISABLE);  // Disable Rx*/

  UART_STATUS = !(UART_STATUS & UART_TimeOutIdle);
  if (UART_STATUS)
  {
    //sprintf(string, "%",cRcvd);
   UART_ByteSend(UART0,&cRcvd);

//    sendchar(&cRcvd); /* just to see echo on the terminal */

    if (fCellSuspended)
      Resume(RESUME_INTERNAL);	// remote wake-up
    else
      Mouse_Send((BYTE)cRcvd);
  }

} /* Keys_Read */


/*===========================================================================*/
/* MAIN program */
/*===========================================================================*/
int i;
int main(void)
{

  #ifdef DEBUG
  debug();
  #endif
   // Configure MCLK = RCLK
  RCCU_MCLKConfig ( RCCU_DEFAULT );

  // Configure FCLK = RCLK /2
  RCCU_FCLKConfig ( RCCU_DEFAULT/*RCCU_RCLK_4*/);

   //Configure PCLK = RCLK /4
  RCCU_PCLKConfig ( RCCU_DEFAULT );

  // Configure the PLL1 ( * 16 , / 2 )
  //RCCU_PLL1Config ( RCCU_Mul_16 , RCCU_Div_4 ) ;

  RCCU_PLL1Config ( RCCU_Mul_16 , RCCU_Div_1 ) ;


  // Wait PLL to lock
   //while(RCCU_FlagStatus(RCCU_PLL1_LOCK)==RESET);

   for ( i = 0x7f ; i> 0; i--)

  // Select PLL1_Output as RCLK clock
  RCCU_RCLKSourceConfig ( RCCU_PLL1_Output ) ;
  /* Configure XTI_Line2 as a falling edge external interrupt to use
  for remote wake up test */
  XTI_Init();
  GPIO_Config(GPIO2, 1<<8, GPIO_INOUT_WP);
  XTI_LineModeConfig(XTI_Line2, XTI_FallingEdge);
  XTI_LineConfig(XTI_Line2, ENABLE);
  XTI_ModeConfig(XTI_Interrupt, ENABLE);
  EIC_IRQChannelPriorityConfig(XTI_IRQChannel, 1);
  EIC_IRQChannelConfig(XTI_IRQChannel, ENABLE);

  gInit();  /* init hardware */

  // UART configuration
  // Configure the GPIO pins as alternate function push-pull
  GPIO_Config(GPIO0, UARTx_Rx_Pin | UARTx_Tx_Pin, GPIO_AF_PP);
  UART_OnOffConfig(UARTx, ENABLE);   // Turn UART on
  UART_FifoConfig (UARTx, DISABLE);  // Disable FIFOs
  UART_FifoReset  (UARTx, UART_RxFIFO);   // Reset the RxFIFO
  UART_FifoReset  (UARTx, UART_TxFIFO);   // Reset the TxFIFO
  UART_LoopBackConfig(UARTx, DISABLE);  // Disable Loop Back
  UART_RxConfig(UARTx, ENABLE);   // Enable Rx
  UART_RxConfig(UARTx, DISABLE);  // Disable Rx
  /* Configure the UART as following:
     - Baudrate = 9600 bps
     - No parity
     - 8 data bits
     - 1 stop bit */
  UART_Config(UARTx, 9600, UART_NO_PARITY, UART_1_StopBits, UARTM_8D);
  UART_OnOffConfig(UARTx, ENABLE);

  UART_StringSend(UART0, "\n\rSTR71x HID Mouse demo\n\r");


  USB_Init();
  UART_RxConfig(UARTx, DISABLE);   // Disable Rx
  UART_StringSend(UART0,"USB configured\n\r");
  UART_StringSend(UART0, "'A' :Left\n\r");
  UART_StringSend(UART0, "'D' :Right\n\r");
  UART_StringSend(UART0, "'W' :Up\n\r");
  UART_StringSend(UART0, "'X' :Down\n\r");


  /* Main loop */
  while(1) {
    /* Put user defined main loop procedures here */
    Keys_Read();
  }
}
