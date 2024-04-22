/*********************************************************************
LowTTS.Cpp - Low level text-to-speech C++ wrapper objects.

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
// #include <initguid.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <dsound.h>
#include "spchwrap.h"

/*********************************************************************
CTTSMode - Speech recognition mode
*/

CTTSMode::CTTSMode (void)
{
   m_pUnkAudio = NULL;
   m_pITTSAttributes = NULL;
   m_pIAttributes = NULL;
   m_pITTSCentral = NULL;
   m_pITTSDialogs = NULL;
   m_pILexPronounce = NULL;
   m_pILexPronounce2 = NULL;
}

CTTSMode::~CTTSMode (void)
{
   if (m_pITTSAttributes)
      m_pITTSAttributes->Release();
   if (m_pIAttributes)
      m_pIAttributes->Release();
   if (m_pITTSCentral)
      m_pITTSCentral->Release();
   if (m_pITTSDialogs)
      m_pITTSDialogs->Release();
   if (m_pILexPronounce)
      m_pILexPronounce->Release();
   if (m_pILexPronounce2)
      m_pILexPronounce2->Release();

#ifndef _DEBUG
   if (m_pUnkAudio)
      m_pUnkAudio->Release();
#else
   long c;
   if (m_pUnkAudio)
   {
      c=m_pUnkAudio->Release();
      if (c) //leak!
	      OutputDebugString("Possible Audio Object leak\r\n");
   }
#endif

}


// specify the audio source
HRESULT  CTTSMode::InitAudioDestMM (DWORD dwDeviceID)
{
   if (m_pUnkAudio)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   PIAUDIOMULTIMEDIADEVICE pIAMM;
   hRes = CoCreateInstance (CLSID_MMAudioDest, NULL, CLSCTX_ALL,
      IID_IAudioMultiMediaDevice, (void**)&pIAMM);
   if (hRes)
      return hRes;
   hRes = pIAMM->DeviceNumSet (dwDeviceID);
   if (hRes) {
      pIAMM->Release();
      return hRes;
   }

   // success
   m_pUnkAudio = (LPUNKNOWN) pIAMM;
   return NOERROR;
}

HRESULT  CTTSMode::InitAudioDestDirect (LPUNKNOWN lpUnkDirect)
{
   if (m_pUnkAudio)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   PIAUDIODIRECT pIAD;
   hRes = CoCreateInstance (CLSID_AudioDestDirect, NULL, CLSCTX_ALL,
      IID_IAudioDirect, (void**)&pIAD);
   if (hRes)
      return hRes;

   // We're essentially Add-ref because pIAudioDirect->does not
   IDirectSound *pIDSC;
   hRes = lpUnkDirect->QueryInterface (IID_IDirectSoundTTS, (PVOID*)&pIDSC);
   if (hRes)
      return hRes;

   hRes = pIAD->Init (pIDSC, IID_IDirectSoundTTS);
   if (hRes) {
      pIDSC->Release();
      return hRes;
   }

   // success
   m_pUnkAudio = (LPUNKNOWN) pIAD;
   return NOERROR;
}

HRESULT  CTTSMode::InitAudioDestObject (LPUNKNOWN lpUnk)
{
   if (m_pUnkAudio)
      return ResultFromScode (E_FAIL);

   // remember and addref
   m_pUnkAudio = (LPUNKNOWN) lpUnk;
   m_pUnkAudio->AddRef();

   return NOERROR;
}

// specify the mode
HRESULT  CTTSMode::Init (void)
{
   TTSMODEINFOW pMI;
   memset (&pMI, 0, sizeof(pMI));
   pMI.language.LanguageID = LANGIDFROMLCID(GetThreadLocale());
   pMI.dwFeatures = TTSFEATURE_ANYWORD | TTSFEATURE_PCOPTIMIZED;
   return Init (&pMI);
}

