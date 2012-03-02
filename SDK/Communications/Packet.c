/**
\file
\author Barrett Edwards

FILE:				Packet.c
AUTHOR:				Barrett Edwards
CREATED:			10 Nov 2006	
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Communication Framework
COMPANION FILES:	Packet.h
DESCRIPTION:		See Packet.h
	
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

/* Includes -----------------------------------------------------------------*/
#include <string.h>

#include "Header.h"
#include "Packet.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/


/** \brief	Computes the checksums on the packet header and data buffers and 
			verifies that they match  the checksums in the Heliosheader struct

\return Returns TRUE if the checksum test succeeds 
		Returns FALSE if the checksum test fails
*/
uint8 verifyPacket(HeliosPacketHeader* header, uint8* buffer)
{
	return (verifyHeader(header) && verifyDataBuffer(header, buffer, header->buffer_size));
}




/** \brief	Computes the checksum on the packet header and verifies that it 
			matches the checksum in the Heliosheader struct

\return Returns TRUE if the checksum test succeeds 
		Returns FALSE if the checksum test fails
*/
uint8 verifyHeader(HeliosPacketHeader* header)
{
	int checksum_header;

	if(header->magic  != MAGIC) return FALSE;
	
	checksum_header = ComputeHeaderChecksum(header);
	if(header->checksum_header != checksum_header) return FALSE;

	return TRUE;
}




/** \brief	Computes the checksum on the packet data buffer and verifies that 
			it matches the checksum in the Heliosheader struct

\return Returns TRUE if the checksum test succeeds 
		Returns FALSE if the checksum test fails
*/
uint8 verifyDataBuffer(HeliosPacketHeader* header, uint8* buffer, uint32 buffer_size)
{
	if(buffer_size > 0)
	{
		int checkusm_data = ComputeDataChecksum(buffer, buffer_size);
		if(header->checksum_data != checkusm_data) return FALSE;
	}
	return TRUE;
}




/** Initialize an empty HeliosPacketHeader with supplied parameters
	The main purpose of the LoadHeader() function is to load the Header struct
	with the passed in parameters AND compute the header and data buffer 
	checksums, which are then stored in the header.
*/
void LoadHeader(HeliosPacketHeader* header, uint8 type, uint32 buffer_size, uint8* buffer)
{
	header->magic			= 0x00BE;
	header->type			= type;
	header->checksum_header = 0;
	header->checksum_data	= 0;
	header->buffer_size		= buffer_size;
	

	if(buffer_size > 0 && type >= PKT_TEXT)
		header->checksum_data = ComputeDataChecksum(buffer,buffer_size);

	header->checksum_header = ComputeHeaderChecksum(header);
}



/** Copies the data from the char buffer into the HeliosPacketHeader 
*/
void BufferToHeader(HeliosPacketHeader* header, uint8* buffer)
{
	header->magic			= buffer[0]; 	
	header->type			= buffer[1];  	
	header->checksum_header	= buffer[2]  <<  8 | buffer[3];  
	header->checksum_data	= buffer[4]  <<  8 | buffer[5];	
	header->buffer_size		= buffer[6]  << 24 | buffer[7] << 16 | buffer[8] << 8 | buffer[9];	
	
	// We can do this this way be cause the PowerPC is BigEndian
	//memcpy(header, buffer, sizeof(HeliosPacketHeader));
}




/** Copies the data from the HeliosPacketHeader to the char buffer
*/
void HeaderToBuffer(HeliosPacketHeader* header, unsigned char* buffer)
{	
	buffer[0] = (unsigned char) ((header->magic						)& 0x00FF);
	buffer[1] = (unsigned char) ((header->type						)& 0x00FF);	
	
	buffer[2] = (unsigned char) ((header->checksum_header	>> 8	)& 0x00FF);	
	buffer[3] = (unsigned char) ((header->checksum_header  			)& 0x00FF);	

	buffer[4] = (unsigned char) ((header->checksum_data     >> 8	)& 0x00FF);
	buffer[5] = (unsigned char) ((header->checksum_data				)& 0x00FF);

	buffer[6] = (unsigned char) ((header->buffer_size 		>> 24	)& 0x00FF);
	buffer[7] = (unsigned char) ((header->buffer_size		>> 16	)& 0x00FF);
	buffer[8] = (unsigned char) ((header->buffer_size		>>  8	)& 0x00FF);
	buffer[9] = (unsigned char) ((header->buffer_size				)& 0x00FF);	
	

	// We can do this this way be cause the PowerPC is BigEndian
	// But not in the simulator
	//memcpy(buffer, header, sizeof(HeliosPacketHeader));
}




/** Computes the checksum of a character array
	The main purpose of this function is to compute a checksum that can be used
	to verify the integrity of data transmitted over a communication channel.

	The checksum algorithm is based on the Adler-32 checksum algorithm. See 
	http://en.wikipedia.org/wiki/Adler-32
	
	This algorithm is comprised of two main steps:
	-# Generate summs
	-# Concatenate the sums in to a 16-bit checksum

	In the first step, we step through the data one byte at a time. For each
	byte of data we add the byte to a 32-bit sum (sumA). The sumA variable is
	then added to the sumB variable. By orchastrating the sums in this way,
	we guarentee that the sums will change values no matter what value the 
	data is. 

	In particular this will catch a dropped zero error. Say for example the 
	data has a bunch of consecutive zeros in the data array. When we go to 
	add the 0 value to the sumA, it will not change. But when we add sumA 
	sumB, then sumB will change because sumA will not be 0. 

	The second step is to take two 32-bit values and generate a single 16-bit
	value that can be used as a checksum. We want to incorporate each of the 
	bits of the two 32-bit sums, so first we add them together. This results
	in a single 32-bit number. Then we take the top 16 bits of that number 
	and XOR them with the lower 16 bits which results in a 16 bit number.
*/
uint32 ComputeDataChecksum(uint8* data, uint32 len)
{	
	uint32	sumA = 1;
	uint32	sumB = 0;
	uint32  rv   = 0;

	while(len--) // Length in bytes
	{
		sumA += *data++;
		sumB += sumA;
	}

	// Combine the two sums and reduce to reduce to 16 bits
	sumA += sumB;
	rv    = ((sumA >> 16) & 0x0000FFFF) ^ (sumA & 0x0000FFFF);

	return rv;
}



/** Computes the 16-bit checksum on the data contained strictly within the HeliosPacketHeader
	To do this we first have to zero out the header checksum variable in the 
	header, because the header checksum in the header struct was zero when we 
	computed the checksum to put in there.
*/
uint32 ComputeHeaderChecksum(HeliosPacketHeader* header)
{
	uint8 buffer[10];
	
	HeaderToBuffer(header,buffer);
	buffer[2] = 0; // clear out headerecksum variable
	buffer[3] = 0; // clear out headerecksum variable

	return ComputeDataChecksum(buffer, 10);
}

