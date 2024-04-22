/***********************************************************************
Demo.Cpp - Code to demo tts queue object.



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
PISTTTSQUEUE      gpISTTTSQueue = NULL;
LPUNKNOWN         gpMaleDest = NULL;
LPUNKNOWN         gpFemaleDest = NULL;


/*************************************************************************
CreateAudioDest - Creates an audio destination object

retursn
   PIUNKNOWN   pIUnk
*/
LPUNKNOWN CreateAudioDest (void)
{
   LPUNKNOWN   pUnk;
   pUnk = NULL;


   CoCreateInstance(CLSID_MMAudioDest, NULL, CLSCTX_ALL,
      IID_IUnknown, (void**)&pUnk);

// #define USEAUDIOTEL
   #ifdef USEAUDIOTEL
   // use the logging audio telephony object
   PIAUDIOTEL  pIAudioTel;
   HRESULT hRes;
   hRes = CoCreateInstance(CLSID_AudioDestTel, NULL, CLSCTX_ALL,
          IID_IAudioTel,(void**)&pIAudioTel);
   if (!hRes) {
      SDATA d;
      WAVEFORMATEX   wfex;
      d.pData = &wfex;
      d.dwSize = sizeof(wfex);
      memset (&wfex, 0, sizeof(wfex));
      wfex.wFormatTag = WAVE_FORMAT_PCM;
      wfex.nChannels = 1;
      wfex.nSamplesPerSec =  22050;
      wfex.nAvgBytesPerSec = 44100;
      wfex.nBlockAlign = 2;
      wfex.wBitsPerSample = 16;

      pIAudioTel->WaveFormatSet (d);
      pIAudioTel->AudioObject (pUnk);
      pUnk->Release();
      pUnk = pIAudioTel;
   }
   #endif

   return pUnk;
}


/*************************************************************************
CreateTTS - Creates a TTS engine.

inputs
   BOOL  fMale - if TRUE then male, FALSE female
   LPUNKNOWN   *ppUnk - Filled with audio destination object
returns
   LPUNKNOWN - IUnknown for main mode object
*/
LPUNKNOWN CreateTTSEngine (BOOL fMale, LPUNKNOWN *ppUnk)
{
   HRESULT        hRes;
   TTSMODEINFOW        ttsResult;        // final result
   WCHAR          Zero = 0;
   PITTSFINDW      pITTSFind;             // find interface
   PITTSCENTRALW  pITTSCentral;           // central interface
   TTSMODEINFOW   mi;

   *ppUnk = NULL;

   // fill in the moed inf
   memset (&mi, 0 ,sizeof(mi));
   mi.wGender = fMale ? GENDER_MALE : GENDER_FEMALE;

   hRes = CoCreateInstance(CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSFindW,
                                                   (void**)&pITTSFind);
   if (FAILED(hRes)) return NULL;

   hRes = pITTSFind->Find(&mi, NULL, &ttsResult);

   if (hRes)
      {
      pITTSFind->Release();
      return NULL;     // error
      }


   LPUNKNOWN   pDest;
   pDest = CreateAudioDest();
   if (!pDest) {
      pITTSFind->Release();
      return NULL;
   }

// Should do select now

   hRes = pITTSFind->Select(ttsResult.gModeID, &pITTSCentral, (LPUNKNOWN) pDest);

   // since some engines leak audio destination, store this away
   // so we can release it later
   pDest->AddRef();
   *ppUnk = pDest;

   if (hRes) {
      pITTSFind->Release();
      return NULL;
      };


   pITTSFind->Release();

   return (LPUNKNOWN) pITTSCentral;
}

/*************************************************************************
CreateAndPopulateQueue - Creates and populates the queue object.

inputs
   none
returns
   BOOL - TRUE if OK, FALSE if fail
*/
BOOL CreateAndPopulateQueue (void)
{
   HRESULT  hRes;
   hRes = CoCreateInstance(CLSID_STTTSQueue, NULL, CLSCTX_ALL, IID_ISTTTSQueue,
                                                   (void**)&gpISTTTSQueue);
   if (FAILED(hRes)) {
      MessageBox (NULL, "Can't create the TTS Queue object. You might need to install SAPI 4.0", NULL, MB_OK);
      return FALSE;
   }

   LPUNKNOWN   pDest;
   pDest = CreateAudioDest();
   if (!pDest) {
      MessageBox (NULL, "Can't create audio destination", NULL, MB_OK);
      return FALSE;
   }

   // send the audio destination down
   hRes = gpISTTTSQueue->WaveAudioDestSet(pDest);
   if (hRes) {
      MessageBox (NULL, "Can't register audio destination with TTS Queue.", NULL, MB_OK);
      return FALSE;
   }
   pDest->Release();

   // Send in TTS engines
   pDest = CreateTTSEngine(TRUE, &gpMaleDest);
   hRes = gpISTTTSQueue->TTSAdd (L"MALE", pDest);
   if (hRes) {
      MessageBox (NULL, "Can't create TTS engine.", NULL, MB_OK);
      return FALSE;
   }

   pDest = CreateTTSEngine(FALSE, &gpFemaleDest);
   hRes = gpISTTTSQueue->TTSAdd (L"FEMALE", pDest);
   if (hRes) {
      MessageBox (NULL, "Can't create TTS engine.", NULL, MB_OK);
      return FALSE;
   }

   // stick in a default beep activated by passing beep down
   gpISTTTSQueue->WaveAddFromList (
      // L"[FromFile]\r"
      // L"c:\\test.wav=hello\r"
      L"[FromResource]\r"
      L"105=beep\r"
      L"105=same-beep\r",
      ghInstance);

   return TRUE;
}

