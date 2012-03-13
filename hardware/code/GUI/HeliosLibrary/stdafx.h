// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include "ddraw.h"

// TODO: reference additional headers your program requires here
#include "SystemTypes.h"


typedef union {
	uint32	u;
	int32	i;
	float	f;
	uint8	c[4];	
} fint;


typedef struct {
	uint8	magic;				// 0xBE
	uint8	type;				// Type of this packet	
	uint16	checksum_header;	// checksum for the packet header
	uint16	checksum_data;		// checksum for the packet data
	uint32	buffer_size;		// # of bytes long of the data field
} HeliosPacketHeader;

typedef struct {
	uint16	y;
	uint16	x;
	uint16	value;
	uint16	other;
} CamFeature;

typedef struct {
	uint16 y;
	uint16 xstart;
	uint16 xend;
	uint16 other;
} CamStreak;

typedef struct {
	uint16	x;
	uint16	y;
	uint16  tx;
	uint16  ty;
	uint32	error;
	uint32	other;
} TrackedFeature;

enum frametable_status {
	FT_STATUS_EMPTY,
	FT_STATUS_ACQUIRING, //Camera is curently writing to this entry
	FT_STATUS_AVAILABLE, //Camera has finished capturing to this entry, it is ready to be checked out
	FT_STATUS_INUSE,     //This has been checked out
	FT_STATUS_COMPLETED, //This has been, checked back in
	FT_STATUS_MISSED,    //We for some reason never checked out this one
	FT_STATUS_NULL,      
	FT_STATUS_SAVED
};




/* Structs ------------------------------------------------------------------*/




enum PacketType 
{
	PKT_IMG_NULL		    = 0,
	PKT_IMG_GRAYSCALE8		= 1,
	PKT_IMG_GRAYSCALE16		= 2,
	PKT_IMG_RGB565			= 3, // These are done this way to match the PLB_CAMERA Protocol
	PKT_IMG_BINARY			= 4,
	PKT_IMG_COLOR_SEGMENTED	= 5,
	PKT_IMG_FEATURE8  		= 6,
	PKT_IMG_FEATURE16 		= 7,
	PKT_IMG_HSV1688			= 8,
	PKT_IMG_YUV				= 9,
	PKT_IMG_FEATURE8_LIST   = 10,	
	PKT_IMG_FEATURE16_LIST  = 11,	
	PKT_IMG_RANK_6_BIT		= 12,
	PKT_IMG_RANK_8_BIT		= 13,
	PKT_IMG_CCSTREAK_LIST	= 14,	
	PKT_IMG_CCSTREAK0_LIST	= 15,	
	PKT_IMG_CCSTREAK1_LIST	= 16,
	PKT_IMG_CCSTREAK2_LIST	= 17,
	PKT_IMG_CCSTREAK3_LIST	= 18,
	PKT_IMG_CCSTREAK4_LIST	= 19,
	PKT_IMG_CCSTREAK5_LIST	= 20,
	PKT_IMG_CCSTREAK6_LIST	= 21,
	PKT_IMG_CCSTREAK7_LIST	= 22,
	
	PKT_TEXT,
	
	PKT_CMD_START,
	PKT_CMD_STOP,	
	PKT_CMD_VIDEO_ON,
	PKT_CMD_VIDEO_OFF,
	PKT_CMD_STATE_ON,
	PKT_CMD_STATE_OFF,
	PKT_CMD_TRANS_FRAME_OPTION,
	PKT_CMD_GUI_COMM_CHANNEL,

	PKT_FRAME_TABLE_ENTRY_SMALL,
	PKT_FRAME_TABLE_ENTRY_FULL,
		
	PKT_THRESHOLDS,
	PKT_COORDINATES,
	PKT_CONN_COMP,
	PKT_CORRELATION,
	PKT_PARTICLES,

	PKT_LMOUSE_CLICK,
	PKT_RMOUSE_CLICK,

	PKT_REQ_REGISTER_DATA,			//GUI has requested a refresh of the Data

	PKT_REG_DATA_CHAR,				// These are used to set specfic registers
	PKT_REG_DATA_INT,
	PKT_REG_DATA_UINT,
	PKT_REG_DATA_HEX,
	PKT_REG_DATA_FLOAT, 

	PKT_REG_DATA_ARRAY_CHAR,		// Just the data arrays
	PKT_REG_DATA_ARRAY_INT,
	PKT_REG_DATA_ARRAY_UINT,
	PKT_REG_DATA_ARRAY_HEX,
	PKT_REG_DATA_ARRAY_FLOAT,

	NUM_PACKET_TYPES
};

#include <stdio.h>
#include <afxmt.h>  
#include <afxwin.h> 
#include <stdlib.h> // malloc
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <string>
#include <fstream>

#include "CyAPI.h"
#include "cv.h"
#include "highgui.h"


#include "Helios.h"
extern Helios* g_helios;



