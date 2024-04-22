/************************************************************************
FakeSR.Cpp - Fake SR mode (main part) for testing purposes.
   Includes the interfaces:
      ISRAttributes
      ISRCentral
      ISRDialogs
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
#include <tchar.h>
#include <speech.h>
#include "fakeSR.h"
#include "fakeSREn.h"

#define  SRERR_STRTRUNCATED   SRERROR(27)

#define   WM_NSATTRIBCHANGED      (WM_USER + 106)
#define   WM_NSINTERFERENCE         (WM_USER + 107)
#define   WM_NSSOUND               (WM_USER + 108)
#define   WM_NSUTTERANCEBEGIN      (WM_USER + 109)
#define   WM_NSUTTERANCEEND         (WM_USER + 110)
#define   WM_NSVUMETER            (WM_USER + 111)

typedef struct {
   PISRGRAMNOTIFYSINKW   pSinkW;
   // NOTE: Have a pSinkA here also, mutually exclusive of pSinkW
   DWORD                  dwNotifiedStart;
   } BUFLIST, *PBUFLIST;

int  MyAStrLen (PCSTR pA);
void NotificationSend (PCList pList, DWORD dwMessage, DWORD dwParam1,
                       DWORD dwParam2, PVOID pVoid);


/************************************************************************
MyAStrLen - ANSI string length.

inputs
   PSTR    pA - A string
returns
   int - length of string (excluding null terminator)
*/

int MyAStrLen (PCSTR pA)
{
    int cnt=0;

    while (*pA) {
        pA++;
        cnt++;
    }

    return cnt;
}


