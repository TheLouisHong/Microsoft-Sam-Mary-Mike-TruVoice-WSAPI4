/***********************************************************************
Demo.Cpp - Code to demo tts to DirectSound.



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

#include <dsound.h>
#include <speech.h>
#include "resource.h"

// in case direct sound 5 is not installed
#ifndef DSBCAPS_CTRL3D

#include <d3dtypes.h>

#define DSBCAPS_CTRL3D              0x00000010
#define DS3D_IMMEDIATE              0x00000000

// DirectSound3DListener {279AFA84-4981-11CE-A521-0020AF0BE560}
DEFINE_GUID(IID_IDirectSound3DListener, 0x279AFA84, 0x4981, 0x11CE, 0xA5, 0x21, 0x00, 0x20, 0xAF, 0x0B, 0xE5, 0x60);
// DirectSound3DBuffer {279AFA86-4981-11CE-A521-0020AF0BE560}
DEFINE_GUID(IID_IDirectSound3DBuffer, 0x279AFA86, 0x4981, 0x11CE, 0xA5, 0x21, 0x00, 0x20, 0xAF, 0x0B, 0xE5, 0x60);



typedef struct _DS3DLISTENER
{
    DWORD           dwSize;
    D3DVECTOR       vPosition;
    D3DVECTOR       vVelocity;
    D3DVECTOR       vOrientFront;
    D3DVECTOR       vOrientTop;
    D3DVALUE        flDistanceFactor;
    D3DVALUE        flRolloffFactor;
    D3DVALUE        flDopplerFactor;
} DS3DLISTENER, *LPDS3DLISTENER;

typedef const DS3DLISTENER *LPCDS3DLISTENER;


typedef struct _DS3DBUFFER
{
    DWORD           dwSize;
    D3DVECTOR       vPosition;
    D3DVECTOR       vVelocity;
    DWORD           dwInsideConeAngle;
    DWORD           dwOutsideConeAngle;
    D3DVECTOR       vConeOrientation;
    LONG            lConeOutsideVolume;
    D3DVALUE        flMinDistance;
    D3DVALUE        flMaxDistance;
    DWORD           dwMode;
} DS3DBUFFER, *LPDS3DBUFFER;

typedef const DS3DBUFFER *LPCDS3DBUFFER;


//
// IDirectSound3DBuffer
//

#undef INTERFACE
#define INTERFACE IDirectSound3DBuffer

DECLARE_INTERFACE_(IDirectSound3DBuffer, IUnknown)
{
    // IUnknown methods
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    // IDirectSoundBuffer3D methods
    STDMETHOD(GetAllParameters)     (THIS_ LPDS3DBUFFER) PURE;
    STDMETHOD(GetConeAngles)        (THIS_ LPDWORD, LPDWORD) PURE;
    STDMETHOD(GetConeOrientation)   (THIS_ LPD3DVECTOR) PURE;
    STDMETHOD(GetConeOutsideVolume) (THIS_ LPLONG) PURE;
    STDMETHOD(GetMaxDistance)       (THIS_ LPD3DVALUE) PURE;
    STDMETHOD(GetMinDistance)       (THIS_ LPD3DVALUE) PURE;
    STDMETHOD(GetMode)              (THIS_ LPDWORD) PURE;
    STDMETHOD(GetPosition)          (THIS_ LPD3DVECTOR) PURE;
    STDMETHOD(GetVelocity)          (THIS_ LPD3DVECTOR) PURE;
    STDMETHOD(SetAllParameters)     (THIS_ LPCDS3DBUFFER, DWORD) PURE;
    STDMETHOD(SetConeAngles)        (THIS_ DWORD, DWORD, DWORD) PURE;
    STDMETHOD(SetConeOrientation)   (THIS_ D3DVALUE, D3DVALUE, D3DVALUE, DWORD) PURE;
    STDMETHOD(SetConeOutsideVolume) (THIS_ LONG, DWORD) PURE;
    STDMETHOD(SetMaxDistance)       (THIS_ D3DVALUE, DWORD) PURE;
    STDMETHOD(SetMinDistance)       (THIS_ D3DVALUE, DWORD) PURE;
    STDMETHOD(SetMode)              (THIS_ DWORD, DWORD) PURE;
    STDMETHOD(SetPosition)          (THIS_ D3DVALUE, D3DVALUE, D3DVALUE, DWORD) PURE;
    STDMETHOD(SetVelocity)          (THIS_ D3DVALUE, D3DVALUE, D3DVALUE, DWORD) PURE;
};



//
// IDirectSound3DListener
//

#undef INTERFACE
#define INTERFACE IDirectSound3DListener

DECLARE_INTERFACE_(IDirectSound3DListener, IUnknown)
{
    // IUnknown methods
    STDMETHOD(QueryInterface)           (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)            (THIS) PURE;
    STDMETHOD_(ULONG,Release)           (THIS) PURE;

    // IDirectSound3D methods
    STDMETHOD(GetAllParameters)         (THIS_ LPDS3DLISTENER) PURE;
    STDMETHOD(GetDistanceFactor)        (THIS_ LPD3DVALUE) PURE;
    STDMETHOD(GetDopplerFactor)         (THIS_ LPD3DVALUE) PURE;
    STDMETHOD(GetOrientation)           (THIS_ LPD3DVECTOR, LPD3DVECTOR) PURE;
    STDMETHOD(GetPosition)              (THIS_ LPD3DVECTOR) PURE;
    STDMETHOD(GetRolloffFactor)         (THIS_ LPD3DVALUE) PURE;
    STDMETHOD(GetVelocity)              (THIS_ LPD3DVECTOR) PURE;
    STDMETHOD(SetAllParameters)         (THIS_ LPCDS3DLISTENER, DWORD) PURE;
    STDMETHOD(SetDistanceFactor)        (THIS_ D3DVALUE, DWORD) PURE;
    STDMETHOD(SetDopplerFactor)         (THIS_ D3DVALUE, DWORD) PURE;
    STDMETHOD(SetOrientation)           (THIS_ D3DVALUE, D3DVALUE, D3DVALUE, D3DVALUE, D3DVALUE, D3DVALUE, DWORD) PURE;
    STDMETHOD(SetPosition)              (THIS_ D3DVALUE, D3DVALUE, D3DVALUE, DWORD) PURE;
    STDMETHOD(SetRolloffFactor)         (THIS_ D3DVALUE, DWORD) PURE;
    STDMETHOD(SetVelocity)              (THIS_ D3DVALUE, D3DVALUE, D3DVALUE, DWORD) PURE;
    STDMETHOD(CommitDeferredSettings)   (THIS) PURE;
};


typedef struct IDirectSound3DListener       *LPDIRECTSOUND3DLISTENER;
typedef struct IDirectSound3DBuffer         *LPDIRECTSOUND3DBUFFER;

#endif


/*************************************************************************
Globals */

