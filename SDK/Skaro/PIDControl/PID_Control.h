/*
 * PID_Control.h
 *
 *  Created on: Jan 31, 2012
 *      Author: Andrew Norgrant
 */

#ifndef PID_CONTROL_H_
#define PID_CONTROL_H_
#include "Header.h"
#include "Gyro.h"

//#include "SystemTypes.h"

#define ENCODER_BASE_ADDR 0x7a400000


//#define WHEEL_CIRCUMFERENCE           0.366f
//#define TICKS_PER_TURN                3185
//#define WHEEL_CIRCUMFERENCE_TICKS     (WHEEL_CIRCUMFERENCE / TICKS_PER_TURN)
#define TICKS_PER_METER                 2000//       (TICKS_PER_TURN / WHEEL_CIRCUMFERENCE)
#define PID_UPDATES_PER_SECOND          100
#define PID_UPDATE_MOD                  (100 / PID_UPDATES_PER_SECOND)
#define TAU								0.5f

typedef struct
	{
	//General Variables
	int outputPID;
	float outputPID_unsat;
	int outputPID_c;
	int outputPID_unsat_c;
	int outputPID_r;
	int outputPID_unsat_r;
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
	int desiredRadiusPID;
	float Kp_r;
	float Kd_r;
	float Ki_r;
	int radiusEqualibrium;
	float integrator_r;
	float differentiator_r;
	float lastError_r;
	int lastCurrentRadius;
	float lastIntegrator_r;
	int currentRadius;
	float error_r;
	int lastClockTicks_r;
	Gyro * gyro;
	} PID;


void PID_Init(PID * pid);
void PID_UpdateVelocity(PID * pid);
void PID_UpdateDistance(PID * pid);
void PID_UpdateCentroid(PID * pid);
void PID_UpdateRadius(PID * pid);
void inline PID_SetDistance(PID * pid,int32 distance);
void inline PID_SetVelocity(PID * pid, int32 velocity);
void inline PID_SetRadius(PID * pid, int direction_RIGHT_LEFT, int radius);
void PID_UpdateDistance(PID * pid);
int sat(int in, int limit);
int downShift(int in, int limit, int out);

//void setSteeringRadius(int direction, uint32 radius_cm); //Right = direction:1 Left = direction:-1
#endif /* PID_CONTROL_H_ */
