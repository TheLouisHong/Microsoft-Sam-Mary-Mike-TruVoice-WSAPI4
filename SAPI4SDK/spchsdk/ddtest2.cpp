/* Test2.cpp - Tests 100-199 */


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <dsound.h>
#include <spchwrap.h>
#include "frame.h"
#include "resource.h"

extern char gszHelloWorldGrammar[];
extern char gszHelloWorld2Grammar[];
extern char gszMultiExportGrammar[];
extern char gszWordSpotGrammar[];
extern char gszListGrammar[];
extern char gszPronGrammar[];
extern char gszAllWorkGrammar[];
extern char gszAllWork2Grammar[];
extern char gszAllWork[];
extern char gszDictationGrammar[];
extern char gszDictationChunkGrammar[];
extern char gszLimitedDomainGrammar[];
extern char gszWordOrder1Grammar[];
extern char gszWordOrder2Grammar[];

#ifdef STRICT
#pragma message("STRICT is define")
#endif


// - Warning if limited domain doesn't support iSRGramDictation
void CALLBACK Test100 (void)
{
   NEEDLIMITEDDOMAIN;
   NEEDENGLISH;

   HRESULT  hRes;
   hRes = SetMainGrammar (gszLimitedDomainGrammar);
   CHECKFAILED ("SetMainGrammar");

   if (!gpSRGramComp->m_pISRGramDictation)
      Warning ("Limited domain grammars should support ISRGramDictation");

   Passed();
}

// - Listappend - Valid name, valid words. Make sure they get recognized
void CALLBACK Test101 (void)
{
   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // append words
   PVOID pList;
   DWORD dwSize;
   pList = ConstructList (L"George Washington\0Thomas Jefferson\0",
      &dwSize);
   hRes = gpSRGramComp->ListAppend (L"lIsT1", pList, dwSize);
   CHECKFAILED ("ListAppend");

   // listen
   hRes = SpeakToSR ("call George Washington");
   CHECKFAILED ("SpeakToSR");
   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   // wait
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get phrasefinish");
      return;
   }

   // else make sure it's right response
   if (!stricmp("call george washington", gSRGramNS.m_szPhraseFinishText))
      Passed();
   else
      Failed ("Didn't hear expected response");

}

// - ListAPpend - valid name, wierd words. make sure they get recognized.
void CALLBACK Test102 (void)
{
   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // append words
   PVOID pList;
   DWORD dwSize;
   pList = ConstructList (L"1 2 3\0" L"4 5 6\0",
      &dwSize);
   hRes = gpSRGramComp->ListAppend (L"lIsT1", pList, dwSize);
   CHECKFAILED ("ListAppend");

   // listen
   hRes = SpeakToSR ("call one two three");
   CHECKFAILED ("SpeakToSR");
   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   // wait
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get phrasefinish");
      return;
   }

   // else make sure it's right response
   if (!stricmp("call 1 2 3", gSRGramNS.m_szPhraseFinishText))
      Passed();
   else
      Failed ("Didn't hear expected response");

}

// - ListAppend - invalid name
// - ListAPpend - invalid paremeters
void CALLBACK Test103 (void)
{
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // append words
   PVOID pList;
   DWORD dwSize;
   pList = ConstructList (L"George Washington\0Thomas Jefferson\0",
      &dwSize);
   hRes = gpSRGramComp->ListAppend (L"Unreal list", pList, dwSize);
   if (!hRes)
      Failed ("Appendend onto non existant list");
   hRes = gpSRGramComp->ListAppend (L"Start", pList, dwSize);
   if (!hRes)
      Failed ("Appendend onto non existant list");

   // try some invalid params
   hRes = gpSRGramComp->ListAppend (L"List1", pList, 0);
   if (!hRes)
      Failed ("Didn't detect invalid parameters");
   hRes = gpSRGramComp->ListAppend (L"List1", NULL, dwSize);
   if (!hRes)
      Failed ("Didn't detect invalid parameters");
   hRes = gpSRGramComp->ListAppend (NULL, pList, dwSize);
   if (!hRes)
      Failed ("Didn't detect invalid parameters");
   hRes = gpSRGramComp->ListAppend (L"This is a very long list name that might cause a crash at some point",
      pList, dwSize);
   if (!hRes)
      Failed ("Didn't detect invalid parameters");

   Passed();
}

// - ListGet - Make sure it works
void CALLBACK Test104 (void)
{
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // get it
   SDATA sd;
   hRes = gpSRGramComp->ListGet (L"lIsT1", &sd);
   CHECKFAILED ("ListGet");

   // should see the word gorge
   BOOL  fFound = FALSE;
   PSRWORDW pWord;
   for (pWord = (PSRWORDW) sd.pData;
      pWord < (PSRWORDW) ((PBYTE)sd.pData + sd.dwSize);
      pWord = (PSRWORDW) ((PBYTE)pWord+pWord->dwSize))
   {
      if (!wcsicmp(L"george", pWord->szWord))
         fFound = TRUE;
   }
   if (!fFound)
      Failed ("Didn't find expected word");

	if( sd.pData )
		CoTaskMemFree (sd.pData);
	sd.pData = NULL;

   Passed();

}

// - ListGet - Make sure after some appends & removal & set, get right response
void CALLBACK Test105 (void)
{
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // get it
   SDATA sd;
   hRes = gpSRGramComp->ListGet (L"lIsT1", &sd);
   CHECKFAILED ("ListGet");

   // append words
   PVOID pList;
   DWORD dwSize;
   pList = ConstructList (L"George Washington\0Mike\0Thomas Jefferson\0",
      &dwSize);
   hRes = gpSRGramComp->ListAppend (L"List1", pList, dwSize);
   CHECKFAILED ("ListAppend");

   pList = ConstructList (L"George Washington\0Mike\0Thomas Jefferson\0",
      &dwSize);
   hRes = gpSRGramComp->ListAppend (L"List1", pList, dwSize);
   CHECKFAILED ("ListAppend");

   // remove some words
   pList = ConstructList (L"George\0",
      &dwSize);
   hRes = gpSRGramComp->ListRemove (L"List1", pList, dwSize);
   CHECKFAILED ("ListRemove");

   if (sd.pData)
      CoTaskMemFree (sd.pData);
	sd.pData = NULL;
   hRes = gpSRGramComp->ListGet (L"lIsT1", &sd);
   CHECKFAILED ("ListGet");

   // should notsee the word gorge
   BOOL  fFoundGeorge = FALSE;
   BOOL  fFoundGeorgeWashington = FALSE;
   DWORD dwMikeCount = 0;
   PSRWORDW pWord;
   for (pWord = (PSRWORDW) sd.pData;
      pWord < (PSRWORDW) ((PBYTE)sd.pData + sd.dwSize);
      pWord = (PSRWORDW) ((PBYTE)pWord+pWord->dwSize))
   {
      if (!wcsicmp(L"george washington", pWord->szWord))
         fFoundGeorgeWashington = TRUE;
      if (!wcsicmp(L"george", pWord->szWord))
         fFoundGeorge = TRUE;
      if (!wcsicmp(L"mike", pWord->szWord))
         dwMikeCount++;
   }
   if (!fFoundGeorgeWashington)
      Failed ("Didn't find expected word");
   if (fFoundGeorge)
      Failed ("Found word that just removed");
   if (dwMikeCount != 1)
      Failed ("List append added word that already existed.");

	if( sd.pData )
		CoTaskMemFree (sd.pData);
	sd.pData = NULL;

   Passed();

}



// - ListGet - Invalid parameters
void CALLBACK Test106 (void)
{
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // get it
   SDATA sd;
   sd.pData = NULL;
   hRes = gpSRGramComp->ListGet (L"Not a real list", &sd);
   if (!hRes)
      Failed ("Didn't fail list get on unreal list");
   if (sd.pData)
      CoTaskMemFree (sd.pData);
	sd.pData = NULL;

   hRes = gpSRGramComp->ListGet (L"list3", &sd);
   if (hRes || (sd.pData != NULL && sd.dwSize != 0))
      Failed ("List3 was empty but didn't return an empty list");
   if (sd.pData)
      CoTaskMemFree (sd.pData);
	sd.pData = NULL;

   // invalid
   hRes = gpSRGramComp->ListGet (L"list1", NULL);
   if (!hRes)
      Failed ("Didn't fail on invalid parameter");
   hRes = gpSRGramComp->ListGet (NULL, &sd);
   if (!hRes)
      Failed ("Didn't fail on invalid parameter");


   Passed();
}



// - ListQuery - Valid list name
// - ListQuery - Invalid list name
// - ListQuery - Invalid params
void CALLBACK Test107 (void)
{
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // list query on valid list names
   BOOL  fRet;
   hRes = gpSRGramComp->ListQuery (L"lIsT1", &fRet);
   CHECKFAILED ("ListQuery");
   if (!fRet)
      Failed ("This list has words");
   hRes = gpSRGramComp->ListQuery (L"list3", &fRet);
   CHECKFAILED ("ListQuery");
   if (fRet)
      Failed ("List3 doesn't have words");

   // list query with invalid name
   hRes = gpSRGramComp->ListQuery (L"Not real", &fRet);
   if (!hRes)
      Failed ("Didn't return error on invalid list");

   // invalid parameters
   hRes = gpSRGramComp->ListQuery (L"List1", NULL);
   if (!hRes)
      Failed ("Didn't check invalid parameters");
   hRes = gpSRGramComp->ListQuery (NULL, &fRet);
   if (!hRes)
      Failed ("Didn't check invalid parameters");

   Passed();
}


// - ListRemove - Remove valid words. Make sure they're not recognized
void CALLBACK Test108 (void)
{
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // remove words
   PVOID pList;
   DWORD dwSize;
   pList = ConstructList (L"George\0",
      &dwSize);
   hRes = gpSRGramComp->ListRemove (L"List1", pList, dwSize);
   CHECKFAILED ("ListRemove");

   // speak and listen
   hRes = SpeakToSR ("call george");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish ();
   CHECKFAILED ("WaitForPhraseFinish");

   // make sure didn't get call geroge
   if (!stricmp("call george", gSRGramNS.m_szPhraseFinishText))
      Failed ("ListRemove didn't really remove the word");

   Passed();
}

// - ListRemove - Remove valid & invalid words. Make sure they're not recognized
void CALLBACK Test109 (void)
{
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // remove words
   PVOID pList;
   DWORD dwSize;
   pList = ConstructList (L"Junk word\0George\0",
      &dwSize);
   hRes = gpSRGramComp->ListRemove (L"List1", pList, dwSize);
   CHECKFAILED ("ListRemove");

   // speak and listen
   hRes = SpeakToSR ("call george");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish ();
   CHECKFAILED ("WaitForPhraseFinish");

   // make sure didn't get call geroge
   if (!stricmp("call george", gSRGramNS.m_szPhraseFinishText))
      Failed ("ListRemove didn't really remove the word");

   Passed();
}


// - ListRemove - Invalid parameters
void CALLBACK Test110 (void)
{
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   PVOID pList;
   DWORD dwSize;
   pList = ConstructList (L"George\0",
      &dwSize);
   
   // invalid params
   hRes = gpSRGramComp->ListRemove (L"List1", pList, 0);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   hRes = gpSRGramComp->ListRemove (L"List1", NULL, dwSize);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   hRes = gpSRGramComp->ListRemove (NULL, pList, dwSize);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   hRes = gpSRGramComp->ListRemove (L"Not real", pList, dwSize);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   Passed();
}


// - ListSet - Empty list
void CALLBACK Test111 (void)
{
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   PVOID pList;
   DWORD dwSize;
   pList = ConstructList (L"\0",
      &dwSize);
   
   // empty list
   hRes = gpSRGramComp->ListSet (L"List1", pList, dwSize);
   CHECKFAILED ("ListSet");

   // make sure it doesn't recognize
   hRes = SpeakToSR ("call george");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish ();
   CHECKFAILED ("WaitForPhraseFinish");

   // make sure didn't get call geroge
   if (!stricmp("call george", gSRGramNS.m_szPhraseFinishText))
      Failed ("ListSet didn't really remove the word");

   // really big list
   WCHAR szHuge[100000];
   char  szTemp[10];
   WCHAR szwTemp[10];
   WCHAR *psz;
   DWORD i;
   psz = szHuge;
   for (i = 0; i < 1000; i++) {
      wsprintf (szTemp, "%d", (int) i);
      MBTWC (szTemp, szwTemp);
      wcscpy (psz, szwTemp);
      psz += (wcslen(szwTemp)+1);
   }
   *(psz++) = 0;
   *(psz++) = 0;

   pList = ConstructList (szHuge, &dwSize);
   hRes = gpSRGramComp->ListSet (L"List1", pList, dwSize);
   CHECKFAILED ("ListSet");

   // make sure it doesn't recognize
   hRes = SpeakToSR ("call six hundred");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish ();
   CHECKFAILED ("WaitForPhraseFinish");

   // make sure didn't get call geroge
   if (stricmp("call 600", gSRGramNS.m_szPhraseFinishText))
      Warning ("ListSet didn't handle very large lists");


   Passed();
}



// - ListSet - Invalid params
void CALLBACK Test112 (void)
{
   NEEDCFG;
   NEEDENGLISH;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   PVOID pList;
   DWORD dwSize;
   pList = ConstructList (L"Fred\0",
      &dwSize);

   // invalid params
   hRes = gpSRGramComp->ListSet (L"List1", NULL, dwSize);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   hRes = gpSRGramComp->ListSet (NULL, pList, dwSize);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   hRes = gpSRGramComp->ListSet (L"Invalid list", pList, dwSize);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   Passed();
}

// - Activate CFG - Autopause on. Make sure that after get a recognition that it's no longer processing.
//	Can do this by sending in a text string with "word1 \pau=2000\ word2". Pause after
//	word1, switch the grammar so that word2 is recognized differently, and then resume.
void CALLBACK Test113 (void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak text
   hRes = SpeakToSR ("hello world. \\pau=4000\\ Call George.");
   CHECKFAILED ("SpeakToSR");

   // activate with autopause
   hRes = gpSRGramComp->Activate (NULL, TRUE);
   CHECKFAILED ("Activate");

   // set some flags so we know if paused
   gSRGramNS.m_fPaused = FALSE;

   // go
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get utterance");
      return;
   }
   // make sure it's the right phrase
   if (stricmp("hello world", gSRGramNS.m_szPhraseFinishText))
      Failed ("Didn't hear expected phrase");

   // if we' haven't heard pause yet, wait a short while for it
   if (!gSRGramNS.m_fPaused) {
      pf = WaitForEvent (1.0, &gSRGramNS.m_fPaused);
      if (pf == &gfAbortTest)
         return;
      else if (!pf) {
         Failed ("Didn't get pause notification");
         return;
      }
   }

   // load new grammar
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // activate without autopause
   hRes = gpSRGramComp->Activate (NULL, FALSE);
   CHECKFAILED ("Activate");

   // wait 30 seconds just to make sure it's paused
   pf = WaitForEvent (30.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf) {
      Failed ("Heard utterance when paused");
      return;
   }

   // resume
   hRes = gpSRMode->Resume();
   CHECKFAILED ("Resume");

   // wait for response
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get utterance");
      return;
   }
   // make sure it's the right phrase
   if (stricmp("call george", gSRGramNS.m_szPhraseFinishText))
      Failed ("Didn't hear expected phrase");

   // clear out
   ClearMainGrammar();
   ClearSRAudio();

   // else worked
   Passed();
}


// - Activate Dictaion - Autopause on. Make sure that after get a recognition that it's no longer processing.
//	Can do this by sending in a text string with "word1 \pau=2000\ word2". Pause after
//	word1, switch the grammar so that word2 is recognized differently, and then resume.
void CALLBACK Test114(void)
{
   NEEDCFG;
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak text
   hRes = SpeakToSR ("hello world. \\pau=4000\\ Call George.");
   CHECKFAILED ("SpeakToSR");

   // activate with autopause
   hRes = gpSRGramComp->Activate (NULL, TRUE);
   CHECKFAILED ("Activate");

   // set some flags so we know if paused
   gSRGramNS.m_fPaused = FALSE;

   // go
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get utterance");
      return;
   }

   // if we' haven't heard pause yet, wait a short while for it
   if (!gSRGramNS.m_fPaused) {
      pf = WaitForEvent (1.0, &gSRGramNS.m_fPaused);
      if (pf == &gfAbortTest)
         return;
      else if (!pf) {
         Failed ("Didn't get pause notification");
         return;
      }
   }

   // load new grammar
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // activate without autopause
   hRes = gpSRGramComp->Activate (NULL, FALSE);
   CHECKFAILED ("Activate");

   // wait 30 seconds just to make sure it's paused
   pf = WaitForEvent (30.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf) {
      Failed ("Heard utterance when paused");
      return;
   }

   // resume
   hRes = gpSRMode->Resume();
   CHECKFAILED ("Resume");

   // wait for response
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get utterance");
      return;
   }
   // make sure it's the right phrase
   if (stricmp("call george", gSRGramNS.m_szPhraseFinishText))
      Failed ("Didn't hear expected phrase");

   // clear out
   ClearMainGrammar();
   ClearSRAudio();

   // else worked
   Passed();
}


// - Activate CFG - Invalid rule
void CALLBACK Test115(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = gpSRGramComp->Activate(NULL, L"Junk Rule");
   if (!hRes)
      Failed ("Didn't check for invalid parameter");

   Passed();
}

// - Activate dictation - Invalid rule
void CALLBACK Test116(void)
{
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = gpSRGramComp->Activate(NULL, L"Junk Rule");
   if (!hRes)
      Failed ("Didn't check for invalid parameter");

   Passed();
}



// - Archive CFG - Without archiving resutls. Reload and see that works the same.
void CALLBACK Test117(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   DWORD dwNeeded;
   hRes = gpSRGramComp->Archive(FALSE, NULL, NULL, &dwNeeded);
   if (hRes == SRERR_NOTSUPPORTED) {
      Warning ("ISRGramComp::Archive not supported");
      return;
   }
   if (hRes != SRERR_NOTENOUGHDATA)
      Failed ("Archive should return Not Enough Data");

   // allocate the memory
   PVOID pMem;
   DWORD dwSize = dwNeeded;
   pMem = malloc (dwNeeded);
   hRes = gpSRGramComp->Archive(FALSE, pMem, dwSize, &dwNeeded);
   CHECKFAILED ("Archive");

   // free this grammar
   ClearMainGrammar();

   // try to load in the archive data
   PCSRGram pSRGram;
   pSRGram = gpSRMode->GrammarLoad (SRGRMFMT_CFGNATIVE,
      pMem, dwSize, &gSRGramNS);
   free (pMem);
   if (!pSRGram) {
      Failed ("Couldn't reload own archived grammar");
      return;
   }

   // make sure it works
   hRes = SpeakToSR ("hello world");
   CHECKFAILED ("SpeakToSR");
   hRes = pSRGram->Activate();
   CHECKFAILED ("Activate");

   // wait for phrase finish
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get phrasefinish");
      return;
   }

   if (stricmp("hello world", gSRGramNS.m_szPhraseFinishText))
      Failed ("Didn't recognize prompt");

   delete pSRGram;

   Passed();
}


// - Archive CFG - With archiving resutls. Reload and see that works the same.
void CALLBACK Test118(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // recognize something to get resutls object
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish ();
   CHECKFAILED ("WaitForPhraseFinish");
   if (!gpSRResult) {
      NotApplicable();
      return;
   }

   DWORD dwNeeded;
   hRes = gpSRGramComp->Archive(TRUE, NULL, NULL, &dwNeeded);
   if (hRes == SRERR_NOTSUPPORTED) {
      Warning ("ISRGramComp::Archive not supported");
      return;
   }
   if (hRes != SRERR_NOTENOUGHDATA)
      Failed ("Archive should return Not Enough Data");

   // allocate the memory
   PVOID pMem;
   DWORD dwSize = dwNeeded;
   pMem = malloc (dwNeeded);
   hRes = gpSRGramComp->Archive(TRUE, pMem, dwSize, &dwNeeded);
   CHECKFAILED ("Archive");

   // free this grammar
   ClearMainGrammar();

   // try to load in the archive data
   gSRGramNS.m_fUnArchive = FALSE;
   PCSRGram pSRGram;
   pSRGram = gpSRMode->GrammarLoad (SRGRMFMT_CFGNATIVE,
      pMem, dwSize, &gSRGramNS);
   free (pMem);
   if (!pSRGram) {
      Failed ("Couldn't reload own archived grammar");
      return;
   }
   // wait a short while to see if unarchive works
   WaitForEvent (1.0);
   if (!gSRGramNS.m_fUnArchive)
      Failed ("Didn't get callback for results object");

   // make sure it works
   hRes = SpeakToSR ("hello world");
   CHECKFAILED ("SpeakToSR");
   hRes = pSRGram->Activate();
   CHECKFAILED ("Activate");

   // wait for phrase finish
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get phrasefinish");
      return;
   }

   if (stricmp("hello world", gSRGramNS.m_szPhraseFinishText))
      Failed ("Didn't recognize prompt");

   delete pSRGram;

   Passed();
}


