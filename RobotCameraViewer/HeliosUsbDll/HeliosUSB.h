/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    HeliosUSB.h
AUTHOR:  Wade Fife
CREATED: 3/2/06

DESCRIPTION

Header file for the HeliosUSB.cpp file.

******************************************************************************/

#include <windows.h>
#include "CyAPI.h"

#define MAX_USB_PACKET_SIZE  512


bool USBOpenHelios(HWND hwnd);
void USBCloseHelios(void);
bool USBWriteHelios(void *data, DWORD numBytes);
bool USBReadHelios(void *dest, DWORD *numBytesRead);
