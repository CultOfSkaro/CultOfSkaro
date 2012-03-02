/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    Wireless.c
AUTHOR:  Wade Fife
CREATED: 12/10/04

DESCRIPTION

This module contains code to handle sending and receiving to wireless
port. It consists of send and receive queues for buffering data as
well as the functions for enqueing data to be sent and dequeuing data
that has been received. The actual transfer of data to and from the
serial port is interrupt driven.

CHANGE LOG

02/04/06 WSF Updated to be uC/OS independent.

******************************************************************************/


// INCLUDES ///////////////////////////////////////////////////////////////////

#include <xgpio_l.h>
#include <xuartlite_l.h>
#include <xparameters.h>

#include "Header.h"
#include "Wireless.h"
#include "Serial.h"
#include "InterruptControl.h"



// DEFINES ////////////////////////////////////////////////////////////////////

#define RECV_BUFFER_SIZE 256
#define SEND_BUFFER_SIZE 256



// MACROS /////////////////////////////////////////////////////////////////////

#define RECV_ENQUEUE(DATA) recvBuffer.data[recvBuffer.tail++] = (DATA); \
                           recvBuffer.size++; \
		                   if(recvBuffer.tail >= RECV_BUFFER_SIZE) \
                               recvBuffer.tail = 0;

#define SEND_ENQUEUE(DATA) sendBuffer.data[sendBuffer.tail++] = (DATA); \
		                   sendBuffer.size++; \
		                   if(sendBuffer.tail >= SEND_BUFFER_SIZE) \
                               sendBuffer.tail = 0;

#define RECV_DEQUEUE() recvBuffer.data[recvBuffer.head++]; \
		               recvBuffer.size--; \
		               if(recvBuffer.head >= RECV_BUFFER_SIZE) \
                           recvBuffer.head = 0;

#define SEND_DEQUEUE() sendBuffer.data[sendBuffer.head++]; \
		               sendBuffer.size--; \
		               if(sendBuffer.head >= SEND_BUFFER_SIZE) \
                           sendBuffer.head = 0;



// DATA TYPES /////////////////////////////////////////////////////////////////

typedef struct {
	int head;
	int tail;
	int size;
	uint8 *data;
} Queue;



// STATIC GLOBALS /////////////////////////////////////////////////////////////

static uint8 recvBufferData[RECV_BUFFER_SIZE];
static uint8 sendBufferData[SEND_BUFFER_SIZE];
static Queue sendBuffer;
static Queue recvBuffer;



// WIRELESS INTERRUPT HANDLER /////////////////////////////////////////////////

// IMPORTANT NOTE ON MODEM'S CTS SIGNAL: If CTS is not asserted (i.e., if the
// modem's send FIFO is full) the interrupt handler can't transfer the next
// byte and must return. Also, since no byte is queued in the UARTS TX FIFO,
// the interrupt may not be asserted again, which means the transfer mechanism
// may stop functioning.
//
// To remedy this situation, the WirelessInterruptHandler() must be called
// manually. An easy solution is to call WirelessInterruptHandler() in a
// periodic timer handler (i.e., the system timer interrupt).
//
// Also, if the wrireless handler is called at regular intervals (e.g., by a
// timer handler) then it becomes unnecessary to use the wireless handler as an
// interrupt handler for the UART, unless a quick response time is desired.

void WirelessInterruptHandler(void)
{
	uint8 byte;
	CPU_MSR msr;

	msr = DISABLE_INTERRUPTS();

	// Check if receive buffer has room and check if data has been received
	while(!XUartLite_mIsReceiveEmpty(WIRELESS_BASEADDR)
		  && recvBuffer.size < RECV_BUFFER_SIZE
		  ) {
		// Get byte from serial port and put it in receive buffer
		RECV_ENQUEUE(XUartLite_RecvByte(WIRELESS_BASEADDR));
	}

#ifdef CHECK_MODEM_CTS
	if(MODEM_CTS())
#endif
		// Check for data to send and check if transmit buffer has room
		while(sendBuffer.size > 0 &&
			  !XUartLite_mIsTransmitFull(WIRELESS_BASEADDR)) {
			
			// Get byte from send buffer and send it to serial port
			byte = SEND_DEQUEUE();
			XUartLite_SendByte(WIRELESS_BASEADDR, byte);
		}
	
	RESTORE_INTERRUPTS(msr);
}





// WIRLESS I/O CODE ///////////////////////////////////////////////////////////


// Setup hardware and software for interrupt based wireless transfer.
void WirelessInit(void)
{
	// Initialize data structures
	sendBuffer.data = sendBufferData;
	sendBuffer.size = 0;
	sendBuffer.head = 0;
	sendBuffer.tail = 0;

	recvBuffer.data = recvBufferData;
	recvBuffer.size = 0;
	recvBuffer.head = 0;
	recvBuffer.tail = 0;

	// Setup wireless handler
	XUartLite_mEnableIntr(WIRELESS_BASEADDR);
}



#ifdef USE_WIRE_SEND_DATA ////////////////////////

