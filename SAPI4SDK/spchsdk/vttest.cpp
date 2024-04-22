/******************************************************************************\
*       vttest.cpp
*
*       Defines the class behaviors for the application.
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "tabdlgs.h"
#include <bufcvt.h>
#include "vtobj.h"

#include "vttest.h"
#include "vttesdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVTTestApp

BEGIN_MESSAGE_MAP(CVTTestApp, CWinApp)
	//{{AFX_MSG_MAP(CVTTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVTTestApp construction

CVTTestApp::CVTTestApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVTTestApp object

CVTTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVTTestApp initialization

BOOL CVTTestApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	Enable3dControls();
//	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Initialize OLE	before creating CVTTestDlg
	if (FAILED(CoInitialize(NULL)))
		return FALSE;

	// Create the main dialog
	CVTTestDlg* pDlg = new CVTTestDlg;
	if(pDlg == NULL)
		return FALSE;

	m_pMainWnd = pDlg;
	// Display the main dialog
	int nResponse = pDlg->DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Delete the main dialog (releases all OLE interfaces)
	delete pDlg;
	// Uninitialize OLE after destroying CPopfndrDlg 
	CoUninitialize ();
	                               
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
