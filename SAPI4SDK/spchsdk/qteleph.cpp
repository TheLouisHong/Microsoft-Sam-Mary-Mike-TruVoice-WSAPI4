/*********************************************************************
Teleph.Cpp - Telephony C++ wrapper objects.

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
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include "spchwrap.h"


/**************************************************************************
TelInfo */

CTelInfo::CTelInfo (void)
{
   m_pITelInfo = NULL;
}

CTelInfo::~CTelInfo (void)
{
   if (m_pITelInfo)
      m_pITelInfo->Release();
}

// Init
HRESULT  CTelInfo::Init (void)
{
   if (m_pITelInfo)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   hRes = CoCreateInstance (CLSID_TelInfo, NULL, CLSCTX_ALL,
      IID_ITelInfo, (void**)&m_pITelInfo);
   if (hRes)
      return hRes;

   return NOERROR;
}

HRESULT  CTelInfo::Init (LPUNKNOWN lpUnk)
{
   if (m_pITelInfo)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   hRes = lpUnk->QueryInterface (IID_ITelInfo, (PVOID*)&m_pITelInfo);
   if (hRes)
      return hRes;
   return hRes;
}


// Init, type set, and quick create
HRESULT  CTelInfo::Init (DWORD dwType, PSRMODEINFOW pSRModeInfo,
   PTTSMODEINFOW pTTSModeInfo, PCWSTR pszWave,
   HMODULE hModApp)
{
   HRESULT  hRes;
   hRes = Init();
   if (hRes)
      return hRes;

   hRes = TypeSet (dwType);
   if (hRes)
      return hRes;

   hRes = QuickCreate (hModApp, pSRModeInfo, pTTSModeInfo, pszWave);
   if (hRes)
      return hRes;

   return NOERROR;
}


