/**
\unit
\author Barrett Edwards

FILE:				Header.h
AUTHOR:				Barrett Edwards	
CREATED:			19 Jan 2007
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	main.c
DESCRIPTION:		

 This is the main global configuration header file for the 
 Helios Embedded Software Project (HESP?)

 All major macros (\#define) statements that configure the software should be
 placed here. 

 Say for example that I wanted to include or exclude some code based on the 
 application I targeting. I would use a \#define in this file to specify that 
 I wanted to include or exclude the code. And then I would put \#ifdef statements
 around the optional code.

 
CHANGE LOG

*/

/* Conditional Include-------------------------------------------------------*/
#ifndef HEADER_H
#define HEADER_H


/* Includes -----------------------------------------------------------------*/
// Include this so we can place printf() statements anywhere in the code without
// having to include stdio
#include <xparameters.h>

//#include "HeliosIO.h"


/* Macros -------------------------------------------------------------------*/
typedef signed char        int8;    ///< Signed 8-bit number
typedef unsigned char      uint8;   ///< Unsigned 8-bit number
typedef signed short       int16;   ///< Signed 16-bit number
typedef unsigned short     uint16;  ///< Unsigned 16-bit number
typedef signed int         int32;	///< Signed 32-bit number
typedef unsigned int       uint32;	///< Unsigned 32-bit number
typedef signed long long   int64;	///< Signed 64-bit number
typedef unsigned long long uint64;	///< Unsigned 64-bit number
typedef int                BOOL;	///< c-style boolean, which is raelly just an int




/// The CPU clock frequency in Hertz (usually 300,000,000)
#define CPU_CLOCK_FREQ		XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ
/// The number of timer-tick interrupts per second (1000 means once every millisecond)
#define SYSTEM_TICK_FREQ	1000       




/// Standardized macro to define the SRAM base address
#define SDRAM_ADDR			((uint8 *)XPAR_PLB_SDRAM_0_BASEADDR)
/// Standardized macro to define the size in bytes of the SRAM
#define SDRAM_SIZE			(XPAR_PLB_SDRAM_0_HIGHADDR-XPAR_PLB_SDRAM_0_BASEADDR+1)
/// Standardized macro to define the SRAM cache region for the PowerPC processor
#define SDRAM_CACHE_REGION	0x80000000




/// Standardized macro to define the BRAM base address
#define BRAM_ADDR			((uint8 *)XPAR_OPB_BRAM_IF_CNTLR_0_BASEADDR)
/// Standardized macro to define the size in bytes of the BRAM
#define BRAM_SIZE			(XPAR_OPB_BRAM_IF_CNTLR_0_HIGHADDR-XPAR_OPB_BRAM_IF_CNTLR_0_BASEADDR+1)
/// Standardized macro to define the BRAM cache region for the PowerPC processor
#define BRAM_CACHE_REGION	0x00000001




/// Defines which memory space regions the PowerPC should cache in its internal instruction cache
#define I_CACHE_REGION		BRAM_CACHE_REGION
/// Defines which memory space regions the PowerPC should cache in its internal data cache
#define D_CACHE_REGION		(SDRAM_CACHE_REGION | BRAM_CACHE_REGION)




/** NULL definition as 0
 */
#ifndef NULL
#define NULL 0
#endif

/** FALSE definition as 0
 */
#ifndef FALSE
#define FALSE 0
#endif

/** TRUE definition as 1
 */
#ifndef TRUE 
#define TRUE 1
#endif

///	Standard MIN(a,b) macro definition
#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

///	Standard MAX(a,b) macro definition
#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif




/** \brief PACKED is the macro to compress a struct to the 
	minimum number of bytes required to fit all the data.

	Sometimes the compiler will pad 8-bit variables in a 
	struct to 16 or 32 bits for memory alignment optimization.
	By using the PACKED macro, the compilier will be forced to 
	'pack' a struct into the minimum number of required bytes.
	
	This is important when sending structs over a comm channel 
	to another computer. If a struct was not packed in memory 
	in a reliable way, then when it was sent over a serial connection
	the other computer on the receiving end would not know which
	bytes of the data corresponded to which bytes in the struct.

	Example:
	\pre
	struct Demo 
	{
		uint8 a;
		uint16 b;
		uint32 c;
	};
	struct Demo demo;
	demo.a = 1;
	demo.b = 2;
	demo.c = 3;
	SendOverSerial( &demo, sizeof(Demo));
	\endpre

	In this example we don't know if the compiler padded the a and b 
	variables to 16-bits or even 32 bits. If we were to perform a 
	sizeof(Demo) we could then learn how much data was padded. But we 
	wouldn't necessarily know where the data was padded. Realistically,
	these issues could be dealt with by just knowing your compiler. But,
	to be on the safe side, and to possibly reduce memory space, try and 
	pack all structs as shown:
	
	\pre
	struct Demo 
	{
		uint8 a;
		uint16 b;
		uint32 c;
	} PACKED;
	\endpre

	This would produce a sizeof(Demo) = 7 (rather than 8 or 12).
*/
#ifdef SIMULATOR
#define PACKED 
#else 
#define PACKED __attribute__((__packed__))
#endif



