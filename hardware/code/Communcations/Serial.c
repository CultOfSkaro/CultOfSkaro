/**
\file
\author Wade Fife

FILE:				Serial.c
AUTHOR:				Wade Fife
CREATED:			13 Oct 2004
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Communication Framework
COMPANION FILES:	Serial.h
DESCRIPTION:		See Serial.h

	Related Functions & Groups
	- Raw serial port functions
		- int   SER_getch			(Xuint32 baseaddr);
		- int   SER_DataReady		(Xuint32 baseaddr);
		- void  SER_PrintChar		(Xuint32 baseaddr, char c);
		- void  SER_PrintNibble	(Xuint32 baseaddr, uint8 nibble);
		- void  SER_PrintByte		(Xuint32 baseaddr, uint8 byte);
		- void  SER_PrintHalfWord	(Xuint32 baseaddr, uint16 hword);
		- void  SER_PrintWord		(Xuint32 baseaddr, uint32 word);
		- void  SER_IntToString	(Xuint32 baseaddr, uint32 value, int sign, char *dest);
		- void  SER_PrintInt		(Xuint32 baseaddr, int32 value);
		- void  SER_PrintUnsigned	(Xuint32 baseaddr, uint32 value);
		- void  SER_FloatToString	(Xuint32 baseaddr, float value, char *dest);
		- void  SER_PrintFloat	(Xuint32 baseaddr, float value);
		- void  SER_SendData		(Xuint32 baseaddr, uint8* data, uint32 size);
	- Text Transmission
		- void  Print(char *);
		- void  Println();
		- void  PrintHex8(uint8 c);
		- void  PrintHex16(uint32 ui);
		- void  PrintHex32(uint32 ui);
		- void  PrintChar(char c);
		- void  PrintInt(int32 i);
		- void  PrintUInt(uint32 ui);
		- void  PrintFloat(float d);
	- Text Utility Functions
		- uint8 hexToAscii(uint8 nibble);
		- int   IntToString(uint32 value, int sign, char *dest);
		- int   FloatToString(float value, char *dest);

CHANGE LOG
- 07 Nov 2005 WSF  Renamed file from Utils.c to Serial.c and removed all non serial
                   I/O related functions. Updated serial port base address \#define.
- 10 Nov 2005 WSF  Added PrintHalfWord() function. Revised PrintWord() to use it
                   instead of endian-dependant for loop.
- 17 May 2006 WSF  Changed PrintFloat() and FloatToString() to use float instead of
                   double type.
- 01 Aug 2007 BBE  Added Doxygen style comments
*/ 

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include <xparameters.h>
#include <xuartlite_l.h>

#include "Header.h"
#include "Serial.h"
#include "Comm.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/





/*********************************************************************
 Functional Group: Raw serial port functions
 Definition:       This group of functions sends raw (non-packed based)
                   data over the serial port
*/
/** Get character from serial port
*/
int SER_getch(Xuint32 baseaddr)
{
#ifdef SIMULATOR
	return 0;
#else
    return XUartLite_RecvByte(baseaddr);
#endif
}


/** Returns 1 if character waiting in incoming UART FIFO. Returns 0 if not.
*/
int SER_DataReady(Xuint32 baseaddr)
{
#ifdef SIMULATOR
	return 0;
#else
    if(!XUartLite_mIsReceiveEmpty(baseaddr))
		return 1;
    return 0;
#endif
}


/** Sends a byte to the UART
*/
void SER_PrintChar(Xuint32 baseaddr, char c)
{
#ifdef SIMULATOR
#else
    XUartLite_SendByte(baseaddr, c);
#endif
}




/** Prints a nibble (a single hex digit represented by a 4-bit number)
*/
void SER_PrintNibble(Xuint32 baseaddr, uint8 nibble)
{
#ifdef SIMULATOR
#else
    uint8 c;

    nibble &= 0xF;
    if((uint8)nibble <= 9) {
        c = nibble + '0';
    }
    else c = nibble-10 + 'A'; 
    
    XUartLite_SendByte(baseaddr, c);
#endif
}



/** Print a byte value as a two-digit hex number using PrintNibble()
*/
void SER_PrintByte(Xuint32 baseaddr, uint8 byte)
{
    SER_PrintNibble(baseaddr, (byte >> 4) & 0xF);
    SER_PrintNibble(baseaddr, byte & 0xF);    
}



/** Print a 2-byte value as a four-digit hex number using PrintNibble()
*/
void SER_PrintHalfWord(Xuint32 baseaddr, uint16 hword)
{
    SER_PrintByte(baseaddr, (uint8)(hword >> 8));
    SER_PrintByte(baseaddr, (uint8)hword);
}




/** Print 4-byte value as an eight-digit hex number using PrintByte()
*/
void SER_PrintWord(Xuint32 baseaddr, uint32 word){
    SER_PrintHalfWord(baseaddr, (uint16)(word >> 16));
    SER_PrintHalfWord(baseaddr, (uint16)word);
}





/** Print value as a base 10 integer.
*/
void SER_PrintInt(Xuint32 baseaddr, int32 value)
{
	int len;
    char outputData[12];
    
	IntToString(value, 1, outputData);
	len = strnlen(outputData, 12);
	SER_SendData(baseaddr, (uint8*) outputData, len);	
}



/** Print value as a base 10 integer.
*/
void SER_PrintUnsigned(Xuint32 baseaddr, uint32 value)
{
	int len;
    char outputData[12];

    IntToString(value, 0, outputData);
	len = strnlen(outputData, 12);
	SER_SendData(baseaddr, (uint8*) outputData, len);	
}




