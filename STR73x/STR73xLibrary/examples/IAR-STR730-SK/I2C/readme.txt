/******************** (C) COPYRIGHT 2005 STMicroelectronics **********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 09/27/2005 :  V1.0
* Description        : Description of the  I²C example
**********************************************************************************
* History:
* 09/27/2005 :  V1.0
*********************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************

Example description
===================
This example demonstrates the following features of the I2C :
- Start a communication between the two STR73x I2C interfaces (I2C0 and I2C1)
      + Using the I2C cell (I2C0) as master in the 7 bit addressing mode
      + Using the I2C cell (I2C1) as slave receiver in the 7 bit addressing mode
- Use the I2C1 in the interrupt mode

Steps:
1) Pin configuration
   Configure the SDA and the SCL lines of the I2C0 and the I2C1 to alternate function
   Open Drain (using the GPIO library).
   
2) Interrupt configuration
   Enable the interrupt genertion in the case of the end of transmission/reception
   
   
3) I2C0 and I2C1 configuration
   Initialize I2C registers to:
   - configure I2C0 and I2C1 speeds
   - configure I2C0 and I2C1 interfaces address
   - enable the acknowledge for I2C0 and I2C1
   - enable the interrupt generation for I2C1
   - enable the I2C0 and the I2C1
   
4) Set I2C0 as Master transmitter and I2C1 as Slave receiver
   - Send the I2C0_Buffer_Tx to I2C1 then compare it with I2C1_Buffer_Rx received 
     buffer and store
   the result in "TransferStatus1" variable. 

5) Set I2C1 as Master transmitter and I2C0 as Slave receiver
   - Send the I2C1_Buffer_Tx to I2C0 then compare it with I2C0_Buffer_Rx received
      buffer and store
   the result in "TransferStatus2" variable. 
   
Directory contents
==================
73x_conf.h  Library Configuration file
73x_it.c    Interrupt handlers
main.c      Main program

How to use it
=============
In order to make the program work, you must do the following :

- Connect the two I2C SCL lines: SCL0 pin (P2.14) and SCL1 pin (P4.6) .
- Connect the two I2C SDA lines: SDA0 pin (P2.15) and SDA1 pin (P4.7) .
- Run the example

- The associated software performs a buffer write from I2C0 to I2C1 followed by 
a write of a buffer from I2C1 to I2C0.
The data read from I2C1 stored in I2C1_Buffer_Rx is compared with the expected 
values of the I2C0_Buffer_Tx.The result of this comparison is stored in the 
"TransferStatus1" variable.
The data read from I2C0 stored in I2C0_Buffer_Rx is compared with the expected 
values of the I2C1_Buffer_Tx.The result of this comparison is stored in the 
"TransferStatus2" variable.

******************* (C) COPYRIGHT 2005 STMicroelectronics *****END OF FILE****
