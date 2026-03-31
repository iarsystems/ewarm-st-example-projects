/******************** (C) COPYRIGHT 2003-2004 STMicroelectronics ****************
* File Name          : LPmode.h
* Author             : MCD APPLICATION TEAM
* Date First Issued  : DECEMBER 2004
* Description        : STR71x Power management routines include file 
*********************************************************************************
* History:
*  12/10/2004 : Created
********************************************************************************
*THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A 
RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR 
CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH 
SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN
IN CONNECTION WITH THEIR PRODUCTS.
********************************************************************************/

#include "71x_lib.h"

typedef enum
{
  CLK2,
  CLK2_16,
  RTC_CLK
} SlowClock_Typedef; 


void SLOW(SlowClock_Typedef clock);

void WFI(void);

void LPWFI(SlowClock_Typedef clock,FunctionalState VRstate);

void STOP(FunctionalState Flashstate,FunctionalState VRstate);

void STANDBY(FunctionalState VRstate);


  
