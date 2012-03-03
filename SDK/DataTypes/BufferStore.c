/**
\file
\author Barrett Edwards

FILE:				BufferStore.c
AUTHOR:				Barrett Edwards
CREATED:			21 May 2007
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	BufferStore.h
DESCRIPTION:		See BufferStore.h

Related Functions & Groups:
	- General Usage:
		- void		BSInit();
		- Buffer*	BSCheckOut(int buffer_type);
		- void		BSCheckIn(Buffer** buf);
	- Info
		- int		BSNumAvailable(int buffer_type);
		- int32		BSCamType2BufferType(uint32 cam_type);
	- Static Memory Allocation
		- void		MemAllocInit(uint32 *base_addr);
		- void*		MemAlloc(uint32 num_bytes);

*/

/* Includes -----------------------------------------------------------------*/
#include <stdlib.h>

//#include <plb_correlator.h>

#include "Header.h"
#include "BufferStore.h"
#include "InterruptControl.h"
#include "Queue.h"
#include "Header.h"

/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/

/// g_mem_alloc_top should always point to the next word of available memory
static uint32* g_mem_alloc_top  = NULL;

/// g_quueues is the pointer to the list of queue pointers. Each 
/// Queue stores Buffers of a single type
static Queue** g_queues = NULL;

/** BufferStore initialization vector
    This vector is where the initialization information is stored
	for the BufferStore. Each row corresponds with a specific 
	Buffer type as delineated in the BufferStoreType enumeration.
	The BSInitVector is used in the BSInit() function to allocate
	the requested number of buffers of the specified size.

	For example, the first row reads:
	\pre
	{5,		FRAME_WIDTH*FRAME_HEIGHT,	1},
	\endpre
	This correspondes to the BufferType: BS_IMAGE_640_480_1B. 
	This row will cause the BufferStore to create 5 buffers,
	which each have a size of FRAME_WIDTH*FRAME_HEIGHT*1. 
	This buffer could be used in the capture of an 8-bit 
	grayscale image.

	The first column in the BSInitVector specifies the number of 
	buffers that need to be created. The second column specifies 
	the number of data entries that should be contained in the 
	buffer. And finally the third column specifies the size 
	in bytes of each of the data entries.

	For example: If a buffer is needed for an array of structs,
	as in: 
	\pre
	{24,	LIST_SIZE_CAPTURE_STREAK,   sizeof(CamStreak)},	
	\endpre
	24 buffers will be created that will has enough memory 
	to contain an number of CamFeature structs equal to 
	LIST_SIZE_CAPTURE_STREAK.

	Note that this BSInitVector must have the same number of rows 
	as there are entries in the BufferStoreType enumeration.
*/
uint32 BSInitVector[NUM_BUFFER_STORE_TYPES][3] = {	
//  Num    Size in Bytes
	//{0,		FRAME_WIDTH*FRAME_HEIGHT,	1},		// BS_IMAGE_640_480_1B
	{20,		FRAME_WIDTH*FRAME_HEIGHT,	2},		// BS_IMAGE_640_480_2B
	//{0,    LIST_SIZE_MAT_XSMALL,		BS_MAT_DATA_SIZE},			// BS_MAT_XSMALL
	//{0,    LIST_SIZE_MAT_SMALL,		BS_MAT_DATA_SIZE},			// BS_MAT_SMALL
	//{0,    LIST_SIZE_MAT_MEDIUM,		BS_MAT_DATA_SIZE},			// BS_MAT_MEDIUM
	//{0,    LIST_SIZE_MAT_LARGE,		BS_MAT_DATA_SIZE}			// BS_MAT_LARGE
	};

/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/


/** Allocate and initialize all the Buffers in the BufferStore
	This function #MUST# be called before any Buffers can be 
	checked out of this BufferStore.

	This function uses the information contained in the 
	BSInitVector to create and initialize the BufferStore
	Buffer structs. 

	STEPS:
	-# Allocate data space for pointers to buffer queues
	-# Allocate memory for the queue structs
	-# Allocate memory for the queue arrays
	-# Allocate memory for the buffer structs and push them into the Queues
	-# Allocate memory for the actuall buffer data space
*/
void BSInit()
{
	// STEP: Allocate data space for pointers to buffer queues
	g_queues = (Queue**) MemAlloc(NUM_BUFFER_STORE_TYPES * sizeof(Queue*));


	// STEP: Allocate memory for the queue structs
	{
		int i;
		//xil_printf("Allocating %d queue*\r\n", NUM_BUFFER_STORE_TYPES);
		for(i = 0 ; i < NUM_BUFFER_STORE_TYPES ; i++)	
			g_queues[i] = (Queue*) MemAlloc(sizeof(Queue));
	}
	

	// STEP: Allocate memory for the queue arrays
	{
		int i;
		for(i = 0 ; i < NUM_BUFFER_STORE_TYPES ; i++)
		{
			int num_of_buffers  = BSInitVector[i][0];
			//xil_printf("%d: Allocating %d Buffer*\r\n", i, num_of_buffers);
			void** ary = (void**) MemAlloc((num_of_buffers) * sizeof(Buffer*));
			QueueInit(g_queues[i], ary, num_of_buffers);
		}
	}

	// STEP: Allocate memory for the buffer structs and push them into the Queues
	{
		int i;
		for(i = 0 ; i < NUM_BUFFER_STORE_TYPES ; i++)
		{
			int num_of_buffers  = BSInitVector[i][0];
			int list_size		= BSInitVector[i][1];
			int data_size		= BSInitVector[i][2];
			
			int j;
			for(j = 0 ; j < num_of_buffers ; j++)
			{
			  //xil_printf("%d: allocating buffer size %d\r\n", j, sizeof(Buffer));
			  Buffer* buf			= MemAlloc(sizeof(Buffer));
			  buf->type			= i;
			  buf->size			= 0;
			  buf->capacity       = list_size * data_size;
			  QueuePush(g_queues[i], buf);
			}		
		}
	}

	// STEP: Allocate memory for the actuall buffer data space
	{
		int i;
		for(i = 0 ; i < NUM_BUFFER_STORE_TYPES ; i++)
		{
			int num_of_buffers  = BSInitVector[i][0];
			int list_size		= BSInitVector[i][1];
			int data_size		= BSInitVector[i][2];
			
			int j;
			for(j = 0 ; j < num_of_buffers ; j++)
			{
			  xil_printf("%d: Allocating %d bytes of data space\r\n", j, list_size*data_size);
			  Buffer* buf			= QueuePop(g_queues[i]);
			  buf->data.voidptr	= MemAlloc(list_size*data_size);
			  xil_printf("Buf at: %08x\r\n", buf);
			  xil_printf("Allocated at: %08x\r\n", buf->data.voidptr);

			  //memset(buf->data.voidptr, 0, list_size*data_size);  //remove the noise from the sdram
			  QueuePush(g_queues[i], buf);
			}		
		}	
	}
}



