/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : wm8731_drv.h
 *    Description : Wolfson WM8731 Audio Codec driver header
 *
 *    History :
 *    1. Date        : October, 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#ifndef __WM8731_DRV_H
#define __WM8731_DRV_H

#define WM8731_ADDR   0x1A

/* WM8731 Registers */
#define WM8731_LLIN   (0x00)
#define WM8731_RLIN   (0x01)
#define WM8731_LHO    (0x02)
#define WM8731_RHO    (0x03)
#define WM8731_AAPC   (0x04)
#define WM8731_DAPC   (0x05)
#define WM8731_PDC    (0x06)
#define WM8731_DAIF   (0x07)
#define WM8731_SC     (0x08)
#define WM8731_AC     (0x09)
#define WM8731_RESET  (0x0F)

/* Return codes */
#define WM8731_RESULT_OK      0
#define WM8731_RESULT_ERROR   1
#define WM8731_RESULT_HWFAULT 2

/* WM8731 Register bifield definitions */
#define WM8731_AAPC_MICBOOST          0
#define WM8731_AAPC_MICBOOST_Disable (0 << WM8731_AAPC_MICBOOST)
#define WM8731_AAPC_MICBOOST_Enable  (1 << WM8731_AAPC_MICBOOST)

#define WM8731_AAPC_MUTEMIC           1
#define WM8731_AAPC_MUTEMIC_Disable  (0 << WM8731_AAPC_MUTEMIC)
#define WM8731_AAPC_MUTEMIC_Enable   (1 << WM8731_AAPC_MUTEMIC)

#define WM8731_AAPC_INSEL             2
#define WM8731_AAPC_INSEL_LineIn     (0 << WM8731_AAPC_INSEL)
#define WM8731_AAPC_INSEL_Mic        (1 << WM8731_AAPC_INSEL)

#define WM8731_AAPC_BYPASS            3
#define WM8731_AAPC_BYPASS_Disable   (0 << WM8731_AAPC_BYPASS)
#define WM8731_AAPC_BYPASS_Enable    (1 << WM8731_AAPC_BYPASS)

#define WM8731_AAPC_DACSEL            4
#define WM8731_AAPC_DACSEL_No        (0 << WM8731_AAPC_DACSEL)
#define WM8731_AAPC_DACSEL_Yes       (1 << WM8731_AAPC_DACSEL)

#define WM8731_AAPC_SIDETONE          5
#define WM8731_AAPC_SIDETONE_Disable (0 << WM8731_AAPC_SIDETONE)
#define WM8731_AAPC_SIDETONE_Enable  (1 << WM8731_AAPC_SIDETONE)

#define WM8731_AAPC_SIDEATT           6
#define WM8731_AAPC_SIDEATT_m6dB     (0 << WM8731_AAPC_SIDEATT)
#define WM8731_AAPC_SIDEATT_m9dB     (1 << WM8731_AAPC_SIDEATT)
#define WM8731_AAPC_SIDEATT_m12dB    (2 << WM8731_AAPC_SIDEATT)
#define WM8731_AAPC_SIDEATT_m15dB    (3 << WM8731_AAPC_SIDEATT)


#define WM8731_DAPC_ADCHPD            0
#define WM8731_DAPC_ADCHPD_Enable    (0 << WM8731_DAPC_ADCHPD)
#define WM8731_DAPC_ADCHPD_Disable   (1 << WM8731_DAPC_ADCHPD)

#define WM8731_DAPC_DEEMP             1
#define WM8731_DAPC_DEEMP_Disable    (0 << WM8731_DAPC_DEEMP)
#define WM8731_DAPC_DEEMP_32kHz      (1 << WM8731_DAPC_DEEMP)
#define WM8731_DAPC_DEEMP_44_1kHz    (2 << WM8731_DAPC_DEEMP)
#define WM8731_DAPC_DEEMP_48kHz      (3 << WM8731_DAPC_DEEMP)

#define WM8731_DAPC_DACMU             3
#define WM8731_DAPC_DACMU_Disable    (0 << WM8731_DAPC_DACMU)
#define WM8731_DAPC_DACMU_Enable     (1 << WM8731_DAPC_DACMU)

#define WM8731_DAPC_HPOR              4
#define WM8731_DAPC_HPOR_ClearOffset (0 << WM8731_DAPC_HPOR)
#define WM8731_DAPC_HPOR_StoreOffset (1 << WM8731_DAPC_HPOR)


#define WM8731_LHO_LHPVOL             0
#define WM8731_LHO_LHPVOL_Set(vol)   (vol << WM8731_LHO_LHPVOL)
#define WM8731_LHO_LHPVOL_Max         0x7F
#define WM8731_LHO_LHPVOL_Min         0x30
#define WM8731_LHO_LHPVOL_Mute        0x00

