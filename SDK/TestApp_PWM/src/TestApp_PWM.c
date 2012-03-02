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

#include <stdlib.h>

#include "xintc.h"
#include "xil_exception.h"
#include "InterruptControl.h"


#include  <fcntl.h>

//#include "SystemTypes.h"
#include "Header.h"
#include "ServoControl.h"
#include "PID_Control.h"
#include <unistd.h>
#include "xuartlite.h"
#include "xuartlite_l.h"
//====================================================

//more #includes to use the encoder
#include "plb_quad_encoder.h"
#include "Timer.h"
#include "HeliosIO.h"

//#include "ISR.h"

#define SET_PID_KP   		0x1
#define SET_PID_KI   		0x2
#define SET_PID_KD   		0x3
#define SET_PID_KP_D 		0x4
#define SET_PID_KI_D 		0x5
#define SET_PID_KD_D 		0x6
#define REQUEST_CONTROL_LOG 0x7
#define SET_DISTANCE		0x8
#define SET_STEERING		0x9
#define SET_MAX_VELOCITY	0xa
#define SET_25_CIRCLE1		0xb
#define SET_20_CIRCLE1		0xc
#define SET_OVERLAP			0xd
#define SET_25_CIRCLE2		0xe
#define SET_20_CIRCLE2		0xf
extern PID pid;

#define	STATE_DEFAULT					0x0
#define STATE_FIGURE_EIGHT_RIGHT_START	0x1
#define STATE_FIGURE_EIGHT_RIGHT_END	0x2
#define STATE_FIGURE_EIGHT_LEFT_START	0x3
#define	STATE_FIGURE_EIGHT_LEFT_END		0x4
#define STATE_FIGURE_EIGHT_RIGHT_HALF 	0x5
#define STATE_FIGURE_EIGHT_RIGHT_END_HALF	0x6
#define STATE_FIGURE_EIGHT_LEFT_START_2	0x7
#define	STATE_FIGURE_EIGHT_LEFT_END_2	0x8

#define CIRCLE_2_5_M_TICKS		16550
#define CIRCLE_2_0_M_TICKS		14100



int logValues[1000];
int logIndex = 0;

int print_flag = 1;
int state = STATE_DEFAULT;
int max_velocity = 4000;

int circle25_ticks1 = CIRCLE_2_5_M_TICKS;
int circle20_ticks1 = CIRCLE_2_0_M_TICKS;
int circle25_ticks2 = CIRCLE_2_5_M_TICKS;
int circle20_ticks2 = CIRCLE_2_0_M_TICKS;
int loop_count = 0;

int overlap = 0;

XUartLite wireless;				/* The instance of the Wireless UartLite Device */
XIntc InterruptController;     /* The instance of the Interrupt Controller */


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

int logData(int actual, int expected){
	int i;
	if(logIndex > 999){
		return 0;
	}
	logValues[logIndex++] = actual;
	logValues[logIndex++] = expected;
}

void WirelessSendHandler(void *CallBackRef, unsigned int EventData)
{
	return;
}

