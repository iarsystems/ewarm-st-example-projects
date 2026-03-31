*** This README.TXT file comes with the STR71x Software Library ***

*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Example description
===================
 This example demonstrates how to use the ADC12 in single channel conversion mode
 and switch on/off a led connected to Port 1 according to the conversion result 
 using the ADC12 interrupt service routine.
 Use the board varistor (mounted on the STR711/12 IAR board) to modify the channel 0
 analog input value.

 This example demonstrates :

  - The conversion mode and the channel selection,
  - The prescaler configuration,
  - The use of the converter in single channel conversion mode,
  - ADC12 interrupt configuration,
  - ADC12 interrupt handling.

 This example is split into 4 parts:

1) Analog input configuration

   - Configure the used analog input (channel 0)to High impedance Analog input.

2) EIC configuration

   - ADC12 channel priority configuration,
   - Enable ADC12 channel interrupt,
   - Enable Interrupt.

3) Converter configuration

  - Initialize the converter,
  - Configure the prescaler register,
  - Configure the conversion mode,
  - Select the channel to be converted,
  - Enable interrupts,
  - Start the convertion.

4) Get the convertion result and switch on/off a led in the ADC12_IRQHandler in 71x_it.c


Hardware implementation:
=======================
 This example works as standalone on the STR711/12 IAR board  and
 extra hardware implementation is not required to run the example.

Directory contents
==================

 71x_conf.h  Library Configuration file
 71x_it.c    Interrupt handlers
 main.c      Main program

How to use it
=============
 In order to make the program work, you must do the following:

  - Just run it.


(C) COPYRIGHT 2003 STMicroelectronics
