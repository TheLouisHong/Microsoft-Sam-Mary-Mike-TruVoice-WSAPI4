/******************************************************************************\
*       ttstst.cpp
*
*       Implements functionality of the application object (CTTStstApp).
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include "stdafx.h"

#include <speech.h>

#include "resource.h"
#include "tabisrce.h"
#include "ttstst.h"
#include "ttsdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTStstApp

BEGIN_MESSAGE_MAP(CTTStstApp, CWinApp)
	//{{AFX_MSG_MAP(CTTStstApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTTStstApp construction

CTTStstApp::CTTStstApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTTStstApp object

CTTStstApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTTStstApp initialization

BOOL CTTStstApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	Enable3dControls();
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	CTTStstDlg dlg;
	m_pMainWnd = &dlg;

	// init OLE...
	DWORD dwVer = CoBuildVersion();
	if (rmm != HIWORD(dwVer)) return FALSE;   // error
	if (FAILED(CoInitialize(NULL))) return FALSE;

	int nResponse = dlg.DoModal();

	CoFreeAllLibraries();

	CoUninitialize ();

	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
