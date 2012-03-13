/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    HeliosAPI.h
AUTHOR:  Barrett Edwards
CREATED: 9 May 2007

DESCRIPTION

	

******************************************************************************/
#ifndef HELIOSAPI_H
#define HELIOSAPI_H


/* Includes -----------------------------------------------------------------*/
/* Defines ------------------------------------------------------------------*/

/** \def DLLEXPORT 
 *  \brief A macro to identify which functions or variables should be exported from the DLL.
 */ 
#define DLLEXPORT __declspec(dllexport)

/* Structs ------------------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/

enum ERegisterType {
	REG_TYPE_CHAR,
	REG_TYPE_INT,
	REG_TYPE_UINT,
	REG_TYPE_HEX,
	REG_TYPE_FLOAT
};

enum EGUIProcessingOptions {
	HGUI_PROCESSING_WRITE_TO_AVI,
	HGUI_PROCESSING_DISPLAY_COORDINATES,
	HGUI_PROCESSING_DISPLAY_CORRELATIONS,
	HNUM_GUI_PROCESSING_OPTIONS
};



extern "C" DLLEXPORT void HInit();
extern "C" DLLEXPORT void HStart();
extern "C" DLLEXPORT void HStop();
extern "C" DLLEXPORT void HStopAndWait();
extern "C" DLLEXPORT void HEnableGUIProcessingOption(int option, int on);
extern "C" DLLEXPORT void HShutter();


extern "C" DLLEXPORT void HAVIInit(const char* filename, int width, int height, double fps, int format_fourcc, int iscolor);
extern "C" DLLEXPORT void HAVISetFormat(int format_fourcc);
extern "C" DLLEXPORT void HAVISetFilename(const char* filename);
extern "C" DLLEXPORT void HAVIWriteAsImages(int b);
extern "C" DLLEXPORT void HAVIStart();
extern "C" DLLEXPORT void HAVIStop();




extern "C" DLLEXPORT size_t HGetTextSize();
extern "C" DLLEXPORT void   HGetText(char* str, int max_num);

extern "C" DLLEXPORT void HSetRegisterChar( int index , unsigned char value);
extern "C" DLLEXPORT void HSetRegisterInt(  int index , int			  value);
extern "C" DLLEXPORT void HSetRegisterUInt( int index , unsigned int  value);
extern "C" DLLEXPORT void HSetRegisterHex(  int index , unsigned int  value);
extern "C" DLLEXPORT void HSetRegisterFloat(int index , float		  value);

extern "C" DLLEXPORT unsigned char	HGetRegisterChar( int index);
extern "C" DLLEXPORT int			HGetRegisterInt(  int index);
extern "C" DLLEXPORT unsigned int	HGetRegisterUInt( int index);
extern "C" DLLEXPORT unsigned int	HGetRegisterHex(  int index);
extern "C" DLLEXPORT float			HGetRegisterFloat(int index);

extern "C" DLLEXPORT void HEnableRunning(int on);
extern "C" DLLEXPORT void HEnableVideoTransmission(int on);
extern "C" DLLEXPORT void HEnableStateTransmission(int on);
extern "C" DLLEXPORT void HSetVideoTransmissionType(int type);
extern "C" DLLEXPORT void HSendMouseLClick(int x, int y);






extern "C" DLLEXPORT bool HDDInit(HWND handle, int width, int height);
extern "C" DLLEXPORT void HDDDraw(IplImage* image);
extern "C" DLLEXPORT void HDDBlack();
extern "C" DLLEXPORT void HDDRefresh();
extern "C" DLLEXPORT void HDDDestroy();
extern "C" DLLEXPORT bool HDDValid();






#endif 
