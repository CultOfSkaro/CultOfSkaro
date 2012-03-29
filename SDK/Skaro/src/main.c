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
#include "PIT.h"

//#include "ISR.h"

volatile int goToTower = 0;
volatile int distanceFromVisionTarget = 0;
//volatile int max_velocity = 1500;

XIntc InterruptController;     /* The instance of the Interrupt Controller */

XUartLite gameboard_uart;




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
#define SET_STEERING_MODE	0xc
#define SET_VELOCITY_MODE	0xd
#define SET_25_CIRCLE2		0xe
#define SET_20_CIRCLE2		0xf
#define FIRE_KILL_SHOT		0x10

// TODO:  clean up and make it non-blocking
void WirelessRecvHandler(void *CallBackRef, unsigned int EventData)
{
	char c = 0;

	if(!XUartLite_Recv(&(wireless.uart), &c, 1)){
		return;
	}

	float float_data;
	int int_data;
	int i;
	char d;
	switch (c){
	case SET_PID_KP:
		read(0,&float_data,sizeof(float));
		navigation.pid.Kp = float_data;
		Wireless_Debug("Setting Kp to:");
		PrintFloat(navigation.pid.Kp);
		break;
	case SET_PID_KI:
		read(0,&float_data,sizeof(float));
		navigation.pid.Ki = float_data;
		Wireless_Debug("Setting Ki to:");
		PrintFloat(navigation.pid.Ki);
		break;
	case SET_PID_KD:
		read(0,&float_data,sizeof(float));
		navigation.pid.Kd = float_data;
		Wireless_Debug("Setting Kd to:");
		PrintFloat(navigation.pid.Kd);
		break;
	case SET_PID_KP_C:
		read(0,&float_data,sizeof(float));
		navigation.pid.Kp_r = float_data;
		Wireless_Debug("Setting Kp_r to:");
		PrintFloat(navigation.pid.Kp_r);
		break;
	case SET_PID_KI_C:
		read(0,&float_data,sizeof(float));
		navigation.pid.Ki_c = float_data;
		Wireless_Debug("Setting Ki_c to:");
		PrintFloat(navigation.pid.Ki_c);
		break;
	case SET_PID_KD_C:
		read(0,&float_data,sizeof(float));
		navigation.pid.Kd_c = float_data;
		Wireless_Debug("Setting Kd_c to:");
		PrintFloat(navigation.pid.Kd_c);
		break;
	case SET_DISTANCE:
		read(0,&int_data,sizeof(int));
		PID_SetDistance(&navigation.pid,int_data);
		Wireless_Debug("Set distance to ");
		PrintInt(int_data);
		Wireless_Debug("\n\r");
		break;
//	case SET_STEERING:
//		read(0,&c,1);
//		SetServo(RC_STR_SERVO, (signed char)c);
//		Wireless_Debug("Set Steering servo to ");
//		PrintInt(c);
//		break;
	case SET_MAX_VELOCITY:
		read(0,&int_data,sizeof(int));
		navigation.pid.maxVelocity = int_data;
		Wireless_Debug("Set velocity to ");
		PrintInt(navigation.pid.maxVelocity);
		Wireless_Debug("\n\r");
		break;
	case SET_VISION_DISTANCE:
		read(0,&int_data,sizeof(int));
		distanceFromVisionTarget = int_data;
		break;
	case SET_VELOCITY_MODE:
		read(0, &c, 1);
		Navigation_SetVelocityMode(&navigation,c);
		break;
	case SET_STEERING_MODE:
		read(0, &c, 1);
		Navigation_SetSteeringMode(&navigation,c);
		break;
	case FIRE_KILL_SHOT:
		Wireless_Debug("Firing Kill Shot!\r\n");
		Game_Shoot(GAME_KILL_SHOT);
		break;
	case 'e':
		goToTower = !goToTower;
		break;
	case 'c':
		Wireless_Debug("Received STOP!\r\n");
		Stop();
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

void issue_Command() {
	// We've received the entire length of the packet
	wireless.receive_in_progress = 0;
	QueuePush(&(wireless.read_queue), (void *)&(wireless.read_array[wireless.read_array_position]));
	if (++(wireless.read_array_position) >= READ_QUEUE_SIZE) {
		wireless.read_array_position = 0;
	}
	scheduler.events.flags.process_wireless_commands = 1;
}

// Note: This code is currently pretty ugly, it needs to be refined a bit
void WirelessRecvHandlerNonBlocking(void *CallBackRef, unsigned int EventData)
{
	Wireless_Command command;

	// TODO: Add length to the packet, so that we don't need to infer it from type
	if (!wireless.receive_in_progress) {
		XUartLite_Recv(&(wireless.uart), &(command.type), 1);
		wireless.receive_in_progress = 1;
		switch (command.type){
			case SET_PID_KP:
				command.length = 4;
				break;
			case SET_PID_KI:
				command.length = 4;
				break;
			case SET_PID_KD:
				command.length = 4;
				break;
			case SET_PID_KP_C:
				command.length = 4;
				break;
			case SET_PID_KI_C:
				command.length = 4;
				break;
			case SET_PID_KD_C:
				command.length = 4;
				break;
			case SET_DISTANCE:
				command.length = 4;
				break;
			case SET_STEERING:
				command.length = 1;
				break;
			case SET_MAX_VELOCITY:
				command.length = 4;
				break;
			case SET_VISION_DISTANCE:
				command.length = 4;
				break;
			case SET_VELOCITY_MODE:
				command.length = 1;
				break;
			case SET_STEERING_MODE:
				command.length = 1;
				break;
			case FIRE_KILL_SHOT:
				command.length = 0;
				break;
			case 'e':
				command.length = 0;
				break;
			case 'c':
				command.length = 0;
				break;
			default:
				command.length = 0;
				break;
			}
			wireless.read_array[wireless.read_array_position] = command;
			if (command.length > 0) {
				XUartLite_Recv(&(wireless.uart), wireless.read_array[wireless.read_array_position].data, command.length);
			} else {
				issue_Command();
			}
	} else if (EventData == wireless.read_array[wireless.read_array_position].length){
		issue_Command();
	}
}

unsigned int gpioRead = 0;

void Gpio_Changed(int gpio) {
	if (CurrentShotType) {
		CurrentShotType = 0;
		Wireless_Debug("Clearing!\r\n");
		XGpio_DiscreteClear(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, CurrentShotType);
	}
}


// This is a dummy task just to test out different events.
void hello(){
	return;
	// Test GPIO
//	Wireless_Debug("Has Flag: ");
//	PrintInt(Game_HaveFlag());
//	Wireless_Debug("\r\n");
//
//	Wireless_Debug("Game Enabled: ");
//	PrintInt(Game_Enabled());
//	Wireless_Debug("\r\n");
//
//	Wireless_Debug("Game Truck Alive: ");
//	PrintInt(Game_Truck_Alive());
//	Wireless_Debug("\r\n");
//
//	Wireless_Debug("Waiting To Shoot: ");
//	PrintInt(Game_WaitingToShoot());
//	Wireless_Debug("\r\n");
//
//	Wireless_Debug("Game State: ");
//	PrintInt(Game_GameState());
//	Wireless_Debug("\r\n");
//
//	Wireless_Debug("Team Number: ");
//	PrintInt(Game_TeamNumber());
//	Wireless_Debug("\r\n");
//	Wireless_Debug("-----------------------\r\n");
}

int numNotFound = 0;
int backupCount = 0;
int backup = 0;

// This needs to take params and avoid using globals
void Vision_ProcessFrame(){

	VisionData *visionData = *vision.snap_vision_data;

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
		if(goToTower){
			PID_SetDistance(&navigation.pid,10000);
			numNotFound++;
#ifdef DEBUG_USB_VISION
			int tmp = 50 * numNotFound;
#else
			int tmp = 10 * numNotFound;
#endif
			if(tmp > 200){
				navigation.pid.currentCentroid = 200;
			} else {
				navigation.pid.currentCentroid = tmp;
			}
		}
	} else {
		numNotFound = 0;

		int toGo = biggest->distance*2000 - distanceFromVisionTarget;

		//-----Backup Centering Code
		if ((navigation.pid.currentVelocity == 0)&&(navigation.pid.currentCentroid < -30 || navigation.pid.currentCentroid > 30)){
			backup = 1;
		}
		if (backupCount < 100 && backup == 1){
			backupCount++;
			toGo = (biggest->distance*2000) - (distanceFromVisionTarget*2);
		}
		else if (backupCount == 100){
			backupCount = 0;
			backup = 0;
		}
		if(goToTower){
			if (toGo < 6000) {
				Game_Shoot(GAME_KILL_SHOT);
			}
			PID_SetDistance(&navigation.pid,toGo);
			navigation.pid.currentCentroid = biggest->center;
		}
	}
	vision.frameRate++;
}

void process_wireless_commands() {
	Wireless_Command command;
	int int_data;
	float float_data;
	char char_data;

	CPU_MSR msr = DISABLE_INTERRUPTS();
	command = *((Wireless_Command *)QueuePop(&(wireless.read_queue)));
	RESTORE_INTERRUPTS(msr);

	switch (command.type){
	case SET_PID_KP:
		navigation.pid.Kp = *((float *)(command.data));
		Wireless_Debug("Setting Kp to:");
		PrintFloat(navigation.pid.Kp);
		break;
	case SET_PID_KI:
		navigation.pid.Ki = *((float *)(command.data));
		Wireless_Debug("Setting Ki to:");
		PrintFloat(navigation.pid.Ki);
		break;
	case SET_PID_KD:
		navigation.pid.Kd = *((float *)(command.data));
		Wireless_Debug("Setting Kd to:");
		PrintFloat(navigation.pid.Kd);
		break;
	case SET_PID_KP_C:
		navigation.pid.Kp_c = *((float *)(command.data));
		Wireless_Debug("Setting Kp_c to:");
		PrintFloat(navigation.pid.Kp_c);
		break;
	case SET_PID_KI_C:
		navigation.pid.Ki_c = *((float *)(command.data));
		Wireless_Debug("Setting Ki_c to:");
		PrintFloat(navigation.pid.Ki_c);
		break;
	case SET_PID_KD_C:
		navigation.pid.Kd_c = *((float *)(command.data));
		Wireless_Debug("Setting Kd_c to:");
		PrintFloat(navigation.pid.Kd_c);
		break;
	case SET_DISTANCE:
		int_data = *((int *)(command.data));
		PID_SetDistance(&navigation.pid,int_data);
		Wireless_Debug("Set distance to ");
		PrintInt(int_data);
		Wireless_Debug("\n\r");
		break;
	case SET_STEERING:
		char_data = command.data[0];
		SetServo(RC_STR_SERVO, (signed char)char_data);
		Wireless_Debug("Set Steering servo to ");
		PrintInt(char_data);
		break;
	case SET_MAX_VELOCITY:
		int_data = *((int *)(command.data));
		navigation.pid.maxVelocity = int_data;
		Wireless_Debug("Set velocity to ");
		PrintInt(navigation.pid.maxVelocity);
		Wireless_Debug("\n\r");
		break;
	case SET_VISION_DISTANCE:
		int_data = *((int *)(command.data));
		distanceFromVisionTarget = int_data;
		Wireless_Debug("Set Tower Distance to ");
		PrintInt(distanceFromVisionTarget);
		break;
	case SET_VELOCITY_MODE:
		char_data = command.data[0];
		Navigation_SetVelocityMode(&navigation,char_data);
		break;
	case SET_STEERING_MODE:
		char_data = command.data[0];
		Navigation_SetSteeringMode(&navigation,char_data);
		break;
	case FIRE_KILL_SHOT:
		Wireless_Debug("Firing Kill Shot!\r\n");
		Game_Shoot(GAME_KILL_SHOT);
		break;
	case 'e':
		goToTower = !goToTower;
		Wireless_Debug("GO!!");
		break;
	case 'c':
		Wireless_Debug("Received STOP!\r\n");
		Stop();
		break;
	default:
		Wireless_Debug("Unknown command received: 0x");
		PrintByte(command.type);
		Wireless_Debug("\r\n");
		break;
	}
}


///////  THIS IS THE START OF WHAT MAIN WILL ACTUALLY LOOK LIKE... EVERYTHING ABOVE MUST GO!!!!!

void calibrate_memory(){
	XCache_DisableDCache();
	XCache_DisableICache();
	MpmcCalibrationExample(XPAR_MPMC_0_DEVICE_ID);
	XCache_EnableICache(0x00000001);
	XCache_EnableDCache(0x00000001);
}

void reportFrameRate(){
	Wireless_Send(&wireless, WIRELESS_VISION_FRAMERATE, 4, &vision.frameRate);
	vision.frameRate = 0;
	Wireless_Blob_Report((*vision.snap_vision_data)->numBlobs,(*vision.snap_vision_data)->blobs);
}

void vision_loop(){
	*vision.snap_vision_data = *vision.live_vision_data;
	if(*vision.snap_vision_data){
		Vision_ProcessFrame();
	}
}

void steering_loop(){
	Navigation_SteeringLoop(&navigation);
}

void velocity_loop(){
	Navigation_VelocityLoop(&navigation);
}
void registerEvents(){
	/* Initialize Tasks */
		Events events;
		Events_Init(&events); 		// clear all events
		//events.flags.velocity_loop = 1;
		events.flags.timer1 = 1;// set the flags we want as triggers
		Scheduler_RegisterTask(&scheduler,velocity_loop,events);  // Register task with Scheduler


		Events_Init(&events); 		// clear all events
		events.flags.timer1 = 1;	// set the flags we want as triggers
		Scheduler_RegisterTask(&scheduler,steering_loop,events);  // Register task with Scheduler

		Events_Init(&events); 		// clear all events
		events.flags.hello = 1;
		events.flags.timer1 = 1;
		Scheduler_RegisterTask(&scheduler,hello,events);

		Events_Init(&events); 		// clear all events
		events.flags.vision = 1;
		Scheduler_RegisterTask(&scheduler,vision_loop,events);


		Events_Init(&events); 		// clear all events
		events.flags.process_wireless_commands = 1;
		Scheduler_RegisterTask(&scheduler,process_wireless_commands,events);

		// Report framerate on timer2
		Events_Init(&events);
		events.flags.timer2 = 1;
		Scheduler_RegisterTask(&scheduler, reportFrameRate, events);
}

void registerTimers(){
	Counter c;
	c.max = 100;
	c.event = &scheduler.events.flags.timer1;
	PIT_AddCounter(&pit,c);
	c.max = 1000;
	c.event = &scheduler.events.flags.timer2;
	PIT_AddCounter(&pit,c);
}

void my_pitHandler(){
	PIT_IncrementCounters(&pit);
	XTime_PITClearInterrupt();
}

void myBeforeHook(){
	if(*vision.live_vision_data != *vision.snap_vision_data){
		scheduler.events.flags.vision = 1;
		scheduler.events.flags.steering_loop = 1;
	}
}

int main (void) {

	calibrate_memory();

	// wait for memory to calibrate and set vision pointers to 0
	usleep(100000);
	Vision_Init(&vision);

	InitInterrupts();

	// Enable Gyro Data
	InitGameSystem();
	HeliosDisableGyro();
	usleep(10000);
	HeliosEnableGyro();
	Scheduler_Init(&scheduler);
	Scheduler_SetBeforeHook(&scheduler,myBeforeHook);
	Navigation_Init(&navigation);

	registerEvents();
	registerTimers();

	ServoInit(RC_STR_SERVO);
	ServoInit(RC_VEL_SERVO);

	PID_SetDistance(&navigation.pid,0);

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


