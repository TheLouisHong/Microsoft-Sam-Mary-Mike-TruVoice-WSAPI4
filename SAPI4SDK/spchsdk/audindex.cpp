/***************************************************************
AudIndex.cpp - Sample that indexes .wav files using dictation.

Copyright (c) 1998 by Microsoft Corporation

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

#include <spchwrap.h>
#include "resource.h"
#include "ctools.h"

void EnableDisablePlay (HWND hWnd);


class CModeNotify : public IAudioDestNotifySink {

   public:

      CModeNotify (void);
      ~CModeNotify (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// IAudioDestNotifySink

      STDMETHODIMP AudioStop   (WORD);
      STDMETHODIMP AudioStart   (void);
      STDMETHODIMP FreeSpace  (DWORD, BOOL);
      STDMETHODIMP BookMark   (DWORD, BOOL);
   };

typedef CModeNotify * PCModeNotify;



class CMyGramNotifySink: public ISRGramNotifySinkW {
   public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ISRNotifySink
	   STDMETHODIMP BookMark       (DWORD);
	   STDMETHODIMP Paused         (void);
	   STDMETHODIMP PhraseFinish   (DWORD, QWORD, QWORD, PSRPHRASEW, LPUNKNOWN);
	   STDMETHODIMP PhraseHypothesis(DWORD, QWORD, QWORD, PSRPHRASEW, LPUNKNOWN);
	   STDMETHODIMP PhraseStart    (QWORD);
	   STDMETHODIMP ReEvaluate     (LPUNKNOWN);
	   STDMETHODIMP Training       (DWORD);
	   STDMETHODIMP UnArchive      (LPUNKNOWN);
};

class CSRWaveAudioNotifySink : public IAudioFileNotifySink {
public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // IAudioSourceNotifySink members
      STDMETHODIMP FileBegin      (DWORD);
      STDMETHODIMP FileEnd        (DWORD);
      STDMETHODIMP QueueEmpty     (void);
      STDMETHODIMP Posn           (QWORD, QWORD);

};

typedef struct {
   DWORD    dwTime;
   BOOL     fBestPath;
} AUDINDEX, *PAUDINDEX;

#define  TIMEREVENT        42
#define  TIMERTIME         20000


HINSTANCE   ghInstance;
char        gszWave[256] = "";
LPWAVEFORMATEX gpWFEX;       // wave format EX
DWORD          gdwWFEXSize;  // size of waveformatex
HMMIO       ghmmio;
DWORD       gdwTotalAudio;    // total # of bytes in data
DWORD       gdwTotalAudioScroll; // scaling for the scroll
DWORD       gdwCurrentAudio;  // current play position
DWORD       gdwStartAudio;    // index into the file where the audio starts
DWORD       gfPlaying = FALSE;        // if we're playing
CList       *gpIndex = NULL;     // index, uses AUDINDEX structures
CSREnum     *gpSREnum = NULL;    // sr enumerator
CSRMode     *gpSRMode = NULL;
CSRGramComp *gpSRGramComp = NULL;
CMyGramNotifySink gCMyGramNotifySink;
CSRWaveAudioNotifySink gSRWANS;
PIAUDIOFILE gpIAudioFileSR;
HWND        ghWndIndexing, ghWndPlay;
BOOL        gfTimerOn = FALSE;
CModeNotify gADNS;
PIAUDIO     gpIAudio = NULL;
PIAUDIODEST gpIAudioDest = NULL;
DWORD       gdwLastMarkPosn;     // last bookmark position


/***********************************************************************
 CModeNotify - Notification object. This will just pass the audio input
   notifications up to the CMode object.
*/

CModeNotify::CModeNotify (void)
{
// Get passed in the CMode object, save this

}


CModeNotify::~CModeNotify (void)
{
// this space intentionally left blank
}



STDMETHODIMP CModeNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */

   if (IsEqualIID (riid, IID_IUnknown)
    || IsEqualIID(riid,IID_IAudioDestNotifySink))

      *ppv = (LPVOID) this;

// otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}



STDMETHODIMP_ (ULONG) CModeNotify::AddRef (void)
{
// normally this increases a reference count, but this object
// is going to be freed as soon as the app is freed, so it doesn't
// matter

   return 1;
}



STDMETHODIMP_(ULONG) CModeNotify::Release (void)
{
// normally this releases a reference count, but this object
// is going to be freed when the application is freed so it doesnt
// matter

   return 1;
}



