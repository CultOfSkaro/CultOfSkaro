#include "Interrupts.h"
#include "xintc.h"
#include "xil_exception.h"
#include "InterruptControl.h"
#include "xtime_l.h"

//#include "SystemTypes.h"
#include "xuartlite.h"
#include "xuartlite_l.h"

//====================================================

#include "HeliosIO.h"
#include "skaro_wireless.h"
#include "Navigation.h"
#include "Serial.h"

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

	XIntc_Start(&InterruptController, XIN_REAL_MODE);

	// Enable interrupts for serial controllers
	XIntc_Enable(&InterruptController, XPAR_XPS_INTC_0_WIRELESS_UART_INTERRUPT_INTR);
	XIntc_Enable(&InterruptController, XPAR_XPS_INTC_0_GAMEBOARD_UART_INTERRUPT_INTR);

	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XIntc_InterruptHandler,
			&InterruptController);

	Xil_ExceptionEnable();

	// Set up send/receive handlers for wireless uart
	XUartLite_SetSendHandler(&(wireless.uart), WirelessSendHandler, &(wireless.uart));
	XUartLite_SetRecvHandler(&(wireless.uart), WirelessRecvHandler, &(wireless.uart));

	// Set up send/receive handlers for gameboard uart
	XUartLite_SetSendHandler(&gameboard_uart, GameboardSendHandler, &gameboard_uart);
	XUartLite_SetRecvHandler(&gameboard_uart, GameboardRecvHandler, &gameboard_uart);

	XUartLite_EnableInterrupt(&(wireless.uart));
	XUartLite_EnableInterrupt(&gameboard_uart);

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
