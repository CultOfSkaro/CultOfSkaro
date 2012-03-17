/**
\unit
\author Barrett Edwards

FILE:				BufferStore.h
AUTHOR:				Barrett Edwards
CREATED:			21 May 2007
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	BufferStore.c
DESCRIPTION:	

	The BufferStore serves as a memory management system. In embedded systems,
	advanced memory management systems ( malloc, free, etc... ) may not meet
	the real-time needs of some applications. To avoid problems with 
	real-time deadlines, the BufferStore provides a collection of 
	statically sized buffers that exist in main memory. 
	
	These buffers can be checked out by user code and then used for any
	general purpose. When the user code is finished using the buffer, the 
	buffer can be checked back into the BufferStore.

	The BufferStore is organized into multiple 'pools.' Each buffer pool
	contains buffers of a single type. A Buffer is checked out by calling 
	BSCheckOut() with the appropriate buffer type. Some Buffer types are 
	sized for specific uses, such as a 640x480 image at 1-Byte per pixel. 
	But even though a Buffer is sized for a specific, the Buffer can be 
	used for any purpose since it is just a continuous block of memeory. 

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

\todo		
Add a generic CheckOut function that allows the user code to pass in
desired size in bytes rather than a Buffer Type.
	

CHANGE LOG 
2-23-2010   Removed excess, re-vamped for the EDK11 project
*/


/* Conditional Inclusion ----------------------------------------------------*/
#ifndef BUFFERSTORE_H
#define BUFFERSTORE_H


/* Includes -----------------------------------------------------------------*/
#include <plb_vision.h>
#include "Header.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/


/** List of all Buffer types
	Each Buffer in the BufferStore has a specific type. This enumeration 
	lists all the types of Buffers contained in the BufferStore.
*/
enum BufferStoreType 
{
	//BS_IMAGE_640_480_1B,
	BS_IMAGE_640_480_2B,
	//BS_MAT_XSMALL,							///< 4 x 1    ( 16-bytes) matrix maximum size
	//BS_MAT_SMALL,							///< 4 x 4    ( 64-bytes) matrix maximum size
	//BS_MAT_MEDIUM,							///< 4 x 500  ( 8K-bytes) matrix maximum size
	//BS_MAT_LARGE,							///< 4 x 1000 (16K-bytes) matrix maximum size
	NUM_BUFFER_STORE_TYPES
};



/** This is a container of all the list sizes used in the BSInitVector
	These really could just be a separate list of \#define statements,
	but by putting them in an enumeration they get to be all cozy in
	a nice container.
*/
enum BufferStoreListSizes
{
	LIST_SIZE_MAT_XSMALL		= (4 * 1),	///< MxN size of a Matrix
	LIST_SIZE_MAT_SMALL			= (4 * 4),	///< MxN size of a Matrix
	LIST_SIZE_MAT_MEDIUM		= (4 * 1000),///< MxN size of a Matrix
	LIST_SIZE_MAT_LARGE			= (4 * 5000)///< MxN size of a Matrix
};

/// This is the data size of each element in a Matrix array
#define BS_MAT_DATA_SIZE		(sizeof(float))


/* Structs ------------------------------------------------------------------*/
/** A struct that describes an individual Buffer in the BufferStore. 
	This Buffer struct is used to keep some accounting and descriptive 
	information about the memory pointer checked out of the BufferStore.
	
	Checking out a buffer from the BufferStore returns a pointer to
	one of these Buffer structs. The alternative to using a Buffer 
	Struct is to just check out a void pointer, which would be similar 
	to how malloc() works. But by checking out a buffer struct, rather 
	than just a void pointer, the maximum capacity and currently used 
	amount (Buffer::size) can be coupled together with the actual memory 
	pointer (Buffer::data::voidptr).

	The Buffer::capacity field is the size of the memory pointer in Bytes.
	The Buffer::size field is not used by the BufferStore and is 
	included in the Buffer struct for ease of use in user code.
	For example. A Buffer is checked out to store a list of features.
	The Buffer can store a maximum of 1000 CamFeatures in the list.
	But, the particular use of this Buffer only needs to store 42
	CamFeatures. Therefore the Buffer::size field could be set to be 42
	indicating that only 42 elements of the Buffer have been used rather 
	than the maximum capacity of 1000. 
*/
struct Buffer 
{
	uint32 type;			///< The type of buffer ( enum BufferStoreType)
	uint32 size;			///< Included for convience in user code algorithms
	uint32 capacity;		///< Maximum capacity of data buffer in BYTES

	/** The buffer_data_union is as union of multiple memory pointers
		This is done this way for user code convience. By including 
		all of these various data types in a single union, user code
		can check out a buffer and then access the memory pointer arrays
		without the need to type cast a void pointer.
		For example, if the user code wanted to access the memory as 
		a uint32 array:
		\pre
		int sum = 0;
		for(int i = 0 ; i < 100 ; i++)
			sum += buffer.data.data32[i];
		\endpre
		or if the user code wanted to access the memory as a uint8 array:
		\pre
		int sum = 0;
		for(int i = 0 ; i < 100 ; i++)
			sum += buffer.data.data8[i];
		\endpre
	*/
	union buffer_data_union
	{
		void				*voidptr;
		uint8				*data8;
		uint16				*data16;
		uint32				*data32;
		float				*fl;
	} data; 	
};
typedef struct Buffer Buffer; ///< typedef struct Buffer Buffer;


/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/

/// Exported so some code that needs to access the arrays directly can do so
extern uint32*	M_uint; 


/* Function Prototypes ------------------------------------------------------*/

void	BSInit();
Buffer* BSCheckOut(int buffer_type);
void    BSCheckIn(Buffer** buf);

int		BSNumAvailable(int buffer_type);
int32	BSCamType2BufferType(uint32 cam_type);

void	MemAllocInit(uint32 *base_addr);
void*	MemAlloc(uint32 num_bytes);

#endif 
