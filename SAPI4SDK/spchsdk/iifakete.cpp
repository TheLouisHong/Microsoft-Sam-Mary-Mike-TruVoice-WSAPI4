/************************************************************************
FakeTTSE.Cpp - Fake TTS engine enumeration & find object.
   This includes the interfaces
      ITTSEnum
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
#include "fakeTTSE.h"

// NOTE: Must eventually have ANSI versions of all functions
//      in this object

/************************************************************************
Defines
*/

#define  NUMTTSMODES         2
#define  TTSMODE_MALE        0
#define  TTSMODE_FEMALE      1


/************************************************************************
CEngineEnum - Manager class.
*/

CEngineEnum::CEngineEnum (LPUNKNOWN punkOuter, LPFNDESTROYED pfnDestroy)
{
   m_cRef          = 0;
   m_punkOuter     = punkOuter;
   m_pfnDestroy    = pfnDestroy;
   m_pEngITTSEnumW = NULL;
   m_pEngITTSEnumA = NULL;

// Start the fake TTS enumeration at the beginning

   dwCurrentEnumPosn = 0;
}


CEngineEnum::~CEngineEnum (void)
{
// Free the contained interfaces

   if (NULL != m_pEngITTSEnumW) delete m_pEngITTSEnumW;
   if (NULL != m_pEngITTSEnumA) delete m_pEngITTSEnumA;
}


BOOL CEngineEnum::FInit (void)
{
   LPUNKNOWN   pIUnknown = (LPUNKNOWN) this;

   if (NULL != m_punkOuter) pIUnknown = m_punkOuter;

// Allocate all of the contained interfaces

   m_pEngITTSEnumW = new CEngITTSEnumW (this, pIUnknown);
   m_pEngITTSEnumA = new CEngITTSEnumA (this, pIUnknown);

   return m_pEngITTSEnumW && m_pEngITTSEnumA ? TRUE : FALSE;
}


STDMETHODIMP CEngineEnum::QueryInterface (REFIID riid, LPVOID *ppv)
{
   *ppv = NULL;

/* always return our IUnkown for IID_IUnknown */

   if (IsEqualIID (riid, IID_IUnknown)) *ppv = (LPVOID) this;

// other interfaces

   if (IsEqualIID(riid, IID_ITTSEnumW)) *ppv = m_pEngITTSEnumW;
   if (IsEqualIID(riid, IID_ITTSEnumA)) *ppv = m_pEngITTSEnumA;

// update the reference count

   if (NULL != *ppv) {

      ((LPUNKNOWN) *ppv)->AddRef();
      return NOERROR;
      }

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

   if (0 == m_cRef) {            // inform destroy that object is going away

      if (NULL != m_pfnDestroy) (*m_pfnDestroy)();

      delete this;
      }

   return cRefT;
}


/************************************************************************
CEngITTSEnumW - ITTS Enumeration inteerface
*/


CEngITTSEnumA::CEngITTSEnumA (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_pObjA = (PCMode)pObj;
   (((PCEngineEnum) m_pObjA)->m_pEngITTSEnumW)->FInit(pObj, punkOuter);
}


CEngITTSEnumW::CEngITTSEnumW (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef =     0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;
}


// This function is called from the ANSI interface constructor -

CEngITTSEnumW::FInit(LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef      = 0;
   m_pObj      = pObj;
   m_punkOuter = punkOuter;

   return NOERROR;
}


CEngITTSEnumA::~CEngITTSEnumA (void)
{
   (((PCEngineEnum) m_pObjA)->m_pEngITTSEnumW)->~CEngITTSEnumW();
}


CEngITTSEnumW::~CEngITTSEnumW (void)
{
// intentionally left blank
}


STDMETHODIMP CEngITTSEnumA::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return (((PCEngineEnum) m_pObjA)->m_pEngITTSEnumW)->QueryInterface(riid, ppv);
}


STDMETHODIMP CEngITTSEnumW::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return m_punkOuter->QueryInterface(riid,ppv);
}


STDMETHODIMP_ (ULONG) CEngITTSEnumA::AddRef(void)
{
   return (((PCEngineEnum) m_pObjA)->m_pEngITTSEnumW)->AddRef();
}


STDMETHODIMP_ (ULONG) CEngITTSEnumW::AddRef(void)
{
   ++m_cRef;
   return m_punkOuter->AddRef();
}


