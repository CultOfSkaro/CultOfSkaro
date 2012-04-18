#include "Interrupts.h"
#include "xintc.h"
#include "xil_exception.h"
#include "InterruptControl.h"
#include "xtime_l.h"
#include "xexception_l.h"

//#include "SystemTypes.h"
#include "xuartlite.h"
#include "xuartlite_l.h"

#include "xgpio.h"
#include "plb_quad_encoder.h"
//====================================================

#include "GameBoard.h"
#include "skaro_wireless.h"
#include "Navigation.h"
#include "Serial.h"
#include "Timer.h"

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

	XIntc_Connect(&InterruptController, XPAR_INTC_0_GPIO_0_VEC_ID,
			      (Xil_ExceptionHandler)GpioHandler, &Gpio);

	XIntc_Start(&InterruptController, XIN_REAL_MODE);

	// Enable interrupts for serial controllers
	XIntc_Enable(&InterruptController, XPAR_XPS_INTC_0_WIRELESS_UART_INTERRUPT_INTR);
	XIntc_Enable(&InterruptController, XPAR_XPS_INTC_0_GAMEBOARD_UART_INTERRUPT_INTR);

	// Enable interrupts for the GPIO
	XIntc_Enable(&InterruptController, XPAR_INTC_0_GPIO_0_VEC_ID);

	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XIntc_InterruptHandler,
			&InterruptController);

	Xil_ExceptionEnable();

	// Set up send/receive handlers for wireless uart
	XUartLite_SetSendHandler(&(wireless.uart), WirelessSendHandler, &(wireless.uart));
	XUartLite_SetRecvHandler(&(wireless.uart), WirelessRecvHandler, &(wireless.uart));

	//XUartLite_SetRecvHandler(&(wireless.uart), WirelessRecvHandlerNonBlocking, &(wireless.uart));
	Wireless_Debug("Wireless should be set up now\r\n");

	// Set up send/receive handlers for gameboard uart
	XUartLite_SetSendHandler(&gameboard_uart, GameboardSendHandler, &gameboard_uart);
	XUartLite_SetRecvHandler(&gameboard_uart, GameboardRecvHandler, &gameboard_uart);

	XUartLite_EnableInterrupt(&(wireless.uart));
	XUartLite_EnableInterrupt(&gameboard_uart);

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
		c = (unsigned char)(int)QueuePop(&(wireless.write_queue));
		XUartLite_Send(&wireless.uart, &c, 1);
	}
}

// If this gets called, something is wrong.  Ask Dan.
void GameboardSendHandler(void *CallBackRef, unsigned int EventData)
{
	Wireless_Debug("GameboardSendHandler is a stub function and should never get called\r\n");
}

// Interrupt handler for gameboard UART
void GameboardRecvHandler(void *CallBackRef, unsigned int EventData)
{
//	static short ticks_prev = 0;
//		static short ticks = 0;
//		static int clocks = 0;
//		static int clocks_prev = 0;
//		static short gyro_moving_window[GYRO_MOVING_WINDOW_SIZE];
//	    static int total_angle;
//		float rate;
//	    int i;
//	    int stale_count;
//		CPU_MSR msr;
//
//
//		// Non-blocking receive of 5 bytes
//		XUartLite_Recv(&gameboard_uart, raw_gyro_data.packet, 5);
//
//		if (EventData == 0) {
//	        /* This condition happens upon the first receive interrupt that we get
//	           At this point we haven't requested a receive (XUartLite_Recv())
//	        */
//
//	        // Get an initial time and tick value for the first velocity calculation
//			msr = DISABLE_INTERRUPTS();
//			ticks_prev = getTicks();
//			clocks_prev = ClockTime();
//			RESTORE_INTERRUPTS(msr);
//
//			return;
//		} else if (EventData == 5) {
//			/* 5 bytes receive
//			   First interrupt from a requested receive (XUartLite_Recv())
//			*/
//
//			if (raw_gyro_data.packet[0] == 'M') {
//	            // Get ticks and clock time simultaneously to calculate velocity
//				msr = DISABLE_INTERRUPTS();
//				ticks = getTicks();
//				clocks = ClockTime();
//				RESTORE_INTERRUPTS(msr);
//
//	            // Get the refresh rate
//				rate = refresh_rate(clocks, clocks_prev);
//
//	            // Calculate velocity
//				raw_gyro_data.velocity = ((float)(ticks - ticks_prev))/rate;
//
//	            // Extract angular velocity from gyro data
//				raw_gyro_data.angular_velocity = (signed char)raw_gyro_data.packet[3];
//				raw_gyro_data.angular_velocity <<= 8;
//				raw_gyro_data.angular_velocity |= (uint32) raw_gyro_data.packet[4];
//
//	            // Move current ticks and clocks to previous
//				ticks_prev = ticks;
//				clocks_prev = clocks;
//
//	            // Shift the moving window
//	            for (i = 0; i < GYRO_MOVING_WINDOW_SIZE - 1; i++) {
//	                gyro_moving_window[i] = gyro_moving_window[i + 1];
//	            }
//	            gyro_moving_window[GYRO_MOVING_WINDOW_SIZE - 1] = raw_gyro_data.angular_velocity;
//
//	            // Trapezoidal integration of gyro samples
//	            // This measurement is in increments of 8.75 milidegrees
//	            raw_gyro_data.total_angle += (raw_gyro_data.angular_velocity + gyro_moving_window[GYRO_MOVING_WINDOW_SIZE - 2]) / GYRO_INTEGRATION_DIVISOR;
//
////	            Wireless_Debug("Raw total angle");
////	            PrintInt((raw_gyro_data.total_angle * 8.75) / 1000);
////	            Wireless_Debug("\r\n");
//
//	            // Check for stale data, this usually happens when the battery gets low
//	            for (i = 0; i < GYRO_MOVING_WINDOW_SIZE - 1; i++) {
//	                if (gyro_moving_window[i] != gyro_moving_window[i + 1]) {
//	                    break;
//	                }
//				}
//	            // The entire moving window is the same value, gyro data is stale
//	            if (i == GYRO_MOVING_WINDOW_SIZE - 1) {
//					Wireless_Debug("--------------------------\r\n");
//					Wireless_Debug("Gyro: getting stale data!\r\n");
//					Wireless_Debug("--------------------------\r\n");
//	            }
//
//			} else {
//	            // The beginning of the packet was not 'M'
//				Wireless_Debug("----------------------------------------------------------\r\n");
//				Wireless_Debug("Gyro: packets out of sync, try resetting the Helios board\r\n");
//				Wireless_Debug("----------------------------------------------------------\r\n");
//			}
//		}
}
