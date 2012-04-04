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
#include "Navigation.h"
#include "Math.h"

int control_log_counter;
int i=0;

void PID_Init(PID * pid){
			//General params
			pid->outputPID = 0;
			pid->outputPID_unsat = 0;
			pid->outputPID_c = 0;
			pid->outputPID_unsat_c = 0;
			pid->outputPID_r = 0;
			pid->outputPID_unsat_r = 0;
			pid->encoderValue = 0;
			pid->lastEncoderValue = getTicks();
			pid->Tau = 0.05f;
			//Velocity params
	        pid->Kp = 0.016f;//0.02f;
	        pid->Kd = 0.000565f;//0.00003f;
	        pid->Ki = 0.015f;//0.025f;
	        pid->integrator = 0.0f;
	        pid->differentiator = 0.0f;
	        pid->desiredVelocityPID = 0;
	        pid->lastError = 0;
	        pid->currentVelocity = 0;
	        pid->currentVelocityBack = 0;
	        pid->lastDesiredVelocity = 0;
	        pid->lastCurrentVelocity = 0;
	        pid->maxVelocity = 0;
	        pid->lastClockTicks = 0;
	        pid->error = 0.0;
		    //Distance params
	        pid->Kp_d = 0.0045f;
	        pid->Kd_d = 0.001f;
	        pid->Ki_d = 0.0000f;
	        pid->integrator_d = 0.0f;
	        pid->differentiator_d = 0.0f;
	        pid->desiredDistancePID = 0;
	    	pid->backStartPoint = 0;
			pid->lastDesiredDistance = 0;
			pid->error_d = 0.0;
	        pid->lastError_d = 0;
	        pid->distanceError = 0;
	        //Heading Angle params
			pid->desiredAnglePID = 0;
			//Centroid params
			pid->desiredCentroidPID = 0;
			pid->Kp_c = 0.11f;
			pid->Kd_c = 0.0001f;
			pid->Ki_c = 0.0f;
			pid->integrator_c = 0.0f;
			pid->differentiator_c = 0.0f;
			pid->lastError_c = 0;
			pid->lastCurrentCentroid = 0;
			pid->currentCentroid = 0;
			pid->error_c = 0.0;
			pid->lastClockTicks_c = 0;
			//Curvature params
			pid->desiredRadiusPID = 0;
			pid->Kp_r = 80000.0f;
			pid->Kd_r = 0.0005f;
			pid->Ki_r = 0.000f;//100000.0f;
			pid->radiusEqualibrium = 0;
			pid->integrator_r = 0.0f;
			pid->differentiator_r = 0.0f;
			pid->lastError_r = 0.0f;
			pid->lastCurrentRadius = 0;
			pid->lastIntegrator_r = 0.0f;
			pid->currentRadius = 0;
			pid->error_r = 0.0f;
			pid->lastClockTicks_r = 0;

	}

void PID_UpdateVelocity(PID * pid)
{
	int desiredVelocity = pid->desiredVelocityPID;
	float P, I, D;
	float refreshRate;
	CPU_MSR msr;

	//------Read Encoder and clock
	msr = DISABLE_INTERRUPTS();
	pid->encoderValue = getTicks();
	uint32 nowClocks = ClockTime();
	Gyro_Calculation(pid->gyro);
	RESTORE_INTERRUPTS(msr);

	//------Time since last function call in seconds
	refreshRate = refresh_rate(nowClocks, pid->lastClockTicks);
	pid->lastClockTicks = nowClocks;

	//------Distance since last function call in ticks
	int encoderDifference = pid->encoderValue - pid->lastEncoderValue;

	//------Calculate Velocity
	pid->currentVelocity = (int)((encoderDifference) / (refreshRate));
	//pid->currentVelocityBack = (int)((encoderDifference) / (refreshRate));

	//------Convert Back Velocity to front
	//pid->currentVelocity = Gyro_VelocityBackToFront(pid->gyro,pid->currentVelocityBack);

	//------Calculate Error
	pid->error = desiredVelocity - (pid->currentVelocity);

	//------Update Derivative
	//pid->differentiator = ((((2*pid->Tau)-refreshRate)/((2*pid->Tau)+refreshRate))*pid->differentiator) + ((2/((2*pid->Tau)+refreshRate))*(pid->currentVelocity - pid->lastCurrentVelocity));
	pid->differentiator = ((((2*pid->Tau)-refreshRate)/((2*pid->Tau)+refreshRate))*pid->differentiator) + ((2/((2*pid->Tau)+refreshRate))*(pid->error - pid->lastError));


	//------Update integrator - AntiWindup(only use the integrator if we are close, but not too close)
	pid->integrator = pid->integrator + ((refreshRate/2)*(pid->error + pid->lastError));


	//------Output Calculation
	P = pid->Kp * pid->error;
	I = pid->Ki * pid->integrator;
	D = pid->Kd * pid->differentiator;

	pid->outputPID_unsat = ((int)P) + ((int)I) - ((int)D);
	pid->outputPID = sat(pid->outputPID_unsat, 40);
	//pid->outputPID = downShift(pid->error, -600, 7);

	pid->integrator = pid->integrator + (refreshRate/pid->Ki)*(pid->outputPID - pid->outputPID_unsat);

	//Wireless_ControlLog_Ext(pid->currentVelocity, desiredVelocity, pid->error, pid->outputPID_unsat, P);
	//------Save states and send PWM to motors
	pid->lastEncoderValue = pid->encoderValue;
	pid->lastCurrentVelocity = pid->currentVelocity;
	pid->lastDesiredVelocity = desiredVelocity;
	SetServo(RC_VEL_SERVO, (int)pid->outputPID);
}

