/************************************************************************
FakeTTS.Cpp - Fake TTS mode (main part) for testing purposes.
   Includes the interfaces:
      ISRAttributes
      ISRCentral
      ISRDialogs
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
#include "fakeTTS.h"
#include "fakettsE.h"

#include "resource.h"

// This is the temporary output window that dummies as a wave out - remove


#define	WM_NSATTRIBCHANGED		(WM_USER + 106)
#define	WM_NSAUDIOSTART			(WM_USER + 107)
#define	WM_NSAUDIOSTOP				(WM_USER + 108)
#define	WM_NSVISUAL					(WM_USER + 109)

typedef struct {
	PITTSBUFNOTIFYSINKW	pSinkW;
	// NOTE: Have a pSinkA here also, mutually exclusive of pSinkW
	DWORD						dwNotifiedStart;
	} BUFLIST, *PBUFLIST;

int  MyAStrLen (PCSTR pA);
void NotificationSend (PCList pList, DWORD dwMessage, DWORD dwParam1, DWORD dwParam2);



/************************************************************************
MyAStrLen - ANSI string length.

inputs
   PCSTR    pA - A string
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
ModeWindowProc - Window which is displayed for a TTS mode.
*/

LRESULT CALLBACK ModeWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

         pMode->hWndEdit = CreateWindow ("EDIT", "TTS results.\r\n",
            WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_READONLY | WS_VISIBLE | WS_CHILD,
            rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
            hWnd, NULL, ghInstance, NULL
            );
         }

         return 0;

      case WM_SIZE:              // resize the edit control
         {
         RECT  rect;

         GetClientRect (hWnd, &rect);
         MoveWindow (pMode->hWndEdit, 0, 0, rect.right, rect.bottom, TRUE);
         }

         return 0;


		case	WM_NSATTRIBCHANGED:
		case	WM_NSAUDIOSTART:
		case	WM_NSAUDIOSTOP:
		case	WM_NSVISUAL:
			NotificationSend (pMode->m_pNotifyList, uMsg, wParam, lParam);
			return 0;
      }

   return DefWindowProc (hWnd, uMsg, wParam, lParam);
}


typedef struct {
	PVOID			pInterface;
	IID			iid;
	DWORD			dwKey;
	} NOTIFY, *PNOTIFY;

