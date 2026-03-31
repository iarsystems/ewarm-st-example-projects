/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : STR91x_usb.h
 *    Description : usb module header file
 *
 *    History :
 *    1. Date        : September 29, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/

#include "includes.h"
#include "usb_9.h"

#ifndef __STR91X_USB_H
#define __STR91X_USB_H

#ifdef HAL_USB_GLOBAL
#define HAL_USB_EXTERN
#else
#define HAL_USB_EXTERN  extern
#endif

#define MSG_FILE stdout

#if USB_TRACE_LM_EN != 0
#define  USB_LM_TRACE_INFO(...)   fprintf(MSG_FILE,  __VA_ARGS__)
#else
#define  USB_LM_TRACE_INFO(...)
#endif

#if USB_TRACE_LW_EN != 0
#define  USB_LW_TRACE_INFO(...)   fprintf(MSG_FILE,  __VA_ARGS__)
#else
#define  USB_LW_TRACE_INFO(...)
#endif

#if USB_TRACE_LE_EN != 0
#define  USB_LE_TRACE_INFO(...)   fprintf(MSG_FILE,  __VA_ARGS__)
#else
#define  USB_LE_TRACE_INFO(...)
#endif

#if USB_TRACE_T9M_EN != 0
#define  USB_T9M_TRACE_INFO(...)   fprintf(MSG_FILE,  __VA_ARGS__)
#else
#define  USB_T9M_TRACE_INFO(...)
#endif

#if USB_TRACE_T9W_EN != 0
#define  USB_T9W_TRACE_INFO(...)   fprintf(MSG_FILE,  __VA_ARGS__)
#else
#define  USB_T9W_TRACE_INFO(...)
#endif

#if USB_TRACE_T9E_EN != 0
#define  USB_T9E_TRACE_INFO(...)   fprintf(MSG_FILE,  __VA_ARGS__)
#else
#define  USB_T9E_TRACE_INFO(...)
#endif

#include "io_macros.h"

/* USB control register (USB_CNTR) */
typedef struct{
__REG16 FRES     : 1;
__REG16 PDWN     : 1;
__REG16 LP_MODE  : 1;
__REG16 FSUSP    : 1;
__REG16 RESUME   : 1;
__REG16          : 2;
__REG16 SZDPRM   : 1;
__REG16 ESOFM    : 1;
__REG16 SOFM     : 1;
__REG16 RESETM   : 1;
__REG16 SUSPM    : 1;
__REG16 WKUPM    : 1;
__REG16 ERRM     : 1;
__REG16 DOVRM    : 1;
__REG16 CTRM     : 1;
} __usb_cntr_bits;

/* USB Interrupt Status Register (USB_ISTR) */
typedef struct{
__REG16 EP_ID  : 4;
__REG16 DIR    : 1;
__REG16        : 2;
__REG16 SZDPR  : 1;
__REG16 ESOF   : 1;
__REG16 SOF    : 1;
__REG16 RESET  : 1;
__REG16 SUSP   : 1;
__REG16 WKUP   : 1;
__REG16 ERR    : 1;
__REG16 DOVR   : 1;
__REG16 CTR    : 1;
} __usb_st_bits;

/* USB Frame Number Register (USB_FNR) */
typedef struct{
__REG16 FN    :11;
__REG16 LSOF  : 2;
__REG16 LCK   : 1;
__REG16 RXDM  : 1;
__REG16 RXDP  : 1;
} __usb_fnr_bits;

/* USB device address (USB_DADDR) */
typedef struct{
__REG16 ADD  : 7;
__REG16 EF   : 1;
__REG16      : 8;
} __usb_daddr_bits;

/* Buffer table address (USB_BTABLE) */
typedef struct{
__REG16         : 3;
__REG16 BTABLE  :13;
} __usb_btable_bits;

/* USB Endpoint n Register (USB_EPnR) */
typedef struct{
__REG16 EA       : 4;
__REG16 STAT_TX  : 2;
__REG16 DTOG_TX  : 1;
__REG16 CTR_TX   : 1;
__REG16 EP_KIND  : 1;
__REG16 EP_TYPE  : 2;
__REG16 SETUP    : 1;
__REG16 STAT_RX  : 2;
__REG16 DTOG_RX  : 1;
__REG16 CTR_RX   : 1;
} __usb_epr_bits;

