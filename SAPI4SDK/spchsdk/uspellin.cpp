/************************************************************************
Spelling.cpp - Simple control to ask for a word spelled out

   This includes the interface:
      IClassFactory
      IUnknown

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
#include <stdio.h>
#include <objbase.h>
#include <objerror.h>
#include <spchwrap.h>
#include "telctl.h"
#include "resource.h"

static WCHAR gszStart[] = L"AskSpelling";

CSpelling::CSpelling()
{
   ClearEnteredData();
   InitUndo();
   InitMisrecognizedList();
}

CSpelling::~CSpelling()
{
   if(m_szParse) {
      for(unsigned int i = 0; i < wcslen(m_szParse); i++)
         free(m_rgszPronunciations[i]);
      free(m_rgszPronunciations);
      free(m_szParse);
   }

   ClearMisrecognizedList();
   ClearUndoData();
}

PCWSTR CSpelling::GetControlName(void)
{
   return L"Spelling";
}

DWORD CSpelling::GetDefaultTextResID(LANGID langID)
{
   // since only have one, return the english one no matter what
   return IDR_SPELLING;
}

void CSpelling::GetCLSID(CLSID *pCLSID)
{
#ifdef BUILDREALCONTROL
   *pCLSID = CLSID_SpellingControl;
#else
   *pCLSID = CLSID_SampleSpellingControl;
#endif
}


void CSpelling::ClearEnteredData() {
   m_szThisPronunciation = 0;
   m_dwRecog = 0;
   m_dwParseSize = 0;
   m_szParse = 0;
   m_rgszPronunciations = 0;
   m_bIsCorrection = false;
}


//
// functions to do with phrase parsing
//


void CSpelling::OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                               DWORD dwParseMemSize, PSRPHRASEW ,
                               LPUNKNOWN lpUnkResult) {
   PISRRESBASICW pISRResBasic;
   HRESULT hRes;

   // note that something parsed OK so that OnState knows
   m_bNewAction = true;

   // make sure the parse is allocated
   if(!pParseMem) {
      DoState(4); // TODO: do we want to bail?
      return;
   }

   // initialize
   m_dwRecog = dwParseID;
   m_bIsCorrection = false;

   // handle DTMF specially because it can never be confused
   if(IsDTMFString((WCHAR *)pParseMem)) {
      ParseDTMFString((WCHAR *)pParseMem);
      DoState(m_iThisCmd);
      return;
   }

   // get the ISRResBasic object
   if(FAILED(hRes = lpUnkResult->QueryInterface(IID_ISRResBasicW, (PVOID *)&pISRResBasic)))
      return;

   for(DWORD dwRank = 0; ; dwRank++) {
      DWORD       dwParseSize;
      DWORD       dwCommand;
      WCHAR      *szPhraseBuffer;
      WCHAR       szSomeBuffer[1024];
      PSRPHRASEW  pSRPhrase = (PSRPHRASEW)szSomeBuffer;

      // get the phrase for this rank
      hRes = pISRResBasic->PhraseGet(dwRank, (PSRPHRASEW)pSRPhrase, sizeof(szSomeBuffer), &dwParseSize);
      if(hRes == SRERR_VALUEOUTOFRANGE) // then we're out of alternatives
         break;
      else if(hRes) // then there were some other errors
         break;
      if(FAILED(hRes = m_pGramComp->PhraseParse(pSRPhrase, &dwCommand, (WCHAR **)(&szPhraseBuffer), &dwParseSize)))
         break;

      // parse the phrase
      if(szPhraseBuffer) {
         WCHAR *szThisPhrase = szPhraseBuffer;

         if(!wcsncmp(szThisPhrase, L"noISaid", 7))
            m_bIsCorrection = true, szThisPhrase += 8;
         else
            ClearMisrecognizedList();
         ParsePhrase(szThisPhrase);
         CoTaskMemFree(szPhraseBuffer);
      }

      // if this phrase isn't "no I said" or isn't the same as before, break
      if(!m_bIsCorrection || !WasAlreadyMisrecognized(m_iThisCmd, m_chThis))
         break;

      // clean up
      if(m_szThisPronunciation)
         free(m_szThisPronunciation), m_szThisPronunciation = 0;
   }

   // if we didn't get a good parse, complain to the user
   if(hRes == SRERR_VALUEOUTOFRANGE) {
      m_bIsCorrection = false; // just in case
      DoState(1);
   }

   // else if we got some random error, bail
   else if(hRes)
      DoState(4);

   // otherwise, do this command
   else {
      // it's added to the misrecognized list because it *could* be
      //   misrecognized, not because it actually is
      AddToMisrecognizedList(m_iThisCmd, m_chThis);
      DoState(m_iThisCmd);
   }
}


bool CSpelling::IsDTMFString(WCHAR *szThisPhrase) {
   return ((szThisPhrase[0] >= L'0') && (szThisPhrase[0] <= L'9') && (szThisPhrase[1] != L'N')) ? true : false;
}


void CSpelling::ParseDTMFString(WCHAR *szThisPhrase) {
   WCHAR *rgchOneChars   = L"adgjmptw";
   WCHAR *rgchTwoChars   = L"bdhknrux";
   WCHAR *rgchThreeChars = L"cfilosvz";
   WCHAR *rgchFourChars  = L"    q  z";
   WCHAR ch;

   // first check that they're within range
   if((szThisPhrase[1] == L'0') ||
      (szThisPhrase[1] >  L'4') ||
      ((szThisPhrase[1] == L'4') && (szThisPhrase[0] != L'6') && (szThisPhrase[0] != L'9'))) {
      m_iThisCmd = 5;
      DoState(m_iThisCmd);
   }

   switch(szThisPhrase[1]) {
   case L'1': ch = rgchOneChars[(int)(szThisPhrase[0] - L'2')]; break;
   case L'2': ch = rgchTwoChars[(int)(szThisPhrase[0] - L'2')]; break;
   case L'3': ch = rgchThreeChars[(int)(szThisPhrase[0] - L'2')]; break;
   case L'4': ch = rgchFourChars[(int)(szThisPhrase[0] - L'2')]; break;
   }
   m_iThisCmd = 10;
   DoAlphabeticChar(ch, false);
}


void CSpelling::ParsePhrase(WCHAR *szThisPhrase) {

   // init m_chThis to nothing
   m_chThis = L'\0';

   // take care of special keywords
        if(!wcscmp(szThisPhrase, L"nothing"))         m_iThisCmd = 2;
   else if(!wcscmp(szThisPhrase, L"finished"))        m_iThisCmd = 3;
   else if(!wcscmp(szThisPhrase, L"delete"))          m_iThisCmd = 20;
   else if(!wcscmp(szThisPhrase, L"recap"))           m_iThisCmd = 21;
   else if(!wcscmp(szThisPhrase, L"restart"))         m_iThisCmd = 22;
   else if(!wcscmp(szThisPhrase, L"instructions"))    m_iThisCmd = 30;
   else if(!wcscmp(szThisPhrase, L"tutorial"))        m_iThisCmd = 31;
   else if(!wcscmp(szThisPhrase, L"listCommands"))    m_iThisCmd = 32;
   else if(!wcscmp(szThisPhrase, L"listWords"))       m_iThisCmd = 33;
   else if(!wcscmp(szThisPhrase, L"listPunctuation")) m_iThisCmd = 34;
   else if(!wcscmp(szThisPhrase, L"learnTouchtone"))  m_iThisCmd = 35;

   // if it's not a keyword, then it's a character
   else {
      WCHAR szAsIn[32];
      DWORD dwSize = 31;
      size_t parselen = wcslen(szThisPhrase);

      m_iThisCmd = 10;

      GetValue(L"Internal", L"AsIn", szAsIn, &dwSize);
      if(wcsstr(szThisPhrase, szAsIn))
         ParseAsInPhrase(szThisPhrase);

      else if((szThisPhrase[parselen-1] == L'N') ||
              (szThisPhrase[parselen-1] == L'P'))
         ParseNumOrPunc(szThisPhrase);

      else
         ParseAlphabeticChar(szThisPhrase);
   }
}


void CSpelling::ParseAsInPhrase(WCHAR *szThisPhrase) {

   if((m_szThisPronunciation = (WCHAR *)malloc((wcslen(szThisPhrase) + 2) * sizeof(WCHAR))) != 0) {
      if(!wcsncmp(szThisPhrase, L"capital", 7)) { // TODO: localize this
         m_chThis = towupper(szThisPhrase[8]);
         wcsncpy(m_szThisPronunciation, szThisPhrase, 9);
         m_szThisPronunciation[9] = L'\0';
         wcscat(m_szThisPronunciation, L".");
         wcscat(m_szThisPronunciation, szThisPhrase + 9);
      }
      else {
         m_chThis = szThisPhrase[0];
         m_szThisPronunciation[0] = szThisPhrase[0];
         m_szThisPronunciation[1] = L'.';
         wcscpy(&(m_szThisPronunciation[2]), &(szThisPhrase[1]));
      }
   }
   else
      m_chThis = L'\0';
}


void CSpelling::ParseNumOrPunc(WCHAR *szThisPhrase) {
   size_t parselen = wcslen(szThisPhrase);

   if((m_szThisPronunciation = (WCHAR *)malloc(parselen * sizeof(WCHAR))) != 0) {
      wcsncpy(m_szThisPronunciation, szThisPhrase, parselen-1);
      m_szThisPronunciation[parselen-1] = L'\0';
   }
   if(szThisPhrase[parselen-1] == L'P') {
      WCHAR szBuffer[6];
      DWORD dwSize = 5;
      GetValue(L"Internal", m_szThisPronunciation, szBuffer, &dwSize);
      m_chThis = szBuffer[1];
   }
   else
      m_chThis = *szThisPhrase;
}


void CSpelling::ParseAlphabeticChar(WCHAR *szThisPhrase) {
   WCHAR ch;
   bool bCapsFlag;

   if(!wcsncmp(szThisPhrase, L"capital", 7)) { // TODO: localize this
      ch = towupper(szThisPhrase[8]);
      bCapsFlag = true;
   }
   else {
      ch = szThisPhrase[0];
      bCapsFlag = false;
   }
   DoAlphabeticChar(ch, bCapsFlag);
}


void CSpelling::DoAlphabeticChar(WCHAR ch, bool bCapsFlag) {
   WCHAR szAsInTemplate[64], szNameBuffer[64], szBuffer[128];
   DWORD dwSize = 63;
   size_t parselen;

   m_chThis = ch;
   if(bCapsFlag)
      GetValue(L"Internal", L"CapitalAsInTemplate", szAsInTemplate, &dwSize);
   else
      GetValue(L"Internal", L"AsInTemplate", szAsInTemplate, &dwSize);

   szBuffer[0] = towlower(m_chThis);
   szBuffer[1] = L'\0';
   dwSize = 63;
   GetValue(L"Internal", szBuffer, szNameBuffer, &dwSize);
   swprintf(szBuffer, szAsInTemplate, m_chThis, szNameBuffer);
   parselen = wcslen(szBuffer);
   if((m_szThisPronunciation = (WCHAR *)malloc((parselen + 1) * sizeof(WCHAR))) != 0)
      wcscpy(m_szThisPronunciation, szBuffer);
}


//
// OnState and its subsidiaries
//


void CSpelling::OnState(DWORD dwStateID)
{
   if(m_bVerifying)
      m_bVerifying = false;

   // sometimes OnState gets called for no apparent reason--we'll weed those out here
   if(!m_bNewAction && (dwStateID != 0)) {
      TTSSpeak(L"Prompts", L"Continue");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      return;
   }
   m_bNewAction = false;

   // handle corrections
   if(m_bIsCorrection)
      Undo();
   else
      SaveUndoData();

   // apply this command
   ApplyCommand(dwStateID);
}


void CSpelling::ApplyCommand(DWORD dwStateID) {
   WCHAR szFixingPrefix[64];
   DWORD dwSize = 63;

   if(m_bIsCorrection)
      GetValue(L"Prompts", L"FixingPrefix", szFixingPrefix, &dwSize);
   else
      szFixingPrefix[0] = L'\0';

   if(szFixingPrefix[0] != L'\0')
      m_pQueue->Speak(szFixingPrefix);

   switch(dwStateID) {
   case 0: // initialization
      ClearEnteredData();
      ClearMisrecognizedList();
      ClearUndoData();

      // say first-time user instructions
      {
         WCHAR szFirstTimeUser[1024], *pch;
         DWORD dwSize = 1023;
         GetValue(L"Instructions", L"FirstTimeUser", szFirstTimeUser, &dwSize);
         if((pch = wcsstr(szFirstTimeUser, L"%bonk%")) == 0)
            m_pQueue->Speak(szFirstTimeUser);
         else {
            *pch = L'\0';
            pch += 6;
            m_pQueue->Speak(szFirstTimeUser);
            ReadyToListenBeep();
            m_pQueue->Speak(pch);
         }
      }

      TTSSpeak(L"Prompts", L"Main");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 1: // heard same character, even though they said "noISaid"
      TTSSpeak(L"Prompts", L"HeardSame");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      m_bNewAction = false;
      return;

   case 2: // nothing
      m_pQueue->Speak(L"Nothing.");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 3: // finished
      m_bNewAction = true;
      if(m_fVerify && m_szParse) {
         m_bVerifying = true;
         DoVerify(L"VerifyPre", L"VerifyPost", m_szParse, 4, 0);
      }
      else
         DoState(4);
      break;

   case 4: // finished and verified
      if(m_szParse) {
         m_dwParseSize = (wcslen(m_szParse) + 1) * sizeof(WCHAR);
         DoFinish(m_dwRecog, m_szParse, m_dwParseSize);
         free(m_szParse), m_szParse = 0;
      }
      else
         DoFinish(m_dwRecog, L"", 0);
      ClearEnteredData();
      ClearMisrecognizedList();
      ClearUndoData();
      break;

   case 5: // bad DTMF punched in
      m_pQueue->Speak(L"Bad D T M F.");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 10: // a letter, number, or punctuation
      if(m_szThisPronunciation) {
         DoConcatenateChar(m_chThis, m_szThisPronunciation);
         m_pQueue->Speak(m_szThisPronunciation);
         m_szThisPronunciation = 0;
      }
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 20: // delete
      DoDelete(true); // true => say what is being deleted
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 21: // recap
      DoRecap();
      TTSSpeak(L"Prompts", L"Continue");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 22: // restart
      TTSSpeak(L"Prompts", L"Restarting");
      DoRestart();
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 30: // instructions
      TTSSpeak(L"Instructions", L"Instructions");
      TTSSpeak(L"Prompts", L"Continue");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 31: // tutorial
      TTSSpeak(L"Instructions", L"Tutorial");
      TTSSpeak(L"Prompts", L"Continue");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 32: // list commands
      TTSSpeak(L"Instructions", L"ListCommands");
      TTSSpeak(L"Prompts", L"Continue");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 33: // list words
      ListWords();
      TTSSpeak(L"Prompts", L"Continue");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 34: // list punctuation
      TTSSpeak(L"Instructions", L"ListPunctuation");
      TTSSpeak(L"Prompts", L"Continue");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;

   case 35: // learn touchtone
      TTSSpeak(L"Instructions", L"LearnTouchtone");
      TTSSpeak(L"Prompts", L"Continue");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;
   }
}


void CSpelling::DoConcatenateChar(WCHAR ch, WCHAR *szThisPronunciation) {
   WCHAR *szTempParse; // will replace m_szParse
   WCHAR **rgszTempPronunciations; // will replace m_rgszPronunciations
   size_t parsesize, pronsize;

   // first, allocate all the memory we'll need
   parsesize = (m_szParse ? (wcslen(m_szParse) + 2) : 2) * sizeof(WCHAR);
   pronsize =  (m_szParse ? (wcslen(m_szParse) + 1) : 1) * sizeof(WCHAR *);
   if(((szTempParse =            (WCHAR *)malloc(parsesize)) == 0) ||
      ((rgszTempPronunciations = (WCHAR **)malloc(pronsize)) == 0)) {
      DoState(4); // TODO: get out quick?
      return;
   }

   // next, copy over the parse string and append the new character
   if(m_szParse) {
      parsesize = wcslen(m_szParse);
      wcscpy(szTempParse, m_szParse);
   }
   else
      parsesize = 0;
   szTempParse[parsesize] = ch;
   szTempParse[parsesize+1] = L'\0';

   // finally, save away this pronunciation string
   // parsesize is already set from the previous paragraph
   if(m_rgszPronunciations)
      memcpy(rgszTempPronunciations, m_rgszPronunciations, parsesize * sizeof(WCHAR *));
   rgszTempPronunciations[parsesize] = szThisPronunciation;

   // finally, replace the old stuff with the new
   free(m_szParse);
   m_szParse = szTempParse;
   free(m_rgszPronunciations);
   m_rgszPronunciations = rgszTempPronunciations;
}


void CSpelling::DoDelete(bool bSayWhatIsBeingDeleted) {
   size_t len;

   // check that we're not trying to delete nothing
   if(!m_szParse) {
      if(bSayWhatIsBeingDeleted)
         m_pQueue->Speak(L"There is nothing to delete.");
      return;
   }
   len = wcslen(m_szParse);

   // if it is appropriate, say what is being deleted
   if(bSayWhatIsBeingDeleted) {
      WCHAR *pch;
      if((pch = (WCHAR *)malloc((wcslen(m_rgszPronunciations[len-1]) + 9 + 1) * sizeof(WCHAR))) == 0)
         return;
      wcscpy(pch, L"Deleting ");
      wcscat(pch, m_rgszPronunciations[len-1]);
      m_pQueue->Speak(pch);
      free(pch);
   }

   // don't delete the pronunciation because it has been saved as undo information!

   // undelete the character lazily (i.e. don't resize unless we go to zero length)
   if(len == 1) {
      free(m_szParse);
      m_szParse = 0;
      free(m_rgszPronunciations);
      m_rgszPronunciations = 0;
      return;
   }
   else {
      m_szParse[len-1] = L'\0';
      m_rgszPronunciations[len-1] = 0;
   }
}


void CSpelling::ListWords() {
   WCHAR rgchCodeLetters[64], szAsInTemplate[64], szNameBuffer[64], szBuffer[128];
   DWORD dwSize = 63;
   int iNumLetters;

   GetValue(L"Internal", L"CodeLetters", rgchCodeLetters, &dwSize);
   iNumLetters = (int)wcslen(rgchCodeLetters);
   dwSize = 63;
   GetValue(L"Internal", L"AsInTemplate", szAsInTemplate, &dwSize);

   TTSSpeak(L"Prompts", L"BeginListWords");

   for(int i = 0; i < iNumLetters; i++) {
      szBuffer[0] = rgchCodeLetters[i];
      szBuffer[1] = L'\0';
      dwSize = 63;
      GetValue(L"Internal", szBuffer, szNameBuffer, &dwSize);
      m_pQueue->Speak(szNameBuffer);
   }

   TTSSpeak(L"Prompts", L"EndListWords");
}


void CSpelling::ListPunctuation() {
   m_pQueue->Speak(L"List punctuation is not implemented yet.");
}


void CSpelling::DoRecap() {
   if(m_szParse) {
      TTSSpeak(L"Prompts", L"BeginSpelling");
      for(unsigned int i = 0; i < wcslen(m_szParse); i++)
         m_pQueue->Speak(m_rgszPronunciations[i]);
      TTSSpeak(L"Prompts", L"EndSpelling");
   }
   else
      TTSSpeak(L"Prompts", L"SpellingNothing");
}


void CSpelling::DoRestart() {
   ClearEnteredData();
}


//
// OnVerify callback
//


void CSpelling::OnVerify() {
   for(unsigned int i = 0; i < wcslen(m_szParse); i++)
      m_pQueue->Speak(m_rgszPronunciations[i]);
}


//
// functions to do with the misrecognized command list
//


void CSpelling::InitMisrecognizedList() {
   m_iNumMisrecognized = 0;
   m_rgiMisrecognizedCmds = 0;
   m_rgchMisrecognizedChars = 0;
}


bool CSpelling::WasAlreadyMisrecognized(int iCmd, WCHAR ch) {
   for(int i = 0; i < m_iNumMisrecognized; i++)
      if(iCmd == m_rgiMisrecognizedCmds[i] &&
         ((iCmd != 10) || (ch == m_rgchMisrecognizedChars[i])))
         return true;
   return false;
}


void CSpelling::AddToMisrecognizedList(int iCmd, WCHAR ch) {
   int *rgi;
   WCHAR *rgch;

   // allocate the necessary memory
   if(((rgi =  (int *)  malloc((m_iNumMisrecognized+1) * sizeof(int))) == 0) ||
      ((rgch = (WCHAR *)malloc((m_iNumMisrecognized+1) * sizeof(WCHAR))) == 0))
      return;

   // copy over the old stuff if need be
   if(m_iNumMisrecognized > 0) {
      memcpy(rgi, m_rgiMisrecognizedCmds, m_iNumMisrecognized * sizeof(int));
      free(m_rgiMisrecognizedCmds);
      memcpy(rgch, m_rgchMisrecognizedChars, m_iNumMisrecognized * sizeof(WCHAR));
      free(m_rgchMisrecognizedChars);
   }

   // put in the new information
   rgi[m_iNumMisrecognized] = iCmd;
   rgch[m_iNumMisrecognized] = ch;
   m_iNumMisrecognized++;

   // put the new arrays in place
   m_rgiMisrecognizedCmds = rgi;
   m_rgchMisrecognizedChars = rgch;
}


void CSpelling::ClearMisrecognizedList() {
   if(m_iNumMisrecognized > 0) {
      free(m_rgiMisrecognizedCmds);
      m_rgiMisrecognizedCmds = 0;
      free(m_rgchMisrecognizedChars);
      m_rgchMisrecognizedChars = 0;
      m_iNumMisrecognized = 0;
   }
}


//
// functions to do with previous data
//


void CSpelling::InitUndo() {
   m_iUndoCmd = -1;
   m_chUndoData = L'\0';
   m_szUndoData = 0;
   m_rgszUndoData = 0;
   m_iUndoArrayLen = 0;
}


void CSpelling::SaveUndoData() {
   ClearUndoData();

   m_iUndoCmd = m_iThisCmd;
   switch(m_iUndoCmd) {

   case 10: // a character
      // don't save away anything because undo here is the same as delete
      break;

   case 23: // delete
   case 24: // mistake
      if(m_szParse) {
         m_chUndoData = m_szParse[wcslen(m_szParse)-1];
         m_szUndoData = m_rgszPronunciations[wcslen(m_szParse)-1];
      }
      else
         m_chUndoData = L'\0';
      break;

   case 25: // restart
      m_szUndoData = m_szParse;
      m_rgszUndoData = m_rgszPronunciations;
      m_iUndoArrayLen = wcslen(m_szParse);
      break;

   default:
      ; // do nothing
      break;
   }
}


void CSpelling::Undo() {
   switch(m_iUndoCmd) {

   case 10: // a character
      // first, manually get the pronunciation of the last char and free it
      // this is because DoDelete() doesn't normally free it because
      //   DoDelete() is normally called right after a SaveUndoData()
      free(m_rgszPronunciations[wcslen(m_szParse)-1]);
      DoDelete(false); // false => don't say what is being deleted
      break;

   case 23: // delete
   case 24: // mistake
      if(m_chUndoData != L'\0') {
         DoConcatenateChar(m_chUndoData, m_szUndoData);
         m_szUndoData = 0;
      }
      break;

   case 25: // restart
      m_szParse = m_szUndoData;
      m_szUndoData = 0;
      m_rgszPronunciations = m_rgszUndoData;
      m_rgszUndoData = 0;
      m_iUndoArrayLen = 0;
      break;

   default:
      ; // do nothing
      break;
   }

   m_iUndoCmd = -1;
}


void CSpelling::ClearUndoData() {
   m_iUndoCmd = -1;
   m_chUndoData = L'\0';
   if(m_szUndoData)
      free(m_szUndoData), m_szUndoData = 0;
   if(m_rgszUndoData) {
      for(int i = 0; i < m_iUndoArrayLen; i++)
         free(m_rgszUndoData[i]);
      free(m_rgszUndoData);
      m_rgszUndoData = 0;
      m_iUndoArrayLen = 0;
   }
}




void CSpelling::OnAskBack() {
   m_pQueue->Speak(L"Interrupted");
   ReadyToListenBeep();
   if(m_bVerifying)
      StartSRWhenReady(L"Verify");
   else
      StartSRWhenReady(gszStart);
}
