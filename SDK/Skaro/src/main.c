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
#include "GameBoard.h"
#include "skaro_wireless.h"
#include "skaro_queue.h"
#include "scheduler.h"
#include "Serial.h"
#include "CrossCore.h"
#include "Navigation.h"
#include "Interrupts.h"
#include "PIT.h"
#include "AI.h"

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
#define SET_RADIUS			0x7
#define SET_DISTANCE		0x8
#define SET_VELOCITY		0x9
#define SET_MAX_VELOCITY	0xa
#define SET_CENTROID_TARGET	0xb
#define SET_MANUAL_MODE		0xc
#define SET_VELOCITY_MODE	0xd
#define SET_25_CIRCLE2		0xe
#define SET_20_CIRCLE2		0xf
#define FIRE_KILL_SHOT		0x10

//void myBeforeHook();

// TODO:  clean up and make it non-blocking
void WirelessRecvHandler(void *CallBackRef, unsigned int EventData)
{
	unsigned char c = 0;

	if(!XUartLite_Recv(&(wireless.uart), &c, 1)){
		return;
	}

	float float_data;
	int int_data;
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
		navigation.pid.Ki_r = float_data;
		Wireless_Debug("Setting Ki_r to:");
		PrintFloat(navigation.pid.Ki_r);
		break;
	case SET_PID_KD_C:
		read(0,&float_data,sizeof(float));
		navigation.pid.Kd_r = float_data;
		Wireless_Debug("Setting Kd_r to:");
		PrintFloat(navigation.pid.Kd_r);
		break;
	case SET_RADIUS:
		read(0,&int_data,sizeof(int));
		PID_SetRadius(&navigation.pid,int_data/abs(int_data) , abs(int_data));
		Wireless_Debug("Set radius to ");
		PrintInt(int_data);
		Wireless_Debug("\n\r" );
		break;
	case SET_DISTANCE:
		read(0,&int_data,sizeof(int));
		PID_SetDistance(&navigation.pid,int_data);
		Wireless_Debug("Set distance to ");
		PrintInt(int_data);
		Wireless_Debug("\n\r" );
		break;
	case SET_VELOCITY:
		read(0,&int_data,sizeof(int));
		PID_SetVelocity(&navigation.pid, int_data);
		Wireless_Debug("Set velocity to ");
		PrintInt(int_data);
		break;
	case SET_MAX_VELOCITY:
		read(0,&int_data,sizeof(int));
		ai.max_velocity = int_data;
		ai.navigation->pid.maxVelocity = int_data;
		PID_SetVelocity(&ai.navigation->pid, ai.max_velocity);
		Wireless_Debug("Set velocity to ");
		PrintInt(ai.max_velocity);
		Wireless_Debug("\n\r");
		break;
	case SET_CENTROID_TARGET:
		read(0,&int_data,sizeof(int));
		Wireless_Debug("Set target to:");
		PrintInt(int_data);
		break;
	case FIRE_KILL_SHOT:
		Wireless_Debug("Received PASS\r\n");
		GB_Shoot(GAME_PASS_SHOT);
		break;
	case 'd':
		Wireless_Debug("Starting AI.\r\n");
		AI_ChangeMode(&ai, GET_THE_FLAG);
		break;
	case SET_MANUAL_MODE:
		Wireless_Debug("Manual Mode.\r\n");
		AI_ChangeMode(&ai, NOOP_AI);
		read(0,&int_data,sizeof(int));
		switch(int_data){
		case 0:
			Wireless_Debug("Distance Mode");
			Navigation_SetVelocityMode(ai.navigation, DISTANCE_VELOCITY_MODE);
			break;
		case 1:
			Wireless_Debug("Velocity_Mode");
			Navigation_SetVelocityMode(ai.navigation, VELOCITY_MODE);
			break;
		case 2:
			Navigation_SetSteeringMode(ai.navigation, CENTROID_MODE);
			break;
		case 3:
			Navigation_SetSteeringMode(ai.navigation, RADIUS_MODE);
			break;
		}
	case 'e':
		goToTower = !goToTower;
		break;
	case 'c':
		Wireless_Debug("Received STOP!\r\n");
		Stop();
		break;
	case 'a':
		Wireless_Debug("Received Emergency STOP!\r\n");
		SetServo(RC_STR_SERVO, 0);
		SetServo(RC_VEL_SERVO, 0);
		DISABLE_INTERRUPTS();
		while(1);
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

int Vision_GetStatus(Vision * vision, VisionData * visionData, Object * obj){
	int j;
	Blob other;
	for(j = 0; j < visionData->numBlobs; j++){
		other = visionData->blobs[j];
		// if it's a team color, ignore it
		if(other.type == BLOB_TYPE_RED || other.type == BLOB_TYPE_PINK || other.type == BLOB_TYPE_CYAN || other.type == BLOB_TYPE_BLUE)
			continue;
		// check if it's inline with the current blob (right above and somewhat centered )
		if(
				(abs(other.center - obj->blob.center) < 3) &&
				((obj->blob.top - other.top) < 30) &&
				(obj->blob.top - other.top > 0)
				){
			if(other.type == BLOB_TYPE_YELLOW){
				return FLAG_OBJECT_STATUS;
			} else if (other.type == BLOB_TYPE_GREEN){
				return ENABLED_OBJECT_STATUS;
			}
		}
	}
	return DISABLED_OBJECT_STATUS;
}
// This needs to take params and avoid using globals
void Vision_ProcessFrame(){

	VisionData *visionData = *vision.snap_vision_data;

	// WORK IN PROGRESS....
	int i;
	Object object;
	// create new inactive objects
	Object c;
	c.active = 0;
	Object b;
	b.active = 0;
	Object p;
	p.active = 0;
	Object r;
	r.active = 0;

	for(i=0; i < visionData->numBlobs; i++) {
		object.blob = visionData->blobs[i];
		object.status = Vision_GetStatus(&vision, visionData, &object);
		object.active = TRUE;
		switch(object.blob.type){
		case BLOB_TYPE_CYAN:
			object.type = TOWER_OBJECT;
			c = object;
			break;
		case BLOB_TYPE_BLUE:
			object.type = TRUCK_OBJECT;
			b = object;
			break;
		case BLOB_TYPE_PINK:
			object.type = TOWER_OBJECT;
			p = object;
			break;
		case BLOB_TYPE_RED:
			object.type = TOWER_OBJECT;
			r = object;
			break;
		}
	}
	if (CURRENT_TEAM == RED_TEAM){
		vision.us.tower = p;
		vision.us.truck = r;
		vision.them.tower = c;
		vision.them.truck = b;
	} else {
		vision.them.tower = p;
		vision.them.truck = r;
		vision.us.tower = c;
		vision.us.truck = b;
	}

	if(navigation.steeringLoopMode == CENTROID_MODE){
		if(vision.current_target->active)
			navigation.pid.currentCentroid = vision.current_target->blob.center;
		scheduler.events.flags.steering_loop = 1;
	}

	vision.frameRate++;
}

void myBeforeHook(){
	if(*vision.live_vision_data != *vision.snap_vision_data){
		scheduler.events.flags.vision = 1;
		scheduler.events.flags.steering_loop = 1;
	}
}

void process_wireless_commands() {
	Wireless_Command command;
	int int_data;
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
		//navigation.pid.maxVelocity = int_data;
		ai.max_velocity = int_data;
		Wireless_Debug("Set velocity to ");
		PrintInt(ai.max_velocity);
		Wireless_Debug("\n\r");
		break;
	case SET_VISION_DISTANCE:
		int_data = *((int *)(command.data));
		distanceFromVisionTarget = int_data;
		Wireless_Debug("Set Tower Distance to ");
		PrintInt(distanceFromVisionTarget);
		break;
	case FIRE_KILL_SHOT:
		Wireless_Debug("Firing Pass Shot!\r\n");
		GB_Shoot(GAME_PASS_SHOT);
		break;
	case 'd':
		ai.state = START;
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
	Wireless_Send(&wireless, WIRELESS_VISION_FRAMERATE, 4, (char*)&vision.frameRate);
	vision.frameRate = 0;
	Wireless_Blob_Report((*vision.snap_vision_data)->numBlobs,(*vision.snap_vision_data)->blobs);
	Wireless_Debug("TtUu:");
	PrintInt(vision.them.truck.active);
	PrintInt(vision.them.tower.active);
	PrintInt(vision.us.tower.active);
	PrintInt(vision.us.truck.active);
	Wireless_Debug("\r\n");
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

void ai_loop(){
	AI_RunAI(&ai);
}

void registerEvents(){
	/* Initialize Tasks */
		Events events;
		Events_Init(&events); 		// clear all events
		//events.flags.velocity_loop = 1;
		events.flags.timer1 = 1;// set the flags we want as triggers
		Scheduler_RegisterTask(&scheduler, velocity_loop, events);  // Register task with Scheduler


		Events_Init(&events); 		// clear all events
		events.flags.timer1 = 1;	// set the flags we want as triggers
		Scheduler_RegisterTask(&scheduler, steering_loop, events);  // Register task with Scheduler

		Events_Init(&events); 		// clear all events
		events.flags.hello = 1;
		events.flags.timer1 = 1;
		Scheduler_RegisterTask(&scheduler, hello, events);

		Events_Init(&events); 		// clear all events
		events.flags.vision = 1;
		Scheduler_RegisterTask(&scheduler, vision_loop, events);


		Events_Init(&events); 		// clear all events
		events.flags.process_wireless_commands = 1;
		Scheduler_RegisterTask(&scheduler, process_wireless_commands, events);

		// Report framerate on timer2
		Events_Init(&events);
		events.flags.timer2 = 1;
		Scheduler_RegisterTask(&scheduler, reportFrameRate, events);

		Events_Init(&events);
		events.flags.timer3 = 1;
		Scheduler_RegisterTask(&scheduler, ai_loop, events);


		Events_Init(&events);
		events.flags.timer3 = 1;
		Scheduler_RegisterTask(&scheduler, myBeforeHook, events);
}

void idle(){
}
void registerTimers(){
	Counter c;
	c.max = 100;
	c.event = &scheduler.events.flags.timer1;
	PIT_AddCounter(&pit,c);
	c.max = 1000;
	c.event = &scheduler.events.flags.timer2;
	PIT_AddCounter(&pit,c);
	c.max = 10;
	c.event = &scheduler.events.flags.timer3;
	PIT_AddCounter(&pit,c);
}

void my_pitHandler(){
	PIT_IncrementCounters(&pit);
	XTime_PITClearInterrupt();
}



int main (void) {

	calibrate_memory();

	// wait for memory to calibrate and set vision pointers to 0
	usleep(100000);
	Vision_Init(&vision);

	// This needs to come before interrupts to ensure
	// that uart packets from the gameboard are in sync
	GB_Init();
	GB_DisableGyro();
	usleep(1000000);

	InitInterrupts();

	// Now that interrupts are enabled, re-enable gyro
	GB_EnableGyro();


	Scheduler_Init(&scheduler);
	Scheduler_SetBeforeHook(&scheduler,idle);
	//Scheduler_SetBeforeHook(&scheduler,myBeforeHook);
	Navigation_Init(&navigation);
	AI_Init(&ai);

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