/* DMA control register 1 (USB_DMACR1) */
typedef struct{
__REG16 DMA_UNLK_RK_EN  :10;
__REG16                 : 6;
} __usb_dmacr1_bits;

/* DMA control register 2 (USB_DMACR2) */
typedef struct{
__REG16 UNLNK_TX_EP_ID1  : 4;
__REG16 UNLNK_TX_EP_ID2  : 4;
__REG16 UNLNK_TX_EP_ID3  : 4;
__REG16 DMA_UNLK_TX_EN   : 2;
__REG16                  : 2;
} __usb_dmacr2_bits;

/* DMA control register 3 (USB_DMACR3) */
typedef struct{
__REG16 LK_TX_EN      : 1;
__REG16 LK_TX_EP_ID   : 4;
__REG16 DMA_LK_TX_EN  : 1;
__REG16 SLE           : 1;
__REG16               : 1;
__REG16 LK_RX_EN      : 1;
__REG16 LK_RX_EP_ID   : 4;
__REG16 DMA_LK_RX_EN  : 1;
__REG16 SR_TX         : 1;
__REG16 SR_RX         : 1;
} __usb_dmacr3_bits;

/* DMA burst size register (USB_DMABSIZE) */
typedef struct{
__REG16 SBSIZE           : 3;
__REG16                  : 1;
__REG16 DBSIZE           : 3;
__REG16                  : 1;
__REG16 LLI_RX_NPACKETS  : 8;
} __usb_dmabsize_bits;

/* DMA LLI register (USB_DMALLI) */
typedef struct{
__REG16 LLI_TX_LNG  : 8;
__REG16 LLI_RX_LNG  : 8;
} __usb_dmalli_bits;

/***************************************************************************
 **
 **  USB
 **
 ***************************************************************************/
__IO_REG16_BIT(USB_EP0R,            0x70000800,__READ_WRITE,__usb_epr_bits);
__IO_REG16_BIT(USB_EP1R,            0x70000804,__READ_WRITE,__usb_epr_bits);
__IO_REG16_BIT(USB_EP2R,            0x70000808,__READ_WRITE,__usb_epr_bits);
__IO_REG16_BIT(USB_EP3R,            0x7000080C,__READ_WRITE,__usb_epr_bits);
__IO_REG16_BIT(USB_EP4R,            0x70000810,__READ_WRITE,__usb_epr_bits);
__IO_REG16_BIT(USB_EP5R,            0x70000814,__READ_WRITE,__usb_epr_bits);
__IO_REG16_BIT(USB_EP6R,            0x70000818,__READ_WRITE,__usb_epr_bits);
__IO_REG16_BIT(USB_EP7R,            0x7000081C,__READ_WRITE,__usb_epr_bits);
__IO_REG16_BIT(USB_EP8R,            0x70000820,__READ_WRITE,__usb_epr_bits);
__IO_REG16_BIT(USB_EP9R,            0x70000824,__READ_WRITE,__usb_epr_bits);
__IO_REG16_BIT(USB_CNTR,            0x70000840,__READ_WRITE,__usb_cntr_bits);
__IO_REG16_BIT(USB_ISTR,            0x70000844,__READ_WRITE,__usb_st_bits);
__IO_REG16_BIT(USB_FNR,             0x70000848,__READ      ,__usb_fnr_bits);
__IO_REG16_BIT(USB_DADDR,           0x7000084C,__READ_WRITE,__usb_daddr_bits);
__IO_REG16_BIT(USB_BTABLE,          0x70000850,__READ_WRITE,__usb_btable_bits);
__IO_REG16_BIT(USB_DMACR1,          0x70000854,__READ_WRITE,__usb_dmacr1_bits);
__IO_REG16_BIT(USB_DMACR2,          0x70000858,__READ_WRITE,__usb_dmacr2_bits);
__IO_REG16_BIT(USB_DMACR3,          0x7000085C,__READ_WRITE,__usb_dmacr3_bits);
__IO_REG16_BIT(USB_DMABSIZE,        0x70000860,__READ_WRITE,__usb_dmabsize_bits);
__IO_REG16_BIT(USB_DMALLI,          0x70000864,__READ_WRITE,__usb_dmalli_bits);

