/********************************************************************
Editor.cpp - grammar compiler editor

 - put standard disclaimers in here.


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

/* If you want to test data logging then define USELOGGING in
   the makefile */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <speech.h>
#include "resource.h"

extern HRESULT Test (void);

/* defines */

#define IDC_CODE		0x2000


/* globals */
HINSTANCE	ghInstance;
HWND		ghWndEditor = NULL;
HWND		ghWndCode = NULL;
HWND		ghWndCompile = NULL;
BOOL		gfDirty = FALSE;
BOOL		gfIsOutOfSync = TRUE;
BOOL		gfCompiled = FALSE;
char		gszName[256] = "";
char		gszAppName[] = "Speech Recognition Grammar Compiler";
PISTGRAMCOMP gpISTGramComp = NULL;

#ifdef USELOGGING
PISTLOG  gpISTLog = NULL;
WCHAR    gszLogApp[] = L"SRG";
#endif

/********************************************************************
SetTitle - Sets the title with the file name
*/
void SetTitle (void)
{
	if (gszName[0]) {
		char	szTemp[256];
		wsprintf (szTemp, "%s - %s", gszName, gszAppName);
		SetWindowText (ghWndEditor, szTemp);
	}
	else
		SetWindowText (ghWndEditor, gszAppName);
}


/********************************************************************
SendTextToCompiler - If the text in the edit buffer is dirty then
	 this sets the text in the grammar compiler object. This will,
	 of course, require a recompile
*/
void SendTextToCompiler (void)
{
	if (!gfIsOutOfSync)
		return;
	gfIsOutOfSync = FALSE;
	gfCompiled = FALSE;

	CHAR	*paTemp;
	WCHAR	*pwTemp;
	DWORD	dwaSize, dwwSize;

	dwaSize = GetWindowTextLength(ghWndCode) + 1;
	paTemp = (CHAR*) malloc(dwaSize);
	if (!paTemp)
		return;
	dwwSize = dwaSize * 2;
	pwTemp = (WCHAR*) malloc(dwwSize);
	if (!pwTemp) {
		free (paTemp);
		return;
	}
	GetWindowText (ghWndCode, paTemp, dwaSize);

	dwwSize = 2 * MultiByteToWideChar (CP_ACP, 0, paTemp, -1,
		pwTemp, dwwSize / 2);

	gpISTGramComp->FromMemory(pwTemp, dwwSize);
	free (paTemp);
	free (pwTemp);

}

/********************************************************************
ReallySaveIt - Save it.
*/
BOOL ReallySaveIt ()
{
   HRESULT hRes;

	SendTextToCompiler();
	WCHAR	szTemp[256];
	MultiByteToWideChar (CP_ACP,0, gszName, -1, szTemp, sizeof(szTemp)/2);

   // depending upon the ending of the file
   DWORD dwOffset;
   dwOffset = strlen (gszName);
   if (dwOffset >= 4)
      dwOffset -= 4;
   else
      dwOffset = 0;

   if (!stricmp(gszName + dwOffset, ".txt")) {
      // get the datat
      LPWSTR   lpStr;
      DWORD    dwSize;
      hRes = gpISTGramComp->TextGet (&lpStr, &dwSize);
      if (hRes || !lpStr) {
		   MessageBox (ghWndEditor, "Can't save the file.", NULL, MB_OK);
         return FALSE;
      }

      // write as unicode

      /* open the file */
      FILE *f;
      f = fopen (gszName, "wb");
      if (!f) {
		   MessageBox (ghWndEditor, "Can't save the file as text.", NULL, MB_OK);
         CoTaskMemFree (lpStr);
         return FALSE;
      }
      WCHAR szHeader = 0xfeff;
      fwrite (&szHeader, 2, 1, f);
      fwrite (lpStr, 2, wcslen(lpStr), f);
      fclose (f);
      CoTaskMemFree (lpStr);
   }
   else if (!stricmp(gszName + dwOffset, ".grm")) {
      // get the raw data
      PVOID pMem;
      DWORD dwSize;
      hRes = gpISTGramComp->GrammarDataGet (FALSE, &pMem, &dwSize);
      if (hRes || !pMem) {
		   MessageBox (ghWndEditor, "You must compile the grammar first.", NULL, MB_OK);
         return FALSE;
      }

      /* open the file */
      FILE *f;
      f = fopen (gszName, "wb");
      if (!f) {
		   MessageBox (ghWndEditor, "Can't save the file.", NULL, MB_OK);
         CoTaskMemFree (pMem);
      }
      fwrite (pMem, 1, dwSize, f);
      fclose (f);
      CoTaskMemFree (pMem);

   }
   else {
      if (gpISTGramComp->ToFile (szTemp)) {
		   MessageBox (ghWndEditor, "Can't save the file.", NULL, MB_OK);
		   return FALSE;
      }
	}

	gfDirty = FALSE;
	SetTitle ();

	return TRUE;
}

