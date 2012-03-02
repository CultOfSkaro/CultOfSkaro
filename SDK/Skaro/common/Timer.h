/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    Timer.h
AUTHOR:  Wade Fife
CREATED: 11/12/04

DESCRIPTION

Header file for Timer.c. See Timer.c for more information.

CHANGE LOG

11/26/05 WSF Modified to support the Xilnx PowerPC 405, including
             64-bit timer values.


******************************************************************************/

#ifndef TIMER_H
#define TIMER_H

#include "Header.h"
#include "xparameters.h"
//#include "SystemTypes.h"



// USER CONFIGURATION /////////////////////////////////////////////////////////

// Define functions to be included for compilation.
#define  USE_CLOCK_DELTA    // Returns clock ticks (32-bit result) since last
                           // call to ClockDelta.
#define USE_CLOCK_DELAY    // Delays specified 32-bit number of cycles.
#undef  USE_CLOCK_DELTA_64 // Returns clock ticks (64-bit result) since last
                           // call to ClockDelta.
#define USE_CLOCK_DELAY_64 // Delays specified 64-bit number of cycles.
#define USE_CLOCK_TIME_64  // Reads current 64-bit timer count, if available.

// Set this #define to the real-time clock frequency (ticks per second)
#define CLOCK_TIMER_FREQ ((unsigned)(XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ))

///////////////////////////////////////////////////////////////////////////////



// DEFINES ////////////////////////////////////////////////////////////////////

#define CLOCK_TICKS_PER_SEC CLOCK_TIMER_FREQ

// Set this #define to return the 32-bit real-time clock value
#define ClockTime() ({ uint32 value; \
                       asm volatile ("mftb %0" : "=r" (value)); \
                       value; })

// Set this #define to return the 64-bit real-time clock value (if available)
#define ClockTime64() GetTimeBase64()



// DATA TYPES /////////////////////////////////////////////////////////////////

typedef union {
	uint32 lastTime;
	uint64 lastTime64;
} ClockTimer;



// PROTOTYPES /////////////////////////////////////////////////////////////////

uint32 ClockDelta(ClockTimer *clock);
void   ClockDelay(uint32 cycles);
uint64 ClockDelta64(ClockTimer *clock);
void   ClockDelay64(uint64 cycles);

uint64 GetTimeBase64(void);


#endif
