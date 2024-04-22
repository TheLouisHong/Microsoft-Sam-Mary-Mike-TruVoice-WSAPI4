/******************************************************************************\
*       srtest.h : main header file for the SRTEST application
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <bufcvt.h>                // UNICODE conversion macros
#include "resource.h"              // main symbols

const int MAX_GRAMMARS     = 100;  // max number of enumerated grammar files on disk
const int MAX_LANGUAGES    = 27;   // number of languages used for the find interface
const int MAX_NAMELEN      = 80;   // number of characters in the name excluding the path info
const int MAX_STRLEN       = 256;  // typical character string size
const int ID_TIMERPOSTRACK = 1;    // ID of the ISRCentral posisition tracking control

#define CursorWait	saveCur = SetCursor( waitCur )
#define CursorGo	SetCursor( saveCur )

typedef struct
{
	TCHAR  szName[MAX_STRLEN];	
} GramName_t;

typedef struct
{
	BOOL    bLinkQuery;
	BOOL    bListQuery;
	TCHAR   szListName[MAX_STRLEN];
	TCHAR   szLinkName[MAX_STRLEN];
	PSRWORD List;
} ISRGramCFG_t;

typedef struct
{
	TCHAR szContextPre[MAX_STRLEN];
	TCHAR szContextPost[MAX_STRLEN];
	WORD  wHint;
	LPSTR lpLikelyWords;
} ISRGramDict_t;

typedef struct
{
	BOOL bPhrase;
	BOOL bBookmark;
	BOOL bPaused;
	BOOL bPhraseHyp;
	BOOL bReEval;
	BOOL bUnarchive;
	BOOL bTraining;
} ISRGramNotFlag_t;

typedef struct
{
	DWORD dwAGainEnable;
	DWORD dwThresh;
	WORD  wEnergyFlr;
	TCHAR szMicName[MAX_STRLEN];
	TCHAR szSpeakName[MAX_STRLEN];
	DWORD dwRealTime;
	BOOL  bEcho;
	DWORD dwTimeOutC;
	DWORD dwTimeOutI;
} ISRAttr_t;

typedef struct
{
	BOOL bAttrChange;
	BOOL bInterfere;
	BOOL bSound;
	BOOL bUtter;
	BOOL bVMeter;
} ISREngNotFlag_t;

extern TCHAR      gszGramPath[];              // the path where the grammar files live
extern int        gnGramFiles;                // the total number of grammar files
extern GramName_t *gpGramFiles;          // the enumerated grammar filenames

// copy TCHAR string to char string...
inline void Unicode2Char( char *to, PTCHAR from, int tosize )
{
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, 0, from, -1, to, tosize, NULL, NULL );
#else
	strcpy( to, from );
#endif
}

// copy char string to TCHAR string...
inline void Char2Unicode( PTCHAR to, char *from, int tosize )
{
#ifdef _UNICODE
	MultiByteToWideChar(CP_ACP, 0, from, -1, to, tosize );
#else
	strcpy( to, from );
#endif
}


/////////////////////////////////////////////////////////////////////////////
// CSRTestApp:
// See srtest.cpp for the implementation of this class
//

class CSRTestApp : public CWinApp
{
public:
	CSRTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSRTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSRTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
