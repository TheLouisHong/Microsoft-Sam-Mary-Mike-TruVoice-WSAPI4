/************************************************************************
FakeSREn.Cpp - Fake SR engine enumeration & find object.
   This includes the interfaces
      ISREnum
      EngISRFind.
      EngIIdentity
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
//#define INITGUID
// #include <compobj.h>
#include <speech.h>
#include "fakeSR.h"
#include "fakeSREn.h"


/************************************************************************
Defines
*/
#define  NUMSRMODES        (2)      // 2 speech recognition modes for this demo
#define  SRMODE_ENGLISH    (0)      // 1st speech recognition mode
#define  SRMODE_FRENCH     (1)      // 2nd speech recognition mode

/************************************************************************
CEngineEnum - Manager class.
*/

CEngineEnum::CEngineEnum (LPUNKNOWN punkOuter, LPFNDESTROYED pfnDestroy)
{
m_cRef=0;
m_punkOuter = punkOuter;
m_pfnDestroy = pfnDestroy;
m_pEngISREnumW = NULL;

// Start the fake SR enumeration at the beginning
dwCurrentEnumPosn = 0;
}


CEngineEnum::~CEngineEnum (void)
{
// Free the contained interfaces
if (NULL != m_pEngISREnumW)
   delete m_pEngISREnumW;
}

BOOL CEngineEnum::FInit (void)
{
LPUNKNOWN      pIUnknown = (LPUNKNOWN) this;

if (NULL != m_punkOuter)
   pIUnknown = m_punkOuter;

// Allocate all of the contained interfaces
m_pEngISREnumW = new CEngISREnumW (this, pIUnknown);

return (m_pEngISREnumW ? TRUE : FALSE);
}

STDMETHODIMP CEngineEnum::QueryInterface (REFIID riid, LPVOID *ppv)
{
*ppv = NULL;

/* always return our IUnkown for IID_IUnknown */
if (IsEqualIID (riid, IID_IUnknown))
   *ppv = (LPVOID) this;

// other interfaces
if (IsEqualIID(riid, IID_ISREnumW))
   *ppv = m_pEngISREnumW;

// update the reference count
if (NULL != *ppv) {
   ((LPUNKNOWN) *ppv) ->AddRef();
   return NOERROR;
   };

return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CEngineEnum::AddRef (void)
{
return ++m_cRef;
}

STDMETHODIMP_(ULONG) CEngineEnum::Release (void)
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



/************************************************************************
CEngISREnumW - ISR Enumeration inteerface
*/


CEngISREnumA::CEngISREnumA (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_pObjA = (PCMode)pObj;
   (((PCEngineEnum) m_pObjA)->m_pEngISREnumW)->FInit(pObj, punkOuter);
}


CEngISREnumW::CEngISREnumW (LPVOID pObj, LPUNKNOWN punkOuter)
{
m_cRef = 0;
m_pObj = pObj;
m_punkOuter = punkOuter;
}

// This function is called from the ANSI interface constructor -

CEngISREnumW::FInit(LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef      = 0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;

   return NOERROR;
}


CEngISREnumA::~CEngISREnumA (void)
{
   (((PCEngineEnum) m_pObjA)->m_pEngISREnumW)->~CEngISREnumW();
}


CEngISREnumW::~CEngISREnumW (void)
{
// intentionally left blank
}


STDMETHODIMP CEngISREnumA::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return (((PCEngineEnum) m_pObjA)->m_pEngISREnumW)->QueryInterface(riid, ppv);
}


STDMETHODIMP CEngISREnumW::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
return m_punkOuter->QueryInterface(riid,ppv);
}


STDMETHODIMP_ (ULONG) CEngISREnumA::AddRef(void)
{
   return (((PCEngineEnum) m_pObjA)->m_pEngISREnumW)->AddRef();
}


STDMETHODIMP_ (ULONG) CEngISREnumW::AddRef(void)
{
++m_cRef;
return m_punkOuter->AddRef();
}


STDMETHODIMP_ (ULONG) CEngISREnumA::Release(void)
{
   return (((PCEngineEnum) m_pObjA)->m_pEngISREnumW)->Release();
}