void PID_UpdateCentroid(PID * pid)
{
	int P, I, D;
	float refreshRate;

	//------Read Encoder and clock
	uint32 nowClocks = ClockTime();

	//------Time since last function call in seconds
	refreshRate = refresh_rate(nowClocks, pid->lastClockTicks_c);
	pid->lastClockTicks_c = nowClocks;

	//------Calculate Error
	pid->error_c = pid->desiredCentroidPID - pid->currentCentroid;

	//------Update Derivative
	//pid->differentiator_c = ((((2*pid->Tau)-refreshRate)/((2*pid->Tau)+refreshRate))*pid->differentiator_c) + ((2/((2*pid->Tau)+refreshRate))*(pid->currentCentroid - pid->lastCurrentCentroid));
	pid->differentiator_c = ((((2*pid->Tau)-refreshRate)/((2*pid->Tau)+refreshRate))*pid->differentiator_c) + ((2/((2*pid->Tau)+refreshRate))*(pid->error_c - pid->lastError_c));

	//------Update integrator - AntiWindup(only use the integrator if we are close, but not too close)
	pid->integrator_c = pid->integrator_c + (refreshRate/2)*(pid->error_c + pid->lastError_c);


	//------Output Calculation
	P = pid->Kp_c * pid->error_c;
	I = pid->Ki_c * pid->integrator_c;
	D = pid->Kd_c * pid->differentiator_c;

	pid->outputPID_unsat_c = (P) + (I) - (D);
	pid->outputPID_c = sat(pid->outputPID_unsat_c, 40);

	pid->integrator_c = pid->integrator_c + (refreshRate/pid->Ki_c)*(pid->outputPID_c - pid->outputPID_unsat_c);

	//------Save states and send PWM to motors
	pid->lastCurrentCentroid = pid->currentCentroid;
	pid->lastError_c = pid->error_c;

	//------Condition output on current velocity
	if (pid->currentVelocity < 0)
		pid->outputPID_c = -pid->outputPID_c;
	if (pid->currentVelocity > -20 && pid->currentVelocity < 20)
		pid->outputPID_c = 0;
	if (pid->outputPID_c == 0)
		pid->outputPID_c = -2;
	SetServo(RC_STR_SERVO, pid->outputPID_c);
}

