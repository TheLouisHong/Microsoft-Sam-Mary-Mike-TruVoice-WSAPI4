/****************************************************************************
main.cpp - Main code for msmouth.

Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>
#include <initguid.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <speech.h>
#include <dsound.h>
#include "resource.h"
#include "paint.h"

#define  WM_UPDATESLIDERS  (WM_USER+89)

/************************************************************************
Notification objects */
class CTTSNotify : public ITTSNotifySink {
   public:
      CTTSNotify (void);
      ~CTTSNotify (void);

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
typedef CTTSNotify * PCTTSNotify;

/* voice text notification sink */
class CVTxtNotify : public IVTxtNotifySink {
   public:
      CVTxtNotify (void);
      ~CVTxtNotify (void);

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // IVTxtNotifySink
		STDMETHOD (AttribChanged)  (DWORD);
		STDMETHOD (Speak)          (PTSTR, PTSTR, DWORD);
		STDMETHOD (SpeakingDone)   (void);
		STDMETHOD (SpeakingStarted)(void);
		STDMETHOD (Visual)         (WCHAR, TCHAR, DWORD, PTTSMOUTH);
   };
typedef CVTxtNotify * PCVTxtNotify;

typedef struct {
   RECT     rPosn;      // position
   BOOL     fUseTagged; // TRUE then used tagged
   GUID     gMode;      // TTS mode
} MOUTHREG, *PMOUTHREG;

HINSTANCE   ghInstance = NULL;
HWND        ghWndMouth = NULL;
MOUTHREG    gMouthReg;
RECT        gRectClient;
TTSMOUTH    gTTSMouth = {0, 192, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00};

PITTSATTRIBUTES   gpITTSAttributes = NULL;
PITTSCENTRAL      gpITTSCentral = NULL;
PIVOICETEXT       gpIVoiceText = NULL;
LPUNKNOWN         gpIAudioUnk = NULL;
CTTSNotify        gTTSNotify;
CVTxtNotify       gVTxtNotify;
LPDIRECTSOUND     gpDirectSound = NULL;
GUID  gNULL;

/********************************************************************
PaintToDC - Paints the mouth to a DC

inputs
   HDC dc
*/
void PaintToDC (HDC hdc)
{
   // Cache & draw onto a bitmap
   HDC      hdcTemp;
   HBITMAP  hBmp;
   hdcTemp = CreateCompatibleDC (hdc);
   hBmp = CreateCompatibleBitmap (hdc, gRectClient.right, gRectClient.bottom);
   SelectObject (hdcTemp, hBmp);

   PaintMouth (&gTTSMouth, hdcTemp, &gRectClient);
   BitBlt(
      hdc,
      0, 0, gRectClient.right, gRectClient.bottom,
      hdcTemp,
      0, 0,
      SRCCOPY);
   DeleteDC (hdcTemp);
   DeleteObject (hBmp);
}


/*************************************************************************
CTTSNotify - Notification object.
*/
CTTSNotify::CTTSNotify (void)
{
}

CTTSNotify::~CTTSNotify (void)
{
// this space intentionally left blank
}

STDMETHODIMP CTTSNotify::QueryInterface (REFIID riid, LPVOID *ppv)
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

STDMETHODIMP_ (ULONG) CTTSNotify::AddRef (void)
{
	// normally this increases a reference count, but this object
	// is going to be freed as soon as the app is freed, so it doesn't
	// matter
	return 1;
}

STDMETHODIMP_(ULONG) CTTSNotify::Release (void)
{
	// normally this releases a reference count, but this object
	// is going to be freed when the application is freed so it doesnt
	// matter
	return 1;
}

STDMETHODIMP CTTSNotify::AttribChanged (DWORD dwAttribID)
{
   return NOERROR;
}

STDMETHODIMP CTTSNotify::AudioStart (QWORD qTimeStamp)
{
   return NOERROR;
}

STDMETHODIMP CTTSNotify::AudioStop (QWORD qTimeStamp)
{
   return NOERROR;
}

STDMETHODIMP CTTSNotify::Visual (QWORD qTimeStamp, CHAR cIPAPhoneme,
				CHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth)
{
   if (pTTSMouth) {
      gTTSMouth = *pTTSMouth;

      HDC   hdc;
      hdc = GetDC (ghWndMouth);
      PaintToDC (hdc);
      ReleaseDC (ghWndMouth, hdc);
   }
   return NOERROR;
}

/*************************************************************************
CVTxtNotify - Notification object.
*/
CVTxtNotify::CVTxtNotify (void)
{
}

CVTxtNotify::~CVTxtNotify (void)
{
// this space intentionally left blank
}

STDMETHODIMP CVTxtNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
	*ppv = NULL;

