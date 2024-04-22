/***********************************************************************
Demo.Cpp - Code to demo tts.



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


/*************************************************************************
Globals */

HINSTANCE         ghInstance;                // instance handle
PITTSCENTRALW     gpITTSCentral = NULL;

/*************************************************************************
FindAndSelect - This finds and selects according to the specific TTSMODEINFOW.

inputs
   PTTSMODEINFOW     pTTSInfo - desired mode
returns
   PITTSCENTRAL - ISRCentral interface to TTS engine
sets:

*/

PITTSCENTRALW FindAndSelect (PTTSMODEINFOW pTTSInfo)
{
   HRESULT        hRes;
   TTSMODEINFOW        ttsResult;        // final result
   WCHAR          Zero = 0;
   PITTSFINDW      pITTSFind;             // find interface
   PIAUDIOMULTIMEDIADEVICE    pIAMM;      // multimedia device interface for audio-dest
   PITTSCENTRALW  pITTSCentral;           // central interface


   hRes = CoCreateInstance(CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSFindW,
                                                   (void**)&pITTSFind);
   if (FAILED(hRes)) return NULL;

   hRes = pITTSFind->Find(pTTSInfo, NULL, &ttsResult);

   if (hRes)
      {
      pITTSFind->Release();
      return NULL;     // error
      }


// Get the audio dest
   hRes = CoCreateInstance(CLSID_MMAudioDest, NULL, CLSCTX_ALL, IID_IAudioMultiMediaDevice,
                                                   (void**)&pIAMM);
   if (hRes)
      {
      pITTSFind->Release();
      return NULL;     // error
      }
	pIAMM->DeviceNumSet (WAVE_MAPPER);

	// Pass off the multi-media-device interface as an IUnknown (since it is one)

// Should do select now

   hRes = pITTSFind->Select(ttsResult.gModeID, &pITTSCentral, (LPUNKNOWN) pIAMM);

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
   case WM_COMMAND:
         switch (LOWORD(wParam))
            {
            case IDOK:
               {
               char  szSpeak[1024];
               WCHAR wszSpeak[1024];
               SDATA data;

               // Speak
               GetDlgItemText (hWnd, IDC_EDIT, szSpeak, sizeof(szSpeak));
               data.dwSize = (DWORD)
                  MultiByteToWideChar(CP_ACP, 0, szSpeak, -1, wszSpeak,
                  sizeof(wszSpeak) / sizeof(WCHAR)) * sizeof(WCHAR);
               data.pData = wszSpeak;
               gpITTSCentral->TextData (CHARSET_TEXT, 0,
                  data, NULL,
                  IID_ITTSBufNotifySinkW);
               }
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
TTSMODEINFOW   ModeInfo;

// try to begin ole

   if (!BeginOLE())
      {
      MessageBox (NULL, "Can't create OLE.", NULL, MB_OK);
      return 1;
      }

// find the right object
   memset (&ModeInfo, 0, sizeof(ModeInfo));
   gpITTSCentral = FindAndSelect (&ModeInfo);
   if (!gpITTSCentral) {
      MessageBox (NULL, "Can't create a TTS engine.", NULL, MB_OK);
      return 1;
      };

// Bring up the dialog box
   DialogBox (hInstance, MAKEINTRESOURCE(IDD_TTS),
      NULL, (DLGPROC) DialogProc);

// try to close ole
   gpITTSCentral->Release();

   if (!EndOLE())
      MessageBox (NULL, "Can't shut down OLE.", NULL, MB_OK);

   return 0;
}


