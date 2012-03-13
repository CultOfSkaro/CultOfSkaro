// HeliosLibrary.h : main header file for the HeliosLibrary DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CHeliosLibraryApp
// See HeliosLibrary.cpp for the implementation of this class
//
class CHeliosLibraryApp : public CWinApp
{
public:
	CHeliosLibraryApp();
	~CHeliosLibraryApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
