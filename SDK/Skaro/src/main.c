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
 * THE ADEQUACY OF THE IMP
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREELEMENTATION, INCLUDING BUT NOT LIMITED TO
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
#include "xcache_l.h"
#include "xintc.h"
#include "xil_exception.h"
#include "InterruptControl.h"
#include "xtime_l.h"
#include "sleep.h"
#include "signal.h"

#include <stdlib.h>

#include <mpmc_calibration.h>
#include <fcntl.h>

//#include "SystemTypes.h"
#include "Header.h"
#include "ServoControl.h"
#include "PID_Control.h"
#include <unistd.h>
#include "xuartlite.h"
#include "xuartlite_l.h"

//====================================================

#include "plb_quad_encoder.h"
#include "Timer.h"
#include "HeliosIO.h"
#include "skaro_wireless.h"
#include "skaro_queue.h"
#include "scheduler.h"
#include "Serial.h"
#include "CrossCore.h"
#include "Navigation.h"
#include "Interrupts.h"

//#include "ISR.h"



volatile int goToTower = 0;
volatile int distanceFromVisionTarget = 0;
//volatile int max_velocity = 1500;

XIntc InterruptController;     /* The instance of the Interrupt Controller */

Skaro_Wireless wireless;
XUartLite gameboard_uart;

Scheduler scheduler;


#define SET_PID_KP   		0x1
#define SET_PID_KI   		0x2
#define SET_PID_KD   		0x3
#define SET_PID_KP_C 		0x4
#define SET_PID_KI_C 		0x5
#define SET_PID_KD_C 		0x6
#define REQUEST_CONTROL_LOG 0x7
#define SET_DISTANCE		0x8
#define SET_STEERING		0x9
#define SET_MAX_VELOCITY	0xa
#define SET_VISION_DISTANCE	0xb
#define SET_20_CIRCLE1		0xc
#define SET_OVERLAP			0xd
#define SET_25_CIRCLE2		0xe
#define SET_20_CIRCLE2		0xf
#define SHOOT_GAME_SHOOT_KILL	0x10
// TODO:  clean up and make it non-blocking
void WirelessRecvHandler(void *CallBackRef, unsigned int EventData)
{
	char c = 0;
	if(!XUartLite_Recv(&(wireless.uart), &c, 1)){
		return;
	}

	char word[11];
	word[10] = 0;
	float temp;
	int temp_int;
	int i;
	char d;
	switch (c){
	case SET_PID_KP:
		read(0,&temp,sizeof(float));
		pid.Kp = temp;
		Wireless_Debug("Setting Kp to:");
		PrintFloat(pid.Kp);
		break;
	case SET_PID_KI:
		read(0,&temp,sizeof(float));
		pid.Ki = temp;
		Wireless_Debug("Setting Ki to:");
		PrintFloat(pid.Ki);
		break;
	case SET_PID_KD:
		read(0,&temp,sizeof(float));
		pid.Kd = temp;
		Wireless_Debug("Setting Kd to:");
		PrintFloat(pid.Kd);
		break;
	case SET_PID_KP_C:
		read(0,&temp,sizeof(float));
		pid.Kp_c = temp;
		Wireless_Debug("Setting Kp_c to:");
		PrintFloat(pid.Kp_c);
		break;
	case SET_PID_KI_C:
		read(0,&temp,sizeof(float));
		pid.Ki_c = temp;
		Wireless_Debug("Setting Ki_c to:");
		PrintFloat(pid.Ki_c);
		break;
	case SET_PID_KD_C:
		read(0,&temp,sizeof(float));
		pid.Kd_c = temp;
		Wireless_Debug("Setting Kd_c to:");
		PrintFloat(pid.Kd_c);
		break;
	case SET_DISTANCE:
		read(0,word,10);
		i = atoi(word);
		setDistance(i);
		Wireless_Debug("Set distance to ");
		PrintInt(i);
		Wireless_Debug("\n\r");
		break;
	case SET_STEERING:
		read(0,&c,1);
		SetServo(RC_STR_SERVO, (signed char)c);
		Wireless_Debug("Set Steering servo to ");
		PrintInt(c);
		break;
	case SET_MAX_VELOCITY:
		read(0,word,10);
		i = atoi(word);
		pid.maxVelocity = i;
		Wireless_Debug("Set velocity to ");
		PrintInt(pid.maxVelocity);
		Wireless_Debug("\n\r");
		break;
	case SET_VISION_DISTANCE:
		read(0,word,10);
		distanceFromVisionTarget = atoi(word);
		break;
//	case SHOOT_GAME_SHOOT_KILL:
//		Game_Shoot(GAME_KILL_SHOT);
//		break;
	case 'e':
		goToTower = !goToTower;
		break;
	default:
		Wireless_Debug("Unknown command received: 0x");
		PrintByte(c);
		Wireless_Debug("\r\n");
		break;
	}
	while(!XUartLite_IsReceiveEmpty(wireless.uart.RegBaseAddress)){
		XUartLite_RecvByte(wireless.uart.RegBaseAddress);
	}
}

