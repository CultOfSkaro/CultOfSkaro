/**
\file
\author Barrett Edwards

FILE:				State.c
AUTHOR:				Barrett Edwards
CREATED:			10 Nov 2006
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	State.h
DESCRIPTION:		See State.h
	
Related Functions & Groups
- Other 
	- void	  STInit();
- Read Functions
	- int8	  testchar(int index);
	- int8    rdchar(int index);
	- int32   rdint(int index);
	- uint32  rduint(int index);
	- uint32  rdhex(int index);
	- float   rdfloat(int index);
- Write Functions
	- void    wrchar(int index, int8 val);
	- void    wrint(int index, int32 val);
	- void    wruint(int index, uint32 val);
	- void    wrhex(int index, uint32 val);
	- void    wrfloat(int index, float val);

*/

/* Includes -----------------------------------------------------------------*/
#include <string.h>

#include "Header.h"

#include "InterruptControl.h"
#include "BufferStore.h"
#include "State.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory ------------------------------------------------------------*/
int8*	M_char;			///< global memory pointer to the char state register array
int32*	M_int;			///< global memory pointer to the int state register array
uint32*	M_uint;			///< global memory pointer to the uint state register array
uint32*	M_hex;			///< global memory pointer to the hex state register array
float*	M_float;		///< global memory pointer to the float state register array

int8*	M_copy_char;	///< global memory pointer to the copy of the char state register array
int32*	M_copy_int;		///< global memory pointer to the copy of the int state register array
uint32*	M_copy_uint;	///< global memory pointer to the copy of the uint state register array	
uint32*	M_copy_hex;		///< global memory pointer to the copy of the hex state register array
float*	M_copy_float;	///< global memory pointer to the copy of the float state register array


/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/

/** Allocate memory for & Initialize the State Registers to default values	
	STEPS:
	-# Statically allocates global memory to the various state register arrays.
	-# Initialize all the state register arrays to zero
	-# Set the Default values of a few registers
	-# Adjustments for simulator
*/
void STInit()
{
	// STEP: Statically allocates global memory to the various state register arrays.
	{
		M_char			= (int8*)	MemAlloc(NUM_MISC_CHAR  * sizeof(int8));
		M_int			= (int32*)	MemAlloc(NUM_MISC_INT   * sizeof(int32));
		M_uint			= (uint32*)	MemAlloc(NUM_MISC_UINT  * sizeof(uint32));
		M_hex			= (uint32*)	MemAlloc(NUM_MISC_HEX   * sizeof(uint32));
		M_float			= (float*)	MemAlloc(NUM_MISC_FLOAT * sizeof(float));

		M_copy_char		= (int8*)	MemAlloc(NUM_MISC_CHAR  * sizeof(int8));
		M_copy_int		= (int32*)	MemAlloc(NUM_MISC_INT   * sizeof(int32));
		M_copy_uint		= (uint32*)	MemAlloc(NUM_MISC_UINT  * sizeof(uint32));
		M_copy_hex		= (uint32*)	MemAlloc(NUM_MISC_HEX   * sizeof(uint32));
		M_copy_float	= (float*)	MemAlloc(NUM_MISC_FLOAT * sizeof(float));
	}

	// STEP: Initialize all the state register arrays to zero
	{
		memset(M_char,  0, NUM_MISC_CHAR  * sizeof(int8));
		memset(M_int,   0, NUM_MISC_INT   * sizeof(int32));
		memset(M_uint,  0, NUM_MISC_UINT  * sizeof(uint32));
		memset(M_hex,   0, NUM_MISC_HEX   * sizeof(uint32));
		memset(M_float, 0, NUM_MISC_FLOAT * sizeof(float));
	}


	// STEP: Set the Default values of a few registers
	{
		M_char[C_RUNNING]						= FALSE;
		M_char[C_SENDING_VIDEO]						= FALSE;
		M_char[C_SENDING_STATE]						= TRUE;
		M_char[C_GUI_COMM_CHANNEL]					= CHAN_USB;	
		M_char[C_GUI_COMM_CHANNEL_BACKUP]			= CHAN_WIRELESS;	
		M_char[C_ENABLE_AUTO_EXPOSURE]				= TRUE;
		M_char[C_ENABLE_AUTO_GAIN]					= TRUE;	
		M_int[I_LSD_HEARTBEAT]						= 100; // every # ms this will do something
		M_int[I_LSD_STATE_DATA]						= 500; // LSD = LoopScheduler Duration
		
		M_uint[U_FRAME_TRANSMIT_OPTION]				= 0;
		M_uint[U_IMAGE_WIDTH]						= FRAME_WIDTH;
		M_uint[U_IMAGE_HEIGHT]						= FRAME_HEIGHT;

		M_uint[U_MANUAL_EXPOSURE]					= 2;
		M_uint[U_MANUAL_GAIN]						= 16;
		M_uint[U_DEBUG]								= 100;

		M_hex[X_STATIC_BUFFER_OPTIONS]	 			= 0x00000000;
	}
}




/** Test and clear a boolean state register in a single atomic opperation
	Some char state registers are used to indicate a boolean value that 
	is used to request a certain action to be performed. In these situations
	the code will test if the register is TRUE, and if it is TRUE, then the 
	code will set the register back to FALSE and then the code will perform 
	some action that was requested. By using the testchar() function the 
	test and clear operations are performed in a single atomic section 
	with interrupts disabled.
*/
int8 testchar(int index) 
{
	CPU_MSR msr; // temporary storage of CPU interrupt state
	int8 rv;     // temporary storage of return value due to interrupt disable / enable
	
	
	msr = DISABLE_INTERRUPTS();  
	{
		rv = M_char[index];	
		if(rv) 
			M_char[index] = FALSE; 
	}
	RESTORE_INTERRUPTS(msr); 

	return rv;
}




