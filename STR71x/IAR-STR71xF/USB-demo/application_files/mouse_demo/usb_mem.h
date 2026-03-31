/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : usb_mem.h
* Author             : MCD Application Team
* Date First Issued  : 27/10/2003
* Description        : utility functions for memory transfers
*
********************************************************************************/


/* function prototypes */
void UserToPMABufferCopy(BYTE *pbUsrBuf,WORD wPMABufAddr, WORD wNBytes);
void PMAToUserBufferCopy(BYTE *pbUsrBuf,WORD wPMABufAddr, WORD wNBytes);
void AddQPointer(WORD *pwQPnt,WORD wNBytes,WORD wQMaxSize);