#define AddrTxOffset    0
#define AddrRxOffset    2
#define CountTxOffset   4
#define CountRxOffset   6

#define USB_RESUME_DLY  8

#define USB_OffsetOfDBT ((Int32U) __segment_size("USB_PACKET_MEMORY") - (sizeof(USB_BuffDeskTbl_t) * (MaxIndOfRealizeEp+1)) & ~7)

#define ReadEpDTB(Slot,Offset)  (*((pInt16U)((Int32U)__segment_begin("USB_PACKET_MEMORY") + (USB_OffsetOfDBT + (Slot)*sizeof(USB_BuffDeskTbl_t)  + (Offset)))))

#define ReadEpDTB_AddrRx(Slot)        ReadEpDTB(Slot,AddrRxOffset)
#define ReadEpDTB_CountRx(Slot)       ReadEpDTB(Slot,CountRxOffset)
#define ReadEpDTB_AddrTx(Slot)        ReadEpDTB(Slot,AddrTxOffset)
#define ReadEpDTB_CountTx(Slot)       ReadEpDTB(Slot,CountTxOffset)

#define WriteEpDTB_AddrRx(Slot,Data)  WriteEpDTB(Slot,AddrRxOffset,Data)
#define WriteEpDTB_CountRx(Slot,Data) WriteEpDTB(Slot,CountRxOffset,Data)
#define WriteEpDTB_AddrTx(Slot,Data)  WriteEpDTB(Slot,AddrTxOffset,Data)
#define WriteEpDTB_CountTx(Slot,Data) WriteEpDTB(Slot,CountTxOffset,Data)

#define USB_CTRL_RW_MASK              0x070F
#define USB_CTRL_CLEAR_ONLY_MASK      0x8080
#define USB_CTRL_TOGGLE_MASK          0x7070

typedef enum _USB_PacketType_t
{
	// Packet type parameters
  UsbSetupPacket = 0,UsbDataOutPacket,UsbDataInPacket,UsbDmaPacket,
} USB_PacketType_t;

typedef enum _USB_ErrorCodes_t
{
  USB_OK = 0,USB_PLL_ERROR, USB_INTR_ERROR,
  USB_EP_OCCUPIER, USB_MEMORY_FULL, USB_EP_NOT_VALID,
  USB_EP_OVERRUN, UB_EP_SETUP_UNDERRUN,
  USB_EP_STALLED, UB_EP_SETUP_OVERWRITE,
  USB_EP_FATAL_ERROR,
} USB_ErrorCodes_t;

typedef enum _EpType_t
{
  EP_BULK_SING_BUFF = 0, EP_CTRL, EP_ISO,
  EP_INTERRUPT, EP_BULK_DOUB_BUF,
} EpType_t;

typedef enum _EpSlot_t
{
  EP_SLOT0 = 0, EP_SLOT1, EP_SLOT2, EP_SLOT3, EP_SLOT4,
  EP_SLOT5, EP_SLOT6, EP_SLOT7, EP_SLOT8, EP_SLOT9,
  EP_MAX_SLOTS
} EpSlot_t;

typedef enum _EpState_t
{
  EP_DISABLED = 0, EP_STALL, EP_NAK, EP_VALID
} EpState_t;

typedef enum _USB_Endpoint_t
{
  CTRL_ENP_OUT=0, CTRL_ENP_IN,
  ENP1_OUT      , ENP1_IN    ,
  ENP2_OUT      , ENP2_IN    ,
  ENP3_OUT      , ENP3_IN    ,
  ENP4_OUT      , ENP4_IN    ,
  ENP5_OUT      , ENP5_IN    ,
  ENP6_OUT      , ENP6_IN    ,
  ENP7_OUT      , ENP7_IN    ,
  ENP8_OUT      , ENP8_IN    ,
  ENP9_OUT      , ENP9_IN    ,
  ENP10_OUT     , ENP10_IN   ,
  ENP11_OUT     , ENP11_IN   ,
  ENP12_OUT     , ENP12_IN   ,
  ENP13_OUT     , ENP13_IN   ,
  ENP14_OUT     , ENP14_IN   ,
  ENP15_OUT     , ENP15_IN   ,
  ENP_MAX_NUMB
} USB_Endpoint_t;

