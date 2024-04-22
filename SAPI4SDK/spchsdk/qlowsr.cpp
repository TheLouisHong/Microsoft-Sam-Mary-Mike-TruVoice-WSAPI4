/*********************************************************************
LowSR.Cpp - Low level speech recognition C++ wrapper objects.

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
#include <dsound.h>
#include "spchwrap.h"
#include <dsound.h>

/*********************************************************************
CSRMode - Speech recognition mode
*/

CSRMode::CSRMode (void)
{
   m_pUnkAudio = NULL;
   m_pISRAttributes = NULL;
   m_pIAttributes = NULL;
   m_pISRCentral = NULL;
   m_pISRDialogs = NULL;
   m_pISRDialogs2 = NULL;
   m_pISRSpeaker = NULL;
   m_pISRSpeaker2 = NULL;
   m_pILexPronounce = NULL;
   m_pILexPronounce2 = NULL;
}

CSRMode::~CSRMode (void)
{
   if (m_pISRAttributes)
      m_pISRAttributes->Release();
   if (m_pIAttributes)
      m_pIAttributes->Release();
   if (m_pISRCentral)
      m_pISRCentral->Release();
   if (m_pISRDialogs)
      m_pISRDialogs->Release();
   if (m_pISRDialogs2)
      m_pISRDialogs2->Release();
   if (m_pISRSpeaker)
      m_pISRSpeaker->Release();
   if (m_pISRSpeaker2)
      m_pISRSpeaker2->Release();
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
HRESULT  CSRMode::InitAudioSourceMM (DWORD dwDeviceID)
{
   if (m_pUnkAudio)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   PIAUDIOMULTIMEDIADEVICE pIAMM;
   hRes = CoCreateInstance (CLSID_MMAudioSource, NULL, CLSCTX_ALL,
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

HRESULT  CSRMode::InitAudioSourceDirect (LPUNKNOWN lpUnkDirect)
{
   if (m_pUnkAudio)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   PIAUDIODIRECT pIAD;
   hRes = CoCreateInstance (CLSID_AudioSourceDirect, NULL, CLSCTX_ALL,
      IID_IAudioDirect, (void**)&pIAD);
   if (hRes)
      return hRes;

   // We're essentially Add-ref because pIAudioDirect->does not
   IDirectSoundCapture *pIDSC;
   hRes = lpUnkDirect->QueryInterface (IID_IDirectSoundCapture, (PVOID*)&pIDSC);
   if (hRes)
      return hRes;

   hRes = pIAD->Init (pIDSC, IID_IDirectSoundCapture);
   if (hRes) {
      pIDSC->Release();
      return hRes;
   }

   // success
   m_pUnkAudio = (LPUNKNOWN) pIAD;
   return NOERROR;
}

HRESULT  CSRMode::InitAudioSourceObject (LPUNKNOWN lpUnk)
{
   if (m_pUnkAudio)
      return ResultFromScode (E_FAIL);

   // remember and addref
   m_pUnkAudio = (LPUNKNOWN) lpUnk;
   m_pUnkAudio->AddRef();

   return NOERROR;
}

// specify the mode
HRESULT  CSRMode::Init (void)
{
   SRMODEINFOW pMI;
   memset (&pMI, 0, sizeof(pMI));
   pMI.language.LanguageID = LANGIDFROMLCID(GetThreadLocale());
   pMI.dwSequencing = SRSEQUENCE_CONTINUOUS;
   pMI.dwGrammars = SRGRAM_CFG;
   return Init (&pMI);
}

HRESULT  CSRMode::Init (GUID gMode)
{
   HRESULT  hRes;
   PISRFINDW   pISRFind;

   // if already have something then error
   if (m_pISRCentral)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_SREnumerator, NULL, CLSCTX_ALL, IID_ISRFindW, (void**)&pISRFind);
   if (hRes)
      return hRes;

   // make sure there's audio object
   if (!m_pUnkAudio) {
      hRes = InitAudioSourceMM (WAVE_MAPPER);
      if (hRes) {
         pISRFind->Release();
         return hRes;
      }
   }

   // select
   hRes = pISRFind->Select(gMode, &m_pISRCentral, m_pUnkAudio);
   pISRFind->Release();
   if (hRes)
      return hRes;

   // enumerate everything else
   m_pISRCentral->QueryInterface (IID_ISRAttributesW, (PVOID*)&m_pISRAttributes);
   m_pISRCentral->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   m_pISRCentral->QueryInterface (IID_ISRDialogsW, (PVOID*)&m_pISRDialogs);
   m_pISRCentral->QueryInterface (IID_ISRDialogs2W, (PVOID*)&m_pISRDialogs2);
   m_pISRCentral->QueryInterface (IID_ISRSpeakerW, (PVOID*)&m_pISRSpeaker);
   m_pISRCentral->QueryInterface (IID_ISRSpeaker2W, (PVOID*)&m_pISRSpeaker2);
   m_pISRCentral->QueryInterface (IID_ILexPronounceW, (PVOID*)&m_pILexPronounce);
   m_pISRCentral->QueryInterface (IID_ILexPronounce2W, (PVOID*)&m_pILexPronounce2);

   return NOERROR;
}

HRESULT  CSRMode::Init (SRMODEINFOW *pSRModeInfo, SRMODEINFORANK *pSRModeInfoRank)
{
   HRESULT  hRes;
   PISRFINDW   pISRFind;
   SRMODEINFOW found;

   // if already have something then error
   if (m_pISRCentral)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_SREnumerator, NULL, CLSCTX_ALL, IID_ISRFindW, (void**)&pISRFind);
   if (hRes)
      return hRes;
   hRes = pISRFind->Find (pSRModeInfo, pSRModeInfoRank, &found);

   // make sure there's audio object
   if (!m_pUnkAudio) {
      hRes = InitAudioSourceMM (WAVE_MAPPER);
      if (hRes) {
         pISRFind->Release();
         return hRes;
      }
   }

   // select
   hRes = pISRFind->Select(found.gModeID, &m_pISRCentral, m_pUnkAudio);
   pISRFind->Release();
   if (hRes)
      return hRes;

   // enumerate everything else
   m_pISRCentral->QueryInterface (IID_ISRAttributesW, (PVOID*)&m_pISRAttributes);
   m_pISRCentral->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   m_pISRCentral->QueryInterface (IID_ISRDialogsW, (PVOID*)&m_pISRDialogs);
   m_pISRCentral->QueryInterface (IID_ISRDialogs2W, (PVOID*)&m_pISRDialogs2);
   m_pISRCentral->QueryInterface (IID_ISRSpeakerW, (PVOID*)&m_pISRSpeaker);
   m_pISRCentral->QueryInterface (IID_ISRSpeaker2W, (PVOID*)&m_pISRSpeaker2);
   m_pISRCentral->QueryInterface (IID_ILexPronounceW, (PVOID*)&m_pILexPronounce);
   m_pISRCentral->QueryInterface (IID_ILexPronounce2W, (PVOID*)&m_pILexPronounce2);

   return NOERROR;
}

HRESULT  CSRMode::Init (LPUNKNOWN lpUnk)
{
   // if already have something then error
   if (m_pISRCentral || !lpUnk)
      return ResultFromScode (E_FAIL);

   // enumerate everything else
   lpUnk->QueryInterface (IID_ISRCentralW, (PVOID*)&m_pISRCentral);
   lpUnk->QueryInterface (IID_ISRAttributesW, (PVOID*)&m_pISRAttributes);
   lpUnk->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   lpUnk->QueryInterface (IID_ISRDialogsW, (PVOID*)&m_pISRDialogs);
   lpUnk->QueryInterface (IID_ISRDialogs2W, (PVOID*)&m_pISRDialogs2);
   lpUnk->QueryInterface (IID_ISRSpeakerW, (PVOID*)&m_pISRSpeaker);
   lpUnk->QueryInterface (IID_ISRSpeaker2W, (PVOID*)&m_pISRSpeaker2);
   lpUnk->QueryInterface (IID_ILexPronounceW, (PVOID*)&m_pILexPronounce);
   lpUnk->QueryInterface (IID_ILexPronounce2W, (PVOID*)&m_pILexPronounce2);

   return NOERROR;
}

// attributes
HRESULT  CSRMode::AutoGainEnableGet (DWORD *pdwAutoGain)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->AutoGainEnableGet (pdwAutoGain);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CSRMode::AutoGainEnableGet (void)
{
   DWORD dwAutoGain = 0;
   AutoGainEnableGet (&dwAutoGain);
   return dwAutoGain;
}

HRESULT  CSRMode::AutoGainEnableSet (DWORD dwAutoGain)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->AutoGainEnableSet (dwAutoGain);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::EchoGet (BOOL *pfEcho)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->EchoGet (pfEcho);
   else
      return ResultFromScode (E_NOINTERFACE);
}

