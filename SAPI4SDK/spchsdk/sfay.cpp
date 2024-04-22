/******************************************************************************\
*       Tray.cpp
*
*       Source file for FEEDBACK.EXE, a demonstration of the Microsoft
*       Speech Application Programming Interface.
*
*       Implementation file for the tray feedback class
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

CTrayFeedBack::CTrayFeedBack()
{
  m_bUseTray=FALSE;
  fpNotifyIcon=NULL;
}


BOOL CTrayFeedBack::InitTray (HINSTANCE hInstance, HWND hWnd)
{
	//Find out whether the operating system supports the Windows Tray
  //and, if supported, initialize data to use tray

  m_hShell32=LoadLibrary ("shell32.dll");
  if (m_hShell32) 
	{
		//SetErrorMode so that system won't bring up error dialog when
		//attempting to access shell32.dll::Shell_NotifyIcon();
		m_ErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

		//Attempt to get a pointer to the Shell_NotifyIcon function
    fpNotifyIcon=(FPNOTIFYICON)(GetProcAddress (m_hShell32, "Shell_NotifyIcon"));

		//return Error Mode to previous state
		SetErrorMode (m_ErrorMode);
		
		//If tray update function does not exist, free Shell32.dll and return
		if(!fpNotifyIcon) return (FALSE);
	}

	//Initialize the m_IconStr Structure
	for (m_loop=LISTEN; m_loop <=THINK; m_loop++) 
	{
		m_IconStr[m_loop].cbSize           = sizeof(NOTIFYICONDATA);
		m_IconStr[m_loop].hWnd             = hWnd;
		m_IconStr[m_loop].uID              = NULL;
	  m_IconStr[m_loop].uFlags           = NIF_MESSAGE|NIF_ICON|NIF_TIP;
		m_IconStr[m_loop].uCallbackMessage = WM_NOTIFYICON;
	}
	
	//Load tooltips strings into the icon structure array.
	LoadString (hInstance, IDS_LISTEN, m_IconStr[LISTEN].szTip, sizeof(m_IconStr->szTip));
	LoadString (hInstance, IDS_SLEEP,  m_IconStr[SLEEP].szTip,  sizeof(m_IconStr->szTip));
	LoadString (hInstance, IDS_OFF,    m_IconStr[OFF].szTip,    sizeof(m_IconStr->szTip));
	LoadString (hInstance, IDS_NOTREC, m_IconStr[NOTREC].szTip, sizeof(m_IconStr->szTip));
	LoadString (hInstance, IDS_RECOG,  m_IconStr[RECOG].szTip,  sizeof(m_IconStr->szTip));
	LoadString (hInstance, IDS_THINK,  m_IconStr[THINK].szTip,  sizeof(m_IconStr->szTip));

  //Load 16x16 icons for display in the tray.
	m_IconStr[LISTEN].hIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_LISTEN16));
	m_IconStr[SLEEP].hIcon  = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_SLEEP16));
	m_IconStr[OFF].hIcon    = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_OFF16));
	m_IconStr[NOTREC].hIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_NOTREC16));
	m_IconStr[RECOG].hIcon  = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_RECOG16));
	m_IconStr[THINK].hIcon  = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_THINK16));

  m_bUseTray = fpNotifyIcon (NIM_ADD, &m_IconStr[THINK]);

  return (m_bUseTray);
}


BOOL CTrayFeedBack::SetIcon (int state)
{
  if ((state < LISTEN) || (state > THINK)) return (FALSE);
  if (m_bUseTray==FALSE) return (FALSE);
  else return (fpNotifyIcon (NIM_MODIFY, &m_IconStr[state]));
}


void CTrayFeedBack::SetRecognizedCommand (PSTR szCommand)
{
  char buff[1000];
  sprintf (buff, "Recognized: %s", szCommand);
  buff[63]='\0';
  sprintf (m_IconStr[RECOG].szTip, "%s", buff);
}


BOOL CTrayFeedBack::Free(void)
{
  return (fpNotifyIcon (NIM_DELETE, &m_IconStr[OFF]));
}


CTrayFeedBack::~CTrayFeedBack(void)
{
  ; //no additional cleanup necessary
}
