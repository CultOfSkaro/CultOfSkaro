/*
 * Navigation.c
 *
 *  Created on: Winter, 2012
 *      Author: Cult of Skaro
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
#include "math.h"

GyroData raw_gyro_data;
Navigation navigation;


//%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%
//--------------------------------------------Navigation Equations/Conversions----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%

////
//--------------------------------------------Gyro Calculation----------------------------------------------------------------
////



////
//--------------------------------------------Convertsions----------------------------------------------------------------
////



////
//--------------------------------------------Absolute Reckoning code----------------------------------------------------------------
////

//MICHAEL/PETER's math...incomplete
void dubin_curves_math(int distance, float bearing) { //input is a relative based destination
	/* Assumptions:
	 * 	* Current position is (0,0) on grid system
	 * 	* Current bearing is 90 degrees
	 * 	* There is no desired bearing in the end, just that we are facing the target
	 * 	* bearing given is an offset from current bearing, ie. 45 degrees is to the left, -45 is to the right
	 */

	//Equation constants
	int buffer = 4000; //(ticks) How far away from target should we be in the end (don't want to get to close)
	int radius = 1; //(meters)

	//Equation Variables
	int straight_distance, arc_distance; //(ticks) Desired lengths of arc and tangent lines
	int Pnx, Pny, Pox, Poy, Cox, Coy, Cnx, Cny;  //(ticks) P=Car_Location, C=Circle_Center, n=new/desired, o=old
	float theta, sigma, phi;  //(radians) theta=arc angle, sigma=angle from circle center to Pn and tangent to circle
	int next_state; //is first turn to the left or to the right? 0=right, 1 = left, -1 = no turn, straight


	if(bearing == 0) {
		arc_distance = 0;
		straight_distance = distance - buffer;
		next_state = -1;
	}
	else {
		if( bearing < 0)  		// to the right
			next_state = 0;
		else
			next_state = 1;

		//Relative to coordinate system conversion
		Pnx = distance * cos(bearing);
		Pny = distance * sin(bearing);

		//Due to assumptions:
		Pox = Poy = Coy = 0; 	//because we are centered at origin
		Cox = radius;
		Cnx = Pnx;				//Not a waypoint, no second circle to loop around...
		Cny = Pny;

		//Find three angles
		int sig = (Cny-Coy)/(Cnx-Cox);
		sigma = PI/2 - 1/sig + 1/(3*sig*sig*sig);		//arctan(sig);
		int ph = radius/sqrt((Cnx-Cox)*(Cnx-Cox) + Cny*Cny);
		phi = PI/2 - ph - ph*ph*ph/6;					//arccos(ph);
		theta = 180 - sigma - phi;

		//Find straight_distance and arc_distance
		straight_distance = radius / (1-(phi*phi)/2);	//radius / cos(phi);
		arc_distance = 	radius*theta;					//2*radius*PI*theta/(2*PI);


		//APROXIMATIONS USED
		//cos(x) 	~= (1-(x^2)/2)
		//sin(x)	~= x - x^3/6 + x^5/120
		//arcTan(x)	~= PI/2 - 1/x + 1/3x^3 - 1/5x^5
		//arcCos(x) ~= PI/2 - x - x^3/6 - 3x^5/40
		//arcSin(x) ~= x + x^3/6 + 3x^5/40
	}
}


//%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%
//--------------------------------------------Controls----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%

void Navigation_Init(Navigation * n){
	Navigation_SetVelocityMode(n,DISTANCE_VELOCITY_MODE);
	Navigation_SetSteeringMode(n,CENTROID_MODE);
	PID_Init(&n->pid);
	Gyro_Init(&n->gyro);
	n->pid.gyro = &n->gyro;
}

void Navigation_SetVelocityMode(Navigation * n,int mode){
	// filter mode via switch case
	switch(mode){
	case DO_NOTHING_MODE:
	case VELOCITY_MODE:
	case DISTANCE_VELOCITY_MODE:
		n->velocityLoopMode = mode;
		break;
	default:
		// if mode wasn't valid, don't do anything
		break;
	};

}
void Navigation_SetSteeringMode(Navigation * n,int mode){
	// filter mode via switch case
	switch(mode){
	case DO_NOTHING_MODE:
	case CENTROID_MODE:
	case RADIUS_MODE:
		n->steeringLoopMode = mode;
		break;
	default:
		// if mode wasn't valid, don't do anything
		break;
	};
}

void Navigation_GoVelocity(Navigation *n, int velocity){
	PID_SetVelocity(&n->pid,velocity);
	//flag velocity 1
	Navigation_SetVelocityMode(n, VELOCITY_MODE);
}

void Navigation_GoDistanceVelocity(Navigation * n, int distance, int velocity){
	PID_SetDistance(&n->pid,distance);
	n->pid.maxVelocity = velocity;
	//flag velocity 2
	Navigation_SetVelocityMode(n, DISTANCE_VELOCITY_MODE);
}

void Navigation_GoCentroid(Navigation * n){
	//flag steering 1
	Navigation_SetSteeringMode(n,CENTROID_MODE);
}

void Navigation_HoldAngle(Navigation * n,int direction_RIGHT_LEFT, int angle){
	int radius = Gyro_SteeringAngleToRadius(&n->gyro, angle);
	PID_SetRadius(&n->pid,direction_RIGHT_LEFT,radius);
	//flag steering 2
	Navigation_SetSteeringMode(n,RADIUS_MODE);
}

void Navigation_HoldRadius(Navigation * n, int direction_RIGHT_LEFT, int radius){
	PID_SetRadius(&n->pid,direction_RIGHT_LEFT,radius);
	//flag steering 2
	Navigation_SetSteeringMode(n,RADIUS_MODE);
}

void Navigation_HoldCurvature(Navigation * n, float curvature, int direction_RIGHT_LEFT){
	int radius = 1/curvature;
	PID_SetRadius(&n->pid,direction_RIGHT_LEFT,radius);
	//flag steering 2
	Navigation_SetSteeringMode(n,RADIUS_MODE);
}

void Navigation_HoldServo(Navigation * n, int servoSetting){
	SetServo(RC_STR_SERVO, servoSetting);
	//flag steering 3
	Navigation_SetSteeringMode(n,DO_NOTHING_MODE);
}

void Stop(){
	PID_SetVelocity(&navigation.pid,0);
	PID_SetDistance(&navigation.pid,0);
}

void Navigation_SteeringLoop(Navigation * n){
	switch(n->steeringLoopMode){
	case CENTROID_MODE:
		PID_UpdateCentroid(&n->pid);
		break;
	case RADIUS_MODE:
		PID_UpdateRadius(&n->pid);
		break;
	case DO_NOTHING_MODE:
	default:
		//Do Nothing
		break;
	}
}

void Navigation_VelocityLoop(Navigation * n){
	switch(n->velocityLoopMode){
	case DISTANCE_VELOCITY_MODE:
		PID_UpdateDistance(&n->pid);
		break;
	case VELOCITY_MODE:
		PID_UpdateVelocity(&n->pid);
		break;
	case DO_NOTHING_MODE:
	default:
		//Do Nothing
		break;
	}
}
