/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:	 HeliosIO.c
AUTHOR:	 Wade Fife
CREATED: 11/7/05

DESCRIPTION

This module contains code to use the user button, two DIP switches, and LED on
the Helios board, rev. 1A. It assumes use of a the OPB GPIO core (v3.01.b) and
the gpio driver (v2.00.a). It also assumes the following connections to the
GPIO core:

  NET<0> = GPIO<31> - User button (LSB)
  NET<1> = GPIO<30> - Switch 1
  NET<2> = GPIO<29> - Switch 2
  NET<3> = GPIO<28> - LED 1
  NET<4> = GPIO<27> - LED 2
  NET<5> = GPIO<26> - SRAM sleep
  NET<6> = GPIO<25> - Flash sleep (MSB)

  * GPIO refers to the register read/written from the GPIO device via software
    and NET refers to the net conntected to the GPIO_IO port on the GPIO core.

Note that NET is assumed to be defined as VEC 4:0 in the MHS file, which causes
its lease significant bit (LSB) to be NET<0> whereas on the PPC the LSB of a
32-bit signal is generally considered to be bit 31. Unfortunately this leads to
a lot of confusion so great care must be excercised. To work properly the LED
bits should be set as output only (i.e., the GPIO_TRI register should be
initialized to 0xFFFFFFE7).

This code also assumes GPIO driver 2.00.a.

CHANGE LOG

11/07/05 WSF Created original file.
12/02/05 WSF Added code to control sleep pins on SRAM and flash.
3/36/10 SFG Added game board IO functions

******************************************************************************/


#include "HeliosIO.h"

#include "Timer.h"
#include "InterruptControl.h"
#include "ServoControl.h"

Xuint32 oldBits;

// FUNCTIONS //////////////////////////////////////////////////////////////////


#ifdef HELIOS_USE_BTN /////////////////////////////////////////////////////////

// Read user button
// Returns 0 or 1, 1 means the button is depressed.
uint8 HeliosReadBTN(void)
{
	return (uint8)(
		XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET) & 0x1
		);
}

#endif



#ifdef HELIOS_USE_SW1 /////////////////////////////////////////////////////////

// Read DIP switch 1
// Returns 0 or 1, 1 means the switch is in the ON position.
uint8 HeliosReadSW1(void)
{
	return (uint8)(
		(XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET) >> 1) & 0x1
		);
}

#endif



#ifdef HELIOS_USE_SW2 /////////////////////////////////////////////////////////

// Read DIP switch 2
// Returns 0 or 1, 1 means the switch is in the ON position.
uint8 HeliosReadSW2(void)
{
	return (uint8)(
		(XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET) >> 2) & 0x1	);
}

#endif



#ifdef HELIOS_USE_LED1 ////////////////////////////////////////////////////////

// Read current state of LED1
// Returns 0 or 1, 1 means the LED is ON.
uint8 HeliosReadLED1(void)
{
	return (uint8)(
		(XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET) >> 3) & 0x1
		);
}


// Set user LED1 state
// Set to 0 for OFF, 1 for ON.
void HeliosSetLED1(uint8 value)
{
	uint32 readValue;

	readValue = XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET);
	readValue &= ~(0x00000008);		   // Clear the LED bit
	readValue |= (value & 0x1) << 3;   // Set LED bit
	XGpio_WriteReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET, readValue);
}

#endif



#ifdef HELIOS_USE_LED2 ////////////////////////////////////////////////////////

// Read current state of LED2
// Returns 0 or 1, 1 means the LED is ON.
uint8 HeliosReadLED2(void)
{
	return (uint8)(
		(XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET) >> 4) & 0x1
		);
}


// Set user LED2 state
// Set to 0 for OFF, 1 for ON.
void HeliosSetLED2(uint8 value)
{
	uint32 readValue;

	readValue = XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET);
	readValue &= ~(0x00000010);		   // Clear the LED bit
	readValue |= (value & 0x1) << 4;   // Set LED bit
	XGpio_WriteReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET, readValue);
	//xil_printf("GPIO1: %x", readValue);
}

#endif



#ifdef HELIOS_USE_SLEEP_SRAM //////////////////////////////////////////////////

