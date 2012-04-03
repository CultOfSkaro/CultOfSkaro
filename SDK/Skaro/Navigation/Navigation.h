#ifndef Navigation_H
#define Navigation_H
#include "Header.h"
#include "PID_Control.h"
#include "Gyro.h"


//PID loop flags
#define DO_NOTHING_MODE			0
#define VELOCITY_MODE 			1
#define DISTANCE_VELOCITY_MODE 	2
#define CENTROID_MODE 			3
#define RADIUS_MODE 			4
#define SERVO_MODE 				5
//Steering directions
#define RIGHT					-1
#define LEFT					1



typedef struct {
	int velocityLoopMode;
	int steeringLoopMode;
	PID pid;
	Gyro gyro;
} Navigation;

extern Navigation navigation;

int velocityBackToFront(int velocityBack);
void encoderBackToFrontCorrections();
int distanceBackToFront(int distance);
int absDistanceFromArchDistCuvrature(float curvature, int archDistance);
void dubin_curves_math(int distance, float bearing);

//API
void Stop();
void Navigation_Init(Navigation * n);
void Navigation_SetVelocityMode(Navigation * n,int mode);
void Navigation_SetSteeringMode(Navigation * n,int mode);
void Navigation_SteeringLoop(Navigation * n);
void Navigation_VelocityLoop(Navigation * n);
void Navigation_GoVelocity(Navigation * n, int velocity);
void Navigation_GoDistanceVelocity(Navigation * n, int velocity, int distance);
void Navigation_GoCentroid(Navigation * n);
void Navigation_HoldAngle(Navigation * n,int direction_RIGHT_LEFT, int angle);
void Navigation_HoldRadius(Navigation * n, int direction_RIGHT_LEFT, int radius);
void Navigation_HoldCurvature(Navigation * n, float curvature, int direction_RIGHT_LEFT);
void Navigation_HoldServo(Navigation * n, int servoSetting);
#endif
