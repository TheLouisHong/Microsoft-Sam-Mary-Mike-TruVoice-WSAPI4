/************************************************************************
Name.cpp - Simple control to ask for a name

   This includes the interface:
      IClassFactory
      IUnknown

Copyright (c) 1995-1997 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <objbase.h>
#include <objerror.h>
#include <spchwrap.h>
#include "telctl.h"
#include "resource.h"

static WCHAR gszStart[] = L"AskName";
static WCHAR gszDefaultSpecialGrammar[] = L"[<Start>]\r\n<Start>=(DefaultResponses)";


CName::CName()
{
   m_szSpecialGrammar = gszDefaultSpecialGrammar;
   m_szParse = 0;
   m_bVerifying = false;
}

CName::~CName()
{
}

PCWSTR CName::GetControlName(void)
{
   return L"Name";
}

DWORD CName::GetDefaultTextResID(LANGID langID)
{
   // since only have one, return the english one no matter what
   return IDR_NAME;
}

void CName::GetCLSID(CLSID *pCLSID)
{
#ifdef BUILDREALCONTROL
   *pCLSID = CLSID_NameControl;
#else
   *pCLSID = CLSID_SampleNameControl;
#endif
}

STDMETHODIMP CName::Compile (LPWSTR *pszError, DWORD *pdwSize)
{
   m_fIsCompiled = FALSE;

   DWORD dwSpecialGrammarLength = (m_szSpecialGrammar ? wcslen(m_szSpecialGrammar) : 0);
   DWORD dwTextLength           = (m_pszText          ? wcslen(m_pszText)          : 0);
   DWORD dwDefaultTextLength    = (m_pszDefaultText   ? wcslen(m_pszDefaultText)   : 0);

   DWORD dwNeededLength = dwSpecialGrammarLength + 2 + dwTextLength + 2 + dwDefaultTextLength + 2;
   WCHAR *sz;
   if((sz = (WCHAR *)malloc((dwNeededLength + 1) * sizeof(WCHAR))) == 0)
      return ResultFromScode(E_OUTOFMEMORY);

   sz[0] = L'\0';
   if(m_szSpecialGrammar)  wcscat(sz, m_szSpecialGrammar), wcscat(sz, L"\r\n");
   if(m_pszText)           wcscat(sz, m_pszText),          wcscat(sz, L"\r\n");
   if(m_pszDefaultText)    wcscat(sz, m_pszDefaultText),   wcscat(sz, L"\r\n");

   // DEBUGGING: write the grammar to a file
   //FILE *pfile = _wfopen(L"C:\\temp\\gram.txt", L"w");
   //fwprintf(pfile, L"%s", sz);
   //fclose(pfile);

   HRESULT  hRes;
   hRes = m_pGramComp->FromMemory(sz, (wcslen(sz) + 1) * 2);
   free(sz); // free the combined memory
   if(hRes)
      return hRes;

   // compile
   //DWORD dwSize = 8191;
   //WCHAR *szError;
   //hRes = m_pGramComp->Compile(&szError, &dwSize);
   hRes = m_pGramComp->Compile(pszError, pdwSize);
   if(hRes)
      return hRes;

   // we're done
   m_fIsCompiled = TRUE;

   // create the grammar for SR
   if (m_pGram) {
      delete m_pGram;
      m_pGram = m_pGramComp->GrammarLoad (m_pSRMode, m_pISRGramNotifySink);
      if (!m_pGram)
         return E_FAIL;
   }

   return NOERROR;
}


void CName::OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                           DWORD dwParseMemSize, PSRPHRASEW pSRPhrase,
                           LPUNKNOWN lpUnkResult) {
   WCHAR *szPhrase = (WCHAR *)pParseMem;
   SNameAuxPerson nap;

   m_dwRecog = dwParseID;
   m_bVerifying = false;

   if(!wcscmp(szPhrase, L"none")) {
      DoState(4);
      return;
   }
   else if(!wcscmp(szPhrase, L"spell")) {
      DoState(5);
      return;
   }
   else if(!wcscmp(szPhrase, L"listNames")) {
      DoState(6);
      return;
   }
   else if(!wcscmp(szPhrase, L"cancel")) {
      DoState(4);
      return;
   }

   // fill in as many fields in nap as the user specified in this parse

   // check for a pre-title
   if(!wcsncmp(szPhrase, L"PRE", 3)) {
      nap.m_szPreTitle = szPhrase + 3;
      szPhrase = wcsstr(szPhrase, L"END");
      *szPhrase = L'\0';
      szPhrase += 3;
   }

   // check for a first name
   if(!wcsncmp(szPhrase, L"FIRST", 5)) {
      nap.m_szFirstName = szPhrase + 5;
      szPhrase = wcsstr(szPhrase, L"END");
      *szPhrase = L'\0';
      szPhrase += 3;
   }

   // check for a last name
   if(!wcsncmp(szPhrase, L"LAST", 4)) {
      nap.m_szLastName = szPhrase + 4;
      szPhrase = wcsstr(szPhrase, L"END");
      *szPhrase = L'\0';
      szPhrase += 3;
   }

   // check for a post-title
   if(!wcsncmp(szPhrase, L"POST", 4)) {
      nap.m_szPostTitle = szPhrase + 4;
      szPhrase = wcsstr(szPhrase, L"END");
      *szPhrase = L'\0';
      szPhrase += 3;
   }

   // check for a full name
   // if we ever actually see FULL, then the name is the rest of the string and there is no END token
   if(!wcsncmp(szPhrase, L"FULL", 4)) {
      nap.m_szFullName = szPhrase + 4;
   }

   // loop through each in the current list to see if they match; if not, discard them
   for(CLinkedListIterator<SNameAuxPerson> lli = m_llPeople.GetHead(); lli.IsValid(); ) {
      CLinkedListIterator<SNameAuxPerson> lli2 = lli;
      lli2.Next();
      if(nap != *(lli.GetData()))
         (void)m_llPeople.EDestroyAt(lli);
      lli = lli2;
   }

   nap.Clear(); // so that when the destructor gets called, it doesn't try to free the strings
   DoState(1);
}


void CName::OnState(DWORD dwStateID)
{
   WCHAR szBuffer[1024];
   DWORD dwSize;

   switch(dwStateID) {
   case 0: // initialize
      m_bIsFirstPrompt = true;
      m_szParse = 0;
      FillInAllNames();
      DoState(1);
      break;

   case 1: // some names left
      if(m_llPeople.GetNumElems() == 1)
         DoState(2);
      else if(m_llPeople.GetNumElems() == 0)
         DoState(4);
      else {
         WCHAR *szPrompt;
         if(m_llPeople.GetNumElems() == 2)
            GrammarForTwo(&szPrompt);
         else
            GrammarForMany(&szPrompt);
         m_pQueue->Speak(szPrompt);
         free(szPrompt);
         if(Compile(NULL, NULL) != NOERROR) {
            DoState(4);
            return;
         }
         free(m_szSpecialGrammar);
         m_szSpecialGrammar = gszDefaultSpecialGrammar;
         ReadyToListenBeep();
         StartSRWhenReady(gszStart);
      }
      break;

   case 2: // only one name left
      m_szParse = m_llPeople.GetHead().GetData()->m_szFullName;
      if(m_fVerify)
         m_bVerifying = true, DoVerify(L"VerifyPre", L"VerifyPost", m_szParse, 3, 0);
      else
         DoState(3);   // verified
      break;

   case 3: // successfully got name
      DoFinish(m_dwRecog, m_szParse, (wcslen(m_szParse) + 1) * sizeof(WCHAR));
      break;

   case 4: // cancelled or didn't get name
      DoFinish(1, L"", 0);
      break;

   case 5: // wants to spell name
      DoFinish(2, L"He wants to spell it", (wcslen(L"He wants to spell it") + 1) * sizeof(WCHAR));
      break;

   case 6: // list names
      if(m_llPeople.GetNumElems() > 5) {
         WCHAR szTemplate[1024];
         dwSize = 1023;
         GetValue(L"Prompts", L"ListNamesWarn", szTemplate, &dwSize);
         swprintf(szBuffer, szTemplate, m_llPeople.GetNumElems());
         m_pQueue->Speak(szBuffer);
         // TODO: actually verify
         m_pQueue->Speak(L"I don't know how to verify right now, so here they are.");
      }
      dwSize = 1023;
      GetValue(L"Prompts", L"ListNamesPre", szBuffer, &dwSize);
      m_pQueue->Speak(szBuffer);
      for(CLinkedListIterator<SNameAuxPerson> lli = m_llPeople.GetHead(); lli.IsValid(); lli.Next())
         m_pQueue->Speak(lli.GetData()->m_szFullName);
      dwSize = 1023;
      GetValue(L"Prompts", L"ListNamesPost", szBuffer, &dwSize);
      m_pQueue->Speak(szBuffer);
      DoState(1);
      break;
   }
}


void CName::FillInAllNames() {
   WCHAR szNameBuffer[128], szValueBuffer[128];
   DWORD dwNameSize, dwValueSize;
   bool bEnd;
   SNameAuxPerson *pnap;

   (void)m_llPeople.EDestroyAll();

   if(!m_pTextParse->SectionFind(L"Names"))
      return;
   bEnd = (m_pTextParse->ValueReset() ? true : false);

   while(bEnd) {
      dwNameSize = 127, dwValueSize = 127;
      m_pTextParse->ValueQuery(szNameBuffer, &dwNameSize, szValueBuffer, &dwValueSize);
      if((pnap = new SNameAuxPerson(szValueBuffer)) == 0) {
         DoState(4);
         return;
      }
      (void)m_llPeople.EAddToTail(pnap);
      bEnd = (m_pTextParse->ValueNext() ? true : false);
   }
}


void CName::GrammarForTwo(WCHAR **pszPrompt) {
   WCHAR szBuffer[128], szGrammarBuffer[16384];
   SNameAuxPerson *pnap1, *pnap2;

   CLinkedListIterator<SNameAuxPerson> lli = m_llPeople.GetHead();
   pnap1 = lli.GetData();
   lli.Next();
   pnap2 = lli.GetData();

   m_szSpecialGrammar = szGrammarBuffer; // TODO: later, actually figure out how big it needs to be and allocate it
   szGrammarBuffer[0] = L'\0';

   wcscpy(m_szSpecialGrammar, L"[<AskName>]\r\n<AskName>=(DefaultResponses)\r\n");

   ConcatGrammarAuxData(m_szSpecialGrammar, pnap1, 1);
   ConcatGrammarAuxData(m_szSpecialGrammar, pnap2, 2);

   swprintf(szBuffer, L"<AskName>=[opt] the first [opt] one \"FULL%s\"\r\n", pnap1->m_szFullName);
   wcscat(m_szSpecialGrammar, szBuffer);
   swprintf(szBuffer, L"<AskName>=[opt] the second [opt] one \"FULL%s\"\r\n", pnap2->m_szFullName);
   wcscat(m_szSpecialGrammar, szBuffer);
   swprintf(szBuffer, L"<AskName>=[opt] the former [opt] one \"FULL%s\"\r\n", pnap1->m_szFullName);
   wcscat(m_szSpecialGrammar, szBuffer);
   swprintf(szBuffer, L"<AskName>=[opt] the latter [opt] one \"FULL%s\"\r\n", pnap2->m_szFullName);
   wcscat(m_szSpecialGrammar, szBuffer);

   // differentiate according to first name
   if(pnap1->m_szFirstName && pnap2->m_szFirstName && wcscmp(pnap1->m_szFirstName, pnap2->m_szFirstName)) {
      ConcatGrammarFirstPossLast(m_szSpecialGrammar, pnap1, 1);
      ConcatGrammarFirstPossLast(m_szSpecialGrammar, pnap2, 2);
   }

   // differentiate according to last name
   else if(pnap1->m_szLastName && pnap2->m_szLastName && wcscmp(pnap1->m_szLastName, pnap2->m_szLastName)) {
      ConcatGrammarPossFirstLast(m_szSpecialGrammar, pnap1, 1);
      ConcatGrammarPossFirstLast(m_szSpecialGrammar, pnap2, 2);
   }
   else {
      // only titles differentiate
      // TODO: what exactly should we do here????
      ConcatGrammarPossFirstLast(m_szSpecialGrammar, pnap1, 1);
      ConcatGrammarPossFirstLast(m_szSpecialGrammar, pnap2, 2);
   }

   wcscat(m_szSpecialGrammar, L"<defaultPrefix>=Mister\r\n");
   wcscat(m_szSpecialGrammar, L"<defaultPrefix>=Missusr\n");
   wcscat(m_szSpecialGrammar, L"<defaultPrefix>=Miss\r\n");
   wcscat(m_szSpecialGrammar, L"<defaultPrefix>=Doctor\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=none of the above \"none\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=neither \"none\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=spell \"spell\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=let me spell [opt] it \"spell\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=I want to spell [opt] it \"spell\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=list [opt] the names \"listNames\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=cancel \"cancel\"\r\n");
   wcscat(m_szSpecialGrammar, L"[<Start>]\r\n<Start>=<AskName>\r\n<Start>=<Verify>\r\n");

   // since we didn't preallocate m_szSpecialGrammar, do it now
   if((m_szSpecialGrammar = (WCHAR *)malloc((wcslen(szGrammarBuffer) + 1) * sizeof(WCHAR))) == 0)
      return;
   wcscpy(m_szSpecialGrammar, szGrammarBuffer);


   WCHAR szChooseTemplate[1024];
   DWORD dwSize = 1023;
   GetValue(L"Prompts", L"ChooseTemplate", szChooseTemplate, &dwSize);
   *pszPrompt = (WCHAR *)malloc(1024);
   swprintf(*pszPrompt, szChooseTemplate, pnap1->m_szFullName, pnap2->m_szFullName);
}


void CName::GrammarForMany(WCHAR **pszPrompt) {
   WCHAR szGrammarBuffer[8192];
   int i = 1;

   m_szSpecialGrammar = szGrammarBuffer;
   wcscpy(m_szSpecialGrammar, L"[<AskName>]\r\n<AskName>=(DefaultResponses)\r\n");

   for(CLinkedListIterator<SNameAuxPerson> lli = m_llPeople.GetHead(); lli.IsValid(); lli.Next(), i++) {
      ConcatGrammarAuxData(m_szSpecialGrammar, lli.GetData(), i);
      ConcatGrammarFirstPossLast(m_szSpecialGrammar, lli.GetData(), i);
      ConcatGrammarLast(m_szSpecialGrammar, lli.GetData(), i);
   }

   wcscat(m_szSpecialGrammar, L"<defaultPrefix>=Mister\r\n");
   wcscat(m_szSpecialGrammar, L"<defaultPrefix>=Missus\r\n");
   wcscat(m_szSpecialGrammar, L"<defaultPrefix>=Miss\r\n");
   wcscat(m_szSpecialGrammar, L"<defaultPrefix>=Doctor\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=none of the above \"none\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=neither \"none\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=spell \"spell\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=let me spell [opt] it \"spell\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=I want to spell [opt] it \"spell\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=list [opt] the names \"listNames\"\r\n");
   wcscat(m_szSpecialGrammar, L"<AskName>=cancel \"cancel\"\r\n");
   wcscat(m_szSpecialGrammar, L"[<Start>]\r\n<Start>=<AskName>\r\n<Start>=<Verify>\r\n");

   if((m_szSpecialGrammar = (WCHAR *)malloc((wcslen(szGrammarBuffer) + 1) * sizeof(WCHAR))) == 0)
      return;
   wcscpy(m_szSpecialGrammar, szGrammarBuffer);

   *pszPrompt = (WCHAR *)malloc(1024);
   DWORD dwSize = 1023;
   if(m_bIsFirstPrompt)
      GetValue(L"Prompts", L"Main", *pszPrompt, &dwSize), m_bIsFirstPrompt = false;
   else
      GetValue(L"Prompts", L"BeSpecific", *pszPrompt, &dwSize);
}


void CName::ConcatGrammarFirstPossLast(WCHAR *szBuffer, SNameAuxPerson *pnap, int iPersonNum) {
   WCHAR szTemp[128];

   if(pnap->m_szFirstName == 0)
      return;

   wcscat(szBuffer, L"<AskName>=");
   if(pnap->m_szPreTitle) {
      swprintf(szTemp, L"[opt] <person%dPre> ", iPersonNum);
      wcscat(szBuffer, szTemp);
   }
   else
      wcscat(szBuffer, L"[opt] <defaultPrefix> ");
   swprintf(szTemp, L"<person%dFirst> ", iPersonNum);
   wcscat(szBuffer, szTemp);
   if(pnap->m_szLastName) {
      swprintf(szTemp, L"[opt] <person%dLast> ", iPersonNum);
      wcscat(szBuffer, szTemp);
   }
   if(pnap->m_szPostTitle) {
      swprintf(szTemp, L"[opt] <person%dPost> ", iPersonNum);
      wcscat(szBuffer, szTemp);
   }
   wcscat(szBuffer, L"\r\n");
}


void CName::ConcatGrammarPossFirstLast(WCHAR *szBuffer, SNameAuxPerson *pnap, int iPersonNum) {
   WCHAR szTemp[128];

   if(pnap->m_szLastName == 0)
      return;

   wcscat(szBuffer, L"<AskName>=");
   if(pnap->m_szPreTitle) {
      swprintf(szTemp, L"[opt] <person%dPre> ", iPersonNum);
      wcscat(szBuffer, szTemp);
   }
   else
      wcscat(szBuffer, L"[opt] <defaultPrefix> ");
   if(pnap->m_szFirstName) {
      swprintf(szTemp, L"[opt] <person%dFirst> ", iPersonNum);
      wcscat(szBuffer, szTemp);
   }
   swprintf(szTemp, L"<person%dLast> ", iPersonNum);
   wcscat(szBuffer, szTemp);
   if(pnap->m_szPostTitle) {
      swprintf(szTemp, L"[opt] <person%dPost> ", iPersonNum);
      wcscat(szBuffer, szTemp);
   }
   wcscat(szBuffer, L"\r\n");
}


void CName::ConcatGrammarLast(WCHAR *szBuffer, SNameAuxPerson *pnap, int iPersonNum) {
   WCHAR szTemp[128];

   if(pnap->m_szLastName == 0)
      return;

   wcscat(szBuffer, L"<AskName>=");
   if(pnap->m_szPreTitle) {
      swprintf(szTemp, L"[opt] <person%dPre> ", iPersonNum);
      wcscat(szBuffer, szTemp);
   }
   else
      wcscat(szBuffer, L"[opt] <defaultPrefix> ");
   swprintf(szTemp, L"<person%dLast> ", iPersonNum);
   wcscat(szBuffer, szTemp);
   if(pnap->m_szPostTitle) {
      swprintf(szTemp, L"[opt] <person%dPost> ", iPersonNum);
      wcscat(szBuffer, szTemp);
   }
   wcscat(szBuffer, L"\r\n");
}


void CName::ConcatGrammarAuxData(WCHAR *szBuffer, SNameAuxPerson *pnap, int iPersonNum) {
   WCHAR szTemp[128];

   if(pnap->m_szPreTitle) {
      swprintf(szTemp, L"<person%dPre>=%s \"PRE%sEND\"\r\n", iPersonNum, pnap->m_szPreTitle, pnap->m_szPreTitle);
      wcscat(szBuffer, szTemp);
   }
   if(pnap->m_szFirstName) {
      swprintf(szTemp, L"<person%dFirst>=%s \"FIRST%sEND\"\r\n", iPersonNum, pnap->m_szFirstName, pnap->m_szFirstName);
      wcscat(szBuffer, szTemp);
   }
   if(pnap->m_szLastName) {
      swprintf(szTemp, L"<person%dLast>=%s \"LAST%sEND\"\r\n", iPersonNum, pnap->m_szLastName, pnap->m_szLastName);
      wcscat(szBuffer, szTemp);
   }
   if(pnap->m_szPostTitle) {
      swprintf(szTemp, L"<person%dPost>=%s \"POST%sEND\"\r\n", iPersonNum, pnap->m_szPostTitle, pnap->m_szPostTitle);
      wcscat(szBuffer, szTemp);
   }
}


void CName::OnAskBack() {
   m_pQueue->Speak(L"Interrupted");
   ReadyToListenBeep();
   if(m_bVerifying)
      StartSRWhenReady(L"Verify");
   else
      StartSRWhenReady(gszStart);
}


SNameAuxPerson::SNameAuxPerson(WCHAR *szName) {
   WCHAR *pchRestOfName;

   Clear();

   if((m_szFullName = (WCHAR *)malloc((wcslen(szName) + 1) * sizeof(WCHAR))) == 0)
      return;
   wcscpy(m_szFullName, szName);

   pchRestOfName = szName;
   GetPreTitle(&pchRestOfName);
   GetFirstName(&pchRestOfName);
   GetLastName(&pchRestOfName);
   GetPostTitle(&pchRestOfName);
}


void SNameAuxPerson::GetPreTitle(WCHAR **ppchRestOfName) {
   WCHAR *pch;
   size_t len;

   if((pch = wcschr(*ppchRestOfName, L' ')) == 0)
      return; // there's only one word, and it can't be a pre-title
   if(pch == *ppchRestOfName)
      return; // it starts with a space (?!)
   if(*(pch - 1) != L'.')
      return; // it is assumed pre-titles are abbreviated and hence always end in a period
   if(pch == (*ppchRestOfName + 2))
      return; // this abbreviation is only one letter, so it's an abbreviated name

   // get a copy of this pre-title
   *pch = L'\0';
   len = wcslen(*ppchRestOfName);
   *pch = L' ';
   if((m_szPreTitle = (WCHAR *)malloc((len + 1) * sizeof(WCHAR))) == 0)
      return;
   wcsncpy(m_szPreTitle, *ppchRestOfName, len);
   m_szPreTitle[len] = L'\0';

   // advance to the first name
   *ppchRestOfName = pch + 1;
}


// NOTE: "first, post-title" doesn't work right now!!!!!

void SNameAuxPerson::GetFirstName(WCHAR **ppchRestOfName) {
   WCHAR *pch;
   size_t len;
   bool bOnlyWord;

   if((pch = wcschr(*ppchRestOfName, L' ')) == 0)
      bOnlyWord = true;
   else {
      bOnlyWord = false;
      *pch = L'\0';
   }
   len = wcslen(*ppchRestOfName);
   if(!bOnlyWord)
      *pch = L' ';

   // get a copy of this first name
   if((m_szFirstName = (WCHAR *)malloc((len + 1) * sizeof(WCHAR))) == 0)
      return;
   wcsncpy(m_szFirstName, *ppchRestOfName, len);
   m_szFirstName[len] = L'\0';

   // advance to last name, or the null character if none exists
   *ppchRestOfName = (bOnlyWord ? pch : (pch + 1));
}


void SNameAuxPerson::GetLastName(WCHAR **ppchRestOfName) {
   WCHAR *pch, chSave;
   bool bOnlyWord;
   size_t len;

   if(**ppchRestOfName == L'\0')
      return; // there are no characters left

   // figure out what sort of post-title there is
   if((pch = wcschr(*ppchRestOfName, L',')) != 0)
      bOnlyWord = false;
   else if((*ppchRestOfName)[wcslen(*ppchRestOfName) - 1] == L'.') {
      bOnlyWord = false;
      if((pch = wcsrchr(*ppchRestOfName, L' ')) == 0)
         return; // there is no last name, only a post-title
   }
   else {
      pch = *ppchRestOfName + wcslen(*ppchRestOfName);
      bOnlyWord = true;
   }

   // figure out how long the last name is
   chSave = *pch;
   *pch = L'\0';
   len = wcslen(*ppchRestOfName);
   *pch = chSave;

   // get a copy of this last name
   if((m_szLastName = (WCHAR *)malloc((len + 1) * sizeof(WCHAR))) == 0)
      return;
   wcsncpy(m_szLastName, *ppchRestOfName, len);
   m_szLastName[len] = L'\0';

   // advance to post-title, or the null character if none exists
   if(bOnlyWord)
      ; // do nothing--pch is already in the right place
   else
      while(*++pch == L' ') ;
   *ppchRestOfName = pch;
}

void SNameAuxPerson::GetPostTitle(WCHAR **ppchRestOfName) {
   size_t len;

   if(**ppchRestOfName == L'\0')
      return; // there are no characters left

   // get a copy of this post-title
   len = wcslen(*ppchRestOfName);
   if((m_szPostTitle = (WCHAR *)malloc((len + 1) * sizeof(WCHAR))) == 0)
      return;
   wcscpy(m_szPostTitle, *ppchRestOfName);

   *ppchRestOfName += len;
}


bool SNameAuxPerson::operator==(SNameAuxPerson& nap) {
   if(m_szFullName && nap.m_szFullName)
      return (wcscmp(m_szFullName, nap.m_szFullName) ? false : true);
   else {
      if(m_szPreTitle && nap.m_szPreTitle && wcscmp(m_szPreTitle, nap.m_szPreTitle))
         return false;
      if(m_szFirstName && nap.m_szFirstName && wcscmp(m_szFirstName, nap.m_szFirstName))
         return false;
      if(m_szLastName && nap.m_szLastName && wcscmp(m_szLastName, nap.m_szLastName))
         return false;
      if(m_szPostTitle && nap.m_szPostTitle && wcscmp(m_szPostTitle, nap.m_szPostTitle))
         return false;
      return true;
   }
}