/** Print approximate floating point number using print().
	Only prints if value is in a decent range.
*/
void SER_PrintFloat(Xuint32 baseaddr, float value)
{
	int len;
    char string[MAX_FLOAT_STRING];

    FloatToString(value, string);
    len = strnlen(string, 12);
	SER_SendData(baseaddr, (uint8*) string, len);	
}


/** Sends an array of data over the serial port
*/
void SER_SendData(Xuint32 baseaddr, uint8* data, uint32 size)
{
#ifdef SIMULATOR
#else
	int i;
	for(i = 0 ; i < size ; i++)
	{
		//add wait for CLS signal here?
		XUartLite_SendByte(baseaddr, data[i]);
	}
#endif
}


/*********************************************************************
 Functional Group: Text Transmission
 Definition:       This group should have functions relating to 
                   sendnig text over the current IO port
*/ 
/** Sends text over the currently selected communication channel in packet form
*/
void Print(char *str)
{
	int size = strnlen(str, 256);	
	if(size > 0)
		TX_SendPacket(PKT_TEXT, size, (uint8*) str);	
}




/** Sends a "\n" over the currently selected communication channel in packet form
*/
void Println()
{
	char str[2];
	str[0] = '\n';
	str[1] = '\0';
	TX_SendPacket(PKT_TEXT,2, (uint8*) str);	
}




/** \brief	Converts an 8-bit number into a 2-digit hex string and sends it over 
			the currently selected communication channel in packet form
*/
void PrintHex8(unsigned char value)
{
	uint8 str[4];
	str[0] = hexToAscii(value >> 4 & 0x0F);
	str[1] = hexToAscii(value      & 0x0F);
	str[2] = '\0';
	str[3] = '\0';

	TX_SendPacket(PKT_TEXT, 3, str);	
}



/** \brief	Converts a 16-bit number into a 4-digit hex string and sends it over 
			the currently selected communication channel in packet form
*/
void PrintHex16(uint32 value)
{	
	uint8 str[6];
	str[0] = hexToAscii(value >> 12 & 0x0F);
	str[1] = hexToAscii(value >>  8 & 0x0F);
	str[2] = hexToAscii(value >>  4 & 0x0F);
	str[3] = hexToAscii(value       & 0x0F);
	str[4] = '\0';
	str[5] = '\0';

	TX_SendPacket(PKT_TEXT, 5, str);	
}


/** \brief	Converts a 32-bit number into a 8-digit hex string and sends it over 
			the currently selected communication channel in packet form
*/
void PrintHex32(uint32 value)
{	
	uint8 str[10];
	str[0] = hexToAscii(value >> 28 & 0x0F);
	str[1] = hexToAscii(value >> 24 & 0x0F);
	str[2] = hexToAscii(value >> 20 & 0x0F);
	str[3] = hexToAscii(value >> 16 & 0x0F);
	str[4] = hexToAscii(value >> 12 & 0x0F);
	str[5] = hexToAscii(value >>  8 & 0x0F);
	str[6] = hexToAscii(value >>  4 & 0x0F);
	str[7] = hexToAscii(value       & 0x0F);
	str[8] = '\0';
	str[9] = '\0';
	
	TX_SendPacket(PKT_TEXT, 9, str);
}


/** \brief  Sends a 8-bit number as a string over the currently selected 
			communication channel in packet form			
*/
void PrintChar(char c)
{
	uint8 str[2];
	str[0] = c;
	str[1] = '\0';
	
	TX_SendPacket(PKT_TEXT, 2, str);
}


/** \brief	Converts a 32-bit number into a base 10 integer string and sends it 
			over the currently selected communication channel in packet form
*/
void PrintInt(int32 value)
{
	char str[12];
	int size = IntToString(value, 1, str);
	if(size > 0)
		TX_SendPacket(PKT_TEXT, size, (uint8*) str);	
}



/** \brief	Converts an unsigned 32-bit number into a base 10 integer string 
			and sends it over the currently selected communication channel 
			in packet form
*/
void PrintUInt(uint32 value)
{
	char str[12];
	int size = IntToString(value, 0, str);
	if(size > 0)
		TX_SendPacket(PKT_TEXT, size, (uint8*) str);	
}




/** \brief	Converts a 32-bit float number into a base 10 integer string 
			and sends it over the currently selected communication channel 
			in packet form
*/
void PrintFloat(float value)
{
	char str[MAX_FLOAT_STRING];
	int size = FloatToString(value, str);
	if(size > 0)
		TX_SendPacket(PKT_TEXT, size, (uint8*) str);	
}





/*********************************************************************
 Functional Group: Text Utility Functions
 Definition:       This group should have functions relating to any 
   				   operation on text data.
*/ 
/** \brief  Converts an single base ten digit number value (0 - 9) into its
			ASCII equivalent value
*/
uint8 hexToAscii(uint8 hex)
{
	if((hex & 0x0F) <= 9)	return (hex    + '0');
	else					return (hex-10 + 'A');
}	 





/** Convert a signed or unsigned integer to a string.  
	If sign is 1 then value is treated as signed, if 0 it is treated as unsigned.
	Note: dest must be at least 12 bytes.
*/
int IntToString(uint32 value, int sign, char *dest){
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
	 return i + 1;
}






/** Converts a floating point number without rounding to a string. 
	It's not 100% precise but it's quite sufficient.  If the value to convert 
	is not in the range MIN_FLOAT..MAX_FLOAT then "?" will be the result.
*/
int FloatToString(float value, char *dest){
    int i, digit, bLeadingZero;
    float divisor;

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
        return 2;
    }
	
    // Store digits
    while(divisor > MIN_DIVISOR/2) {		
			digit = (int)(value / divisor);  
		  
        if(!bLeadingZero || digit != 0) {
            dest[i++] = digit + '0';
            value -= (float)digit * divisor;
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
	 return i + 1;
}











