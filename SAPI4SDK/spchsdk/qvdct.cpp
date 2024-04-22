/*********************************************************************
VDct.Cpp - Voice Dictation C++ wrapper objects.

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
Voice Dictation */

CVoiceDictation::CVoiceDictation (void)
{
   m_pIVDctAttributes = NULL;
   m_pIAttributes = NULL;
   m_pIVDctCommands = NULL;
   m_pIVDctCommandsBuiltIn = NULL;
   m_pIVDctCommandsApp = NULL;
   m_pIVDctDialogs = NULL;
   m_pIVDctGlossary = NULL;
   m_pIVDctGUI = NULL;
   m_pIVDctInvTextNorm = NULL;
   m_pIVDctText = NULL;
   m_pIVoiceDictation = NULL;
   m_pISRSpeaker = NULL;
   m_pILexPronounce = NULL;
   m_pILexPronounce2 = NULL;
   m_pIVDctTextCache = NULL;
   m_pIVDctText2 = NULL;
}

CVoiceDictation::~CVoiceDictation (void)
{
   if (m_pIVDctAttributes)
      m_pIVDctAttributes->Release();
   if (m_pIAttributes)
      m_pIAttributes->Release();
   if (m_pIVDctCommands)
      m_pIVDctCommands->Release();
   if (m_pIVDctCommandsBuiltIn)
      m_pIVDctCommandsBuiltIn->Release();
   if (m_pIVDctCommandsApp)
      m_pIVDctCommandsApp->Release();
   if (m_pIVDctDialogs)
      m_pIVDctDialogs->Release();
   if (m_pIVDctGlossary)
      m_pIVDctGlossary->Release();
   if (m_pIVDctGUI)
      m_pIVDctGUI->Release();
   if (m_pIVDctInvTextNorm)
      m_pIVDctInvTextNorm->Release();
   if (m_pIVDctText)
      m_pIVDctText->Release();
   if (m_pIVoiceDictation)
      m_pIVoiceDictation->Release();
   if (m_pISRSpeaker)
      m_pISRSpeaker->Release();
   if (m_pILexPronounce)
      m_pILexPronounce->Release();
   if (m_pILexPronounce2)
      m_pILexPronounce2->Release();
   if (m_pIVDctTextCache)
      m_pIVDctTextCache->Release();
   if (m_pIVDctText2)
      m_pIVDctText->Release();
}

// initalization
HRESULT  CVoiceDictation::Init (void)
{
   HRESULT  hRes;
   LPUNKNOWN   lpUnk;

   hRes = CoCreateInstance (CLSID_VDct, NULL, CLSCTX_LOCAL_SERVER,
      IID_IUnknown, (void**)&lpUnk);
   if (hRes)
      return hRes;

   hRes = Init (lpUnk);
   lpUnk->Release();
   return hRes;

   return NOERROR;
}

HRESULT  CVoiceDictation::Init (LPUNKNOWN pIUnkVDct)
{
   if (m_pIVoiceDictation)
      return ResultFromScode (E_FAIL);

   HRESULT  hRes;
   hRes = pIUnkVDct->QueryInterface (IID_IVoiceDictationW, (PVOID*)&m_pIVoiceDictation);
   if (hRes)
      return hRes;

   // other interfaces
   pIUnkVDct->QueryInterface (IID_IVDctAttributesW, (PVOID*)&m_pIVDctAttributes);
   pIUnkVDct->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   pIUnkVDct->QueryInterface (IID_IVDctCommandsW, (PVOID*)&m_pIVDctCommands);
   pIUnkVDct->QueryInterface (IID_IVDctCommandsBuiltInW, (PVOID*)&m_pIVDctCommandsBuiltIn);
   pIUnkVDct->QueryInterface (IID_IVDctCommandsAppW, (PVOID*)&m_pIVDctCommandsApp);
   pIUnkVDct->QueryInterface (IID_IVDctDialogsW, (PVOID*)&m_pIVDctDialogs);
   pIUnkVDct->QueryInterface (IID_IVDctGlossaryW, (PVOID*)&m_pIVDctGlossary);
   pIUnkVDct->QueryInterface (IID_IVDctGUI, (PVOID*)&m_pIVDctGUI);
   pIUnkVDct->QueryInterface (IID_IVDctInvTextNormW, (PVOID*)&m_pIVDctInvTextNorm);
   pIUnkVDct->QueryInterface (IID_IVDctTextW, (PVOID*)&m_pIVDctText);
   pIUnkVDct->QueryInterface (IID_ISRSpeakerW, (PVOID*)&m_pISRSpeaker);
   pIUnkVDct->QueryInterface (IID_ILexPronounceW, (PVOID*)&m_pILexPronounce);
   pIUnkVDct->QueryInterface (IID_ILexPronounce2W, (PVOID*)&m_pILexPronounce2);
   pIUnkVDct->QueryInterface (IID_IVDctTextCache, (PVOID*)&m_pIVDctTextCache);
   pIUnkVDct->QueryInterface (IID_IVDctText2W, (PVOID*)&m_pIVDctText2);

   return NOERROR;
}