STDMETHODIMP CModeNotify::FreeSpace(DWORD dwBytes, BOOL fEOF)
{
   if (!gfPlaying)
      return NOERROR;

   // ask again how much free
   DWORD dwFree;
   BOOL  f;
   dwFree = 0;
   gpIAudioDest->FreeSpace (&dwFree, &f);

   dwFree = min(dwFree, gdwTotalAudio - gdwCurrentAudio);

   // inefficient code - figure out which word to show
   DWORD dwMark = (DWORD)-1L;

   if (gpIndex) {
      DWORD dwNum ,i;
      dwNum = gpIndex->GetNumElems();
      for (i =0; i < dwNum; i++) {
         PAUDINDEX   p;
         p = (PAUDINDEX) gpIndex->GetElem(i);
         if (!p) break;

         if (p->fBestPath && (p->dwTime > gdwCurrentAudio)) {
            DWORD dwDist;
            dwDist = p->dwTime - gdwCurrentAudio;

            if (dwDist > dwFree)
               break;   // no mark yet

            dwMark = i;
            dwFree = min(dwFree, dwDist);
            break;
         }
      }
   }

   if (dwFree) {
      // allocate the memory and load it in
      BYTE  *p;
      p = (BYTE*) malloc (dwFree);
      if (!p)
         return NOERROR;
      mmioSeek (ghmmio, gdwCurrentAudio + gdwStartAudio, SEEK_SET);
      mmioRead( ghmmio, (LPSTR)p, dwFree);
      gpIAudioDest->DataSet (p, dwFree);
      free (p);
      gdwCurrentAudio += dwFree;
   }

   // If his position of word, should probably set bookmark

   if (dwMark != (DWORD)-1L)
      gpIAudioDest->BookMark (dwMark);


   if ((gdwCurrentAudio >= gdwTotalAudio) && gfPlaying) {
      // stop
      gpIAudio->UnClaim();
      gfPlaying = FALSE;
      gdwLastMarkPosn = 0;

      EnableDisablePlay (ghWndPlay);
   }

   return NOERROR;
}


/**************************************************************
DisplayWord - displays the word at the specified index. Returns the
   DWORD index, or -1 if not found
   */
DWORD DisplayWord (DWORD dwMarkID)
{
   // look this up in the index and show the word
   PAUDINDEX   p;
   p = (PAUDINDEX) gpIndex->GetElem(dwMarkID);
   if (p) {
      SetDlgItemText (ghWndPlay, IDC_WORDS, (char*) (p+1));
      return p->dwTime;
   }
   else
      return (DWORD)-1L;
}

STDMETHODIMP CModeNotify::BookMark(DWORD dwMarkID, BOOL fFlush)
{
   if (fFlush)
      return NOERROR;

   DWORD dw;
   dw = DisplayWord (dwMarkID);
   if (dw != (DWORD)-1L) {
      gdwLastMarkPosn = dw;
      SetScrollPos (GetDlgItem (ghWndPlay, IDC_POSN),SB_CTL, gdwLastMarkPosn / gdwTotalAudioScroll, TRUE);
   }


   return NOERROR;
}

STDMETHODIMP CModeNotify::AudioStop(WORD wReason)
{
   return NOERROR;
}


STDMETHODIMP CModeNotify::AudioStart(void)
{
   return NOERROR;
}



