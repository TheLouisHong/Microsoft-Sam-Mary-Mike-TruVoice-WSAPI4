/*********************************************************************
VTxt.Cpp - Voice text C++ wrapper objects.

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

/*************************************************************************
Voice Text */

CVoiceText::CVoiceText (void)
{
   m_pIVTxtAttributes = NULL;
   m_pIAttributes = NULL;
   m_pIVTxtDialogs = NULL;
   m_pIVoiceText = NULL;
}

CVoiceText::~CVoiceText (void)
{
   if (m_pIVTxtAttributes)
      m_pIVTxtAttributes->Release();
   if (m_pIAttributes)
      m_pIAttributes->Release();
   if (m_pIVTxtDialogs)
      m_pIVTxtDialogs->Release();
   if (m_pIVoiceText)
      m_pIVoiceText->Release();
}

// initalization
HRESULT  CVoiceText::Init (void)
{
   HRESULT  hRes;

   if (m_pIVoiceText)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_VTxt, NULL, CLSCTX_LOCAL_SERVER,
      IID_IVoiceTextW, (void**)&m_pIVoiceText);
   if (hRes)
      return hRes;

   m_pIVoiceText->QueryInterface (IID_IVTxtAttributesW, (PVOID*)&m_pIVTxtAttributes);
   m_pIVoiceText->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   m_pIVoiceText->QueryInterface (IID_IVTxtDialogsW, (PVOID*)&m_pIVTxtDialogs);

   return NOERROR;
}

HRESULT  CVoiceText::Init (LPUNKNOWN pIUnkVTxt)
{
   HRESULT  hRes;

   if (m_pIVoiceText)
      return ResultFromScode (E_FAIL);

   hRes = pIUnkVTxt->QueryInterface (IID_IVoiceTextW, (PVOID*)&m_pIVoiceText);
   if (hRes)
      return hRes;
   m_pIVoiceText->QueryInterface (IID_IVTxtAttributesW, (PVOID*)&m_pIVTxtAttributes);
   m_pIVoiceText->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   m_pIVoiceText->QueryInterface (IID_IVTxtDialogsW, (PVOID*)&m_pIVTxtDialogs);

   return NOERROR;
}


// initialization and registration combined
HRESULT  CVoiceText::Init (PCWSTR pszSite, PCWSTR pszApplication, 
   PVOID pNotifyInterface, IID IIDNotifyInterface,
   DWORD dwFlags, PVTSITEINFO pSiteInfo)
{
   HRESULT  hRes;

   hRes = Init ();
   if (hRes)
      return hRes;

   // register
   hRes = Register (pszSite, pszApplication, pNotifyInterface,
      IIDNotifyInterface, dwFlags, pSiteInfo);
   return hRes;
}

HRESULT  CVoiceText::Init (PCWSTR pszApplication, PIVTXTNOTIFYSINKW pNotifyInterface,
   PCWSTR pszSite,
   DWORD dwFlags, PVTSITEINFO pSiteInfo)
{
   HRESULT  hRes;

   hRes = Init ();
   if (hRes)
      return hRes;

   // register
   hRes = Register (pszApplication, pNotifyInterface,
      pszSite, dwFlags, pSiteInfo);
   return hRes;
}