void PID_UpdateRadius(PID * pid)
{
	int P, I, D;
	double refreshRate;
	CPU_MSR msr;

	//------Read Encoder and clock
	msr = DISABLE_INTERRUPTS();
	float desiredRadius = pid->desiredRadiusPID;//----//get info from camera here
	uint32 nowClocks = ClockTime();
	Gyro_Calculation(pid->gyro);
	RESTORE_INTERRUPTS(msr);

	//------Time since last function call in seconds
	refreshRate = refresh_rate(nowClocks, pid->lastClockTicks_r);
	pid->lastClockTicks_r = nowClocks;

	//------Current Radius at front of car
	pid->currentRadius = pid->gyro->frontRadius;
	if (pid->currentRadius > 6000)
		pid->currentRadius = 6000;
	else if (pid->currentRadius < -6000)
		pid->currentRadius = -6000;

	//------Calculate Error
	pid->error_r = ((float)(pid->currentRadius - desiredRadius))/(pid->currentRadius * desiredRadius);

	//------Update Derivative
	pid->differentiator_r = ((((2*pid->Tau)-refreshRate)/((2*pid->Tau)+refreshRate))*pid->differentiator_r) + ((2/((2*pid->Tau)+refreshRate))*(pid->currentRadius - pid->lastCurrentRadius));

	//------Update integrator - AntiWindup(only use the integrator if we are close, but not too close)
	if (abs(((int)(1000000*pid->error_r))) > 200)
		pid->integrator_r = ((int)((pid->integrator_r))) + ((int)(1000000*((refreshRate/2)*(pid->error_r + pid->lastError_r))));

	//------Output Calculation
	//pid->Ki_r = pid->Ki_r/1000000;
	P = (pid->Kp_r * pid->error_r);
	I = (pid->Ki_r * pid->integrator_r);
	D = (pid->Kd_r * pid->differentiator_r);
	pid->radiusEqualibrium = (40000.0/desiredRadius);


//	Wireless_Debug("P: ");
//	PrintInt(P);
//	Wireless_Debug("\n\r");
//	Wireless_Debug("Integral: ");
//	PrintFloat(pid->integrator_r);
//	Wireless_Debug("\n\r");
//	Wireless_Debug("error: ");
//	PrintFloat(pid->error_r);
//	Wireless_Debug("\n\r");
//	Wireless_Debug("lastError: ");
//	PrintFloat(pid->lastError_r);
//	Wireless_Debug("\n\r");
//	Wireless_Debug("Refresh: ");
//	PrintFloat(refreshRate);
//	Wireless_Debug("\n\r");
//	Wireless_Debug("I: ");
//	PrintInt(I);
//	Wireless_Debug("\n\r");
//	Wireless_Debug("velocityBack: ");
//		PrintFloat(pid->currentVelocityBack);
//		Wireless_Debug("\n\r");
//		Wireless_Debug("velocityFront: ");
//			PrintFloat(pid->currentVelocity);
//			Wireless_Debug("\n\r");
//	Wireless_Debug("-----------------------------");
//	Wireless_Debug("\n\r");
//	Wireless_Debug("D: ");
//	PrintInt(D);
//	Wireless_Debug("\n\r");

	Wireless_ControlLog_Ext(pid->currentRadius, pid->desiredRadiusPID, pid->error, pid->outputPID_unsat, P);

	pid->outputPID_unsat_r = (P + I - D) + pid->radiusEqualibrium;

	pid->outputPID_r = sat(pid->outputPID_unsat_r, 40);

	//pid->integrator_r = ((int)pid->integrator_r) + ((int)(1000000*(refreshRate/pid->Ki_r)*(pid->outputPID_r - pid->outputPID_unsat_r)));

	//------Save Info for graph
	Wireless_ControlLog_Ext(pid->currentRadius, desiredRadius, pid->error_r, pid->outputPID_unsat_r, I);

	//------Save states and send PWM to motors
	pid->lastCurrentRadius = pid->currentRadius;
	pid->lastError_r = pid->error_r;
	pid->lastIntegrator_r = pid->integrator_r;

//	Wireless_Debug("Last Error: ");
//	PrintFloat(pid->lastError_r);
//	Wireless_Debug("\n\r");
//	Wireless_Debug("Integral: ");
//	PrintFloat(pid->integrator_r);
//	Wireless_Debug("\n\r");
//	Wireless_Debug("-----------------------------");
//	Wireless_Debug("\n\r");

	SetServo(RC_STR_SERVO, pid->outputPID_r);
}
void PID_DistanceFlag(PID * pid, int desiredError){
	int ticks = getTicks();
	pid->distanceError = pid->desiredDistancePID - ticks;
	int distanceError = pid->distanceError;

	if (distanceError <= desiredError){
		//flag
	}
}

