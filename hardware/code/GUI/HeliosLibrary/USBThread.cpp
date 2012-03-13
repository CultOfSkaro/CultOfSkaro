
#include "stdafx.h"
#include "CyAPI.h"
#include "USBThread.h"
#include "BufferStore.h"
#include "ReceiverThread.h"

USBThread::USBThread(BufferStore* bs, ReceiverThread* rt)
{
	printf("USBThread::USBThread()\r\n");	

	m_debug_file		= NULL;//fopen("debugfile_usbthread.txt","w");
	
	USBDevice			= NULL;

	m_bufferstore		= bs;
	m_receiverthread	= rt;
	m_running			= false;

	m_semaphore_thread	= new CSemaphore(1,1,NULL,NULL);
}

USBThread::~USBThread()
{
	printf("USBThread::~USBThread()\r\n");
	m_running = false;

	USBClose();
	//m_semaphore_thread->Lock();	//Wait until the ThreadLoop stops before returning
	
	delete m_semaphore_thread;
	if(m_debug_file != NULL)
		fclose(m_debug_file);
}


bool USBThread::Start()
{
	//printf("USBThread::Start()\r\n");

	bool return_value = true;

	if(!m_running)
	{
		m_running = true;
		AfxBeginThread(ThreadFunction, (void*)this);
	}
	else 
	{
		return_value = false;
	}

	return return_value;
}

bool USBThread::Stop()
{
	printf("USBThread::Stop()\r\n");

	m_running = false;

	return true;
}


bool USBThread::StopAndWait()
{
	printf("USBThread::StopAndWait()\r\n");
	bool return_value = true;

	m_running = false;

	m_semaphore_thread->Lock(); //Wait here until ThreadLoop exits
	m_semaphore_thread->Unlock();

	return return_value;
}


UINT USBThread::ThreadFunction(LPVOID pParam)
{
	USBThread*	self						= (USBThread*)pParam;
	bool		IsUSBOpen					= false;
	bool		read_result					= false;
	DWORD		num_bytes_read				= 0;
	DWORD		desired_size				= 640*480*2+4+2+10;
	Buffer*		buffer						= NULL;
	int			no_buffer_available_count	= 0;

	printf("USBThread::ThreadLoop()\r\n");

	self->m_semaphore_thread->Lock();
	{
		while (self->IsRunning())//#1
		{
			printf("USBThread::ThreadLoop() Beginning of main whileloop (#1)\r\n");

			// Keep trying to open the usb until it opens or we are told to stop
			printf("USBThread::ThreadLoop() Trying to Open USB\r\n");
			while (self->IsRunning())//#2
			{
				printf(".");
				IsUSBOpen = self->USBOpen(NULL);
				if(IsUSBOpen)
					break;
				
				Sleep(2000);
			}//End: while(running) #2 
			printf("Success!\r\n");
		
			while (self->IsRunning())//#3
			{	
				/* STEPS:
				 * 1: Check out buffer from buffer store
				 * 2: If there were no buffers available then just wait and try again
				 * 3: Perform USB read 
				 * 4: Error checking / handling
				 * 5: Hand off the buffer to the receiver thread
				 */

				//Step 1: Check out buffer from buffer store
				buffer = self->m_bufferstore->CheckOut(BufferStore::BUF_TYPE_USB_PACKET);
				
				//printf("USBThread::ThreadLoop() buffer %x\r\n", buffer);

				//Step 2: If there were no buffers available then just wait and try again
				if(buffer == NULL)
				{
					no_buffer_available_count++;
					printf("-------------------------------------\r\n");
					printf("USBThread::ThreadLoop() No buffer Available! count: %d\r\n",no_buffer_available_count);				
					Sleep(USBTHREAD_NO_BUFFER_SLEEP_TIME);
					continue;
				}
			
				//printf("USBThread::ThreadLoop() Starting Read: \r\n");

				//Step 3: Perform USB read 
				read_result = self->USBRead(buffer->data, &num_bytes_read, desired_size);				
				
				//printf("USBThread::ThreadLoop() read_result: %d\r\n",read_result);

				//Step 4: Error checking / handling
				if(!read_result) 
				{
					self->m_bufferstore->CheckIn(&buffer);
					printf("!");
					
					if(self->IsUSBOpen())
						continue;
					else 
					{
						printf("USBThread::ThreadLoop() USBDisconnected: Closing the USB and opening it again\r\n");
						break;
					}					
				}

				//printf("USBThread::ThreadLoop() num_bytes_read: %d\r\n",num_bytes_read);
				//printf("USBThread::ThreadLoop() IsUSBOpen?: %d\r\n",self->IsUSBOpen());

				if(num_bytes_read == 0)
				{		
					self->m_bufferstore->CheckIn(&buffer);
					
					//Check if USB is still open. If it is then continue, if it is not, then break;
					if(self->IsUSBOpen())
						continue;
					else 
					{
						printf("USBThread::ThreadLoop() USBDisconnected: Closing the USB and opening it again\r\n");
						break;
					}
						
				}

				//Step 5: Hand off the buffer to the receiver thread
				bool received = self->m_receiverthread->receive(buffer);
				
				// If the receiver thread failed to accept the buffer, then check it back in
				if(!received)
				{
					printf("USBThread::ThreadLoop(): receiverthread failed to accept the buffer...\r\n");
					self->m_bufferstore->CheckIn(&buffer);
				}

				buffer = NULL;
			}//End: while(running) #3

			printf("USBThread::ThreadLoop() Exited main whileloop (#3)\r\n");	
					
			self->USBClose();
		}//End: while(running) #1
	}
	self->m_semaphore_thread->Unlock();

	printf("USBThread::ThreadLoop() Exiting\r\n");
	return 0;
}