// IVTxtAttributes
HRESULT  CVoiceText::DeviceGet (DWORD *pdwDeviceID)
{
   if (m_pIVTxtAttributes)
      return m_pIVTxtAttributes->DeviceGet(pdwDeviceID);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceText::DeviceGet (void)
{
   DWORD dw = 0;
   DeviceGet (&dw);
   return dw;
}

HRESULT  CVoiceText::DeviceSet (DWORD dwDeviceID)
{
   if (m_pIVTxtAttributes)
      return m_pIVTxtAttributes->DeviceSet(dwDeviceID);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::EnabledGet (DWORD *dwEnabled)
{
   if (m_pIVTxtAttributes)
      return m_pIVTxtAttributes->EnabledGet(dwEnabled);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceText::EnabledGet (void)
{
   DWORD dw = 0;
   EnabledGet (&dw);
   return dw;
}

HRESULT  CVoiceText::EnabledSet (DWORD dwEnabled)
{
   if (m_pIVTxtAttributes)
      return m_pIVTxtAttributes->EnabledSet(dwEnabled);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::IsSpeaking (BOOL *pfSpeaking)
{
   if (m_pIVTxtAttributes)
      return m_pIVTxtAttributes->IsSpeaking(pfSpeaking);
   else
      return ResultFromScode (E_NOINTERFACE);
}

BOOL     CVoiceText::IsSpeaking (void)
{
   BOOL dw = 0;
   IsSpeaking (&dw);
   return dw;
}

HRESULT  CVoiceText::SpeedGet (DWORD *pdwSpeed)
{
   if (m_pIVTxtAttributes)
      return m_pIVTxtAttributes->SpeedGet(pdwSpeed);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceText::SpeedGet (void)
{
   DWORD dw = 0;
   SpeedGet (&dw);
   return dw;
}

HRESULT  CVoiceText::SpeedSet (DWORD dwSpeed)
{
   if (m_pIVTxtAttributes)
      return m_pIVTxtAttributes->SpeedSet(dwSpeed);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::TTSModeGet (GUID *pgVoice)
{
   if (m_pIVTxtAttributes)
      return m_pIVTxtAttributes->TTSModeGet(pgVoice);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::TTSModeSet (GUID gVoice)
{
   if (m_pIVTxtAttributes)
      return m_pIVTxtAttributes->TTSModeSet(gVoice);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// IVTxtDialogs
#ifdef STRICT
HRESULT  CVoiceText::AboutDlg (PVOID hWndParent, PWSTR pszTitle)
#else
HRESULT  CVoiceText::AboutDlg (HWND hWndParent, PWSTR pszTitle)
#endif
{
   if (m_pIVTxtDialogs)
      return m_pIVTxtDialogs->AboutDlg((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceText::GeneralDlg (PVOID hWndParent, PWSTR pszTitle)
#else
HRESULT  CVoiceText::GeneralDlg (HWND hWndParent, PWSTR pszTitle)
#endif
{
   if (m_pIVTxtDialogs)
      return m_pIVTxtDialogs->GeneralDlg((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceText::LexiconDlg (PVOID hWndParent, PWSTR pszTitle)
#else
HRESULT  CVoiceText::LexiconDlg (HWND hWndParent, PWSTR pszTitle)
#endif
{
   if (m_pIVTxtDialogs)
      return m_pIVTxtDialogs->LexiconDlg((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceText::TranslateDlg (PVOID hWndParent, PWSTR pszTitle)
#else
HRESULT  CVoiceText::TranslateDlg (HWND hWndParent, PWSTR pszTitle)
#endif
{
   if (m_pIVTxtDialogs)
      return m_pIVTxtDialogs->TranslateDlg((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// IVoiceText
HRESULT  CVoiceText::AudioFastForward (void)
{
   if (m_pIVoiceText)
      return m_pIVoiceText->AudioFastForward();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::AudioPause (void)
{
   if (m_pIVoiceText)
      return m_pIVoiceText->AudioPause();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::AudioResume (void)
{
   if (m_pIVoiceText)
      return m_pIVoiceText->AudioResume();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::AudioRewind (void)
{
   if (m_pIVoiceText)
      return m_pIVoiceText->AudioRewind();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::Register (PCWSTR pszSite, PCWSTR pszApplication, 
   PVOID pNotifyInterface, IID IIDNotifyInterface,
   DWORD dwFlags, PVTSITEINFO pSiteInfo)
{
   if (m_pIVoiceText)
      return m_pIVoiceText->Register(pszSite, pszApplication,
         (PIVTXTNOTIFYSINK) pNotifyInterface, IIDNotifyInterface, dwFlags, pSiteInfo);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::Register (PCWSTR pszApplication, PIVTXTNOTIFYSINKW pNotifyInterface,
   PCWSTR pszSite,
   DWORD dwFlags, PVTSITEINFO pSiteInfo)
{
   return Register (pszSite, pszApplication, pNotifyInterface,
      IID_IVTxtNotifySinkW, dwFlags, pSiteInfo);
}

HRESULT  CVoiceText::Speak (PCWSTR pszSpeak, DWORD dwFlags, PCWSTR pszTags)
{
   if (m_pIVoiceText)
      return m_pIVoiceText->Speak(pszSpeak, dwFlags, pszTags);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::SpeakAndWait (PCWSTR pszSpeak, DWORD dwFlags, PCWSTR pszTags)
{
   HRESULT err;
 
   err = Speak(pszSpeak,dwFlags,pszTags);

   while(IsSpeaking())
   {
	MSG msg;
	
	Sleep(50);
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		DispatchMessage(&msg);
	}
   }
   return(err);
}

HRESULT  CVoiceText::StopSpeaking (void)
{
   if (m_pIVoiceText)
      return m_pIVoiceText->StopSpeaking();
   else
      return ResultFromScode (E_NOINTERFACE);
}


/* IAttributes */
HRESULT  CVoiceText::DWORDGet (DWORD dwAttribute, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDGet (dwAttribute, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::DWORDSet (DWORD dwAttribute, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDSet (dwAttribute, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::StringGet (DWORD dwAttribute, PWSTR psz, DWORD dw, DWORD *pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringGet (dwAttribute,psz, dw, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::StringSet (DWORD dwAttribute, PCWSTR psz)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringSet (dwAttribute, psz);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::MemoryGet (DWORD dwAttribute, PVOID * ppMem, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemoryGet (dwAttribute, ppMem, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceText::MemorySet (DWORD dwAttribute, PVOID pMem, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemorySet (dwAttribute, pMem, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}





/***********************************************************************
CVTxtNotifySink */

STDMETHODIMP CVTxtNotifySink::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IVTxtNotifySinkW))
   	{
	*ppv = (LPVOID) this;
	AddRef();
	return ResultFromScode (NOERROR);
	}
   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CVTxtNotifySink::AddRef (void)
{
   return(++m_RefCount);
}

STDMETHODIMP_(ULONG) CVTxtNotifySink::Release (void)
{
   if((--m_RefCount) == 0)
   {
	delete this;
	return(0);
   }
   return(m_RefCount);
};


STDMETHODIMP CVTxtNotifySink::AttribChanged (DWORD dwID)
{
   return NOERROR;
}

STDMETHODIMP CVTxtNotifySink::Visual (WCHAR c1, WCHAR c2, DWORD dw, PTTSMOUTH pMouth)
{
   return NOERROR;
}

STDMETHODIMP CVTxtNotifySink::Speak (PWSTR psz1, PWSTR psz2, DWORD dw)
{
   return NOERROR;
}

STDMETHODIMP CVTxtNotifySink::SpeakingStarted (void)
{
   return NOERROR;
}

STDMETHODIMP CVTxtNotifySink::SpeakingDone (void)
{
   return NOERROR;
}

