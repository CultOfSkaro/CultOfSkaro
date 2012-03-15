/*
 * PID_Control.c
 *
 *  Created on: Jan 31, 2012
 *      Author: Andrew Norgrant
 */


// Located in: ppc405_0/include/xparameters.h
#include "xparameters.h"
#include "xcache_l.h"
#include <stdio.h>
#include <stdlib.h>
#include "xutil.h"
#include <xintc_l.h>
#include "ServoControl.h"
#include "PID_Control.h"
#include "InterruptControl.h"
#include "Header.h"
#include "skaro_wireless.h"
//====================================================
//more #includes to use the encoder
#include "plb_quad_encoder.h"
#include "Timer.h"
#include "HeliosIO.h"
#include "Serial.h"
#include "scheduler.h"

extern Scheduler scheduler;

PID pid;

int control_log_counter;
int i=0;

void initPID(){
			//General params
			pid.outputPID = 0;
			pid.outputPID_unsat = 0;
			pid.encoderValue = 0;
			pid.lastEncoderValue = getTicks();
			pid.Tau = 0.05f;

			//Velocity params
	        pid.Kp = 0.02f;
	        pid.Kd = 0.0001f;
	        pid.Ki = 0.005f;
	        pid.integrator = 0.0f;
	        pid.differentiator = 0.0f;
	        pid.desiredVelocityPID = 0;
	        pid.lastError = 0;
	        pid.lastDesiredVelocity = 0.0;
	        pid.lastCurrentVelocity = 0.0f;
	        pid.lastClockTicks = 0;
	        pid.error = 0.0;
		    //Distance params
	        pid.Kp_d = 0.0045f;
	        pid.Kd_d = 0.001f;
	        pid.Ki_d = 0.0000f;
	        pid.integrator_d = 0.0f;
	        pid.differentiator_d = 0.0f;
	        pid.desiredDistancePID = 0;
			pid.lastDesiredDistance = 0;
			pid.error_d = 0.0;
	        pid.lastError_d = 0;
	        pid.distanceError = 0;
	        //Angle params
			pid.desiredAnglePID = 0;


	}

void updateVelocityOutput()
{
	float desiredVelocity = pid.desiredVelocityPID;
	float P, I, D, currentVelocity;
	uint32 deltaClocks;
	CPU_MSR msr;

	//------Read Encoder and clock
	msr = DISABLE_INTERRUPTS();
	pid.encoderValue = getTicks();
	uint32 nowClocks = ClockTime();
	RESTORE_INTERRUPTS(msr);

	//------Time since last function call in seconds
	uint32 maxClocks = 0xffffffff;
	if ((nowClocks < pid.lastClockTicks))
		deltaClocks = (maxClocks-pid.lastClockTicks)+nowClocks;
	else
		deltaClocks = nowClocks - pid.lastClockTicks;

	float refreshRate = ((float)deltaClocks)/XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ ;//time passed since last function call: sec
	//uint32 refreshRate = (deltaClocks)/(XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ/1000) ;

	pid.lastClockTicks = nowClocks;


	//------Distance since last function call in ticks
	int encoderDifference = pid.encoderValue - pid.lastEncoderValue;

	//------Calculate Velocity
	currentVelocity = ((encoderDifference) / (refreshRate));

	//------Calculate Error
	pid.error = desiredVelocity - (currentVelocity);

	//------Send Data to gui graphing function
	if(++control_log_counter > 0){
		//Wireless_ControlLog(currentVelocity, desiredVelocity);
		control_log_counter = 0;
	}

	//------Update Derivative
	pid.differentiator = (2*pid.Tau-refreshRate)/(2*pid.Tau+refreshRate)*pid.differentiator + 2/(2*pid.Tau+refreshRate)*(currentVelocity - pid.lastCurrentVelocity);
	//pid.differentiator = (2*pid.Tau-refreshRate)/(2*pid.Tau+refreshRate)*pid.differentiator + 2/(2*pid.Tau+refreshRate)*(pid.error - pid.lastError);

	//------Update integrator - AntiWindup(only use the integrator if we are close, but not too close)
	//if ((pid.error < 1000 && pid.error > -1000) && (pid.error > 100 || pid.error < -100)){
			pid.integrator = pid.integrator + (refreshRate/2)*(pid.error + pid.lastError);
	//} else {
	//	pid.integrator = 0;
	//}

	//------Output Calculation
	P = pid.Kp * pid.error;
	I = pid.Ki * pid.integrator;
	D = pid.Kd * pid.differentiator;

	pid.outputPID_unsat = ((int)P) + ((int)I) - ((int)D);
	pid.outputPID = sat(pid.outputPID_unsat, 60);

	if(++i > 5){
		/*
		Wireless_Debug("P:");
		PrintFloat(P);
		Wireless_Debug("I:");
		PrintFloat(I);
		Wireless_Debug("D:");
		PrintFloat(D);
		//Wireless_Debug("I word:");
		//PrintWord(*((uint32 *)&I));
		//Wireless_Debug("D word:");
		//PrintWord(*((uint32 *)&D));
		Wireless_Debug("outputPID_unsat");
		PrintFloat(pid.outputPID_unsat);
		Wireless_Debug("outputPID");
		PrintFloat(pid.outputPID);
		Wireless_Debug("CurrentVelocity");
		PrintFloat(currentVelocity);
		Wireless_Debug("LastCurrentVelocity");
		PrintFloat(pid.lastCurrentVelocity);
		Wireless_Debug("------------------");
		*/
		i = 0;
		}
	int deltaTime = refreshRate;
	Wireless_ControlLog_Ext(currentVelocity, pid.lastCurrentVelocity, pid.outputPID, pid.outputPID_unsat, deltaTime);

	pid.integrator = pid.integrator + (refreshRate/pid.Ki)*(pid.outputPID - pid.outputPID_unsat);

	//------Save states and send PWM to motors
	pid.lastEncoderValue = pid.encoderValue;
	pid.lastCurrentVelocity = currentVelocity;
	pid.lastDesiredVelocity = desiredVelocity;
	SetServo(RC_VEL_SERVO, (int)pid.outputPID);
}

