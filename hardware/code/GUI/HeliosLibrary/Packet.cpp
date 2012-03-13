#include "StdAfx.h"
#include "Packet.h"


bool Packet::ParseAndVerifyBuffer(void* buffer, HeliosPacketHeader* header, void** data)
{
	unsigned char* buf = (unsigned char*) buffer;

	/* Steps:
	 * 1: Read buffer into header 
	 * 2: Verify Header
	 * 3: Verify Data
	 */

	// Step 1: Read buffer into header 
	Packet::BufferToHeader(header,buffer);
	*data = (void*) &buf[10];

	// Step2: Verify Header
	if(header->magic  != 0x00BE)
	{
		printf("-------------------------------\r\n");
		printf("\t\tPacket::ParseAndVerifyBuffer() Magicc Number Failed. ");
		printf(" %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\r\n"	,buf[0]
																					,buf[1]
																					,buf[2]
																					,buf[3]
																					,buf[4]
																					,buf[5]
																					,buf[6]
																					,buf[7]
																					,buf[8]
																					,buf[9]
																					,buf[10]
																					,buf[11]
																					,buf[12]
																					,buf[13]
																					,buf[14]
																					,buf[15]
																					,buf[16]
																					,buf[17]
																					,buf[18]
																					,buf[19]
																					);
		return false;
	}
	
	int checksum_header = Packet::ComputeHeaderChecksum(header);
	if(header->checksum_header != checksum_header) 
	{
		printf("-------------------------------\r\n");
		printf("\t\tPacket::ParseAndVerifyBuffer() Verify Header Checksum Failed. ");
		printf("\t\tMagicc: %d \r\nPacket Type: %d. \r\nHeader.hchecksum:%x   \r\nDataChecksum %d  \r\nbuffersize %x \r\ncomputed checksum: %x\r\n", header->magic,header->type,header->checksum_header,header->checksum_data,header->buffer_size,checksum_header);
		return false;
	}

	
	// Step2: Verify Data
	if(header->buffer_size > 0 && header->type >= PKT_TEXT){
		int checksum_data = Packet::ComputeDataChecksum(&buf[10], header->buffer_size);
		if(header->checksum_data != checksum_data) {
			printf("-------------------------------\r\n");
			printf("\t\tPacket::ParseAndVerifyBuffer() Verify Data Checksum Failed. ");
			printf("\t\tMagicc: %d \r\nPacket Type: %d. \r\nHeader.hchecksum:%x   \r\nDataChecksum %d  \r\nbuffersize %x \r\ncomputed checksum: %x\r\n", header->magic,header->type,header->checksum_header,header->checksum_data,header->buffer_size,checksum_data);
			return false;
		}
	}
	
	return true;
}


void Packet::BufferToHeader(HeliosPacketHeader* header, void* buffer)
{
	unsigned char* buf = (unsigned char*) buffer;

	header->magic			= buf[0];
	header->type			= buf[1];
	header->checksum_header	= buf[2] <<  8 | buf[3];
	header->checksum_data	= buf[4] <<  8 | buf[5];
	header->buffer_size		= buf[6] << 24 | buf[7] << 16 | buf[8] << 8 | buf[9];
}

void Packet::HeaderToBuffer(HeliosPacketHeader* header, void* buffer)
{
	unsigned char* buf = (unsigned char*) buffer;

	buf[0] = (unsigned char) ((header->magic					)& 0x00FF);
	buf[1] = (unsigned char) ((header->type						)& 0x00FF);	
	
	buf[2] = (unsigned char) ((header->checksum_header	>> 8	)& 0x00FF);	
	buf[3] = (unsigned char) ((header->checksum_header  		)& 0x00FF);	

	buf[4] = (unsigned char) ((header->checksum_data	>> 8	)& 0x00FF);
	buf[5] = (unsigned char) ((header->checksum_data			)& 0x00FF);

	buf[6] = (unsigned char) ((header->buffer_size		>> 24	)& 0x00FF);
	buf[7] = (unsigned char) ((header->buffer_size		>> 16	)& 0x00FF);
	buf[8] = (unsigned char) ((header->buffer_size		>>  8	)& 0x00FF);
	buf[9] = (unsigned char) ((header->buffer_size				)& 0x00FF);	
}

unsigned int Packet::ComputeHeaderChecksum(HeliosPacketHeader* header){	
	unsigned char buffer[10];
	
	Packet::HeaderToBuffer(header,buffer);
	buffer[2] = 0; // clear out hchecksum variable
	buffer[3] = 0; // clear out hchecksum variable

	//printf("Packet::ComputeHeaderChecksum() About to compute header checksum: %x %x %x %x %x %x %x %x %x %x\r\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9]);

	unsigned int rv = ComputeDataChecksum(buffer, 10);
	//qDebug("Packet::ComputeHeaderChecksum() Computed Header Checksum: %x\r\n",rv);
	return rv;
}

unsigned int Packet::ComputeDataChecksum(unsigned char* data, int size){
	//printf("Packet::ComputeDataChecksum(): data:%x bufsize:%d\r\n",data,size);
	
	int			 len  = size;		// Length in 8-bit words 
	unsigned int sum1 = 1;
	unsigned int sum2 = 0;
	unsigned int rv;

	while(len--){
		sum1 += *data++;
		sum2 += sum1;
	}
	/* Second reduction step to reduce sums to 8 bits */
	sum1 += sum2;
	rv    = ((sum1 >> 16) & 0x0000FFFF) ^ (sum1 & 0x0000FFFF);

	//printf("Packet::ComputeDataChecksum() Computed Data Checksum: %x\r\n",rv);
	return rv;
}



///////////////////////////////////////////

void Packet::LoadHeader(HeliosPacketHeader* header, unsigned char type, int buffer_size, unsigned char* buffer){
	header->magic			= 0xBE;
	header->type			= type;
	header->checksum_header	= 0;
	header->checksum_data	= 0;
	header->buffer_size		= buffer_size;	

	if(buffer_size > 0)
		header->checksum_data = Packet::ComputeDataChecksum(buffer, buffer_size);

	header->checksum_header = Packet::ComputeHeaderChecksum(header);
}

///////////////////////////////////////////////////////////////////////



unsigned int Packet::SwitchEndian(unsigned int value)
{
	fint fi;
	fi.u = value;
	return (fi.c[0] << 24 | fi.c[1] << 16 | fi.c[2] << 8 | fi.c[3]);	
}








		

		