typedef enum _UsbUserEvent_t
{
  UsbEp0Out = 0 , UsbEp0In,
  UsbEp1Out     , UsbEp1In,
  UsbEp2Out     , UsbEp2In,
  UsbEp3Out     , UsbEp3In,
  UsbEp4Out     , UsbEp4In,
  UsbEp5Out     , UsbEp5In,
  UsbEp6Out     , UsbEp6In,
  UsbEp7Out     , UsbEp7In,
  UsbEp8Out     , UsbEp8In,
  UsbEp9Out     , UsbEp9In,
  UsbEp10Out    , UsbEp10In,
  UsbEp11Out    , UsbEp11In,
  UsbEp12Out    , UsbEp12In,
  UsbEp13Out    , UsbEp13In,
  UsbEp14Out    , UsbEp14In,
  UsbEp15Out    , UsbEp15In,
  UsbClassEp0OutPacket,
  UsbVendorEp0OutPacket,
  UsbUserSofEvent,
  UsbUserClass,
  UsbUserVendor,
  UsbUserGetDescriptor,
  UsbUserConfigure,
  UsbUserReset,
  UsbUserConnect,
  UsbUserSuspend,
  UsbUserEpStall,
  UsbUserEpUnStall,
  UsbLastEvent,
}UsbUserEvent_t;

typedef enum _USB_DevStatusReqType_t
{
  USB_DevConnectStatus = 0, USB_SuspendStatus, USB_ResetStatus
} USB_DevStatusReqType_t;

typedef enum _UsbResumeEvent_t
{
  USB_RESUME_SOF_EVENT = 0, USB_RESUME_SOFT_EVENT,
  USB_RESUME_WAKE_UP_EVENT,
} UsbResumeEvent_t, *pUsbResumeEvent_t;

typedef union _RxCount_t
{
  Int16U Count;
  struct
  {
    Int16U CountField     : 10;
    Int16U NubBlockField  :  5;
    Int16U BlSizeField    :  1;
  };
} RxCount_t, *pRxCount_t;

typedef struct _PacketMemUse_t
{
  USB_Endpoint_t RpAddr;
  Int16U         Start;
  Int16U         Size;
  struct _PacketMemUse_t * pNext;
} PacketMemUse_t, *pPacketMemUse_t;

typedef struct _EpInfo_t
{
  volatile pInt32U    pEpCtrl;
           EpSlot_t   EpSlot;
           Int32U     MaxSize;
           EpType_t   EpType;
           Int32U     EpIntrFl;
} EpInfo_t, *pEpInfo_t;


#pragma pack(1)
typedef struct _USB_BuffDeskTbl_t
{
  Int16U    AddrTx;
  Int16U    AddrRx;
  Int16U    CountTx;
  RxCount_t CountRx;
} USB_BuffDeskTbl_t, *pUSB_BuffDeskTbl_t;
#pragma pack()

typedef union _pIntrStatus_t
{
  Int32U Status;
  struct {
    Int32U EP_ID  : 4;
    Int32U DIR    : 1;
    Int32U        : 2;
    Int32U SZDPR  : 1;
    Int32U ESOF   : 1;
    Int32U SOF    : 1;
    Int32U RESET  : 1;
    Int32U SUSP   : 1;
    Int32U WKUP   : 1;
    Int32U ERR    : 1;
    Int32U DOVR   : 1;
    Int32U CTR    : 1;
    Int32U        :16;
  };
} IntrStatus_t, *pIntrStatus_t;

typedef Int32U UsbDefStatus_t;
typedef void * (* UserFunc_t)(void * Arg);

#define bmCTRM                      0x8000
#define bmDOVRM                     0x4000
#define bmERRM                      0x2000
#define bmWKUPM                     0x1000
#define bmSUSPM                     0x0800
#define bmRESETM                    0x0400
#define bmSOFM                      0x0200
#define bmESOFM                     0x0100

/*************************************************************************
 * Function Name: EpCtrlSet_EA
 * Parameters: volatile pInt32U pReg, Int32U Data
 *
 * Return: none
 *
 * Description: Set EP Addr
 *
 *************************************************************************/
void EpCtrlSet_EA(volatile pInt32U pReg, Int32U Data);

