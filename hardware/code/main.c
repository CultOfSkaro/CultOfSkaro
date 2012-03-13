/**
\file
\author Barrett Edwards

FILE:				main.c
AUTHOR:				Barrett Edwards
CREATED:			25 Sep 2006
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	Header.c	
DESCRIPTION:		

CHANGE LOG
*/ 


/* Includes -----------------------------------------------------------------*/
#ifdef SIMULATOR
#include <cv.h>
#include <highgui.h>
#endif

#include <xcache_l.h>
#include <xtime_l.h>
#include <xgpio_l.h>
#include <xexception_l.h>
#include <xintc_l.h>

#include <plb_vision.h>
#include <USB_IO.h>

#include "Header.h"
#include "Timer.h"
#include "init.h"
#include "FrameTable.h"
#include "State.h"
#include "mpmc_mem_test_example.h"
#include "ServoControl.h"
#include "plb_quad_encoder.h"
#include "HeliosIO.h"
#include "RobotRacersVisionCode.h"
#include "FrameReaderHSV.h"
/* Macros ------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Global Memory ------------------------------------------------------------*/
	int32 str;
	int32 thr;

/** The entry point for the Helios Power Software
	This function is responsible for initializing the Helios Computing platform
	and excuting the while(1) loop that creates a Round-Robin with Interrupts
	style of embedded computing. 

	Initialization of the Helios Computing Platform consists of three main 
	tasks. The first is to initialize and configure the FPGA / PowerPC 
	computing hardware so that they can actually function. And the second is to
	initialize the software components of the system so that the code will run.
	And the thrid is to configure any external devices (i.e. camera).
*/
void showHelp(){
	xil_printf("Use the number pad to control the car via Terminal\r\n");
	xil_printf("8 Increment forward throttle\r\n");
	xil_printf("4 Steer Left\r\n");
	xil_printf("5 Center Steering\r\n");
	xil_printf("6 Steer Right\r\n");
	xil_printf("2 Increment reverse throttle\r\n");
	xil_printf("x Fire kill shot\r\n");
	xil_printf("z Fire pass shot\r\n");
	xil_printf("s Print truck status\r\n");
	xil_printf("h Print this help message\r\n");
	//xil_printf("q Center servos and exit\r\n\r\n");
}



int main (void) {
	char c;
	Xuint32 velcount = 0;
	Xuint32 eTicks = 0;
	Xuint32 maxTicks = 10;
	Xuint32 sTicks1 = 0;
	Xuint32 sTicks2 = 0;
	Xuint32 vel = 0;
	Xuint32 cvel = 0;
	Xuint32 pid = 0;
	
	str = 0;
	thr = 0;
	
	Init();
	xil_printf("-- Starting PWM Test Mode --\r\n");
	xil_printf("NOTICE: Make sure car is placed on stand before proceeding!!\r\n");
	showHelp();
	//Initialze servos to center position
	ServoInit(RC_STR_SERVO);
	ServoInit(RC_VEL_SERVO);
	xil_printf("Steering Setting: %d\r\n",GetServo(RC_STR_SERVO));
	xil_printf("Throttle Setting: %d\r\n\r\n",GetServo(RC_VEL_SERVO));

	while(1){
	
		
	
		read(0,&c,1);
		switch (c){
			case 'h':	// show help
				showHelp();
				break;
			case '4':	// turn left more
				if(str < 64)
					str += 16;
				break;
			case '5':	// center steering
				str = 0;
				break;
			case '6':	// turn right more
				if(str > -64)
					str -= 16;
				break;
			case '8':	// increase velocity
				if(thr < 64)
					thr += 2;
				break;
			case '2':	// decrease velocity
				if(thr > -64)
					thr -= 2;
				break;
			case '0':	// all stop
				SetServo(RC_VEL_SERVO, -thr);
				ClockDelay(100000000);
				thr = 0;
				//str = 0;
				cvel = 0;
				break;
				
			case 'z':	// pass shot
				Game_Shoot(GAME_PASS_SHOT);
				break;
			case 'x':	// kill shot
				Game_Shoot(GAME_KILL_SHOT);
				break;
			case 's':	// print status
				xil_printf("Truck Alive? : %d\r\n", Game_Enabled());
				xil_printf("Have Flag? : %d\r\n", Game_HaveFlag());
				xil_printf("Team number: %d\r\n", Game_TeamNumber());
				xil_printf("Game State: %d\r\n", Game_GameState());
				break;
				
			/*case 'g'://Method added to test a short straight followed by 180degree left turn
				str = 0;
				SetServo(RC_STR_SERVO, str);
				
				str = 64;
				SetServo(RC_STR_SERVO, str);
				clearEncoder();
				maxTicks = 3850;
				eTicks = getTicks();
				while (eTicks < maxTicks){
					eTicks = getTicks();
				}
				
				break;*/
			/*case 'l'://Method added to test a short straight followed by 90degree left turn
				str = 0;
				SetServo(RC_STR_SERVO, str);
				clearEncoder();
				//maxTicks = 2000;
				//thr = 14;
				SetServo(RC_VEL_SERVO, thr);
				eTicks = getTicks();
				while (eTicks < maxTicks){
					eTicks = getTicks();
				}
				str = 64;
				SetServo(RC_STR_SERVO, str);
				clearEncoder();
				maxTicks = 1875;
				eTicks = getTicks();
				while (eTicks < maxTicks){
					eTicks = getTicks();
				}
				str = 0;
				thr = 0;
				//SetServo(RC_VEL_SERVO, thr);
				break;*/
			/*case 'q':	// quit
				ServoInit(RC_STR_SERVO);
				ServoInit(RC_VEL_SERVO);
				print("-- Exiting main() --\r\n");
				XCache_DisableDCache();
				XCache_DisableICache();
				return 0;
				break;*/
			default:
				
				break;
		}
		if(!Game_Enabled()){
			str = 0;
			thr = 0;
		}
			SetServo(RC_STR_SERVO, str);
			SetServo(RC_VEL_SERVO, thr);

	}

	print("-- Exiting main() --\r\n");
	return 0;
}
