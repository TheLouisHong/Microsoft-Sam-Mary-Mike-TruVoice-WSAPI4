/************************************************************************
FakeSRGr.Cpp - Fake SR grammar for testing purposes.
   Includes the interfaces:
      ISRGrammar
      IIdentity
      IUnknown

Copyright (c) 1994-1998 by Microsoft Corporation

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
#include <objerror.h>
#include <speech.h>
#include "fakeSR.h"

/*************************************************************************
GrmWindowProc - Window which is displayed for a grammar.
*/
LRESULT CALLBACK GrmWindowProc (HWND hWnd, UINT uMsg,
   WPARAM wParam, LPARAM lParam)
{
PCGrm   pGrm = (PCGrm)GetWindowLong(hWnd, 0);

switch (uMsg) {
   case WM_CLOSE:
      return 0;   // dont close

   case WM_CREATE:
      {
      RECT  rect;

      pGrm = (PCGrm) ((CREATESTRUCT *) lParam)->lpCreateParams;
      SetWindowLong (hWnd, 0, (DWORD) pGrm);

      GetClientRect (hWnd, &rect);
      pGrm->hWndEdit = CreateWindow (
         "EDIT",
         "",
         WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | WS_VISIBLE | WS_CHILD,
         rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
         hWnd, NULL, ghInstance, NULL
         );
      }
      return 0;

   case WM_SIZE:
      {
      // resize the edit control
      RECT  rect;
      GetClientRect (hWnd, &rect);
      MoveWindow (pGrm->hWndEdit, 0, 0, rect.right, rect.bottom, TRUE);
      }
      return 0;

   };

return DefWindowProc (hWnd, uMsg, wParam, lParam);
}


/************************************************************************
DisectStandardFormat - Disect the standard grammar format (for finite
   state grammars). For the demo, this merely extracts the strings and
   stores them in a list object. This list is searched through for a match
   later on.

inputs
   CHAR *   pData - Data.
	DWORD		dwSize - # of bytes
returns
   PCList - List object
*/
PCList DisectStandardFormat (CHAR * pData, DWORD dwSize)
{
PCList   pList;
PSRWORDW  pSRWord;
CGramFormatBrowse 	browse;
BYTE		*pWords;

// Initialize the browse object
if (!browse.Init ((PVOID) pData, dwSize))
	return NULL;

// Make sure it's the right format
if ((browse.GetGrammarFormat() != SRHDRTYPE_CFG) ||
	!(browse.GetGrammarFlags() & SRHDRFLAG_UNICODE) )
		return NULL;	// wrong format

// make a new list
pList = new CList();
if (!pList)
   return NULL;

// start disecting.
pWords = (BYTE*) browse.FindChunk (SRCKCFG_WORDS, &dwSize);
if (!pWords) {
	// Cant find it
	delete pList;
	return NULL;
	};

// while we have space left, start extarcting SRWORD structures
while (dwSize) {
   pSRWord = (PSRWORDW) pWords;
   dwSize -= pSRWord->dwSize;
   pWords += pSRWord->dwSize;

   // Store this away
   pList->AddElem (pSRWord, pSRWord->dwSize);
   };

return pList;
}


/************************************************************************
CGrm - Grammar class. The same code handles all modes (French & English)
*/

CGrm::CGrm (LPUNKNOWN punkOuter, LPFNDESTROYED pfnDestroy,
   VOID * pCMode)
{
m_cRef=0;
m_punkOuter = punkOuter;
m_pfnDestroy = pfnDestroy;
m_pGrmISRGramCommonW = NULL;
m_pGrmISRGramCFGW = NULL;
m_pCMode = pCMode;
m_bActive = FALSE;
m_pList = NULL;
hWndMain = hWndEdit = NULL;
m_dwGramActive = 0;
m_NotifyW = NULL;
m_fHooked = FALSE;
}


