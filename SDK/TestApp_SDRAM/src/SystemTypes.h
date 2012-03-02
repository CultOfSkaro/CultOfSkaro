/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    SystemTypes.h
AUTHOR:  Wade Fife
CREATED: 10/5/04

DESCRIPTION

Contains type definitions for signed and unsigned types, specified by
size. Using these types eases portability and makes explicit when specific
sizes are needed. This particular file is configured for a 32-Bit processor.

NOTES

Please note that it isn't always necessary to use these types. For example, it
is generally safe to assume that an "int" will never be less than 16 bits. If
the size of the variable isn't important and the range isn't important then it
may be better to use the standard C type.

CHANGE LOG

11/07/05  WSF  Added BOOL type and TRUE, FALSE definitions.
12/28/05  WSF  Added "signed" keyword, since PPC char defaults to unsigned.

******************************************************************************/

#ifndef SYSTEM_TYPES_H
#define SYSTEM_TYPES_H

typedef signed char        int8;    // Signed 8-bit number
typedef unsigned char      uint8;   // Unsigned 8-bit number

typedef signed short       int16;   // Signed 16-bit number
typedef unsigned short     uint16;  // Unsigned 16-bit number

typedef signed int         int32;   // Signed 32-bit number
typedef unsigned int       uint32;  // Unsigned 32-bit number

typedef signed long long   int64;   // Signed 64-bit number
typedef unsigned long long uint64;  // Unsigned 64-bit number

typedef int                BOOL;

#ifndef NULL
#define NULL 0
#endif

#define FALSE 0
#define TRUE  1

#endif