// Interrupt handler for gameboard UART
void GameboardRecvHandler(void *CallBackRef, unsigned int EventData)
{

	XUartLite_Recv(&gameboard_uart, raw_gyro_data.packet, 5);

	if (EventData == 5) {
		raw_gyro_data.velocity = (signed char)raw_gyro_data.packet[1];
		raw_gyro_data.velocity <<= 8;
		raw_gyro_data.velocity |= (uint32) raw_gyro_data.packet[2];

		raw_gyro_data.angular_velocity = (signed char)raw_gyro_data.packet[3];
		raw_gyro_data.angular_velocity <<= 8;
		raw_gyro_data.angular_velocity |= (uint32) raw_gyro_data.packet[4];
	}
}


// For the pit handler, we increment a counter.  Every time the counter reaches a certain threshold,
// We set the timer flag in the scheduler's events struct.  We reset the counter.
int counter = 0;
int counter2 = 0;
int counter3 = 0;
int update = 0;

void my_pitHandler(){
	counter++;
	counter3++;
	if(counter > 100){
		scheduler.events.flags.velocity_loop = 1;
		counter = 0;
	}

	if(*live_vision_data != *snap_vision_data){
		scheduler.events.flags.vision = 1;
		scheduler.events.flags.steering_loop = 1;
		counter2++;
	}

	if(counter3 > 1000){
		Wireless_Send(&wireless, 3, 4, &counter2);
		counter3 = 0;
		counter2 = 0;
	}
	XTime_PITClearInterrupt();
}

// This is a dummy task just to test out different events.
void hello(){

}