CGrm::~CGrm (void)
{
// Free the window
if (NULL != hWndMain)
   DestroyWindow (hWndMain);

// Deactivate if we're active
if (m_bActive)
   m_pGrmISRGramCommonW->Deactivate (NULL);

// Free the contained interfaces
if (m_NotifyW)
	m_NotifyW->Release();
if (NULL != m_pGrmISRGramCommonW)
   delete m_pGrmISRGramCommonW;
if (NULL != m_pGrmISRGramCFGW)
   delete m_pGrmISRGramCFGW;

// Tell the parent that the grammar is being released
if (NULL != m_pCMode)
   ((PCMode) m_pCMode)->GrammarRelease(this);

// Free the memory
if (NULL != m_pList)
   delete m_pList;

// Release the ISRCentralW interface that hooked when created
// the object
if (m_fHooked)
	((PCMode)m_pCMode)->m_pModeISRCentralW->Release();
}

BOOL CGrm::FInit (SRGRMFMT eFormat, SDATA dData,
   PISRGRAMNOTIFYSINKW pNSW)
{
LPUNKNOWN      pIUnknown = (LPUNKNOWN) this;
WNDCLASS wc;

if (eFormat != SRGRMFMT_CFG)
   return FALSE;  // Can only handle CFG formats

m_NotifyW = pNSW;

if (NULL != m_punkOuter)
   pIUnknown = m_punkOuter;

// Allocate all of the contained interfaces
m_pGrmISRGramCommonW = new CGrmISRGramCommonW (this, pIUnknown);
m_pGrmISRGramCFGW = new CGrmISRGramCFGW (this, pIUnknown);

// Extract the gramar
m_pList = DisectStandardFormat ((char*)dData.pData, dData.dwSize);

// create a window to represent the grammar.
// Real engines don't do this, but its grreat for testing
/* create the class */
memset (&wc, 0, sizeof(wc));
wc.style = CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc = GrmWindowProc;
wc.hInstance = ghInstance;
wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
wc.hCursor = LoadCursor (NULL, MAKEINTRESOURCE(IDC_ARROW));
wc.hbrBackground = (HBRUSH) (COLOR_BACKGROUND+1);
wc.lpszMenuName = NULL;
wc.lpszClassName = "SRGrmClass";
wc.cbWndExtra = sizeof(long);
RegisterClass (&wc);

/* create the window */
hWndMain = CreateWindow (
   wc.lpszClassName,
   "Grammar Object",
   WS_OVERLAPPEDWINDOW,
   CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
   NULL, NULL, ghInstance, (VOID*) this
   );
ShowWindow (hWndMain, SW_MINIMIZE | SW_SHOW);

// Do an addref on the central interface so that the speech-recognition
// object won't shut down until all of the grammars have shut down.
//	(Although the app may think that it's shut down.) Keep a flag
// indicating that we've done an addref.
((PCMode)m_pCMode)->m_pModeISRCentralW->AddRef();
m_fHooked = TRUE;

return (m_pGrmISRGramCommonW && m_pGrmISRGramCFGW && m_pList);
}


