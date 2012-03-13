// Helios.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Helios.h"
#include "USBThread.h"

#include "AVIWriterThread.h"
#include "ReceiverThread.h"
#include "USBThread.h"

#include "FrameTable.h"		
#include "FrameTableEntry.h"		
#include "BufferStore.h"
#include "Register.h"
#include "Text.h"
#include "DirectDrawWrapper.h"

#include "Packet.h"			// Static classes
#include "ImageUtilities.h"	// Static classes


using std::ifstream;



//////////////////////////////////////////////////////
Helios::Helios()
{
	printf("Helios::Helios() \r\n");

	m_bufferstore		= new BufferStore();	

	m_aviwriterthread	= new AVIWriterThread(m_bufferstore);
	m_receiverthread	= new ReceiverThread(this);
	m_usbthread			= new USBThread(m_bufferstore, m_receiverthread);

	m_text				= new Text(100);
	m_frametable		= new FrameTable(1000, 80, m_bufferstore);

	m_ddwrapper			= new CDirectDrawWrapper;
	//read in register names from txt file
	ReadInRegisterNames(string("Registers.txt"));
	
	memset(m_gui_processing_options, 0 , Helios::NUM_GUI_PROCESSING_OPTIONS*sizeof(int));
}

Helios::~Helios(void)
{
	printf("Helios::~Helios() \r\n");

	this->Stop();

	// DO NOT do this!! if this deconstructor is being called, it is probably too late!!!
	// The threads will have already stopped running and will not receive any more messages
	//this->StopAndWait(); // This call will wait until the threads have all stopped
	
	//if (m_frametable)		delete m_frametable;
	//if (m_text)				delete m_text;

	//if (m_usbthread)		delete m_usbthread;
	//if (m_receiverthread)	delete m_receiverthread;
	//if (m_aviwirterthread)	delete m_aviwriterthread;
	
	//if (m_bufferstore)		delete m_bufferstore;
}



void Helios::Start()
{
	//printf("Helios::start() \r\n");	
	m_receiverthread->Start();
	m_usbthread->Start();
}

void Helios::Stop()
{
	printf("Helios::stop() \r\n");
	
	m_usbthread->Stop();		// This call will NOT wait until the threads have all stopped
	m_receiverthread->Stop();	// This call will NOT wait until the threads have all stopped	
}

void Helios::StopAndWait()
{
	//printf("Helios::StopAndWait() \r\n");

	m_usbthread->StopAndWait();		 // This call will wait until the threads have all stopped	
	m_receiverthread->StopAndWait(); // This call will wait until the threads have all stopped	
	cvDestroyAllWindows();
}



size_t Helios::TextSize()
{ 
	return m_text->size();
}

string Helios::GetText()
{ 
	return m_text->get(); 
}



void Helios::SendRegisterChar(int index, unsigned char value)
{
	uint32 buffer32[2];
	buffer32[0] = Packet::SwitchEndian(index);
	buffer32[1] = Packet::SwitchEndian(value);

	SendPacket(PKT_REG_DATA_CHAR, 8, (unsigned char*)buffer32);
}

void Helios::SendRegisterInt(int index, int value)
{
	uint32 buffer32[2];
	buffer32[0] = Packet::SwitchEndian(index);
	buffer32[1] = Packet::SwitchEndian(value);

	SendPacket(PKT_REG_DATA_INT, 8, (unsigned char*)buffer32);
}

void Helios::SendRegisterUInt(int index, unsigned int value)
{
	uint32 buffer32[2];
	buffer32[0] = Packet::SwitchEndian(index);
	buffer32[1] = Packet::SwitchEndian(value);

	SendPacket(PKT_REG_DATA_UINT, 8, (unsigned char*)buffer32);
}

void Helios::SendRegisterHex(int index, unsigned int value)
{
	uint32 buffer32[2];
	buffer32[0] = Packet::SwitchEndian(index);
	buffer32[1] = Packet::SwitchEndian(value);

	SendPacket(PKT_REG_DATA_HEX, 8, (unsigned char*)buffer32);
}

void Helios::SendRegisterFloat(int index, float value)
{
	fint fi;
	fi.f = value;

	uint32 buffer32[2];
	buffer32[0] = Packet::SwitchEndian(index);
	buffer32[1] = Packet::SwitchEndian(fi.u);

	SendPacket(PKT_REG_DATA_FLOAT, 8, (unsigned char*)buffer32);
}




void Helios::EnableRunning(bool on)
{
	if(on)
		SendPacket(PKT_CMD_START,0,NULL);
	else 
		SendPacket(PKT_CMD_STOP,0,NULL);
}

