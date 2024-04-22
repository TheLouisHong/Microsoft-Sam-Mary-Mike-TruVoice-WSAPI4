/*******************************************************
test.cpp - main code.



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
#include <tchar.h>
#include <speech.h>
#include "resource.h"

extern HINSTANCE ghInstance;
extern HWND ghWndEditor;
extern PISTGRAMCOMP gpISTGramComp;

#ifdef USELOGGING
extern PISTLOG  gpISTLog;
extern WCHAR    gszLogApp[];
#endif

/* typedefs */
/* globals */
static   PISRENUM    gpISREnum = NULL;
static   PISRFIND    gpISRFind = NULL;
static   PISRCENTRAL gpISRCentral = NULL;
static   PISRATTRIBUTES  gpISRAttributes = NULL;  // attributes
static   PISRGRAMCOMMON gpISRGramCommon = NULL;
static   HWND        ghWndTest = NULL;

static   _TCHAR       gszSREngine[128];
static   _TCHAR       gszUserName[128];

class CTestGramNotify : public ISRGramNotifySinkW {
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
	   STDMETHODIMP PhraseFinish   (DWORD, QWORD, QWORD, PSRPHRASEW, LPUNKNOWN);
	   STDMETHODIMP PhraseHypothesis(DWORD, QWORD, QWORD, PSRPHRASEW, LPUNKNOWN);
	   STDMETHODIMP PhraseStart    (QWORD);
	   STDMETHODIMP ReEvaluate     (LPUNKNOWN);
	   STDMETHODIMP Training       (DWORD);
	   STDMETHODIMP UnArchive      (LPUNKNOWN);
   };
typedef CTestGramNotify * PCTestGramNotify;

CTestGramNotify      gTestGramNotify;

class CTestNotify : public ISRNotifySinkW {
   private:

   public:
      CTestNotify (void);
      ~CTestNotify (void);

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ISRNotifySink
      STDMETHODIMP AttribChanged  (DWORD);
      STDMETHODIMP Interference   (QWORD, QWORD, DWORD);
      STDMETHODIMP Sound          (QWORD, QWORD);
      STDMETHODIMP UtteranceBegin (QWORD);
      STDMETHODIMP UtteranceEnd   (QWORD, QWORD);
      STDMETHODIMP VUMeter        (QWORD, WORD);
   };
typedef CTestNotify * PCTestNotify;

CTestNotify      gTestNotify;

void FillResListWindow(HWND hWndList, LPUNKNOWN punkResult);
void ShowSupportedInterfaces(HWND hWnd, LPUNKNOWN punkResult);

/*************************************************************************
CTestNotify - Notification object. All this this will do is display
   its notification on the main window.
*/
CTestNotify::CTestNotify (void)
{
// this space intentionally left blank
}

CTestNotify::~CTestNotify (void)
{
// this space intentionally left blank
}

STDMETHODIMP CTestNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
*ppv = NULL;

/* always return our IUnkown for IID_IUnknown */
if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRNotifySinkW)) {
   *ppv = (LPVOID) this;
   return NOERROR;
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
};


STDMETHODIMP CTestNotify::AttribChanged(DWORD dwID)
{
   return NOERROR;
}

STDMETHODIMP CTestNotify::Interference(QWORD qwStart, QWORD qwEnd, DWORD dwID)
{
#ifdef USELOGGING
   if (gpISTLog)
      gpISTLog->Text (gszLogApp, L"Interference", 3);
#endif
   return NOERROR;
}

STDMETHODIMP CTestNotify::Sound(QWORD qwTime,QWORD qwEnd)
{
#ifdef USELOGGING
   if (gpISTLog)
      gpISTLog->Text (gszLogApp, L"Sound", 3);
#endif
   return NOERROR;
}

STDMETHODIMP CTestNotify::UtteranceBegin(QWORD qwTime)
{
#ifdef USELOGGING
   if (gpISTLog)
      gpISTLog->Text (gszLogApp, L"UtteranceBegin", 3);
#endif
   return NOERROR;
}

STDMETHODIMP CTestNotify::UtteranceEnd(QWORD qwStart, QWORD qwEnd)
{
#ifdef USELOGGING
   if (gpISTLog)
      gpISTLog->Text (gszLogApp, L"UtteranceEnd", 3);
#endif
   return NOERROR;
}

STDMETHODIMP CTestNotify::VUMeter(QWORD qwTime, WORD wLevel)
{
   return NOERROR;
}