// ITelInfo
HRESULT  CTelInfo::ObjectGet (GUID gObject, LPUNKNOWN *ppIUnk)
{
   if (m_pITelInfo)
      return m_pITelInfo->ObjectGet (gObject, ppIUnk);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelInfo::ObjectGet (GUID gObject, IID iid, PVOID* ppI)
{
   HRESULT  hRes;
   LPUNKNOWN   lpUnk;
   hRes = ObjectGet (gObject, &lpUnk);
   if (hRes)
      return hRes;

   hRes = lpUnk->QueryInterface (iid, ppI);
   lpUnk->Release();
   if (hRes) {
      return hRes;
   }

   return hRes;
}

HRESULT  CTelInfo::ObjectSet (GUID gObject, LPUNKNOWN pIUnk)
{
   if (m_pITelInfo)
      return m_pITelInfo->ObjectSet (gObject, pIUnk);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelInfo::DWORDGet (GUID gDWORD, DWORD *pdwValue)
{
   if (m_pITelInfo)
      return m_pITelInfo->DWORDGet (gDWORD, pdwValue);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CTelInfo::DWORDGet (GUID gDWORD)
{
   DWORD dw = 0;
   DWORDGet (gDWORD, &dw);
   return dw;
}

HRESULT  CTelInfo::DWORDSet (GUID gDWORD, DWORD dwValue)
{
   if (m_pITelInfo)
      return m_pITelInfo->DWORDSet (gDWORD, dwValue);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelInfo::MemoryGet (GUID gMem, PVOID *ppMem, DWORD *pdwSize)
{
   if (m_pITelInfo)
      return m_pITelInfo->MemoryGet (gMem, ppMem, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelInfo::MemorySet (GUID gMem, PVOID pMem, DWORD dwSize)
{
   if (m_pITelInfo)
      return m_pITelInfo->MemorySet (gMem, pMem, dwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelInfo::SendDTMF (WCHAR cDTMF)
{
   if (m_pITelInfo)
      return m_pITelInfo->SendDTMF (cDTMF);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelInfo::SendAbort (DWORD dwVal)
{
   if (m_pITelInfo)
      return m_pITelInfo->SendAbort (dwVal);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelInfo::TypeSet (DWORD dwType)
{
   if (m_pITelInfo)
      return m_pITelInfo->TypeSet (dwType);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelInfo::AudioSourceCreate (LPUNKNOWN *ppIUnk)
{
   if (m_pITelInfo)
      return m_pITelInfo->AudioSourceCreate (ppIUnk);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelInfo::AudioDestCreate (LPUNKNOWN *ppIUnk)
{
   if (m_pITelInfo)
      return m_pITelInfo->AudioDestCreate (ppIUnk);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelInfo::QuickCreate (HMODULE hModApp, PSRMODEINFOW pSRModeInfo,
   PTTSMODEINFOW pTTSModeInfo, PCWSTR pszWave, BOOL fUseLogging)
{
   if (m_pITelInfo)
      return m_pITelInfo->QuickCreate (hModApp, pSRModeInfo, pTTSModeInfo,
                                       pszWave, fUseLogging);
   else
      return ResultFromScode (E_NOINTERFACE);
}


HRESULT  CTelInfo::QuickCreate (HMODULE hModApp, PSRMODEINFOW pSRModeInfo,
   PTTSMODEINFOW pTTSModeInfo, DWORD dwResourceID, BOOL fUseLogging)
{
   // if there are no resources specified then don't care
   if (!dwResourceID)
      return QuickCreate (hModApp, pSRModeInfo, pTTSModeInfo, (PCWSTR) NULL, fUseLogging);

   // else, there are resources, so load them
   HRSRC hRsrc;

   hRsrc = FindResource(hModApp, MAKEINTRESOURCE(dwResourceID), "TEXT");
   if (!hRsrc)
      return E_FAIL;

   HGLOBAL hMem;
   hMem = LoadResource (hModApp, hRsrc);
   if (!hMem)
      return E_FAIL;

   // loock the memory
   PVOID pMem;
   pMem = LockResource(hMem);
   if (!pMem) {
      FreeResource (hRsrc);
      return E_OUTOFMEMORY;
   }

   // call the other WaveAddFromList
   HRESULT  hRes;
   hRes = QuickCreate (hModApp, pSRModeInfo, pTTSModeInfo, (PCWSTR) pMem, fUseLogging);

   // Free the resource
   FreeResource (hRsrc);

   return hRes;
}


PCSRMode CTelInfo::SRModeGet (void)
{
   LPUNKNOWN   pUnk;
   HRESULT     hRes;

   hRes = ObjectGet (TELOBJ_SPEECHRECOG, &pUnk);
   if (hRes)
      return NULL;

   PCSRMode pMode;
   pMode = new CSRMode;
   if (!pMode) {
      pUnk->Release();
      return NOERROR;
   }

   hRes = pMode->Init (pUnk);
   pUnk->Release();
   if (hRes) {
      delete pMode;
      return NULL;
   }

   return pMode;
}

PCSTTTSQueue CTelInfo::STTTSQueueGet (void)
{
   LPUNKNOWN   pUnk;
   HRESULT     hRes;

   hRes = ObjectGet (TELOBJ_TTSQUEUE, &pUnk);
   if (hRes)
      return NULL;

   PCSTTTSQueue pMode;
   pMode = new CSTTTSQueue;
   if (!pMode) {
      pUnk->Release();
      return NOERROR;
   }

   hRes = pMode->Init (pUnk);
   pUnk->Release();
   if (hRes) {
      delete pMode;
      return NULL;
   }

   return pMode;
}

PCSTLog  CTelInfo::STLogGet (void)
{
   LPUNKNOWN   pUnk;
   HRESULT     hRes;

   hRes = ObjectGet (TELOBJ_LOGGING, &pUnk);
   if (hRes)
      return NULL;

   PCSTLog pMode;
   pMode = new CSTLog;
   if (!pMode) {
      pUnk->Release();
      return NOERROR;
   }

   hRes = pMode->Init (pUnk);
   pUnk->Release();
   if (hRes) {
      delete pMode;
      return NULL;
   }

   return pMode;
}

PITELNOTIFYSINK CTelInfo::TelNotifySinkGet (void)
{
   PITELNOTIFYSINK   pITel;
   HRESULT  hRes;

   hRes = ObjectGet (TELOBJ_NOTIFYSINK, IID_ITelNotifySink, (void**) &pITel);
   if (hRes)
      return NULL;

   return pITel;
}

HRESULT CTelInfo::TelNotifySinkSet (PITELNOTIFYSINK pITel)
{
   return ObjectSet (TELOBJ_NOTIFYSINK, pITel);
}


HRESULT           CTelInfo::EnableOperatorSet (BOOL f)
{
   return DWORDSet(TELDWORD_EnableOperator, (DWORD) f);
}

BOOL              CTelInfo::EnableOperatorGet (void)
{
   return (BOOL) DWORDGet (TELDWORD_EnableOperator);
}

HRESULT           CTelInfo::DisableSpeedChangeSet (BOOL f)
{
   return DWORDSet(TELDWORD_DisableSpeedChange, (DWORD) f);
}

BOOL              CTelInfo::DisableSpeedChangeGet (void)
{
   return (BOOL) DWORDGet (TELDWORD_DisableSpeedChange);
}

HRESULT           CTelInfo::EnableAskHangUpSet (BOOL f)
{
   return DWORDSet(TELDWORD_EnableAskHangUp, (DWORD) f);
}

BOOL              CTelInfo::EnableAskHangUpGet (void)
{
   return (BOOL) DWORDGet (TELDWORD_EnableAskHangUp);
}


/**************************************************************************
Telephony control */

CTelControl::CTelControl (void)
{
   m_pITelControl = NULL;
   m_dwResultFinish = 0;
   m_pMemFinish = 0;
   m_dwSizeFinish = 0;
   m_fFinish = TRUE;
}

CTelControl::~CTelControl (void)
{
   if (m_pITelControl)
      m_pITelControl->Release();
   if (m_pMemFinish)
      CoTaskMemFree (m_pMemFinish);
}

// Init
HRESULT  CTelControl::Init (CLSID clsid)
{
   if (m_pITelControl)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   hRes = CoCreateInstance (clsid, NULL, CLSCTX_ALL,
      IID_ITelControl, (void**)&m_pITelControl);
   if (hRes)
      return hRes;

   return NOERROR;
}

HRESULT  CTelControl::Init (LPUNKNOWN lpUnk)
{
   if (m_pITelControl)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   hRes = lpUnk->QueryInterface (IID_ITelControl, (PVOID*)&m_pITelControl);
   if (hRes)
      return hRes;
   return hRes;
}

// Initialization that also does ObjectSet
HRESULT  CTelControl::Init (CLSID clsid, PITELINFO pITelInfo)
{
   HRESULT  hRes;
   hRes = Init (clsid);
   if (hRes)
      return hRes;
   hRes = ObjectSet (pITelInfo);
   if (hRes)
      return hRes;
   return NOERROR;
}

HRESULT  CTelControl::Init (CLSID clsid, PCTelInfo pCTelInfo)
{
   return Init (clsid, pCTelInfo->m_pITelInfo);
}

// ITelControl
HRESULT  CTelControl::FromMemory (PVOID pMem, DWORD dwSize)
{
   if (m_pITelControl)
      return m_pITelControl->FromMemory (pMem, dwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::FromStream (IStream *pIStream)
{
   if (m_pITelControl)
      return m_pITelControl->FromStream (pIStream);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::FromFile (PCWSTR pszFile)
{
   if (m_pITelControl)
      return m_pITelControl->FromFile (pszFile);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CTelControl::FromResource (PVOID hInst, DWORD dwResID)
#else
HRESULT  CTelControl::FromResource (HINSTANCE hInst, DWORD dwResID)
#endif
{
   if (m_pITelControl)
      return m_pITelControl->FromResource ((HINSTANCE)hInst, dwResID);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::Compile (LPWSTR *ppszErr, DWORD *pdwErr)
{
   if (m_pITelControl)
      return m_pITelControl->Compile (ppszErr, pdwErr);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::IsCompiled (BOOL *pfCompiled)
{
   if (m_pITelControl)
      return m_pITelControl->IsCompiled (pfCompiled);
   else
      return ResultFromScode (E_NOINTERFACE);
}

BOOL     CTelControl::IsCompiled (void)
{
   BOOL  f = FALSE;
   IsCompiled (&f);
   return f;
}

HRESULT  CTelControl::LanguageGet (LANGUAGEW *pLang)
{
   if (m_pITelControl)
      return m_pITelControl->LanguageGet (pLang);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::ToMemory (PVOID *ppMem, DWORD *pdwSize)
{
   if (m_pITelControl)
      return m_pITelControl->ToMemory (ppMem, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::ToStream (IStream *pIStream)
{
   if (m_pITelControl)
      return m_pITelControl->ToStream (pIStream);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::ToFile (PCWSTR pszFile)
{
   if (m_pITelControl)
      return m_pITelControl->ToFile (pszFile);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::TextGet (LPWSTR *ppszText, DWORD *pdwSize)
{
   if (m_pITelControl)
      return m_pITelControl->TextGet (ppszText, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::TextDefaultGet (LPWSTR *ppszText, DWORD *pdwSize)
{
   if (m_pITelControl)
      return m_pITelControl->TextDefaultGet (ppszText, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::ObjectSet (PITELINFO pITelInfo)
{
   if (m_pITelControl)
      return m_pITelControl->ObjectSet (pITelInfo);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::Start (PITELCONTROLNOTIFYSINK pITelControlNotifySink)
{
   if (m_pITelControl)
      return m_pITelControl->Start (pITelControlNotifySink);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTelControl::Abort (void)
{
   if (m_pITelControl)
      return m_pITelControl->Abort ();
   else
      return ResultFromScode (E_NOINTERFACE);
}


// Load & compile if necessary & do syncrhronous
HRESULT  CTelControl::GoFromMemory (PVOID pMem, DWORD dwSize,
   DWORD *pdwResult, PVOID *ppMem, DWORD *pdwSize)
{
   HRESULT  hRes;
   hRes = FromMemory (pMem, dwSize);
   if (hRes)
      return hRes;

   return Go (pdwResult, ppMem, pdwSize);
}

HRESULT  CTelControl::GoFromStream (IStream *pIStream,
   DWORD *pdwResult, PVOID *ppMem, DWORD *pdwSize)
{
   HRESULT  hRes;
   hRes = FromStream (pIStream);
   if (hRes)
      return hRes;

   return Go (pdwResult, ppMem, pdwSize);
}

HRESULT  CTelControl::GoFromFile (PCWSTR pszFile,
   DWORD *pdwResult, PVOID *ppMem, DWORD *pdwSize)
{
   HRESULT  hRes;
   hRes = FromFile (pszFile);
   if (hRes)
      return hRes;

   return Go (pdwResult, ppMem, pdwSize);
}

#ifdef STRICT
HRESULT  CTelControl::GoFromResource (PVOID hInst, DWORD dwResID,
   DWORD *pdwResult, PVOID *ppMem, DWORD *pdwSize)
#else
HRESULT  CTelControl::GoFromResource (HINSTANCE hInst, DWORD dwResID,
   DWORD *pdwResult, PVOID *ppMem, DWORD *pdwSize)
#endif
{
   HRESULT  hRes;
   hRes = FromResource ((HINSTANCE) hInst, dwResID);
   if (hRes)
      return hRes;

   return Go (pdwResult, ppMem, pdwSize);
}

// Syncrhonous execution (must be compiled)
HRESULT  CTelControl::Go (DWORD *pdwResult, PVOID *ppMem, DWORD *pdwSize)
{
   HRESULT  hRes;

   // error if already supposed to be in loop
   if (!m_fFinish)
      return ResultFromScode (E_FAIL);

   // compile it if it's not already
   if (!IsCompiled()) {
      hRes = Compile();
      if (hRes)
         return hRes;
   }

   // blank out some params
   m_pMemFinish = NULL;
   m_dwSizeFinish = 0;
   m_fFinish = FALSE;
   m_dwResultFinish = 0;

   // start
   hRes = Start (this);
   if (hRes) {
      m_fFinish = TRUE;
      return hRes;
   }

   // wait in message loop
   MSG msg;
	while (!m_fFinish && GetMessage(&msg, NULL, NULL, NULL) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

   // fill in the values
   if (pdwResult)
      *pdwResult = m_dwResultFinish;
   if (ppMem)
      *ppMem = m_pMemFinish;
   else {
      if (m_pMemFinish)
         CoTaskMemFree (m_pMemFinish);
   }
   m_pMemFinish = NULL;
   if (pdwSize)
      *pdwSize = m_dwSizeFinish;

   return NOERROR;

}

STDMETHODIMP CTelControl::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITelControlNotifySink))
   {
      *ppv = (LPVOID) this;
      AddRef();
       return ResultFromScode (NOERROR);
   }   // otherwise, cant find


   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CTelControl::AddRef (void)
{
   // normally this increases a reference count, but this object
   // is going to be freed as soon as the app is freed, so it doesn't
   // matter
   return 1;
}

STDMETHODIMP_(ULONG) CTelControl::Release (void)
{
   // normally this releases a reference count, but this object
   // is going to be freed when the application is freed so it doesnt
   // matter
   return 1;
};

STDMETHODIMP CTelControl::Finish(DWORD dwResult, PVOID pMem, DWORD dwSize)
{
   m_dwResultFinish = dwResult;
   m_pMemFinish = NULL;
   m_dwSizeFinish = 0;
   if (pMem) {
      m_pMemFinish = CoTaskMemAlloc (dwSize);
      if (m_pMemFinish)
         memcpy (m_pMemFinish, pMem, dwSize);
      m_dwSizeFinish = dwSize;
   }
   m_fFinish = TRUE;
   return NOERROR;
}

STDMETHODIMP CTelControl::Info(DWORD dwResult, PVOID pMem, DWORD dwSize)
{
   // ignore info
   return NOERROR;
}


/*************************************************************************
CTelControlNotifySink */

STDMETHODIMP CTelControlNotifySink::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITelControlNotifySink))
   {
      *ppv = (LPVOID) this;
	AddRef();
      return ResultFromScode (NOERROR);
   }   // otherwise, cant find


   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CTelControlNotifySink::AddRef (void)
{
   // normally this increases a reference count, but this object
   // is going to be freed as soon as the app is freed, so it doesn't
   // matter
   return 1;
}

STDMETHODIMP_(ULONG) CTelControlNotifySink::Release (void)
{
   // normally this releases a reference count, but this object
   // is going to be freed when the application is freed so it doesnt
   // matter
   return 1;
};

STDMETHODIMP CTelControlNotifySink::Finish(DWORD dwResult, PVOID pMem, DWORD dwSize)
{
   return NOERROR;
}

STDMETHODIMP CTelControlNotifySink::Info(DWORD dwResult, PVOID pMem, DWORD dwSize)
{
   // ignore info
   return NOERROR;
}


/*************************************************************************
CTelNotifySink */

STDMETHODIMP CTelNotifySink::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITelNotifySink))
    {
      *ppv = (LPVOID) this;
	AddRef();
      return ResultFromScode (NOERROR);
    }// otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CTelNotifySink::AddRef (void)
{
   // normally this increases a reference count, but this object
   // is going to be freed as soon as the app is freed, so it doesn't
   // matter
   return 1;
}

STDMETHODIMP_(ULONG) CTelNotifySink::Release (void)
{
   // normally this releases a reference count, but this object
   // is going to be freed when the application is freed so it doesnt
   // matter
   return 1;
};

STDMETHODIMP CTelNotifySink::DTMF(WCHAR wcDTMF)
{
   return NOERROR;
}

STDMETHODIMP CTelNotifySink::Abort(DWORD dw)
{
   // ignore info
   return NOERROR;
}


