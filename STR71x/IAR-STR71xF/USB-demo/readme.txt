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
MOUSE is a sample application using an endpoint programmed as INTERRUPT type giving 
to a PC a channel to read the status of a virtual HID mouse.
Motion sensors are emulated by means of a PC terminal emulator connected to the UART0 
of the STR71x.

During the enumeration phase, the PC detects the presence of the mouse device and loads 
the associated software driver and then it periodically polls the endpoint with IN tokens.
Mouse is a device configured as a HID device.

How to use it
=============
To test this mouse tutorial, a minimal configuration is required :
-connect a PC serial port to the UART0

-on the PC, run a terminal emulation program (i.e. Hyperterminal) configured with the
following parameters :
  -baud rate = 9600
  -data bits = 8
  -parity    = no
  -stop bits = 1
  -flow control = no


- run program
- use the USB cable to connect the PC to USB of the Board.

Mouse motion events are emulated with the following PC keyboard keys:
[A] = right
[D] = left
[W] = up
[X] = down

The first time the device is connected to a Windows 98 computer the driver of this new 
hardware is loaded. Relative windows driver is : USB Human Interface Device


;******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****
