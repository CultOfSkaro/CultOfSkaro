/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    ServoControl.c
AUTHOR:  Wade Fife
CREATED: 01/20/05

DESCRIPTION

Software to control a standard servos (e.g., in RC cars) using Wade
Fife's PWM peripheral.

Configure the base address and PLB clock frequency for the PWM
peripheral in the header file. The PWM peripheral must be configured
to have one PWM unit for each servo to be controlled.

CHANGLE LOG

2/9/06 WSF Changed SetServo to use the range -64 to +64 to simplify arithmetic.
1/25/10 Drew Withers, modified for use on PLB

******************************************************************************/

#include "plb_pwm_ctrl.h"
#include "Header.h"
#include "ServoControl.h"



// DEFINES ////////////////////////////////////////////////////////////////////


// These define constants related to servo control. For example, SERVO_PERIOD
// represents the PWM period (in bus clock cycles) expected by most servos and
// SERVO_DUTY_CTR is the duty (in cycles) required to move the servo to its
// center or neutral position. SERVO_DISP is the amount the duty is allowed to
// change from the neutral position. SERVO_DUTY_MIN and SERVO_DUTY_MAX are the
// minimum and maximum allowed duty cycles (e.g., hard left and hard right).
#define SERVO_PERIOD    ((uint32)(SERVO_PLB_FREQ * 0.02   + 0.5))
#define SERVO_DUTY_CTR  ((uint32)(SERVO_PLB_FREQ * 0.0015 + 0.5))
#define SERVO_DISP      ((uint32)(SERVO_PLB_FREQ * 0.0006 + 0.5))
#define SERVO_DUTY_MIN  ((uint32)(SERVO_DUTY_CTR - SERVO_DISP))
#define SERVO_DUTY_MAX  ((uint32)(SERVO_DUTY_CTR + SERVO_DISP))



// FUNCTIONS //////////////////////////////////////////////////////////////////


// Initializes the period for the servo and set the PWM to move it to its
// center position.
void ServoInit(int servoNum)
{
	// Set PWM period for servo control
	SET_PWM_PERIOD(SERVO_BASE_ADDR, servoNum, SERVO_PERIOD);

	// Set PWM duty so that servo moves to center position
	SET_PWM_DUTY(SERVO_BASE_ADDR, servoNum, SERVO_DUTY_CTR);
}



// Set servo position from -64 to +64
void SetServo(int servoNum, char position)
{
	uint32 duty;
	int spos = (int)position;
	if (spos > 127) spos -= 256;

	// Saturate input
	if(spos < -64) spos = -64;
	else if(spos > 64) spos = 64;

	duty = SERVO_DUTY_CTR + ((int32)SERVO_DISP * (int32)spos) / 64;

	SET_PWM_DUTY(SERVO_BASE_ADDR, servoNum, duty);
}


// Get servo position from -64 to +64
char GetServo(int servoNum)
{
	int32 position;

	position = ((int32)GET_PWM_DUTY(SERVO_BASE_ADDR, servoNum) - (int32)SERVO_DUTY_CTR)*64/(int32)SERVO_DISP;

	return (char)position;
}

