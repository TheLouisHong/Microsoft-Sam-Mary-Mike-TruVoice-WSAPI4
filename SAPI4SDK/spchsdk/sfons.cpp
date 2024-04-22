/******************************************************************************\
*       Icons.cpp
*
*       Implementation file for the Windows 3.1 style feedback class
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

//the one and only CFeedBackApp object. (defined in Feedback.cpp) 
extern CFeedBackApp theApp;

CIcons::CIcons (void)
{
  ;
}

CIcons::~CIcons (void)
{
  ;
}

BOOL CIcons::InitIcons (HINSTANCE hInstance)
{
	LoadString (hInstance, IDS_LISTEN, m_strCaption[LISTEN].szCaption, sizeof(m_strCaption->szCaption));
	LoadString (hInstance, IDS_SLEEP,  m_strCaption[SLEEP].szCaption,  sizeof(m_strCaption->szCaption));
	LoadString (hInstance, IDS_OFF,    m_strCaption[OFF].szCaption,    sizeof(m_strCaption->szCaption));
	LoadString (hInstance, IDS_NOTREC, m_strCaption[NOTREC].szCaption, sizeof(m_strCaption->szCaption));
	LoadString (hInstance, IDS_RECOG,  m_strCaption[RECOG].szCaption,  sizeof(m_strCaption->szCaption));
	LoadString (hInstance, IDS_THINK,  m_strCaption[THINK].szCaption,  sizeof(m_strCaption->szCaption));

  m_Icons[LISTEN] = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_LISTEN32));
	m_Icons[SLEEP]  = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_SLEEP32));
	m_Icons[OFF]    = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_OFF32));
	m_Icons[NOTREC] = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_NOTREC32));
	m_Icons[RECOG]  = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_RECOG32));
	m_Icons[THINK]  = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_THINK32));

  return TRUE;
}

void CIcons::SetRecognizedCommand (PSTR szCommand)
{
  char buff[1000];
  sprintf (buff, "Recognized: %s", szCommand);
  buff[63]='\0';
  sprintf (m_strCaption[RECOG].szCaption, "%s", buff);
}
