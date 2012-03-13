/**

FILE:				USBThread.h
VERSION:			
AUTHOR:				Barrett Edwards
CREATED:			13 July 2007
PLATFORM:			Helios Computing Platform
FRAMEWORK:			GUI - Backend
COMPANION FILES:	USBThread.cpp
DESCRIPTION:		



CHANGE LOG
*/ 

/* Conditional Inclusion ----------------------------------------------------*/
#pragma once




/* Includes -----------------------------------------------------------------*/
#include "stdafx.h"




/* Macros -------------------------------------------------------------------*/
#define MAX_USB_PACKET_SIZE				 512 
#define USB_OUT_TIMEOUT					1000 
#define USB_IN_TIMEOUT					1000
#define USBTHREAD_NO_BUFFER_SLEEP_TIME	  10
#define USB_TRANSFER_BLOCK_SIZE			 1201




/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
// Specify GUID for yhelios device driver {90CC1BBE-EAAC-48a0-9D48-295612AA9715}
static const GUID yHeliosGUID = {0x90CC1BBE, 0xEAAC, 0x48a0,{0x9D, 0x48, 0x29, 0x56, 0x12, 0xAA, 0x97, 0x15}};




/* External Definitions -----------------------------------------------------*/
class BufferStore;
class ReceiverThread;



/* Function Prototypes ------------------------------------------------------*/

class USBThread
{
public:
	USBThread(BufferStore* bs, ReceiverThread* rt);
	~USBThread();
	
	bool IsRunning() { return m_running; }

	bool Start();
	bool Stop();
	bool StopAndWait();

	bool Send(void *data, DWORD num_bytes);
	
protected:
	static UINT __cdecl ThreadFunction(LPVOID pParam);
	
	bool IsUSBOpen();
	bool USBOpen(HWND hwnd);
	void USBClose(void);
	bool USBWrite(void *data, DWORD numBytes);
	bool USBRead(void *dest, DWORD *numBytesRead, DWORD bufferSize);


protected:
	FILE*				m_debug_file;

	BufferStore*		m_bufferstore;
	ReceiverThread*		m_receiverthread;

	CCyUSBDevice*		USBDevice;

	volatile bool		m_running;
	CSemaphore*			m_semaphore_thread;
	
	friend class		CHeliosLibraryApp; // Allow the DLL App to close the USB on DLL Unload from memory
};





