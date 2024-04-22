
/******************************************************************************\
*       FeedBack.cpp
*
*       Implements non-OLE2 elements of the CFeedBackApp class
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
#include "FeedBack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeedBackApp

BEGIN_MESSAGE_MAP(CFeedBackApp, CWinApp)
	//{{AFX_MSG_MAP(CFeedBackApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeedBackApp construction

CFeedBackApp::CFeedBackApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFeedBackApp object

CFeedBackApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFeedBackApp initialization

BOOL CFeedBackApp::InitInstance()
{
	BOOL	bRes;
	
	Enable3dControls();
				 
	//set default delay to 5 seconds (5000 ms)
  	m_iDelay = 5000;

	//Create Splash Dialog
  	CSplashDlg *pSplashDlg = new CSplashDlg;
  	int hResult = pSplashDlg->DoModal();
  	delete pSplashDlg;
  	if (hResult != IDOK) return FALSE;

  	//send out speechstarted msg and modify registry entry
	bRes = StartSpeech();
	if(bRes)
	{
		//Allocate the main dialog window
  		m_pDlg = new CFeedBackDlg;
		m_pMainWnd = m_pDlg;
		gVCmdNotifySink.m_pDlg = m_pDlg;
		
		//create the main dialog window
		m_pDlg->DoModal();
		
		// send out a speechended message and modify registry entry
		StopSpeech();
		
  		//clean up icons
	  	if (m_pDlg->m_bSupTray)
	  	{
	    	m_pTray->Free();
	    	delete m_pTray;
	  	}
	  	else
	  	{
	   		delete m_pIcon;
	  	}
		
		delete m_pDlg;
	}
	else
		MessageBox(NULL,"A navigator application (such as Microsoft Voice) is already running. Please \
close the other navigator application before starting Feedback", "Error", MB_ICONSTOP);


	//release OLE2 interfaces
	EndOle ();
  
  	
	//return FALSE so that the application will exit.
	return FALSE;
}

/*****************************************************************************
* StartSpeech(void)
* 
* This function checks if another navigator application is running. If so, it 
* returns FALSE. If another navigator is not installed StartSpeech creates the
* registry entries and send the WM_SPEECHSTARTED msg. If another navigator is 
* installed but not running StartSpeech	make a change in the registry setting
* and sends the speechstarted msg.
*****************************************************************************/
BOOL CFeedBackApp::StartSpeech(void)
{
	int 	iRes;
	UINT 	uMsg;
	HKEY  	hKey;
	BOOL	bSpeechState = TRUE;
	DWORD	dwDisposition;

	iRes = CheckNavigator();
	if(iRes == 0) // no other navigator is running
	{ 
		//send start message
   		uMsg = RegisterWindowMessage("WM_SPEECHSTARTED");
   		if (uMsg)
      		::PostMessage(HWND_BROADCAST, uMsg, 0, 0L);
		
		//set registry key
		RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Voice", 0, KEY_WRITE, &hKey);
		RegSetValueEx(hKey, "UseSpeech", 0, REG_DWORD, (LPBYTE)&bSpeechState, sizeof(BOOL));
		RegCloseKey (hKey);
		
		return TRUE;
	}
	else if(iRes == -1)// navigator is not installed
	{
		//build key	and set usespeech to TRUE
		RegCreateKeyEx(	HKEY_CURRENT_USER, 
						"Software\\Voice", 
						0, 
						"", 
						REG_OPTION_NON_VOLATILE, 
						KEY_ALL_ACCESS,	 
						NULL, 
						&hKey, 
						&dwDisposition);
		RegSetValueEx(hKey, "UseSpeech", 0, REG_DWORD, (LPBYTE)&bSpeechState, sizeof(BOOL));
		RegCloseKey (hKey);

		return TRUE; 
	};
	// else if(iRes == 1)// another navigator is already running
   	return FALSE;
}

/*****************************************************************************
* StopSpeech(void)
*
* Updates the registry to indicate that the navigator app has exited and 
* sends out the WM_SPEECHENDED message.
*****************************************************************************/
void CFeedBackApp::StopSpeech(void)
{
	UINT 	uMsg;
	HKEY  	hKey;
	BOOL	bSpeechState = FALSE;

	//set registry key to 0
	RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Voice", 0, KEY_WRITE, &hKey);
	RegSetValueEx(hKey, "UseSpeech", 0, REG_DWORD, (LPBYTE)&bSpeechState, sizeof(BOOL));	
	RegCloseKey (hKey);
	
	//send out stop message
	uMsg = RegisterWindowMessage("WM_SPEECHENDED");
   	if (uMsg)
    	::PostMessage(HWND_BROADCAST, uMsg, 0, 0L);

}

/****************************************************************************
*	CheckNavigator:
*
*	Checks the registry entries to see if a navigator application
*	has been installed on the machine. If the Navigator is installed
*	CheckNavigator returns its state(0 [not running], 1 [running]) else if no 
*	navigator is found it returns -1.
****************************************************************************/
int CFeedBackApp::CheckNavigator(void)
{
	HKEY  hKey;
	DWORD dwType=REG_DWORD, dwSize=sizeof(DWORD), dwVal;

	if( RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Voice", 0, KEY_READ, &hKey) != ERROR_SUCCESS )
		return -1;
	
	if( RegQueryValueEx (hKey, "UseSpeech", 0, &dwType, (LPBYTE)&dwVal, &dwSize) != ERROR_SUCCESS )
		return -1;
	
	RegCloseKey (hKey);

	return (int)dwVal;
}

