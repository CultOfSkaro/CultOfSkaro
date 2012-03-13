
#include "stdafx.h"
#include "HeliosAPI.h"
#include "Helios.h"
#include "DirectDrawWrapper.h"
#include "Register.h"
#include "AVIWriterThread.h"
#include "FrameTableEntry.h"

Helios* g_helios = NULL;

extern "C" DLLEXPORT void HInit()
{	
	if(!g_helios)
		g_helios = new Helios();		
}

extern "C" DLLEXPORT void HStart()
{
	if(g_helios)
		g_helios->Start();
}


extern "C" DLLEXPORT void HStop()
{
	if(g_helios)
		g_helios->Stop();
}

extern "C" DLLEXPORT void HStopAndWait()
{
	if(g_helios)
		g_helios->StopAndWait();
}

extern "C" DLLEXPORT void HEnableGUIProcessingOption(int option, int value)
{
	//printf("HEnableGUIProcessingOption(%d,%d)\r\n",option,value);
	if(g_helios)
		if(0 <= option && option < Helios::NUM_GUI_PROCESSING_OPTIONS)
			g_helios->m_gui_processing_options[option] = value;
}

extern "C" DLLEXPORT void HShutter()
{
	if(g_helios)
		g_helios->Shutter();			
}


extern "C" DLLEXPORT void HAVIInit(const char* filename, int width, int height, double fps, int format_fourcc, int iscolor)
{
	if(g_helios)
		g_helios->m_aviwriterthread->Init(filename,width,height,fps,format_fourcc,iscolor);
}


extern "C" DLLEXPORT void HAVISetFormat(int format_fourcc)
{
	if(g_helios)
		g_helios->m_aviwriterthread->SetFormat(format_fourcc);
}

extern "C" DLLEXPORT void HAVISetFilename(const char* filename)
{
	if(g_helios)
		g_helios->m_aviwriterthread->SetFilename(filename);
}

extern "C" DLLEXPORT void HAVIWriteAsImages(int b)
{
	if(g_helios)
	{
		if (b)	g_helios->m_aviwriterthread->WriteAsImages(true);
		else	g_helios->m_aviwriterthread->WriteAsImages(false);
	}
}


extern "C" DLLEXPORT void HAVIStart()
{
	if(g_helios)
	{
		bool opened_successfuly = g_helios->m_aviwriterthread->Start();
		if(opened_successfuly)
			HEnableGUIProcessingOption(HGUI_PROCESSING_WRITE_TO_AVI,1);
	}
}

extern "C" DLLEXPORT void HAVIStop()
{
	if(g_helios)
	{
		HEnableGUIProcessingOption(HGUI_PROCESSING_WRITE_TO_AVI,0);
		g_helios->m_aviwriterthread->Stop();		
	}
}


extern "C" DLLEXPORT size_t HGetTextSize()
{
	size_t return_value = 0;

	if(g_helios)
		return_value = g_helios->TextSize();

	return return_value;
}

extern "C" DLLEXPORT void HGetText(char* str, int max_num)
{
	if(g_helios)
	{
		string str2 = g_helios->GetText();
		for(int i = 0 ; i < (int)str2.size()+1 && i < max_num ; i++)
			str[i] = str2[i];
	}
}




//Register API
extern "C" DLLEXPORT void HSetRegisterChar( int index , unsigned char value)
{
	printf("HSetRegisterChar(%d,%u)\r\n",index,value);

	if(g_helios)
		g_helios->SendRegisterChar(index, value);

}

extern "C" DLLEXPORT void HSetRegisterInt(int index , int value)
{
	printf("HSetRegisterInt(%d,%d)\r\n",index,value);

	if(g_helios)
		g_helios->SendRegisterInt(index, value);
}

extern "C" DLLEXPORT void HSetRegisterUInt( int index , unsigned int value)
{
	printf("HSetRegisterUInt(%d,%u)\r\n",index,value);
	if(g_helios)
		g_helios->SendRegisterUInt(index, value);
}

extern "C" DLLEXPORT void HSetRegisterHex( int index , unsigned int value)
{
	printf("HSetRegisterHex(0x%x,%u)\r\n",index,value);
	if(g_helios)
		g_helios->SendRegisterHex(index, value);
}

extern "C" DLLEXPORT void HSetRegisterFloat(int index , float value)
{
	printf("HSetRegisterFloat(%d,%f)\r\n",index,value);
	if(g_helios)
		g_helios->SendRegisterFloat(index, value);
}


extern "C" DLLEXPORT unsigned char HGetRegisterChar(int index)
{
	unsigned char return_value = 0;

	if(g_helios)
		return_value = g_helios->m_register_char.getData(index);

	return return_value;
}

extern "C" DLLEXPORT int HGetRegisterInt(int index)
{
	int return_value = 0;

	if(g_helios)
		return_value = g_helios->m_register_int.getData(index);

	return return_value;
}

extern "C" DLLEXPORT unsigned int HGetRegisterUInt(int index)
{
	unsigned int return_value = 0;

	if(g_helios)
		return_value = g_helios->m_register_uint.getData(index);

	return return_value;
}

extern "C" DLLEXPORT unsigned int HGetRegisterHex(int index)
{
	unsigned int return_value = 0;

	if(g_helios)
		return_value = g_helios->m_register_hex.getData(index);

	return return_value;
}

extern "C" DLLEXPORT float HGetRegisterFloat(int index)
{
	float return_value = 0;

	if(g_helios)
		return_value = g_helios->m_register_float.getData(index);

	return return_value;
}





extern "C" DLLEXPORT void HEnableRunning(int on)
{
	if(g_helios)
	{
		if(on)	g_helios->EnableRunning(true);
		else    g_helios->EnableRunning(false);
	}
}

extern "C" DLLEXPORT void HEnableVideoTransmission(int on)
{
	if(g_helios)
	{
		if(on)	g_helios->EnableVideoTransmission(true);
		else	g_helios->EnableVideoTransmission(false);
	}
}

extern "C" DLLEXPORT void HEnableStateTransmission(int on)
{
	if(g_helios)
	{
		if(on)	g_helios->EnableStateTransmission(true);
		else	g_helios->EnableStateTransmission(false);
	}
}

extern "C" DLLEXPORT void HSetVideoTransmissionType(int type)
{
	if(g_helios)
		g_helios->SetVideoTransmissionType(type);
}

extern "C" DLLEXPORT void HSendMouseLClick(int x, int y)
{
	if(g_helios)
		g_helios->SendMouseLClick(x,y);
}









// Direct Draw Surface Control
extern "C" DLLEXPORT bool HDDInit(HWND handle, int width, int height)
{
	if(!g_helios)
		return false;

	bool return_value = g_helios->m_ddwrapper->Init(handle,width,height);

	if(return_value)
		g_helios->m_ddwrapper->Black();

	return return_value;
}

extern "C" DLLEXPORT void HDDDraw(IplImage* image)
{
	if(!g_helios)
		return;

	g_helios->m_ddwrapper->DrawImage(image);
}

extern "C" DLLEXPORT void HDDBlack()
{
	if(!g_helios)
		return;
	g_helios->m_ddwrapper->Black();
}

extern "C" DLLEXPORT void HDDRefresh()
{
	if(!g_helios)
		return;
	g_helios->m_ddwrapper->Refresh();
}

extern "C" DLLEXPORT void HDDDestroy()
{
	if(g_helios)
		if(g_helios->m_ddwrapper)
			g_helios->m_ddwrapper->Destroy();			
}

extern "C" DLLEXPORT bool HDDValid()
{
	if(!g_helios)
		return false;
	return g_helios->m_ddwrapper->Valid();
}
