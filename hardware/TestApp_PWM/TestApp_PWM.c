/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * Xilinx EDK 11.1 EDK_L.29.1
 *
 * This file is a sample test application
 *
 * This application is intended to test and/or illustrate some
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * XPS project when you run the "Generate Libraries" menu item
 * in XPS.
 *
 * Your XPS project directory is at:
 *    C:\EDK11\Rebuild2\
 */


// Located in: ppc405_0/include/xparameters.h
#include "xparameters.h"

#include "xcache_l.h"

#include "stdio.h"

#include "xutil.h"

#include <xintc_l.h>

//#include "SystemTypes.h"

#include "ServoControl.h"
//====================================================

//more #includes to use the encoder
#include "plb_quad_encoder.h"
#include "Timer.h"
#include "HeliosIO.h"
//#include "ISR.h"

int PIDvel(Xuint32 target, Xuint32 cur){
	if (cur < (target-5))
		return 1;
	else if (cur > (target+10))
		return 3;
	else
		return 7;
}

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
	int32 str;
	int32 thr;
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
	
	XCache_EnableICache(0x00000001);
	XCache_EnableDCache(0x00000001);
	
	XExc_Init();
	XIntc_RegisterHandler( XPAR_XPS_INTC_0_BASEADDR,
							XPAR_XPS_INTC_0_XPS_GPIO_1_IP2INTC_IRPT_INTR,
							(XInterruptHandler)ISR_GPIO,
							(void *)NULL);
	XIntc_mEnableIntr(XPAR_XPS_INTC_0_BASEADDR, XPAR_XPS_GPIO_1_IP2INTC_IRPT_MASK);
	XIntc_mMasterEnable(XPAR_XPS_INTC_0_BASEADDR);
	InitGameSystem();
	//XExc_mEnableExceptions(XEXC_NON_CRITICAL);
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
					str += 8;
				break;
			case '5':	// center steering
				str = 0;
				break;
			case '6':	// turn right more
				if(str > -64)
					str -= 8;
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
				thr = 0;
				str = 0;
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
				
			case 'g'://Method added to test a short straight followed by 180degree left turn
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
				
				break;
			case 'l'://Method added to test a short straight followed by 90degree left turn
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
				break;
			case 'q':	// quit
				ServoInit(RC_STR_SERVO);
				ServoInit(RC_VEL_SERVO);
				print("-- Exiting main() --\r\n");
				XCache_DisableDCache();
				XCache_DisableICache();
				return 0;
				break;
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
	XCache_DisableDCache();
	XCache_DisableICache();
	return 0;
}
