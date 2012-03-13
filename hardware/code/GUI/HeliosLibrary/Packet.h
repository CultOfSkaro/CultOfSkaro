#pragma once

#include "stdafx.h"

class Packet
{
public:	
	Packet(void){}

	static bool		ParseAndVerifyBuffer(void* buffer, HeliosPacketHeader* header, void** data);

	static void		BufferToHeader(HeliosPacketHeader* header, void* buffer);
	static void		HeaderToBuffer(HeliosPacketHeader* header, void* buffer);
	static uint32	SwitchEndian(uint32 value);
	


	static uint32	ComputeHeaderChecksum(HeliosPacketHeader* header);
	static uint32	ComputeDataChecksum(uint8* data, int size);

	static void		LoadHeader(HeliosPacketHeader* header, uint8 type, int buffer_size, uint8* buffer);
};
