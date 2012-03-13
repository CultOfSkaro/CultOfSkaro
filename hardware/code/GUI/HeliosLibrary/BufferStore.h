#ifndef BUFFERSTORE_H
#define BUFFERSTORE_H

#include "stdafx.h"
using std::stack;


typedef struct {
	int		type;
	union {
		uint8*		data;
		uint32*		data32;
		CvMat*		mat;
		IplImage*   image;
	}; 	
} Buffer;


class BufferStore
{
public:
	BufferStore();
	~BufferStore();

	enum BufferType {	
		BUF_TYPE_USB_PACKET,
		BUF_TYPE_IMAGE_640_480_1B,
		BUF_TYPE_IMAGE_640_480_2B,
		BUF_TYPE_IMAGE_640_480_3B,
		BUF_TYPE_IMAGE_640_480_4B,		
		BUF_TYPE_STREAK_LIST,	
		BUF_TYPE_FEATURE_LIST,	
		BUF_TYPE_TRACKED_FEATURES_LIST,
		NUM_BUFFER_STORE_TYPES
	};

	Buffer* CheckOut(int type);
	void	CheckIn(Buffer** buffer);
	
protected:
	stack<Buffer*>	m_buffers[NUM_BUFFER_STORE_TYPES];
	
	CSemaphore*		m_semaphore;
};

#endif