/********************************************************************
SaveAs - Asks the user where to save the file. It then saves.

inputs
	none
returns
	BOOL - TRUE if all OK, FALSE if user cancelled
*/
BOOL SaveAs (void)
{
	OPENFILENAME	ofn;

	memset (&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = ghWndEditor;
	ofn.hInstance = ghInstance;
	ofn.lpstrFilter =
		"Speech Recognition Grammar File (*.srg)\0" "*.srg\0"
		"Text File (*.txt)\0" "*.txt\0"
		"Standard Grammar Format (*.grm)\0" "*.grm\0"
		"\0\0";
	ofn.lpstrFile = gszName;
	ofn.nMaxFile = sizeof(gszName);
	ofn.Flags = OFN_HIDEREADONLY; 
	ofn.lpstrDefExt = "srg";

	if (!GetSaveFileName(&ofn))
		return FALSE;

   // fill in new file extension
   if (ofn.nFileExtension) {
      CHAR* pExt;
      switch (ofn.nFilterIndex) {
      case 2:
         pExt = "txt";
         break;
      case 3:
         pExt = "grm";
         break;
      case 1:
      default:
         pExt = "srg";
      }
      strcpy (gszName + ofn.nFileExtension, pExt);
   }

	// Really save it
	return ReallySaveIt();
}



/********************************************************************
Save - Saves the file. If the file doens't have a name then it
	asks then it does a save-as.

inputs
	none
returns
	BOOL - TRUE if all OK, FALSE if user cancelled
*/
BOOL Save (void)
{
	if (!gszName[0])
		return SaveAs();

	return ReallySaveIt();
}


/********************************************************************
AskToSaveIfNecessary - Asks the user if they want to save any changes
	if the file is dirty. If it's not dirty they're not asked.

inputs
	nonre
returns
	BOOL - TRUE if everything OK, FALSE is not OK and pressed cancel
*/
BOOL AskToSaveIfNecessary (void)
{
	if (!gfDirty)
		return TRUE;

	int	iRet;
	iRet = MessageBox (ghWndEditor,
		"The file has been modified. Do you want to save it?",
		gszAppName,
		MB_YESNOCANCEL);
	if (iRet == IDNO)
		return TRUE;
	if (iRet == IDYES)
		return Save();

	// else cancelled
	return FALSE;
}


/********************************************************************
New - Clears out the buffer. If there's anything dirty it first
	asks the user if they want to save changes.

inputs
	none
returns
	BOOL - TRUE if everything OK, FALSE if is not OK
*/
BOOL New (void)
{
	if (!AskToSaveIfNecessary())
		return FALSE;

	gfDirty = gfCompiled = FALSE;
	gfIsOutOfSync = TRUE;
	gszName[0] = 0;
	SetTitle();
	SetWindowText (ghWndCode, "");
	SetWindowText (ghWndCompile, "");

	return TRUE;
}

/********************************************************************
ReallyOpenIt - Really opens the file.

returns
   BOOL - TRUE if everything OK, FALSE if cancel
*/
BOOL ReallyOpenIt (void)
{
	HRESULT hRes;
	WCHAR	szTemp[256];
	MultiByteToWideChar (CP_ACP,0, gszName, -1, szTemp, sizeof(szTemp)/2);
	hRes = gpISTGramComp->FromFile (szTemp);
	if (hRes) {
		MessageBox (ghWndEditor, "Can't open the file.", NULL, MB_OK);
		return FALSE;
	}

	// get the text, convert to ANSI, and send it
	SetWindowText (ghWndCode, "");
	SetWindowText (ghWndCompile, "");
	WCHAR	*pMem;
	DWORD	dwSize;
	hRes = gpISTGramComp->TextGet (&pMem, &dwSize);
	if (!hRes) {
		char	*pTemp;
		pTemp = (char*) malloc (dwSize);
		if (!pTemp)
			return FALSE;	// error
		WideCharToMultiByte (CP_ACP,0, pMem, -1, pTemp, dwSize, 0, NULL);
		SetWindowText (ghWndCode, pTemp);
		SendMessage (ghWndCode, EM_SETSEL, 0,0);
		free (pTemp);
		CoTaskMemFree (pMem);
	}


	gfDirty = FALSE;
	gfIsOutOfSync = FALSE;
	gpISTGramComp->IsCompiled(&gfCompiled);

	SetTitle ();

   return TRUE;
}

/********************************************************************
Open - Opens a file. If another file is open it will ask the user
	if they want to save first.

inputs
	NONE
returns
	BOOL - TRUE if everything OK, FALSE if cancel
*/
BOOL Open (void)
{
	if (!AskToSaveIfNecessary())
		return FALSE;

	// 
	OPENFILENAME	ofn;

	memset (&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = ghWndEditor;
	ofn.hInstance = ghInstance;
	ofn.lpstrFilter =
		"Grammar or Text (*.srg;*.txt)\0" "*.srg;*.txt\0"
		"Speech Recognition Grammar File (*.srg)\0" "*.srg\0"
		"Unicode text file (*.txt)\0" "*.txt\0"
		"\0\0";
	ofn.lpstrFile = gszName;
	ofn.nMaxFile = sizeof(gszName);
	ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST; 
	ofn.lpstrDefExt = "srg";

	if (!GetOpenFileName(&ofn))
		return FALSE;

	// open it
   if (!ReallyOpenIt())
      return FALSE;
	return TRUE;
}


/********************************************************************
CompileTheGrammar - Compiles the grammar

inputs
	BOOL		fForce - If TRUE then it always re-compiles. If FALSE
					if only recompiles if it's out of date.
returns
	BOOL - TRUE if compile succeded. FALSE if it failed
*/
BOOL CompileTheGrammar (BOOL fForce)
{
	// do we need to update the compiler with our status?
	SendTextToCompiler();

	if (!fForce && gfCompiled)
		return TRUE;	// already compiled

	// since we've already sent the text down, ask the grammar
	// compiler to compile
	HRESULT	hRes;
	WCHAR	*pRes;
	DWORD	dwSize;
	hRes = gpISTGramComp->Compile(&pRes, &dwSize);

	// show the text
	if (pRes) {
		char	*pTemp;
		pTemp = (char*) malloc (dwSize + 100);
		if (!pTemp)
			return FALSE;	// error
		WideCharToMultiByte (CP_ACP,0, pRes, -1, pTemp, dwSize, 0, NULL);
		SetWindowText (ghWndCompile, pTemp);

      if (hRes)
         strcat (pTemp, "\r\nCOMPILE FAILED!");

		free (pTemp);
		CoTaskMemFree (pRes);

      // Set selection at end
		SendMessage (ghWndCompile, EM_SETSEL, GetWindowTextLength(ghWndCompile),GetWindowTextLength(ghWndCompile));

		SendMessage (ghWndCompile, EM_SCROLLCARET, 0,0);

	}
	else
		SetWindowText (ghWndCompile, "");
	if (!hRes)
		gfCompiled = TRUE;
	

	return hRes ? FALSE : TRUE;
}





/********************************************************************
EditorWndProc - Editor window proc
*/
LONG CALLBACK EditorWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
   case WM_SETFOCUS:
      SetFocus (ghWndCode);
      break;
	case WM_CREATE:
		ghWndCode = CreateWindowEx (
			WS_EX_CLIENTEDGE,
			"EDIT",
			"",
			WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | 
			ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN,
			1, 1, 1, 1,
			hwnd,
			(HMENU) IDC_CODE,
			ghInstance,
			NULL
			);
		ghWndCompile = CreateWindowEx (
			WS_EX_CLIENTEDGE,
			"EDIT",
			"Compiler messages will appear here.",
			WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | 
			ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN |
			ES_READONLY,
			1, 1, 1, 1,
			hwnd,
			NULL,
			ghInstance,
			NULL
			);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_NEW:
			New();
			break;
		case IDM_FILE_SAVE:
			Save();
			break;
		case IDM_FILE_SAVEAS:
			SaveAs();
			break;
		case IDM_FILE_OPEN:
			Open();
			break;
		case IDM_FILE_EXIT:
			PostMessage (hwnd, WM_CLOSE, 0, 0);
			break;
		case IDM_GRAMMAR_COMPILE:
			// Compile the grammar
			if (!CompileTheGrammar(TRUE))
				MessageBox (hwnd, "The compile failed.", NULL, MB_OK);
			break;
		case IDM_GRAMMAR_TEST:
			// Make sure the grammar is compiled
			if (!CompileTheGrammar(FALSE)) {
				MessageBox (hwnd, "The grammar compiled failed. You cannot test until it compiles.", NULL, MB_OK);
				return 0;
			}

			// Run the test
         {
         HRESULT hRes;
			hRes = Test();
         if (hRes) {
            char  szTemp[256];
            wsprintf (szTemp, "The SR engine reported an error: 0x%x", (DWORD) hRes);
				MessageBox (hwnd, szTemp, NULL, MB_OK);
         }
         }
			break;

      case ID_HELP_HELP:
         MessageBox (hwnd,
            "The grammar compiler demonstrates the Grammar Compiler object.\r\n"
            "For information on the text format, look through the samples or "
            "read the Grammar Compiler object documentation. (Located in the "
            "Speech Tools section.)",
            "Help", MB_OK);
         break;

		case IDC_CODE:
			if (HIWORD(wParam) == EN_CHANGE) {
				gfDirty = TRUE;
				gfIsOutOfSync = TRUE;
				gfCompiled = FALSE;
			}
			break;
		}
		break;
	case WM_CLOSE:
		// If dirty ask if want to save and if want to compile
		if (!AskToSaveIfNecessary())
			return 0;
		DestroyWindow (hwnd);
		return 0;
	case WM_SIZE:
		// move the editort and compiler window
		MoveWindow (ghWndCode, 0, 0,
			LOWORD(lParam), HIWORD(lParam) * 3 / 4, TRUE);
		MoveWindow (ghWndCompile, 0, HIWORD(lParam) * 3 / 4 + 3,
			LOWORD(lParam), max(0, (int) HIWORD(lParam) / 4 - 3), TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage (0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


/********************************************************************
WinMain - Main entry point
*/
int WINAPI WinMain( 
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	ghInstance = hInstance;

   // Set the current working directory
   if (!lpCmdLine || !lpCmdLine[0]) {
      char  szModule[256];
      char *pSlash;
      GetModuleFileName (hInstance, szModule, sizeof(szModule));
      for (pSlash = strchr(szModule,'\\'); pSlash && strchr(pSlash+1,'\\');
         pSlash = strchr(pSlash+1,'\\'));
      if (pSlash)
         *pSlash = 0;
      SetCurrentDirectory (szModule);
   }


	// init ole
	CoInitialize(NULL);

	// Create the grammar compiler object
	HRESULT	hRes;
	hRes = CoCreateInstance (CLSID_STGramComp, NULL, CLSCTX_ALL, IID_ISTGramComp, (void**)&gpISTGramComp);
	if (hRes) {
		MessageBox (NULL, "Can't load the grammar compiler. You must have the 4.0 version of the API installed.\n", NULL, MB_OK);
		return 0;
	}

#ifdef USELOGGING
   CoCreateInstance (CLSID_STLog, NULL, CLSCTX_ALL, IID_ISTLog, (void**)&gpISTLog);
   if (gpISTLog) {
      gpISTLog->ToFile(L"c:\\out.stl");
      gpISTLog->ShowOutput(TRUE);
      gpISTLog->Text (gszLogApp, L"Start logging", 1);
   }
#endif

	// register the windows class
	WNDCLASS wc;
	char	szClass[] = "SRG Editor";
	memset (&wc, 0, sizeof(wc));
	wc.style = 0; //CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = EditorWndProc;
	wc.hInstance = ghInstance;
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE+1);
	wc.lpszMenuName = MAKEINTRESOURCE (IDM_EDITOR);
	wc.lpszClassName = szClass;
   wc.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_MAIN));
	RegisterClass (&wc);

	// create the window
	ghWndEditor = CreateWindowEx (
		WS_EX_WINDOWEDGE,
		szClass,
		"SRG Editor",
		WS_OVERLAPPEDWINDOW | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, ghInstance, NULL);
	if (!ghWndEditor)
		return -1;

	// message loop
    MSG msg;
	SetTitle();
	ShowWindow( ghWndEditor, nCmdShow );
	UpdateWindow( ghWndEditor );

	// If there's a command line option then open up
	// the file immediately
   if (lpCmdLine && lpCmdLine[0]) {
      strcpy (gszName, lpCmdLine);
      ReallyOpenIt();
   }


	while( GetMessage(&msg, NULL, NULL, NULL) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


#ifdef USELOGGING
   if (gpISTLog)
      gpISTLog->Release();
#endif

	// finish with ole
	if (gpISTGramComp)
		gpISTGramComp->Release();
	CoUninitialize ();

	// done
	return 0;
}

