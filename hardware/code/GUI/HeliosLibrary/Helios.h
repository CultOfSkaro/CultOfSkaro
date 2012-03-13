// Helios.h : main header file for the Helios DLL
//

#pragma once

#include "stdafx.h"
#include "BufferStore.h"

using std::string;

class BufferStore;

class AVIWriterThread;
class ReceiverThread;
class USBThread;

class Text;
class FrameTable;

class CDirectDrawWrapper;
#include "Register.h"

//template <typename T> class Register;


class Helios
{
public:
	Helios();
	~Helios(void);

	enum EGUIProcessingOptions {
		GUI_PROCESSING_WRITE_TO_AVI,	
		GUI_PROCESSING_DISPLAY_COORDINATES,
		GUI_PROCESSING_DISPLAY_CORRELATIONS,
		NUM_GUI_PROCESSING_OPTIONS
	};

	void Start();
	void Stop();
	void StopAndWait();
	
	size_t	TextSize(); 
	string	GetText();	


	void SendRegisterChar( int index, unsigned char value);
	void SendRegisterInt(  int index, int		    value);
	void SendRegisterUInt( int index, unsigned int  value);
	void SendRegisterHex(  int index, unsigned int  value);
	void SendRegisterFloat(int index, float         value);

	void EnableRunning(bool on);
	void EnableVideoTransmission(bool on);
	void EnableStateTransmission(bool on);
	void SetVideoTransmissionType(int type);
	void Shutter();
	void SendMouseLClick(int x, int y);

	void SendPacket(unsigned char type, int buffer_size, unsigned char* data);
	
	// What kind of public API should be here ? Or should the stuff just be public	

protected: 
	void receive(Buffer* buffer); // Called by ReceiverThread which is a friend class
	void ReadInRegisterNames(string filename);	

public: 
	BufferStore*			m_bufferstore;
	
	AVIWriterThread* 		m_aviwriterthread;
	ReceiverThread*			m_receiverthread;
	USBThread*				m_usbthread;

	Text*					m_text;
	FrameTable*				m_frametable;	
	Register<uint8>			m_register_char;
	Register<int>			m_register_int;
	Register<uint32>		m_register_uint;
	Register<uint32>		m_register_hex;
	Register<float>			m_register_float;

	CDirectDrawWrapper*		m_ddwrapper;

	int						m_gui_processing_options[NUM_GUI_PROCESSING_OPTIONS];

	friend class ReceiverThread;
	friend class CHeliosLibraryApp;
};
//*/