/** Determine which type of Buffer is required for a specific CAM_FRAME type.
	Each CAM_FRAME type has a required size in bytes. This function 
	allows the user code to determine which buffer type is right 
	for a specific CAM_FRAME type.
*/
int32 BSCamType2BufferType(uint32 cam_type)
{
	int32 buffer_type = -1;

	switch(cam_type){ 
	//case VISION_FRAME_GRAYSCALE8:			buffer_type = BS_IMAGE_640_480_1B;		break;
	case VISION_FRAME_RGB565:				buffer_type = BS_IMAGE_640_480_2B;		break;
	default: break;
	}	

	return buffer_type;
}



/** Checks out a Buffer from the BufferStore for general use.
	BSCheckOut() and BSCheckIn() are the two major functions 
	that will be most commonly used from the BufferStore. 

\return Returns a pointer to a Buffer struct of the requested type
if there are any available. Returns NULL if there are none available 
(which means they are all checked out) or if an error occurs 
(you request a buffer type that doesn't exist).
*/
Buffer* BSCheckOut(int buffer_type)
{
	xil_printf("BSCheckOut type: %d\r\n", buffer_type);
	xil_printf("g_queues: %08x\r\n", g_queues);
	if(		g_queues == NULL 
		||	buffer_type < 0 
		||	buffer_type >= NUM_BUFFER_STORE_TYPES)
		return NULL;

	xil_printf("g_queues[buffer_type]: %08x\r\n", g_queues[buffer_type]);
	Buffer* buf = (Buffer*) QueuePop(g_queues[buffer_type]);
	xil_printf("BSCheckOut Buffer addr: %08x\r\n", buf);
	return buf;
}


/** Checks in a Buffer to the BufferStore after general use.
	BSCheckOut() and BSCheckIn() are the two major functions 
	that will be most commonly used from the BufferStore. 

	When a Buffer has been Checked out, used and is nolonger 
	needed by suer code, it must be checked back into the 
	BufferStore so that it can be used again at a later time.
*/
void BSCheckIn(Buffer** buf)
{
	if(*buf == NULL)
		return;

	(*buf)->size = 0;
	QueuePush(g_queues[(*buf)->type], (*buf));
	*buf = NULL;
}



/** Returns the number of buffers available of a requested type.
*/
int BSNumAvailable(int buffer_type)
{
	if(		g_queues == NULL 
		||	buffer_type < 0 
		||	buffer_type >= NUM_BUFFER_STORE_TYPES)
		return 0;

	return g_queues[buffer_type]->size;
}


/** Initialize the static memory allocatiion uint.
	This function must be called #ONCE# and only once to initialize the
	allocation unit.

	This will cause all requests to MemAlloc() to statically allocate memory
	starting at the base_addr memory address
*/
void MemAllocInit(uint32 *base_addr)
{
	if(g_mem_alloc_top == NULL) {
		g_mem_alloc_top = base_addr;
		xil_printf("MemAlloc top set to: %08x\r\n", base_addr);
	}
}



/** #Permanently# allocates given number of  bytes (8-byte allinged).
	Returns a pointer to the first byte of allocated memory.
	This meory #cannot# be 'freed'. 

	This function uses the global variable g_mem_alloc_top to store 
	the current top of the allocated memory space.

	Reentrancy: This function IS reentrant.
*/
void* MemAlloc(uint32 num_bytes)
{
	CPU_MSR msr;

	uint32 *returnValue;
	uint32 numWords;

	// Round up to nearest whole word	
	numWords = (num_bytes >> 2);
	if(num_bytes & 0x3) 
		numWords++;

	msr = DISABLE_INTERRUPTS();
	{
		if(((uint32)g_mem_alloc_top) & 0x04)
			g_mem_alloc_top++;

		returnValue = g_mem_alloc_top;
		g_mem_alloc_top += numWords;		
	}
	RESTORE_INTERRUPTS(msr);

	return (void *)returnValue;
}


