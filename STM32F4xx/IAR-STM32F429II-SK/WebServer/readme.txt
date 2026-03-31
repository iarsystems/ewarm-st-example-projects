########################################################################
#
#                           lwipWebserver.eww
#
# $Revision: 39 $
#
########################################################################

DESCRIPTION
===========
   This example project shows how to use the IAR Embedded Workbench
 for ARM to develop code for the IAR-STM32F429II-SK board. It is an
 embedded Web Server based on lwIP TCP/IP stack v1.3.2 and features
 three HTML pages that form a compact and interactive Web Server to
 interact with IAR-STM32F429II-SK board.
  You will be able to address the IAR-STM32F429II-SK board as a web
 page using your web browser, as well as controlling the four LEDs on
 the board through the browser and finally you will be able to get
 continuously, each second, the ADC2 Channel 6 converted value and
 the board temperature, both displayed on the web page of your browser.
 The project can be configured to use Static IP or DHCP (see main.h).
 The IP address that is selected will be shown on the onboard LCD display.
 If the DHCP procedure is not successful, the selected static IP will
 be 192.168.0.200

 The PNG decoding is based on LodePNG decoder - http://lodev.org/lodepng/

COMPATIBILITY
=============
    The example project is compatible with IAR-STM32F429II-SK
  evaluation board. By default, the project is configured to use the
  J-Link JTAG/SWD interface.

CONFIGURATION
=============
    Make sure that the following jumpers are correctly configured on the
  IAR-STM32F429II-SK evaluation board:

   Jumpers:
    PWR_SEL - depending of power source
    B0_1/B0_0 - B0_0
    B1_1/B1_0 - B1_0

GETTING STARTED
===============
  1) Build and download the example.

  2) Run the example.
