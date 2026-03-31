/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : xti.c
* Author             : MCD Application Team
* Date First Issued  : 08/06/2003
* Description        : This file provides all the XTI software functions
********************************************************************************
* History:
*  08/06/2003 : Created
*******************************************************************************/

#include "xti.h"

/*******************************************************************************
* Function Name  : XTI_Init
* Description    : Initialize the XTI
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void XTI_Init(void)
{
  XTI->SRL=0x00;
  XTI->MRH=0x00;
  XTI->MRL=0x00;
  XTI->TRH=0x00;
  XTI->TRL=0x00;
  XTI->PRH=0x00;
  XTI->PRL=0x00;
  XTI->CTRL=0x00;
  XTI->CTRL=0x00;
}

/*******************************************************************************
* Function Name  : XTI_LineModeConfig
* Description    : Configure the trigger edge.
* Input 1        : Lines to be configured.
* Input 2        : The trigger edge can be FallingEdge or RisingEdge.
* Output         : None
* Return         : None
*******************************************************************************/
void XTI_LineModeConfig(u16 Lines, XTITriggerEdge_TypeDef TriggerEdge)
{
  if(TriggerEdge == XTI_FallingEdge)
  {
    XTI->TRL &= ~Lines;
    XTI->TRH &= ~(Lines>>8)&0x00FF;
  }
  else
  {
    XTI->TRL |= Lines;
    XTI->TRH |= (Lines>>8)&0x00FF;
  }
}

/*******************************************************************************
* Function Name  : XTI_LineConfig
* Description    : Enable and disable lines interrupts
* Input 1        : Lines to be configured.
* Input 2        : NewStatus can be ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void XTI_LineConfig(u16 Lines, functionalstate NewStatus)
{
  if(NewStatus)
  {
    XTI->MRL |= Lines;
    XTI->MRH |= (Lines>>8)&0x00FF;
  }
  else
  {
    XTI->MRL &= ~Lines;
    XTI->MRL &= ~(Lines>>8)&0x00FF;
  }
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
