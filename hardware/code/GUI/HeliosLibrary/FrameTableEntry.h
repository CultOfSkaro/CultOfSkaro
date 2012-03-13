#ifndef FRAMETABLEENTRY_H
#define FRAMETABLEENTRY_H

#include "stdafx.h"
#include "SystemTypes.h"
#include "BufferStore.h"


class FrameTableEntry
{
public:
	FrameTableEntry();	
		
	enum BufferIndex{
		FTE_BUFFER_IMAGE,
		FTE_BUFFER_CORRELATIONS,
		FTE_NUM_BUFFERS
	};


public:
	uint32	m_id;
	uint32	m_ms;
	uint32	m_frameCount;
	uint32	m_saveoptions;
	
	uint32	m_status;
	uint32	m_numFeatures;
	uint32	m_maxHarrisValue;

	Buffer*	m_buffers[FTE_NUM_BUFFERS];
	
};
#endif
