/*******************************************************
main.cpp - main code.



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
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>

#include <speech.h>
#include "resource.h"


// Turn this on to use direct sound capture. NOTE: This will only work
// with Direct Sound 5.0 or better!
// #define DIRECTSOUND

#ifdef DIRECTSOUND
#include <dsound.h>
#endif



/* typedefs */
/* globals */
HINSTANCE   ghInstance = NULL;
static   PISRENUM    gpISREnum = NULL;
static   PISRFIND    gpISRFind = NULL;
static   PISRCENTRAL gpISRCentral = NULL;
static   PISRATTRIBUTES  gpISRAttributes = NULL;  // attributes
static   PISRGRAMCOMMON gpISRGramCommon = NULL;
static   HWND        ghWndTest = NULL;

static   char        gszGrammarFile[256];
static   char        gszSREngine[128];


class CTestGramNotify : public ISRGramNotifySink {
   private:

   public:
      CTestGramNotify (void);
      ~CTestGramNotify (void);

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ISRNotifySink
	   STDMETHODIMP BookMark       (DWORD);
	   STDMETHODIMP Paused         (void);
	   STDMETHODIMP PhraseFinish   (DWORD, QWORD, QWORD, PSRPHRASE, LPUNKNOWN);
	   STDMETHODIMP PhraseHypothesis(DWORD, QWORD, QWORD, PSRPHRASE, LPUNKNOWN);
	   STDMETHODIMP PhraseStart    (QWORD);
	   STDMETHODIMP ReEvaluate     (LPUNKNOWN);
	   STDMETHODIMP Training       (DWORD);
	   STDMETHODIMP UnArchive      (LPUNKNOWN);
   };
typedef CTestGramNotify * PCTestGramNotify;

CTestGramNotify      gTestGramNotify;

/*************************************************************************
CTestGramNotify - Notification object. All this this will do is display
   its notification on the main window.
*/
CTestGramNotify::CTestGramNotify (void)
{
// this space intentionally left blank
}

CTestGramNotify::~CTestGramNotify (void)
{
// this space intentionally left blank
}

STDMETHODIMP CTestGramNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
*ppv = NULL;

/* always return our IUnkown for IID_IUnknown */
if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRGramNotifySink)) {
   *ppv = (LPVOID) this;
   return NOERROR;
}

// otherwise, cant find

return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CTestGramNotify::AddRef (void)
{
// normally this increases a reference count, but this object
// is going to be freed as soon as the app is freed, so it doesn't
// matter
return 1;
}

STDMETHODIMP_(ULONG) CTestGramNotify::Release (void)
{
// normally this releases a reference count, but this object
// is going to be freed when the application is freed so it doesnt
// matter
return 1;
};


STDMETHODIMP CTestGramNotify::BookMark(DWORD dwID)
{
return NOERROR;
}

STDMETHODIMP CTestGramNotify::Paused(void)
{
return NOERROR;
}

STDMETHODIMP CTestGramNotify::PhraseFinish(DWORD dwFlags, QWORD qTimeStampBegin,
   QWORD qTimeStampEnd, PSRPHRASE pSRPhrase, LPUNKNOWN lpResults)
{
   if (!pSRPhrase)
      return NOERROR;

   // write out a string with the results
   char  szTemp[512];
   szTemp[0] = 0;
   PSRWORD  pSRWord, pSRMax;
   // loop through all of the words and display them
   pSRMax = (PSRWORD) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
   pSRWord = (PSRWORD) (pSRPhrase->abWords);
   while (pSRWord < pSRMax)
   {
      // add word
      strcat (szTemp, pSRWord->szWord);
      strcat (szTemp, " ");

      pSRWord = (PSRWORD) ((BYTE*) pSRWord + pSRWord->dwSize);
   };

   // append this to the text in the edit box
   HWND  hWndEdit;
   if (ghWndTest) {
      hWndEdit = GetDlgItem(ghWndTest, IDC_TEXT);
      int   iLen;
      iLen = GetWindowTextLength(hWndEdit);
      SendMessage (hWndEdit, EM_SETSEL, iLen, iLen);
      SendMessage (hWndEdit, EM_REPLACESEL, FALSE, (LPARAM) szTemp);
   };
	return NOERROR;
}