// - Archive CFG - Invalid params
void CALLBACK Test119(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   DWORD dwNeeded;
   hRes = gpSRGramComp->Archive(FALSE, NULL, NULL, NULL);
   if (!hRes)
      Failed ("Not checking invalid params");
   hRes = gpSRGramComp->Archive(FALSE, NULL, rand(), &dwNeeded);
   if (!hRes)
      Failed ("Not checking invalid params");
   hRes = gpSRGramComp->Archive(FALSE, &dwNeeded, NULL, &dwNeeded);
   if (!hRes)
      Failed ("Not checking invalid params");

   Passed();
}


// - Archive Dictation - Without archiving resutls. Reload and see that works the same.
void CALLBACK Test120(void)
{
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

   DWORD dwNeeded;
   hRes = gpSRGramComp->Archive(FALSE, NULL, NULL, &dwNeeded);
   if (hRes == SRERR_NOTSUPPORTED) {
      Warning ("ISRGramComp::Archive not supported");
      return;
   }
   if (hRes != SRERR_NOTENOUGHDATA)
      Failed ("Archive should return Not Enough Data");

   // allocate the memory
   PVOID pMem;
   DWORD dwSize = dwNeeded;
   pMem = malloc (dwNeeded);
   hRes = gpSRGramComp->Archive(FALSE, pMem, dwSize, &dwNeeded);
   CHECKFAILED ("Archive");

   // free this grammar
   ClearMainGrammar();

   // try to load in the archive data
   PCSRGram pSRGram;
   pSRGram = gpSRMode->GrammarLoad (SRGRMFMT_DICTATIONNATIVE,
      pMem, dwSize, &gSRGramNS);
   free (pMem);
   if (!pSRGram) {
      Failed ("Couldn't reload own archived grammar");
      return;
   }

   // make sure it works
   hRes = SpeakToSR ("hello world");
   CHECKFAILED ("SpeakToSR");
   hRes = pSRGram->Activate();
   CHECKFAILED ("Activate");

   // wait for phrase finish
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get phrasefinish");
      return;
   }

   delete pSRGram;

   Passed();
}


// - Archive Dictation - With archiving resutls. Reload and see that works the same.
void CALLBACK Test121(void)
{
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

   // recognize something to get results object
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish ();
   CHECKFAILED ("WaitForPhraseFinish");
   if (!gpSRResult) {
      NotApplicable();
      return;
   }

   DWORD dwNeeded;
   hRes = gpSRGramComp->Archive(TRUE, NULL, NULL, &dwNeeded);
   if (hRes == SRERR_NOTSUPPORTED) {
      Warning ("ISRGramComp::Archive not supported");
      return;
   }
   if (hRes != SRERR_NOTENOUGHDATA)
      Failed ("Archive should return Not Enough Data");

   // allocate the memory
   PVOID pMem;
   DWORD dwSize = dwNeeded;
   pMem = malloc (dwNeeded);
   hRes = gpSRGramComp->Archive(TRUE, pMem, dwSize, &dwNeeded);
   CHECKFAILED ("Archive");

   // free this grammar
   ClearMainGrammar();

   // try to load in the archive data
   gSRGramNS.m_fUnArchive = FALSE;
   PCSRGram pSRGram;
   pSRGram = gpSRMode->GrammarLoad (SRGRMFMT_DICTATIONNATIVE,
      pMem, dwSize, &gSRGramNS);
   free (pMem);
   if (!pSRGram) {
      Failed ("Couldn't reload own archived grammar");
      return;
   }
   // wait a short while to see if unarchive works
   WaitForEvent (1.0);
   if (!gSRGramNS.m_fUnArchive)
      Failed ("Didn't get callback for results object");

   // make sure it works
   hRes = SpeakToSR ("hello world");
   CHECKFAILED ("SpeakToSR");
   hRes = pSRGram->Activate();
   CHECKFAILED ("Activate");

   // wait for phrase finish
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get phrasefinish");
      return;
   }

   delete pSRGram;

   Passed();
}


// - Archive DICTATION - Invalid params
void CALLBACK Test122(void)
{
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

   DWORD dwNeeded;
   hRes = gpSRGramComp->Archive(FALSE, NULL, NULL, NULL);
   if (!hRes)
      Failed ("Not checking invalid params");
   hRes = gpSRGramComp->Archive(FALSE, NULL, rand(), &dwNeeded);
   if (!hRes)
      Failed ("Not checking invalid params");
   hRes = gpSRGramComp->Archive(FALSE, &dwNeeded, NULL, &dwNeeded);
   if (!hRes)
      Failed ("Not checking invalid params");

   Passed();
}



// - BookMark - Set for future point and see if gets hit
void CALLBACK Test123(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // get the current position & pause it 1/2 second from now
   hRes = gpSRGramComp->BookMark (
      gpSRMode->PosnGet() + gpWFEXSR->nAvgBytesPerSec / 2,
      42);
   CHECKFAILED ("BookMark");

   // speak to sr
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // set some variables so we know if boookmark hit
   gSRGramNS.m_fBookMark = gSRGramNS.m_fPaused = FALSE;

   // turn on listening
   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   // wait for 20 seconds listening for prhase start
   BOOL *pf;
   pf = WaitForEvent (20.0, &gSRGramNS.m_fPhraseStart);
   if (pf == &gfAbortTest)
      return;
   else if (pf)
      Failed ("Heard utterance even though was supposed to be paused");

   // make sure the paused & bookmakr got hit
   if (!gSRGramNS.m_fPaused)
      Failed ("Didn't get paused");
   if (!gSRGramNS.m_fBookMark)
      Failed ("Didn't get bookmark");
   if (gSRGramNS.m_dwBookMarkMark != 42)
      Failed ("Didn't return right bookmark ID");

   // else it worked, resume
   hRes = gpSRMode->Resume();
   CHECKFAILED ("Resume");

   // make sure we hear
   pf = WaitForEvent (20.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf)
      Failed ("Didn't get phrase finish");

   ClearSRAudio();
   ClearMainGrammar();

   Passed();
}

// - BookMark - Set for future point and see if gets hit
void CALLBACK Test124(void)
{
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

   // get the current position & pause it 1/2 second from now
   hRes = gpSRGramComp->BookMark (
      gpSRMode->PosnGet() + gpWFEXSR->nAvgBytesPerSec / 2,
      42);
   CHECKFAILED ("BookMark");

   // speak to sr
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // set some variables so we know if boookmark hit
   gSRGramNS.m_fBookMark = gSRGramNS.m_fPaused = FALSE;

   // turn on listening
   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   // wait for 20 seconds listening for prhase start
   BOOL *pf;
   pf = WaitForEvent (20.0, &gSRGramNS.m_fPhraseStart);
   if (pf == &gfAbortTest)
      return;
   else if (pf)
      Failed ("Heard utterance even though was supposed to be paused");

   // make sure the paused & bookmakr got hit
   if (!gSRGramNS.m_fPaused)
      Failed ("Didn't get paused");
   if (!gSRGramNS.m_fBookMark)
      Failed ("Didn't get bookmark");
   if (gSRGramNS.m_dwBookMarkMark != 42)
      Failed ("Didn't return right bookmark ID");

   // else it worked, resume
   hRes = gpSRMode->Resume();
   CHECKFAILED ("Resume");

   // make sure we hear
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf)
      Failed ("Didn't get phrase finish");

   ClearSRAudio();
   ClearMainGrammar();

   Passed();
}


// - BOokMark - Invalid params
void CALLBACK Test125(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;
   
   // make sure we've used some audio
   SpeakAndListen ("Hello world", gszHelloWorldGrammar);

   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // set a bookamrk before the current position
   hRes = gpSRGramComp->BookMark (
      gpSRMode->PosnGet() - gpWFEXSR->nAvgBytesPerSec / 2,
      42);
   if (!hRes)
      Failed ("Bookmark didn't detect was being called before current time");

   ClearMainGrammar();

   Passed();
}


// - BookMark - set for the future, but kill the grammar
void CALLBACK Test126(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // get the current position & pause it 1/2 second from now
   hRes = gpSRGramComp->BookMark (
      gpSRMode->PosnGet() + gpWFEXSR->nAvgBytesPerSec / 2,
      42);
   CHECKFAILED ("BookMark");
   ClearMainGrammar();

   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak to sr
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // set some variables so we know if boookmark hit
   gSRGramNS.m_fBookMark = gSRGramNS.m_fPaused = FALSE;

   hRes = WaitForPhraseFinish ();
   CHECKFAILED ("WaitForPhraseFinish");

   if (gSRGramNS.m_fBookMark || gSRGramNS.m_fPaused)
      Failed ("Bookmark went off even though cleared the grammar");

   ClearSRAudio();
   ClearMainGrammar();

   Passed();
}



// - Deactivate - Rule that's not activated
// - Deactivate - Invalid rule
void CALLBACK Test127(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // not-active rule
   hRes = gpSRGramComp->Deactivate();
   if (!hRes)
      Failed ("Didn't complain that grammar wasn't active");
   hRes = gpSRGramComp->Deactivate(L"Start");
   if (!hRes)
      Failed ("Didn't complain that grammar wasn't active");

   // non-existant rule
   hRes = gpSRGramComp->Deactivate(L"Junk rule");
   if (!hRes)
      Failed ("Didn't complain that rule didn't exist");

   Passed();
}


// - TrainPhrase - Valid params.
// - TrainPhrase - Invalid params
void CALLBACK Test128(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");
   SDATA s;

   // make sure error's out on invalid param
   hRes = gpSRGramComp->TrainPhrase (SRTQEX_RECOMMENDED, NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   hRes = gpSRGramComp->TrainPhrase ((DWORD) 42, NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   // see if it's supported
   hRes = gpSRGramComp->TrainPhrase (SRTQEX_RECOMMENDED, &s);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable();
      return;
   }
   CHECKFAILED ("TrainPhrase");
   if (s.pData)
      CoTaskMemFree (s.pData);
	s.pData = NULL;

   // try required
   hRes = gpSRGramComp->TrainPhrase (SRTQEX_REQUIRED, &s);
   CHECKFAILED ("TrainPhrase");


   Passed();
}



// - TrainQuery - See what it says
void CALLBACK Test129(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // invalid
   hRes = gpSRGramComp->TrainQuery (NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   // see if works
   DWORD dw;
   hRes = gpSRGramComp->TrainQuery (&dw);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable();
      return;
   }
   CHECKFAILED ("TrainQuery");

#define TQ_ALL    (SRGRAMQ_DIALOG | SRGRAMQ_GENERALTRAIN | SRGRAMQ_PHRASE)
   if (dw & ~TQ_ALL)
      Warning ("Bits set for unknown types of training");

   Passed();

}

// - Context - set prior to something that will obviously cause the right "to" "too" or "two"
//	to get recognized, send an utterance in, and see if it's right.
// - Context - set post to something like "vegas" or "angeles" and see that "los" or "las"
//	is correctly recognized.
void CALLBACK Test130(void)
{
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = gpSRGramComp->Context (L"send mail", NULL);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable ("Context not supported");
      return;
   }
   CHECKFAILED ("Context");

   // speak two
   hRes = SpeakToSR ("two");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   if (stricmp("to", gSRGramNS.m_szPhraseFinishText))
      Information ("Left context seems to be ignored");

   // try a different context
   hRes = gpSRGramComp->Context (L"eight nine", NULL);
   CHECKFAILED ("Context");
   hRes = SpeakToSR ("two");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   if (stricmp("two", gSRGramNS.m_szPhraseFinishText))
      Information ("Left context seems to be ignored");

   // back to mail
   hRes = gpSRGramComp->Context (L"send mail", NULL);
   CHECKFAILED ("Context");
   hRes = SpeakToSR ("two");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   if (stricmp("to", gSRGramNS.m_szPhraseFinishText))
      Information ("Left context seems to be ignored");

   // try a different context
   hRes = gpSRGramComp->Context (NULL, L"vegas");
   CHECKFAILED ("Context");
   hRes = SpeakToSR ("loss");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   if (stricmp("las", gSRGramNS.m_szPhraseFinishText))
      Information ("Right context seems to be ignored");

   // try a different context
   hRes = gpSRGramComp->Context (NULL, L"angeles");
   CHECKFAILED ("Context");
   hRes = SpeakToSR ("loss");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   if (stricmp("los", gSRGramNS.m_szPhraseFinishText))
      Information ("Right context seems to be ignored");

   // try a different context
   hRes = gpSRGramComp->Context (L". I want to gamble in", L"vegas");
   CHECKFAILED ("Context");
   hRes = SpeakToSR ("loss");
   CHECKFAILED ("SpeakToSR");
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   if (stricmp("las", gSRGramNS.m_szPhraseFinishText))
      Information ("Right context seems to be ignored");

   // done
   ClearMainGrammar();
   ClearSRAudio();

   Passed();
}


// Context - invalid parameters
void CALLBACK Test131(void)
{
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = gpSRGramComp->Context (NULL, NULL);
   if (!hRes)
      Warning ("Didn't fail for invalid parameters");

   Passed();
}


// - Hint - Valid param
// - Hint - Invalid param
void CALLBACK Test132(void)
{
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = gpSRGramComp->Hint (L"dAte");
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable();
      return;
   }
   CHECKFAILED("Hint");

   hRes = gpSRGramComp->Hint (NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   Passed();
}


// - Words - Valid param
// - Words - Invalid param
void CALLBACK Test133(void)
{
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = gpSRGramComp->Words (L"favorite FBI 1");
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable();
      return;
   }
   CHECKFAILED("Words");

   hRes = gpSRGramComp->Words (NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   Passed();
}


// - Make sure than can recognize yes/no
void CALLBACK Test134(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   char  szYesNoGrammar[] =
      "[<Start>]\n"
      "<Start>=yes\n"
      "<Start>=no\n";

   char  *psz;
   psz = SpeakAndListen ("Yes", szYesNoGrammar);
   if (!psz || stricmp(psz, "yes"))
      Failed ("Didn't recognize yes");
   psz = SpeakAndListen ("no", szYesNoGrammar);
   if (!psz || stricmp(psz, "no"))
      Failed ("Didn't recognize no");

   Passed();
}



// - DeteriorationGet/Set - Set value and make sure can re-read
void CALLBACK Test135(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   HRESULT  hRes;
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   DWORD dwMem, dwTime, dwNum;
   hRes = gpSRGramComp->DeteriorationGet (&dwMem, &dwTime, &dwNum);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable();
      return;
   }
   CHECKFAILED ("DeteriorationGet");


   // try some invalid parameters
   hRes = gpSRGramComp->DeteriorationGet (NULL, NULL, NULL);
   if (!hRes)
      Failed ("DeteriorationGet didn't detect invalid parameters");

   // different value
   hRes = gpSRGramComp->DeteriorationSet (1000, 100, 10);
   CHECKFAILED ("DeteriorationSet");

   // verify
   DWORD    dwMem2, dwTime2, dwNum2;
   hRes = gpSRGramComp->DeteriorationGet (&dwMem2, &dwTime2, &dwNum2);
   CHECKFAILED ("DeteriorationGet");
   if ((1000 != dwMem2) || (100 != dwTime2) || (10 != dwNum2))
      Failed ("Couldn't reload same values");

   // restore the deterioration
   hRes = gpSRGramComp->DeteriorationSet (dwMem, dwTime, dwNum);
   CHECKFAILED ("DeteriorationSet");

   Passed();
}


// - DeteriorationGet/Set - See if value settings work
void CALLBACK Test136(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   HRESULT  hRes;
   hRes = SetMainGrammar (gszAllWork2Grammar);
   CHECKFAILED ("SetMainGrammar");

   DWORD dwMem, dwTime, dwNum;
   hRes = gpSRGramComp->DeteriorationGet (&dwMem, &dwTime, &dwNum);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable();
      return;
   }
   CHECKFAILED ("DeteriorationGet");

#define  MAXRESULT   50
   PCSRResult  apRes[MAXRESULT];
   DWORD       dwNumRes = 0;

   DWORD    dwMode, dwLock;

   // lock or don't lock
   for (dwLock = 0; dwLock < 2; dwLock++)
   // loop through 3 modes, one testing memory, one time, and one number
   for (dwMode = 0; dwMode < 3; dwMode++) {
      // print out information
      Information ();
      Information ("*** Test results deterioration ***");
      Information (dwLock ? "Locked (Shouldn't deteriorate)" : "Unlocked (Should deteriorate)");
      if (dwMode == 0)
         Information ("Deteriorate when low memory");
      else if (dwMode == 1)
         Information ("Deteriorate over time");
      else
         Information ("Deteriorate when enough objects");

      // init some variables
      dwNumRes = 0;

      // set the values
      DWORD dwNewMem, dwNewTime, dwNewNum;
      dwNewMem = (dwMode == 0) ? (rand()%100000) : 0x0fffffff;
      dwNewTime = (dwMode == 1) ? (rand()%100) : 0x0fffffff;
      dwNewNum = (dwMode == 2) ? (rand()%25) : 0x0fffffff;
      hRes = gpSRGramComp->DeteriorationSet (dwNewMem, dwNewTime, dwNewNum);
      CHECKFAILED ("DeteriorationSet");

      DWORD dwFirstMem, dwFirstTime;
      DWORD dwTotalMem = 0;

      // loop through the results objects and get
      memset (apRes, 0, sizeof(apRes));
      for (dwNumRes = 0; dwNumRes < MAXRESULT; dwNumRes++) {
         char  szTemp[128];
         wsprintf (szTemp, "Result Object #%d", (int) dwNumRes);
         Information (szTemp);

         // get a results object
         apRes[dwNumRes] = GetResultsObject();
         if (!apRes[dwNumRes]) {
            Failed ("Can't get a results object");
            break;
         }

         // if this is the first one, then get some important statistics
         DWORD dwTemp;
         if (!dwNumRes) {
            hRes = apRes[0]->Get (&dwTemp, &dwFirstMem);
            if (hRes) {
               Failed ("MemoryGet", hRes);
               break;
            }
            dwTotalMem += dwFirstMem;
            dwFirstTime = GetTickCount();

            if (dwLock) {
               hRes = apRes[0]->LockSet(TRUE);
               if (hRes) {
                  Failed ("Lock", hRes);
                  break;
               }
            }
            continue;
         }

         // sum up the memory
         DWORD dwTemp2;
         hRes = apRes[dwNumRes]->Get (&dwTemp2, &dwTemp);
         if (hRes) {
            Failed ("MemoryGet", hRes);
            break;
         }
         dwTotalMem += dwTemp;

         // else, do a test to see what the current values are like
         // for the first object
         DWORD dwCurMem, dwCurTime;
         hRes = apRes[0]->Get (&dwTemp2, &dwCurMem);
         if (hRes) {
            Failed ("MemoryGet", hRes);
            break;
         }
         dwCurTime = GetTickCount();

         // see if we've changed
         if (dwCurMem != dwFirstMem) {
            Information ("Object deteriorated");

            // it changed
            // were we supposed to? leave some slack
            if (dwLock) {
               Failed ("Object deteriorated even though locked");
               break;
            }
            if ((dwTotalMem * 5 / 4) >= dwNewMem) {
               Information ("Properly deteriorated");
               Passed ();
               break;
            }
            else if (((dwCurTime - dwFirstTime) / 1000 + 30) >= dwNewTime) {
               Information ("Properly deteriorated");
               Passed ();
               break;
            }
            else if ((dwNumRes + 5) >= dwNewNum) {
               Information ("Properly deteriorated");
               Passed ();
               break;
            }

            // else, if it got here, it changed for no reason
            Failed ("Object deteriorated for no good reason");
            break;
         }
         else {
            // it's the same
            // were we supposed to? cut it some slack

            if (
               ((dwTotalMem * 4 / 5) < (dwNewMem+500000)) &&
               (((dwCurTime - dwFirstTime) / 1000) < (dwNewTime + 120)) &&
               (dwNumRes < (dwNewNum + 5)) )
               continue;

            if (dwLock) {
               Information ("Object didn't deteriorate, but it was locked. This is good");
               Passed();
               break;
            }

            // if we got here, it didn't free as soon as it was supposed to
            Failed ("Object wasn't deteriorated when it should have");
            break;
         }

      }

      // if stop, free the results objects
      for (dwNumRes = 0; dwNumRes < MAXRESULT; dwNumRes++)
         if (apRes[dwNumRes])
            delete apRes[dwNumRes];

   }


   // restore the deterioration
   hRes = gpSRGramComp->DeteriorationSet (dwMem, dwTime, dwNum);
   CHECKFAILED ("DeteriorationSet");

   Passed();
}