/*************************************************************************
ParseIt - Parses a PSRPHRASEW.

inputs
	PSRPHRASEW		pPhrase- phrase. If this is NULL then it uses
                  the DWORD to indicate which result to get
   DWORD          dwAlt - Alternate result to get. Only usedif pPhrase is NULL.
	char			szTemp - string to fill in with parse description
returns
	none
*/
HRESULT ParseIt (PSRPHRASEW pPhrase, DWORD dwAlt, char *szTemp)
{
	HRESULT	hRes;
	DWORD	dwCmdID;
	WCHAR	*pMem;
	DWORD	dwSize;

   if (pPhrase)
	   hRes = gpISTGramComp->PhraseParse (pPhrase, &dwCmdID, &pMem, &dwSize);
   else
      hRes = gpISTGramComp->PhraseParseAlt (dwAlt, &dwCmdID, &pMem, &dwSize);
	if (hRes) {
      if (pPhrase)
		   strcpy (szTemp, "FAILED");
		return hRes;
	}

   if (pPhrase)
	   sprintf (szTemp, "Cmd=%d (0x%x)", (int) dwCmdID, (int) dwCmdID);
   else
	   sprintf (szTemp, "Alternate %d: Cmd=%d (0x%x)", (int) dwAlt, (int) dwCmdID, (int) dwCmdID);

	if (pMem) {
		// parse extra strings
		char szaTemp[256];
		WideCharToMultiByte (CP_ACP,0, pMem, -1, szaTemp, sizeof(szaTemp),
			NULL, NULL);
		strcat (szTemp, ", ");
		strcat (szTemp, szaTemp);

		// free memory
		CoTaskMemFree (pMem);
	}

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
if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRGramNotifySinkW)) {
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
   QWORD qTimeStampEnd, PSRPHRASEW pSRPhrase, LPUNKNOWN lpResults)
{
	char  szTemp[512];
	PSRWORDW  pSRWord, pSRMax;

	if (!pSRPhrase) {
		SetDlgItemText (ghWndTest, IDC_RECOGEDIT, "[Unrecognized]");
		SetDlgItemText (ghWndTest, IDC_PARSE, "");
		SetDlgItemText (ghWndTest, IDC_RECOGNIZED, "");

#ifdef USELOGGING
      if (gpISTLog)
         gpISTLog->Text (gszLogApp, L"PhraseFinish: Unrecognized", 2);
#endif
		return NOERROR;
	}

#ifdef USELOGGING
   {
      WCHAR szLog[256];

      wcscpy (szLog, L"PhraseFinish:");

	   // write out a string with the results
	   // loop through all of the words and display them
	   pSRMax = (PSRWORDW) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
	   pSRWord = (PSRWORDW) (pSRPhrase->abWords);

	   szTemp[0] = 0;

	   while (pSRWord < pSRMax)
	   {
		   // add word
         wcscat (szLog, L" ");
         wcscat (szLog, pSRWord->szWord);
			pSRWord = (PSRWORDW) ((BYTE*) pSRWord + pSRWord->dwSize);
	   };

      if (gpISTLog)
         gpISTLog->Text (gszLogApp, szLog, 2);
   }
#endif

	// write out a string with the results
	// loop through all of the words and display them
	pSRMax = (PSRWORDW) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
	pSRWord = (PSRWORDW) (pSRPhrase->abWords);

	szTemp[0] = 0;

	while (pSRWord < pSRMax)
	{
		// add word
		char	szaTemp[256];
		WideCharToMultiByte (CP_ACP,0, pSRWord->szWord, -1, szaTemp, sizeof(szaTemp),
			NULL, NULL);

		strcat (szTemp, szaTemp);
		strcat (szTemp, " ");

		pSRWord = (PSRWORDW) ((BYTE*) pSRWord + pSRWord->dwSize);
	};
	SetDlgItemText (ghWndTest, IDC_RECOGEDIT, szTemp);

	// Need to write out the parse string
	ParseIt (pSRPhrase, 0, szTemp);
	SetDlgItemText (ghWndTest, IDC_PARSE, szTemp);

	if (lpResults) {
		FillResListWindow(GetDlgItem(ghWndTest, IDC_RECOGNIZED), lpResults);
#ifdef _DEBUG
      ShowSupportedInterfaces (GetDlgItem(ghWndTest, IDC_RECOGNIZED), lpResults);
#endif // _DEBUG
	}

	// Set the scroll position to the stop
	SendMessage (GetDlgItem(ghWndTest, IDC_RECOGNIZED), EM_SETSEL, 0, 0);
	SendMessage (GetDlgItem(ghWndTest, IDC_RECOGNIZED), EM_SCROLLCARET, 0, 0);


	return NOERROR;
}


