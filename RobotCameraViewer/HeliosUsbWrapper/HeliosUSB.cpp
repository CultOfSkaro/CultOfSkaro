/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    HeliosUSB.cpp
AUTHOR:  Wade Fife
CREATED: 3/2/06

DESCRIPTION

This module provides easy access to the yhelios USB driver, a version of the
generic Cypress USB driver. USBOpenHelios() and USBCloseHelios() open and close
the driver connection. Use USBReadHelios() and USBWriteHelios() to read and
write to the Helios board.

******************************************************************************/

#include "StdAfx.h"
#include "HeliosUSB.h"
#pragma comment (lib, "Setupapi.lib")

// DEFINES ///////////////////////////////////////////////////////////////////

#define USB_OUT_TIMEOUT  2000
#define USB_IN_TIMEOUT   2000



// GLOBALS ///////////////////////////////////////////////////////////////////

//{f0f3c967-4eb9-11e1-8ea9-0018716a659d}
//static const GUID yHeliosGUID = {0xf0f3c967, 0x4eb9, 0x11e1, 0x8e, 0xa9, 0x00, 0x18, 0x71, 0x6a, 0x65, 0x9d };
// {AE18AA60-7F6A-11d4-97DD-00010229B959}
//static const GUID yHeliosGUID = {0xAE18AA60, 0x7F6A, 0x11d4, 0x97, 0xDD, 0x00, 0x01, 0x02, 0x29, 0xB9, 0x59 };
// {AE18A550-7F6A-11d4-97DD-00010229B959}
//static const GUID yHeliosGUID = {0xAE18A550, 0x7F6A, 0x11d4, 0x97, 0xDD, 0x00, 0x01, 0x02, 0x29, 0xB9, 0x59 };
// Specify GUID for yhelios device driver
// {90CC1BBE-EAAC-48a0-9D48-295612AA9715}
static const GUID yHeliosGUID = {0x90CC1BBE, 0xEAAC, 0x48a0, 0x9D, 0x48, 0x29, 0x56, 0x12, 0xAA, 0x97, 0x15 };

CCyUSBDevice* USBDevice = NULL;
static CCyUSBEndPoint* USBOutEndPt = NULL;
static CCyUSBEndPoint* USBInEndPt = NULL;



// FUNCTIONS /////////////////////////////////////////////////////////////////


// Opens the Cypress USB device driver (yhelios) and connects to Helios.
// Parameter hwnd may be NULL if it is not desired.
bool USBOpenHelios(HWND hwnd)
{
	// Make sure not already open
	if(USBDevice) {
		USBCloseHelios();
	}

	// Open a USB Device
	//USBDevice = new CCyUSBDevice(NULL);
	USBDevice = new CCyUSBDevice(hwnd, yHeliosGUID);
	if(USBDevice) {
	// Attempt to open device 0
		int b;
		if((b = USBDevice->DeviceCount()) && !USBDevice->Open(0)) {
			USBDevice->Reset();
			USBDevice->Open(0);
		}

		if(USBDevice->IsOpen()) {
			USBOutEndPt = USBDevice->BulkOutEndPt;
			if(USBOutEndPt != NULL) USBOutEndPt->TimeOut = USB_OUT_TIMEOUT;
			if(USBOutEndPt != NULL) USBOutEndPt->Reset();

			USBInEndPt = USBDevice->BulkInEndPt;
			if(USBInEndPt != NULL) USBInEndPt->TimeOut = USB_IN_TIMEOUT;
			if(USBInEndPt != NULL) USBInEndPt->Reset();

			if(USBOutEndPt != NULL && USBInEndPt != NULL) 
				return true;
		}
	} 

	// Couldn't load device so clean up
	USBCloseHelios();
	return false;
}



// Cleans up and closes USB connection.
void USBCloseHelios(void)
{
	if(USBDevice != NULL) {
		// Reset device to flush any active API calls in other threads
		USBDevice->Reset();
		delete USBDevice;
		USBDevice = NULL;
	}
}



// Writes data to Helios USB. Returns false if an error occurs.
// NOTE: If Helios USB chip is configured in word-wide mode (16-bit)
//       then you should only transfer multiples of 2 bytes.
bool USBWriteHelios(void *data, DWORD numBytes)
{
	LONG numBytesWritten;
	DWORD i = 0;

	while(i < numBytes) {
		numBytesWritten = numBytes - i; // Prepare alias
		if(!USBOutEndPt->XferData(&((BYTE *)data)[i], numBytesWritten)) {
			if(!USBDevice || !USBDevice->IsOpen()) return false;
		}
		i += numBytesWritten;
	}

	return true;
}



// Reads a packet from the Helios USB. The size of the packet is returned in numBytesRead.
// Returns false if an error occurs.
// NOTE: This function will block until a packet is received.
// NOTE: The dest buffer must be MAX_PACKET_SIZE or larger to avoid buffer overflow.
bool USBReadHelios(void *dest, DWORD *numBytesRead)
{
	LONG numBytes;

	do {
		numBytes = MAX_USB_PACKET_SIZE;  // Setup reference
		if(!USBInEndPt->XferData((BYTE *)dest, numBytes)) {
			if((!USBDevice) || (!(USBDevice->IsOpen()))) return false;
		}
	} while(numBytes == 0);

	*numBytesRead = numBytes;

	return true;
}
