/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:	 MemoryTest.c
AUTHOR:	 Wade Fife
CREATED: 11/26/05

DESCRIPTION

This module contains code to test off-chip system memory. It is written
specifically for testing 32-bit memories. Care should be used when testing
memories of other widths.

CHANGE LOG

11/26/05 WSF Created original file.
03/17/06 WSF Added DATA_MASK to allow testing of only certain bits of the data
             bus.

******************************************************************************/

#include <stdio.h>

#include "Serial.h"
#include "Header.h"
#include "MemoryTest.h"


// DEFINES ////////////////////////////////////////////////////////////////////

// Indicate big or little endian byte ordering
#define BIG_ENDIAN
#undef LITTLE_ENDIAN

// Define this mask to indicate which data bits to consider in the test. That
// is, if a bit here is 0, that data bit won't be tested in the code below.
#define DATA_MASK (0xFFFFFFFF)


// Macro to creates individual byte masks based on DATA_MASK
#ifdef BIG_ENDIAN
#define BYTE_MASK(OFFSET) ((uint8)((uint32)(DATA_MASK) >> (8*(~(OFFSET) & 3))))
#else
#define BYTE_MASK(OFFSET) ((uint8)((uint32)(DATA_MASK) >> (8*( (OFFSET) & 3))))
#endif



// FUNCTIONS //////////////////////////////////////////////////////////////////


// Fills a MemError struct with the passed data, if pError != NULL.
// If pError is NULL, then nothing is done.
void FillError(MemError *pError, uint32 addr, uint32 value, 
			   uint32 expected, int size)
{
	if(pError == NULL) return;

	pError->addr = addr;
	pError->value = value;
	pError->expected = expected;
	pError->size = size;
}



// Prints a text string that reports the contents of a MemError
// struct.
//
// If the size field of the MemError struct is 0, then the function
// returns without printing any message.
void ReportError(MemError *pError)
{
	char *sizeString;

	if(pError == NULL) return;
	if(pError->size == 0) return;

	print("\r\nAt 0x");
	xil_printf("%x", pError->addr);
	print(", expected ");
	
	switch(pError->size) {
	case 1:
		sizeString = "byte";
		break;
	case 2:
		sizeString = "half-word";
		break;
	case 4:
		sizeString = "word";
		break;
	default:
		sizeString = "{bad size}";
	}
	
	print(sizeString);
	print(" 0x");
	xil_printf("%x", pError->expected);
	print(", read 0x");
	xil_printf("%x", pError->value);
	print(". ^(");
	xil_printf("%x", ((pError->expected) ^ (pError->value)));
	print(")\r\n");
}



// Zeros a single word then verifies that the word was zeroed.
//
// Returns FALSE and fills the MemError struct if the operation
// fails. Returns TRUE otherwise.
BOOL ZeroAndVerify(uint32 *addr, MemError *pError) {
	volatile uint32 *ptr;
	uint32 value;

	ptr = addr;
	
	// Zero location
	*ptr = 0;

	// Verify that it's zero
	value = (*ptr & DATA_MASK);
	if(value != 0) {
		FillError(pError, (uint32)ptr, value, 0, 4);
		return FALSE;
	}

	return TRUE;
}



// Verifies the contents of the memory region described by addr and
// size to make sure all locations are equal to the given testValue.
//
// Returns FALSE and fills the MemError struct if the check fails.
// REturn TRUE otherwise.
BOOL VerifyMemoryValue32(uint32 *addr, int size, uint32 testValue,
					  MemError *pError)
{
	volatile uint32 *ptr;
	uint32 value;
	int count;

	ptr = addr;
	testValue &= DATA_MASK;

	for(count = 0; count < size; count += 4) {
		value = *ptr;
		if((value & DATA_MASK) != testValue) {
			FillError(pError, (uint32)ptr, value, testValue, 4);
			return FALSE;
		}
		ptr++;
	} 

	return TRUE;
}