/*************************************************************************
 * Function Name: EpCtrlGet_EA
 * Parameters: volatile pInt32U pReg
 *
 * Return: Int16U
 *
 * Description: Get EP Addr
 *
 *************************************************************************/
inline
Int16U EpCtrlGet_EA(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlSet_STAT_TX
 * Parameters: volatile pInt32U pReg, EpState_t Data
 *
 * Return: none
 *
 * Description: Set EP IN State
 *
 *************************************************************************/
void EpCtrlSet_STAT_TX (volatile pInt32U pReg, EpState_t Data);

/*************************************************************************
 * Function Name: EpCtrlGet_STAT_TX
 * Parameters: volatile pInt32U pReg
 *
 * Return: Int32U
 *
 * Description: Get EP IN Status
 *
 *************************************************************************/
inline
Int32U EpCtrlGet_STAT_TX (volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlSet_DTOG_TX
 * Parameters: volatile pInt32U pReg, Int32U Data
 *
 * Return: none
 *
 * Description: Set EP DTOG_TX bit (IN)
 *
 *************************************************************************/
void EpCtrlSet_DTOG_TX(volatile pInt32U pReg, Int32U Data);

/*************************************************************************
 * Function Name: EpCtrlToggle_DTOG_TX
 * Parameters: volatile pInt32U pReg
 *
 * Return: none
 *
 * Description: Toggle EP DTOG_TX bit (IN)
 *
 *************************************************************************/
void EpCtrlToggle_DTOG_TX(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlGet_DTOG_TX
 * Parameters: volatile pInt32U pReg
 *
 * Return: Int32U
 *
 * Description: Get EP DTOG_TX bit (IN)
 *
 *************************************************************************/
inline
Int32U EpCtrlGet_DTOG_TX(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlClr_CTR_TX
 * Parameters: volatile pInt32U pReg
 *
 * Return: none
 *
 * Description: Clear EP CTR_TX bit (IN)
 *
 *************************************************************************/
void EpCtrlClr_CTR_TX(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlGet_CTR_TX
 * Parameters: volatile pInt32U pReg
 *
 * Return: UserFunc_t
 *
 * Description: Get EP CTR_TX bit (IN)
 *
 *************************************************************************/
inline
Int32U EpCtrlGet_CTR_TX(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlSet_EP_KIND
 * Parameters: volatile pInt32U pReg, Int32U Data
 *
 * Return: none
 *
 * Description: Set EP KIND bit
 *
 *************************************************************************/
void EpCtrlSet_EP_KIND (volatile pInt32U pReg, Int32U Data);

/*************************************************************************
 * Function Name: EpCtrlGet_EP_KIND
 * Parameters: volatile pInt32U pReg
 *
 * Return: Int32U
 *
 * Description: Get EP KIND bit
 *
 *************************************************************************/
inline
Int32U EpCtrlGet_EP_KIND(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlSet_EP_TYPE
 * Parameters: volatile pInt32U pReg, EpType_t Data
 *
 * Return: none
 *
 * Description: Set EP type
 *
 *************************************************************************/
void EpCtrlSet_EP_TYPE(volatile pInt32U pReg, EpType_t Data);

/*************************************************************************
 * Function Name: EpCtrlGet_EP_TYPE
 * Parameters: volatile pInt32U pReg
 *
 * Return: Int32U
 *
 * Description: Get EP type
 *
 *************************************************************************/
inline
Int32U EpCtrlGet_EP_TYPE(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlGet_SETUP
 * Parameters: volatile pInt32U pReg
 *
 * Return: Int32U
 *
 * Description: Get EP SETUP bit
 *
 *************************************************************************/
inline
Int32U EpCtrlGet_SETUP(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlSet_STAT_RX
 * Parameters: volatile pInt32U pReg, EpState_t Data
 *
 * Return: none
 *
 * Description: Set EP STAT_RX (OUT)
 *
 *************************************************************************/
void EpCtrlSet_STAT_RX (volatile pInt32U pReg, EpState_t Data);

/*************************************************************************
 * Function Name: EpCtrlGet_STAT_RX
 * Parameters: volatile pInt32U pReg
 *
 * Return: Int32U
 *
 * Description: Get EP STAT_RX (OUT)
 *
 *************************************************************************/
inline
Int32U EpCtrlGet_STAT_RX(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlSet_DTOG_RX
 * Parameters: volatile pInt32U pReg, Int32U Data
 *
 * Return: none
 *
 * Description: Set DTOG_RX bit
 *
 *************************************************************************/
void EpCtrlSet_DTOG_RX(volatile pInt32U pReg, Int32U Data);

/*************************************************************************
 * Function Name: EpCtrlToggle_DTOG_RX
 * Parameters: volatile Int16U pReg
 *
 * Return: none
 *
 * Description: Toggle DTOG_RX bit
 *
 *************************************************************************/
void EpCtrlToggle_DTOG_RX(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlGet_DTOG_RX
 * Parameters: volatile pInt32U pReg
 *
 * Return: Int32U
 *
 * Description: Get EP DTOG_RX bit (OUT)
 *
 *************************************************************************/
inline
Int32U EpCtrlGet_DTOG_RX(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlClr_CTR_RX
 * Parameters: volatile pInt32U pReg
 *
 * Return: none
 *
 * Description: Clear EP CTR_RX bit (OUT)
 *
 *************************************************************************/
void EpCtrlClr_CTR_RX(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: EpCtrlGet_CTR_RX
 * Parameters: volatile pInt32U pReg
 *
 * Return: Int32U
 *
 * Description: Get EP CTR_RX bit (OUT)
 *
 *************************************************************************/
inline
Int32U EpCtrlGet_CTR_RX(volatile pInt32U pReg);

/*************************************************************************
 * Function Name: USB_UserFuncRegistering
 * Parameters: UserFunc_t UserFunc, UsbUserEvent_t UserFuncInd
 *
 * Return: UserFunc_t
 *
 * Description: Registered User callback function
 *
 *************************************************************************/
UserFunc_t USB_UserFuncRegistering (UserFunc_t UserFunc, UsbUserEvent_t UserFuncInd);

/*************************************************************************
 * Function Name: UsbCoreReq
 * Parameters:  UsbCoreReqType_t Type
 *
 * Return: Int32U
 *
 * Description: Return device states
 *
 *************************************************************************/
Int32U UsbCoreReq (UsbCoreReqType_t Type);

/*************************************************************************
 * Function Name: UsbWakeUp
 * Parameters:  none
 *
 * Return: none
 *
 * Description: WakeUp device from suspend mode
 *
 *************************************************************************/
void UsbWakeUp (void);

/*************************************************************************
 * Function Name: USB_Init
 * Parameters: Int32U IntrSlotHi, Int32U IntrSlotLo,
 *             CommUserFpnt_t UserCoreConfigure
 *
 * Return: none
 *
 * Description: Init USB
 *
 *************************************************************************/
void USB_Init(Int32U IntrSlotHi, Int32U IntrSlotLo,
              CommUserFpnt_t UserCoreConfigure);

/*************************************************************************
 * Function Name: USB_HwReset
 * Parameters: none
 *
 * Return: none
 *
 * Description: Reset Usb engine
 *
 *************************************************************************/
void USB_HwReset (void);

/*************************************************************************
 * Function Name: USB_RealizeEp
 * Parameters: USB_Endpoint_t EndPoint, Int32U MaxPacketSize,
 *             EpType_t EpKind, EpSlot_t EpSlot,
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: Enable or disable endpoint
 *
 *************************************************************************/
USB_ErrorCodes_t USB_RealizeEp( USB_Endpoint_t EndPoint, Int32U MaxPacketSize,
                                EpType_t EpType, EpSlot_t EpSlot);

/*************************************************************************
 * Function Name: USB_SetAdd
 * Parameters: Int32U DevAdd - device address between 0 - 127
 *
 * Return: none
 *
 * Description: Set device address
 *
 *************************************************************************/
void USB_SetAdd(Int32U DevAdd);
#define USB_SetDefAdd() USB_SetAdd(0)

/*************************************************************************
 * Function Name: USB_ConnectRes
 * Parameters: Boolean Conn
 *
 * Return: none
 *
 * Description: Enable Pull-Up resistor
 *
 *************************************************************************/
void USB_ConnectRes (Boolean Conn);

/*************************************************************************
 * Function Name: USB_GetDevStatus
 * Parameters: USB_DevStatusReqType_t Type
 *
 * Return: Boolean
 *
 * Description: Return USB device status
 *
 *************************************************************************/
Boolean USB_GetDevStatus (USB_DevStatusReqType_t Type);

/*************************************************************************
 * Function Name: USB_EpValidate
 * Parameters: USB_Endpoint_t EndPoint, Boolean State
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: Validate/Unvalidate EP buffer
 *
 *************************************************************************/
USB_ErrorCodes_t USB_EpValidate( USB_Endpoint_t EndPoint, Boolean State);

/*************************************************************************
 * Function Name: USB_EpWrite
 * Parameters: USB_Endpoint_t EndPoint, pInt8U pData, pInt32U pCount
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: Endpoint Write (IN)
 *
 *************************************************************************/
USB_ErrorCodes_t USB_EpWrite (USB_Endpoint_t EndPoint, pInt8U pData,
                              pInt32U pCount);

/*************************************************************************
 * Function Name: USB_EpRead
 * Parameters: USB_Endpoint_t EndPoint, pInt8U pData, pInt32U pCount
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: Endpoint Read (OUT)
 *
 *************************************************************************/
USB_ErrorCodes_t USB_EpRead (USB_Endpoint_t EndPoint, pInt8U pData,
                             pInt32U pCount);

/*************************************************************************
 * Function Name: USB_EpLogToPhysAdd
 * Parameters: Int8U EpLogAdd
 *
 * Return: USB_Endpoint_t
 *
 * Description: Convert the logical to physical address
 *
 *************************************************************************/
USB_Endpoint_t USB_EpLogToPhysAdd (Int8U EpLogAdd);

#if USB_SOF_EVENT > 0
/*************************************************************************
 * Function Name: USB_GetFrameNumb
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Return current value of SOF number
 *
 *************************************************************************/
Int32U USB_GetFrameNumb (void);
#endif // USB_SOF_EVENT > 0


#if USB_HIGH_PRIORITY_EVENT > 0
/*************************************************************************
 * Function Name: USB_HIGH_ISR
 * Parameters: none
 *
 * Return: none
 *
 * Description: High priority USB interrupt subroutine
 *
 *************************************************************************/
void USB_HIGH_ISR (void);
#endif // USB_HIGH_PRIORITY_EVENT > 0

/*************************************************************************
 * Function Name: USB_ISR
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB interrupt subroutine
 *
 *************************************************************************/
void USB_ISR (void);

/*************************************************************************
 * Function Name: USB_WakeUp
 * Parameters: none
 *
 * Return: none
 *
 * Description: Wake up Usb
 *
 *************************************************************************/
static
void USB_WakeUp (void);

/*************************************************************************
 * Function Name: USB_Configure
 * Parameters: Boolean Configure
 *
 * Return: none
 *
 * Description: Configure device
 *  When Configure != 0 enable all Realize Ep
 *
 *************************************************************************/
static
void USB_Configure (Boolean Configure);

/*************************************************************************
 * Function Name: USB_SetStallEP
 * Parameters: USB_Endpoint_t EndPoint, Boolean Stall
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: The endpoint stall/unstall
 *
 *************************************************************************/
USB_ErrorCodes_t USB_SetStallEP (USB_Endpoint_t EndPoint, Boolean Stall);

/*************************************************************************
 * Function Name: USB_GetStallEP
 * Parameters: USB_Endpoint_t EndPoint, pBoolean pStall
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: Get stall state of the endpoint
 *
 *************************************************************************/
static
USB_ErrorCodes_t USB_GetStallEP (USB_Endpoint_t EndPoint, pBoolean pStall);

/*************************************************************************
 * Function Name: USB_Resume
 * Parameters: UsbResumeEvent_t UsbResumeEvent
 *
 * Return: none
 *
 * Description: USB Resume implement
 *
 *************************************************************************/
static
void USB_Resume (UsbResumeEvent_t UsbResumeEvent);

/*************************************************************************
 * Function Name: UsbCtrlEp
 * Parameters:  void * pArg
 *
 * Return: none
 *
 * Description: USB Ctrl EP Callback
 *
 *************************************************************************/
static inline
void UsbCtrlEp (USB_PacketType_t Packet);

#endif //__STR91X_USB_H