/** Read a Char out of the char state register array at the requested index.
	If the requested index is out-of-bounds, the function returns zero.
*/
int8 rdchar(int index)	
{
	CPU_MSR msr;	// temporary storage of CPU interrupt state
	int8 rv;		// temporary storage of return value due to interrupt disable / enable

	if(		index < 0 
		||	index >= NUM_MISC_CHAR)
		return 0;

	msr = DISABLE_INTERRUPTS();  
	{
		rv = M_char[index];	
	}
	RESTORE_INTERRUPTS(msr); 

	return rv;
}




/** Read a signed int out of the int state register array at the requested index.
	If the requested index is out-of-bounds, the function returns zero.
*/
int32 rdint(int index)
{
	CPU_MSR msr;	// temporary storage of CPU interrupt state
	int32 rv;		// temporary storage of return value due to interrupt disable / enable

	if(		index < 0 
		||	index >= NUM_MISC_INT)
		return 0;

	msr = DISABLE_INTERRUPTS();  
	{
		rv = M_int[index];		
	}
	RESTORE_INTERRUPTS(msr); 

	return rv;
}




/** Read a unsigned int out of the uint state register array at the requested index.
	If the requested index is out-of-bounds, the function returns zero.
*/
uint32 rduint(int index)
{
	CPU_MSR msr;	// temporary storage of CPU interrupt state
	uint32 rv;		// temporary storage of return value due to interrupt disable / enable

	if(		index < 0 
		||	index >= NUM_MISC_UINT)
		return 0;

	msr = DISABLE_INTERRUPTS();  
	{
		rv = M_uint[index];	
	}
	RESTORE_INTERRUPTS(msr); 
	return rv;
}




/** Read a hex (uint32) out of the hex state register array at the requested index.
	If the requested index is out-of-bounds, the function returns zero.
*/
uint32 rdhex(int index)
{
	CPU_MSR msr;	// temporary storage of CPU interrupt state
	uint32 rv;		// temporary storage of return value due to interrupt disable / enable

	if(		index < 0 
		||	index >= NUM_MISC_HEX)
		return 0;	

	msr = DISABLE_INTERRUPTS();  
	{	
		rv = M_hex[index];		
	}
	RESTORE_INTERRUPTS(msr); 
	return rv;
}




/** Read a 32-bit single precision float out of the float state register array at the requested index.
	If the requested index is out-of-bounds, the function returns zero.
*/
float rdfloat(int index)
{
	CPU_MSR msr;	// temporary storage of CPU interrupt state
	float rv;		// temporary storage of return value due to interrupt disable / enable

	if(		index < 0 
		||	index >= NUM_MISC_FLOAT)
		return 0;	

	msr = DISABLE_INTERRUPTS();  
	{
		rv = M_float[index];	
	}
	RESTORE_INTERRUPTS(msr); 
	return rv;
}




/** Writes a single char value into the char state register array at the requested index.
	If the requested index is out-of-bounds, the function does nothing.
*/
void wrchar(int index, int8 val)
{
	CPU_MSR msr;	// temporary storage of CPU interrupt state

	if(		index < 0 
		||	index >= NUM_MISC_CHAR)
		return;	

	msr = DISABLE_INTERRUPTS();  
	{
		M_char[index] = val;	
	}
	RESTORE_INTERRUPTS(msr);
}




/** Writes a single signed int value into the int state register array at the requested index.
	If the requested index is out-of-bounds, the function does nothing.
*/
void wrint(int index, int32 val)
{	
	CPU_MSR msr;	// temporary storage of CPU interrupt state

	if(		index < 0 
		||	index >= NUM_MISC_INT)
		return;	

	msr = DISABLE_INTERRUPTS();  
	{
		M_int[index] = val;	
	}
	RESTORE_INTERRUPTS(msr);
}




/** Writes a single unsigned int value into the uint state register array at the requested index.
	If the requested index is out-of-bounds, the function does nothing.
*/
void wruint(int index, uint32 val)
{
	CPU_MSR msr;	// temporary storage of CPU interrupt state

	if(		index < 0 
		||	index >= NUM_MISC_UINT)
		return;	

	msr = DISABLE_INTERRUPTS();  
	{
		M_uint[index] = val;	
	}
	RESTORE_INTERRUPTS(msr);
}




/** Writes a single 32-bit hex value into the hex state register array at the requested index.
	If the requested index is out-of-bounds, the function does nothing.
*/
void wrhex(int index, uint32 val)
{	
	CPU_MSR msr;	// temporary storage of CPU interrupt state

	if(		index < 0 
		||	index >= NUM_MISC_HEX)
		return;	

	msr = DISABLE_INTERRUPTS();  
	{
		M_hex[index] = val;	
	}
	RESTORE_INTERRUPTS(msr);
}




/** Writes a single 32-bit single precision float value into the float state register array at the requested index.
	If the requested index is out-of-bounds, the function does nothing.
*/
void wrfloat(int index, float val)
{
	CPU_MSR msr;	// temporary storage of CPU interrupt state

	if(		index < 0 
		||	index >= NUM_MISC_FLOAT)
		return;	

	msr = DISABLE_INTERRUPTS();
	{
		M_float[index] = val;
	}
	RESTORE_INTERRUPTS(msr);
}



