#include "stdafx.h"
#include "FrameTable.h"
#include "FrameTableEntry.h"

using std::deque;

FrameTable::FrameTable(int table_size, int release_depth, BufferStore* bufferstore)
	: m_bufferstore(bufferstore),
	  m_release_depth(release_depth),
	  m_semaphore(new CSemaphore(1,1,NULL,NULL))
{
	// allocate all frame table entries
	for(int i = 0 ; i < table_size ; i++)		
		m_table.push_back(new FrameTableEntry);	
}

FrameTable::~FrameTable(void)
{
	m_semaphore->Lock();
	// Release all frame table entries
	for(size_t i = 0 ; i < m_table.size() ; i++)	
		if(m_table.at(i) != NULL)
			delete m_table.at(i);

	delete m_semaphore;
}


void FrameTable::addFTE(FrameTableEntry* fte)
{
	//printf("FrameTable::addFTE(): frameCount:%d\r\n",fte->m_frameCount);

	m_semaphore->Lock();
	{
		/* STEPS
		 * 1: Pop the last FTE off the back of the table
		 * 2: Copy the data contained in the passed in FTE 
		 *    to the table FTE
		 * 3: Push the tableFTE on to the front of the table 
		 * 4: Release any buffers store by FTE's in the table 
		 *    that are older then a set threshold 
		 */

		// STEP 1: 
		FrameTableEntry* tablefte = m_table.back();
		m_table.pop_back();


		// STEP 2: 
		*tablefte = *fte;
		

		// STEP 3:
		m_table.push_front(tablefte);


		// STEP 4: 
		FrameTableEntry* fte2clear = m_table.at(m_release_depth-1);
		if(fte2clear != NULL)		
			for(int i = 0 ; i < FrameTableEntry::FTE_NUM_BUFFERS ; i++)		
				m_bufferstore->CheckIn(&fte2clear->m_buffers[i]);	
			
		
	}
	m_semaphore->Unlock();
}



FrameTableEntry* FrameTable::getFTE(uint32 index)
{
	FrameTableEntry* fte = NULL;

	m_semaphore->Lock();
	{
		if(index < m_table.size())
			fte = m_table.at(index);
	}
	m_semaphore->Unlock();

	return fte;
}

int FrameTable::size()
{
	return (int) m_table.size();
}