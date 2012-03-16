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

extern PID pid;

// Gyro degrees per sec

GYRO_CORRECTIONS gyro;

void initGyroCalculation()
{
	gyro.omega = 0.0f; //w
	gyro.backCurvature = 0.0f; //k_b
	gyro.frontCurvature = 0.0f; //k_f
	gyro.frontVelocity = 0.0f; //v
	gyro.steeringAngle = 0.0f; //delta
	gyro.wheelBase = 540; // b = ~27cm = ~540ticks

	//S is the distance recorded by the encoder
	gyro.backEncoder = 0.0f; //S_b
	gyro.frontEncoder = 0.0f; //S_f
}

void gyroCalculation(int gyroInput, int velocityBack)
{
	gyro.omega = (PI*gyroInput)/180;
	gyro.backCurvature = gyro.omega/velocityBack;
	// V_f
	//Error because of math.h not included, we're using ^ and srt()
	gyro.frontVelocity = velocityBack * sqrt(1+((gyro.backCurvature)*gyro.backCurvature*(gyro.wheelBase)*gyro.wheelBase));
	// K_f
	gyro.frontCurvature = gyro.omega/gyro.frontVelocity;
	// Delta
	//Doesn't like the arcsin()
	gyro.steeringAngle = asin(gyro.wheelBase * gyro.frontCurvature);
}

void encoderBackToFrontCorrections()
{
	gyro.backEncoder = pid.encoderValue - pid.lastEncoderValue;
	//Error because of math.h not included, we're using ^
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

void cameraInterpretation(int centroid, int distance){

	int backDistance = distanceFrontToBack(distance);

}
