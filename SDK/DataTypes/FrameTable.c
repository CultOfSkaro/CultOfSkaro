/**
\file
\author Barrett Edwards

FILE:				FrameTable.c
AUTHOR:				Barrett Edwards
CREATED:			21 Aug 2006
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	FrameTable.c
DESCRIPTION:		See FrameTable.h
	
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

*/

/* Includes -----------------------------------------------------------------*/
#include <string.h>

#include <plb_vision.h>

#include "Header.h"
#include "InterruptControl.h"
#include "BufferStore.h"
#include "State.h"
#include "FrameTable.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
FrameTableEntry**	g_frametable;	///< The base pointer of the FrameTable List of FrameTableEntriy pointers
FrameTableEntry*	g_capture_fte;	///< Pointer to the FTE that is currently being captured into by the Camera Core
FrameTableEntry*	g_current_fte;	///< The most recently captured FTE. When you call FT_CheckOutFrame() you get this FTE.


/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/


/** Allocate & Initialize the FrameTable List of FrameTableEntries
	The FT_Init() function is called only once at bootup and is used to malloc 
	(which is really MemAlloc()) the memory that will be used to store the various 
	frames and tables that will be pointed to by each entry in the FrameTable.

	Steps: 
	-#  Allocate memory for the FrameTable pointer array
	-#  Allocate Memory for the FrameTableEntry Structs
	-#  Initialize the FrameTable Entry Structs
	-#  Assemble LinkedList
	-#  Initialize global pointers to NULL
*/   
void FT_Init()
{
	// Step: Allocate memory for the FrameTable pointer array
	g_frametable = (FrameTableEntry**) MemAlloc(NUM_FRAME_TABLE_ENTRIES * sizeof(FrameTableEntry*));
	


	// Step: Allocate Memory for the FrameTableEntry Structs
	{
		int i;
		for(i = 0 ; i < NUM_FRAME_TABLE_ENTRIES ; i++)	
			g_frametable[i] = (FrameTableEntry*) MemAlloc(sizeof(FrameTableEntry));
	}
	


	// Step: Initialize the FrameTable Entry Structs
	//       Set the entire struct to 0 using memset()
	//       and then assign each entry an ID number
	{
		int i;
		for(i = 0 ; i < NUM_FRAME_TABLE_ENTRIES ; i++)
		{	
			memset(g_frametable[i], 0, sizeof(FrameTableEntry));
			g_frametable[i]->id	= i;		
		}
	}


	// STEP: Assemble LinkedList
	{
		int i;
		for(i = 0 ; i < NUM_FRAME_TABLE_ENTRIES-1 ; i++)
		{	
			g_frametable[i]->next		= g_frametable[i+1];
			g_frametable[i+1]->previous = g_frametable[i];
		}
		g_frametable[NUM_FRAME_TABLE_ENTRIES-1]->next = g_frametable[0];
		g_frametable[0]->previous					  = g_frametable[NUM_FRAME_TABLE_ENTRIES-1];
	}



	// STEP: Initialize global pointers to NULL
	{
		g_current_fte = NULL;
		g_capture_fte = NULL;
	}
}





/** Print out debug information to the GUI
*/
void FT_PrintFrameTable()
{
	CPU_MSR msr;

	xil_printf("FT_PrintFrameTable()\r\n");
	
	msr = DISABLE_INTERRUPTS();
	{
		int i,j;
		for(i = 0 ; i < NUM_FRAME_TABLE_ENTRIES ; i++)
		{	
			xil_printf("FT_PrintFrameTable(): i:\t%d\tloc: 0x%x\r\n", i, &g_frametable[i]);
			xil_printf("FT_PrintFrameTable(): status:\t%d\r\n", g_frametable[i]->status);
						
			for(j = 0 ; j < VISION_MAX_NUM_FRAMES ; j++)
			{
			  if(j != VISION_FRAME_NOT_AVAILABLE)
			    xil_printf("FT_PrintFrameTable(): Frame %d :\t0x%x\r\n", j,(uint32)g_frametable[i]->frame_address[j]);
			}
		}
	}
	RESTORE_INTERRUPTS(msr);
} 