// Fills the indicated memory region with the indicated value.
// The memory size must be a multiple of 32 bytes!
void SetMemory(uint32 *addr, int size, uint32 value)
{
	int count;
	volatile uint32 *ptr;

	ptr = addr;
	
	for(count = 0; count < size; count += 32) {
		ptr[0] = value;
		ptr[1] = value;
		ptr[2] = value;
		ptr[3] = value;
		ptr[4] = value;
		ptr[5] = value;
		ptr[6] = value;
		ptr[7] = value;
		ptr += 8;
	}
}



// Writes zeros to memory, then verifies results.
//
// Returns FALSE if fails and fills the MemError struct. Returns TRUE
// if successful.
BOOL TestWriteZeros32(uint32 *addr, int size, MemError *pError)
{
	volatile uint32 *ptr;
	uint32 *startPtr, *endPtr;
	uint32 value;

	startPtr = (uint32 *)addr;
	endPtr = (uint32 *)(addr + size/sizeof(*ptr));

	// Zero memory
	SetMemory(addr, size, 0);

	// Verify it's cleared
	for(ptr = startPtr; ptr < endPtr; ptr++) {
		value = *ptr & DATA_MASK;
		if(value != 0) {
			FillError(pError, (uint32)ptr, value, 0, 4);
			return FALSE;
		}
	}	

	return TRUE;
}



// Writes ones to memory, then verifies results.
//
// Returns FALSE if fails and fills the MemError struct. Returns TRUE
// if successful.
BOOL TestWriteOnes32(uint32 *addr, int size, MemError *pError)
{
	volatile uint32 *ptr;
	uint32 *startPtr, *endPtr;
	uint32 value;

	startPtr = (uint32 *)addr;
	endPtr = (uint32 *)(addr + size/sizeof(*ptr));

	// Set memory to all ones
	SetMemory(addr, size, ~0);

	// Verify it's cleared
	for(ptr = startPtr; ptr < endPtr; ptr++) {
		value = *ptr & DATA_MASK;
		if(value != (~0 & DATA_MASK)) {
			FillError(pError, (uint32)ptr, value, ~0 & DATA_MASK, 4);
			return FALSE;
		}
	}	

	return TRUE;
}



// Write the address to memory, then verify results.
//
// Returns FALSE if fails, TRUE otherwise.  If FALSE, *pError struct
// will be filled.
BOOL TestWriteAddress32(uint32 *addr, int size, MemError *pError)
{
	volatile uint32 *ptr;
	uint32 *startPtr, *endPtr;
	uint32 value;

	startPtr = (uint32 *)addr;
	endPtr = (uint32 *)(addr + size/sizeof(*ptr));

	// Write addresses to memory
	for(ptr = startPtr; ptr < endPtr; ptr++) *ptr = (uint32)ptr;

	// Verify values are correct
 	for(ptr = startPtr; ptr < endPtr; ptr++) {
		value = (*ptr & DATA_MASK);
		if(value != ((uint32)ptr & DATA_MASK)) {
			FillError(pError, (uint32)ptr, value, (uint32)ptr & DATA_MASK, 4);
			return FALSE;
		}
	}

	return TRUE;
}



// Write the address inverse to memory, then verify results.
//
// Returns FALSE if fails, TRUE otherwise.  If FALSE, *pError struct
// will be filled.
BOOL TestWriteAddressInverse32(uint32 *addr, int size, MemError *pError)
{
	volatile uint32 *ptr;
	uint32 *startPtr, *endPtr;
	uint32 value;

	startPtr = (uint32 *)addr;
	endPtr = (uint32 *)(addr + size/sizeof(*ptr));

	// Write addresses to memory
	for(ptr = startPtr; ptr < endPtr; ptr++) *ptr = ~(uint32)ptr;

	// Verify values are correct
 	for(ptr = startPtr; ptr < endPtr; ptr++) {
		value = *ptr & DATA_MASK;
		if(value != (~(uint32)ptr & DATA_MASK)) {
			FillError(pError, (uint32)ptr, value, ~(uint32)ptr & DATA_MASK, 4);
			return FALSE;
		}
	}

	return TRUE;
}