STDMETHODIMP CTestGramNotify::PhraseHypothesis(DWORD dwFlags, QWORD qTimeStampBegin,
   QWORD qTimeStampEnd, PSRPHRASE pSRPhrase, LPUNKNOWN lpResults)
{
// Ignore this
return NOERROR;
}

STDMETHODIMP CTestGramNotify::PhraseStart(QWORD qTimeStampBegin)
{

return NOERROR;
}

STDMETHODIMP CTestGramNotify::ReEvaluate(LPUNKNOWN lpUnk)
{
return NOERROR;
}

STDMETHODIMP CTestGramNotify::Training(DWORD dwTrain)
{
return NOERROR;
}

STDMETHODIMP CTestGramNotify::UnArchive(LPUNKNOWN lpUnk)
{
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
      // remember this window
      ghWndTest = hWnd;

      // activate the grammar for this window
      gpISRGramCommon->Activate(NULL, FALSE, NULL);

      break;   // continue on

   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
         case IDCANCEL:
            // force a stop
            ghWndTest = NULL;
            gpISRGramCommon->Deactivate(NULL);
            EndDialog (hWnd, IDCANCEL);
            return TRUE;
         };
      break;
   };

return FALSE;  // didn't handle
}


/*******************************************************
_InfoDlgProc - Dialog procedure for getting the training
   information.

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK _InfoDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

switch (uMsg) {
   case WM_INITDIALOG:
      // Fill in list-box with SR engine modes
      {
      SRMODEINFO     SRInfo;
      gpISREnum->Reset();
      while (!gpISREnum->Next(1, &SRInfo, NULL))
         if (SRInfo.dwGrammars & SRGRAM_DICTATION)
            SendDlgItemMessage (hWnd, IDC_SRENGINE, CB_ADDSTRING,
               0, (LPARAM) SRInfo.szModeName);
      }
      SendDlgItemMessage (hWnd, IDC_SRENGINE, CB_SETCURSEL,
         0, (LPARAM) 0);

      break;   // continue on

   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
            #define  GETAD(dwID,szDest)   GetDlgItemText(hWnd,dwID,szDest,sizeof(szDest))
            #define  GETINT(dwID,szDest)   szDest = GetDlgItemInt(hWnd,dwID,NULL,FALSE)
            GETAD(IDC_GRAMMARFILE, gszGrammarFile);
            GETAD(IDC_SRENGINE, gszSREngine);

            EndDialog (hWnd, IDOK);
            return TRUE;
         case IDCANCEL:
            EndDialog (hWnd, IDCANCEL);
            return TRUE;
         };
      break;
   };

return FALSE;  // didn't handle
}




/*******************************************************
SelectSREngine - Given an engine's mode-name string, this
   selects the engine to be used for the recognizer.

inputs
   char           szMode - Mode name
   PISRFINDW      pISRFind - Find interface
   PIUNKNOWN      pIUnknown - IUnknown for the audio source (this will
      be addrefed and passed down to the engine)
   PISRCENTRALW   *pISRCentral - Filled in with the ISRCentral interface
   PISRATTRIBUTES *gpISRAttributes - Filled in with the Attributes interface
returns
   HRESULT - error
*/
HRESULT SelectSREngine (char *szMode, PISRFIND pISRFind,
   LPUNKNOWN pIUnknown, PISRCENTRAL *pISRCentral, PISRATTRIBUTES *ppISRAttributes)
{
SRMODEINFO SRInfo, SRDest;
HRESULT     hRes;

memset (&SRInfo, 0 , sizeof(SRInfo));
strcpy (SRInfo.szModeName, szMode);

hRes = pISRFind->Find(&SRInfo, NULL, &SRDest);
if (hRes) return hRes;

hRes = pISRFind->Select(SRDest.gModeID, pISRCentral, pIUnknown);
if (hRes) return hRes;

hRes = (*pISRCentral)->QueryInterface (IID_ISRAttributes, (void**) ppISRAttributes);
return hRes;
}