// Sets the sleep state of the SRAM chip (i.e., the ZZ pin). A state
// argument of 1 puts the SRAM into sleep mode. A state argument of 0
// makes the SRAM operational.
//
// NOTE: Test show that putting the SRAM into sleep mode saves about
// 14.7 mW of power with a 5V board input.
void HeliosSetSleepSRAM(uint8 state)
{
	uint32 readValue;

	readValue = XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET);
	readValue &= ~(0x00000020);		   // Clear the ZZ bit
	readValue |= (state & 0x1) << 5;   // Set SRAM ZZ signal
	XGpio_WriteReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET, readValue);	
}



// Gets the current sleep state of the SRAM. Returns 1 if the SRAM is
// in sleep mode. Returns 0 if it is in operating mode.
uint8 HeliosGetSleepSRAM(void)
{
	uint32 readValue;

	readValue = XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET);
	readValue = (readValue >> 5) & 1;

	return readValue;
}

#endif



#ifdef HELIOS_USE_SLEEP_FLASH /////////////////////////////////////////////////

// Sets the sleep state of the flash chip (i.e., the RP# pin). A state
// argument of 1 puts the flash into deep power-down mode. A state
// argument of 0 makes the flash operational.
//
// NOTE: The flash, a Micron Q-Flash, already has such a low idle
// current that the deep-power down mode typically has a negligable
// effect on the overall power consumption.
void HeliosSetSleepFlash(uint8 state)
{
	uint32 readValue;

	readValue = XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET);
	readValue &= ~(0x00000040);		    // Clear the RP# bit
	readValue |= ((~state) & 0x1) << 6; // Set flash RP# signal
	XGpio_WriteReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET, readValue);	
}



// Gets the current sleep state of the flash. Returns 1 if the flash
// is in deep power-down mode. Returns 0 if it is in operating mode.
uint8 HeliosGetSleepFlash(void)
{
	uint32 readValue;

	readValue = XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA_OFFSET);
	readValue = ((~readValue) >> 6) & 1;

	return readValue;
}

#endif

#ifdef HELIOS_USE_GAME_SYSTEM
/* GLOBAL MEMORY */
XGpio Gpio;

/* FUNCTIONS */
/* Initializes the game system.  Interrupts should have already been initialized in INIT_ISR.  This function
 * initializes the GPIO high-level interface (using the GPio struct), sets the data direction for the game-system GPIO pins
 * (Game system GPIO pins are on channel 2 of the GPIO core, while Helios LEDs, Switches, and Button are on channel 1),
 * and enables GPIO interrupts in the GPIO core (only on channel 2).
 * This function must be called before interrupts will work from the game system.
 */
