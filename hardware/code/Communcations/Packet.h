/**
\unit
\author Barrett Edwards

FILE:				Packet.h
AUTHOR:				Barrett Edwards
CREATED:			20 July 2006	
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Communication Framework
COMPANION FILES:	Packet.c
DESCRIPTION:		
	
	This file implements the functionality to construct and verify packets for
	communication with external devices (usually the GUI). 

	Transmit example: In this example, the code wants to create a packet that
	will send some text over the communication channel. The LoadHeader() 
	function will compute the checksum for data and the header and store them
	in the HeliosPacketHeader. 
	\pre
	...
	HeliosPacketHeader header;
	char msg[] = "Hello World!";
	LoadHeader( &header, PKT_TEXT, strlen(msg), (uint8*) msg);
	...send packet header over communication channel here
	...send packet data (msg) over communication channel here
	...
	\endpre




	Related Functions & Groups
	- Verification
		- uint8  verifyPacket(HeliosPacketHeader* header, uint8* buffer);
		- uint8  verifyHeader(HeliosPacketHeader* header);
		- uint8  verifyDataBuffer(HeliosPacketHeader* header, uint8* buffer, uint32 buffer_size);
	- Checksum Operations
		- uint32 ComputeDataChecksum(uint8* data, uint32 size);
		- uint32 ComputeHeaderChecksum(HeliosPacketHeader* header);
	- Header Operations
		- void LoadHeader(HeliosPacketHeader* header, uint8 type, uint32 buffer_size, uint8* data);
		- void BufferToHeader(HeliosPacketHeader* header, uint8* buffer);
		- void HeaderToBuffer(HeliosPacketHeader* header, uint8* buffer);

CHANGE LOG
- 01 Aug 2007 BBE Added Doxygen style comments

*/ 

/* Conditional Inclusion ----------------------------------------------------*/
#ifndef PACKET_H
#define PACKET_H


/* Includes -----------------------------------------------------------------*/
#include <stdio.h>

#include "Header.h"


/* Macros -------------------------------------------------------------------*/
/// This is the magic number that is the first byte of a communcation Packet
#define MAGIC 0xBE


/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/

uint8  verifyPacket(HeliosPacketHeader* header, uint8* buffer);
uint8  verifyHeader(HeliosPacketHeader* header);
uint8  verifyDataBuffer(HeliosPacketHeader* header, uint8* buffer, uint32 buffer_size);
uint32 ComputeDataChecksum(uint8* data, uint32 size);
uint32 ComputeHeaderChecksum(HeliosPacketHeader* header);

void LoadHeader(HeliosPacketHeader* header, uint8 type, uint32 buffer_size, uint8* data);
void BufferToHeader(HeliosPacketHeader* header, uint8* buffer);
void HeaderToBuffer(HeliosPacketHeader* header, uint8* buffer);

#endif