	/* always return our IUnknown for IID_IUnknown */
	if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IVTxtNotifySink))
	{
		*ppv = (LPVOID) this;
		return S_OK;
	}

	// otherwise, cant find
	return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CVTxtNotify::AddRef (void)
{
	// normally this increases a reference count, but this object
	// is going to be freed as soon as the app is freed, so it doesn't
	// matter
	return 1;
}

STDMETHODIMP_(ULONG) CVTxtNotify::Release (void)
{
	// normally this releases a reference count, but this object
	// is going to be freed when the application is freed so it doesnt
	// matter
	return 1;
}

STDMETHODIMP CVTxtNotify::AttribChanged (DWORD dwAttribID)
{
   return NOERROR;
}

STDMETHODIMP CVTxtNotify::Speak (PTSTR a, PTSTR b, DWORD c)
{
   return NOERROR;
}

STDMETHODIMP CVTxtNotify::SpeakingDone (void)
{
   return NOERROR;
}

STDMETHODIMP CVTxtNotify::SpeakingStarted (void)
{
   return NOERROR;
}

STDMETHODIMP CVTxtNotify::Visual (WCHAR cIPAPhoneme,
				TCHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth)
{
   if (pTTSMouth) {
      gTTSMouth = *pTTSMouth;

      HDC   hdc;
      hdc = GetDC (ghWndMouth);
      PaintToDC (hdc);
      ReleaseDC (ghWndMouth, hdc);
   }
   return NOERROR;
}

/********************************************************************
StartVoiceText - This starts voice text so the mouth will also
   animate if any application speaks using voice text. It doesn't
   do anything else, like make sure it's the right gender or
   allow pausing/stopping/resuming, or changing the voice
*/
void StartVoiceText (void)
{
   HRESULT  hRes;
   hRes = CoCreateInstance(CLSID_VTxt, NULL, CLSCTX_ALL,
      IID_IVoiceText, (void**)&gpIVoiceText);
   if (hRes)
      return;

   hRes = gpIVoiceText->Register (NULL, "Microsoft Mouth",
      &gVTxtNotify, IID_IVTxtNotifySink,  VTXTF_ALLMESSAGES, NULL);
}