/** Initiate a camera core frame capture into the specified FTE
	The FT_StartCapture() function is called at the end of the 
	FT_InterruptHandlerFrameTable() interrupt handler and will configure the 
	camera core to write the various frames to those pointed to by the 
	FrameTable[0] entry. And it will then initiate a new frame capture.

	
	STEPS:
	-# Check in any Static Buffer frames that are still in the FTE
	-# Determine which Frames need to be saved to memory
	-# Checkout the buffers for those frames
	-# Write save options to the Camera Core (of the succesfully checkout buffers)
	-# Write Frame memory addresses to camera core
	-# Write various other options to the camera core
	-# Initiate Camera Capture	
	-# Apply camera chip settings
*/
void FT_StartCapture(FrameTableEntry* fte)
{	
	uint32 save_options;
	
	if(fte == NULL)
		return;

	
	// STEP: check in any Static Buffer frames that are still in the FTE
	{
		int i;
		for(i = 0 ; i < VISION_MAX_NUM_FRAMES ; i++)
			BSCheckIn(&fte->frame_address[i]);							
		
	}


	// STEP: Determine which Frames need to be saved to memory
	save_options =   rdhex(X_CAM_SAVE_OPTIONS_BASE) 
	               | rdhex(X_CAM_SAVE_OPTIONS_EXTENDED) 
	    		   | rdhex(X_STATIC_BUFFER_OPTIONS);

	save_options = 0x2;
	/* STEP: Checkout the buffers for those frames
			 In this step, we look at the local save_options variable. Each 
			 bit represents whether the frame at that index should be saved or
			 not. If the (save_options & 0x01) bit is a 1 then we save the frame 
			 at index 0x01, otherwise we do not save the frame at index 0x01.

			 To do this we use a bit mask. We loop through all of the 16 possible 
			 frame indexes (the camera core can only support up to 16 frames) and 
			 test if each one has been #requested# to be saved. If the frame at 
			 a particular bit index has been requested to be saved then we find out 
			 what Camera Frame Type that frame is. If the frame at that index turns
			 out to be unavailavle then we don't save it and we clear the bit in the
			 save_options since we cannot save a frame that doesn't exist. That situation
			 would be a rare occurance. Normally the frame type will one of the possible 
			 image or lists and delineated in plb_camera.h. Once we have found the type
			 of frame we are going to capture, then we determine what type of Buffer
			 from the BufferStore needs to be checked out to put that frame in. If
			 the checked out buffer turns out to be NULL, then we have hit an error 
			 some where and the BufferStore doesn't know what type of Buffer corresponds 
			 with that kind of Camera Frame. Again, if we cannot checkout a buffer for 
			 that frame then we need to clear the bit in the save_options because we 
			 cannot save that frame since we do not have a place in memory to write it to.
	*/
	{
		int i;
		for(i = 0 ; i < VISION_MAX_NUM_FRAMES ; i++)
		{
			uint32 mask;

			//Initially set the address to 0
			fte->frame_address[i] = NULL;

			mask = 0x01 << i;
			if(save_options & mask)
			{
				uint32 type = camera0.frame_type[i]; // i.e. type = VISION_FRAME_RGB565

				if(type != VISION_FRAME_NOT_AVAILABLE)
				{
					Buffer* buf = BSCheckOut(BSCamType2BufferType(type));
					
					if(buf != NULL) // The buffer was sucessfully checked out...
						fte->frame_address[i] = buf;
					else 
					{	// The buffer was NOT successfuly checked out so don't try and save the frame...					
						//HeliosSetLED2(!HeliosReadLED2());
						save_options &= (~mask);
					}
				} else 
				{
				  // The buffer was NOT successfuly checked out so don't try and save the frame...				
				  save_options &= (~mask);
				}
			}		
		}
	}


	// STEP: Write save options to the Camera Core 
	//SetFrameSaveOptions(camera0.base_address, save_options);					

	/* STEP:	Write Frame memory addresses to camera core
				The Camera Core uses DMA to transfer incomming image data 
				directly to main memory. So the Camera Core needs to be 
				told the 32-bit memory addres where it can write the image 
				data. Here we write the 32-bit memory addresses to the 
				Camera Core 
	*/
	{
		int i;
		for(i = 0 ; i < VISION_MAX_NUM_FRAMES ; i++)
		{
			uint32 ptr = 0;

			if(fte->frame_address[i] != NULL)
				ptr = (uint32)fte->frame_address[i]->data.voidptr;
			SetFrameAddress(i, ptr);
		}
	}


	// STEP: 5: Write various other options to the camera core

	
	// STEP 6: Initiate Camera Capture
	StartFrameCapture();
	fte->status		= FT_STATUS_ACQUIRING;
	g_capture_fte	= fte;	


	// STEP 7: Apply camera chip settings
	// 1: Exposure
	// 2: Gain
#if DAUGHTER_BOARD == DAUGHTER_BOARD_AVT
	{
		uint32 mask;

		mask = (rdchar(C_ENABLE_AUTO_GAIN) << 1) | rdchar(C_ENABLE_AUTO_EXPOSURE);
		mask = mask & 0x03;
		
		WriteCameraRegister(0xAF, mask);
		WriteCameraRegister(0x0B, rduint(U_MANUAL_EXPOSURE));
		WriteCameraRegister(0x35, rduint(U_MANUAL_GAIN));
	}
#endif
}




 
/**
	This handler is only called when the frame grabber hardware core has finnished reading in a frame to memory
	The InterruptHandlerFrameTable() is the interrupt handler that is called 
	each time the camera core has finished captureing a frame from the camera.
 
	STEPS: 
	-#  Calculate FPS_CAM
	-#  Update status of recently completed frame
	-#  Read out the ending frame pointer address and store them in the FrameTableEntry
	-#  Check in any buffers ownned by the old Entry if it was missed
	-#  Initiate the next frame capture
 */ 
