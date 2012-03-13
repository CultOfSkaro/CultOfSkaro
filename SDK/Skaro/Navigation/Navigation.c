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

extern PID pid;

// Gyro degrees per sec

GYRO_CORRECTIONS gyro;

void gyroCalculation(int gyroInput, int velocityBack)
{
	gyro.omega = (PI*gyroInput)/180;
	gyro.backCurvature = gyro.omega/velocityBack;
	// V_f
	//Error because of math.h not included, we're using ^ and srt()
	/////////gyro.frontVelocity = velocityBack * sqrt(1+((gyro.backCurvature)^2*(gyro.wheelBase)^2));
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
	/////////gyro.frontEncoder = (gyro.backEncoder/(1 - ((gyro.steeringAngle)^2)/2));
}


