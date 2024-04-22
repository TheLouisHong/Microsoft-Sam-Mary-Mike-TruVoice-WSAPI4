/***********************************************************************
Demo.Cpp - Code to demo tts to audio file destination.



Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/


#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include <initguid.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>

#include <speech.h>
#include "resource.h"

/************************************************************************
Notification objects */
class CTestNotify : public ITTSNotifySink {
   public:
      CTestNotify (void);
      ~CTestNotify (void);

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ITTSNotifySink
		STDMETHOD (AttribChanged)  (DWORD);
		STDMETHOD (AudioStart)     (QWORD);
		STDMETHOD (AudioStop)      (QWORD);
		STDMETHOD (Visual)         (QWORD, CHAR, CHAR, DWORD, PTTSMOUTH);
   };
typedef CTestNotify * PCTestNotify;


/*************************************************************************
Globals */

HINSTANCE         ghInstance;                // instance handle
PITTSCENTRAL     gpITTSCentral = NULL;
CTestNotify       gNotify;
HWND              ghWndMain = NULL;
WORD              gwRealTime = 0x100;
char              gszFile[256] = "";
char              gszVoice[256] = "";
PIAUDIOFILE       gpIAF = NULL;      // audio file device interface for audio-dest
WCHAR             gwszFile[256] = L"";  // wide character file name

/*************************************************************************
CTestNotify - Notification object.
*/
CTestNotify::CTestNotify (void)
{
}

CTestNotify::~CTestNotify (void)
{
// this space intentionally left blank
}

STDMETHODIMP CTestNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
	*ppv = NULL;

	/* always return our IUnknown for IID_IUnknown */
	if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITTSNotifySink))
	{
		*ppv = (LPVOID) this;
		return S_OK;
	}

	// otherwise, cant find
	return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CTestNotify::AddRef (void)
{
	// normally this increases a reference count, but this object
	// is going to be freed as soon as the app is freed, so it doesn't
	// matter
	return 1;
}

STDMETHODIMP_(ULONG) CTestNotify::Release (void)
{
	// normally this releases a reference count, but this object
	// is going to be freed when the application is freed so it doesnt
	// matter
	return 1;
}

STDMETHODIMP CTestNotify::AttribChanged (DWORD dwAttribID)
{
   return NOERROR;
}

STDMETHODIMP CTestNotify::AudioStart (QWORD qTimeStamp)
{
   if (ghWndMain)
      SetDlgItemText (ghWndMain, IDC_STATUS, "Writing to file");

   return NOERROR;
}

STDMETHODIMP CTestNotify::AudioStop (QWORD qTimeStamp)
{
   if (ghWndMain)
      SetDlgItemText (ghWndMain, IDC_STATUS, "");

   // close the file
   gpIAF->Flush();


   return NOERROR;
}

STDMETHODIMP CTestNotify::Visual (QWORD qTimeStamp, CHAR cIPAPhoneme,
				CHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth)
{
   return NOERROR;
}


/*************************************************************************
FindAndSelect - This finds and selects according to the specific TTSMODEINFO.

inputs
   PTTSMODEINFO     pTTSInfo - desired mode
returns
   PITTSCENTRAL - ISRCentral interface to TTS engine
sets:

*/

PITTSCENTRAL FindAndSelect (PTTSMODEINFO pTTSInfo)
{
   HRESULT        hRes;
   TTSMODEINFO        ttsResult;        // final result
   WCHAR          Zero = 0;
   PITTSFIND      pITTSFind;             // find interface
   PITTSCENTRAL  pITTSCentral;           // central interface


   hRes = CoCreateInstance(CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSFind,
                                                   (void**)&pITTSFind);
   if (FAILED(hRes)) return NULL;

   hRes = pITTSFind->Find(pTTSInfo, NULL, &ttsResult);

   if (hRes)
      {
      pITTSFind->Release();
      return NULL;     // error
      }


// Get the audio dest
   hRes = CoCreateInstance(CLSID_AudioDestFile, NULL, CLSCTX_ALL, IID_IAudioFile,
                                                   (void**)&gpIAF);
   if (hRes)
      {
      pITTSFind->Release();
      return NULL;     // error
      }

	// Pass off the audio file interface as an IUnknown (since it is one)

// Should do select now

   hRes = pITTSFind->Select(ttsResult.gModeID, &pITTSCentral, gpIAF);

   if (hRes) {
      pITTSFind->Release();
      return NULL;
      };

// free random stuff up

   pITTSFind->Release();

   return pITTSCentral;
}




/*************************************************************************
BeginOLE - This begins the OLE.

inputs
   none
returns
   BOOL - TRUE if is succedes
*/

BOOL BeginOLE (void)
{
   DWORD    dwVer;

// Initialize OLE

   SetMessageQueue(96);
   dwVer = CoBuildVersion();

   if (rmm != HIWORD(dwVer)) return FALSE;         // error

   if (FAILED(CoInitialize(NULL))) return FALSE;

   return TRUE;
}


