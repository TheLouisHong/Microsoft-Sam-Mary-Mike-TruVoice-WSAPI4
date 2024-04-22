/*******************************************************
main.cpp - main code.

Copyright c. 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <mmsystem.h>
#include <objbase.h>
#include <initguid.h>
#include <objerror.h>
#include <ole2ver.h>
#include <dsound.h>
#include <speech.h>
#include <spchwrap.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>

#include <speech.h>
#include <spchwrap.h>
#include "resource.h"

BOOL CalcAccuracy (char *pszRecog, char *pszCorrect,
                   float *fWordError, float *fLineError);

/* add rt */
VOID ParseArg(LPSTR lpszCmdLine);
static BOOL FSzCmdLineToArgv(char **argv, int *pargc, int cargAlloc,
	PSTR szCmdLine, PSTR szProg);
VOID InfoFromArgv(VOID);
HRESULT loadspkr();
/* end add rt */


/* typedefs */
typedef struct {
   char     szInputText[256];
   char     szGrammarFile[256];
   char     szSREngine[128];
   char     szSpeaker[256];
   char     szOutput[256];
   char     szSilenceWave[256];
   char     szMatchText[256];
   BOOL     fCorrection;
   BOOL     fNewLine;
   BOOL     fShowGeneral;
   WORD     wRealTime;
} TESTINFO, *PTESTINFO;


/*******************************************************************
These includes are for the wave manipulation object in the SDK. The
wave manipulation object is used for the wave editor, and in the
accuracy test, it's used for downsampling
*/
// {8EE328E7-FDF4-11d0-8FAE-08002BE4E62A}
DEFINE_GUID(CLSID_WaveData, 
0x8ee328e7, 0xfdf4, 0x11d0, 0x8f, 0xae, 0x8, 0x0, 0x2b, 0xe4, 0xe6, 0x2a);

#undef   INTERFACE
#define  INTERFACE   IWaveData

// {8EE328E7-FDF4-11d0-8FAE-08002BE4E62A}
DEFINE_GUID(IID_IWaveData, 
0x8ee328e7, 0xfdf4, 0x11d0, 0x8f, 0xae, 0x8, 0x0, 0x2b, 0xe4, 0xe6, 0x2a);

