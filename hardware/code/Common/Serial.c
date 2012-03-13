/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    Serial.c
AUTHOR:  Wade Fife
CREATED: 10/13/04

DESCRIPTION

This file contains several useful functions for text I/O, such as printing
numbers to the serial port.

CHANGE LOG

11/07/05 WSF  Renamed file from Utils.c to Serial.c and removed all non serial
              I/O related functions. Updated serial port base address #define.
11/10/05 WSF  Added PrintHalfWord() function. Revised PrintWord() to use it
              instead of endian-dependant for loop.
05/17/06 WSF  Changed PrintFloat() and FloatToString() to use float instead of
              double type.

******************************************************************************/

#include <xparameters.h>
#include <xuartlite_l.h>
#include <stdio.h>

#include "SystemTypes.h"
#include "Serial.h"



// SERIAL I/O CODE ////////////////////////////////////////////////////////////


#ifdef USE_GETCH //////////////////////////////////////////////////////////////

// Get character from serial port
int getch(void)
{
    return XUartLite_RecvByte(SER_BASEADDR);
}

#endif



#ifdef USE_KBHIT //////////////////////////////////////////////////////////////

// Returns 1 if character waiting in incoming UART FIFO. Returns 0 if not.
int kbhit(void)
{
    if(!XUartLite_mIsReceiveEmpty(SER_BASEADDR)) return 1;
    return 0;
}

#endif



#ifdef USE_PRINT_CHAR /////////////////////////////////////////////////////////

// Sends a byte to the UART
void PrintChar(char c)
{
    XUartLite_SendByte(SER_BASEADDR, c);
}

#endif



#ifdef USE_PRINT_NIBBLE ///////////////////////////////////////////////////////

// Prints a nibble (a single hex digit represented by a 4-bit number)
void PrintNibble(uint8 nibble)
{
    uint8 c;

    nibble &= 0xF;
    if((uint8)nibble <= 9) {
        c = nibble + '0';
    }
    else c = nibble-10 + 'A'; 
    
    XUartLite_SendByte(SER_BASEADDR, c);
}

#endif



#ifdef USE_PRINT_BYTE /////////////////////////////////////////////////////////

// Print a byte value as a two-digit hex number using PrintNibble()
void PrintByte(uint8 byte)
{
    PrintNibble((byte >> 4) & 0xF);
    PrintNibble(byte & 0xF);    
}

#endif



#ifdef USE_PRINT_HALF_WORD ////////////////////////////////////////////////////

// Print a 2-byte value as a four-digit hex number using PrintNibble()
void PrintHalfWord(uint16 hword)
{
    PrintByte((uint8)(hword >> 8));
    PrintByte((uint8)hword);
}

#endif



#ifdef USE_PRINT_WORD /////////////////////////////////////////////////////////

// Print 4-byte value as an eight-digit hex number using PrintByte()
void PrintWord(uint32 word)
{
    PrintHalfWord((uint16)(word >> 16));
    PrintHalfWord((uint16)word);
}

#endif



#ifdef USE_INT_TO_STRING //////////////////////////////////////////////////////

// Convert a signed or unsigned integer to a string.  If sign is 1
// then value is treated as signed, if 0 it is treated as unsigned.
// Note: dest must be at least 12 bytes.
void IntToString(uint32 value, int sign, char *dest)
{
    int i;
    uint32 divisor;

    i = 0;
    divisor = 1000000000;

    // Check for negative
    if(sign && ((int32)value) < 0) {
        dest[0] = '-';
        i++;
        value = -value;
    }

    // Find place of first digit
    if(value == 0) divisor = 1;
    else while(value/divisor == 0) divisor /= 10;

    // Store digits
    while(divisor != 0) {
        dest[i] = (char)((unsigned int)value / divisor) + '0';
        i++;
        value = (unsigned int)value % divisor;
        divisor = divisor / 10;
    }
    
    dest[i] = '\0';
}

#endif



#ifdef USE_PRINT_INT //////////////////////////////////////////////////////////

// Print value as a base 10 integer.
void PrintInt(int32 value)
{
    char outputData[12];
    IntToString(value, 1, outputData);
    print(outputData);
}

#endif



#ifdef USE_PRINT_UNSIGNED /////////////////////////////////////////////////////

// Print value as a base 10 integer.
void PrintUnsigned(uint32 value)
{
    char outputData[12];
    IntToString(value, 0, outputData);
    print(outputData);
}

#endif



#ifdef USE_FLOAT_TO_STRING ////////////////////////////////////////////////////

// Converts a floating point number without rounding to a string. It's
// not 100% precise but it's quite sufficient.  If the value to
// convert is not in the range MIN_FLOAT..MAX_FLOAT then "?" will be
// the result.
void FloatToString(float value, char *dest)
{
    int i, digit, bLeadingZero;
    double divisor;

    bLeadingZero = 1;

    i = 0;
    divisor = MAX_DIVISOR;

    // Check for negative
    if(value < 0) {
        dest[0] = '-';
        i++;
        value = -value;
    }

    // Check for valid range
    if(value > MAX_FLOAT || (value < MIN_FLOAT && value != 0.0)) {
        dest[0] = '?';
        dest[1] = '\0';
        return;
    }

    // Store digits
    while(divisor > MIN_DIVISOR/2) {
        digit = (int)(value / divisor);
        
        if(!bLeadingZero || digit != 0) {
            dest[i++] = digit + '0';
            value -= (double)digit * divisor;
            bLeadingZero = 0;
        }

        // Is it time to print the decimal point?
        if(0.5 < divisor && divisor < 1.5) {
            if(bLeadingZero) dest[i++] ='0';
            dest[i++] = '.';
            bLeadingZero = 0;
        }

        divisor = divisor / 10;
    }
    
    dest[i] = '\0';
}

#endif



#ifdef USE_PRINT_FLOAT ////////////////////////////////////////////////////////

// Print approximate floating point number using print().
// Only prints if value is in a decent range.
void PrintFloat(float value)
{
    char string[MAX_FLOAT_STRING];

    FloatToString(value, string);
    print(string);
}

#endif