STDMETHODIMP_(ULONG) CEngISREnumW::Release(void)
{
--m_cRef;
return m_punkOuter->Release();
}


STDMETHODIMP CEngISREnumA::Next (ULONG cel, PSRMODEINFOA pttsInfo, ULONG* pdwFetched)
{
   HRESULT        hRes;
   PSRMODEINFOA  pSRModeInfoA = pttsInfo;
   PSRMODEINFOW  pSRMODEINFOW;
   PSRMODEINFOW  pSRModeInfoW;
   ULONG          i;

   pSRMODEINFOW = 
   pSRModeInfoW = (PSRMODEINFOW) malloc(cel * sizeof(SRMODEINFOW));

   if (!pSRMODEINFOW) return ResultFromScode(E_OUTOFMEMORY);

   hRes = (((PCEngineEnum) m_pObjA)->m_pEngISREnumW)->Next(cel, pSRMODEINFOW, pdwFetched);

   if (hRes == NOERROR)

      for (i = 1; i <= *pdwFetched; i++, pSRModeInfoA++, pSRModeInfoW++)
         {
         pSRModeInfoA->gEngineID = pSRModeInfoW->gEngineID;

         WideCharToMultiByte(CP_ACP, 0, pSRModeInfoW->szMfgName, -1,
                             pSRModeInfoA->szMfgName, TTSI_NAMELEN, NULL, NULL);

         WideCharToMultiByte(CP_ACP, 0, pSRModeInfoW->szProductName, -1,
                             pSRModeInfoA->szProductName, TTSI_NAMELEN, NULL, NULL);

         pSRModeInfoA->gModeID = pSRModeInfoW->gModeID;

         WideCharToMultiByte(CP_ACP, 0, pSRModeInfoW->szModeName, -1,
                             pSRModeInfoA->szModeName, TTSI_NAMELEN, NULL, NULL);

         pSRModeInfoA->language.LanguageID = pSRModeInfoW->language.LanguageID;

         WideCharToMultiByte(CP_ACP, 0, pSRModeInfoW->language.szDialect, -1,
                             pSRModeInfoA->language.szDialect, LANG_LEN, NULL, NULL);

         pSRModeInfoA->dwSequencing     = pSRModeInfoW->dwSequencing;
         pSRModeInfoA->dwMaxWordsVocab  = pSRModeInfoW->dwMaxWordsVocab;
         pSRModeInfoA->dwMaxWordsState  = pSRModeInfoW->dwMaxWordsState;
         pSRModeInfoA->dwGrammars       = pSRModeInfoW->dwGrammars;
         pSRModeInfoA->dwFeatures       = pSRModeInfoW->dwFeatures;
         pSRModeInfoA->dwInterfaces     = pSRModeInfoW->dwInterfaces;
         pSRModeInfoA->dwEngineFeatures = pSRModeInfoW->dwEngineFeatures;
         }

   free(pSRMODEINFOW);

   return hRes;         // hRes came from the call to CISREnumW::Next
}