/*****************************************************************************
CMyGramNotifySink - SR grammar notify sink
*/
STDMETHODIMP CMyGramNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRGramNotifySinkW)) {
      *ppObj = (LPVOID) this;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CMyGramNotifySink::AddRef (void)
{
   return 1;
}

STDMETHODIMP_ (ULONG) CMyGramNotifySink::Release (void)
{
   return 1;
}

STDMETHODIMP CMyGramNotifySink::BookMark (DWORD dwMark)
{
   return NOERROR;
}

STDMETHODIMP CMyGramNotifySink::Paused (void)
{
   return NOERROR;
}

STDMETHODIMP CMyGramNotifySink::PhraseFinish (DWORD dwFlags, QWORD qwStart,
                                                   QWORD qwEnd, PSRPHRASEW pSRPhrase,
                                                   LPUNKNOWN pUnk)
{
   // Set off timer if at end, so we give some extra time for sr to respond
   if (gfTimerOn) {
      KillTimer (ghWndIndexing, TIMEREVENT);
      SetTimer (ghWndIndexing, TIMEREVENT, TIMERTIME, 0);
   }

   // if it doesn't support ISRResGraphEx, then ignore it
   PISRRESGRAPHEX pGraph = NULL;
   if (pUnk)
      pUnk->QueryInterface (IID_ISRResGraphEx, (PVOID*) &pGraph);
   if (!pGraph)
      return NOERROR;

   // else, we get the best path
   HRESULT hRes;
   SDATA   sdPath;
   sdPath.pData = NULL;
   hRes = pGraph->BestPathEnum (0, NULL, 0, NULL, 0,
      FALSE, &sdPath);
   if (hRes)
      goto done;

   // enumerate all the arcs
   DWORD    adwArcs[10000];
   DWORD    dwNeeded, dwArcs;
   dwArcs = 0;
   hRes = pGraph->GetAllArcs (adwArcs, sizeof(adwArcs), &dwArcs, &dwNeeded);
   if (dwNeeded > sizeof(adwArcs)) {
      // only take a small set
      dwArcs = sizeof(adwArcs) / sizeof(DWORD);
   }
   if (hRes)
      goto done;


   // loop through all of the arcs
   DWORD i;
   for (i = 0; i < dwArcs; i++) {
      DWORD dwArc;
      dwArc = adwArcs[i];

      // find out if this is one of the N-best arcs
      DWORD j;
      BOOL fMatch = FALSE;
      for (j = 0; j < sdPath.dwSize / sizeof(DWORD); j++) {
         DWORD *p;
         p = (DWORD*) sdPath.pData + j;
         if (*p == dwArc) {
            fMatch = TRUE;
            break;
         }
      }

      // find the node to the left
      DWORD dwNode;
      hRes = pGraph->NodeGet (dwArc, &dwNode, FALSE);
      if (hRes)
         continue;   // error

      // get the time of the node
      DWORD dwStartTime;
      dwStartTime = 0;
      SDATA sd;
      sd.pData = NULL;
      pGraph->DataGet (dwNode, SRGNODE_TIME, &sd);
      if (sd.pData) {
         dwStartTime = (DWORD) ( *((QWORD*) sd.pData));
         CoTaskMemFree (sd.pData);
      }

      // get the word text, and convert to ansi
      char  szWord[256];
      szWord[0] = 0;
      sd.pData = NULL;
      hRes = pGraph->DataGet (dwArc, SRGARC_WORDTEXT, &sd);
      if (sd.pData) {
		   WideCharToMultiByte (CP_ACP,0, (WCHAR*) sd.pData, -1, szWord, sizeof(szWord),
			   NULL, NULL);
         CoTaskMemFree (sd.pData);
      }

      // if there's no word then continue
      if (!szWord[0])
         continue;

      // Store this in the list
      if (!gpIndex)
         gpIndex = new CList;
      if (!gpIndex)
         continue;

      // fill memory with the information
      BYTE     abHuge[1000];
      PAUDINDEX   pa;
      DWORD    dwSize;
      pa = (PAUDINDEX) abHuge;
      pa->dwTime = dwStartTime;
      pa->fBestPath = fMatch;
      strcpy ((char*)(pa + 1), szWord);
      dwSize = sizeof(AUDINDEX) + strlen(szWord) + 1;

      // insert it at the end
      DWORD dwNumElem;
      DWORD dwInsert;
      BOOL fSame;
      fSame = FALSE;
      dwNumElem = gpIndex->GetNumElems();
      dwInsert = (DWORD)-1L;
      for (j = dwNumElem - 1; j < dwNumElem; j--) {
         PAUDINDEX   p;
         p = (PAUDINDEX) gpIndex->GetElem(j);
         if (!p)
            break;

         // if it's completely equal then skip entirely
         if ((p->dwTime == dwStartTime) && (p->fBestPath == fMatch) &&
            !stricmp((char*)(pa+1), (char*)(p+1))) {
            fSame = TRUE;
            break;
         }

         if (p->dwTime >= dwStartTime)
            dwInsert = j;
         if (p->dwTime < dwStartTime)
            break;
      }
      if (fSame)
         continue;

      if (dwInsert != (DWORD)-1L)
         gpIndex->InsertElem (dwInsert, abHuge, dwSize);
      else
         gpIndex->AddElem (abHuge, dwSize);

   }



done:
   if (sdPath.pData)
      CoTaskMemFree (sdPath.pData);

   pGraph->Release();
	return NOERROR;

}


STDMETHODIMP CMyGramNotifySink::PhraseHypothesis (DWORD dwFlags, QWORD qwStart,
                                                   QWORD qwEnd, PSRPHRASEW pSRPhrase,
                                                   LPUNKNOWN pUnk)
{
   // Set off timer if at end, so we give some extra time for sr to respond
   if (gfTimerOn) {
      KillTimer (ghWndIndexing, TIMEREVENT);
      SetTimer (ghWndIndexing, TIMEREVENT, TIMERTIME, 0);
   }

   // print out the results
	PSRWORDW  pSRWord, pSRMax;
	if (!pSRPhrase || !(dwFlags & ISRNOTEFIN_THISGRAMMAR)) {
		SetDlgItemText (ghWndIndexing, IDC_HYP, "");
		return NOERROR;
	}

	// write out a string with the results
	// loop through all of the words and display them
	pSRMax = (PSRWORDW) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
	pSRWord = (PSRWORDW) (pSRPhrase->abWords);
   char  szTemp[1000];

	szTemp[0] = 0;

	while (pSRWord < pSRMax)
	{
		// add word
		char	szaTemp[256];
		WideCharToMultiByte (CP_ACP,0, pSRWord->szWord, -1, szaTemp, sizeof(szaTemp),
			NULL, NULL);

      if ((strlen(szTemp) + strlen(szaTemp) + 2) < sizeof(szTemp)) {

         if (pSRWord != (PSRWORDW) (pSRPhrase->abWords))
		      strcat (szTemp, " ");
		   strcat (szTemp, szaTemp);
      }

		pSRWord = (PSRWORDW) ((BYTE*) pSRWord + pSRWord->dwSize);
	};
	SetDlgItemText (ghWndIndexing, IDC_HYP, szTemp);

   return NOERROR;
}


STDMETHODIMP CMyGramNotifySink::PhraseStart (QWORD qwTime)
{
   // Set off timer if at end, so we give some extra time for sr to respond
   if (gfTimerOn) {
      KillTimer (ghWndIndexing, TIMEREVENT);
      SetTimer (ghWndIndexing, TIMEREVENT, TIMERTIME, 0);
   }

   return NOERROR;
}

STDMETHODIMP CMyGramNotifySink::ReEvaluate (LPUNKNOWN pUnk)
{
   return NOERROR;
}

STDMETHODIMP CMyGramNotifySink::Training (DWORD dwVal)
{
   return NOERROR;
}

STDMETHODIMP CMyGramNotifySink::UnArchive (LPUNKNOWN pUnk)
{
   return NOERROR;
}


/*****************************************************************************
CSRWaveAudioNotifySink - Wave audio notify sink that inficates how
	much audio has been processed.
*/

STDMETHODIMP CSRWaveAudioNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IAudioFileNotifySink)) {
      *ppObj = (LPVOID) this;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CSRWaveAudioNotifySink::AddRef (void)
{
   return 1;
}

STDMETHODIMP_ (ULONG) CSRWaveAudioNotifySink::Release (void)
{
   return 1;
}

STDMETHODIMP CSRWaveAudioNotifySink::FileBegin      (DWORD dwVal)
{
   // Do nothing
   return NOERROR;
}

STDMETHODIMP CSRWaveAudioNotifySink::FileEnd        (DWORD dwVal)
{
   // Set off timer if at end, so we give some extra time for sr to respond
   if (gfTimerOn)
      KillTimer (ghWndIndexing, TIMEREVENT);
   gfTimerOn = TRUE;
   SetTimer (ghWndIndexing, TIMEREVENT, TIMERTIME, 0);


   return NOERROR;
}

STDMETHODIMP CSRWaveAudioNotifySink::QueueEmpty     (void)
{
   // do nothing
   return NOERROR;
}

STDMETHODIMP CSRWaveAudioNotifySink::Posn (QWORD qwProcessed, QWORD qwLeft)
{
   // Percent doent
   DWORD dwTotal = ((DWORD) qwProcessed + (DWORD) qwLeft) / 100;
   if (!dwTotal)
      dwTotal = 1;

   char  szTemp[128];
   wsprintf (szTemp, "%d%%", (int) qwProcessed / (int) dwTotal);
   SetDlgItemText (ghWndIndexing, IDC_PROGRESS, szTemp);
   return NOERROR;
}



/*************************************************************************
Indexing - Show progress on indexing
*/
BOOL CALLBACK IndexingDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
      {
         // remember this window
         ghWndIndexing = hWnd;
         gfTimerOn = FALSE;

         HRESULT hRes;
         hRes = gpSRGramComp->Activate();
         if (hRes) {
            MessageBox (hWnd, "Can't activate speech recognition.", NULL, MB_OK);
            EndDialog (hWnd, IDCANCEL);
            return TRUE;
         }
      }
      break;

   case WM_TIMER:
      // Kil lthe timer
      if (gfTimerOn) {
         KillTimer (ghWndIndexing, TIMEREVENT);
         gfTimerOn = FALSE;
      }

      // deactivate
      gpSRGramComp->Deactivate();
      EndDialog (hWnd, IDOK);

      return TRUE;

   case WM_CLOSE:
      PostMessage (hWnd, WM_COMMAND, IDCANCEL, 0);
      return TRUE;


   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDCANCEL:
         if (gfTimerOn)
            KillTimer (hWnd, TIMEREVENT);
         gfTimerOn = FALSE;

         gpSRGramComp->Deactivate();
         EndDialog (hWnd, IDCANCEL);
         break;
      }
   }
   return FALSE;
}


