*** This README.TXT file comes with the STR71x Software Library ***

*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************

Example description
===================
 This example shows how to use the UART in order to communicate between the 
 STR71x and a PC using RS232 protocol.
 The main program configures the UART0 to generate an interrupt after a byte
 reception. The received byte will be read in the UART0 interrupt service routine to be
 transmitted back to the Host PC.
 The communication is performed using the «Hyperterminal» application under Windows.

 It is made of 3 parts :

   1) Analog input configuration,

   2) UART configuration,

   2) EIC configuration.
   
   
Hardware implementation:
=======================
 Just connect the demo board using a serial link to a PC using RS232_1(UART0)
 on the STR711/12 IAR board

 Directory contents
 ==================
 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program

How to use it
=============
In order to make the program work, you must do the following :
 
- Open «Hyperterminal» application and configure the right COM port as following :

  * 9600 bps
  * 8 data bit
  * no parity
  * 1 Stop_bit
  * No flow control
  
- Use the Jumpers RX01 and T01 to choose the UART0
- Connect a PC serial port to the UART0
- Run the program

Press any key and if the program work properly you will see the same character
in the «Hyperterminal» application windows. If you stop the program execution,the
    characters won't be displayed.

(C) COPYRIGHT 2003 STMicroelectronics
