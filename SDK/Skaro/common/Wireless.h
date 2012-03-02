/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    Wireless.h
AUTHOR:  Wade Fife
CREATED: 12/10/04

DESCRIPTION

Header file for Wireless.c

******************************************************************************/

#ifndef WIRELESS_H
#define WIRELESS_H

#include "Header.h"



// USER CONFIGURATION /////////////////////////////////////////////////////////

// Define wich functions you wish to include in compilation. Please
// note that there may be dependencies between functions.
#define USE_WIRE_SEND_DATA    // Queues a packet of data for trasnfer
#define USE_WIRE_RECV_DATA    // Reads data from receive buffer
//
#define USE_WIRE_SEND         // Sends a single byte (blocking)
#define USE_WIRE_GETCH        // Blocks and returns byte when received
#define USE_WIRE_DATA_READY   // Blocks until data is in receive buffer
//
#define USE_WIRE_PRINT        // Sends a string over wireless 
//
#undef USE_WIRE_NIBBLE       // Sends a 4-bit hex digit over wireless
#undef USE_WIRE_BYTE         // Sends an 8-bit hex number using WireNibble
#undef USE_WIRE_WORD         // Sends a 32-bit hex number using WireByte
//
#define USE_WIRE_INT          // Sends integer using WirePrint and IntToString
#undef USE_WIRE_UNSIGNED     // Sends unsigned using WirePrint and IntToString
//
#undef USE_WIRE_PRINT_FIXED  // Sends fixed using WirePrint and FixedToString
#define USE_WIRE_PRINT_FLOAT  // Sends float using WirePrint and FloatToString


// Define the following to be the base address of the wireless UART
#define WIRELESS_BASEADDR (XPAR_WIRELESS_UART_BASEADDR)

// Define this if you want CTS to be checked before sending more
// data. Otherwise undef it. See note for WirelessInterruptHandler()
// in Wireless.c for more information.
#define CHECK_MODEM_CTS

// Define this to return the value of the CTS signal (positive asserted)
#define MODEM_CTS() \
        ((~XGpio_mGetDataReg(XPAR_XPS_GPIO_1_BASEADDR, 1)) & 0x80)

///////////////////////////////////////////////////////////////////////////////



// FUNCTION PROTOTYPES ////////////////////////////////////////////////////////

// Wireless Handler
void WirelessInterruptHandler(void);

// Function primitives
void  WirelessInit(void);
int   WireSendData(uint8 *data, int numBytes);
int   WireRecvData(uint8 *dest, int numBytes);

// High-level functions
void  WireSend(uint8 byte);
uint8 WireGetch(void);
int   WireDataReady(void);
void  WirePrint(uint8 *string);
void  WireNibble(uint8 nibble);
void  WireByte(uint8 byte);
void  WireWord(uint32 word);
void  WireInt(int32 value);
void  WireUnsigned(uint32 value);
void  WirePrintVixed(uint32 value, int n);
void  WirePrintFloat(double value);


#endif
