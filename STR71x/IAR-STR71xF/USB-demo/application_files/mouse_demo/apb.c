/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : apb.c
* Author             : MCD Application Team
* Date First Issued  : 30/05/2003
* Description        : This file provides all the APB software functions
********************************************************************************
* History:
*  30/05/03 : Creation.
*******************************************************************************/

#include "apb.h"

/*******************************************************************************
* Function Name  : APB_ErrorSourceStatus
* Description    : Get details about errors that occured in the APB Bridge
* Input          : APBx ( APB1 or APB2 )
* Return         : Error Info : - Error_Type: ( APB_Out_Of_Memory, APB_Time_Out )
*                               - Operation_Type: ( APB_Read, APB_Write )
*                               - Peripheral_Adr: (u16)
*******************************************************************************/
APB_Error_Params APB_ErrorSourceStatus ( APB_TypeDef *APBx )
{
  u32 Tmp;
  u32 TmpAdr;
  u16 TmpFlag;
  APB_Error_Params result;

// Gets the Error Type
// if BSR[4] = 1 => An Out Of Memory Error
// if BSR[5] = 1 => A Time Out Error
  TmpFlag = (APBx->BSR & 0x00000030) >> 4;
  if ( TmpFlag == 1 )
  {
    result.Error_Type = APB_Out_Of_Memory;
    TmpAdr = APBx->OMR & 0x00FFFF80;
  }
  if ( TmpFlag == 2 )
  {
    result.Error_Type = APB_Time_Out;
    TmpAdr = APBx->TOER & 0x00FFFF80;
  }
// Gets the Operation Type
  Tmp = ( TmpAdr & APB_nRW_Mask ) >> APB_nRW_Index;
  result.Operation_Type = ( Tmp == 1 ) ? APB_Write : APB_Read;
// Gets the Peripheral Address
  TmpAdr = ( TmpAdr & APB_Peripheral_Mask ) >> APB_Peripheral_Index;
  result.Peripherals_Addr = TmpAdr;

  return result;
}

/*******************************************************************************
* Function Name  : APB_FlagStatus
* Description    : Get the status of the APB register Flags
* Input          : (*APBx) ( APB1 or APB2 )
*                : The wanted Flag ( APBT, OUTM, ABORT, ABTEN, OMnRW, TOnRW  )
* Return         : Flag Status ( SET, RESET )
*******************************************************************************/
flagstatus APB_FlagStatus ( APB_TypeDef *APBx,
                            APB_Flags Flag )
{
  u16 Tmp;
  u8 Tmp_RegIndex;
  u8 Tmp_FlagIndex;

  Tmp_FlagIndex = ( Flag & APB_Flag_Mask ) -  1;
  Tmp_RegIndex  = ( Flag & APB_Reg_Mask  ) >> 5;

  switch ( Tmp_RegIndex )
  {
// Flags in the BSR register
    case 1 : Tmp = ( APBx->BSR  >> Tmp_FlagIndex ) & 0x01; break;
// Flags in the TOR register
    case 2 : Tmp = ( APBx->TOR  >> Tmp_FlagIndex ) & 0x01; break;
// Flags in the OMR register
    case 3 : Tmp = ( APBx->OMR  >> Tmp_FlagIndex ) & 0x01; break;
// Flags in the TOER register
    case 4 : Tmp = ( APBx->TOER >> Tmp_FlagIndex ) & 0x01; break;
  }

  return Tmp == 1 ? SET : RESET;
}

/*******************************************************************************
* Function Name  : APB_FlagClear
* Description    : Clears a specified Flag on the APB register
* Input          : APBx ( APB1 or APB2 )
*                : The wanted flag ( APBT, OUTM, ABORT)
* Return         : None
*******************************************************************************/
void APB_FlagClear ( APB_TypeDef *APBx,
                     APB_Flags Flag )
{
  u8 Tmp_RegIndex;
  u8 Tmp_FlagIndex;

  Tmp_FlagIndex = (Flag & APB_Flag_Mask ) - 1;
  Tmp_RegIndex  = (Flag & APB_Reg_Mask ) >> 5;

// Act only on the BSR register Flags
  if ( Tmp_RegIndex == 1 ) APBx->BSR = 0x01 << Tmp_FlagIndex;
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