/*************************************************************************
SelectSR - Select the speech recognition engine to index.
*/
BOOL CALLBACK SelectSRDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
      {
         gpSREnum->Reset();

         // loop through and list all the sr engines
         SRMODEINFOW mi;
         while (!gpSREnum->Next(&mi)) {
            if (!(mi.dwGrammars & SRGRAM_DICTATION) || !(mi.dwInterfaces & SRI_ISRRESGRAPHEX))
               continue;

            // add it
            char  szTemp[256];
	         WideCharToMultiByte (CP_ACP, 0, mi.szProductName, -1, szTemp, sizeof(szTemp),
		         0, 0);

            SendDlgItemMessage (hWnd, IDC_SR, CB_ADDSTRING, 0, (LPARAM) szTemp);
         }

         SendDlgItemMessage (hWnd, IDC_SR, CB_SETCURSEL, 0, 0);

         // set the speed buttons
         CheckDlgButton (hWnd, IDC_14, BST_CHECKED);
         CheckDlgButton (hWnd, IDC_MAXACC, BST_CHECKED);
      }
      break;

   case WM_CLOSE:
      EndDialog (hWnd, IDCANCEL);
      return TRUE;


   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDOK:
         {
         // get the text & create the engine
         char szaTemp[128];
         SRMODEINFOW mi, mi2;
         WCHAR szwTemp[128];
         int   iNum;
         iNum = SendDlgItemMessage (hWnd, IDC_SR, CB_GETCURSEL, 0, 0);
         if (iNum == CB_ERR) {
            MessageBox (hWnd, "You don't have any speech recognition engines that can be used for indexing.",
               NULL, MB_OK);
            return TRUE;
         }
         szaTemp[0] = 0;
         SendDlgItemMessage (hWnd, IDC_SR, CB_GETLBTEXT, iNum, (LPARAM) szaTemp);
         memset (&mi, 0, sizeof(mi));
	      MultiByteToWideChar (CP_ACP, 0, szaTemp, -1,
		      mi.szProductName, sizeof(mi.szProductName) / sizeof(WCHAR));
         gpSREnum->Find (&mi, &mi2);
         if (wcsicmp(mi.szProductName, mi2.szProductName)) {
            MessageBox (hWnd, "Unexpected error.",
               NULL, MB_OK);
            return TRUE;
         }

         // delete the sr engine
         if (gpSRMode)
            delete gpSRMode;
         if (gpSRGramComp)
            delete gpSRGramComp;
         if (gpIAudioFileSR)
            gpIAudioFileSR->Release();
         gpSRMode = NULL;
         gpSRGramComp = NULL;
         gpIAudioFileSR = NULL;

         // create the sr engine
         gpSRMode = new CSRMode;
         if (!gpSRMode)
            return TRUE;
         HRESULT hRes;

         // Need to create a wave audio object
	      hRes = CoCreateInstance(CLSID_AudioSourceFile, 
		      NULL, CLSCTX_INPROC_SERVER, 
		      IID_IAudioFile, 
		      (PVOID *) &gpIAudioFileSR);
         if (hRes) {
            MessageBox (hWnd, "Unexpected error.",
               NULL, MB_OK);
         }
	      gpIAudioFileSR -> Register(&gSRWANS);

         // add the file
	      MultiByteToWideChar (CP_ACP, 0, gszWave, -1,
		      szwTemp, sizeof(szwTemp) / sizeof(WCHAR));
         hRes = gpIAudioFileSR->Add (szwTemp, 1);
         if (hRes) {
            MessageBox (hWnd, "Unexpected error.",
               NULL, MB_OK);
         }

         // set the playback speed
         WORD  wSpeed = 0x100;
         if (IsDlgButtonChecked (hWnd,IDC_18))
            wSpeed /= 8;
         else if (IsDlgButtonChecked (hWnd,IDC_14))
            wSpeed /= 4;
         else if (IsDlgButtonChecked (hWnd,IDC_14))
            wSpeed /= 4;
         else if (IsDlgButtonChecked (hWnd,IDC_12))
            wSpeed /= 2;
         else if (IsDlgButtonChecked (hWnd,IDC_2))
            wSpeed *= 2;
         else if (IsDlgButtonChecked (hWnd,IDC_4))
            wSpeed *= 4;
         gpIAudioFileSR->RealTimeSet (wSpeed);

         // init the engine
         gpSRMode->InitAudioSourceObject(gpIAudioFileSR);
         gpIAudioFileSR->Release();
         gpIAudioFileSR = NULL;

         hRes = gpSRMode->Init (mi2.gModeID);
         if (hRes) {
            MessageBox (hWnd, "The speech recognition engine cannot use the wave file format you've specified. "
               "You should use the wave editor to re-sample it to an acceptable rate, usually "
               "8, 11, 16, or 22 kHz 16-bit mono.", NULL, MB_OK);
            return TRUE;
         }

         // set speaker, accuracy, and thread priority
         szaTemp[0] = 0;
         GetDlgItemText (hWnd, IDC_SPEAKER, szaTemp, sizeof(szaTemp));
	      MultiByteToWideChar (CP_ACP, 0, szaTemp, -1,
		      szwTemp, sizeof(szwTemp) / sizeof(WCHAR));
         if (szwTemp[0])
            gpSRMode->SpeakerSet (szwTemp);
         DWORD dwAcc = 50;
         if (IsDlgButtonChecked (hWnd,IDC_MINACC))
            dwAcc = 0;
         else if (IsDlgButtonChecked (hWnd,IDC_MAXACC))
            dwAcc = 100;
         gpSRMode->DWORDSet (SRATTR_ACCURACYSLIDER, dwAcc);
         gpSRMode->DWORDSet (SRATTR_THREADPRIORITY, THREAD_PRIORITY_LOWEST);



         // create the grammar
         WCHAR *psz;
         psz = 
            L"[Grammar]\n"
            L"type=dictation\n";
         gpSRGramComp = gpSRMode->GrammarFromMemory (&gCMyGramNotifySink, psz, (wcslen(psz)+1)*2);
         if (!gpSRGramComp) {
            MessageBox (hWnd, "Unexpected error.",
               NULL, MB_OK);
            return TRUE;
         }


         // Pull up dialog box to scan in data
         DialogBox (ghInstance, MAKEINTRESOURCE(IDD_INDEXING),
            hWnd, (DLGPROC) IndexingDlgProc);
         ghWndIndexing = NULL;

         // Write out index data to file
         char  szIndexFile[256];
         wsprintf (szIndexFile, "%s.idx", gszWave);
         FILE  *f;
         f = fopen (szIndexFile, "wt");
         if (f) {
            DWORD dwNumElem;
            DWORD j;
            dwNumElem = gpIndex ? gpIndex->GetNumElems() : 0;
            for (j = 0; j < dwNumElem; j++) {
               PAUDINDEX   p;
               p = (PAUDINDEX) gpIndex->GetElem(j);
               if (!p)
                  break;
               fprintf (f, "%d,%c,%s\n",
                  (int) p->dwTime,
                  p->fBestPath ? 'b' : 'a',
                  (char*)(p+1));
            }
            fclose (f);
         }
         else {
            MessageBox (hWnd, "Unable to write the index file!", NULL, MB_OK);
         }

         // delete the sr engine
         if (gpSRGramComp)
            delete gpSRGramComp;
         if (gpIAudioFileSR)
            gpIAudioFileSR->Release();
         if (gpSRMode)
            delete gpSRMode;
         gpSRMode = NULL;
         gpSRGramComp = NULL;
         gpIAudioFileSR = NULL;

         EndDialog (hWnd, IDOK);
         return TRUE;
         }
      case IDCANCEL:
         EndDialog (hWnd, IDCANCEL);
         return TRUE;
      }
   };

   return FALSE;
}


