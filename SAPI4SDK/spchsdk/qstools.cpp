/*********************************************************************
STools.Cpp - Speech tools C++ wrapper objects.

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

/***************************************************************************8
CSTGramComp */

CSTGramComp::CSTGramComp (void)
{
   m_pISTGramComp = NULL;
}

CSTGramComp::~CSTGramComp (void)
{
   if (m_pISTGramComp)
      m_pISTGramComp->Release();
}

// intialization functions
HRESULT  CSTGramComp::Init (void)
{
   HRESULT  hRes;

   if (m_pISTGramComp)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_STGramComp, NULL, CLSCTX_ALL,
      IID_ISTGramComp, (void**)&m_pISTGramComp);
   if (hRes)
      return hRes;

   return NOERROR;
}

HRESULT  CSTGramComp::Init (PISTGRAMCOMP pISTGramComp)
{
   if (m_pISTGramComp)
      return ResultFromScode (E_FAIL);

   m_pISTGramComp = pISTGramComp;
   m_pISTGramComp->AddRef();
   return NOERROR;
}

HRESULT  CSTGramComp::Init (PVOID pMem, DWORD dwSize)       // also loads
{
   HRESULT  hRes;
   
   hRes = Init ();
   if (hRes)
      return hRes;
   hRes = FromMemory (pMem, dwSize);
   return hRes;
}

#ifdef STRICT
HRESULT  CSTGramComp::InitResource (PVOID hInst, DWORD dwResID) // also loads
#else
HRESULT  CSTGramComp::InitResource (HINSTANCE hInst, DWORD dwResID) // also loads
#endif
{
   HRESULT  hRes;
   
   hRes = Init ();
   if (hRes)
      return hRes;
   hRes = FromResource ((HINSTANCE) hInst, dwResID);
   return hRes;
}

HRESULT  CSTGramComp::Init (PCWSTR pszFile)                 // also loads
{
   HRESULT  hRes;
   
   hRes = Init ();
   if (hRes)
      return hRes;
   hRes = FromFile (pszFile);
   return hRes;
}

HRESULT  CSTGramComp::Init (IStream *pIStream)              // also loads
{
   HRESULT  hRes;
   
   hRes = Init ();
   if (hRes)
      return hRes;
   hRes = FromStream (pIStream);
   return hRes;
}


