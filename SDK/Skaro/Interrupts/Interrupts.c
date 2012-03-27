#include "Interrupts.h"
#include "xintc.h"
#include "xil_exception.h"
#include "InterruptControl.h"
#include "xtime_l.h"

//#include "SystemTypes.h"
#include "xuartlite.h"
#include "xuartlite_l.h"

// GPIO
#include "xgpio.h"

//====================================================

#include "HeliosIO.h"
#include "skaro_wireless.h"
#include "Navigation.h"
#include "Serial.h"

extern XGpio Gpio;

void InitInterrupts() {
	// Initialize wireless uart
	XUartLite_Initialize(&(wireless.uart), XPAR_WIRELESS_UART_DEVICE_ID);
	XUartLite_ResetFifos(&(wireless.uart));
	Wireless_Init(&wireless);

	// Initialize gameboard uart
	XUartLite_Initialize(&gameboard_uart, XPAR_GAMEBOARD_UART_DEVICE_ID);
	XUartLite_ResetFifos(&gameboard_uart);

	// Initialize the interrupt controller
	XIntc_Initialize(&InterruptController, XPAR_INTC_0_DEVICE_ID);

	// Connect the wireless uart to the interrupt controller
	XIntc_Connect(&InterruptController, XPAR_XPS_INTC_0_WIRELESS_UART_INTERRUPT_INTR,
			(XInterruptHandler)XUartLite_InterruptHandler,
			(void *)&(wireless.uart));

	// Connect the gameboard uart to the interrupt controller
	XIntc_Connect(&InterruptController, XPAR_XPS_INTC_0_GAMEBOARD_UART_INTERRUPT_INTR,
			(XInterruptHandler)XUartLite_InterruptHandler,
			(void *)&gameboard_uart);

	//XIntc_Connect(&InterruptController, XPAR_INTC_0_GPIO_0_VEC_ID,
	//		      (Xil_ExceptionHandler)GpioHandler, &Gpio);

	XIntc_Start(&InterruptController, XIN_REAL_MODE);

	// Enable interrupts for serial controllers
	XIntc_Enable(&InterruptController, XPAR_XPS_INTC_0_WIRELESS_UART_INTERRUPT_INTR);
	XIntc_Enable(&InterruptController, XPAR_XPS_INTC_0_GAMEBOARD_UART_INTERRUPT_INTR);

	// Enable interrupts for the GPIO
	//XIntc_Enable(&InterruptController, XPAR_INTC_0_GPIO_0_VEC_ID);

	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XIntc_InterruptHandler,
			&InterruptController);

	Xil_ExceptionEnable();

	// Set up send/receive handlers for wireless uart
	XUartLite_SetSendHandler(&(wireless.uart), WirelessSendHandler, &(wireless.uart));
	XUartLite_SetRecvHandler(&(wireless.uart), WirelessRecvHandler, &(wireless.uart));

	//XUartLite_SetRecvHandler(&(wireless.uart), WirelessRecvHandlerNonBlocking, &(wireless.uart));
	Wireless_Debug("Wireless should be set up now");

	// Set up send/receive handlers for gameboard uart
	XUartLite_SetSendHandler(&gameboard_uart, GameboardSendHandler, &gameboard_uart);
	XUartLite_SetRecvHandler(&gameboard_uart, GameboardRecvHandler, &gameboard_uart);

	XUartLite_EnableInterrupt(&(wireless.uart));
	XUartLite_EnableInterrupt(&gameboard_uart);

	// Enable interrupts for GPIO
	//XGpio_InterruptEnable(&Gpio, XGPIO_IR_CH2_MASK);
	//XGpio_InterruptGlobalEnable(&Gpio);

	// Set up PIT
	XExceptionHandler pithandler = &my_pitHandler;
	XExc_RegisterHandler(XEXC_ID_PIT_INT, pithandler,
			0);
	XTime_PITEnableAutoReload();
	// PIT should be set to 1ms
	XTime_PITSetInterval(300000);
	XExc_mEnableExceptions(XEXC_ALL);
	XTime_PITEnableInterrupt();
	XTime_PITClearInterrupt();
}

void WirelessSendHandler(void *CallBackRef, unsigned int EventData)
{
	unsigned char c;
	wireless.send_in_progress = 0;
	if(!QueueIsEmpty(&(wireless.write_queue))) {
		wireless.send_in_progress = 1;
		c = QueuePop(&(wireless.write_queue));
		XUartLite_Send(&wireless.uart, &c, 1);
	}
}

// If this gets called, something is wrong.  Ask Dan.
void GameboardSendHandler(void *CallBackRef, unsigned int EventData)
{
	Wireless_Debug("This should never get called\r\n");
}

// Interrupt handler for gameboard UART
void GameboardRecvHandler(void *CallBackRef, unsigned int EventData)
{

	XUartLite_Recv(&gameboard_uart, raw_gyro_data.packet, 5);

	if (EventData == 5) {
		raw_gyro_data.velocity = (signed char)raw_gyro_data.packet[1];
		raw_gyro_data.velocity <<= 8;
		raw_gyro_data.velocity |= (uint32) raw_gyro_data.packet[2];

		raw_gyro_data.angular_velocity = (signed char)raw_gyro_data.packet[3];
		raw_gyro_data.angular_velocity <<= 8;
		raw_gyro_data.angular_velocity |= (uint32) raw_gyro_data.packet[4];
	}
}

void GpioHandler(void *InstancePtr)
{
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

	XGpio *GpioPtr = (XGpio *)InstancePtr;
	u32 gameSystem;
	//u32 gameSystemChanged = 0;
	static u32 gameSystemPrevious = 0;

	/*
	 * Disable the interrupt
	 */
	XGpio_InterruptDisable(GpioPtr, XGPIO_IR_CH2_MASK);

	/*
	 * There should not be any other interrupts occuring other than the
	 * the button changes
	 */
	if ((XGpio_InterruptGetStatus(GpioPtr) & XGPIO_IR_CH2_MASK) !=
			XGPIO_IR_CH2_MASK) {
		return;
	}

	/*
	 * Read state of push buttons and determine which ones changed
	 * states from the previous interrupt. Save a copy of the buttons
	 * for the next interrupt
	 */
	gameSystem = XGpio_DiscreteRead(GpioPtr, GAME_SYSTEM_GPIO_CHANNEL);
	//gameSystemChanged = gameSystem ^ gameSystemPrevious;


	if (((~gameSystemPrevious) & GAME_WAIT_TO_SHOOT) && (gameSystem & GAME_WAIT_TO_SHOOT)) {
		// Wait to shoot went from low to high
		XGpio_DiscreteClear(&Gpio, GAME_SYSTEM_GPIO_CHANNEL, CurrentShotType);
		CurrentShotType = 0;
	}

	/* Clear the interrupt such that it is no longer pending in the GPIO */

	(void)XGpio_InterruptClear(GpioPtr, XGPIO_IR_CH2_MASK);

	/*
	 * Enable the interrupt
	 */

	gameSystemPrevious = gameSystem;
	XGpio_InterruptEnable(GpioPtr, XGPIO_IR_CH2_MASK);
}