/********************************************************************
LoadTTS - Loads a TTS engine. It frees up the old TTS engine.

inputs
   GUID  *pgMode - Mode to look for. If NULL then free TTS engine & dont load
returns
   HRESULT - error
*/
HRESULT LoadTTS (GUID *pgMode)
{
   if (gpITTSAttributes) {
      gpITTSAttributes->Release();
      gpITTSAttributes = NULL;
   }
   if (gpITTSCentral) {
      gpITTSCentral->Release();
      gpITTSCentral = NULL;
   }

   // HACK - Try waiting till all messages are cleared
    MSG msg;
	while( PeekMessage(&msg, NULL, NULL, NULL,PM_REMOVE) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

   // HACK - Release audio source
   if (gpIAudioUnk) {
      while (gpIAudioUnk->Release());
      gpIAudioUnk = NULL;
   }

// #define USEDIRECT

   // rfelease direct sound
#ifdef USEDIRECT
   if (gpDirectSound) {
      gpDirectSound->Release();
      gpDirectSound = NULL;
   }
#endif // USEDIRECT

   if (!pgMode)
      return NOERROR;

   // audio object
   HRESULT  hRes;
#ifdef USEDIRECT
   // crreate direct sound stuff
   hRes = CoCreateInstance (CLSID_DirectSound, NULL,
      CLSCTX_ALL, IID_IDirectSound, (LPVOID*) &gpDirectSound);
   if (hRes)
      return hRes;
   hRes = gpDirectSound->Initialize(NULL);
   if (hRes)
      return hRes;

   hRes = gpDirectSound->SetCooperativeLevel (ghWndMouth, DSSCL_NORMAL);
   if (hRes)
      return hRes;

   PIAUDIODIRECT pIAD;
	hRes = CoCreateInstance (CLSID_AudioDestDirect, NULL, CLSCTX_ALL, IID_IAudioDirect, (void**)&pIAD);
   if (hRes)
      return hRes;

   // tell the audio object about our stuff
   gpDirectSound->AddRef();
   hRes = pIAD->Init ((PVOID) gpDirectSound,IID_IDirectSound);
   if (hRes)
      return hRes;
   gpIAudioUnk = (LPUNKNOWN) pIAD;

#else
   hRes = CoCreateInstance(CLSID_MMAudioDest, NULL, CLSCTX_ALL,
      IID_IUnknown, (void**)&gpIAudioUnk);
   if (hRes)
      return hRes;
#endif

   TTSMODEINFO    ttsResult;        // final result
   PITTSFIND      pITTSFind;             // find interface
   TTSMODEINFO    mi;

   // fill in the moed inf
   memset (&mi, 0 ,sizeof(mi));
   mi.gModeID = *pgMode;
   mi.dwFeatures = TTSFEATURE_VISUAL;

   hRes = CoCreateInstance(CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSFind,
                                                   (void**)&pITTSFind);
   if (FAILED(hRes)) return hRes;

   hRes = pITTSFind->Find(&mi, NULL, &ttsResult);

   if (hRes)
      {
      pITTSFind->Release();
      return hRes;     // error
      }

   // rewrite the mode
   *pgMode = ttsResult.gModeID;

   // Should do select now

   gpIAudioUnk->AddRef();
   hRes = pITTSFind->Select(ttsResult.gModeID, &gpITTSCentral,
      (LPUNKNOWN) gpIAudioUnk);
   if (hRes) {
      pITTSFind->Release();
      gpIAudioUnk->Release();
      return NULL;
      };


   pITTSFind->Release();
   gpITTSCentral->QueryInterface(IID_ITTSAttributes, (LPVOID *)&gpITTSAttributes);

   // Register notification sink
   DWORD dwKey;
	gpITTSCentral->Register((void*)&gTTSNotify, IID_ITTSNotifySink,
                                 &dwKey);

   // figure out the gender and refresh
   PaintGender (ttsResult.wGender == GENDER_MALE);
   if (ghWndMouth)
      InvalidateRect (ghWndMouth, NULL, FALSE);


   return NOERROR;
}

/*************************************************************************
ExerciseDialogProc
*/
BOOL CALLBACK ExerciseDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
      {
         // set limits on all the sliders
         SendDlgItemMessage(hWnd, IDC_MOUTHHEIGHT, TBM_SETRANGE,
            FALSE, MAKELONG(0, 255) );
         SendDlgItemMessage(hWnd, IDC_MOUTHWIDTH, TBM_SETRANGE,
            FALSE, MAKELONG(0, 255) );
         SendDlgItemMessage(hWnd, IDC_MOUTHUPTURN, TBM_SETRANGE,
            FALSE, MAKELONG(0, 255) );
         SendDlgItemMessage(hWnd, IDC_UPPERTEETH, TBM_SETRANGE,
            FALSE, MAKELONG(0, 255) );
         SendDlgItemMessage(hWnd, IDC_LOWERTEETH, TBM_SETRANGE,
            FALSE, MAKELONG(0, 255) );
         SendDlgItemMessage(hWnd, IDC_TONGUE, TBM_SETRANGE,
            FALSE, MAKELONG(0, 255) );
         SendDlgItemMessage(hWnd, IDC_LIPTENSION, TBM_SETRANGE,
            FALSE, MAKELONG(0, 255) );

         SendDlgItemMessage (hWnd, IDC_MOUTHHEIGHT, TBM_SETPOS, TRUE,
            gTTSMouth.bMouthHeight);
         SendDlgItemMessage (hWnd, IDC_MOUTHWIDTH, TBM_SETPOS, TRUE,
            gTTSMouth.bMouthWidth);
         SendDlgItemMessage (hWnd, IDC_MOUTHUPTURN, TBM_SETPOS, TRUE,
            gTTSMouth.bMouthUpturn);
         SendDlgItemMessage (hWnd, IDC_UPPERTEETH, TBM_SETPOS, TRUE,
            gTTSMouth.bTeethUpperVisible);
         SendDlgItemMessage (hWnd, IDC_LOWERTEETH, TBM_SETPOS, TRUE,
            gTTSMouth.bTeethLowerVisible);
         SendDlgItemMessage (hWnd, IDC_TONGUE, TBM_SETPOS, TRUE,
            gTTSMouth.bTonguePosn);
         SendDlgItemMessage (hWnd, IDC_LIPTENSION, TBM_SETPOS, TRUE,
            gTTSMouth.bLipTension);

      }
      break;
   case WM_HSCROLL:
      {
      // Handle scroll bars
      HWND  hWndScroll = (HWND) lParam;
      int   iPos, iMin, iMax, iDelta;
      iPos = SendMessage (hWndScroll, TBM_GETPOS, 0, 0);
      iMin = SendMessage (hWndScroll, TBM_GETRANGEMIN, 0, 0);
      iMax = SendMessage (hWndScroll, TBM_GETRANGEMAX, 0, 0);
      iDelta = (iMax - iMin) / 10;
   
      switch (LOWORD(wParam)) {
      case SB_BOTTOM:
         //iPos = iMax;
         break;
      case SB_ENDSCROLL:
         return TRUE;   // do nothing
      case SB_LINELEFT:
      case SB_PAGELEFT:
         //iPos -= iDelta;
         break;
      case SB_LINERIGHT:
      case SB_PAGERIGHT:
         //iPos += iDelta;
         break;
      case SB_THUMBTRACK:
         return TRUE;
      case SB_THUMBPOSITION:
         iPos = HIWORD(wParam);
         break;
      case SB_TOP:
         iPos = iMin;
         break;
      }

      if (iPos < iMin)
         iPos = iMin;
      if (iPos > iMax)
         iPos = iMax;
      SendMessage (hWndScroll, TBM_SETPOS, TRUE, iPos);

      // Get the pitch & volume & set those
      PostMessage (hWnd, WM_UPDATESLIDERS, 0, 0);

      return TRUE;
      }  
      break;
   case WM_CLOSE:
      SendMessage (hWnd, WM_COMMAND, IDOK, 0);
      return TRUE;
   case WM_UPDATESLIDERS:
      {
         gTTSMouth.bMouthHeight = (BYTE) SendDlgItemMessage (hWnd,
            IDC_MOUTHHEIGHT, TBM_GETPOS, 0, 0);
         gTTSMouth.bMouthWidth = (BYTE) SendDlgItemMessage (hWnd,
            IDC_MOUTHWIDTH, TBM_GETPOS, 0, 0);
         gTTSMouth.bMouthUpturn = (BYTE) SendDlgItemMessage (hWnd,
            IDC_MOUTHUPTURN, TBM_GETPOS, 0, 0);
         gTTSMouth.bTeethUpperVisible = (BYTE) SendDlgItemMessage (hWnd,
            IDC_UPPERTEETH, TBM_GETPOS, 0, 0);
         gTTSMouth.bTeethLowerVisible = (BYTE) SendDlgItemMessage (hWnd,
            IDC_LOWERTEETH, TBM_GETPOS, 0, 0);
         gTTSMouth.bTonguePosn = (BYTE) SendDlgItemMessage (hWnd,
            IDC_TONGUE, TBM_GETPOS, 0, 0);
         gTTSMouth.bLipTension = (BYTE) SendDlgItemMessage (hWnd,
            IDC_LIPTENSION, TBM_GETPOS, 0, 0);

         InvalidateRect (ghWndMouth, NULL, FALSE);
      }
      break;
   case WM_COMMAND:
      switch (LOWORD(wParam))
      {
      case IDOK:
         // get the check box item
         gMouthReg.fUseTagged = IsDlgButtonChecked(hWnd, IDC_TAGGED);
         EndDialog (hWnd, IDOK);
      }
      break;
   };

   return FALSE;  // didn't handle
}