// - Hold onto results object & free engine/grammar and make sure doesn't crash
void CALLBACK Test137(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   PCSRResult  pRes;
   pRes = GetResultsObject (NULL, gszAllWork2Grammar);
   if (!pRes) {
      NotApplicable();
      return;
   }

   // free everything - make sure to get rid of hack to release results object
   LPUNKNOWN   pUnk;
   pUnk = gpSRMode->m_pUnkAudio;
   gpSRMode->m_pUnkAudio = NULL;
   DestroySREngine();

   // make sure results object doesn't crash
   GUID gID;
   HRESULT hRes;
   hRes = pRes->Identify(&gID);
   if (hRes)
      Failed ("Identify failed");

   // release the results object, and then audio
   delete pRes;
   while (pUnk->Release());

   // create a new engine
   CreateSREngine();

   // Passes if it hasn't crashed
   Passed();
}



// - Hold onto grammar object & free engine/results and make sure doesn't crash
void CALLBACK Test138(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   HRESULT hRes;
   hRes = SetMainGrammar (gszAllWork2Grammar);
   CHECKFAILED ("SetMainGrammar");
   PCSRGramComp pGram;
   pGram = gpSRGramComp;
   gpSRGramComp = NULL;

   // free everything - make sure to get rid of hack to release results object
   LPUNKNOWN   pUnk;
   pUnk = gpSRMode->m_pUnkAudio;
   gpSRMode->m_pUnkAudio = NULL;
   DestroySREngine();

   // make sure results object doesn't crash
   hRes = pGram->Deactivate();

   // release the results object, and then audio
   delete pGram;
   while (pUnk->Release());

   // create a new engine
   CreateSREngine();

   // Passes if it hasn't crashed
   Passed();
}



// - Cause SR not to like wave format & fail on create.  Also test ref count on audio obect.
void CALLBACK Test139 (void)
{
   HRESULT hRes;

   NEEDAUDIO;

   // create
   CSRMode SRMode;
   PIAUDIOFILE pIAudioFileSR;
   
   // create the audio source from wave
	hRes = CoCreateInstance(CLSID_AudioSourceFile, 
		                     NULL, CLSCTX_INPROC_SERVER, 
		                     IID_IAudioFile, 
		                     (PVOID *) &pIAudioFileSR); 
	if (hRes) 
      return;
   gSRWANS.m_qwPosn = 0;
	pIAudioFileSR -> Register(&gSRWANS);
   pIAudioFileSR->RealTimeSet(0x100);

   // Wave format that won't work
   WAVEFORMATEX   wfex;
   wfex.wFormatTag = 638;

   PIAUDIO  pAudio;
   pIAudioFileSR->QueryInterface (IID_IAudio, (PVOID*) &pAudio); 
   if (pAudio) {
      SDATA s;
      s.pData = &wfex;
      s.dwSize = sizeof(wfex);
      pAudio->WaveFormatSet (s);
      pAudio->Release();
   }

   // init the engine
   hRes = SRMode.InitAudioSourceObject(pIAudioFileSR);
   if (hRes) {
      if(pIAudioFileSR->Release() != 0)
	      Failed("SR Engine left audio object addrefed on an error");
      return;
   }

   // get the ref count
   DWORD dwCount;
   dwCount= pIAudioFileSR->AddRef();

   hRes = SRMode.Init (gSRModeInfo.gModeID);
   if (!hRes)
      Failed("SR Engine didn't test the wave device");

   // Make sure the count's right
   // This release is for the explict call to AddRef()
   if (pIAudioFileSR->Release() != (dwCount-1))
      Failed("SR engine didn't release the audio object");

   // This release is for the CoCreateInstance 
   if (pIAudioFileSR->Release() != 1) 
	   Failed("Audio Object leak.");
   Passed();

   // The CSRMode object SRMode will be destroyed when we exit from
   // this function.  The destructor will call Release for the call
   // to InitAudioSourceObject().
}


// - See if SR can handle absolute silence
void CALLBACK Test140 (void)
{
   HRESULT hRes;

   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak
   hRes = SpeakToSR ("hello world", TRUE, FALSE, FALSE);
   CHECKFAILED ("SpeakToSR");

   // listen
   hRes = WaitForPhraseFinish();
   if (hRes)
      Warning ("Engine didn't cope with absolute silence");

   ClearMainGrammar();
   ClearSRAudio();

   Passed();

}


// - PhraseFinish - Make sure time stamps are reasonable
void CALLBACK Test141 (void)
{
   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

   HRESULT  hRes;
   ClearSRAudio();
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // get the current position of audio (before SR starts litening)
   QWORD qwStart;
   qwStart = gpSRMode->PosnGet();

   // start listening
   hRes = gpSRGramComp->Activate();
   CHECKFAILED("Activate");

   // wait until all the audio is exhausted - just wait 60 seconds
   BOOL  *pf;
   pf = WaitForEvent (60.0);
   if (pf == &gfAbortTest)
      return;

   ClearMainGrammar();
   ClearSRAudio();

   // get the position now
   QWORD qwEnd;
   qwEnd = gpSRMode->PosnGet();

   // Check out what PhraseStart & PhraseFinish claimed.
   // Notifications should have been sent as follows:
   //    UtterenceBegin(UttBeginTS)
   //    PhraseStart(PhStTS)
   //    UtterenceEnd(UttEndTSStart, UttEndTSEnd)
   //    PhraseFinish(PhFiTSBegin, PhFiTSEnd)
   //
   // The following relationships should hold:
   //    UttBeginTS, PhStTS, and PhFiTSBegin should be all happen around
   //    the same time.
   //
   //    UttBeginTS should equal UttEndTSStart.
   //
   //    UttEndTSEnd and PhFiTSEnd should happen around the same time.
   int nAllowedDiff;

   // For now we assume that 1/2 second is close enough
   nAllowedDiff = (int) (gpWFEXSR->nAvgBytesPerSec / 2);

   // Validate that UtterenceBegin and PhraseStart happened around the same time
   if (abs((int)(gSRNS.m_qwUtteranceBeginTime - gSRGramNS.m_qwPhraseStartTime)) > nAllowedDiff)
      Failed ("Time of UtteranceBegin not close enough to PhraseStart time");

   // Validate that the PhraseStart time and the start time specified by PhraseFinish
   // happened around the same time.
   if (abs((int)(gSRGramNS.m_qwPhraseStartTime - gSRGramNS.m_qwPhraseFinishStart)) > nAllowedDiff)
      Failed ("Time of PhraseStart not close enough to PhraseFinish start time");
   
   // Validate that the UtterenceBegin time and the start time specifed by UtterenceEnd
   // are equal.
   if (gSRNS.m_qwUtteranceBeginTime != gSRNS.m_qwUtteranceEndStart)
      Failed ("Time of UtterenceBegin not close enough to UtterenceEnd start time");

   // Validate that the end time specified by UtterenceEnd is close to the end time
   // specified by PhraseFinish.
   if (abs((int)(gSRNS.m_qwUtteranceEndEnd - gSRGramNS.m_qwPhraseFinishEnd)) > nAllowedDiff)
      Failed ("Time of UtterenceEnd end time not close enough to PhraseFinish end time");

   // make sure that the start & end times are approximately correct
   // we know when the utterance approximately started/stopped, because
   // we know the extent of the audio, and we know how much silence is
   // at the beginning end
   DWORD dwError;
   qwStart += gpWFEXSR->nAvgBytesPerSec * 1;
   qwEnd -= gpWFEXSR->nAvgBytesPerSec * 4;
   dwError = gpWFEXSR->nAvgBytesPerSec / 2;

   if ((gSRGramNS.m_qwPhraseFinishStart < (qwStart - dwError)) ||
      (gSRGramNS.m_qwPhraseFinishStart > (qwStart + dwError)) )
      Failed ("Start time is off more than 1/2 second from real start time");
   if ((gSRGramNS.m_qwPhraseFinishEnd < (qwEnd - dwError)) ||
      (gSRGramNS.m_qwPhraseFinishEnd > (qwEnd + dwError)) )
      Failed ("End time is off more than 1/2 second from real end time");

   // done
   Passed();
}

// - PhraseHypothesis - Make sure called on recognition
// - PhraseHypothesis - Make sure time stamps are reasonable
// - PhraseHypothesis - Make sure flags are potentially valid, and there are no invalid flags.
// - only Warnings are reported as this function is optional
void CALLBACK Test142 (void)
{
   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

   HRESULT  hRes;
   ClearSRAudio();
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // start listening
   hRes = gpSRGramComp->Activate();
   CHECKFAILED("Activate");

   BOOL  fHypothesis = FALSE;

   // clear out the old hypothesis
   if (gpSRResult)
      delete gpSRResult;
   gpSRResult = NULL;

   // get ready to back up a hypothesis
   PCSRResult  pResHypothesis = NULL;
   PCSRResult  pResFinish = NULL;
   BOOL fSeenPhraseFinish = FALSE;

   gSRGramNS.m_fPhraseFinish = FALSE;
   gSRGramNS.m_fPhraseStart = FALSE;

   // For now we assume that 1/2 second is close enough for timestamps
   int nAllowedDiff;
   nAllowedDiff = (int) (gpWFEXSR->nAvgBytesPerSec / 2);

   // wait until all the audio is exhausted - just wait 60 seconds
   while (TRUE) {
      BOOL  *pf;
      pf = WaitForEvent (30.0, &gSRGramNS.m_fPhraseHypothesis);
      if (pf == &gfAbortTest)
         return;
      else if (!pf)
         break;

      fHypothesis = TRUE;

      // Check and see if it's a PhraseFinish
      if (gSRGramNS.m_fPhraseFinish) {
         // Should have received a PhraseStart
         if (!gSRGramNS.m_fPhraseStart)
            Warning ("Got a PhraseFinish but no PhraseStart");

         // This should be the only PhraseFinish we receive
         if (!fSeenPhraseFinish) {
            // Flag that we have received a PhraseFinish and save
            // the Results Object pointer for later use.
            fSeenPhraseFinish = TRUE;
            pResFinish = gpSRResult;
            gpSRResult = NULL;
         }
         else {
            // We have received a PhraseHypothesis after a PhraseFinish
            Warning ("Got a PhraseHypothesis following a PhraseFinish");
            delete gpSRResult;
            gpSRResult = NULL;
         }

         // Loop and wait for next event or a timeout.  We expect a timeout
         continue;
      }

      // Should always be TRUE since this is the event we are waiting for
      if (!gSRGramNS.m_fPhraseHypothesis)
         Warning ("Srfund:  Internal event wait error");

      if (!gSRGramNS.m_fPhraseStart)
         Warning ("Got a PhraseHypothesis but no PhraseStart");

      // make sure that each hypothesis object is different
      if (gpSRResult) {
         if (pResHypothesis) {
            // compare the unique IDs
            GUID  g1, g2;
            hRes = pResHypothesis->Identify(&g1);
            if (hRes)
               Warning ("Result object wouldn't identify");
            hRes = gpSRResult->Identify(&g2);
            if (hRes)
               Warning ("Result object wouldn't identify");

            if (IsEqualGUID(g1,g2) || (pResHypothesis->m_pISRResBasic == gpSRResult->m_pISRResBasic))
               Warning ("The results of subsequent hypothesis are the same. They shouldn't be");
         }
         
         // store this one away
         delete pResHypothesis;
         pResHypothesis = gpSRResult;
         gpSRResult = NULL;
      }
   }

   // Validate that PhraseHypothesis and PhraseFinish happened around the same time
   if (abs((int)(gSRGramNS.m_qwPhraseHypothesisStart - gSRGramNS.m_qwPhraseFinishStart)) > nAllowedDiff)
      Warning ("PhraseHypothesis start time not close enough to PhraseFinish start time");

   ClearMainGrammar();
   ClearSRAudio();

   if (pResFinish && pResHypothesis) {
      // compare the unique IDs
      GUID  g1, g2;
      hRes = pResHypothesis->Identify(&g1);
      if (hRes)
         Warning ("Result object wouldn't identify");
      hRes = pResFinish->Identify(&g2);
      if (hRes)
         Warning ("Result object wouldn't identify");

      if (IsEqualGUID(g1,g2) || (pResHypothesis->m_pISRResBasic == pResFinish->m_pISRResBasic))
         Warning ("The results of subsequent hypothesis are the same. They shouldn't be");
   }

   delete pResHypothesis;
   delete pResFinish;

   if (!fHypothesis)
      Warning ("Didn't send phrase hypothesis");

   // done
   Passed();
}

// - Training - See if get this when first create gramamr on untrained.
void CALLBACK Test143 (void)
{
   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;
   NEEDSRINTERFACE (SRI_ISRSPEAKER);

   HRESULT  hRes;

   // arrange things so we have a new, untrained speaker
   hRes = gpSRMode->Select(L"2");
   CHECKFAILED ("Select");
   gpSRMode->Delete(L"1");  // don't check error intentionally
   hRes = gpSRMode->Select(L"1");
   CHECKFAILED ("Select");

   // set a flag so we know if we got the training requested message
   gSRGramNS.m_fTraining = FALSE;

   // listen
   SpeakAndListen ("Hello world", gszHelloWorldGrammar);

   // has the training flag been set?
   if (!gSRGramNS.m_fTraining)
      Warning ("Didn't get a training-request message for a new user");


   Passed();
}



// - Interference - Make sure get SRMSGINT_AUDIODATA_STARTED at the right time
// - Interference - Make sure that get SRMSGINT_AUDIODATA_STOPPED at the right time
// - Interference - Make sure that get SRMSGINT_IAUDIO_STARTED at the right time
// - Interference - Make sure that get SRMSGINT_IAUDIO_STOPPED at the right time
// - Training - See if get this when first create gramamr on untrained.
void CALLBACK Test144 (void)
{
   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

   HRESULT  hRes;
   
   // preload some audio
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // load grammar
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

#define  HASINTERFERENCE(x)      (gSRNS.m_dwInterferenceValues & (1 << (x)))

   // set flag so no if got interference message
   gSRNS.m_fInterference = FALSE;
   gSRNS.m_dwInterferenceValues = 0;

   // start listening
   hRes = gpSRGramComp->Activate();
   CHECKFAILED("Activate");

   // wait 1/4 second(s), and make sure we've gotten an audio started message
   BOOL  *pf;
   pf = WaitForEvent (.25);
   if (pf == &gfAbortTest)
      return;
   if (!gSRNS.m_fInterference || !HASINTERFERENCE(SRMSGINT_IAUDIO_STARTED))
      Warning ("Engine didn't pass up notification when it got an IAudioSourceNotifySink::Start");

   // wait for the recognition to go through, and for the engine
   // to return complaining that the audio has stopped
   gSRNS.m_fInterference = FALSE;
   gSRNS.m_dwInterferenceValues = 0;
   pf = WaitForEvent (60.0);
   if (pf == &gfAbortTest)
      return;
   if (!gSRNS.m_fInterference || !HASINTERFERENCE(SRMSGINT_AUDIODATA_STOPPED))
      Warning ("Engine didn't pass up notification that it's being starved for audio.");

   // send in some more audio
   gSRNS.m_fInterference = FALSE;
   gSRNS.m_dwInterferenceValues = 0;
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // Wait for the for the engine to say that the audio has resumed and for
   // the recognition to go through.
   pf = WaitForEvent (60);
   if (pf == &gfAbortTest)
      return;
   if (!gSRNS.m_fInterference || !HASINTERFERENCE(SRMSGINT_AUDIODATA_STARTED))
      Warning ("Engine didn't pass up notification when that it is no longer being starved");

   // stop listening
   gSRNS.m_fInterference = FALSE;
   gSRNS.m_dwInterferenceValues = 0;
   ClearMainGrammar();
   ClearSRAudio();

   // wait 1/4 second, and make sure we've gotten an audio stopped message
   pf = WaitForEvent (.25);
   if (pf == &gfAbortTest)
      return;
   if (!gSRNS.m_fInterference || !HASINTERFERENCE(SRMSGINT_IAUDIO_STOPPED))
      Warning ("Engine didn't pass up notification when it got an IAudioSourceNotifySink::Stop");

   Passed();
}



// - VUMeter - Make sure get this.
void CALLBACK Test145(void)
{
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   gSRNS.m_fVUMeter = FALSE;
   SpeakAndListen ("Hello world", gszHelloWorldGrammar);
   if (!gSRNS.m_fVUMeter)
      Warning ("Didn't get VU meter");

   Passed();
}



// - FlagsGet - Make sure that returns for rank=0, and make sure it's the same value as from PhraseFinish
// - FlagsGet - See how many ranks can go up to
// - FlagsGet - Invalid Param
// - FlagsGet - Make sure that if can get a value for rank N, than also PhraseGet works for same index
void CALLBACK Test146(void)
{
   NEEDSRINTERFACE (SRI_ISRESBASIC);
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   PCSRResult  pRes;
   HRESULT hRes;
   pRes = GetResultsObject(gszAllWork, gszAllWork2Grammar);
   if (!pRes) {
      Failed ("Didn't get results object");
      return;
   }

   // rank 0
   DWORD dwFlags, dwFlagsLast;
   hRes = pRes->FlagsGet (0, &dwFlags);
   if (hRes) {
      Failed ("FlagsGet", hRes);
   }
   if (dwFlags != gSRGramNS.m_dwPhraseFinishFlags)
      Failed ("FlagsGet doesn't match the return from PhraseFinish");

   // see hom many ranks can go up to
   DWORD dw;
   dwFlagsLast = gSRGramNS.m_dwPhraseFinishFlags;
   for (dw = 0; ;dw++) {
      hRes = pRes->FlagsGet (dw, &dwFlags);
      if (hRes)
         break;

      // make sure the all of the recognized (above rejection)
      // responses are first
      if ((dwFlags & ISRNOTEFIN_RECOGNIZED) && !(dwFlagsLast & ISRNOTEFIN_RECOGNIZED))
         Failed ("A lower scale result object was above the rejection threshold, while a previous one was below");

      dwFlagsLast = dwFlags;

      // make sure PhraseGet works for each of these
      BYTE  abHuge[10000];
      hRes = pRes->PhraseGet (dw, (PSRPHRASEW) &abHuge,
         sizeof (abHuge), &dwFlags);
      if (hRes) {
         Failed ("PhraseGet failed but FlagsGet succeeded", hRes);
         break;
      }
   }

   // try an invalid param
   hRes = pRes->FlagsGet (0, NULL);
   if (!hRes)
      Failed ("Didn't detect an invalid parameter");

   // free the results object
   delete pRes;

   Passed();
}



// - Identify - Make sure works
// - Identify - Invalid param

