/** 
\file
\author Barrett Edwards

FILE:				LoopScheduler.h
AUTHOR:				Barrett Edwards
CREATED:			11 Jan 2006
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	LoopScheduler.h
DESCRIPTION:		See LoopScheduler.h

	Related Functions & Groups
	- void LoopScheduler();
	- void LoopSchedulerTimerTick();
*/

/* Includes -----------------------------------------------------------------*/
#include "Header.h"
#include "State.h"
#include "HeliosIO.h"
#include "LoopScheduler.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
#include "FrameTable.h"
extern FrameTableEntry*	g_capture_fte;
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/


/** Tick function to reduce the remaining time on scheduled events
	Durring each timer interrupt (every 1 ms) the LoopSchedulerTimerTick()
	function is called. In this function, bookkeeping State register values 
	are decrimented	by 1. Each of these values represents the number of 
	milliseconds until the requested operation is performed. Notice that 
	the requested operations are #NOT# performed in the 
	LoopSchedulerTimerTick() function. That function is called from an ISR
	and should not just go about wildly doing any thing it wants.
*/
void LoopSchedulerTimerTick()
{
	M_int[I_LSC_HEARTBEAT]--;
	M_int[I_LSC_STATE_DATA]--;
	M_int[I_LSC_READ_BUTTON]--;	
}




/** Handle any scheduled events whose time has come
	When the request timers reach or fall below 0 they will be executed in 
	the LoopScheduler() function. This function is called every time through
	the main while(1) loop. That way when a request is serviced, it does not 
	happen in an interrupt.

	The basics of hos this works is, this function tests to see if an event
	schedule timer variable has elapsed (equal to or less than 0). If it has
	elapsed, then the requested action is performed and the event schedule 
	time is reset to a predefined value or register value.
*/
int stallTimes = 0;
void LoopScheduler()
{
	// The heartbeat timer controls how often the green LED blinks
	if(rdint(I_LSC_HEARTBEAT) <= 0)
	{
		wrint(I_LSC_HEARTBEAT, rdint(I_LSD_HEARTBEAT)); 	
		HeliosSetLED1(!HeliosReadLED1());
	}
	


	// the state data timer controls how often the state register data is 
	// sent to the GUI. 
	if(rdint(I_LSC_STATE_DATA) <= 0)
	{
		wrint(I_LSC_STATE_DATA, rdint(I_LSD_STATE_DATA)); 	
		wrchar(C_REQ_STATE_DATA,TRUE);	
	}
	


	// The read button timer controls how often we read the button.
	// This is done to "de-bounce" the button
	
	if(rdint(I_LSC_READ_BUTTON) <= 0)
	{
		wrint(I_LSC_READ_BUTTON, 100); // Hard code to the button to be read every 100ms

		if(HeliosReadBTN())
		{
			xil_printf("BUTTON!");
			// Do something in response to button push
			Game_Shoot(GAME_KILL_SHOT);
			wrint(I_LSC_READ_BUTTON, 750); // if the button was pushed then we want to wait longer before reading it again to avoid double pushing
		}
	}
	
	int stat = CameraStatus(XPAR_PLB_VISION_0_BASEADDR);
	if(stat == 0xD1) //capture done but FIFO not empty
	{
		stallTimes++;
		if(stallTimes > 1000)
		{
		  //xil_printf("Reboot");
			ResetCameraCore();
			FT_StartCapture(g_capture_fte->next);
			//StartFrameCapture(XPAR_PLB_VISION_0_BASEADDR);
			stallTimes = 0;
		}
	}

	
	//These should also be calculated every so many ms
	// but i don't need them right now so i am not implementing them
	//calculateSpeed();
	//TruckControlInterruptRoutine();
}




