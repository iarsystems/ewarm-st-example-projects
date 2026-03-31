/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : usb_mem.c
* Author             : MCD Application Team
* Date First Issued  : 27/10/2003
* Description        : utility functions for memory transfers
*
********************************************************************************/



/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#include "USB_lib.h"
#include "USB_mem.h"
/*=============================================================================*/
/*=============================================================================*/

/* ----------------------------------------------------------------------------*/
/* UserToPMABufferCopy */
/* copy a buffer from user memory area to packet memory area (PMA) */
/* IN : pbUsrBuf = pointer to user memory area */
/*      wPMABufAddr = address into PMA */
/*		wNBytes = no. of bytes to be copied */
/* OUT: none */
/* ----------------------------------------------------------------------------*/
void UserToPMABufferCopy(BYTE *pbUsrBuf,WORD wPMABufAddr, WORD wNBytes)
{
 BYTE *pbVal;
 WORD wVal;
 DWORD *pdwVal;
 int wNTrasf=wNBytes;

	 if((wNBytes) == 0) return;
	 pdwVal = (DWORD *)(wPMABufAddr*2 + PMAAddr);
	 while(TRUE)
	 {
		wVal = 0;
		pbVal = (BYTE *)&wVal;
	 	if((wNTrasf--) > 0)
	 	{
		 	*pbVal++ = *pbUsrBuf++;
	 	 	if((wNTrasf--) > 0)
	 	 	{
			 	*pbVal++ = *pbUsrBuf++;
	 	 	}
	 	}
		*pdwVal++ = (DWORD)wVal;
		if(wNTrasf == 0) return;
	 }/* while */

} /* UserToPMABufferCopy */

/* ----------------------------------------------------------------------------*/
/* PMAToUserBufferCopy */
/* copy a buffer from user memory area to packet memory area (PMA) */
/* IN : pbUsrBuf = pointer to user memory area */
/*      wPMABufAddr = address into PMA */
/*		wNBytes = no. of bytes to be copied */
/* OUT: none */
/* ----------------------------------------------------------------------------*/
void PMAToUserBufferCopy(BYTE *pbUsrBuf,WORD wPMABufAddr, WORD wNBytes)
{
 BYTE *pbVal;
 WORD wNTrasf=wNBytes;

	 if((wNBytes) == 0) return;
	 pbVal = (BYTE *)(wPMABufAddr*2 + PMAAddr);
	 while(TRUE)
	 {
	 	 *pbUsrBuf++ = *pbVal++;
	 	 if((--wNTrasf) == 0) return;
	 	 *pbUsrBuf++ = *pbVal++;
	 	 if((--wNTrasf) == 0) return;
		 pbVal++; pbVal++;
	 }/* while */

} /* PMAToUserBufferCopy */

/* ----------------------------------------------------------------------------*/
/* AddQPointer */
/* update a circular queue pointer */
/* IN : pwQPnt = pointer to circular queue pointer */
/*      wNBytes = number to be added */
/*		wQMaxSize = circular queue size */
/* OUT: none */
/* ----------------------------------------------------------------------------*/
void AddQPointer(WORD *pwQPnt,WORD wNBytes,WORD wQMaxSize)
{
 WORD wResult;
 	wResult = *pwQPnt;
	wResult += wNBytes;
	if(wResult > wQMaxSize-1)
		    wResult -= wQMaxSize;
	*pwQPnt = wResult;
}/* AddQPointer */