#define WM8731_LHO_LZCEN              7
#define WM8731_LHO_LZCEN_Disable     (0 << WM8731_LHO_LZCEN)
#define WM8731_LHO_LZCEN_Enable      (1 << WM8731_LHO_LZCEN)

/* 8th bit of WM8731_LHOH */
#define WM8731_LHOH_LRHPBOTH          0
#define WM8731_LHOH_LRHPBOTH_Disable (0 << WM8731_LHOH_LRHPBOTH)
#define WM8731_LHOH_LRHPBOTH_Enable  (1 << WM8731_LHOH_LRHPBOTH)


#define WM8731_RHO_RHPVOL             0
#define WM8731_RHO_RHPVOL_Set(vol)    (vol << WM8731_RHO_RHPVOL)
#define WM8731_RHO_RHPVOL_Max         0x7F
#define WM8731_RHO_RHPVOL_Min         0x30
#define WM8731_RHO_RHPVOL_Mute        0x00

#define WM8731_RHO_RZCEN              7
#define WM8731_RHO_RZCEN_Disable     (0 << WM8731_RHO_RZCEN)
#define WM8731_RHO_RZCEN_Enable      (1 << WM8731_RHO_RZCEN)

/* 8th bit of WM8731_RHO */
#define WM8731_RHOH_RLHPBOTH          0
#define WM8731_RHOH_RLHPBOTH_Disable (0 << WM8731_RHOH_RLHPBOTH)
#define WM8731_RHOH_RLHPBOTH_Enable  (1 << WM8731_RHOH_RLHPBOTH)


#define WM8731_SC_USBNORM             0
#define WM8731_SC_USBNORM_ModeNormal (0 << WM8731_SC_USBNORM)
#define WM8731_SC_USBNORM_ModeUSB    (1 << WM8731_SC_USBNORM)

#define WM8731_SC_BOSR                1
#define WM8731_SC_BOSR_USB250fs      (0 << WM8731_SC_BOSR)
#define WM8731_SC_BOSR_USB272fs      (1 << WM8731_SC_BOSR)
#define WM8731_SC_BOSR_Normal256fs   (0 << WM8731_SC_BOSR)
#define WM8731_SC_BOSR_Normal128fs   (0 << WM8731_SC_BOSR)
#define WM8731_SC_BOSR_Normal384fs   (1 << WM8731_SC_BOSR)
#define WM8731_SC_BOSR_Normal192fs   (1 << WM8731_SC_BOSR)

#define WM8731_SC_SR                  2
#define WM8731_SC_SR_Set(val)        (val << WM8731_SC_SR)

#define WM8731_SC_CLKDIV2             7
#define WM8731_SC_CLKDIV2_Disable    (0 << WM8731_SC_CLKDIV2)
#define WM8731_SC_CLKDIV2_Enable     (1 << WM8731_SC_CLKDIV2)


#define WM8731_DAIF_FORMAT            0
#define WM8731_DAIF_FORMAT_RightJ    (0 << WM8731_DAIF_FORMAT)
#define WM8731_DAIF_FORMAT_LeftJ     (1 << WM8731_DAIF_FORMAT)
#define WM8731_DAIF_FORMAT_I2SLeft1J (2 << WM8731_DAIF_FORMAT)
#define WM8731_DAIF_FORMAT_DSPMode   (3 << WM8731_DAIF_FORMAT)

#define WM8731_DAIF_IWL               2
#define WM8731_DAIF_IWL_16bit        (0 << WM8731_DAIF_IWL)
#define WM8731_DAIF_IWL_20bit        (1 << WM8731_DAIF_IWL)
#define WM8731_DAIF_IWL_24bit        (2 << WM8731_DAIF_IWL)
#define WM8731_DAIF_IWL_32bit        (3 << WM8731_DAIF_IWL)

#define WM8731_DAIF_LRP               4
#define WM8731_DAIF_LRP_MSB1BCLK     (0 << WM8731_DAIF_LRP)
#define WM8731_DAIF_LRP_MSB2BCLK     (1 << WM8731_DAIF_LRP)
#define WM8731_DAIF_LRP_RightOnLow   (0 << WM8731_DAIF_LRP)
#define WM8731_DAIF_LRP_RightOnHigh  (1 << WM8731_DAIF_LRP)

#define WM8731_DAIF_LRSWAP            5
#define WM8731_DAIF_LRSWAP_Disable   (0 << WM8731_DAIF_LRSWAP)
#define WM8731_DAIF_LRSWAP_Enable    (1 << WM8731_DAIF_LRSWAP)