STDMETHODIMP_ (ULONG) CEngITTSEnumA::Release(void)
{
   return (((PCEngineEnum) m_pObjA)->m_pEngITTSEnumW)->Release();
}


STDMETHODIMP_(ULONG) CEngITTSEnumW::Release(void)
{
   --m_cRef;
   return m_punkOuter->Release();
}


/**************************************************************************************

   This bogus enumerator pretends to provide you with as many modes as you request - up
   to a purely arbitrary limit hardcoded elsewhere (#define NUMTTSMODES 2) & stuffs 'em
   with nothing real - just 0's & one bit of the Feature flag.

   All members of the array will currently be the same engine - OK?
   We can make more distinct (fake) engines if needed.

 */

STDMETHODIMP CEngITTSEnumA::Next (ULONG cel, PTTSMODEINFOA pttsInfo, ULONG* pdwFetched)
{
   HRESULT        hRes;
   PTTSMODEINFOA  pTTSModeInfoA = pttsInfo;
   PTTSMODEINFOW  pTTSMODEINFOW;
   PTTSMODEINFOW  pTTSModeInfoW;
   ULONG          i;

   pTTSMODEINFOW = 
   pTTSModeInfoW = (PTTSMODEINFOW) malloc(cel * sizeof(TTSMODEINFOW));

   if (!pTTSMODEINFOW) return ResultFromScode(E_OUTOFMEMORY);

   hRes = (((PCEngineEnum) m_pObjA)->m_pEngITTSEnumW)->Next(cel, pTTSMODEINFOW, pdwFetched);

   if (hRes == NOERROR)

      for (i = 1; i <= *pdwFetched; i++, pTTSModeInfoA++, pTTSModeInfoW++)
         {
         pTTSModeInfoA->gEngineID = pTTSModeInfoW->gEngineID;

         WideCharToMultiByte(CP_ACP, 0, pTTSModeInfoW->szMfgName, -1,
                             pTTSModeInfoA->szMfgName, TTSI_NAMELEN, NULL, NULL);

         WideCharToMultiByte(CP_ACP, 0, pTTSModeInfoW->szProductName, -1,
                             pTTSModeInfoA->szProductName, TTSI_NAMELEN, NULL, NULL);

         pTTSModeInfoA->gModeID = pTTSModeInfoW->gModeID;

         WideCharToMultiByte(CP_ACP, 0, pTTSModeInfoW->szModeName, -1,
                             pTTSModeInfoA->szModeName, TTSI_NAMELEN, NULL, NULL);

         pTTSModeInfoA->language.LanguageID = pTTSModeInfoW->language.LanguageID;

         WideCharToMultiByte(CP_ACP, 0, pTTSModeInfoW->language.szDialect, -1,
                             pTTSModeInfoA->language.szDialect, LANG_LEN, NULL, NULL);

         WideCharToMultiByte(CP_ACP, 0, pTTSModeInfoW->szSpeaker, -1,
                             pTTSModeInfoA->szSpeaker, TTSI_NAMELEN, NULL, NULL);

         WideCharToMultiByte(CP_ACP, 0, pTTSModeInfoW->szStyle, -1,
                             pTTSModeInfoA->szStyle, TTSI_STYLELEN, NULL, NULL);

         pTTSModeInfoA->wGender          = pTTSModeInfoW->wGender;
         pTTSModeInfoA->wAge             = pTTSModeInfoW->wAge;
         pTTSModeInfoA->dwFeatures       = pTTSModeInfoW->dwFeatures;
         pTTSModeInfoA->dwInterfaces     = pTTSModeInfoW->dwInterfaces;
         pTTSModeInfoA->dwEngineFeatures = pTTSModeInfoW->dwEngineFeatures;
         }

   free(pTTSMODEINFOW);

   return hRes;         // hRes came from the call to CITTSEnumW::Next
}


