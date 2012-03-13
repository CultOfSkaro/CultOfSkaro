
#include "stdafx.h"
#include "ReceiverThread.h"
#include "Helios.h"


ReceiverThread::ReceiverThread(Helios* helios)
{
	printf("ReceiverThread::ReceiverThread()\r\n");

	m_running			= false;
	m_semaphore_thread	= new CSemaphore(1,1,NULL,NULL);
	m_semaphore_queue	= new CSemaphore(1,1,NULL,NULL);
	m_semaphore_ready	= new CSemaphore(0,100,NULL,NULL);
	
	m_helios			= helios;

	m_queue.reserve(100);
}

ReceiverThread::~ReceiverThread()
{
	printf("ReceiverThread::~ReceiverThread()\r\n");
	m_running = false;

	//m_semaphore_thread->Lock(); // Wait for ThreadLoop() to exit before returning	

	delete m_semaphore_thread;
	delete m_semaphore_queue;
	delete m_semaphore_ready;
}


bool ReceiverThread::Start()
{
	//printf("ReceiverThread::Start()\r\n");

	bool return_value = true;

	if(!m_running)
	{
		m_running = true;
		AfxBeginThread(ThreadLoop, (void*)this);
	}
	else 
		return_value = false;

	return return_value;
}

bool ReceiverThread::Stop()
{
	printf("ReceiverThread::Stop()\r\n");

	m_running = false;

	return true;
}

bool ReceiverThread::StopAndWait()
{
	printf("ReceiverThread::StopAndWait()\r\n");
	bool return_value = true;

	m_running = false;

	m_semaphore_thread->Lock();   // Wait for ThreadLoop() to exit before returning	
	m_semaphore_thread->Unlock(); 

	return return_value;
}



UINT ReceiverThread::ThreadLoop(LPVOID pParam)
{
	ReceiverThread*	self	= (ReceiverThread*)pParam;
	Buffer*			buffer  = NULL;
	
	printf("\t\t\t\tReceiverThread::ThreadLoop()\r\n");
	
	self->m_semaphore_thread->Lock();
	{
		while (self->IsRunning())
		{	
			/* STEPS:
			 * 1: Pend on the semaphore. Determin if we acquired the Semaphore or if we timed out
			 * 2: Check if Queue of frames to process is empty 
			 * 3: If a buffer was acquired then do something with it
			 */

			//printf("\t\t\t\tReceiverThread::ThreadLoop() Pending on the m_semaphore_ready\r\n");

			//Step 1: Pend on the semaphore
			if(self->m_semaphore_ready->Lock(250))  // Resource has been locked
			{			
				//printf("\t\t\t\tReceiverThread::ThreadLoop() Acquired the m_semaphore_ready \r\n");

				//Step 2: Check if Queue of frames to process is empty 
				self->m_semaphore_queue->Lock();
				{
					if(self->m_queue.size() > 0 )
					{					
						buffer = self->m_queue.front();				// get the first element
						self->m_queue.erase(self->m_queue.begin()); // Pop the first element off the queue
						
						//printf("\t\t\t\tReceiverThread::ThreadLoop() Acquired the buffer %x\r\n",buffer);
					}
					else 
						buffer = NULL;			
				}
				self->m_semaphore_queue->Unlock();

				//Step 3: If a buffer was acquired then do something with it
				if(buffer != NULL)
				{
					self->m_helios->receive(buffer);
				}
			}
		}
	}
	self->m_semaphore_thread->Unlock();
	printf("\t\t\t\tReceiverThread::ThreadLoop() Exiting\r\n");

	return 0;
}




bool ReceiverThread::receive(Buffer* buffer)
{
	//printf("ReceiverThread::receive() \r\n");

	bool return_value = true;
	
	this->m_semaphore_queue->Lock();
	{
		if(m_queue.size() < m_queue.capacity())
		{
			//printf("ReceiverThread::receive() appending a buffer to the queue %x\r\n",buffer);
			this->m_queue.push_back(buffer);
			this->m_semaphore_ready->Unlock(1);
		}
		else 
			return_value = false;
	}
	this->m_semaphore_queue->Unlock();

	return return_value;
}