/*************************************************************************
ModeWindowProc - Window which is displayed for a SR mode.
*/
LRESULT CALLBACK ModeWindowProc (HWND hWnd, UINT uMsg,
   WPARAM wParam, LPARAM lParam)
{
PCMode   pMode = (PCMode)GetWindowLong(hWnd, 0);

switch (uMsg) {
   case WM_CLOSE:
      return 0;   // dont close

   case WM_CREATE:
      {
      RECT  rect;

      pMode = (PCMode) ((CREATESTRUCT *) lParam)->lpCreateParams;
      SetWindowLong (hWnd, 0, (DWORD) pMode);

      GetClientRect (hWnd, &rect);
      pMode->hWndEdit = CreateWindow (
         "EDIT",
         "SR results\n",
         WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_READONLY | WS_VISIBLE | WS_CHILD,
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
      MoveWindow (pMode->hWndEdit, 0, 0, rect.right, rect.bottom, TRUE);
      }
      return 0;

   case   WM_NSATTRIBCHANGED:
   case   WM_NSINTERFERENCE:
   case   WM_NSSOUND:
   case   WM_NSUTTERANCEBEGIN:
   case   WM_NSUTTERANCEEND:
   case   WM_NSVUMETER:
         NotificationSend (pMode->m_pNotifyList, uMsg, wParam, lParam, NULL);
         return 0;
   }

return DefWindowProc (hWnd, uMsg, wParam, lParam);
}


typedef struct {
   PVOID         pInterface;
   IID         iid;
   DWORD         dwKey;
   } NOTIFY, *PNOTIFY;

/*************************************************************************
NotificationRegister - This function adds an application to the list of
   notifcations-objects which are not be notified.

inputs
   PCList   pList - List object that contains the notifications
   PVOID      pInterface - Pointer to the interface
   IID      iid - Interface #
returns
   DWORD - Key #, or 0 if error
*/
DWORD NotificationRegister (PCList pList, PVOID pInterface, IID iid)
{
static    DWORD dwKeyCount = 1;   // key-count
NOTIFY   n;

// fill out the structuire
n.pInterface = pInterface;
n.iid = iid;
n.dwKey = dwKeyCount++;

// Add it to the list
if (!pList->AddElem (&n, sizeof(n)))
   return 0;
else
   return n.dwKey;
}



/***************************************************************************8
NotificationUnRegister - This unregisters a registered interface.

inputs
   PCList   pList - List object that contains the notifications
   DWORD      dwKey - Key value to unregister
returns
   BOOL - TTRUE if found it, FALSE if didn't
*/
BOOL NotificationUnRegister (PCList pList, DWORD dwKey)
{
DWORD   i, dwNumElems;
PNOTIFY   pn;

dwNumElems = pList->GetNumElems();
for (i = 0; i < dwNumElems; i++) {
   pn = (PNOTIFY) pList->GetElem(i);
   if (!pn)
      continue;   // error
   if (pn->dwKey == dwKey) {
      pList->RemoveElem (i);
      return TRUE;
      };
   };

// else cant find
return FALSE;
}

/***************************************************************************8
NotificationUnRegisterAll - This unregisters a all registered intefcaes
   and calls release on them.

inputs
   PCList   pList - List object that contains the notifications
returns
   none
*/
void NotificationUnRegisterAll (PCList pList)
{
PNOTIFY   pn;

while (pn = (PNOTIFY) pList->GetElem(0)) {
   ((LPUNKNOWN) pn->pInterface)->Release();
   pList->RemoveElem(0);
   };
}


/**************************************************************************
NotificationSend - This sends out a notification to all of the registered
   interface objects.

inputs
   PCList   pList - List object that contains the notifications
   DWORD      dwMessage - Message number
   DWORD      dwParam1 - first parameter (depends on message #)
   DWORD      dwParam2 - second parameter (decpond on message #)
   PVOID      pVoid - Extra parameter

returns
   none
*/
void NotificationSend (PCList pList, DWORD dwMessage, DWORD dwParam1, DWORD dwParam2,
   PVOID pVoid)
{
DWORD   i, dwNumElems;
PNOTIFY   pn;
PISRNOTIFYSINKW   pnsw;

dwNumElems = pList->GetNumElems();
for (i = 0; i < dwNumElems; i++) {
   pn = (PNOTIFY) pList->GetElem(i);
   if (!pn)
      continue;   // error

   // We only suppord IID_ITTSNotifySinkW at the moment
   if (!IsEqualIID(pn->iid, IID_ISRNotifySinkW))
      continue;
   pnsw = (PISRNOTIFYSINKW) pn->pInterface;

   switch (dwMessage) {
      // FIll this in
      case   WM_NSATTRIBCHANGED:
         pnsw->AttribChanged (dwParam1);
         break;
      case   WM_NSINTERFERENCE:
         // This engine doesn't send this/
         break;
      case   WM_NSSOUND:
         // This engine doesn't send this
         break;
      case   WM_NSUTTERANCEBEGIN:
         pnsw->UtteranceBegin (((QWORD) dwParam2 << 32) | dwParam1);
         break;
      case   WM_NSUTTERANCEEND:
         {
         PQWORD   pqW = (PQWORD) dwParam1;
         pnsw->UtteranceEnd (pqW[0], pqW[1]);
         free ((PVOID) pqW);
         }
         break;
      case   WM_NSVUMETER:
         {
         pnsw->VUMeter (*((PQWORD) pVoid), (WORD) dwParam1);
         }
         break;
      };
   };
}



/************************************************************************
CMode - SR Mode class. The same code handles all modes (French & English)
*/

CMode::CMode(LPUNKNOWN punkOuter, LPFNDESTROYED pfnDestroy, LPUNKNOWN lpAudio)
{
// NOTE: When create mode, will need other stuff included

   m_cRef                = 0;
   m_punkOuter           = punkOuter;
   m_pfnDestroy          = pfnDestroy;
   m_pModeISRAttributesW = NULL;
   m_pModeISRAttributesA = NULL;
   m_pModeISRCentralW    = NULL;
   m_pModeISRCentralA    = NULL;
   m_pModeISRDialogsW    = NULL;
   m_pModeISRDialogsA    = NULL;
   hWndMain              = NULL;
   hWndEdit              = NULL;
   m_pList               = NULL;
   m_pNotify             = NULL;
   m_fUtterance          = FALSE;
   m_pNotifyList         = NULL;

   m_wFloor              = 0;
   m_pstrMicrophone      = NULL;
   m_dwRealTime          = 0;
   m_pstrSpeaker         = NULL;
   m_dwTimeOutCompl      = 0;
   m_dwTimeOutIncompl    = 0;

// under normal circumstances, we would store away the user
// name, and environment here, but the fake engine doesn't care

   m_pAudioIAudioSrc       = NULL;
   m_pAudioIAudio          = NULL;
   m_pAudioIUnknown          = lpAudio;

   memset(&m_SRModeInfo, 0, sizeof(m_SRModeInfo));

// intialize attributes

   m_dwConfidence       = 50;
   m_dwAutoGain         = 50;

}


CMode::~CMode (void)
{
   // Unregister
   if (m_pNotifyList) {
      NotificationUnRegisterAll (m_pNotifyList);
      delete m_pNotifyList;
      };

// Free the contained interfaces

   if (NULL != m_pModeISRAttributesW) delete m_pModeISRAttributesW;
   if (NULL != m_pModeISRAttributesA) delete m_pModeISRAttributesA;
   if (NULL != m_pModeISRCentralW   ) delete m_pModeISRCentralW;
   if (NULL != m_pModeISRCentralA   ) delete m_pModeISRCentralA;
   if (NULL != m_pModeISRDialogsW   ) delete m_pModeISRDialogsW;
   if (NULL != m_pModeISRDialogsA   ) delete m_pModeISRDialogsA;
   if (m_pAudioIAudioSrc) m_pAudioIAudioSrc->Release();
   if (m_pAudioIAudio   ) m_pAudioIAudio->Release();
   if (m_pAudioIUnknown ) m_pAudioIUnknown->Release();
   if (hWndMain) DestroyWindow (hWndMain);
   if (m_pNotify) delete m_pNotify;
   if (m_pList)
      delete m_pList;
}


BOOL CMode::FInit (void)
{
LPUNKNOWN      pIUnknown = (LPUNKNOWN) this;
WNDCLASS wc;
WAVEFORMATEX   wfex;


if (NULL != m_punkOuter)
   pIUnknown = m_punkOuter;

// Talk to the audio object and see if it can do 11 kHz, 8-bit mono
wfex.wFormatTag = WAVE_FORMAT_PCM;
wfex.nChannels = 1;
wfex.nSamplesPerSec = 11025;
wfex.nAvgBytesPerSec = 11025;
wfex.nBlockAlign = 1;
wfex.wBitsPerSample = 8;
wfex.cbSize = 0;
if (!FAILED(m_pAudioIUnknown->QueryInterface(IID_IAudio, (VOID**)&m_pAudioIAudio)) &&
   !FAILED (m_pAudioIUnknown->QueryInterface(IID_IAudioSource, (VOID**) &m_pAudioIAudioSrc)) ) {
      // see if it like sthe wave-format
      SDATA      sd;

      sd.pData = (PVOID) &wfex;
      sd.dwSize = sizeof(wfex);
      if (FAILED(m_pAudioIAudio->WaveFormatSet (sd))) {
         // It cant handle the wave-format that we want so fail
         return FALSE;
         };
      };

// Allocate all of the contained interfaces

m_pModeISRCentralW    = new CModeISRCentralW    (this, pIUnknown);
m_pModeISRCentralA    = new CModeISRCentralA    (this, pIUnknown);
m_pModeISRDialogsW    = new CModeISRDialogsW    (this, pIUnknown);
m_pModeISRDialogsA    = new CModeISRDialogsA    (this, pIUnknown);
m_pModeISRAttributesW = new CModeISRAttributesW (this, pIUnknown);
m_pModeISRAttributesA = new CModeISRAttributesA (this, pIUnknown);
m_pList = new CList ();
m_pNotify = new CModeNotify((void*) this);
m_pNotifyList          = new CList();


// Pass in the notification mode so that it gets notification
if (m_pAudioIAudioSrc && m_pAudioIAudio && m_pNotify)
   m_pAudioIAudio->PassNotify(m_pNotify, IID_IAudioSourceNotifySink);


// create a window to represent the SR engine.
// Real engines don't do this, but its grreat for testing
/* create the class */
memset (&wc, 0, sizeof(wc));
wc.style = CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc = ModeWindowProc;
wc.hInstance = ghInstance;
wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
wc.hCursor = LoadCursor (NULL, MAKEINTRESOURCE(IDC_ARROW));
wc.hbrBackground = (HBRUSH) (COLOR_BACKGROUND+1);
wc.lpszMenuName = NULL;
wc.lpszClassName = "SRModeClass";
wc.cbWndExtra = sizeof(long);
RegisterClass (&wc);

/* create the window */
hWndMain = CreateWindow (
   wc.lpszClassName,
   "Speech Recognition Mode",
   WS_OVERLAPPEDWINDOW,
   CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
   NULL, NULL, ghInstance, (VOID*) this
   );
ShowWindow (hWndMain, SW_MINIMIZE | SW_SHOW);


return (m_pModeISRAttributesW && m_pModeISRCentralW && m_pModeISRDialogsW &&
        m_pModeISRAttributesA && m_pModeISRCentralA && m_pModeISRDialogsA &&
   m_pAudioIAudio &&
   m_pAudioIAudioSrc &&
   m_pNotifyList &&
   m_pList && m_pNotify);
}


/*************************************************************************
Output - This outputs a string to the mode window.

inputs
   PSTR    pStr - Pointer to the string.
returns
   none
*/
void CMode::Output (PSTR pStr)
{
int      iLen;

if (!hWndEdit)   
   return;

iLen = GetWindowTextLength (hWndEdit);
SendMessage (hWndEdit, EM_SETSEL, (WPARAM) iLen, (LPARAM) iLen);
SendMessage (hWndEdit, EM_REPLACESEL, (WPARAM) 0, (LPARAM) (LPVOID *) pStr);
}


STDMETHODIMP CMode::QueryInterface (REFIID riid, LPVOID *ppv)
{
*ppv = NULL;

/* always return our IUnkown for IID_IUnknown */
if (IsEqualIID (riid, IID_IUnknown))
   *ppv = (LPVOID) this;

// other interfaces

if (IsEqualIID(riid, IID_ISRAttributesW)) *ppv = m_pModeISRAttributesW;
if (IsEqualIID(riid, IID_ISRAttributesA)) *ppv = m_pModeISRAttributesA;
if (IsEqualIID(riid, IID_ISRCentralW   )) *ppv = m_pModeISRCentralW;
if (IsEqualIID(riid, IID_ISRCentralA   )) *ppv = m_pModeISRCentralA;
if (IsEqualIID(riid, IID_ISRDialogsW   )) *ppv = m_pModeISRDialogsW;
if (IsEqualIID(riid, IID_ISRDialogsA   )) *ppv = m_pModeISRDialogsA;

// update the reference count
if (NULL != *ppv) {
   ((LPUNKNOWN) *ppv) ->AddRef();
   return NOERROR;
   }

return ResultFromScode (E_NOINTERFACE);
}


STDMETHODIMP_ (ULONG) CMode::AddRef (void)
{
return ++m_cRef;
}


STDMETHODIMP_(ULONG) CMode::Release (void)
{
   ULONG    cRefT;

   cRefT = --m_cRef;

   if (0 == m_cRef)
      {
      if (NULL != m_pfnDestroy) (*m_pfnDestroy)(); // inform the destroy fn
      delete this;
      }

   return cRefT;
}

STDMETHODIMP CMode::DataAvailable(DWORD dwData, BOOL fEOF)
{
   LPBYTE   pByte;
   HRESULT  hRes;
   DWORD    dwLoaded;
   QWORD    qwTimeStamp;
   WORD     wLevel;
   BOOL     fHaveUtterance;
   DWORD    i;

// if we have data, then read it all in and start analyzing it.
// Because this is just a test, I'm going to allocate the memory
// on the fly, and then free it. A real recognizer would have its
// own buffers.

   pByte = (LPBYTE) malloc (dwData);

   if (!pByte) return ResultFromScode (E_OUTOFMEMORY);

   m_pAudioIAudio->TotalGet (&qwTimeStamp);
   hRes = m_pAudioIAudioSrc->DataGet (pByte, dwData, &dwLoaded);

   if (hRes)
      {
      free (pByte);
      return hRes;
      }

// Do a Vu meter

   if (dwLoaded > 0)
      {
      BYTE  bMax = 0x00, bMin = 0xff;
      DWORD i;

      for (i = 0; i < dwLoaded; i++)
         {
         if (pByte[i] > bMax) bMax = pByte[i];
         if (pByte[i] < bMin) bMin = pByte[i];
         }

// do the max of these

      bMax = (bMax >= 0x80) ? (bMax - 0x80) : (0x7f - bMax);
      bMin = (bMin >= 0x80) ? (bMin - 0x80) : (0x7f - bMin);
      bMax = (bMax > bMin) ? bMax : bMin;

// Notify the applications on the list
      NotificationSend (m_pNotifyList, WM_NSVUMETER,
         (DWORD)(wLevel = (WORD) bMax * (WORD) (0xffff / 0x7f)) ,
         0, &qwTimeStamp);

// This is a stupid technique to detect the start/stop of
// an utterance, but it works well enough for testing purposes.

      fHaveUtterance = (wLevel > 0x8000);
      };

if (fHaveUtterance != m_fUtterance) {
   m_fUtterance = fHaveUtterance;
   if (fHaveUtterance) {
      // Someone just started talking
      m_qwUttStart = qwTimeStamp;
      PostMessage (hWndMain, WM_NSUTTERANCEBEGIN,
         (WPARAM) (DWORD) qwTimeStamp, (LPARAM) (DWORD) (qwTimeStamp >> 32));
      }
   else {
      PQWORD pqw;

      pqw = (PQWORD) malloc (sizeof(QWORD) * 2);
      pqw[0] = m_qwUttStart;
      pqw[1] = qwTimeStamp;
      PostMessage (hWndMain, WM_NSUTTERANCEEND, (WPARAM) (DWORD) pqw, 0);

      // Loop through all of the grammars and inform them that
      // the utterance is done. This is mostly for test purposes.
      // They will then send a recognition up tot he application
      // if anything way typed in their edit controls
      for (i = 0; i < m_pList->GetNumElems(); i++) {
         PCGrm *ppTmp;
         ppTmp = (PCGrm *) m_pList->GetElem(i);
         (*ppTmp)->Utterance (m_qwUttStart, qwTimeStamp);
         }
      }
   }


// We should analyze the data here and send it in for recognition.

// All done, so free the memory
free (pByte);
return NOERROR;
}

void CMode::GrammarRelease (PCGrm pGrm)
{
DWORD dwNum, i;
PCGrm *ppTmp;

for (i =0, dwNum = m_pList->GetNumElems(); i < dwNum; i++) {
   ppTmp = (PCGrm *) m_pList->GetElem(i);
   if (*ppTmp == pGrm) {
      // we found a match
      m_pList->RemoveElem(i);
      return;  // done
      }
   }

// if we get here then bad things have happened
}



/************************************************************************
CModeISRCentralW - ISRCentral interface
*/

CModeISRCentralA::CModeISRCentralA (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_pObjA = (PCMode)pObj;
   (((PCMode) m_pObjA)->m_pModeISRCentralW)->FInit(pObj, punkOuter);
}


CModeISRCentralW::CModeISRCentralW (LPVOID pObj, LPUNKNOWN punkOuter)
{
m_cRef = 0;
m_pObj = pObj;
m_punkOuter = punkOuter;
}


// This function is called from the ANSI interface constructor -

CModeISRCentralW::FInit(LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef      = 0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;

   return NOERROR;
}


CModeISRCentralA::~CModeISRCentralA(void)
{
   (((PCMode) m_pObjA)->m_pModeISRCentralW)->~CModeISRCentralW();
}


CModeISRCentralW::~CModeISRCentralW (void)
{
// intentionally left blank
}


STDMETHODIMP CModeISRCentralA::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return (((PCMode) m_pObjA)->m_pModeISRCentralW)->QueryInterface(riid, ppv);
}


