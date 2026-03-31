/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/22/2006 : Version 1.0
* Description        : Description of the STR91x Webserver Demo
********************************************************************************
* History:
* 05/22/2006 : Version 1.0
* 06/19/2006 : version 1.1
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
This demo shows a webserver application runing on the top of the uIP TCP-IP
stack.

Directory contents
==================
- STR91xlibrary: STR91x standard library files 
- ethernet_lib: ENET driver(91x_enet.c .h, memcopy.s)
- uip : uIP stack & applications files
- project : project files 


How to use it
=============
- compile the project, you need to include the following source files:
 + the following STR91x library files (from STR91xlibrary folder):
   - 91x_gpio.c
   - 91x_fmi.c
   - 91x_lib.c
   - 91x_scu.c
 + All ethernet_lib files 
 + the following uip files (from uip folder):
   - cgi.c
   - fs.c
   - httpd.c
   - uip.c
   - uip_arp.c
   - uip_arch.c
   - uIPMain.c
 + the following project files (from project folder)
   - main.c
   - 91x_it.c
   - 91x_init.s 
   - 91x_vect.s

- Change your PC IP address to 192.168.0.x (x different from 8)  
- Connect your PC to the STR912-SK IAR board using an EThernet crossover 
  cable or through a hub or switch.
- Run the application
- ping the board, by typing in the dos command window: "ping 192.168.0.8"
- type http:\\192.168.0.8 in your internet browser to load the STR91x webpage


******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****
