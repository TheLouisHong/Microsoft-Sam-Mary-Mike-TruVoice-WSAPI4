/*********************************************************************
LowSR.Cpp - Voice Commands C++ wrapper objects.

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
VoiceCommands */

CVoiceCommands::CVoiceCommands (void)
{
   m_pIVoiceCmd = NULL;
   m_pIVCmdAttributes = NULL;
   m_pIAttributes = NULL;
   m_pIVCmdDialogs = NULL;
}

CVoiceCommands::~CVoiceCommands (void)
{
   if (m_pIVoiceCmd)
      m_pIVoiceCmd->Release();
   if (m_pIVCmdAttributes)
      m_pIVCmdAttributes->Release();
   if (m_pIAttributes)
      m_pIAttributes->Release();
   if (m_pIVCmdDialogs)
      m_pIVCmdDialogs->Release();
}


// initalization
HRESULT  CVoiceCommands::Init (void)
{
   HRESULT  hRes;

   if (m_pIVoiceCmd)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_VCmd, NULL, CLSCTX_LOCAL_SERVER,
      IID_IVoiceCmdW, (void**)&m_pIVoiceCmd);
   if (hRes)
      return hRes;

   m_pIVoiceCmd->QueryInterface (IID_IVCmdAttributesW, (PVOID*)&m_pIVCmdAttributes);
   m_pIVoiceCmd->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   m_pIVoiceCmd->QueryInterface (IID_IVCmdDialogsW, (PVOID*)&m_pIVCmdDialogs);

   return NOERROR;
}

HRESULT  CVoiceCommands::Init (LPUNKNOWN pIUnkVCmd)
{
   HRESULT  hRes;

   if (m_pIVoiceCmd)
      return ResultFromScode (E_FAIL);

   hRes = pIUnkVCmd->QueryInterface (IID_IVoiceCmdW, (PVOID*)&m_pIVoiceCmd);
   if (hRes)
      return hRes;
   m_pIVoiceCmd->QueryInterface (IID_IVCmdAttributesW, (PVOID*)&m_pIVCmdAttributes);
   m_pIVoiceCmd->QueryInterface (IID_IAttributesW, (PVOID*)&m_pIAttributes);
   m_pIVoiceCmd->QueryInterface (IID_IVCmdDialogsW, (PVOID*)&m_pIVCmdDialogs);

   return NOERROR;
}

// initialization and registration combined
HRESULT  CVoiceCommands::Init ( PCWSTR pszSite, PVOID pNotifyInterface,
   IID IIDNotifyInterface, DWORD dwFlags,
   PVCSITEINFOW pSiteInfo)
{
   HRESULT  hRes;

   hRes = Init ();
   if (hRes)
      return hRes;

   // register
   hRes = Register (pszSite, pNotifyInterface, IIDNotifyInterface, dwFlags,
      pSiteInfo);
   return hRes;
}

HRESULT  CVoiceCommands::Init (PIVCMDNOTIFYSINKW pNotifyInterface, PCWSTR pszSite, 
   DWORD dwFlags, PVCSITEINFOW pSiteInfo)
{
   HRESULT  hRes;

   hRes = Init ();
   if (hRes)
      return hRes;

   // register
   hRes = Register (pNotifyInterface, pszSite, dwFlags, pSiteInfo);
   return hRes;
}