/*************************************************************************
OpenGrammarFile - This opens a grammar file.

inputs
   char      szFile - file to open
   PSDATA    pData - This is filled in with a pointer to memory
               and the size of the data.
returns
   BOOL - TTRUE if successful
*/
BOOL OpenGrammarFile (char *szFile, PSDATA pData)
{
HFILE hf;              /* file handle */

/* Open the file. */
hf = _lopen (szFile, OF_READ);
if (HFILE_ERROR == hf)
   return FALSE;
pData->dwSize = _llseek (hf, 0, 2);
pData->pData = malloc (pData->dwSize);
if (!pData->pData) {
   _lclose (hf);
   return FALSE;
   };
_llseek (hf, 0, 0);
_lread (hf, pData->pData, pData->dwSize);
_lclose (hf);

return TRUE;
}


/*******************************************************
GetTestingInfo - This gets the information about what
   we will test from a dialog and fills in the globals. It
   creates instances of the engines.

inputs
   none
returns
   BOOL - TRUE if got it, FALSE if user pressed cancel
*/
HRESULT GetTrainingInfo (void)
{
HRESULT hRes;
IAudioMultiMediaDevice *pIAMM = NULL;

// get the enumerators
hRes = CoCreateInstance (CLSID_SREnumerator, NULL, CLSCTX_ALL, IID_ISREnum, (void**)&gpISREnum);
if (hRes) return hRes;
hRes = gpISREnum->QueryInterface(IID_ISRFind, (void**)&gpISRFind);
if (hRes) return hRes;

#ifdef DIRECTSOUND
   LPDIRECTSOUNDCAPTURE     lpDirectSoundCapture = NULL;
   PIAUDIODIRECT pIAD;

   // crreate direct sound stuff
   hRes = CoCreateInstance (CLSID_DirectSoundCapture, NULL,
      CLSCTX_ALL, IID_IDirectSoundCapture, (LPVOID*) &lpDirectSoundCapture);
   if (hRes) {
		MessageBox(NULL, "Can't find CLSID_DirectSound. You need to install DirectSound 5.0!", NULL, MB_OK );
	   return FALSE; 	
   }
   hRes = lpDirectSoundCapture->Initialize(GUID_NULL);
   if (hRes) {
		MessageBox(NULL, "Call to initialize failed.", NULL, MB_OK );
	   return FALSE; 	
   }

// Get the audio dest
	hRes = CoCreateInstance (CLSID_AudioSourceDirect, NULL, CLSCTX_ALL, IID_IAudioDirect, (void**)&pIAD);
   if (hRes)
      {
		MessageBox(NULL, "Call to Create Audio Source Direct.", NULL, MB_OK );
      return FALSE;     // error
      }

   // tell the audio object about our stuff
//   lpDirectSoundCapture->AddRef();
   pIAD->Init ((PVOID) lpDirectSoundCapture,IID_IDirectSoundCapture);

   // IMPORTANT: This is evil, but since we're only using pIAMM and a PIUNKNONW
   // it doesnt make any difference
   pIAMM = (PIAUDIOMULTIMEDIADEVICE) pIAD;

#else // DIRECTSOUND


   hRes = CoCreateInstance(CLSID_MMAudioSource, NULL, CLSCTX_ALL,
          IID_IAudioMultiMediaDevice,(void**)&pIAMM);
   if (hRes) return hRes;
   pIAMM->DeviceNumSet (WAVE_MAPPER);
#endif // DIRECTSOUND

// Create an instrumented audio source and pass the multumedia object into it
IAudioSourceInstrumented *pIAudioInst = NULL;
hRes = CoCreateInstance(CLSID_InstAudioSource, NULL, CLSCTX_ALL,
       IID_IAudioSourceInstrumented,(void**)&pIAudioInst);
if (hRes) return hRes;
pIAudioInst->AudioSource (pIAMM);
pIAudioInst->RegistrySet (L"Software\\Microsoft\\DctDemo");


// ask the user
if (IDOK != DialogBox (ghInstance, MAKEINTRESOURCE(IDD_GRAMMARFILE),
   NULL, (DLGPROC) _InfoDlgProc)) {
      // shut down
      return ResultFromScode (E_FAIL);
      };

// get the engine
hRes = SelectSREngine (gszSREngine, gpISRFind, pIAudioInst, &gpISRCentral, &gpISRAttributes);
if (hRes) return hRes;

// load the grammar
// create grammar object & grammar notification sink
SDATA pData;
pData.pData = NULL;
if (gszGrammarFile[0] && !OpenGrammarFile (gszGrammarFile, &pData))
   MessageBox (NULL, "Can't open the grammar file. Using the default one.",
      NULL, MB_OK);
// If not grammar file then create a default one
SRHEADER srHeader;
if (!pData.pData) {
   srHeader.dwType  = SRHDRTYPE_DICTATION;
   srHeader.dwFlags = SRHDRFLAG_UNICODE;
   pData.pData = &srHeader;
   pData.dwSize = sizeof(srHeader);
}


SRHEADER *pSRHeader;
pSRHeader = (SRHEADER*) pData.pData;
SRGRMFMT fmt;
fmt = SRGRMFMT_CFG;
if (pSRHeader->dwType == SRHDRTYPE_DICTATION)
   fmt = SRGRMFMT_DICTATION;
if (pSRHeader->dwType == SRHDRTYPE_LIMITEDDOMAIN)
   fmt = SRGRMFMT_LIMITEDDOMAIN;

// load it
LPUNKNOWN   lpUnk;
hRes = gpISRCentral->GrammarLoad (fmt, pData, (PVOID) &gTestGramNotify, IID_ISRGramNotifySink, &lpUnk);
if (pData.pData != &srHeader)
   free(pData.pData);
if (hRes) return hRes;

hRes = lpUnk->QueryInterface (IID_ISRGramCommon, (void**)&gpISRGramCommon);
lpUnk->Release();
if (hRes) return hRes;

return NOERROR;
}