DECLARE_INTERFACE_ (IWaveData, IUnknown) {

   // IUnknown members
   STDMETHOD(QueryInterface)  (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
   STDMETHOD_(ULONG,AddRef)   (THIS) PURE;
   STDMETHOD_(ULONG,Release)  (THIS) PURE;

   // IWaveData members
   STDMETHOD (ChunkEnum) (THIS_ DWORD *, DWORD) PURE;
   STDMETHOD (ChunkDelete) (THIS_ DWORD) PURE;
   STDMETHOD (ChunkGet) (THIS_ DWORD, LPUNKNOWN*) PURE;
   STDMETHOD (ChunkAdd) (THIS_ DWORD, LPUNKNOWN) PURE;
   STDMETHOD (Clone) (THIS_ LPUNKNOWN*) PURE;
   STDMETHOD (MaxTimeGet) (THIS_ double*) PURE;
   STDMETHOD (LastModifiedGet) (THIS_ DWORD *) PURE;
   STDMETHOD (FromFile) (THIS_ PCWSTR) PURE;
   STDMETHOD (FromMemory) (THIS_ PVOID, DWORD) PURE;
   STDMETHOD (ToFile) (THIS_ PCWSTR) PURE;
   STDMETHOD (ToMemory) (THIS_ PVOID*, DWORD*) PURE;
   STDMETHOD (Copy) (THIS_ double, double, LPUNKNOWN*) PURE;
   STDMETHOD (Paste) (THIS_ double, double, LPUNKNOWN) PURE;
   STDMETHOD (ClearAll) (THIS) PURE;
   STDMETHOD (GetWFEX) (THIS_ WAVEFORMATEX **ppWFEX) PURE;
   };

typedef IWaveData * PIWAVEDATA;
typedef struct {
   double   fViewStart;
   double   fViewEnd;
   double   fSelStart;
   double   fSelEnd;
   double   fCaret;
} WAVEVIEWPOSN, *PWAVEVIEWPOSN;


#undef   INTERFACE
#define  INTERFACE   IWaveNotifySink

// {8EE328EE-FDF4-11d0-8FAE-08002BE4E62A}
DEFINE_GUID(IID_IWaveNotifySink, 
0x8ee328ee, 0xfdf4, 0x11d0, 0x8f, 0xae, 0x8, 0x0, 0x2b, 0xe4, 0xe6, 0x2a);

DECLARE_INTERFACE_ (IWaveNotifySink, IUnknown) {

   // IUnknown members
   STDMETHOD(QueryInterface)  (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
   STDMETHOD_(ULONG,AddRef)   (THIS) PURE;
   STDMETHOD_(ULONG,Release)  (THIS) PURE;

   // IWaveNotifySink members
   STDMETHOD (RequestViewChange) (THIS_ PWAVEVIEWPOSN) PURE;
   STDMETHOD (DataChanged) (THIS) PURE;
   STDMETHOD (RequestPlay) (THIS) PURE;
   STDMETHOD (PercentComplete) (THIS_ WORD) PURE;
   STDMETHOD (RequestShowClass) (THIS_ CLSID) PURE;
   };

typedef IWaveNotifySink * PIWAVENOTIFYSINK;



#undef   INTERFACE
#define  INTERFACE   IWaveFX

// {8EE328F3-FDF4-11d0-8FAE-08002BE4E62A}
DEFINE_GUID(IID_IWaveFX, 
0x8ee328f3, 0xfdf4, 0x11d0, 0x8f, 0xae, 0x8, 0x0, 0x2b, 0xe4, 0xe6, 0x2a);

DECLARE_INTERFACE_ (IWaveFX, IUnknown) {

   // IUnknown members
   STDMETHOD(QueryInterface)  (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
   STDMETHOD_(ULONG,AddRef)   (THIS) PURE;
   STDMETHOD_(ULONG,Release)  (THIS) PURE;

   // IWaveChunk members
   STDMETHOD (Dialog) (THIS_ HWND, PIWAVEDATA, double, double, DWORD, PVOID*, DWORD*) PURE;
   STDMETHOD (FX) (THIS_ HWND, PIWAVEDATA, double, double, DWORD, PVOID, DWORD, PIWAVENOTIFYSINK) PURE;
   };

typedef IWaveFX * PIWAVEFX;


// {8EE328F5-FDF4-11d0-8FAE-08002BE4E62A}
// The input data is a WAVEFORMATEX structure
DEFINE_GUID(CLSID_WaveFXFormat, 
0x8ee328f5, 0xfdf4, 0x11d0, 0x8f, 0xae, 0x8, 0x0, 0x2b, 0xe4, 0xe6, 0x2a);




/********* End includes for wave manipulation data *********/



/* globals */
TESTINFO ti;
HINSTANCE   ghInstance = NULL;
BOOL        gfTimerSet = FALSE;
static   PIAUDIOFILE gpIAudioFile = NULL;
static   PISRENUM    gpISREnum = NULL;
static   PISRFIND    gpISRFind = NULL;
static   PISRCENTRAL gpISRCentral = NULL;
static   PISRATTRIBUTES  gpISRAttributes = NULL;  // attributes
static   PISRGRAMCOMMON gpISRGramCommon = NULL;
static   PISRDIALOGS gpISRDialogs = NULL;
static   HWND        ghWndTest = NULL;

static   FILE*       gFileOut = NULL;

static   DWORD       gdwStartTime = 0;
static   DWORD       gdwFinishTime = 0;

/* add rt */
#define	cargMost     100
static	int	     carg = 0;
static	LPSTR	     rgszArg[cargMost];
char    resfile[256]="";
FILE  *fresFile;
DWORD WInsertions,WDeletions,WSubstitutions,WRight;
PISRSPEAKER gpISRSpeaker = NULL;
static LPSTR pszGuidVer = "1DBF7AA2-4FC8-11d0-BBEC-00AA006C2A6D";
// switch to load the speaker (TRUE, the speaker is already loaded)
static bool SpeakerLoaded = TRUE;
/* end add rt */


// results object queue
#define  QSIZE       5
typedef struct {
  BOOL            fIsValid;
  PISRRESBASIC    pISRResBasic;
  BYTE            abPhrase[10000];
  DWORD           dwPhraseSize;
} QUEUE, *PQUEUE;
static   QUEUE gaQ[QSIZE];
      // lower indexes are older

#define  PRETIME     1000     // ms
#define  POSTTIME    20000    // ms


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

class CTestFileNotify : public IAudioFileNotifySink {
   private:

   public:
      CTestFileNotify (void);
      ~CTestFileNotify (void);

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ISRNotifySink
      STDMETHODIMP FileBegin      (DWORD);
      STDMETHODIMP FileEnd        (DWORD);
      STDMETHODIMP QueueEmpty     (void);
      STDMETHODIMP Posn           (QWORD, QWORD);
   };
typedef CTestFileNotify * PCTestFileNotify;

CTestFileNotify      gTestFileNotify;


/*********************************************************************
MakeSureFileHasRightFormat - This loads in a .wav file and makes sure
   that it has the right data format (sampling rate - basically). If
   it doens't it asks the user if they wish to change the file. If the
   user presses yes then the file is converted to the right format.

   This function uses a COM object provided in the SDK by the wave
   editor.

inputs
   HWND     hWnd - To create message box on top of
   WCHAR    *pszWave - Wave file
   WAVEFORMATEX *pWFEX - What the wave format needs to be
   DOWRD    dwWFEXSize - size of the wfes
returns
   HRESULT - error
*/
HRESULT MakeSureFileHasRightFormat (HWND hWnd, WCHAR *pszWave,
                                    WAVEFORMATEX *pWFEX, DWORD dwWFEXSize)
{
   PIWAVEFX   pIWaveFX = NULL;
   PIWAVEDATA pIWaveData = NULL;
   HRESULT hRes = NOERROR;

   pIWaveData = NULL;


   // Create TTS transform
   hRes = CoCreateInstance (CLSID_WaveFXFormat, NULL, CLSCTX_ALL, IID_IWaveFX, (void**)&pIWaveFX);
   if (hRes) {
      return E_FAIL;
   }

   // Create wave data
   hRes = CoCreateInstance (CLSID_WaveData, NULL, CLSCTX_ALL, IID_IWaveData, (void**)&pIWaveData);
   if (hRes) {
      pIWaveFX->Release();
      return E_FAIL;
   }


   // load it in
   hRes = pIWaveData->FromFile (pszWave);
   if (hRes) {
      // no file here
      goto theend;
   }

   PWAVEFORMATEX  pOld;
   pOld = NULL;
   pIWaveData->GetWFEX(&pOld);
   if (!pOld || !pWFEX || !memcmp(pOld, pWFEX, sizeof(PCMWAVEFORMAT))) {
      // same format
      hRes = NOERROR;
      goto theend;
   }
   
   // else ask the user
   char  szTemp[256], szTemp2[256];
   WideCharToMultiByte (CP_ACP, 0, pszWave, -1, szTemp, sizeof(szTemp), 0,0);
   wsprintf (szTemp2, "The file %s does not have a sampling rate compatible with the speech "
      "recognition engine. Do you want to convert it to the right sampling rate?", szTemp);
   if (IDYES != MessageBox (hWnd, szTemp2,
      "Accuracy Test",
      MB_YESNO)) {
      // doesn't want to convert
      hRes = E_FAIL;
      goto theend;
   }

   // else we need to downsample
   // do the FX
   hRes = pIWaveFX->FX(hWnd, pIWaveData,
                     0, 0, 0,
                     pWFEX, dwWFEXSize, NULL);
   if (!hRes) {
      // write out the file. First convert file name to unicode
      pIWaveData->ToFile(pszWave);

   }



theend:
   // Free up TTS transform
   if (pIWaveFX)
      pIWaveFX->Release();
   pIWaveFX = NULL;
   if (pIWaveData)
      pIWaveData->Release();
   pIWaveData = NULL;


   return hRes;

}





/********************************************************************8
WritePhraseFinish - Given phrasefinish information, this writes it
   out to the file.

inputs
   PSRPHRASE   pSRPhrase - phrase
   BOOL        fHypothesis - If TRUE is a hypothesis
returns
   none
*/
void WritePhraseFinish (PSRPHRASE pSRPhrase, BOOL fHypothesis)
{
   // write out a string with the results
   char  szTemp[10000];
   szTemp[0] = 0;
   if (pSRPhrase) {
      PSRWORD  pSRWord, pSRMax;
      // loop through all of the words and display them
      pSRMax = (PSRWORD) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
      pSRWord = (PSRWORD) (pSRPhrase->abWords);
      while (pSRWord < pSRMax)
      {
         // add word
         if ((pSRWord != (PSRWORD) pSRPhrase->abWords) || !ti.fNewLine)
            strcat (szTemp, " ");
         strcat (szTemp, pSRWord->szWord);

         pSRWord = (PSRWORD) ((BYTE*) pSRWord + pSRWord->dwSize);
      };
   };

   if (!szTemp[0])
      strcpy (szTemp, "[Unrecognized]");

   if (ti.fNewLine)
      strcat (szTemp, "\r\n");

   // Append this to the file
   if (!fHypothesis) {
      fputs (szTemp, gFileOut);
      fflush (gFileOut);
   }
   
   // append this to the text in the edit box
   HWND  hWndEdit;
   if (ghWndTest) {
      // phrase
      if (!fHypothesis) {
         hWndEdit = GetDlgItem(ghWndTest, IDC_RECOGNIZED);

         int   iLen;
         iLen = GetWindowTextLength(hWndEdit);
         SendMessage (hWndEdit, EM_SETSEL, iLen, iLen);
         SendMessage (hWndEdit, EM_REPLACESEL, FALSE, (LPARAM) szTemp);
      }

      // hypothesis
      hWndEdit = GetDlgItem(ghWndTest, IDC_HYPOTHESIS);
      SetWindowText (hWndEdit, fHypothesis ? szTemp : "");
   };
	return;
}

/********************************************************************8
FlushResultsObject - Writes out the oldest results object(s) to
   the file.

inputs
   BOOL  fAll - If TRUE then flush all of them. Else, FALSE so flush
      only the oldest
returns
   none
*/
void FlushResultsObject (BOOL fAll)
{
   DWORD i;

   // loop
   for (i = 0; i < (DWORD) (fAll ? QSIZE : 1); i++) {
      if (gaQ[i].fIsValid) {
         BYTE  bTemp[20480];
         DWORD dwPhraseNeeded = 0;
         HRESULT  hRes;

         if (gaQ[i].pISRResBasic) {
		      hRes = gaQ[i].pISRResBasic->PhraseGet(0, (PSRPHRASE) bTemp, 
					      sizeof(bTemp), &dwPhraseNeeded);
            if (hRes)
               dwPhraseNeeded = 0;
            else
               dwPhraseNeeded = ((PSRPHRASE)bTemp)->dwSize;

            gaQ[i].pISRResBasic->Release();
            gaQ[i].pISRResBasic = 0;
         }
         else {
            dwPhraseNeeded = gaQ[i].dwPhraseSize;
            memcpy (bTemp, gaQ[i].abPhrase, dwPhraseNeeded);
         }

         // write it
         WritePhraseFinish (dwPhraseNeeded ? (PSRPHRASE) bTemp : NULL, FALSE);
      }
   }

   // copy down
   memmove (gaQ, gaQ + 1,
      (sizeof(QUEUE) * (QSIZE - 1)) );

   // wipe the high one
   memset(&gaQ[QSIZE-1], 0, sizeof(QUEUE));
}

/********************************************************************
NewResultsObject - Pass this an IUnknown. It will query interface
   for a ISRResBasic and add it to the qeue. It will flush the oldest
   element to the file.

inputs
   IUnknown *pIUnknown - IUnknown. This is not addrefed
   PSRPHRASE   pSRPhrase - phrase ifknown. Can be NULL
returns
   HRESULT hRes - error
*/
HRESULT NewResultsObject (IUnknown *pIUnknown, PSRPHRASE pSRPhrase)
{
   if (!pIUnknown)
      return ResultFromScode (E_FAIL);

   // flush the old one
   FlushResultsObject(FALSE);

   // query for the new basic
   PQUEUE   pq = &gaQ[QSIZE-1];
   pq->fIsValid = TRUE;
   pq->pISRResBasic = NULL;
   if (pIUnknown)
      pIUnknown->QueryInterface(IID_ISRResBasic, (LPVOID*)&pq->pISRResBasic);
   if (pSRPhrase) {
      memcpy (pq->abPhrase, pSRPhrase, pSRPhrase->dwSize);
      pq->dwPhraseSize = pSRPhrase->dwSize;
   }
   else
      pq->dwPhraseSize = 0;

   return NOERROR;
}

/*************************************************************************
CTestFileNotify - Notification object. All this this will do is display
   its notification on the main window.
*/
CTestFileNotify::CTestFileNotify (void)
{
// this space intentionally left blank
}

CTestFileNotify::~CTestFileNotify (void)
{
// this space intentionally left blank
}

STDMETHODIMP CTestFileNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
*ppv = NULL;

/* always return our IUnkown for IID_IUnknown */
if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IAudioFileNotifySink)) {
   *ppv = (LPVOID) this;
   return NOERROR;
}

// otherwise, cant find

return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CTestFileNotify::AddRef (void)
{
// normally this increases a reference count, but this object
// is going to be freed as soon as the app is freed, so it doesn't
// matter
return 1;
}

STDMETHODIMP_(ULONG) CTestFileNotify::Release (void)
{
// normally this releases a reference count, but this object
// is going to be freed when the application is freed so it doesnt
// matter
return 1;
};

STDMETHODIMP CTestFileNotify::FileBegin(DWORD dwID)
{
   char  szTemp[16];
   wsprintf (szTemp, "%d", (int) dwID);
   SetDlgItemText (ghWndTest, IDC_FILENUM, szTemp);
   return NOERROR;
}

STDMETHODIMP CTestFileNotify::FileEnd(DWORD dwID)
{
   SetDlgItemText (ghWndTest, IDC_FILENUM, "");
   return NOERROR;
}

STDMETHODIMP CTestFileNotify::QueueEmpty (void)
{
   SetDlgItemText (ghWndTest, IDC_FILENUM, "EOF");

   // Start a timer that goes off if there's no response in N seconds
   if (ghWndTest) { 
      gfTimerSet = TRUE;
      SetTimer (ghWndTest, 1, POSTTIME, NULL);
   }
   gdwFinishTime = timeGetTime();

   return NOERROR;
}

STDMETHODIMP CTestFileNotify::Posn (QWORD qwUsed, QWORD qwLeft)
{
   QWORD qwTotal;
   qwTotal = (qwUsed + qwLeft) / 10000;
   if (!qwTotal)
      qwTotal = 1;

   char  szTemp[16];
   double d = (double) ((int) (qwUsed / qwTotal)) / 100.0;
   if (d > 100.0)
      d = 100.0;
   sprintf (szTemp, "%.2f", d);
   SetDlgItemText (ghWndTest, IDC_PERCENTDONE, szTemp);
   return NOERROR;
}


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
   // potentially restart the timer if it's already going
   // because SR might be slow
   if (gfTimerSet) {
      KillTimer (ghWndTest, 1);
      SetTimer (ghWndTest, 1, POSTTIME, NULL);
   }

   if (ti.fCorrection)
      NewResultsObject(lpResults, pSRPhrase);
   else
      WritePhraseFinish (pSRPhrase, FALSE);

   return NOERROR;
}


