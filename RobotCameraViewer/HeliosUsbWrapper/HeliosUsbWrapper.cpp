// This is the main DLL file.
// This is the main DLL file.
// Based on approach #3 in http://social.msdn.microsoft.com/forums/en-US/vcgeneral/thread/299da822-5539-4e5b-9ba7-b614e564c9f4/

#pragma once

#include "stdafx.h"
#include "HeliosUsbWrapper.h"
#include "HeliosUSB.h"

using namespace System;

namespace HeliosUsbWrapper
{
	public ref class HeliosUsb
	{
	public:
		static bool Open(IntPtr hwnd) {
			return USBOpenHelios((HWND)hwnd.ToPointer());
		}
		static void Close(void) {
			USBCloseHelios();
		}
		static bool Write(IntPtr data, int numBytes) {
			return USBWriteHelios(data.ToPointer(), numBytes);
		}
		static int Read(IntPtr dest) {
			int numBytesRead;
			if (!USBReadHelios(dest.ToPointer(), (DWORD*)&numBytesRead)) {
				throw gcnew Exception("Error reading from USB");
			}

			return numBytesRead;
		}
	};
}

/*
		Port95NT () 
		{
		};

		static bool IsInstalled()
		{
			bool installed = false;
			HMODULE hLib = LoadLibraryA("dlportio.dll");	// Port95NT installer places this in \windows\system32
			if(hLib != NULL)
			{
				FARPROC proc = GetProcAddress(hLib, "DlPortReadPortUchar");	// Choose any function...
				installed = (proc != NULL);
				FreeLibrary(hLib);
			}
			return installed;
		}

	};

	public ref class LPT1
	{
	public:
		LPT1 () 
		{
		};

		// A parallel port's output register is at the "base" address (0x378 for LPT1) and it's input register is at base+1
		static DWORD WriteAddr = 0x378;
		static DWORD ReadAddr = WriteAddr+1;
	};

	public ref class U8 
	{
	public:
		U8 () 
		{
		};

		static UCHAR Read(ULONG port)
		{
			return DlPortReadPortUchar(port);
		}
		static void ReadArray(ULONG port, PUCHAR Buffer, ULONG Count)
		{
			DlPortReadPortBufferUchar(port, Buffer, Count);
		}

		static void Write(ULONG port, UCHAR Value)
		{
			DlPortWritePortUchar(port, Value);
		}

		static void WriteArray(ULONG port, PUCHAR Buffer, ULONG Count)
		{
			DlPortWritePortBufferUchar(port, Buffer, Count);
		}
	};

	public ref class U16
	{
	public:
		U16 () 
		{
		};

		static USHORT Read(ULONG port)
		{
			return DlPortReadPortUshort(port);
		}

		static void ReadU16Array(ULONG port, PUSHORT Buffer, ULONG Count)
		{
			DlPortReadPortBufferUshort(port, Buffer, Count);
		}

		static void Write(ULONG port, USHORT Value)
		{
			DlPortWritePortUshort(port, Value);
		}

		static void WriteArray(ULONG port, IN PUSHORT Buffer, ULONG Count)
		{
			DlPortWritePortBufferUshort(port, Buffer, Count);
		}

	};

	public ref class U32
	{
	public:
		U32 () 
		{
		};

		static ULONG Read(ULONG port)
		{
			return DlPortReadPortUlong(port);
		}

		static void ReadArray(ULONG port, PULONG Buffer, ULONG Count)
		{
			DlPortReadPortBufferUlong(port, Buffer, Count);
		}

		static void Write(ULONG port, ULONG Value)
		{
			DlPortWritePortUlong(port, Value);
		}

		static void WriteArray(ULONG port, PULONG Buffer, ULONG Count)
		{
			DlPortWritePortBufferUlong(port, Buffer, Count);
		}


	};
}
*/