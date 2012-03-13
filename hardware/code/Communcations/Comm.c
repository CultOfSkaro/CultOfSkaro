/**
\file
\author Barrett Edwards

FILE:				Comm.c
AUTHOR:				Barrett Edwards	
CREATED:			13 July 2006	
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Communication Framework
COMPANION FILES:	Comm.h
DESCRIPTION:		See Comm.h

	Related Functions & Groups 
	- Receive
		- void RX_ProcessPacket(HeliosPacketHeader* header, uint8* buffer);
	- Transmitt 
		- void TX_SendState();
		- void TX_SendImage(FrameTableEntry* fte, uint8 burst);
		- void TX_SendPacket(uint8 type, uint32 buffer_size, uint8* buffer);

CHANGE LOG
- 01 Aug 2007 BBE Added Doxygen style comments

*/ 

/* Includes -----------------------------------------------------------------*/
#include <string.h>

#include <plb_vision.h>
#include <USB_IO.h>

#include "Header.h"
#include "Timer.h"
#include "Packet.h"
#include "State.h"
#include "HeliosIO.h"
#include "InterruptControl.h"
#include "Comm.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/

/** \brief  Interprate and handle a packed sent from an external device 
			(usually the GUI) to the Helios. 

	The GUI (or other extenal device such as the Magicc lab autopilot) sends
	packets to the Helios. These packets instuct the Helios to perform certain
	actions, like 'start trasmitting the video' or adjust state register 
	values. 
	
	This function looks at the HeliosPacketHeader::type field and interprets 
	the data accordingly. 

	Prior to the execution of this function, the HeliosPacketHeader should have
	been verified using the checksum verification methods in the Packet.h file.
*/ 
void RX_ProcessPacket(HeliosPacketHeader *header, uint8* buffer)
{
	//uint8*  buffer8  = (uint8*)  buffer;
	//uint16* buffer16 = (uint16*) buffer;
	uint32* buffer32 = (uint32*) buffer;
	//float*  bufferf  = (float*)  buffer;
	fint fi;
	switch(header->type){		
	case PKT_IMG_NULL:																			break;
	case PKT_IMG_GRAYSCALE8:																	break;
	case PKT_IMG_RGB565:																		break;
	case PKT_TEXT:																				break;
		
	case PKT_CMD_START:					wrchar(C_RUNNING				, TRUE);				break; 
	case PKT_CMD_STOP:					wrchar(C_RUNNING				, FALSE);				break; 
	case PKT_CMD_VIDEO_ON:				wrchar(C_SENDING_VIDEO			, TRUE);				break;
	case PKT_CMD_VIDEO_OFF:				wrchar(C_SENDING_VIDEO			, FALSE);				break;
	case PKT_CMD_STATE_ON:				wrchar(C_SENDING_STATE			, TRUE);				break;
	case PKT_CMD_STATE_OFF:				wrchar(C_SENDING_STATE			, FALSE);				break;

	/*case PKT_CMD_TRANS_FRAME_OPTION:	wruint(U_FRAME_TRANSMIT_OPTION	 , buffer32[0]);			
										wrhex(X_CAM_SAVE_OPTIONS_EXTENDED , 0x01 << buffer32[0]);
										break;
		*/								
	case PKT_CMD_GUI_COMM_CHANNEL:		wrchar(C_GUI_COMM_CHANNEL		, buffer32[1]);				break;	

	case PKT_FRAME_TABLE_ENTRY_SMALL:																break;
	case PKT_FRAME_TABLE_ENTRY_FULL:																break;

	//GUI has requested a refresh of the State Data
	case PKT_REQ_REGISTER_DATA:			wrchar(C_REQ_STATE_DATA,TRUE);								break;

	// These 4 are used to set specfic registers in the Helios state registers
	case PKT_REG_DATA_CHAR:				wrchar(buffer32[0],buffer32[1]);							break;
	case PKT_REG_DATA_INT:				wrint( buffer32[0],buffer32[1]);							break;
	case PKT_REG_DATA_UINT:				wruint(buffer32[0],buffer32[1]);							break;
	//case PKT_REG_DATA_HEX:				wrhex(buffer32[0],buffer32[1]);								break;
	case PKT_REG_DATA_FLOAT:			fi.u = buffer32[1];		
										wrfloat(buffer32[0],fi.f);									break;
	
	// Just the data arrays
	case PKT_REG_DATA_ARRAY_CHAR:		/* Helios -> GUI only */									break;
	case PKT_REG_DATA_ARRAY_INT:		/* Helios -> GUI only */									break;
	case PKT_REG_DATA_ARRAY_UINT:		/* Helios -> GUI only */									break;
	case PKT_REG_DATA_ARRAY_HEX:		/* Helios -> GUI only */									break;
	case PKT_REG_DATA_ARRAY_FLOAT:		/* Helios -> GUI only */									break;

	default: break;
	}
}