/*******************************************************
GetItAllSetup - This goes through the setup code, by:
   0) Init OLE
   1) Find out what the user wants.
   2) Create an audio source
   3) Get the engine
   4) Call UserInit

inputs
   none
returns
   HRESULT - error
*/
HRESULT GetItAllSetup (void)
{
HRESULT           hRes;

// init OLE
hRes = CoInitialize(NULL);
if (hRes) return hRes;

hRes = GetTrainingInfo();
if (hRes)
   return hRes;

return NOERROR;
}




/********************************************************
ShutItAllDown - this shuts everything down, by:
   1) Call UserEnd
   2) Release engine
   3) Release audio source references
   4) End OLE

inputs
   none
returns
   HRESULT
*/
HRESULT ShutItAllDown (void)
{

if (gpISREnum)
   gpISREnum->Release();
if (gpISRFind)
   gpISRFind->Release();
if (gpISRCentral)
   gpISRCentral->Release();
if (gpISRAttributes)
   gpISRAttributes->Release();
if (gpISRGramCommon) {
   gpISRGramCommon->Deactivate(NULL);
   gpISRGramCommon->Release();
}

// Shut down OLE
CoUninitialize ();

return NOERROR;
}




/*******************************************************
WinMain - main entry point for windows
*/

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpszCmdLine, int nCmdShow)
{
HRESULT  hRes;

// save variables
ghInstance = hInstance;

// try initializing
hRes = GetItAllSetup();
if (hRes) {
   MessageBox (NULL, "Couldn't initialize DctDemo. You might not have a dictation engine installed.", NULL, MB_OK);
	// close it all down
	ShutItAllDown ();
	return 1;
   };

// pull up test dialog
DialogBox (ghInstance, MAKEINTRESOURCE(IDD_DEMO),
   NULL, (DLGPROC) _DemoDlgProc);

// close it all down
ShutItAllDown ();

return 0;
}