STDMETHODIMP CTestGramNotify::PhraseHypothesis(DWORD dwFlags, QWORD qTimeStampBegin,
   QWORD qTimeStampEnd, PSRPHRASEW pSRPhrase, LPUNKNOWN lpResults)
{
// Ignore this
return NOERROR;
}

STDMETHODIMP CTestGramNotify::PhraseStart(QWORD qTimeStampBegin)
{

SetDlgItemText (ghWndTest, IDC_RECOGEDIT, "[Heard...]");
SetDlgItemText (ghWndTest, IDC_PARSE, "");
SetDlgItemText (ghWndTest, IDC_RECOGNIZED, "");
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

/*********************************************************
DisplayText - Displays text to the window
*/
void DisplayText (HWND hWnd, char *psz)
{
	SendMessage (hWnd, EM_SETSEL, (WPARAM) -1, -1);
	SendMessage (hWnd, EM_REPLACESEL, 0, (LPARAM) psz);
}

void DisplayText (HWND hWnd, WCHAR *pwsz)
{
   char  psz[10000];
	WideCharToMultiByte (CP_ACP,0, pwsz, -1, psz, sizeof(psz),
		NULL, NULL);
	SendMessage (hWnd, EM_SETSEL, (WPARAM) -1, -1);
	SendMessage (hWnd, EM_REPLACESEL, 0, (LPARAM) psz);
}

/**************************************************************
ShowSupportedInterfaces - Goes through all of the interfaces that
can be supported by the results object, and displays information about
them.
*/
void ShowSupportedInterfaces(HWND hWnd, LPUNKNOWN punkResult)
{
   PISRRESAUDIO   pISRResAudio = NULL;
   PISRRESAUDIOEX pISRResAudioEx = NULL;
   PISRRESBASICW  pISRResBasic = NULL;
   PISRRESCORRECTIONW pISRResCorrection = NULL;
   PISRRESEVAL    pISRResEval = NULL;
   PISRRESGRAPHW  pISRResGraph = NULL;
   PISRRESGRAPHEX pISRResGraphEx = NULL;
   PISRRESMEMORY  pISRResMemory = NULL;
   PISRRESMERGE   pISRResMerge = NULL;
   PISRRESMODIFYGUI  pISRResModifyGUI = NULL;
   PISRRESSCORES  pISRResScores = NULL;
   PISRRESSPEAKERW pISRResSpeaker = NULL;
   char  szTemp[256];
   WCHAR szwTemp[256];
   SDATA sd;

   DisplayText (hWnd, "\r\n=================================\r\n");

   punkResult->QueryInterface (IID_ISRResAudio, (PVOID*) &pISRResAudio);
   punkResult->QueryInterface (IID_ISRResAudioEx, (PVOID*) &pISRResAudioEx);
   punkResult->QueryInterface (IID_ISRResBasicW, (PVOID*) &pISRResBasic);
   punkResult->QueryInterface (IID_ISRResCorrectionW, (PVOID*) &pISRResCorrection);
   punkResult->QueryInterface (IID_ISRResEval, (PVOID*) &pISRResEval);
   punkResult->QueryInterface (IID_ISRResGraphW, (PVOID*) &pISRResGraph);
   punkResult->QueryInterface (IID_ISRResGraphEx, (PVOID*) &pISRResGraphEx);
   punkResult->QueryInterface (IID_ISRResMemory, (PVOID*) &pISRResMemory);
   punkResult->QueryInterface (IID_ISRResMerge, (PVOID*) &pISRResMerge);
   punkResult->QueryInterface (IID_ISRResModifyGUI, (PVOID*) &pISRResModifyGUI);
   punkResult->QueryInterface (IID_ISRResScores, (PVOID*) &pISRResScores);
   punkResult->QueryInterface (IID_ISRResSpeakerW, (PVOID*) &pISRResSpeaker);

   // ISRResAudio
   DisplayText (hWnd, "\r\n========================");
   if (pISRResAudio) {
      DisplayText (hWnd, "\r\nISRResAudio supported");

      sd.pData = NULL;
      pISRResAudio->GetWAV (&sd);
      if (sd.pData) {
         CoTaskMemFree (sd.pData);
         wsprintf (szTemp, " - %d bytes of data", (int) sd.dwSize);
         DisplayText (hWnd, szTemp);
      }
      else
         DisplayText (hWnd, " - Failed to get audio");
   }
   else {
      DisplayText (hWnd, "\r\nISRResAudio not supported");
   }


   // ISRResBasic
   QWORD qwStart, qwEnd;
   DisplayText (hWnd, "\r\n========================");
   if (pISRResBasic) {
      DisplayText (hWnd, "\r\nISRResBasic supported");

      // flags
      DWORD dwFlags;
      pISRResBasic->FlagsGet (0, &dwFlags);
      wsprintf (szTemp, "\r\nFlagsGet 0=%x", (int)dwFlags);
      DisplayText (hWnd, szTemp);

      // guid
      GUID  gID;
      pISRResBasic->Identify (&gID);
      DisplayText (hWnd, "\r\nIdentify=");
      StringFromGUID2(gID, szwTemp, sizeof(szwTemp));
      DisplayText (hWnd, szwTemp);

      // start/end time
      pISRResBasic->TimeGet (&qwStart, &qwEnd);
      sprintf (szTemp, "\r\nStart/end time= %I64x, %I64x", qwStart, qwEnd);
      DisplayText (hWnd, szTemp);
   }
   else {
      DisplayText (hWnd, "\r\nISRResBasic not supported");
   }


   // ISRResScore
   DisplayText (hWnd, "\r\n========================");
   if (pISRResScores) {
      DisplayText (hWnd, "\r\nISRResScores supported");

      DWORD i, j;
      for (i = 0; i < 5; i++) {
         long  lScore;
         if (pISRResScores->GetPhraseScore (i, &lScore))
            break;

         wsprintf (szTemp, "\r\nPhrase %d score = %d : ", (int) i, lScore);
         DisplayText (hWnd, szTemp);

         DWORD dwReq;
         long  alScore[100];
         dwReq = 0;
         pISRResScores->GetWordScores (i, alScore, sizeof(alScore), &dwReq);
         for (j = 0; j < dwReq / 4; j++) {
            wsprintf (szTemp, "%d ", alScore[j]);
            DisplayText (hWnd, szTemp);
         }
      }
   }
   else {
      DisplayText (hWnd, "\r\nISRResScores not supported");
   }

   // ISRResMemory
   DisplayText (hWnd, "\r\n========================");
   if (pISRResMemory) {
      DisplayText (hWnd, "\r\nISRResMemory supported");
      DWORD dwMem, dwKind;
      dwMem = dwKind = 0;
      pISRResMemory->Get (&dwKind, &dwMem);
      wsprintf (szTemp, " - Kind=%x, memory=%d", (int) dwKind, (int) dwMem);
      DisplayText (hWnd, szTemp);
   }
   else
      DisplayText (hWnd, "\r\nISRResMemory not supported");

   // ISRResGraphEx
   DisplayText (hWnd, "\r\n========================");
   if (pISRResGraphEx) {
      DisplayText (hWnd, "\r\nISRResGraphEx supported");

      DWORD dwBin1[1000];
      DWORD dwBin2[1000];
      DWORD dw;

      // start/end node
      pISRResGraphEx->NodeStartGet (&dw);
      wsprintf (szTemp, "\r\nStart node=%d", dw);
      DisplayText (hWnd, szTemp);
      pISRResGraphEx->NodeEndGet (&dw);
      wsprintf (szTemp, "\r\nEnd node=%d", dw);
      DisplayText (hWnd, szTemp);


      // get list of nodes & display incoming & outgoing arcs
      DWORD dwNum, dwNeeded;
      pISRResGraphEx->GetAllNodes (dwBin1, sizeof(dwBin1), &dwNum, &dwNeeded);
      if (dwNum > sizeof(dwBin1)/4)
         dwNum = sizeof(dwBin1)/4;
      DWORD dwNode, dwNodeIndex;
      for (dwNodeIndex = 0; dwNodeIndex < dwNum; dwNodeIndex++) {
         dwNode = dwBin1[dwNodeIndex];
         // display info for each node
         wsprintf (szTemp, "\r\nNode %d", dwNode);
         DisplayText (hWnd, szTemp);

         // time
         QWORD *pqw;
         sd.pData = NULL;
         pISRResGraphEx->DataGet (dwNode, SRGNODE_TIME, &sd);
         pqw = (QWORD*) sd.pData;
         if (pqw) {
            sprintf (szTemp, "\r\n\tTime=%I64x", *pqw);
            DisplayText (hWnd, szTemp);
            CoTaskMemFree (sd.pData);
         }

         // get all arcs
         DWORD dwOutgoing;
         for (dwOutgoing = 0; dwOutgoing <= 1; dwOutgoing++) {
            DWORD dwNumArc;
            pISRResGraphEx->ArcEnum (dwNode, dwBin2, sizeof(dwBin2), &dwNumArc, dwOutgoing);

            DWORD dwArcIndex, dwArc;
            for (dwArcIndex = 0; dwArcIndex < dwNumArc; dwArcIndex++) {
               dwArc = dwBin2[dwArcIndex];

               sprintf (szTemp,
                  dwOutgoing ? "\r\n\tOutgoing arc %d - " : "\r\n\tIncoming arc %d - ",
                  dwArc);
               DisplayText (hWnd, szTemp);

               // get the arc string
               sd.pData = NULL;
               pISRResGraphEx->DataGet (dwArc, SRGARC_WORDTEXT, &sd);
               if (sd.pData && ((WCHAR*)sd.pData)[0]) {
                  DisplayText (hWnd, (WCHAR*) sd.pData);
                  CoTaskMemFree (sd.pData);
               }
               else {
                  if (sd.pData) {
                     CoTaskMemFree (sd.pData);
                     sd.pData = NULL;
                  }

                  pISRResGraphEx->DataGet (dwArc, SRGARC_NONLEXICALTEXT, &sd);
                  if (sd.pData) {
                     DisplayText (hWnd, (WCHAR*) sd.pData);
                     CoTaskMemFree (sd.pData);
                  }
                  else
                     DisplayText (hWnd, "<no info>");
               }

               // display the score
               DWORD dwScore;
               dwScore = 0;
               pISRResGraphEx->DWORDGet (dwArc, SRGARC_ACOUSTICSCORE, &dwScore);
               wsprintf (szTemp, ", score=%x", (int) dwScore);
               DisplayText (hWnd, szTemp);

            }
         }
      }

   }
   else
      DisplayText (hWnd, "\r\nISRResGraphEx not supported");

   // ISRResGraph
   DisplayText (hWnd, "\r\n========================");
   if (pISRResGraph) {
      DisplayText (hWnd, "\r\nISRResGraph supported");

      SRRESWORDNODE  wn;
      BYTE           abTemp[256];
      SRWORDW*       pWord;
      DWORD          dwNeeded;

      // recusrively find all the nodes
      DWORD dwBin[1000];
      DWORD dwFillInto = 1;
      DWORD dwParseUpTo = 0;
      DWORD i, j;
      DWORD dwNode;
      dwBin[0] = 1;
      for (; dwParseUpTo < dwFillInto; dwParseUpTo++) {
         pWord = (SRWORDW*) &abTemp;
         dwNode = dwBin[dwParseUpTo];
         memset (&wn ,0, sizeof(wn));
         if (pISRResGraph->GetWordNode (dwNode, &wn, pWord, sizeof(abTemp), &dwNeeded))
            break;

         // display
         wsprintf (szTemp, "\r\nNode #%d = ", dwNode);
         DisplayText (hWnd, szTemp);
         DisplayText (hWnd, pWord->szWord);
         sprintf (szTemp, "(start/end=%I64x, %I64x, score=%d)",
            wn.qwStartTime, wn.qwEndTime, wn.dwWordScore);
         DisplayText (hWnd, szTemp);
         wsprintf (szTemp, "\r\n\tNext=%d, Prev=%d, Up=%d, Down=%d",
            wn.dwNextWordNode, wn.dwPreviousWordNode,
            wn.dwUpAlternateWordNode, wn.dwDownAlternateWordNode);
         DisplayText (hWnd, szTemp);

         // add to other nodes
         for (i = 0; i < 4; i++) {
            DWORD dwNew;
            switch (i) {
            case 0:
               dwNew = wn.dwNextWordNode;
               break;
            case 1:
               dwNew = wn.dwPreviousWordNode;
               break;
            case 2:
               dwNew = wn.dwUpAlternateWordNode;
               break;
            case 3:
               dwNew = wn.dwDownAlternateWordNode;
               break;
            }
            if (!dwNew)
               continue;

            for (j = 0; j < dwFillInto; j++)
               if (dwBin[j] == dwNew)
                  break;
            if (j < dwFillInto)
               continue;   // already there
            dwBin[dwFillInto++] = dwNew;
         }

      }



   }
   else
      DisplayText (hWnd, "\r\nISRResGraph not supported");


   DisplayText (hWnd, "\r\n========================");
   // ISRResAudioEx
   if (pISRResAudioEx)
      DisplayText (hWnd, "\r\nISRResAudioEx supported");
   else
      DisplayText (hWnd, "\r\nISRResAudioEx not supported");

   // ISRResCorrection
   if (pISRResCorrection)
      DisplayText (hWnd, "\r\nISRResCorrection supported");
   else
      DisplayText (hWnd, "\r\nISRResCorrection not supported");

   // ISRResEval
   if (pISRResEval)
      DisplayText (hWnd, "\r\nISRResEval supported");
   else
      DisplayText (hWnd, "\r\nISRResEval not supported");

   // ISRResMerge
   if (pISRResMerge)
      DisplayText (hWnd, "\r\nISRResMerge supported");
   else
      DisplayText (hWnd, "\r\nISRResMerge not supported");

   // ISRResModifyGUI
   if (pISRResModifyGUI)
      DisplayText (hWnd, "\r\nISRResModifyGUI supported");
   else
      DisplayText (hWnd, "\r\nISRResModifyGUI not supported");

   // ISRResSpeaker
   if (pISRResSpeaker)
      DisplayText (hWnd, "\r\nISRResSpeaker supported");
   else
      DisplayText (hWnd, "\r\nISRResSpeaker not supported");


   // release it all
#define  RELEASE(x)  if(x) (x)->Release()
   RELEASE(pISRResAudio);
   RELEASE(pISRResAudioEx);
   RELEASE(pISRResBasic);
   RELEASE(pISRResCorrection);
   RELEASE(pISRResEval);
   RELEASE(pISRResGraph);
   RELEASE(pISRResGraphEx);
   RELEASE(pISRResMemory);
   RELEASE(pISRResMerge);
   RELEASE(pISRResModifyGUI);
   RELEASE(pISRResScores);
   RELEASE(pISRResSpeaker);

}

/*******************************************************
GetIndexResult - Find the element of the global
results list with the given index
*/

void FillResListWindow(HWND hWndEdit, LPUNKNOWN punkResult)
{
	PISRRESBASICW pISRResBasic;
	PISRRESSCORES pISRResScores;
	DWORD        dwSizeNeeded;
	BYTE	     aBuf[1024];
	PSRPHRASEW    pSRPhrase = (PSRPHRASEW) aBuf;
	HRESULT      hRes;
	DWORD        dwRank = 0;
	CHAR		 szTemp[5120], szTTemp[512];
	CHAR		 szaTemp[256];
	long		 lPhraseScore;
	long		 lWordScores[50];
	DWORD		 dwNWordScores;
	UINT		 iWdCnt;
	PSRWORDW		 pSRWord, pSRMax;

	hRes = punkResult -> QueryInterface(IID_ISRResBasicW, (PVOID *) &pISRResBasic);

	if (FAILED(hRes))
		throw (HRESULT) hRes;

	hRes = punkResult -> QueryInterface(IID_ISRResScores, (PVOID *) &pISRResScores);

	if (FAILED(hRes) && (hRes != E_NOINTERFACE))
		throw (HRESULT) hRes;

	SendMessage(hWndEdit, WM_SETTEXT, 0, (LPARAM) "");

	for (;dwRank < 5;) {
	  // Determine how large of a buffer we need
		hRes = pISRResBasic->PhraseGet(dwRank, pSRPhrase, sizeof(aBuf), &dwSizeNeeded);
		if (hRes == SRERR_VALUEOUTOFRANGE) {
			// Rank ID too high
			break;
		}

		if (hRes) {
			break;
		}

		if (NULL != pISRResScores) {
			hRes = pISRResScores -> GetPhraseScore(dwRank, &lPhraseScore);

			if (FAILED(hRes)) {
				break;
			}

			hRes = pISRResScores -> GetWordScores(dwRank, &lWordScores[0], 50 * sizeof(long), &dwNWordScores);

			if (FAILED(hRes)) {
				break;
			}
		}

		// write out a string with the results
		// loop through all of the words and display them
		pSRMax = (PSRWORDW) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
		pSRWord = (PSRWORDW) (pSRPhrase->abWords);


		if (dwRank)
			_stprintf (szTemp, "Alternative %d\r\n", dwRank);
		else
			_stprintf (szTemp, "Top alternative\r\n");

		if (NULL != pISRResScores) {
			sprintf(szTTemp, "Score = %d\r\n", lPhraseScore);
			strcat (szTemp, szTTemp);
		};

		if (pSRMax == pSRWord) {
			strcpy(szTemp, "<Empty result>\r\n");
		} else {
			iWdCnt = 0;

			strcat(szTemp, "Words: ");
			while (pSRWord < pSRMax)
			{
				// add word
				WideCharToMultiByte (CP_ACP,0, pSRWord->szWord, -1, szaTemp, sizeof(szaTemp),
					NULL, NULL);

				strcat (szTemp, szaTemp);

#if SHOWWORDSCORES
				if (NULL != pISRResScores) {
					sprintf(szTTemp, "(%d)", lWordScores[iWdCnt++]);
					strcat(szTemp, szTTemp);
				}
#endif // SHOWWORDSCORES
				strcat (szTemp, " ");

				pSRWord = (PSRWORDW) ((BYTE*) pSRWord + pSRWord->dwSize);
			}

			strcat (szTemp, "\r\n");
			strcat (szTemp, "Parse: ");
			ParseIt (pSRPhrase, 0, szTemp + strlen(szTemp));
			strcat (szTemp, "\r\n");

         // parse all the alternates
         DWORD dwAlt;
         for (dwAlt = 1; ; dwAlt++) {
            hRes = ParseIt (NULL, dwAlt, szTemp + strlen(szTemp));
			   strcat (szTemp, "\r\n");
            if (hRes)
               break;
         }
		}

      DisplayText (hWndEdit, szTemp);
      DisplayText (hWndEdit, "\r\n==================================\r\n");

		++dwRank;
	}

	pISRResBasic -> Release();
	if (NULL != pISRResScores)
		pISRResScores -> Release();


	return;
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

      // activate the grammar for this window
      gpISRGramCommon->Activate(NULL /*hWnd*/, FALSE, NULL);

      break;   // continue on

   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
         case IDCANCEL:
            // force a stop
            ghWndTest = NULL;
            gpISRGramCommon->Deactivate(NULL);

            // archive if possible
            if (gpISRGramCommon) {
               HRESULT  hRes;
               DWORD dwData;
               hRes = gpISRGramCommon->Archive (FALSE, NULL, 0, &dwData);
               if (hRes == ResultFromScode(SRERR_NOTENOUGHDATA)) {
                  // data
                  PVOID pMem;
                  pMem = malloc(dwData);
                  hRes = gpISRGramCommon->Archive(FALSE, pMem, dwData, &dwData);
                  if (!hRes)
                     gpISTGramComp->GrammarDataSet(pMem, dwData);
                  free (pMem);
               }
            }

            EndDialog (hWnd, IDCANCEL);
            return TRUE;

		 default:
			 return FALSE;
         };
      break;
   };