XStatus InitGameSystem(void)
{
	XStatus Status;
	//Interrupts were already initialized in ISR.c (INIT_ISR())
	xil_printf("Initializing GPIO...");
	Status = XGpio_Initialize(&Gpio, GPIO_DEVICE_ID);
   if (Status != XST_SUCCESS)
   {
		return XST_FAILURE;
   }
	XGpio_SetDataDirection(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, 0xFFFFFF3F);
	XGpio_SetDataDirection(&Gpio, 1, 0xFFFFFFE7);
	xil_printf("Enabling interrupts...");
	XGpio_InterruptEnable(&Gpio, XGPIO_IR_CH2_MASK);
   XGpio_InterruptGlobalEnable(&Gpio);
	xil_printf("Done!\r\n");
	return XST_SUCCESS;
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
void  Game_Shoot(uint8 shotType)
{
	if(Game_Enabled() && !Game_WaitingToShoot())
	{
		xil_printf("Shooting...%x\r\n", shotType);
		XGpio_DiscreteSet(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, shotType);
		XGpio_DiscreteClear(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, (~shotType) & GAME_SHOT_TYPE_M);
		XGpio_DiscreteSet(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, GAME_SHOOT_M);
		ClockDelay(200000);
		XGpio_DiscreteClear(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, GAME_SHOOT_M);
	}
	else if(Game_WaitingToShoot())
		xil_printf("Cannot shoot, still waiting...\r\n");
	else if(!Game_Enabled())
		xil_printf("Cannot shoot, game is not enabled!");
	xil_printf("Game state: %x\r\n", XGpio_DiscreteRead(&Gpio, GAME_SYSTEM_GPIO_CHANNEL));
}
/* Returns the current state of the truck.  The enabled bit is set automatically for you by the GPIO ISR.
 * This function simply returns the value set by the ISR.
 * 1 - Truck is enabled
 * 0 - Truck is disabled
 */
uint8 Game_Enabled(void)
{
	//return GAMEENABLED;
	//xil_printf("GE: %x\r\n", XGpio_DiscreteRead(&Gpio, GAME_SYSTEM_GPIO_CHANNEL));
	return (XGpio_DiscreteRead(&Gpio, GAME_SYSTEM_GPIO_CHANNEL) & GAME_TRUCK_ALIVE_M) >> 4;
}
/* Returns whether or not this truck has the flag.  The flag bit is set automatically for you by the GPIO ISR.
 * This function simply returns the value set by the ISR.
 * 1 - Truck has flag
 * 0 - Truck does not have flag
 */
uint8 Game_HaveFlag(void)
{
	//return GAMEFLAG;
	return (XGpio_DiscreteRead(&Gpio, GAME_SYSTEM_GPIO_CHANNEL) & GAME_HAVE_FLAG_M) >> 5;
}

uint8 Game_TeamNumber(void){
	return (XGpio_DiscreteRead(&Gpio, GAME_SYSTEM_GPIO_CHANNEL) & 0x0003);
}

uint8 Game_GameState(void){
	return (XGpio_DiscreteRead(&Gpio, GAME_SYSTEM_GPIO_CHANNEL) & GAME_STATE_M) >> 2;
}
/* This is the function called by the GPIO ISR if you receive a HIT acknowledgement after firing a shot.
 * Put whatever code you like here (play a cool sound, etc...).
 * A HIT is understood as receiving a HIT signal while wait-to-shoot is still high (from the game board).
 * Therefore, this function is called while wait-to-shoot is still high, but wait-to-shoot will go down
 * shortly after this function is called.
 */
void  Game_Hit(void)
{
	xil_printf("HIT!\r\n");
	//What to do if you hit a truck
}
/* This is the function called by the GPIO ISR if you receive a MISS acknowledgement after firing a shot.
 * Put whatever code you like here (play a cool sound, etc...).
 * A MISS is understood as receiving a MISS signal while wait-to-shoot is still high (from the game board).
 * Therefore, this function is called while wait-to-shoot is still high, but wait-to-shoot will go down
 * shortly after this function is called.
 */
void  Game_Miss(void)
{
	xil_printf("MISS!\r\n");
	//What to do if you missed a truck
}
/* Returns whether or not this truck is still waiting to shoot.  The wait to shoot bit is set automatically 
 * for you by the GPIO ISR.
 * This function simply returns the value set by the ISR.
 * 1 - Waiting to shoot
 * 0 - Ready to shoot
 */ 
uint8  Game_WaitingToShoot(void)
{
	//return GAMEWTS;
	return 0;//!(XGpio_DiscreteRead(&Gpio, GAME_SYSTEM_GPIO_CHANNEL) & GAME_READY_TO_SHOOT_M);
}
/* This is the function called by the GPIO ISR if you receive a GAME PAUSED signal.
 * Put whatever code you need here to properly implement game protocol for a game being paused.
 * A GAME PAUSED is understood as receiving a HIT signal while wait-to-shoot is LOW (from the game board).
 */
void Game_Paused(void)
{
	//what to do if the game is paused
	xil_printf("Game paused!\r\n");
}
/* This is the function called by the GPIO ISR if you receive a FLAG CAPTURED (GAME OVER) signal.
 * Put whatever code you need here to properly implement game protocol for a game ending (return to base, etc..)
 * A FLAG CAPTURED is understood as receiving a MISS signal while wait-to-shoot is LOW (from the game board).
 */
void Game_FlagCaptured(void)
{
	xil_printf("Flag captured, GAME OVER!\r\n");
	//what to do if a flag was captured (game over/reset)
}
/* This is the function called by the GPIO ISR if you receive a GAME UN-PAUSED signal.
 * Put whatever code you need here to properly implement game protocol for a game becoming un-paused (GAME START).
 * A GAME UN-PAUSED is understood as a HIT signal going LOW while wait-to-shoot is LOW (from the game board).
 */
void Game_UnPaused(void)
{
	xil_printf("Game un-paused!\r\n");
	//what to do if the game was just un-paused
}



#endif