void PID_UpdateDistance(PID * pid){
	int ticks = getTicks();
	pid->distanceError = pid->desiredDistancePID - ticks;
	int distanceError = pid->distanceError;

	int velocity = pid->maxVelocity;
	CPU_MSR msr;

	//------Convert distance from back to front of car
//	msr = DISABLE_INTERRUPTS();
//	Gyro_Calculation(pid->gyro);
//	int ticks = getTicks();
//	RESTORE_INTERRUPTS(msr);
//	int backStartPoint = pid->backStartPoint;
//	int distanceTraveledBack = ticks - backStartPoint;
//	int distanceTraveledFront = Gyro_DistanceTraveledBackToFront(pid->gyro, distanceTraveledBack);
//	pid->distanceError = pid->desiredDistancePID - distanceTraveledFront;
//	int distanceError = pid->distanceError;
//
//	Wireless_Debug("distanceBack: ");
//				PrintFloat(distanceTraveledBack);
//				Wireless_Debug("\n\r");
//				Wireless_Debug("distanceFront: ");
//							PrintFloat(distanceTraveledFront);
//							Wireless_Debug("\n\r");
	if (velocity <= 2000){
			if (distanceError < 1500 && distanceError >= 1000)
				velocity = 800;
			else if (distanceError < 1000 && distanceError >= 700)
				velocity = 600;
			else if (distanceError < 700 && distanceError >= 500)
				velocity = 500;
			else if (distanceError < 500 && distanceError >= 200)
				velocity = 400;
			else if (distanceError < 200 && distanceError >= 40)
				velocity = 200;
			else if (distanceError < 40 && distanceError >= -40)
				velocity = 0;
			else if (distanceError < -40 && distanceError >= -1000)
				velocity = -200;
			else if (distanceError < -1000 && distanceError >= -8000)
				velocity = -400;
			else if (distanceError < -8000)
				velocity = 0;
	}
	else{
		if (distanceError < 1500 && distanceError >= 1000)
			velocity = velocity/2;
		else if (distanceError < 1000 && distanceError >= 700)
			velocity = velocity/3;
		else if (distanceError < 700 && distanceError >= 500)
			velocity = velocity/4;
		else if (distanceError < 500 && distanceError >= 200)
			velocity = velocity/5;
		else if (distanceError < 200 && distanceError >= 100)
			velocity = 0;
		else if (distanceError < 100 && distanceError >= 40)
			velocity = 200;
		else if (distanceError < 40 && distanceError >= -40)
			velocity = 0;
		else if (distanceError < -40 && distanceError >= -100)
			velocity = -200;
		else if (distanceError < -200 && distanceError >= -500)
			velocity = -velocity/5;
		else if (distanceError < -500 && distanceError >= -700)
			velocity = -velocity/4;
		else if (distanceError < -700 && distanceError >= -1000)
			velocity = -velocity/3;
		else if (distanceError < -1000 && distanceError >= -8000)
			velocity = -velocity/2;
		else if (distanceError < -8000)
			velocity = 0;
	}

	PID_SetVelocity(pid,velocity);
	PID_UpdateVelocity(pid);

	//	int vel =  pid->desiredVelocityPID;
	//
	//	if(distanceError < -100)
	//		vel = -200;
	//	else if(distanceError < vel/3)
	//		vel = vel * 2/3;
	//
	//	pid->desiredVelocityPID = velocity;
	//	updateVelocityOutput();
}


void inline PID_SetDistance(PID * pid, int distance){
	pid->backStartPoint = getTicks();
	pid->desiredDistancePID = distance + getTicks();
}

void inline PID_SetVelocity(PID * pid, int velocity){
	if (velocity > 4000)
		velocity = 4000;
	else if (velocity < -2000)
		velocity = -2000;
	pid->desiredVelocityPID = velocity;
}

void inline PID_SetRadius(PID * pid, int direction_RIGHT_LEFT, int radius_in_ticks){
	if (radius_in_ticks > 5000)
		radius_in_ticks = 5000;
	else if (radius_in_ticks < 1000)
		radius_in_ticks = 1000;
	pid->desiredRadiusPID = radius_in_ticks*direction_RIGHT_LEFT;
}


/// PRIVATE FUNCTION
int sat(int in, int limit) {
	if (in >= limit){
		return limit;
	}
	else if (in <= -limit){
		return -limit;
	}
	return in;
}

