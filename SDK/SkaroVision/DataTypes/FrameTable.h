/** 
\unit
\author Barrett Edwards

FILE:				FrameTable.h
AUTHOR:				Barrett Edwards
CREATED:			21 Aug 2006
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	FrameTable.c
DESCRIPTION:	
	
	The FrameTable is a linked list of FrameTableEntries (FTE). Each of these 
	FTEs describe a single frame that has been captured and processed by the 
	camera. By creating a large list of FTEs, a significant amount of #history#
	can be preserved for the use of the system. For Example, an algorithm wishes
	to track the motion of certain object. The algorithm could use the large 
	amount of history stored in the FrameTable to look at the data in previous 
	frames. 
	
	The general processing flow of the Helios architecture can be described 
	in the following steps:
	-# Initiate a frame capture from the camera
	-# Wait until the current frame capture has completed 
		- Then start the next capture while we do the rest
	-# Checkout the FTE containing the acquired image data
	-# Run algorithms on image data
	-# Perform some useful action or vehicle control
	-# Check in the FTE
	-# Loop back to item 2

	Related Functions & Groups	
	- Helper Functions
		- void				FT_Init(); 
		- void				FT_PrintFrameTable();
	- Sequential Capture Mechanism
		- void				FT_StartCapture(FrameTableEntry* fte); 
		- void				FT_InterruptHandlerFrameTable();
	- General Usage
		-FrameTableEntry* 	FT_CheckOutFrame();
		-void 				FT_CheckInFrame(FrameTableEntry* fte);

CHANGE LOG
2-23-2010 SGF Updated for the EDK 11 Project
*/ 

/* Conditional Inclusion ----------------------------------------------------*/
#ifndef FRAMETABLE_H_
#define FRAMETABLE_H_


/* Includes -----------------------------------------------------------------*/
//#include <plb_correlator.h>

#include "Header.h"
#include "BufferStore.h"

/* Macros -------------------------------------------------------------------*/
/** This controls how much memory we allocate for FrameTableEntry structs	
	Each FrameTableEntry represents a single processed image from the camera.
	By storing multiple FrameTableEntry structures, a history of information is
	stored about images acquired in the past. These FrameTableEntries are 
	stored in a list called the FrameTable. 

	So if we store a large number FrameTableEntrys in the FrameTable, we can 
	'see' further back into the past.

	At 60 fps, 600 FrameTableEntrys will store 10 seconds of information about
	the past.
*/
#define NUM_FRAME_TABLE_ENTRIES		20



/* Enumerations -------------------------------------------------------------*/
/** Possible status states of a FrameTableEntry
	These enumeration values are used in the FrameTableEntry::status field to
	indicate the current status of the particular FTE. 

	At boot up, all FrameTableEntries are initialized to FT_STATUS_EMPTY, which
	implies that the FTE has not yet been used to acquire a frame from the 
	camera core. When the FrameTable initiates a frame capture from the camera
	core, it sets the FTE status to FT_STATUS_ACQUIRING, which implies that 
	the image data from the camera core is being written into the memory 
	pointers that the FTE currently owns.

	Once the frame capture has completed, the FTE will be labeled as 
	FT_STATUS_AVAILABLE. This implies that it is the most recently acquired 
	image data from the camera and has not yet been checked out or processed 
	by any user code. If this FTE is checked out, it will be then labeled as
	FT_STATUS_INUSE until it is checked back in and then labeled 
	FT_STATUS_COMPLETED. 

	If for some reason this FTE is never checked out, then when the next frame 
	capture completes this FTE will be labeled as FT_STATUS_MISSED. This is done
	to imply that the FTE was never checkout out or processed by user code. This 
	will happen when the image processing algorithms are too slow to keep up with
	the frame rate of the camera. For example, say the image processing algorithms 
	take 20 ms per frame on average. If the camera is running at 30 fps (33 ms per 
	frame) then no frames will be dropped / missed. But if the camera is running 
	at 60 fps (16 ms per frame) then it is certain that some FTE's will be missed.
*/
enum FrameTableStatus 
{
	FT_STATUS_EMPTY,	 ///< This FTE has not yet been used
	FT_STATUS_ACQUIRING, ///< Camera Core is curently writing to this entry
	FT_STATUS_AVAILABLE, ///< Camera Core has finished capturing to this entry, it is ready to be checked out
	FT_STATUS_INUSE,     ///< This has been checked out and is being used by user code
	FT_STATUS_COMPLETED, ///< This has been checked out, used, and checked back in
	FT_STATUS_MISSED,    ///< We for some reason never checked out this one (dropped frame)
	FT_STATUS_NULL,      
	FT_STATUS_SAVED
};