/** Transmitts the state registers to an external device (usually the GUI)
	The state registers are constantly chaging. Both in the user code 
	and in interrupt handler routines so there is no telling when a 
	register might change. This presents a problem for data transmission.
	The packet transmission protocol used by the Helios uses a checksum
	to ensure that a transmission is accurate. This checksum is computed 
	on the data that is being sent, which in this case is the values in the
	state registers. 
	
	#BUT# since the state registers might change at any time, the values in
	the state register arrays cannot be guarenteed to remain constant in the 
	time between when the checksum is computed and when the values are actually
	sent out over the transmission line. #SO# we disable interrupts and then 
	copy the state register arrays into a second set of arrays. Once the data
	is coppied in the second set of arrays interrupts are re-enabled. The 
	checksum is then computed on the copied version of the state register 
	arrays. Then the packets containing the state register data and the 
	corresponding checksum are transmitted.	
*/
void TX_SendState()
{
	CPU_MSR msr;
	
	msr = DISABLE_INTERRUPTS();
	{   // Copy the registers into a second set of arrays before sending them to avoid race conditions
		memcpy((uint8*)M_copy_char, (uint8*)M_char, NUM_MISC_CHAR *sizeof(uint8)	);
		memcpy((uint8*)M_copy_int,  (uint8*)M_int,  NUM_MISC_INT  *sizeof(int32)	);
		memcpy((uint8*)M_copy_uint, (uint8*)M_uint, NUM_MISC_UINT *sizeof(uint32)	);
		memcpy((uint8*)M_copy_hex,  (uint8*)M_hex,  NUM_MISC_HEX  *sizeof(uint32)	);
		memcpy((uint8*)M_copy_float,(uint8*)M_float,NUM_MISC_FLOAT*sizeof(float)	);
	}
	RESTORE_INTERRUPTS(msr);	

	TX_SendPacket(PKT_REG_DATA_ARRAY_CHAR , NUM_MISC_CHAR  *sizeof(uint8) , (uint8*)M_copy_char );	
	TX_SendPacket(PKT_REG_DATA_ARRAY_INT  , NUM_MISC_INT   *sizeof(int32) , (uint8*)M_copy_int  );	
	TX_SendPacket(PKT_REG_DATA_ARRAY_UINT , NUM_MISC_UINT  *sizeof(uint32), (uint8*)M_copy_uint );	
	TX_SendPacket(PKT_REG_DATA_ARRAY_HEX  , NUM_MISC_HEX   *sizeof(uint32), (uint8*)M_copy_hex  );	
	TX_SendPacket(PKT_REG_DATA_ARRAY_FLOAT, NUM_MISC_FLOAT *sizeof(float) , (uint8*)M_copy_float);	
}