STDMETHODIMP CGrm::QueryInterface (REFIID riid, LPVOID *ppv)
{
*ppv = NULL;

/* always return our IUnkown for IID_IUnknown */
if (IsEqualIID (riid, IID_IUnknown))
   *ppv = (LPVOID) this;

// other interfaces
if (IsEqualIID(riid, IID_ISRGramCommonW))
   *ppv = m_pGrmISRGramCommonW;
if (IsEqualIID(riid, IID_ISRGramCFGW))
   *ppv = m_pGrmISRGramCFGW;

// update the reference count
if (NULL != *ppv) {
   ((LPUNKNOWN) *ppv) ->AddRef();
   return NOERROR;
   };

return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CGrm::AddRef (void)
{
return ++m_cRef;
}

STDMETHODIMP_(ULONG) CGrm::Release (void)
{
ULONG    cRefT;

cRefT = --m_cRef;

if (0 == m_cRef) {
   // inform destroy that object is going away
   if (NULL != m_pfnDestroy)
      (*m_pfnDestroy)();

   delete this;
   };

return cRefT;
};

/* RecognizeFromString - This does recognition from an ANSI string.
   It identifies unique words (no punctuation), finds a match in the
   grammar, and then sends notification to the application */
CGrm::RecognizeFromString (PSTR pStr)
{
PSRPHRASEW      pSRPhrase;
char           *pStart, *pCur;
PSRWORDW        pSRWord;    // current position of writing out word
PSRWORDW        pSRRef;     // reference word
char           cTemp;
char           szTemp[64], szCompare[64];
DWORD          dwNumElems = m_pList->GetNumElems();
DWORD          i;
SDATA           dData = {NULL,0};

// Notify that we're starting to process
if (m_NotifyW)
	m_NotifyW->PhraseStart (m_qwBegin);

// Allocate enough memory for a phrase. Just allocate a lot
// because I'm too lazy to do otherwise
pSRPhrase = (PSRPHRASEW) malloc (1024);
if (!pSRPhrase)
   return FALSE;  //error

//find a whole word in the string, and add it to the phrase
pSRWord = (PSRWORDW) pSRPhrase->abWords;
while (*pStr) {
   // loop until get to the beginning of a word
   for (pStart = pStr; !IsCharAlpha(*pStart) && *pStart; pStart++);
   if (!(*pStart))
      break;   // end of line

   // loop until the end of the word is found
   for (pCur = pStart; IsCharAlpha(*pCur) && *pCur; pCur++);
   cTemp = *pCur;
   *pCur = '\0';
   strcpy (szCompare, pStart);
   *pCur = cTemp;

   // Loop through the linked list and see if one matches
   // NTOE: This is very very inefficient
   for (i = 0; i < dwNumElems; i++) {
      // get the unicode version, convert it to ansi, etc.
      pSRRef = (PSRWORDW) m_pList->GetElem(i);
      WideCharToMultiByte (CP_ACP, 0, pSRRef->szWord, -1,
         szTemp, sizeof(szTemp), NULL, NULL);
      if (!stricmp(szTemp, szCompare))
         break;   // we found it
      };
   if (i >= dwNumElems) {
      // we couldn't find the word. We have a misrecognition.
      m_NotifyW->PhraseFinish (0, m_qwBegin, m_qwEnd, NULL, NULL);
      free ((void*) pSRPhrase);
      return FALSE;
      };
   // else, we recognize it, to add it to the list
   memcpy (pSRWord, pSRRef, pSRRef->dwSize);
   pSRWord = (PSRWORDW) (((char *) pSRWord) + pSRRef->dwSize);

   // update the new position
   pStr = pCur;
   };

// fix the size of the SR phrase
pSRPhrase->dwSize = (DWORD) ((BYTE *) pSRWord - (BYTE*)pSRPhrase);

// We recognized something. Send this up to the application
m_NotifyW->PhraseFinish (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR,
	m_qwBegin, m_qwEnd, pSRPhrase, NULL);

// Free the memory
free ((void*) pSRPhrase);

return TRUE;
}

void CGrm::Utterance (QWORD qwBegin, QWORD qwEnd)
{
char  szString[128];
int   iLen;

// This should really be activating/deactivating grammars based upon
//	what window has focus. However, that is a lot of code and this is
// just some demo code so that engine vendors can understand the basic
// process.

// If we're disabled then don't allow any typing
if (!m_bActive) {
   return;
   };

iLen = GetWindowTextLength(hWndEdit);
if (!iLen)
   return;  // nothing typed in

if (iLen >= (sizeof(szString) - 1)) {
   // error, so blank out the window
   SetWindowText (hWndEdit, "");
   return;
   };
GetWindowText (hWndEdit, szString, sizeof(szString));

// elsem it's a full string, so recognize from it
m_qwBegin = qwBegin;
m_qwEnd = qwEnd;
this->RecognizeFromString (szString);
SetWindowText (hWndEdit, "");  // Blank it out
}


/************************************************************************
CGrmISRGramCommonW - ISRGrammar inteerface
*/
CGrmISRGramCommonW::CGrmISRGramCommonW (LPVOID pObj, LPUNKNOWN punkOuter)
{
m_cRef = 0;
m_pObj = pObj;
m_punkOuter = punkOuter;
}

CGrmISRGramCommonW::~CGrmISRGramCommonW (void)
{
// intentionally left blank
}

STDMETHODIMP CGrmISRGramCommonW::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
return m_punkOuter->QueryInterface(riid,ppv);
}

STDMETHODIMP_ (ULONG) CGrmISRGramCommonW::AddRef(void)
{
++m_cRef;
return m_punkOuter->AddRef();
}

STDMETHODIMP_ (ULONG) CGrmISRGramCommonW::Release(void)
{
--m_cRef;
return m_punkOuter->Release();
}

