/******************************************************************************\
*       srtest.cpp : Defines the class behaviors for the application.
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

#ifndef _SPEECH_
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <speech.h>
#endif  // _SPEECH_

#include "srtest.h"
#include "srdlg.h"
#include "srfunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSRTestApp

BEGIN_MESSAGE_MAP(CSRTestApp, CWinApp)
	//{{AFX_MSG_MAP(CSRTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSRTestApp construction

CSRTestApp::CSRTestApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSRTestApp object

CSRTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSRTestApp initialization

BOOL CSRTestApp::InitInstance()
{
	DWORD dwVer;

	BOOL fVer = GetVersion() & 0x80000000;
	if( sizeof(TCHAR) == 2 && fVer )
	{
		MessageBoxA( NULL, "This version of windows doesn't support the UNICODE runtime api.", NULL, MB_OK );
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	Enable3dControls();
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	CSRTestDlg dlg;
	m_pMainWnd = &dlg;

	// init OLE...
	SetMessageQueue(96);
	dwVer = CoBuildVersion();

	if( rmm != HIWORD(dwVer)       ) return FALSE;   // error
	if( FAILED(CoInitialize(NULL)) ) return FALSE;

	// tell MFC to get .ini profiles from the registry instead of .ini files,
	// the registry key is HKEY_CURRENT_USER\software\microsoft\srtest,
	// if I don't call this then all profiles returned with AfxGetApp()->GetProfileString()
	// will be taken from the actual .ini files. 
	SetRegistryKey( TEXT("Microsoft") );

	int nResponse = dlg.DoModal();

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