void FT_InterruptHandlerFrameTable()
{
	static uint32 msLastFrame = 0;
	CPU_MSR msr;

	msr = DISABLE_INTERRUPTS();
	{
		wruint(U_FRAME_COUNT, rduint(U_FRAME_COUNT)+1);


		//Step: Calculate FPS_CAM
		{	
			uint32 now  = rduint(U_MS);
			uint32 diff = now - msLastFrame;
			msLastFrame = now;
			if(diff != 0)	wruint(U_FPS_CAM, 1000/diff );
			else			wruint(U_FPS_CAM, 0);
		}
		
		

		// STEP: Update status of recently completed frame
		{
			g_capture_fte->status 				= FT_STATUS_AVAILABLE;
			g_capture_fte->frameCount 			= rduint(U_FRAME_COUNT);
			g_capture_fte->camera				= &camera0;
			g_capture_fte->saveoptions 			= GetFrameSaveOptions();
		}
		

		
		// STEP: Read out the ending frame pointer address and store them in the FrameTableEntry
		//       This is important in the accurate determination of the length of list data 
		//       produced by the Camera Core
		{
			int i;
			for(i = 0 ; i < VISION_MAX_NUM_FRAMES ; i++)
				g_capture_fte->frame_address_end[i] = (uint8*) GetFrameAddress(i);
		}
		

		// STEP: Check in any buffers ownned by the old Entry if it was missed
		if((g_current_fte != NULL) && (g_current_fte->status == FT_STATUS_AVAILABLE))
		{			
			FT_CheckInFrame(g_current_fte);
			g_current_fte->status = FT_STATUS_MISSED;			
		}



			
		// STEP: Initiate the next frame capture
		g_current_fte = g_capture_fte;
		FT_StartCapture(g_capture_fte->next);

	}
	RESTORE_INTERRUPTS(msr);	
}


 


/** Checkout the most recently acquired FTE
	The user will typically call FT_CheckOutFrame() and perform some 
	apllication specific processing on the frames and then once finished, 
	the user will then check that FrameTableEntry back in so that it can 
	be used again.

\return Returns the most recently completed frame. Returns NULL if no new frame is available.
*/ 
FrameTableEntry* FT_CheckOutFrame()
{	
	FrameTableEntry* fte = NULL;

	CPU_MSR msr = DISABLE_INTERRUPTS();
	{		
		if(g_current_fte->status == FT_STATUS_AVAILABLE)
		{
			fte				= g_current_fte;
			fte->status		= FT_STATUS_INUSE;
		}

		//Calculate FPS of Software Processing System
		{
			static uint32 msLastFrame = 0;
			if(fte != NULL)
			{	
				uint32 time = rduint(U_MS);
				uint32 diff = time - msLastFrame;
				msLastFrame = time;
				if(diff != 0)	wruint(U_FPS, 1000/diff );
				else			wruint(U_FPS, 0);			
			}
		}
	}
	RESTORE_INTERRUPTS(msr);

	return fte;
} 




/** Checkin an FTE that was checked out for processing
	The user will typically call FT_CheckOutFrame() and perform some 
	apllication specific processing on the frames and then once finished, 
	the user will then check that FrameTableEntry back in so that it can 
	be used again.

	Checking in an FTE will, by default, CheckIn all the Buffers that the 
	FTE owns back into the BufferStore. This will cause the FTE to loose 
	some of its information. So, if there is something that you want to save
	then you have better set a static_buffer_option or place it in some other 
	field in the FTE or else it might get lost.

	The static_buffer_option is a bit vector where a 1 indicates that a Buffer
	in the frame_address array should not be checked back in by default. Be 
	careful how this is used. If you for get to checkin some types of buffers
	then the BufferStore will run out of them and then new frames will not be 
	able to check them out. 
*/ 
void FT_CheckInFrame(FrameTableEntry* fte)
{
	CPU_MSR msr = DISABLE_INTERRUPTS();
	{
		if(fte!= NULL)
		{
			int i;
			uint32 static_buffer_options;

			fte->status = FT_STATUS_COMPLETED;	
			
			// Check in the Buffers that this FTE had checked out
			// But do not check in the Static Buffers
			static_buffer_options = rdhex(X_STATIC_BUFFER_OPTIONS);			
			for(i = 0 ; i < VISION_MAX_NUM_FRAMES ; i++)
			{
				uint32 mask = 0x01 << i;
				if(!(static_buffer_options & mask))
					BSCheckIn(&fte->frame_address[i]);									
			}

		}
	}
	RESTORE_INTERRUPTS(msr);
}