STDMETHODIMP CModeISRCentralW::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return m_punkOuter->QueryInterface(riid,ppv);
}


STDMETHODIMP_ (ULONG) CModeISRCentralA::AddRef(void)
{
   return (((PCMode) m_pObjA)->m_pModeISRCentralW)->AddRef();
}


STDMETHODIMP_ (ULONG) CModeISRCentralW::AddRef(void)
{
   ++m_cRef;
   return m_punkOuter->AddRef();
}


STDMETHODIMP_(ULONG) CModeISRCentralA::Release(void)
{
   return (((PCMode) m_pObjA)->m_pModeISRCentralW)->Release();
}


STDMETHODIMP_ (ULONG) CModeISRCentralW::Release(void)
{
   --m_cRef;
   return m_punkOuter->Release();
}


STDMETHODIMP CModeISRCentralA::ModeGet (PSRMODEINFOA pModeInfo)
{
   HRESULT     hRes;
   SRMODEINFOW ModeInfoW;

   hRes = (((PCMode) m_pObjA)->m_pModeISRCentralW)->ModeGet(&ModeInfoW);

   if (hRes == NOERROR)
      {
      pModeInfo->gEngineID = ModeInfoW.gEngineID;

      WideCharToMultiByte(CP_ACP, 0, ModeInfoW.szMfgName, -1,
                          pModeInfo->szMfgName, SRMI_NAMELEN, NULL, NULL);

      WideCharToMultiByte(CP_ACP, 0, ModeInfoW.szProductName, -1,
                          pModeInfo->szProductName, SRMI_NAMELEN, NULL, NULL);

      pModeInfo->gModeID = ModeInfoW.gModeID;

      WideCharToMultiByte(CP_ACP, 0, ModeInfoW.szModeName, -1,
                          pModeInfo->szModeName, SRMI_NAMELEN, NULL, NULL);

      pModeInfo->language.LanguageID = ModeInfoW.language.LanguageID;

      WideCharToMultiByte(CP_ACP, 0, ModeInfoW.language.szDialect, -1,
                          pModeInfo->language.szDialect, LANG_LEN, NULL, NULL);

      pModeInfo->dwSequencing     = ModeInfoW.dwSequencing;
      pModeInfo->dwMaxWordsVocab  = ModeInfoW.dwMaxWordsVocab;;
      pModeInfo->dwMaxWordsState  = ModeInfoW.dwMaxWordsState;
      pModeInfo->dwGrammars       = ModeInfoW.dwGrammars;
      pModeInfo->dwFeatures       = ModeInfoW.dwFeatures;
      pModeInfo->dwInterfaces     = ModeInfoW.dwInterfaces;
      pModeInfo->dwEngineFeatures = ModeInfoW.dwEngineFeatures;
      }

   return hRes;
}