STDMETHODIMP CEngITTSEnumW::Next (ULONG cel, PTTSMODEINFOW pttsInfo, ULONG* pdwFetched)
{
   DWORD dwEnum, i;
   DWORD dwCurEnum;
   int   iRet;

// some error checking

   if (!pttsInfo) return ResultFromScode (E_INVALIDARG);
   if ((cel != 1) && !pdwFetched) return ResultFromScode (E_INVALIDARG);

// if our current position is already at the end then return an error

   if (((PCEngineEnum) m_pObj)->dwCurrentEnumPosn >= NUMTTSMODES)
      {
      if (pdwFetched) *pdwFetched = 0;
      memset (pttsInfo, 0, cel * sizeof(TTSMODEINFOW));
      return ResultFromScode (S_FALSE);
      }

// how many can we really enumerate?

   dwEnum = NUMTTSMODES - ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn;

   if (dwEnum > cel) dwEnum = cel;

   if (pdwFetched) *pdwFetched = dwEnum;

// do the copying

   for (i = 0; i < dwEnum; i++, pttsInfo++)
      {
      memset (pttsInfo, 0, sizeof(*pttsInfo));
      pttsInfo->dwFeatures = TTSFEATURE_ANYWORD;

      dwCurEnum = ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn + i;

      pttsInfo->gEngineID = CLSID_FakeTTSEngineEnum;   // SB ..Female
      iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "Acme Corporation", -1L,
         pttsInfo->szModeName, sizeof(pttsInfo->szMfgName) / sizeof(WCHAR));
      iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "Acme Talker", -1L,
         pttsInfo->szModeName, sizeof(pttsInfo->szProductName) / sizeof(WCHAR));

      switch (dwCurEnum)
         {
         case TTSMODE_MALE:

            pttsInfo->gModeID = MODEID_Male;   // SB ..Male
            pttsInfo->wGender = GENDER_MALE;
            iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "Acme Male Voice", -1L,
               pttsInfo->szModeName, sizeof(pttsInfo->szModeName) / sizeof(WCHAR));
            pttsInfo->language.LanguageID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
            iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "United States", -1L,
               pttsInfo->language.szDialect, sizeof(pttsInfo->szModeName) / sizeof(WCHAR));
            break;

         case TTSMODE_FEMALE:

            pttsInfo->gModeID = MODEID_Female;   // SB ..Female
            pttsInfo->wGender = GENDER_FEMALE;
            iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "Acme Female Voice", -1L,
               pttsInfo->szModeName, sizeof(pttsInfo->szModeName) / sizeof(WCHAR));
            pttsInfo->language.LanguageID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
            iRet = MultiByteToWideChar (CP_ACP, MB_PRECOMPOSED, "United States", -1L,
               pttsInfo->language.szDialect, sizeof(pttsInfo->szModeName) / sizeof(WCHAR));
            break;
         }
     }

   ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn += dwEnum;

   return (cel == dwEnum) ? NOERROR : ResultFromScode (S_FALSE);
}


STDMETHODIMP CEngITTSEnumA::Skip (ULONG cel)
{
   return (((PCEngineEnum) m_pObjA)->m_pEngITTSEnumW)->Skip(cel);
}


STDMETHODIMP CEngITTSEnumW::Skip (ULONG cel)
{
// If we skip beyond the edge of the modes then return an error
// and set the position to the end

   if ((cel + ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn) > NUMTTSMODES) 
      {
      ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn = NUMTTSMODES;
      return ResultFromScode (S_FALSE);
      }

// else, we're not skipping too much

   ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn += cel;
   return NOERROR;
}


STDMETHODIMP CEngITTSEnumA::Reset (void)
{
   return (((PCEngineEnum) m_pObjA)->m_pEngITTSEnumW)->Reset();
}


STDMETHODIMP CEngITTSEnumW::Reset (void)
{
// Set the current position back to the beginning

   ((PCEngineEnum) m_pObj)->dwCurrentEnumPosn = 0;

   return NOERROR;
}


STDMETHODIMP CEngITTSEnumA::Clone (ITTSEnumA * FAR * ppITTSEnum)
{
   PITTSENUMW  pITTSEnumW;
   HRESULT     hRes;

   if (!ppITTSEnum) return ResultFromScode(E_INVALIDARG);

   hRes = (((PCEngineEnum) m_pObjA)->m_pEngITTSEnumW)->Clone(&pITTSEnumW);

   if (hRes == NOERROR)
      {
      hRes = pITTSEnumW->QueryInterface(IID_ITTSEnumA, (void**) ppITTSEnum);
      pITTSEnumW->Release();
      }

   return hRes;
}