// Tests writing the value 0xAAAAAAAA followed by 0x55555555, which
// inverts the data bus with each write.
//
// Returns FALSE if the test fails and fills out the MemError
// struct. Return TRUE if successful.
BOOL TestDataPounding32(uint32 *addr, int size, MemError *pError)
{
	volatile uint32 *ptr;
	uint32 *startPtr, *endPtr;
	uint32 value;
	const register uint32 valA = 0xAAAAAAAA;
	const register uint32 val5 = 0x55555555;

	startPtr = (uint32 *)addr;
	endPtr = (uint32 *)(addr + size/sizeof(*ptr));

	// Write addresses to memory
	for(ptr = startPtr; ptr < endPtr; ) {
		*ptr++ = valA;
		*ptr++ = val5;
	}

	// Verify values are correct
 	for(ptr = startPtr; ptr < endPtr; ) {
		value = *ptr & DATA_MASK;
		if(value != (valA & DATA_MASK)) {
			FillError(pError, (uint32)ptr, value, valA & DATA_MASK, 4);
			return FALSE;
		}
		ptr++;
		value = *ptr & DATA_MASK;
		if(value != (val5 & DATA_MASK)) {
			FillError(pError, (uint32)ptr, value, val5 & DATA_MASK, 4);
			return FALSE;
		}
		ptr++;
	}

	return TRUE;
}



// Tests writting the the following addresses in succession: 0xA..A,
// 0x5..5, 0xF..F, 0x0..0 (where the address width is appropriate for
// memory size). This checks that even with rapid change in address
// bits still delivers correct address to memory.
//
// Note: addr is forced to a word boundary.
//
// Returns FALSE if fails and fills out the MemError struct. Returns
// TRUE if successful.
BOOL TestAddressPounding32(uint32 *addr, int size, MemError *pError)
{
	volatile uint8 *ptra;
	volatile uint8 *ptrb;
	volatile uint8 *ptrc;
	volatile uint8 *ptrd;
	int8 aMask, bMask, cMask, dMask;
	uint32 addrMask;
	uint8 value;
	int addrSize;
	const register uint32 vala = 0xA5;
	const register uint32 valb = 0x5A;

	// Determine number of address bits
	addrSize = 0;
	for(addrMask = 1; addrMask <= size; addrMask = (addrMask << 1) | 1) {
		addrSize++;
	}
	addrMask >>= 1;

	// Initialize pointers to use
	addr = (uint32 *)((uint32)addr & (~3));    // Force to word boundary
	ptra = (uint8 *)addr;
	ptrb = (uint8 *)((uint32)addr + (0x55555555 & addrMask));
	ptrc = (uint8 *)((uint32)addr + (0xAAAAAAAA & addrMask));
	ptrd = (uint8 *)((uint32)addr + (0xFFFFFFFF & addrMask));

	aMask = BYTE_MASK(0);
	bMask = BYTE_MASK(0x55555555);
	cMask = BYTE_MASK(0xAAAAAAAA);
	dMask = BYTE_MASK(0xFFFFFFFF);

	// Write values
	*ptra = vala;
	*ptrb = vala;
	*ptrc = vala;
	*ptrd = vala;

	// Verify
	value = (*ptra & aMask);
	if(value != (vala & aMask)) {
		FillError(pError, (uint32)ptra, value, vala & aMask, 1);
		return FALSE;
	}
	value = (*ptrb & bMask);
	if(value != (vala & bMask)) {
		FillError(pError, (uint32)ptrb, value, vala & bMask, 1);
		return FALSE;
	}
	value = (*ptrc & cMask);
	if(value != (vala & cMask)) {
		FillError(pError, (uint32)ptrc, value, vala & cMask, 1);
		return FALSE;
	}
	value = (*ptrd & dMask);
	if(value != (vala & dMask)) {
		FillError(pError, (uint32)ptrd, value, vala & dMask, 1);
		return FALSE;
	}

	// Write values
	*ptra = valb;
	*ptrb = valb;
	*ptrc = valb;
	*ptrd = valb;

	value = (*ptra & aMask);
	if(value != (valb & aMask)) {
		FillError(pError, (uint32)ptra, value, valb & aMask, 1);
		return FALSE;
	}
	value = (*ptrb & bMask);
	if(value != (valb & bMask)) {
		FillError(pError, (uint32)ptrb, value, valb & bMask, 1);
		return FALSE;
	}
	value = (*ptrc & cMask);
	if(value != (valb & cMask)) {
		FillError(pError, (uint32)ptrc, value, valb & cMask, 1);
		return FALSE;
	}
	value = (*ptrd & dMask);
	if(value != (valb & dMask)) {
		FillError(pError, (uint32)ptrd, value, valb & dMask, 1);
		return FALSE;
	}

	return TRUE;	
}