void Helios::EnableVideoTransmission(bool on)
{
	if(on)
		SendPacket(PKT_CMD_VIDEO_ON,0,NULL);
	else 
		SendPacket(PKT_CMD_VIDEO_OFF,0,NULL);
}

void Helios::EnableStateTransmission(bool on)
{
	if(on)
		SendPacket(PKT_CMD_STATE_ON,0,NULL);
	else 
		SendPacket(PKT_CMD_STATE_OFF,0,NULL);
}

void Helios::SetVideoTransmissionType(int type)
{
	uint32 buffer32[1];
	buffer32[0] = Packet::SwitchEndian(type);	

	SendPacket(PKT_CMD_TRANS_FRAME_OPTION, 4, (unsigned char*)buffer32);
}

void Helios::Shutter()
{	
	static int count = 0;

	Buffer* buffer;
	int i = 0;
	do{
		FrameTableEntry* fte = m_frametable->getFTE(i);
		i++;
		
		if(fte != NULL)
			buffer = fte->m_buffers[FrameTableEntry::FTE_BUFFER_IMAGE];
		else 
			buffer = NULL;

	} while(buffer == NULL && i < m_frametable->size());

	if(buffer != NULL)
	{
		if(buffer->image != NULL)
		{
			string avifilename = m_aviwriterthread->GetFilename();			
			int loc = (int) avifilename.find_last_of('\\');
			
			string substr = avifilename.substr(0,loc+1);
		
			char str[256];
			sprintf_s(str, 256, "image%03d.bmp",count);
			substr = substr.append(str);

			count++;
			cvSaveImage(substr.c_str(), buffer->image);

			printf(avifilename.c_str());printf("\r\n");			
			printf(substr.c_str()); printf("\r\n");
		}
		else 
			printf("image was null\r\n");
	}
	else
		printf("buffer was null %d\r\n",i);
}


void Helios::SendMouseLClick(int x, int y)
{
	uint32 buffer32[2];
	buffer32[0] = Packet::SwitchEndian(x);
	buffer32[1] = Packet::SwitchEndian(y);

	SendPacket(PKT_LMOUSE_CLICK, 2*sizeof(uint32), (unsigned char*)buffer32);
}


void Helios::SendPacket(unsigned char type, int buffer_size, unsigned char* data)
{
	//printf("SendPacket(%u,%u)\r\n",type,buffer_size);	
	//printf("Data: ");
	//for(int i = 0 ; i < buffer_size ; i++)
	//	printf(" %x",data[i]);
	//printf("\r\n");

	HeliosPacketHeader header;
	Packet::LoadHeader(&header, type, buffer_size, data);

	uint8 buffer_header[10];
	Packet::HeaderToBuffer(&header,buffer_header);
	m_usbthread->Send((void*) buffer_header, 10);
	if(buffer_size > 0)
		m_usbthread->Send((void*) data, buffer_size);
}








