
/*******************************************************************
*
* CAUTION: This file is automatically generated by libgen.
* Version: Xilinx EDK 13.3 EDK_O.76xd
* DO NOT EDIT.
*
* Copyright (c) 1995-2011 Xilinx, Inc.  All rights reserved.

* 
* Description: Driver configuration
*
*******************************************************************/

#include "xparameters.h"
#include "xuartlite.h"

/*
* The configuration table for devices
*/

XUartLite_Config XUartLite_ConfigTable[] =
{
	{
		XPAR_SERIAL_UART_DEVICE_ID,
		XPAR_SERIAL_UART_BASEADDR,
		XPAR_SERIAL_UART_BAUDRATE,
		XPAR_SERIAL_UART_USE_PARITY,
		XPAR_SERIAL_UART_ODD_PARITY,
		XPAR_SERIAL_UART_DATA_BITS
	},
	{
		XPAR_WIRELESS_UART_DEVICE_ID,
		XPAR_WIRELESS_UART_BASEADDR,
		XPAR_WIRELESS_UART_BAUDRATE,
		XPAR_WIRELESS_UART_USE_PARITY,
		XPAR_WIRELESS_UART_ODD_PARITY,
		XPAR_WIRELESS_UART_DATA_BITS
	}
};