/* Structs ------------------------------------------------------------------*/
/** Reduced FrameTableEntry for informative transmission to GUI
	When a new FTE becomes available, the Helios needs to inform the GUI that 
	a new FTE has been checked out. This is so that the GUI can stay syncronized 
	with the Helios Power Software. The GUI attempts to mimic the state that the
	Power code is in. So by sending a little bit of information to the GUI right
	away when a FTE is checked out then the GUI knows it can associate all 
	received information in the near future with that FTE / framecount / ms 
	identifier(s)
*/
struct FrameTableEntrySmall 
{
	uint32			id;				///< FTE ID number. Each FTE has a unique ID number
	uint32			ms;				///< The ms (system uptime) that this frame finished capture 
	uint32			frameCount;		///< A sequential count of acquired images from the Camera
	uint32			saveoptions;	///< A 32-bit vector where each bit represents if a framelet at that index was 'saved'
} PACKED;
typedef struct FrameTableEntrySmall FrameTableEntrySmall; ///< typedef struct FrameTableEntrySmall FrameTableEntrySmall;




/** Container for all the information that is calcualted from a single frame
	The FrameTableEntry serves as a general container to store information 
	about a single frame captured from the camera. The general processing flow 
	of the Helios architecture can be described in the following steps:
	-# Initiate a frame capture from the camera
	-# Wait until the current frame capture has completed 
		- Then start the next capture while we do the rest
	-# Checkout the FTE containing the acquired image data
	-# Run algorithms on image data
	-# Perform some useful action or vehicle control
	-# Check in the FTE
	-# Loop back to item 2
	
	This information consits of a few general types:
	-# General info
	-# Camera Core Computed Information
	-# User code processing information
	-# Linked List Linkage (The FrameTable is a circular linked list)
	
	The #General Info# provides some basic data to describe the frame. This
	data consists of the FTE fields: id, ms, frameCount, saveoptions, status,
	and camera (which decribes the Camera Core used to caputre the image)
	
	The FTE stores a list of memory addresses where images and lists produced
	by the camera core are stored. These memory addresses are allocated through
	the use of Buffers from the BufferStore. When FT_StartFrameCapture() is 
	called, the required memory buffers are checked out of the BufferStore
	and allocated to the FTE. The memory addresses are then passed to the
	camera core, which will use DMA to transfer image & list data directly 
	to the memory location.

	The #Camera Core Computed Information# represents the information that 
	has been produced by the Camera Core. The BufferStore Buffers are stored
	in this section along with decriptive information that has been computed
	by the camera core. Such as the number of harris corner features that 
	were found in the image.

	The #User code processing information# is where user code & algorithms 
	should store information that has been computed on this particular FTE.
	Say for example we are looking for the center of mass of a large RED 
	object in the image. Once we perform the connected components and 
	filtering algorithms, we could store the resulting (x,y) center of mass
	values in this section. 

	The #Linked List Linkage# is to allow the connection of all the FTE's 
	into a large circular linked list. By organizing the FTE's in this way,
	any algorithm that checksout an FTE can have instant access to the 
	previous FTE and all of its saved internal information. This is very 
	valuable to image processing algorithms that use data from multiple 
	images. 	
*/
struct FrameTableEntry 
{
	//General Info
	uint32		id;					///< FTE ID number. Each FTE has a unique ID number
	uint32		ms;					///< The ms (system uptime) that this frame finished capture 
	uint32		frameCount;			///< A sequential count of acquired images from the Camera
	uint32		saveoptions;		///< A 32-bit vector where each bit represents if a framelet at that index was 'saved'		
	uint32		status;				///< The current usage status of this FTE. Must be a value from FrameTableStatus
	CameraInfo*	camera;				///< Information about the CameraCore that was used to capture this frame

	//Camera Core Computed Info
	Buffer* 		frame_address[VISION_MAX_NUM_FRAMES];		///< Image Pointers for FPGA Camera Core to store data in
	uint8* 		frame_address_end[VISION_MAX_NUM_FRAMES];	///< Ending address of the image pointers after data has bee stored in them
	
	// Linked List Linkage	
	struct FrameTableEntry *previous;	///< The FTE's are arranged into a circular Linked List. This pointer points the the next #older# frame
	struct FrameTableEntry *next;		///< The FTE's are arranged into a circular Linked List. This pointer points the the next #newer# frame
} PACKED;
typedef struct FrameTableEntry FrameTableEntry; ///< typedef struct FrameTableEntry FrameTableEntry;




/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
extern FrameTableEntry** g_frametable; ///< Included to allow direct access to the entire FrameTable


/* Function Prototypes ------------------------------------------------------*/
void				FT_Init(); 
void				FT_PrintFrameTable();

void				FT_StartCapture(FrameTableEntry* fte); 
void				FT_InterruptHandlerFrameTable();

FrameTableEntry* 	FT_CheckOutFrame();
void 				FT_CheckInFrame(FrameTableEntry* fte);

#endif