void CALLBACK Test147(void)
{
   NEEDSRINTERFACE (SRI_ISRESBASIC);
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   PCSRResult  pRes;
   HRESULT hRes;
   pRes = GetResultsObject(gszAllWork, gszAllWork2Grammar);
   if (!pRes) {
      Failed ("Didn't get results object");
      return;
   }

   GUID  g;
   hRes = pRes->Identify(&g);
   if (hRes)
      Failed ("Identify", hRes);
   if (IsEqualGUID (g, GUID_NULL))
      Failed ("Identify returned a NULL GUID");

   // invalid param
   hRes = pRes->Identify (NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");


   // free the results object
   delete pRes;

   Passed();
}

// - PhraseGet - Get rank 0, and make sure its the same value as phrase finish
// - PhraseGet - try with null memory to make sure it returns enough memory, and then tyr
//	phraseget again with enough memory
// - PhraseGet - See how many ranks can go up to
// - PhraseGet - Invalid params
void CALLBACK Test148(void)
{
   NEEDSRINTERFACE (SRI_ISRESBASIC);
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   PCSRResult  pRes;
   HRESULT hRes;

   char szTestGrammar[] =
      "[<Start>]\n"
      "<Start>=blue\n"
      "<Start>=blues\n"
      "<Start>=blew\n"
      "<Start>=blow\n"
      ;

   pRes = GetResultsObject("blue", szTestGrammar);
   
   if (!pRes) {
      Failed ("Didn't get results object");
      return;
   }

   DWORD dw;
   DWORD dwNeeded;
   for (dw = 0; ; dw++) {
      BYTE  abHuge[10000];
      PSRPHRASEW  pPhrase = (PSRPHRASEW) abHuge;

      // get the phrase
      hRes = pRes->PhraseGet (dw, NULL, NULL, &dwNeeded);
      if (hRes != SRERR_NOTENOUGHDATA) {
         if (dw && (hRes == SRERR_VALUEOUTOFRANGE)) {
            // if there's only one results object then warning
            if (dw == 1)
               Warning ("This only had one alternative. Expected more");

            break;   // no more phrases
         }
         
         // if it got here, didn't get expected error
         Failed ("PhraseGet", hRes);
      }

      // try using the memory claimed
      DWORD dwNeed;
      dwNeed = dwNeeded;
      hRes = pRes->PhraseGet (dw, (PSRPHRASEW) abHuge, dwNeed, &dwNeeded);
      if (hRes) {
         Failed ("PhraseGet", hRes);
         break;
      }

      // make sure its right about the size
      if (pPhrase->dwSize > dwNeed)
         Failed ("PhraseGet said that needed less space than really needed");

      // If asking for the rank 0 alternative, then this should be the same
      // as the SRPHRASE given in the phrase finish call.
      if (!dw) {
         // Have to compare individual SRWORD structures since DWORD alignment
         // may result in garbage padding bytes that may differ from phrase
         // to phrase.
         PSRPHRASEW pPhraseFinish = (PSRPHRASEW) gSRGramNS.m_abPhraseFinishPhrase;
         PSRPHRASEW pPhraseGet = (PSRPHRASEW) abHuge;
         PSRWORDW   pPFWord = (PSRWORDW) pPhraseFinish->abWords;
         PSRWORDW   pPGWord = (PSRWORDW) pPhraseGet->abWords;
         DWORD      dwCount = pPhraseFinish->dwSize - sizeof(PSRPHRASEW);
         if (pPhraseFinish->dwSize != pPhraseGet->dwSize)
            Failed("Rank 0 phrase different length then phrase returned via PhraseFinish.");
         else {
            while (dwCount) {
               if (pPFWord->dwWordNum != pPGWord->dwWordNum) {
                  Failed("Rank 0 phrase has different WordNum than phrase returned via PhraseFinish.");
                  break;
               }
               if (wcscmp(pPFWord->szWord, pPGWord->szWord)) {
                  Failed("Rank 0 phrase has different Word text than phrase returned via PhraseFinish.");
                  break;
               }
               
               dwCount -= pPFWord->dwSize;
               pPFWord =  (PSRWORDW) (((PSTR) pPFWord) + pPFWord->dwSize);
               pPGWord =  (PSRWORDW) (((PSTR) pPGWord) + pPGWord->dwSize);
            }
         }
      }

      // make sure that can get flags for this
      hRes = pRes->FlagsGet (dw, &dwNeed);
      if (hRes)
         Failed ("FlagsGet", hRes);
   }

   // try some invalid parameters
   hRes = pRes->PhraseGet (0, 0, 0, 0);
   if (!hRes)
      Failed ("Phrase get didn't detect invalid parameters");
   hRes = pRes->PhraseGet (0, 0, 10000, &dwNeeded);
   if (!hRes)
      Failed ("Phrase get didn't detect invalid parameters");

   // free the results object
   delete pRes;

   Passed();
}


// - TimeGet - Make sure it's a reasonable value, and the same value as from phrasefinsh
// - TimeGet - Invalid params
void CALLBACK Test149(void)
{
   NEEDSRINTERFACE (SRI_ISRESBASIC);
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   PCSRResult  pRes;
   HRESULT hRes;
   pRes = GetResultsObject(gszAllWork, gszAllWork2Grammar);
   if (!pRes) {
      Failed ("Didn't get results object");
      return;
   }

   QWORD qwStart, qwEnd;
   hRes = pRes->TimeGet(&qwStart, &qwEnd);
   if (hRes)
      Failed ("TimeGet", hRes);
   if ((qwStart != gSRGramNS.m_qwPhraseFinishStart) ||
      (qwEnd != gSRGramNS.m_qwPhraseFinishEnd))
      Failed ("TimeGet doesn't have same time stamp as ISRGramNotifySink::IAudio");

   // get only one of the params
   QWORD qwTemp;
   hRes = pRes->TimeGet (&qwTemp, NULL);
   if (hRes)
      Failed ("TimeGet",hRes);
   if (qwTemp != qwStart)
      Failed ("Didn't return right value");
   hRes = pRes->TimeGet (NULL, &qwTemp);
   if (hRes)
      Failed ("TimeGet",hRes);
   if (qwTemp != qwEnd)
      Failed ("Didn't return right value");


   // invalid params
   hRes = pRes->TimeGet (NULL, NULL);
   // don't check the error. Just make sure it doesn't crash


   // free the results object
   delete pRes;

   Passed();
}

// - Correction - Try it with valid params
// - Correction - Invalid params
void CALLBACK Test150(void)
{
   NEEDSRINTERFACE (SRI_ISRESCORRECTION);
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;

   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");
	
	PCSRResult  pRes;
   pRes = GetResultsObject(gszAllWork, NULL);
   if (!pRes) {
      Failed ("Didn't get results object");
      return;
   }

   // construct a PSR Phrase
   PVOID pMem;
   DWORD dwSize;
   pMem = ConstructPhrase (
      L"all\0work\0and\0no\0play\0makes\0jack\0a\0dull\0boy\0",
      &dwSize);

	// replace constructed phrase with the result that's actually heard
	DWORD dwSizeNeeded;
	hRes = pRes->PhraseGet(0, (PSRPHRASEW)pMem, dwSize, &dwSizeNeeded);


   // send down
   hRes = pRes->Correction((PSRPHRASEW)pMem, SRCORCONFIDENCE_VERY);
   if (hRes)
      Failed ("Correction", hRes);

   // test invalid parameters
   hRes = pRes->Correction (NULL, SRCORCONFIDENCE_VERY);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   hRes = pRes->Correction ((PSRPHRASEW)pMem, 52);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   // free the results object
   delete pRes;

	ClearMainGrammar();

   Passed();
}




// - Validate - Try with valid params
// - Validate - Try with invalid params
void CALLBACK Test151(void)
{
   NEEDSRINTERFACE (SRI_ISRESCORRECTION);
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;

   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");
	
	PCSRResult  pRes;
   pRes = GetResultsObject(gszAllWork, NULL);
   if (!pRes) {
      Failed ("Didn't get results object");
      return;
   }

   // send down
   hRes = pRes->Validate(SRCORCONFIDENCE_SOME);
   if (hRes)
      Failed ("Validate", hRes);

   // test invalid parameters
   hRes = pRes->Validate (52);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   // free the results object
   delete pRes;

	ClearMainGrammar();

   Passed();
}


// - ReEvaluate - Try with valid params
// - ReEvaluate - Try with invalid params
void CALLBACK Test152(void)
{
   NEEDSRINTERFACE (SRI_ISRESEVAL);
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;

   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");
	
	PCSRResult  pRes;
   pRes = GetResultsObject(gszAllWork, NULL);
   if (!pRes) {
      Failed ("Didn't get results object");
      return;
   }

   // send down
   BOOL  f;
   hRes = pRes->ReEvaluate(&f);
   if (hRes)
      Failed ("ReEvaluate", hRes);

   // test invalid parameters
   hRes = pRes->ReEvaluate(NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   // free the results object
   delete pRes;

	ClearMainGrammar();

   Passed();
}



// - ISRResMemory::Free - Try freeing audio and see if disappears
// - ISRResMemory::Free - Try freeing word graph and see if disappears
// - ISRResMemory::Free - Invalid params
// - ISResMemory::Get - try it
// - ISRREsMemory::Get - Free something and see if memory goes down
// - Get - Invalid params
void CALLBACK Test153(void)
{
   NEEDSRINTERFACE (SRI_ISRESMEMORY);
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;

   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");
	
	PCSRResult  pRes;
   pRes = GetResultsObject(gszAllWork, NULL);
   if (!pRes) {
      Failed ("Didn't get results object");
      return;
   }

   // get the values
   DWORD dwKind, dwValue;
   hRes = pRes->Get(&dwKind, &dwValue);
   if (hRes)
      Failed ("Get", hRes);

   // free it all
   hRes = pRes->Free (dwKind);
   if (hRes)
      Failed ("Free", hRes);

   // see what it says now
   DWORD dwKind2, dwValue2;
   hRes = pRes->Get(&dwKind2, &dwValue2);
   if (hRes)
      Failed ("Get", hRes);
   if (dwKind2)
      Warning ("Told result object to free it all, but not all was freed");
   if (dwValue2 >= dwValue)
      Warning ("Told result object to free it all, but memory didn't change");

   // if there was audio, and now there's not, doa  check to make sure it's gone
   if ((dwKind & SRRESMEMKIND_AUDIO) && !(dwKind2 & SRRESMEMKIND_AUDIO)) {
      SDATA s;
      hRes = pRes->GetWAV (&s);
      if (!hRes) {
         Failed ("Freeing audio didn't seem to free it");
         if (s.pData)
            CoTaskMemFree (s.pData);
			s.pData = NULL;
      }
   }

   // invalid params
   hRes = pRes->Get(NULL, NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   // no invalid parameters for free, expect unahandled types.
   // expect the engine to just ignore
   hRes = pRes->Free (0xffffffff);
   if (hRes)
      Failed ("Free data types that don't exist", hRes);

   // free the results object
   delete pRes;

	ClearMainGrammar();

   Passed();
}




// - ISRResMemory::LockGet - Lock and see if deteriorates. Do times deterioration?
// - ISRResMemory::Unlock - Unlock and see if deteriorates

void CALLBACK Test154(void)
{
   NEEDSRINTERFACE (SRI_ISRESMEMORY);
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;

   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");
	
	PCSRResult  pRes;
   pRes = GetResultsObject(gszAllWork, NULL);
   if (!pRes) {
      Failed ("Didn't get results object");
      return;
   }

   BOOL  fLock;
   hRes = pRes->LockGet(&fLock);
   if (hRes)
      Failed ("LockGet", hRes);
   if (fLock)
      Failed ("Result object shouldn't start out locked");

   // set
   hRes = pRes->LockSet (TRUE);
   if (hRes)
      Failed ("LockSet", hRes);
   hRes = pRes->LockGet(&fLock);
   if (hRes)
      Failed ("LockGet", hRes);
   if (!fLock)
      Failed ("Result object didn't lock");

   // set
   hRes = pRes->LockSet (FALSE);
   if (hRes)
      Failed ("LockSet", hRes);
   hRes = pRes->LockGet(&fLock);
   if (hRes)
      Failed ("LockGet", hRes);
   if (fLock)
      Failed ("Result object didn't unlock");

   // invalid params
   hRes = pRes->LockGet (NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   // free the results object
   delete pRes;

	ClearMainGrammar();

   Passed();
}


// - ISRResScore::GetPhraseScore & GetWordScore - Make sure can at least get score for index 0.
// - ISRResScore::Phrase&WordScore - Make sure can get score for an index that works
//	with ISRResBasic
// - ISRResScore::Phrase&WordScore - Make sure that paramters do not exceed valid settings.
// - ISRResScore::Phrase&WordScore - Invalid params
void CALLBACK Test155(void)
{
   NEEDSRINTERFACE (SRI_ISRESSCORES);
   NEEDCFG;
	NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;

//   HRESULT  hRes;
//   hRes = SetMainGrammar (gszDictationGrammar);
//   CHECKFAILED ("SetMainGrammar");
	
//	PCSRResult  pRes;
//   pRes = GetResultsObject(gszAllWork, NULL);
	
   PCSRResult  pRes;
   HRESULT hRes;
   pRes = GetResultsObject(gszAllWork, gszAllWork2Grammar);

   DWORD dw;
   DWORD dwNeeded;
   long  lLastScore = 100;
   for (dw = 0; ; dw++) {
      BYTE  abHuge[1000];
      PSRPHRASEW  pPhrase = (PSRPHRASEW) abHuge;

      // get the phrase
      hRes = pRes->PhraseGet (dw, (PSRPHRASEW) abHuge, sizeof(abHuge), &dwNeeded);
      if (hRes) {
         // no more phrases
         break;
      }

      // get phrase score
      long  lScore;
      hRes = pRes->GetPhraseScore (dw, &lScore);
      if (hRes) {
         Failed ("GetPhraseScore", hRes);
         break;
      }
      if ((lScore > 100) || (lScore < -100))
         Failed ("Score out of range");
      if (lScore > lLastScore)
         Failed ("A lower ranked score is higher than the higher ranked score");
      lLastScore = lScore;

      // get the word scores
      long  alScore[100];
      hRes = pRes->GetWordScores(dw, NULL, NULL, &dwNeeded);
      if (hRes != SRERR_NOTENOUGHDATA) {
         Failed ("GetWordScores", hRes);
         break;
      }
      hRes = pRes->GetWordScores (dw, alScore, dwNeeded, &dwNeeded);
      if (hRes) {
         Failed ("GetWordScores", hRes);
         break;
      }
      
      // make sure they're in range
      DWORD i;
      for (i = 0; i < (dwNeeded / 4); i++)
         if ((alScore[i] > 100) || (alScore[i] < -100))
            Failed ("Score out of range");

      // make sure the right number of words are representd
      DWORD dwCount;
      PSRWORDW pWord;
      for (pWord = (PSRWORDW)(pPhrase+1), dwCount = 0;
         pWord < (PSRWORDW) ((BYTE*)pPhrase + pPhrase->dwSize);
         pWord = (PSRWORDW) ((BYTE*)pWord + pWord->dwSize), dwCount++ );
      if (dwCount != (dwNeeded / 4))
         Failed ("Wrong number of words indicated by scores");

   }

   // invalid interfaces
   hRes = pRes->GetPhraseScore (0, NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   hRes = pRes->GetWordScores (0, 0, 0, 0);
   if (!hRes)
      Failed ("Didn't detect invalid parameters");
   hRes = pRes->GetWordScores (0, 0, 10000, &dwNeeded);
   if (!hRes)
      Failed ("Didn't detect invalid parameters");

   // free the results object
   delete pRes;

	//ClearMainGrammar();

   Passed();
}


// - ISRResAudio
void CALLBACK Test156(void)
{
   NEEDSRINTERFACE (SRI_ISRESAUDIO);
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;

   HRESULT  hRes;
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");
	
	PCSRResult  pRes;
   pRes = GetResultsObject(gszAllWork, NULL);
   if (!pRes) {
      Failed ("Didn't get results object");
      return;
   }

   // get the audio
   SDATA s;
   hRes = pRes->GetWAV (&s);
   if (hRes) {
      Failed ("GetWAV", hRes);
      goto done;
   }

   // play it out
   sndPlaySound (NULL, NULL);
   if (!sndPlaySound ((LPCSTR) s.pData, SND_MEMORY | SND_SYNC))
      Failed ("Couldn't play sound out to sound card with sndPlaySound"
         " - Make sure no other sounds were playing");

   // free it
	if( s.pData )
		CoTaskMemFree (s.pData);
	s.pData = NULL;

   // invalid param
   hRes = pRes->GetWAV (NULL);
   if (!hRes)
      Failed ("GetWAV didn't detect invalid parameter");

done:
   // free the results object
   delete pRes;

	ClearMainGrammar();

   Passed();
}

// - Make sure the recognizer doesn't time out too quickly
void CALLBACK Test157(void)
{
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;

   char  szForm1[] =
      "[<Start>]\n"
      "<Start>=(DictationOnOff)\n"
      "[(DictationOnOff)]\n"
      "50=start dictating\n"
      "=stop (DictationOnOff2)\n"
      "[(DictationOnOff2)]\n"
      "51=dictating\n"
      "51=dictation\n"
      "52=listening\n"
      "53=listening for commands\n"
      ;

   char  szForm2[] =
      "[<Start>]\n"
      "<Start>=(DictationOnOff)\n"
      "[(DictationOnOff)]\n"
      "50=start dictating\n"
      "=stop (DictationOnOff2)\n"
      "[(DictationOnOff2)]\n"
      "51=dictating\n"
      "51=dictation\n"
      "52=listening [opt] (ForCommands)\n"
      "[(ForCommands)]\n"
      "1=for commands\n"
      ;

   char  szSpeak[] = "stop listening for commands";

   char  *psz;
   psz = SpeakAndListen (szSpeak, szForm1);
   if (!psz)
      return;
   if (stricmp (psz, szSpeak))
      Failed ("Didn't hear correct phrase");

   psz = SpeakAndListen (szSpeak, szForm2);
   if (!psz)
      return;
   if (stricmp (psz, szSpeak))
      Failed ("Didn't hear correct phrase");

   Passed();
}

// Lex: Get pronunciations - known valid word
// note: it is recommeded that ILexPronounce2::GetFrom be used to instead
void CALLBACK Test158(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE);	
	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;
   HRESULT  hRes;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// in 
	WCHAR *pszText = L"Hello";
	WORD wSense = 0;

 	// out
	WCHAR szPronounce[256];
	DWORD dwPronounceNeeded;
	VOICEPARTOFSPEECH PartOfSpeech = VPS_UNKNOWN;
	PVOID pEngineInfo;
	pEngineInfo = malloc (14 * 64);
	DWORD dwEngineInfoNeeded = 0;
	
	// use with main object
	if( !(gpSRMode->m_pILexPronounce) ) 
		Warning ("unable to test through main object gpSRMode->m_pILexPronounce is NULL");
	else {
		do {
			hRes = gpSRMode->m_pILexPronounce->Get(
		 		CHARSET_IPAPHONETIC,		//CHARSET_TEXT ,CHARSET_IPAPHONETIC ,CHARSET_ENGINEPHONETIC 
				pszText,					// word to get
				wSense,						// sense
				szPronounce,				// output buffer
				sizeof(szPronounce),
				&dwPronounceNeeded,			// size needed if pszPronounce is too small
				&PartOfSpeech,				// output part of speach value
				pEngineInfo,				// output additional info
				sizeof(pEngineInfo),
				&dwEngineInfoNeeded);		// sized needed of addition info if pEnginInfo is too small
			wSense++;
		} while ( !hRes && wSense < 64 );
		if( (hRes != LEXERR_NOTINLEX && hRes != LEXERR_INVALIDSENSE) || wSense == 1)	{// if failed on first try or other error then failed
			Failed ("Get failed with main object", hRes);
			bMainPass = FALSE;
		}
		if( wSense >= 64 )
			Failed ("Get does not support Sense parameter");
	}

	// access through grammer object
	wSense = 0;
   do {
		hRes = gpSRGramComp->Get(
	 		CHARSET_IPAPHONETIC,		//CHARSET_TEXT ,CHARSET_IPAPHONETIC ,CHARSET_ENGINEPHONETIC 
			pszText,					// word to get
			wSense,						// sense
			szPronounce,				// output buffer
			sizeof(szPronounce),
			&dwPronounceNeeded,			// size needed if pszPronounce is too small
			&PartOfSpeech,				// output part of speach value
			pEngineInfo,				// output additional info
			sizeof(pEngineInfo),
			&dwEngineInfoNeeded);		// sized needed of addition info if pEnginInfo is too small
		wSense++;
	} while ( !hRes && wSense < 64 );

	if (hRes == SRERR_NOTSUPPORTED || hRes == SRERR_INVALIDINTERFACE) {
      Warning ("ISRGramComp::Get not supported");
   }
	else {
  		if( hRes != LEXERR_INVALIDSENSE || wSense == 1)	{// if failed on first try or other error then failed
			Failed ("Get failed with grammer object", hRes);
			bGramPass = FALSE;
		}
		if( wSense >= 64 )
			Failed ("Get does not support Sense parameter");
	}

	// cleanup
	free (pEngineInfo);
	ClearMainGrammar();
	
	// check failure condition
	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}

// Lex: Get pronunciations - get pronunciation of word not in lex
// note: it is recommended that ILexPronounce2::GetFrom be used instead
void CALLBACK Test159(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");
	
	// in 
	WCHAR *pszText = L"abcdefghijklmnop";
	WCHAR *pszNULL = NULL;
	WORD wSense = 0;

 	// out
	WCHAR szPronounce[512];
	DWORD dwPronounceNeeded = 0;
	VOICEPARTOFSPEECH PartOfSpeech = VPS_UNKNOWN;
	PVOID pEngineInfo = malloc (14 * sizeof(WCHAR));
	DWORD dwEngineInfoNeeded = 0;
	
	// check main object
	if( !(gpSRMode->m_pILexPronounce) )
		Warning ("unable to test: gpSRMode->m_pILexPronounce is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce->Get(CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
			// check failure condition
		if( FAILED(hRes) && hRes != LEXERR_NOTINLEX ) {
			Failed ("Expected LEXERR_NOTINLEX error for word not in lex through main object", hRes);
			bMainPass = FALSE;
		}
			// check for NULL word
		hRes = gpSRMode->m_pILexPronounce->Get(CHARSET_IPAPHONETIC,pszNULL,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
		if( hRes != SRERR_INVALIDPARAM ) {
			Failed ("Expected SRERR_INVALIDPARAM for get of NULL word through main object", hRes);
			bMainPass = FALSE;
		}
	}
		 
	// check grammer object
	hRes = gpSRGramComp->Get(CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
	if( FAILED(hRes) && hRes != LEXERR_NOTINLEX ) {
		Failed ("Expected LEXERR_NOTINLEX error for word not in lex", hRes);
		bGramPass = FALSE;
	}

	// check for get of NULL word
	hRes = gpSRGramComp->Get(CHARSET_IPAPHONETIC,pszNULL,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
	if( hRes != SRERR_INVALIDPARAM ) {
		Failed ("Expected SRERR_INVALIDPARAM for get of NULL word through grammer object", hRes);
		bGramPass = FALSE;
	}


	// cleanup
	free (pEngineInfo);
	ClearMainGrammar();

	if( bGramPass & bMainPass ) {
		Passed();
	}

	return;
}

// Lex: Get pronunciations - known valid word, check returns proper size when buffer too small.
// note: it is recommended that ILexPronounce2::GetFrom be used instead
void CALLBACK Test160(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE);	
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// in 
	WCHAR *pszText = L"Hello";
	WORD wSense = 0;

 	// out
	WCHAR szPronounce[2];
	DWORD dwPronounceNeeded = 0;
	VOICEPARTOFSPEECH PartOfSpeech = VPS_UNKNOWN;
	PVOID pEngineInfo;
	pEngineInfo = malloc (14 * 64);
	DWORD dwEngineInfoNeeded = 0;
	
	if( !(gpSRMode->m_pILexPronounce) ) 
		Warning ("unable to test: gpSRMode->m_pILexPronounce is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce->Get(CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
		// check failure condition
		if( (hRes != LEXERR_NOTINLEX && hRes != LEXERR_ENGBUFTOOSMALL && hRes != LEXERR_PRNBUFTOOSMALL) || dwPronounceNeeded == 0) {
			Failed ("Main object Get did not indicate return buffer too small or PronounceNeeded not set");
			bMainPass = FALSE;
		}
	}
		 
	hRes = gpSRGramComp->Get(CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
	// check failure condition
	if( (hRes != LEXERR_ENGBUFTOOSMALL && hRes != LEXERR_PRNBUFTOOSMALL) || dwPronounceNeeded == 0) {
		Failed ("Grammer object Get did not indicate return buffer too small or PronounceNeeded not set");
		bGramPass = FALSE;
	}
	
	// cleanup
	free (pEngineInfo);
	ClearMainGrammar();

	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}

// Lex: Add pronunciations - valid word
// note: it is recommmeded that ILexPronounce2::AddTo be used instead
void CALLBACK Test161(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// in
	WCHAR *pszText = L"TestNown";
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  

	// test main object
	if( !(gpSRMode->m_pILexPronounce) ) 
		Warning ("unable to test: gpSRMode->m_pILexPronounce is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce->Add(CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
		if( FAILED(hRes) ) {
			Failed ("Main object failed to add valid word", hRes);
			bMainPass = FALSE;
		}
		else {
			//delete entry
			hRes = gpSRMode->m_pILexPronounce->Remove(pszText, 0);
			if( FAILED(hRes) ) 
				Warning ("Passed although Remove returned error when trying to remove word added", hRes);
		}
	}
		
	pszText = L"TestNown2";

	// test grammer object
	hRes = gpSRGramComp->Add(CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
	if( FAILED(hRes)) {
		Failed ("Grammer object failed to add valid word", hRes);
		bGramPass = FALSE;
	}
	else {
		//delete entry
		hRes = gpSRMode->m_pILexPronounce->Remove(pszText, 0);
		if(FAILED(hRes)) 
			Warning ("Passed although Remove returned error when trying to remove word added", hRes);
	}
	
	// clean up
	ClearMainGrammar();
	
	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}


// Lex: Add pronunciations - known invalid word
// note: it is recommmeded that ILexPronounce2::AddTo be used instead
void CALLBACK Test162(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

	// load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// in
	WCHAR *pszText = NULL;
	WCHAR szPronounce[512];
	DWORD dwPronounceNeeded = 0;
	VOICEPARTOFSPEECH	PartOfSpeech = VPS_UNKNOWN;
	PVOID	pEngineInfo = malloc (1024);
	DWORD dwEngineInfoNeeded = 0;
	WORD wSense = 0;
	
	if( !(gpSRMode->m_pILexPronounce) ) 
		Warning ("unable to test: gpSRMode->m_pILexPronounce is NULL");
	else {		// get word to try to add
		hRes = gpSRMode->m_pILexPronounce->Add(CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_ADJECTIVE,NULL,0);
		if( SUCCEEDED(hRes) ) {
			bMainPass = FALSE;
			Failed ("no error return on adding invalid word through main object");
		}
	}			

	// check through grammer object
	hRes = gpSRGramComp->Add(CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_ADJECTIVE,NULL,0);
	if( SUCCEEDED(hRes) ) {
		bGramPass = FALSE;
		Failed ("no error return on adding invalid word through grammer object");
	}
	
	//clean up
	ClearMainGrammar();
	free (pEngineInfo);
	
	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}

// Lex: Add pronunciations - valid word, get it
// note: it is recommmeded that ILexPronounce2::AddTo and GetFrom be used instead
void CALLBACK Test163(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE);
	HRESULT  hRes;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// Add in
	WCHAR *pszText = L"TestNown";
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  

	// Get in 
	WORD wSense = 0;

 	// out
	DWORD dwPronounceNeeded;
	VOICEPARTOFSPEECH PartOfSpeech = VPS_UNKNOWN;
	PVOID pEngineInfo;
	pEngineInfo = malloc (14 * 64);
	DWORD dwEngineInfoNeeded = 0;


	if( !(gpSRMode->m_pILexPronounce) )
		Warning ("unable to test: gpSRMode->m_pILexPronounce is NULL");
	else {		// attempt to add word to main object
		hRes = gpSRMode->m_pILexPronounce->Add(CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
		if( FAILED(hRes) ) {
			if( hRes == LEXERR_ALREADYINLEX ) {
				hRes = gpSRMode->m_pILexPronounce->Remove(pszText,0);
				if( FAILED(hRes) )
					Failed ("Failed to remove word through main object", hRes);
			}
			else
				Warning ("Failed to add valid word in main object", hRes);
		}
		else	{
         WCHAR szPron[512];

			// get word that was added
			hRes = gpSRMode->m_pILexPronounce->Get(CHARSET_IPAPHONETIC,pszText,wSense,szPron,sizeof(szPron),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
			if( FAILED(hRes) )
				Failed ("Failed to get added word in main object", hRes);

			// attempt to remove word that was added
			WORD wSense = 0;
			do {
				hRes = gpSRMode->m_pILexPronounce->Remove(pszText, wSense);
				if(FAILED(hRes) && hRes != LEXERR_INVALIDLEX && hRes != LEXERR_INVALIDSENSE) 
					Failed ("Failed to remove added word in main object", hRes);
				wSense++;
			} while (wSense < 10 && hRes != 0 && hRes != LEXERR_INVALIDSENSE);
		}
	}
		// attempt to add word to grammer object
	hRes = gpSRGramComp->Add(CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
	if( FAILED(hRes) ) {
		if( hRes == LEXERR_ALREADYINLEX ) {
			Failed("word already in lexicon, attempting to remove it");
			hRes = gpSRGramComp->Remove(pszText, 0);
			if(FAILED(hRes))
				Failed ("Failed to remove added through Grammer object", hRes);
		}
		else
			Warning ("Failed to add valid word in Grammer object", hRes);
	}
	else {
		WCHAR szPron[512];

      hRes = gpSRGramComp->Get(CHARSET_IPAPHONETIC,pszText,wSense,szPron,sizeof(szPron),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
		if(FAILED(hRes))
			Failed ("Failed to get added word in Grammer object", hRes);

		// attempt to remove word that was added
		WORD wSense = 0;
		do {
			hRes = gpSRGramComp->Remove(pszText, wSense);
			if(FAILED(hRes) && hRes != LEXERR_INVALIDLEX && hRes != LEXERR_INVALIDSENSE ) 
				Failed ("Failed to remove added word in Grammer object", hRes);
			wSense++;
		} while (wSense < 10 && hRes != 0 && hRes != LEXERR_INVALIDSENSE);
	}

	// clean up
	free (pEngineInfo);
	ClearMainGrammar();

	Passed();

	return;
}

// Lex: Add pronunciations - invalid param
// note: it is recommmeded that ILexPronounce2::AddTo be used instead
void CALLBACK Test164(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// Add in
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  

	// Get in 
	WORD wSense = 0;

 	// Get out
	DWORD *pdwPronounceNeeded = NULL;
	VOICEPARTOFSPEECH *pPartOfSpeech = NULL;
	
	if( !(gpSRMode->m_pILexPronounce) ) 
		Warning ("Unable to test: gpSRMode->m_pILexPronounce is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce->Add(CHARSET_IPAPHONETIC,NULL,szPronounce,VPS_NOUN,NULL,0);
		if( hRes != E_INVALIDARG ) {
			Failed ("Failed to return E_INVALIDARG for add of NULL word in main object", hRes);
			bMainPass = FALSE;
		}
	}

	hRes = gpSRGramComp->Add(CHARSET_IPAPHONETIC,NULL,szPronounce,VPS_NOUN,NULL,0);
	if( hRes != E_INVALIDARG ) {
			Failed ("Failed to return E_INVALIDARG for add of NULL word in Grammer object", hRes);
			bGramPass = FALSE;
	}

	// clean up
	ClearMainGrammar();

	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}

// Lex: Remove word - known valid words. 
// note: it is recommmeded that ILexPronounce2::RemoveFrom be used instead
void CALLBACK Test165(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// in
	WCHAR *pszText = L"green";
	WCHAR *pszPronounce = L"green";

 	// out
	DWORD dwPronounceNeeded;
	VOICEPARTOFSPEECH PartOfSpeech = VPS_UNKNOWN;
	PVOID pEngineInfo;
	pEngineInfo = malloc (14 * 64);
	DWORD dwEngineInfoNeeded = 0;

	// remove valid word
	if( !(gpSRMode->m_pILexPronounce) ) 
		Warning ("unable to test: gpSRMode->m_pILexPronounce is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce->Remove(pszText, 0);
		if( FAILED(hRes) && hRes != LEXERR_NOTINLEX && hRes != LEXERR_INVALIDLEX)
			Warning ("failed to remove valid word through main object", hRes);
		else {
         WCHAR szPron[512];
			// attempt to get word just removed
			if( !(gpSRMode->m_pILexPronounce2) ) {
				Information ("gpSRMode->m_pILexPronounce2 not supported, unable to attempt GetFrom lexicon word was removed from");
			}
			else {
				hRes = gpSRMode->m_pILexPronounce2->GetFrom(ILP2_USER,CHARSET_IPAPHONETIC,pszText,0,szPron,sizeof(szPron),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
				if( hRes != LEXERR_NOTINLEX ) {
					Failed ("get returned error other than LEXERR_NOTINLEX after remove of valid word through main object");
					bMainPass = FALSE;
				}
				// attempt to add word back that was removed
				hRes = gpSRMode->m_pILexPronounce2->AddTo(ILP2_USER,CHARSET_IPAPHONETIC,pszText,pszPronounce,VPS_NOUN,NULL,0);
				if( FAILED(hRes) ) {
					Failed ("unable to add word back that was just removed through main object", hRes);
					bMainPass = FALSE;
				}
			}
		}
	}

	// try to remove different word from main object as main object might not have put it's word back
	pszText = L"blue";
	pszPronounce = L"blue";

	// revove valid word through grammer object
	hRes = gpSRGramComp->Remove(pszText, 0);
	if( FAILED(hRes) && hRes != LEXERR_NOTINLEX && hRes != LEXERR_INVALIDLEX)
		Warning ("failed to remove valid word through grammer object", hRes);
	else {
      WCHAR szPron[512];
		// attempt to get word just removed
		hRes = gpSRGramComp->GetFrom(ILP2_USER,CHARSET_IPAPHONETIC,pszText,0,szPron,sizeof(szPron),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
		if( hRes != LEXERR_NOTINLEX ) {
			Failed ("get returned error other than LEXERR_NOTINLEX after remove of valid word through grammer object", hRes);
			bGramPass = FALSE;
		}
		// attempt to add word back that was removed
		hRes = gpSRGramComp->AddTo(ILP2_USER,CHARSET_IPAPHONETIC,pszText,pszPronounce,VPS_NOUN,NULL,0);
		if( FAILED(hRes) ) {
			Failed ("unable to add word back that was just removed through grammer object", hRes);
			bGramPass = FALSE;
		}
	}

	// clean up
	free( pEngineInfo );
	ClearMainGrammar();

	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}

// Lex: Remove word - known invalid word. 
// note: it is recommmeded that ILexPronounce2::RemoveFrom be used instead
void CALLBACK Test166(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// in
	WCHAR *pszText = L"abcdefghijklmnop";

 	if( !(gpSRMode->m_pILexPronounce) ) {
		Warning ("unable to test: gpSRMode->m_pILexPronounce is NULL");
	}
	else {	// try to remove invalid word through main object
			hRes = gpSRMode->m_pILexPronounce->Remove(pszText, 0);
			// either word is not in lexicon or the sense points to a read only lex
			if( hRes != LEXERR_NOTINLEX && hRes != LEXERR_INVALIDLEX )
				Failed("Did not return correct error for remove of word not in lex",hRes);
	}

	// try to remove invalid word through grammer object
	hRes = gpSRGramComp->Remove(pszText, 0);
		if( hRes != LEXERR_NOTINLEX && hRes != LEXERR_INVALIDLEX )
			Failed("Did not return correct error for remove of word not in lex",hRes);
	
	// clean up
	ClearMainGrammar();

	Passed();

	return;
}

// Lex2: GetFrom pronunciations - known valid word
void CALLBACK Test167(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);	
	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;
   HRESULT  hRes;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// in 
	WCHAR *pszText = L"Hello";
	WORD wSense = 0;

 	// out
	WCHAR szPronounce[256];
	DWORD dwPronounceNeeded;
	VOICEPARTOFSPEECH PartOfSpeech = VPS_UNKNOWN;
	PVOID pEngineInfo;
	pEngineInfo = malloc (14 * 64);
	DWORD dwEngineInfoNeeded = 0;
	DWORD dwLexicons = 0;

	// use with main object
	if( !(gpSRMode->m_pILexPronounce2) ) 
		Information ("unable to test through main object gpSRMode->m_pILexPronounce2 is NULL");
	else {
		// first get all lexicons that can read from
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_READ,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test through main object, QueryLexicons failed", hRes);
		}
		else {
			// check each lexicon by shifting through each bit
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++)	{
				if( dwQueryLex & dwLexicons ) {
					do {		// check each sense
						hRes = gpSRMode->m_pILexPronounce2->GetFrom(dwQueryLex,CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);		// sized needed of addition info if pEnginInfo is too small
						wSense++;
					} while ( !hRes && wSense < 64 );
					if( !(hRes == LEXERR_NOTINLEX && dwQueryLex == ILP2_USER))	{
						if( hRes != LEXERR_INVALIDSENSE || wSense == 1 ) {
							Failed ("GetFrom failed with main object", hRes);
							bMainPass = FALSE;
						}
						if( wSense >= 64 )
							Failed ("GetFrom had more than 64 Sense's for a word");
					}
					wSense = 0;
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}

	// access through grammer object
	// first get all lexicons that can read from
	hRes = gpSRGramComp->QueryLexicons(QUERY_READ,&dwLexicons);
	if( FAILED(hRes) ) {
		Warning( "unable to test through grammer object, QueryLexicons failed", hRes);
	}
	else {
		// check each lexicon by shifting through each bit
		DWORD dwQueryLex = 0;
		for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++)	{
			if( dwQueryLex & dwLexicons ) {
				do {		// check each sense
					hRes = gpSRGramComp->GetFrom(dwQueryLex,CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);		// sized needed of addition info if pEnginInfo is too small
					wSense++;
				} while ( !hRes && wSense < 64 );
				if( !(hRes == LEXERR_NOTINLEX && dwQueryLex == ILP2_USER))	{
					if( hRes != LEXERR_INVALIDSENSE || wSense == 1 ) {
						Failed ("GetFrom failed with grammer object", hRes);
						bMainPass = FALSE;
					}
					if( wSense >= 64 )
						Failed ("GetFrom had more than 64 Senses for a word");
				}
				wSense = 0;
			}
			dwQueryLex = SETBIT(iCount);
		}
	}

	
	// cleanup
	free (pEngineInfo);
	ClearMainGrammar();
	
	// check failure condition
	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}

// Lex2: GetFrom pronunciations - get pronunciation of word not in LTS lex
void CALLBACK Test168(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");
	
	// in 
	WCHAR *pszText = L"abcdefghijklmnop";
	WORD wSense = 0;

 	// out
	WCHAR szPronounce[512];
	DWORD dwPronounceNeeded = 0;
	VOICEPARTOFSPEECH PartOfSpeech = VPS_UNKNOWN;
	PVOID pEngineInfo = malloc (14 * sizeof(WCHAR));
	DWORD dwEngineInfoNeeded = 0;
	DWORD dwLexicons = 0;
	
	// check main object
	if( !(gpSRMode->m_pILexPronounce2) ) {
		Information ("unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	}
	else {
		hRes = gpSRMode->m_pILexPronounce2->GetFrom(ILP2_LTS,CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
		// check failure condition
		if( FAILED(hRes) ) {
			Failed ("Did not return pronunciation of word not in lex", hRes);
			bMainPass = FALSE;
		}
	}
	
	// check grammer object
	hRes = gpSRGramComp->GetFrom(ILP2_LTS,CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
	if( FAILED(hRes) ) {
		Failed ("Did not return pronunciation of word not in lex", hRes);
		bGramPass = FALSE;
	}

	// cleanup
	free (pEngineInfo);
	ClearMainGrammar();

	if( bGramPass & bMainPass ) {
		Passed();
	}

	return;
}

// Lex2: GetFrom pronunciations - known valid word, check returns proper size when buffer too small.
void CALLBACK Test169(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);	
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// in 
	WCHAR *pszText = L"Hello";
	WORD wSense = 0;

 	// out
	WCHAR szPronounce[2];
	DWORD dwPronounceNeeded = 0;
	VOICEPARTOFSPEECH PartOfSpeech = VPS_UNKNOWN;
	PVOID pEngineInfo;
	pEngineInfo = malloc (14 * 64);
	DWORD dwEngineInfoNeeded = 0;
	DWORD dwLexicons = 0;
	
	// test main object
	if( !(gpSRMode->m_pILexPronounce2) ) 
		Information ("unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_READ,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test AddTo through main object, QueryLexicons failed", hRes);
		}
		else {
			// check each lexicon by shifting through each bit
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++)	{
				if( dwQueryLex & dwLexicons ) {
					hRes = gpSRMode->m_pILexPronounce2->GetFrom(dwQueryLex,CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
					// check failure condition
					if( !(hRes == LEXERR_ENGBUFTOOSMALL || hRes == LEXERR_PRNBUFTOOSMALL || (hRes == LEXERR_NOTINLEX && dwQueryLex == ILP2_USER) || dwPronounceNeeded != 0) ) {
						Failed ("Main object GetFrom did not indicate return buffer too small or PronounceNeeded not set");
						bMainPass = FALSE;
					}
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}
		 
	hRes = gpSRGramComp->QueryLexicons(QUERY_READ,&dwLexicons);
	if( FAILED(hRes) ) {
		Warning( "unable to test AddTo through main object, QueryLexicons failed", hRes);
	}
	else {
		// check each lexicon by shifting through each bit
		DWORD dwQueryLex = 0;
		for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++)	{
			if( dwQueryLex & dwLexicons ) {
				hRes = gpSRGramComp->GetFrom(dwQueryLex,CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
				// check failure condition
				if( !(hRes == LEXERR_ENGBUFTOOSMALL || hRes == LEXERR_PRNBUFTOOSMALL || (hRes == LEXERR_NOTINLEX && dwQueryLex == ILP2_USER) || dwPronounceNeeded != 0) ) {
					Failed ("Main object GetFrom did not indicate return buffer too small or PronounceNeeded not set");
					bMainPass = FALSE;
				}
			}
			dwQueryLex = SETBIT(iCount);
		}
	}
	// cleanup
	free (pEngineInfo);
	ClearMainGrammar();

	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}

// Lex2: AddTo pronunciations - attempt to AddTo and RemoveFrom LTS
void CALLBACK Test170(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// in
	WCHAR *pszText = L"TestNown";
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  

	// test main object
	if( !(gpSRMode->m_pILexPronounce2) ) 
		Information ("unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce2->AddTo(ILP2_LTS,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
		if( SUCCEEDED(hRes) ) {
			Failed ("Main object returned no error on AddTo LTS lexicon");
			bMainPass = FALSE;
		}
		//try and delete an entry
		hRes = gpSRMode->m_pILexPronounce2->RemoveFrom(ILP2_LTS,pszText, 0);
		if( SUCCEEDED(hRes) ) {
			Failed ("Main object returned no error on RemoveFrom LTS lexicon");
			bMainPass = FALSE;
		}
	}
		
	pszText = L"TestNown2";

	// test grammer object
	hRes = gpSRGramComp->AddTo(ILP2_LTS,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
	if( SUCCEEDED(hRes) ) {
		Failed ("Grammer object returned no error on AddTo LTS lexicon");
		bGramPass = FALSE;
	}
	
	// attempT to an delete entry
	hRes = gpSRGramComp->RemoveFrom(ILP2_LTS,pszText, 0);
	if( SUCCEEDED(hRes) ) {
		Failed ("Grammer object returned no error on RemoveFrom LTS lexicon");
		bGramPass = FALSE;
	}
		
	// clean up
	ClearMainGrammar();
	
	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}

// Lex2: AddTo pronunciations - valid word
void CALLBACK Test171(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// in
	WCHAR *pszText = L"TestNown";
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  

	// out
	DWORD dwLexicons = 0;

	// test main object
	if( !(gpSRMode->m_pILexPronounce2) ) 
		Information ("unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test AddTo through main object, QueryLexicons failed", hRes);
		}
		else {
			// check each lexicon by shifting through each bit
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++)	{
				if( dwQueryLex & dwLexicons ) {
					hRes = gpSRMode->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
					if( FAILED(hRes) ) {
						Failed ("Main object failed to add valid word", hRes);
						bMainPass = FALSE;
					}
					else {
						//delete entry
						hRes = gpSRMode->m_pILexPronounce2->RemoveFrom(dwQueryLex,pszText, 0);
						if( FAILED(hRes) ) {
							Warning ("Passed although Remove returned error when trying to remove word added", hRes);
						}
					}
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}
		
	pszText = L"TestNown2";
	
	// test grammer object
	hRes = gpSRGramComp->QueryLexicons(QUERY_WRITE,&dwLexicons);
	if( FAILED(hRes) ) {
		Warning( "unable to test AddTo in grammer object, QueryLexicons failed",hRes);
	}
	else {
		// check each lexicon by shifting through each bit
		DWORD dwQueryLex = 0;
		for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
			if( dwQueryLex & dwLexicons ) {
				hRes = gpSRGramComp->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
				if( FAILED(hRes)) {
					Failed ("Grammer object failed to add valid word", hRes);
					bGramPass = FALSE;
				}
				else {
					//delete entry
					hRes = gpSRGramComp->RemoveFrom(dwQueryLex,pszText, 0);
					if(FAILED(hRes)) {
						Warning ("Passed although Remove returned error when trying to remove word added", hRes);
					}
				}
			}
			dwQueryLex = SETBIT(iCount);
		}
	}
	
	// clean up
	ClearMainGrammar();
	
	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}

// Lex2: AddTo pronunciations - known invalid word, duplicate
void CALLBACK Test172(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);
	HRESULT  hRes;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	WCHAR *pszText = L"helow";
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  

	DWORD dwPronounceNeeded = 0;
	VOICEPARTOFSPEECH	PartOfSpeech = VPS_UNKNOWN;
	PVOID	pEngineInfo = malloc (14 * 64);
	DWORD dwEngineInfoNeeded = 0;
	DWORD	dwLexicons = 0;
	WORD wSense = 0;

   if( !(gpSRMode->m_pILexPronounce2) ) 
		Information ("unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) 
			Warning ( "unable to test main object: QueryLexicons failed", hRes);
		else {
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
				if( dwQueryLex & dwLexicons ) {  //add same word twice, should get error second time
					hRes = gpSRMode->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_ADJECTIVE,NULL,0);
					if( FAILED(hRes) ) 
						Failed("failed to add valid word through main object",hRes);
					else {
						hRes = gpSRMode->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_ADJECTIVE,NULL,0);
						if( hRes != LEXERR_ALREADYINLEX) {
							if( SUCCEEDED(hRes) ) 
								Failed ("no error return on adding invalid word through main object");
							else
								Failed ("wrong error returned on add of invalid word through main object",hRes);
						}
					}
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}

	// check through grammer object
	hRes = gpSRGramComp->QueryLexicons(QUERY_WRITE,&dwLexicons);
	if( FAILED(hRes) ) {
		Warning( "unable to test main object: QueryLexicons failed", hRes);
	}
	else {
		DWORD dwQueryLex = 0;
		for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
			if( dwQueryLex & dwLexicons ) {  //add same word twice, should get error second time
				hRes = gpSRGramComp->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_ADJECTIVE,NULL,0);
				if( FAILED(hRes) ) 
					Failed("failed to add valid word through grammer object",hRes);
				else {
					hRes = gpSRGramComp->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_ADJECTIVE,NULL,0);
					if( hRes != LEXERR_ALREADYINLEX) {
						if( SUCCEEDED(hRes) ) 
							Failed ("no error return on adding invalid word through grammer object");
						else
							Failed ("wrong error returned on add of invalid word through grammer object",hRes);
					}
				}
			}
			dwQueryLex = SETBIT(iCount);
		}
	}

	//clean up
	ClearMainGrammar();
	free (pEngineInfo);
	
	Passed();

	return;
}

// Lex2: AddTo pronunciations - valid word, get it
void CALLBACK Test173(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// Add in
	WCHAR *pszText = L"TestNown";
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  

	// Get in 
	WORD wSense = 0;

 	// out
	DWORD dwPronounceNeeded;
	VOICEPARTOFSPEECH PartOfSpeech = VPS_UNKNOWN;
	PVOID pEngineInfo;
	pEngineInfo = malloc (14 * 64);
	DWORD dwEngineInfoNeeded = 0;
	DWORD dwLexicons = 0;

	if( !(gpSRMode->m_pILexPronounce2) ) {
		Information ("unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	}
	else {
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		else {
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
				if( dwQueryLex & dwLexicons ) {
					hRes = gpSRMode->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
					if( FAILED(hRes) ) 
						Warning ("Failed to add valid word in main object", hRes);
					else
					{
						WCHAR szPron[512];
						hRes = gpSRMode->m_pILexPronounce2->GetFrom(dwQueryLex,CHARSET_IPAPHONETIC,pszText,wSense,szPron,sizeof(szPron),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
						if( FAILED(hRes) ) {
							Failed ("Failed to get added word in main object", hRes);
							bMainPass = FALSE;
						}
					}
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}
	
	// check grammer object
	hRes = gpSRGramComp->QueryLexicons(QUERY_WRITE,&dwLexicons);  //get writeable lexicons
	if( FAILED(hRes) ) {
		Warning( "unable to test grammer object: QueryLexicons failed", hRes);
	}
	else {
		DWORD dwQueryLex = 0;
		for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
			if( dwQueryLex & dwLexicons ) {
				hRes = gpSRGramComp->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
				if( FAILED(hRes) )
					Warning ("Failed to add valid word in Grammer object", hRes);
				else
				{
					WCHAR szPron[512];
					hRes = gpSRGramComp->GetFrom(dwQueryLex,CHARSET_IPAPHONETIC,pszText,wSense,szPron,sizeof(szPron),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
					if(FAILED(hRes)) {
						Failed ("Failed to get added word in Grammer object", hRes);
						bGramPass = FALSE;
					}
				}
			}
			dwQueryLex = SETBIT(iCount);
		}
	}

	// clean up
	free (pEngineInfo);
	ClearMainGrammar();

	if( bGramPass & bMainPass ) {
		Passed();
	}

	return;
}


// Lex2: AddTo pronunciations - invalid param
void CALLBACK Test174(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// Add in
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  

	// Get in 
	WORD wSense = 0;

 	// Get out
	DWORD *pdwPronounceNeeded = NULL;
	VOICEPARTOFSPEECH *pPartOfSpeech = NULL;
	DWORD dwLexicons = 0;
	
	// check main object
	if( !(gpSRMode->m_pILexPronounce2) ) 
		Information ("Unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		else {
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
				if( dwQueryLex & dwLexicons ) {
					hRes = gpSRMode->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,NULL,szPronounce,VPS_NOUN,NULL,0);
					if( hRes != E_INVALIDARG ) {
						Failed ("Failed to return E_INVALIDARG for add of NULL word in main object", hRes);
						bMainPass = FALSE;
					}
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}

	// check grammer object
	hRes = gpSRGramComp->QueryLexicons(QUERY_WRITE,&dwLexicons);  //get writeable lexicons
	if( FAILED(hRes) ) {
		Warning( "unable to test grammer object: QueryLexicons failed", hRes);
	}
	else {
		DWORD dwQueryLex = 0;
		for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
			if( dwQueryLex & dwLexicons ) {
				hRes = gpSRGramComp->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,NULL,szPronounce,VPS_NOUN,NULL,0);
				if( hRes != E_INVALIDARG ) {
						Failed ("Failed to return E_INVALIDARG for add of NULL word in Grammer object", hRes);
						bGramPass = FALSE;
				}
			}
			dwQueryLex = SETBIT(iCount);
		}
	}

	// clean up
	ClearMainGrammar();

	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}

// Lex2: QueryLexicons - verify all writable lex's are read lex's.
void CALLBACK Test175(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;

	// out
	DWORD dwWriteLexicons = 0;
	DWORD dwReadLexicons = 0;

   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// check main object
	if( !(gpSRMode->m_pILexPronounce2) ) 
		Information ("Unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwWriteLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_READ,&dwReadLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		if( (dwWriteLexicons & dwReadLexicons) != dwWriteLexicons) {
			bMainPass = FALSE;
		}
	}

	
	// check grammer object
	hRes = gpSRGramComp->QueryLexicons(QUERY_WRITE,&dwWriteLexicons);  //get writeable lexicons
	if( FAILED(hRes) ) {
		Warning( "unable to test grammer object: QueryLexicons failed", hRes);
	}
	hRes = gpSRGramComp->QueryLexicons(QUERY_READ,&dwReadLexicons);  //get writeable lexicons
	if( FAILED(hRes) ) {
		Warning( "unable to test grammer object: QueryLexicons failed", hRes);
	}
	if( (dwWriteLexicons & dwReadLexicons) != dwWriteLexicons) {
		bGramPass = FALSE;
	}

	// clean up
	ClearMainGrammar();

	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}

// Lex2: ChangeSpelling - known valid word
void CALLBACK Test176(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;
	WCHAR *pszOld = L"Hello";
	WCHAR *pszNew = L"Hello";
	DWORD dwLexicons = 0;


   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");


	// check main object
	if( !(gpSRMode->m_pILexPronounce2) ) 
		Information ("Unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		else {
			DWORD dwQueryLex = 0;	
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
				if( dwQueryLex & dwLexicons ) {
					hRes = gpSRMode->m_pILexPronounce2->ChangeSpelling(dwQueryLex, pszOld, pszNew);
					if( hRes != LEXERR_NOTINLEX && FAILED(hRes) ) {
						Failed ("Failed to change spelling with valid word with main object", hRes);
						bMainPass = FALSE;
					}
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}


	// check grammer object
	hRes = gpSRGramComp->QueryLexicons(QUERY_WRITE,&dwLexicons);  //get writeable lexicons
	if( FAILED(hRes) ) {
		Warning( "unable to test grammer object: QueryLexicons failed", hRes);
	}
	else {
		DWORD dwQueryLex = 0;
		for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
			if( dwQueryLex & dwLexicons ) {
				hRes = gpSRGramComp->ChangeSpelling(dwQueryLex, pszOld, pszNew);
				if( hRes != LEXERR_NOTINLEX && FAILED(hRes) ) {
					Failed ( "Failed to change spelling with valid word in grammer object", hRes);
					bGramPass = FALSE;
				}
			}
			dwQueryLex = SETBIT(iCount);
		}
	}

	// clean up
	ClearMainGrammar();

	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}	

// Lex2: ChangeSpelling - known invalid word
void CALLBACK Test177(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;
	WCHAR *pszOld = L"abcdefghijklmnopqrstuvwxyz";
	WCHAR *pszNew = L"zyzwvutsrqponmlkjihgfedcba";
	DWORD dwLexicons = 0;


   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");


	// check main object
	if( !(gpSRMode->m_pILexPronounce2) ) 
		Information ("Unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		else {
			DWORD dwQueryLex = 0;	
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
				if( dwQueryLex & dwLexicons ) {
					hRes = gpSRMode->m_pILexPronounce2->ChangeSpelling(dwQueryLex, pszOld, pszNew);
					if( SUCCEEDED(hRes) ) {
						Failed ("Failed to return error on attempt change spelling of invalid word in main object");
						bMainPass = FALSE;
					}
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}


	// check grammer object
	hRes = gpSRGramComp->QueryLexicons(QUERY_WRITE,&dwLexicons);  //get writeable lexicons
	if( FAILED(hRes) ) {
		Warning( "unable to test grammer object: QueryLexicons failed", hRes);
	}
	else {
		DWORD dwQueryLex = 0;
		for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
			if( dwQueryLex & dwLexicons ) {
				hRes = gpSRGramComp->ChangeSpelling(dwQueryLex, pszOld, pszNew);
				if( SUCCEEDED(hRes) ) {
					Failed ( "Failed to return error on attempt change spelling of invalid word in grammer object");
					bGramPass = FALSE;
				}
			}
			dwQueryLex = SETBIT(iCount);
		}
	}

	// clean up
	ClearMainGrammar();

	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}	
		
// Lex2: ChangeSpelling - invalid param
void CALLBACK Test178(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;
	WCHAR *pszOld = L"Hello";
	WCHAR *pszNew = L"Hello";
	DWORD dwLexicons = 0;


   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");


	// check main object
	if( !(gpSRMode->m_pILexPronounce2) ) 
		Information ("Unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		else {
			DWORD dwQueryLex = 0;	
			// check each lexicon
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
				if( dwQueryLex & dwLexicons ) {
					
					// pass NULL as value to change
					hRes = gpSRMode->m_pILexPronounce2->ChangeSpelling(dwQueryLex, NULL, pszNew);
					if( SUCCEEDED(hRes) ) {
						Failed ("Failed to return error with invalid param in main object");
						bMainPass = FALSE;
					}
					
					// pass NULL as value to change to
					hRes = gpSRMode->m_pILexPronounce2->ChangeSpelling(dwQueryLex, pszOld, NULL);
					if( SUCCEEDED(hRes) ) {
						Failed ("Failed to return error with invalid param in main object");
						bMainPass = FALSE;
					}
				}
				dwQueryLex = SETBIT(iCount);
			}

			// pass invalid number for lexicon
			hRes = gpSRMode->m_pILexPronounce2->ChangeSpelling(-1, pszOld, pszNew);
			if( SUCCEEDED(hRes) ) {
				Failed ("Failed to return error with invalid param in main object");
				bMainPass = FALSE;
			}

		}
	}


	// check grammer object
	hRes = gpSRGramComp->QueryLexicons(QUERY_WRITE,&dwLexicons);  //get writeable lexicons
	if( FAILED(hRes) ) {
		Warning( "unable to test grammer object: QueryLexicons failed", hRes);
	}
	else {
		DWORD dwQueryLex = 0;
		for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
			if( dwQueryLex & dwLexicons ) {
				// pass NULL as value to change
				hRes = gpSRGramComp->ChangeSpelling(dwQueryLex, NULL, pszNew);
				if( SUCCEEDED(hRes) ) {
					Failed ("Failed to return error with invalid param in grammer object");
					bMainPass = FALSE;
				}
				
				// pass NULL as value to change to
				hRes = gpSRGramComp->ChangeSpelling(dwQueryLex, pszOld, NULL);
				if( SUCCEEDED(hRes) ) {
					Failed ("Failed to return error with invalid param in grammer object");
					bMainPass = FALSE;
				}
			}
			dwQueryLex = SETBIT(iCount);
		}

		// pass invalid number for lexicon
		hRes = gpSRGramComp->ChangeSpelling(-1, pszOld, pszNew);
		if( SUCCEEDED(hRes) ) {
			Failed ("Failed to return error with invalid param in grammer object");
			bMainPass = FALSE;
		}
	}

	// clean up
	ClearMainGrammar();

	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}	

// Lex2: ChangeSpelling - known valid word, change back
void CALLBACK Test179(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	bool bMainPass = TRUE;	
	bool bGramPass = TRUE;
	WCHAR *pszOld = L"Hello";
	WCHAR *pszNew = L"Hollo";
	DWORD dwLexicons = 0;


   // load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");


	// check main object
	if( !(gpSRMode->m_pILexPronounce2) ) 
		Information ("Unable to test: gpSRMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpSRMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		else {
			DWORD dwQueryLex = 0;	
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
				if( dwQueryLex & dwLexicons ) {
					hRes = gpSRMode->m_pILexPronounce2->ChangeSpelling(dwQueryLex, pszOld, pszNew);
					if( FAILED(hRes) ) {
						if( !(hRes == LEXERR_NOTINLEX && dwQueryLex == ILP2_USER) ) {
							Failed ( "Failed to change spelling with valid word in grammer object", hRes);
							bGramPass = FALSE;
						}
					}
					else {			// change back
						hRes = gpSRMode->m_pILexPronounce2->ChangeSpelling(dwQueryLex, pszNew, pszOld);
						if( FAILED(hRes) ) {
							Failed ("Failed to change spelling back with valid word with main object", hRes);
							bMainPass = FALSE;
						}
					}
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}


	// check grammer object
	hRes = gpSRGramComp->QueryLexicons(QUERY_WRITE,&dwLexicons);  //get writeable lexicons
	if( FAILED(hRes) ) {
		Warning( "unable to test grammer object: QueryLexicons failed", hRes);
	}
	else {
		DWORD dwQueryLex = 0;
		for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
			if( dwQueryLex & dwLexicons ) {
				hRes = gpSRGramComp->ChangeSpelling(dwQueryLex, pszOld, pszNew);
				if( FAILED(hRes) ) {
					if( !(hRes == LEXERR_NOTINLEX && dwQueryLex == ILP2_USER) ) {
						Failed ( "Failed to change spelling with valid word in grammer object", hRes);
						bGramPass = FALSE;
					}
				}
				else {				// change back
					hRes = gpSRGramComp->ChangeSpelling(dwQueryLex, pszNew, pszOld);
					if( FAILED(hRes) ) {
						Failed ( "Failed to change spelling back with valid word in grammer object", hRes);
						bGramPass = FALSE;
					}
				}
			}
			dwQueryLex = SETBIT(iCount);
		}
	}

	// clean up
	ClearMainGrammar();

	if( bMainPass & bGramPass ) {
		Passed();
	}

	return;
}	


// IResGraphEx - NodeStartGet
void CALLBACK Test180(void)
{
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);
	HRESULT hRes;

   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

	DWORD dwNodeID;

   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak
	hRes = SpeakToSR ("all work and no play makes Jack a dull boy");
   CHECKFAILED ("SpeakToSR");

   // listen
   hRes = WaitForPhraseFinish();
   if (hRes)
      Warning ("Engine didn't cope with simple phrase");

	if (gpSRResult) {
		if (!(gpSRResult->m_pISRResGraphEx)) {
			Information ("Unable to test: gpSRMode->m_pISRResGraphEx is NULL");
		}
		else {  // test
			hRes = gpSRResult->NodeStartGet( &dwNodeID );
			if (FAILED(hRes))
				Failed ("Failed to get starting node",hRes);

			hRes = gpSRResult->NodeStartGet( NULL );
			if (hRes != AUDERR_INVALIDPARAM)
				Failed ("Failed to return correct error on pass of NULL param", hRes);
		}
	}
	
   ClearMainGrammar();
   ClearSRAudio();

   Passed();

	return;
}


// IResGraphEx - NodeEndGet
void CALLBACK Test181(void)
{
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);
	HRESULT hRes;

   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

	DWORD dwNodeID;

   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak
	hRes = SpeakToSR ("all work and no play makes Jack a dull boy");
   CHECKFAILED ("SpeakToSR");

   // listen
   hRes = WaitForPhraseFinish();
   if (hRes)
      Warning ("Engine didn't cope with simple phrase");

	if (gpSRResult) {
		if (!(gpSRResult->m_pISRResGraphEx)) {
			Information ("Unable to test: gpSRMode->m_pISRResGraphEx is NULL");
		}
		else {  // test
			hRes = gpSRResult->NodeEndGet( &dwNodeID );
			if (FAILED(hRes))
				Failed ("Failed to get ending node",hRes);

			hRes = gpSRResult->NodeEndGet( NULL );
			if (SUCCEEDED(hRes))
				Failed ("Failed to return error on pass of NULL param", hRes);
		}
	}
	
   ClearMainGrammar();
   ClearSRAudio();

   Passed();

	return;
}


// IResGraphEx - ArcEnum
void CALLBACK Test182(void)
{
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);
	HRESULT hRes;

   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

	// variable specific to ISRResGraph
	DWORD dwNodeID;
	DWORD aArc[100];
	memset(aArc, 0, sizeof(aArc));
	DWORD dwNum;

   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak
	hRes = SpeakToSR ("all work and no play makes Jack a dull boy");
   CHECKFAILED ("SpeakToSR");

   // listen
   hRes = WaitForPhraseFinish();
   if (hRes)
      Warning ("Engine didn't cope with simple phrase");

	if (gpSRResult) {
		if (!(gpSRResult->m_pISRResGraphEx))
			Information ("Unable to test: gpSRMode->m_pISRResGraphEx is NULL");
		else {  // test
			hRes = gpSRResult->NodeStartGet( &dwNodeID );
			if (FAILED(hRes))
				Failed ("Failed to get starting node",hRes);
			else {		// Outgoing
				hRes = gpSRResult->ArcEnum( dwNodeID, aArc, 100*sizeof(DWORD), &dwNum, TRUE);
				if (FAILED(hRes))
					Failed ("Failed to enumerate outgoing arcs", hRes);
							// Incoming
				hRes = gpSRResult->ArcEnum( dwNodeID, aArc, 100*sizeof(DWORD), &dwNum, FALSE);
				if (FAILED(hRes))
					Failed ("Failed to enumerate incoming arcs", hRes);

				// check for invalid param's
				hRes = gpSRResult->ArcEnum( dwNodeID, aArc, 100*sizeof(DWORD), NULL, FALSE);
				if (SUCCEEDED(hRes))
					Failed ("Failed to return error on invalid param");

			}
		}
	}
	
   ClearMainGrammar();
   ClearSRAudio();

   Passed();

	return;
}

// IResGraphEx - GetAllArcs
void CALLBACK Test183(void)
{
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);
	HRESULT hRes;

   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

	// variable specific to ISRResGraph
	DWORD *padwArcID;
	DWORD dwSize = 0;
	DWORD dwNumArcs;
	DWORD dwNeeded;

   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak
	hRes = SpeakToSR ("all work and no play makes Jack a dull boy");
   CHECKFAILED ("SpeakToSR");

   // listen
   hRes = WaitForPhraseFinish();
   if (hRes)
      Warning ("Engine didn't cope with simple phrase");

	if (gpSRResult) {
		if (!(gpSRResult->m_pISRResGraphEx))
			Information ("Unable to test: gpSRMode->m_pISRResGraphEx is NULL");
		else {  // Get the size of Array required and allocate it
			hRes = gpSRResult->GetAllArcs(0, 0, NULL, &dwNeeded);
			if (dwNeeded==0)
				Failed ("Failed to get number of arcs needed", hRes);
			else
				if( (padwArcID = new DWORD[dwNeeded]) == NULL ) {
					Information ("Out of memory for DWORD array.");
					return;
				}
				dwSize = dwNeeded;
				hRes = gpSRResult->GetAllArcs(padwArcID, dwSize, &dwNumArcs, &dwNeeded);
				if( FAILED(hRes) )
					Failed ("Failed on valid call to GetAllArcs",hRes);

				// check invalid params
				hRes = gpSRResult->GetAllArcs(padwArcID, NULL, &dwNumArcs, &dwNeeded);
				if( SUCCEEDED(hRes) )
					Failed ("Failed to return error for invalid param");

				hRes = gpSRResult->GetAllArcs(padwArcID, dwSize, NULL, &dwNeeded);
				if( SUCCEEDED(hRes) )
					Failed ("Failed to return error for invalid param");

				hRes = gpSRResult->GetAllArcs(NULL, dwSize, &dwNumArcs, NULL);
				if( SUCCEEDED(hRes) )
					Failed ("Failed to return error for invalid param");
		}
	}
		
		// clean up
	if( padwArcID )
		delete [] padwArcID;

	
	ClearMainGrammar();
   ClearSRAudio();

   Passed();

	return;
}


// IResGraphEx - GetAllNodes
void CALLBACK Test184(void)
{
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);
	HRESULT hRes;

   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

	// variable specific to ISRResGraph
	DWORD *padwNodeID;
	DWORD dwSize = 0;
	DWORD dwNumNodes;
	DWORD dwNeeded;

   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak
	hRes = SpeakToSR ("all work and no play makes Jack a dull boy");
   CHECKFAILED ("SpeakToSR");

   // listen
   hRes = WaitForPhraseFinish();
   if (hRes)
      Warning ("Engine didn't cope with simple phrase");

	if (gpSRResult) {
		if (!(gpSRResult->m_pISRResGraphEx))
			Information ("Unable to test: gpSRMode->m_pISRResGraphEx is NULL");
		else {  // Get the size of Array required and allocate it
			hRes = gpSRResult->GetAllNodes(NULL, NULL, NULL, &dwNeeded);
			if (dwNeeded==0)
				Failed ("Failed to get number of arcs needed", hRes);
			else {		// allocate the memory for all the nodes needed
				if( (padwNodeID = new DWORD[dwNeeded]) == NULL ) {
					Information ("Out of memory for DWORD array.");
					return;
				}
				dwSize = dwNeeded;  //get all the nodes
				hRes = gpSRResult->GetAllNodes(padwNodeID, dwSize, &dwNumNodes, &dwNeeded);
				if( FAILED(hRes) )
					Failed ("Failed on valid call to GetAllNodes",hRes);

				// check invalid params
				hRes = gpSRResult->GetAllNodes(padwNodeID, NULL, &dwNumNodes, &dwNeeded);
				if( SUCCEEDED(hRes) )
					Failed ("Failed to return error for invalid param");

				hRes = gpSRResult->GetAllNodes(padwNodeID, dwSize, NULL, &dwNeeded);
				if( SUCCEEDED(hRes) )
					Failed ("Failed to return error for invalid param");

				hRes = gpSRResult->GetAllNodes(NULL, dwSize, &dwNumNodes, NULL);
				if( SUCCEEDED(hRes) )
					Failed ("Failed to return error for invalid param");
			}
		}
	}
		
		// clean up
	if( padwNodeID )
		delete [] padwNodeID;

			
	ClearMainGrammar();
   ClearSRAudio();

   Passed();

	return;
}


// IResGraphEx - NodeGet - get all nodes from enumerated arcs
void CALLBACK Test185(void)
{
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);
	HRESULT hRes;

   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

	// variable specific to ISRResGraph
	DWORD dwNode;
	DWORD dwNodeID;
	DWORD aArc[100];
	memset(aArc, 0, sizeof(aArc));
	DWORD dwNum;

   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak
	hRes = SpeakToSR ("all work and no play makes Jack a dull boy");
   CHECKFAILED ("SpeakToSR");

   // listen
   hRes = WaitForPhraseFinish();
   if (hRes)
      Warning ("Engine didn't cope with simple phrase");

	if (gpSRResult) {
		if (!(gpSRResult->m_pISRResGraphEx))
			Information ("Unable to test: gpSRMode->m_pISRResGraphEx is NULL");
		else {  
			hRes = gpSRResult->NodeStartGet( &dwNodeID );
			if (FAILED(hRes))
				Failed ("Failed to get starting node",hRes);
			else {		// Outgoing
				hRes = gpSRResult->ArcEnum( dwNodeID, aArc, sizeof(aArc), &dwNum, TRUE);
				if (FAILED(hRes)) 
					Failed ("Failed to enumerate outgoing arcs", hRes);
				else { // NodeGet for each arc
					for( DWORD dwCount = 0; dwCount<dwNum; dwCount++)
					{	// nodes to the right
						hRes = gpSRResult->NodeGet(aArc[dwCount], &dwNode, TRUE);
						if( FAILED(hRes) )
							Failed ("Failed to get valid node",hRes);
						// nodes to the left
						hRes = gpSRResult->NodeGet(aArc[dwCount], &dwNode, FALSE);
						if( FAILED(hRes) )
							Failed ("Failed to get valid node",hRes);
					
						// check invalid parameters
						hRes = gpSRResult->NodeGet(aArc[dwCount], NULL, FALSE);
						if( SUCCEEDED(hRes) )
							Failed ("Failed to return error for invalid parameter");
					}
				}
				
					// Incoming
				hRes = gpSRResult->ArcEnum( dwNodeID, aArc, sizeof(aArc), &dwNum, FALSE);
				if (FAILED(hRes))
					Failed ("Failed to enumerate outgoing arcs", hRes);
				else { // NodeGet for each arc
					for( DWORD dwCount = 0; dwCount<dwNum; dwCount++)
					{
						// nodes to the right
						hRes = gpSRResult->NodeGet(aArc[dwCount], &dwNode, TRUE);
						if( FAILED(hRes) )
							Failed ("Failed to get valid node",hRes);
						// nodes to the left
						hRes = gpSRResult->NodeGet(aArc[dwCount], &dwNode, FALSE);
						if( FAILED(hRes) )
							Failed ("Failed to get valid node",hRes);

						// check invalid parameters
						hRes = gpSRResult->NodeGet(aArc[dwCount], NULL, FALSE);
						if( SUCCEEDED(hRes) )
							Failed ("Failed to return error for invalid parameter");

					}
				}
			}
		}
	}
			


   ClearMainGrammar();
   ClearSRAudio();

   Passed();

	return;
}

// IResGraphEx - DWORDGet - get attribute of node
void CALLBACK Test186(void)
{
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);
	HRESULT hRes;

   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

	// variable specific to ISRResGraph
	DWORD *padwNodeID;
	DWORD dwSize = 0;
	DWORD dwNumNodes;
	DWORD dwNeeded;
	DWORD dwValue = 0;

   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak
	hRes = SpeakToSR ("all work and no play makes Jack a dull boy");
   CHECKFAILED ("SpeakToSR");

   // listen
   hRes = WaitForPhraseFinish();
   if (hRes)
      Warning ("Engine didn't cope with simple phrase");

	if (gpSRResult) {
		if (!(gpSRResult->m_pISRResGraphEx))
			Information ("Unable to test: gpSRMode->m_pISRResGraphEx is NULL");
		else {  // Get the size of Array required and allocate it
			hRes = gpSRResult->GetAllNodes(NULL, NULL, NULL, &dwNeeded);
			if (dwNeeded==0)
				Failed ("Failed to get number of arcs needed", hRes);
			else {
				if( (padwNodeID = new DWORD[dwNeeded]) == NULL ) {
					Information ("Out of memory for DWORD array.");
					return;
				}
				dwSize = dwNeeded;		// get all the nodes so that we can check each of them
				hRes = gpSRResult->GetAllNodes(padwNodeID, dwSize, &dwNumNodes, &dwNeeded);
				if( FAILED(hRes) )
					Failed ("Failed on valid call to GetAllNodes",hRes);
				else {		// loop through each node and make sure can get the attributes
					for (DWORD dwCount=0; dwCount<dwNumNodes; dwCount++) {		
						hRes = gpSRResult->DWORDGet(padwNodeID[dwCount], SRGARC_ACOUSTICSCORE, &dwValue);
						if( FAILED(hRes) )
							Failed ("Failed to get attribute of node",hRes);
					}
				}
			}
		}
	}

	if( padwNodeID )
		delete [] padwNodeID;

   ClearMainGrammar();
   ClearSRAudio();

   Passed();

	return;
}
			
// IResGraphEx - DataGet - get data for all nodes
void CALLBACK Test187(void)
{
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);
	HRESULT hRes;

   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

	// variable specific to ISRResGraph
	DWORD *padwNodeID;
	DWORD dwSize = 0;
	DWORD dwNumNodes;
	DWORD dwNeeded;
	SDATA sData;

	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak
	hRes = SpeakToSR ("all work and no play makes Jack a dull boy");
   CHECKFAILED ("SpeakToSR");

   // listen
   hRes = WaitForPhraseFinish();
   if (hRes) {
      Failed ("Engine didn't cope with simple phrase, unable to complete test", hRes);
		goto cleanup;
	}

	if (gpSRResult) {
		if (!(gpSRResult->m_pISRResGraphEx)) {
			Information ("Unable to test: gpSRMode->m_pISRResGraphEx is NULL");
			goto cleanup;
		}

		// Get the size of Array required
		hRes = gpSRResult->GetAllNodes(NULL, NULL, NULL, &dwNeeded);
		if (dwNeeded==0) {
			Failed ("Failed to get number of arcs needed", hRes);
			goto cleanup;
		}

			// allocate memory for array to hold all nodes
		if( (padwNodeID = new DWORD[dwNeeded]) == NULL ) {
			Information ("Out of memory for DWORD array.");
			goto cleanup;
		}

			// get all the nodes so that we can check each of them
		dwSize = dwNeeded;
		hRes = gpSRResult->GetAllNodes(padwNodeID, dwSize, &dwNumNodes, &dwNeeded);
		if( FAILED(hRes) ) {
			Failed ("Failed on valid call to GetAllNodes",hRes);
			goto cleanup;
		}

			// loop through each node and make sure can get the data
		for (DWORD dwCount=0; dwCount<dwNumNodes; dwCount++) {		
			hRes = gpSRResult->DataGet(padwNodeID[dwCount], SRGNODE_TIME, &sData); 
			if( FAILED(hRes) )
				Failed ("Failed to get node-time attribute of node",hRes);
		}
	}

	cleanup:

	if( padwNodeID ) 
		delete [] padwNodeID;
	
	CoTaskMemFree( sData.pData ); 

   ClearMainGrammar();
   ClearSRAudio();

   Passed();

	return;
}

// IResGraphEx - DataGet - get data for all arcs
void CALLBACK Test188(void)
{
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);
	HRESULT hRes;
	NEEDCFG;
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   

	// variable specific to ISRResGraph
	DWORD *padwArcID = NULL;
	DWORD dwSize;
	DWORD dwNumArcs;
	DWORD dwNeeded;
	SDATA sData;

   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak
	hRes = SpeakToSR ("hello world");
   CHECKFAILED ("SpeakToSR");

   // listen
   hRes = WaitForPhraseFinish();
   if (hRes) {
      Failed ("Engine didn't cope with simple phrase, unable to complete test", hRes);
		goto cleanup;
	}

	if (gpSRResult) {
		if (!(gpSRResult->m_pISRResGraphEx)) {
			Information ("Unable to test: gpSRMode->m_pISRResGraphEx is NULL");
			goto cleanup;
		}

			// Get the size of Array required
		hRes = gpSRResult->GetAllArcs(NULL, NULL, NULL, &dwNeeded);
		if (hRes != SRERR_NOTENOUGHDATA) {
			Failed ("Expected a SRERR_NOTENOUGHDATA result code", hRes);
			goto cleanup;
		}

      if (dwNeeded == 0) {
			Failed ("Result object with no arcs");
			goto cleanup;
      }
			// allocate memory for array to hold all arcs
		if( (padwArcID = (DWORD *) malloc(dwNeeded)) == NULL ) {
			Information ("Out of memory for DWORD array.");
			goto cleanup;
		}

	   // get all the arcs so that we can check each of them
		dwSize = dwNeeded;
		hRes = gpSRResult->GetAllArcs(padwArcID, dwSize, &dwNumArcs, &dwNeeded);
		if( FAILED(hRes) ) {
			Failed ("Failed on valid call to GetAllArcs",hRes);
			goto cleanup;
		}

		// loop through each arcs and make sure can get the word text
		for (DWORD dwCount=0; dwCount<dwNumArcs; dwCount++) {		
			hRes = gpSRResult->DataGet(padwArcID[dwCount], SRGARC_WORDTEXT, &sData); 
         if (sData.pData) {
	         CoTaskMemFree( sData.pData ); 
				sData.pData = NULL;
			}
			if( FAILED(hRes) )
				Failed ("Failed to get word text attribute of arc",hRes);
		}

      // loop through each arcs and make sure can get the non-lexical text
		for (dwCount=0; dwCount<dwNumArcs; dwCount++) {		
			hRes = gpSRResult->DataGet(padwArcID[dwCount], SRGARC_NONLEXICALTEXT, &sData); 
         if (sData.pData) {
	         CoTaskMemFree( sData.pData ); 
				sData.pData = NULL;
			}
			if( FAILED(hRes) )
				Failed ("Failed to get non-lexical attribute of arc",hRes);
		}

      // loop through each arcs and make sure can get the pronunciation
		for (dwCount=0; dwCount<dwNumArcs; dwCount++) {		
			hRes = gpSRResult->DataGet(padwArcID[dwCount], SRGARC_WORDPRONUNCIATION, &sData); 
         if (sData.pData) {
	         CoTaskMemFree( sData.pData ); 
				sData.pData = NULL;
			}
			if( FAILED(hRes) )
				Failed ("Failed to get pronunciation attribute of arc",hRes);
		}
	}

	cleanup:

   free(padwArcID);

   ClearMainGrammar();
   ClearSRAudio();

   Passed();

	return;
}

// IResGraphEx - ScoreGet - get score for all single arcs
void CALLBACK Test189(void)
{
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);
	HRESULT hRes;

   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

	// variable specific to ISRResGraph
	DWORD *padwArcID = NULL;
	DWORD dwSize;
	DWORD dwNumArcs;
	DWORD dwNeeded;

	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak
	hRes = SpeakToSR ("all work and no play makes jack a dull boy");
   CHECKFAILED ("SpeakToSR");

   // listen
   hRes = WaitForPhraseFinish();
   if (hRes) {
      Failed ("Engine didn't cope with simple phrase, unable to complete test", hRes);
		goto cleanup;
	}

	if (gpSRResult) {
		if (!(gpSRResult->m_pISRResGraphEx)) {
			Information ("Unable to test: gpSRMode->m_pISRResGraphEx is NULL");
			goto cleanup;
		}

			// Get the size of Array required
		hRes = gpSRResult->GetAllArcs(NULL, NULL, NULL, &dwNeeded);
		if (hRes != SRERR_NOTENOUGHDATA) {
			Failed ("Expected a SRERR_NOTENOUGHDATA result code", hRes);
			goto cleanup;
		}

      if (dwNeeded == 0) {
			Failed ("Result object with no arcs");
			goto cleanup;
      }
			// allocate memory for array to hold all arcs
		if( (padwArcID = (DWORD *) malloc(dwNeeded)) == NULL ) {
			Information ("Out of memory for DWORD array.");
			goto cleanup;
		}

	   // get all the arcs so that we can check each of them
		dwSize = dwNeeded;
		hRes = gpSRResult->GetAllArcs(padwArcID, dwSize, &dwNumArcs, &dwNeeded);
		if( FAILED(hRes) ) {
			Failed ("Failed on valid call to GetAllArcs",hRes);
			goto cleanup;
		}

			// loop through each arcs and make sure can get the score
		DWORD dwScore = 0;
		DWORD dwTempArcID;
		for (DWORD dwCount=0; dwCount<dwNumArcs; dwCount++) {
			dwTempArcID = padwArcID[dwCount];
			hRes = gpSRResult->ScoreGet(SRGEX_ACOUSTICONLY, &dwTempArcID, 1, 0, 1, &dwScore);
			if( FAILED(hRes) )
				Failed ("Failed to get word text attribute of arc",hRes);
		}

	}

	cleanup:

   free(padwArcID);

   ClearMainGrammar();
   ClearSRAudio();

   Passed();

	return;
}

// IResGraphEx - BestPathEnum - get paths and score them
void CALLBACK Test190(void)
{
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);
	HRESULT hRes;

   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;

	// variable specific to ISRResGraph
	DWORD dwStartPath;
	DWORD dwEndPath;
	int   nScore;
#if 0
	int   nOldScore;
#endif
   SDATA sData;
	DWORD dwPathIndexCount;

 
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("SetMainGrammar");

	// speak
	hRes = SpeakToSR ("tovess");
   CHECKFAILED ("SpeakToSR");


   // listen
   hRes = WaitForPhraseFinish();
   if (hRes) {
      Failed ("Engine didn't cope with simple phrase, unable to complete test", hRes);
		goto cleanup;
	}

	if (gpSRResult) {
		if (!(gpSRResult->m_pISRResGraphEx)) {
			Information ("Unable to test: gpSRMode->m_pISRResGraphEx is NULL");
			goto cleanup;
		}
		
		// get starting node for possible best paths
		hRes = gpSRResult->NodeStartGet( &dwStartPath );
		if (FAILED(hRes)) {
			Failed ("Failed to get starting node",hRes);
			goto cleanup;
		}

		// get ending node for possible best paths
		hRes = gpSRResult->NodeEndGet( &dwEndPath );
		if (FAILED(hRes)) {
			Failed ("Failed to get ending node",hRes);
			goto cleanup;
		}

		//check for invalid params, NULL for SDATA
		hRes = gpSRResult->BestPathEnum (0, &dwStartPath, 1, &dwEndPath, 1, FALSE, NULL);
		if( SUCCEEDED(hRes) )
			Failed ("BestPathEnum succeeded with invalid params");
		
		// get some best paths, without requirement that they start or end on the starting or ending nodes
		for(DWORD dwRank=0; dwRank<5;dwRank++) {

			// check that best path is sorted by score
			hRes = gpSRResult->BestPathEnum (dwRank, NULL, 1, NULL, 1, FALSE, &sData);
			if( hRes == E_UNEXPECTED )
				break;

			if( FAILED(hRes) ) {
				Failed ("Failed to retrieve path", hRes);
				goto cleanup;
			}
			// get score for path in sData
			dwPathIndexCount = sData.dwSize / sizeof(DWORD);
			if( sData.pData != NULL ) {
				hRes = gpSRResult->ScoreGet(SRGEX_ACOUSTICONLY, (DWORD*) sData.pData, 
                                        dwPathIndexCount, 0, dwPathIndexCount, 
                                        (DWORD *) &nScore);
				if( FAILED(hRes) ) 
					Failed ("Failed to get score",hRes);
#if 0
				else	{	// compare scores, top ranks should always have higher scores than lower ranks
					if( dwRank	> 1 ) {
						if( nOldScore < nScore )
							Failed ("lower ranked path had higher score then next higher ranked path");
					}
					nOldScore = nScore;
				}
#endif

				if (sData.pData) {
					CoTaskMemFree( sData.pData ); 
					sData.pData = NULL;
				}
			}
		}
	}
			

	cleanup:

   ClearMainGrammar();
   ClearSRAudio();

   Passed();

	return;
}