// Test the data bus wiring by writing/reading data to address 0x1,
// then shifting adress to the left and repeating.
//
// Returns FALSE if fails and fills out the MemError struct. Returns
// TRUE if successful.
BOOL TestWalkingOnesData32(uint32 *addr, int size, MemError *pError)
{
	volatile uint32 *ptr;
	uint32 data, value;
	int i;

	ptr = addr;
	data = 1;

	// Run test for single walking bit
	for(i = 0; i < 32; i++) {
		// Write current data value
		*ptr = data;
		
		// Readback and verify
		value = *ptr;
		if(value != data) {
			FillError(pError, (uint32)ptr, value, data, 4);
			return FALSE;
		}

		// Walk data to the left by 1 bit
		data <<= 1;
	}

	data = ~1;

	// Run test for single walking 0
	for(i = 0; i < 32; i++) {
		// Write current data value
		*ptr = data;
		
		// Readback and verify
		value = *ptr & DATA_MASK;
		if(value != (data & DATA_MASK)) {
			FillError(pError, (uint32)ptr, value, data & DATA_MASK, 4);
			return FALSE;
		}

		// Walk data to the left by 1 bit
		data = (data << 1) | 1;
	}

	return TRUE;
}



// Test the address bus wiring by writing/reading data to address 0x1,
// then shifting adress to the left and repeating.
//
// Note that to be complete, this test should verify that the data
// wasn't written to any other address than the address under
// test. For simplicity (assuming a single RAM), this code does NOT
// perform this check.
//
// Returns FALSE if fails and fills out the MemError struct. Returns
// TRUE if successful.
BOOL TestWalkingOnesAddress32(uint32 *addr, int size, MemError *pError)
{
	volatile uint8 *ptr;
	uint32 offset;
	uint8 data, value;
	int i;
	int addrSize, addrMask;

	// Determine number of address bits
	addrSize = 0;
	for(addrMask = 1; addrMask <= size; addrMask = (addrMask << 1) |1) {
		addrSize++;
	}
	addrMask >>= 1;

	offset = 1;
	data = 0xA5;

	// Run test for single walking 1
	for(i = 0; i < addrSize; i++) {
		// Determine new address
		ptr = (volatile uint8 *)((uint32)addr + offset);

		// Write current data value
		*ptr = data;
		
		// Readback and verify
		value = *ptr & BYTE_MASK(offset);
		if(value != (data & BYTE_MASK(offset))) {
			FillError(pError, (uint32)ptr, value, data & BYTE_MASK(offset), 1);
			return FALSE;
		}

		// Walk address to the left by 1 bit
		offset <<= 1;
	}

	offset = (~1) & addrMask;

	// Run test for single walking 0
	for(i = 2; i < addrSize; i++) {
		// Determine new address
		ptr = (volatile uint8 *)((uint32)addr + offset);

		// Write current data value
		*ptr = data;
		
		// Readback and verify
		value = *ptr & BYTE_MASK(offset);
		if(value != (data & BYTE_MASK(offset))) {
			FillError(pError, (uint32)ptr, value, data & BYTE_MASK(offset), 1);
			return FALSE;
		}

		// Walk address to the left by 1 bit
		offset = ((offset << 1) | 1) & addrMask;
	}

	return TRUE;
}