// initialization and registration combined
HRESULT  CVoiceDictation::Init (PCWSTR pszApplication, PCWSTR pszTopic,
   IStorage* pISessionData, PCWSTR pszSite,
   PVOID pNotifyInterface, IID IIDNotifyInterfaceW, DWORD dwFlags)
{
   HRESULT  hRes;

   hRes = Init ();
   if (hRes)
      return hRes;

   // register
   hRes = Register (pszApplication, pszTopic, pISessionData, pszSite,
      pNotifyInterface, IIDNotifyInterfaceW, dwFlags);
   return hRes;
}

HRESULT  CVoiceDictation::Init (PCWSTR pszApplication, PIVDCTNOTIFYSINKW pNotifyInterface,
   PCWSTR pszTopic,
   PCWSTR pszSite,
   DWORD dwFlags,
   IStorage* pISessionDat)
{
   HRESULT  hRes;

   hRes = Init ();
   if (hRes)
      return hRes;

   // register
   hRes = Register (pszApplication, pNotifyInterface, pszTopic, pszSite,
      dwFlags, pISessionDat);
   return hRes;
}


// IVDctAttributes
HRESULT  CVoiceDictation::AutoGainEnableGet (DWORD *pdwAutoGain)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->AutoGainEnableGet (pdwAutoGain);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceDictation::AutoGainEnableGet (void)
{
   DWORD dwAutoGain = 0;
   AutoGainEnableGet (&dwAutoGain);
   return dwAutoGain;
}

HRESULT  CVoiceDictation::AutoGainEnableSet (DWORD dwAutoGain)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->AutoGainEnableSet (dwAutoGain);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::EchoGet (BOOL *pfEcho)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->EchoGet (pfEcho);
   else
      return ResultFromScode (E_NOINTERFACE);
}

BOOL     CVoiceDictation::EchoGet (void)
{
   BOOL fEcho = 0;
   EchoGet (&fEcho);
   return fEcho;
}

HRESULT  CVoiceDictation::EchoSet (BOOL fEcho)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->EchoSet (fEcho);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::EnergyFloorGet (WORD *pwEnergy)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->EnergyFloorGet (pwEnergy);
   else
      return ResultFromScode (E_NOINTERFACE);
}

WORD     CVoiceDictation::EnergyFloorGet (void)
{
   WORD wEnergy = 0;
   EnergyFloorGet (&wEnergy);
   return wEnergy;
}