#define WM8731_DAIF_MS                6
#define WM8731_DAIF_MS_Disable       (0 << WM8731_DAIF_MS)
#define WM8731_DAIF_MS_Enable        (1 << WM8731_DAIF_MS)

#define WM8731_DAIF_BCLKINV           7
#define WM8731_DAIF_BCLKINV_Disable  (0 << WM8731_DAIF_BCLKINV)
#define WM8731_DAIF_BCLKINV_Enable   (1 << WM8731_DAIF_BCLKINV)


#define WM8731_PDC_LINEINPD           0
#define WM8731_PDC_LINEINPD_Disable  (0 << WM8731_PDC_LINEINPD)
#define WM8731_PDC_LINEINPD_Enable   (1 << WM8731_PDC_LINEINPD)

#define WM8731_PDC_MICPD              1
#define WM8731_PDC_MICPD_Disable     (0 << WM8731_PDC_MICPD)
#define WM8731_PDC_MICPD_Enable      (1 << WM8731_PDC_MICPD)

#define WM8731_PDC_ADCPD              2
#define WM8731_PDC_ADCPD_Disable     (0 << WM8731_PDC_ADCPD)
#define WM8731_PDC_ADCPD_Enable      (1 << WM8731_PDC_ADCPD)

#define WM8731_PDC_DACPD              3
#define WM8731_PDC_DACPD_Disable     (0 << WM8731_PDC_DACPD)
#define WM8731_PDC_DACPD_Enable      (1 << WM8731_PDC_DACPD)

#define WM8731_PDC_OUTPD              4
#define WM8731_PDC_OUTPD_Disable     (0 << WM8731_PDC_OUTPD)
#define WM8731_PDC_OUTPD_Enable      (1 << WM8731_PDC_OUTPD)

#define WM8731_PDC_OSCPD              5
#define WM8731_PDC_OSCPD_Disable     (0 << WM8731_PDC_OSCPD)
#define WM8731_PDC_OSCPD_Enable      (1 << WM8731_PDC_OSCPD)

#define WM8731_PDC_CLKOUTPD           6
#define WM8731_PDC_CLKOUTPD_Disable  (0 << WM8731_PDC_CLKOUTPD)
#define WM8731_PDC_CLKOUTPD_Enable   (1 << WM8731_PDC_CLKOUTPD)

#define WM8731_PDC_POWEROFF           7
#define WM8731_PDC_POWEROFF_Disable  (0 << WM8731_PDC_POWEROFF)
#define WM8731_PDC_POWEROFF_Enable   (1 << WM8731_PDC_POWEROFF)


#define WM8731_LLIN_LINVOL            0
#define WM8731_LLIN_LINVOL_Set(vol)  (vol << WM8731_LLIN_LINVOL)
#define WM8731_LLIN_LINVOL_Min        0x00
#define WM8731_LLIN_LINVOL_Max        0x1F

#define WM8731_LLIN_LINMUTE           7
#define WM8731_LLIN_LINMUTE_Disable  (0 << WM8731_LLIN_LINMUTE)
#define WM8731_LLIN_LINMUTE_Enable   (1 << WM8731_LLIN_LINMUTE)

/* 8th bit of WM8731_LLIN */
#define WM8731_LLINH_LRINBOTH          0
#define WM8731_LLINH_LRINBOTH_Disable (0 << WM8731_LLINH_LRINBOTH)
#define WM8731_LLINH_LRINBOTH_Enable  (1 << WM8731_LLINH_LRINBOTH)


#define WM8731_RLIN_RINVOL            0
#define WM8731_RLIN_RINVOL_Set(vol)  (vol << WM8731_RLIN_RINVOL)
#define WM8731_RLIN_RINVOL_Min        0x00
#define WM8731_RLIN_RINVOL_Max        0x1F

#define WM8731_RLIN_RINMUTE           7
#define WM8731_RLIN_RINMUTE_Disable  (0 << WM8731_RLIN_RINMUTE)
#define WM8731_RLIN_RINMUTE_Enable   (1 << WM8731_RLIN_RINMUTE)

/* 8th bit of WM8731_RLIN */
#define WM8731_RLINH_RLINBOTH          0
#define WM8731_RLINH_RLINBOTH_Disable (0 << WM8731_RLINH_RLINBOTH)
#define WM8731_RLINH_RLINBOTH_Enable  (1 << WM8731_RLINH_RLINBOTH)

/*************************************************************************
 * Function Name: WM8731_Init
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Init Audio Codec
 *
 *************************************************************************/
uint32_t WM8731_Init(void);