// Writes bytes to memory in groups of 4 to non-consecutive byte
// addresses. Tests the capability of the memory to correctly perform
// byte writes.
//
// Returns FALSE and fills the MemError struct if the operation
// fails. Returns TRUE if successful.
BOOL TestByteWrite(uint32 *addr, int size, MemError *pError)
{
	volatile uint8 *ptr;
	uint8 *startPtr, *endPtr;
	const register uint8 valAA = 0xAA;
	const register uint8 val55 = 0x55;
	const register uint8 val00 = 0x00;
	const register uint8 valFF = 0xFF;
	register uint8 val0, val1, val2, val3;
	uint8 value;

	ptr = (uint8 *)addr;

	startPtr = (uint8 *)addr;
	endPtr = ((uint8 *)addr) + size;


	// TEST BYTE WRITE ENABLE OPERATION //

	if(!ZeroAndVerify((uint32 *)ptr, pError)) return FALSE;
	
	// Test byte write of 0th byte
	ptr[0] = valFF;
	value = ptr[0] & BYTE_MASK(0);
	if(value != (valFF & BYTE_MASK(0))) {
		FillError(pError, (uint32)ptr, value, valFF & BYTE_MASK(0), 1);
		return FALSE;
	}
	
	if(!ZeroAndVerify((uint32 *)ptr, pError)) return FALSE;

	// Test byte write of 3rd byte
	ptr[3] = valFF;
	value = ptr[3] & BYTE_MASK(3);
	if(value != (valFF & BYTE_MASK(3))) {
		FillError(pError, (uint32)ptr+3, value, valFF & BYTE_MASK(3), 1);
		return FALSE;
	}
	
	if(!ZeroAndVerify((uint32 *)ptr, pError)) return FALSE;
	
	// Test byte write of 1st byte
	ptr[1] = valFF;
	value = ptr[1] & BYTE_MASK(1);
	if(value != (valFF & BYTE_MASK(1))) {
		FillError(pError, (uint32)ptr+1, value, valFF & BYTE_MASK(1), 1);
		return FALSE;
	}

	if(!ZeroAndVerify((uint32 *)ptr, pError)) return FALSE;

	// Test byte write of 2rd byte
	ptr[2] = valFF;
	value = ptr[2] & BYTE_MASK(2);
	if(value != (valFF & BYTE_MASK(2))) {
		FillError(pError, (uint32)ptr+2, value, valFF & BYTE_MASK(2), 1);
		return FALSE;
	}

	if(!ZeroAndVerify((uint32 *)ptr, pError)) return FALSE;


	// TEST BYTE WRITES OVER WHOLE MEMORY //

	// Write bytes to memory
	for(ptr = startPtr; ptr < endPtr; ptr += 4) {
		ptr[0] = val55;
		ptr[3] = valAA;
		ptr[1] = val00;
		ptr[2] = valFF;
	}

	// Verify correct values were written by doing byte reads
	for(ptr = startPtr; ptr < endPtr; ptr += 4) {
		val0 = ptr[0] & BYTE_MASK(0);
		val1 = ptr[1] & BYTE_MASK(1);
		val2 = ptr[2] & BYTE_MASK(2);
		val3 = ptr[3] & BYTE_MASK(3);
		if(val0 != (val55 & BYTE_MASK(0))) {
			FillError(pError, (uint32)ptr, val0, val55 & BYTE_MASK(0), 1);
			return FALSE;
		}
		if(val1 != (val00 & BYTE_MASK(1))) {
			FillError(pError, (uint32)ptr+1, val1, val00 & BYTE_MASK(1), 1);
			return FALSE;
		}
		if(val2 != (valFF & BYTE_MASK(2))) {
			FillError(pError, (uint32)ptr+2, val2, valFF & BYTE_MASK(2), 1);
			return FALSE;
		}
		if(val3 != (valAA & BYTE_MASK(3))) {
			FillError(pError, (uint32)ptr+3, val3, valAA & BYTE_MASK(3), 1);
			return FALSE;
		}
	}

	// Write opposite bytes to memory
	for(ptr = startPtr; ptr < endPtr; ptr += 4) {
		ptr[0] = valAA;
		ptr[3] = val55;
		ptr[1] = valFF;
		ptr[2] = val00;
	}

	// Verify correct values were written by doing byte reads
	for(ptr = startPtr; ptr < endPtr; ptr += 4) {
		val0 = ptr[0] & BYTE_MASK(0);
		val1 = ptr[1] & BYTE_MASK(1);
		val2 = ptr[2] & BYTE_MASK(2);
		val3 = ptr[3] & BYTE_MASK(3);
		if(val0 != (valAA & BYTE_MASK(0))) {
			FillError(pError, (uint32)ptr, val0, valAA & BYTE_MASK(0), 1);
			return FALSE;
		}
		if(val1 != (valFF & BYTE_MASK(1))) {
			FillError(pError, (uint32)ptr+1, val1, valFF & BYTE_MASK(1), 1);
			return FALSE;
		}
		if(val2 != (val00 & BYTE_MASK(2))) {
			FillError(pError, (uint32)ptr+2, val2, val00 & BYTE_MASK(2), 1);
			return FALSE;
		}
		if(val3 != (val55 & BYTE_MASK(3))) {
			FillError(pError, (uint32)ptr+3, val3, val55 & BYTE_MASK(3), 1);
			return FALSE;
		}
	}

	return TRUE;
}



