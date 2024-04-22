/*********************************************************************
FullDup.Cpp - Demonstrates using full duplex sound cards.

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
HWND                 ghWndMain = NULL;
PCTTSMode            gpCTTSMode = NULL;
PCSRMode             gpCSRMode = NULL;



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

   // speak it out
   WCHAR szwTemp[128];
   GetWindowText (ghWndStatic, szTemp, sizeof(szTemp));
   MultiByteToWideChar (CP_ACP, 0, szTemp, -1, szwTemp, 128);
   gpCTTSMode->AudioReset();

   hRes = gpCTTSMode->Speak(L"I heard ");
   if (hRes) {
      MessageBox (ghWndMain, "Error speaking results. You may not have a full duplex sound card.", NULL, MB_OK);
      return NOERROR;
   }
   hRes = gpCTTSMode->Speak(szwTemp);
   if (hRes) {
      MessageBox (ghWndMain, "Error speaking results. You may not have a full duplex sound card.", NULL, MB_OK);
   }

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
      ghWndMain = hWnd;
      break;   // continue on
   case WM_CLOSE:
      EndDialog (hWnd, IDOK);
      gpCSRGramComp->Deactivate();
      gpCTTSMode->AudioReset();
      break;

   };

return FALSE;  // didn't handle
}

/************************************************************
LoadFullDuplex - Given the requirement of full duplex, this
loads the SR & TTS engines in.

It sets the global variables for the SR & TTS engine.

inputs
   HWND        hWnd - Window to pull up message boxes
   GUID        gSR - SR mode ID;
   DWORD       dwWaveIn - Wave in device
   GUID        gTTS - TTS mode iD
   DWORD       dwWaveOut - Wave out device
returns
   TRUE if succede, false if fail
*/
BOOL LoadFullDuplex (HWND hWnd, GUID gSR, DWORD dwWaveIn, GUID gTTS, DWORD dwWaveOut)
{
   HRESULT  hRes;
   SDATA Data;
   Data.pData = NULL;

   // free existing engines if there are any
   if (gpCSRMode)
      delete gpCSRMode;
   if (gpCTTSMode)
      delete gpCTTSMode;
   gpCSRMode = new CSRMode;
   gpCTTSMode = new CTTSMode;
   if (!gpCSRMode || !gpCTTSMode)
      return FALSE;

   // create the SR engine and see what wave device it is
   hRes = gpCSRMode->InitAudioSourceMM (dwWaveIn);
   if (hRes) {
      MessageBox (hWnd, "Can't select wave device.", NULL, MB_OK);
      return FALSE;
   }
   hRes = gpCSRMode->Init (gSR);
   if (hRes) {
      MessageBox (hWnd, "Can't load speech recognition engine.", NULL, MB_OK);
      return FALSE;
   }
   PIAUDIO  pIAudio;
   pIAudio = NULL;
   gpCSRMode->m_pUnkAudio->QueryInterface (IID_IAudio, (PVOID*)&pIAudio);
   if (!pIAudio) {
      MessageBox (hWnd, "Speech recognition engine doesn't use a wave device.", NULL, MB_OK);
      return FALSE;
   }
   Data.pData = NULL;
   pIAudio->WaveFormatGet (&Data);
   if (!Data.pData) {
      MessageBox (hWnd, "Speech recognition engine doesn't use a wave device.", NULL, MB_OK);
      pIAudio->Release();
      return FALSE;
   }
   pIAudio->Release();

   // open wave in & wave out with the same format & see if that works
   // ahead of time
   MMRESULT mm;
   HWAVEIN hwi;
   HWAVEOUT hwo;
   mm = waveInOpen (&hwi, dwWaveIn, (LPWAVEFORMATEX) Data.pData,
      (DWORD) hWnd, 0, CALLBACK_WINDOW);
   if (mm) {
      CoTaskMemFree (Data.pData);
      MessageBox (hWnd, "Can't open wave-in device. Another application must be using it.", NULL, MB_OK);
      return FALSE;
   }

   // open wave out at same time, using same format
   hwo = NULL;
   mm = waveOutOpen (&hwo, dwWaveOut, (LPWAVEFORMATEX) Data.pData,
      (DWORD) hWnd, 0, CALLBACK_WINDOW);
   if (mm) {
      // can't open with same bits per sample. Try different bits per sample
      PWAVEFORMATEX  pwfex;
      pwfex = (PWAVEFORMATEX) Data.pData;
      pwfex->nAvgBytesPerSec /= 2;
      pwfex->nBlockAlign /= 2;
      pwfex->wBitsPerSample /=2;

      mm = waveOutOpen (&hwo, dwWaveOut, (LPWAVEFORMATEX) Data.pData,
         (DWORD) hWnd, 0, CALLBACK_WINDOW);

      if (mm) {
         waveInClose (hwi);
         hwi = NULL;
         MessageBox (hWnd, "Full duplex not supported", NULL, MB_OK);
         CoTaskMemFree (Data.pData);
         return FALSE;
      }
      else
         MessageBox (hWnd, "Warning: Full duplex supported, but TTS must use 8 bits per sample instead of 16", "Warning", MB_OK);
   }

   // close both of these since we have the information
   if (hwi)
      waveInClose (hwi);
   if (hwo)
      waveOutClose (hwo);

   // try to create a tts engine and force it to the required sampling rate
   hRes = gpCTTSMode->InitAudioDestMM (dwWaveOut);
   if (hRes) {
      MessageBox (hWnd, "Can't select wave device.", NULL, MB_OK);
      return FALSE;
   }
   pIAudio = NULL;
   gpCTTSMode->m_pUnkAudio->QueryInterface (IID_IAudio, (PVOID*)&pIAudio);
   if (!pIAudio) {
      MessageBox (hWnd, "Unexpected things happened.", NULL, MB_OK);
      return FALSE;
   }
   hRes = pIAudio->WaveFormatSet (Data);
   pIAudio->Release();
   if (hRes) {
      MessageBox (hWnd, "Unexpected things happened.", NULL, MB_OK);
      return FALSE;
   }
   hRes = gpCTTSMode->Init (gTTS);
   if (hRes) {
      MessageBox (hWnd, "Can't load the text-to-speech engine with the forced wave format.", NULL, MB_OK);
      return FALSE;
   }

   CoTaskMemFree (Data.pData);
   return TRUE;
}