/*************************************************************************
 * Function Name: WM8731_LineIn_Mute_Left
 * Parameters: Boolean Mute
 *
 * Return: Boolean
 *
 * Description: Mute Left LineIn Input
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Mute_Left(Boolean Mute);

/*************************************************************************
 * Function Name: WM8731_LineIn_Mute_Right
 * Parameters: Boolean Mute
 *
 * Return: Boolean
 *
 * Description: Mute Right LineIn Input
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Mute_Right(Boolean Mute);

/*************************************************************************
 * Function Name: WM8731_LineIn_Mute
 * Parameters: Boolean Mute
 *
 * Return: Boolean
 *
 * Description: Mute LineIn Input
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Mute(Boolean Mute);

/*************************************************************************
 * Function Name: WM8731_LineIn_Volume_Left
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Left LineIn Volume
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Volume_Left(uint8_t volume);

/*************************************************************************
 * Function Name: WM8731_LineIn_Volume_Right
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Right LineIn Volume
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Volume_Right(uint8_t volume);

/*************************************************************************
 * Function Name: WM8731_LineIn_Volume
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control LineIn Volume
 *
 *************************************************************************/
uint32_t WM8731_LineIn_Volume(uint8_t volume);

/*************************************************************************
 * Function Name: WM8731_PowerDownControl
 * Parameters: uint8_t pdc
 *
 * Return: Boolean
 *
 * Description: Control Left LineIn Volume
 *
 *************************************************************************/
uint32_t WM8731_PowerDownControl(uint8_t pdc);

/*************************************************************************
 * Function Name: WM8731_Headphone_Volume_Left
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Output left-channel Volume
 *
 *************************************************************************/
uint32_t WM8731_Headphone_Volume_Left(uint8_t volume);

/*************************************************************************
 * Function Name: WM8731_Headphone_Volume_Right
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Output right-channel Volume
 *
 *************************************************************************/
uint32_t WM8731_Headphone_Volume_Right(uint8_t volume);

/*************************************************************************
 * Function Name: WM8731_Headphone_Volume
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Output Volume
 *
 *************************************************************************/
uint32_t WM8731_Headphone_Volume(uint8_t volume);

/*************************************************************************
 * Function Name: WM8731_AAPC_Control
 * Parameters: uint8_t aapc
 *
 * Return: Boolean
 *
 * Description: Set AAPC register
 *
 *************************************************************************/
uint32_t WM8731_AAPC_Set(uint8_t aapc);

/*************************************************************************
 * Function Name: WM8731_Mic_Mute_Left
 * Parameters: uint8_t volume
 *
 * Return: Boolean
 *
 * Description: Control Output Volume
 *
 *************************************************************************/
uint32_t WM8731_Mic_Mute(Boolean Mute);

/*************************************************************************
 * Function Name: WM8731_Mic_Boost
 * Parameters: Boolean Boost
 *
 * Return: Boolean
 *
 * Description: Boost (on/off) of the mic input
 *
 *************************************************************************/
uint32_t WM8731_Mic_Boost(Boolean Boost);

/*************************************************************************
 * Function Name: WM8731_InputSelectMic
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Selects the mic input
 *
 *************************************************************************/
uint32_t WM8731_InputSelectMic(void);

/*************************************************************************
 * Function Name: WM8731_InputSelectLineIn
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Selects the line-in input
 *
 *************************************************************************/
uint32_t WM8731_InputSelectLineIn(void);

/*************************************************************************
 * Function Name: WM8731_SC_Set
 * Parameters: uint8_t sc
 *
 * Return: Boolean
 *
 * Description: Sets the SC register
 *
 *************************************************************************/
uint32_t WM8731_SC_Set(uint8_t sc);

/*************************************************************************
 * Function Name: WM8731_DAIF_Set
 * Parameters: uint8_t daif
 *
 * Return: Boolean
 *
 * Description: Sets the DAIF register
 *
 *************************************************************************/
uint32_t WM8731_DAIF_Set(uint8_t daif);

/*************************************************************************
 * Function Name: WM8731_DAPC_Set
 * Parameters: uint8_t dapc
 *
 * Return: Boolean
 *
 * Description: Sets the DAPC register
 *
 *************************************************************************/
uint32_t WM8731_DAPC_Set(uint8_t dapc);

/*************************************************************************
 * Function Name: WM8731_Reset
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Resets the codec
 *
 *************************************************************************/
uint32_t WM8731_Reset(void);

/*************************************************************************
 * Function Name: WM8731_ActivateInterface
 * Parameters: Boolean Active
 *
 * Return: Boolean
 *
 * Description: Activates the interface
 *
 *************************************************************************/
uint32_t WM8731_ActivateInterface(Boolean Active);

#endif // __WM8731_DRV_H
