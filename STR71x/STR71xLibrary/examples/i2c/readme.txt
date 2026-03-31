*** This README.TXT file comes with the STARM71x Software Library ***

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
 This example provides a basic example showing how to use the I2C software library
 to communicate with an I2C EEPROM.

 The associated software performs a write operation to the I2C EEPROM
 followed by a read operation of the data already stored in the EEPROM.

 The example is made of 4 main parts :
  1)  I2C peripheral configuration.
  2)  Write operation to the I2C EEPROM.
  3)  Wait until the I2C EEPROM write operation is completed.
  4)  Read and display, in the debugger output window, the data already stored
      in the I2C EEPROM.

Hardware implementation:
=======================
The user has to connect an I2C eeprom to I2C as follow:
- Connect EEPROM SDA line to the MICRO SDA line(SDA0 on the board)
- Connect EEPROM SCL line to the MICRO SCL line(SCL0 on the board)
- Connect address lines
- Connect EEPROM Vdd pin to VCC
- Connect EEPROM Vss pin to GND

How to use it
=============
 In order to make the program work, you must do the following :

    
  - Just  run it.


(C) COPYRIGHT 2004 STMicroelectronics