void WirelessRecvHandler(void *CallBackRef, unsigned int EventData)
{
	char c;
	if(!XUartLite_IsReceiveEmpty(wireless.RegBaseAddress)){
		c = XUartLite_RecvByte(wireless.RegBaseAddress);
	} else {
		c = 0;
	}
	char word[11];
	float temp;
	int i;
	switch (c){
		case SET_PID_KP:
			read(0,&c,1);
			pid.Kp = c/25500.0;
			xil_printf("We are setting Kp to ");
			PrintFloat(pid.Kp);
			print("\n\r");
			break;
		case SET_PID_KI:
			read(0,&c,1);
			pid.Ki = c/25500.0;
			xil_printf("We are setting Ki to ");
			PrintFloat(pid.Ki);
			print("\n\r");
			break;
		case SET_PID_KD:
			read(0,&c,1);
			pid.Kd = c/25500.0;
			xil_printf("We are setting Kd to ");
			PrintFloat(pid.Kd);
			print("\n\r");
			break;
		case SET_PID_KP_D:
			read(0,&c,1);
			xil_printf("received %d", c);
			pid.Kp_d = c/25500.0;
			xil_printf("We are setting Kp_d to ");
			PrintFloat(pid.Kp_d);
			print("\n\r");
			break;
		case SET_PID_KI_D:
			read(0,&c,1);
			pid.Ki_d = c/25500.0;
			xil_printf("We are setting Ki_d to ");
			PrintFloat(pid.Ki_d);
			print("\n\r");
			break;
		case SET_PID_KD_D:
			read(0,&c,1);
			pid.Kd_d = c/25500.0;
			xil_printf("We are setting Kd_d to ");
			PrintFloat(pid.Kd_d);
			print("\n\r");
			break;
		case '7':
		case REQUEST_CONTROL_LOG:
			for(i = 0; i < 1000; i++){
				xil_printf("%d\n",logValues[i]);
			}
			break;
		case SET_DISTANCE:
			read(0,&word,11);
			xil_printf("received %s", word);
			i = atoi(word);
			setDistance(i);
			xil_printf("set distance to %d", i);
			break;
		case SET_STEERING:
			read(0,&c,1);
			SetServo(RC_STR_SERVO, (signed char)c);
			xil_printf("Set Steering servo to %d\n\r", (signed char)c);
			break;
		case SET_MAX_VELOCITY:
			read(0,&word,11);
			xil_printf("received %s", word);
			i = atoi(word);
			setVelocity(i);
			//max_velocity = i;
			xil_printf("set max velocity to %d", i);
			break;
		case SET_25_CIRCLE1:
			read(0,&word,11);
			xil_printf("received %s", word);
			circle25_ticks1 = atoi(word);
			xil_printf("set distance to %d", circle25_ticks1);
			break;
		case SET_20_CIRCLE1:
			read(0,&word,11);
			xil_printf("received %s", word);
			circle20_ticks1 = atoi(word);
			xil_printf("set distance to %d", circle20_ticks1);
			break;
		case SET_25_CIRCLE2:
			read(0,&word,11);
			xil_printf("received %s", word);
			circle25_ticks2 = atoi(word);
			xil_printf("set distance to %d", circle25_ticks2);
			break;
		case SET_20_CIRCLE2:
			read(0,&word,11);
			xil_printf("received %s", word);
			circle20_ticks2 = atoi(word);
			xil_printf("set distance to %d", circle20_ticks2);
			break;
		case SET_OVERLAP:
			read(0,&word,11);
			xil_printf("received %s", word);
			overlap = atoi(word);
			xil_printf("set distance to %d", overlap);
			break;
		case 'a':
			loop_count = 2;
			state = STATE_FIGURE_EIGHT_LEFT_START;
			break;
		case 'b':
			loop_count = 2;
			state = STATE_FIGURE_EIGHT_RIGHT_START;
			break;
		case 'd':
			print_flag = 0;
			break;
		case 'e':
			print_flag = 1;
			break;
		case 'c':
			setDistance(0);
			break;
		case '1':
			setDistance(10000);
			setVelocity(1000);
			break;
		case '2':
			setDistance(10000);
			setVelocity(2000);
			break;
		case '3':
			setDistance(10000);
			setVelocity(3000);
			break;
		case '4':
			setDistance(10000);
			setVelocity(4000);
			break;
		case '5':
			setDistance(10000);
			setVelocity(5000);
			break;
	}
	while(!XUartLite_IsReceiveEmpty(wireless.RegBaseAddress)){
		XUartLite_RecvByte(wireless.RegBaseAddress);
	}
}