//#ifdef SIMULATOR
//#ifdef fabsf
//#undef fabsf
//#endif
//#define fabsf abs
//#endif

/* Enumerations -------------------------------------------------------------*/

/** List of all possible Communication channels the Helios can use
*/
enum CommChannels 
{
	CHAN_NULL,		///< No communication channel should be used. Kind of like /dev/null
	CHAN_SERIAL,	///< Use the RS-232 Serial port on the Helios Main Board
	CHAN_WIRELESS,	///< Use the Zigbee / Aerocomm wireless serial connection
	CHAN_TTL,		///< A serial port that uses TTL voltages (3.3v) usually used to communicate with Magicc lab autopilot
	CHAN_USB		///< High speed USB usb link to desktop	 (this is required for video transmission)
};




/** This defines all the types of packets that can be sent to and from the GUI
	These values (0 - 255) are used in the HeliosPacketHeader::type field

	The first few packet types are specifically numbered in this way to match 
	the PLB_CAMERA Protocol in plb_camera.h
*/
enum PacketType 
{
	PKT_IMG_NULL		    = 0,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_GRAYSCALE8		= 1,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_GRAYSCALE16		= 2,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_RGB565			= 3,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_BINARY			= 4,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_COLOR_SEGMENTED	= 5,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_FEATURE8  		= 6,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_FEATURE16 		= 7,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_HSV1688			= 8,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_YUV				= 9,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_FEATURE8_LIST   = 10,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_FEATURE16_LIST  = 11,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_RANK_6_BIT		= 12,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_RANK_8_BIT		= 13,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_CCSTREAK_LIST	= 14,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_CCSTREAK0_LIST	= 15,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_CCSTREAK1_LIST	= 16,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_CCSTREAK2_LIST	= 17,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_CCSTREAK3_LIST	= 18,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_CCSTREAK4_LIST	= 19,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_CCSTREAK5_LIST	= 20,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_CCSTREAK6_LIST	= 21,	///< Helios->GUI The Helios is sending the GUI and image of this type
	PKT_IMG_CCSTREAK7_LIST	= 22,	///< Helios->GUI The Helios is sending the GUI and image of this type
	
	PKT_TEXT				= 23,	///< Helios->GUI The Helios is sending some text the GUI to print out
	
	PKT_CMD_START,					///< GUI->Helios The GUI is telling the Helios to Start processing
	PKT_CMD_STOP,					///< GUI->Helios The GUI is telling the Helios to Stop processing
	PKT_CMD_VIDEO_ON,				///< GUI->Helios The GUI is telling the Helios to start sending the captured images to the GUI 
	PKT_CMD_VIDEO_OFF,				///< GUI->Helios The GUI is telling the Helios to stop sending the captured images to the GUI 
	PKT_CMD_STATE_ON,				///< GUI->Helios The GUI is telling the Helios to start sending the state registers to the GUI
	PKT_CMD_STATE_OFF,				///< GUI->Helios The GUI is telling the Helios to stop sending the state registers to the GUI
	PKT_CMD_TRANS_FRAME_OPTION,		///< GUI->Helios The GUI is telling the Helios which camera frame to send to the GUI
	PKT_CMD_GUI_COMM_CHANNEL,		///< GUI->Helios The GUI is telling the Helios which communication channel to use to transmit to the GUI
		
	PKT_FRAME_TABLE_ENTRY_SMALL,	///< Helios->GUI Send the reduced version of the FrameTableEntry to the GUI for informative purposes
	PKT_FRAME_TABLE_ENTRY_FULL,		///< Helios->GUI Send the full version of the FrameTableEntry to the GUI for informative purposes

	PKT_THRESHOLDS,					///< GUI->Helios The GUI is telling the Helios to set the HSV color threshold to the accompanying values
	PKT_COORDINATES,				///< Helios->GUI The Helios is sending a list of feature points to the GUI (possibly to display as x's)
	PKT_CONN_COMP,					///< Helios->GUI The Helios is sending a list of connected components for display purposes
	PKT_CORRELATION,				///< Helios->GUI The Helios is sending a list of correlated feature points (x,y,tx,ty)
	PKT_PARTICLES,					///< Helios->GUI The Helios is sending a list of Particle structs to display

