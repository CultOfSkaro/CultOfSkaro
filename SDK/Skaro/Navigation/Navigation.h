#ifndef Navigation_H
#define Navigation_H_
#include "Header.h"

#define PI 3.14159265
#define CONVERT_TO_RAD_SEC 0.0001527163 // (8.75 milidegees/sec)(1/1000)(pi/180)

typedef struct
{
	float omega; //w
	float backCurvature; //k_b
	float frontCurvature; //k_f
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

extern GyroData raw_gyro_data;
extern GYRO_CORRECTIONS gyro;
extern volatile int max_velocity;

void steering_loop();
void velocity_loop();
void gyroCalculation();
int velocityBackToFront(int velocityBack);
void encoderBackToFrontCorrections();
int distanceBackToFront(int distance);
int absDistanceFromArchDistCuvrature(float curvature, int archDistance);
void dubin_curves_math(int distance, float bearing);

#endif
