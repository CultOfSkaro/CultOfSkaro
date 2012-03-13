/******************************************************************************

FILE:    ServoControl.c
AUTHOR:  Travis Haroldsen
CREATED: 02/04/10

DESCRIPTION

Software to interface with the USB.  It includes functions for 
setting parameters of the USB core and writing and reading to and
from the USB.


******************************************************************************/


#include "xparameters.h"

#include "xbasic_types.h"

#include "USB_IO.h"


void USB_init() {
	XIo_Out32(USB_BASEADDR + USB_WR_BURSTSIZE_OFFSET, 16);
	XIo_Out32(USB_BASEADDR + USB_WR_STARTADDR_OFFSET, 0);
	XIo_Out32(USB_BASEADDR + USB_WR_XFERSIZE_OFFSET, 0);
}

Xuint32 USB_Connected()
{
	return ((USB_getStatus() & USB_CONNECTED) != 0);
}
Xuint32 USB_Busy()
{
	return ((USB_getStatus() & USB_DMA_SESSION_ACTIVE) != 0);
}
/*
*	returns if the USB controller is available to be written to
*/
u32 USB_writeReady() {
	return ((USB_getStatus() & USB_WRITE_STATUS) == USB_VALID_WRITE);
}



/*
*	returns if valid read data is present in the USB controller.
*/
u32 USB_readReady() {
	return ((USB_getStatus() & USB_READ_STATUS) == USB_VALID_READ);
}



/*
	Read the next 16 bit word in the USB fifo.  No error checking is
	performed and will return a value even if there is no valid data
	to read.
	
	return : next 16 bit word
*/
u16 USB_readWord() {
	return (u16) XIo_In32(USB_BASEADDR + USB_RD_DATAREAD_OFFSET);
}



/*
	Reads nWords 16 bit words from the USB and stores them into memory starting at
	startAddr.  USB_burstRead16 will read upto nWords are read or until no more data
	is available over the USB.  USB_burstRead16 then returns the number of words read.
	
	params :
		startAddr - start of address to store data into
		
		nWords - number of 16 bit words to read in
		
	returns :
		number of 16 bit words read
*/
u32 USB_burstRead16(u16* startAddr, u32 nWords) {
	if(!USB_Connected())
		return -1;
	u32 i;

	for (i = 0; i <nWords; ++i) {
		if ((USB_getStatus() & USB_READ_STATUS) != USB_VALID_READ)
			return i;

		*startAddr = XIo_In32(USB_BASEADDR + USB_RD_DATAREAD_OFFSET);
		++startAddr;
	}
	
	return i;
}



/*
	Reads nDWords 32 bit double words from the USB and stores them into memory starting at
	startAddr.  USB_burstRead32 will continue until no more words are available.  If only
	the first half of the Dword is available when run, USB_burstRead32 will stall until the
	second half becomes available.
	
	params :
		startAddr - start of address to store data into
		
		nWords - number of 32 bit double words to read in
		
	return :
		number of Dwords read
*/
u32 USB_burstRead32(u32* startAddr, u32 nDWords) {
	u32 i;
	if(!USB_Connected())
		return -1;
	
	for (i = 0; i <nDWords; i++) {
		if ((USB_getStatus() & USB_READ_STATUS) != USB_VALID_READ)
			return i;
		
		u32 firstWord = XIo_In32(USB_BASEADDR + USB_RD_DATAREAD_OFFSET) << 16;
		
		while (USB_getStatus() & USB_READ_STATUS != USB_VALID_READ);
		
		*startAddr = firstWord + XIo_In32(USB_BASEADDR + USB_RD_DATAREAD_OFFSET);
		++startAddr;
	}
	
	return i;
}



/*
	forces the USB to send data in buffer even if buffer is not full
*/
void USB_forceCommit() {
	if(!USB_Connected())
		return;

	if ((USB_getStatus() & USB_WRITE_STATUS) == USB_VALID_WRITE)
		XIo_Out32(USB_BASEADDR + USB_WR_SEND_OFFSET, USB_COMMIT);
}



/*
	write a single 16 bit word to the USB.  The function will wait
	until the USB is available to write to.
	
	params
		data - 16 bit word to be sent throught the USB
*/
void USB_singleWrite(u16 data) {
	if(!USB_Connected())
		return;

	while ((USB_getStatus() & USB_WRITE_STATUS) != USB_VALID_WRITE);
	
	XIo_Out32(USB_BASEADDR + USB_WR_DATAWRITE_OFFSET, ((u32) data) << 16);
}
/* non-burst multi-16-bit-write */
void USB_multiWrite(Xuint16* data, int numWords)
{
	int i;
	if(!USB_Connected())
		return;

	for(i = 0; i < numWords; i++)
	{
		USB_singleWrite(data[i]);
	}
	//	USB_forceCommit();

}


/*
	Sets the length of the burst reads across the PLB made by the USB controller
	during burst writes.  Larger values will yield better efficieny at the expense
	of longer bus latencies.
	
	params :
		size - length of burst in 16 bit words, valid values are 8, 16, 32, 64, 128
			recommended : 128
	
	returns :
		1 if successful
		0 if unsuccessful
*/
int USB_setBurstSize(u32 size) {
	if(!USB_Connected())
		return 0;

	if (size == 128 || size == 64 || size == 32
		|| size == 16 || size == 8) {
	
		XIo_Out32(USB_BASEADDR + USB_WR_BURSTSIZE_OFFSET, size / 8);
		return 1;
	} else {
		return 0;
	}
}



/*
	writes xferSize number of 16 bit words starting at startAddr.  The USB controller
	will access memory and alternate between reading and sending data across the USB.
	The lengths of each read can be set by USB_setBurstSize.  USB_blockWrite will
	wait for the USB to be ready before writing.
	
	params :
		startAddr - the beginning address to read data from
		
		xferSize - the number of 16 bit words to transfer.
*/
int USB_blockWrite(u32* startAddr, u32 xferSize) {
	if(!USB_Connected())
		return 0;

	if (xferSize % 2 != 0)
		return 0;
	
	XIo_Out32(USB_BASEADDR + USB_WR_STARTADDR_OFFSET, startAddr);
	XIo_Out32(USB_BASEADDR + USB_WR_XFERSIZE_OFFSET, xferSize);

	while ((USB_getStatus() & USB_WRITE_STATUS) != USB_VALID_WRITE);
	
	XIo_Out32(USB_BASEADDR + USB_WR_SEND_OFFSET, USB_BURST_XFER);
		
	XIo_Out32(USB_BASEADDR + USB_WR_STARTADDR_OFFSET, 0);
	XIo_Out32(USB_BASEADDR + USB_WR_XFERSIZE_OFFSET, 0);
	
	return 1;
}
