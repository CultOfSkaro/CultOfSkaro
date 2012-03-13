/** 
\unit
\author Barrett Edwards

FILE:				LoopScheduler.h
AUTHOR:				Barrett Edwards
CREATED:			11 Jan 2006
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	LoopScheduler.c	
DESCRIPTION:		

	The purpose of the LoopScheduler is to provide a basic infastructure
	to schedule or perform specific tasks at regular time intervals. The 
	Helios System is currently configured to produce a timer interrupt 
	every 1 millisecond. Therefore, the LoopScheduler can only schedule 
	events at that grain of resolution. 

	Durring each timer interrupt (every 1 ms) the LoopSchedulerTimerTick()
	function is called. In that function, bookkeeping State register values 
	are decrimented	by 1. Each of these values represents the number of 
	milliseconds until the requested operation is performed. Notice that 
	the requested operations are #NOT# performed in the 
	LoopSchedulerTimerTick() function. That function is called from an ISR
	and should not just go about wildly doing any thing it wants.

	When the request timers reach or fall below 0 they will be executed in 
	the LoopScheduler() function. This function is called every time through
	the main while(1) loop. That way when a request is serviced, it does not 
	happen in an interrupt.

	Related Functions & Groups
	- void LoopScheduler();
	- void LoopSchedulerTimerTick();

CHANGE LOG

*/ 

/* Conditional Inclusion ----------------------------------------------------*/
#ifndef LOOPSCHEDULER_H
#define LOOPSCHEDULER_H

/* Includes -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/

void LoopScheduler();
void LoopSchedulerTimerTick();

#endif