STDMETHODIMP CEngISREnumW::Next (ULONG cel, PSRMODEINFOW psrInfo, ULONG* pdwFetched)
{
DWORD dwEnum, i, dwCurEnum;
int      iRet;

// some error checking

   if (!psrInfo) return ResultFromScode (E_INVALIDARG);
   if ((cel != 1) && !pdwFetched) return ResultFromScode (E_INVALIDARG);

// if our current position is already at the end then return an error

   if (((PCEngineEnum) m_pObj)->dwCurrentEnumPosn >= NUMSRMODES)
      {
      if (pdwFetched) *pdwFetched = 0;
      memset (psrInfo, 0, cel * sizeof(SRMODEINFOW));
      return  ResultFromScode (S_FALSE);
      }

// how many can we really enumerate?

dwEnum = NUMSRMODES - ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn;
if (dwEnum > cel)
   dwEnum = cel;
if (pdwFetched)
   *pdwFetched = dwEnum;

// do the copying
for (i = 0; i < dwEnum; i++, psrInfo++) {
   // blank out the structure
   memset (psrInfo, 0, sizeof(*psrInfo));

   // fill in generic information which is no 0
   psrInfo->dwSequencing = SRSEQUENCE_CONTINUOUS;
   psrInfo->dwMaxWordsVocab = 2000;
   psrInfo->dwMaxWordsState = 10;
   psrInfo->dwGrammars = SRGRAM_CFG;
   psrInfo->dwFeatures = SRFEATURE_TRAINWORD | SRFEATURE_TRAINPHONETIC |
      SRFEATURE_ANYWORD;


   psrInfo->gEngineID = CLSID_FakeSREngineEnum;   // SB ..Female
   iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "Acme Corporation", -1L,
      psrInfo->szMfgName, sizeof(psrInfo->szMfgName) / sizeof(WCHAR));
   iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "Acme Listener", -1L,
      psrInfo->szProductName, sizeof(psrInfo->szProductName) / sizeof(WCHAR));

   // special case
   dwCurEnum = ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn + i;
   switch (dwCurEnum) {
      case SRMODE_ENGLISH:
         psrInfo->gModeID = CLSID_FakeSREngineEnumEnglishMode;
         iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "ACME English Recognition", -1L,
            psrInfo->szModeName, sizeof(psrInfo->szModeName) / sizeof(WCHAR));
         psrInfo->language.LanguageID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
         iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "United States", -1L,
            psrInfo->language.szDialect, sizeof(psrInfo->language.szDialect) / sizeof(WCHAR));
         break;
      case SRMODE_FRENCH:
         psrInfo->gModeID = CLSID_FakeSREngineEnumFrenchMode;
         iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "ACME French Recognition", -1L,
            psrInfo->szModeName, sizeof(psrInfo->szModeName) / sizeof(WCHAR));
         psrInfo->language.LanguageID = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH);
         iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "Paris", -1L,
            psrInfo->language.szDialect, sizeof(psrInfo->language.szDialect) / sizeof(WCHAR));
         break;
      };
   };

   ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn += dwEnum;


   return (cel == dwEnum) ? NOERROR : ResultFromScode(S_FALSE);
}


STDMETHODIMP CEngISREnumA::Skip (ULONG cel)
{
   return (((PCEngineEnum) m_pObjA)->m_pEngISREnumW)->Skip(cel);
}


STDMETHODIMP CEngISREnumW::Skip (ULONG cel)
{
// If we skip beyond the edge of the modes then return an error
// and set the position to the end

   if ((cel + ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn) > NUMSRMODES)
      {
      ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn = NUMSRMODES;
      return ResultFromScode (S_FALSE);
      }

// else, we're not skipping too much

   ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn += cel;
   return NOERROR;
}


STDMETHODIMP CEngISREnumA::Reset (void)
{
   return (((PCEngineEnum) m_pObjA)->m_pEngISREnumW)->Reset();
}


STDMETHODIMP CEngISREnumW::Reset (void)
{
// Set the current position back to the beginning
((PCEngineEnum) m_pObj)->dwCurrentEnumPosn = 0;

return NOERROR;
}


STDMETHODIMP CEngISREnumA::Clone (ISREnumA * FAR * ppISREnum)
{
   PISRENUMW  pISREnumW;
   HRESULT     hRes;

   if (!ppISREnum) return ResultFromScode(E_INVALIDARG);

   hRes = (((PCEngineEnum) m_pObjA)->m_pEngISREnumW)->Clone(&pISREnumW);

   if (hRes == NOERROR)
      {
      hRes = pISREnumW->QueryInterface(IID_ISREnumA, (void**) ppISREnum);
      pISREnumW->Release();
      }

   return hRes;
}


STDMETHODIMP CEngISREnumW::Clone (ISREnumW * FAR * ppISREnum)
{
PCEngineEnum   pObj;
HRESULT        hr;

if (!ppISREnum) return ResultFromScode(E_INVALIDARG);

// Create a new enumeration object
*ppISREnum = NULL;
hr = ResultFromScode (E_OUTOFMEMORY);
pObj = new CEngineEnum (
   ((PCEngineEnum)m_pObj)->m_punkOuter,
   ((PCEngineEnum)m_pObj)->m_pfnDestroy);

if (NULL == pObj)
   return hr;
if (pObj->FInit())
   hr = pObj->QueryInterface(IID_ISREnumW, (void**)ppISREnum);

// Kill the object if initial creation or FInit failed
if (FAILED(hr)) {
   delete pObj;
   return hr;
   }
else
   gEngObjectCount++;

// set its current position to the current enumerator's position
pObj->dwCurrentEnumPosn = ((PCEngineEnum)m_pObj)->dwCurrentEnumPosn;

// done
return NOERROR;
}


