#ifndef SKARO_GYRO
#define SKARO_GYRO
//Constants
#define PI 						3.14159265
#define CONVERT_TO_RAD_SEC 		0.0001527163 // (8.75 milidegees/sec)(1/1000)(pi/180)
#define WHEELBASE 				540 // b = ~27cm = ~540ticks



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
	unsigned char packet[5];
} GyroData;

extern GyroData raw_gyro_data;

void Gyro_Init(Gyro * g);
void Gyro_Calculation(Gyro * gyro);

#endif