#define  NUMVOICES      3

HINSTANCE         ghInstance;                // instance handle
PITTSCENTRAL      gpITTSCentral[NUMVOICES];
PIAUDIODIRECT     gpIAD[NUMVOICES];
HWND              ghWndMain = NULL;
char              gszVoice[NUMVOICES][256];
LPDIRECTSOUND     lpDirectSound = NULL;
BOOL              gfUse3D = FALSE;
LPDIRECTSOUNDBUFFER gpdsbPrimary = NULL;

/*************************************************************************
FindAndSelect - This finds and selects according to the specific TTSMODEINFO.

inputs
   PTTSMODEINFO     pTTSInfo - desired mode
returns
   PITTSCENTRAL - ISRCentral interface to TTS engine
sets:

*/

PITTSCENTRAL FindAndSelect (PTTSMODEINFO pTTSInfo, PIAUDIODIRECT *ppIAD, DWORD dwNum)
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
   PIAUDIODIRECT pIAD;
	hRes = CoCreateInstance (CLSID_AudioDestDirect, NULL, CLSCTX_ALL, IID_IAudioDirect, (void**)&pIAD);
   if (hRes)
      {
      pITTSFind->Release();
      return NULL;     // error
      }

   // if use direct 3d then set some flags
   if (gfUse3D) {
      PIATTRIBUTESW  pIA = NULL;
      pIAD->QueryInterface (IID_IAttributesW, (PVOID*) &pIA);
      if (pIA) {
         DWORD dwFlags;
         pIA->DWORDGet (AUDATTR_DIRECTFLAGS, &dwFlags);
         dwFlags = (dwFlags | DSBCAPS_CTRL3D) & (~DSBCAPS_CTRLPAN);
         pIA->DWORDSet (AUDATTR_DIRECTFLAGS, dwFlags);
         pIA->Release();
      }
   }

   // tell the audio object about our stuff
   lpDirectSound->AddRef();
   pIAD->Init ((PVOID) lpDirectSound,IID_IDirectSound);

   pIAD->AddRef();
   *ppIAD = pIAD;

