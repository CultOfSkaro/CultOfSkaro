#include "Gyro.h"
#include "math.h"
#include "InterruptControl.h"

void Gyro_Init(Gyro * gyro){
	gyro->omega = 0.0f; //w
	gyro->backCurvature = 0.0f; //k_b
	gyro->frontCurvature = 0.0f; //k_f
	gyro->frontVelocity = 0.0f; //v
	gyro->steeringAngle = 0.0f; //delta
	gyro->wheelBase = WHEELBASE; // b = ~27cm = ~540ticks
	gyro->frontRadius = 0;
	gyro->backRadius = 0;

	//S is the distance recorded by the encoder
	gyro->backEncoder = 0.0f; //S_b
	gyro->frontEncoder = 0.0f; //S_f
}

void Gyro_Calculation(Gyro * gyro)
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
	gyro->backVelocity = raw_gyro_data.velocity;

	// W
	gyro->omega = (CONVERT_TO_RAD_SEC * raw_gyro_data.angular_velocity);

	//R_b
	gyro->backRadius = gyro->backVelocity/gyro->omega;
	// K_b
	gyro->backCurvature = gyro->omega/gyro->backVelocity;

	// V_f
	gyro->frontVelocity = gyro->backVelocity * sqrt(1+((gyro->backCurvature)*gyro->backCurvature*(gyro->wheelBase)*gyro->wheelBase));
	// K_f
	gyro->frontCurvature = gyro->omega/gyro->frontVelocity;
	//R_b
	gyro->frontRadius = gyro->frontVelocity/gyro->omega;
	// Delta
	gyro->steeringAngle = asin(gyro->wheelBase * gyro->frontCurvature);
}

inline float Gyro_SteeringAngleToCurvature(Gyro * gyro, int steeringAngle){
	return (sin(gyro->frontCurvature))/gyro->wheelBase;
}

inline float Gyro_SteeringAngleToRadius(Gyro * gyro, int steeringAngle){
	return ((sin((1/gyro->frontCurvature)))/gyro->wheelBase);
}

inline float Gyro_CurvatureBackToFront(Gyro * gyro)
{
	return gyro->frontCurvature = gyro->omega/gyro->frontVelocity;
}

inline int Gyro_VelocityBackToFront(Gyro * gyro, int velocityBack)
{
	// K_b
	gyro->backCurvature = gyro->omega/velocityBack;
	// V_f
	return gyro->frontVelocity = velocityBack * sqrt(1+((gyro->backCurvature*gyro->backCurvature)*(gyro->wheelBase*gyro->wheelBase)));
}

//void Gyro_EncoderBackToFrontCorrections(Gyro * gyro)
//{
//	gyro->backEncoder = navigation.pid.encoderValue - navigation.pid.lastEncoderValue;
//	gyro->frontEncoder = (gyro->backEncoder/(1 - ((gyro->steeringAngle*gyro->steeringAngle))/2));
//}

inline int Gyro_DistanceBackToFront(Gyro * gyro, int distance)
{
	return distance/(1-((gyro->steeringAngle*gyro->steeringAngle)/2)); //distance devided by cos(delta)
}

inline int Gyro_DistanceFrontToBack(Gyro * gyro, int distance)
{
	return distance*(1-((gyro->steeringAngle*gyro->steeringAngle)/2)); //distance devided by cos(delta)
}

inline int Gyro_AbsDistanceFromArchDistCuvrature(Gyro * gyro, float curvature, int archDistance)
{
	return archDistance-(int)((curvature*curvature*archDistance*archDistance*archDistance)/24);
}

/*
inline void Gyro_CameraInterpretation(Gyro * gyro, int centroid, int distance){
	int backDistance = Gyro_DistanceFrontToBack(gyro, distance);
}
*/
