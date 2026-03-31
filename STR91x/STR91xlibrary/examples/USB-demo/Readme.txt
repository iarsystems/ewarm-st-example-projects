/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Description of the USB Demo 
********************************************************************************
* History:
* 05/22/2007 : Version 1.2
* 05/24/2006 : Version 1.1
* 05/18/2006 : Version 1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Example description
===================
This example provides a description of how to use the USB on the STR91x devices.
The STR91x device is enumerated as an USB Mouse, that uses the native
PC Host USB HID driver.
Push-buttons(connected to p7.3, p7.4, p7.5 and p7.6) mounted on the STR912-SK-IAR 
board are used to emulate the Mouse directions.


Directory contents
==================

 + include

   - 91x_conf.h  Library Configuration file
   - usb_conf.h  USB Library configuration file
   - usb_desc.h  Mouse descriptor header
   - usb_pwr.h   Mouse Power header
   - usb_prop.h  Mouse Prosessing header

 + source

   - 91x_it.c    Interrupt handlers
   - main.c      Main program
   - usb_desc.c  Mouse descriptor file
   - usb_pwr.c   Mouse Power file
   - usb_prop.c  Mouse Prosessing file
   - usb_istr.c  Main USB interrupts file
   - usb_endp.c  Non-Control interrupt routines file


Hardware environment
====================
 - STR912-SK-IAR board
   
How to use it
=============
In order to make the program work, you must do the following :
- connect P7.4 and P7.3 to GND
- Run the example
- To emulate the Mouse directions press B1/B2 Button or 
   connect P7.3 and P7.4 to VCC

******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****