STDMETHODIMP CModeISRCentralW::ModeGet (PSRMODEINFOW pModeInfo)
{
   if (!pModeInfo) return ResultFromScode (SRERR_INVALIDPARAM);

   memcpy(pModeInfo, &((PCMode)m_pObj)->m_SRModeInfo, sizeof(SRMODEINFOW));

   return NOERROR;
}


STDMETHODIMP CModeISRCentralA::GrammarLoad (SRGRMFMT eFormat, SDATA dData,
               PVOID pNotify, IID iidNotify, LPUNKNOWN * lpUnkGrammar)
{
   return (((PCMode) m_pObjA)->m_pModeISRCentralW)->GrammarLoad(eFormat,
                               dData, pNotify, iidNotify, lpUnkGrammar);
}


STDMETHODIMP CModeISRCentralW::GrammarLoad (SRGRMFMT eFormat, SDATA dData,
               PVOID pNotify, IID iidNotify, LPUNKNOWN * lpUnkGrammar)
{
   PCGrm    pGrm;
   HRESULT  hr;

   if (!dData.pData || !lpUnkGrammar) return ResultFromScode (SRERR_INVALIDPARAM);

   if (eFormat !=  SRGRMFMT_CFG) return ResultFromScode (SRERR_GRAMMARWRONGTYPE);

   if (pNotify && !IsEqualIID(iidNotify, IID_ISRGramNotifySinkW))
      return ResultFromScode (SRERR_INVALIDINTERFACE);
   // NOTE: Need this to do ANSI eventually

// check errors

   *lpUnkGrammar = NULL;

// Try to create a new enumeration object

   pGrm = new CGrm (NULL, ((PCMode)m_pObj)->m_pfnDestroy, m_pObj);

   hr = ResultFromScode (E_OUTOFMEMORY);

   if (!pGrm) return hr;

   if (pGrm->FInit(eFormat, dData, (PISRGRAMNOTIFYSINKW) pNotify))

      hr = pGrm->QueryInterface(IID_IUnknown, (VOID**)lpUnkGrammar);

// kill the object if the finit failed

   if (FAILED(hr)) delete pGrm;
   else            gEngObjectCount++;

// If we succeded, then add this to the list of grammars
// so that we can notify them later when events occur

   if (!hr && ((PCMode)m_pObj)->m_pList)  

      (((PCMode)m_pObj)->m_pList)->AddElem ((void*) &pGrm, sizeof(pGrm));

   return hr;
}