void updateDistanceOutput()
{
	int32 desiredDistance = pid.desiredDistancePID;
	int32 P, I, D;
	uint32 deltaClocks;
	CPU_MSR msr;

	msr = DISABLE_INTERRUPTS();
	pid.encoderValue = getTicks();
	uint32 nowClocks = ClockTime();
	RESTORE_INTERRUPTS(msr);
	//logData(desiredDistance, pid.encoderValue);

	//------Time since last function call in seconds
	uint32 maxClocks = 0xffffffff;
	if ((nowClocks < pid.lastClockTicks))
		deltaClocks = (maxClocks-pid.lastClockTicks)+nowClocks;
	else
		deltaClocks = nowClocks - pid.lastClockTicks;
	float deltaTime = ((float)deltaClocks)/XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ ;//time passed since last function call: sec
	pid.error_d = desiredDistance - pid.encoderValue;

	//------Update integrator - AntiWindup(only use the integrator if we are close, but not too close)
	if ((pid.error_d < 1000 && pid.error_d > -1000) && (pid.error_d > 100 || pid.error_d < -100)){
		pid.integrator_d = pid.integrator_d + (deltaTime/2)*(pid.error_d + pid.lastError_d);
	}
	else
		pid.integrator_d = 0;

	//------only use the integrator if we are close, but not too close
//	if (pid.error_d < 500 && pid.error_d > -500 &&
//			(pid.error_d > 100 || pid.error_d < -100))
//		pid.integrator_d += pid.error_d * deltaTime;
//	else
//		pid.integrator_d = 0;

	//------Update Derivative
	pid.differentiator_d = (2*pid.Tau-deltaTime)/(2*pid.Tau+deltaTime)*pid.differentiator_d + 2/(2*pid.Tau+deltaTime)*(pid.error_d-pid.lastError_d);

	P = pid.Kp_d * pid.error_d;
	I = pid.Ki_d * pid.integrator_d;
	D = pid.Kd_d * pid.differentiator_d;


	pid.outputPID_unsat = P + I - D;
	pid.outputPID = sat(pid.outputPID_unsat, 60);

	//------if we are really close, don't do anything!
	//if (pid.error_d < 100 && pid.error_d > -100) {
	//	pid.outputPID = 0;
	//}

	//------Integrator Anti-windup
//	if (pid.Ki_d != 0.0f) {
//		pid.integrator_d = pid.integrator_d + deltaTime/pid.Ki_d * (pid.outputPID - pid.outputPID_unsat);
//	}

	pid.lastError_d = pid.error_d;
	pid.lastDesiredDistance = desiredDistance;

	SetServo(RC_VEL_SERVO, pid.outputPID);
}

