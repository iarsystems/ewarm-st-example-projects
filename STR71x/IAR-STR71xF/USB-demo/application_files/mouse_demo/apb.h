/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : apb.h
* Author             : MCD Application Team
* Date First Issued  : 30/05/2003
* Description        : This file contains all the functions prototypes for the
*                      APB bridge software library.
********************************************************************************
* History:
*  30/05/03 : Creation.
*******************************************************************************/
#ifndef __APB_H
#define __APB_H

#include "71x_map.h"

typedef enum
{
	APB_Out_Of_Memory,
    APB_Time_Out
} Error_Types;

typedef enum
{
	APB_Read,
    APB_Write
} Operation_Types;

typedef /*volatile*/ struct
{
	Error_Types Error_Type;
	Operation_Types Operation_Type;
	u16 Peripherals_Addr;
} APB_Error_Params;

typedef enum
{
	APB_APBT  = 0x26,
    APB_OUTM  = 0x25,
    APB_ABORT = 0x21,
    APB_ABTEN = 0x49,
    APB_OMnRW = 0x68,
    APB_TOnRW = 0x88
} APB_Flags;


#define  I2C0_Periph      0x0002
#define  I2C1_Periph      0x0004
#define  UART0_Periph     0x0010
#define  UART1_Periph     0x0020
#define  UART2_Periph     0x0040
#define  UART3_Periph     0x0080
#define  USB_Periph       0x0100
#define  CAN_Periph       0x0200
#define  BSPI0_Periph     0x0400
#define  BSPI1_Periph     0x0800

#define  XTI_Periph       0x0002
#define  GPIO0_Periph     0x0004
#define  GPIO1_Periph     0x0008
#define  GPIO2_Periph     0x0010
#define  ADC12_Periph     0x0080
#define  TIM0_Periph      0x0200
#define  TIM1_Periph      0x0400
#define  TIM2_Periph      0x0800
#define  TIM3_Periph      0x1000
#define  RTC_Periph       0x2000
#define  WDG_Periph       0x4000
#define  EIC_Periph       0x8000


// ABORT  : ABORT Flag BSR[0]
#define APB_ABORT_Mask       0x00000001

// OUTM  : Out Of Memory Flag  BSR[4]
#define APB_OUTM_Mask        0x00000010
#define APB_OUTM_Index       0x04

// APBT  : Time Out Abort Flag BSR[5]
#define APB_APBT_Mask        0x00000020
#define APB_APBT_Index       0x05

// ABTEN : Abort ENABLE Flag TOR[8]
#define APB_ABTEN_Mask       0x00000100
#define APB_ABTEN_Index      0x08

// nRW   : Read Write Operation Flag OMR[7] & TOER[7]
#define APB_nRW_Mask         0x00000080
#define APB_nRW_Index        0x07

// Peripheral_Address  : Peripheral address Flag OMR[14:0] & TOER[14:0]
#define APB_Peripheral_Mask  0x00FFFF00
#define APB_Peripheral_Index 0x08

// TOUT  : Time Out counter mask TOR[4:0]
#define APB_TOUT_Mask        0x0000001F

// Flag Mask
#define APB_Flag_Mask        0x1F
#define APB_Reg_Mask         0xE0

/*******************************************************************************
* Function Name  : APB_TocValue
* Description    : Get the current time out counter
*                : value on the corresponding APB bridge passed in parameter.
* Input          : APBx ( APB1 or APB2 )
* Return         : The Current Time out counter value on the Bridge (u8)
*******************************************************************************/
inline u8 APB_TocValue ( APB_TypeDef *APBx )
{
	return APBx->TOR & APB_TOUT_Mask;
}

/*******************************************************************************
* Function Name  : APB_TocConfig
* Description    : Updates the time out counter value in the corresponding APB
*                  bridge passed in parameter.
* Input          : APBx ( APB1 or APB2 )
*                  The new time out counter value (u8)
* Return         : None
*******************************************************************************/
inline void APB_TocConfig ( APB_TypeDef *APBx, u8 New_TOC)
{
	APBx->TOR = (APBx->TOR & APB_ABTEN_Mask) | ( New_TOC & APB_TOUT_Mask );
}