STDMETHODIMP CModeISRCentralA::Pause (void)
{
   return(((PCMode) m_pObjA)->m_pModeISRCentralW)->Pause();
}


STDMETHODIMP CModeISRCentralW::Pause (void)
{
// Pause/resume isn't supported in the demo

   return ResultFromScode (SRERR_NOTSUPPORTED);
}


STDMETHODIMP CModeISRCentralA::PosnGet(PQWORD pqTime)
{
   return(((PCMode) m_pObjA)->m_pModeISRCentralW)->PosnGet(pqTime);
}


STDMETHODIMP CModeISRCentralW::PosnGet(PQWORD pqTime)
{
   PCMode  pMode = (PCMode) m_pObj;

   return pMode->m_pAudioIAudio->PosnGet (pqTime);
}


STDMETHODIMP CModeISRCentralA::Resume (void)
{
   return(((PCMode) m_pObjA)->m_pModeISRCentralW)->Resume();
}


STDMETHODIMP CModeISRCentralW::Resume (void)
{
// Pause/resume isn't supported in the demo

   return ResultFromScode (SRERR_NOTSUPPORTED);
}


STDMETHODIMP CModeISRCentralA::ToFileTime(PQWORD pqTime, FILETIME * ft)
{
   return(((PCMode) m_pObjA)->m_pModeISRCentralW)->ToFileTime(pqTime, ft);
}


STDMETHODIMP CModeISRCentralW::ToFileTime(PQWORD pqTime, FILETIME * ft)
{
PCMode  pMode = (PCMode) m_pObj;

return pMode->m_pAudioIAudio->ToFileTime (pqTime, ft);
}


STDMETHODIMP CModeISRCentralA::Register(PVOID pInterface, IID iid,
                                        DWORD * pdwKey)
{
   return(((PCMode) m_pObjA)->m_pModeISRCentralW)->Register(pInterface, iid,
                                                            pdwKey);
}


STDMETHODIMP CModeISRCentralW::Register(PVOID pInterface, IID iid,
   DWORD * pdwKey)
{
PCMode  pMode = (PCMode) m_pObj;

if (!IsEqualIID (iid, IID_ISRNotifySinkW))

   return ResultFromScode (SRERR_INVALIDINTERFACE);

*pdwKey = NotificationRegister (pMode->m_pNotifyList, pInterface, iid);
if (!(*pdwKey))
   return ResultFromScode (E_OUTOFMEMORY);
else
   return NOERROR;
}


STDMETHODIMP CModeISRCentralA::UnRegister(DWORD dwKey)
{
   return(((PCMode) m_pObjA)->m_pModeISRCentralW)->UnRegister(dwKey);
}


STDMETHODIMP CModeISRCentralW::UnRegister(DWORD dwKey)
{
PCMode  pMode = (PCMode) m_pObj;

if (NotificationUnRegister (pMode->m_pNotifyList, dwKey))
   return ResultFromScode (SRERR_INVALIDKEY);
else
   return NOERROR;
}


/************************************************************************
CModeISRAttributesW - ISRAttributes interface
*/

CModeISRAttributesA::CModeISRAttributesA (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_pObjA = (PCMode)pObj;
   (((PCMode) m_pObjA)->m_pModeISRAttributesW)->FInit(pObj, punkOuter);
}


CModeISRAttributesW::CModeISRAttributesW (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef = 0;
   m_pObj = pObj;
   m_punkOuter = punkOuter;
}


// This function is called from the ANSI interface constructor -

CModeISRAttributesW::FInit(LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef      = 0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;

   return NOERROR;
}


CModeISRAttributesA::~CModeISRAttributesA (void)
{
   (((PCMode) m_pObjA)->m_pModeISRAttributesW)->~CModeISRAttributesW();
}


CModeISRAttributesW::~CModeISRAttributesW (void)
{
// intentionally left blank
}


STDMETHODIMP CModeISRAttributesA::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->QueryInterface(riid, ppv);
}


STDMETHODIMP CModeISRAttributesW::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
return m_punkOuter->QueryInterface(riid,ppv);
}


STDMETHODIMP_ (ULONG) CModeISRAttributesA::AddRef(void)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->AddRef();
}

   
STDMETHODIMP_ (ULONG) CModeISRAttributesW::AddRef(void)
{
++m_cRef;
return m_punkOuter->AddRef();
}


STDMETHODIMP_ (ULONG) CModeISRAttributesA::Release(void)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->Release();
}


STDMETHODIMP_ (ULONG) CModeISRAttributesW::Release(void)
{
--m_cRef;
return m_punkOuter->Release();
}


STDMETHODIMP CModeISRAttributesA::AutoGainEnableGet (DWORD * pdwAutoGain)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->AutoGainEnableGet(pdwAutoGain);
}