BOOL RunAllMemoryTests(uint32 *addr, int size)
{
	MemError error;

	if(DATA_MASK != 0xFFFFFFFF) {
		print("INFO: Tests will only consider bits 0x");
		PrintWord(DATA_MASK);
		print(" of 32-bit data bus.\r\n");
	}
	
	print("Testing writing of zeros . . . ");
	if(!TestWriteZeros32(addr, size, &error)) {
		ReportError(&error);
		return FALSE;
	}
	else print("OK\r\n");

	print("Testing writing of ones . . . ");
	if(!TestWriteOnes32(addr, size, &error)) {
		ReportError(&error);
		return FALSE;
	}
	else print("OK\r\n");

	print("Testing writing of address . . . ");
	if(!TestWriteAddress32(addr, size, &error)) {
		ReportError(&error);
		return FALSE;
	}
	else print("OK\r\n");
	
	print("Testing writing of address inverse . . . ");
	if(!TestWriteAddressInverse32(addr, size, &error)) {
		ReportError(&error);
		return FALSE;
	}
	else print("OK\r\n");

	print("Testing data bus pounding . . . ");
	if(!TestDataPounding32(addr, size, &error)) {
		ReportError(&error);
		return FALSE;
	}
	else print("OK\r\n");

	print("Testing address bus pounding . . . ");
	if(!TestAddressPounding32(addr, size, &error)) {
		ReportError(&error);
		return FALSE;
	}
	else print("OK\r\n");

	print("Testing walking ones on data bus . . . ");
	if(!TestWalkingOnesData32(addr, size, &error)) {
		ReportError(&error);
		return FALSE;
	}
	else print("OK\r\n");

	print("Testing walking ones on address bus . . . ");
	if(!TestWalkingOnesAddress32(addr, size, &error)) {
		ReportError(&error);
		return FALSE;
	}
	else print("OK\r\n");

	print("Testing writing of bytes . . . ");
	if(!TestByteWrite(addr, size, &error)) {
		ReportError(&error);
		return FALSE;
	}
	else print("OK\r\n");

	return TRUE;
}
