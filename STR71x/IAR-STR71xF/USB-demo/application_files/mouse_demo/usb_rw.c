/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

PROJECT  : ARM7 USB Core
COMPILER : ADS 1.0

MODULE  :  USB_rw.c

AUTHOR :  Stefano Ballarin - TXT
-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

DESCRIPTION : 

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

MODIFICATIONS :

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#include "USB_lib.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	NAME:	UsbWrite
	INPUT:
		EPnum - the endpoint number to be sent
		pEPinfo - the address of an ENDPOINT_INFO structure
				This structure controls the data sending
	OUTPUT:
	DESCRIPTION:
		Calls pEPinfo->CopyData() to copy data to the transmit buffer and send them
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
void UsbWrite(BYTE EPnum, ENDPOINT_INFO *pEPinfo)
{
	BYTE	*DataBuffer;
	BYTE	*(*CopyRoutine)(WORD);
	WORD	Length;
	
	Length = pEPinfo->PacketSize;	   // get size of the packet 
	if (Length > pEPinfo->Usb_wLength) // how many data to be sent?
		Length = pEPinfo->Usb_wLength; // if less than packet size ...
									   // it is last packet
									   
	CopyRoutine = pEPinfo->CopyData;	 // get copy routine address
	DataBuffer = (*CopyRoutine)(Length); // get address of data buffer
//	mSetEPTx(EPnum, DataBuffer, Length);
//sb sistemare	SetEPTxAddr(EPnum, DataBuffer);		 // set parameters for dma transfer
	SetEPTxCount(EPnum, Length);

	pEPinfo->Usb_wLength -= Length;	   // update number of data to send
	pEPinfo->Usb_wOffset += Length;	   // update offset in data buffer

	SetEPTxStatus(EPnum, EP_TX_VALID);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	NAME:	UsbRead
	INPUT:
		Source - the address of endpoint receiving buffer
		pEPinfo - the address of an ENDPOINT_INFO structure
				This structure controls the data receiving
	OUTPUT:
	DESCRIPTION:
		Copy received data for endpoint Rx buffer to user buffer
		User buffer address is obtained by calling pEPinfo->CopyData()
		The buffer returned by CopyData(Length)	must be capable of Length bytes.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
void UsbRead0(BYTE EPnum, ENDPOINT_INFO *pEPinfo)
{
	WORD size = pEPinfo->PacketSize;	// get size of the packet
	BYTE *Buffer;						 
										
	if (size > pEPinfo->Usb_rLength) { // how many data to be received ?	
		size = pEPinfo->Usb_rLength;   // if less than packet size ...
		pEPinfo->PacketSize = size;	   // it is last packet	
	}									

	Buffer = (*pEPinfo->CopyData)(size); // get address of data buffer
//	mSetEPRx(EPnum, Buffer, size);
//sb sistemare	SetEPRxAddr(EPnum, Buffer);			 // set parameters for dma transfer
	SetEPRxCount(EPnum, size);
	SetEPRxStatus(EPnum, EP_RX_VALID);
}

void UsbRead(BYTE EPnum, ENDPOINT_INFO *pEPinfo)
{
	WORD Length;

	if (pEPinfo->Usb_rLength == 0)	// no data to transfer
		return;

	Length = pEPinfo->PacketSize - GetEPRxCount(EPnum);	  //get no. of received data
	if (Length == 0)
		return;

	pEPinfo->Usb_rLength -= Length;	  // update number of data to receive
	pEPinfo->Usb_rOffset += Length;	  // update offset in data buffer

	UsbRead0(EPnum, pEPinfo);		  // set parameters for dma transfer
}

