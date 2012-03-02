/**
\unit
\author Barrett Edwards

FILE:				Comm.h
AUTHOR:				Barrett Edwards	
CREATED:			13 July 2006	
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Communication Framework
COMPANION FILES:	Comm.c
DESCRIPTION:		

	This file should contain the subroutines to transmitt and receive packet
	based communication with external devices (i.e. the GUI)
	
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

/* Conditional Inclusion ----------------------------------------------------*/
#ifndef COMM_H
#define COMM_H


/* Includes -----------------------------------------------------------------*/
#include "Header.h"
#include "FrameTable.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/

void RX_ProcessPacket(HeliosPacketHeader* header, uint8* buffer);

void TX_SendState();
void TX_SendImage(FrameTableEntry* fte, uint8 burst);
void TX_SendPacket(uint8 type, uint32 buffer_size, uint8* buffer);

#endif