/*************************************************************************
AddWaveDialogProc
*/
BOOL CALLBACK AddWaveDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_COMMAND:
         switch (LOWORD(wParam))
            {
            case IDOK:
               {
               char  szTemp[256];
               WCHAR szFile[256], szString[256];
               GetDlgItemText (hWnd, IDC_NAME, szTemp, sizeof(szTemp));
               MultiByteToWideChar (CP_ACP, 0, szTemp, -1,szFile, 256);
               GetDlgItemText (hWnd, IDC_STRING, szTemp, sizeof(szTemp));
               MultiByteToWideChar (CP_ACP, 0, szTemp, -1,szString, 256);

               if (gpISTTTSQueue->WaveAddFromFile(szString, szFile))
                  MessageBox (hWnd, "Can't open the file. The file may not exist, "
                  "or it might not be 22 kHz, 16-bit mono PCM.", NULL, MB_OK);
               else
                  MessageBox (hWnd, "File added.", "TTSQ", MB_OK);
               EndDialog (hWnd, IDOK);
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
RemoveWaveDialogProc
*/
BOOL CALLBACK RemoveWaveDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_COMMAND:
         switch (LOWORD(wParam))
            {
            case IDOK:
               {
               char  szTemp[256];
               WCHAR szString[256];
               GetDlgItemText (hWnd, IDC_STRING, szTemp, sizeof(szTemp));
               MultiByteToWideChar (CP_ACP, 0, szTemp, -1,szString, 256);

               if (gpISTTTSQueue->WaveRemove(szString))
                  MessageBox (hWnd, "Can't remove wave string.", NULL, MB_OK);
               else
                  MessageBox (hWnd, "Wave file removed.", "TTSQ", MB_OK);
               EndDialog (hWnd, IDOK);
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
DialogProc
*/
BOOL CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      CheckDlgButton (hWnd, IDC_MALE, BST_CHECKED);
      break;
   case WM_COMMAND:
         switch (LOWORD(wParam))
            {
            case IDC_ADDWAVE:
               DialogBox (ghInstance, MAKEINTRESOURCE(IDD_ADDWAVE),
                  hWnd, (DLGPROC) AddWaveDialogProc);
               break;
            case IDC_REMOVEWAVE:
               DialogBox (ghInstance, MAKEINTRESOURCE(IDD_REMOVEWAVE),
                  hWnd, (DLGPROC) RemoveWaveDialogProc);
               break;
            case IDC_RESET:
               gpISTTTSQueue->AudioReset();
               break;
            case IDC_PAUSE:
               gpISTTTSQueue->AudioPause();
               break;
            case IDC_RESUME:
               gpISTTTSQueue->AudioResume();
               break;
            case IDOK:
               // speak button
               {
               char  szTemp[1024];
               WCHAR szwTemp[1024];
               GetDlgItemText (hWnd, IDC_EDIT, szTemp, sizeof(szTemp));
               MultiByteToWideChar (CP_ACP, 0, szTemp, -1,szwTemp, 1024);

               // Get voice from radio button
               if (gpISTTTSQueue->Speak (szwTemp,
                  IsDlgButtonChecked (hWnd, IDC_MALE) ? L"MALE" : L"FEMALE", 42))
                  MessageBox (hWnd, "Error speaking", NULL, MB_OK);
               
               }

               if (IsDlgButtonChecked (hWnd, IDC_BEEP))
                  gpISTTTSQueue->Speak (L"beep", NULL, 0);
               break;
            case IDCANCEL:
               gpISTTTSQueue->AudioReset();
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
   ghInstance = hInstance;

   // try to begin ole
   CoInitialize(NULL);


   // find the right object
   if (!CreateAndPopulateQueue())
      return -1;

   // Bring up the dialog box
   DialogBox (hInstance, MAKEINTRESOURCE(IDD_TTS),
      NULL, (DLGPROC) DialogProc);

   // try to close ole
   if (gpISTTTSQueue)
      gpISTTTSQueue->Release();
   
   // release audio dest because some TTS engines don't release
   while (gpMaleDest && gpMaleDest->Release());
   while (gpFemaleDest && gpFemaleDest->Release());

   CoUninitialize();
   return 0;
}