// make sure can load word spotting grammer
void CALLBACK Test191(void)

{
   NEEDENGLISH;
   NEEDCFG;

   HRESULT hRes;
   hRes = SetMainGrammar (gszWordSpotGrammar);
   ClearMainGrammar();
   if (FAILED(hRes)) {
      Failed ("unable to load a word spotting grammar");
		return;
	}

	if (!(gSRModeInfo.dwFeatures & SRFEATURE_WILDCARD)) 
      Information ("SRMODEINFO claims that word spotting is not supported, but it's possible to load a word spotting grammar");
  

	NEEDAUDIO;
 
   char *psz;

   psz = SpeakAndListen ("could you help me make a long distance call please, thank you",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG for word spotting grammer");

   psz = SpeakAndListen ("long distance",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("long distance call",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("Make it collect to my mother",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("make it collect",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("we need to make a person to person call",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("I want to fly to Arizona.",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("I want to fly to Arizona right now.",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("Arizona",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("send me to New York.",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("Mister Smith could you come over here.",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("Misses Jones could you come over here.",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("Misses Jones",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");

   psz = SpeakAndListen ("computer I want to send mail by way of Mister Jones now",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");


	Passed();
	
	return;
}

// Lex: Add pronunciations - fill user dictionary
// note: it is recommmeded that ILexPronounce2::AddTo used instead
void CALLBACK Test192(void)
{
	NEEDSRINTERFACE (SRI_ILEXPRONOUNCE);
	HRESULT  hRes;

		
	// load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	// Add in
	WCHAR pszText[64];
		// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  
	DWORD dwAddCount = 0;
	DWORD dwRemoveCount = 0;
	char buffer[40];

	if( !(gpSRMode->m_pILexPronounce) ) {
		Warning ("unable to test: gpSRMode->m_pILexPronounce is NULL");
		goto cleanup;
	}

	// add words until failure
	do {

		// word to add is count in base 36, all alpha-numeric characters.
		_ultow(dwAddCount, pszText, 36); 

		// attempt to adds word to main object
		hRes = gpSRMode->m_pILexPronounce->Add(CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
		if( SUCCEEDED(hRes) )
			dwAddCount++;

	} while (SUCCEEDED(hRes));

	// record words added
	_itoa( dwAddCount, buffer, 10 );
	strcat( buffer, " words added." );
	Information (buffer);

	//remove all words added
	do {

		// word to remove is count in base 36, all alpha-numeric characters.
		_ultow(dwRemoveCount, pszText, 36); 

		// removes words, assumes sense is 0
		hRes = gpSRMode->m_pILexPronounce->Remove(pszText,0);
		if( SUCCEEDED(hRes) ) {
			dwRemoveCount++;
		}

	} while (SUCCEEDED(hRes) && dwAddCount > dwRemoveCount );

	// record words removeds
	_itoa( dwRemoveCount, buffer, 10 );
	strcat( buffer, " words removed." );
	Information (buffer);
	
	

	// clean up
cleanup:

  	ClearMainGrammar();

	Passed();

	return;
}

// IAttributes - DWORDGet
void CALLBACK Test193(void)
{
	NEEDSRINTERFACE (SRI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwAttrib = 0;
	DWORD dwValue = 0;
	DWORD dwTempValue = 0;
		
	// load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	if( !(gpSRMode->m_pIAttributes) ) {
		Warning ("unable to test: gpSRMode->m_pISRAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of an invalid argument
	hRes = gpSRMode->m_pIAttributes->DWORDGet (NULL, &dwValue);
	if (hRes != E_INVALIDARG)
		Failed("m_pIAttributes->DWORDGet did not return invalid argument on NULL attrib",hRes);
		
	// try to get all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
		hRes = gpSRMode->m_pIAttributes->DWORDGet (dwAttrib, &dwValue);
      if( FAILED(hRes) && hRes != E_INVALIDARG && hRes != E_NOTIMPL )
	      Failed("DWORDGet Failed with unknown error",hRes);

		// check the returned a value by called second time a making sure it's the same.
		if( SUCCEEDED(hRes)) {
			dwTempValue = dwValue + 1;
			hRes = gpSRMode->m_pIAttributes->DWORDGet (dwAttrib, &dwTempValue);
			if( dwValue != dwTempValue )
				Failed( "DWORDGet Indicated success but did not return a value");
		}
	}


		// cleanup
cleanup:

	ClearMainGrammar();

	Passed();
	return;
}

// IAttributes - DWORDSet - try all values and make sure they work
void CALLBACK Test194(void)
{
	NEEDSRINTERFACE (SRI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwAttrib = 0;
	DWORD dwOldValue = 0;
	DWORD dwValue = 0;

			
	// load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	if( !(gpSRMode->m_pIAttributes) ) {
		Warning ("unable to test: gpSRMode->m_pISRAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of an invalid argument
	hRes = gpSRMode->m_pIAttributes->DWORDSet (NULL, dwValue);
	if (hRes != E_INVALIDARG)
		Failed("m_pIAttributes->DWORDSet did not return invalid argument on NULL attrib",hRes);
		
	// try to set all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
		hRes = gpSRMode->m_pIAttributes->DWORDGet (dwAttrib, &dwValue);	
		if( SUCCEEDED(hRes) ) {
			// make it works first
			hRes = gpSRMode->m_pIAttributes->DWORDSet (dwAttrib, dwValue);
         if (dwAttrib == SRATTR_RESULTSINFO_POSSIBLE && hRes != E_ACCESSDENIED)
				Failed("Unexpected result code for SRATTR_RESULTSINFO_POSSIBLE",hRes);
         else
            continue;

			if( FAILED(hRes)) 
				Failed("was able to DWORDGet attribute but could not DWORDSet it",hRes);
			else {
				// set to new value check that it got set
				dwOldValue = dwValue;
				dwValue++;
				hRes = gpSRMode->m_pIAttributes->DWORDSet (dwAttrib, dwValue);
				if( FAILED(hRes) )
					Failed( "failed to DWORDSet valid attribute",hRes);
				
				// change value slightly to insure it get's a value and that it's the correct value
				dwValue++;
				hRes = gpSRMode->m_pIAttributes->DWORDGet (dwAttrib, &dwValue);
				if( FAILED(hRes) || (dwValue != dwOldValue + 1) )
					Failed( "DWORDSet failed to set value",hRes);

				// set back to the way we found it
				hRes = gpSRMode->m_pIAttributes->DWORDSet (dwAttrib, dwOldValue);
				if( FAILED(hRes) )
					Failed( "unable to set attribute back to original value",hRes);
			}
		}

      if( FAILED(hRes) && hRes != E_INVALIDARG && hRes != E_NOTIMPL ) 
 		   Failed("DWORDSet Failed with unknown error",hRes);
	}


		// cleanup
cleanup:

	ClearMainGrammar();

	Passed();
	return;
}

// IAttributes - DWORDSet/Get - check specific attributes
void CALLBACK Test195(void)
{
	NEEDSRINTERFACE (SRI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwValue;
	DWORD dwOldValue;

	// load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	if( !(gpSRMode->m_pIAttributes) ) {
		Warning ("unable to test: gpSRMode->m_pISRAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of SRATTR_TIMEOUT_COMPLETE attribute

	// get the current setting
	hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_TIMEOUT_COMPLETE, &dwOldValue);
	if( FAILED(hRes) )
		Failed("Failed to get timeout_complete attribute value",hRes);
	
	// try to change attribute and check that it stays changed
	hRes = gpSRMode->m_pIAttributes->DWORDSet (SRATTR_TIMEOUT_COMPLETE, dwOldValue + 1);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set timeout_complete back to starting value",hRes);
	else {
		hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_TIMEOUT_COMPLETE, &dwValue);
		if( FAILED(hRes) || dwValue != dwOldValue + 1 )
			Failed("Failed to set timeout_complete value",hRes);
	}

	// set it back to old value
	hRes = gpSRMode->m_pIAttributes->DWORDSet (SRATTR_TIMEOUT_COMPLETE, dwOldValue);
	if( FAILED(hRes) && hRes != E_NOTIMPL )
		Failed("Failed to set timeout_complete back to starting value",hRes);

	// double check that it got set correctly
	hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_TIMEOUT_COMPLETE, &dwValue);
	if( FAILED(hRes) || dwValue != dwOldValue )
		Failed("Failed to set timeout_complete back to starting value",hRes);


	// cleanup
cleanup:

	ClearMainGrammar();

	Passed();

	return;
}


// IAttributes - StringGet
void CALLBACK Test196(void)
{
	NEEDSRINTERFACE (SRI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwAttrib;
	WCHAR szString[1024];
	DWORD dwSize = 0;
	DWORD dwNeeded = 0;

	// load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	if( !(gpSRMode->m_pIAttributes) ) {
		Warning ("unable to test: gpSRMode->m_pISRAttributes is NULL");
		goto cleanup;
	}

	// check proper handle of invalid parameter
	hRes = gpSRMode->m_pIAttributes->StringGet (NULL, szString, dwSize, &dwNeeded);
	if (hRes != E_INVALIDARG)
		Failed("StringGet did not return invalid argument on NULL attrib",hRes);

		// try to set all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
		dwNeeded = 0;

		// get buffer size needed for string
		hRes = gpSRMode->m_pIAttributes->StringGet (dwAttrib, szString, 0, &dwNeeded);

		// should return one of two condition, otherwise it's a failure
		if( hRes != E_INVALIDARG && hRes != E_BUFFERTOOSMALL && hRes != E_NOTIMPL )
			Failed ("Failed to return correct hRes, expect E_INVALIDARG,E_NOTIMPL or E_BUFFERTOOSMALL", hRes);
	
			// check for handle of invalid parameter for valid attribs
		if( hRes == E_BUFFERTOOSMALL ) {
#define  FILL_CHARACTER    0xFB
				// check that recieved information in string when all params are valid
			_wcsnset(szString, FILL_CHARACTER, sizeof(szString));
			hRes = gpSRMode->m_pIAttributes->StringGet (dwAttrib, szString, dwNeeded, &dwNeeded);
			if ( FAILED(hRes) )
				Failed ("Failed to get string",hRes);
			else {
				if( szString[0] == FILL_CHARACTER ) // string didn't get populated with data
					Failed ("String did not get populated with data");
			}
		}
				// check for invalid argument handling of each attribute
		// check for invalid parameter of szString
		hRes = gpSRMode->m_pIAttributes->StringGet (dwAttrib, NULL, dwNeeded, &dwNeeded);
		if (SUCCEEDED(hRes))
			Failed("StringGet did not fail for a NULL szString param",hRes);

		// check invalid dwNeeded param
		hRes = gpSRMode->m_pIAttributes->StringGet (dwAttrib, szString, 0, NULL);
		if (hRes != E_INVALIDARG)
			Failed("StringGet did not return invalid argument when dwNeeded pointer is NULL",hRes);
	}

	// cleanup
cleanup:

	ClearMainGrammar();
	
	Passed();
	return;
}

// IAttributes - StringSet
void CALLBACK Test197(void)
{
	NEEDSRINTERFACE (SRI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwNeeded = 0;
	DWORD dwAttrib = 0;
	WCHAR szString[1024];

	// load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	if( !(gpSRMode->m_pIAttributes) ) {
		Warning ("unable to test: gpSRMode->m_pISRAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of an invalid argument
	hRes = gpSRMode->m_pIAttributes->StringSet (NULL, szString);
	if (hRes != E_INVALIDARG)
		Failed("m_pIAttributes->StringSet did not return invalid argument on NULL attrib",hRes);
	
		// check invalid szString param
	hRes = gpSRMode->m_pIAttributes->StringSet (dwAttrib, NULL);
	if (hRes != E_INVALIDARG)
		Failed("StringGet did not return invalid argument when szString pointer is NULL",hRes);


	// try to set all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
		dwNeeded = 0;

		// get buffer size needed for string
		hRes = gpSRMode->m_pIAttributes->StringGet (dwAttrib, szString, 0, &dwNeeded);

		// should return one of two condition, otherwise it's a failure
		if( hRes != E_INVALIDARG && hRes != E_BUFFERTOOSMALL && hRes != E_NOTIMPL ) {
			Failed ("Failed StringGet with unexpected error, cannot complete test", hRes);
			continue;
		}
	
			// get current string for attribute
		if( hRes == E_BUFFERTOOSMALL ) {
				// check that recieved information in string when all params are valid
			_wcsnset(szString, FILL_CHARACTER, sizeof(szString));
			hRes = gpSRMode->m_pIAttributes->StringGet (dwAttrib, szString, dwNeeded, &dwNeeded);
			if ( FAILED(hRes) ) {
				Failed ("Failed to get string for this attrib, cannot complete test",hRes);
				continue;
			}
			else {
				if( szString[0] == FILL_CHARACTER ) {// string didn't get populated with data
					Failed ("String did not get populated with data, cannot complete test");
					continue;
				}
			}

			// now that we have the string associated with the attrib, we can try to set it to a new value
			// the new value is the same as the old so it should be valid and acceptable
			hRes = gpSRMode->m_pIAttributes->StringSet (dwAttrib, szString);
			if( FAILED(hRes) )
				Failed ("Failed to set string to the same value it already has",hRes);
		
		}
	}

		// cleanup
cleanup:
	
	ClearMainGrammar();


	Passed();
	return;
}

// IAttribute - MemoryGet
void CALLBACK Test198(void)
{
	NEEDSRINTERFACE (SRI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwNeeded = 0;
	DWORD dwAttrib = 0;
	DWORD dwSize = 0;
	void * pMem = NULL;

	// load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	if( !(gpSRMode->m_pIAttributes) ) {
		Warning ("unable to test: gpSRMode->m_pISRAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of an invalid argument
	hRes = gpSRMode->m_pIAttributes->MemoryGet (NULL, &pMem, &dwSize);
	if( SUCCEEDED(hRes) )
		Failed ("Failed to return an error with NULL attribute");

	// try to set all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
	
		dwSize = 0;
		// check that returns correct error regardless of attribute supported or not
		hRes = gpSRMode->m_pIAttributes->MemoryGet (dwAttrib, &pMem, &dwSize);
		if( !(SUCCEEDED(hRes) || hRes == E_INVALIDARG || hRes == E_WRONGTYPE || 
				hRes == E_ACCESSDENIED || hRes == E_OUTOFMEMORY || hRes == E_NOTIMPL) )
			Failed ("incorrect return value", hRes);

		if( SUCCEEDED(hRes)) {
			// if success check dwSize for value
			if( dwSize == 0 )
				Failed ("indicated success yet dwSize not set");

			// if success check that pointer points somewhere
			if( !pMem )
				Failed ("indicated success yet memory pointer not set");
		}
	
		// clean up before getting any more
		if( pMem ) {
			CoTaskMemFree( pMem );
			pMem = NULL;
		}
	}

cleanup:

	if( pMem )
		CoTaskMemFree( pMem );


	ClearMainGrammar();

	Passed();
	return;
}

// IAttribute - MemorySet
void CALLBACK Test199(void)
{
	NEEDSRINTERFACE (SRI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwNeeded = 0;
	DWORD dwAttrib = 0;
	DWORD dwSize = 0;
	void * pMem = NULL;

	// load grammer object
	hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

	if( !(gpSRMode->m_pIAttributes) ) {
		Warning ("unable to test: gpSRMode->m_pISRAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of an invalid argument
	hRes = gpSRMode->m_pIAttributes->MemoryGet (NULL, &pMem, &dwSize);
	if( SUCCEEDED(hRes) )
		Failed ("Failed to return an error with NULL attribute");

	// try to set all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
	
		dwSize = 0;
		// check that returns correct error regardless of attribute supported or not
		hRes = gpSRMode->m_pIAttributes->MemoryGet (dwAttrib, &pMem, &dwSize);
		if( !(SUCCEEDED(hRes) || hRes == E_INVALIDARG || hRes == E_WRONGTYPE || 
				hRes == E_ACCESSDENIED || hRes == E_OUTOFMEMORY || hRes == E_NOTIMPL) )
			Failed ("uncorrect return value", hRes);

		if( SUCCEEDED(hRes)) {
			// if success check dwSize for value
			if( dwSize == 0 )
				Failed ("indicated success yet dwSize not set");

			// if success check that pointer points somewhere
			if( !pMem )
				Failed ("indicated success yet memory pointer not set");


			// try to set memory
			hRes = gpSRMode->m_pIAttributes->MemorySet (dwAttrib, pMem, dwSize);
			if( !(SUCCEEDED(hRes) || hRes == E_INVALIDARG || hRes == E_WRONGTYPE || 
				hRes == E_ACCESSDENIED || hRes == E_OUTOFMEMORY || hRes == E_NOTIMPL) ) {

				Failed ("Failed on MemorySet",hRes);
			}
		}
	
		// clean up before getting any more
		if( pMem ) {
			CoTaskMemFree( pMem );
			pMem = NULL;
		}
	}

cleanup:

	if( pMem )
		CoTaskMemFree( pMem );


	ClearMainGrammar();

	Passed();
	return;
}














