#ifndef RECEIVERTHREAD_H
#define RECEIVERTHREAD_H

#include "stdafx.h"
#include "BufferStore.h"

using std::vector;

class Helios;

class ReceiverThread
{
public:
	ReceiverThread(Helios* h);
	~ReceiverThread();
	
	bool IsRunning() { return m_running; }
	bool Start();
	bool Stop();
	bool StopAndWait();
	
	bool receive(Buffer* buffer);
	
protected:
	static UINT __cdecl ThreadLoop(LPVOID pParam);
	

protected:		
	vector<Buffer*>		m_queue;

	volatile bool		m_running;
	
	CSemaphore*			m_semaphore_thread;
	CSemaphore*			m_semaphore_ready;
	CSemaphore*			m_semaphore_queue;

	Helios*				m_helios;
};

#endif