/*************************************************************************
FreeCurrentWave - Frees all of the data for the current wave.
*/
void FreeCurrentWave (void)
{
   if (gpWFEX)
      free (gpWFEX);
   gpWFEX = NULL;

   if (ghmmio)
      mmioClose (ghmmio, 0);
   ghmmio = NULL;

   gszWave[0] = 0;

   if (gpIndex)
      delete gpIndex;
   gpIndex = NULL;

   // Free up audio playback object
   if (gpIAudio)
      gpIAudio->Release();
   gpIAudio = NULL;

   if (gpIAudioDest)
      gpIAudioDest->Release();
   gpIAudioDest = NULL;
}

/*************************************************************************
OpenWave - This opens a wave for playback, and opens the audio index.

inputs
   HWND  hWnd - window to pull common dialog off of, and also where file
            name is.
*/
void OpenWave (HWND hWnd)
{
   // free the old one
   FreeCurrentWave ();

   // open
	OPENFILENAME	ofn;

	memset (&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = ghInstance;
	ofn.lpstrFilter =
		"Wave File (*.wav)\0" "*.wav\0"
		"\0\0";
	ofn.lpstrFile = gszWave;
	ofn.nMaxFile = sizeof(gszWave);
	ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST; 
	ofn.lpstrDefExt = "wav";

	if (!GetOpenFileName(&ofn))
		return;

   // try opening this
	// open and make sure can support wave format
	ghmmio = mmioOpen( gszWave, NULL, MMIO_READ | MMIO_ALLOCBUF );
   if(!ghmmio) {
      MessageBox (hWnd, "Can't open the specified file.", NULL, MB_OK);
      gszWave[0] = 0;
      return;  // can't open
   }

	MMCKINFO mminfopar, mminfosub;
	// search for wave type...
	mminfopar.fccType = mmioFOURCC('W','A','V','E');
	if( mmioDescend( ghmmio, (LPMMCKINFO)&mminfopar, NULL, MMIO_FINDRIFF ) ) {
      MessageBox (hWnd, "Can't open the specified file.", NULL, MB_OK);
      gszWave[0] = 0;
		mmioClose (ghmmio, 0);
		return;
	}
	
	// get the file
	// search for format chunk...
	mminfosub.ckid = mmioFOURCC('f','m','t',' ');
	if( mmioDescend( ghmmio, &mminfosub, &mminfopar, MMIO_FINDCHUNK ) ) {
      MessageBox (hWnd, "Can't open the specified file.", NULL, MB_OK);
      gszWave[0] = 0;
		mmioClose (ghmmio, 0);
		return;
	}
		
	// read the wave format from the file...
   gpWFEX = (WAVEFORMATEX *) malloc (mminfosub.cksize);
	mmioRead(ghmmio, (HPSTR)gpWFEX, mminfosub.cksize );
   gdwWFEXSize = mminfosub.cksize;

	// get the total wave data size (mminfo.cksize)...
	mmioAscend( ghmmio, &mminfosub, 0 );
	mminfosub.ckid = mmioFOURCC('d','a','t','a');
	if( mmioDescend( ghmmio, &mminfosub, &mminfopar, MMIO_FINDCHUNK ) ) {
      MessageBox (hWnd, "Can't open the specified file.", NULL, MB_OK);
      gszWave[0] = 0;
		mmioClose (ghmmio, 0);
		return;
	}
	
   gdwTotalAudio = mminfosub.cksize;
   gdwTotalAudioScroll = max (1, gdwTotalAudio / 1024);
   gdwCurrentAudio = 0;
   gdwLastMarkPosn = 0;
   gdwStartAudio = (DWORD) mmioSeek (ghmmio, 0, SEEK_CUR);
   gfPlaying = FALSE;
   SetScrollRange (GetDlgItem (ghWndPlay, IDC_POSN),SB_CTL, 0, 1024, FALSE);
   SetScrollPos (GetDlgItem (ghWndPlay, IDC_POSN),SB_CTL, gdwCurrentAudio / gdwTotalAudioScroll, TRUE);


   // create the audio object
	CoCreateInstance(CLSID_MMAudioDest, 
		NULL, CLSCTX_INPROC_SERVER, 
		IID_IAudio, 
		(PVOID *) &gpIAudio);
   gpIAudio->QueryInterface (IID_IAudioDest, (PVOID*) &gpIAudioDest);
   gpIAudio->PassNotify(&gADNS, IID_IAudioDestNotifySink);
   SDATA sd;
   sd.pData = gpWFEX;
   sd.dwSize = gdwWFEXSize;
   gpIAudio->WaveFormatSet (sd);

   // Need to read in the index file
   char  szIndexFile[256];
   wsprintf (szIndexFile, "%s.idx", gszWave);
   FILE  *f;
   f = fopen (szIndexFile, "rt");
   if (f) {
      BYTE  abHuge[1000];
      char  c;
      int   i;
      PAUDINDEX p;
      char  *psz;
      p = (PAUDINDEX) abHuge;
      psz = (char*) (p+1);

      while (3 == fscanf(f, "%d,%c,%s\n", &i, &c, psz)) {
         p->dwTime = (DWORD) i;
         p->fBestPath = (c == 'b');
         if (!gpIndex)
            gpIndex = new CList;
         if (!gpIndex)
            break;
         gpIndex->AddElem (p, sizeof(AUDINDEX) + strlen(psz) + 1);
      }
      fclose (f);
   }



   // ask if no index
   if (!gpIndex) {
      if (IDYES == MessageBox (hWnd, "Do you wish to index the wave file now?",
         "Index", MB_YESNO))
            DialogBox (ghInstance, MAKEINTRESOURCE(IDD_SELECTSR),
               hWnd, (DLGPROC) SelectSRDlgProc);

   }
}



/*************************************************************************
EnableDisablePlay - Enables/disabled windows based upon what data's available.
*/
void EnableDisablePlay (HWND hWnd)
{
   SetDlgItemText (hWnd, IDC_WAVEFILE, gszWave);
   EnableWindow (GetDlgItem(hWnd, IDC_OPENWAVE), !gfPlaying);
   EnableWindow (GetDlgItem(hWnd, IDC_INDEXWAVE), !gfPlaying && gszWave[0]);
   EnableWindow (GetDlgItem(hWnd, IDC_POSN), gpIndex ? TRUE : FALSE);
   EnableWindow (GetDlgItem(hWnd, IDC_STOP), gpIndex && gfPlaying);
   EnableWindow (GetDlgItem(hWnd, IDC_PLAY), gpIndex && !gfPlaying);
   EnableWindow (GetDlgItem(hWnd, IDC_SEARCHFOR), gpIndex ? TRUE : FALSE);
   EnableWindow (GetDlgItem(hWnd, IDC_FINDNEXT), gpIndex ? TRUE : FALSE);
   EnableWindow (GetDlgItem(hWnd, IDC_FINDPREV), gpIndex ? TRUE : FALSE);
   EnableWindow (GetDlgItem(hWnd, IDC_USEALT), gpIndex ? TRUE : FALSE);
}


/*************************************************************************
FindNextPrev - Finds the next (or previous) occurance of the word

inputs
   char     *psz - word to look for
   BOOL     fNext - if TRUE, look forward. False, look back
   BOOL     fUseAlts - If TRUE, look at alts
*/
void FindNextPrev (char *psz, BOOL fNext, BOOL fUseAlts)
{
   // first off, find out where our current position is
   DWORD dwNum ,i;
   PAUDINDEX   p;
   DWORD dwMark = 0;
   // don't find word in immediate vicinity
   DWORD dwTime = gdwLastMarkPosn + (fNext ? 32000 : -32000);
   dwNum = gpIndex->GetNumElems();
   for (i =0; i < dwNum; i++) {
      p = (PAUDINDEX) gpIndex->GetElem(i);
      if (!p) break;

      if (p->dwTime <= dwTime)
         dwMark = i;
      else
         break;
   }

   // find it
   DWORD dwFound = (DWORD)-1L;
   if (fNext) { // forward
      for (i = dwMark+1; i < dwNum; i++) {
         p = (PAUDINDEX) gpIndex->GetElem(i);
         if (!p) break;

         if (!fUseAlts)
            if (!p->fBestPath)
               continue;

         // compare the strings
         if (!stricmp ((char*)(p+1), psz)) {
            dwFound = i;
            break;
         }
      }
   }
   else { // back
      for (i = dwMark-1; i < dwNum; i--) {
         p = (PAUDINDEX) gpIndex->GetElem(i);
         if (!p) break;

         if (!fUseAlts)
            if (!p->fBestPath)
               continue;

         // compare the strings
         if (!stricmp ((char*)(p+1), psz)) {
            dwFound = i;
            break;
         }
      }
   }

   if (dwFound == (DWORD)-1L) {
      char  szTemp[512];
      wsprintf (szTemp, "Can't find the word '%s'.", psz);
      MessageBox (ghWndPlay, szTemp, "Index", MB_OK);
      return;
   }

   // else, found it
   // if playing then change playback position
   p = (PAUDINDEX) gpIndex->GetElem(dwFound);
   gdwCurrentAudio = p->dwTime;
   gdwLastMarkPosn = gdwCurrentAudio;
   if (gfPlaying) {
      gpIAudio->Flush();
      gADNS.FreeSpace (0,0);
   }
   gADNS.BookMark(dwFound, FALSE);

}

/*************************************************************************
Play - Dialog box to allow playback.

*/
BOOL CALLBACK PlayDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
      {
         ghWndPlay = hWnd;

         EnableDisablePlay (hWnd);
         CheckDlgButton (hWnd, IDC_USEALT, BST_CHECKED);
      }
      break;

   case WM_CLOSE:
      // stop playing
      FreeCurrentWave();

      EndDialog (hWnd, IDCANCEL);
      return TRUE;


   case WM_HSCROLL:
      {
      // Handle scroll bars
      HWND  hWndScroll = (HWND) lParam;
      int   iPos, iMin, iMax, iDelta;
      iPos = GetScrollPos (hWndScroll, SB_CTL);
      iMin = 0;
      iMax = 1024;
      iDelta = 16000 / gdwTotalAudioScroll + 1;
   
      switch (LOWORD(wParam)) {
      case SB_BOTTOM:
         //iPos = iMax;
         break;
      case SB_ENDSCROLL:
         return TRUE;   // do nothing
      case SB_LINELEFT:
         iPos -= iDelta;
         break;
      case SB_PAGELEFT:
         iPos -= iDelta * 10;
         break;
      case SB_LINERIGHT:
         iPos += iDelta;
         break;
      case SB_PAGERIGHT:
         iPos += iDelta * 10;
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
      SetScrollPos (hWndScroll, SB_CTL, iPos, TRUE);

      // if playing then change playback position
      gdwCurrentAudio = (DWORD) iPos * gdwTotalAudioScroll;
      gdwCurrentAudio -= (gdwCurrentAudio % gpWFEX->nBlockAlign);
      gdwLastMarkPosn = gdwCurrentAudio;
      if (gfPlaying) {
         gpIAudio->Flush();
         gADNS.FreeSpace (0,0);
      }

      // Need to update word displayed
      // this is slow, but easy to understand
      if (gpIndex) {
         DWORD dwNum ,i;
         dwNum = gpIndex->GetNumElems();
         for (i =0; i < dwNum; i++) {
            PAUDINDEX   p;
            p = (PAUDINDEX) gpIndex->GetElem(i);
            if (!p) break;

            if (p->fBestPath && (p->dwTime > gdwCurrentAudio)) {
               DisplayWord (i);
               break;
            }
         }
      }



      return TRUE;
      }  
      break;

   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDC_OPENWAVE:
         OpenWave (hWnd);
         EnableDisablePlay (hWnd);
         break;
      case IDC_INDEXWAVE:
         // if we already have an index then ask for verification
         if (gpIndex) {
            if (IDYES != MessageBox (hWnd, "You have already indexed the file. Do you want to re-index it?",
               "Index", MB_YESNO))
               return TRUE;
            delete gpIndex;
            gpIndex = NULL;
         }

         DialogBox (ghInstance, MAKEINTRESOURCE(IDD_SELECTSR),
            hWnd, (DLGPROC) SelectSRDlgProc);
         EnableDisablePlay (hWnd);
         break;
      case IDC_PLAY:
         {
         if (gfPlaying)
            return TRUE;
         gdwCurrentAudio = gdwLastMarkPosn;
         if (gdwTotalAudio <= gdwCurrentAudio)
            gdwCurrentAudio = 0;
         SetScrollPos (GetDlgItem (ghWndPlay, IDC_POSN),SB_CTL, gdwCurrentAudio / gdwTotalAudioScroll, TRUE);

         HRESULT hRes;
         hRes = gpIAudio->Claim();
         if (hRes) {
            MessageBox (hWnd, "Audio device in use.", NULL, MB_OK);
            return TRUE;
         }
         gpIAudio->Start();
         gfPlaying = TRUE;

         // send some audio
         gADNS.FreeSpace (0,0);

         EnableDisablePlay (hWnd);
         break;
         }

      case IDC_STOP:
         {
         if (!gfPlaying)
            return TRUE;

         // stop
         gpIAudio->Flush();
         gpIAudio->UnClaim();
         gfPlaying = FALSE;

         EnableDisablePlay (hWnd);
         break;
         }
      case IDC_FINDNEXT:
      case IDC_FINDPREV:
         {
            char  szTemp[256];
            szTemp[0] = 0;
            GetDlgItemText (hWnd, IDC_SEARCHFOR, szTemp, sizeof(szTemp));

            FindNextPrev (szTemp, LOWORD(wParam) == IDC_FINDNEXT,
               IsDlgButtonChecked (hWnd, IDC_USEALT));
         }
         break;
      }
   };

   return FALSE;
}



/*************************************************************************
winmain - Windows main code.
*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
   ghInstance = hInstance;

   CoInitialize(NULL);

   gpSREnum = new CSREnum;
   gpSREnum->Init();

   DialogBox (ghInstance, MAKEINTRESOURCE(IDD_PLAY),
      NULL, (DLGPROC) PlayDlgProc);

   if (gpSREnum)
      delete gpSREnum;
   if (gpSRMode)
      delete gpSRMode;
   if (gpSRGramComp)
      delete gpSRGramComp;
   if (gpIAudioFileSR)
      gpIAudioFileSR->Release();

   CoUninitialize ();

   return 0;
}


