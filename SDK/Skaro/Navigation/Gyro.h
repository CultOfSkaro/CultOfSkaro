#ifndef SKARO_GYRO
#define SKARO_GYRO
//Constants
#define PI 						3.14159265
#define CONVERT_TO_RAD_SEC 		0.0001527163 // (8.75 milidegees/sec)(1/1000)(pi/180)
#define WHEELBASE 				540 // b = ~27cm = ~540ticks

#define GYRO_MOVING_WINDOW_SIZE 5
// 1/10 seconds per sample and divide by 2 to get average of samples
#define GYRO_INTEGRATION_DIVISOR 20



typedef struct
{
	float omega; //w
	float backCurvature; //k_b
	float frontCurvature; //k_f
	int backRadius; //k_b
	int frontRadius; //k_f
	int frontVelocity; //v
	float steeringAngle; //delta
	int wheelBase; // b
	int backVelocity;//v_b
	//S is the distance recorded by the encoder
	int backEncoder; //S_b
	int frontEncoder; //S_f

}Gyro;

typedef struct {
	short angular_velocity;
	short velocity;
	int total_angle;
	unsigned char packet[5];
} GyroData;

extern GyroData raw_gyro_data;

void Gyro_Init(Gyro * g);
void Gyro_Calculation(Gyro * gyro);
inline int Gyro_GetTotalAngle();
inline float Gyro_SteeringAngleToRadius(Gyro * gyro, int steeringAngle);
inline float Gyro_SteeringAngleToCurvature(Gyro * gyro, int steeringAngle);
inline float Gyro_CurvatureBackToFront(Gyro * gyro);
inline int Gyro_VelocityBackToFront(Gyro * gyro, int velocityBack);
inline int Gyro_DistanceTraveledBackToFront(Gyro * gyro, int distance);
inline int Gyro_DistanceTraveledFrontToBack(Gyro * gyro, int distance);
inline int Gyro_AbsDistanceFromArchDistCuvrature(Gyro * gyro, float curvature, int archDistance);

#endif
