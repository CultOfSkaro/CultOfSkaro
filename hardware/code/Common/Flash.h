/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    Flash.h
AUTHOR:  Wade S. Fife
CREATED: 11/26/05

DESCRIPTION

Header file for Flash.c.

CHANGE LOG

11/26/05 WSF Created original file.

******************************************************************************/

#ifndef FLASH_H
#define FLASH_H



// FLASH COMMANDS /////////////////////////////////////////////////////////////

#define FLASH_CMD_READ_QUERY         0x98  // Entery flash query mode
#define FLASH_CMD_READ_ARRAY         0xFF  // Enter flash read array mode
#define FLASH_CMD_BLOCK_ERASE_0      0x20  // Block erase, first cycle
#define FLASH_CMD_CONFIRM            0xD0  // Block erase, second cycle
#define FLASH_CMD_PROGRAM            0x40  // Program byte/word, first cycle
#define FLASH_CMD_CLEAR_STATUS_REG   0x50  // Clear stutus register



// FLASH STATUS BITS //////////////////////////////////////////////////////////

#define FLASH_STATUS_ISMS            0x80  // Write state machine status



// MACROS /////////////////////////////////////////////////////////////////////

#define SEND_FLASH16(ADDR, CMD) ((*(volatile uint16 *)ADDR) = CMD)
#define READ_FLASH16(ADDR, OFF) (*(((volatile uint16 *)ADDR) + OFF))



// DATA TYPES /////////////////////////////////////////////////////////////////

typedef struct {
	int size;
	int blockSize;
	uint32 blockMask;
	uint8 *addr;
} Flash;



// FUNCTION PROTOTYPES ////////////////////////////////////////////////////////

BOOL QueryDevice16(uint8 *addr, Flash *pFlash);
BOOL EraseFlashBlock(uint8 *addr);
BOOL WriteFlash16(uint8 *addr, uint16 value);
BOOL WriteFlashData16(uint8 *addr, void *data, int size);
BOOL EraseFlash(uint8 *addr, int size, int blockSize);


#endif