	PKT_LMOUSE_CLICK,				///< GUI->Helios The user has left-clicked on the video window
	PKT_RMOUSE_CLICK,				///< GUI->Helios The user has right-clicked on the video window

	PKT_REQ_REGISTER_DATA,			///< GUI->Helios has requested a refresh of the Data

	PKT_REG_DATA_CHAR,				///< GUI->Helios Used to set specfic registers
	PKT_REG_DATA_INT,				///< GUI->Helios Used to set specfic registers
	PKT_REG_DATA_UINT,				///< GUI->Helios Used to set specfic registers
	PKT_REG_DATA_HEX,				///< GUI->Helios Used to set specfic registers
	PKT_REG_DATA_FLOAT,				///< GUI->Helios Used to set specfic registers

	PKT_REG_DATA_ARRAY_CHAR,		///< Helios->GUI Send the entire data array of registers
	PKT_REG_DATA_ARRAY_INT,			///< Helios->GUI Send the entire data array of registers
	PKT_REG_DATA_ARRAY_UINT,		///< Helios->GUI Send the entire data array of registers
	PKT_REG_DATA_ARRAY_HEX,			///< Helios->GUI Send the entire data array of registers
	PKT_REG_DATA_ARRAY_FLOAT,		///< Helios->GUI Send the entire data array of registers

	NUM_PACKET_TYPES
};




/** All possible color based connected component identifiers
	These enumerations are used to tell the connected components 
	algoerithm which color it should group together into connected 
	components.

	These are usually used int the X_CONN_COMP_OPTIONS state register.
*/
enum CONN_COMP_OPTIONS
{	
	CONN_COMP_OPTIONS_BLACK		= 0x01,
	CONN_COMP_OPTIONS_RED		= 0x02,
	CONN_COMP_OPTIONS_YELLOW	= 0x04,
	CONN_COMP_OPTIONS_GREEN		= 0x08,
	CONN_COMP_OPTIONS_CYAN		= 0x10,
	CONN_COMP_OPTIONS_BLUE		= 0x20,
	CONN_COMP_OPTIONS_PURPLE	= 0x40,
	CONN_COMP_OPTIONS_WHITE		= 0x80
};




/* Structs ------------------------------------------------------------------*/
/** A fint union is used to access the raw bits of all primative data types (except double).
	A fint provides the functionality to perform byte operations on a primative 
	data type (except doubles) with out problems caused by type conversion. 
		
	A fint occupies only 4 bytes in memory.

	The most common use is to reorder a Little-endian data into a Big-
	endian data or vise-versa. Say for example you wanted to convert a float 
	from Big-endian to Little-endian. You would do the following: 

	\code 
	fint fi;
	fi.f = big_endian_float_value;
	little_endian_float_value = fi.c[3] << 24 | fi.c[2] << 16 | fi.c[1] << 8 | fi.c[0];		
	\endcode
 */
union fint
{
	uint32  u;		///< 32-bit unsigned integer
	float	f;		///< 32-bit single precision floating point
	int32	i;		///< 32-bit signed integer 
	char	c[4];	///< 4 element char array
};
typedef union fint fint;




/** 
*/
struct Point
{
	int x;
	int y;
} PACKED;
typedef struct Point Point;




/** The packet header used to communicate with the GUI 
	
	NOTE: The headers MUST be an even number of bytes due to 
	the USB data width of 16-bits
*/
struct HeliosPacketHeader 
{
	uint8	magic;				// 0xBE
	uint8	type;				// Type of this packet	
	uint16	checksum_header;	// checksum for the packet header
	uint16	checksum_data;		// checksum for the packet data
	uint32	buffer_size;		// # of bytes long of the data field
} PACKED;
typedef struct HeliosPacketHeader HeliosPacketHeader; ///< typedef struct HeliosPacketHeader HeliosPacketHeader;




/** Harris feature list element generated by camera core
	The Camera Core can produce a harris feature "image" or it can produce a 
	"list" of harris feature points. The CamFeature struct is a representation
	of a harris feature point that the camera core produces. 

	The camera core doesn't actually "think" in terms of structs. It just 
	writes data to memory in a certain format. The CamFeature struct matches
	this format so we can cast the data generated by the camera core as a list
	of CamFeature structs and we will be ok.
*/
struct CamFeature 
{
	uint16	y;		///< y location of the harris feature (0-479)
	uint16	x;		///< x location of the harris feature (0-639)
	uint16	value;	///< Strength of the harris feature (0-65535)
	uint16	other;  ///< Not used by camera core, so it is just zeros
} PACKED;
typedef struct CamFeature CamFeature; ///< typedef struct CamFeature CamFeature;




