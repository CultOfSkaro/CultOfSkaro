#include "stdafx.h"

#include "FrameTableEntry.h"


FrameTableEntry::FrameTableEntry()
	: m_id(0),
	  m_ms(0),
	  m_frameCount(0),
	  m_saveoptions(0),
	  m_status(0),
	  m_numFeatures(0),
	  m_maxHarrisValue(0)
{
	for(int i = 0 ; i < FTE_NUM_BUFFERS ; i++)
		m_buffers[i] = NULL;
}