// Buffers the indicated number of bytes for transfer over wireless erial port.
// Returns 1 if successfull, 0 otherwise.
int WireSendData(uint8 *data, int numBytes)
{
	CPU_MSR  msr;
	uint8 byte;

	msr = DISABLE_INTERRUPTS();

	// Check for buffer overflow
	if(sendBuffer.size + numBytes > SEND_BUFFER_SIZE) {
		RESTORE_INTERRUPTS(msr);
		return 0;
	}
	
	// Adjust buffer to indicate new size
	sendBuffer.size += numBytes;

	// Queue up data
	while(numBytes--) {
		sendBuffer.data[sendBuffer.tail++] = *data++;
		if(sendBuffer.tail >= SEND_BUFFER_SIZE) sendBuffer.tail = 0;
	}

	// Check if hardware TX FIFO is empty
#ifdef CHECK_MODEM_CTS
	if(MODEM_CTS())
#endif
		if(XUartLite_mGetStatusReg(WIRELESS_BASEADDR) | 
		   XUL_SR_TX_FIFO_EMPTY) {
			// Start transfer of first byte; rest will be sent by int handler.
			byte = SEND_DEQUEUE();
			XUartLite_SendByte(WIRELESS_BASEADDR, byte);
		}

	RESTORE_INTERRUPTS(msr);

	return 1;
}

#endif



#ifdef USE_WIRE_RECV_DATA ////////////////////////

// Gets the indicated number of bytes from the serial buffer.
// Returns 1 if successfull, 0 otherwise.
int WireRecvData(uint8 *dest, int numBytes)
{
	CPU_MSR msr;

	msr = DISABLE_INTERRUPTS();

	// Check for buffer underflow
	if(recvBuffer.size < numBytes) {
		RESTORE_INTERRUPTS(msr);
		return 0;
	}

	recvBuffer.size -= numBytes;

	// Queue up data
	while(numBytes--) {
		*dest++ = recvBuffer.data[recvBuffer.head++];
		if(recvBuffer.head >= RECV_BUFFER_SIZE) recvBuffer.head = 0;
	}

	RESTORE_INTERRUPTS(msr);

	return 1;
}

#endif



#ifdef USE_WIRE_SEND /////////////////////////////

// Send a byte to the wireless link's UART
void WireSend(uint8 byte)
{
	while(!WireSendData(&byte, 1));
}

#endif



#ifdef USE_WIRE_GETCH ////////////////////////////

// Like getch() for the wireless port
uint8 WireGetch(void)
{
	uint8 returnValue;

	while(!WireRecvData(&returnValue, 1));

	return returnValue;
}

#endif



#ifdef USE_WIRE_DATA_READY ///////////////////////

// Like kbhit(), but returns number of bytes ready to be read.
int WireDataReady(void)
{
	CPU_MSR msr;
	int returnValue;

	msr = DISABLE_INTERRUPTS();
	returnValue = recvBuffer.size;
	RESTORE_INTERRUPTS(msr);

	return returnValue;
}

#endif



#ifdef USE_WIRE_PRINT ////////////////////////////

// Send a string to wireless link's UART
void WirePrint(uint8 *string)
{
	int stringLength;

	// Find string length
	for(stringLength = 0; string[stringLength] != '\0'; stringLength++);

	// Buffer the string
	while(!WireSendData(string, stringLength));
}

#endif



#ifdef USE_WIRE_NIBBLE ///////////////////////////

// Prints a nibble using WireSend (single hex digit, or 4-bit number)
void WireNibble(uint8 nibble)
{
	uint8 c;

	nibble &= 0xF;
	if((uint8)nibble <= 9) {
		c = nibble + '0';
	}
	else c = nibble-10 + 'A'; 

	while(!WireSendData(&c, 1));
}

#endif



#ifdef USE_WIRE_BYTE /////////////////////////////

// Print a byte value as a two-digit hex number using WireNibble()
void WireByte(uint8 byte)
{
	WireNibble((byte >> 4) & 0xF);
	WireNibble(byte & 0xF);	
}

#endif



#ifdef USE_WIRE_WORD /////////////////////////////

// Print 4-byte value as an eight-digit hex number using WireByte()
void WireWord(uint32 word)
{
	int i;

	for(i = 0; i < 4; i++) {
		WireByte(((unsigned char *)(&word))[i]);
	}
}

#endif



#ifdef USE_WIRE_INT //////////////////////////////

// Print value as a base 10 integer.
void WireInt(int32 value)
{
	char outputData[12];
	IntToString(value, 1, outputData);
	WirePrint(outputData);
}

#endif



#ifdef USE_WIRE_UNSIGNED /////////////////////////

// Print value as a base 10 integer.
void WireUnsigned(uint32 value)
{
	char outputData[12];
	IntToString(value, 0, outputData);
	WirePrint(outputData);
}

#endif



#ifdef USE_WIRE_PRINT_FLOAT //////////////////////

// Print approximate floating point number using WirePrint().
// Only prints if value is in a decent range.
void WirePrintFloat(double value)
{
	char string[MAX_FLOAT_STRING];

	FloatToString(value, string);
	WirePrint(string);
}

#endif



#ifdef USE_WIRE_PRINT_FIXED //////////////////////

// Print fixed point number using WirePrint().
void WirePrintFixed(uint32 value, int n)
{
	char string[MAX_FLOAT_STRING];

	FixedToString(value, n, string);
	WirePrint(string);
}

#endif
