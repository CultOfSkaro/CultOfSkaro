
#include "stdafx.h"
#include "AVIWriterThread.h"
#include "Helios.h"


AVIWriterThread::AVIWriterThread(BufferStore* bufferstore)
{
	printf("AVIWriterThread::AVIWriterThread()\r\n");

	m_running			= false;
	m_semaphore_thread	= new CSemaphore(1,1,NULL,NULL);
	m_semaphore_queue	= new CSemaphore(1,1,NULL,NULL);
	m_semaphore_ready	= new CSemaphore(0,100,NULL,NULL);
	
	m_write_as_images	= false;
	cvvideowriter		= NULL;
	m_bufferstore		= bufferstore;

	this->Init(	"video.avi", 
				640, 
				480, 
				30.0, 
				CV_FOURCC('I','V','5','0'), 
				1);
}

AVIWriterThread::~AVIWriterThread()
{
	printf("AVIWriterThread::~AVIWriterThread()\r\n");
	m_running = false;

	//m_semaphore_thread->Lock(); // Wait for ThreadLoop() to exit before returning	

	delete m_semaphore_thread;
	delete m_semaphore_queue;
	delete m_semaphore_ready;
}


bool AVIWriterThread::Start()
{
	printf("AVIWriterThread::Start()\r\n");

	bool return_value = true;

	if(!m_running)
	{		
		if(!this->m_write_as_images)
		{
			cvvideowriter = cvCreateVideoWriter(	this->m_filename.c_str(), 
													this->m_format, 
													this->m_fps, 
													cvSize(this->m_width,this->m_height), 
													this->m_iscolor);
			if(!cvvideowriter)
			{
				printf("AVIWriterThread::Start() AVI file creation Aborted\r\n");
				return false;
			}
		}		
		m_running = true;
		AfxBeginThread(ThreadLoop, (void*)this);
	}
	else 
		return_value = false;

	return return_value;
}

bool AVIWriterThread::Stop()
{
	printf("AVIWriterThread::Stop()\r\n");

	m_running = false;

	return true;
}

bool AVIWriterThread::StopAndWait()
{
	printf("AVIWriterThread::StopAndWait()\r\n");
	bool return_value = true;

	m_running = false;

	m_semaphore_thread->Lock();   // Wait for ThreadLoop() to exit before returning	
	m_semaphore_thread->Unlock(); 

	return return_value;
}



UINT AVIWriterThread::ThreadLoop(LPVOID pParam)
{
	AVIWriterThread*	self			= (AVIWriterThread*)pParam;
	Buffer*				buffer			= NULL;
		
	printf("\t\t\t\tAVIWriterThread::ThreadLoop()\r\n");
	
	if(!self->m_write_as_images)
	{		
		if(!self->cvvideowriter)
		{
			self->m_running = false;
			return 0;
		}
	}

	self->m_semaphore_thread->Lock();
	{
		while(self->IsRunning())
		{	
			/* STEPS:
			 * 1: Pend on the semaphore. Determine if we acquired the Semaphore or if we timed out
			 * 2: Check if Queue of frames to process is empty 
			 * 3: If a buffer was acquired then do something with it
			 */

			//Step 1: Pend on the semaphore
			if(self->m_semaphore_ready->Lock(250))  // Resource has been locked
			{			
				//Step 2: Check if Queue of frames to process is empty 
				self->m_semaphore_queue->Lock();
				{
					if(self->m_queue.size() > 0 )
					{					
						buffer = self->m_queue.front();		// get the first element
						self->m_queue.pop();				// Pop the first element off the queue
					}
					else 
						buffer = NULL;			
				}
				self->m_semaphore_queue->Unlock();

				//Step 3: If a buffer was acquired then do something with it
				if(buffer != NULL)
				{
					if(    buffer->type >= BufferStore::BUF_TYPE_IMAGE_640_480_1B 
						&& buffer->type <= BufferStore::BUF_TYPE_IMAGE_640_480_4B)
					{												
						if(self->m_write_as_images)
						{   							
							int loc = (int)self->m_filename.find_last_of('\\');							
							string substr = self->m_filename.substr(0,loc+1);
						
							char str[256];							
							sprintf_s(str, 256,"image%06d.bmp", buffer->image->alphaChannel);
							substr = substr.append(str);
							
							cvSaveImage(substr.c_str(), buffer->image);
						}else
						{	//Write to AVI File here
							if(self->cvvideowriter)
								cvWriteFrame(self->cvvideowriter, buffer->image );
						}
					}
					
					//CheckIn buffer
					self->m_bufferstore->CheckIn(&buffer);

				}//End: if(buffer != NULL)
			}//End: if(self->m_semaphore_ready->Lock(250))
		}//End: while(self->IsRunning())
	}
	self->m_semaphore_thread->Unlock();
	printf("\t\t\t\tAVIWriterThread::ThreadLoop() Exiting\r\n");


	//Empty the queue
	self->m_semaphore_queue->Lock();
	{
		while(!self->m_queue.empty())
		{
			buffer = self->m_queue.front();		// get the first element
			self->m_queue.pop();				// Pop the first element off the queue
			self->m_bufferstore->CheckIn(&buffer);
		}
	}
	self->m_semaphore_queue->Unlock();

	if(self->cvvideowriter)
		cvReleaseVideoWriter(&self->cvvideowriter);

	return 0;
}


void AVIWriterThread::Init(const char* filename, int width, int height, double fps, int format, int iscolor)
{
	m_filename	= string(filename);
	m_width		= width;
	m_height	= height;
	m_fps		= fps;
	m_format	= format;
	m_iscolor	= iscolor;
}


void AVIWriterThread::SetFormat(int format)
{
	m_format = format;
}

void AVIWriterThread::SetFilename(const char* filename)
{
	m_filename = string(filename);
}

string AVIWriterThread::GetFilename()
{
	return m_filename;
}

void AVIWriterThread::WriteAsImages(bool b)
{
	m_write_as_images = b;
}


bool AVIWriterThread::Write(Buffer* buffer)
{
	//printf("AVIWriterThread::Write() \r\n");
	if(this->IsRunning())
	{
		this->m_semaphore_queue->Lock();
		{
			//printf("AVIWriterThread::Write() appending a buffer to the queue %x\r\n",buffer);
			this->m_queue.push(buffer);
			this->m_semaphore_ready->Unlock(1);
		}
		this->m_semaphore_queue->Unlock();
	}

	return true;
}