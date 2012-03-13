#ifndef AVIWRITERTHREAD_H
#define AVIWRITERTHREAD_H

#include "stdafx.h"
#include "BufferStore.h"

using std::string;
using std::queue;

class AVIWriterThread
{
public:
	AVIWriterThread(BufferStore* bufferstore);
	~AVIWriterThread();
	
	bool IsRunning() { return m_running; }
	bool Start();
	bool Stop();
	bool StopAndWait();
	
	void	Init(const char* filename, int width=640, int height=480, double fps=30.0, int format=CV_FOURCC('I','V','5','0'), int iscolor=1);
	void	SetFormat(int format);
	void	SetFilename(const char* filename);
	string  GetFilename();
	bool	Write(Buffer* buffer);
	void	WriteAsImages(bool b);
	
protected:
	static UINT __cdecl ThreadLoop(LPVOID pParam);
	

protected:		
	queue<Buffer*>		m_queue;

	volatile bool		m_running;
	
	CSemaphore*			m_semaphore_thread;
	CSemaphore*			m_semaphore_ready;
	CSemaphore*			m_semaphore_queue;

	CvVideoWriter*		cvvideowriter;
	bool				m_write_as_images;
	BufferStore*		m_bufferstore;
	string				m_filename;
	int					m_width;
	int					m_height;
	double				m_fps;
	int					m_format;
	int					m_iscolor;
};

#endif