STDMETHODIMP CModeISRAttributesW::AutoGainEnableGet (DWORD * pdwAutoGain)
{
if (!pdwAutoGain)
   return ResultFromScode (SRERR_INVALIDPARAM);

*pdwAutoGain = ((PCMode)m_pObj)->m_dwAutoGain;

return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::AutoGainEnableSet (DWORD dwAutoGain)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->AutoGainEnableSet(dwAutoGain);
}


STDMETHODIMP CModeISRAttributesW::AutoGainEnableSet (DWORD dwAutoGain)
{
if (dwAutoGain > 100)
   dwAutoGain = 100;

PostMessage (((PCMode)m_pObj)->hWndMain, WM_NSATTRIBCHANGED, ISRNSAC_AUTOGAINENABLE, 0);
((PCMode)m_pObj)->m_dwAutoGain = dwAutoGain;

return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::EchoGet (BOOL * pfEcho)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->EchoGet(pfEcho);
}


STDMETHODIMP CModeISRAttributesW::EchoGet (BOOL * pfEcho)
{
   if (!pfEcho) return ResultFromScode(SRERR_INVALIDPARAM);

   *pfEcho = ((PCMode)m_pObj)->m_fEcho;

   return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::EchoSet (BOOL fEcho)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->EchoSet(fEcho);
}


STDMETHODIMP CModeISRAttributesW::EchoSet (BOOL fEcho)
{
   PostMessage (((PCMode)m_pObj)->hWndMain, WM_NSATTRIBCHANGED, ISRNSAC_ECHO, 0);

   ((PCMode)m_pObj)->m_fEcho;

   return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::EnergyFloorGet (WORD * pwFloor)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->EnergyFloorGet(pwFloor);
}


STDMETHODIMP CModeISRAttributesW::EnergyFloorGet (WORD * pwFloor)
{
   if (!pwFloor) return ResultFromScode(SRERR_INVALIDPARAM);

   *pwFloor = ((PCMode) m_pObj)->m_wFloor;

   return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::EnergyFloorSet (WORD wFloor)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->EnergyFloorSet(wFloor);
}


