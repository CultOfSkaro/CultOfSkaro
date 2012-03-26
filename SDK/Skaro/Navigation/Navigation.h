#ifndef Navigation_H
#define Navigation_H_
#include "Header.h"

//Constants
#define PI 						3.14159265
#define CONVERT_TO_RAD_SEC 		0.0001527163 // (8.75 milidegees/sec)(1/1000)(pi/180)
#define WHEELBASE 				540 // b = ~27cm = ~540ticks
//PID loop flags
#define DO_NOTHING_MODE			0
#define VELOCITY_MODE 			1
#define DISTANCE_VELOCITY_MODE 	2
#define CENTROID_MODE 			3
#define CURVATURE_MODE 			4
#define SERVO_MODE 				5
//Steering directions
#define RIGHT					-1
#define LEFT					1

typedef struct
{
	float omega; //w
	float backCurvature; //k_b
	float frontCurvature; //k_f
	float backRadius; //k_b
	float frontRadius; //k_f
	float frontVelocity; //v
	float steeringAngle; //delta
	int wheelBase; // b
	int velocityBack;//v_b
	//S is the distance recorded by the encoder
	float backEncoder; //S_b
	float frontEncoder; //S_f

}GYRO_CORRECTIONS;

typedef struct {
	short angular_velocity;
	short velocity;
	unsigned char packet[5];
} GyroData;

typedef struct {
	int velocityLoopMode;
	int steeringLoopMode;
} Navigation;

extern GyroData raw_gyro_data;
extern GYRO_CORRECTIONS gyro;
extern Navigation navigation;
extern volatile int max_velocity;

void steering_loop();
void velocity_loop();
void gyroCalculation();
int velocityBackToFront(int velocityBack);
void encoderBackToFrontCorrections();
int distanceBackToFront(int distance);
int absDistanceFromArchDistCuvrature(float curvature, int archDistance);
void dubin_curves_math(int distance, float bearing);

//API
void Navigation_Init(Navigation * n);
void Navigation_SetVelocityMode(Navigation * n,int mode);
void Navigation_SetSteeringMode(Navigation * n,int mode);
#endif