/*******************************************************************************
* Function Name  : APB_AbortGenerationConfig
* Description    : Enables/Disables the abort generation condition on the
*                  bridge passed on parameter.
* Input          : APBx ( APB1 or APB2 )
*                  New APB status ( ENABLE or DISABLE )
* Return         : None
*******************************************************************************/
inline void APB_AbortGenerationConfig ( APB_TypeDef *APBx,
                                 functionalstate New_Status )
{
	if (New_Status == ENABLE) APBx->TOR |= APB_ABTEN_Mask;
	else APBx->TOR &= ~APB_ABTEN_Mask;
}

/*******************************************************************************
* Function Name  : APB_AbortGenerationStatus
* Description    : Tests the Abort Generation on the corresponding APB bridge
*                  passed on parameter.
* Input          : APBx ( APB1 or APB2 )
* Return         : The Current Abort Generation Status ( SET, RESET )
*******************************************************************************/
inline flagstatus APB_AbortGenerationStatus ( APB_TypeDef *APBx )
{
	return ((APBx->TOR & APB_ABTEN_Mask) >> APB_ABTEN_Index) == 0 ? RESET : SET;
}

/*******************************************************************************
* Function Name  : APB_ErrorSourceStatus
* Description    : Get details about errors that occured in the APB Bridge
* Input          : APBx ( APB1 or APB2 )
* Return         : Error Info : - Error_Type: ( Out_Of_Memory, Time_Out )
*                               - Operation_Type: ( Read, Write )
*                               - Peripheral_Adr: (u16)
*******************************************************************************/
APB_Error_Params APB_ErrorSourceStatus ( APB_TypeDef *APBx );

/*******************************************************************************
* Function Name  : APB_ClockConfig
* Description    : Enables/Disables the Clock gating for peripherals on the APB
*                  bridge passed in parameters.
* Input          : APBx ( APB1 or APB2 )
*                  NewStatus ENABLE or DISABLE
*                  NewValue (u16)
* Return         : None
*******************************************************************************/
inline void APB_ClockConfig ( APB_TypeDef *APBx,
                       functionalstate New_Status,
                       u16 New_Value )
{
	if (New_Status == ENABLE) APBx->CKDIS &= ~New_Value;
	else APBx->CKDIS |= New_Value;
}

/*******************************************************************************
* Function Name  : APB_SwResetConfig
* Description    : Enables/Disables the software Reset for peripherals on the APB
*                  bridge passed in parameters.
* Input          : APBx ( APB1 or APB2 )
*                  NewStatus ENABLE or DISABLE
*                  NewValue (u16)
* Return         : None
*******************************************************************************/
inline void APB_SwResetConfig ( APB_TypeDef *APBx,
                         functionalstate New_Status,
                         u16 New_Value )
{
	if (New_Status == ENABLE) APBx->SWRES |= New_Value;
	else APBx->SWRES &= ~New_Value;
}

/*******************************************************************************
* Function Name  : APB_FlagStatus
* Description    : Get the status of the APB register Flags
* Input          : APBx ( APB1 or APB2 )
*                : The wanted Flag ( APBT, OUTM, ABORT, ABTEN, OMnRW, TOnRW  )
* Return         : Flag Status ( SET, RESET )
*******************************************************************************/
flagstatus APB_FlagStatus ( APB_TypeDef *APBx,
                            APB_Flags Flag );

/*******************************************************************************
* Function Name  : APB_FlagClear
* Description    : Clears a specified Flag on the APB register
* Input          : APBx ( APB1 or APB2 )
*                : The wanted Flag ( APBT, OUTM, ABORT, ABTEN, OMnRW, TOnRW  )
* Return         : None
*******************************************************************************/
void APB_FlagClear ( APB_TypeDef *APBx,
                     APB_Flags Flag );

#endif	// __APB_H

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