int downShift(int in, int limit, int out) {
	if (in <= limit){
		return out;
	}
	return in;
}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//----------------------------------------------Dead Code----------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//	if (velocity == 1 ||(velocity > 0 && velocity <= 1000))
//		velocity = 1000;
//	else if (velocity == 2 ||(velocity > 1000 && velocity <= 2000))
//		velocity = 2000;
//	else if (velocity == 3 ||(velocity > 2000 && velocity <= 3000))
//		velocity = 3000;
//	else if (velocity == 4 ||(velocity > 3000 && velocity <= 4000))
//		velocity = 4000;
//	else if (velocity == -1 ||(velocity < 0 && velocity >= -1000))
//		velocity = -1000;
//	else if (velocity == -2 ||(velocity < 1000 && velocity >= -2000))
//		velocity = -2000;
//	else if (velocity == -3 ||(velocity < 2000 && velocity >= -3000))
//		velocity = -3000;
//	else if (velocity == -4 ||(velocity < 3000 && velocity >= -4000))
//		velocity = -4000;
//	else if (velocity == 1000000)
//		velocity = 5000;
//	else
//		velocity = 0;
//---------------------------------------------------------------------------------------------------
//	struct tmp_struct {
//		float diff;
//		float tau;
//		float refreshRate;
//		float currentVelocity;
//		float lastCurrentVelocity;
//	};
//	struct tmp_struct TMP;
//	TMP.diff = pid.differentiator;
//	TMP.tau = pid.Tau;
//	TMP.refreshRate = refreshRate;
//	TMP.currentVelocity = pid.currentVelocity;
//	TMP.lastCurrentVelocity = pid.lastCurrentVelocity;
//	Wireless_Send(&wireless, 4, sizeof(struct tmp_struct), &TMP);
//---------------------------------------------------------------------------------------------------
//void updateDistanceOutput()
//{
//	int32 desiredDistance = pid.desiredDistancePID;
//	int32 P, I, D;
//	uint32 deltaClocks;
//	CPU_MSR msr;
//
//	msr = DISABLE_INTERRUPTS();
//	pid.encoderValue = getTicks();
//	uint32 nowClocks = ClockTime();
//	RESTORE_INTERRUPTS(msr);
//	//logData(desiredDistance, pid.encoderValue);
//
//	//------Time since last function call in seconds
//	uint32 maxClocks = 0xffffffff;
//	if ((nowClocks < pid.lastClockTicks))
//		deltaClocks = (maxClocks-pid.lastClockTicks)+nowClocks;
//	else
//		deltaClocks = nowClocks - pid.lastClockTicks;
//	float deltaTime = ((float)deltaClocks)/XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ ;//time passed since last function call: sec
//	pid.error_d = desiredDistance - pid.encoderValue;
//
//	//------Update integrator - AntiWindup(only use the integrator if we are close, but not too close)
//	if ((pid.error_d < 1000 && pid.error_d > -1000) && (pid.error_d > 100 || pid.error_d < -100)){
//		pid.integrator_d = pid.integrator_d + (deltaTime/2)*(pid.error_d + pid.lastError_d);
//	}
//	else
//		pid.integrator_d = 0;
//
//	//------only use the integrator if we are close, but not too close
////	if (pid.error_d < 500 && pid.error_d > -500 &&
////			(pid.error_d > 100 || pid.error_d < -100))
////		pid.integrator_d += pid.error_d * deltaTime;
////	else
////		pid.integrator_d = 0;
//
//	//------Update Derivative
//	pid.differentiator_d = (2*pid.Tau-deltaTime)/(2*pid.Tau+deltaTime)*pid.differentiator_d + 2/(2*pid.Tau+deltaTime)*(pid.error_d-pid.lastError_d);
//
//	P = pid.Kp_d * pid.error_d;
//	I = pid.Ki_d * pid.integrator_d;
//	D = pid.Kd_d * pid.differentiator_d;
//
//
//	pid.outputPID_unsat = P + I - D;
//	pid.outputPID = sat(pid.outputPID_unsat, 60);
//
//	//------if we are really close, don't do anything!
//	//if (pid.error_d < 100 && pid.error_d > -100) {
//	//	pid.outputPID = 0;
//	//}
//
//	//------Integrator Anti-windup
////	if (pid.Ki_d != 0.0f) {
////		pid.integrator_d = pid.integrator_d + deltaTime/pid.Ki_d * (pid.outputPID - pid.outputPID_unsat);
////	}
//
//	pid.lastError_d = pid.error_d;
//	pid.lastDesiredDistance = desiredDistance;
//
//	SetServo(RC_VEL_SERVO, pid.outputPID);
//}
//---------------------------------------------------------------------------------------------------
//void setSteeringRadius(int direction, uint32 radius_cm) {
//	int str = 0;
//	switch (radius_cm){
//			case 100:
//				str = 64;
//				break;
//			case 125:
//				str = 56;
//				break;
//			case 150:
//				str = 48;
//				break;
//			case 175:
//				str = 44;
//				break;
//			case 200:
//				str = 40;
//				break;
//			case 225:
//				str = 36;
//				break;
//			case 250:
//				str = 32;
//				break;
//			case 275:
//				str = 28;
//				break;
//			case 300:
//				str = 24;
//				break;
//			default:
//				str = 0;
//				break;
//	}
//	SetServo(RC_STR_SERVO, (direction*str));
//}
//---------------------------------------------------------------------------------------------------

