/**
\unit
\author Barrett Edwards

FILE:				State.h
AUTHOR:				Barrett Edwards
CREATED:			10 Nov 2006
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	State.c
DESCRIPTION:	
	
	The State is intended to serve as a place to store variables or other 
	parameters that need to be adjusted at runtime. By creating a single 
	storage system of primative types (char, int, float, etc...), all of 
	these global variables can be easily added to or reduced to meet the 
	need of an application. 

	For example, the user code requirs a new integer variable called 
	ERROR_CODE. So to add this variable to the State registers two things 
	need to be done. First, is to add an entry name I_ERROR_CODE in the 
	StateRegisterInt enumeration. The second thing that needs to be done 
	is to add the entry name I_ERROR_CODE to the Registers.txt file that 
	the GUI uses to populate the registers list in the GUI.	The prefix 
	'#I_#' is #important#! The prefix is used by the GUI to determine which
	type of register the name is connected to. (i.e #C_# == char, #I_# == int)

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

CHANGELOG
2-23-2010 SGF Removed a lot of SAA stuff to simplify and reduce overhead
*/

/* Conditional Inclusion ----------------------------------------------------*/
#ifndef STATE_H
#define STATE_H


/* Includes -----------------------------------------------------------------*/
#include "Header.h"


/* Macros -------------------------------------------------------------------*/
/** Version of the state register variables
	This defines the current version of the state variables (char,int,uint,hex,float)
	This version # should be incremented every time the state enumerations change
*/
#define ST_VERSION	16


/* Enumerations -------------------------------------------------------------*/
/** Names of the #char# State Registers
	These register names can only be used with the setchar() and rdchar() 
	state functions. These serve as a place to store variables or other 
	parameters that need to be adjusted at runtime.

	Example:
	\pre
	setchar( C_RUNNING , TRUE);
	char is_it_running = rdchar( C_RUNNING );
	\endpre
*/
enum StateRegisterChar 
{
	C_RUNNING,
	C_SENDING_VIDEO,
	C_SENDING_STATE,
	C_GUI_COMM_CHANNEL,	
	C_GUI_COMM_CHANNEL_BACKUP, // If the Main comm channel is disconnected (usb) then us this one
	C_ENABLE_AUTO_EXPOSURE,
	C_ENABLE_AUTO_GAIN,
	C_DEBUG,
	C_REQ_STATE_DATA,
	C_REQ_PRINT_FRAME_TABLE,
	C_REQ_START_CAM_CORE,
	NUM_MISC_CHAR
};




/** Names of the #int# State Registers
	These register names can only be used with the setint() and rdint() 
	state functions. These serve as a place to store variables or other 
	parameters that need to be adjusted at runtime.

	Example:
	\pre
	setchar( C_RUNNING , TRUE);
	char is_it_running = rdchar( C_RUNNING );
	\endpre
*/
enum StateRegisterInt 
{	
	I_DEBUG,
	I_LSC_HEARTBEAT,
	I_LSD_HEARTBEAT,
	I_LSC_STATE_DATA,
	I_LSD_STATE_DATA,
	I_LSC_READ_BUTTON,

	NUM_MISC_INT
};



/** Names of the #uint# State Registers
	These register names can only be used with the setuint() and rduint() 
	state functions. These serve as a place to store variables or other 
	parameters that need to be adjusted at runtime.

	Example:
	\pre
	setchar( C_RUNNING , TRUE);
	char is_it_running = rdchar( C_RUNNING );
	\endpre
*/
enum StateRegisterUInt 
{
	U_MS,
	U_FPS,
	U_FPS_CAM,
	U_FRAME_COUNT,
	U_FRAME_COUNT_TX,
	U_FRAME_TRANSMIT_OPTION,
	U_MANUAL_EXPOSURE,
	U_MANUAL_GAIN,
	U_DEBUG,	
	U_IMAGE_WIDTH,
	U_IMAGE_HEIGHT,
	U_ISR_COUNT_USB,
	U_CAMERA_VERSION,	
	NUM_MISC_UINT
};




/** Names of the #hex# State Registers
	These register names can only be used with the sethex() and gethex() 
	state functions. These serve as a place to store variables or other 
	parameters that need to be adjusted at runtime.

	Example:
	\pre
	setchar( C_RUNNING , TRUE);
	char is_it_running = rdchar( C_RUNNING );
	\endpre
*/
enum StateRegisterHex 
{
	X_CAM_REG_ADDRESS,
	X_CAM_REG_VALUE,
	X_DEBUG,
	X_STATIC_BUFFER_OPTIONS,
	X_CAM_SAVE_OPTIONS_BASE,	
	X_CAM_SAVE_OPTIONS_EXTENDED,
	X_CAM0_STATUS,
	X_MEMORY_INIT_RESULTS,
	X_MEM_ALLOC_ADDR,
	NUM_MISC_HEX
};




/** Names of the #float# State Registers
	These register names can only be used with the setfloat() and getfloat() 
	state functions. These serve as a place to store variables or other 
	parameters that need to be adjusted at runtime.

	Example:
	\pre
	setchar( C_RUNNING , TRUE);
	char is_it_running = rdchar( C_RUNNING );
	\endpre
*/
enum StateRegisterFloat 
{
	F_DEBUG,
	NUM_MISC_FLOAT
};



/* Structs ------------------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
// These should only be access within a critical section with interrupts turned off
extern int8*		M_char;
extern int32*		M_int;
extern uint32*		M_uint;
extern uint32*		M_hex;
extern float*		M_float;



// These serve as a place to copy the content of the state registers temporarily
// such as when you want to send them over a communication channel and don't want 
// them to be modified in the background durring the transfer
extern int8*		M_copy_char;
extern int32*		M_copy_int;
extern uint32*		M_copy_uint;
extern uint32*		M_copy_hex;
extern float*		M_copy_float;


/* Function Prototypes ------------------------------------------------------*/
void	STInit();

int8	testchar(int index);
int8    rdchar(int index);
int32   rdint(int index);
uint32  rduint(int index);
uint32  rdhex(int index);
float   rdfloat(int index);

void    wrchar(int index, int8 val);
void    wrint(int index, int32 val);
void    wruint(int index, uint32 val);
void    wrhex(int index, uint32 val);
void    wrfloat(int index, float val);

#endif