/** Connected Components streak list element generated by camera core
	
*/
struct CamStreak 
{
	uint16 y;		///< The y row in the image where this streak is
	uint16 xstart;	///< The x location where this streak starts
	uint16 xend;	///< The x location where this streak ends
	uint16 other;	///< zeros
} PACKED;
typedef struct CamStreak CamStreak; ///< typedef struct CamStreak CamStreak;




/** Data list format for instructing the plb_correlator core what point to track
*/
struct TrackingFeature 
{
	uint16	y;
	uint16	x;
	uint16 sy;
	uint16 sx;	
} PACKED;
typedef struct TrackingFeature TrackingFeature;




/** Data list element produced by the plb_correlator core for each feature tracked
*/
struct TrackedFeature 
{
	uint16	 x;
	uint16	 y;
	uint16  tx;
	uint16  ty;
	uint32	 error;
	uint8	 tx_subpixel;
	uint8	 ty_subpixel;
	uint16	 other;
} PACKED;
typedef struct TrackedFeature TrackedFeature;




/** Represents a single Connected Component
	The (y,xstart,xend) fields define a single connected component streak
	as computed in the camera core. 

	The additional fields (num_streaks,num_pixels,xsum,ysum) are used to 
	define the center-of-mass of the connected component and how large it 
	is. 

	The other fields (head,next,end) are used to create a linked list of 
	individual streaks that make up this Connected Component
*/
struct ConnectedComponent 
{
	uint16 y;				///< CamStreak y value 
	uint16 xstart;			///< CamStreak xstart value 
	uint16 xend;			///< CamStreak xend value 
	uint16 status;			///< used in connected components generation algorithm 

	// Connected Component summary info
	uint32 num_streaks;		///< The number of CamStreak objects that made up this connected component
	uint32 num_pixels;		///< For Connected Componenet center of mass calculations
	uint32 xsum;			///< For Connected Componenet center of mass calculations. center_x = xsum / num_pixels
	uint32 ysum;			///< For Connected Componenet center of mass calculations. center_y = ysum / num_pixels

	struct ConnectedComponent *head;	///< Connected Component Linked list. Head pointer to start of a connected component linked list. 
	struct ConnectedComponent *next;	///< Connected Component Linked list. Pointer to the Next component in a connected component linked list.
	struct ConnectedComponent *end;		///< Connected Component Linked list. Pointer to the Last component in a connected component linked list.
} PACKED;
typedef struct ConnectedComponent ConnComp; ///< typedef struct ConnectedComponent ConnComp;




/*  // Removed until i implement the code that uses this
typedef struct 
{
    float northPosition;
    float eastPosition;
    float altitude;
    float roll;
    float pitch;
    float heading;
    float groundtrack;
    float airspeed;
    float groundspeed;
    float hag;
} UAVState;
//*/




/* Function Prototypes ------------------------------------------------------*/
/**	Dynamically allocate memory on the {#Stack#{
	
	Since the allocated memory is on the stack frame of the current function, 
	it is automatically freed when the function returns.

	This is defined here because I cannot find the header file that declares it. But some where 
	in all the library files that EDK includes, this function is implemented so it links in. 

	The only reason it is included is for use in the OpenCV SVD function that I (Barrett) copied. 

	\param size The number of bytes that you want to allocate on the stack. 
	There is a maximum size you can allocate. I (Barrett) am not really sure how much that is, but 
	I do know that we have limited stack space on the Helios so don't request too much. 
	I would imagine that an array of a dozen ints would be fine. But trying to alocate 307,200 bytes
	(the size of a 640x480 image at 1 byte-per-pixel) would {#NOT#{ be ok.

	\return returns a void* to the base address of the allocated memory. This is {#NOT#{ malloc()!
	So it is not guarenteed to return a NULL pointer if something goes wrong. I also do not believe 
	that there is any stack overflow checking. So if you arbitrarily run out of stack space, BAD THINGS WILL HAPPEN.
*/
void* alloca(int size);
void  mainloop();
void Init();
void Init_Interrupts();
void Init_Camera();
#endif





// File Comment Template
/* 
FILE:				
VERSION:			
AUTHOR:				
CREATED:			
PLATFORM:			Helios Computing Platform
FRAMEWORK:			
COMPANION FILES:	
DESCRIPTION:		

FUNCTIONAL GROUPS:
FG1: 
FG2:

CHANGE LOG
*/ 
/* Conditional Inclusion ----------------------------------------------------*/
/* Includes -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/