STDMETHODIMP CEngITTSEnumW::Clone (ITTSEnumW * FAR * ppITTSEnum)
{
   PCEngineEnum   pObj;
   HRESULT        hr;

   if (!ppITTSEnum) return ResultFromScode(E_INVALIDARG);

// Create a new enumeration object

   *ppITTSEnum = NULL;
   hr          = ResultFromScode (E_OUTOFMEMORY);
   pObj        = new CEngineEnum (((PCEngineEnum)m_pObj)->m_punkOuter,
                               ((PCEngineEnum)m_pObj)->m_pfnDestroy);

   if (NULL == pObj) return hr;
   if (pObj->FInit()) hr =pObj->QueryInterface(IID_ITTSEnumW,(void**)ppITTSEnum);

// Kill the object if initial creation or FInit failed

   if (FAILED(hr)) {
      delete pObj;
      return hr;
      }

   else gEngObjectCount++;


// set its current position to the current enumerator's position

   pObj->dwCurrentEnumPosn = ((PCEngineEnum)m_pObj)->dwCurrentEnumPosn;

// done

   return NOERROR;
}


STDMETHODIMP CEngITTSEnumA::Select(GUID gModeID, PITTSCENTRALA *ppITTSCentral,
                                   LPUNKNOWN pAudio)
{
   PITTSCENTRALW  pITTSCentralW;
   HRESULT        hRes;

   hRes = (((PCEngineEnum) m_pObjA)->m_pEngITTSEnumW)->Select(gModeID,
                                                         &pITTSCentralW, pAudio);
   if (hRes == NOERROR)
      {
      hRes = pITTSCentralW->QueryInterface(IID_ITTSCentralA, (void**) ppITTSCentral);
      pITTSCentralW->Release();
      }

   return hRes;
}


STDMETHODIMP CEngITTSEnumW::Select(GUID gModeID, PITTSCENTRALW *ppITTSCentral,
                                   LPUNKNOWN pAudio)
{
   PCMode         pMode;
   HRESULT        hr;
   PTTSMODEINFOW  pTTSInfo;

   if (!ppITTSCentral  || !pAudio)
      return ResultFromScode (E_INVALIDARG);

   if (!IsEqualGUID(gModeID, MODEID_Male) &&
       !IsEqualGUID(gModeID, MODEID_Female))

      return ResultFromScode (TTSERR_INVALIDMODE);

   *ppITTSCentral = NULL;

// Try to create a new enumeration object

   pMode = new CMode (NULL, ((PCEngineEnum)m_pObj)->m_pfnDestroy, pAudio);
   hr = ResultFromScode (E_OUTOFMEMORY);
   if (!pMode) return hr;

   if (pMode->FInit())
      hr = pMode->QueryInterface(IID_ITTSCentralW, (VOID**)ppITTSCentral);

   if (FAILED(hr)) delete pMode;    // kill the object if the finit failed

   else
      {
      pTTSInfo = &pMode->m_TTSModeInfo;

      memset (pTTSInfo, 0, sizeof(*pTTSInfo));

      pTTSInfo->gModeID    = gModeID;
      pTTSInfo->dwFeatures = TTSFEATURE_ANYWORD;
      pTTSInfo->gEngineID  = CLSID_FakeTTSEngineEnum;

      MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "Acme Corporation",
                          -1L, pTTSInfo->szModeName,
                          sizeof(pTTSInfo->szMfgName) / sizeof(WCHAR));

      MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "Acme Talker", -1L,
                          pTTSInfo->szModeName,
                          sizeof(pTTSInfo->szProductName) / sizeof(WCHAR));

      if (IsEqualGUID(gModeID, MODEID_Male))
         {
         MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "Acme Male Voice", -1L,
                             pTTSInfo->szModeName,
                             sizeof(pTTSInfo->szModeName) / sizeof(WCHAR));

         pTTSInfo->language.LanguageID = MAKELANGID(LANG_ENGLISH,
                                                    SUBLANG_ENGLISH_US);

         MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "United States", -1L,
                             pTTSInfo->language.szDialect,
                             sizeof(pTTSInfo->szModeName) / sizeof(WCHAR));
         }

      else
         {
         MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "Acme Female Voice", -1L,
                             pTTSInfo->szModeName,
                             sizeof(pTTSInfo->szModeName) / sizeof(WCHAR));

         pTTSInfo->language.LanguageID = MAKELANGID(LANG_ENGLISH,
                                                    SUBLANG_ENGLISH_US);

         MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "United States", -1L,
                             pTTSInfo->language.szDialect,
                             sizeof(pTTSInfo->szModeName) / sizeof(WCHAR));
         }

      gEngObjectCount++;
      }

   return hr;
}
