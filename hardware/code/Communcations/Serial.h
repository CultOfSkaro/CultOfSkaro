/**
\unit
\author Barrett Edwards

FILE:				Serial.h
AUTHOR:				Barrett Edwards
CREATED:			13 Nov 2006
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Communication Framework
COMPANION FILES:	Serial.c
DESCRIPTION:		

	This module provides the functionality to communicate over the RS-232 
	Serial port. Also bundled with the RS-232 functionality, are the 
	'Print' commands that will send packet based text to an external
	device (usually the GUI)


	Related Functions & Groups
	- Raw serial port functions
		- int   SER_getch			(Xuint32 baseaddr);
		- int   SER_DataReady		(Xuint32 baseaddr);
		- void  SER_PrintChar		(Xuint32 baseaddr, char c);
		- void  SER_PrintNibble	(Xuint32 baseaddr, uint8 nibble);
		- void  SER_PrintByte		(Xuint32 baseaddr, uint8 byte);
		- void  SER_PrintHalfWord	(Xuint32 baseaddr, uint16 hword);
		- void  SER_PrintWord		(Xuint32 baseaddr, uint32 word);
		- void  SER_IntToString	(Xuint32 baseaddr, uint32 value, int sign, char *dest);
		- void  SER_PrintInt		(Xuint32 baseaddr, int32 value);
		- void  SER_PrintUnsigned	(Xuint32 baseaddr, uint32 value);
		- void  SER_FloatToString	(Xuint32 baseaddr, float value, char *dest);
		- void  SER_PrintFloat	(Xuint32 baseaddr, float value);
		- void  SER_SendData		(Xuint32 baseaddr, uint8* data, uint32 size);
	- Text Transmission
		- void  Print(char *);
		- void  Println();
		- void  PrintHex8(uint8 c);
		- void  PrintHex16(uint32 ui);
		- void  PrintHex32(uint32 ui);
		- void  PrintChar(char c);
		- void  PrintInt(int32 i);
		- void  PrintUInt(uint32 ui);
		- void  PrintFloat(float d);
	- Text Utility Functions
		- uint8 hexToAscii(uint8 nibble);
		- int   IntToString(uint32 value, int sign, char *dest);
		- int   FloatToString(float value, char *dest);

CHANGE LOG
- 01 Aug 2007 BBE Added Doxygen style comments
*/ 

/* Conditional Inclusion ----------------------------------------------------*/
#ifndef SERIAL_H
#define SERIAL_H


/* Includes -----------------------------------------------------------------*/
#include <xparameters.h>
#include <xuartlite_l.h>

#include "Header.h"

/* Macros -------------------------------------------------------------------*/

#define SER_BASEADDR XPAR_OPB_UART_SERIAL_BASEADDR

#ifdef XPAR_OPB_UART_WIRELESS_BASEADDR
#define WIR_BASEADDR XPAR_OPB_UART_WIRELESS_BASEADDR
#else
#define WIR_BASEADDR SER_BASEADDR
#endif
       
#ifdef XPAR_OPB_UART_TTL_BASEADDR
#define TTL_BASEADDR XPAR_OPB_UART_TTL_BASEADDR
#else
#define TTL_BASEADDR WIR_BASEADDR
#endif

// The follwing are used by PrintFloat().
#define MAX_DIVISOR      1000000000.0  // Must be a power of 10
#define MIN_DIVISOR      0.000001      // Must be a power of 10
#define MAX_FLOAT_STRING 21            // Based on above, a '-', and NULL byte
#define MAX_FLOAT        (MAX_DIVISOR * 9.9)
#define MIN_FLOAT        MIN_DIVISOR



/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/






/*********************************************************************
 Functional Group: Raw serial port functions
 Definition:       This group of functions sends raw (non-packed based)
                   data over the serial port
*/ 
int   SER_getch			(Xuint32 baseaddr);
int   SER_DataReady		(Xuint32 baseaddr);

void  SER_PrintChar		(Xuint32 baseaddr, char c);
void  SER_PrintNibble	(Xuint32 baseaddr, uint8 nibble);
void  SER_PrintByte		(Xuint32 baseaddr, uint8 byte);
void  SER_PrintHalfWord	(Xuint32 baseaddr, uint16 hword);
void  SER_PrintWord		(Xuint32 baseaddr, uint32 word);
void  SER_IntToString	(Xuint32 baseaddr, uint32 value, int sign, char *dest);
void  SER_PrintInt		(Xuint32 baseaddr, int32 value);
void  SER_PrintUnsigned	(Xuint32 baseaddr, uint32 value);
void  SER_FloatToString	(Xuint32 baseaddr, float value, char *dest);
void  SER_PrintFloat	(Xuint32 baseaddr, float value);

void  SER_SendData		(Xuint32 baseaddr, uint8* data, uint32 size);




/*********************************************************************
 Functional Group: Text Transmission
 Definition:       This group should have functions relating to 
                   sendnig text over the current IO port
*/ 
void  Print(char *);
void  Println();

void  PrintHex8(uint8 c);
void  PrintHex16(uint32 ui);
void  PrintHex32(uint32 ui);

void  PrintChar(char c);
void  PrintInt(int32 i);
void  PrintUInt(uint32 ui);
void  PrintFloat(float d);



/*********************************************************************
 Functional Group: Text Utility Functions
 Definition:       This group should have functions relating to any 
   				   operation on text data.
*/ 
uint8 hexToAscii(uint8 nibble);
int   IntToString(uint32 value, int sign, char *dest);
int   FloatToString(float value, char *dest);




#endif
