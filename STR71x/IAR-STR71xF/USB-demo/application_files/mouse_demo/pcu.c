/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : pcu.c
* Author             : MCD Application Team
* Date First Issued  : 09/09/03
* Description        : This file provides all the Power Control Unit functions
********************************************************************************
* History:
*  20/05/03 : creation
*******************************************************************************/

#include "pcu.h"
#include "rccu.h"

/*******************************************************************************
* Function Name  : PCU_VRConfig
* Description    : This routine is used to configure PCU voltage regultors
* Input 1        : MVR : Main voltage Regulator
                   LPR : Low Power Regulator
* Input 2        : ENABLE : Enable the Voltage Regulator
                   DISABLE: Disable ( ByPass ) the VR
* Return         : None
*******************************************************************************/
void PCU_VRConfig ( PCU_VR Xvr, functionalstate Xstate )
{
  u16 Tmp = PCU->PWRCTRL;
  switch ( Xvr )
  {
    case PCU_MVR :
        // Configure the Main Voltage Regulator
        if (Xstate == DISABLE) Tmp |= PCU_MVR_Mask; else Tmp &= ~PCU_MVR_Mask;
        break;
    case PCU_LPR :
        // Configure the Low power Voltage Regulator
        if (Xstate == DISABLE) Tmp |= PCU_LPR_Mask; else Tmp &= ~PCU_LPR_Mask;
        break;
  }
  // Unlock Power Control Register
  PCU->PWRCTRL |= PCU_WREN_Mask;
  PCU->PWRCTRL = Tmp | PCU_WREN_Mask;
}

/*******************************************************************************
* Function Name  : PCU_EnterWFI
* Description    : This routine is used to force the Device to enter in WFI mode
* Input 1        : CLOCK2_16 : Clock2_16 as system clock for WFI mode
*                  EXTERNAL  : external clock as system clock for WFI mode
* Input 2        : ENABLE : Enable Low Power Regulator during Wait For Interrupt Mode
*                  DISABLE: Disable Low Power Regulator during Wait For Interrupt Mode
* Input 3        : ENABLE : Enable Low Power Mode during Wait For Interrupt Mode
*                  DISABLE: Disable Low Power Mode during Wait For Interrupt Mode
* Return         : None
*******************************************************************************/
void PCU_EnterWFI ( WFI_CLOCKS Xclock, functionalstate Xlpr, functionalstate Xlpm )
{
  u32 Tmp;
  // Enable Low Power Regulator in WFI mode
  Tmp = PCU->PWRCTRL;
  // Unlock Power Control Register
  PCU->PWRCTRL |= PCU_WREN_Mask;
  PCU->PWRCTRL = Xlpr == ENABLE ? Tmp & ~PCU_LPRWFI_Mask : Tmp | PCU_LPRWFI_Mask;
  // WFI Clock Selection
  Tmp = RCCU->CLKCTL;
  RCCU->CLKCTL = Xclock == WFI_CLOCK2_16  ? Tmp & ~PCU_WFI_CKSEL_Mask : Tmp | PCU_WFI_CKSEL_Mask;
  // Low Power Mode during WFI mode
  Tmp = RCCU->CLKCTL;
  RCCU->CLKCTL = Xlpm == DISABLE ? Tmp & ~PCU_LPOWFI_Mask : Tmp | PCU_LPOWFI_Mask;
  // Enter WFI Mode
  RCCU->SYSMODE &= ~0x0001;
}

/*******************************************************************************
* Function Name  : PCU_EnterLPM
* Description    : This routine is used to force the device to enter in low
*                  power modes
* Input          : PCU_SLOW : Slow Mode
                   PCU_STOP : Stop Mode
                   PCU_STANDBY : StandBy Mode
* Return         : None
*******************************************************************************/
void PCU_EnterLPM ( LPM_MODES Xmode  )
{
  switch ( Xmode )
  {
    // Slow Mode
    case PCU_SLOW:
    {
      RCCU->PLLCONF |= 0x87;
      RCCU_RCLKSourceConfig ( RCCU_PLL1_Output );
      break;
    }

    // Stop Mode
    case PCU_STOP:
    {
      // Enable Stop EN bit
      RCCU->CLKCTL |= PCU_STOP_EN_Mask;

      // Write '1' to Stop Bit
      XTI->CTRL |= 0x04;
      // Write '0' to Stop Bit
      XTI->CTRL &= 0x03;
      // Write '1' to Stop Bit
      XTI->CTRL |= 0x04;
      // add Delay

      asm(
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "NOP");


      break;
      // test if the Device entred in a Stop Mode.
    }
    // PCU_STANDBY Mode
    case PCU_STANDBY:
    {
	  PCU->PWRCTRL |= PCU_WREN_Mask;	// Unlock Power Control Register
	  PCU->PWRCTRL |= PCU_PWRDWN_Mask;	// Set the Power Down flag
    }
  }
}
/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/

