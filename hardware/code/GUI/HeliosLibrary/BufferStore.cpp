#include "StdAfx.h"
#include "BufferStore.h"


static int config[BufferStore::NUM_BUFFER_STORE_TYPES][3] = {
//# Buffers    Size/rows          columns
	{100	, 640*480*4+6+10	,	  0},	// BUF_TYPE_USB_PACKET
	{  0	,	  480			,	640},	// BUF_TYPE_IMAGE_640_480_1B
	{  0	,	  480			,	640},	// BUF_TYPE_IMAGE_640_480_2B
	{  0	,	  480			,	640},	// BUF_TYPE_IMAGE_640_480_3B
	{100	,	  480			,	640},	// BUF_TYPE_IMAGE_640_480_4B	
	{100	,	10000			,	  3},   // BUF_TYPE_STREAK_LIST	
	{100	,	10000			,	  3},	// BUF_TYPE_FEATURE_LIST,	
	{100	,	 5000			,	  6}	// BUF_TYPE_TRACKED_FEATURES_LIST
};


BufferStore::BufferStore()
{		
	printf("BufferStore::BufferStore()\r\n");

	m_semaphore	= new CSemaphore(1,1,NULL,NULL);
	
	for(int i = 0 ; i < NUM_BUFFER_STORE_TYPES ; i++)
	{
		int numBuffers = config[i][0];
		int bufferSize = config[i][1];
		int bufferRows = config[i][1];
		int bufferCols = config[i][2];

		for(int j = 0 ; j < numBuffers ; j++)
		{
			Buffer* buf = (Buffer*) malloc(sizeof(Buffer));
			if(buf == NULL)
				break;
				
			buf->type = i;
			buf->data = NULL;
			
			switch(i){
			case BUF_TYPE_USB_PACKET:				buf->data  = (uint8*) malloc(bufferSize);									break;
			case BUF_TYPE_IMAGE_640_480_1B:			buf->image = cvCreateImage(cvSize(bufferCols,bufferRows),IPL_DEPTH_8U,1);	break;
			case BUF_TYPE_IMAGE_640_480_2B:			buf->image = cvCreateImage(cvSize(bufferCols,bufferRows),IPL_DEPTH_8U,2);	break;
			case BUF_TYPE_IMAGE_640_480_3B:			buf->image = cvCreateImage(cvSize(bufferCols,bufferRows),IPL_DEPTH_8U,3);	break;
			case BUF_TYPE_IMAGE_640_480_4B:			buf->image = cvCreateImage(cvSize(bufferCols,bufferRows),IPL_DEPTH_8U,4);	break;
			case BUF_TYPE_STREAK_LIST:				buf->mat   = cvCreateMat(bufferRows, bufferCols, CV_64F);					break;
			case BUF_TYPE_FEATURE_LIST:				buf->mat   = cvCreateMat(bufferRows, bufferCols, CV_64F);					break;
			case BUF_TYPE_TRACKED_FEATURES_LIST:	buf->mat   = cvCreateMat(bufferRows, bufferCols, CV_64F);					break;
			default: break;
			}
			
			if(buf->data != NULL)
				m_buffers[i].push(buf);
			else 
				free(buf);	
		}
	}	
}

BufferStore::~BufferStore()
{
	printf("BufferStore::~BufferStore()\r\n");

	m_semaphore->Lock();

	// Free all the buffers 
	for(int i = 0 ; i < NUM_BUFFER_STORE_TYPES ; i++)
	{
		while(!m_buffers[i].empty())
		{
			Buffer* buf = m_buffers[i].top();
			m_buffers[i].pop();
			
			// Free the data
			switch(i){
			case BUF_TYPE_USB_PACKET:				free(buf->data); 				break;
			case BUF_TYPE_IMAGE_640_480_1B:			
			case BUF_TYPE_IMAGE_640_480_2B:			
			case BUF_TYPE_IMAGE_640_480_3B:			
			case BUF_TYPE_IMAGE_640_480_4B:			cvReleaseImage(&buf->image);	break;
			case BUF_TYPE_STREAK_LIST:				
			case BUF_TYPE_FEATURE_LIST:				
			case BUF_TYPE_TRACKED_FEATURES_LIST:	cvReleaseMat(&buf->mat);		break;
			default: break;
			}

			// then Free struct
			free(buf);
		}
	}

	delete m_semaphore;
}


Buffer* BufferStore::CheckOut(int type)
{
	Buffer* return_value = NULL;
	
	// Enter a critical section so no funny busness with multiple threads
	m_semaphore->Lock();
	{
		if(0 <= type && type < NUM_BUFFER_STORE_TYPES)
		{
			if(!m_buffers[type].empty())
			{
				return_value = m_buffers[type].top();
				m_buffers[type].pop();
			}
		}
	}
	m_semaphore->Unlock();

	//printf("BufferStore::CheckOut() %x\r\n",return_value);

	return return_value;
}


void BufferStore::CheckIn(Buffer** buffer)
{
	Buffer* buf = *buffer;

	if(buf == NULL)
		return;

	//printf("\t\t\t\tBufferStore::CheckIn() %x\r\n",buffer);

	// Enter a critical section so no funny busness with multiple threads
	m_semaphore->Lock();
	{
		if(0 <= buf->type && buf->type < NUM_BUFFER_STORE_TYPES)
		{
			if(buf->data != NULL)
			{
				m_buffers[buf->type].push(buf); 		
			}				
		}
	}
	m_semaphore->Unlock();

	*buffer = NULL;
}