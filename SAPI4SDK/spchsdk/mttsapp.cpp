/*


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

// TTSAPP.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CTTSAPPApp

BEGIN_MESSAGE_MAP(CTTSAPPApp, CWinApp)
	//{{AFX_MSG_MAP(CTTSAPPApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTTSAPPApp construction

CTTSAPPApp::CTTSAPPApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTTSAPPApp object

CTTSAPPApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTTSAPPApp initialization

BOOL CTTSAPPApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    // init OLE
    if (FAILED(CoInitialize(NULL))) return FALSE;

	CTTSAPPDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

    CoUninitialize ();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
