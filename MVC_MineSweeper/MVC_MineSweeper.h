
// MVC_MineSweeper.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Controller.h"


// CMVC_MineSweeperApp:
// See MVC_MineSweeper.cpp for the implementation of this class
//

class CMVC_MineSweeperApp : public CWinApp
{
public:
	CMVC_MineSweeperApp();
	~CMVC_MineSweeperApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
private:
	Model *model;
	Controller *cont;
public:
	Model* GetModel()
	{
		return model;
	}
	Controller* GetController()
	{
		return cont;
	}
};

extern CMVC_MineSweeperApp theApp;