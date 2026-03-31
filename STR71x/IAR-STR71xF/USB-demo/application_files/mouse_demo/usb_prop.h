/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : usb_prop.h
* Author             : MCD Application Team
* Date First Issued  : 27/10/2003
* Description        : All processings related to Mouse device
*
********************************************************************************/

void MOUSE_init(void);
void MOUSE_Reset(void);
#define MOUSE_Status_In	NOP_Process
#define MOUSE_Status_Out	NOP_Process
RESULT MOUSE_Data_Setup(BYTE);
RESULT MOUSE_NoData_Setup(BYTE);
BYTE *MOUSE_GetDeviceDescriptor(WORD );
BYTE *MOUSE_GetConfigDescriptor(WORD);
BYTE *MOUSE_GetStringDescriptor(WORD);