BOOL     CSRMode::EchoGet (void)
{
   BOOL fEcho = 0;
   EchoGet (&fEcho);
   return fEcho;
}

HRESULT  CSRMode::EchoSet (BOOL fEcho)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->EchoSet (fEcho);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::EnergyFloorGet (WORD *pwEnergy)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->EnergyFloorGet (pwEnergy);
   else
      return ResultFromScode (E_NOINTERFACE);
}

WORD     CSRMode::EnergyFloorGet (void)
{
   WORD wEnergy = 0;
   EnergyFloorGet (&wEnergy);
   return wEnergy;
}

HRESULT  CSRMode::EnergyFloorSet (WORD wEnergy)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->EnergyFloorSet (wEnergy);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::MicrophoneGet (WCHAR *pszMicrophone, DWORD dwMicrophoneSize, DWORD *pdwNeeded)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->MicrophoneGet (pszMicrophone, dwMicrophoneSize, pdwNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::MicrophoneSet (WCHAR *pszMicrophone)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->MicrophoneSet (pszMicrophone);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::RealTimeGet (DWORD *pdwRealTime)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->RealTimeGet (pdwRealTime);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CSRMode::RealTimeGet (void)
{
   DWORD dwRealTime = 0;
   RealTimeGet (&dwRealTime);
   return dwRealTime;
}

HRESULT  CSRMode::RealTimeSet (DWORD dwRealTime)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->RealTimeSet (dwRealTime);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::SpeakerGet (WCHAR *pszSpeaker, DWORD dwSpeakerSize, DWORD *pdwNeeded)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->SpeakerGet (pszSpeaker, dwSpeakerSize, pdwNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::SpeakerSet (WCHAR *pszSpeaker)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->SpeakerSet (pszSpeaker);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::ThresholdGet (DWORD *pdwThreshold)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->ThresholdGet (pdwThreshold);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CSRMode::ThresholdGet (void)
{
   DWORD dwThresholdGet = 0;
   ThresholdGet (&dwThresholdGet);
   return dwThresholdGet;
}

HRESULT  CSRMode::ThresholdSet (DWORD dwThreshold)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->ThresholdSet (dwThreshold);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::TimeOutGet (DWORD *pdwIncomplete, DWORD *pdwComplete)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->TimeOutGet (pdwIncomplete, pdwComplete);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::TimeOutSet(DWORD dwIncomplete, DWORD dwComplete)
{
   if (m_pISRAttributes)
      return m_pISRAttributes->TimeOutSet (dwIncomplete, dwComplete);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// ISRCentral
HRESULT  CSRMode::GrammarLoad (SRGRMFMT eFormat,
   SDATA dData,
   PVOID pNotifyInterface, IID IIDNotifyInterface,
   LPUNKNOWN *ppiUnknown)
{
   if (m_pISRCentral)
      return m_pISRCentral->GrammarLoad(eFormat, dData, pNotifyInterface, IIDNotifyInterface, ppiUnknown);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::GrammarLoad (SRGRMFMT eFormat,
   PVOID pMem, DWORD dwSize,
   PVOID pNotifyInterface, IID IIDNotifyInterface,
   LPUNKNOWN *ppiUnknown)
{
   SDATA d;
   d.pData = pMem;
   d.dwSize = dwSize;

   return GrammarLoad (eFormat, d, pNotifyInterface, IIDNotifyInterface, ppiUnknown);
}
HRESULT  CSRMode::GrammarLoad (SRGRMFMT eFormat,
   SDATA dData,
   PISRGRAMNOTIFYSINKW pISRGramCommon,
   LPUNKNOWN *ppiUnknown)
{
   return GrammarLoad (eFormat, dData, pISRGramCommon,
      IID_ISRGramNotifySinkW, ppiUnknown);
}
HRESULT  CSRMode::GrammarLoad (SRGRMFMT eFormat,
   PVOID pMem, DWORD dwSize,
   PISRGRAMNOTIFYSINKW pISRGramCommon,
   LPUNKNOWN *ppiUnknown)
{
   SDATA d;
   d.pData = pMem;
   d.dwSize = dwSize;

   return GrammarLoad (eFormat, d, pISRGramCommon, IID_ISRGramNotifySinkW, ppiUnknown);
}

CSRGram* CSRMode::GrammarLoad (SRGRMFMT eFormat,
   PVOID pMem, DWORD dwSize,
   PISRGRAMNOTIFYSINKW pISRGramCommon)
{
   CSRGram* pGram;
   pGram = new CSRGram;
   if (!pGram)
      return NULL;

   LPUNKNOWN   pUnk;
   HRESULT     hRes;
   hRes = GrammarLoad (eFormat, pMem, dwSize, pISRGramCommon, &pUnk);
   if (hRes) {
      delete pGram;
      return NULL;
   }

   pGram->Init (pUnk);
   pUnk->Release();
   return pGram;
}


HRESULT  CSRMode::ModeGet (PSRMODEINFOW pModeInfo)
{
   if (m_pISRCentral)
      return m_pISRCentral->ModeGet (pModeInfo);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Pause (void)
{
   if (m_pISRCentral)
      return m_pISRCentral->Pause ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::PosnGet (QWORD *pqwTimeStamp)
{
   if (m_pISRCentral)
      return m_pISRCentral->PosnGet (pqwTimeStamp);
   else
      return ResultFromScode (E_NOINTERFACE);
}

QWORD    CSRMode::PosnGet (void)
{
   QWORD qwPosn = 0;
   PosnGet (&qwPosn);
   return qwPosn;
}

HRESULT  CSRMode::Register (PVOID pNotifyInterface, IID IIDNotifyInterface, DWORD *pdwKey)
{
   if (m_pISRCentral)
      return m_pISRCentral->Register (pNotifyInterface, IIDNotifyInterface, pdwKey);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Register (PISRNOTIFYSINKW pISRNotifySink, DWORD *pdwKey)
{
   return Register (pISRNotifySink, IID_ISRNotifySinkW, pdwKey);
}

HRESULT  CSRMode::Resume (void)
{
   if (m_pISRCentral)
      return m_pISRCentral->Resume ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::ToFileTime (PQWORD pqWord, FILETIME *pFT)
{
   if (m_pISRCentral)
      return m_pISRCentral->ToFileTime (pqWord, pFT);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::UnRegister (DWORD dwKey)
{
   if (m_pISRCentral)
      return m_pISRCentral->UnRegister (dwKey);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// ISRDialogs
#ifdef STRICT
HRESULT  CSRMode::AboutDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CSRMode::AboutDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pISRDialogs)
      return m_pISRDialogs->AboutDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSRMode::GeneralDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CSRMode::GeneralDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pISRDialogs)
      return m_pISRDialogs->GeneralDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSRMode::LexiconDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CSRMode::LexiconDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pISRDialogs)
      return m_pISRDialogs->LexiconDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSRMode::TrainGeneralDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CSRMode::TrainGeneralDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pISRDialogs)
      return m_pISRDialogs->TrainGeneralDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSRMode::TrainMicDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CSRMode::TrainMicDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pISRDialogs)
      return m_pISRDialogs->TrainMicDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}


#ifdef STRICT
HRESULT  CSRMode::TrainPhrasesDlg (PVOID hWndParent, PCWSTR pszTitle, PCWSTR pszText)
#else
HRESULT  CSRMode::TrainPhrasesDlg (HWND hWndParent, PCWSTR pszTitle, PCWSTR pszText)
#endif
{
   if (m_pISRDialogs2)
      return m_pISRDialogs2->TrainPhrasesDlg ((HWND) hWndParent, pszTitle, pszText);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// ISRSpeaker
HRESULT  CSRMode::Delete (WCHAR *pszSpeakerName)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Delete (pszSpeakerName);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Enum (PWSTR *ppszBuffer, DWORD *pdwBufSize)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Enum (ppszBuffer, pdwBufSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Merge (WCHAR *pszSpeakerName, PVOID pSpeakerData, DWORD dwSpeakerData)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Merge (pszSpeakerName, pSpeakerData, dwSpeakerData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::New (WCHAR *pszSpeakerName)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->New (pszSpeakerName);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Query (WCHAR *pszSpeakerName, DWORD dwSize, DWORD *pdwNeeded)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Query (pszSpeakerName, dwSize, pdwNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Read (WCHAR *pszSpeakerName, PVOID *ppBuffer, DWORD *pdwBufSize)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Read (pszSpeakerName, ppBuffer, pdwBufSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Revert (WCHAR *pszSpeakerName)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Revert (pszSpeakerName);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Select(WCHAR *pszSpeakerName, BOOL fLock)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Select (pszSpeakerName, fLock);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Write (WCHAR *pszSpeakerName, PVOID pSpeakerData, DWORD dwSpeakerData)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Write (pszSpeakerName, pSpeakerData, dwSpeakerData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Commit (void)
{
   if (m_pISRSpeaker2)
      return m_pISRSpeaker2->Commit ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Rename (PCWSTR psz1, PCWSTR psz2)
{
   if (m_pISRSpeaker2)
      return m_pISRSpeaker2->Rename (psz1, psz2);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::GetChangedInfo (BOOL *pf, FILETIME *pft)
{
   if (m_pISRSpeaker2)
      return m_pISRSpeaker2->GetChangedInfo (pf, pft);
   else
      return ResultFromScode (E_NOINTERFACE);
}



/* IAttributes */
HRESULT  CSRMode::DWORDGet (DWORD dwAttribute, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDGet (dwAttribute, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::DWORDSet (DWORD dwAttribute, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDSet (dwAttribute, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::StringGet (DWORD dwAttribute, PWSTR psz, DWORD dw, DWORD *pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringGet (dwAttribute,psz, dw, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::StringSet (DWORD dwAttribute, PCWSTR psz)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringSet (dwAttribute, psz);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::MemoryGet (DWORD dwAttribute, PVOID * ppMem, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemoryGet (dwAttribute, ppMem, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::MemorySet (DWORD dwAttribute, PVOID pMem, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemorySet (dwAttribute, pMem, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}



// create gramcomp objects
CSRGramComp *CSRMode::Grammar (PISRGRAMNOTIFYSINKW pISRGramNotifySink)
{
   CSRGramComp *pCSRGramComp;

   pCSRGramComp = new CSRGramComp;
   if (!pCSRGramComp)
      return NULL;

   HRESULT  hRes;
   hRes = pCSRGramComp->Init(this, pISRGramNotifySink);
   if (hRes) {
      delete pCSRGramComp;
      return NULL;
   }

   return pCSRGramComp;
}

CSRGramComp *CSRMode::GrammarFromMemory (PISRGRAMNOTIFYSINKW pISRGramNotifySink, PVOID pMem, DWORD dwSize)
{
   CSRGramComp *pCSRGramComp;

   pCSRGramComp = Grammar (pISRGramNotifySink);
   if (!pCSRGramComp)
      return NULL;

   // load it in
   HRESULT  hRes;
   hRes = pCSRGramComp->GrammarFromMemory (pMem, dwSize);
   if (hRes) {
      delete pCSRGramComp;
      return NULL;
   }

   return pCSRGramComp;
}

#ifdef STRICT
CSRGramComp *CSRMode::GrammarFromResource (PISRGRAMNOTIFYSINKW pISRGramNotifySink, PVOID hInst, DWORD dwResID)
#else
CSRGramComp *CSRMode::GrammarFromResource (PISRGRAMNOTIFYSINKW pISRGramNotifySink, HINSTANCE hInst, DWORD dwResID)
#endif
{
   CSRGramComp *pCSRGramComp;

   pCSRGramComp = Grammar (pISRGramNotifySink);
   if (!pCSRGramComp)
      return NULL;

   // load it in
   HRESULT  hRes;
   hRes = pCSRGramComp->GrammarFromResource ((HINSTANCE) hInst, dwResID);
   if (hRes) {
      delete pCSRGramComp;
      return NULL;
   }

   return pCSRGramComp;
}

CSRGramComp *CSRMode::GrammarFromFile (PISRGRAMNOTIFYSINKW pISRGramNotifySink, PCWSTR pszFile)
{
   CSRGramComp *pCSRGramComp;

   pCSRGramComp = Grammar (pISRGramNotifySink);
   if (!pCSRGramComp)
      return NULL;

   // load it in
   HRESULT  hRes;
   hRes = pCSRGramComp->GrammarFromFile  (pszFile);
   if (hRes) {
      delete pCSRGramComp;
      return NULL;
   }

   return pCSRGramComp;
}

CSRGramComp *CSRMode::GrammarFromStream (PISRGRAMNOTIFYSINKW pISRGramNotifySink, IStream *pIStream)
{
   CSRGramComp *pCSRGramComp;

   pCSRGramComp = Grammar (pISRGramNotifySink);
   if (!pCSRGramComp)
      return NULL;

   // load it in
   HRESULT  hRes;
   hRes = pCSRGramComp->GrammarFromStream (pIStream);
   if (hRes) {
      delete pCSRGramComp;
      return NULL;
   }

   return pCSRGramComp;
}




// ILexPronounce
HRESULT  CSRMode::Add(VOICECHARSET CharSet, WCHAR *pszText, WCHAR *pszPronounce, 
   VOICEPARTOFSPEECH PartOfSpeech, PVOID pEngineInfo, 
   DWORD dwEngineInfoSize)
{
   if (m_pILexPronounce)
      return m_pILexPronounce->Add (CharSet, pszText, pszPronounce,
         PartOfSpeech, pEngineInfo, dwEngineInfoSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::Get(VOICECHARSET CharSet, WCHAR *pszText, WORD wSense, 
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

HRESULT  CSRMode::Remove(WCHAR *pszText, WORD wSense)
{
   if (m_pILexPronounce)
      return m_pILexPronounce->Remove (pszText, wSense);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// ILexPronounce2
HRESULT  CSRMode::AddTo(DWORD dwLex, VOICECHARSET CharSet, WCHAR *pszText, WCHAR *pszPronounce, 
   VOICEPARTOFSPEECH PartOfSpeech, PVOID pEngineInfo, 
   DWORD dwEngineInfoSize)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->AddTo (dwLex, CharSet, pszText, pszPronounce,
         PartOfSpeech, pEngineInfo, dwEngineInfoSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::GetFrom(DWORD dwLex, VOICECHARSET CharSet, WCHAR *pszText, WORD wSense, 
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

HRESULT  CSRMode::RemoveFrom(DWORD dwLex, WCHAR *pszText, WORD wSense)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->RemoveFrom (dwLex, pszText, wSense);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::QueryLexicons (BOOL f, DWORD *pdw)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->QueryLexicons (f, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRMode::ChangeSpelling (DWORD dwLex, PCWSTR psz1, PCWSTR psz2)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->ChangeSpelling (dwLex, psz1, psz2);
   else
      return ResultFromScode (E_NOINTERFACE);
}

/**********************************************************************
Speech recognition enumerator
*/
CSREnum::CSREnum (void)
{
   m_pISREnum = NULL;
   m_pISRFind = NULL;
}
CSREnum::~CSREnum (void)
{
   if (m_pISREnum)
      m_pISREnum->Release();
   if (m_pISRFind)
      m_pISRFind->Release();
}

HRESULT  CSREnum::Init (void)
{
   if (m_pISREnum)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   hRes = CoCreateInstance (CLSID_SREnumerator, NULL, CLSCTX_ALL,
      IID_ISREnumW, (void**)&m_pISREnum);
   if (hRes)
      return hRes;
   hRes = m_pISREnum->QueryInterface (IID_ISRFindW, (PVOID*) &m_pISRFind);

   return hRes;
}

HRESULT  CSREnum::Init (LPUNKNOWN lpUnkEnum)
{
   if (m_pISREnum)
      return ResultFromScode (E_FAIL);

   HRESULT hRes;
   hRes = lpUnkEnum->QueryInterface (IID_ISREnumW, (PVOID*) &m_pISREnum);
   if (hRes)
      return hRes;

   hRes = lpUnkEnum->QueryInterface (IID_ISRFindW, (PVOID*) &m_pISRFind);
   if (hRes)
      return hRes;

   return NOERROR;
}

HRESULT  CSREnum::Next (ULONG uNum, PSRMODEINFOW pSRModeInfo, ULONG *uFound)
{
   if (m_pISREnum)
      return m_pISREnum->Next (uNum, pSRModeInfo, uFound);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSREnum::Next (PSRMODEINFOW pSRModeInfo)
{
   return Next (1, pSRModeInfo, NULL);
}

HRESULT  CSREnum::Skip (ULONG uNum)
{
   if (m_pISREnum)
      return m_pISREnum->Skip (uNum);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSREnum::Reset (void)
{
   if (m_pISREnum)
      return m_pISREnum->Reset ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

CSREnum* CSREnum::Clone (void)
{
   if (!m_pISREnum)
      return NULL;

   PISRENUMW   pNew;
   HRESULT  hRes;
   hRes = m_pISREnum->Clone (&pNew);
   if (hRes)
      return NULL;
   CSREnum *pNewShare;
   pNewShare = new CSREnum;
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

HRESULT  CSREnum::Select (GUID gModeID, PCSRMode *ppCSRMode, LPUNKNOWN pUnkAudio)
{
   if (!m_pISREnum)
      return ResultFromScode (E_NOINTERFACE);

   *ppCSRMode = NULL;

   HRESULT  hRes;
   PISRCENTRALW   pISRCentral;
   hRes = m_pISREnum->Select (gModeID, &pISRCentral, pUnkAudio);
   if (hRes)
      return hRes;

   CSRMode  *pMode;
   pMode = new CSRMode;
   if (!pMode) {
      pISRCentral->Release();
      return ResultFromScode (E_OUTOFMEMORY);
   }
   hRes = pMode->Init (pISRCentral);
   pISRCentral->Release();
   *ppCSRMode = pMode;
   return hRes;
}

HRESULT  CSREnum::Find (PSRMODEINFOW pSRFind, PSRMODEINFORANK pRank, PSRMODEINFOW pSRFound)
{
   if (m_pISRFind)
      return m_pISRFind->Find (pSRFind, pRank, pSRFound);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSREnum::Find (PSRMODEINFOW pSRFind, PSRMODEINFOW pSRFound)
{
   return Find (pSRFind, NULL, pSRFound);
}


/**********************************************************************
Speech recognition sharing object
*/
CSRShare::CSRShare (void)
{
   m_pISRShare = NULL;
}
CSRShare::~CSRShare (void)
{
   if (m_pISRShare)
      m_pISRShare->Release();
}

HRESULT  CSRShare::Init (void)
{
   if (m_pISRShare)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   hRes = CoCreateInstance (CLSID_SRShare, NULL, CLSCTX_ALL,
      IID_IEnumSRShareW, (void**)&m_pISRShare);
   if (hRes)
      return hRes;

   return NOERROR;
}

HRESULT  CSRShare::Init (LPUNKNOWN lpUnkShare)
{
   if (m_pISRShare)
      return ResultFromScode (E_FAIL);

   HRESULT hRes;
   hRes = lpUnkShare->QueryInterface (IID_IEnumSRShareW, (PVOID*) &m_pISRShare);
   if (hRes)
      return hRes;

   return NOERROR;
}

HRESULT  CSRShare::Next (ULONG uNum, PSRSHAREW pSRShare, ULONG *uFound)
{
   if (m_pISRShare)
      return m_pISRShare->Next (uNum, pSRShare, uFound);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRShare::Next (PSRSHAREW pSRShare)
{
   return Next (1, pSRShare, NULL);
}

HRESULT  CSRShare::Skip (ULONG uNum)
{
   if (m_pISRShare)
      return m_pISRShare->Skip (uNum);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRShare::Reset (void)
{
   if (m_pISRShare)
      return m_pISRShare->Reset ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

CSRShare* CSRShare::Clone (void)
{
   if (!m_pISRShare)
      return NULL;

   PIENUMSRSHAREW   pNew;
   HRESULT  hRes;
   hRes = m_pISRShare->Clone (&pNew);
   if (hRes)
      return NULL;
   CSRShare *pNewShare;
   pNewShare = new CSRShare;
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

HRESULT  CSRShare::New (DWORD dwDeviceID, GUID gModeID, PCSRMode *ppCSRMode, QWORD *pqwInstance)
{
   if (!m_pISRShare)
      return ResultFromScode (E_NOINTERFACE);

   *ppCSRMode = NULL;

   HRESULT  hRes;
   PISRCENTRALW   pISRCentral;
   hRes = m_pISRShare->New (dwDeviceID, gModeID, &pISRCentral, pqwInstance);
   if (hRes)
      return hRes;

   CSRMode  *pMode;
   pMode = new CSRMode;
   if (!pMode) {
      pISRCentral->Release();
      return ResultFromScode (E_OUTOFMEMORY);
   }
   hRes = pMode->Init (pISRCentral);
   pISRCentral->Release();
   return hRes;
}

HRESULT  CSRShare::New (GUID gModeID, PCSRMode *ppCSRMode, QWORD *pqwInstance)
{
   return New ((DWORD) WAVE_MAPPER, gModeID, ppCSRMode, pqwInstance);
}


HRESULT  CSRShare::Share (QWORD qwInstance, PCSRMode *ppCSRMode)
{
   if (!m_pISRShare)
      return ResultFromScode (E_NOINTERFACE);

   *ppCSRMode = NULL;

   HRESULT  hRes;
   PISRCENTRALW   pISRCentral;
   hRes = m_pISRShare->Share (qwInstance, &pISRCentral);
   if (hRes)
      return hRes;

   CSRMode  *pMode;
   pMode = new CSRMode;
   if (!pMode) {
      pISRCentral->Release();
      return ResultFromScode (E_OUTOFMEMORY);
   }
   hRes = pMode->Init (pISRCentral);
   pISRCentral->Release();
   return hRes;
}

HRESULT  CSRShare::Detach (QWORD qwInstance)
{
   if (m_pISRShare)
      return m_pISRShare->Detach (qwInstance);
   else
      return ResultFromScode (E_NOINTERFACE);
}



/**********************************************************************
CSRGram */

CSRGram::CSRGram (void)
{
   m_pISRGramCommon = NULL;
   m_pIAttributes = NULL;
   m_pISRGramCFG = NULL;
   m_pISRGramDictation = NULL;
   m_pISRGramInsertionGUI = NULL;
   m_pILexPronounce = NULL;
   m_pILexPronounce2 = NULL;
}

CSRGram::~CSRGram (void)
{
   if (m_pISRGramCommon)
      m_pISRGramCommon->Release();
   if (m_pIAttributes)
      m_pIAttributes->Release();
   if (m_pISRGramCFG)
      m_pISRGramCFG->Release();
   if (m_pISRGramDictation)
      m_pISRGramDictation->Release();
   if (m_pISRGramInsertionGUI)
      m_pISRGramInsertionGUI->Release();
   if (m_pILexPronounce)
      m_pILexPronounce->Release();
   if (m_pILexPronounce2)
      m_pILexPronounce2->Release();
}


HRESULT  CSRGram::Init (LPUNKNOWN pUnkGram)
{
   if (m_pISRGramCommon) {
      if (m_pISRGramCommon)
         m_pISRGramCommon->Release();
      if (m_pIAttributes)
         m_pIAttributes->Release();
      if (m_pISRGramCFG)
         m_pISRGramCFG->Release();
      if (m_pISRGramDictation)
         m_pISRGramDictation->Release();
      if (m_pISRGramInsertionGUI)
         m_pISRGramInsertionGUI->Release();
      if (m_pILexPronounce)
         m_pILexPronounce->Release();
      if (m_pILexPronounce2)
         m_pILexPronounce2->Release();

      m_pISRGramCommon = NULL;
      m_pIAttributes = NULL;
      m_pISRGramCFG = NULL;
      m_pISRGramDictation = NULL;
      m_pISRGramInsertionGUI = NULL;
      m_pILexPronounce = NULL;
      m_pILexPronounce2 = NULL;
   }

   pUnkGram->QueryInterface (IID_ISRGramCommonW, (PVOID*)&m_pISRGramCommon);
   pUnkGram->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   pUnkGram->QueryInterface (IID_ISRGramDictationW, (PVOID*)&m_pISRGramDictation);
   pUnkGram->QueryInterface (IID_ISRGramCFGW, (PVOID*)&m_pISRGramCFG);
   pUnkGram->QueryInterface (IID_ISRGramInsertionGUI, (PVOID*)&m_pISRGramInsertionGUI);
   pUnkGram->QueryInterface (IID_ILexPronounceW, (PVOID*)&m_pILexPronounce);
   pUnkGram->QueryInterface (IID_ILexPronounce2W, (PVOID*)&m_pILexPronounce2);

   return NOERROR;
}


// ISRGramCommon
#ifdef STRICT
HRESULT  CSRGram::Activate (PVOID hWndListening, BOOL fAutoPause, PCWSTR pszInfo)
#else
HRESULT  CSRGram::Activate (HWND hWndListening, BOOL fAutoPause, PCWSTR pszInfo)
#endif
{
   if (m_pISRGramCommon)
      return m_pISRGramCommon->Activate ((HWND) hWndListening, fAutoPause, pszInfo);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSRGram::Activate (PVOID hWndListening, PCWSTR pszInfo)
#else
HRESULT  CSRGram::Activate (HWND hWndListening, PCWSTR pszInfo)
#endif
{
   return Activate (hWndListening, FALSE, pszInfo);
}

HRESULT  CSRGram::Archive (BOOL fArchiveResults, PVOID pBuf, DWORD dwBufSize,
   DWORD *pdwBufNeeded)
{
   if (m_pISRGramCommon)
      return m_pISRGramCommon->Archive (fArchiveResults, pBuf, dwBufSize, pdwBufNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::BookMark (QWORD qwTime, DWORD dwBookMarkID)
{
   if (m_pISRGramCommon)
      return m_pISRGramCommon->BookMark (qwTime, dwBookMarkID);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::Deactivate(PCWSTR pszInfo)
{
   if (m_pISRGramCommon)
      return m_pISRGramCommon->Deactivate (pszInfo);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::DeteriorationGet (DWORD *pdwMemory, DWORD *pdwTime,
                   DWORD *pdwObjects)
{
   if (m_pISRGramCommon)
      return m_pISRGramCommon->DeteriorationGet (pdwMemory, pdwTime, pdwObjects);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::DeteriorationSet(DWORD dwMemory, DWORD dwTime, DWORD dwObjects)
{
   if (m_pISRGramCommon)
      return m_pISRGramCommon->DeteriorationSet (dwMemory, dwTime, dwObjects);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSRGram::TrainDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CSRGram::TrainDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pISRGramCommon)
      return m_pISRGramCommon->TrainDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::TrainPhrase (DWORD dwExtent, PSDATA pData)
{
   if (m_pISRGramCommon)
      return m_pISRGramCommon->TrainPhrase (dwExtent, pData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::TrainQuery (DWORD *pdwFlags)
{
   if (m_pISRGramCommon)
      return m_pISRGramCommon->TrainQuery (pdwFlags);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// ISRGramCFG
HRESULT  CSRGram::LinkQuery (WCHAR *pszLinkName, BOOL *pfExist)
{
   if (m_pISRGramCFG)
      return m_pISRGramCFG->LinkQuery (pszLinkName, pfExist);
   else
      return ResultFromScode (E_NOINTERFACE);
}


BOOL     CSRGram::LinkQuery (WCHAR *pszLinkName)
{
   BOOL  fExist = FALSE;
   LinkQuery (pszLinkName, &fExist);
   return fExist;
}


HRESULT  CSRGram::ListAppend (WCHAR *pszListName, SDATA dWord)
{
   if (m_pISRGramCFG)
      return m_pISRGramCFG->ListAppend (pszListName, dWord);
   else
      return ResultFromScode (E_NOINTERFACE);
}


HRESULT  CSRGram::ListAppend (WCHAR *pszListName, PVOID pData, DWORD dwSize)
{
   SDATA d;
   d.pData = pData;
   d.dwSize = dwSize;
   return ListAppend (pszListName, d);
}

HRESULT  CSRGram::ListGet (WCHAR *pszListName, PSDATA pdWord)
{
   if (m_pISRGramCFG)
      return m_pISRGramCFG->ListGet (pszListName, pdWord);
   else
      return ResultFromScode (E_NOINTERFACE);
}


HRESULT  CSRGram::ListQuery (WCHAR *pszListName, BOOL *fExist)
{
   if (m_pISRGramCFG)
      return m_pISRGramCFG->ListQuery (pszListName, fExist);
   else
      return ResultFromScode (E_NOINTERFACE);
}

BOOL     CSRGram::ListQuery (WCHAR *pszListName)
{
   BOOL  fExist = FALSE;
   ListQuery (pszListName, &fExist);
   return fExist;
}


HRESULT  CSRGram::ListRemove (WCHAR *pszListName, SDATA dWord)
{
   if (m_pISRGramCFG)
      return m_pISRGramCFG->ListRemove (pszListName, dWord);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::ListRemove (WCHAR *pszListName, PVOID pData, DWORD dwSize)
{
   SDATA d;
   d.pData = pData;
   d.dwSize = dwSize;
   return ListRemove (pszListName, d);
}

HRESULT  CSRGram::ListSet (WCHAR *pszListName, SDATA dWord)
{
   if (m_pISRGramCFG)
      return m_pISRGramCFG->ListSet (pszListName, dWord);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::ListSet (WCHAR *pszListName, PVOID pData, DWORD dwSize)
{
   SDATA d;
   d.pData = pData;
   d.dwSize = dwSize;
   return ListSet (pszListName, d);
}


// ISRGramDictation
HRESULT  CSRGram::Context(PCWSTR pszPrior, PCWSTR pszAfter)
{
   if (m_pISRGramDictation)
      return m_pISRGramDictation->Context (pszPrior, pszAfter);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::Hint (PCWSTR pszHint)
{
   if (m_pISRGramDictation)
      return m_pISRGramDictation->Hint (pszHint);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::Words (PCWSTR pszWords)
{
   if (m_pISRGramDictation)
      return m_pISRGramDictation->Words (pszWords);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// ISRGramInsertionGUI
HRESULT  CSRGram::Hide (void)
{
   if (m_pISRGramInsertionGUI)
      return m_pISRGramInsertionGUI->Hide ();
   else
      return ResultFromScode (E_NOINTERFACE);
}


HRESULT  CSRGram::Move (RECT rCursor)
{
   if (m_pISRGramInsertionGUI)
      return m_pISRGramInsertionGUI->Move (rCursor);
   else
      return ResultFromScode (E_NOINTERFACE);
}


#ifdef STRICT
HRESULT  CSRGram::Show (PVOID hWnd)
#else
HRESULT  CSRGram::Show (HWND hWnd)
#endif
{
   if (m_pISRGramInsertionGUI)
      return m_pISRGramInsertionGUI->Show ((HWND) hWnd);
   else
      return ResultFromScode (E_NOINTERFACE);
}

// ILexPronounce
HRESULT  CSRGram::Add(VOICECHARSET CharSet, WCHAR *pszText, WCHAR *pszPronounce, 
   VOICEPARTOFSPEECH PartOfSpeech, PVOID pEngineInfo, 
   DWORD dwEngineInfoSize)
{
   if (m_pILexPronounce)
      return m_pILexPronounce->Add (CharSet, pszText, pszPronounce,
         PartOfSpeech, pEngineInfo, dwEngineInfoSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::Get(VOICECHARSET CharSet, WCHAR *pszText, WORD wSense, 
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

HRESULT  CSRGram::Remove(WCHAR *pszText, WORD wSense)
{
   if (m_pILexPronounce)
      return m_pILexPronounce->Remove (pszText, wSense);
   else
      return ResultFromScode (E_NOINTERFACE);
}

// ILexPronounce2
HRESULT  CSRGram::AddTo(DWORD dwLex, VOICECHARSET CharSet, WCHAR *pszText, WCHAR *pszPronounce, 
   VOICEPARTOFSPEECH PartOfSpeech, PVOID pEngineInfo, 
   DWORD dwEngineInfoSize)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->AddTo (dwLex, CharSet, pszText, pszPronounce,
         PartOfSpeech, pEngineInfo, dwEngineInfoSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::GetFrom(DWORD dwLex, VOICECHARSET CharSet, WCHAR *pszText, WORD wSense, 
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

HRESULT  CSRGram::RemoveFrom(DWORD dwLex, WCHAR *pszText, WORD wSense)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->RemoveFrom (dwLex, pszText, wSense);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::QueryLexicons (BOOL f, DWORD *pdw)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->QueryLexicons (f, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::ChangeSpelling (DWORD dwLex, PCWSTR psz1, PCWSTR psz2)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->ChangeSpelling (dwLex, psz1, psz2);
   else
      return ResultFromScode (E_NOINTERFACE);
}


/* IAttributes */
HRESULT  CSRGram::DWORDGet (DWORD dwAttribute, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDGet (dwAttribute, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::DWORDSet (DWORD dwAttribute, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDSet (dwAttribute, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::StringGet (DWORD dwAttribute, PWSTR psz, DWORD dw, DWORD *pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringGet (dwAttribute,psz, dw, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::StringSet (DWORD dwAttribute, PCWSTR psz)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringSet (dwAttribute, psz);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::MemoryGet (DWORD dwAttribute, PVOID * ppMem, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemoryGet (dwAttribute, ppMem, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRGram::MemorySet (DWORD dwAttribute, PVOID pMem, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemorySet (dwAttribute, pMem, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}





/*********************************************************************
CSRResult */

CSRResult::CSRResult (void)
{
   m_pISRResAudio = NULL;
   m_pISRResAudioEx = NULL;
   m_pISRResBasic = NULL;
   m_pIAttributes = NULL;
   m_pISRResCorrection = NULL;
   m_pISRResEval = NULL;
   m_pISRResGraph = NULL;
   m_pISRResGraphEx = NULL;
   m_pISRResMemory = NULL;
   m_pISRResMerge = NULL;
   m_pISRResModifyGUI = NULL;
   m_pISRResScores = NULL;
   m_pISRResSpeaker = NULL;
}

CSRResult::~CSRResult (void)
{
   if (m_pISRResAudio)
      m_pISRResAudio->Release();
   if (m_pISRResAudioEx)
      m_pISRResAudioEx->Release();
   if (m_pISRResBasic)
      m_pISRResBasic->Release();
   if (m_pIAttributes)
      m_pIAttributes->Release();
   if (m_pISRResCorrection)
      m_pISRResCorrection->Release();
   if (m_pISRResEval)
      m_pISRResEval->Release();
   if (m_pISRResGraph)
      m_pISRResGraph->Release();
   if (m_pISRResGraphEx)
      m_pISRResGraphEx->Release();
   if (m_pISRResMemory)
      m_pISRResMemory->Release();
   if (m_pISRResMerge)
      m_pISRResMerge->Release();
   if (m_pISRResModifyGUI)
      m_pISRResModifyGUI->Release();
   if (m_pISRResScores)
      m_pISRResScores->Release();
   if (m_pISRResSpeaker)
      m_pISRResSpeaker->Release();
}



// initalization.
HRESULT  CSRResult::Init (LPUNKNOWN pUnkResult)
{
   // if already loaded then error
   if (m_pISRResBasic)
      return ResultFromScode (E_FAIL);

   pUnkResult->QueryInterface (IID_ISRResAudio, (PVOID*)&m_pISRResAudio);
   pUnkResult->QueryInterface (IID_ISRResAudioEx, (PVOID*)&m_pISRResAudioEx);
   pUnkResult->QueryInterface (IID_ISRResBasicW, (PVOID*)&m_pISRResBasic);
   pUnkResult->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   pUnkResult->QueryInterface (IID_ISRResCorrectionW, (PVOID*)&m_pISRResCorrection);
   pUnkResult->QueryInterface (IID_ISRResEval, (PVOID*)&m_pISRResEval);
   pUnkResult->QueryInterface (IID_ISRResGraphW, (PVOID*)&m_pISRResGraph);
   pUnkResult->QueryInterface (IID_ISRResGraphEx, (PVOID*)&m_pISRResGraphEx);
   pUnkResult->QueryInterface (IID_ISRResMemory, (PVOID*)&m_pISRResMemory);
   pUnkResult->QueryInterface (IID_ISRResMerge, (PVOID*)&m_pISRResMerge);
   pUnkResult->QueryInterface (IID_ISRResModifyGUI, (PVOID*)&m_pISRResModifyGUI);
   pUnkResult->QueryInterface (IID_ISRResScores, (PVOID*)&m_pISRResScores);
   pUnkResult->QueryInterface (IID_ISRResSpeakerW, (PVOID*)&m_pISRResSpeaker);

   return NOERROR;
}

// isrresaudio
HRESULT  CSRResult::GetWAV (PSDATA pWav)
{
   if (m_pISRResAudio)
      return m_pISRResAudio->GetWAV(pWav);
   else
      return ResultFromScode (E_NOINTERFACE);
}

// isrresaudio ex
HRESULT  CSRResult::GetWAV (PSDATA pWav, QWORD qwStart, QWORD qwEnd)
{
   if (m_pISRResAudioEx)
      return m_pISRResAudioEx->GetWAV(pWav, qwStart, qwEnd);
   else
      return ResultFromScode (E_NOINTERFACE);
}

// isrresbasic
HRESULT  CSRResult::FlagsGet (DWORD dwRank, DWORD *pdwFlags)
{
   if (m_pISRResBasic)
      return m_pISRResBasic->FlagsGet(dwRank, pdwFlags);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CSRResult::FlagsGet (DWORD dwRank)
{
   DWORD dwFlags = 0;
   FlagsGet (dwRank, &dwFlags);
   return dwFlags;
}

HRESULT  CSRResult::Identify (GUID *pgIdentity)
{
   if (m_pISRResBasic)
      return m_pISRResBasic->Identify(pgIdentity);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::PhraseGet (DWORD dwRank, PSRPHRASEW pSRPhrase,
   DWORD dwPhraseSize, DWORD *pdwPhraseNeeded)
{
   if (m_pISRResBasic)
      return m_pISRResBasic->PhraseGet(dwRank, pSRPhrase, dwPhraseSize, pdwPhraseNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::TimeGet (PQWORD pqTimeStampBegin, PQWORD pqTimeStampEnd)
{
   if (m_pISRResBasic)
      return m_pISRResBasic->TimeGet(pqTimeStampBegin, pqTimeStampEnd);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// isrrescorrection
HRESULT  CSRResult::Correction (PSRPHRASEW pSRPhrase, WORD wConfidence)
{
   if (m_pISRResCorrection)
      return m_pISRResCorrection->Correction(pSRPhrase, wConfidence);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::Validate (WORD wConfidence)
{
   if (m_pISRResCorrection)
      return m_pISRResCorrection->Validate(wConfidence);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// isrreseval
HRESULT  CSRResult::ReEvaluate (BOOL *pfChanged)
{
   if (m_pISRResEval)
      return m_pISRResEval->ReEvaluate(pfChanged);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// isrresgraph
HRESULT  CSRResult::BestPathPhoneme(DWORD dwRank, DWORD *padwPath, DWORD dwPathSize,
   DWORD *pdwPathNeeded)
{
   if (m_pISRResGraph)
      return m_pISRResGraph->BestPathPhoneme(dwRank, padwPath, dwPathSize, pdwPathNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::BestPathWord(DWORD dwRank, DWORD *padwPath,
   DWORD dwPathSize, DWORD *pdwPathNeeded)
{
   if (m_pISRResGraph)
      return m_pISRResGraph->BestPathWord(dwRank, padwPath, dwPathSize, pdwPathNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::GetPhonemeNode (DWORD dwPhonemeNode, PSRRESPHONEMENODE pNode,
   PWCHAR pcIPA, PWCHAR pcEngine)
{
   if (m_pISRResGraph)
      return m_pISRResGraph->GetPhonemeNode(dwPhonemeNode, pNode, pcIPA, pcEngine);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::GetWordNode (DWORD dwWordNode, PSRRESWORDNODE pNode, 
   PSRWORDW pSRWord, DWORD dwMemSize, DWORD *pdwMemNeeded)
{
   if (m_pISRResGraph)
      return m_pISRResGraph->GetWordNode(dwWordNode, pNode, pSRWord, dwMemSize, pdwMemNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::PathScorePhoneme(DWORD *paNodes, DWORD dwNumNodes,
   LONG *plScore)
{
   if (m_pISRResGraph)
      return m_pISRResGraph->PathScorePhoneme(paNodes, dwNumNodes, plScore);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::PathScoreWord (DWORD *paNodes, DWORD dwNumNodes, LONG *plScore)
{
   if (m_pISRResGraph)
      return m_pISRResGraph->PathScoreWord(paNodes, dwNumNodes, plScore);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// isrresgraphex
HRESULT  CSRResult::NodeStartGet(DWORD *pdw)
{
   if (m_pISRResGraphEx)
      return m_pISRResGraphEx->NodeStartGet(pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::NodeEndGet(DWORD *pdw)
{
   if (m_pISRResGraphEx)
      return m_pISRResGraphEx->NodeEndGet(pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::ArcEnum(DWORD dwNode, DWORD *pdwBuf, DWORD dwSize,
                            DWORD *pdwNum, BOOL fOutgoing)
{
   if (m_pISRResGraphEx)
      return m_pISRResGraphEx->ArcEnum(dwNode, pdwBuf, dwSize, pdwNum, fOutgoing);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::GetAllArcs(DWORD *padwArcID, DWORD dwSize, DWORD *pdwNumArcs,
                               DWORD *pdwNeeded)
{
   if (m_pISRResGraphEx)
      return m_pISRResGraphEx->GetAllArcs(padwArcID, dwSize, pdwNumArcs, pdwNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}


HRESULT  CSRResult::GetAllNodes(DWORD *padwArcID, DWORD dwSize, DWORD *pdwNumArcs,
                               DWORD *pdwNeeded)
{
   if (m_pISRResGraphEx)
      return m_pISRResGraphEx->GetAllNodes(padwArcID, dwSize, pdwNumArcs, pdwNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::NodeGet(DWORD dwArcID, DWORD *pdwNode, BOOL fDestination)
{
   if (m_pISRResGraphEx)
      return m_pISRResGraphEx->NodeGet(dwArcID, pdwNode, fDestination);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::DWORDGet(DWORD dwID, GUID gAttrib, DWORD *pdwValue)
{
   if (m_pISRResGraphEx)
      return m_pISRResGraphEx->DWORDGet(dwID, gAttrib, pdwValue);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::DataGet(DWORD dwID, GUID gAttrib, SDATA *psData)
{
   if (m_pISRResGraphEx)
      return m_pISRResGraphEx->DataGet(dwID, gAttrib, psData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::ScoreGet(DWORD dwScoreType, DWORD *pdwPath,
                             DWORD dwPathSteps, DWORD dwPathIndexStart,
                             DWORD dwPathIndexCount, DWORD *pdwScore)
{
   if (m_pISRResGraphEx)
      return m_pISRResGraphEx->ScoreGet(dwScoreType, pdwPath, dwPathSteps, dwPathIndexStart,
         dwPathIndexCount, pdwScore);
   else
      return ResultFromScode (E_NOINTERFACE);
}


HRESULT  CSRResult::BestPathEnum(DWORD dwRank, DWORD *pdwStartPath,
                                 DWORD dwStartPathSteps, DWORD *padwEndPath,
                                 DWORD dwEndPathSteps, BOOL fExactMatch,
                                 PSDATA psData)
{
   if (m_pISRResGraphEx)
      return m_pISRResGraphEx->BestPathEnum(dwRank, pdwStartPath, dwStartPathSteps,
         padwEndPath, dwEndPathSteps, fExactMatch, psData);
   else
      return ResultFromScode (E_NOINTERFACE);
}



// isrresmemory
HRESULT  CSRResult::Free (DWORD dwKind)
{
   if (m_pISRResMemory)
      return m_pISRResMemory->Free(dwKind);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::Get (DWORD *pdwKind, DWORD *pdwMemory)
{
   if (m_pISRResMemory)
      return m_pISRResMemory->Get(pdwKind, pdwMemory);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::LockGet(BOOL *pfLock)
{
   if (m_pISRResMemory)
      return m_pISRResMemory->LockGet(pfLock);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::LockSet(BOOL fLock)
{
   if (m_pISRResMemory)
      return m_pISRResMemory->LockSet(fLock);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// isrresmerge
HRESULT  CSRResult::Merge (LPUNKNOWN pIUnkAdjacent, LPUNKNOWN *ppIUnkMerge)
{
   if (m_pISRResMerge)
      return m_pISRResMerge->Merge(pIUnkAdjacent, ppIUnkMerge);
   else
      return ResultFromScode (E_NOINTERFACE);
}

CSRResult* CSRResult::Merge(CSRResult * pAdjacent)
{
   LPUNKNOWN   pUnkMerge;
   PCSRResult  pNew;
   HRESULT     hRes;
   LPUNKNOWN   pIUnkAdjacent;

   if (FAILED(pAdjacent->m_pISRResMerge->QueryInterface(IID_IUnknown, (void **) &pIUnkAdjacent)))
      return NULL;
   hRes = Merge (pIUnkAdjacent, &pUnkMerge);
   pIUnkAdjacent->Release();
   if (hRes)
      return NULL;
   pNew = new CSRResult;
   if (!pNew) {
      pUnkMerge->Release();
      return NULL;
   }
   hRes = pNew->Init (pUnkMerge);
   pUnkMerge->Release();
   if (hRes) {
      delete pNew;
      return NULL;
   }

   return pNew;
}

HRESULT CSRResult::Merge(CSRResult * pAdjacent, CSRResult **ppMerged)
{
   LPUNKNOWN   pUnkMerge;
   PCSRResult  pNew;
   HRESULT     hRes;
   LPUNKNOWN   pIUnkAdjacent;

   if (FAILED(hRes = pAdjacent->m_pISRResMerge->QueryInterface(IID_IUnknown, (void **) &pIUnkAdjacent)))
      return hRes;
   hRes = Merge (pIUnkAdjacent, &pUnkMerge);
   pIUnkAdjacent->Release();
   if (hRes)
      return hRes;
   pNew = new CSRResult;
   if (!pNew) {
      pUnkMerge->Release();
      return hRes;
   }
   hRes = pNew->Init (pUnkMerge);
   pUnkMerge->Release();
   if (hRes) {
      delete pNew;
      return hRes;
   }

   *ppMerged = pNew;
   return NOERROR;
}

HRESULT  CSRResult::Split (QWORD qwSplitTime, LPUNKNOWN *ppIUnkLeft, 
   LPUNKNOWN *ppIUnkRight)
{
   if (m_pISRResMerge)
      return m_pISRResMerge->Split(qwSplitTime, ppIUnkLeft, ppIUnkRight);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::Split (QWORD qwSplitTime, CSRResult **ppLeft, CSRResult **ppRight)
{
   *ppLeft = NULL;
   *ppRight = NULL;

   HRESULT  hRes;
   LPUNKNOWN   pUnkLeft, pUnkRight;

   hRes = Split (qwSplitTime, &pUnkLeft, &pUnkRight);
   if (hRes)
      return hRes;
   if (pUnkLeft) {
      *ppLeft = new CSRResult;
      (*ppLeft)->Init (pUnkLeft);
      pUnkLeft->Release();
   }
   if (pUnkRight) {
      *ppRight = new CSRResult;
      (*ppRight)->Init (pUnkRight);
      pUnkRight->Release();
   }

   return NOERROR;
}


// isrresmmodifygui
HRESULT  CSRResult::Hide (void)
{
   if (m_pISRResModifyGUI)
      return m_pISRResModifyGUI->Hide();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::Move (RECT * rSelection)
{
   if (m_pISRResModifyGUI)
      return m_pISRResModifyGUI->Move(rSelection);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSRResult::Show (PVOID hWnd)
#else
HRESULT  CSRResult::Show (HWND hWnd)
#endif
{
   if (m_pISRResModifyGUI)
      return m_pISRResModifyGUI->Show((HWND) hWnd);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// isrresscore
HRESULT  CSRResult::GetPhraseScore (DWORD dwRank, long *plScore)
{
   if (m_pISRResScores)
      return m_pISRResScores->GetPhraseScore(dwRank, plScore);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::GetWordScores (DWORD dwRank, long *palScore,
   DWORD dwWordScoreSize, DWORD *pdwWordScoreRequired)
{
   if (m_pISRResScores)
      return m_pISRResScores->GetWordScores(dwRank, palScore, dwWordScoreSize, pdwWordScoreRequired);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// isrresspeaker
HRESULT  CSRResult::SpeakerCorrection (PCWSTR pszName, WORD wConfidence)
{
   if (m_pISRResSpeaker)
      return m_pISRResSpeaker->Correction(pszName, wConfidence);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::SpeakerIdentify (DWORD dwRank, WCHAR *pszName, DWORD dwNameSize,
   DWORD *pdwNeeded, LONG *plScore)
{
   if (m_pISRResSpeaker)
      return m_pISRResSpeaker->Identify(dwRank, pszName, dwNameSize, pdwNeeded, plScore);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::SpeakerIdentifyForFree (BOOL *pfFree)
{
   if (m_pISRResSpeaker)
      return m_pISRResSpeaker->IdentifyForFree(pfFree);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::SpeakerValidate (WORD wConfidence)
{
   if (m_pISRResSpeaker)
      return m_pISRResSpeaker->Validate(wConfidence);
   else
      return ResultFromScode (E_NOINTERFACE);
}

/* IAttributes */
HRESULT  CSRResult::DWORDGet (DWORD dwAttribute, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDGet (dwAttribute, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::DWORDSet (DWORD dwAttribute, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDSet (dwAttribute, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::StringGet (DWORD dwAttribute, PWSTR psz, DWORD dw, DWORD *pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringGet (dwAttribute,psz, dw, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::StringSet (DWORD dwAttribute, PCWSTR psz)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringSet (dwAttribute, psz);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::MemoryGet (DWORD dwAttribute, PVOID * ppMem, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemoryGet (dwAttribute, ppMem, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSRResult::MemorySet (DWORD dwAttribute, PVOID pMem, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemorySet (dwAttribute, pMem, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}



/*****************************************************************************
CSRNotifySink */


STDMETHODIMP CSRNotifySink::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRNotifySinkW) ||
      IsEqualIID(riid,IID_ISRNotifySink2))
   {
      *ppv = (LPVOID) this;
      AddRef();
       return ResultFromScode (NOERROR);
   }   // otherwise, cant find


   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CSRNotifySink::AddRef (void)
{
   return(++m_RefCount);
}

STDMETHODIMP_(ULONG) CSRNotifySink::Release (void)
{
   if((--m_RefCount) == 0)
   {
	delete this;
	return(0);
   }
   return(m_RefCount);
};


STDMETHODIMP CSRNotifySink::AttribChanged (DWORD dwID)
{
   return NOERROR;
}

STDMETHODIMP CSRNotifySink::Interference (QWORD qwBegin, QWORD qwEnd, DWORD dwNum)
{
   return NOERROR;
}

STDMETHODIMP CSRNotifySink::Sound (QWORD qwBegin, QWORD qwEnd)
{
   return NOERROR;
}

STDMETHODIMP CSRNotifySink::UtteranceBegin (QWORD qwTime)
{
   return NOERROR;
}

STDMETHODIMP CSRNotifySink::UtteranceEnd (QWORD qwBegin, QWORD qwEnd)
{
   return NOERROR;
}

STDMETHODIMP CSRNotifySink::VUMeter (QWORD qwTime, WORD wLevel)
{
   return NOERROR;
}

STDMETHODIMP CSRNotifySink::Training (void)
{
   return NOERROR;
}

STDMETHODIMP CSRNotifySink::Error (LPUNKNOWN pUnk)
{
   return NOERROR;
}

STDMETHODIMP CSRNotifySink::Warning (LPUNKNOWN pUnk)
{
   return NOERROR;
}



/**************************************************************************8
CSRGramNotifySink */


STDMETHODIMP CSRGramNotifySink::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRGramNotifySinkW))
   {
      *ppv = (LPVOID) this;
       AddRef();
	return ResultFromScode (NOERROR);
   }   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CSRGramNotifySink::AddRef (void)
{
   return(++m_RefCount);
}

STDMETHODIMP_(ULONG) CSRGramNotifySink::Release (void)
{
   if((--m_RefCount) == 0)
   {
	delete this;
	return(0);
   }
   return(m_RefCount);
};

STDMETHODIMP CSRGramNotifySink::BookMark(DWORD dwID)
{
   return NOERROR;
}

STDMETHODIMP CSRGramNotifySink::Paused(void)
{
   return NOERROR;
}

STDMETHODIMP CSRGramNotifySink::PhraseFinish(DWORD dwFlags, QWORD qTimeStampBegin,
   QWORD qTimeStampEnd, PSRPHRASEW pSRPhrase, LPUNKNOWN lpResults)
{
	return NOERROR;
}


STDMETHODIMP CSRGramNotifySink::PhraseHypothesis(DWORD dwFlags, QWORD qTimeStampBegin,
   QWORD qTimeStampEnd, PSRPHRASEW pSRPhrase, LPUNKNOWN lpResults)
{
   return NOERROR;
}

STDMETHODIMP CSRGramNotifySink::PhraseStart(QWORD qTimeStampBegin)
{
   return NOERROR;
}

STDMETHODIMP CSRGramNotifySink::ReEvaluate(LPUNKNOWN lpUnk)
{
   return NOERROR;
}

STDMETHODIMP CSRGramNotifySink::Training(DWORD dwTrain)
{
   return NOERROR;
}

STDMETHODIMP CSRGramNotifySink::UnArchive(LPUNKNOWN lpUnk)
{
   return NOERROR;
}

