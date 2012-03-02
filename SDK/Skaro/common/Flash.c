/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    Flash.c
AUTHOR:  Wade S. Fife
CREATED: 11/26/05

DESCRIPTION

This module contains code to read, write, and manage a 16-bit flash. The code
here is specifically intended for a 16-bit flash interface and should NOT be
used for an 8-bit or 32-bit interface without modification.

CHANGE LOG

11/26/05  WSF  Created original file.

******************************************************************************/

#include <stdio.h>

#include "Header.h"
#include "Flash.h"



// FUNCTIONS //////////////////////////////////////////////////////////////////


// Determines and returns the size of a flash based on the size of its address
// bus (n).
static int FlashSize(int n)
{
	int p;

	if(n == 0) return 0;
	n--;

	p = 2;
	while(n--) p *= 2;

	return p;
}



// Communicates with a 16-bit flash device to verify that it is there. Fills
// the Flash data struct with the size, block size, block address mask, and
// base address.
//
// Returns TRUE if successful, FALSE if no device is detected.
BOOL QueryDevice16(uint8 *addr, Flash *pFlash)
{
	volatile uint8 *ptr;
	int numBlocks;
	char QRY[3];

	ptr = addr;

	// Enter read query mode
	SEND_FLASH16(ptr, FLASH_CMD_READ_QUERY);
	QRY[0] = READ_FLASH16(ptr, 0x10);
	QRY[1] = READ_FLASH16(ptr, 0x11);
	QRY[2] = READ_FLASH16(ptr, 0x12);
	if(!(QRY[0] == 'Q' && QRY[1] == 'R' && QRY[2] == 'Y')) return FALSE;

	pFlash->addr = addr;
	
	// Query flash size
	pFlash->size = FlashSize(READ_FLASH16(ptr, 0x27));

	// Query number of blocks
	numBlocks = ((READ_FLASH16(ptr, 0x2E) << 8) | READ_FLASH16(ptr, 0x2D)) + 1;
	pFlash->blockSize = pFlash->size / numBlocks;

	// Create block mask for generating block addresses
	pFlash->blockMask = ~(pFlash->blockSize - 1);

	// Restore read array mode
	SEND_FLASH16(ptr, FLASH_CMD_READ_ARRAY);

	return TRUE;
}



// Erases the block in a flash memory starting at the given address.
//
// WARNING: Block address must be on a block aligned! For example, if the block
// size is 128 KB than the lower 17 bits of addr must be 0.
//
// Returns TRUE if successful, FALSE if an error occurs.
BOOL EraseFlashBlock(uint8 *addr)
{
	volatile uint8 *ptr;
	uint16 status;

	ptr = addr;

	// Clear status register
	SEND_FLASH16(ptr, FLASH_CMD_CLEAR_STATUS_REG);

	// Send erase command
	SEND_FLASH16(ptr, FLASH_CMD_BLOCK_ERASE_0);
	SEND_FLASH16(ptr, FLASH_CMD_CONFIRM);

	// Wait for completion
	while(((status = READ_FLASH16(ptr, 0)) & FLASH_STATUS_ISMS)
		  != FLASH_STATUS_ISMS);
	if(status != FLASH_STATUS_ISMS) return FALSE;

	// Restore read query mode
	SEND_FLASH16(ptr, FLASH_CMD_READ_ARRAY);

	return TRUE;
}



// Writes the given 16-bit value to addr, in a flash. The flash block being
// written to should have been erased previously. This function is for 16-bit
// flashes only.
//
// WARNING: 16-bit flash writes MUST be aligned on 2-byte boundaries (i.e., the
// LSB of addr must be 0).
//
// Returns TRUE if successful, FALSE if an error occurs.
BOOL WriteFlash16(uint8 *addr, uint16 value)
{
	volatile uint8 *ptr;
	uint16 status;

	ptr = addr;

	if(((uint32)addr & 1) != 0) return FALSE;

	// Clear status register
	SEND_FLASH16(ptr, FLASH_CMD_CLEAR_STATUS_REG);

	// Send program command
	SEND_FLASH16(ptr, FLASH_CMD_PROGRAM);
	SEND_FLASH16(ptr, value);

	// Wait for completion
	while(((status = READ_FLASH16(ptr, 0)) & FLASH_STATUS_ISMS)
		  != FLASH_STATUS_ISMS);
	if(status != FLASH_STATUS_ISMS) return FALSE;

	// Restore read query mode
	SEND_FLASH16(ptr, FLASH_CMD_READ_ARRAY);

	return TRUE;
}



// Writes the given data of the indicated size to a 16-bit flash. The flash
// block being written to should have been erased previously. Unlike the
// WriteFlash16() function, this function can do a write of any size and does
// not require addr to be aligned any specific way.
//
// Because of this relaxation of alignment and size requirements, the
// implementation is a bit slow.
//
// Returns TRUE if successful, FALSE if an error occurs.
BOOL WriteFlashData16(uint8 *addr, void *data, int size)
{
	int count;
	uint16 value;
	uint8 *ptr;

	count = 0;
	ptr = (uint8 *)data;

	// Determine if we must start with a half-word write
	if(((uint32)addr & 1) == 1) {
		// Do half word write

		addr--;

		// Read current value (for byte at lower address)
		value = *(volatile uint16 *)addr;

		// Adjust value with new data
		((uint8 *)&value)[1] = *ptr++;

		// Write back value
		if(!WriteFlash16(addr, value)) return FALSE;
		

		addr += 2;
		count++;
	}

	// Write intermediate values
	for(; count < size-1; count += 2) {
		// Generate next value
		((uint8 *)&value)[0] = *ptr++;
		((uint8 *)&value)[1] = *ptr++;
		
		if(!WriteFlash16(addr, value)) return FALSE;
		addr += 2;
	}

	// Determine if we must finish with half-word write
	if(count < size) {
		// Do half word write

		// Read current value (for byte at lower address)
		value = *(volatile uint16 *)addr;

		// Adjust value with new ptr
		((uint8 *)&value)[0] = *ptr;

		// Write back value
		if(!WriteFlash16(addr, value)) return FALSE;
	}

	return TRUE;
}



// Erases all blocks in the region described by addr and size. It can be used
// to erase multiple blocks or the entire flash.
//
// NOTE: On a flash, erasing has the effect of setting the memory contents to
// all ones (0xFF)
//
// NOTE: The argument addr should be on a block boundary!
//
// Returns TRUE if succesful, FALSE if an error occurs.
BOOL EraseFlash(uint8 *addr, int size, int blockSize)
{
	volatile uint8 *ptr;
	int byteCount;
	uint16 status;

	ptr = addr;

	for(byteCount = 0; byteCount < size; byteCount += blockSize) {
		// Send program command
		SEND_FLASH16(ptr, FLASH_CMD_BLOCK_ERASE_0);
		SEND_FLASH16(ptr, FLASH_CMD_CONFIRM);

		// Wait for completion
		while(((status = READ_FLASH16(ptr, 0)) & FLASH_STATUS_ISMS)
			  != FLASH_STATUS_ISMS);
		if(status != FLASH_STATUS_ISMS) return FALSE;

		// Adjust block address
		ptr += blockSize;
	}

	// Restore read query mode
	SEND_FLASH16(addr, FLASH_CMD_READ_ARRAY);	

	return TRUE;
}
