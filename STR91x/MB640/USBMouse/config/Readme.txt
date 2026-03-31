/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : JTrace's macro.
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

To use the JTrace you must do the following:

- Add the ETM_Init.mac macro file to your projcet:

Project-> Options -> Debugger -> use macro file

- Put the 2 Jumpers J15 and J16 respectively on the 2 positions:ETM_P67 and ETM_P66

- Do not connect the MC (Motor Control) connector since it shares pins with the 
  ETM jumpers.

- Start the Debugger then open the Trace Setup Window(J-link->Trace Setup...)

- Select the "Normal, Half-rate clocking" as Trace port mode

/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE*****/
