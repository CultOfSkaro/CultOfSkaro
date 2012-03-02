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
	  uint32 distanceMode;             // PID mode
	  int desiredEncoderValue;   // Want to do something when encoder get to this value
	  int lastEncoderValue;              // Last position input
	  int lastDesiredVelocity;
	  uint32 iState;         // Integrator state
	  uint32 count;
	  uint32 iMax;                   // Maximum and minimum allowable integrator state
	  uint32 iMin;
	  float Ki;                     // integral gain
	  float Kp;                     // proportional gain
	  float Kd;                     // derivative gain
	  uint32 lastClockTicks;
	  float error;
	  int outputPID;
	  float outputPID_unsat;
	  float differentiator;
	  float integrator;
	  int lastError;
	  float desiredVelocityPID;
	  int desiredAnglePID;
	  int encoderValue;
	  //Distance Variables
	  uint32 lastDeltaDistanceEncoderValue;
	  uint32 desiredDistancePID;
	  uint32 lastDesiredDistance;
	  uint32 lastCurrentDistance;
	  float lastCurrentVelocity;
	  int error_d;
	  float Kp_d;
	  float Kd_d;
	  float Ki_d;
	  float Tau;
	  float Ts;
	  int lastDerivative;
	  float differentiator_d;
	  float integrator_d;
	  int lastError_d;
	  int distanceError;
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
void setDistance(int32 distance);
void setVelocity(int32 velocity);
void updateDistanceSetVelocity(int velocity);
void setSteeringRadius(int direction, uint32 radius_cm); //Right = direction:1 Left = direction:-1
int sat(float in, float limit);


#endif /* PID_CONTROL_H_ */