int numNotFound = 0;
int backupCount = 0;
int backup = 0;
void vision(){

	#define IMAGE_WIDTH 640
	#define OBJECT_WIDTH       0.08255
	#define FIELD_OF_VISION    ((float)(25.5 * 3.1415 / 180))
	#define DISTANCE_CONSTANT  (IMAGE_WIDTH * OBJECT_WIDTH / FIELD_OF_VISION)

	float distance;
	int center;
	float angle;

	*snap_vision_data = *live_vision_data;
	if(!*snap_vision_data){
		return;
	}

	VisionData *visionData = *snap_vision_data;


	//Wireless_Debug("Blobs Detected: ");
	int i;
	Blob * biggest = 0;
	for(i=0; i < visionData->numBlobs; i++) {
		if(visionData->blobs[i].type == BLOB_TYPE_RED)
			continue;
		if(visionData->blobs[i].width < 20 || visionData->blobs[i].width > 300)
			continue;
		if(!biggest){
			biggest = &visionData->blobs[i];
		} else {
			if(visionData->blobs[i].width > biggest->width) {
				biggest = &visionData->blobs[i];
			}
		}
	}

	if(biggest == 0) {
		Wireless_Debug("NO BLOBS FOUND!");
		if(goToTower){
			setDistance(10000);
			numNotFound++;
#ifdef DEBUG_USB_VISION
			int tmp = 50 * numNotFound;
#else
			int tmp = 10 * numNotFound;
#endif
			if(tmp > 350){
				pid.currentCentroid = 350;
			} else {
				pid.currentCentroid = tmp;
			}
		}
		return;
	} else {
		numNotFound = 0;
		distance = DISTANCE_CONSTANT / biggest->width;
		center = biggest->left + (biggest->width / 2) - (IMAGE_WIDTH / 2);
		angle = FIELD_OF_VISION * center / IMAGE_WIDTH;

		Wireless_Debug("Blobs founds:");
		PrintInt(visionData->numBlobs);

		Wireless_Debug("Biggest Blob:");
	//	Wireless_Debug("Left:");
	//	PrintInt(biggest->left);
	//	Wireless_Debug("Top:");
	//	PrintInt(biggest->top);
		Wireless_Debug("Width:");
		PrintInt(biggest->width);
	//	Wireless_Debug("Height:");
	//	PrintInt(biggest->height);
		Wireless_Debug("Distance:");
		PrintFloat(distance);
		Wireless_Debug("Angle:");
		PrintFloat(angle);
		Wireless_Debug("Centroid:");
		PrintFloat(pid.currentCentroid);
		int toGo = distance*2000 - distanceFromVisionTarget;

		//-----Backup Centering Code
		if ((pid.currentVelocity == 0)&&(pid.currentCentroid < -30 || pid.currentCentroid > 30)){
			backup = 1;
		}
		if (backupCount < 100 && backup == 1){
			backupCount++;
			toGo = (distance*2000) - (distanceFromVisionTarget*2);
		}
		else if (backupCount == 100){
			backupCount = 0;
			backup = 0;
		}
		//-----

		Wireless_Debug("To Go:");
		PrintInt(toGo);
		Wireless_Debug("Velocity:");
		PrintFloat(pid.currentVelocity);
		if(goToTower){
			setDistance(toGo);
			pid.currentCentroid = center;
		}
	}
}


void calibrate_memory(){
	print("Calibrating Memory...");
	XCache_DisableDCache();
	XCache_DisableICache();
	MpmcCalibrationExample(XPAR_MPMC_0_DEVICE_ID);
	XCache_EnableICache(0x00000001);
	XCache_EnableDCache(0x00000001);
	print("Done\r\n");
}

void registerEvents(){
	/* Initialize Tasks */
		Events velocity_events;  // in order to register the control loop,
		// we need an events struct
		Events_Init(&velocity_events); 		// clear all events
		velocity_events.flags.velocity_loop = 1;	// set the flags we want as triggers
		Scheduler_RegisterTask(&scheduler,velocity_loop,velocity_events);  // Register task with Scheduler

		Events steering_events;  // in order to register the control loop,
		// we need an events struct
		Events_Init(&steering_events); 		// clear all events
		steering_events.flags.steering_loop = 1;	// set the flags we want as triggers
		Scheduler_RegisterTask(&scheduler,steering_loop,steering_events);  // Register task with Scheduler

		// Another example
		Events hello_events;
		Events_Init(&hello_events);
		hello_events.flags.hello = 1;
		Scheduler_RegisterTask(&scheduler,hello,hello_events);

		// Another example
		Events vision_events;
		Events_Init(&vision_events);
		vision_events.flags.vision = 1;
		Scheduler_RegisterTask(&scheduler,vision,vision_events);
}

int main (void) {

	calibrate_memory();

	// wait for memory to calibrate and set vision pointers to 0
	usleep(100000);
	*snap_vision_data = 0;
	*live_vision_data = 0;

	InitInterrupts();

	Scheduler_Init(&scheduler);

	registerEvents();

	InitGameSystem();
	HeliosEnableGyro();

	ServoInit(RC_STR_SERVO);
	ServoInit(RC_VEL_SERVO);

	initPID();

	setDistance(0);

	SetServo(RC_STR_SERVO, 1);

	while(1){
		/*  Run Scheduler repeatedly */
		Scheduler_Run(&scheduler);

	}

	Wireless_Debug("-- Exiting main() --\r\n");
	XCache_DisableDCache();
	XCache_DisableICache();
	return 0;
}