bool USBThread::Send(void *data, DWORD num_bytes)
{
	bool return_value = true;

	return_value = USBWrite(data, num_bytes);		

	return return_value;
}






bool USBThread::IsUSBOpen()
{
	//return USBDevice->IsOpen();
	/*char str[256];
	USBDevice->UsbdStatusString(USBInEndPt->UsbdStatus, str);

	printf("IsUSBOpen(): ");
	str[255] = '\0';
	printf(str);
	printf("\r\n");*/

	char c = USBDevice->PowerState();

	if(c) 
		return true;
	else  
		return false;
}


// Opens the Cypress USB device driver (yhelios) and connects to Helios.
// Parameter hwnd may be NULL if it is not desired.
bool USBThread::USBOpen(HWND hwnd)
{
	//printf("USBThread::USBOpen()\r\n");
	CCyUSBEndPoint*	USBOutEndPt = NULL;
	CCyUSBEndPoint*	USBInEndPt  = NULL;

	bool return_value = true;

	// Make sure not already open
	if(USBDevice) 
		USBClose();
 
	// Open a USB Device
	USBDevice = new CCyUSBDevice(hwnd, yHeliosGUID);
	if(USBDevice != NULL) 
	{	// Attempt to open device 0
		if(USBDevice->DeviceCount() && !USBDevice->Open(0)) 
		{
			USBDevice->Reset();
			USBDevice->Open(0);
		}

		if(USBDevice->IsOpen()) 
		{ 
			USBOutEndPt	= USBDevice->BulkOutEndPt;
			if(USBOutEndPt != NULL)
			{
				USBOutEndPt->TimeOut = USB_OUT_TIMEOUT;
				USBOutEndPt->Reset();

				USBInEndPt = USBDevice->BulkInEndPt;
				if(USBInEndPt != NULL)
				{
					USBInEndPt->TimeOut		= USB_IN_TIMEOUT;
					USBInEndPt->SetXferSize(512*USB_TRANSFER_BLOCK_SIZE);
					USBInEndPt->Reset();
					
					return_value = true;
				}
				else 
				{	// For some reason we could not grab the USB in endpoint. guess the open failed
					return_value = false;			
				}
			}
			else 
			{   // For some reason we could not grab the USB out endpoint. guess the open failed
				return_value = false;
			}
		}
		else 
		{	//USBDevice->isOpen() returned null. guess the open failed
			return_value = false;
		}
	}
	else 
	{
		// USBDevice is NULL so, the open failed
		return_value = false;
	}

	// Couldn't load device so clean up
	if(!return_value)
		USBClose();

	return return_value;
}



// Cleans up and closes USB connection.
void USBThread::USBClose(void)
{
	//printf("USBThread::USBClose()\r\n");

	if(USBDevice != NULL) 
	{		
		USBDevice->Reset();// Reset device to flush any active API calls in other threads
		delete USBDevice;
		USBDevice = NULL;
	}
}



// Writes data to Helios USB. Returns false if an error occurs.
// NOTE: If Helios USB chip is configured in word-wide mode (16-bit)
//       then you should only transfer multiples of 2 bytes.
bool USBThread::USBWrite(void *data, DWORD numBytes)
{
	LONG			numBytesWritten;
	DWORD			i					= 0;
	bool			return_value		= false;
	CCyUSBEndPoint*	USBOutEndPt			= NULL;
	
	if(USBDevice != NULL)
	{
		USBOutEndPt	= USBDevice->BulkOutEndPt;
		if(USBOutEndPt != NULL)
		{
			while(i < numBytes) 
			{
				numBytesWritten = numBytes - i; // Prepare alias
				if(!USBOutEndPt->XferData(&((BYTE *)data)[i], numBytesWritten)) 
				{
					return_value = false;
					break;
				}
				i += numBytesWritten;
			}
		}
		else
		{
			return_value = false;
		}
	}
	else 
	{
		return_value = false;
	}

	return return_value;
}



// Reads a packet from the Helios USB. The size of the packet is returned in numBytesRead.
// Returns false if an error occurs.
// NOTE: This function will NOT block until a packet is received.
// NOTE: The dest buffer must be MAX_PACKET_SIZE or larger to avoid buffer overflow.
bool USBThread::USBRead(void *dest, DWORD *numBytesRead, DWORD bufferSize)
{
	bool			return_value	= true;
	LONG			numBytes		= bufferSize;  // Setup reference
	CCyUSBEndPoint* USBInEndPt		= NULL;
	
	if(USBDevice != NULL)
	{
		USBInEndPt = USBDevice->BulkInEndPt;
		if(USBInEndPt != NULL)
		{
			if(!USBInEndPt->XferData((BYTE *)dest, numBytes)) 
			{				
				return_value = false;
			}			
			*numBytesRead = numBytes;
		}
		else 
		{
			return_value = false;
		}
	}
	else 
	{
		return_value = false;
	}

	return return_value;
}
