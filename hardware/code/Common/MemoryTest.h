/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:	 MemoryTest.h
AUTHOR:	 Wade Fife
CREATED: 11/26/05

DESCRIPTION

Header file for MemoryTest.c.

CHANGE LOG

11/26/05 WSF  Created original file.

******************************************************************************/

#ifndef MEMORY_TEST_H
#define MEMORY_TEST_H


#include "SystemTypes.h"



// DATA TYPES /////////////////////////////////////////////////////////////////

typedef struct {
	uint32 addr;     // Address where error occured
	uint32 value;    // Value that was read from memory
	uint32 expected; // Value that was expected from memory
	uint32 size;     // Size of read/write (1, 2, or 4)
} MemError;



// FUNCTION PROTOTYPES ////////////////////////////////////////////////////////

void FillError(MemError *pError, uint32 addr, uint32 value, 
			   uint32 expected, int size);
void ReportError(MemError *pError);
BOOL VerifyMemoryValue(uint32 *addr, int size, uint32 testValue,
					   MemError *pError);
BOOL TestWriteZeros32(uint32 *addr, int size, MemError *pError);
BOOL TestWriteOnes32(uint32 *addr, int size, MemError *pError);
BOOL TestWriteAddress32(uint32 *addr, int size, MemError *pError);
BOOL TestWriteAddressInverse32(uint32 *addr, int size, MemError *pError);
BOOL TestDataPounding32(uint32 *addr, int size, MemError *pError);
BOOL TestAddressPounding32(uint32 *addr, int size, MemError *pError);
BOOL TestWalkingOnesAdress32(uint32 *addr, int size, MemError *pError);
BOOL TestWalkingOnesData32(uint32 *addr, int size, MemError *pError);
BOOL TestByteWrite(uint32 *addr, int size, MemError *pError);
BOOL RunAllMemoryTests(uint32 *addr, int size);


#endif