STDMETHODIMP CModeISRAttributesW::EnergyFloorSet (WORD wFloor)
{
   if (wFloor < 0 || wFloor > 100) return ResultFromScode(SRERR_VALUEOUTOFRANGE);

   PostMessage (((PCMode)m_pObj)->hWndMain, WM_NSATTRIBCHANGED, ISRNSAC_ENERGYFLOOR, 0);
   ((PCMode)m_pObj)->m_wFloor = wFloor;

   return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::MicrophoneGet(PSTR pMic, DWORD dwSize, DWORD *pdwSizeNeeded)
{
   HRESULT  hRes;
   PWSTR    pMicW;

   if (!pMic) return ResultFromScode(SRERR_INVALIDPARAM);

   pMicW = (PWSTR) malloc(dwSize * sizeof(WCHAR));

   if (pMicW)
      {
      hRes = (((PCMode) m_pObjA)->m_pModeISRAttributesW)->MicrophoneGet(pMicW, dwSize, pdwSizeNeeded);
      WideCharToMultiByte (CP_ACP, 0, pMicW, -1, pMic, dwSize, NULL, NULL);
      free(pMicW);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


/*******************************************************************************
 * 
 * returns
 *    
 *    SRERR_STRTRUNCATED   The buffer is too short, but filled with a truncated
 *                         version of the requested string.
 *    
 ******************************************************************************/

STDMETHODIMP CModeISRAttributesW::MicrophoneGet (PWSTR wMic, DWORD dwSize, DWORD *pdwSizeNeeded)
{
   if (!wMic || !pdwSizeNeeded) return ResultFromScode(SRERR_INVALIDPARAM);

   if (_tcslen(((PCMode) m_pObj)->m_pstrMicrophone) >= dwSize)

      return ResultFromScode(SRERR_STRTRUNCATED);

// This function not implemented because it's not necessary for
// demonstration purposes.

return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::MicrophoneSet(PCSTR pMic)
{
   HRESULT  hRes;
   DWORD    cchMic = MyAStrLen(pMic) + 1;
   PWSTR    pMicW;

   pMicW = (PWSTR) malloc(cchMic * sizeof(WCHAR));

   if (pMicW)
      {
      MultiByteToWideChar(CP_ACP, 0, pMic, -1, pMicW, cchMic);
      hRes = (((PCMode) m_pObjA)->m_pModeISRAttributesW)->MicrophoneSet(pMicW);
      free(pMicW);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeISRAttributesW::MicrophoneSet (PCWSTR wMic)
{
// This function not implemented because it's not necessary for
// demonstration purposes.
return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::RealTimeGet (DWORD * pdwRealTime)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->RealTimeGet(pdwRealTime);
}


STDMETHODIMP CModeISRAttributesW::RealTimeGet (DWORD * pdwRealTime)
{
// This function not implemented because it's not necessary for
// demonstration purposes.
return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::RealTimeSet (DWORD dwRealTime)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->RealTimeSet(dwRealTime);
}


STDMETHODIMP CModeISRAttributesW::RealTimeSet (DWORD dwRealTime)
{
// This function not implemented because it's not necessary for
// demonstration purposes.
return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::SpeakerGet (PSTR pSpkr, DWORD dwSize, DWORD *pdwSizeNeeded)
{
   HRESULT  hRes;
   PWSTR    pSpkrW;

   pSpkrW = (PWSTR) malloc(dwSize * sizeof(WCHAR));

   if (pSpkrW)
      {
      hRes = (((PCMode) m_pObjA)->m_pModeISRAttributesW)->SpeakerGet(pSpkrW, dwSize, pdwSizeNeeded);
      WideCharToMultiByte (CP_ACP, 0, pSpkrW, -1, pSpkr, dwSize, NULL, NULL);
      free(pSpkrW);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeISRAttributesW::SpeakerGet (PWSTR wSpkr, DWORD dwSize, DWORD *pdwSizeNeeded)
{
// This function not implemented because it's not necessary for
// demonstration purposes.
return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::SpeakerSet (PCSTR pSpkr)
{
   HRESULT  hRes;
   DWORD    cchSpkr = MyAStrLen(pSpkr) + 1;
   PWSTR    pSpkrW;

   pSpkrW = (PWSTR) malloc(cchSpkr * sizeof(WCHAR));

   if (pSpkrW)
      {
      MultiByteToWideChar(CP_ACP, 0, pSpkr, -1, pSpkrW, cchSpkr);
      hRes = (((PCMode) m_pObjA)->m_pModeISRAttributesW)->SpeakerSet(pSpkrW);
      free(pSpkrW);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeISRAttributesW::SpeakerSet (PCWSTR wSpkr)
{
// This function not implemented because it's not necessary for
// demonstration purposes.
return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::ThresholdGet (DWORD * pdwConfidenceGet)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->ThresholdGet(pdwConfidenceGet);
}


STDMETHODIMP CModeISRAttributesW::ThresholdGet (DWORD * pdwConfidenceGet)
{
if (!pdwConfidenceGet)
   return ResultFromScode (SRERR_INVALIDPARAM);

*pdwConfidenceGet = ((PCMode)m_pObj)->m_dwConfidence;

return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::ThresholdSet (DWORD dwConfidenceSet)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->ThresholdSet(dwConfidenceSet);
}


STDMETHODIMP CModeISRAttributesW::ThresholdSet (DWORD dwConfidenceSet)
{
if (dwConfidenceSet > 100)
   return ResultFromScode (SRERR_INVALIDPARAM);

PostMessage (((PCMode)m_pObj)->hWndMain, WM_NSATTRIBCHANGED, ISRNSAC_THRESHOLD, 0);
((PCMode)m_pObj)->m_dwConfidence = dwConfidenceSet;

return NOERROR;
}


STDMETHODIMP CModeISRAttributesA::TimeOutGet (DWORD * pdwTime1, DWORD * pdwTime2)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->TimeOutGet(pdwTime1, pdwTime2);
}


STDMETHODIMP CModeISRAttributesW::TimeOutGet (DWORD * pdwTime1, DWORD * pdwTime2)
{
// This function not implemented because it's not necessary for
// demonstration purposes.

   return ResultFromScode (SRERR_NOTSUPPORTED);
}


STDMETHODIMP CModeISRAttributesA::TimeOutSet (DWORD dwTime1, DWORD dwTime2)
{
   return (((PCMode) m_pObjA)->m_pModeISRAttributesW)->TimeOutSet(dwTime1, dwTime2);
}


STDMETHODIMP CModeISRAttributesW::TimeOutSet (DWORD dwTime1, DWORD dwTime2)
{
// This function not implemented because it's not necessary for
// demonstration purposes.
   return ResultFromScode (SRERR_NOTSUPPORTED);
}


/************************************************************************
CModeISRDialogsW - ISRAttributes interface
*/

CModeISRDialogsA::CModeISRDialogsA (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_pObjA = (PCMode)pObj;
   (((PCMode) m_pObjA)->m_pModeISRDialogsW)->FInit(pObj, punkOuter);
}


CModeISRDialogsW ::CModeISRDialogsW (LPVOID pObj, LPUNKNOWN punkOuter)
{
m_cRef = 0;
m_pObj = pObj;
m_punkOuter = punkOuter;
}


// This function is called from the ANSI interface constructor -

CModeISRDialogsW::FInit(LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef      = 0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;

   return NOERROR;
}


CModeISRDialogsA::~CModeISRDialogsA (void)
{
   (((PCMode) m_pObjA)->m_pModeISRDialogsW)->~CModeISRDialogsW();
}


CModeISRDialogsW::~CModeISRDialogsW (void)
{
// intentionally left blank
}


STDMETHODIMP CModeISRDialogsA::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return (((PCMode) m_pObjA)->m_pModeISRDialogsW)->QueryInterface(riid, ppv);
}


STDMETHODIMP CModeISRDialogsW::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
return m_punkOuter->QueryInterface(riid,ppv);
}


STDMETHODIMP_ (ULONG) CModeISRDialogsA::AddRef(void)
{
   return (((PCMode) m_pObjA)->m_pModeISRDialogsW)->AddRef();
}


STDMETHODIMP_ (ULONG) CModeISRDialogsW::AddRef(void)
{
++m_cRef;
return m_punkOuter->AddRef();
}


STDMETHODIMP_ (ULONG) CModeISRDialogsA::Release(void)
{
   return (((PCMode) m_pObjA)->m_pModeISRDialogsW)->Release();
}


STDMETHODIMP_ (ULONG) CModeISRDialogsW::Release(void)
{
--m_cRef;
return m_punkOuter->Release();
}


STDMETHODIMP CModeISRDialogsA::AboutDlg (HWND hWndParent, PCSTR pszTitle)
{
   HRESULT  hRes;
   DWORD    cchTitle = MyAStrLen(pszTitle) + 1;
   PWSTR    pwszTitle;

   pwszTitle = (PWSTR) malloc(cchTitle * sizeof(WCHAR));

   if (pwszTitle)
      {
      MultiByteToWideChar(CP_ACP, 0, pszTitle, -1, pwszTitle, cchTitle);
      hRes = (((PCMode) m_pObjA)->m_pModeISRDialogsW)->AboutDlg(hWndParent, pwszTitle);
      free(pwszTitle);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeISRDialogsW::AboutDlg (HWND hWndParent, PCWSTR pszTitle)
{
   char  szTemp[256];

// a valid handle?

   if (!hWndParent || !IsWindow(hWndParent))

      return ResultFromScode (SRERR_INVALIDPARAM);

// convert from UNICODE to ANSI

   if (pszTitle)

      WideCharToMultiByte (CP_ACP, 0, pszTitle, -1, szTemp, sizeof(szTemp), NULL, NULL);

// Do a simple dialog for this example

   MessageBox (hWndParent, "About dialog box", pszTitle ? szTemp : NULL, MB_OK);
   
   return NOERROR;
}


STDMETHODIMP CModeISRDialogsA::GeneralDlg(HWND hWndParent, PCSTR pszTitle)
{
   HRESULT  hRes;
   DWORD    cchTitle = MyAStrLen(pszTitle) + 1;
   PWSTR    pwszTitle;

   pwszTitle = (PWSTR) malloc(cchTitle * sizeof(WCHAR));

   if (pwszTitle)
      {
      MultiByteToWideChar(CP_ACP, 0, pszTitle, -1, pwszTitle, cchTitle);
      hRes = (((PCMode) m_pObjA)->m_pModeISRDialogsW)->GeneralDlg(hWndParent, pwszTitle);
      free(pwszTitle);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeISRDialogsW::GeneralDlg (HWND hWndParent, PCWSTR pszTitle)
{
   char  szTemp[256];

// a valid handle?

   if (!hWndParent || !IsWindow(hWndParent))

      return ResultFromScode (SRERR_INVALIDPARAM);

// convert from UNICODE to ANSI

   if (pszTitle)

      WideCharToMultiByte (CP_ACP, 0, pszTitle, -1, szTemp, sizeof(szTemp), NULL, NULL);

// Do a simple dialog for this example

   MessageBox (hWndParent, "General dialog box", pszTitle ? szTemp : NULL, MB_OK);
   return NOERROR;
}


STDMETHODIMP CModeISRDialogsA::LexiconDlg(HWND hWndParent, PCSTR pszTitle)
{
   HRESULT  hRes;
   DWORD    cchTitle = MyAStrLen(pszTitle) + 1;
   PWSTR    pwszTitle;

   pwszTitle = (PWSTR) malloc(cchTitle * sizeof(WCHAR));

   if (pwszTitle)
      {
      MultiByteToWideChar(CP_ACP, 0, pszTitle, -1, pwszTitle, cchTitle);
      hRes = (((PCMode) m_pObjA)->m_pModeISRDialogsW)->LexiconDlg(hWndParent, pwszTitle);
      free(pwszTitle);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeISRDialogsW::LexiconDlg (HWND hWndParent, PCWSTR pszTitle)
{
   char  szTemp[256];

// a valid handle?

   if (!hWndParent || !IsWindow(hWndParent))

      return ResultFromScode (SRERR_INVALIDPARAM);

// convert from UNICODE to ANSI

   if (pszTitle)

      WideCharToMultiByte (CP_ACP, 0, pszTitle, -1, szTemp, sizeof(szTemp), NULL, NULL);

// Do a simple dialog for this example

   MessageBox (hWndParent, "Lexicon dialog box", pszTitle ? szTemp : NULL, MB_OK);
   return NOERROR;
}


STDMETHODIMP CModeISRDialogsA::TrainMicDlg(HWND hWndParent, PCSTR pszTitle)
{
   HRESULT  hRes;
   DWORD    cchTitle = MyAStrLen(pszTitle) + 1;
   PWSTR    pwszTitle;

   pwszTitle = (PWSTR) malloc(cchTitle * sizeof(WCHAR));

   if (pwszTitle)
      {
      MultiByteToWideChar(CP_ACP, 0, pszTitle, -1, pwszTitle, cchTitle);
      hRes = (((PCMode) m_pObjA)->m_pModeISRDialogsW)->TrainMicDlg(hWndParent, pwszTitle);
      free(pwszTitle);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeISRDialogsW::TrainMicDlg (HWND hWndParent, PCWSTR pszTitle)
{
// Claim that this one is not supported
return ResultFromScode(SRERR_NOTSUPPORTED);
}


STDMETHODIMP CModeISRDialogsA::TrainGeneralDlg(HWND hWndParent, PCSTR pszTitle)
{
   HRESULT  hRes;
   DWORD    cchTitle = MyAStrLen(pszTitle) + 1;
   PWSTR    pwszTitle;

   pwszTitle = (PWSTR) malloc(cchTitle * sizeof(WCHAR));

   if (pwszTitle)
      {
      MultiByteToWideChar(CP_ACP, 0, pszTitle, -1, pwszTitle, cchTitle);
      hRes = (((PCMode) m_pObjA)->m_pModeISRDialogsW)->TrainGeneralDlg(hWndParent, pwszTitle);
      free(pwszTitle);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeISRDialogsW::TrainGeneralDlg (HWND hWndParent, PCWSTR pszTitle)
{
   char  szTemp[256];

// a valid handle?

   if (!hWndParent || !IsWindow(hWndParent))

      return ResultFromScode (SRERR_INVALIDPARAM);

// convert from UNICODE to ANSI

   if (pszTitle)

      WideCharToMultiByte (CP_ACP, 0, pszTitle, -1, szTemp, sizeof(szTemp), NULL, NULL);

// Do a simple dialog for this example

   MessageBox (hWndParent, "Training dialog box", pszTitle ? szTemp : NULL, MB_OK);
   return NOERROR;
}



/*************************************************************************
CModeNotify - Notification object. This will just pass the audio input
   notifications up to the CMode object.
*/

CModeNotify::CModeNotify (void * pObj)
{
// Get passed in the CMode object, save this
m_pObj = pObj;
}

CModeNotify::~CModeNotify (void)
{
// this space intentionally left blank
}

STDMETHODIMP CModeNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
*ppv = NULL;

/* always return our IUnkown for IID_IUnknown */
if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IAudioSourceNotifySink))
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

STDMETHODIMP CModeNotify::AudioStop (WORD wReason)
{
// ignore this
return NOERROR;
}

STDMETHODIMP CModeNotify::AudioStart (void)
{
// ignore this
return NOERROR;
}

STDMETHODIMP CModeNotify::DataAvailable (DWORD dwData, BOOL fEOF)
{
// pass it on
return ((PCMode) m_pObj)->DataAvailable(dwData, fEOF);
}

STDMETHODIMP CModeNotify::Overflow (DWORD dwLost)
{
// ignore this
return NOERROR;
}


