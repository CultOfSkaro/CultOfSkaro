#include "GameBoard.h"
#include "Serial.h"
#include "skaro_wireless.h"
#include "scheduler.h"

XGpio Gpio;
GameBoard gameBoard;
volatile int CurrentShotType;

void GB_Init()
{
	//set up globals
	gameBoard.alive = FALSE;
	gameBoard.hasFlag = FALSE;

	//Interrupts were already initialized in ISR.c (INIT_ISR())
	//Wireless_Debug("Initializing GPIO...");

	XGpio_Initialize(&Gpio, GPIO_DEVICE_ID);

	XGpio_SetDataDirection(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, GAME_SYSTEM_DIRECTION);
	XGpio_SetDataDirection(&Gpio, 1, 0xFFFFFFE7); //0xFFFFFFE7

	// Enable interrupts for GPIO
	XGpio_InterruptGlobalEnable(&Gpio);
	XGpio_InterruptEnable(&Gpio, XGPIO_IR_CH2_MASK);
}

void GB_EnableGyro() {
	XGpio_DiscreteSet(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, HELIOS_ENABLE_GYRO);
}

void GB_DisableGyro() {
	XGpio_DiscreteClear(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, HELIOS_ENABLE_GYRO);
}

/* Fires a shot specified by shotType
 *   This function will assert the shot type pin on the GPIO interface specified by shotType,
 *   shotType should be one of three shots (use the #defines)
 *   GAME_SHOOT_KILL - Kill shot
 *   GAME_SHOOT_PASS	- Pass shot
 *   GAME_SHOOT_REVIVE - Revive shot
 * The shot pin is automatically de-asserted by the wait-to-shoot ISR.  This function checks for the correct
 * conditions before allowing a shot to happen (truck enabled and not waiting to shoot).
 */
void GB_Shoot(uint16 shotType)
{
	//only shoot if the game is in play
	if (gameBoard.gameNotInPlay) return;

	if(!CurrentShotType) {
		Wireless_Debug("Shooting:");
		PrintInt(shotType);
		Wireless_Debug("\n\r");
		//ensure valid states for shooting
		if (shotType == GAME_KILL_SHOT && !gameBoard.alive) return;
		else if (shotType == GAME_PASS_SHOT && !gameBoard.hasFlag) return;
		else if (shotType == GAME_REVIVE_SHOT && gameBoard.alive) return;

		Wireless_Debug("Firing Shot\r\n");
		CurrentShotType = shotType;
		XGpio_DiscreteSet(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, shotType);
	}
}

#define SHOOT_STATE_NONE 0
#define SHOOT_STATE_WAITING 1

void GpioHandler(void *InstancePtr) {
	/*
	Game System Interface (16 bits)
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
	*/


	static int shootState = SHOOT_STATE_NONE;

	XGpio *GpioPtr = (XGpio *)InstancePtr;
	u32 gameState = XGpio_DiscreteRead(GpioPtr, GAME_SYSTEM_GPIO_CHANNEL);

	//check global flags
	gameBoard.alive = (gameState & GAME_TRUCK_ALIVE_M) != 0;
	gameBoard.hasFlag = (gameState & GAME_HAVE_FLAG_M) != 0;
	gameBoard.gameNotInPlay = (gameState & GAME_NOT_IN_PLAY) != 0;


	BOOL hit = (gameState & GAME_HIT_ACK) != 0;
	if(!gameBoard.prev_hit_state && hit)
		gameBoard.hit = 1;
	gameBoard.prev_hit_state = hit;

	scheduler.events.flags.gb_state_change = 1;

	/*
	Wireless_Debug("Gpio Changed: ");
	PrintWord(gameState);
	Wireless_Debug("\r\n");
	*/

	if (CurrentShotType != 0 && shootState == SHOOT_STATE_NONE &&
		(gameState & GAME_WAIT_TO_SHOOT) != 0) {

		//shot started
		shootState = SHOOT_STATE_WAITING;
	} else if (CurrentShotType != 0 && shootState == SHOOT_STATE_WAITING &&
		(gameState & GAME_WAIT_TO_SHOOT) == 0) {

		//done shooting
		XGpio_DiscreteClear(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, CurrentShotType);
		CurrentShotType = 0;
		shootState = SHOOT_STATE_NONE;
		Wireless_Debug("Shot Complete\r\n");
	}

	/* Clear the interrupt such that it is no longer pending in the GPIO */
	(void)XGpio_InterruptClear(GpioPtr, XGPIO_IR_CH2_MASK);
}