// Should do select now

   hRes = pITTSFind->Select(ttsResult.gModeID, &pITTSCentral, (LPUNKNOWN) pIAD);

   if (hRes) {
      pITTSFind->Release();
      return NULL;
      };


   if (gfUse3D) {
      LPDIRECTSOUND3DBUFFER   pIL = NULL;
      pIAD->QueryInterface (IID_IDirectSound3DBuffer, (PVOID*) &pIL);
#define  RANDPOS        ((rand() % 5) - 2.5f)
      if (pIL) {
		  DS3DBUFFER b;
		  b.dwSize = sizeof(b);
		  hRes = pIL->GetAllParameters (&b);
		  b.vPosition.x = ((float) dwNum - 1.0f) * 5;
		  b.vPosition.y = RANDPOS;
		  b.vPosition.z = RANDPOS;
		  hRes = pIL->SetAllParameters (&b, DS3D_IMMEDIATE);

         // hRes = pIL->SetPosition (RANDPOS, RANDPOS, RANDPOS, DS3D_IMMEDIATE);

         pIL->Release();
      }
      else {
         static BOOL sShown = FALSE;
         if (!sShown) {
            MessageBox (NULL, "3D sound not available", NULL, MB_OK);
            sShown = TRUE;
         }
      }
   }

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
      {
      ghWndMain = hWnd;

      HRESULT hRes;
      hRes = lpDirectSound->SetCooperativeLevel (hWnd, DSSCL_PRIORITY);
      if (hRes)
		   MessageBox( NULL, "SetCooperationLevel failed.", NULL, MB_OK );
	   // set the primary buffer to 44 kHz, 16bit stereo
	   if (gfUse3D) {
			DSBUFFERDESC    dsbd;


			// hRes = lpDirectSound->SetCooperativeLevel( NULL, DSSCL_PRIORITY);

			ZeroMemory( &dsbd, sizeof(DSBUFFERDESC));
			dsbd.dwSize = sizeof(DSBUFFERDESC);
			dsbd.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;

			WAVEFORMATEX	wfex;
			memset (&wfex, 0, sizeof(wfex));
			wfex.wFormatTag = WAVE_FORMAT_PCM;
			wfex.nSamplesPerSec = 22050;
			wfex.nChannels = 2;
			wfex.wBitsPerSample = 16;
			wfex.nBlockAlign = 2 * wfex.nChannels;
			wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

			hRes = lpDirectSound->CreateSoundBuffer( &dsbd, &gpdsbPrimary, NULL );
			if (gpdsbPrimary)
				hRes = gpdsbPrimary->SetFormat (&wfex);
	   }


      }
      break;
   case WM_COMMAND:
         switch (LOWORD(wParam))
            {
            case IDC_SPEAK1:
            case IDC_SPEAK2:
            case IDC_SPEAK3:
               {
               char  szSpeak[1024];
               SDATA data;
               int   i;

               switch (LOWORD(wParam)) {
                  case IDC_SPEAK1:
                     i = 0;
                     break;
                  case IDC_SPEAK2:
                     i = 1;
                     break;
                  case IDC_SPEAK3:
                     i = 2;
                     break;
               }

               // Speak
               GetDlgItemText (hWnd, IDC_EDIT, szSpeak, sizeof(szSpeak));
               data.dwSize = strlen(szSpeak) + 1;
               data.pData = szSpeak;
               gpITTSCentral[i]->TextData (CHARSET_TEXT, 0,
                  data, NULL,
                  IID_ITTSBufNotifySink);
               }
               return TRUE;
            case IDCANCEL:
               {
               int i;
               for (i = 0; i < NUMVOICES; i++)
                  gpITTSCentral[i]->AudioReset ();
               EndDialog (hWnd, IDCANCEL);
               }
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
      while (!pITTSEnum->Next(1, &TTSInfo, NULL)) {
         SendDlgItemMessage (hWnd, IDC_VOICE1, CB_ADDSTRING,
            0, (LPARAM) TTSInfo.szModeName);
         SendDlgItemMessage (hWnd, IDC_VOICE2, CB_ADDSTRING,
            0, (LPARAM) TTSInfo.szModeName);
         SendDlgItemMessage (hWnd, IDC_VOICE3, CB_ADDSTRING,
            0, (LPARAM) TTSInfo.szModeName);
      }
      SendDlgItemMessage (hWnd, IDC_VOICE1, CB_SETCURSEL,
         0, (LPARAM) 0);
      SendDlgItemMessage (hWnd, IDC_VOICE2, CB_SETCURSEL,
         0, (LPARAM) 0);
      SendDlgItemMessage (hWnd, IDC_VOICE3, CB_SETCURSEL,
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
            GetDlgItemText (hWnd, IDC_VOICE1, gszVoice[0], sizeof(gszVoice[0]));
            GetDlgItemText (hWnd, IDC_VOICE2, gszVoice[1], sizeof(gszVoice[1]));
            GetDlgItemText (hWnd, IDC_VOICE3, gszVoice[2], sizeof(gszVoice[2]));

            gfUse3D = IsDlgButtonChecked (hWnd, IDC_USE3D);

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

   // crreate direct sound stuff
   HRESULT hRes;
   hRes = CoCreateInstance (CLSID_DirectSound, NULL,
      CLSCTX_ALL, IID_IDirectSound, (LPVOID*) &lpDirectSound);
   if (hRes) {
		MessageBox(NULL, "Can't find CLSID_DirectSound. You need to install DirectSound!", NULL, MB_OK );
	   return -1; 	
   }
   hRes = lpDirectSound->Initialize(NULL);
   if (hRes) {
		MessageBox( NULL, "Call to initialize failed.", NULL, MB_OK );
	   return -1; 	
   }

// find the right object
   int   i;
   for (i = 0; i < NUMVOICES; i++) {
      memset (&ModeInfo, 0, sizeof(ModeInfo));
      strcpy (ModeInfo.szModeName, gszVoice[i]);
      gpITTSCentral[i] = FindAndSelect (&ModeInfo, &gpIAD[i], i);
      if (!gpITTSCentral[i]) {
         MessageBox (NULL, "Can't create a TTS engine.", NULL, MB_OK);
         return 1;
         };
   }


// Bring up the dialog box
   DialogBox (hInstance, MAKEINTRESOURCE(IDD_TTS),
      NULL, (DLGPROC) DialogProc);

// try to close ole
   for (i =0; i < NUMVOICES; i++)
      gpITTSCentral[i]->Release();

// release the audio dest
   for (i = 0; i < NUMVOICES; i++)
      while (gpIAD[i]->Release());

   if (gpdsbPrimary)
	   gpdsbPrimary->Release();

   lpDirectSound->Release();

   if (!EndOLE())
      MessageBox (NULL, "Can't shut down OLE.", NULL, MB_OK);

   return 0;
}