HRESULT  CTTSMode::Init (GUID gMode)
{
   HRESULT  hRes;
   PITTSFINDW   pITTSFind;

   // if already have something then error
   if (m_pITTSCentral)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSFindW, (void**)&pITTSFind);
   if (hRes)
      return hRes;

   // make sure there's audio object
   if (!m_pUnkAudio) {
      hRes = InitAudioDestMM (WAVE_MAPPER);
      if (hRes) {
         pITTSFind->Release();
         return hRes;
      }
   }

   // select
   hRes = pITTSFind->Select(gMode, &m_pITTSCentral, m_pUnkAudio);
   pITTSFind->Release();
   if (hRes)
      return hRes;

   // enumerate everything else
   m_pITTSCentral->QueryInterface (IID_ITTSAttributesW, (PVOID*)&m_pITTSAttributes);
   m_pITTSCentral->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   m_pITTSCentral->QueryInterface (IID_ITTSDialogsW, (PVOID*)&m_pITTSDialogs);
   m_pITTSCentral->QueryInterface (IID_ILexPronounceW, (PVOID*)&m_pILexPronounce);
   m_pITTSCentral->QueryInterface (IID_ILexPronounce2W, (PVOID*)&m_pILexPronounce2);

   return NOERROR;
}

HRESULT  CTTSMode::Init (TTSMODEINFOW *pTTSModeInfo, TTSMODEINFORANK *pTTSModeInfoRank)
{
   HRESULT  hRes;
   PITTSFINDW   pITTSFind;
   TTSMODEINFOW found;

   // if already have something then error
   if (m_pITTSCentral)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSFindW, (void**)&pITTSFind);
   if (hRes)
      return hRes;
   hRes = pITTSFind->Find (pTTSModeInfo, pTTSModeInfoRank, &found);

   // make sure there's audio object
   if (!m_pUnkAudio) {
      hRes = InitAudioDestMM (WAVE_MAPPER);
      if (hRes) {
         pITTSFind->Release();
         return hRes;
      }
   }

   // select
   hRes = pITTSFind->Select(found.gModeID, &m_pITTSCentral, m_pUnkAudio);
   pITTSFind->Release();
   if (hRes)
      return hRes;

   // enumerate everything else
   m_pITTSCentral->QueryInterface (IID_ITTSAttributesW, (PVOID*)&m_pITTSAttributes);
   m_pITTSCentral->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   m_pITTSCentral->QueryInterface (IID_ITTSDialogsW, (PVOID*)&m_pITTSDialogs);
   m_pITTSCentral->QueryInterface (IID_ILexPronounceW, (PVOID*)&m_pILexPronounce);
   m_pITTSCentral->QueryInterface (IID_ILexPronounce2W, (PVOID*)&m_pILexPronounce2);

   return NOERROR;
}

HRESULT  CTTSMode::Init (LPUNKNOWN lpUnk)
{
   // if already have something then error
   if (m_pITTSCentral || !lpUnk)
      return ResultFromScode (E_FAIL);

   // enumerate everything else
   lpUnk->QueryInterface (IID_ITTSCentralW, (PVOID*)&m_pITTSCentral);
   lpUnk->QueryInterface (IID_ITTSAttributesW, (PVOID*)&m_pITTSAttributes);
   lpUnk->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   lpUnk->QueryInterface (IID_ITTSDialogsW, (PVOID*)&m_pITTSDialogs);
   lpUnk->QueryInterface (IID_ILexPronounceW, (PVOID*)&m_pILexPronounce);
   lpUnk->QueryInterface (IID_ILexPronounce2W, (PVOID*)&m_pILexPronounce2);

   return NOERROR;
}

HRESULT  CTTSMode::PitchGet (WORD *pwPitch)
{
   if (m_pITTSAttributes)
      return m_pITTSAttributes->PitchGet (pwPitch);
   else
      return ResultFromScode(E_NOINTERFACE);
}

WORD     CTTSMode::PitchGet (void)
{
   WORD  wPitch = 0;
   PitchGet (&wPitch);
   return wPitch;
}

