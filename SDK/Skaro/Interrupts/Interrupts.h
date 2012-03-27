#ifndef SKARO_INTERRUPTS
#define INTERRUPTS

#include "skaro_wireless.h"
#include "xintc.h"
#include "xil_exception.h"

#define SET_PID_KP   		0x1
#define SET_PID_KI   		0x2
#define SET_PID_KD   		0x3
#define SET_PID_KP_C 		0x4
#define SET_PID_KI_C 		0x5
#define SET_PID_KD_C 		0x6
#define REQUEST_CONTROL_LOG 0x7
#define SET_DISTANCE		0x8
#define SET_STEERING		0x9
#define SET_MAX_VELOCITY	0xa
#define SET_VISION_DISTANCE	0xb
#define SET_20_CIRCLE1		0xc
#define SET_OVERLAP			0xd
#define SET_25_CIRCLE2		0xe
#define SET_20_CIRCLE2		0xf
#define SHOOT_GAME_SHOOT_KILL	0x10

void InitInterrupts();
void WirelessSendHandler(void *CallBackRef, unsigned int EventData);
void GameboardSendHandler(void *CallBackRef, unsigned int EventData);
// These are in main... because....
void my_pitHandler();
void WirelessRecvHandler(void *CallBackRef, unsigned int EventData);
void WirelessRecvHandlerNonBlocking(void *CallBackRef, unsigned int EventData);
void GameboardRecvHandler(void *CallBackRef, unsigned int EventData);
void GpioHandler();

extern Skaro_Wireless wireless;

extern XUartLite gameboard_uart;

extern XIntc InterruptController;

#endif