/*************************************************************************
FromTextDialogProc
*/
BOOL CALLBACK FromTextDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_CLOSE:
      SendMessage (hWnd, WM_COMMAND, IDCANCEL, 0);
      return TRUE;
   case WM_COMMAND:
      switch (LOWORD(wParam))
      {
      case IDOK:
         {
            // play a sample
            SDATA d;
            char  szTemp[10000];
            GetDlgItemText (hWnd, IDC_TEXT, szTemp, sizeof(szTemp));
            d.pData = szTemp;
            d.dwSize = strlen(szTemp)+1;
            gpITTSCentral->TextData(CHARSET_TEXT,
               gMouthReg.fUseTagged ? TTSDATAFLAG_TAGGED: 0, d, NULL,  IID_ITTSBufNotifySink);
         }
         break;
      case IDCANCEL:
         EndDialog (hWnd, IDCANCEL);
         return TRUE;
      break;
      }
   };

   return FALSE;  // didn't handle
}


/*************************************************************************
SettingsDialogProc
*/
BOOL CALLBACK SettingsDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#define MAXVOICE  100
   static GUID agVoice[MAXVOICE];

   switch (uMsg) {
   case WM_INITDIALOG:
      {
         // check dialog
         CheckDlgButton (hWnd, IDC_TAGGED,
            gMouthReg.fUseTagged ? BST_CHECKED : BST_UNCHECKED);

         // initalize the list box
         PITTSENUM   pITTSEnum = NULL;
         CoCreateInstance(CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSEnum,
                                                   (void**)&pITTSEnum);

         DWORD i;
         TTSMODEINFO inf;
         DWORD dwTimes;
         for (i = 0; ; i++) {
            if (pITTSEnum->Next(1, &inf, &dwTimes))
               break;

            // add the name
            agVoice[i] = inf.gModeID;
            SendDlgItemMessage (hWnd, IDC_VOICE, CB_ADDSTRING, 0, (LPARAM) inf.szModeName);
            if (IsEqualGUID (inf.gModeID, gMouthReg.gMode))
               SendDlgItemMessage (hWnd, IDC_VOICE, CB_SETCURSEL, i, 0);

         }
         pITTSEnum->Release();

         // Get the pitch & volume & set those
         SendMessage (hWnd, WM_UPDATESLIDERS, 0, 0);

      }
      break;
   case WM_HSCROLL:
      {
      // Handle scroll bars
      HWND  hWndScroll = (HWND) lParam;
      int   iPos, iMin, iMax, iDelta;
      iPos = SendMessage (hWndScroll, TBM_GETPOS, 0, 0);
      iMin = SendMessage (hWndScroll, TBM_GETRANGEMIN, 0, 0);
      iMax = SendMessage (hWndScroll, TBM_GETRANGEMAX, 0, 0);
      iDelta = (iMax - iMin) / 10;
   
      switch (LOWORD(wParam)) {
      case SB_BOTTOM:
         //iPos = iMax;
         break;
      case SB_ENDSCROLL:
         return TRUE;   // do nothing
      case SB_LINELEFT:
      case SB_PAGELEFT:
         //iPos -= iDelta;
         break;
      case SB_LINERIGHT:
      case SB_PAGERIGHT:
         //iPos += iDelta;
         break;
      case SB_THUMBTRACK:
         return TRUE;
      case SB_THUMBPOSITION:
         iPos = HIWORD(wParam);
         break;
      case SB_TOP:
         iPos = iMin;
         break;
      }

      if (iPos < iMin)
         iPos = iMin;
      if (iPos > iMax)
         iPos = iMax;
      SendMessage (hWndScroll, TBM_SETPOS, TRUE, iPos);

      // Set the attribute
      if (hWndScroll == GetDlgItem (hWnd, IDC_PITCH))
         gpITTSAttributes->PitchSet ((WORD) iPos);
      else
         gpITTSAttributes->SpeedSet ((WORD) iPos);

      // reset & play a sample
      SDATA d;
      char  szSpeak[] = "Hello.";
      d.pData = szSpeak;
      d.dwSize = sizeof(szSpeak);
      gpITTSCentral->AudioReset();
      gpITTSCentral->TextData(CHARSET_TEXT,
         gMouthReg.fUseTagged ? TTSDATAFLAG_TAGGED: 0, d, NULL,  IID_ITTSBufNotifySink);

      return TRUE;
      }  
      break;
   case WM_CLOSE:
      SendMessage (hWnd, WM_COMMAND, IDOK, 0);
      return TRUE;
   case WM_UPDATESLIDERS:
      {
         DWORD dwMin, dwMax, dwCur;
         WORD wMin, wMax, wCur;

         // pitch
         wMin = wMax = wCur = 0;
         gpITTSAttributes->PitchGet(&wCur);
         gpITTSAttributes->PitchSet(TTSATTR_MINPITCH);
         gpITTSAttributes->PitchGet(&wMin);
         gpITTSAttributes->PitchSet(TTSATTR_MAXPITCH);
         gpITTSAttributes->PitchGet(&wMax);
         gpITTSAttributes->PitchSet(wCur);

         SendDlgItemMessage(hWnd, IDC_PITCH, TBM_SETRANGE,
            FALSE, MAKELONG((WORD)wMin, (WORD)wMax) );
         SendDlgItemMessage(hWnd, IDC_PITCH, TBM_SETPOS,
            TRUE, wCur );

         // speed
         dwMin = dwMax = dwCur = 0;
         gpITTSAttributes->SpeedGet(&dwCur);
         gpITTSAttributes->SpeedSet(TTSATTR_MINSPEED);
         gpITTSAttributes->SpeedGet(&dwMin);
         gpITTSAttributes->SpeedSet(TTSATTR_MAXSPEED);
         gpITTSAttributes->SpeedGet(&dwMax);
         gpITTSAttributes->SpeedSet(dwCur);

         SendDlgItemMessage(hWnd, IDC_SPEED, TBM_SETRANGE,
            FALSE, MAKELONG((WORD)dwMin, (WORD)dwMax) );
         SendDlgItemMessage(hWnd, IDC_SPEED, TBM_SETPOS,
            TRUE, dwCur );
      }
      break;
   case WM_COMMAND:
      switch (LOWORD(wParam))
      {
      case IDC_SPEAK:
         {
            // play a sample
            SDATA d;
            char  szTemp[10000];
            GetDlgItemText (hWnd, IDC_TEXT, szTemp, sizeof(szTemp));
            d.pData = szTemp;
            d.dwSize = strlen(szTemp)+1;
            gpITTSCentral->TextData(CHARSET_TEXT,
               IsDlgButtonChecked(hWnd, IDC_TAGGED) ? TTSDATAFLAG_TAGGED: 0, d, NULL,  IID_ITTSBufNotifySink);
         }
         break;
      case IDC_VOICE:
         {
            if (HIWORD(wParam) != LBN_SELCHANGE)
               return FALSE;
            DWORD i;
            i = SendDlgItemMessage (hWnd, IDC_VOICE, CB_GETCURSEL, 0, 0);
            if (i < MAXVOICE) {
               gMouthReg.gMode = agVoice[i];

               HCURSOR   hCur;
               hCur = SetCursor(LoadCursor(NULL, IDC_WAIT));
               LoadTTS (&gMouthReg.gMode);
               SetCursor (hCur);

               // Get the pitch & volume & set those
               SendMessage (hWnd, WM_UPDATESLIDERS, 0, 0);
            }
         }
         
         break;
      case IDOK:
         // get the check box item
         gMouthReg.fUseTagged = IsDlgButtonChecked(hWnd, IDC_TAGGED);
         EndDialog (hWnd, IDOK);
      }
      break;
   };

   return FALSE;  // didn't handle
}



