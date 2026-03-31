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
 This example shows how to use  the ADC12  in single channel conversion mode. 
 The ADC12_Calib function used to perform the  ADC12 result calibration.
 The calibrated conversion result is stored in the "value" variable which can
 be monitored in the watch window.


 This example demonstrates the following features of the ADC12 :

  - The use of the converter in the single channel conversion mode,
  - The prescaler configuration.  


 It is made of 4 parts :

  1) Analog input configuration
    - Configure the used analog input to high impedance Analog input

  2) Converter configuration
    - Initialize the converter,
    - Configure the prescaler register,
    - Configure the conversion mode,
    - Select the channel to be converted,
    - Start the conversion.

  3) Get the conversion Data
    - Wait until conversion is finished,
    - Read the conversion result,
    - Calibrate the conversion result (using the "ADC12_Calib" function).    

 4) 
  - Change the  Red Led luminosity according to ADC12 conversion value using the potentiometer. 
  - Press The But Button to blink the Led.

Hardware implementation:
=======================
 This example works as standalone on the STR711/STR712 IAR boards 


How to use it
=============
 In order to make the program work, you must do the following :

   - put the jumper DIS/LED on the position 1-2(only for STR711 IAR board)
   - run program

(C) COPYRIGHT 2003 STMicroelectronics