void setDistance(int32 distance){
	pid.desiredDistancePID = distance + getTicks();
}

void setVelocity(int32 velocity){

	pid.desiredVelocityPID = velocity;
}

void updateDistanceSetVelocity(int velocity){
	int ticks = getTicks();
	pid.distanceError = pid.desiredDistancePID - ticks;
	int distanceError = pid.distanceError;

//	int vel =  pid.desiredVelocityPID;
//
//	if(distanceError < -100)
//		vel = -200;
//	else if(distanceError < vel/3)
//		vel = vel * 2/3;
//
//	pid.desiredVelocityPID = velocity;
//	updateVelocityOutput();

	if (velocity == 1 ||(velocity > 0 && velocity <= 1000))
		velocity = 1000;
	else if (velocity == 2 ||(velocity > 1000 && velocity <= 2000))
		velocity = 2000;
	else if (velocity == 3 ||(velocity > 2000 && velocity <= 3000))
		velocity = 3000;
	else if (velocity == 4 ||(velocity > 3000 && velocity <= 4000))
		velocity = 4000;
	else if (velocity == -1 ||(velocity < 0 && velocity >= -1000))
		velocity = -1000;
	else if (velocity == -2 ||(velocity < 1000 && velocity >= -2000))
		velocity = -2000;
	else if (velocity == -3 ||(velocity < 2000 && velocity >= -3000))
		velocity = -3000;
	else if (velocity == -4 ||(velocity < 3000 && velocity >= -4000))
		velocity = -4000;
	else if (velocity == 1000000)
		velocity = 5000;
	else
		velocity = 0;

	if (distanceError < 1500 && distanceError >= 1000)
		velocity = velocity/2;
	else if (distanceError < 1000 && distanceError >= 700)
		velocity = velocity/3;
	else if (distanceError < 700 && distanceError >= 500)
		velocity = velocity/4;
	else if (distanceError < 500 && distanceError >= 200)
		velocity = velocity/5;
				else if (distanceError < 200)
					velocity = 0;
//	else if (distanceError < 200 && distanceError >= 100)
//		velocity = 0;
//	else if (distanceError < 100 && distanceError >= 2)
//		velocity = 700;
//	else if (distanceError < 2 && distanceError >= -2){
//		velocity = 0;
//	}
//	else if (distanceError > -2 && distanceError <= -500)
//		velocity = -700;
//	else if (distanceError > -500 && distanceError <= -2)
//		velocity = -velocity/4;
//	else if (distanceError > -700 && distanceError <= -500)
//		velocity = -velocity/3;
//	else if (distanceError > -1000 && distanceError <= -700)
//		velocity = -velocity/2;
//	else if (distanceError > -1500 && distanceError <= -1000)
//		velocity = -velocity;

	setVelocity(velocity);
	updateVelocityOutput();
}

void setSteeringRadius(int direction, uint32 radius_cm) {
	int str = 0;
	switch (radius_cm){
			case 100:
				str = 64;
				break;
			case 125:
				str = 56;
				break;
			case 150:
				str = 48;
				break;
			case 175:
				str = 44;
				break;
			case 200:
				str = 40;
				break;
			case 225:
				str = 36;
				break;
			case 250:
				str = 32;
				break;
			case 275:
				str = 28;
				break;
			case 300:
				str = 24;
				break;
			default:
				str = 0;
				break;
	}
	SetServo(RC_STR_SERVO, (direction*str));
}

int sat(int in, int limit) {
	if (in >= limit){
		return limit;
	}
	else if (in <= -limit){
		return -limit;
	}
	return in;
}