void InitInterrupts() {
		XUartLite_Initialize(&wireless, XPAR_WIRELESS_UART_DEVICE_ID);

		/*
		 * Initialize the interrupt controller driver so that it is ready to
		 * use.
		 */
		XIntc_Initialize(&InterruptController, XPAR_INTC_0_DEVICE_ID);


		/*
		 * Connect a device driver handler that will be called when an interrupt
		 * for the device occurs, the device driver handler performs the
		 * specific interrupt processing for the device.
		 */
		XIntc_Connect(&InterruptController, XPAR_XPS_INTC_0_WIRELESS_UART_INTERRUPT_INTR,
				(XInterruptHandler)XUartLite_InterruptHandler,
				(void *)&wireless);

		/*
		 * Start the interrupt controller such that interrupts are enabled for
		 * all devices that cause interrupts, specific real mode so that
		 * the UartLite can cause interrupts through the interrupt controller.
		 */
		XIntc_Start(&InterruptController, XIN_REAL_MODE);

		/*
		 * Enable the interrupt for the UartLite device.
		 */
		XIntc_Enable(&InterruptController, XPAR_XPS_INTC_0_WIRELESS_UART_INTERRUPT_INTR);

		/*
		 * Initialize the exception table.
		 */
		Xil_ExceptionInit();

		/*
		 * Register the interrupt controller handler with the exception table.
		 */
		Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XIntc_InterruptHandler,
				&InterruptController);

		/*
		 * Enable exceptions.
		 */
		Xil_ExceptionEnable();
		XUartLite_SetSendHandler(&wireless, WirelessSendHandler, &wireless);
		XUartLite_SetRecvHandler(&wireless, WirelessRecvHandler, &wireless);
		XUartLite_EnableInterrupt(&wireless);
}

