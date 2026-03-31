/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : usb_pwr.h
* Author             : MCD Application Team
* Date First Issued  : 27/10/2003
* Description        : connection/disconnection & power management
*
********************************************************************************/

typedef enum _RESUME_STATE{
	RESUME_EXTERNAL,
	RESUME_INTERNAL,
	RESUME_LATER,
	RESUME_WAIT,
	RESUME_START,
	RESUME_ON,
	RESUME_OFF,
	RESUME_ESOF
} RESUME_STATE;

typedef enum _DEVICE_STATE{
	UNCONNECTED,
	ATTACHED,
	POWERED,
	DEFAULT,
	ADDRESSED,
	CONFIGURED
} DEVICE_STATE;

extern volatile BYTE bDeviceState; /* USB device status */
extern DWORD dwLedVal; /* alpha-display value */
extern volatile BOOL fSuspendEnabled;  /* true when suspend is possible */

/* function prototypes */
void SetFPGAClock(DWORD dwClkVal);
void Suspend(void);
void Resume_Init(void);
void Resume(RESUME_STATE eResumeSetVal);
void WriteAlphaDisp(DWORD dwLedVal);
void DisplayDevState(void);