return FALSE;  // didn't handle
}


/*******************************************************
_InfoDlgProc - Dialog procedure for getting the testing
   information.

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK _InfoDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

switch (uMsg) {
   case WM_INITDIALOG:
      // Fill in list-box with SR engine modes

      {
	  // Fill in only with engines that can handle
	  // the grammar
	  DWORD dwBitField = 0xffffffff;
	  SRGRMFMT fmt = SRGRMFMT_CFG;
	  gpISTGramComp->GrammarFormatGet (&fmt);
	  switch (fmt) {
	  case SRGRMFMT_CFG:
		 dwBitField = SRGRAM_CFG;
		 break;
	  case SRGRMFMT_LIMITEDDOMAIN:
		 dwBitField = SRGRAM_LIMITEDDOMAIN;
		 break;
	  case SRGRMFMT_DICTATION:
		 dwBitField = SRGRAM_DICTATION;
		 break;
	  }

      SRMODEINFO     SRInfo;
      gpISREnum->Reset();
      while (!gpISREnum->Next(1, &SRInfo, NULL))
		 if (SRInfo.dwGrammars & dwBitField)
			SendDlgItemMessage (hWnd, IDC_SRENGINE, CB_ADDSTRING,
               0, (LPARAM) SRInfo.szModeName);
      }
	  SendDlgItemMessage (hWnd, IDC_SRENGINE, CB_SETCURSEL, 0, 0);

      break;   // continue on

   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
            #define  GETAD(dwID,szDest)   GetDlgItemText(hWnd,dwID,szDest,sizeof(szDest))
            #define  GETINT(dwID,szDest)   szDest = GetDlgItemInt(hWnd,dwID,NULL,FALSE)
            GETAD(IDC_SRENGINE, gszSREngine);
            GETAD(IDC_SRENGINE, gszUserName);

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
   _TCHAR         szMode - Mode name
   PISRFINDW      pISRFind - Find interface
   PIUNKNOWN      pIUnknown - IUnknown for the audio source (this will
      be addrefed and passed down to the engine)
   PISRCENTRALW   *pISRCentral - Filled in with the ISRCentral interface
   PISRATTRIBUTES *gpISRAttributes - Filled in with the Attributes interface
returns
   HRESULT - error
*/
HRESULT SelectSREngine (_TCHAR *szMode, PISRFIND pISRFind,
   LPUNKNOWN pIUnknown, PISRCENTRAL *pISRCentral, PISRATTRIBUTES *ppISRAttributes)
{
SRMODEINFO SRInfo, SRDest;
HRESULT     hRes;

memset (&SRInfo, 0 , sizeof(SRInfo));
_tcscpy (SRInfo.szModeName, szMode);

hRes = pISRFind->Find(&SRInfo, NULL, &SRDest);
if (hRes) return hRes;

hRes = pISRFind->Select(SRDest.gModeID, pISRCentral, pIUnknown);
if (hRes) return hRes;

hRes = (*pISRCentral)->QueryInterface (IID_ISRAttributes, (void**) ppISRAttributes);
return hRes;
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
HRESULT GetTestingInfo (void)
{
HRESULT hRes;
LPUNKNOWN pIAMM = NULL;

// get the enumerators
hRes = CoCreateInstance (CLSID_SREnumerator, NULL, CLSCTX_ALL, IID_ISREnum, (void**)&gpISREnum);
if (hRes) return hRes;
hRes = gpISREnum->QueryInterface(IID_ISRFind, (void**)&gpISRFind);
if (hRes) return hRes;

hRes = CoCreateInstance(CLSID_MMAudioSource, NULL, CLSCTX_ALL,
       IID_IUnknown,(void**)&pIAMM);
if (hRes) return hRes;

#ifdef USEAUDIOTEL
// use the logging audio telephony object
PIAUDIOTEL  pIAudioTel;
hRes = CoCreateInstance(CLSID_AudioSourceTel, NULL, CLSCTX_ALL,
       IID_IAudioTel,(void**)&pIAudioTel);
if (!hRes) {
   SDATA d;
   WAVEFORMATEX   wfex;
   d.pData = &wfex;
   d.dwSize = sizeof(wfex);
   memset (&wfex, 0, sizeof(wfex));
   wfex.wFormatTag = WAVE_FORMAT_PCM;
   wfex.nChannels = 1;
   wfex.nSamplesPerSec =  16000;
   wfex.nAvgBytesPerSec = 32000;
   wfex.nBlockAlign = 2;
   wfex.wBitsPerSample = 16;

   pIAudioTel->WaveFormatSet (d);
   pIAudioTel->AudioObject (pIAMM);
   pIAMM->Release();
   pIAMM = pIAudioTel;
}
#endif

#ifdef USELOGGING
// use the logging audio source
PIAUDIOSOURCELOG  pIAudioSourceLog;
hRes = CoCreateInstance(CLSID_AudioSourceLog, NULL, CLSCTX_ALL,
       IID_IAudioSourceLog,(void**)&pIAudioSourceLog);
if (!hRes) {
   pIAudioSourceLog->UseLogging (gpISTLog);
   pIAudioSourceLog->AudioSource (pIAMM);
   pIAMM->Release();
   pIAMM = pIAudioSourceLog;
}
#endif

// ask the user
if (IDOK != DialogBox (ghInstance, MAKEINTRESOURCE(IDD_GRAMMARFILE),
   ghWndEditor, (DLGPROC) _InfoDlgProc)) {
      // shut down
      return ResultFromScode (E_FAIL);
      };

// get the engine
hRes = SelectSREngine (gszSREngine, gpISRFind, pIAMM, &gpISRCentral, &gpISRAttributes);
pIAMM->Release(); // since engine should have addrefed
if (hRes) return hRes;

// register the notification sink. We don't need to unregister because
// it'll be freed when release engine
DWORD dwKey;
gpISRCentral->Register (&gTestNotify, IID_ISRNotifySinkW, &dwKey);

if (gszUserName[0])
	gpISRAttributes->SpeakerSet (gszUserName);

// load the grammar
// use the grammar compiler's grammr load to reduce code. This also does
// the default lists
LPUNKNOWN   lpUnk = NULL;
hRes = gpISTGramComp->GrammarLoad (gpISRCentral, (PVOID) &gTestGramNotify, IID_ISRGramNotifySinkW, &lpUnk);
if (hRes)
   return hRes;

hRes = lpUnk->QueryInterface (IID_ISRGramCommon, (void**)&gpISRGramCommon);
lpUnk->Release();
if (hRes) return hRes;

// try for a CFG and set the lists by defaul
return NOERROR;
}



/*******************************************************
Test - Do the test

inputs
returns
   HRESULT - error
*/
HRESULT Test (void)
{
	HRESULT           hRes;
   DWORD dwRet;

	hRes = GetTestingInfo();
	if (hRes)
	   return hRes;

	// pull up test dialog
	DialogBox (ghInstance, MAKEINTRESOURCE(IDD_RESULTSDIALOG),
	   ghWndEditor, (DLGPROC) _TestDlgProc);

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
	   dwRet = gpISRGramCommon->Release();
	}
	return NOERROR;
}








