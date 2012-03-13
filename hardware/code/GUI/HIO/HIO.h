// HIO.h : main header file for the HIO application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CHIOApp:
// See HIO.cpp for the implementation of this class
//

class CHIOApp : public CWinApp
{
public:
	CHIOApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};

extern CHIOApp theApp;