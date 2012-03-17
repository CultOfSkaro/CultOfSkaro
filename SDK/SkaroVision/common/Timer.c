/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    Timer.c
AUTHOR:  Wade Fife
CREATED: 10/13/04

DESCRIPTION

This module contains code for the PowerPC hardware timer, called the time-base
register.

This module also contains a simple clock manager API that uses the real-time
clock. The macro ClockTime() simply returns the 32-bit real-time clock
count. You can also use the ClockDelta() function to time events. To do this,
create a timer by declaring a ClockTimer object. You then start the timer by
calling ClockDelta(), passing it a pointer to your timer. Each subsequent call
to ClockDelta() with that timer as a parameter will then return the number of
clock ticks since the previous call to ClockDelta() for that timer.

The macro ClockTime() (in the header file) must be configured to read the
hardware timer and CLOCK_TIMER_FREQ should be defined to be the frequency in Hz
of the real-time clock timer.

CHANGE LOG

11/26/05  WSF  Modified file to support PowerPC, including 64-bit timer values.

******************************************************************************/

#include "Timer.h"



// REAL-TIME CLOCK API ////////////////////////////////////////////////////////


#ifdef USE_CLOCK_DELTA

// Return the number of clock ticks since the last call to
// ClockDelta().
uint32 ClockDelta(ClockTimer *clock)
{
    uint32 time, delta;

    time = ClockTime();

    delta = time - clock->lastTime;
    clock->lastTime = time;

    return delta;
}

#endif



#ifdef USE_CLOCK_DELTA_64

// Return the number of clock ticks since the last call to
// ClockDelta().
uint64 ClockDelta64(ClockTimer *clock)
{
    uint64 time, delta;

    time = ClockTime64();

    delta = time - clock->lastTime64;
    clock->lastTime64 = time;

    return delta;
}

#endif



#ifdef USE_CLOCK_DELAY

// This will block for the specified number of clock cycels. It is a
// busy wait that polls the clock until the time has elapsed. Use a
// non-busy wait (non-polling) instead when possible.
void ClockDelay(uint32 cycles)
{
	uint32 time;

	time = ClockTime();
	while(ClockTime() - time < cycles);
}

#endif



#ifdef USE_CLOCK_DELAY_64

// This will block for the specified number of clock cycels. It is a
// busy wait that polls the clock until the time has elapsed. Use a
// non-busy wait (non-polling) instead when possible.
void ClockDelay64(uint64 cycles)
{
	uint64 time;

	time = ClockTime64();
	while(ClockTime64() - time < cycles);
}

#endif



#ifdef USE_CLOCK_TIME_64

// Gets the 64-bit time base register value by reading the uppter (TBU) and
// lower (TBL) values and combining them into a 64-bit. It also checks for
// wraparound of the time-base register and corrects for it if it occurs.
uint64 GetTimeBase64(void)
{
	register uint32 lower, upper, firstUpper;

	do {
		asm volatile ("mftbu %0" : "=r" (firstUpper));
		asm volatile ("mftb %0" : "=r" (lower));
		asm volatile ("mftbu %0" : "=r" (upper));
	} while(upper != firstUpper);

	return ((uint64)upper << 32) | lower;
}

#endif