/*************************************************************************
NotificationRegister - This function adds an application to the list of
	notifcations-objects which are not be notified.

inputs
	PCList	pList - List object that contains the notifications
	PVOID		pInterface - Pointer to the interface
	IID		iid - Interface #
returns
	DWORD - Key #, or 0 if error
*/
DWORD NotificationRegister (PCList pList, PVOID pInterface, IID iid)
{
static 	DWORD dwKeyCount = 1;	// key-count
NOTIFY	n;

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
	PCList	pList - List object that contains the notifications
	DWORD		dwKey - Key value to unregister
returns
	BOOL - TTRUE if found it, FALSE if didn't
*/
BOOL NotificationUnRegister (PCList pList, DWORD dwKey)
{
DWORD	i, dwNumElems;
PNOTIFY	pn;

dwNumElems = pList->GetNumElems();
for (i = 0; i < dwNumElems; i++) {
	pn = (PNOTIFY) pList->GetElem(i);
	if (!pn)
		continue;	// error
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
	PCList	pList - List object that contains the notifications
returns
	none
*/
void NotificationUnRegisterAll (PCList pList)
{
PNOTIFY	pn;

while (pn = (PNOTIFY) pList->GetElem(0)) {
	((LPUNKNOWN) pn->pInterface)->Release();
	pList->RemoveElem(0);
	};
}


/**************************************************************************
NotificationSend - This sends out a notification to all of the registered
	interface objects.

inputs
	PCList	pList - List object that contains the notifications
	DWORD		dwMessage - Message number
	DWORD		dwParam1 - first parameter (depends on message #)
	DWORD		dwParam2 - second parameter (decpond on message #)

returns
	none
*/
void NotificationSend (PCList pList, DWORD dwMessage, DWORD dwParam1, DWORD dwParam2)
{
DWORD	i, dwNumElems;
PNOTIFY	pn;
PITTSNOTIFYSINKW	pnsw;

dwNumElems = pList->GetNumElems();
for (i = 0; i < dwNumElems; i++) {
	pn = (PNOTIFY) pList->GetElem(i);
	if (!pn)
		continue;	// error

	// We only suppord IID_ITTSNotifySinkW at the moment
	if (!IsEqualIID(pn->iid, IID_ITTSNotifySinkW))
		continue;
	pnsw = (PITTSNOTIFYSINKW) pn->pInterface;

	switch (dwMessage) {
		// FIll this in
		case	WM_NSATTRIBCHANGED:
			pnsw->AttribChanged (dwParam1);
			break;
		case	WM_NSAUDIOSTART:
			pnsw->AudioStart (((QWORD) dwParam2 << 32) | dwParam1);
			break;
		case	WM_NSAUDIOSTOP:
			pnsw->AudioStop (((QWORD) dwParam2 << 32) | dwParam1);
			break;
		case	WM_NSVISUAL:
			// not supported in the demo
			break;
		};
	};
}



/************************************************************************
CMode - TTS Mode class. The same code handles all modes (French & English)

   inputs:

      LPUNKNOWN         punkOuter
      LPFNDESTROYED     pfnDestroy
      PITTSNOTIFYSINK   ntNotify
      PIAUDIODEST       ntAudio
*/

CMode::CMode (LPUNKNOWN punkOuter, LPFNDESTROYED pfnDestroy,
              LPUNKNOWN pAudio)
{
// NOTE: When create mode will need other stuff included

   m_cRef                = 0;
   m_punkOuter           = punkOuter;
   m_pfnDestroy          = pfnDestroy;
   m_pModeITTSAttributesW = NULL;
   m_pModeITTSAttributesA = NULL;
   m_pModeITTSCentralW    = NULL;
   m_pModeITTSCentralA    = NULL;
   m_pModeITTSDialogsW    = NULL;
   m_pModeITTSDialogsA    = NULL;
   m_pTextList           = NULL;
   m_pIDList             = NULL;
   m_fSpeaking           = TRUE;
   m_dwVolume            = 0x8000;
	m_pNotifyList			 = NULL;
   m_fClaimed            = FALSE;
   m_fPaused             = FALSE;
   m_fDataPending        = FALSE;

   hWndMain              =
   hWndEdit              = NULL;

   m_pAudioIAudioDest    = NULL;
	m_pAudioIAudio        = NULL;
	m_pAudioIUnknown      = pAudio;
   pAudio->AddRef();

// intialize attributes

// NOTE: Will need to initialize other stuff
}



CMode::~CMode (void)
{
// Free the contained interfaces

	// Unregister
	if (m_pNotifyList) {
		NotificationUnRegisterAll (m_pNotifyList);
		delete m_pNotifyList;
		};

   if (NULL != m_pModeITTSAttributesW) delete m_pModeITTSAttributesW;
   if (NULL != m_pModeITTSAttributesA) delete m_pModeITTSAttributesA;
   if (NULL != m_pModeITTSCentralW   ) delete m_pModeITTSCentralW;
   if (NULL != m_pModeITTSCentralA   ) delete m_pModeITTSCentralA;
   if (NULL != m_pModeITTSDialogsW   ) delete m_pModeITTSDialogsW;
   if (NULL != m_pModeITTSDialogsA   ) delete m_pModeITTSDialogsA;
   if (m_pTextList                  ) delete m_pTextList;
   if (m_pIDList                    ) delete m_pIDList;

   if (hWndMain         ) DestroyWindow (hWndMain);
   if (m_pAudioIAudio)
   	m_pAudioIAudio->PassNotify(NULL, IID_IAudioDestNotifySink);
   if (m_pAudioIAudioDest    ) m_pAudioIAudioDest->Release();
   if (m_pAudioIAudio    ) m_pAudioIAudio->Release();
   if (m_pAudioIUnknown    ) m_pAudioIUnknown->Release();
   if (m_pNotify)
      delete m_pNotify;

// NOTE: Free up other stuff here
}



BOOL CMode::FInit (void)
{
   LPUNKNOWN   pIUnknown = (LPUNKNOWN) this;
   WNDCLASS    wc;
	WAVEFORMATEX	wfex;

   if (NULL != m_punkOuter) pIUnknown = m_punkOuter;

// Talk to the audio object and see if it can do 11 kHz, 8-bit mono
	wfex.wFormatTag = WAVE_FORMAT_PCM;
	wfex.nChannels = 1;
	wfex.nSamplesPerSec = 11025;
	wfex.nAvgBytesPerSec = 11025;
	wfex.nBlockAlign = 1;
	wfex.wBitsPerSample = 8;
	wfex.cbSize = 0;
   if (!FAILED(m_pAudioIUnknown->QueryInterface(IID_IAudio, (VOID**)&m_pAudioIAudio)) &&
		!FAILED (m_pAudioIUnknown->QueryInterface(IID_IAudioDest, (VOID**) &m_pAudioIAudioDest)) ) {
			// see if it like sthe wave-format
			SDATA		sd;

			sd.pData = (PVOID) &wfex;
			sd.dwSize = sizeof(wfex);
			if (FAILED(m_pAudioIAudio->WaveFormatSet (sd))) {
				// It cant handle the wave-format that we want so fail
				return FALSE;
				};
			};


// Allocate all of the contained interfaces

   m_pModeITTSCentralW    = new CModeITTSCentralW (this, pIUnknown);
   m_pModeITTSCentralA    = new CModeITTSCentralA (this, pIUnknown);
   m_pModeITTSDialogsW    = new CModeITTSDialogsW (this, pIUnknown);
   m_pModeITTSDialogsA    = new CModeITTSDialogsA (this, pIUnknown);
   m_pModeITTSAttributesW = new CModeITTSAttributesW (this, pIUnknown);
   m_pModeITTSAttributesA = new CModeITTSAttributesA (this, pIUnknown);
   m_pNotify              = new CModeNotify((void*) this);
   m_pTextList            = new CList();
   m_pIDList              = new CList();
	m_pNotifyList			  = new CList();

// Pass in the notification mode so that it gets notification

   if (m_pAudioIAudioDest && m_pNotify)
   	m_pAudioIAudio->PassNotify((PVOID)m_pNotify, IID_IAudioDestNotifySink);

// create a window to represent the TTS engine.
// Real engines don't do this, but its great for testing

/* create the class */

   memset (&wc, 0, sizeof(wc));
   wc.style = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc = ModeWindowProc;
   wc.hInstance = ghInstance;
   wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
   wc.hCursor = LoadCursor (NULL, MAKEINTRESOURCE(IDC_ARROW));
   wc.hbrBackground = (HBRUSH) (COLOR_BACKGROUND+1);
   wc.lpszMenuName = NULL;
   wc.lpszClassName = "TTSModeClass";
   wc.cbWndExtra = sizeof(long);
   RegisterClass (&wc);

// create the window

   hWndMain = CreateWindow (
      wc.lpszClassName, "Text to Speech Mode", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
      NULL, NULL, ghInstance, (VOID*) this
      );

   ShowWindow (hWndMain, SW_MINIMIZE | SW_SHOW);

// NOTE: Will need to initialize other stuff

   return (m_pModeITTSAttributesW
        && m_pModeITTSAttributesA
        && m_pModeITTSCentralW
        && m_pModeITTSCentralA
        && m_pModeITTSCentralW
        && m_pModeITTSDialogsA
		  && m_pAudioIAudio
		  && m_pAudioIAudioDest
		  && m_pNotifyList
        && m_pNotify && m_pTextList && m_pIDList);

// NOTE: Eventually check for m_pITTSNotifySink and m_pAudioIAudioDest being valid
}



/*************************************************************************
CMode::SendAudioIfCan - This function looks at the amount of free space
   in the digital audio destination, and if there's enough, it does some
   TTS processing (convert text to digital audio) and sends the digital
   audio to the output device.

inputs
   none
returns
   HRESULT - error
*/

HRESULT CMode::SendAudioIfCan(void)
{
   DWORD    dwFree;
   HRESULT  hRes;
   BOOL     bEOF;
   CHAR     c;

   if (!m_fSpeaking) return NOERROR;

   while (TRUE)
      {
      BYTE     bPCM[1024];
      DWORD    i;

// Dont do this unless have data to convert

      if (!m_pTextList->GetNumElems()) return NOERROR;

// Check for sufficient free space at audio dest (& whether we're at end)

      hRes = m_pAudioIAudioDest->FreeSpace(&dwFree, &bEOF);

      if (hRes) return hRes;

      if (bEOF)
         {
         m_fSpeaking = FALSE;                      // stop speaking at end
         m_fDataPending = FALSE;                   // no more data
         return NOERROR;                           // do nothing
         }

// Out minimum block size is 256 samples

      if (dwFree < sizeof(bPCM)) return NOERROR;   // available is too small
//
// A bookmark, here, denotes a character's worth of data.
// When play encounters the bookmark - it notifies the app.
// The test app will print 1 character then.
//
      Get1Char(&c);
      m_pAudioIAudioDest->BookMark((DWORD) c);          // but pMode->BookMark prints

// create a sawtooth wave for demonstration purposed

      for (i = 0; i < sizeof(bPCM); i++) bPCM[i] = (BYTE) (i * 16);

// add it

      hRes = m_pAudioIAudioDest->DataSet(bPCM, sizeof(bPCM));
      if (hRes) return hRes;
      }

   return NOERROR;
}


/*************************************************************************

   Gets 1 char from front of pTextList (i.e. 1st char of 1st Elem)
   removing it from the list
   and returning TRUE,
   unless no more stuff in list: --> FALSE.
*/

BOOL CMode::Get1Char(CHAR *cp)
{
   PWCHAR   pwcsItem;
   QWORD    qwTime;
	PBUFLIST	pbl;

   while (m_pTextList->GetNumElems()
     &&   wcslen(pwcsItem = (PWCHAR) m_pTextList->GetElem(0)) == 0)
      {
      m_pAudioIAudio->TotalGet(&qwTime);
		pbl = (PBUFLIST) m_pIDList->GetElem(0);
		if (pbl->pSinkW) {
			(pbl->pSinkW)->TextDataDone(qwTime, 0);
			(pbl->pSinkW)->Release();
			};

      m_pTextList->RemoveElem(0);
      m_pIDList->RemoveElem(0);

      *cp = '\n';
      return TRUE;
      }

   if (m_pTextList->GetNumElems())
      {
		pbl = (PBUFLIST) m_pIDList->GetElem(0);

      pwcsItem = (PWCHAR) m_pTextList->GetElem(0);
      WideCharToMultiByte(CP_ACP, 0, pwcsItem, -1, cp, 1, NULL, NULL);
      memmove(pwcsItem, pwcsItem + 1, wcslen(pwcsItem) * sizeof(WCHAR));

		// Notify application that buffer has started
		if (!pbl->dwNotifiedStart) {
			pbl->dwNotifiedStart = TRUE;
	      m_pAudioIAudio->TotalGet(&qwTime);
			(pbl->pSinkW)->TextDataStarted (qwTime);
			};

      return TRUE;
      }

   return FALSE;
}


void CMode::Print1Char(HWND hWnd, CHAR ch)
{
   CHAR     c[2];
   INT      iLen;

   c[0]  =  ch;
   c[1]  = '\0';

   iLen = GetWindowTextLength(hWndEdit);
   SendMessage (hWndEdit, EM_SETSEL    , (WPARAM) iLen, (LPARAM) iLen);
   SendMessage (hWndEdit, EM_REPLACESEL, (WPARAM) 0   , (LPARAM) (LPVOID *) c);
}


/*************************************************************************

CMode::SpeakIfNecessary - Call this whenever a buffer is added or the
   speaking flag is changed. This will start up the audio output device
   if necessary, to start speaking, or will pause the audio output device
   to stop.

inputs
   none
returns
   HRESULT- error
*/

HRESULT        CMode::SpeakIfNecessary(void)
{
HRESULT  hRes;

   if (m_fSpeaking)
      {
      hRes = this->SendAudioIfCan();
      if (hRes) return hRes;
      return m_pAudioIAudio->Start();
      }

   else return m_pAudioIAudio->Stop();
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

   if (!hWndEdit)    return;

   iLen = GetWindowTextLength (hWndEdit);
   SendMessage (hWndEdit, EM_SETSEL, (WPARAM) iLen, (LPARAM) iLen);
   SendMessage (hWndEdit, EM_REPLACESEL, (WPARAM) 0, (LPARAM) (LPVOID *) pStr);
}


STDMETHODIMP CMode::QueryInterface (REFIID riid, LPVOID *ppv)
{
   *ppv = NULL;

/* always return our IUnkown for IID_IUnknown */

   if (IsEqualIID (riid, IID_IUnknown)) *ppv = (LPVOID) this;

// other interfaces

   if (IsEqualIID(riid, IID_ITTSAttributesW)) *ppv = m_pModeITTSAttributesW;
   if (IsEqualIID(riid, IID_ITTSCentralW   )) *ppv = m_pModeITTSCentralW;
   if (IsEqualIID(riid, IID_ITTSDialogsW   )) *ppv = m_pModeITTSDialogsW;
   if (IsEqualIID(riid, IID_ITTSAttributesA)) *ppv = m_pModeITTSAttributesA;
   if (IsEqualIID(riid, IID_ITTSCentralA   )) *ppv = m_pModeITTSCentralA;
   if (IsEqualIID(riid, IID_ITTSDialogsA   )) *ppv = m_pModeITTSDialogsA;

// update the reference count

   if (NULL != *ppv) {

      ((LPUNKNOWN) *ppv)->AddRef();
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

// inform destroy that object is going away

   if (0 == m_cRef) {

      if (NULL != m_pfnDestroy) (*m_pfnDestroy)();
      delete this;
      }

   return cRefT;
}


/************************************************************************
CModeITTSCentralW - ITTSCentral inteerface
*/

CModeITTSCentralA::CModeITTSCentralA (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_pObjA = (PCMode)pObj;
   (((PCMode) m_pObjA)->m_pModeITTSCentralW)->FInit(pObj, punkOuter);
}


CModeITTSCentralW::CModeITTSCentralW (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef      = 0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;
}


// This function is called from the ANSI interface constructor -

CModeITTSCentralW::FInit(LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef      = 0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;

   return NOERROR;
}


CModeITTSCentralA::~CModeITTSCentralA (void)
{
   (((PCMode) m_pObjA)->m_pModeITTSCentralW)->~CModeITTSCentralW();
}


CModeITTSCentralW::~CModeITTSCentralW (void)
{
// intentionally left blank
}


STDMETHODIMP CModeITTSCentralA::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return (((PCMode) m_pObjA)->m_pModeITTSCentralW)->QueryInterface(riid, ppv);
}


STDMETHODIMP CModeITTSCentralW::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return m_punkOuter->QueryInterface(riid,ppv);
}


STDMETHODIMP_ (ULONG) CModeITTSCentralA::AddRef(void)
{
   return (((PCMode) m_pObjA)->m_pModeITTSCentralW)->AddRef();
}


STDMETHODIMP_ (ULONG) CModeITTSCentralW::AddRef(void)
{
   ++m_cRef;
   return m_punkOuter->AddRef();
}


STDMETHODIMP_ (ULONG) CModeITTSCentralA::Release(void)
{
   return (((PCMode) m_pObjA)->m_pModeITTSCentralW)->Release();
}


STDMETHODIMP_ (ULONG) CModeITTSCentralW::Release(void)
{
   --m_cRef;
   return m_punkOuter->Release();
}


STDMETHODIMP CModeITTSCentralA::Inject (PCSTR pszInject)
{
   HRESULT  hRes;
   DWORD    cchInject = MyAStrLen(pszInject) + 1;
   PWSTR    pwszInject;

   pwszInject = (PWSTR) malloc(cchInject * sizeof(WCHAR));

   if (pwszInject)
      {
      MultiByteToWideChar(CP_ACP, 0, pszInject, -1, pwszInject, cchInject);
      hRes = (((PCMode) m_pObjA)->m_pModeITTSCentralW)->Inject(pwszInject);
      free(pwszInject);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeITTSCentralW::Inject (PCWSTR pszInject)
{
	// Ignore this because we dont support tags in the demo
   return ResultFromScode (TTSERR_NOTSUPPORTED);
}


STDMETHODIMP CModeITTSCentralA::ModeGet (PTTSMODEINFOA pModeInfo)
{
   HRESULT        hRes;
   TTSMODEINFOW   ModeInfoW;

   hRes = (((PCMode) m_pObjA)->m_pModeITTSCentralW)->ModeGet(&ModeInfoW);

   if (hRes == NOERROR)
      {
      pModeInfo->gEngineID = ModeInfoW.gEngineID;

      WideCharToMultiByte(CP_ACP, 0, ModeInfoW.szMfgName, -1,
                          pModeInfo->szMfgName, TTSI_NAMELEN, NULL, NULL);

      WideCharToMultiByte(CP_ACP, 0, ModeInfoW.szProductName, -1,
                          pModeInfo->szProductName, TTSI_NAMELEN, NULL, NULL);

      pModeInfo->gModeID = ModeInfoW.gModeID;

      WideCharToMultiByte(CP_ACP, 0, ModeInfoW.szModeName, -1,
                          pModeInfo->szModeName, TTSI_NAMELEN, NULL, NULL);

      pModeInfo->language.LanguageID = ModeInfoW.language.LanguageID;

      WideCharToMultiByte(CP_ACP, 0, ModeInfoW.language.szDialect, -1,
                          pModeInfo->language.szDialect, LANG_LEN, NULL, NULL);

      WideCharToMultiByte(CP_ACP, 0, ModeInfoW.szSpeaker, -1,
                          pModeInfo->szSpeaker, TTSI_NAMELEN, NULL, NULL);

      WideCharToMultiByte(CP_ACP, 0, ModeInfoW.szStyle, -1,
                          pModeInfo->szStyle, TTSI_NAMELEN, NULL, NULL);

      pModeInfo->wGender          = ModeInfoW.wGender;
      pModeInfo->wAge             = ModeInfoW.wAge;
      pModeInfo->dwFeatures       = ModeInfoW.dwFeatures;
      pModeInfo->dwInterfaces     = ModeInfoW.dwInterfaces;
      pModeInfo->dwEngineFeatures = ModeInfoW.dwEngineFeatures;
      }

   return hRes;
}


STDMETHODIMP CModeITTSCentralW::ModeGet (PTTSMODEINFOW pModeInfo)
{
   if (!pModeInfo) return ResultFromScode (TTSERR_INVALIDPARAM);

   memcpy(pModeInfo, &((PCMode)m_pObj)->m_TTSModeInfo, sizeof(TTSMODEINFOW));

   return NOERROR;
}


STDMETHODIMP CModeITTSCentralA::Phoneme(VOICECHARSET vc, DWORD dwFlags, SDATA dText,
                                        PSDATA pdPhoneme)
{
   return (((PCMode) m_pObjA)->m_pModeITTSCentralW)->Phoneme(vc, dwFlags, dText,
                                                             pdPhoneme);
}


STDMETHODIMP CModeITTSCentralW::Phoneme(VOICECHARSET vc, DWORD dwFlags, SDATA dText,
                                        PSDATA pdPhoneme)
{
   return ResultFromScode(TTSERR_NOTSUPPORTED);
}


STDMETHODIMP CModeITTSCentralA::PosnGet(PQWORD pqTime)
{
   return (((PCMode) m_pObjA)->m_pModeITTSCentralW)->PosnGet(pqTime);
}


STDMETHODIMP CModeITTSCentralW::PosnGet(PQWORD pqTime)
{
   PCMode  pMode = (PCMode) m_pObj;

   return pMode->m_pAudioIAudio->PosnGet (pqTime);
}


STDMETHODIMP CModeITTSCentralA::TextData(VOICECHARSET eCharacterSet, DWORD dwFlags,
                                      SDATA dText, PVOID pNotifyInterface, IID iid)
{
   // convert from ANSI to unicode
   int   iRet;
   // create a temporary buffer
   SDATA    sTemp;

   sTemp.dwSize = dText.dwSize * sizeof(WCHAR);
   sTemp.pData = malloc(sTemp.dwSize);
   if (!sTemp.pData)
      return ResultFromScode(E_OUTOFMEMORY);
   iRet = MultiByteToWideChar(CP_ACP, 0, (CHAR*)dText.pData, -1,
      (WCHAR*)sTemp.pData, sTemp.dwSize / sizeof(WCHAR));
   if (iRet <= 0)
      return ResultFromScode (E_FAIL);
   sTemp.dwSize = (DWORD) iRet * sizeof(WCHAR);

   HRESULT  hRes;
   hRes = (((PCMode) m_pObjA)->m_pModeITTSCentralW)->TextData(eCharacterSet,
                                        dwFlags, sTemp, pNotifyInterface, iid);
   free (sTemp.pData);
   return hRes;
}


STDMETHODIMP CModeITTSCentralW::TextData(VOICECHARSET eCharacterSet, DWORD dwFlags,
                                         SDATA dText, PVOID pNotifyInterface, IID iid)
{
   PCMode  pMode = (PCMode) m_pObj;
	BUFLIST	bl;
   HRESULT  hRes;

// allow only UNICODETEXT

   if (eCharacterSet != CHARSET_TEXT)
      return ResultFromScode(TTSERR_INVALIDCHAR);
	if (pNotifyInterface && !IsEqualIID(iid, IID_ITTSBufNotifySinkW))
		return ResultFromScode (TTSERR_INVALIDINTERFACE);
	// NOTE: support ANSI version eventually

// add to text to linked list

   if (!pMode->m_pTextList->AddElem(dText.pData, dText.dwSize))

      return ResultFromScode(E_OUTOFMEMORY);

	// Store the notify sink away
	memset (&bl, 0, sizeof(bl));
	if (IsEqualIID(iid, IID_ITTSBufNotifySinkW))
		bl.pSinkW = (PITTSBUFNOTIFYSINKW) pNotifyInterface;
	if (!pMode->m_pIDList->AddElem (&bl, sizeof(bl)))
      return ResultFromScode(E_OUTOFMEMORY);

   // Claim the device
   if (!pMode->m_fClaimed && !pMode->m_fPaused) {
      hRes = pMode->m_pAudioIAudio->Claim();
      if (hRes)
         return hRes;
      pMode->m_fClaimed = TRUE;
      hRes = pMode->m_pAudioIAudio->Start();
      if (hRes)
         return hRes;
      pMode->m_fDataPending = TRUE;
      }
   else if (!pMode->m_fClaimed && pMode->m_fPaused) {
      pMode->m_fDataPending = TRUE;
      }
   else if (pMode->m_fClaimed && !pMode->m_fPaused) {
      pMode->m_fDataPending = TRUE;
      }
   else {
      // Bad error
      return ResultFromScode(E_FAIL);
      };

   return pMode->SendAudioIfCan();
}


STDMETHODIMP CModeITTSCentralA::ToFileTime(PQWORD pqTime, FILETIME * ft)
{
   return (((PCMode) m_pObjA)->m_pModeITTSCentralW)->ToFileTime(pqTime, ft);
}


STDMETHODIMP CModeITTSCentralW::ToFileTime(PQWORD pqTime, FILETIME * ft)
{
   PCMode  pMode = (PCMode) m_pObj;

   return pMode->m_pAudioIAudio->ToFileTime (pqTime, ft);
}


STDMETHODIMP CModeITTSCentralA::AudioPause(void)
{
   return (((PCMode) m_pObjA)->m_pModeITTSCentralW)->AudioPause();
}


STDMETHODIMP CModeITTSCentralW::AudioPause(void)
{
PCMode  pMode = (PCMode) m_pObj;
HRESULT  hRes;

if (pMode->m_fPaused) {
   return ResultFromScode(TTSERR_ALREADYPAUSED);
   }
else if (!pMode->m_fPaused && pMode->m_fClaimed) {
   pMode->m_pAudioIAudio->Stop();
   pMode->m_pAudioIAudio->UnClaim();
   pMode->m_fClaimed = FALSE;
   pMode->m_fPaused = TRUE;
   }
else if (!pMode->m_fPaused && !pMode->m_fClaimed) {
   pMode->m_pAudioIAudio->Stop();
   pMode->m_fPaused = TRUE;
   };
pMode->m_fSpeaking = FALSE;
hRes = pMode->SpeakIfNecessary();
return hRes;
}


STDMETHODIMP CModeITTSCentralA::AudioReset(void)
{
   return (((PCMode) m_pObjA)->m_pModeITTSCentralW)->AudioReset();
}


STDMETHODIMP CModeITTSCentralW::AudioReset(void)
{
   PCMode   pMode = (PCMode) m_pObj;
   QWORD    qwTime;
	PBUFLIST	pbl;
   // HRESULT  hRes;

// Stop the audio
   if (!pMode->m_fClaimed && !pMode->m_fPaused) {
      pMode->m_pAudioIAudio->Stop();
      pMode->m_pAudioIAudio->Flush();
      }
   else if (pMode->m_fClaimed && !pMode->m_fPaused) {
      pMode->m_pAudioIAudio->Stop();
      pMode->m_pAudioIAudio->Flush();
      pMode->m_pAudioIAudio->UnClaim();
      pMode->m_fClaimed = pMode->m_fPaused = pMode->m_fDataPending = FALSE;
      }
   else if (!pMode->m_fClaimed && pMode->m_fPaused) {
      pMode->m_pAudioIAudio->Flush();
      pMode->m_fClaimed = pMode->m_fPaused = pMode->m_fDataPending = FALSE;
      }
   else {
      // shouldn't happen
      return ResultFromScode (E_FAIL);
      };

// for (i = 0; i < pList->GetNumElems(); i++)
//    pList->SetElem(i, "", 1);

   while (pMode->m_pTextList->GetNumElems())
      {
      pMode->m_pAudioIAudio->TotalGet(&qwTime);

		// Notify the app that the buffer is about to be released forcefully
		pbl = (PBUFLIST) pMode->m_pIDList->GetElem(0);
		if (pbl->pSinkW) {
			(pbl->pSinkW)->TextDataDone(qwTime, TTSBNS_ABORTED);
			(pbl->pSinkW)->Release();
			};

      pMode->m_pTextList->RemoveElem(0);
      pMode->m_pIDList->RemoveElem(0);
      }

   return NOERROR;
}


STDMETHODIMP CModeITTSCentralA::AudioResume(void)
{
   return (((PCMode) m_pObjA)->m_pModeITTSCentralW)->AudioResume();
}


STDMETHODIMP CModeITTSCentralW::AudioResume(void)
{
PCMode  pMode = (PCMode) m_pObj;
HRESULT  hRes;

if (!pMode->m_fPaused)
   return ResultFromScode(TTSERR_NOTPAUSED);
else if (pMode->m_fPaused && !pMode->m_fClaimed) {
      hRes = pMode->m_pAudioIAudio->Claim();
      if (hRes)
         return hRes;
      pMode->m_fClaimed = TRUE;
      pMode->m_fPaused = FALSE;
      hRes = pMode->m_pAudioIAudio->Start();
      if (hRes)
         return hRes;
      
      // If there's not data then just call unclaim
      if (!pMode->m_fDataPending) {
         pMode->m_fClaimed = FALSE;
         return pMode->m_pAudioIAudio->UnClaim();
         };
   }
else {
   // this shouldn't happen
   return ResultFromScode (E_FAIL);
   };

pMode->m_fSpeaking = TRUE;
hRes = pMode->SpeakIfNecessary();
return hRes;
}


STDMETHODIMP CModeITTSCentralA::Register(PVOID pInterface, IID iid, DWORD *pdwKey)
{
   return (((PCMode) m_pObjA)->m_pModeITTSCentralW)->Register(pInterface, iid,
                                                              pdwKey);
}


STDMETHODIMP CModeITTSCentralW::Register(PVOID pInterface, IID iid, DWORD *pdwKey)
{
PCMode  pMode = (PCMode) m_pObj;

if (!IsEqualIID (iid, IID_ITTSNotifySinkW))
	return ResultFromScode (TTSERR_INVALIDINTERFACE);
// NOTE: Eventually accept an ANSI interface

*pdwKey = NotificationRegister (pMode->m_pNotifyList, pInterface, iid);
if (!(*pdwKey))
	return ResultFromScode (E_OUTOFMEMORY);
else
	return NOERROR;
}


STDMETHODIMP CModeITTSCentralA::UnRegister(DWORD dwKey)
{
   return (((PCMode) m_pObjA)->m_pModeITTSCentralW)->UnRegister(dwKey);
}


STDMETHODIMP CModeITTSCentralW::UnRegister(DWORD dwKey)
{
PCMode  pMode = (PCMode) m_pObj;

if (NotificationUnRegister (pMode->m_pNotifyList, dwKey))
	return ResultFromScode (TTSERR_INVALIDKEY);
else
	return NOERROR;
}


/************************************************************************
CModeITTSAttributesW - ITTSCentral inteerface
*/

CModeITTSAttributesA::CModeITTSAttributesA (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_pObjA = (PCMode)pObj;
   (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->FInit(pObj, punkOuter);
}


CModeITTSAttributesW::CModeITTSAttributesW (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef      = 0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;
}


// This function is called from the ANSI interface constructor -

CModeITTSAttributesW::FInit(LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef      = 0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;

   return NOERROR;
}


CModeITTSAttributesA::~CModeITTSAttributesA (void)
{
   (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->~CModeITTSAttributesW();
}


CModeITTSAttributesW::~CModeITTSAttributesW (void)
{
}


STDMETHODIMP CModeITTSAttributesA::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->QueryInterface(riid, ppv);
}


STDMETHODIMP CModeITTSAttributesW::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return m_punkOuter->QueryInterface(riid,ppv);
}


STDMETHODIMP_ (ULONG) CModeITTSAttributesA::AddRef(void)
{
   return (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->AddRef();
}


STDMETHODIMP_ (ULONG) CModeITTSAttributesW::AddRef(void)
{
   ++m_cRef;
   return m_punkOuter->AddRef();
}


STDMETHODIMP_ (ULONG) CModeITTSAttributesA::Release(void)
{
   return (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->Release();
}


STDMETHODIMP_ (ULONG) CModeITTSAttributesW::Release(void)
{
   --m_cRef;
   return m_punkOuter->Release();
}


STDMETHODIMP CModeITTSAttributesA::PitchGet(WORD *pdwPitch)
{
   return (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->PitchGet(pdwPitch);
}


STDMETHODIMP CModeITTSAttributesW::PitchGet(WORD *pdwPitch)
{
   *pdwPitch = ((PCMode)m_pObj)->m_wPitch;
   return NOERROR;
}


STDMETHODIMP CModeITTSAttributesA::PitchSet(WORD dwPitch)
{
   return (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->PitchSet(dwPitch);
}


STDMETHODIMP CModeITTSAttributesW::PitchSet(WORD dwPitch)
{
   ((PCMode)m_pObj)->m_wPitch = dwPitch;
	PostMessage (((PCMode)m_pObj)->hWndMain, WM_NSATTRIBCHANGED, TTSNSAC_PITCH, 0);
   return NOERROR;
}


STDMETHODIMP CModeITTSAttributesA::RealTimeGet(DWORD *pdwReal)
{
   return (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->RealTimeGet(pdwReal);
}


STDMETHODIMP CModeITTSAttributesW::RealTimeGet(DWORD *pdwReal)
{
   *pdwReal = ((PCMode)m_pObj)->m_dwRealTime;
   return NOERROR;
}


STDMETHODIMP CModeITTSAttributesA::RealTimeSet(DWORD dwReal)
{
   return (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->RealTimeSet(dwReal);
}


STDMETHODIMP CModeITTSAttributesW::RealTimeSet(DWORD dwReal)
{
   ((PCMode)m_pObj)->m_dwRealTime = dwReal;
	PostMessage (((PCMode)m_pObj)->hWndMain, WM_NSATTRIBCHANGED, TTSNSAC_REALTIME, 0);
   return NOERROR;
}


STDMETHODIMP CModeITTSAttributesA::SpeedGet(DWORD *pdwSpeed)
{
   return (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->SpeedGet(pdwSpeed);
}


STDMETHODIMP CModeITTSAttributesW::SpeedGet(DWORD *pdwSpeed)
{
   *pdwSpeed = ((PCMode)m_pObj)->m_dwSpeed;
   return NOERROR;
}


STDMETHODIMP CModeITTSAttributesA::SpeedSet(DWORD dwSpeed)
{
   return (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->SpeedSet(dwSpeed);
}


STDMETHODIMP CModeITTSAttributesW::SpeedSet(DWORD dwSpeed)
{
   ((PCMode)m_pObj)->m_dwSpeed = dwSpeed;
	PostMessage (((PCMode)m_pObj)->hWndMain, WM_NSATTRIBCHANGED, TTSNSAC_SPEED, 0);

   return NOERROR;
}


STDMETHODIMP CModeITTSAttributesA::VolumeGet(DWORD *pdwVolume)
{
   return (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->VolumeGet(pdwVolume);
}


STDMETHODIMP CModeITTSAttributesW::VolumeGet(DWORD *pdwVolume)
{
   if (((PCMode)m_pObj)->m_pAudioIAudio->LevelGet(pdwVolume))

      return ResultFromScode (TTSERR_NOTSUPPORTED);

   return NOERROR;
}


STDMETHODIMP CModeITTSAttributesA::VolumeSet(DWORD dwVolume)
{
   return (((PCMode) m_pObjA)->m_pModeITTSAttributesW)->VolumeSet(dwVolume);
}


STDMETHODIMP CModeITTSAttributesW::VolumeSet(DWORD dwVolume)
{
	PostMessage (((PCMode)m_pObj)->hWndMain, WM_NSATTRIBCHANGED, TTSNSAC_VOLUME, 0);
   if (((PCMode)m_pObj)->m_pAudioIAudio->LevelSet(dwVolume))

      return ResultFromScode (TTSERR_NOTSUPPORTED);

   return NOERROR;
}


/************************************************************************
CModeITTSDialogsW - ITTSDialogs interface
*/

CModeITTSDialogsA::CModeITTSDialogsA(LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_pObjA = (PCMode)pObj;
   (((PCMode) m_pObjA)->m_pModeITTSDialogsW)->FInit(pObj, punkOuter);
}


CModeITTSDialogsW::CModeITTSDialogsW (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef      = 0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;
}


// This function is called from the ANSI interface constructor -

CModeITTSDialogsW::FInit(LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef      = 0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;

   return NOERROR;
}


CModeITTSDialogsA::~CModeITTSDialogsA (void)
{
   (((PCMode) m_pObjA)->m_pModeITTSDialogsW)->~CModeITTSDialogsW();
}


CModeITTSDialogsW::~CModeITTSDialogsW (void)
{
// intentionally left blank
}


STDMETHODIMP CModeITTSDialogsA::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return (((PCMode) m_pObjA)->m_pModeITTSDialogsW)->QueryInterface(riid, ppv);
}


STDMETHODIMP CModeITTSDialogsW::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return m_punkOuter->QueryInterface(riid,ppv);
}


STDMETHODIMP_ (ULONG) CModeITTSDialogsA::AddRef(void)
{
   return (((PCMode) m_pObjA)->m_pModeITTSDialogsW)->AddRef();
}


STDMETHODIMP_ (ULONG) CModeITTSDialogsW::AddRef(void)
{
   ++m_cRef;
   return m_punkOuter->AddRef();
}


STDMETHODIMP_ (ULONG) CModeITTSDialogsA::Release(void)
{
   return (((PCMode) m_pObjA)->m_pModeITTSDialogsW)->Release();
}


STDMETHODIMP_ (ULONG) CModeITTSDialogsW::Release(void)
{
   --m_cRef;
   return m_punkOuter->Release();
}


STDMETHODIMP CModeITTSDialogsA::AboutDlg (HWND hWndParent, PCSTR pszTitle)
{
   HRESULT  hRes;
   DWORD    cchTitle = MyAStrLen(pszTitle) + 1;
   PWSTR    pwszTitle;

   pwszTitle = (PWSTR) malloc(cchTitle * sizeof(WCHAR));

   if (pwszTitle)
      {
      MultiByteToWideChar(CP_ACP, 0, pszTitle, -1, pwszTitle, cchTitle);
      hRes = (((PCMode) m_pObjA)->m_pModeITTSDialogsW)->AboutDlg(hWndParent, pwszTitle);
      free(pwszTitle);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeITTSDialogsW::AboutDlg (HWND hWndParent, PCWSTR pszTitle)
{
   char  szTemp[256];

// a valid handle?

   if (!hWndParent || !IsWindow(hWndParent))

      return ResultFromScode (SRERR_INVALIDPARAM);

// convert from UNICODE to ANSI

   if (pszTitle)

      WideCharToMultiByte(CP_ACP, 0, pszTitle, -1, szTemp, sizeof(szTemp), NULL, NULL);

// Do a simple dialog for this example

   MessageBox (hWndParent, "About dialog box", pszTitle ? szTemp : NULL, MB_OK);
   return NOERROR;
}


STDMETHODIMP CModeITTSDialogsA::LexiconDlg (HWND hWndParent, PCSTR pszTitle)
{
   HRESULT  hRes;
   DWORD    cchTitle = MyAStrLen(pszTitle) + 1;
   PWSTR    pwszTitle;

   pwszTitle = (PWSTR) malloc(cchTitle * sizeof(WCHAR));

   if (pwszTitle)
      {
      MultiByteToWideChar(CP_ACP, 0, pszTitle, -1, pwszTitle, cchTitle);
      hRes = (((PCMode) m_pObjA)->m_pModeITTSDialogsW)->LexiconDlg(hWndParent, pwszTitle);
      free(pwszTitle);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeITTSDialogsW::LexiconDlg (HWND hWndParent, PCWSTR pszTitle)
{
   char  szTemp[256];

// a valid handle?

   if (!hWndParent || !IsWindow(hWndParent))

      return ResultFromScode (SRERR_INVALIDPARAM);

// convert from UNICODE to ANSI

   if (pszTitle)

      WideCharToMultiByte(CP_ACP, 0, pszTitle, -1,
                          szTemp, sizeof(szTemp), NULL, NULL);

// Do a simple dialog for this example

   MessageBox (hWndParent, "Lexicon dialog box", pszTitle ? szTemp : NULL, MB_OK);
   return NOERROR;
}


STDMETHODIMP CModeITTSDialogsA::GeneralDlg (HWND hWndParent, PCSTR pszTitle)
{
   HRESULT  hRes;
   DWORD    cchTitle = MyAStrLen(pszTitle) + 1;
   PWSTR    pwszTitle;

   pwszTitle = (PWSTR) malloc(cchTitle * sizeof(WCHAR));

   if (pwszTitle)
      {
      MultiByteToWideChar(CP_ACP, 0, pszTitle, -1, pwszTitle, cchTitle);
      hRes = (((PCMode) m_pObjA)->m_pModeITTSDialogsW)->GeneralDlg(hWndParent, pwszTitle);
      free(pwszTitle);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeITTSDialogsW::GeneralDlg (HWND hWndParent, PCWSTR pszTitle)
{
   char  szTemp[256];

// a valid handle?

   if (!hWndParent || !IsWindow(hWndParent))

      return ResultFromScode (SRERR_INVALIDPARAM);

// convert from UNICODE to ANSI

   if (pszTitle)

      WideCharToMultiByte(CP_ACP, 0, pszTitle, -1,
                          szTemp, sizeof(szTemp), NULL, NULL);

// Do a simple dialog for this example

   MessageBox (hWndParent, "General dialog box", pszTitle ? szTemp : NULL, MB_OK);
   return NOERROR;
}


STDMETHODIMP CModeITTSDialogsA::TranslateDlg (HWND hWndParent, PCSTR pszTitle)
{
   HRESULT  hRes;
   DWORD    cchTitle = MyAStrLen(pszTitle) + 1;
   PWSTR    pwszTitle;

   pwszTitle = (PWSTR) malloc(cchTitle * sizeof(WCHAR));

   if (pwszTitle)
      {
      MultiByteToWideChar(CP_ACP, 0, pszTitle, -1, pwszTitle, cchTitle);
      hRes = (((PCMode) m_pObjA)->m_pModeITTSDialogsW)->TranslateDlg(hWndParent, pwszTitle);
      free(pwszTitle);
      return(hRes);
      }

   else
   
      return ResultFromScode (E_OUTOFMEMORY);
}


STDMETHODIMP CModeITTSDialogsW::TranslateDlg (HWND hWndParent, PCWSTR pszTitle)
{
   char  szTemp[256];

// a valid handle?

   if (!hWndParent || !IsWindow(hWndParent))

      return ResultFromScode (SRERR_INVALIDPARAM);

// convert from UNICODE to ANSI

   if (pszTitle)

      WideCharToMultiByte(CP_ACP, 0, pszTitle, -1,
                          szTemp, sizeof(szTemp), NULL, NULL);

// Do a simple dialog for this example

   MessageBox (hWndParent, "Translate dialog box", pszTitle ? szTemp : NULL, MB_OK);
   return NOERROR;
}


/***********************************************************************
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
   PCMode pMode = (PCMode) m_pObj;

   return pMode->SendAudioIfCan();           // add to text to linked list
}



STDMETHODIMP CModeNotify::BookMark(DWORD dwMarkID, BOOL fFlush)
{
   PCMode pMode = (PCMode) m_pObj;

   pMode->Print1Char(pMode->hWndEdit, (CHAR) dwMarkID);

   return NOERROR;
}

STDMETHODIMP CModeNotify::AudioStop(WORD wReason)
{
	QWORD	qwTime;

	((PCMode)m_pObj)->m_pAudioIAudio->TotalGet (&qwTime);
	PostMessage (((PCMode)m_pObj)->hWndMain, WM_NSAUDIOSTOP,
		 (DWORD) qwTime, (DWORD) (qwTime >> 32));
   return NOERROR;
}


STDMETHODIMP CModeNotify::AudioStart(void)
{
	QWORD	qwTime;

	((PCMode)m_pObj)->m_pAudioIAudio->TotalGet (&qwTime);
	PostMessage (((PCMode)m_pObj)->hWndMain, WM_NSAUDIOSTART,
		 (DWORD) qwTime, (DWORD) (qwTime >> 32));
   return NOERROR;
}