int main (void) {
	int32 str;
	int32 thr;
	char c;
//	Xuint32 velcount = 0;
	Xuint32 eTicks = 0;
	Xuint32 maxTicks = 10;
//	Xuint32 sTicks1 = 0;
//	Xuint32 sTicks2 = 0;
//	Xuint32 vel = 0;
	Xuint32 cvel = 0;
//	Xuint32 pid = 0;
	int ticks;
	int ticks_prev;
	
	InitInterrupts();

	str = 0;
	thr = 0;
	
	XCache_EnableICache(0x00000001);
	XCache_EnableDCache(0x00000001);
	
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
	SetServo(RC_STR_SERVO, 2);
	initPID();
	//setVelocity(3000);
	setDistance(0);
	SetServo(RC_STR_SERVO, 1);
	int distance;
	while(1){
		usleep(10000);
		//updateDistanceSetVelocity(max_velocity);
		updateVelocityOutput();
		//updateDistanceSetVelocity();
		switch(state) {
			case STATE_DEFAULT:
				break;
			case STATE_FIGURE_EIGHT_LEFT_START:
				if(!loop_count){
					state = STATE_DEFAULT;
					break;
				}
				SetServo(RC_STR_SERVO, LEFT*26);
				if(loop_count == 2){
					setDistance(2*(circle25_ticks1 + circle20_ticks1) - 3*overlap);
				}
				loop_count--;
				state = STATE_FIGURE_EIGHT_LEFT_END;
				break;
			case STATE_FIGURE_EIGHT_LEFT_END:
					if (pid.distanceError < (circle20_ticks1 + circle25_ticks1 + circle20_ticks1 )) {
						state = STATE_FIGURE_EIGHT_LEFT_START;
						SetServo(RC_STR_SERVO, RIGHT*31);
					}
					break;
			case STATE_FIGURE_EIGHT_RIGHT_START:
				if(!loop_count){
					state = STATE_DEFAULT;
					break;
				}
				if(loop_count == 2){
					setDistance(circle25_ticks1 + circle25_ticks2 + circle20_ticks1 + circle20_ticks2);
					state = STATE_FIGURE_EIGHT_RIGHT_END;
					SetServo(RC_STR_SERVO, RIGHT*26);
					loop_count--;
				} else if (pid.distanceError < (circle20_ticks2 + circle25_ticks2 )) {
					state = STATE_FIGURE_EIGHT_RIGHT_HALF;
					SetServo(RC_STR_SERVO, RIGHT*26);
					loop_count--;
				}
				break;
			case STATE_FIGURE_EIGHT_RIGHT_HALF:
				if(loop_count == 1){
					distance = circle25_ticks2 + circle20_ticks1 +circle20_ticks2 + circle25_ticks1/2;
				} else {
					distance = circle25_ticks2/2 + circle20_ticks2;
				}
				if (pid.distanceError < distance) {
					state = STATE_FIGURE_EIGHT_RIGHT_END;
					SetServo(RC_STR_SERVO, RIGHT*29);
				}
				break;
			case STATE_FIGURE_EIGHT_RIGHT_END:
				if(loop_count == 1){
					distance = circle25_ticks2 + circle20_ticks1 +circle20_ticks2;
				} else {
					distance = circle20_ticks2;
				}
				if (pid.distanceError < distance) {
					state = STATE_FIGURE_EIGHT_RIGHT_END_HALF;
					SetServo(RC_STR_SERVO, LEFT*31);
				}
				break;
			case STATE_FIGURE_EIGHT_RIGHT_END_HALF:
				if(loop_count == 1){
					distance = circle25_ticks2 + circle20_ticks2 +circle20_ticks1/2;
				} else {
					distance = circle20_ticks2/2;
				}
				if (pid.distanceError < distance) {
					state = STATE_FIGURE_EIGHT_RIGHT_START;
					SetServo(RC_STR_SERVO, LEFT*31);
				}
				break;
		}
	}
//	while(1){
//		read(0,&c,1);
//		xil_printf("read c");
//		switch (c){
//			case 'd':
//				print("hello\n\r");
//				break;
//			case 'h':	// show help
//				showHelp();
//				break;
//			case '4':	// turn left more
//				if(str < 64)
//					str += 8;
//				xil_printf("Str: %d/n/r", str);
//				break;
//			case '5':	// center steering
//				str = 0;
//				xil_printf("Str: %d/n/r", str);
//				break;
//			case '6':	// turn right more
//				if(str > -64)
//					str -= 8;
//				xil_printf("Str: %d/n/r", str);
//				break;
//			case '8':	// increase velocity
//				xil_printf("8");
//				if(thr < 64)
//					thr += 2;
//				break;
//			case '2':	// decrease velocity
//				xil_printf("2");
//				if(thr > -64)
//					thr -= 2;
//				break;
//			case '0':	// all stop
//				thr = 0;
//				str = 0;
//				cvel = 0;
//				break;
//
//			case 'z':	// pass shot
//				Game_Shoot(GAME_PASS_SHOT);
//				break;
//			case 'x':	// kill shot
//				Game_Shoot(GAME_KILL_SHOT);
//				break;
//			case 's':	// print status
//				xil_printf("Truck Alive? : %d\r\n", Game_Enabled());
//				xil_printf("Have Flag? : %d\r\n", Game_HaveFlag());
//				xil_printf("Team number: %d\r\n", Game_TeamNumber());
//				xil_printf("Game State: %d\r\n", Game_GameState());
//				xil_printf("Game State: %d\r\n", Game_GameState());
//				break;
//			case 'g'://Method added to test a short straight followed by 180degree left turn
//				str = 0;
//				SetServo(RC_STR_SERVO, str);
//
//				str = 64;
//				SetServo(RC_STR_SERVO, str);
//				clearEncoder();
//				maxTicks = 3850;
//				eTicks = getTicks();
//				while (eTicks < maxTicks){
//					eTicks = getTicks();
//				}
//
//				break;
//			case 'l'://Method added to test a short straight followed by 90degree left turn
//				str = 0;
//				SetServo(RC_STR_SERVO, str);
//				clearEncoder();
//				//maxTicks = 2000;
//				//thr = 14;
//				SetServo(RC_VEL_SERVO, thr);
//				eTicks = getTicks();
//				while (eTicks < maxTicks){
//					eTicks = getTicks();
//				}
//				str = 64;
//				SetServo(RC_STR_SERVO, str);
//				clearEncoder();
//				maxTicks = 1875;
//				eTicks = getTicks();
//				while (eTicks < maxTicks){
//					eTicks = getTicks();
//				}
//				str = 0;
//				thr = 0;
//				//SetServo(RC_VEL_SERVO, thr);
//				break;
//			case 'q':	// quit
//				ServoInit(RC_STR_SERVO);
//				ServoInit(RC_VEL_SERVO);
//				print("-- Exiting main() --\r\n");
//				XCache_DisableDCache();
//				XCache_DisableICache();
//				return 0;
//				break;
//			case 'a':
//				xil_printf("a");
//				initPID();
//				xil_printf("set");
//				while(1){
//					usleep(1000);
//					updateVelocityOutput();
//				}
//				xil_printf("update");
//				break;
//			case 'b':
//				print("b");
//				initPID();
//				while(1){
//					updateDistanceOutput();
//					usleep(1000);
//				}
//				break;
//			case 'r':	// quit
//				print("-- 1 --\r\n");
//				setSteeringRadius(1, 300);
//				usleep(1000000);
//				print("-- 2 --\r\n");
//				setSteeringRadius(1, 100);
//				usleep(1000000);
//				print("-- 3 --\r\n");
//				setSteeringRadius(-1, 100);
//				usleep(1000000);
//				print("-- 4 --\r\n");
//				setSteeringRadius(-1, 300);
//				usleep(1000000);
//				break;
//
//			case 't':
//
//				print("Testing float vs int speed.\r\n");
//
//				uint32 start = ClockTime();
//				int i;
//				for (i = 0; i < 100000000; i++) {
//					float f = 427894.0f / 423.4f;
//				}
//				uint32 end = ClockTime();
//				xil_printf("Float divide time: %d\r\n", end - start);
//				start = ClockTime();
//				for (i = 0; i < 100000000; i++) {
//					int d = 427894 / 423;
//				}
//				end = ClockTime();
//				xil_printf("Int divide time: %d\r\n", end - start);
//
//				 start = ClockTime();
//				for (i = 0; i < 100000000; i++) {
//					float f = 427894.0f * 423.4f;
//				}
//				 end = ClockTime();
//				xil_printf("Float multiply time: %d\r\n", end - start);
//
//				start = ClockTime();
//				for (i = 0; i < 100000000; i++) {
//					int d = 427894 * 423;
//				}
//				end = ClockTime();
//				xil_printf("Int multiply time: %d\r\n", end - start);
//
//				start = ClockTime();
//				for (i = 0; i < 100000000; i++) {
//					float f = 427894.0f + 423.4f;
//				}
//				end = ClockTime();
//				xil_printf("Float add time: %d\r\n", end - start);
//
//				start = ClockTime();
//				for (i = 0; i < 100000000; i++) {
//					int d = 427894 + 423;
//				}
//				end = ClockTime();
//				xil_printf("Int add time: %d\r\n", end - start);
//
//				start = ClockTime();
//				for (i = 0; i < 100000000; i++) {
//					float f = 427894.0f - 423.4f;
//				}
//				end = ClockTime();
//				xil_printf("Float subtract time: %d\r\n", end - start);
//
//				start = ClockTime();
//				for (i = 0; i < 100000000; i++) {
//					int d = 427894 - 423;
//				}
//				end = ClockTime();
//				xil_printf("Int subtract time: %d\r\n", end - start);
//
//				break;
//
//
//			case 'c':
//				SetServo(RC_VEL_SERVO, 10);
//				usleep(250000);
//				SetServo(RC_VEL_SERVO, 20);
//				usleep(250000);
//				SetServo(RC_VEL_SERVO, 30);
//				usleep(250000);
//				SetServo(RC_VEL_SERVO, 40);
//				usleep(250000);
//				SetServo(RC_VEL_SERVO, 60);
//				ticks_prev = getTicks();
//				usleep(10000000);
//				ticks = getTicks();
//				xil_printf("Ticks per second: %d", (ticks - ticks_prev)/10);
//				SetServo(RC_VEL_SERVO, 0);
//				break;
//
//			case 'e':
//				ticks = getTicks();
//				xil_printf("Encoder ticks: %d\r\n", ticks);
//				break;
//
//			default:
//				xil_printf("default");
//				break;
//		}
//		if(!Game_Enabled()){
//			str = 0;
//			thr = 0;
//		}
//
//		SetServo(RC_STR_SERVO, str);
//		SetServo(RC_VEL_SERVO, thr);
//}

	print("-- Exiting main() --\r\n");
	XCache_DisableDCache();
	XCache_DisableICache();
	return 0;
}