/********************************************************************
MouthWndProc - Editor window proc
*/
LONG CALLBACK MouthWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
   case WM_CREATE:
      {
         ghWndMouth = hwnd;

         if (LoadTTS(&gNULL)) {
            MessageBox (hwnd, "Can't load a text-to-speech engine.", NULL, MB_OK);
            PostMessage (hwnd, WM_CLOSE, 0, 0);
            return 0;
         }

         SDATA d;
         char  szSpeak[] = "Hello.";
         d.pData = szSpeak;
         d.dwSize = sizeof(szSpeak);
         gpITTSCentral->TextData(CHARSET_TEXT,
            gMouthReg.fUseTagged ? TTSDATAFLAG_TAGGED: 0, d, NULL,  IID_ITTSBufNotifySink);
      }
      break;
   case WM_PAINT:
      {
         PAINTSTRUCT ps;
         HDC   hdc;
         hdc = BeginPaint (hwnd, &ps);
         PaintToDC (hdc);

         EndPaint (hwnd, &ps);
      }
      break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
      case ID_SPEAK_OPTIONS:
         DialogBox (ghInstance, MAKEINTRESOURCE(IDD_SETTINGS),
            hwnd, (DLGPROC) SettingsDialogProc);
         break;
      case ID_SPEAK_EXERCISEMOUTH:
         DialogBox (ghInstance, MAKEINTRESOURCE(IDD_EXERCISE),
            hwnd, (DLGPROC) ExerciseDialogProc);
         break;
      case ID_SPEAK_FROMTEXT:
         DialogBox (ghInstance, MAKEINTRESOURCE(IDD_FROMTEXT),
            hwnd, (DLGPROC) FromTextDialogProc);
         break;
      case ID_SPEAK_STOP:
         gpITTSCentral->AudioReset();
         break;
      case ID_SPEAK_PAUSE:
         gpITTSCentral->AudioPause();
         break;
      case ID_SPEAK_RESUME:
         gpITTSCentral->AudioResume();
         break;
	   case ID_EXIT:
         PostMessage (hwnd, WM_CLOSE, 0,0);
		   break;
      case ID_SPEAK_INTRODUCTION:
         {
            SDATA d;
            char  szSpeak[] = "Welcome to the Microsoft Mouth application. "
               "It speaks text that you type in or copy onto the clipboard.";
            d.pData = szSpeak;
            d.dwSize = sizeof(szSpeak);
            gpITTSCentral->TextData(CHARSET_TEXT,
               gMouthReg.fUseTagged ? TTSDATAFLAG_TAGGED: 0, d, NULL,  IID_ITTSBufNotifySink);
            return 0;
         }
         break;
      case ID_SPEAK_FROMCLIPBOARD:
         {
         PTSTR    pszClipboard;
         HANDLE   hClipData;
         BOOL     fRet;

         fRet = OpenClipboard(NULL);
         if (!fRet) {
            SDATA d;
            char  szSpeak[] = "Can't open the clipboard.";
            d.pData = szSpeak;
            d.dwSize = sizeof(szSpeak);
            gpITTSCentral->TextData(CHARSET_TEXT,
               gMouthReg.fUseTagged ? TTSDATAFLAG_TAGGED: 0, d, NULL,  IID_ITTSBufNotifySink);
            return 0;
         }

         if (!(hClipData = GetClipboardData(CF_TEXT)))
         {
            SDATA d;
            char  szSpeak[] = "You must copy something to the clipboard.";
            d.pData = szSpeak;
            d.dwSize = sizeof(szSpeak);
            gpITTSCentral->TextData(CHARSET_TEXT,
               gMouthReg.fUseTagged ? TTSDATAFLAG_TAGGED: 0, d, NULL,  IID_ITTSBufNotifySink);
            return 0;
         }

         if (!(pszClipboard = (PTSTR)GlobalLock(hClipData)))
         {
            CloseClipboard();
            return 0;
         }

         SDATA d;
         d.pData = pszClipboard;
         d.dwSize = strlen(pszClipboard)+1;
         gpITTSCentral->TextData(CHARSET_TEXT,
            gMouthReg.fUseTagged ? TTSDATAFLAG_TAGGED: 0, d,
            NULL,  IID_ITTSBufNotifySink);


         GlobalUnlock(hClipData);
         CloseClipboard();
         break;
         }
		}
		break;
	case WM_CLOSE:
      {
      gpITTSCentral->AudioReset();

      // get the screen position
      WINDOWPLACEMENT wp;
      wp.length = sizeof(wp);
      GetWindowPlacement (ghWndMouth, &wp);
      gMouthReg.rPosn = wp.rcNormalPosition;

		DestroyWindow (hwnd);
      return 0;
      }
	case WM_SIZE:
      // Ask to redraw mouth
      GetClientRect (hwnd, &gRectClient);
      InvalidateRect (hwnd, NULL, FALSE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage (0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}




/********************************************************************
LoadDefaults - Load the defaults from the registry.

*/
void LoadDefaults (void)
{
   HKEY  hKey;

   // initialize defaults;
   memset (&gMouthReg, 0, sizeof(gMouthReg));
   gMouthReg.rPosn.left = 150;
   gMouthReg.rPosn.top = 150;
   gMouthReg.rPosn.right = 450;
   gMouthReg.rPosn.bottom = 300;
   gMouthReg.fUseTagged = FALSE;

   DWORD dwDisposition;
   if (RegCreateKeyEx(HKEY_CURRENT_USER, "software\\microsoft\\msmouth",
      NULL, 0, 0,
      KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition))
         return;

   // find out how much memory to allocate for the name
   DWORD dwNeeded = sizeof(gMouthReg);
   DWORD dwType;
   RegQueryValueEx(hKey, "Settings", NULL, &dwType, (PBYTE) &gMouthReg, &dwNeeded);

   RegCloseKey (hKey);

}


/********************************************************************
SaveDefaults - Save the defaults to the registry.

*/
void SaveDefaults (void)
{
   HKEY  hKey;

   DWORD dwDisposition;
   if (RegCreateKeyEx(HKEY_CURRENT_USER, "software\\microsoft\\msmouth",
      NULL, 0, 0,
      KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition))
         return;

   // find out how much memory to allocate for the name
   RegSetValueEx(hKey, "Settings", 0, REG_BINARY, (PBYTE) &gMouthReg, sizeof(gMouthReg));

   RegCloseKey (hKey);

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

	// init ole
	CoInitialize(NULL);
   InitCommonControls ();


	// register the windows class
	WNDCLASS wc;
	char	szClass[] = "MSMouth";
	memset (&wc, 0, sizeof(wc));
	wc.style = 0; //CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MouthWndProc;
	wc.hInstance = ghInstance;
	wc.hbrBackground = NULL; // (HBRUSH) (COLOR_BTNFACE+1);
	wc.lpszMenuName = MAKEINTRESOURCE (IDM_MOUTH);
	wc.lpszClassName = szClass;
   wc.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_MOUTH));
   wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	RegisterClass (&wc);

   // Remember which corner of the screen it was in
   LoadDefaults();
   memset (&gNULL, 0 ,sizeof(gNULL));
   // start voice text. Comment this out to speed up load time
   // StartVoiceText();

	// create the window
	ghWndMouth = CreateWindowEx (
		WS_EX_WINDOWEDGE | WS_EX_TOPMOST,
		szClass,
		"Microsoft Mouth",
		WS_OVERLAPPEDWINDOW | WS_SIZEBOX,
		gMouthReg.rPosn.left, gMouthReg.rPosn.top,
      gMouthReg.rPosn.right - gMouthReg.rPosn.left,
      gMouthReg.rPosn.bottom - gMouthReg.rPosn.top,
		NULL, NULL, ghInstance, NULL);
	if (!ghWndMouth)
		return -1;

	// message loop
    MSG msg;
	ShowWindow( ghWndMouth, nCmdShow );
	UpdateWindow( ghWndMouth );


	while( GetMessage(&msg, NULL, NULL, NULL) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


   SaveDefaults();

   // save the TTS voice to the registry so it's a default
   TTSMODEINFO   mi;
   memset (&mi, 0, sizeof(mi));
   if (gpITTSCentral)
      gpITTSCentral->ModeGet (&mi);
   HKEY  hKey;
   hKey = 0;

   DWORD dwDisposition;
   RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Voice\\VoiceText\\Local PC",
      NULL, 0, 0,
      KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
   if (hKey) {
      RegSetValueEx(hKey, "Mode", 0, REG_BINARY, (PBYTE) &mi.gModeID, sizeof(mi.gModeID));
      RegCloseKey (hKey);
   }



   // Free up TTS voice
   LoadTTS (NULL);
   if (gpIVoiceText)
      gpIVoiceText->Release();
	CoUninitialize ();

	// done
	return 0;
}

