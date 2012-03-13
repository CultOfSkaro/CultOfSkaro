#ifndef FRAMETABLE_H
#define FRAMETABLE_H

#include "stdafx.h"
using std::deque;

class FrameTableEntry;
class BufferStore;

class FrameTable
{
public:
	FrameTable(int table_size, int release_depth, BufferStore* bufferstore);
	~FrameTable(void);

public:
	void			 addFTE(FrameTableEntry* fte);
	FrameTableEntry* getFTE(uint32 index);
	int				 size();

protected: 
	BufferStore*				m_bufferstore;
	int							m_release_depth;

	deque<FrameTableEntry*>		m_table;
	CSemaphore*					m_semaphore;
};
#endif