// IVoiceCommands
HRESULT  CVoiceCommands::CmdMimic (PVCMDNAMEW pMenu, PCWSTR pszCommand)
{
   if (m_pIVoiceCmd)
      return m_pIVoiceCmd->CmdMimic (pMenu, pszCommand);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::CmdMimic (PCWSTR pszApplication, PCWSTR pszState, PCWSTR pszCommand)
{
   VCMDNAMEW name;
   wcscpy (name.szApplication, pszApplication);
   wcscpy (name.szState, pszState);

   return CmdMimic (&name, pszCommand);
}

HRESULT  CVoiceCommands::MenuCreate (PVCMDNAMEW pName, PLANGUAGEW pLanguage, DWORD dwFlags,
      PIVCMDMENUW *ppIVCmdMenu)
{
   if (m_pIVoiceCmd)
      return m_pIVoiceCmd->MenuCreate (pName, pLanguage, dwFlags, ppIVCmdMenu);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::MenuCreate (PCWSTR pszApplication, PCWSTR pszState,
      CVoiceMenu **ppCVoiceMenu,
      DWORD dwFlags)
{
   VCMDNAMEW name;
   wcscpy (name.szApplication, pszApplication);
   wcscpy (name.szState, pszState);

   PIVCMDMENUW pIVCmdMenu;
   HRESULT     hRes;
   *ppCVoiceMenu = NULL;
   hRes = MenuCreate (&name, NULL, dwFlags, &pIVCmdMenu);
   if (hRes)
      return hRes;

   *ppCVoiceMenu = new CVoiceMenu;
   if (!(*ppCVoiceMenu)) {
      pIVCmdMenu->Release();
      *ppCVoiceMenu = NULL;
      return ResultFromScode (E_OUTOFMEMORY);
   }

   hRes = (*ppCVoiceMenu)->Init (pIVCmdMenu);
   pIVCmdMenu->Release();

   if (hRes) {
      *ppCVoiceMenu = NULL;
      return hRes;
   }

   return NOERROR;
}

CVoiceMenu* CVoiceCommands::MenuCreate (PCWSTR pszApplication, PCWSTR pszState,
      DWORD dwFlags)
{
   CVoiceMenu   *pMenu;
   HRESULT     hRes;

   hRes = MenuCreate (pszApplication, pszState, &pMenu, dwFlags);
   if (hRes)
      return NULL;

   return pMenu;
}

HRESULT  CVoiceCommands::MenuDelete (PVCMDNAMEW pName)
{
   if (m_pIVoiceCmd)
      return m_pIVoiceCmd->MenuDelete (pName);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::MenuDelete (PCWSTR pszApplication, PCWSTR pszState)
{
   VCMDNAMEW name;
   wcscpy (name.szApplication, pszApplication);
   wcscpy (name.szState, pszState);

   return MenuDelete (&name);
}

HRESULT  CVoiceCommands::MenuEnum (DWORD dwFlags, PCWSTR pszApplicationFilter, 
      PCWSTR pszStateFilter, PIVCMDENUMW *ppiVCmdEnum)
{
   if (m_pIVoiceCmd)
      return m_pIVoiceCmd->MenuEnum (dwFlags, pszApplicationFilter, pszStateFilter, ppiVCmdEnum);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::Register (PCWSTR pszSite, PVOID pNotifyInterface,
   IID IIDNotifyInterface, DWORD dwFlags,
   PVCSITEINFOW pSiteInfo)
{
   if (m_pIVoiceCmd)
      return m_pIVoiceCmd->Register (pszSite, (LPUNKNOWN) pNotifyInterface,
         IIDNotifyInterface, dwFlags, pSiteInfo);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::Register (PIVCMDNOTIFYSINKW pNotifyInterface, PCWSTR pszSite, 
   DWORD dwFlags, PVCSITEINFOW pSiteInfo)
{
   return Register (pszSite, pNotifyInterface, IID_IVCmdNotifySinkW,
      dwFlags, pSiteInfo);
}


// IVCmdAttributes
HRESULT  CVoiceCommands::AutoGainEnableGet (DWORD *pdwAutoGain)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->AutoGainEnableGet (pdwAutoGain);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceCommands::AutoGainEnableGet (void)
{
   DWORD dw = 0;
   AutoGainEnableGet (&dw);
   return dw;
}

HRESULT  CVoiceCommands::AutoGainEnableSet (DWORD dwAutoGain)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->AutoGainEnableSet (dwAutoGain);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::AwakeStateGet (DWORD *pdwAwakeState)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->AwakeStateGet (pdwAwakeState);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceCommands::AwakeStateGet (void)
{
   DWORD dw = 0;
   AwakeStateGet (&dw);
   return dw;
}

HRESULT  CVoiceCommands::AwakeStateSet (DWORD pdwAwakeState)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->AwakeStateSet (pdwAwakeState);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::DeviceGet (DWORD *pdwDevice)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->DeviceGet (pdwDevice);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceCommands::DeviceGet (void)
{
   DWORD dw = 0;
   DeviceGet (&dw);
   return dw;
}

HRESULT  CVoiceCommands::DeviceSet (DWORD pdwDevice)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->DeviceSet (pdwDevice);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::EnabledGet (DWORD *pdwEnabled)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->EnabledGet (pdwEnabled);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceCommands::EnabledGet (void)
{
   DWORD dw = 0;
   EnabledGet (&dw);
   return dw;
}

HRESULT  CVoiceCommands::EnabledSet (DWORD pdwEnabled)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->EnabledSet (pdwEnabled);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::MicrophoneGet (WCHAR *pszMicrophone, DWORD dwMicrophoneSize, DWORD *pdwNeeded)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->MicrophoneGet (pszMicrophone,dwMicrophoneSize,pdwNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::MicrophoneSet (WCHAR *pszMicrophone)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->MicrophoneSet (pszMicrophone);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::SpeakerGet (WCHAR *pszSpeaker, DWORD dwSpeakerSize, DWORD *pdwNeeded)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->SpeakerGet (pszSpeaker,dwSpeakerSize,pdwNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::SpeakerSet (WCHAR *pszSpeaker)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->SpeakerSet (pszSpeaker);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::SRModeGet (GUID *pgMode)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->SRModeGet (pgMode);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::SRModeSet (GUID gMode)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->SRModeSet (gMode);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::ThresholdGet (DWORD *pdwThreshold)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->ThresholdGet (pdwThreshold);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceCommands::ThresholdGet (void)
{
   DWORD dw = 0;
   ThresholdGet (&dw);
   return dw;
}

HRESULT  CVoiceCommands::ThresholdSet (DWORD dwThreshold)
{
   if (m_pIVCmdAttributes)
      return m_pIVCmdAttributes->ThresholdSet (dwThreshold);
   else
      return ResultFromScode (E_NOINTERFACE);
}


// ISRDialogs
#ifdef STRICT
HRESULT  CVoiceCommands::AboutDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CVoiceCommands::AboutDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pIVCmdDialogs)
      return m_pIVCmdDialogs->AboutDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceCommands::GeneralDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CVoiceCommands::GeneralDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pIVCmdDialogs)
      return m_pIVCmdDialogs->GeneralDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceCommands::LexiconDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CVoiceCommands::LexiconDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pIVCmdDialogs)
      return m_pIVCmdDialogs->LexiconDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceCommands::TrainGeneralDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CVoiceCommands::TrainGeneralDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pIVCmdDialogs)
      return m_pIVCmdDialogs->TrainGeneralDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceCommands::TrainMicDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CVoiceCommands::TrainMicDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pIVCmdDialogs)
      return m_pIVCmdDialogs->TrainMicDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}


/* IAttributes */
HRESULT  CVoiceCommands::DWORDGet (DWORD dwAttribute, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDGet (dwAttribute, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::DWORDSet (DWORD dwAttribute, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->DWORDSet (dwAttribute, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::StringGet (DWORD dwAttribute, PWSTR psz, DWORD dw, DWORD *pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringGet (dwAttribute,psz, dw, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::StringSet (DWORD dwAttribute, PCWSTR psz)
{
   if (m_pIAttributes)
      return m_pIAttributes->StringSet (dwAttribute, psz);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::MemoryGet (DWORD dwAttribute, PVOID * ppMem, DWORD* pdw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemoryGet (dwAttribute, ppMem, pdw);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceCommands::MemorySet (DWORD dwAttribute, PVOID pMem, DWORD dw)
{
   if (m_pIAttributes)
      return m_pIAttributes->MemorySet (dwAttribute, pMem, dw);
   else
      return ResultFromScode (E_NOINTERFACE);
}





/**************************************************************************
VoiceMenu */

CVoiceMenu::CVoiceMenu (void)
{
   m_pIVCmdMenu = NULL;
}

CVoiceMenu::~CVoiceMenu (void)
{
   if (m_pIVCmdMenu)
      m_pIVCmdMenu->Release();
}

// initalization
HRESULT  CVoiceMenu::Init (LPUNKNOWN pIUnkVMenu)
{
   if (!pIUnkVMenu)
      return ResultFromScode (E_FAIL);

   return pIUnkVMenu->QueryInterface (IID_IVCmdMenuW, (PVOID*)&m_pIVCmdMenu);
}


// IVCmdMenu
#ifdef STRICT
HRESULT  CVoiceMenu::Activate (PVOID hWndListening, DWORD dwFlags)
#else
HRESULT  CVoiceMenu::Activate (HWND hWndListening, DWORD dwFlags)
#endif
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->Activate ((HWND) hWndListening, dwFlags);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceMenu::Add (DWORD dwCmdNum, SDATA dData, DWORD *pdwCmdStart)
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->Add (dwCmdNum, dData, pdwCmdStart);
   else
      return ResultFromScode (E_NOINTERFACE);
}

static PVCMDCOMMAND WrapUpCommand (DWORD dwID, PCWSTR pszCommand,
                            PCWSTR pszDescription, PCWSTR pszCategory,
                            DWORD dwFlags, PVOID pAction, DWORD dwActionSize)
{
   // calculate the size needed
   DWORD dwNeeded, dwNeededCommand, dwNeededDescription, dwNeededCategory;
   dwNeededCommand = pszCommand ? ((wcslen(pszCommand)+1)*2) : 0;
   dwNeededDescription = pszDescription ? ((wcslen(pszDescription)+1)*2) : 0;
   dwNeededCategory = pszCategory ? ((wcslen(pszCategory)+1)*2) : 0;
   dwNeeded = sizeof (VCMDCOMMAND) + dwNeededCommand + dwNeededDescription +
      dwNeededCategory + dwActionSize;
   dwNeeded = (dwNeeded + 3) & ~3;  // DWORD align

   // alocate the memory
   PVCMDCOMMAND   pVCmd;
   pVCmd = (PVCMDCOMMAND) malloc (dwNeeded);
   if (!pVCmd)
      return NULL;

   DWORD dwCur = sizeof(VCMDCOMMAND);
   memset (pVCmd, 0, sizeof(VCMDCOMMAND));
   pVCmd->dwSize = dwNeeded;
   pVCmd->dwFlags = dwFlags;
   pVCmd->dwID = dwID;
   if (pszCommand) {
      wcscpy ((PWSTR) ((PBYTE) pVCmd + dwCur), pszCommand);
      pVCmd->dwCommand = dwCur;
      dwCur += dwNeededCommand;
   }
   if (pszDescription) {
      wcscpy ((PWSTR) ((PBYTE) pVCmd + dwCur), pszDescription);
      pVCmd->dwDescription = dwCur;
      dwCur += dwNeededDescription;
   }
   if (pszCategory) {
      wcscpy ((PWSTR) ((PBYTE) pVCmd + dwCur), pszCategory);
      pVCmd->dwCategory = dwCur;
      dwCur += dwNeededCategory;
   }
   if (pAction) {
      memcpy ((PBYTE) pVCmd + dwCur, pAction, dwActionSize);
      pVCmd->dwAction = dwCur;
      pVCmd->dwActionSize = dwActionSize;
   }

   return pVCmd;
}


HRESULT  CVoiceMenu::AddOneCommand (DWORD dwID, PCWSTR pszCommand,
   PCWSTR pszDescription, PCWSTR pszCategory,
   DWORD dwFlags, PVOID pAction, DWORD dwActionSize)
{
   PVCMDCOMMAND   pCmd;

   pCmd = WrapUpCommand (dwID, pszCommand, pszDescription, pszCategory,
                            dwFlags, pAction, dwActionSize);
   if (!pCmd)
      return ResultFromScode (E_OUTOFMEMORY);

   HRESULT  hRes;
   SDATA    dData;
   dData.pData = pCmd;
   dData.dwSize = pCmd->dwSize;
   DWORD    dwTemp;
   hRes = Add (1, dData, &dwTemp);
   free (pCmd);

   return hRes;
}

HRESULT  CVoiceMenu::Deactivate (void)
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->Deactivate ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceMenu::EnableItem (DWORD dwEnable, DWORD dwCmdNum, DWORD dwFlag)
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->EnableItem (dwEnable, dwCmdNum, dwFlag);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceMenu::Get (DWORD dwCmdStart, DWORD dwCmdNum, DWORD dwFlag,
   PSDATA pdData, DWORD *pdwCmdNum)
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->Get (dwCmdStart, dwCmdNum, dwFlag, pdData, pdwCmdNum);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceMenu::ListGet (PCWSTR pszList, PSDATA pdList, DWORD *pdwListNum)
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->ListGet (pszList, pdList, pdwListNum);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceMenu::ListSet (PCWSTR pszList, DWORD dwListNum, SDATA dList)
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->ListSet (pszList, dwListNum, dList);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceMenu::Num (DWORD *pdwNumCmd)
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->Num (pdwNumCmd);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CVoiceMenu::Num (void)
{
   DWORD dw = 0;
   Num (&dw);
   return dw;
}

HRESULT  CVoiceMenu::Remove (DWORD dwCmdStart, DWORD dwCmdNum, DWORD dwFlag)
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->Remove (dwCmdStart, dwCmdNum, dwFlag);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceMenu::Set (DWORD dwCmdStart, DWORD dwCmdNum, DWORD dwFlag, 
   SDATA dData)
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->Set (dwCmdStart, dwCmdNum, dwFlag, dData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CVoiceMenu::SetOneCommand (DWORD dwCmdNum, DWORD dwFlag,
   DWORD dwID, PCWSTR pszCommand,
   PCWSTR pszDescription, PCWSTR pszCategory,
   DWORD dwFlags, PVOID pAction, DWORD dwActionSize)
{
   PVCMDCOMMAND   pCmd;

   pCmd = WrapUpCommand (dwID, pszCommand, pszDescription, pszCategory,
                            dwFlags, pAction, dwActionSize);
   if (!pCmd)
      return ResultFromScode (E_OUTOFMEMORY);

   HRESULT  hRes;
   SDATA    dData;
   dData.pData = pCmd;
   dData.dwSize = pCmd->dwSize;
   hRes = Set (dwCmdNum, 1, dwFlag, dData);
   free (pCmd);

   return hRes;
}

HRESULT  CVoiceMenu::SetItem (DWORD dwEnable, DWORD dwCmdNum, DWORD dwFlag)
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->SetItem (dwEnable, dwCmdNum, dwFlag);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CVoiceMenu::TrainMenuDlg (PVOID hWndParent, PCWSTR pszTitle)
#else
HRESULT  CVoiceMenu::TrainMenuDlg (HWND hWndParent, PCWSTR pszTitle)
#endif
{
   if (m_pIVCmdMenu)
      return m_pIVCmdMenu->TrainMenuDlg ((HWND) hWndParent, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}

   
/***********************************************************************
CVCmdNotifySink */

STDMETHODIMP CVCmdNotifySink::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IVCmdNotifySinkW))
	{
	*ppv = (LPVOID) this;
	AddRef();
	return ResultFromScode (NOERROR);
	}
   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CVCmdNotifySink::AddRef (void)
{
    return(++m_RefCount);
}

STDMETHODIMP_(ULONG) CVCmdNotifySink::Release (void)
{
   if((--m_RefCount) == 0)
   {
	delete this;
	return(0);
   }
   return(m_RefCount);
};


STDMETHODIMP CVCmdNotifySink::AttribChanged (DWORD dwID)
{
   return NOERROR;
}

STDMETHODIMP CVCmdNotifySink::CommandRecognize (DWORD dw1,
                                                PVCMDNAMEW pName,
                                                DWORD dw2,
                                                DWORD dw3,
                                                PVOID pMem,
                                                DWORD dw4,
                                                PWSTR psz1,
                                                PWSTR psz2)
{
   return NOERROR;
}

STDMETHODIMP CVCmdNotifySink::CommandOther (PVCMDNAMEW pName, PWSTR psz)
{
   return NOERROR;
}

STDMETHODIMP CVCmdNotifySink::CommandStart (void)
{
   return NOERROR;
}

STDMETHODIMP CVCmdNotifySink::MenuActivate (PVCMDNAMEW pName, BOOL b)
{
   return NOERROR;
}

STDMETHODIMP CVCmdNotifySink::UtteranceBegin (void)
{
   return NOERROR;
}

STDMETHODIMP CVCmdNotifySink::UtteranceEnd (void)
{
   return NOERROR;
}

STDMETHODIMP CVCmdNotifySink::VUMeter (WORD wLevel)
{
   return NOERROR;
}

STDMETHODIMP CVCmdNotifySink::Interference (DWORD dwInt)
{
   return NOERROR;
}