STDMETHODIMP CTestGramNotify::PhraseHypothesis(DWORD dwFlags, QWORD qTimeStampBegin,
   QWORD qTimeStampEnd, PSRPHRASE pSRPhrase, LPUNKNOWN lpResults)
{
   // potentially restart the timer if it's already going
   // because SR might be slow
   if (gfTimerSet) {
      KillTimer (ghWndTest, 1);
      SetTimer (ghWndTest, 1, POSTTIME, NULL);
   }

   // write this out
   WritePhraseFinish (pSRPhrase, TRUE);
   return NOERROR;
}

STDMETHODIMP CTestGramNotify::PhraseStart(QWORD qTimeStampBegin)
{

   // potentially restart the timer if it's already going
   // because SR might be slow
   if (gfTimerSet) {
      KillTimer (ghWndTest, 1);
      SetTimer (ghWndTest, 1, POSTTIME, NULL);
   }

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
_TestDlgProc - Do the testing.

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK _TestDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

switch (uMsg) {
   case WM_INITDIALOG:
      // remember this window
      ghWndTest = hWnd;

      // Maybe pull up the general dialog
      if (ti.fShowGeneral &&gpISRDialogs)
         gpISRDialogs->GeneralDlg (hWnd, NULL);

      // activate it
      gpISRGramCommon->Activate(NULL, FALSE, NULL);

      // Set the initial timer
      gdwStartTime = timeGetTime() + PRETIME;

      break;   // continue on

   case WM_TIMER:
      // This timer goes off N seconds after the last recognition
      // so we know we can stop
      KillTimer (hWnd, 1);
      gfTimerSet = FALSE;
      gpISRGramCommon->Deactivate(NULL);
      EndDialog (hWnd, IDOK);
      break;

   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
         case IDCANCEL:
            // force a stop
            ghWndTest = NULL;
            gpISRGramCommon->Deactivate(NULL);

            // indicate that the user stopped
            KillTimer (hWnd, 1);
            gfTimerSet = FALSE;
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
         SendDlgItemMessage (hWnd, IDC_SRENGINE, CB_ADDSTRING,
               0, (LPARAM) SRInfo.szModeName);

      SendDlgItemMessage (hWnd, IDC_SRENGINE, CB_SELECTSTRING,
         -1, (LPARAM) ti.szSREngine);
      }

      SetDlgItemText (hWnd, IDC_INPUTTEXT, ti.szInputText);
      SetDlgItemText (hWnd, IDC_SILENCEWAVE, ti.szSilenceWave);
      SetDlgItemText (hWnd, IDC_GRAMMARFILE, ti.szGrammarFile);
      SetDlgItemText (hWnd, IDC_SPEAKER, ti.szSpeaker);
      SetDlgItemText (hWnd, IDC_OUTPUT, ti.szOutput);
      SetDlgItemText (hWnd, IDC_MATCHTEXT, ti.szMatchText);

      // Default to having the get info checked
      if (ti.fCorrection)
         CheckDlgButton (hWnd, IDC_CORRECTION, BST_CHECKED);

      // Default to having the new-line checked
      if (ti.fNewLine)
         CheckDlgButton (hWnd, IDC_NEWLINE, BST_CHECKED);

      // Default is to have 1x real time
      switch (ti.wRealTime) {
      case 0x40:
         CheckDlgButton (hWnd, IDC_QUARTERXREALTIME, BST_CHECKED);
         break;
      case 0x80:
         CheckDlgButton (hWnd, IDC_HALFXREALTIME, BST_CHECKED);
         break;
      case 0x200:
         CheckDlgButton (hWnd, IDC_2XREALTIME, BST_CHECKED);
         break;
      case 0x400:
         CheckDlgButton (hWnd, IDC_4XREALTIME, BST_CHECKED);
         break;
      case 0x800:
         CheckDlgButton (hWnd, IDC_8XREALTIME, BST_CHECKED);
         break;
      default: // 0x100
         CheckDlgButton (hWnd, IDC_1XREALTIME, BST_CHECKED);
      }

      break;   // continue on

   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
            #define  GETAD(dwID,szDest)   GetDlgItemText(hWnd,dwID,szDest,sizeof(szDest))
            #define  GETINT(dwID,szDest)   szDest = GetDlgItemInt(hWnd,dwID,NULL,FALSE)
            GETAD(IDC_INPUTTEXT, ti.szInputText);
            GETAD(IDC_SILENCEWAVE, ti.szSilenceWave);
            GETAD(IDC_GRAMMARFILE, ti.szGrammarFile);
            GETAD(IDC_SRENGINE, ti.szSREngine);
            GETAD(IDC_SPEAKER, ti.szSpeaker);
            GETAD(IDC_OUTPUT, ti.szOutput);
            GETAD(IDC_MATCHTEXT, ti.szMatchText);

            // set the global for correction based upon dialog
            ti.fCorrection = (BST_CHECKED == IsDlgButtonChecked(hWnd, IDC_CORRECTION));
            ti.fShowGeneral = (BST_CHECKED == IsDlgButtonChecked(hWnd, IDC_SHOWGENERAL));
            ti.fNewLine = (BST_CHECKED == IsDlgButtonChecked(hWnd, IDC_NEWLINE));

            // how real time?
            if (BST_CHECKED == IsDlgButtonChecked(hWnd, IDC_2XREALTIME))
               ti.wRealTime = 0x200;
            else if (BST_CHECKED == IsDlgButtonChecked(hWnd, IDC_4XREALTIME))
               ti.wRealTime = 0x400;
            else if (BST_CHECKED == IsDlgButtonChecked(hWnd, IDC_8XREALTIME))
               ti.wRealTime = 0x800;
            else if (BST_CHECKED == IsDlgButtonChecked(hWnd, IDC_HALFXREALTIME))
               ti.wRealTime = 0x80;
            else if (BST_CHECKED == IsDlgButtonChecked(hWnd, IDC_QUARTERXREALTIME))
               ti.wRealTime = 0x40;
            else
               ti.wRealTime = 0x100;

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
   PISRDIALOGS    *gpISRDialogs - Filled in with the dialogs interface
returns
   HRESULT - error
*/
HRESULT SelectSREngine (char *szMode, PISRFIND pISRFind,
   LPUNKNOWN pIUnknown, PISRCENTRAL *pISRCentral, PISRATTRIBUTES *ppISRAttributes,
   PISRDIALOGS *pISRDialogs)
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
hRes = (*pISRCentral)->QueryInterface (IID_ISRDialogs, (void**) pISRDialogs);
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
   // if it end's with .grm then open it binary, else use the
   // grammar compiler object
   int   len;
   len = strlen(szFile);
   if ((len >= 4) && !stricmp(szFile + (len-4), ".grm")) {
      HFILE hf;              /* file handle */

      /* Open the file. */
      hf = _lopen (szFile, OF_READ);
      if (hf == HFILE_ERROR)
         return FALSE;
      pData->dwSize = _llseek (hf, 0, 2);
      pData->pData = CoTaskMemAlloc (pData->dwSize);
      if (!pData->pData) {
         _lclose (hf);
         return FALSE;
         };
      _llseek (hf, 0, 0);
      _lread (hf, pData->pData, pData->dwSize);
      _lclose (hf);
   }
   else {
      // use the grammar compiler object
      CSTGramComp  gc;
      WCHAR szName[256];
      MultiByteToWideChar (CP_ACP, 0, szFile, -1, szName, sizeof(szName)/2);
      if (gc.Init (szName))
         return FALSE;
      if (gc.Compile())
         return FALSE;
      if (gc.GrammarDataGet (FALSE, &pData->pData, &pData->dwSize))
         return FALSE;
   }


   return TRUE;
}

