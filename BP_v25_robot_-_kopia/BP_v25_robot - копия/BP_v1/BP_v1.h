// BP_v1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CBP_v1App:
// See BP_v1.cpp for the implementation of this class
//

class CBP_v1App : public CWinApp
{
public:
	CBP_v1App();

// Overrides
	public:
	virtual BOOL InitInstance();

//virtual BOOL PreTranslateMessage(MSG* pMsg); 
	

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CBP_v1App theApp;