// gramcomp functions
HRESULT  CSTGramComp::FromMemory (PVOID pMem, DWORD dwSize)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->FromMemory (pMem, dwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSTGramComp::FromResource (PVOID hInst, DWORD dwResID)
#else
HRESULT  CSTGramComp::FromResource (HINSTANCE hInst, DWORD dwResID)
#endif
{
   if (m_pISTGramComp)
      return m_pISTGramComp->FromResource ((HINSTANCE) hInst, dwResID);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTGramComp::FromFile (PCWSTR pszFile)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->FromFile (pszFile);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTGramComp::FromStream (IStream *pIStream)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->FromStream (pIStream);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTGramComp::Compile (LPWSTR* ppszError, DWORD *pdwSize)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->Compile (ppszError, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTGramComp::IsCompiled (BOOL *pfCompiled)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->IsCompiled (pfCompiled);
   else
      return ResultFromScode (E_NOINTERFACE);
}

BOOL     CSTGramComp::IsCompiled (void)
{
   BOOL  fCompiled = FALSE;
   IsCompiled (&fCompiled);
   return fCompiled;
}

HRESULT  CSTGramComp::ToMemory (PVOID *ppMem, DWORD *pdwSize)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->ToMemory (ppMem, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTGramComp::ToFile (PCWSTR pszFile)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->ToFile (pszFile);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTGramComp::ToStream (IStream *pIStream)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->ToStream (pIStream);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTGramComp::TextGet (LPWSTR *pszText, DWORD *pdwSize)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->TextGet (pszText, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTGramComp::LanguageGet (LANGUAGEW *pLang)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->LanguageGet (pLang);
   else
      return ResultFromScode (E_NOINTERFACE);
}

LANGID   CSTGramComp::LanguageGet (void)
{
   LANGUAGEW   lang;
   lang.LanguageID = 0;
   LanguageGet (&lang);
   return lang.LanguageID;
}

HRESULT  CSTGramComp::GrammarFormatGet (SRGRMFMT* pFmt)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->GrammarFormatGet (pFmt);
   else
      return ResultFromScode (E_NOINTERFACE);
}

SRGRMFMT CSTGramComp::GrammarFormatGet (void)
{
   SRGRMFMT fmt = (SRGRMFMT) 0;
   GrammarFormatGet (&fmt);
   return fmt;
}

HRESULT  CSTGramComp::GrammarDataGet (BOOL fEngine, PVOID *ppMem, DWORD *pdwSize)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->GrammarDataGet (fEngine, ppMem, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTGramComp::GrammarDataSet (PVOID pMem, DWORD dwSize)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->GrammarDataSet (pMem, dwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTGramComp::AutoList (PISRGRAMCFGW pISRGramCFG)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->AutoList (pISRGramCFG);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTGramComp::AutoList (PCSRGram pCSRGram)
{
   return AutoList (pCSRGram->m_pISRGramCFG);
}

HRESULT  CSTGramComp::PhraseParse (PSRPHRASEW pSRPhrase, DWORD *pdwCmdID,
   LPWSTR *ppszParse, DWORD *pdwSize)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->PhraseParse (pSRPhrase, pdwCmdID, ppszParse, pdwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CSTGramComp::PhraseParse (PSRPHRASEW pSRPhrase,
   LPWSTR *ppszParse, DWORD *pdwSize)
{
   DWORD dwCmdID = 0;
   PhraseParse (pSRPhrase, &dwCmdID, ppszParse, pdwSize);
   return dwCmdID;
}

HRESULT  CSTGramComp::GrammarLoad (LPUNKNOWN lpUnkMode, PVOID pNotifySink,
   IID IIDGramNotifySink, LPUNKNOWN *ppIUnkGram)
{
   if (m_pISTGramComp)
      return m_pISTGramComp->GrammarLoad (lpUnkMode, pNotifySink, IIDGramNotifySink, ppIUnkGram);
   else
      return ResultFromScode (E_NOINTERFACE);
}

PCSRGram CSTGramComp::GrammarLoad (PCSRMode pCSRMode, PISRGRAMNOTIFYSINKW pNotifySink)
{
   LPUNKNOWN   pIUnkGram = NULL;
   HRESULT     hRes;
   hRes = GrammarLoad (pCSRMode->m_pISRCentral, pNotifySink,
      IID_ISRGramNotifySinkW, &pIUnkGram);
   if (hRes)
      return NULL;

   CSRGram  *pCSRGram;
   pCSRGram = new CSRGram;
   if (!pCSRGram) {
      pIUnkGram->Release();
      return NULL;
   }

   pCSRGram->Init(pIUnkGram);
   pIUnkGram->Release();

   return pCSRGram;
}


/****************************************************************************
SRGramComp - This is a compiled SRGram and STGramComp object that makes
   it really easy to use both the compiler and the grammar.
*/

CSRGramComp::CSRGramComp (void)
{
   m_pISRCentral = NULL;
   m_pISRGramNotifySink = NULL;
}

CSRGramComp::~CSRGramComp (void)
{
   if (m_pISRCentral)
      m_pISRCentral->Release();
   if (m_pISRGramNotifySink)
      m_pISRGramNotifySink->Release();
}

// intialization functions
HRESULT  CSRGramComp::Init (PCSRMode pCSRMode, PISRGRAMNOTIFYSINKW pISRGramNotifySink)
{
   if (m_pISRCentral || !pISRGramNotifySink)
      return ResultFromScode (E_FAIL);

   m_pISRCentral = pCSRMode->m_pISRCentral;
   if (!m_pISRCentral)
      return ResultFromScode (E_NOINTERFACE);
   m_pISRCentral->AddRef();
   m_pISRGramNotifySink = pISRGramNotifySink;
   m_pISRGramNotifySink->AddRef();

   HRESULT  hRes;
   hRes = this->CSTGramComp::Init();
   if (hRes)
      return hRes;

   return NOERROR;
}

// Function do FromXXX, compile (if necessary), and GrammarLoad()
// If any of these fails then it's all freed
HRESULT  CSRGramComp::GrammarFromMemory (PVOID pMem, DWORD dwSize)
{
   HRESULT  hRes;

   // load it in
   hRes = FromMemory (pMem, dwSize);
   if (hRes)
      return hRes;

   // compile if necessary
   if (!IsCompiled()) {
      hRes = Compile();
      if (hRes)
         return hRes;
   }

   // load the grammar
   return GrammarLoad();
}

#ifdef STRICT
HRESULT  CSRGramComp::GrammarFromResource (PVOID hInst, DWORD dwResID)
#else
HRESULT  CSRGramComp::GrammarFromResource (HINSTANCE hInst, DWORD dwResID)
#endif
{
   HRESULT  hRes;

   // load it in
   hRes = FromResource ((HINSTANCE) hInst, dwResID);
   if (hRes)
      return hRes;

   // compile if necessary
   if (!IsCompiled()) {
      hRes = Compile();
      if (hRes)
         return hRes;
   }

   // load the grammar
   return GrammarLoad();
}

HRESULT  CSRGramComp::GrammarFromFile (PCWSTR pszFile)
{
   HRESULT  hRes;

   // load it in
   hRes = FromFile (pszFile);
   if (hRes)
      return hRes;

   // compile if necessary
   if (!IsCompiled()) {
      hRes = Compile();
      if (hRes)
         return hRes;
   }

   // load the grammar
   return GrammarLoad();
}

HRESULT  CSRGramComp::GrammarFromStream (IStream *pIStream)
{
   HRESULT  hRes;

   // load it in
   hRes = FromStream (pIStream);
   if (hRes)
      return hRes;

   // compile if necessary
   if (!IsCompiled()) {
      hRes = Compile();
      if (hRes)
         return hRes;
   }

   // load the grammar
   return GrammarLoad();
}


// overloaded functions because now the grammar, central, or isrgramnotifysink
HRESULT  CSRGramComp::AutoList (void)
{
   if (!m_pISRGramCFG)
      return ResultFromScode (E_NOINTERFACE);

   return this->CSTGramComp::AutoList (m_pISRGramCFG);
}

HRESULT  CSRGramComp::GrammarLoad (void)
{
   if (!m_pISRCentral || !m_pISRGramNotifySink)
      return ResultFromScode (E_NOINTERFACE);

   HRESULT     hRes;
   LPUNKNOWN   pUnkGram;
   hRes = this->CSTGramComp::GrammarLoad ((LPUNKNOWN) m_pISRCentral, (PVOID) m_pISRGramNotifySink,
      (IID) IID_ISRGramNotifySinkW, (LPUNKNOWN*) &pUnkGram);
   if (hRes) return hRes;

   // store the grammar away
   hRes = this->CSRGram::Init(pUnkGram);
   pUnkGram->Release();
   return hRes;
}

HRESULT  CSRGramComp::GrammarDataSet (short fArchiveResults)
{
   if (!m_pISRGramCommon)
      return ResultFromScode (E_NOINTERFACE);

   HRESULT  hRes;
   DWORD dwBufNeeded = 0;
   hRes = m_pISRGramCommon->Archive(fArchiveResults, NULL, 0,
      &dwBufNeeded);
   if (hRes != SRERR_NOTENOUGHDATA )
      return hRes;

   // allocate enough memory
   PVOID pMem;
   pMem = malloc (dwBufNeeded);
   if (!pMem)
      return ResultFromScode (E_OUTOFMEMORY);

   // call archive again
   hRes = m_pISRGramCommon->Archive(fArchiveResults, pMem, dwBufNeeded,
      &dwBufNeeded);
   if (hRes) {
      free (pMem);
      return hRes;
   }

   // write the memory
   hRes = this->CSTGramComp::GrammarDataSet (pMem, dwBufNeeded);
   free (pMem);
   return hRes;
}



/***********************************************************************
STMicWizard */

CSTMicWizard::CSTMicWizard (void)
{
   m_pISTMicWizard = NULL;
}

CSTMicWizard::~CSTMicWizard (void)
{
   if (m_pISTMicWizard)
      m_pISTMicWizard->Release();
}

// initalization
HRESULT  CSTMicWizard::Init (void)
{
   HRESULT  hRes;

   if (m_pISTMicWizard)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_STMicWizard, NULL, CLSCTX_ALL,
      IID_ISTMicWizard, (void**)&m_pISTMicWizard);
   if (hRes)
      return hRes;

   return NOERROR;
}


// ISTMicWizard
HRESULT  CSTMicWizard::InfoGet (PMICWIZARDINFO pInfo)
{
   if (m_pISTMicWizard)
      return m_pISTMicWizard->InfoGet (pInfo);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTMicWizard::InfoSet (PMICWIZARDINFO pInfo)
{
   if (m_pISTMicWizard)
      return m_pISTMicWizard->InfoSet (pInfo);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSTMicWizard::Wizard (PVOID hWndParent, DWORD dwUse,
            DWORD dwWaveInDevice ,
            DWORD dwSamplesPerSec ,
            DWORD dwFlags)
#else
HRESULT  CSTMicWizard::Wizard (HWND hWndParent, DWORD dwUse,
            DWORD dwWaveInDevice ,
            DWORD dwSamplesPerSec ,
            DWORD dwFlags)
#endif
{
   if (m_pISTMicWizard)
      return m_pISTMicWizard->Wizard ((HWND) hWndParent, dwUse, dwWaveInDevice, dwSamplesPerSec, dwFlags);
   else
      return ResultFromScode (E_NOINTERFACE);
}

/***********************************************************************
STLexDlg */

CSTLexDlg::CSTLexDlg (void)
{
   m_pISTLexDlg = NULL;
}

CSTLexDlg::~CSTLexDlg (void)
{
   if (m_pISTLexDlg)
      m_pISTLexDlg->Release();
}

// initalization
HRESULT  CSTLexDlg::Init (void)
{
   HRESULT  hRes;

   if (m_pISTLexDlg)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_STLexDlg, NULL, CLSCTX_ALL,
      IID_ISTLexDlg, (void**)&m_pISTLexDlg);
   if (hRes)
      return hRes;

   return NOERROR;
}

#ifdef STRICT
HRESULT  CSTLexDlg::LexDlg (PVOID hWndParent,
                             PCWSTR pszWord,
                             DWORD dwFlags,
                             LANGID langID,
                             LPUNKNOWN pUnkLex,
                             LPUNKNOWN pUnkTTS,
                             LPUNKNOWN pUnkBackupLex,
                             PCWSTR pszPhonemes, PCWSTR pszTitle)
#else
HRESULT  CSTLexDlg::LexDlg (HWND hWndParent,
                             PCWSTR pszWord,
                             DWORD dwFlags,
                             LANGID langID,
                             LPUNKNOWN pUnkLex,
                             LPUNKNOWN pUnkTTS,
                             LPUNKNOWN pUnkBackupLex,
                             PCWSTR pszPhonemes, PCWSTR pszTitle)
#endif
{
   if (m_pISTLexDlg)
      return m_pISTLexDlg->LexDlg ((HWND) hWndParent, pszWord, dwFlags, langID, pUnkLex,
         pUnkTTS, pUnkBackupLex, pszPhonemes, pszTitle);
   else
      return ResultFromScode (E_NOINTERFACE);
}


/***********************************************************************
STPhoneConv */

CSTPhoneConv::CSTPhoneConv (void)
{
   m_pISTPhoneConv = NULL;
}

CSTPhoneConv::~CSTPhoneConv (void)
{
   if (m_pISTPhoneConv)
      m_pISTPhoneConv->Release();
}

// initalization
HRESULT  CSTPhoneConv::Init (void)
{
   HRESULT  hRes;

   if (m_pISTPhoneConv)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_STPhoneConv, NULL, CLSCTX_ALL,
      IID_ISTPhoneConv, (void**)&m_pISTPhoneConv);
   if (hRes)
      return hRes;

   return NOERROR;
}


HRESULT  CSTPhoneConv::FromIPA (LPCWSTR pszFromIPA, DWORD *pdwFirstInvalid,
   LPWSTR pszToSet, DWORD dwNumChars, DWORD *pdwCharsNeeded)
{
   if (m_pISTPhoneConv)
      return m_pISTPhoneConv->FromIPA (pszFromIPA, pdwFirstInvalid,
         pszToSet, dwNumChars, pdwCharsNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTPhoneConv::Get (SDATA *pData)
{
   if (m_pISTPhoneConv)
      return m_pISTPhoneConv->Get (pData);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTPhoneConv::SetByLang (LANGID lang)
{
   if (m_pISTPhoneConv)
      return m_pISTPhoneConv->SetByLang (lang);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTPhoneConv::SetByText (LPCWSTR pszText)
{
   if (m_pISTPhoneConv)
      return m_pISTPhoneConv->SetByText (pszText);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTPhoneConv::ToIPA(LPCWSTR pszFromSet, DWORD *pdwFirstInvalid,
   LPWSTR pszToIPA, DWORD dwNumChars, DWORD *pdwCharsNeeded)
{
   if (m_pISTPhoneConv)
      return m_pISTPhoneConv->ToIPA (pszFromSet, pdwFirstInvalid,
         pszToIPA, dwNumChars, pdwCharsNeeded);
   else
      return ResultFromScode (E_NOINTERFACE);
}



/***********************************************************************
STLog */

CSTLog::CSTLog (void)
{
   m_pISTLog = NULL;
}

CSTLog::~CSTLog (void)
{
   if (m_pISTLog)
      m_pISTLog->Release();
}

// initalization
HRESULT  CSTLog::Init (void)
{
   HRESULT  hRes;

   if (m_pISTLog)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_STLog, NULL, CLSCTX_ALL,
      IID_ISTLog, (void**)&m_pISTLog);
   if (hRes)
      return hRes;

   return NOERROR;
}

HRESULT  CSTLog::Init (LPUNKNOWN lpUnk)
{
   if (m_pISTLog)
      return ResultFromScode (E_FAIL);

   return lpUnk->QueryInterface (IID_ISTLog, (PVOID*)&m_pISTLog);
}

HRESULT  CSTLog::ToFile (PCWSTR pszFile)
{
   if (m_pISTLog)
      return m_pISTLog->ToFile (pszFile);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTLog::ToStream (IStream *pIStream)
{
   if (m_pISTLog)
      return m_pISTLog->ToStream (pIStream);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTLog::Text (PCWSTR pszSource, PCWSTR pszMessage, WORD wDetail)
{
   if (m_pISTLog)
      return m_pISTLog->Text (pszSource, pszMessage, wDetail);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTLog::Data (PCWSTR pszSource, PCWSTR pszMessage, WORD wDetail,
            PVOID pMem, DWORD dwSize)
{
   if (m_pISTLog)
      return m_pISTLog->Data (pszSource, pszMessage, wDetail, pMem, dwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTLog::MaxDetailSet (WORD wDetail)
{
   if (m_pISTLog)
      return m_pISTLog->MaxDetailSet (wDetail);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTLog::MaxDetailGet (WORD *pwDetail)
{
   if (m_pISTLog)
      return m_pISTLog->MaxDetailGet (pwDetail);
   else
      return ResultFromScode (E_NOINTERFACE);
}

WORD     CSTLog::MaxDetailGet (void)
{
   WORD  wDetail = 0;
   MaxDetailGet (&wDetail);
   return wDetail;
}

HRESULT  CSTLog::ShowOutput (BOOL fShow)
{
   if (m_pISTLog)
      return m_pISTLog->ShowOutput (fShow);
   else
      return ResultFromScode (E_NOINTERFACE);
}





/***********************************************************************
STTTSQueue */

CSTTTSQueue::CSTTTSQueue (void)
{
   m_pISTTTSQueue = NULL;
}

CSTTTSQueue::~CSTTTSQueue (void)
{
   if (m_pISTTTSQueue)
      m_pISTTTSQueue->Release();
}

// initalization
HRESULT  CSTTTSQueue::Init (void)
{
   HRESULT  hRes;

   if (m_pISTTTSQueue)
      return ResultFromScode (E_FAIL);

   hRes = CoCreateInstance (CLSID_STTTSQueue, NULL, CLSCTX_ALL,
      IID_ISTTTSQueue, (void**)&m_pISTTTSQueue);
   if (hRes)
      return hRes;

   return NOERROR;
}

HRESULT  CSTTTSQueue::Init (LPUNKNOWN lpUnk)
{
   if (m_pISTTTSQueue)
      return ResultFromScode (E_FAIL);

   return lpUnk->QueryInterface (IID_ISTTTSQueue, (PVOID*)&m_pISTTTSQueue);
}

HRESULT  CSTTTSQueue::WaveAddFromFile (PCWSTR pszSpeakString, PCWSTR pszFile)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->WaveAddFromFile (pszSpeakString, pszFile);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::WaveAddFromStream (PCWSTR pszSpeakString, IStream *pIStream)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->WaveAddFromStream (pszSpeakString, pIStream);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::WaveAddFromMemory (PCWSTR pszSpeakString, PVOID pMem, DWORD dwSize)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->WaveAddFromMemory (pszSpeakString, pMem, dwSize);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSTTTSQueue::WaveAddFromResource (PCWSTR pszSpeakString, PVOID hModule, DWORD dwID)
#else
HRESULT  CSTTTSQueue::WaveAddFromResource (PCWSTR pszSpeakString, HMODULE hModule, DWORD dwID)
#endif
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->WaveAddFromResource (pszSpeakString, (HINSTANCE) hModule, dwID);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSTTTSQueue::WaveAddFromList (PCWSTR pszString, PVOID hModule)
#else
HRESULT  CSTTTSQueue::WaveAddFromList (PCWSTR pszString, HMODULE hModule)
#endif
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->WaveAddFromList (pszString, (HINSTANCE) hModule);
   else
      return ResultFromScode (E_NOINTERFACE);
}

#ifdef STRICT
HRESULT  CSTTTSQueue::WaveAddFromList (DWORD dwResourceID, PVOID hModule)
#else
HRESULT  CSTTTSQueue::WaveAddFromList (DWORD dwResourceID, HMODULE hModule)
#endif
{
   HRSRC hRsrc;

   hRsrc = FindResource((HINSTANCE) hModule, MAKEINTRESOURCE(dwResourceID), "TEXT");
   if (!hRsrc)
      return E_FAIL;

   HGLOBAL hMem;
   hMem = LoadResource ((HINSTANCE) hModule, hRsrc);
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
   hRes = WaveAddFromList ((PCWSTR) pMem, (HINSTANCE) hModule);

   // Free the resource
   FreeResource (hRsrc);

   return hRes;
}

HRESULT  CSTTTSQueue::WaveRemove (PCWSTR pszSpeakString)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->WaveRemove (pszSpeakString);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::WaveAudioDestSet (LPUNKNOWN pIUnk)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->WaveAudioDestSet (pIUnk);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::WaveLevelSet (DWORD dwLevel)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->WaveLevelSet (dwLevel);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::WaveLevelGet (DWORD *pdwLevel)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->WaveLevelGet (pdwLevel);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD    CSTTTSQueue::WaveLevelGet (void)
{
   DWORD dwLevel = 0;
   WaveLevelGet (&dwLevel);
   return dwLevel;
}

HRESULT  CSTTTSQueue::TTSAdd (PCWSTR pszVoice, LPUNKNOWN pUnkTTS)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->TTSAdd (pszVoice, pUnkTTS);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::TTSAdd (PCWSTR pszVoice, PCTTSMode pCTTSMode)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->TTSAdd (pszVoice, pCTTSMode->m_pITTSCentral);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::TTSRemove (PCWSTR pszVoice)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->TTSRemove (pszVoice);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::TTSGet (PCWSTR pszVoice, LPUNKNOWN *ppUnkTTS)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->TTSGet (pszVoice, ppUnkTTS);
   else
      return ResultFromScode (E_NOINTERFACE);
}

PCTTSMode CSTTTSQueue::TTSGet (PCWSTR pszVoice)
{
   LPUNKNOWN   pUnk;
   HRESULT     hRes;
   hRes = TTSGet (pszVoice, &pUnk);
   if (hRes)
      return NULL;

   PCTTSMode   pMode;
   pMode = new CTTSMode;
   if (!pMode) {
      pUnk->Release();
      return NULL;
   }
   hRes = pMode->Init (pUnk);
   pUnk->Release();
   if (hRes) {
      delete pMode;
      return NULL;
   }

   return pMode;
}

HRESULT  CSTTTSQueue::Speak (PCWSTR pszSpeak, PCWSTR pszVoice, DWORD dwID)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->Speak (pszSpeak, pszVoice, dwID);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::TextData (VOICECHARSET eCharSet, DWORD dwFlags,
   SDATA dData, PCWSTR pszVoice, DWORD dwID)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->TextData (eCharSet, dwFlags, dData, pszVoice, dwID);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::AudioPause (void)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->AudioPause ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::AudioResume (void)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->AudioResume ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::AudioReset (void)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->AudioReset ();
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::Register (PVOID pNotifySink, REFIID IIDNotifySink)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->Register (pNotifySink, IIDNotifySink);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::Register (PISTTTSQUEUENOTIFYSINK pNotifySink)
{
   return Register (pNotifySink, IID_ISTTTSQueueNotifySink);
}

HRESULT  CSTTTSQueue::UseLogging (LPUNKNOWN pUnkLog)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->UseLogging (pUnkLog);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::UseLogging (PCSTLog pLog)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->UseLogging (pLog->m_pISTLog);
   else
      return ResultFromScode (E_NOINTERFACE);
}

HRESULT  CSTTTSQueue::IsSpeaking (DWORD *pdwSpeak)
{
   if (m_pISTTTSQueue)
      return m_pISTTTSQueue->IsSpeaking (pdwSpeak);
   else
      return ResultFromScode (E_NOINTERFACE);
}

DWORD  CSTTTSQueue::IsSpeaking (void)
{
   DWORD dw = 0;
   IsSpeaking (&dw);
   return dw;
}

/* WaitUntilDoneSpeaking - Waits in a message loop until the IsSpeaking
   function returns false */
void CSTTTSQueue::WaitUntilDoneSpeaking (void)
{
   // wait in message loop
   MSG msg;
   DWORD dwSpeak;
	while (GetMessage(&msg, NULL, NULL, NULL) )
	{
        dwSpeak = IsSpeaking();
        if (!dwSpeak)
            break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}





/**************************************************************************8
CSTTTSQueueNotifySink */

STDMETHODIMP CSTTTSQueueNotifySink::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISTTTSQueueNotifySink))
   {
      *ppv = (LPVOID) this;
      AddRef();
      return ResultFromScode (NOERROR);
   }
   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CSTTTSQueueNotifySink::AddRef (void)
{
   return(++m_RefCount);
}

STDMETHODIMP_(ULONG) CSTTTSQueueNotifySink::Release (void)
{
   if((--m_RefCount) == 0)
   {
	delete this;
   }
   return(m_RefCount);
};

STDMETHODIMP CSTTTSQueueNotifySink::Start(void)
{
   return NOERROR;
}

STDMETHODIMP CSTTTSQueueNotifySink::Stop(void)
{
   return NOERROR;
}

STDMETHODIMP CSTTTSQueueNotifySink::SpeakID(DWORD dwID)
{
   return NOERROR;
}

STDMETHODIMP CSTTTSQueueNotifySink::Error(HRESULT hRes)
{
   return NOERROR;
}

STDMETHODIMP CSTTTSQueueNotifySink::Bookmark(DWORD dwID)
{
   return NOERROR;
}


