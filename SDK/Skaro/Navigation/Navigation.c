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
GYRO_CORRECTIONS gyro;
Navigation navigation;


//%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%
//--------------------------------------------Navigation Equations/Conversions----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%

////
//--------------------------------------------Gyro Calculation----------------------------------------------------------------
////

void initGyroCalculation()
{
	gyro.omega = 0.0f; //w
	gyro.backCurvature = 0.0f; //k_b
	gyro.frontCurvature = 0.0f; //k_f
	gyro.frontVelocity = 0.0f; //v
	gyro.steeringAngle = 0.0f; //delta
	gyro.wheelBase = WHEELBASE; // b = ~27cm = ~540ticks

	//S is the distance recorded by the encoder
	gyro.backEncoder = 0.0f; //S_b
	gyro.frontEncoder = 0.0f; //S_f
}

void gyroCalculation()
{
	short velocity;
	short angular_velocity;
	CPU_MSR msr;
	//gyro.velocityBack = pid.currentVelocityBack;

	msr = DISABLE_INTERRUPTS();
	velocity = raw_gyro_data.velocity;
	angular_velocity = raw_gyro_data.angular_velocity;
	RESTORE_INTERRUPTS(msr);

	// V_b
	gyro.velocityBack = 2*(raw_gyro_data.velocity/.1);
	// W
	gyro.omega = CONVERT_TO_RAD_SEC * raw_gyro_data.angular_velocity;
	//Wireless_Debug("Omgega:");
	//PrintFloat(gyro.omega);
	// K_b
	gyro.backCurvature = gyro.omega/gyro.velocityBack;
	//R_b
	gyro.backRadius = 1/gyro.backCurvature;
	// V_f
	gyro.frontVelocity = gyro.velocityBack * sqrt(1+((gyro.backCurvature)*gyro.backCurvature*(gyro.wheelBase)*gyro.wheelBase));
	// K_f
	gyro.frontCurvature = gyro.omega/gyro.frontVelocity;
	//R_b
	gyro.frontRadius = 1/gyro.frontCurvature;
	// Delta
	gyro.steeringAngle = asin(gyro.wheelBase * gyro.frontCurvature);
}

////
//--------------------------------------------Convertsions----------------------------------------------------------------
////

float steeringAngleToCurvature(int steeringAngle){
	return (sin(gyro.frontCurvature))/gyro.wheelBase;
}

float curvatureBackToFront()
{
	return gyro.frontCurvature = gyro.omega/gyro.frontVelocity;
}

int velocityBackToFront(int velocityBack)
{
	// K_b
	gyro.backCurvature = gyro.omega/velocityBack;
	// V_f
	return gyro.frontVelocity = velocityBack * sqrt(1+((gyro.backCurvature*gyro.backCurvature)*(gyro.wheelBase*gyro.wheelBase)));
}

void encoderBackToFrontCorrections()
{
	gyro.backEncoder = pid.encoderValue - pid.lastEncoderValue;
	gyro.frontEncoder = (gyro.backEncoder/(1 - ((gyro.steeringAngle*gyro.steeringAngle))/2));
}

int distanceBackToFront(int distance)
{
	return distance/(1-((gyro.steeringAngle*gyro.steeringAngle)/2)); //distance devided by cos(delta)
}

int distanceFrontToBack(int distance)
{
	return distance*(1-((gyro.steeringAngle*gyro.steeringAngle)/2)); //distance devided by cos(delta)
}

int absDistanceFromArchDistCuvrature(float curvature, int archDistance)
{
	return archDistance-(int)((curvature*curvature*archDistance*archDistance*archDistance)/24);
}

void cameraInterpretation(int centroid, int distance){

	int backDistance = distanceFrontToBack(distance);

}

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
}

void Navigation_SetVelocityMode(Navigation * n,int mode){
	// filter mode via switch case
	switch(mode){
	case DO_NOTHING_MODE:
	case VELOCITY_MODE:
	case DISTANCE_VELOCITY_MODE:
		n->velocityLoopMode = mode;
	};
	// if mode wasn't valid, don't do anything
}
void Navigation_SetSteeringMode(Navigation * n,int mode){
	// filter mode via switch case
	switch(mode){
	case DO_NOTHING_MODE:
	case CENTROID_MODE:
	case CURVATURE_MODE:
		n->steeringLoopMode = mode;
	};
	// if mode wasn't valid, don't do anything
}

void goVelocity(int velocity){
	setVelocity(velocity);
	//flag velocity 1
	Navigation_SetVelocityMode(&navigation, VELOCITY_MODE);
}

void goDistance_Velocity(int distance, int velocity){
	setDistance(distance);
	pid.maxVelocity = velocity;
	//flag velocity 2
	Navigation_SetVelocityMode(&navigation, DISTANCE_VELOCITY_MODE);
}

void goCentroid(){
	//flag steering 1
	Navigation_SetSteeringMode(&navigation,CENTROID_MODE);
}

void holdAngle(int angle, int direction){
	int curvature = steeringAngleToCurvature(angle);
	setCurvature((direction*curvature));
	//flag steering 2
	Navigation_SetSteeringMode(&navigation,CURVATURE_MODE);
}

void holdRadius(int radius, int direction){
	int curvature = 1/radius;
	setCurvature((direction*curvature));
	//flag steering 2
	Navigation_SetSteeringMode(&navigation,CURVATURE_MODE);
}

void holdCurvature(int curvature, int direction){
	setCurvature((direction*curvature));
	//flag steering 2
	Navigation_SetSteeringMode(&navigation,CURVATURE_MODE);
}
//
//void holdServo(int servoSetting){
//	SetServo(RC_STR_SERVO, servoSetting);
//	//flag steering 3
//	controls.steeringLoopMode = SERVO_MODE;
//}

void Stop(){
	setVelocity(0);
	setDistance(0);
}

void steering_loop(){
//	switch(navigation.steeringLoopMode){
//	case CENTROID_MODE:
//		updateCentroid();
//	case CURVATURE_MODE:
//		updateCurvatureOutput();
//	}
	//else if (controls.steeringLoopMode == CURVATURE_MODE){
		//setCurvature(-1);  ANDREW.  WHAT DOES THIS DO????? ---- Just trying to test curvature AN
		//updateCurvatureOutput();
	//}
	SetServo(RC_STR_SERVO, 35);
}

void velocity_loop(){
//	switch(navigation.steeringLoopMode){
//	case DISTANCE_VELOCITY_MODE:
		updateDistanceSetVelocity(pid.maxVelocity);
//	case VELOCITY_MODE:
//		updateVelocityOutput();
//	}
}
