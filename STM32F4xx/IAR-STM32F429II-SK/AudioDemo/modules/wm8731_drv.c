/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : wm8731_drv.c
 *    Description : Wolfson WM8731 Audio Codec Driver
 *
 *    History :
 *    1. Date        : October, 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 3186 $
 **************************************************************************/

#include "arm_comm.h"
#include "stdint.h"
#include "wm8731_drv.h"
#include "wm8731_drv_usr.h"

volatile uint8_t Data[2];

uint8_t volumeLineInLeft;
uint8_t volumeLineInRight;

Boolean muteLineInLeft;
Boolean muteLineInRight;

uint8_t registerAAPC;

uint8_t volumeHeadphoneLeft;
uint8_t volumeHeadphoneRight;

Boolean muteHeadphoneLeft;
Boolean muteHeadphoneRight;

/*************************************************************************
 * Function Name: WM8731_Init
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Init Audio Codec
 *
 *************************************************************************/
uint32_t WM8731_Init(void)
{
uint32_t result;

  registerAAPC = WM8731_AAPC_MICBOOST_Enable  |
                 WM8731_AAPC_MUTEMIC_Enable   |
                 WM8731_AAPC_INSEL_LineIn     |
                 WM8731_AAPC_BYPASS_Disable   |
                 WM8731_AAPC_DACSEL_Yes       |
                 WM8731_AAPC_SIDETONE_Disable |
                 WM8731_AAPC_SIDEATT_m6dB;

  result = WM8731_Reset();
  if(result != WM8731_RESULT_OK) return result;

  result = WM8731_Headphone_Volume(WM8731_LHO_LHPVOL_Mute);
  if(result != WM8731_RESULT_OK) return result;

  result = WM8731_LineIn_Mute(TRUE);
  if(result != WM8731_RESULT_OK) return result;

  result = WM8731_DAIF_Set(WM8731_DAIF_FORMAT_I2SLeft1J | WM8731_DAIF_LRP_RightOnHigh);
  if(result != WM8731_RESULT_OK) return result;

  result = WM8731_SC_Set(WM8731_SC_SR_Set(0x08));
  if(result != WM8731_RESULT_OK) return result;

  result = WM8731_DAPC_Set(0);
  if(result != WM8731_RESULT_OK) return result;

  result = WM8731_AAPC_Set(registerAAPC);
  if(result != WM8731_RESULT_OK) return result;

  result = WM8731_ActivateInterface(TRUE);
  if(result != WM8731_RESULT_OK) return result;

  result = WM8731_PowerDownControl(WM8731_PDC_MICPD_Disable   |
                                   WM8731_PDC_ADCPD_Disable   |
                                   WM8731_PDC_DACPD_Disable   |
                                   WM8731_PDC_OUTPD_Disable   |
                                   WM8731_PDC_OSCPD_Enable    |
                                   WM8731_PDC_CLKOUTPD_Enable |
                                   WM8731_PDC_POWEROFF_Disable);
  if(result != WM8731_RESULT_OK) return result;

  result = WM8731_Headphone_Volume((WM8731_LHO_LHPVOL_Max+WM8731_LHO_LHPVOL_Min)/2);
  if(result != WM8731_RESULT_OK) return result;

  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_LineIn_Mute_Left
 * Parameters: Boolean Mute
 *
 * Return: Boolean
 *
 * Description: Mute Left LineIn Input
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Mute_Left(Boolean Mute)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  Data[0] = WM8731_LLIN << 1;
  if (Mute)
    Data[1] = WM8731_LLIN_LINMUTE_Enable | WM8731_LLIN_LINVOL_Set(volumeLineInLeft);
  else
    Data[1] = WM8731_LLIN_LINMUTE_Disable | WM8731_LLIN_LINVOL_Set(volumeLineInLeft);
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  muteLineInLeft = (Mute)?WM8731_LLIN_LINMUTE_Enable:WM8731_LLIN_LINMUTE_Disable;
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_LineIn_Mute_Right
 * Parameters: Boolean Mute
 *
 * Return: Boolean
 *
 * Description: Mute Right LineIn Input
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Mute_Right(Boolean Mute)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  Data[0] = WM8731_RLIN << 1;
  if (Mute)
    Data[1] = WM8731_RLIN_RINMUTE_Enable | WM8731_RLIN_RINVOL_Set(volumeLineInRight);
  else
    Data[1] = WM8731_RLIN_RINMUTE_Disable | WM8731_RLIN_RINVOL_Set(volumeLineInRight);
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  muteLineInRight = (Mute)?WM8731_RLIN_RINMUTE_Enable:WM8731_RLIN_RINMUTE_Disable;
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_LineIn_Mute
 * Parameters: Boolean Mute
 *
 * Return: Boolean
 *
 * Description: Mute LineIn Input
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Mute(Boolean Mute)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  Data[0]  = WM8731_RLIN << 1;
  Data[0] |= WM8731_RLINH_RLINBOTH_Enable;
  if (Mute)
    Data[1] = WM8731_RLIN_RINMUTE_Enable | WM8731_RLIN_RINVOL_Set(volumeLineInRight);
  else
    Data[1] = WM8731_RLIN_RINMUTE_Disable | WM8731_RLIN_RINVOL_Set(volumeLineInRight);
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  muteLineInRight = (Mute)?WM8731_RLIN_RINMUTE_Enable:WM8731_RLIN_RINMUTE_Disable;
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_LineIn_Volume_Left
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Left LineIn Volume
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Volume_Left(uint8_t volume)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  if(volume > WM8731_LLIN_LINVOL_Max) volume = WM8731_LLIN_LINVOL_Max;
  Data[0] = WM8731_LLIN << 1;
  Data[1] = muteLineInLeft | WM8731_LLIN_LINVOL_Set(volume);
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  volumeLineInLeft = volume;
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_LineIn_Volume_Right
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Right LineIn Volume
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Volume_Right(uint8_t volume)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  if(volume > WM8731_RLIN_RINVOL_Max) volume = WM8731_RLIN_RINVOL_Max;
  Data[0] = WM8731_RLIN << 1;
  Data[1] = muteLineInRight | WM8731_RLIN_RINVOL_Set(volume);
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  volumeLineInRight = volume;
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_LineIn_Volume
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control LineIn Volume
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Volume(uint8_t volume)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  if(volume > WM8731_RLIN_RINVOL_Max) volume = WM8731_RLIN_RINVOL_Max;
  Data[0]  = WM8731_RLIN << 1;
  Data[0] |= WM8731_RLINH_RLINBOTH_Enable;
  Data[1]  = muteLineInRight | WM8731_RLIN_RINVOL_Set(volume);
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  volumeLineInRight = volumeLineInLeft = volume;
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_PowerDownControl
 * Parameters: uint8_t pdc
 *
 * Return: Boolean
 *
 * Description: Control Left LineIn Volume
 *
 *************************************************************************/
uint32_t WM8731_PowerDownControl(uint8_t pdc)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  Data[0] = WM8731_PDC << 1;
  Data[1] = pdc;
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_Headphone_Volume_Left
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Output left-channel Volume
 *
 *************************************************************************/
uint32_t WM8731_Headphone_Volume_Left(uint8_t volume)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  if(volume > WM8731_LHO_LHPVOL_Max) volume = WM8731_LHO_LHPVOL_Max;
  if(volume < WM8731_LHO_LHPVOL_Min) volume = WM8731_LHO_LHPVOL_Mute;
  Data[0]  = WM8731_LHO << 1;
  Data[1]  = muteHeadphoneLeft | WM8731_LHO_LZCEN_Enable | WM8731_LHO_LHPVOL_Set(volume);
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  volumeHeadphoneLeft = volume;
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_Headphone_Volume_Right
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Output right-channel Volume
 *
 *************************************************************************/
uint32_t WM8731_Headphone_Volume_Right(uint8_t volume)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  if(volume > WM8731_RHO_RHPVOL_Max) volume = WM8731_RHO_RHPVOL_Max;
  if(volume < WM8731_RHO_RHPVOL_Min) volume = WM8731_RHO_RHPVOL_Mute;
  Data[0] = WM8731_RHO << 1;
  Data[1] = muteHeadphoneRight | WM8731_RHO_RZCEN_Enable | WM8731_RHO_RHPVOL_Set(volume);
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  volumeHeadphoneRight = volume;
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_Headphone_Volume
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Output Volume
 *
 *************************************************************************/
uint32_t WM8731_Headphone_Volume(uint8_t volume)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  if(volume > WM8731_RHO_RHPVOL_Max) volume = WM8731_RHO_RHPVOL_Max;
  if(volume < WM8731_RHO_RHPVOL_Min) volume = WM8731_RHO_RHPVOL_Mute;
  Data[0]  = WM8731_RHO << 1;
  Data[0] |= WM8731_RHOH_RLHPBOTH_Enable;
  Data[1]  = muteHeadphoneRight | WM8731_RHO_RZCEN_Enable | WM8731_RHO_RHPVOL_Set(volume);
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  volumeHeadphoneRight = volumeHeadphoneLeft = volume;
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_AAPC_Control
 * Parameters: uint8_t aapc
 *
 * Return: Boolean
 *
 * Description: Set AAPC register
 *
 *************************************************************************/
uint32_t WM8731_AAPC_Set(uint8_t aapc)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  Data[0] = WM8731_AAPC << 1;
  Data[1] = aapc;
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  registerAAPC = aapc;
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_Mic_Mute
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Output Volume
 *
 *************************************************************************/
uint32_t WM8731_Mic_Mute(Boolean Mute)
{
  if (Mute)
    return WM8731_AAPC_Set(registerAAPC | (1<<WM8731_AAPC_MUTEMIC));
  else
    return WM8731_AAPC_Set(registerAAPC & ~(1<<WM8731_AAPC_MUTEMIC));
}

/*************************************************************************
 * Function Name: WM8731_Mic_Boost
 * Parameters: Boolean Boost
 *
 * Return: Boolean
 *
 * Description: Boost (on/off) of the mic input
 *
 *************************************************************************/
uint32_t WM8731_Mic_Boost(Boolean Boost)
{
  if (Boost)
    return WM8731_AAPC_Set(registerAAPC | (1<<WM8731_AAPC_MICBOOST));
  else
    return WM8731_AAPC_Set(registerAAPC & ~(1<<WM8731_AAPC_MICBOOST));
}

/*************************************************************************
 * Function Name: WM8731_InputSelectMic
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Selects the mic input
 *
 *************************************************************************/
uint32_t WM8731_InputSelectMic(void)
{
  return WM8731_AAPC_Set(registerAAPC | (1<<WM8731_AAPC_INSEL));
}

/*************************************************************************
 * Function Name: WM8731_InputSelectLineIn
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Selects the line-in input
 *
 *************************************************************************/
uint32_t WM8731_InputSelectLineIn(void)
{
  return WM8731_AAPC_Set(registerAAPC & ~(1<<WM8731_AAPC_INSEL));
}

/*************************************************************************
 * Function Name: WM8731_SC_Set
 * Parameters: uint8_t sc
 *
 * Return: Boolean
 *
 * Description: Sets the SC register
 *
 *************************************************************************/
uint32_t WM8731_SC_Set(uint8_t sc)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  Data[0] = WM8731_SC << 1;
  Data[1] = sc;
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_DAIF_Set
 * Parameters: uint8_t daif
 *
 * Return: Boolean
 *
 * Description: Sets the DAIF register
 *
 *************************************************************************/
uint32_t WM8731_DAIF_Set(uint8_t daif)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  Data[0] = WM8731_DAIF << 1;
  Data[1] = daif;
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_DAPC_Set
 * Parameters: uint8_t dapc
 *
 * Return: Boolean
 *
 * Description: Sets the DAPC register
 *
 *************************************************************************/
uint32_t WM8731_DAPC_Set(uint8_t dapc)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  Data[0] = WM8731_DAPC << 1;
  Data[1] = dapc;
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_Reset
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Resets the codec
 *
 *************************************************************************/
uint32_t WM8731_Reset(void)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  Data[0] = WM8731_RESET << 1;
  Data[1] = 0;
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}

/*************************************************************************
 * Function Name: WM8731_ActivateInterface
 * Parameters: Boolean Active
 *
 * Return: Boolean
 *
 * Description: Activates the interface
 *
 *************************************************************************/
uint32_t WM8731_ActivateInterface(Boolean Active)
{
  if(FALSE == I2C_OPEN_FUNC()) return WM8731_RESULT_HWFAULT;
  Data[0] = WM8731_AC << 1;
  Data[1] = (Active)?1:0;
  if (FALSE == I2C_SEND_FUNC(WM8731_ADDR, &Data[0], 2)) { I2C_CLOSE_FUNC(); return WM8731_RESULT_ERROR;}
  I2C_CLOSE_FUNC();
  return WM8731_RESULT_OK;
}