/*******************************************************
SelectDlgProc - Allow the user to select which SR/TTS engine and wave
   device to use. It sets up the globals in the prcoess.

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK SelectDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      {
         char  szTemp[128];

         // fill the list of SR engines
         CSREnum  SREnum;
         SREnum.Init();
         SRMODEINFOW srmi;
         while (!SREnum.Next (&srmi)) {
            WideCharToMultiByte (CP_ACP, 0, srmi.szModeName, -1, szTemp, sizeof(szTemp), 0,0);
            SendDlgItemMessage (hWnd, IDC_SR, CB_ADDSTRING, 0, (LPARAM) szTemp);
         }
         SendDlgItemMessage (hWnd, IDC_SR, CB_SETCURSEL, 0, 0);

         // fill the list of TTS engines
         CTTSEnum   TTSEnum;
         TTSEnum.Init();
         TTSMODEINFOW ttsmi;
         while (!TTSEnum.Next (&ttsmi)) {
            WideCharToMultiByte (CP_ACP, 0, ttsmi.szModeName, -1, szTemp, sizeof(szTemp), 0,0);
            SendDlgItemMessage (hWnd, IDC_TTS, CB_ADDSTRING, 0, (LPARAM) szTemp);
         }
         SendDlgItemMessage (hWnd, IDC_TTS, CB_SETCURSEL, 0, 0);

         // fill the list of wave-in devices
         DWORD i, dwNum;
         WAVEINCAPS  wic;
         dwNum = waveInGetNumDevs();
         for (i = 0; i < dwNum; i++) {
            waveInGetDevCaps (i, &wic, sizeof(wic));
            SendDlgItemMessage (hWnd, IDC_WAVEIN, CB_ADDSTRING, 0, (LPARAM) wic.szPname);
         }
         SendDlgItemMessage (hWnd, IDC_WAVEIN, CB_SETCURSEL, 0, 0);

         // fill the list of wave-out devices
         WAVEOUTCAPS  woc;
         dwNum = waveOutGetNumDevs();
         for (i = 0; i < dwNum; i++) {
            waveOutGetDevCaps (i, &woc, sizeof(woc));
            SendDlgItemMessage (hWnd, IDC_WAVEOUT, CB_ADDSTRING, 0, (LPARAM) woc.szPname);
         }
         SendDlgItemMessage (hWnd, IDC_WAVEOUT, CB_SETCURSEL, 0, 0);
      }
      break;   // continue on
   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDOK:
         {
            // get the index into the engines & wave devices
            DWORD dwSR, dwTTS, dwWaveIn, dwWaveOut;
            dwSR = SendDlgItemMessage (hWnd, IDC_SR, CB_GETCURSEL, 0, 0);
            dwTTS = SendDlgItemMessage (hWnd, IDC_TTS, CB_GETCURSEL, 0, 0);
            dwWaveIn = SendDlgItemMessage (hWnd, IDC_WAVEIN, CB_GETCURSEL, 0, 0);
            dwWaveOut = SendDlgItemMessage (hWnd, IDC_WAVEOUT, CB_GETCURSEL, 0, 0);

            // figure out the guids for SR/tts
            SRMODEINFOW srmi;
            TTSMODEINFOW   ttsmi;
            CSREnum  SREnum;
            CTTSEnum   TTSEnum;
            SREnum.Init();
            TTSEnum.Init();
            SREnum.Skip (dwSR);
            TTSEnum.Skip (dwTTS);
            SREnum.Next (&srmi);
            TTSEnum.Next (&ttsmi);

            // try to load these
            if (LoadFullDuplex (hWnd, srmi.gModeID, dwWaveIn, ttsmi.gModeID, dwWaveOut))
               EndDialog (hWnd, IDOK);

         }
         return 1;
      case IDCANCEL:
         EndDialog (hWnd, IDCANCEL);
         return 1;
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
   HRESULT hRes;
   CoInitialize(NULL);

   // pull up test dialog
   if (IDOK != DialogBox (hInstance, MAKEINTRESOURCE(IDD_SELECT),
      NULL, (DLGPROC) SelectDlgProc))
      goto alldone;

   char  *szGrammar;
   szGrammar =
      "[Grammar]\n"
      "LangID=1033\n"
      "[<Start>]\n"
      "<Start>=<Float>\n";
   gGramNotifySink = new CTestGramNotify;
   if(!gGramNotifySink)
		  return - 1;
   gGramNotifySink->AddRef();
   gpCSRGramComp = gpCSRMode->GrammarFromMemory (gGramNotifySink,
      szGrammar, strlen(szGrammar)+1);
   if (!gpCSRGramComp) {
      MessageBox (NULL, "Couldn't load digits grammar.", NULL, MB_OK);
      goto alldone;
   }

   hRes = gpCSRGramComp->Activate(NULL);
   if (hRes) {
      MessageBox (NULL, "Speech recognition failed to listen. You may not have a full duplex sound card.", NULL, MB_OK);
      goto alldone;
   }

   // pull up test dialog
   DialogBox (hInstance, MAKEINTRESOURCE(IDD_DEMO),
      NULL, (DLGPROC) _DemoDlgProc);

alldone:
   // free up
   if(gGramNotifySink) {
      gGramNotifySink->Release();
   }
   if (gpCSRGramComp)
      delete gpCSRGramComp;
   // Free TTS
   if (gpCTTSMode)
      delete gpCTTSMode;
   if (gpCSRMode)
      delete gpCSRMode;

   CoUninitialize();

   return 0;
}



