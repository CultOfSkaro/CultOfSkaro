/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    Serial.h
AUTHOR:  Wade Fife
CREATED: 10/13/04

DESCRIPTION

Header file for Serial.c. See Serial.c for more information.

CHANGE LOG

11/07/05 WSF Added SendByte() and RecvByte() macros. Added SER_BASEADDR
             definition.
11/10/05 WSF Added PrintHalfWord() prototype and #define.
12/23/05 WSF Added ByteReady() macro.
01/31/06 WSF Changed to serial address to STDIN_BASEADDRESS to work 
             independently of UART instance name.
05/17/06 WSF Changed PrintFloat() and FloatToString() to use float instead of
             double type. Changed MAX/MIN_DIVISOR.

******************************************************************************/

#ifndef SERIAL_H
#define SERIAL_H

#include <xuartlite_l.h>

#include "SystemTypes.h"
#include "xparameters.h"



// USER CONFIGURATION /////////////////////////////////////////////////////////

// Define base address of UART Light core to use
#define SER_BASEADDR   STDIN_BASEADDRESS    // Always defined in xparameters.h

// Define which functions you want to include in compilation. Please
// note that there may be dependencies between functions.
#define USE_GETCH             // Like conio.h getch()
#define USE_KBHIT             // Like conio.h kbhit()
//
#define USE_PRINT_CHAR        // Print single character
#define USE_PRINT_NIBBLE      // Print 4-bit number as hex digit
#define USE_PRINT_BYTE        // Print 8-bit hex number using PrintNibble
#define USE_PRINT_HALF_WORD   // Print 16-bit hex number using PrintByte
#define USE_PRINT_WORD        // Print 32-bit hex number using PrintHalfWord
//
#define USE_INT_TO_STRING     // Convert signed or unsigned int to string
#define USE_PRINT_INT         // Print integer using print and IntToString
#define USE_PRINT_UNSIGNED    // Print unsigned using print and IntToString
//
#define USE_FLOAT_TO_STRING   // Convert double to string
#define USE_PRINT_FLOAT       // Print float using print and FloatToString
//

///////////////////////////////////////////////////////////////////////////////



// DEFINITIONS ////////////////////////////////////////////////////////////////

// The follwing are used by PrintFloat().
#define MAX_DIVISOR      1000000000.0  // Must be a power of 10
#define MIN_DIVISOR      0.000001      // Must be a power of 10
#define MAX_FLOAT_STRING 21            // Based on above, a '-', and NULL byte
#define MAX_FLOAT        (MAX_DIVISOR * 9.9)
#define MIN_FLOAT        MIN_DIVISOR

#define BS_KEY  8
#define TAB_KEY 9
#define ENT_KEY 10
#define RET_KEY 10
#define ESC_KEY 27
#define SPC_KEY 32



// FUNCTION PROTOTYPES ////////////////////////////////////////////////////////

int   getch(void);
int   kbhit(void);
void  PrintChar(char c);
void  PrintNibble(uint8 nibble);
void  PrintByte(uint8 byte);
void  PrintHalfWord(uint16 hword);
void  PrintWord(uint32 word);
void  IntToString(uint32 value, int sign, char *dest);
void  PrintInt(int32 value);
void  PrintUnsigned(uint32 value);
void  FloatToString(float value, char *dest);
void  PrintFloat(float value);

// Use #define for synonyms
#define SendByte(C)  PrintChar(C)
#define RecvByte(C)  getch()
#define ByteReady()  kbhit()


#endif
