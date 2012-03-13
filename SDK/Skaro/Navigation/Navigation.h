#ifndef Navigation_H
#define Navigation_H_
#include "Header.h"

#define PI 3.14159265

typedef struct
{
	float omega; //w
	float backCurvature; //k_b
	float frontCurvature; //k_f
	float frontVelocity; //v
	float steeringAngle; //delta
	int wheelBase; // b

	//S is the distance recorded by the encoder
	float backEncoder; //S_b
	float frontEncoder; //S_f

}GYRO_CORRECTIONS;

void gyroCalculation(int gyroInput, int velocityBack);
void encoderBackToFrontCorrections();

#endif
