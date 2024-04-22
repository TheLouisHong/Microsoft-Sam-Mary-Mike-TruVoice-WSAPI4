/*********************************************************************
WrapDemo.Cpp - Demonstrates using the C++ wrappers for TTS & SR.

Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <mmsystem.h>
#include <initguid.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <dsound.h>
#include <spchwrap.h>
#include "resource.h"

class CTestGramNotify : public CSRGramNotifySink {
   public:
	   STDMETHODIMP PhraseFinish   (DWORD, QWORD, QWORD, PSRPHRASEW, LPUNKNOWN);
   };
typedef CTestGramNotify * PCTestGramNotify;

PCTestGramNotify     gGramNotifySink;
PCSRGramComp         gpCSRGramComp = NULL;
HWND                 ghWndStatic = NULL;
PCTTSMode            gpCTTSMode = NULL;


/****************************************************************
Grammar Notification Sink */
STDMETHODIMP CTestGramNotify::PhraseFinish(DWORD dwFlags, QWORD qTimeStampBegin,
   QWORD qTimeStampEnd, PSRPHRASEW pSRPhrase, LPUNKNOWN lpResults)
{
   WCHAR    *pszRet = NULL;
   DWORD    dwRetSize;
   DWORD    dwRetVal;
   HRESULT  hRes;
   char     szTemp[128];

   hRes = gpCSRGramComp->PhraseParse(pSRPhrase, &dwRetVal, &pszRet, &dwRetSize);
   if (hRes)
      SetWindowText (ghWndStatic, "Unrecognized");
   else {
      WideCharToMultiByte (CP_ACP, 0, pszRet, -1, szTemp, sizeof(szTemp), 0,0);
      SetWindowText (ghWndStatic, szTemp);
   }
   if (pszRet)
      CoTaskMemFree (pszRet);

   return NOERROR;
}

/*******************************************************
_DemoDlgProc - Do the testing.

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK _DemoDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      ghWndStatic = GetDlgItem (hWnd, IDC_RESULT);
      break;   // continue on
   case WM_CLOSE:
      EndDialog (hWnd, IDOK);
      gpCSRGramComp->Deactivate();
      gpCTTSMode->AudioReset();
      break;

   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDC_LISTEN:
         {
            HRESULT  hRes;
            hRes = gpCSRGramComp->Activate(NULL);
            if (hRes) {
               MessageBox (hWnd, "Error activating grammar.", NULL, MB_OK);
            }
         }
         break;
      case IDC_STOPLISTENING:
         {
            HRESULT  hRes;
            hRes = gpCSRGramComp->Deactivate();
            if (hRes) {
               MessageBox (hWnd, "Error deactivating grammar.", NULL, MB_OK);
            }
         }
         break;
      case IDC_SPEAK:
         {
            WCHAR szwTemp[128];
            char  szaTemp[128];
            GetDlgItemText (hWnd, IDC_RESULT, szaTemp, sizeof(szaTemp));
            MultiByteToWideChar (CP_ACP, 0, szaTemp, -1, szwTemp, 128);
            HRESULT  hRes;
            hRes = gpCTTSMode->Speak(szwTemp);
            if (hRes) {
               MessageBox (hWnd, "Error speaking results.", NULL, MB_OK);
            }

         }
         break;
      }
      break;
   };

return FALSE;  // didn't handle
}


/*******************************************************
WinMain - main entry point for windows
*/

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpszCmdLine, int nCmdShow)
{
   CoInitialize(NULL);

   // put these brackets to the SRMode, SRGramComp & TTSMode are freed
   // before OLE is unitialized
   {
      CSRMode        mode;

      // initalize SR & load grammar
      HRESULT hRes;
      hRes = mode.Init();
      if (hRes) {
         MessageBox (NULL, "Can't initialize speech recognition.", NULL, MB_OK);
         return -1;
      }
      
      char  szGrammar[] =
         "[Grammar]\n"
         "LangID=1033\n"
         "[<Start>]\n"
         "<Start>=<Float>\n";
	  gGramNotifySink = new CTestGramNotify;
	  if(!gGramNotifySink)
		  return - 1;
	  gGramNotifySink->AddRef();
      gpCSRGramComp = mode.GrammarFromMemory (gGramNotifySink,
         szGrammar, strlen(szGrammar)+1);
      if (!gpCSRGramComp) {
         MessageBox (NULL, "Couldn't load digits grammar.", NULL, MB_OK);
         return -1;
      }

      // Text-to-speech
      gpCTTSMode = new CTTSMode;
      if (gpCTTSMode->Init()) {
         MessageBox (NULL, "Couldn't initialize TTS.", NULL, MB_OK);
         return -1;
      }

      // pull up test dialog
      DialogBox (hInstance, MAKEINTRESOURCE(IDD_DEMO),
         NULL, (DLGPROC) _DemoDlgProc);

      // free up
	  if(gGramNotifySink)
	  {
		  gGramNotifySink->Release();
	  }
      if (gpCSRGramComp)
         delete gpCSRGramComp;
      // Free TTS
      if (gpCTTSMode)
         delete gpCTTSMode;

   }
   CoUninitialize();

   return 0;
}