HRESULT  CVoiceDictation::EnergyFloorSet (WORD wEnergy)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->EnergyFloorSet (wEnergy);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::MicrophoneGet (WCHAR *pszMicrophone, DWORD dwMicrophoneSize, DWORD *pdwNeeded)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->MicrophoneGet (pszMicrophone, dwMicrophoneSize, pdwNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::MicrophoneSet (WCHAR *pszMicrophone)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->MicrophoneSet (pszMicrophone);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::RealTimeGet (DWORD *pdwRealTime)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->RealTimeGet (pdwRealTime);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceDictation::RealTimeGet (void)
{
   DWORD dwRealTime = 0;
   RealTimeGet (&dwRealTime);
   return dwRealTime;
}

HRESULT  CVoiceDictation::RealTimeSet (DWORD dwRealTime)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->RealTimeSet (dwRealTime);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::SpeakerGet (WCHAR *pszSpeaker, DWORD dwSpeakerSize, DWORD *pdwNeeded)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->SpeakerGet (pszSpeaker, dwSpeakerSize, pdwNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::SpeakerSet (WCHAR *pszSpeaker)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->SpeakerSet (pszSpeaker);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::ThresholdGet (DWORD *pdwThreshold)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->ThresholdGet (pdwThreshold);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceDictation::ThresholdGet (void)
{
   DWORD dwThresholdGet = 0;
   ThresholdGet (&dwThresholdGet);
   return dwThresholdGet;
}

HRESULT  CVoiceDictation::ThresholdSet (DWORD dwThreshold)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->ThresholdSet (dwThreshold);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TimeOutGet (DWORD *pdwIncomplete, DWORD *pdwComplete)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->TimeOutGet (pdwIncomplete, pdwComplete);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TimeOutSet(DWORD dwIncomplete, DWORD dwComplete)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->TimeOutSet (dwIncomplete, dwComplete);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceDictation::IsAnyoneDictating(PVOID hWnd, WCHAR *pszName, DWORD dwNameSize, DWORD *pdwNameNeeded)
#else
HRESULT  CVoiceDictation::IsAnyoneDictating(HWND hWnd, WCHAR *pszName, DWORD dwNameSize, DWORD *pdwNameNeeded)
#endif
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->IsAnyoneDictating ((HWND) hWnd, pszName, dwNameSize, pdwNameNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::MemoryGet (VDCTMEMORY *pDctMemory)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->MemoryGet (pDctMemory);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::MemorySet (VDCTMEMORY *pDctMemory)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->MemorySet (pDctMemory);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::ModeGet (DWORD *pdwMode)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->ModeGet (pdwMode);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceDictation::ModeGet (void)
{
   DWORD dw = 0;
   ModeGet (&dw);
   return dw;
}

HRESULT  CVoiceDictation::ModeSet (DWORD dwMode)
{
   if (m_pIVDctAttributes)
      return m_pIVDctAttributes->ModeSet (dwMode);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// IVDctCommands
HRESULT  CVoiceDictation::CommandAdd (BOOL fGlobal, DWORD dwCmdNum, SDATA dData, DWORD *pdwCmdStart)
{
   if (m_pIVDctCommands)
      return m_pIVDctCommands->Add (fGlobal, dwCmdNum, dData, pdwCmdStart);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::CommandGet (BOOL fGlobal, DWORD dwCmdStart, DWORD dwCmdNum, DWORD dwFlag,
   PSDATA pdData, DWORD *pdwCmdNum)
{
   if (m_pIVDctCommands)
      return m_pIVDctCommands->Get (fGlobal, dwCmdStart, dwCmdNum, dwFlag,
         pdData, pdwCmdNum);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::CommandNum (BOOL fGlobal, DWORD *pdwNumCmd)
{
   if (m_pIVDctCommands)
      return m_pIVDctCommands->Num (fGlobal, pdwNumCmd);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceDictation::CommandNum (BOOL fGlobal)
{
   DWORD dw = 0;
   CommandNum (fGlobal, &dw);
   return dw;
}

HRESULT  CVoiceDictation::CommandRemove (BOOL fGlobal, DWORD dwCmdStart, DWORD dwCmdNum, DWORD dwFlag)
{
   if (m_pIVDctCommands)
      return m_pIVDctCommands->Remove (fGlobal, dwCmdStart, dwCmdNum, dwFlag);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::CommandSet(BOOL fGlobal, DWORD dwCmdStart, DWORD dwCmdNum, DWORD dwFlag, 
   SDATA dData)
{
   if (m_pIVDctCommands)
      return m_pIVDctCommands->Set (fGlobal, dwCmdStart, dwCmdNum, dwFlag, dData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::CommandEnableItem (BOOL fGlobal, DWORD dwEnable, DWORD dwCmdNum, DWORD dwFlag)
{
   if (m_pIVDctCommands)
      return m_pIVDctCommands->EnableItem (fGlobal, dwEnable, dwCmdNum, dwFlag);
   else
      return ResultFromScode (E_NOINTERFACE);
}



// IVDctDialogs
#ifdef STRICT
HRESULT  CVoiceDictation::AboutDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CVoiceDictation::AboutDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pIVDctDialogs)
      return m_pIVDctDialogs->AboutDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceDictation::GeneralDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CVoiceDictation::GeneralDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pIVDctDialogs)
      return m_pIVDctDialogs->GeneralDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceDictation::LexiconDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CVoiceDictation::LexiconDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pIVDctDialogs)
      return m_pIVDctDialogs->LexiconDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceDictation::TrainGeneralDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CVoiceDictation::TrainGeneralDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pIVDctDialogs)
      return m_pIVDctDialogs->TrainGeneralDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceDictation::TrainMicDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CVoiceDictation::TrainMicDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pIVDctDialogs)
      return m_pIVDctDialogs->TrainMicDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

// IVDctGlossary
HRESULT  CVoiceDictation::GlossaryAdd (BOOL fGlobal, DWORD dwCmdNum, SDATA dData, DWORD *pdwCmdStart)
{
   if (m_pIVDctGlossary)
      return m_pIVDctGlossary->Add (fGlobal, dwCmdNum, dData, pdwCmdStart);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::GlossaryGet (BOOL fGlobal, DWORD dwCmdStart, DWORD dwCmdNum, DWORD dwFlag,
   PSDATA pdData, DWORD *pdwCmdNum)
{
   if (m_pIVDctGlossary)
      return m_pIVDctGlossary->Get (fGlobal, dwCmdStart, dwCmdNum, dwFlag,
         pdData, pdwCmdNum);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::GlossaryNum (BOOL fGlobal, DWORD *pdwNumCmd)
{
   if (m_pIVDctGlossary)
      return m_pIVDctGlossary->Num (fGlobal, pdwNumCmd);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceDictation::GlossaryNum (BOOL fGlobal)
{
   DWORD dw = 0;
   GlossaryNum (fGlobal, &dw);
   return dw;
}

HRESULT  CVoiceDictation::GlossaryRemove (BOOL fGlobal, DWORD dwCmdStart, DWORD dwCmdNum, DWORD dwFlag)
{
   if (m_pIVDctGlossary)
      return m_pIVDctGlossary->Remove (fGlobal, dwCmdStart, dwCmdNum, dwFlag);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::GlossarySet(BOOL fGlobal, DWORD dwCmdStart, DWORD dwCmdNum, DWORD dwFlag, 
   SDATA dData)
{
   if (m_pIVDctGlossary)
      return m_pIVDctGlossary->Set (fGlobal, dwCmdStart, dwCmdNum, dwFlag, dData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::GlossaryEnableItem (BOOL fGlobal, DWORD dwEnable, DWORD dwCmdNum, DWORD dwFlag)
{
   if (m_pIVDctGlossary)
      return m_pIVDctGlossary->EnableItem (fGlobal, dwEnable, dwCmdNum, dwFlag);
   else
      return ResultFromScode (E_NOINTERFACE);
}



// IVDctGUI
HRESULT  CVoiceDictation::FlagsGet (DWORD *pdwFlags)
{
   if (m_pIVDctGUI)
      return m_pIVDctGUI->FlagsGet (pdwFlags);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceDictation::FlagsGet (void)
{
   DWORD dw = 0;
   FlagsGet (&dw);
   return dw;
}

HRESULT  CVoiceDictation::FlagsSet (DWORD dwFlags)
{
   if (m_pIVDctGUI)
      return m_pIVDctGUI->FlagsSet (dwFlags);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::SetSelRect (RECT *pRect)
{
   if (m_pIVDctGUI)
      return m_pIVDctGUI->SetSelRect (pRect);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// IVDctTextNorm
HRESULT  CVoiceDictation::OptionsEnum (PWSTR *ppOptions, DWORD *pdwSize)
{
   if (m_pIVDctInvTextNorm)
      return m_pIVDctInvTextNorm->OptionsEnum (ppOptions, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::OptionsGet (PCWSTR pszOptionName, BOOL *pfOn)
{
   if (m_pIVDctInvTextNorm)
      return m_pIVDctInvTextNorm->OptionsGet (pszOptionName, pfOn);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::OptionsSet (PCWSTR pszOptionName, BOOL fOn)
{
   if (m_pIVDctInvTextNorm)
      return m_pIVDctInvTextNorm->OptionsSet (pszOptionName, fOn);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// IVDctText
HRESULT  CVoiceDictation::BookmarkAdd (VDCTBOOKMARK *pBookMark)
{
   if (m_pIVDctText)
      return m_pIVDctText->BookmarkAdd (pBookMark);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BookmarkEnum (DWORD dwStart, DWORD dwNumChars,
   PVDCTBOOKMARK *ppBookMark, DWORD *pdwNumMarks)
{
   if (m_pIVDctText)
      return m_pIVDctText->BookmarkEnum (dwStart, dwNumChars,
         ppBookMark, pdwNumMarks);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BookmarkQuery (DWORD dwID, VDCTBOOKMARK *pBookMark)
{
   if (m_pIVDctText)
      return m_pIVDctText->BookmarkQuery (dwID, pBookMark);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BookmarkRemove (DWORD dwMark)
{
   if (m_pIVDctText)
      return m_pIVDctText->BookmarkRemove (dwMark);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::FX(DWORD dwFX)
{
   if (m_pIVDctText)
      return m_pIVDctText->FX (dwFX);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::GetChanges (DWORD *pdwNewStart, DWORD *pdwNewEnd,
   DWORD *pdwOldStart, DWORD *pdwOldEnd)
{
   if (m_pIVDctText)
      return m_pIVDctText->GetChanges (pdwNewStart, pdwNewEnd,
         pdwOldStart, pdwOldEnd);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Hint (PCWSTR pszHint)
{
   if (m_pIVDctText)
      return m_pIVDctText->Hint (pszHint);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Lock ()
{
   if (m_pIVDctText)
      return m_pIVDctText->Lock ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::ResultsGet (DWORD dwStart, DWORD dwNumChars,
   DWORD *pdwPhraseStart, DWORD *pdwPhraseNumChars,
   LPUNKNOWN *ppIUnkPhraseResults)
{
   if (m_pIVDctText)
      return m_pIVDctText->ResultsGet (dwStart, dwNumChars,
         pdwPhraseStart, pdwPhraseNumChars, ppIUnkPhraseResults);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::ResultsGet (DWORD dwStart, DWORD dwNumChars,
   DWORD *pdwPhraseStart, DWORD *pdwPhraseNumChars,
   PCSRResult *ppCSRResult)
{
   LPUNKNOWN   pUnk;
   HRESULT     hRes;
   hRes = ResultsGet (dwStart, dwNumChars, pdwPhraseStart, pdwPhraseNumChars,
      &pUnk);
   if (hRes)
      return hRes;

   PCSRResult  pRes;
   pRes = new CSRResult;
   if (!pRes) {
      pUnk->Release();
      return ResultFromScode(E_OUTOFMEMORY);
   }
   pRes->Init (pUnk);
   pUnk->Release();
   return NOERROR;
}

HRESULT  CVoiceDictation::TextGet (DWORD dwStart, DWORD dwNumChars, PSDATA pData)
{
   if (m_pIVDctText)
      return m_pIVDctText->TextGet (dwStart, dwNumChars,
         pData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TextMove (DWORD dwStart, DWORD dwNumChars,
   DWORD dwMoveTo, DWORD dwReason)
{
   if (m_pIVDctText)
      return m_pIVDctText->TextMove (dwStart, dwNumChars, dwMoveTo, dwReason);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TextRemove (DWORD dwStart, DWORD dwNumChars,
   DWORD dwReason)
{
   if (m_pIVDctText)
      return m_pIVDctText->TextRemove (dwStart, dwNumChars, dwReason);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TextSelGet (DWORD *pdwStart, DWORD *pdwNumChars)
{
   if (m_pIVDctText)
      return m_pIVDctText->TextSelGet (pdwStart, pdwNumChars);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TextSelSet (DWORD dwStart, DWORD dwNumChars)
{
   if (m_pIVDctText)
      return m_pIVDctText->TextSelSet (dwStart, dwNumChars);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TextSet (DWORD dwStart, DWORD dwNumChars,
   PCWSTR pszBuf, DWORD dwReason)
{
   if (m_pIVDctText)
      return m_pIVDctText->TextSet (dwStart, dwNumChars, pszBuf, dwReason);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::UnLock()
{
   if (m_pIVDctText)
      return m_pIVDctText->UnLock ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Words (PCWSTR pszWords)
{
   if (m_pIVDctText)
      return m_pIVDctText->Words (pszWords);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// IVDctText2
HRESULT  CVoiceDictation::ITNApply (DWORD dwStart, DWORD dwNumChars)
{
   if (m_pIVDctText2)
      return m_pIVDctText2->ITNApply (dwStart, dwNumChars);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::ITNExpand (DWORD dwStart, DWORD dwNumChars)
{
   if (m_pIVDctText2)
      return m_pIVDctText2->ITNExpand (dwStart, dwNumChars);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::ResultsGet2 (DWORD dwStart, DWORD dwNumChars,
   DWORD *pdwPhraseStart, DWORD *pdwPhraseNumChars,
	LPUNKNOWN *ppIUnkPhraseResults, DWORD *pdwNodeLeft, DWORD *pdwNodeRight)
{
   if (m_pIVDctText2)
      return m_pIVDctText2->ResultsGet2 (dwStart, dwNumChars,
         pdwPhraseStart, pdwPhraseNumChars,
	      ppIUnkPhraseResults, pdwNodeLeft, pdwNodeRight);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::ResultsSet (DWORD dwStart, DWORD dwNumChars, 
				               LPUNKNOWN pIUnkPhraseResults, DWORD dwNodeLeft, DWORD dwNodeRight)
{
   if (m_pIVDctText2)
      return m_pIVDctText2->ResultsSet (dwStart, dwNumChars,
         pIUnkPhraseResults, dwNodeLeft, dwNodeRight);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// IVoiceDictation
#ifdef STRICT
HRESULT  CVoiceDictation::Activate(PVOID hWnd)
#else
HRESULT  CVoiceDictation::Activate(HWND hWnd)
#endif
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->Activate ((HWND) hWnd);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::CFGSet(LANGID lang, PCWSTR pszTopic, PCWSTR pszCFG)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->CFGSet (lang, pszTopic, pszCFG);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Deactivate(void)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->Deactivate ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Register (PCWSTR pszApplication, PCWSTR pszTopic,
   IStorage* pISessionData, PCWSTR pszSite,
   PVOID pNotifyInterface, IID IIDNotifyInterfaceW, DWORD dwFlags)
{
   if (m_pIVoiceDictation) {
      HRESULT  hRes;
      hRes = m_pIVoiceDictation->Register (pszApplication, pszTopic,
         pISessionData, pszSite, (PIVDCTNOTIFYSINK) pNotifyInterface, IIDNotifyInterfaceW,
         dwFlags);

      // ilexpronounce changes depending upon if registered
      if (!m_pILexPronounce)
         m_pIVoiceDictation->QueryInterface (IID_ILexPronounceW, (PVOID*)&m_pILexPronounce);
      if (!m_pILexPronounce2)
         m_pIVoiceDictation->QueryInterface (IID_ILexPronounce2W, (PVOID*)&m_pILexPronounce2);

      return hRes;
   }
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Register (PCWSTR pszApplication, PIVDCTNOTIFYSINKW pNotifyInterface,
   PCWSTR pszTopic,
   PCWSTR pszSite,
   DWORD dwFlags,
   IStorage* pISessionDat)
{
   return Register (pszApplication, pszTopic, pISessionDat, pszSite,
      pNotifyInterface, IID_IVDctNotifySinkW, dwFlags);
}

HRESULT  CVoiceDictation::SessionDeserialize (IStorage* pISessionData)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->SessionDeserialize (pISessionData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::SessionSerialize (IStorage* pISessionData)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->SessionSerialize (pISessionData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::SiteInfoGet (PCWSTR pszSite, PVDSITEINFOW pSiteInfo)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->SiteInfoGet (pszSite, pSiteInfo);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::SiteInfoSet (PCWSTR pszSite, PVDSITEINFOW pSiteInfo)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->SiteInfoSet (pszSite, pSiteInfo);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TopicAddGrammar (PCWSTR pszTopic, SDATA sData)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->TopicAddGrammar (pszTopic, sData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TopicAddString (PCWSTR pszTopic, LANGUAGEW *pLanguage)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->TopicAddString (pszTopic, pLanguage);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TopicDeserialize (IStorage* pITopicData)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->TopicDeserialize (pITopicData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TopicEnum (PSDATA pData)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->TopicEnum (pData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TopicRemove (PCWSTR pszTopic)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->TopicRemove (pszTopic);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::TopicSerialize (IStorage* pITopicData)
{
   if (m_pIVoiceDictation)
      return m_pIVoiceDictation->TopicSerialize (pITopicData);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// ISRSpeaker
HRESULT  CVoiceDictation::Delete (WCHAR *pszSpeakerName)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Delete (pszSpeakerName);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Enum (PWSTR *ppszBuffer, DWORD *pdwBufSize)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Enum (ppszBuffer, pdwBufSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Merge (WCHAR *pszSpeakerName, PVOID pSpeakerData, DWORD dwSpeakerData)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Merge (pszSpeakerName, pSpeakerData, dwSpeakerData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::New (WCHAR *pszSpeakerName)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->New (pszSpeakerName);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Query (WCHAR *pszSpeakerName, DWORD dwSize, DWORD *pdwNeeded)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Query (pszSpeakerName, dwSize, pdwNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Read (WCHAR *pszSpeakerName, PVOID *ppBuffer, DWORD *pdwBufSize)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Read (pszSpeakerName, ppBuffer, pdwBufSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Revert (WCHAR *pszSpeakerName)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Revert (pszSpeakerName);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Select(WCHAR *pszSpeakerName, BOOL fLock)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Select (pszSpeakerName, fLock);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Write (WCHAR *pszSpeakerName, PVOID pSpeakerData, DWORD dwSpeakerData)
{
   if (m_pISRSpeaker)
      return m_pISRSpeaker->Write (pszSpeakerName, pSpeakerData, dwSpeakerData);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// ILexPronounce
HRESULT  CVoiceDictation::Add(VOICECHARSET CharSet, WCHAR *pszText, WCHAR *pszPronounce, 
   VOICEPARTOFSPEECH PartOfSpeech, PVOID pEngineInfo, 
   DWORD dwEngineInfoSize)
{
   if (m_pILexPronounce)
      return m_pILexPronounce->Add (CharSet, pszText, pszPronounce,
         PartOfSpeech, pEngineInfo, dwEngineInfoSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::Get(VOICECHARSET CharSet, WCHAR *pszText, WORD wSense, 
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

HRESULT  CVoiceDictation::Remove(WCHAR *pszText, WORD wSense)
{
   if (m_pILexPronounce)
      return m_pILexPronounce->Remove (pszText, wSense);
   else
      return ResultFromScode (E_NOINTERFACE);
}



// ILexPronounce2
HRESULT  CVoiceDictation::AddTo(DWORD dwLex, VOICECHARSET CharSet, WCHAR *pszText, WCHAR *pszPronounce, 
   VOICEPARTOFSPEECH PartOfSpeech, PVOID pEngineInfo, 
   DWORD dwEngineInfoSize)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->AddTo (dwLex, CharSet, pszText, pszPronounce,
         PartOfSpeech, pEngineInfo, dwEngineInfoSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::GetFrom(DWORD dwLex, VOICECHARSET CharSet, WCHAR *pszText, WORD wSense, 
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

HRESULT  CVoiceDictation::RemoveFrom(DWORD dwLex, WCHAR *pszText, WORD wSense)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->RemoveFrom (dwLex, pszText, wSense);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::QueryLexicons (BOOL f, DWORD *pdw)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->QueryLexicons (f, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::ChangeSpelling (DWORD dwLex, PCWSTR psz1, PCWSTR psz2)
{
   if (m_pILexPronounce2)
      return m_pILexPronounce2->ChangeSpelling (dwLex, psz1, psz2);
   else
      return ResultFromScode (E_NOINTERFACE);
}






// vdctcommandsbuiltin
#ifdef STRICT
HRESULT  CVoiceDictation::BuiltInActivate (void *hWnd, DWORD dwFlags)
#else
HRESULT  CVoiceDictation::BuiltInActivate (HWND hWnd, DWORD dwFlags)
#endif
{
   if (m_pIVDctCommandsBuiltIn)
      return m_pIVDctCommandsBuiltIn->Activate ((HWND) hWnd, dwFlags);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BuiltInDeactivate (void)
{
   if (m_pIVDctCommandsBuiltIn)
      return m_pIVDctCommandsBuiltIn->Deactivate ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BuiltInFromMemory (PVOID pMem, DWORD dwSize)
{
   if (m_pIVDctCommandsBuiltIn)
      return m_pIVDctCommandsBuiltIn->FromMemory (pMem, dwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BuiltInFromFile (PCWSTR pszFile)
{
   if (m_pIVDctCommandsBuiltIn)
      return m_pIVDctCommandsBuiltIn->FromFile (pszFile);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BuiltInFromStream (IStream *pIStream)
{
   if (m_pIVDctCommandsBuiltIn)
      return m_pIVDctCommandsBuiltIn->FromStream (pIStream);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BuiltInListSet (PCWSTR pszList, PVOID pMem, DWORD dwSize)
{
   if (m_pIVDctCommandsBuiltIn)
      return m_pIVDctCommandsBuiltIn->ListSet (pszList, pMem, dwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BuiltInTextGet (LPWSTR* pszText, DWORD* pdwSize)
{
   if (m_pIVDctCommandsBuiltIn)
      return m_pIVDctCommandsBuiltIn->TextGet (pszText, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BuiltInToMemory (PVOID* ppMem, DWORD* pdwSize)
{
   if (m_pIVDctCommandsBuiltIn)
      return m_pIVDctCommandsBuiltIn->ToMemory (ppMem, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BuiltInToFile (PCWSTR pszFile)
{
   if (m_pIVDctCommandsBuiltIn)
      return m_pIVDctCommandsBuiltIn->ToFile (pszFile);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::BuiltInToStream (IStream *pIStream)
{
   if (m_pIVDctCommandsBuiltIn)
      return m_pIVDctCommandsBuiltIn->ToStream (pIStream);
   else
      return ResultFromScode (E_NOINTERFACE);
}



// vdctcommandsApp
#ifdef STRICT
HRESULT  CVoiceDictation::AppActivate (void *hWnd, DWORD dwFlags)
#else
HRESULT  CVoiceDictation::AppActivate (HWND hWnd, DWORD dwFlags)
#endif
{
   if (m_pIVDctCommandsApp)
      return m_pIVDctCommandsApp->Activate ((HWND) hWnd, dwFlags);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::AppDeactivate (void)
{
   if (m_pIVDctCommandsApp)
      return m_pIVDctCommandsApp->Deactivate ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::AppFromMemory (PVOID pMem, DWORD dwSize)
{
   if (m_pIVDctCommandsApp)
      return m_pIVDctCommandsApp->FromMemory (pMem, dwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::AppFromFile (PCWSTR pszFile)
{
   if (m_pIVDctCommandsApp)
      return m_pIVDctCommandsApp->FromFile (pszFile);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::AppFromStream (IStream *pIStream)
{
   if (m_pIVDctCommandsApp)
      return m_pIVDctCommandsApp->FromStream (pIStream);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::AppListSet (PCWSTR pszList, PVOID pMem, DWORD dwSize)
{
   if (m_pIVDctCommandsApp)
      return m_pIVDctCommandsApp->ListSet (pszList, pMem, dwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::AppTextGet (LPWSTR* pszText, DWORD* pdwSize)
{
   if (m_pIVDctCommandsApp)
      return m_pIVDctCommandsApp->TextGet (pszText, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::AppToMemory (PVOID* ppMem, DWORD* pdwSize)
{
   if (m_pIVDctCommandsApp)
      return m_pIVDctCommandsApp->ToMemory (ppMem, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::AppToFile (PCWSTR pszFile)
{
   if (m_pIVDctCommandsApp)
      return m_pIVDctCommandsApp->ToFile (pszFile);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::AppToStream (IStream *pIStream)
{
   if (m_pIVDctCommandsApp)
      return m_pIVDctCommandsApp->ToStream (pIStream);
   else
      return ResultFromScode (E_NOINTERFACE);
}


/* IAttributes */
HRESULT  CVoiceDictation::DWORDGet (DWORD dwAttribute, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDGet (dwAttribute, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::DWORDSet (DWORD dwAttribute, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDSet (dwAttribute, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::StringGet (DWORD dwAttribute, PWSTR psz, DWORD dw, DWORD *pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringGet (dwAttribute,psz, dw, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::StringSet (DWORD dwAttribute, PCWSTR psz)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringSet (dwAttribute, psz);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::MemoryGet (DWORD dwAttribute, PVOID * ppMem, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemoryGet (dwAttribute, ppMem, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::MemorySet (DWORD dwAttribute, PVOID pMem, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemorySet (dwAttribute, pMem, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// IVDctTextCache
HRESULT  CVoiceDictation::CopyToBin (DWORD dwStart, DWORD dwNumChars, DWORD dwBin)
{
   if (m_pIVDctTextCache)
      return m_pIVDctTextCache->CopyToBin (dwStart, dwNumChars, dwBin);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::CopyToMemory (DWORD dwStart, DWORD dwNumChars,
                                        PVOID* ppMem, DWORD* pdwMem,
                                        LPUNKNOWN** ppUnk, DWORD* pdwUnk)
{
   if (m_pIVDctTextCache)
      return m_pIVDctTextCache->CopyToMemory (dwStart, dwNumChars, ppMem, pdwMem, ppUnk, pdwUnk);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::PasteFromBin (DWORD dwStart, DWORD dwNumChars, DWORD dwBin)
{
   if (m_pIVDctTextCache)
      return m_pIVDctTextCache->PasteFromBin (dwStart, dwNumChars, dwBin);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceDictation::PasteFromMemory (DWORD dwStart, DWORD dwNumChars,
                                           PVOID pMem, DWORD dwMem,
                                           LPUNKNOWN* pUnk, DWORD dwUnk)
{
   if (m_pIVDctTextCache)
      return m_pIVDctTextCache->PasteFromMemory (dwStart, dwNumChars, pMem, dwMem, pUnk, dwUnk);
   else
      return ResultFromScode (E_NOINTERFACE);
}





/***********************************************************************
CVDctNotifySink */

STDMETHODIMP CVDctNotifySink::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IVDctNotifySinkW) ||
      IsEqualIID(riid,IID_IVDctNotifySink2W))
	{
	*ppv = (LPVOID) this;
	AddRef();
	return ResultFromScode (NOERROR);
	}   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CVDctNotifySink::AddRef (void)
{
  return(++m_RefCount);
}

STDMETHODIMP_(ULONG) CVDctNotifySink::Release (void)
{
   if((--m_RefCount) == 0)
   {
	delete this;
	return(0);
   }
   return(m_RefCount);
};


STDMETHODIMP CVDctNotifySink::CommandBuiltIn (PWSTR psz)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::CommandOther (PWSTR psz)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::CommandRecognize (DWORD dw1, DWORD dw2,
                                                DWORD dw3, PVOID dw4, PWSTR psz)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::TextSelChanged (void)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::TextChanged (DWORD dwReason)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::TextBookmarkChanged (DWORD dwReason)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::PhraseStart (void)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::PhraseFinish (DWORD dw, PSRPHRASEW pSRPhrase)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::PhraseHypothesis (DWORD dw, PSRPHRASEW pSRPhrase)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::UtteranceBegin (void)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::UtteranceEnd (void)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::VUMeter (WORD wVU)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::AttribChanged (DWORD dwAttrib)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::Interference (DWORD dwAttrib)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::Training (DWORD dw)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::Dictating (PCWSTR psz, BOOL f)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::Error (LPUNKNOWN pUnk)
{
   return NOERROR;
}

STDMETHODIMP CVDctNotifySink::Warning (LPUNKNOWN pUnk)
{
   return NOERROR;
}