/*************************************************************************
EndOLE - This closes up the OLE.

inputs
   none
returns
   BOOL - TRUE if succede
*/

BOOL EndOLE (void)
{
// Free up all of OLE

   CoUninitialize ();

   return TRUE;
}


/*************************************************************************
DialogProc
*/
BOOL CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      ghWndMain = hWnd;
      break;
   case WM_COMMAND:
         switch (LOWORD(wParam))
            {
            case IDOK:
               {
               char  szSpeak[10240];
               SDATA data;

               // open the file
               if (gpIAF->Set(gwszFile, 1)) {
                  MessageBox (hWnd, "Can't open the file!", NULL, MB_OK);
                  return TRUE;
               }

               // Speak
               GetDlgItemText (hWnd, IDC_EDIT, szSpeak, sizeof(szSpeak));
               data.dwSize = strlen(szSpeak) + 1;
               data.pData = szSpeak;
               gpITTSCentral->TextData (CHARSET_TEXT, TTSDATAFLAG_TAGGED,
                  data, NULL,
                  IID_ITTSBufNotifySink);
               }
               return TRUE;
            case IDCANCEL:
               gpITTSCentral->AudioReset ();
               EndDialog (hWnd, IDCANCEL);
               return TRUE;
            }
      break;
   };

return FALSE;  // didn't handle
}



/*************************************************************************
ChooseDialogProc - Chose the voice
*/
BOOL CALLBACK ChooseDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      {
      HRESULT hRes;
      PITTSENUM   pITTSEnum;

      // list the voices
      // get the enumerators
      hRes = CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSEnum, (void**)&pITTSEnum);
      if (hRes) {
         MessageBox (NULL, "Can't open TTS engines", NULL, MB_OK);
         EndDialog (hWnd, IDCANCEL);
         return FALSE;
      }
      TTSMODEINFO     TTSInfo;
      while (!pITTSEnum->Next(1, &TTSInfo, NULL))
         SendDlgItemMessage (hWnd, IDC_VOICE, CB_ADDSTRING,
            0, (LPARAM) TTSInfo.szModeName);
      SendDlgItemMessage (hWnd, IDC_VOICE, CB_SETCURSEL,
         0, (LPARAM) 0);

      pITTSEnum->Release();

      // Default to 1x
      CheckDlgButton (hWnd, IDC_1XREALTIME, BST_CHECKED);
      }
      break;
   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
            GetDlgItemText (hWnd, IDC_VOICE, gszVoice, sizeof(gszVoice));
            GetDlgItemText (hWnd, IDC_FILE, gszFile, sizeof(gszFile));
            if (IsDlgButtonChecked (hWnd, IDC_2XREALTIME))
               gwRealTime = 0x200;
            else if (IsDlgButtonChecked (hWnd, IDC_4XREALTIME))
               gwRealTime = 0x400;
            else if (IsDlgButtonChecked (hWnd, IDC_8XREALTIME))
               gwRealTime = 0x800;
            else
               gwRealTime = 0x100;

            EndDialog (hWnd, IDOK);
            return TRUE;
         case IDCANCEL:
            EndDialog (hWnd, IDCANCEL);
            return TRUE;
         }
      break;
   };

return FALSE;  // didn't handle
}

/*************************************************************************
winmain - Windows main code.
*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
TTSMODEINFO   ModeInfo;

// try to begin ole

   if (!BeginOLE())
      {
      MessageBox (NULL, "Can't create OLE.", NULL, MB_OK);
      return 1;
      }

// Bring up the dialog box
   if (DialogBox (hInstance, MAKEINTRESOURCE(IDD_CHOOSE),
      NULL, (DLGPROC) ChooseDialogProc) != IDOK)
      return -1;

// find the right object
   memset (&ModeInfo, 0, sizeof(ModeInfo));
   strcpy (ModeInfo.szModeName, gszVoice);
   gpITTSCentral = FindAndSelect (&ModeInfo);
   if (!gpITTSCentral) {
      MessageBox (NULL, "Can't create a TTS engine.", NULL, MB_OK);
      return 1;
      };
   MultiByteToWideChar (CP_ACP, 0, gszFile, -1,
      gwszFile, sizeof(gwszFile) / sizeof(WCHAR));
   gpIAF->RealTimeSet(gwRealTime);

   // register so we get the notifications
   DWORD dwRegKey;
   gpITTSCentral->Register ((void*)&gNotify, IID_ITTSNotifySink,
                                 &dwRegKey);

// Bring up the dialog box
   DialogBox (hInstance, MAKEINTRESOURCE(IDD_TTS),
      NULL, (DLGPROC) DialogProc);

// try to close ole
   gpIAF->Flush();
   gpIAF->Release();
   gpITTSCentral->Release();

   if (!EndOLE())
      MessageBox (NULL, "Can't shut down OLE.", NULL, MB_OK);

   return 0;
}


