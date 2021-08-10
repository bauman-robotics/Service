
// MfcApplication.h : main header file for the MfcApplication application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMfcApplicationApp:
// See MfcApplication.cpp for the implementation of this class
//

class CMfcApplicationApp : public CWinAppEx
{
public:
	CMfcApplicationApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMfcApplicationApp theApp;