/** Transmitts captured image data to an external device (usually the GUI)
	One of the main purpose of the Helios platform is the acquire and process 
	image data. This is the function that wraps some image data into a packet 
	and sends it to the GUI so the Uuser can actually see what the Helios sees.
	To do this we first have to figure out a few things like, what type of image
	data we are sending, how big it is and what communication channel we are 
	using. 

	STEPS: 
	-#  Ensure that the USB is the communcation channel. Image data can be 
		transmitted over other comm channels, like the serial RS-232, but they
		are really really slow compared to the USB 2.0. So we don't even 
		attempt to transmitt large image data over a non-USB connection. 
	-#  The U_FRAME_TRANSMIT_OPTION state register controlls which of the 
		computed framelets should be transmitted. The U_FRAME_TRANSMIT_OPTION 
		is an index ranging from 0 to CAM_MAX_NUM_FRAMES, which is 16 at the
		moment. 
	-#  We then find out what type of Camera framelet is contained at that index
		and the 32-bit memory address where the image data is in main memory
	-#  If the framelet that we want to transmit was not actually saved by the
		Camera Core then we cannot send it because it doesn't exist. So we bail.
	-#  We then compute the size in bytes of the image data
	-#  If we get to this point, then we are ready to send out the image data
		over the USB connection, But we first have to check to see if the USB
		is currently in the middle of a communication transaction. If it is then
		we cannot send over the USB until that transaction is completed. So now 
		there are two options that we can do. We can spin in a while(1) loop and 
		wait until USB is no longer busy or we can just not send the image data.
		Since this implementation is designed for real-time opperation, we cannot
		make assumptions about how long we might wait for the USB to become
		available (which might be never, say if it gets unplugged...). So we 
		just about this transmission
	-#  If we get here then the USB is available for use. We first send the 
		packet header information by hand (one uint16 value at a time). And 
		then we initiate a burst transaction where the USB core on the FPGA
		reads the large image data directly out of memory and sends it over 
		the USB cable. 

	By using a USB burst opperation, the USB core performs a lot of of the 
	grunt work by itself, which frees up the PowerPC to do other things. 	
*/ 
void TX_SendImage(FrameTableEntry* fte, uint8 burst)
{	
	uint8* addr;
	uint32 index, size, type;
	HeliosPacketHeader header;
	
	if(fte == NULL || (rdchar(C_GUI_COMM_CHANNEL) != CHAN_USB))
	{
		xil_printf("SendImage, FTE null or channel not USB\r\n");
		return;
	}
	index = rduint(U_FRAME_TRANSMIT_OPTION);	
	type  = camera0.frame_type[index];	
	addr  = fte->frame_address[VISION_FRAME_RGB565]->data.data8;	
	//xil_printf("frame type %d\r\n", type);
	//xil_printf("RGB565 type %d\r\n", VISION_FRAME_RGB565);

	/*if(!(fte->saveoptions & camera0.frame_save_index[type]))
	{
		xil_printf("SendImage, save options not set\r\n");
		return;
		}*/
	size  = CalculateImageBufferSize(&camera0,VISION_FRAME_RGB565);//type);
	
	if(size == 0)
	{
		xil_printf("SendImage, buffer size == 0\r\n");
		return;
	}
	
	if(USB_Busy())
	{
		xil_printf("SendImage, USB was busy\r\n");
		return;
	}

	LoadHeader(&header, type, size+6, addr);	
	USB_multiWrite((uint16*)&header, sizeof(HeliosPacketHeader)/2);	//Send the Header
	USB_singleWrite((fte->frameCount >> 16 ) & 0x0000FFFF); //USB_forceCommit();// Write the FrameCount
	USB_singleWrite((fte->frameCount       ) & 0x0000FFFF); //USB_forceCommit();
	USB_singleWrite(fte->camera->id);	//USB_forceCommit(); // Write the Id of the camera
	if(burst) 
	  {
	    USB_blockWrite((Xuint32*)addr, size/2);	// Send the Image (Burst)
	  }
	else
	  {
	    USB_multiWrite((uint16*)addr, size/2);		
	    USB_forceCommit();
	  }
	wruint(U_FRAME_COUNT_TX,rduint(U_FRAME_COUNT_TX)+1);
}




/** Transmitts a general communicaion packet over the currently selected communication channel
*/
void TX_SendPacket(uint8 type, uint32 buffer_size, uint8* buffer)
{
	int size;
	HeliosPacketHeader header;

	LoadHeader(&header, type, buffer_size, buffer);
	//switch(rdchar(C_GUI_COMM_CHANNEL)){
	//default:
	//case CHAN_NULL:		break;  	
	/*case CHAN_SERIAL:	SER_SendData(SER_BASEADDR, (uint8*)&header, sizeof(HeliosPacketHeader));	
						SER_SendData(SER_BASEADDR, buffer, buffer_size);
						break;
		*/				
	/*case CHAN_WIRELESS: SER_SendData(WIR_BASEADDR, (uint8*)&header, sizeof(HeliosPacketHeader));	
						SER_SendData(WIR_BASEADDR, buffer, buffer_size);	
						break;*/

	//case CHAN_USB:		
		USB_multiWrite((uint16*)&header, sizeof(HeliosPacketHeader)/2);
		size = buffer_size / 2;
		if(buffer_size & 0x01) 
			size++;
		if (size > 0)
		{
			USB_multiWrite((uint16*)buffer, size);	
			USB_forceCommit();
		}
		else 
			USB_forceCommit();
		//break;
	//}	
}