STDMETHODIMP CGrmISRGramCommonW::Activate (HWND hWndActivate, BOOL fAutoPause,
	PCWSTR szRule)
{
PCGrm pGrm = (PCGrm) m_pObj;
PCMode pMode = (PCMode) pGrm->m_pCMode;

// This function should really maintain a list of what rules are active,
// and when they're active for what windows. However, that's a lot of code
// that would cloud up and confuse the demo code. So, we're ignoring both
//	hwndActivate and szRule.

// Aslo, the fAutoPause would automatically pause the recognition from
// occuring after this utterance. But again, it would only make the demo
// less readable.

if (pGrm->m_bActive)
   return NOERROR;   // already active

pGrm->m_bActive = TRUE;
pGrm->m_dwGramActive++;

// Start recording
(pMode->m_pAudioIAudio)->Start();

return NOERROR;
}

STDMETHODIMP CGrmISRGramCommonW::Archive (BOOL fWhat, PVOID pMem,
	DWORD dwMemSize, DWORD * pdwMemNeeded)
{
return ResultFromScode (SRERR_NOTSUPPORTED);
}

STDMETHODIMP CGrmISRGramCommonW::BookMark (QWORD qwTime, DWORD dwValue)
{
// Bookmarks are not supported in the demo because they would introduce
// too much complexity.
return ResultFromScode (SRERR_NOTSUPPORTED);
}

STDMETHODIMP CGrmISRGramCommonW::Deactivate (PCWSTR szRule)
{
PCGrm pGrm = (PCGrm) m_pObj;
PCMode pMode = (PCMode) pGrm->m_pCMode;

// szRule is ignored since activate ignores them.

if (!pGrm->m_bActive)
   return NOERROR;   // already active

pGrm->m_bActive = FALSE;
pGrm->m_dwGramActive--;

// Stop recording if we're the last grammar active
if (!pGrm->m_dwGramActive) {
   (pMode->m_pAudioIAudio)->Stop();
   (pMode->m_pAudioIAudio)->Flush();
   };

return NOERROR;
}

STDMETHODIMP CGrmISRGramCommonW::DeteriorationGet (DWORD * pdwDet1,
	DWORD *pdwDet2, DWORD * pdwDet3)
{
// Deterioration is not supported in the demo because results objects
// are not supported.
return ResultFromScode (SRERR_NOTSUPPORTED);
}

STDMETHODIMP CGrmISRGramCommonW::DeteriorationSet (DWORD dwDet1,
	DWORD dwDet2, DWORD dwDet3)
{
// Deterioration is not supported in the demo because results objects
// are not supported.
return ResultFromScode (SRERR_NOTSUPPORTED);
}


STDMETHODIMP CGrmISRGramCommonW::TrainDlg (HWND hWnd, PCWSTR pszTitle)
{
return ResultFromScode (SRERR_NOTSUPPORTED);
}

STDMETHODIMP CGrmISRGramCommonW::TrainPhrase (DWORD dwExtent, PSDATA pData)
{
return ResultFromScode (SRERR_NOTSUPPORTED);
}


STDMETHODIMP CGrmISRGramCommonW::TrainQuery (DWORD * pdwFlags)
{
if (!pdwFlags)
	return ResultFromScode (SRERR_INVALIDPARAM);
*pdwFlags = 0;	// Just to retrun a value
return NOERROR;
}



/************************************************************************
CGrmISRGramCFGW - ISRGramCFGW inteerface
*/
CGrmISRGramCFGW::CGrmISRGramCFGW (LPVOID pObj, LPUNKNOWN punkOuter)
{
m_cRef = 0;
m_pObj = pObj;
m_punkOuter = punkOuter;
}

CGrmISRGramCFGW::~CGrmISRGramCFGW (void)
{
// intentionally left blank
}

STDMETHODIMP CGrmISRGramCFGW::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
return m_punkOuter->QueryInterface(riid,ppv);
}

STDMETHODIMP_ (ULONG) CGrmISRGramCFGW::AddRef(void)
{
++m_cRef;
return m_punkOuter->AddRef();
}

STDMETHODIMP_ (ULONG) CGrmISRGramCFGW::Release(void)
{
--m_cRef;
return m_punkOuter->Release();
}