/*******************************************************
QueueUpudio - This reads in either the wave file specified
   in the dialog, or reads in a file containing a list of
   files. These are all queued up. Silence might be queued up
   along with these.
*/
void QueueUpAudio (void)
{
   WCHAR szwFile[256];
   DWORD dwFileNum = 1;

   gpIAudioFile->RealTimeSet (ti.wRealTime);

   // figure out the wave format
   SDATA sWFEX;
   sWFEX.pData = NULL;
   PIAUDIO  pIAudio;
   pIAudio = NULL;
   gpIAudioFile->QueryInterface (IID_IAudio, (LPVOID*) &pIAudio);
   if (pIAudio) {
      pIAudio->WaveFormatGet (&sWFEX);
      pIAudio->Release();
   }

   // first, if there's a silence wave then qeuue it
   if (ti.szSilenceWave[0]) {
      MultiByteToWideChar (CP_ACP, 0, ti.szSilenceWave, -1,
         szwFile, sizeof(szwFile) / sizeof(WCHAR));

      // see if we should convert it
      if (pIAudio && sWFEX.pData)
         MakeSureFileHasRightFormat (NULL, szwFile,
            (WAVEFORMATEX*) sWFEX.pData,
            sWFEX.dwSize);

      gpIAudioFile->Add (szwFile, 0);
   }

   // if this has just .wav then queue right away
   CharLower (ti.szInputText);
   if (strstr(ti.szInputText, ".wav")) {
      MultiByteToWideChar (CP_ACP, 0, ti.szInputText, -1,
         szwFile, sizeof(szwFile) / sizeof(WCHAR));

      // see if we should convert it
      if (pIAudio && sWFEX.pData)
         MakeSureFileHasRightFormat (NULL, szwFile,
            (WAVEFORMATEX*) sWFEX.pData,
            sWFEX.dwSize);

      gpIAudioFile->Add (szwFile, dwFileNum++);
   }
   else {
      FILE  *fIn;
      char  szFile[256];
      fIn = fopen (ti.szInputText, "rt");
      if (!fIn)
         return;   //error
      while (fgets(szFile, sizeof(szFile), fIn)) {
         char *szEnd;
         szEnd = strchr(szFile, '\n');
         if (szEnd) *szEnd = 0;
         if (!szFile[0])
            continue;
         MultiByteToWideChar (CP_ACP, 0, szFile, -1,
            szwFile, sizeof(szwFile) / sizeof(WCHAR));

         // see if we should convert it
         if (pIAudio && sWFEX.pData)
            MakeSureFileHasRightFormat (NULL, szwFile,
               (WAVEFORMATEX*) sWFEX.pData,
               sWFEX.dwSize);

         gpIAudioFile->Add (szwFile, dwFileNum++);

         if (ti.szSilenceWave[0]) {
            MultiByteToWideChar (CP_ACP, 0, ti.szSilenceWave, -1,
               szwFile, sizeof(szwFile) / sizeof(WCHAR));
            gpIAudioFile->Add (szwFile, 0);
         }
      }
      fclose (fIn);
   }

   // finally, if there's a silence wave then qeuue it
   if (ti.szSilenceWave[0]) {
            MultiByteToWideChar (CP_ACP, 0, ti.szSilenceWave, -1,
               szwFile, sizeof(szwFile) / sizeof(WCHAR));
            gpIAudioFile->Add (szwFile, 0);
         }

   if (sWFEX.pData)
      CoTaskMemFree (sWFEX.pData);

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

   // get the enumerators
   hRes = CoCreateInstance (CLSID_SREnumerator, NULL, CLSCTX_ALL, IID_ISREnum, (void**)&gpISREnum);
   if (hRes) return hRes;
   hRes = gpISREnum->QueryInterface(IID_ISRFind, (void**)&gpISRFind);
   if (hRes) return hRes;


   // initialize the preferences
   memset (&ti, 0, sizeof(ti));
   char  szFill[] = "fill this in";
   strcpy (ti.szInputText, szFill);
   strcpy (ti.szGrammarFile, szFill);
   strcpy (ti.szOutput, szFill);
   strcpy (ti.szSilenceWave, szFill);
   strcpy (ti.szMatchText, szFill);
   ti.fCorrection = TRUE;
   ti.fNewLine = TRUE;
   ti.wRealTime = 0x100;

   // get the registry entry for preferences
   DWORD dwType, dwNeeded;
   HKEY  hKey;
   PSTR  pszSRBuf = 0, pszTTSBuf = 0;

   // read speech engine mode names
   hKey = NULL;
   LONG status = RegOpenKeyEx( HKEY_CURRENT_USER,
                             "Software\\Microsoft\\SpeechAPI\\AccTest",
                             NULL, KEY_ALL_ACCESS, &hKey );
   if (!hKey) {
      DWORD dwDisposition;
      status = RegCreateKeyEx( HKEY_CURRENT_USER,
        "Software\\Microsoft\\SpeechAPI\\AccTest",
         NULL, NULL, REG_OPTION_NON_VOLATILE,
         KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition );
   }

   if (hKey) {
	   dwNeeded = sizeof(ti);
	   status = RegQueryValueEx(hKey, "Settings", NULL, &dwType, (PBYTE) &ti, &dwNeeded);
   }

   // ask the user
   /* add rt */
   if (carg != 0)
		{
		InfoFromArgv();
		}
   else if (IDOK != DialogBox (ghInstance, MAKEINTRESOURCE(IDD_GETINFO),
      NULL, (DLGPROC) _InfoDlgProc)) {
   /* end add rt */
         // Close the preferences registry entry
         RegCloseKey (hKey);

         // shut down
         return ResultFromScode (E_FAIL);
         };

   // Write the registry entry for preferences
   RegSetValueEx( hKey, "Settings", NULL, REG_BINARY,
       (PBYTE)&ti, sizeof(ti) );
   RegCloseKey (hKey);

   // create the audio source
   hRes = CoCreateInstance (CLSID_AudioSourceFile, NULL, CLSCTX_ALL,
                            IID_IAudioFile, (void**) &gpIAudioFile);
   if (hRes) return hRes;

   gpIAudioFile->Register (&gTestFileNotify);

   // Set WRITEOUTAUDIO if you want all of the .wav files merged into one
// #define  WRITEOUTAUDIO
#ifdef WRITEOUTAUDIO
   PIAUDIOSOURCEINSTRUMENTED pIASI;
   pIASI = NULL;
   CoCreateInstance (CLSID_InstAudioSource, NULL, CLSCTX_ALL,
      IID_IAudioSourceInstrumented, (void**) &pIASI);
   pIASI->AudioSource (gpIAudioFile);
   pIASI->WaveFileWriteSet (L"c:\\out.wav");
   pIASI->StateSet (IASISTATE_PASSWRITETOWAVE);

#endif

   // get the engine
   hRes = SelectSREngine (ti.szSREngine, gpISRFind,
#ifdef WRITEOUTAUDIO
      pIASI,
#else
      gpIAudioFile,
#endif
      &gpISRCentral, &gpISRAttributes,
      &gpISRDialogs);
#ifdef WRITEOUTAUDIO
   pIASI->Release(); // since sr engine will do an addref
#endif
   if (hRes) return hRes;

   // set the speaker?
   if (ti.szSpeaker[0]) {
      if (!SpeakerLoaded) {
		hRes = gpISRCentral->QueryInterface(IID_ISRSpeaker, (LPVOID *)&gpISRSpeaker);
		if (hRes) return hRes;
		hRes=loadspkr();
		if (hRes) return hRes; 
	  }
      hRes=gpISRAttributes->SpeakerSet(ti.szSpeaker);
      if (hRes) return hRes;
   }

   // load the grammar
   // create grammar object & grammar notification sink
   SDATA pData;
   pData.pData = NULL;
   if (ti.szGrammarFile[0] && !OpenGrammarFile (ti.szGrammarFile, &pData))
      return ResultFromScode(STG_E_FILENOTFOUND);
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
      CoTaskMemFree(pData.pData);
   if (hRes) return hRes;

   hRes = lpUnk->QueryInterface (IID_ISRGramCommon, (void**)&gpISRGramCommon);
   lpUnk->Release();

   if (hRes) return hRes;

   // queue up the audio
   QueueUpAudio();

   // openthe output
   gFileOut = fopen (ti.szOutput, "wt");
   if (!gFileOut) {
      MessageBox (NULL, "Can't open output file.", NULL, MB_OK);
      return ResultFromScode (E_FAIL);
   };

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
if (gpISRDialogs)
   gpISRDialogs->Release();
if (gpISRGramCommon) {
   gpISRGramCommon->Deactivate(NULL);
   gpISRGramCommon->Release();
}
/* add rt */
   // Release speaker interface
if (gpISRSpeaker)
      gpISRSpeaker->Release();
/* end add rt */

// release audio source information
if (gpIAudioFile) {
   gpIAudioFile->Register (NULL);
   gpIAudioFile->Release();
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

   // blank out results memory
   memset (gaQ, 0, sizeof(gaQ));

   // save variables
   ghInstance = hInstance;

   /* add rt */

   // parse the command line
   ParseArg(lpszCmdLine);
   
   /* end add rt */

   // try initializing
   hRes = GetItAllSetup();
   if (hRes) {
      MessageBox (NULL, "Couldn't initialize something.", NULL, MB_OK);
	   // close it all down
	   ShutItAllDown ();
	   return 1;
      };

   // pull up test dialog
   int   iRet;
   iRet = DialogBox (ghInstance, MAKEINTRESOURCE(IDD_STATUS),
      NULL, (DLGPROC) _TestDlgProc);

   // flush the files
   // flush the results object
   FlushResultsObject(TRUE);

   if (gFileOut)
      fclose (gFileOut);


   if (IDOK == iRet) {
         // Show statistics
         char  szTemp[128];
         float fWordError, fLineError;
         CalcAccuracy (ti.szOutput, ti.szMatchText,
            &fWordError, &fLineError);
         sprintf (szTemp, "Word error rate = %f\r\nLine error rate = %f",
            (double) fWordError, (double) fLineError);
         /* add rt */
		   if (*resfile) {
		          fresFile = fopen (resfile, "a");
            if (!fresFile) 
               MessageBox (NULL, "Can't open results file.", NULL, MB_OK);
            else {
			   fprintf(fresFile,"%s %s %d %d %d %d\n",ti.szSpeaker,ti.szInputText,(int)WRight,
				   (int)WInsertions,(int)WDeletions,(int)WSubstitutions);
            }
		    }
         else
   		    MessageBox (NULL, szTemp, "Finished", MB_OK);

         };

   // close it all down
   ShutItAllDown ();

return 0;
}


/* add rt */

VOID ParseArg(LPSTR lpszCmdLine)
{
   if (!FSzCmdLineToArgv(rgszArg, &carg, cargMost, lpszCmdLine, "AccTest"))
		{
		carg = 0;
		}
}

/*************************************************************************
FSzCmdLineToArgv - This goes the actual work of FReadArgV, converting
   a file (actually memory) in a series of NULL-terminated strings used
   for the command line. Enough said.
*/
static BOOL FSzCmdLineToArgv(char **argv, int *pargc, int cargAlloc,
	PSTR szCmdLine, PSTR szProg)
        {
        int cb = strlen(szCmdLine);
        int fCollecting = FALSE;
        int fInQuote = FALSE;
        int ib, iarg = 0;

        for (ib = 0, fCollecting = FALSE, fInQuote = FALSE; ib < cb;
                        ib++, szCmdLine++)
                {
                int fIsWhite = isspace(*szCmdLine);
                int fIsQuote = (*szCmdLine == '"');

                if (fCollecting && ((fIsWhite && !fInQuote) || (fInQuote && fIsQuote)))
                        {
                        *szCmdLine = '\0';
                        iarg++;
                        fCollecting = fInQuote = FALSE;
                        if (iarg == cargAlloc)
                                {
                                break;
                                }
                        }
                else if (!fCollecting)
                        {
                        if (fIsQuote)
                                {
                                fCollecting = fInQuote = TRUE;
                                argv[iarg] = szCmdLine + 1;
                                }
                        else if (!fIsWhite)
                                {
                                fCollecting = TRUE;
                                argv[iarg] = szCmdLine;
                                }
                        }
                }
        if (fCollecting)
                iarg++;
        *pargc = iarg;
#if 0
        for (iarg = 0; iarg < *pargc; iarg++)
                fprintf(stdout, "Arg %d: <%s>\n", iarg, (*pargv)[iarg]);
        fflush(stdout);
#endif
        return TRUE;
        }

VOID InfoFromArgv(VOID)
	{
	int iarg;

	for (iarg = 0; iarg < carg; iarg++)
		{
		char *szFlag = rgszArg[iarg];
		
		if (strcmp(szFlag, "-ip") == 0)
			{
		   strcpy (ti.szInputText, rgszArg[++iarg]);
			}
		else if (strcmp(szFlag, "-grm") == 0)
			{
		   strcpy (ti.szGrammarFile, rgszArg[++iarg]);
			}
		else if (strcmp(szFlag, "-op") == 0)
			{
		   strcpy (ti.szOutput, rgszArg[++iarg]);
			}
		else if (strcmp(szFlag, "-match") == 0)
			{
		   strcpy (ti.szMatchText, rgszArg[++iarg]);
			}
		else if (strcmp(szFlag, "-sreng") == 0)
			{
		   strcpy (ti.szSREngine, rgszArg[++iarg]);
			}
		else if (strcmp(szFlag, "-spk") == 0)
			{
		   strcpy (ti.szSpeaker, rgszArg[++iarg]);
			}
		else if (strcmp(szFlag, "-cor") == 0)
			{
			if (sscanf(rgszArg[++iarg], "%d", &ti.fCorrection) != 1)
				break;
			}
		else if (strcmp(szFlag, "-nl") == 0)
			{
			if (sscanf(rgszArg[++iarg], "%d", &ti.fNewLine) != 1)
				break;
			}
		else if (strcmp(szFlag, "-rt") == 0)
			{
			if (sscanf(rgszArg[++iarg], "%hx", &ti.wRealTime) != 1)
				break;
		}

		else if (strcmp(szFlag, "-res") == 0)
			{
			strcpy(resfile, rgszArg[++iarg]);
			}
		else if (strcmp(szFlag, "-sil") == 0)
			{
			strcpy(ti.szSilenceWave, rgszArg[++iarg]);
			}
        else if (strcmp(szFlag, "-load") == 0)
			{
             SpeakerLoaded = FALSE;
			} 
		else
			{
			break;
			}
		}
	if (iarg != carg)
		carg = 0;			// some error
	}

HRESULT loadspkr()
{
  HANDLE hFile;
  HRESULT hRes=E_FAIL;
  LPVOID pData = NULL;
  DWORD dwSize, d;
  char *pszAux=NULL;

  struct {
     char Guid[37];
     char szUser[256];
  } header;

  hFile = CreateFile(ti.szSpeaker, GENERIC_READ, FILE_SHARE_READ,
     NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (INVALID_HANDLE_VALUE == hFile) {
      hRes=E_FAIL;
	  goto Error;
  }

  dwSize = GetFileSize(hFile, NULL);
  if (0xFFFFFFFF == dwSize) {
      hRes=E_FAIL;
	  goto Error;
  }

  dwSize -=sizeof(header);
  pData = calloc(dwSize, 1);
  if (!pData) {
	 hRes=E_FAIL;
     goto Error;
  }

  ReadFile(hFile, &header, sizeof(header), &d, NULL);
  if (strcmp(header.Guid, pszGuidVer)) {
	  hRes=E_FAIL;
	  goto Error;
  }

  ReadFile(hFile, pData, dwSize, &d, NULL);

  // write the user
  pszAux=strchr(ti.szSpeaker,'.');
  if (pszAux) *pszAux=0;
  pszAux=strrchr(ti.szSpeaker,'\\');
  if (pszAux) strcpy(ti.szSpeaker,pszAux+1);
  hRes = gpISRSpeaker->Write(ti.szSpeaker, pData, dwSize);
 
  if ( hRes )
    if (hRes==SRERR_SPEAKEREXISTS) {
       hRes = gpISRSpeaker->Delete(ti.szSpeaker);
	   if (hRes) goto Error;
       hRes = gpISRSpeaker->Write(ti.szSpeaker, pData, dwSize);       
       if (hRes) goto Error;
    }
    else goto Error;

   CloseHandle(hFile);
   free(pData);
   return(0);
Error : CloseHandle(hFile);
        free(pData);
		return (hRes);
}

/* end add rt */

