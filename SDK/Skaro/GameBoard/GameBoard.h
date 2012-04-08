#ifndef ROBOT_RACER_GAME_BOARD
#define ROBOT_RACER_GAME_BOARD

#include "Header.h"
//#include "SystemTypes.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xexception_l.h"

//***********************************************************************
//CONSTANTS

#define GPIO_DEVICE_ID				XPAR_XPS_GPIO_1_DEVICE_ID
#define INTC_DEVICE_ID				XPAR_XPS_INTC_0_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID	XPAR_XPS_INTC_0_XPS_GPIO_0_IP2INTC_IRPT_INTR
#define GAME_SYSTEM_GPIO_CHANNEL	2
#define	GAME_SYSTEM_DIRECTION		0xFFFFD8FF
#define GAME_SYSTEM_GPIO_INT		XPAR_XPS_GPIO_1_IP2INTC_IRPT_MASK

// Game System Interface (16 bits)
#define GAME_TRUCK_IS_BASE_M		0x0001
#define GAME_HAVE_FLAG_M			0x0002
#define GAME_TRUCK_ALIVE_M			0x0004
#define GAME_TRUCK_REGISTERED_M		0x0008
#define GAME_TEAM_NUM_1_M			0x0010
#define GAME_TEAM_NUM_0_M			0x0020
#define GAME_STATE_1_M				0x0040		//stop, go, pause, win
#define GAME_STATE_0_M				0x0080
#define GAME_KILL_SHOT				0x0100
#define GAME_PASS_SHOT				0x0200
#define GAME_REVIVE_SHOT			0x0400
#define GAME_HIT_ACK				0x0800
#define GAME_MISS_ACK				0x1000
#define HELIOS_ENABLE_GYRO			0x2000
#define GAME_NOT_IN_PLAY			0x4000
#define GAME_WAIT_TO_SHOOT			0x8000

//***********************************************************************
//TYPES

typedef struct {
	BOOL alive;
	BOOL hasFlag;
	BOOL prev_hit_state;
	BOOL hit;
	BOOL gameNotInPlay;
} GameBoard;


//***********************************************************************
//GLOBALS

extern XGpio Gpio;
extern GameBoard gameBoard;

//***********************************************************************
//FUNCTIONS

void GB_Init();
void GB_Shoot(uint16 shotType);
void GB_EnableGyro();
void GB_DisableGyro();

#endif