STDMETHODIMP CEngISREnumA::Select(GUID gModeID, PISRCENTRALA *ppISRCentral,
                                   LPUNKNOWN pAudio)
{
   PISRCENTRALW  pISRCentralW;
   HRESULT        hRes;

   hRes = (((PCEngineEnum) m_pObjA)->m_pEngISREnumW)->Select(gModeID,
                                                         &pISRCentralW, pAudio);
   if (hRes == NOERROR)
      {
      hRes = pISRCentralW->QueryInterface(IID_ISRCentralA, (void**) ppISRCentral);
      pISRCentralW->Release();
      }

   return hRes;
}


STDMETHODIMP CEngISREnumW::Select(GUID gModeID, PISRCENTRALW *ppISRCentral,
                                  LPUNKNOWN lpAudio)
{
   PCMode         pMode;
   HRESULT        hr;
   PSRMODEINFOW   pSRInfo;

   if (!ppISRCentral || !lpAudio)

      return ResultFromScode (E_INVALIDARG);

// Make sure that this is one of the modes we support

   if (!IsEqualGUID(gModeID, CLSID_FakeSREngineEnumEnglishMode)
    && !IsEqualGUID(gModeID, CLSID_FakeSREngineEnumFrenchMode ))

      return ResultFromScode (SRERR_INVALIDMODE);

   *ppISRCentral = NULL;

// Try to create a new enumeration object

   pMode = new CMode(NULL, ((PCEngineEnum)m_pObj)->m_pfnDestroy, lpAudio);

   hr    = ResultFromScode(E_OUTOFMEMORY);

   if (!pMode) return hr;

   if (pMode->FInit())

      hr = pMode->QueryInterface(IID_ISRCentralW, (VOID**)ppISRCentral);

// kill the object if the finit failed

   if (FAILED(hr)) delete pMode;

   else
      {
      pSRInfo = &pMode->m_SRModeInfo;

      memset (pSRInfo, 0, sizeof(SRMODEINFOW));

// fill in generic information which is no 0

      pSRInfo->dwSequencing    = SRSEQUENCE_CONTINUOUS;
      pSRInfo->dwMaxWordsVocab = 2000;
      pSRInfo->dwMaxWordsState = 10;
      pSRInfo->dwGrammars      = SRGRAM_CFG;
      pSRInfo->dwFeatures      = SRFEATURE_TRAINWORD | SRFEATURE_TRAINPHONETIC
                               | SRFEATURE_ANYWORD;

      pSRInfo->gEngineID = CLSID_FakeSREngineEnum;

      MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "Acme Corporation", -1L,
                          pSRInfo->szMfgName,
                          sizeof(pSRInfo->szMfgName) / sizeof(WCHAR));

      MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "Acme Listener", -1L,
                          pSRInfo->szProductName,
                          sizeof(pSRInfo->szProductName) / sizeof(WCHAR));

      pSRInfo->gModeID = gModeID;

      if (IsEqualGUID(gModeID, CLSID_FakeSREngineEnumEnglishMode))
         {
         MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "ACME English Recognition",
                             -1L, pSRInfo->szModeName,
                             sizeof(pSRInfo->szModeName) / sizeof(WCHAR));

         pSRInfo->language.LanguageID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);

         MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "United States", -1L,
                             pSRInfo->language.szDialect,
                             sizeof(pSRInfo->language.szDialect) / sizeof(WCHAR));
         }

      else
         {
         MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
                             "ACME French Recognition", -1L,
                             pSRInfo->szModeName,
                             sizeof(pSRInfo->szModeName) / sizeof(WCHAR));

         pSRInfo->language.LanguageID = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH);

         MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "Paris", -1L,
                           pSRInfo->language.szDialect,
                           sizeof(pSRInfo->language.szDialect) / sizeof(WCHAR));
         }

      gEngObjectCount++;
      }

   return hr;
}