STDMETHODIMP CGrmISRGramCFGW::LinkQuery (PCWSTR szLink, BOOL * fUsed)
{
// Links and lists are not supported in the demo.
return ResultFromScode (SRERR_NOTSUPPORTED);
}

STDMETHODIMP CGrmISRGramCFGW::ListAppend (PCWSTR szList, SDATA sData)
{
// Links and lists are not supported in the demo.
return ResultFromScode (SRERR_NOTSUPPORTED);
}

STDMETHODIMP CGrmISRGramCFGW::ListGet (PCWSTR szList, PSDATA sData)
{
// Links and lists are not supported in the demo.
return ResultFromScode (SRERR_NOTSUPPORTED);
}

STDMETHODIMP CGrmISRGramCFGW::ListRemove (PCWSTR szList, SDATA sData)
{
// Links and lists are not supported in the demo.
return ResultFromScode (SRERR_NOTSUPPORTED);
}

STDMETHODIMP CGrmISRGramCFGW::ListSet (PCWSTR szList, SDATA sData)
{
// Links and lists are not supported in the demo.
return ResultFromScode (SRERR_NOTSUPPORTED);
}

STDMETHODIMP CGrmISRGramCFGW::ListQuery (PCWSTR szList, BOOL * fUsed)
{
// Links and lists are not supported in the demo.
return ResultFromScode (SRERR_NOTSUPPORTED);
}






/***************************************************************
CGramFormatBrowse - This object allows an application to easily
	browse through the chunks in a grammar format.

	::~CGramForamtBrowse - This frees up all allocated memory.

	::Init - The Init takes a PVOID to the beginning
			of the chunk data, and a size. It allocates enough memory to
			store the data, copies it all over. Returns TRUE if success.

	::FindChunk - Searches for the given chunk ID. If it can't find
			it then the function returns 0. Otherwise, it returns a
			pointer to the memory of the chunk & the number of bytes
			in it.

	::GetGrammarFormat - This returns the grammar format tag in the
			header.

	::GetGrammarFlags - This returns the flags in the header
*/

CGramFormatBrowse::CGramFormatBrowse (void)
{
m_pMem = NULL;
m_dwMemSize= NULL;
}

CGramFormatBrowse::~CGramFormatBrowse (void)
{
if (m_pMem)
	free (m_pMem);
}

BOOL CGramFormatBrowse::Init (PVOID pMem, DWORD dwSize)
{
if (!pMem || !dwSize)
	return FALSE;

if (m_pMem)
	free (m_pMem);

m_pMem = malloc (dwSize);
m_dwMemSize = dwSize;
if (!m_pMem)
	return FALSE;

memcpy (m_pMem, pMem, dwSize);

return TRUE;
}

PVOID CGramFormatBrowse::FindChunk (DWORD dwChunkID, DWORD *pdwSize)
{
DWORD	dwPosn;
PBYTE	pB;
PSRCHUNK	psrc;

if (!m_pMem || (m_dwMemSize < sizeof(SRHEADER)))
	return NULL;

// start at the beginning
dwPosn = sizeof(SRHEADER);
pB = (PBYTE) m_pMem + dwPosn;

while (dwPosn < m_dwMemSize + sizeof(SRCHUNK)) {
	psrc = (PSRCHUNK) pB;

	// See if this is the chunk we're looking for
	if (psrc->dwChunkID == dwChunkID) {
		// yup
		*pdwSize = psrc->dwChunkSize;
		return (PVOID) (psrc + 1);
		};

	// If not, Move the curent pointer onto the next one
	dwPosn += sizeof(SRCHUNK) + ((psrc->dwChunkSize + 3) & 0xfffffffc);
		// DWORD align
	pB = (PBYTE) m_pMem + dwPosn;

	};

return NULL;
}

DWORD CGramFormatBrowse::GetGrammarFormat (void)
{
if (!m_pMem || (m_dwMemSize < sizeof(SRHEADER)))
	return 0;	//error
return ((PSRHEADER) m_pMem)->dwType;
}

DWORD CGramFormatBrowse::GetGrammarFlags (void)
{
if (!m_pMem || (m_dwMemSize < sizeof(SRHEADER)))
	return 0;	//error
return ((PSRHEADER) m_pMem)->dwFlags;
}