void Helios::receive(Buffer* buffer)
{
	//printf("\t\t\t\tHelios::receive() \r\n");
	
	/* Steps:
	 * 1: Parse the buffer into: HeliosPacketHeader and a data buffer and verify the packet
	 * 2: Pass the packet through a case statement to handle the packet
	 * 3: Release the packet back to the BudfferStore
	 */


	//Step 1: Parse the buffer into: HeliosPacketHeader and a data buffer and verify the packet
	HeliosPacketHeader header;
	void* data;
	bool packet_valid = Packet::ParseAndVerifyBuffer(buffer->data, &header, &data);

	if(!packet_valid)
	{
		printf("\t\t\t\tHelios::receive() ParseAndVerifyBuffer() Failed.\r\n");
		m_bufferstore->CheckIn(&buffer);
		return;
	}

	//Step 2: Pass the packet through a case statement to handle the packet
	switch(header.type){		
	case PKT_IMG_NULL:
	case PKT_IMG_GRAYSCALE8:
	case PKT_IMG_GRAYSCALE16:
	case PKT_IMG_RGB565:
	case PKT_IMG_BINARY:
	case PKT_IMG_COLOR_SEGMENTED:
	case PKT_IMG_FEATURE8:
	case PKT_IMG_FEATURE16:
	case PKT_IMG_HSV1688:
	case PKT_IMG_YUV:
	case PKT_IMG_FEATURE8_LIST:
	case PKT_IMG_FEATURE16_LIST:
	case PKT_IMG_RANK_6_BIT:
	case PKT_IMG_RANK_8_BIT:
	case PKT_IMG_CCSTREAK_LIST:	
	case PKT_IMG_CCSTREAK0_LIST:
	case PKT_IMG_CCSTREAK1_LIST:
	case PKT_IMG_CCSTREAK2_LIST:
	case PKT_IMG_CCSTREAK3_LIST:
	case PKT_IMG_CCSTREAK4_LIST:
	case PKT_IMG_CCSTREAK5_LIST:
	case PKT_IMG_CCSTREAK6_LIST:
	case PKT_IMG_CCSTREAK7_LIST:		
		{
			Buffer* imagebuffer = m_bufferstore->CheckOut(BufferStore::BUF_TYPE_IMAGE_640_480_4B);												

			if(imagebuffer == NULL)
			{
				printf("Helios::receive(): Failed to Checkout Image buffer.\r\n");
				break;
			}
			
			bool rv = ImageUtilities::BufferToImage(&header, data, imagebuffer->image, header.type);

			if(rv)
			{
				if(m_frametable->getFTE(0)->m_buffers[FrameTableEntry::FTE_BUFFER_IMAGE] == NULL)
					m_frametable->getFTE(0)->m_buffers[FrameTableEntry::FTE_BUFFER_IMAGE] = imagebuffer;
				else
				{
					printf("Helios::receive(): FTE Already had an image??? \r\n");
					m_bufferstore->CheckIn(&imagebuffer);
				}
			}
			else
			{
					printf("Helios::receive(): Call to ImageUtilities::BufferToImage() Failed \r\n");
					m_bufferstore->CheckIn(&imagebuffer);
			}
		}
		break;
		
	case PKT_TEXT:						m_text->add(string((char*)data));											break;
		
	case PKT_CMD_START:					/* Only to send to helios board*/											break; 
	case PKT_CMD_STOP:					/* Only to send to helios board*/											break; 
	case PKT_CMD_VIDEO_ON:				/* Only to send to helios board*/											break;
	case PKT_CMD_VIDEO_OFF:				/* Only to send to helios board*/											break;
	case PKT_CMD_STATE_ON:				/* Only to send to helios board*/											break;
	case PKT_CMD_STATE_OFF:				/* Only to send to helios board*/											break;
	case PKT_CMD_TRANS_FRAME_OPTION:	/* Only to send to helios board*/											break;
	case PKT_CMD_GUI_COMM_CHANNEL:		/* Only to send to helios board*/											break;
		
	case PKT_FRAME_TABLE_ENTRY_SMALL:	
		{
			FrameTableEntry	fte;
			uint32* data32 = (uint32*) data;
			fte.m_id		  = Packet::SwitchEndian(data32[0]);
			fte.m_ms		  = Packet::SwitchEndian(data32[1]);
			fte.m_frameCount  = Packet::SwitchEndian(data32[2]);
			fte.m_saveoptions = Packet::SwitchEndian(data32[3]);
			m_frametable->addFTE(&fte);
		}		
		break;

	case PKT_FRAME_TABLE_ENTRY_FULL:	
		{
			FrameTableEntry* tablefte = m_frametable->getFTE(0);
			uint32* data32 = (uint32*) data;
			if(tablefte->m_id == Packet::SwitchEndian(data32[0]))
			{
				//Fill in the rest of the FTE
				//tablefte.m_id		       = Packet::SwitchEndian(data32[0]);
				//tablefte.m_ms		       = Packet::SwitchEndian(data32[1]);
				//tablefte.m_frameCount    = Packet::SwitchEndian(data32[2]);
				//tablefte.m_saveoptions   = Packet::SwitchEndian(data32[3]);
				tablefte->m_status		   = Packet::SwitchEndian(data32[4]);
				tablefte->m_numFeatures	   = Packet::SwitchEndian(data32[5]);
				tablefte->m_maxHarrisValue = Packet::SwitchEndian(data32[6]);
				//...
			} 
			else 
			{
				FrameTableEntry	fte;
				fte.m_id		  = Packet::SwitchEndian(data32[0]);
				fte.m_ms		  = Packet::SwitchEndian(data32[1]);
				fte.m_frameCount  = Packet::SwitchEndian(data32[2]);
				fte.m_saveoptions = Packet::SwitchEndian(data32[3]);
				m_frametable->addFTE(&fte);
			}

			// Now draw correlations on image and then display the image and maybe write it out to an AVI
			if(tablefte->m_buffers[FrameTableEntry::FTE_BUFFER_IMAGE] == NULL)
			{
				Buffer* imagebuffer = m_bufferstore->CheckOut(BufferStore::BUF_TYPE_IMAGE_640_480_4B);
				if(imagebuffer != NULL) 
				{
					cvSet(imagebuffer->image,cvScalar(0,0,0,0));
					tablefte->m_buffers[FrameTableEntry::FTE_BUFFER_IMAGE] = imagebuffer;
				}
			}

			if(tablefte->m_buffers[FrameTableEntry::FTE_BUFFER_IMAGE] != NULL) 
			{
				Buffer* imagebuffer = tablefte->m_buffers[FrameTableEntry::FTE_BUFFER_IMAGE];

				if(m_gui_processing_options[Helios::GUI_PROCESSING_DISPLAY_CORRELATIONS])
					if(tablefte->m_buffers[FrameTableEntry::FTE_BUFFER_CORRELATIONS] != NULL)
						ImageUtilities::DrawCorrelations(imagebuffer->image, tablefte->m_buffers[FrameTableEntry::FTE_BUFFER_CORRELATIONS]->mat);			

				m_ddwrapper->DrawImage(imagebuffer->image);	

				if(m_gui_processing_options[Helios::GUI_PROCESSING_WRITE_TO_AVI])
				{
					if(m_aviwriterthread->IsRunning())
					{
						m_aviwriterthread->Write(imagebuffer);
						tablefte->m_buffers[FrameTableEntry::FTE_BUFFER_IMAGE] = NULL;
					}
				}
			}
		}
		break;

	case PKT_THRESHOLDS:				/* Only to send to helios board*/											break;

	case PKT_COORDINATES:																							break;

	case PKT_CONN_COMP:					
		{
		
		}
		break;

	case PKT_CORRELATION:				
		{												
			uint8*	data8  = (uint8*) data;
			Buffer*	corbuf = m_bufferstore->CheckOut(BufferStore::BUF_TYPE_TRACKED_FEATURES_LIST);   
			if(corbuf != NULL && corbuf->mat != NULL)
			{
				int num = header.buffer_size / 16; // Number of Correlations in the packet
				corbuf->mat->rows = num;
				for(int i=0, k=0 ; i < num ; i++, k+=16)
				{
					uint16 x	 = data8[k+0]  <<  8 | data8[k+1];
					uint16 y	 = data8[k+2]  <<  8 | data8[k+3];
					uint16 tx    = data8[k+4]  <<  8 | data8[k+5];
					uint16 ty    = data8[k+6]  <<  8 | data8[k+7];
					uint32 error = data8[k+8]  << 24 | data8[k+9]  << 16 | data8[k+10] << 8 | data8[k+11];
					uint32 other = data8[k+12] << 24 | data8[k+13] << 16 | data8[k+14] << 8 | data8[k+15];
					cvmSet(corbuf->mat, i, 0, x);
					cvmSet(corbuf->mat, i, 1, y);
					cvmSet(corbuf->mat, i, 2, tx);
					cvmSet(corbuf->mat, i, 3, ty);
					cvmSet(corbuf->mat, i, 4, error);													
					cvmSet(corbuf->mat, i, 5, other);
				}

				FrameTableEntry* tablefte = m_frametable->getFTE(0);
				tablefte->m_buffers[FrameTableEntry::FTE_BUFFER_CORRELATIONS] = corbuf;												
			}
		}
		break;

	case PKT_PARTICLES:			
		{
			uint8*				data8		= (uint8*) data;
			uint32*				data32		= (uint32*) data;
			FrameTableEntry*	tablefte	= m_frametable->getFTE(0);
			Buffer*				buf			= tablefte->m_buffers[FrameTableEntry::FTE_BUFFER_IMAGE];
			int					num			= header.buffer_size / 40;
			int					side		= 5;
			int					thickness   = 3;
			
			if(buf == NULL)
			{
				buf = m_bufferstore->CheckOut(BufferStore::BUF_TYPE_IMAGE_640_480_4B);
				if(buf != NULL)
				{
					cvZero(buf->image);
					tablefte->m_buffers[FrameTableEntry::FTE_BUFFER_IMAGE] = buf;
				}
			}


			if(buf != NULL)
			{
				IplImage *image = buf->image;

				//printf("size %d num %d\r\n",header.buffer_size, header.buffer_size/40);
				for(int i=0, k = 0 ; i < num ; i++)
				{
					uint32 cur_locx	= data8[k+ 0] << 24 | data8[k+ 1] << 16 | data8[k+ 2] << 8 | data8[k+3];
					uint32 cur_locy	= data8[k+ 4] << 24 | data8[k+ 5] << 16 | data8[k+ 6] << 8 | data8[k+7];
					uint32 new_locx	= data8[k+ 8] << 24 | data8[k+ 9] << 16 | data8[k+10] << 8 | data8[k+11];
					uint32 new_locy	= data8[k+12] << 24 | data8[k+13] << 16 | data8[k+14] << 8 | data8[k+15];
					uint32 pst_locx	= data8[k+16] << 24 | data8[k+17] << 16 | data8[k+18] << 8 | data8[k+19];
					uint32 pst_locy	= data8[k+20] << 24 | data8[k+21] << 16 | data8[k+22] << 8 | data8[k+23];
					uint32 cur_age	= data8[k+24] << 24 | data8[k+25] << 16 | data8[k+26] << 8 | data8[k+27];
					uint32 org_age	= data8[k+28] << 24 | data8[k+29] << 16 | data8[k+30] << 8 | data8[k+31];
					int    x		= (int) cur_locx;
					int    y		= (int) cur_locy;
					int    age		= (int) cur_age;
					int	   threat   = (int) new_locy;
					
					if(threat == -1)
					{
						cvRectangle(	image, 
										cvPoint(x-side, y-side), 
										cvPoint(x+side, y+side), 
										CV_RGB(255,0,0),
										thickness);						
					}

					//printf("x:%d y:%d\r\n",x,y);
					k += 40;
				}			
			}
		}



		//for(int i=0, k=0 ; i < num ; i++, k+=40)
		//{
			
			//
			//int grav		= Packet::SwitchEndian(data[k+3]);
			//int age			= Packet::SwitchEndian(data[k+4]);
			//int com_sumx	= Packet::SwitchEndian(data[k+5]);
			//int com_sumy	= Packet::SwitchEndian(data[k+6]);
			//int com_count	= Packet::SwitchEndian(data[k+7]);
			//int pastx		= Packet::SwitchEndian(data[k+8]);				
			//int pasty		= Packet::SwitchEndian(data[k+9]);

			
		
			/*
			cvRectangle(	tablefte->m_buffers[FrameTableEntry::FTE_BUFFER_IMAGE]->image, 
							cvPoint(x-side, y-side), 
							cvPoint(x+side, y+side), 
							CV_RGB(255,0,0));	*/
		//}

		break;


	case PKT_LMOUSE_CLICK:																							break;
	case PKT_RMOUSE_CLICK:																							break;

	//GUI has requested a refresh of the State Data	
	case PKT_REQ_REGISTER_DATA:			/* Only to send to helios board*/											break;

	// These 4 are used to set specfic registers
	case PKT_REG_DATA_CHAR:				/*The Helios board never sends registers one at a time*/					break;
	case PKT_REG_DATA_INT:				/* these are only to send registers to the Helios from the GUI*/			break;
	case PKT_REG_DATA_UINT:																							break;
	case PKT_REG_DATA_HEX:																							break;
	case PKT_REG_DATA_FLOAT:																						break;

	
	// Just the data arrays
	case PKT_REG_DATA_ARRAY_CHAR:		m_register_char.setAllData( header.buffer_size/1,  (uint8*)data);			break;
	case PKT_REG_DATA_ARRAY_INT:		m_register_int.setAllData(  header.buffer_size/4,    (int*)data);			break;
	case PKT_REG_DATA_ARRAY_UINT:		m_register_uint.setAllData( header.buffer_size/4, (uint32*)data);			break;
	case PKT_REG_DATA_ARRAY_HEX:        m_register_hex.setAllData(  header.buffer_size/4, (uint32*)data);			break;
	case PKT_REG_DATA_ARRAY_FLOAT:		m_register_float.setAllData(header.buffer_size/4,  (float*)data);			break;
																												
	default: break;
	}	


	//Step 3: Release the packet back to the BudfferStore
	//printf("checking buffer %x\r\n",buffer);
	m_bufferstore->CheckIn(&buffer);	
}


//*
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

void Helios::ReadInRegisterNames(string filename)
{
	ifstream fin;
	
	fin.open(filename.c_str());
	if(fin.fail())
	{
		printf("ifstream failed.\r\n");		

		fin.close();
		fin.clear();
		fin.open(string("..\\").append(filename).c_str());
		if(fin.fail())
			printf("ifstream failed again.\r\n");
	}

	string str;

	while(getline(fin,str)) 
	{		
		switch(str[0]){
		case 'C':	m_register_char.addString(str);		break;
		case 'I':	m_register_int.addString(str);		break;
		case 'U':	m_register_uint.addString(str);		break;
		case 'X':	m_register_hex.addString(str);		break;
		case 'F':	m_register_float.addString(str);	break;
		default:	printf("Helios::ReadInRegisterNames() Error. Register name in Registers.txt does not start with C, I, U, X, or F"); break;
		}    
	}	
}