HRESULT  CTTSMode::PitchSet (WORD wPitch)
{
   if (m_pITTSAttributes)
      return m_pITTSAttributes->PitchSet (wPitch);
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::RealTimeGet (DWORD *pdwRealTime)
{
   if (m_pITTSAttributes)
      return m_pITTSAttributes->RealTimeGet (pdwRealTime);
   else
      return ResultFromScode(E_NOINTERFACE);
}

DWORD    CTTSMode::RealTimeGet (void)
{
   DWORD  dwRealTime = 0;
   RealTimeGet (&dwRealTime);
   return dwRealTime;
}

HRESULT  CTTSMode::RealTimeSet (DWORD dwRealTime)
{
   if (m_pITTSAttributes)
      return m_pITTSAttributes->RealTimeSet (dwRealTime);
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::SpeedGet (DWORD *pdwSpeed)
{
   if (m_pITTSAttributes)
      return m_pITTSAttributes->SpeedGet (pdwSpeed);
   else
      return ResultFromScode(E_NOINTERFACE);
}

DWORD    CTTSMode::SpeedGet (void)
{
   DWORD  dwSpeed = 0;
   SpeedGet (&dwSpeed);
   return dwSpeed;
}

HRESULT  CTTSMode::SpeedSet (DWORD dwSpeed)
{
   if (m_pITTSAttributes)
      return m_pITTSAttributes->SpeedSet (dwSpeed);
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::VolumeGet (DWORD *pdwVolume)
{
   if (m_pITTSAttributes)
      return m_pITTSAttributes->VolumeGet (pdwVolume);
   else
      return ResultFromScode(E_NOINTERFACE);
}

DWORD    CTTSMode::VolumeGet (void)
{
   DWORD  dwVolume = 0;
   VolumeGet (&dwVolume);
   return dwVolume;
}

HRESULT  CTTSMode::VolumeSet (DWORD dwVolume)
{
   if (m_pITTSAttributes)
      return m_pITTSAttributes->VolumeSet (dwVolume);
   else
      return ResultFromScode(E_NOINTERFACE);
}


// ITTSCentral
HRESULT  CTTSMode::AudioPause (void)
{
   if (m_pITTSCentral)
      return m_pITTSCentral->AudioPause ();
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::AudioReset (void)
{
   if (m_pITTSCentral)
      return m_pITTSCentral->AudioReset ();
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::AudioResume (void)
{
   if (m_pITTSCentral)
      return m_pITTSCentral->AudioResume ();
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::Inject (WCHAR *pszTag)
{
   if (m_pITTSCentral)
      return m_pITTSCentral->Inject (pszTag);
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::ModeGet (PTTSMODEINFOW pttsInfo)
{
   if (m_pITTSCentral)
      return m_pITTSCentral->ModeGet (pttsInfo);
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::Phoneme(VOICECHARSET eCharacterSet, DWORD dwFlags, 
            SDATA dText, PSDATA pdPhoneme)
{
   if (m_pITTSCentral)
      return m_pITTSCentral->Phoneme (eCharacterSet, dwFlags, dText, pdPhoneme);
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::PosnGet (QWORD *pqwTimeStamp)
{
   if (m_pITTSCentral)
      return m_pITTSCentral->PosnGet (pqwTimeStamp);
   else
      return ResultFromScode(E_NOINTERFACE);
}

QWORD    CTTSMode::PosnGet (void)
{
   QWORD qwPosn = 0;
   PosnGet (&qwPosn);
   return qwPosn;
}

HRESULT  CTTSMode::Register (PVOID pNotifyInterface, IID IIDNotifyInterface,
            DWORD *pdwKey)
{
   if (m_pITTSCentral)
      return m_pITTSCentral->Register (pNotifyInterface, IIDNotifyInterface, pdwKey);
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::Register (PITTSNOTIFYSINKW pNotifyInterface, DWORD *pdwKey)
{
   return Register ((PVOID)pNotifyInterface, IID_ITTSNotifySinkW, pdwKey);
}

HRESULT  CTTSMode::TextData (VOICECHARSET eCharacterSet, DWORD dwFlags, SDATA dText, 
            PVOID pNotifyInterface, IID IIDNotifyInterface)
{
   if (m_pITTSCentral)
      return m_pITTSCentral->TextData (eCharacterSet, dwFlags, dText, pNotifyInterface, IIDNotifyInterface);
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::TextData (VOICECHARSET eCharacterSet, DWORD dwFlags, SDATA dText, 
            PITTSBUFNOTIFYSINKW pNotifyInterface)
{
   return TextData (eCharacterSet, dwFlags, dText, (PVOID) pNotifyInterface, IID_ITTSBufNotifySinkW);
}

HRESULT  CTTSMode::Speak (PCWSTR pszSpeak,BOOL fTagged,
            PITTSBUFNOTIFYSINKW pNotifyInterface)
{
   SDATA d;
   d.pData = (PVOID) pszSpeak;
   d.dwSize = (wcslen(pszSpeak) + 1) * 2;

   return TextData (CHARSET_TEXT,
      fTagged ? TTSDATAFLAG_TAGGED : 0,
      d, (PVOID) pNotifyInterface,  IID_ITTSBufNotifySinkW);
}

HRESULT  CTTSMode::ToFileTime (PQWORD pqTimeStamp, FILETIME *pFT)
{
   if (m_pITTSCentral)
      return m_pITTSCentral->ToFileTime (pqTimeStamp, pFT);
   else
      return ResultFromScode(E_NOINTERFACE);
}

HRESULT  CTTSMode::UnRegister (DWORD dwKey)
{
   if (m_pITTSCentral)
      return m_pITTSCentral->UnRegister (dwKey);
   else
      return ResultFromScode(E_NOINTERFACE);
}


// ITTSDialogs
#ifdef STRICT
HRESULT  CTTSMode::AboutDlg (PVOID hWndParent, PWSTR pszTitle)
#else
HRESULT  CTTSMode::AboutDlg (HWND hWndParent, PWSTR pszTitle)
#endif
{
   if (m_pITTSDialogs)
      return m_pITTSDialogs->AboutDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode(E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CTTSMode::GeneralDlg (PVOID hWndParent, PWSTR pszTitle)
#else
HRESULT  CTTSMode::GeneralDlg (HWND hWndParent, PWSTR pszTitle)
#endif
{
   if (m_pITTSDialogs)
      return m_pITTSDialogs->GeneralDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode(E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CTTSMode::LexiconDlg (PVOID hWndParent, PWSTR pszTitle)
#else
HRESULT  CTTSMode::LexiconDlg (HWND hWndParent, PWSTR pszTitle)
#endif
{
   if (m_pITTSDialogs)
      return m_pITTSDialogs->LexiconDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode(E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CTTSMode::TranslateDlg (PVOID hWndParent, PWSTR pszTitle)
#else
HRESULT  CTTSMode::TranslateDlg (HWND hWndParent, PWSTR pszTitle)
#endif
{
   if (m_pITTSDialogs)
      return m_pITTSDialogs->TranslateDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode(E_NOINTERFACE);
}


// ILexPronounce
HRESULT  CTTSMode::Add(VOICECHARSET CharSet, PCWSTR pszText, WCHAR *pszPronounce, 
   VOICEPARTOFSPEECH PartOfSpeech, PVOID pEngineInfo, 
   DWORD dwEngineInfoSize)
{
   if (m_pILexPronounce)
      return m_pILexPronounce->Add (CharSet, pszText, pszPronounce,
         PartOfSpeech, pEngineInfo, dwEngineInfoSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSMode::Get(VOICECHARSET CharSet, PCWSTR pszText, WORD wSense, 
   WCHAR *pszPronounce, DWORD dwPronounceSize, 
   DWORD *pdwPronounceNeeded, VOICEPARTOFSPEECH *pPartOfSpeech, 
   PVOID pEngineInfo, DWORD dwEngineInfoSize, 
   DWORD *pdwEngineInfoNeeded)
{
   if (m_pILexPronounce)
      return m_pILexPronounce->Get (CharSet, pszText, wSense,
         pszPronounce, dwPronounceSize, pdwPronounceNeeded,
         pPartOfSpeech, pEngineInfo, dwEngineInfoSize,
         pdwEngineInfoNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSMode::Remove(PCWSTR pszText, WORD wSense)
{
   if (m_pILexPronounce)
      return m_pILexPronounce->Remove (pszText, wSense);
   else
      return ResultFromScode (E_NOINTERFACE);
}

// ILexPronounce2
HRESULT  CTTSMode::AddTo(DWORD dwLex, VOICECHARSET CharSet, WCHAR *pszText, WCHAR *pszPronounce, 
   VOICEPARTOFSPEECH PartOfSpeech, PVOID pEngineInfo, 
   DWORD dwEngineInfoSize)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->AddTo (dwLex, CharSet, pszText, pszPronounce,
         PartOfSpeech, pEngineInfo, dwEngineInfoSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSMode::GetFrom(DWORD dwLex, VOICECHARSET CharSet, WCHAR *pszText, WORD wSense, 
   WCHAR *pszPronounce, DWORD dwPronounceSize, 
   DWORD *pdwPronounceNeeded, VOICEPARTOFSPEECH *pPartOfSpeech, 
   PVOID pEngineInfo, DWORD dwEngineInfoSize, 
   DWORD *pdwEngineInfoNeeded)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->GetFrom (dwLex, CharSet, pszText, wSense,
         pszPronounce, dwPronounceSize, pdwPronounceNeeded,
         pPartOfSpeech, pEngineInfo, dwEngineInfoSize,
         pdwEngineInfoNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSMode::RemoveFrom(DWORD dwLex, WCHAR *pszText, WORD wSense)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->RemoveFrom (dwLex, pszText, wSense);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSMode::QueryLexicons (BOOL f, DWORD *pdw)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->QueryLexicons (f, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSMode::ChangeSpelling (DWORD dwLex, PCWSTR psz1, PCWSTR psz2)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->ChangeSpelling (dwLex, psz1, psz2);
   else
      return ResultFromScode (E_NOINTERFACE);
}



/* IAttributes */
HRESULT  CTTSMode::DWORDGet (DWORD dwAttribute, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDGet (dwAttribute, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSMode::DWORDSet (DWORD dwAttribute, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDSet (dwAttribute, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSMode::StringGet (DWORD dwAttribute, PWSTR psz, DWORD dw, DWORD *pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringGet (dwAttribute,psz, dw, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSMode::StringSet (DWORD dwAttribute, PCWSTR psz)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringSet (dwAttribute, psz);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSMode::MemoryGet (DWORD dwAttribute, PVOID * ppMem, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemoryGet (dwAttribute, ppMem, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSMode::MemorySet (DWORD dwAttribute, PVOID pMem, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemorySet (dwAttribute, pMem, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}





/**********************************************************************
Text-to-Speech enumerator
*/
CTTSEnum::CTTSEnum (void)
{
   m_pITTSEnum = NULL;
   m_pITTSFind = NULL;
}
CTTSEnum::~CTTSEnum (void)
{
   if (m_pITTSEnum)
      m_pITTSEnum->Release();
   if (m_pITTSFind)
      m_pITTSFind->Release();
}

HRESULT  CTTSEnum::Init (void)
{
   if (m_pITTSEnum)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   hRes = CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_ALL,
      IID_ITTSEnumW, (void**)&m_pITTSEnum);
   if (hRes)
      return hRes;
   hRes = m_pITTSEnum->QueryInterface (IID_ITTSFindW, (PVOID*) &m_pITTSFind);

   return hRes;
}

HRESULT  CTTSEnum::Init (LPUNKNOWN lpUnkEnum)
{
   if (m_pITTSEnum)
      return ResultFromScode (E_FAIL);

   HRESULT hRes;
   hRes = lpUnkEnum->QueryInterface (IID_ITTSEnumW, (PVOID*) &m_pITTSEnum);
   if (hRes)
      return hRes;

   hRes = lpUnkEnum->QueryInterface (IID_ITTSFindW, (PVOID*) &m_pITTSFind);
   if (hRes)
      return hRes;

   return NOERROR;
}

HRESULT  CTTSEnum::Next (ULONG uNum, PTTSMODEINFOW pTTSModeInfo, ULONG *uFound)
{
   if (m_pITTSEnum)
      return m_pITTSEnum->Next (uNum, pTTSModeInfo, uFound);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSEnum::Next (PTTSMODEINFOW pTTSModeInfo)
{
   return Next (1, pTTSModeInfo, NULL);
}

HRESULT  CTTSEnum::Skip (ULONG uNum)
{
   if (m_pITTSEnum)
      return m_pITTSEnum->Skip (uNum);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSEnum::Reset (void)
{
   if (m_pITTSEnum)
      return m_pITTSEnum->Reset ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

CTTSEnum* CTTSEnum::Clone (void)
{
   if (!m_pITTSEnum)
      return NULL;

   PITTSENUMW   pNew;
   HRESULT  hRes;
   hRes = m_pITTSEnum->Clone (&pNew);
   if (hRes)
      return NULL;
   CTTSEnum *pNewShare;
   pNewShare = new CTTSEnum;
   if (!pNewShare) {
      pNew->Release();
      return NULL;
   }
   hRes = pNewShare->Init(pNew);
   if (hRes) {
      delete pNewShare;
      pNew->Release();
      return NULL;
   }

   return pNewShare;
}

HRESULT  CTTSEnum::Select (GUID gModeID, PCTTSMode *ppCTTSMode, LPUNKNOWN pUnkAudio)
{
   if (!m_pITTSEnum)
      return ResultFromScode (E_NOINTERFACE);

   *ppCTTSMode = NULL;

   HRESULT  hRes;
   PITTSCENTRALW   pITTSCentral;
   hRes = m_pITTSEnum->Select (gModeID, &pITTSCentral, pUnkAudio);
   if (hRes)
      return hRes;

   CTTSMode  *pMode;
   pMode = new CTTSMode;
   if (!pMode) {
      pITTSCentral->Release();
      return ResultFromScode (E_OUTOFMEMORY);
   }
   hRes = pMode->Init (pITTSCentral);
   pITTSCentral->Release();
   *ppCTTSMode = pMode;
   return hRes;
}

HRESULT  CTTSEnum::Find (PTTSMODEINFOW pTTSFind, PTTSMODEINFORANK pRank, PTTSMODEINFOW pTTSFound)
{
   if (m_pITTSFind)
      return m_pITTSFind->Find (pTTSFind, pRank, pTTSFound);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CTTSEnum::Find (PTTSMODEINFOW pTTSFind, PTTSMODEINFOW pTTSFound)
{
   return Find (pTTSFind, NULL, pTTSFound);
}




/*****************************************************************************
CTTSNotifySink */

STDMETHODIMP CTTSNotifySink::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITTSNotifySinkW) ||
       IsEqualIID(riid,IID_ITTSNotifySink2W))
   {
      *ppv = (LPVOID) this;
      AddRef();
      return ResultFromScode (NOERROR);
   }   // otherwise, cant find

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CTTSNotifySink::AddRef (void)
{
   return(++m_RefCount);
}

STDMETHODIMP_(ULONG) CTTSNotifySink::Release (void)
{
   if((--m_RefCount) == 0)
   {
	delete this;
	return(0); 
   }
   return(m_RefCount);
};


STDMETHODIMP CTTSNotifySink::AttribChanged (DWORD dwID)
{
   return NOERROR;
}

STDMETHODIMP CTTSNotifySink::AudioStart (QWORD qwTime)
{
   return NOERROR;
}

STDMETHODIMP CTTSNotifySink::AudioStop (QWORD qwTime)
{
   return NOERROR;
}

STDMETHODIMP CTTSNotifySink::Visual (QWORD qwTime,
                                     WCHAR c1, WCHAR c2,
                                     DWORD dw, PTTSMOUTH pMouth)
{
   return NOERROR;
}

STDMETHODIMP CTTSNotifySink::Error (LPUNKNOWN pUnk)
{
   return NOERROR;
}

STDMETHODIMP CTTSNotifySink::Warning (LPUNKNOWN pUnk)
{
   return NOERROR;
}


STDMETHODIMP CTTSNotifySink::VisualFuture (DWORD dwMillisec, QWORD qwTime,
                                     WCHAR c1, WCHAR c2,
                                     DWORD dw, PTTSMOUTH pMouth)
{
   return NOERROR;
}

/*****************************************************************************
CTTSBufNotifySink */

STDMETHODIMP CTTSBufNotifySink::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITTSBufNotifySink))
   {
      *ppv = (LPVOID) this;
      AddRef();
	return ResultFromScode (NOERROR);
   }// otherwise, cant find

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CTTSBufNotifySink::AddRef (void)
{
  return(++m_RefCount);
}

STDMETHODIMP_(ULONG) CTTSBufNotifySink::Release (void)
{
   if((--m_RefCount) == 0)
   {
	delete this;
	return(0);
   }
   return(m_RefCount);
};


STDMETHODIMP CTTSBufNotifySink::TextDataDone (QWORD qwTime, DWORD dw)
{
   return NOERROR;
}

STDMETHODIMP CTTSBufNotifySink::TextDataStarted (QWORD qwTime)
{
   return NOERROR;
}

STDMETHODIMP CTTSBufNotifySink::BookMark (QWORD qwTime, DWORD dwNum)
{
   return NOERROR;
}

STDMETHODIMP CTTSBufNotifySink::WordPosition (QWORD qwTime, DWORD dwNum)
{
   return NOERROR;
}
