/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    HeliosIO.h
AUTHOR:  Wade Fife
CREATED: 11/07/05

DESCRIPTION

Header file for HeliosIO.c

CHANGE LOG

11/07/05 WSF  Created original file.
12/02/05 WSF  Added code to control sleep pins on SRAM and flash.
3/36/10 SFG Added game board IO functions

******************************************************************************/

#ifndef HELIOS_IO_H
#define HELIOS_IO_H

#include "Header.h"
//#include "SystemTypes.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xexception_l.h"


// USER CONFIGURATION /////////////////////////////////////////////////////////

// Baseaddress of GPIO module for board I/O
#include "xparameters.h"
#define HELIOS_IO_BASEADDR    XPAR_XPS_GPIO_1_BASEADDR

// Define the components for which you would like to include code for
// compilation.
#define HELIOS_USE_BTN             // Code to read user button
#define HELIOS_USE_SW1             // Code to read DIP SW1
#define HELIOS_USE_SW2             // Code to read DIP SW2
#define HELIOS_USE_LED1            // Code to read/write user LED1
#define HELIOS_USE_LED2            // Code to read/write user LED2
#undef HELIOS_USE_SLEEP_SRAM      // Code to sleep/awake SRAM
#undef HELIOS_USE_SLEEP_FLASH     // Code to sleep/awake FLASH


#define HELIOS_USE_GAME_SYSTEM    // Functions for using the game board GPIO interface
	#define GPIO_DEVICE_ID				XPAR_XPS_GPIO_1_DEVICE_ID
	#define INTC_DEVICE_ID				XPAR_XPS_INTC_0_DEVICE_ID
	#define INTC_GPIO_INTERRUPT_ID	XPAR_XPS_INTC_0_XPS_GPIO_0_IP2INTC_IRPT_INTR
	#define GAME_SYSTEM_GPIO_CHANNEL	2
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

///////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES ////////////////////////////////////////////////////////
#define Game_ClearShots() \
	XGpio_DiscreteClear(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, GAME_SHOOT_M);

uint8  HeliosReadBTN(void);
uint8  HeliosReadSW1(void);
uint8  HeliosReadSW2(void);
uint8  HeliosReadLED1(void);
void   HeliosSetLED1(uint8 value);
uint8  HeliosReadLED2(void);
void   HeliosSetLED2(uint8 value);
void   HeliosSetSleepSRAM(uint8 state);
uint8  HeliosGetSleepSRAM(void);
void   HeliosSetSleepFlash(uint8 state);
uint8  HeliosGetSleepFlash(void);
void   HeliosEnableGyro(void);
// Game board function prototypes
void   Game_State();
void   Game_Shoot(uint16 shotType);
uint8  Game_Enabled(void);
uint8  Game_Truck_Alive(void);
uint8  Game_HaveFlag(void);
void  Game_Hit(void);
void  Game_Miss(void);
uint8 Game_WaitingToShoot(void);
void Game_Paused();
void Game_UnPaused();
void Game_FlagCaptured();
XStatus InitGameSystem(void);
uint8 Game_TeamNumber(void);
uint8 Game_GameState(void);
void ISR_GPIO(void);

#endif
