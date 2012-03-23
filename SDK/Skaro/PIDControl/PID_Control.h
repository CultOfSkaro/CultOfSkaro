/*
 * PID_Control.h
 *
 *  Created on: Jan 31, 2012
 *      Author: Andrew Norgrant
 */

#ifndef PID_CONTROL_H_
#define PID_CONTROL_H_
#include "Header.h"

//#include "SystemTypes.h"

typedef struct
	{
	//General Variables
	int outputPID;
	float outputPID_unsat;
	int outputPID_c;
	int outputPID_unsat_c;
	int outputPID_k;
	int outputPID_unsat_k;
	int encoderValue;
	int lastEncoderValue;              // Last position input
	float Tau;
	//Velocity Variables
	float Ki;                     // integral gain
    float Kp;                     // proportional gain
    float Kd;                     // derivative gain
    float differentiator;
	float integrator;
	int lastError;
	int desiredVelocityPID;
	int currentVelocity;
	int currentVelocityBack;
	int lastDesiredVelocity;
	int lastCurrentVelocity;
	uint32 lastClockTicks;
	float error;
	//Distance Variables
	float Kp_d;
	float Kd_d;
	float Ki_d;
	float differentiator_d;
	float integrator_d;
	uint32 desiredDistancePID;
	uint32 lastDesiredDistance;
	int error_d;
	int lastError_d;
	int distanceError;
	uint32 lastCurrentDistance;
	int maxVelocity;
	//Angle Heading Variables
	int desiredAnglePID;
	//Angle Centroid Variables
	int desiredCentroidPID;
	float Kp_c;
	float Kd_c;
	float Ki_c;
	float integrator_c;
	float differentiator_c;
	int lastError_c;
	int lastCurrentCentroid;
	int currentCentroid;
	int error_c;
	int lastClockTicks_c;
	//Angle Curvature Variables
	int desiredCurvaturePID;
	float Kp_k;
	float Kd_k;
	float Ki_k;
	float integrator_k;
	float differentiator_k;
	int lastError_k;
	int lastCurrentCurvature;
	int currentCurvature;
	int error_k;
	int lastClockTicks_k;
	} PID;

#define ENCODER_BASE_ADDR 0x7a400000

//#define WHEEL_CIRCUMFERENCE           0.366f
//#define TICKS_PER_TURN                3185
//#define WHEEL_CIRCUMFERENCE_TICKS     (WHEEL_CIRCUMFERENCE / TICKS_PER_TURN)
#define TICKS_PER_METER                 2000//       (TICKS_PER_TURN / WHEEL_CIRCUMFERENCE)
#define PID_UPDATES_PER_SECOND          100
#define PID_UPDATE_MOD                  (100 / PID_UPDATES_PER_SECOND)
#define TAU								0.5f
#define RIGHT							-1
#define LEFT							1

void initPID();
void updateVelocityOutput();
void updateDistanceOutput();
void updateCentroid();
void updateCurvatureOutput();
void setDistance(int32 distance);
void setVelocity(int32 velocity);
void setCurvature(int curvature);
void updateDistanceSetVelocity(int velocity);
int sat(int in, int limit);

extern volatile PID pid;

//void setSteeringRadius(int direction, uint32 radius_cm); //Right = direction:1 Left = direction:-1
#endif /* PID_CONTROL_H_ */
