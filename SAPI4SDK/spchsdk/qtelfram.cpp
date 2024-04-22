/*********************************************************************
TelFrame.Cpp - Telephony framwork C++ wrapper objects.

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
#include <stdio.h>
#include "spchwrap.h"

/*************************************************************************
CTelControlFramework - This object is to make generating telephony
controls as easy as possible. It provides a framwork that an application
can use and then subclass
*/

static WCHAR gszPrompts[] = L"Prompts";
static WCHAR gszSettings[] = L"Settings";
static WCHAR gszDTMFMap[] = L"DTMFMap";
static WCHAR gszDTMFMapString[] = L"DTMFString";
static WCHAR gszDTMFCount[] = L"Count";

/* !!!! Initialization function !!! */
/*************************************************************************
CTelControlFramework::CTelControlFramework - The construction initializes
   the default values to zero or default values.
*/

CTelControlFramework::CTelControlFramework (void)
{
   m_pszText = NULL;
   m_pszDefaultText = NULL;
   m_pTextParse = NULL;
   m_pDefaultTextParse = NULL;
   m_fStarted = FALSE;
   m_fIsCompiled = FALSE;
   m_cRef = 0;
   m_plObjCount = NULL;
   m_hModule = NULL;
   m_dwDefaultTextResIDLoaded = 0;
   m_pISRNotifySink = NULL;
   m_pISRGramNotifySink = NULL;
   m_dwSRKey = 0;
   m_pISTTTSQueueNotifySink = NULL;
   m_pITelNotifySink = NULL;
   m_fNotifySinksUsed = FALSE;
   m_pSRMode = NULL;
   m_pGram = NULL;
   m_pGramComp = NULL;
   m_pQueue = NULL;
   m_pLog = NULL;
   m_pTelInfo = NULL;
   m_pITelControlNotifySink = NULL;
   m_dwTimeOut = 0;
   m_dwNoAnswerTime = 10000;  // 10 sec
   m_dwNoAnswer = 0;
   m_dwNoAnswerMax = 3;
   m_lang.LanguageID = 0;
   m_lang.szDialect[0] = 0;
   m_szRuleActive[0] = 0;
   m_fActiveWhenDoneTalking = FALSE;
   m_szRuleToActivate[0] = 0;
   m_fIsSRActive = FALSE;
   m_fFullDuplex = FALSE;
   m_dwCurState = 0;
   m_fActivateWhenDoneTalking = FALSE;
   m_pszPreSpeakItem = NULL;
   m_pszPostSpeakItem = NULL;
   m_pszVerifySpeak = NULL;
   m_dwOnCorrectState = 0;
   m_dwOnWrongState = 0;
   m_fVerify = FALSE;
   m_pTLSpoken = NULL;
   m_DTMFcount = 0;
   m_DTMFString[0] = 0;
   m_fUseTAPIBeep = FALSE;
   m_hCall = NULL;
   m_dwPlayBeep = FALSE;
   m_fDisableRecogBeep = FALSE;
}

/*************************************************************************
CTelControlFramework::~CTelControlFramework - The destructor removes
   notification sinkds from the speech recognition and text to speech objects,
   frees speech recognition if it's there, releases all of the interfaces
   the control knows about.
*/
CTelControlFramework::~CTelControlFramework (void)
{
   StopAllMedia();
   UnregisterNotificationSinks();
   FreeUpControlData();

   // free default text if necessary
   if (m_pszDefaultText) {
      free (m_pszDefaultText);
      m_pszDefaultText = NULL;
   }
   if (m_pDefaultTextParse) {
      delete m_pDefaultTextParse;
      m_pDefaultTextParse = NULL;
   }

   // unregister and delete app notify sink
   if (m_pITelControlNotifySink) {
      m_pITelControlNotifySink->Release();
      m_pITelControlNotifySink = NULL;
   }

   // free up engines and other pointers
   if (m_pSRMode)
      delete m_pSRMode;
   if (m_pGram)
      delete m_pGram;
   if (m_pGramComp)
      delete m_pGramComp;
   if (m_pQueue)
      delete m_pQueue;
   if (m_pLog)
      delete m_pLog;
   if (m_pTelInfo)
      delete m_pTelInfo;
   if (m_pTLSpoken)
      delete m_pTLSpoken;

   // free up notification sinks
   if (m_pISRNotifySink)
      delete m_pISRNotifySink;
   if (m_pISRGramNotifySink)
      delete m_pISRGramNotifySink;
   if (m_pISTTTSQueueNotifySink)
      delete m_pISTTTSQueueNotifySink;
   if (m_pITelNotifySink)
      delete m_pITelNotifySink;

   // decrement count
   if (m_plObjCount)
      InterlockedDecrement(m_plObjCount);

}


/*************************************************************************
CTelControlFramework::Init - This is called by the class factory of the
   object. The init function is passed a pointer to a LONG that will be
   increased by one right away, and decreased by one when the object
   is destroyed. THe value will be modified with InterlockIncreament
   and InterlockDecremement so it's thread safe. The app must also pass
   in the instance handle.

*/
#ifdef STRICT
HRESULT CTelControlFramework::Init (LONG *pObjCount, PVOID hModule)
#else
HRESULT CTelControlFramework::Init (LONG *pObjCount, HMODULE hModule)
#endif
{
   // increase the count so if the object is destroyed it will note this
   m_plObjCount = pObjCount;
   if (m_plObjCount)
      InterlockedIncrement (m_plObjCount);

   // remember hmodule for loading resources
   m_hModule = (HMODULE) hModule;

   // create the notification sink objects
   m_pISRNotifySink = new CTCSRNotifySink (this);
   m_pISRGramNotifySink = new CTCSRGramNotifySink (this);
   m_pISTTTSQueueNotifySink = new CTCSTTTTSQueueNotifySink (this);
   m_pITelNotifySink = new CTCTelNotifySink (this);
   if (!m_pISRNotifySink || !m_pISRGramNotifySink || !m_pISTTTSQueueNotifySink ||
      !m_pITelNotifySink)
      return ResultFromScode (E_OUTOFMEMORY);

   // start out with english defaults
   LoadInDefaultText (1033);

   return NOERROR;
}





/* !!!! parts that will be subclassed !!!! */

/*************************************************************************
CTelControlFramework::GetControlName - Returns a pointer to a WCHAR of
   the control's name. Used for returnning logging.

   THE CONTROL MUST PROVIDE THIS FUNCTION.
*/
PCWSTR CTelControlFramework::GetControlName (void)
{
   static WCHAR szName[] = L"DefaultControl"; // should use a different string
   return szName;
}


/*************************************************************************
CTelControlFramework::GetDefaultTextResID - Returns a resource ID of the
   default text, given a specific language ID. The resources are stored as "TEXT" type.
   They must be unicode.

   THE CONTROL MUST PROVIDE THIS FUNCTION.
*/
DWORD CTelControlFramework::GetDefaultTextResID (LANGID langID)
{
   switch (PRIMARYLANGID(langID)) {
   case LANG_ENGLISH:
      return 0;      // should return a different value
   default:
      return 0;      // should return a different value
   }
}


/*************************************************************************
CTelControlFramework::GetCLSID - Returns a buffer to the classID of the
   control

   THE CONTROL MUST PROVIDE THIS FUNCTION.
*/
void CTelControlFramework::GetCLSID (CLSID *pCLSID)
{
   *pCLSID = GUID_NULL;    // should be a different guid
}

/*************************************************************************
CTelControlFramework::OnState - Called when the telephony control first
   starts with a state of 0. The application should probably start speaking here, and setting
   state information. This might be called based upon other recognitions
   with different state value. A control wishing to change the state should
   call DoState instead of OnState.

   In general, this function should:
      - Set a state variable (if the control has multiple states)
      - Send text down to be spoken. Use TTSSpeak
      - Send a ReadToListenBeep
      - Send a speech recognition grammar. Use StartSRWhenReady.
      - Return

   THE CONTROL MUST PROVIDE THIS FUNCTION.
*/
void CTelControlFramework::OnState (DWORD dwStateID)
{
   // Should fill this in, doing an action depending upon the state.
   // The initial state is 0
   // Use m_dwCurState also.
   return;
}


/*************************************************************************
CTelControlFramework::OnNoAnswer - Called when there's no answer from SR
   after N seconds. Speech recognition and TTS will both be stopped at this
   time. A control is likely to switch to a new state by calling DoState().

   By default this repeats the prompt.
*/
void CTelControlFramework::OnNoAnswer (void)
{
   DoState (m_dwCurState);
}

/*************************************************************************
CTelControlFramework::OnPhraseParse - Called when a recognition has come
   back from the recognizer and has been parsed. The application should
   override this to determine and set which state to go to. Will usually
   switch to a new state or finish by callin DoState()

   THE CONTROL MUST PROVIDE THIS FUNCTION.
*/
void CTelControlFramework::OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                         DWORD dwParseMemSize,
                                         PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult)
{
   // dwParseID is the parse command #
   // pParseMem and dwParseMemSize are the parse results
   // pSRPhrase is the recognized phrase, if the control needs it
   // lpUnkResults is the IUnk for the results object, if the application needs it. May be NULL

   return;
}


/*************************************************************************
CTelControlFramework::OnPhraseUnrecognized - Called when a recognition has come
   back from the recognizer and has been parsed, but it's
   unrecognized. The application should override this to determine
   and set which state to go to. Will usually switch to a new state or
   finish by callin DoState()

   The default behavior is to speak [Prompts] Unrecognized, and then
   help, repeat the current state
*/
void CTelControlFramework::OnPhraseUnrecognized (PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult)
{
   // pSRPhrase is the recognized phrase, if the control needs it. May be NULL
   // lpUnkResults is the IUnk for the results object, if the application needs it. May be NULL

   TTSSpeak (gszPrompts, L"Unrecognized");
   OnAskHelp();

   return;
}



/*************************************************************************
CTelControlFramework::OnDTMF - This function is called when DTMF comes in.
   The application will want to override this and handle the DTMF based
   upon the current state. Will usually switch to a new state or finish
   by calling DoState().

   The default behavious is to look in a table, [DTMFMap], in the control
   to see what DTMF maps to what recognition. If a DTMF appears there it's
   mapped to that phraserecognized number, and then OnPhraseParse
   is called. If it's not present, then default behavious for 0, #, and *
   are used, or it's marked as unrecognized. By default, # does
   OnAskRepeat(), 0 if OnAskHelp, and * is OnAskBack().
*/

void CTelControlFramework::OnDTMF (WORD wDTMF)
{
   // wDTMF is the DTMF value
   
   // get the value from DTMFMap
   WCHAR sz[2];
   sz[0] = wDTMF;
   sz[1] = 0;
   LONG  lVal;
   BOOL fDTMFString;
   unsigned long sSize;
   WCHAR *DTMFString;
   long count;

   lVal = GetValue (gszDTMFMap, sz, 0);

   sSize = 0;
   DTMFString = NULL;

   fDTMFString = GetValue (gszDTMFMapString,sz,DTMFString,&sSize);
   if(sSize)
   {
       DTMFString = (WCHAR *) CoTaskMemAlloc(sSize*2+2);
       fDTMFString = GetValue (gszDTMFMapString,sz,DTMFString,&sSize);

       //if the count=# keyword is in the DTMFString section, wait for #
       //tones before returning. This allows users to quickly enter a number
       //without losing digits because of beeps.
      
       count = GetValue (gszDTMFMapString,gszDTMFCount,0);
       if(count)        
       {
          m_DTMFString[m_DTMFcount] = sz[0];
	        m_DTMFString[m_DTMFcount+1] = 0;
	        m_DTMFcount++;
	        if((m_DTMFcount >=count) || (m_DTMFcount>=MAXDTMFCOUNT))
	        {
		         OnPhraseParse ((DWORD) lVal, m_DTMFString,m_DTMFcount*2+2, NULL, NULL);		 
		         m_DTMFcount=0; //restart DTMF counting
	        }
           else
              StartSR (m_szRuleActive);
	        return;	 
       }
   }
                                    
   m_DTMFcount=0; //restart DTMF counting

   if (lVal || (DTMFString && fDTMFString)) 
   {	  
         OnPhraseParse ((DWORD) lVal, DTMFString, sSize*2+2, NULL, NULL);		 
	 return;	  
   }
   if(DTMFString)
   {
	 CoTaskMemFree(DTMFString);
   }

   // else, do defaults
   switch (wDTMF) {
   case L'*':
      OnAskBack();
      break;
   case L'0':
      OnAskHelp();
      break;
   case L'#':
      OnAskRepeat();
      break;
   default:
      OnPhraseUnrecognized(NULL, NULL);
   }
   return;
}


/*************************************************************************
CTelControlFramework::OnInterference - Called when an interference message
   comes from the speech recognizer. Many controls will ignore this. Will
   usually switch to a new state or finish by calling DoState().

   A CONTROL IS LIKELY TO REPLACE THIS
*/
void CTelControlFramework::OnInterference (DWORD dwInterference)
{
   // dwInterference comes from SRNotifySink::Interference

   return;
}

/*************************************************************************
CTelControlFramework::OnTTSStop - Called when the TTSQ object stops.
   Most controls will leave this. By default this starts the speech recognition
   engine (if not already started) and a timer to indicate response time
   before calling the no-response.

   A CONTROL IS LIKELY TO REPLACE THIS
*/
void CTelControlFramework::OnTTSStop (void)
{
   // if we're supposed to use the TAPI beep, then we just finised
   // talking, so beep
   if (m_fUseTAPIBeep && m_dwPlayBeep) {
      PlayTAPIBeep (m_dwPlayBeep);
      m_dwPlayBeep = 0;
   }

   // start SR if there's any to start (and will start the timer)
   if (m_fActivateWhenDoneTalking) {
      StartSR (m_szRuleToActivate);
      m_fActivateWhenDoneTalking = NULL;
   }

}


/*************************************************************************
CTelControlFramework::OnTTSBookmark - Called when a bookmark occurs in
   the text. Only controls that use bookmarks in their text will need
   this.

   A CONTROL IS LIKELY TO REPLACE THIS
*/
void CTelControlFramework::OnTTSBookmark (DWORD dwMark)
{
   // dwMark is the TTS bookmark number
   return;
}


/*************************************************************************
CTelControlFramework::FromMemoryArchive - Loads in an archived version.
   Controls should override this if they want to save application time
   in loading and compiling.

   If not overridden, this:
      - Stips off the GUID from the beginning
      - Calls FromMemory

   A CONTROL IS LIKELY TO REPLACE THIS
*/
HRESULT CTelControlFramework::FromMemoryArchive (PVOID pMem, DWORD dwSize)
{
   HRESULT hRes;

   // figure out how large the compiled grammar chunk is
   PBYTE pCur;
   DWORD dwGramSize;
   pCur = (PBYTE) pMem + sizeof(CLSID);
   dwGramSize = *((DWORD*) pCur);
   pCur += sizeof(DWORD);

   // load in the text
   hRes = FromMemory (pCur + dwGramSize, dwSize - sizeof(CLSID) - sizeof(DWORD) - dwGramSize);
   if (hRes)
      return hRes;

   // load in the grammar if it's there
   if (dwGramSize && m_pGramComp) {
      hRes = m_pGramComp->FromMemory (pCur, dwGramSize);
      if (!hRes)
         m_pGramComp->IsCompiled(&m_fIsCompiled);

   };

   return NOERROR;
}


/*************************************************************************
CTelControlFramework::ToMemory - Writes all of the original data (and
   compiled data to memory). An control will want to replace this and
   FromMemoryArchive if it wants application to be able to load controls
   faster, and to not have to compile. The default behaviour is to write
   the control's class GUID followed by the unicode text.

   A CONTROL IS LIKELY TO REPLACE THIS
*/
STDMETHODIMP CTelControlFramework::ToMemory (PVOID *ppMem, DWORD *pdwSize)
{
   PBYTE pMem, pSave;

   *ppMem = NULL;
   *pdwSize = 0;

   if (m_pszText == NULL)
     return ResultFromScode (E_FAIL);

   // if we've got a compiled grammar object then store that away
   DWORD dwGramSize;
   PVOID pGramMem;
   dwGramSize = 0;
   pGramMem = 0;
   if (m_pGramComp) {
      m_pGramComp->ToMemory (&pGramMem, &dwGramSize);
   }

   // get memory for the data
   DWORD   dwTextSize;
   DWORD    dwTotalSize;
   dwTextSize = (wcslen(m_pszText)+1)*2;
   dwTotalSize = dwTextSize + sizeof(CLSID) + sizeof(DWORD) + dwGramSize;
   pMem = (PBYTE) CoTaskMemAlloc(dwTotalSize);
   if (!pMem)
     return E_OUTOFMEMORY;
   pSave = pMem;

   // put in the GUID identifier first
   GetCLSID ((CLSID*) pMem);
   pMem += sizeof(CLSID);

   // write the grammar chunk size
   *((DWORD*)pMem) = dwGramSize;
   pMem += sizeof(DWORD);
   memcpy (pMem, pGramMem, dwGramSize);
   if (pGramMem)
      CoTaskMemFree (pGramMem);
   pMem += dwGramSize;

   // move in the data
   memcpy(pMem, m_pszText, dwTextSize);

   *ppMem = pSave;
   *pdwSize = dwTotalSize;
   return NOERROR;
}


/*************************************************************************
CTelControlFramework::Compile - Compiles the control. A control might want
   to replace this. The default implemntation combines the control's text
   with its default (original text first), and passed that down to the
   grammar compiler. The output of the grammar compiler is passed out
   of this compile. This system only allows for one grammar per control,
   but the control can activate individual rules.

   A CONTROL IS LIKELY TO REPLACE THIS
*/
STDMETHODIMP CTelControlFramework::Compile (LPWSTR *ppszError, DWORD *pdwSize)
{
   m_fIsCompiled = FALSE;

   // combine the app-specific and default text, putting a CR inbetween
   DWORD dwTextChars, dwDefaultChars;
   dwTextChars = m_pszText ? wcslen(m_pszText) : 0;
   dwDefaultChars = m_pszDefaultText ? wcslen(m_pszDefaultText) : 0;
   if (!dwTextChars && !dwDefaultChars)
      return ResultFromScode (E_FAIL); // can't compile nothing
   DWORD dwNeeded;
   dwNeeded = (dwTextChars + dwDefaultChars + 2) * 2;

   // allocate
   WCHAR*   psz;
   psz = (WCHAR*) malloc (dwNeeded);
   if (!psz)
      return ResultFromScode (E_OUTOFMEMORY);
   memcpy (psz, m_pszText, dwTextChars*2);
   psz[dwTextChars] = L'\n';
   memcpy (psz + (dwTextChars+1), m_pszDefaultText, dwDefaultChars*2);
   psz[dwTextChars + dwDefaultChars + 1] = 0;  // NULL terminate

   HRESULT  hRes;
   hRes = m_pGramComp->FromMemory (psz, dwNeeded);
   free (psz); // free the combined memory
   if (hRes)
      return hRes;

   // compile
   hRes = m_pGramComp->Compile (ppszError, pdwSize);
   if (hRes)
      return hRes;

   // we're done
   m_fIsCompiled = TRUE;
   return NOERROR;
}


/*************************************************************************
CTelControlFramework::OnAbort - This function handles an abort situation.
   By default, this calls DoFinish() with a -1 parameter to indicate an
   abort. An application might override this, but doesn't have to.
*/
void CTelControlFramework::OnAbort (void)
{
   StopAllMedia();
   DoFinish (TCR_ABORT, NULL, 0);
}

/*************************************************************************
CTelControlFramework::OnAskBack - The user has asked to go back to the
   previous question.

   By default this checkes [Settings] CanGoBack, and sees if it's TRUE.
   If so, this quits the control with TCR_ASKBACK. Else, it pretends
   that it didn't understand and calls OnUnrecognized().

*/
void CTelControlFramework::OnAskBack (void)
{
   LONG  lVal;
   lVal = GetValue (gszSettings, L"CanGoBack", 0);
   if (lVal) {
      DoFinish (TCR_ASKBACK);
      return;
   }

   // else unrecognized
   OnPhraseUnrecognized (NULL, NULL);
}


/*************************************************************************
CTelControlFramework::OnAskHelp - The user has asked for help.

   By default this speaks [Prompts] Help and re-does the current state.

*/
void CTelControlFramework::OnAskHelp (void)
{
   TTSSpeak (gszPrompts, (m_dwCurState == STATE_VERIFY) ? L"HelpVerify" : L"Help");
   DoState (m_dwCurState);
}


/*************************************************************************
CTelControlFramework::OnAskWhere - The user has asked where he/she is.

   By default this speaks [Prompts] Where, and re-dows the current state.

*/
void CTelControlFramework::OnAskWhere (void)
{
   TTSSpeak (gszPrompts, L"Where");
   DoState (m_dwCurState);
}


/*************************************************************************
CTelControlFramework::OnAskRepeat - The user has asked to repeat the question.

   By default this re-does the current state.

*/
void CTelControlFramework::OnAskRepeat (void)
{
   DoState (m_dwCurState);
}


/*************************************************************************
CTelControlFramework::OnAskOperator - The user has asked for an operator.

   By default this checkes TELDWORD_EnableOperator. If TRUE it quits the
   control and returns TCR_ASKOPERATOR. Else, it pretends its unrecognized.

*/
void CTelControlFramework::OnAskOperator (void)
{
   if (m_pTelInfo->DWORDGet(TELDWORD_EnableOperator)) {
      DoFinish (TCR_ASKOPERATOR);
      return;
   }

   // else pretend unrecognized
   OnPhraseUnrecognized (NULL, NULL);
}


/*************************************************************************
CTelControlFramework::OnAskHangUp - The user has asked to hang up.

   By default this checkes TELDWORD_EnableAskHangUp. If TRUE it quits the
   control and returns TCR_ASKHANGUP. Else, it pretends its unrecognized.

*/
void CTelControlFramework::OnAskHangUp (void)
{
   if (m_pTelInfo->DWORDGet(TELDWORD_EnableAskHangUp)) {
      DoFinish (TCR_ASKHANGUP);
      return;
   }

   // else pretend unrecognized
   OnPhraseUnrecognized (NULL, NULL);
}

#define  SPEEDCHANGE    1.2

/*************************************************************************
CTelControlFramework::OnAskSpeakFaster - The user has asked for the
   computer to speak faster.

   By default this checkes TELDWORD_DisableSpeedChange. If TRUE it
   pretends its unrecognized. Else, it finds the primary text-to-speech
   engine and increases the speed. It then speaks [Prompts] SpeakingFaster.

*/
void CTelControlFramework::OnAskSpeakFaster (void)
{
   if (m_pTelInfo->DWORDGet(TELDWORD_DisableSpeedChange)) {
      // else pretend unrecognized
      OnPhraseUnrecognized (NULL, NULL);
      return;
   }

   // find the TTS engine
   PCTTSMode   pMode;
   pMode = m_pQueue->TTSGet(NULL);
   pMode->SpeedSet ((WORD) (pMode->SpeedGet() * SPEEDCHANGE));
   delete pMode;

   // speak and return to current state
   TTSSpeak (gszPrompts, L"SpeakingFaster");
   DoState (m_dwCurState);
}


/*************************************************************************
CTelControlFramework::OnAskSpeakSlower - The user has asked for the
   computer to speak slower.

   By default this checkes TELDWORD_DisableSpeedChange. If TRUE it
   pretends its unrecognized. Else, it finds the primary text-to-speech
   engine and reduces the speed. It then speaks [Prompts] SpeakingSlower.

*/
void CTelControlFramework::OnAskSpeakSlower (void)
{
   if (m_pTelInfo->DWORDGet(TELDWORD_DisableSpeedChange)) {
      // else pretend unrecognized
      OnPhraseUnrecognized (NULL, NULL);
      return;
   }

   // find the TTS engine
   PCTTSMode   pMode;
   pMode = m_pQueue->TTSGet(NULL);
   pMode->SpeedSet ((WORD) (pMode->SpeedGet() / SPEEDCHANGE));
   delete pMode;

   // speak and return to current state
   TTSSpeak (gszPrompts, L"SpeakingSlower");
   DoState (m_dwCurState);
}





/*************************************************************************
CTelControlFramework::OnPhraseHypothesisPare - Called on a hypothesis.
   The application will most likely ignore this.
*/
void CTelControlFramework::OnPhraseHypothesisParse (DWORD dwParseID, PVOID pParseMem,
                                         DWORD dwParseMemSize,
                                         PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult)
{
   // dwParseID is the parse command #
   // pParseMem and dwParseMemSize are the parse results
   // pSRPhrase is the recognized phrase, if the control needs it
   // lpUnkResults is the IUnk for the results object, if the application needs it. May be NULL

   return;
}


/*************************************************************************
CTelControlFramework::OnPhraseStart - Called when a phrase starts getting
   parsed. An application probably doesn't do anything with this.
*/
void CTelControlFramework::OnPhraseStart (void)
{
   return;
}


/*************************************************************************
CTelControlFramework::OnTTSStart - Called when the TTSQ object starts.
   Most controls will leave this
*/
void CTelControlFramework::OnTTSStart (void)
{
   return;
}


/*************************************************************************
CTelControlFramework::OnTTSSpeakID - Called when a new TTS buffer starts
   playing. Most controls will ignore this.
*/
void CTelControlFramework::OnTTSSpeakID (DWORD dwSpeakID)
{
   return;
}


/*************************************************************************
CTelControlFramework::OnTTSError - Called when an error occurs in TTS.
   Many controls will ignore this. By default this does nothing
*/
void CTelControlFramework::OnTTSError (HRESULT hRes)
{
   return;
}


/*************************************************************************
CTelControlFramework::ObjectSet - Calls by the application to set the telinfo
   object. When called this does the following
      - If a telinfo is already known an error returns
      - Create PCSRMode, PCSTTTSQueue, PCSTLog objects
      - Find out of it's full or half duplex at set m_fFullDuplex

   Controls will probably not override this.
*/
STDMETHODIMP CTelControlFramework::ObjectSet (PITELINFO pInfo)
{
   HRESULT  hRes;

   if (m_pTelInfo || m_pGram || m_pGramComp || m_pSRMode || m_pQueue || m_pLog)
      return ResultFromScode (E_FAIL);

   m_pTelInfo = new CTelInfo;
   if (!m_pTelInfo)
      return ResultFromScode (E_OUTOFMEMORY);
   hRes = m_pTelInfo->Init (pInfo);
   if (hRes)
      return hRes;
   m_pSRMode = m_pTelInfo->SRModeGet();
   m_pQueue = m_pTelInfo->STTTSQueueGet();
   m_pLog = m_pTelInfo->STLogGet();

   if (!m_pSRMode || !m_pQueue)
      return ResultFromScode (E_FAIL);

   // initialize the grammar object
   m_pGramComp = new CSTGramComp;
   if (!m_pGramComp)
      return ResultFromScode (E_FAIL);
   hRes = m_pGramComp->Init();
   if (hRes)
      return hRes;

   // Find out if it's full or half duplex. Or find out when start?
   m_fFullDuplex = m_pTelInfo->DWORDGet(TELDWORD_FullDuplex) &&
      m_pTelInfo->DWORDGet(TELDWORD_EchoCancel);

   return NOERROR;
}



/*************************************************************************
CTelControlFramework::FreeUpControlData - Called (by destructor & fromemory)
   to free up any existing data that's specific to the control data.
   By default, this calls FreeUpDefaultControlData. APps may want to override this
*/
void CTelControlFramework::FreeUpControlData (void)
{
   FreeUpDefaultControlData ();

}


/*************************************************************************
CTelControlFramework::FromMemory -
   - Frees up the old control data
   - If it's a saved version (first 16 bytes are class GUID) then
      it calls FromMemoryArhive and returns.
   - It detects if it's ANSI/Unicode,
   - Load in appropriate default text based on LangID setting

   Apps will not usually override this.
*/
STDMETHODIMP CTelControlFramework::FromMemory (PVOID pMem, DWORD dwSize)
{
    BOOL  fIsUnicode = FALSE;
    BOOL  fHasFEFF = FALSE;

    // if we're started then don't allow
    if (m_fStarted)
       return ResultFromScode (E_FAIL);

    // free up old stuff
    FreeUpControlData();

    // check to see if this "compiled"
    if (dwSize > sizeof(CLSID)) {
       CLSID   cMatch;
       GetCLSID (&cMatch);

       if (IsEqualGUID(*(GUID *)pMem, cMatch))
          return FromMemoryArchive (pMem, dwSize);
    }

    // see if it's unicode

    // unicode data starts with 0xfeff sometimes
    if ((dwSize >= 2) && ( ((PCWSTR)pMem)[0] == 0xfeff) ) {
        fIsUnicode = TRUE;
        fHasFEFF = TRUE;
    }

    // or, if there's a unicode CR in the data then
    // we know it's not ANSI
    if (!fIsUnicode) {
        WCHAR *pTemp;
        DWORD i;

        for (pTemp = (WCHAR*) pMem, i= 0;
            ((i+3) < dwSize) && *pTemp;
            i += 2, pTemp++)

            if (*pTemp == L'\n') {
                fIsUnicode = TRUE;
                break;
            }
    }

    // Copy the memory over
    DWORD   dwNeeded;
    dwNeeded = (fIsUnicode ? dwSize : (dwSize * 2)) + 2; // extra round off
    m_pszText = (LPWSTR) malloc (dwNeeded);
    if (!m_pszText)
        return E_OUTOFMEMORY;
    memset (m_pszText, 0, dwNeeded);

    if (fIsUnicode) {
        memcpy (m_pszText, (PCWSTR) pMem + (fHasFEFF ? 1 : 0),
           fHasFEFF ? (dwSize - 2) : dwSize);
    }
    else {   // ansi
        MultiByteToWideChar(CP_ACP, 0, (LPCSTR) pMem, dwSize, m_pszText, dwNeeded/2);
    }

    // parse this, and based on the parse, load in the right default text
    m_pTextParse = new CInfParse (m_pszText, wcslen(m_pszText)+1);
    if (!m_pTextParse)
       return ResultFromScode (E_OUTOFMEMORY);

    LONG lVal = 1033;
    if (m_pTextParse->SectionFind (L"Control"))
       if (m_pTextParse->ValueFind (L"LangID"))
          m_pTextParse->ValueQuery (NULL, NULL, &lVal);
    
    m_lang.LanguageID = (LANGID) lVal;
    LoadInDefaultText (m_lang.LanguageID);
    // Don't bother with dialect for controls (by default)

    // done
    return NOERROR;
}


/*************************************************************************
CTelControlFramework::FromFile - Loads in data from file. The default
   implementation just loads it all from a file and calls FromMemory
   with the data. Apps can replace this for more efficiency.
*/
STDMETHODIMP CTelControlFramework::FromFile (PCWSTR pwszFile)
{
    HANDLE  hFile;
    char    szTemp[512];

    szTemp[0] = 0;
    WideCharToMultiByte (CP_ACP, 0, pwszFile, -1, szTemp, sizeof(szTemp),
                         NULL, NULL);

    // open the file and fail if it doens't exist
    hFile = CreateFile(szTemp, GENERIC_READ, 0, NULL,
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return E_FAIL;

    DWORD   dwSize;

    dwSize = SetFilePointer(hFile, 0, NULL, FILE_END);
    SetFilePointer (hFile, 0, NULL, FILE_BEGIN);
    if (dwSize == 0xffffffff)
        return E_FAIL;

    // allocate the memory and read in
    PVOID pTemp;
    pTemp = malloc (dwSize);
    if (!pTemp) {
        CloseHandle (hFile);
        return E_OUTOFMEMORY;
    }

    DWORD   dwRead;
    ReadFile (hFile, pTemp, dwSize, &dwRead, NULL);
    CloseHandle (hFile);

    HRESULT hRes;
    hRes = FromMemory (pTemp, dwSize);
    free (pTemp);
    return hRes;
}


/*************************************************************************
CTelControlFramework::FromStream - Loads in the data from a stream. The
   default implementation just loads it call in and calls FromMemory.
   Apps can replace for more efficiency.
*/
STDMETHODIMP CTelControlFramework::FromStream (IStream *pIStream)
{
    if (!pIStream)
        return E_INVALIDARG;

    ULARGE_INTEGER  li;
    LARGE_INTEGER liIn;
    memset (&li, 0, sizeof(li));
    memset (&liIn, 0, sizeof(liIn));

    pIStream->Seek(liIn, STREAM_SEEK_END, &li);

    DWORD dwSize;
    dwSize = (DWORD) li.LowPart;
    pIStream->Seek(liIn, STREAM_SEEK_SET, NULL);

    // allocate the memory and read in
    PVOID pTemp;

    pTemp = malloc (dwSize);
    if (!pTemp) {
        return ResultFromScode (E_OUTOFMEMORY);
    }

    pIStream->Read (pTemp, dwSize, NULL);

    HRESULT hRes;
    hRes = FromMemory (pTemp, dwSize);

    free (pTemp);

    return hRes;
}


/*************************************************************************
CTelControlFramework::FromResource - Loads the data in from a resource.
   The default implementation just loads it all in and calls FromMemory.
   Apps can replace for more efficiency. It loads from a "TEL" resource.
*/
#ifdef STRICT
STDMETHODIMP CTelControlFramework::FromResource (PVOID hInst, DWORD dwID)
#else
STDMETHODIMP CTelControlFramework::FromResource (HINSTANCE hInst, DWORD dwID)
#endif
{
    HRSRC   hRsrc;
    HRESULT hRes;

    hRsrc = FindResource((HINSTANCE) hInst, MAKEINTRESOURCE(dwID), "TEL");
    if (!hRsrc)
        return ResultFromScode (E_FAIL);

    HGLOBAL hMem;
    hMem = LoadResource ((HINSTANCE) hInst, hRsrc);
    if (!hMem)
        return ResultFromScode(E_OUTOFMEMORY);

    // lock the memory
    PVOID pMem;
    pMem = LockResource(hMem);
    if (!pMem) {
        FreeResource (hRsrc);
        return ResultFromScode (E_OUTOFMEMORY);
    }

    DWORD dwSize;
    dwSize = SizeofResource((HINSTANCE) hInst, hRsrc);


    // load it
    hRes = FromMemory (pMem, dwSize);

    // Free the resource
    if (hMem)
        FreeResource (hRsrc);

    return hRes;
}


/*************************************************************************
CTelControlFramework::IsCompiled - Returns TRUE if the data is compiled,
   FALSE if it isn't. The default implementatino looks at the m_fIsCompiled
   variabled.
*/
STDMETHODIMP CTelControlFramework::IsCompiled (BOOL *pfCompiled)
{
    if (!pfCompiled)
        return E_INVALIDARG;
   
    *pfCompiled = m_fIsCompiled;
    return NOERROR;
}


/*************************************************************************
CTelControlFramework::LanguageGet - Returns the language of the control.
   This is gotten from the m_lang variable by default
*/
STDMETHODIMP CTelControlFramework::LanguageGet (LANGUAGEW *pLang)
{
   if (!pLang)
      return E_INVALIDARG;

   *pLang = m_lang;

   return NOERROR;
}


/*************************************************************************
CTelControlFramework::ToStream - Writes the original data & compiled data
   to a stream. The default implementation just calls ToMemory and writes
   that out to a stream
*/
STDMETHODIMP CTelControlFramework::ToStream (IStream *pIStream)
{
    PVOID   pMem;
    DWORD   dwSize;
    HRESULT hr;

    hr = ToMemory(&pMem, &dwSize);
    if (hr != NOERROR)
        return hr;

    hr = pIStream->Write (pMem, dwSize, NULL);
    CoTaskMemFree (pMem);
    return NOERROR;
}


/*************************************************************************
CTelControlFramework::ToFile - Writes the original data & compiled data
   to a file. The default implementation jus tcalls ToMemory and writes
   that out to a file.
*/
STDMETHODIMP CTelControlFramework::ToFile (PCWSTR pwszFile)
{
    HANDLE  hFile;
    char    szTemp[512];
    PVOID   pMem;
    DWORD   dwSize, dwWritten;
    HRESULT hr;

    hr = ToMemory(&pMem, &dwSize);
    if (hr != NOERROR)
        return hr;

    szTemp[0] = 0;
    WideCharToMultiByte(CP_ACP, 0, pwszFile, -1, szTemp, sizeof(szTemp),
                        NULL, NULL);

    // open the file
    hFile = CreateFile(szTemp, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                       CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        CoTaskMemFree(pMem);
        return E_FAIL;
    }

    WriteFile(hFile, pMem, dwSize, &dwWritten, NULL);
    CloseHandle(hFile);

    CoTaskMemFree (pMem);
    return NOERROR;
}


/*************************************************************************
CTelControlFramework::TextGet - Fills a buffer in with the text of the control.
   Teh default implementation gets the text from m_pszText.
*/
STDMETHODIMP CTelControlFramework::TextGet (LPWSTR *ppszText, DWORD *pdwSize)
{
    PBYTE pMem;

    *ppszText = NULL;
    *pdwSize = 0;

    if (m_pszText == NULL)
        return NOERROR;

    DWORD   dwSize;
    dwSize = (wcslen(m_pszText)+1) *2;
    pMem = (PBYTE) CoTaskMemAlloc(dwSize);
    if (!pMem)
        return E_OUTOFMEMORY;
    memcpy(pMem, m_pszText, dwSize);

    *ppszText = (PWSTR) pMem;
    *pdwSize = dwSize;
    return NOERROR;
}


/*************************************************************************
CTelControlFramework::TextDefaultGet - Fills in a buffer with the defaults
   of the control. The default implementation gets the text from m_pszDefaultText.
*/
STDMETHODIMP CTelControlFramework::TextDefaultGet (LPWSTR *ppszText, DWORD *pdwSize)
{
    PBYTE pMem;

    *ppszText = NULL;
    *pdwSize = 0;

    if (m_pszDefaultText == NULL)
        return NOERROR;

    DWORD   dwSize;
    dwSize = (wcslen(m_pszDefaultText)+1) *2;
    pMem = (PBYTE) CoTaskMemAlloc(dwSize);
    if (!pMem)
        return E_OUTOFMEMORY;
    memcpy(pMem, m_pszDefaultText, dwSize);

    *ppszText = (PWSTR) pMem;
    *pdwSize = dwSize;
    return NOERROR;
}


/*************************************************************************
CTelControlFramework::Start - Called by the application when it wants
   to start the control up. The default implementation does the following.
      - Make sure we're not already started
      - Addrefs the applications notification sink
      - Get the NoAnswer time from the app data or defaults
      - Registers all notification sinks with objects
      - Logs that control has started
      - Calls DoState(0)
*/
STDMETHODIMP CTelControlFramework::Start (PITELCONTROLNOTIFYSINK pNotifySink)
{
    // if we're already started or we're not compiled then fail
    if (m_fStarted || !m_fIsCompiled || m_pGram)
       return E_FAIL;

    // if there's no notification sink then fail
    if (!pNotifySink)
       return SRERR_INVALIDPARAM;

    // create the grammar for SR
    m_pGram = m_pGramComp->GrammarLoad (m_pSRMode, m_pISRGramNotifySink);
    if (!m_pGram)
       return E_FAIL;

    // register the notification sink
    m_pITelControlNotifySink = pNotifySink;
    m_pITelControlNotifySink->AddRef();

    // blank out a bunch of state variables
    m_DTMFcount=0; 
    m_fActivateWhenDoneTalking = FALSE;
    m_fIsSRActive = FALSE;
    m_dwCurState = 0;
    m_dwNoAnswerTime = GetValue (gszSettings, L"NoAnswerTime", 10 /* default 10 sec*/);
    m_dwNoAnswerTime *= 1000; // make it milliseconds
    m_dwNoAnswerMax = GetValue (gszSettings, L"NoAnswerMax", 3);
    m_dwNoAnswer = 0;
    m_fVerify = GetValue (gszSettings, L"Verify", 0) ? TRUE : FALSE;

    // set the timeouts
    DWORD dwTimeoutComplete   = GetValue (gszSettings, L"TimeoutComplete",   150);
    DWORD dwTimeoutIncomplete = GetValue (gszSettings, L"TimeoutIncomplete", 500);
    DWORD dwIncompleteMax, dwCompleteMax, dwIncompleteMin, dwCompleteMin;
    m_pSRMode->TimeOutSet(SRATTR_MAXTOINCOMPLETE, SRATTR_MAXTOCOMPLETE);
    m_pSRMode->TimeOutGet(&dwIncompleteMax, &dwCompleteMax);
    m_pSRMode->TimeOutSet(SRATTR_MINTOINCOMPLETE, SRATTR_MINTOCOMPLETE);
    m_pSRMode->TimeOutGet(&dwIncompleteMin, &dwCompleteMin);
    if (dwTimeoutComplete < dwCompleteMin)          dwTimeoutComplete = dwCompleteMin;
    else if (dwTimeoutComplete > dwCompleteMax)     dwTimeoutComplete = dwCompleteMax;
    if (dwTimeoutIncomplete < dwIncompleteMin)      dwTimeoutIncomplete = dwIncompleteMin;
    else if (dwTimeoutIncomplete > dwIncompleteMax) dwTimeoutIncomplete = dwIncompleteMax;
    if(dwTimeoutIncomplete < dwTimeoutComplete)     dwTimeoutIncomplete = dwTimeoutComplete + 1;
    m_pSRMode->TimeOutSet (dwTimeoutIncomplete, dwTimeoutComplete);

    // set the rejection threshold
    DWORD dwThreshold = GetValue(gszSettings, L"RejectionThreshold", 50);
    m_pSRMode->ThresholdSet(dwThreshold);

    // free up the list of spoken
    if (m_pTLSpoken)
       delete m_pTLSpoken;
    m_pTLSpoken = NULL;
    m_pTLSpoken = new CTreeList;

   // find out if we should use tapi's beeps
   DWORD dw1, dw2;
   dw1 = m_pTelInfo->DWORDGet(TELDWORD_HCALL);
   dw2 = m_pTelInfo->DWORDGet(TELDWORD_UseTAPIBeep);
   m_hCall = (HCALL) dw1;
   m_fUseTAPIBeep = dw1 && dw2;
   m_fDisableRecogBeep = !m_pTelInfo->DWORDGet(TELDWORD_EnableRecognizeBeeps);

    // register the notification sinks so we know when stuff happens
    RegisterNotificationSinks();

    // log the start
    if (m_pLog)
       m_pLog->Text (GetControlName(), GetControlName(), 2);

    DoState (0);
    
    return NOERROR;
}

/*************************************************************************
CTelControlFramework::DoState - Sets the m_dwCurState parameter, logs
   the state transition, and then calls OnState(). Controls should
   not override this function.
*/
void CTelControlFramework::DoState (DWORD dwState)
{
   // log this
   if (m_pLog) {
      WCHAR szTemp[32];
      swprintf (szTemp, L"Switch to state %d", (int) dwState, 3);
      m_pLog->Text (GetControlName(), szTemp, 2);
   }

   m_dwCurState = dwState;

   // hook for special virification state
   if (m_dwCurState == STATE_VERIFY) {
      if (m_pszPreSpeakItem)
         TTSSpeak (gszPrompts, m_pszPreSpeakItem);
      OnVerify();
      if (m_pszPostSpeakItem)
         TTSSpeak (gszPrompts, m_pszPostSpeakItem);
      ReadyToListenBeep();
      StartSRWhenReady (L"Verify");
      return;
   }

   OnState (m_dwCurState);
}

/*************************************************************************
CTelControlFramework::OnVerify - Called to speak what the user has
   entered so that the user can verify it.  Controls usually won't
   override this function.
*/
void CTelControlFramework::OnVerify ()
{
   if (m_pszVerifySpeak)
      m_pQueue->Speak (m_pszVerifySpeak);
}

/*************************************************************************
CTelControlFramework::DoVerify - Puts in the control in a verification
   mode. This will ask the user if the data was correct. If the user
   answers yes then dwOnCorrectState is called, else dwOnWrongState is
   called. It also handles cases where the user doesn't respond, etc.

   For DoVerify() to work, the control must have the following values
   in its default text (or the application must supply them):
   [Prompts]
   <pszPreSpeakItem>=<string to speak before>
   <pszPostSpeakItem>=<string to speak after>
   HelpVerify=<string to speak if user asks for help>
   
   [<Start>]
   <Start>=... your stuff...
   <Start>=<Verify>

   [<Verify>]
   ; something similar to this. It's very important that yes=1, no=2
   1=(YesAnswers)
   2=(NoAnswers)
   =(DefaultResponses)

   The DoVerify() command works as follows:
   1) Calls DoState(STATE_VERIFY). This is automatically handled by DoState.
   2) The STATE_VERIFY state does:
      a) TTSSpeak ("[Prompts]", pszPreSpeakItem);
      b) m_pTTSQueue->Speak (pszVerifySpeak);
      c) TTSSpeak ("[Prompts]", pszPostSpeakItem);
      d) ReadyToListenBeep()
      e) StartSRWhenReady ("Verify");
   3) When a PhraseFinish notification comes in, and the current state is
      STATE_VERIFY, the PhraseParse or PhraseUnrecognized calls are not
      made, but appropriate ones are done for verification.
   4) When DTMF comes in, if the state is STATE_VERIFY, the appropraite
      options are hooked.
   5) Based on phrase finish (or DTMF), a new DoState() is called with
      either dwOnCorrectState or dwOnWrongState

inputs
   PCWSTR   pszPreSpeakItem - String to speak when starting to talk. THis
               pointer must be valid for the duration of verification.
   PCWSTR   pszPostSpeakItem - String to speak after speak pszVerifySpeak.
               This pointer must be valid for the duration of verification.
   PCWSTR   pszVerifySpeak - Text to be spoken for verification.
   DWORD    dwOnCorrectState - state to switch to when verirication is done & correct.
   DWORD    dwOnWrongState - state to switch to when verification is done & incorrect.
*/
HRESULT CTelControlFramework::DoVerify (PCWSTR pszPreSpeakItem, PCWSTR pszPostSpeakItem,
   PCWSTR pszVerifySpeak, DWORD dwOnCorrectState, DWORD dwOnWrongState)
{
   m_pszPreSpeakItem = pszPreSpeakItem;
   m_pszPostSpeakItem = pszPostSpeakItem;
   m_pszVerifySpeak = pszVerifySpeak;
   m_dwOnCorrectState = dwOnCorrectState;
   m_dwOnWrongState = dwOnWrongState;

   DoState (STATE_VERIFY);

   return NOERROR;
}

/*************************************************************************
CTelControlFramework::Abort - Called by the application when it wants
   to abort. The control does not usually override the default implmentation.
   The default implementation is:
      - Make sure we're really started
      - Stop TTS & SR & timer
      - Call DoFinish() with a -1 return
*/
STDMETHODIMP CTelControlFramework::Abort (void)
{
   if (!m_fStarted)
      return E_FAIL;

   // call finished
   StopAllMedia();
   DoFinish (TCR_ABORT, NULL, 0);

   return NOERROR;
}



/* !!! useful functions to call !!!! */
/*************************************************************************
CTelControlFramework::StopAllMedia - Stops TTS and stops SR from listening.
   These are not restarted. Stops the timer also.
*/
void CTelControlFramework::StopAllMedia (void)
{
   StopSR();

   // stop TTS
   if (m_pQueue)
      m_pQueue->AudioReset();
}

/*************************************************************************
CTelControlFramework::StartSR - Starts speech recognition right away,
   activating the specified rule in SR. (If another rule was active then
   that is deactivated right away. The control should not usually call
   this because it isn't safe in half-duplex. It starts up the response timer too.

   Controls should call this rather than activating directly because it
   sets up some variables, and turn on the response timer.
*/
HRESULT CTelControlFramework::StartSR (PCWSTR pszRule)
{
   StopSR();
   m_fActivateWhenDoneTalking = FALSE;

   // activate the rule
   HRESULT  hRes;
   if (!m_pGramComp)
      return E_FAIL;

   hRes = m_pGram->Activate (NULL, (pszRule && pszRule[0]) ? pszRule : NULL);
   if (hRes)
      return hRes;
   wcscpy (m_szRuleActive, pszRule ? pszRule : L"");
   m_fIsSRActive = TRUE;

   // start the time-out timer
   // do this by remember the time when we should stop
   m_dwTimeOut = GetTickCount() + m_dwNoAnswerTime;

   // log this
   if (m_pLog) {
      WCHAR szTemp[128];
      swprintf (szTemp, L"Activate %s", m_szRuleActive, 3);
      m_pLog->Text (GetControlName(), szTemp, 2);
   }

   return NOERROR;
}


/*************************************************************************
CTelControlFramework::StartSRWhenReady - Starts the speech recognition
   at the next available moment. (Stops SR if it's going now.) In a full
   duplex system it starts right away. In a half duplex system that's
   speaking TTS, it sets a flag to start when TTS stops. This is used
   in OnTTSStop.
*/
HRESULT CTelControlFramework::StartSRWhenReady (PCWSTR pszRule)
{
   // stop SR now just in case
   StopSR();

   // On a full duplex system start right away

   // if we're not speaking now then just do startsr
   if (m_fFullDuplex || !m_pQueue->IsSpeaking())
      return StartSR (pszRule);

   // else, we're talking, so remembeer this for later
   wcscpy (m_szRuleToActivate, pszRule ? pszRule : L"");
   m_fActivateWhenDoneTalking = TRUE;

   return NOERROR;
}

/*************************************************************************
CTelControlFramework::StopSR - Stops speech recognition right away. If
   speech recognition was slated for later (when TTS done speaking), then
   that is removed also. This also kills the wait timer.
*/
HRESULT CTelControlFramework::StopSR (void)
{
   m_fActivateWhenDoneTalking = FALSE;

   // kill the timer
   m_dwTimeOut = 0;


   // stop SR
   if (m_pGram && m_fIsSRActive) {
      m_pGram->Deactivate (m_szRuleActive[0] ? m_szRuleActive : NULL);
      m_fIsSRActive = FALSE;

      // log this
      if (m_pLog) {
         WCHAR szTemp[128];
         swprintf (szTemp, L"Deactivate %s", m_szRuleActive, 3);
         m_pLog->Text (GetControlName(), szTemp, 2);
      }

   }

   return NOERROR;
}

/*************************************************************************
CTelControlFramework::PlayTAPIBeep - Plays the TAPI beep, specified
   by the dwBeep param.
*/
HRESULT CTelControlFramework::PlayTAPIBeep (DWORD dwBeep)
{
   if (!dwBeep || !m_hCall)
      return FALSE;

   long  iRet;
   iRet = lineGenerateTone (m_hCall, LINETONEMODE_BEEP, 100, 0, NULL);
   return iRet ? E_FAIL : NOERROR;
}


/*************************************************************************
CTelControlFramework::ReadyToListenBeep - Plays the beep indicating that
   ready to listen.
*/
HRESULT CTelControlFramework::ReadyToListenBeep (void)
{
   // if disabled then just return
   if (m_fDisableRecogBeep)
      return NOERROR;

   // if we're supposed to use the TAPI beep, then don't queue
   // up a beep
   if (m_fUseTAPIBeep) {
      if (m_pQueue->IsSpeaking())
         m_dwPlayBeep = 1;
      else {
         m_dwPlayBeep = 0;
         PlayTAPIBeep (1);
      }
      return NOERROR;
   }

   return m_pQueue->Speak (L"<Listening>");
}

/*************************************************************************
CTelControlFramework::RecognizedBeep - Plays the recognized beep
*/
HRESULT CTelControlFramework::RecognizedBeep (void)
{
   // if disabled then just return
   if (m_fDisableRecogBeep)
      return NOERROR;

   // if we're supposed to use the TAPI beep, then don't queue
   // up a beep
   if (m_fUseTAPIBeep) {
      if (m_pQueue->IsSpeaking())
         m_dwPlayBeep = 1;
      else {
         m_dwPlayBeep = 0;
         PlayTAPIBeep (1);
      }
      return NOERROR;
   }
   return m_pQueue->Speak (L"<Recognized>");
}


/*************************************************************************
CTelControlFramework::RecordingBeep - Plays the recording beep
*/
HRESULT CTelControlFramework::RecordingBeep (void)
{
   // if we're supposed to use the TAPI beep, then don't queue
   // up a beep
   if (m_fUseTAPIBeep) {
      if (m_pQueue->IsSpeaking())
         m_dwPlayBeep = 1;
      else {
         m_dwPlayBeep = 0;
         PlayTAPIBeep (1);
      }
      return NOERROR;
   }

   return m_pQueue->Speak (L"<Recording>");
}


/*************************************************************************
CTelControlFramework::UnrecognizedBeep - Plays the unrecognize beep
*/
HRESULT CTelControlFramework::UnrecognizedBeep (void)
{
   // if disabled then just return
   if (m_fDisableRecogBeep)
      return NOERROR;

   // if we're supposed to use the TAPI beep, then don't queue
   // up a beep
   if (m_fUseTAPIBeep) {
      if (m_pQueue->IsSpeaking())
         m_dwPlayBeep = 1;
      else {
         m_dwPlayBeep = 0;
         PlayTAPIBeep (1);
      }
      return NOERROR;
   }

   return m_pQueue->Speak (L"<Unrecognized>");
}


/*************************************************************************
CTelControlFramework::FreeUpDefaultControlData - Frees up default control
   data. (Should only be called by the FreeUpControlData function). This
   includes XXX
*/
HRESULT CTelControlFramework::FreeUpDefaultControlData (void)
{
   // we are no longer compiled
   m_fIsCompiled = FALSE;

   // Free up the text
   if (m_pszText) {
      free (m_pszText);
      m_pszText = NULL;
   }
   if (m_pTextParse) {
      delete m_pTextParse;
      m_pTextParse = NULL;
   }

   // don't bother freeing up the grammar since we'll just reuse that later
   return NOERROR;
}

/*************************************************************************
CTelControlFramework::LoadInDefaultText - Given a language ID, this finds
   the resource ID, and loads in the default text. (if nothing new needs
   to get loaded in then it isn't). The default text is then parsed and
   stored in a member variable for use.
*/
HRESULT CTelControlFramework::LoadInDefaultText (LANGID lang)
{

   DWORD dwResID;
   dwResID = GetDefaultTextResID (lang);
   if (dwResID == m_dwDefaultTextResIDLoaded)
      return NOERROR;

   m_dwDefaultTextResIDLoaded = 0;

   HRSRC hRsrc = NULL;
   HGLOBAL  hMem = NULL;
   char  *pMem = NULL;
   DWORD dwSize = 0;

   // free default text if necessary
   if (m_pszDefaultText) {
      free (m_pszDefaultText);
      m_pszDefaultText = NULL;
   }
   if (m_pDefaultTextParse) {
      delete m_pDefaultTextParse;
      m_pDefaultTextParse = NULL;
   }

   hRsrc = FindResource(m_hModule, MAKEINTRESOURCE(dwResID), "TEXT");
   if (!hRsrc)
      return E_FAIL;

   hMem = LoadResource (m_hModule, hRsrc);
   if (!hMem)
      return E_FAIL;

   // loock the memory
   pMem = (char*) LockResource(hMem);
   if (!pMem) {
      FreeResource (hRsrc);
      return E_FAIL;
   }
   dwSize = SizeofResource(m_hModule, hRsrc);

   // allocate memory for this. We may need to add on an extra NULL
   // and we may need to remove ffef from the beginning
   BOOL fHasFEFF =  FALSE;

   if ((dwSize >= 2) && ( ((PCWSTR)pMem)[0] == 0xfeff) ) {
     fHasFEFF = TRUE;
   }

   // copy the memory
   DWORD dwNeeded;
   dwNeeded = dwSize + 2 - (fHasFEFF ? 2 : 0);
   m_pszDefaultText = (WCHAR*) malloc (dwNeeded);
   if (!m_pszDefaultText) {
      FreeResource (hRsrc);
      return E_OUTOFMEMORY;
   }
   memset (m_pszDefaultText, 0, dwNeeded);
   memcpy (m_pszDefaultText, (PCWSTR) pMem + (fHasFEFF ? 1 : 0), dwNeeded - 2);

   // free the resource
   FreeResource (hRsrc);

   // create new parse
   m_pDefaultTextParse = new CInfParse (m_pszDefaultText, dwNeeded / 2);
   if (!m_pDefaultTextParse)
      return E_OUTOFMEMORY;

   // Free the resource
   m_dwDefaultTextResIDLoaded = dwResID;

   return NOERROR;
}



/*************************************************************************
CTelControlFramework::DoFinish - Stops TTS & SR & timers, and sends a
   a notification to the calling application that finish has occured.
   It also removes all notification sinks. An application will not need
   to override this. The information is logged. Also releases the application's
   notification sink.
*/
void CTelControlFramework::DoFinish (DWORD dwResult, PVOID pMem, DWORD dwSize)
{
   // log this
   if (m_pLog) {
      WCHAR szTemp[128];
      swprintf (szTemp, L"Finish %d", (int) dwResult);
      m_pLog->Data (GetControlName(), szTemp, 2, pMem, dwSize);
   }

   // stop speech recognition, but leave TTS going because
   // maybe it's supposed to
   StopSR();
   UnregisterNotificationSinks();

   // release the list of text we've spoken
   if (m_pTLSpoken)
      delete m_pTLSpoken;
   m_pTLSpoken = NULL;

   // call the app
   if (m_pITelControlNotifySink) {
      m_pITelControlNotifySink->Finish (dwResult, pMem, dwSize);
      m_pITelControlNotifySink->Release();
      m_pITelControlNotifySink = NULL;
   }

   // done
}


/*************************************************************************
CTelControlFramework::DoInfo - Sends info to the application. By default
   the information is logged.
*/
void CTelControlFramework::DoInfo (DWORD dwResult, PVOID pMem, DWORD dwSize)
{
   // log this
   if (m_pLog) {
      WCHAR szTemp[128];
      swprintf (szTemp, L"Info %d", (int) dwResult);
      m_pLog->Data (GetControlName(), szTemp, 3, pMem, dwSize);
   }

   // call the app
   if (m_pITelControlNotifySink)
      m_pITelControlNotifySink->Finish (dwResult, pMem, dwSize);

   // done
}


/*************************************************************************
CTelControlFramework::GetValue - Called when the control wants to get
   a value from the text passed in by the application. This first looks
   in the app's input text, and then in the defaults for the value.

inputs
   PCWSTR      pszSection - Section name
   PCWSTR      pszValue - Value name
   PWSTR       pszRet - Filled with string of the value
   DWORD       *pdwSize - Must intially be filled with the size of available
                  memory in pszRet (in bytes). Filled with number of bytes copies/needed
returns
   BOOL - TRUE if works
*/
BOOL CTelControlFramework::GetValue (PCWSTR pszSection, PCWSTR pszValue,
                                    PWSTR pszRet, DWORD *dwSize)
{
   // try the app's parameters
   if (m_pTextParse && m_pTextParse->SectionFind(pszSection) &&
      m_pTextParse->ValueFind(pszValue))
      return m_pTextParse->ValueQuery (NULL, NULL, pszRet, dwSize);

   // try the default parameters
   if (m_pDefaultTextParse && m_pDefaultTextParse->SectionFind(pszSection) &&
      m_pDefaultTextParse->ValueFind(pszValue))
      return m_pDefaultTextParse->ValueQuery (NULL, NULL, pszRet, dwSize);
   return FALSE;
}


/*************************************************************************
CTelControlFramework::GetValue - Returns a number value instead of string.

inputs
   PCWSTR      pszSection - Section name
   PCWSTR      pszValue - Value name
   LONG        lDefault - Default value if can't find
returns
   LONG - Value
*/
LONG CTelControlFramework::GetValue (PCWSTR pszSection, PCWSTR pszValue, LONG lDefault)
{
   LONG  lVal = lDefault;

   // try the app's parameters
   if (m_pTextParse && m_pTextParse->SectionFind(pszSection) &&
      m_pTextParse->ValueFind(pszValue)) {
         m_pTextParse->ValueQuery (NULL, NULL, &lVal);
         return lVal;
   }

   // try the default parameters
   if (m_pDefaultTextParse && m_pDefaultTextParse->SectionFind(pszSection) &&
      m_pDefaultTextParse->ValueFind(pszValue)) {
         m_pDefaultTextParse->ValueQuery (NULL, NULL, &lVal);
         return lVal;
   }

   return lDefault;
}

/*************************************************************************
CTelControlFramework::TTSSpeak - Speaks. Gets the string to be spoken by getting
   one of the values. It also gets a value to find the default voice to
   use for it. Applications should use this is possible since it allows
   more easily for applications to replace strings.

   Ex: TTSSpeak ("SpeakStrings", "Main") will reference:

   [SpeakStrings]
   Main=This is the string to speak.
   Main.2=Shorter prompt, spoken the second time. <<< Optional
   Main.Voice=Male    <<<< optional

inputs
   PCWSTR      pszSection - Section to look under
   PCWSTR      pszValue - Value. The name of the voice is pszValue.Voice.
returns
   HRESULT - error
*/
HRESULT CTelControlFramework::TTSSpeak (PCWSTR pszSection, PCWSTR pszValue)
{
   WCHAR szSpeakString[5000]; // allow something reasonably large.
   WCHAR szVoice[64];
   WCHAR szLookIn[128];

   DWORD dwSize;
   BOOL  fHave = FALSE;

   // see if we've spoken this already
   if (m_pTLSpoken && m_pTLSpoken->NameToNumber (pszValue)) {
      // we spoke this prompt already, see if we can find a .2 version
      WCHAR szTemp[64];
      wcscpy (szTemp, pszValue);
      wcscat (szTemp, L".2");
      dwSize = sizeof(szSpeakString);
      if (GetValue (pszSection, szTemp, szSpeakString, &dwSize))
         fHave = TRUE;
   }
   else {
      // add to the list that we've spoken it, so the next time
      // we try .2
      if (m_pTLSpoken)
         m_pTLSpoken->AddElem (pszValue, 0, 0);
   }
   
   // else, speak from the default location
   if (!fHave) {
      dwSize = sizeof(szSpeakString);
      if (!GetValue (pszSection, pszValue, szSpeakString, &dwSize))
         return ResultFromScode (E_FAIL);
   }

   // get the name
   wcscpy (szLookIn, pszValue);
   wcscat (szLookIn, L".Voice");
   szVoice[0] = 0;
   dwSize = sizeof(szVoice);
   GetValue (pszSection, szLookIn, szVoice, &dwSize);

   // speak
   return m_pQueue->Speak (szSpeakString, szVoice);
}



/* !!!! You won't be calling these that often */

/*************************************************************************
CTelControlFramework::UnregisterNotificationSinks - Unregisters all of the
   notification sinks so that we don't get called unexpectedly.
*/
void CTelControlFramework::UnregisterNotificationSinks (void)
{
   // free the grammar object if it's around
   if (m_pGram) {
      delete m_pGram;
      m_pGram = NULL;
   }

   if (!m_fNotifySinksUsed)
      return;

   m_fNotifySinksUsed = FALSE;
   if (m_pSRMode)
      m_pSRMode->UnRegister (m_dwSRKey);
   if (m_pQueue)
      m_pQueue->Register (NULL);
   if (m_pTelInfo)
      m_pTelInfo->TelNotifySinkSet(NULL);
}

/*************************************************************************
CTelControlFramework::RegisterNotificationSinks - Register all of the
   necessary notification sinks so we start getting called for events.
*/
void CTelControlFramework::RegisterNotificationSinks (void)
{
   if (m_fNotifySinksUsed)
      return;

   m_fNotifySinksUsed = TRUE;
   if (m_pSRMode)
      m_pSRMode->Register (m_pISRNotifySink, &m_dwSRKey);
   if (m_pQueue)
      m_pQueue->Register (m_pISTTTSQueueNotifySink);
   if (m_pTelInfo)
      m_pTelInfo->TelNotifySinkSet(m_pITelNotifySink);
}


/*************************************************************************
CTelControlFramework::QueryInterface, AddRef, and Release
*/
STDMETHODIMP CTelControlFramework::QueryInterface (REFIID riid, LPVOID FAR * ppv)
{
    *ppv = NULL;

    // Any intercae on this object is the obvject pointer
    if ( IsEqualIID(riid, IID_IUnknown) ||
         IsEqualIID(riid, IID_ITelControl) )
        *ppv = (LPVOID) this;

    // If we assign an interface then addref
    if (NULL != *ppv) {
        ((LPUNKNOWN)*ppv)->AddRef();
        return NOERROR;
    }

    return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_ (ULONG) CTelControlFramework::AddRef(void)
{
    return ++m_cRef;
}

STDMETHODIMP_ (ULONG) CTelControlFramework::Release(void)
{
    ULONG cRefT;

    cRefT = --m_cRef;

    if (0L == m_cRef)
        delete this;

    return cRefT;
}



/*************************************************************************
CTCTelNotifySink - This is a notification sink object that the telephony
   control creates to get notifies when DTMF comes in, and abort comes
   in. The application should not override these since they also do
   some logging. If the application wants to override functionality
   it should override OnDTMF and OnAbort.

   ::DTMF - This logs, and calls StopAllMedia. Then calls OnDTMF.
   ::Abort - THis logs, calls StopAllMedia. Then calls OnAbort.
*/
CTCTelNotifySink::CTCTelNotifySink(CTelControlFramework *pCtrl)
{
    m_pTelControl = pCtrl;
}

CTCTelNotifySink::~CTCTelNotifySink()
{
    // Intentionally left blank

}


STDMETHODIMP CTCTelNotifySink::QueryInterface (REFIID riid, LPVOID *ppv)
{
    *ppv = NULL;

    /* always return our IUnkown for IID_IUnknown */
    if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITelNotifySink)) {
        *ppv = (LPVOID) this;
	AddRef();
        return NOERROR;
    }

    // otherwise, cant find
    return E_NOINTERFACE;
}

STDMETHODIMP_ (ULONG) CTCTelNotifySink::AddRef (void)
{
    // normally this increases a reference count, but this object
    // is going to be freed as soon as the object is freed, so it doesn't
    // matter
    return 1;
}

STDMETHODIMP_(ULONG) CTCTelNotifySink::Release (void)
{
    // normally this releases a reference count, but this object
    // is going to be freed when the object is freed so it doesnt
    // matter
    return 1;
};


STDMETHODIMP CTCTelNotifySink::DTMF(WCHAR wcDTMF)
{
   // user answered so rmember this
   m_pTelControl->m_dwNoAnswer = 0;


   // log
   if (m_pTelControl->m_pLog) {
      WCHAR szTemp[64];
      swprintf (szTemp, L"DTMF = %c", (int) wcDTMF);
      m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), szTemp, 1);
   }

   m_pTelControl->StopAllMedia();

   // If we're in a verify state then interpret DTMF here, else cann onDTMF
   // and let the derived code handle it
   if (m_pTelControl->m_dwCurState == STATE_VERIFY) {
      switch (wcDTMF) {
      case '1':
      case '2':
         m_pTelControl->DoState ((wcDTMF == L'1') ?
            m_pTelControl->m_dwOnCorrectState :
            m_pTelControl->m_dwOnWrongState);
         return NOERROR;
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
         m_pTelControl->OnPhraseUnrecognized(NULL,NULL);
         return NOERROR;
      default:
         break;   // go on and call OnDTMF
      }
   }

   m_pTelControl->OnDTMF (wcDTMF);
   return NOERROR;
}

STDMETHODIMP CTCTelNotifySink::Abort(DWORD dwReason)
{
   // log
   if (m_pTelControl->m_pLog) {
      m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), L"Abort", 2);
   }

   m_pTelControl->StopAllMedia();
   m_pTelControl->OnAbort ();
   return NOERROR;
}



/*************************************************************************
CTCSRGramNotifySink - This is a grammar notification sink coming from the
   speech recognition. If the sub-class control creates it sown control
   it should probably use one of these. These functions should not
   be overrided though.

   ::BookMark - return NOERROR
   ::Paused - return NOERROR
   ::PhraseFinish - Logs this. Parses using the grammar object. Calls
         OnPhraseParse to act. Also shuts up TTS & turns off SR.
         Based on the parse, this will call recognized or unrecognize beep.
         Will call OnPhraseUnrecognized if not recognized.

   ::PhraseHypothesis - Logs this. Parses using the grammar object.
         Calls OnPhraseHypothesisParse. Also shuts up TTS. (Doesn't turn off SR)
   ::PhraseStart - Logs this. Calls OnPhraseStart
   ::ReEvaluate - Return NOERROR
   ::Training - Return NORROR
   ::UnArchive - Return NOERROR
*/
CTCSRGramNotifySink::CTCSRGramNotifySink(CTelControlFramework* pCtrl)
{
    m_pTelControl = pCtrl;
}

CTCSRGramNotifySink::~CTCSRGramNotifySink()
{
    // Intentionally left blank
}


STDMETHODIMP CTCSRGramNotifySink::QueryInterface (REFIID riid, LPVOID *ppv)
{
    *ppv = NULL;

    /* always return our IUnkown for IID_IUnknown */
    if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRGramNotifySinkW)) {
        *ppv = (LPVOID) this;
	AddRef();
        return NOERROR;
    }

    // otherwise, cant find
    return E_NOINTERFACE;
}

STDMETHODIMP_ (ULONG) CTCSRGramNotifySink::AddRef (void)
{
// normally this increases a reference count, but this object
// is going to be freed as soon as the app is freed, so it doesn't
// matter
return 1;
}

STDMETHODIMP_(ULONG) CTCSRGramNotifySink::Release (void)
{
// normally this releases a reference count, but this object
// is going to be freed when the application is freed so it doesnt
// matter
return 1;
};


STDMETHODIMP CTCSRGramNotifySink::PhraseFinish(DWORD dwFlags,
                                          QWORD qTimeStampBegin,
                                          QWORD qTimeStampEnd,
                                          PSRPHRASEW pSRPhrase,
                                          LPUNKNOWN pIUnknownResult)
{
   // log the words recognized
   if (m_pTelControl->m_pLog) {
      if (pSRPhrase) {
         WCHAR szLog[512];

         wcscpy (szLog, L"PhraseFinish:");

	      // write out a string with the results
	      // loop through all of the words and display them
         PSRWORDW pSRMax, pSRWord;
	      pSRMax = (PSRWORDW) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
	      pSRWord = (PSRWORDW) (pSRPhrase->abWords);

	      while (pSRWord < pSRMax)
	      {
		      // add word
            wcscat (szLog, L" ");
            wcscat (szLog, pSRWord->szWord);
			   pSRWord = (PSRWORDW) ((BYTE*) pSRWord + pSRWord->dwSize);
	      };

         m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), szLog, 1);
      }
      else
         m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), L"PhraseFinish: Unrecognized", 1);
   }

#ifdef RHETOREX
   // Disable barge in stopping the phrase finish so we can get
   // a complete utterance
   return NOERROR;
   if (m_pTelControl->m_pQueue->IsSpeaking())
      return NOERROR;
#endif // RHETOREX


   // we answered so rmember this
   m_pTelControl->m_dwNoAnswer = 0;

   // figure out if recognized or unrecognized, and try to parse if
   // recognized. If can't parse then it's unregonized
   BOOL     fRecog;
   HRESULT  hRes;
   PWSTR pszParse = NULL;
   DWORD dwParseSize = 0, dwCmd = 0;

   fRecog = (pSRPhrase ? TRUE : FALSE);
   if (!(dwFlags & (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)))
      fRecog = FALSE;
   if (fRecog) {
      hRes = m_pTelControl->m_pGramComp->PhraseParse(pSRPhrase, &dwCmd, &pszParse, &dwParseSize);
      if (hRes)
         fRecog = FALSE;
   }

   // log what was parsed out
   if (m_pTelControl->m_pLog) {
      WCHAR szTemp[256];
      swprintf (szTemp, L"PhraseParse = %d : %s", (int) dwCmd,
         pszParse ? pszParse : L"NONE");
      m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), szTemp, 2);
   }

   // turn off TTS & SR
   m_pTelControl->StopAllMedia();

   // play beep based on recognized or unrecognized
   if (fRecog)
      m_pTelControl->RecognizedBeep();
   else
      m_pTelControl->UnrecognizedBeep();

   // if the parse was successful then we might want to case on
   // some special phrases
   if (fRecog) switch (dwCmd) {
      case TCR_ASKOPERATOR:
         if (pszParse)
            CoTaskMemFree (pszParse);
         m_pTelControl->OnAskOperator();
         return NOERROR;
      case TCR_ASKHANGUP:
         if (pszParse)
            CoTaskMemFree (pszParse);
         m_pTelControl->OnAskHangUp();
         return NOERROR;
      case TCR_ASKBACK:
         if (pszParse)
            CoTaskMemFree (pszParse);
         m_pTelControl->OnAskBack();
         return NOERROR;
      case TCR_ASKWHERE:
         if (pszParse)
            CoTaskMemFree (pszParse);
         m_pTelControl->OnAskWhere();
         return NOERROR;
      case TCR_ASKHELP:
         if (pszParse)
            CoTaskMemFree (pszParse);
         m_pTelControl->OnAskHelp();
         return NOERROR;
      case TCR_ASKREPEAT:
         if (pszParse)
            CoTaskMemFree (pszParse);
         m_pTelControl->OnAskRepeat();
         return NOERROR;
      case TCR_ASKSPEAKFASTER:
         if (pszParse)
            CoTaskMemFree (pszParse);
         m_pTelControl->OnAskSpeakFaster();
         return NOERROR;
      case TCR_ASKSPEAKSLOWER:
         if (pszParse)
            CoTaskMemFree (pszParse);
         m_pTelControl->OnAskSpeakSlower();
         return NOERROR;

   }

   // if we're in the verify state then don't send the recognition
   // on to the derived control
   if (m_pTelControl->m_dwCurState == STATE_VERIFY) {
      if (pszParse)
         CoTaskMemFree (pszParse);
      pszParse = NULL;
      if ((dwCmd != 1) && (dwCmd != 2))
         fRecog = FALSE;

      if (fRecog)
         m_pTelControl->DoState ((dwCmd == 1) ?
            m_pTelControl->m_dwOnCorrectState :
            m_pTelControl->m_dwOnWrongState);
      else
         m_pTelControl->OnPhraseUnrecognized (NULL, NULL);
      return NOERROR;
   }


   // Call OnPhraseFinish or OnPhraseUnrecognized
   if (fRecog)
      m_pTelControl->OnPhraseParse (dwCmd, pszParse, dwParseSize, pSRPhrase, pIUnknownResult);
   else
      m_pTelControl->OnPhraseUnrecognized (pSRPhrase, pIUnknownResult);

   // Free up parse information
   if (pszParse)
      CoTaskMemFree (pszParse);

   return NOERROR;
}

STDMETHODIMP CTCSRGramNotifySink::PhraseHypothesis(DWORD dwFlags,
                                              QWORD qTimeStampBegin,
                                              QWORD qTimeStampEnd,
                                              PSRPHRASEW pSRPhrase,
                                              LPUNKNOWN pIUnknownResult)
{
   // log the words recognized
   if (m_pTelControl->m_pLog) {
      if (pSRPhrase) {
         WCHAR szLog[512];

         wcscpy (szLog, L"PhraseHypthesis:");

	      // write out a string with the results
	      // loop through all of the words and display them
         PSRWORDW pSRMax, pSRWord;
	      pSRMax = (PSRWORDW) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
	      pSRWord = (PSRWORDW) (pSRPhrase->abWords);

	      while (pSRWord < pSRMax)
	      {
		      // add word
            wcscat (szLog, L" ");
            wcscat (szLog, pSRWord->szWord);
			   pSRWord = (PSRWORDW) ((BYTE*) pSRWord + pSRWord->dwSize);
	      };

         m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), szLog, 3);
      }
      else
         m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), L"PhraseHypothesis: Unrecognized", 3);
   }

#ifdef RHETOREX
   //  Disable barge in stopping the phrase finish so we can get
   // a complete utterance
   return NOERROR;
   if (m_pTelControl->m_pQueue->IsSpeaking())
      return NOERROR;
#endif // RHETOREX


   // figure out if recognized or unrecognized, and try to parse if
   // recognized. If can't parse then it's unregonized
   BOOL     fRecog;
   HRESULT  hRes;
   PWSTR pszParse = NULL;
   DWORD dwParseSize = 0, dwCmd = 0;

   fRecog = (pSRPhrase ? TRUE : FALSE);
   if (!(dwFlags & (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)))
      fRecog = FALSE;
   if (fRecog) {
      hRes = m_pTelControl->m_pGramComp->PhraseParse(pSRPhrase, &dwCmd, &pszParse, &dwParseSize);
      if (hRes)
         fRecog = FALSE;
   }

   // log what was parsed out
   if (m_pTelControl->m_pLog) {
      WCHAR szTemp[256];
      swprintf (szTemp, L"PhraseHypothesisParse = %d : %s", (int) dwCmd,
         pszParse ? pszParse : L"NONE");
      m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), szTemp, 3);
   }

   // turn off TTS
   m_pTelControl->m_pQueue->AudioReset();

   // Call OnPhraseHypothesisParse
   if (fRecog)
      m_pTelControl->OnPhraseHypothesisParse (dwCmd, pszParse, dwParseSize, pSRPhrase, pIUnknownResult);

   // Free up parse information
   if (pszParse)
      CoTaskMemFree (pszParse);

   return NOERROR;
}

STDMETHODIMP CTCSRGramNotifySink::PhraseStart (QWORD qTimeStampBegin)
{
   // log
   if (m_pTelControl->m_pLog)
      m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), L"PhraseStart", 4);

   return NOERROR;
}

STDMETHODIMP CTCSRGramNotifySink::ReEvaluate (LPUNKNOWN pIUnknown)
{
    return NOERROR;
}

STDMETHODIMP CTCSRGramNotifySink::Training (DWORD dwTrain)
{
    return NOERROR;
}

STDMETHODIMP CTCSRGramNotifySink::UnArchive (LPUNKNOWN pIUnknown)
{
    return NOERROR;
}

STDMETHODIMP CTCSRGramNotifySink::BookMark (DWORD dwMark)
{
    return NOERROR;
}

STDMETHODIMP CTCSRGramNotifySink::Paused (void)
{
    return NOERROR;
}




/*************************************************************************
CTCSRNotifySink - This notification sink is provided by the grammar to the
   SR engine when it starts listening. It's mostly used for logging but
   can be used for notifications if the user is too loud/quiet, etc.

   ::AttribChanged - return NOERROR
   ::Interference - Call OnInterference
   ::Sound - return NOERROR
   ::UtteranceBegin - Logged. Return NOERROR
   ::UtteranceEnd - Logged. Return NOERROR
   ::VUMeter - Return NOERROR. Use this to do timer-outs
*/
CTCSRNotifySink::CTCSRNotifySink (CTelControlFramework* pCtrl)
{
   m_pTelControl = pCtrl;
}

CTCSRNotifySink::~CTCSRNotifySink (void)
{
// this space intentionally left blank
}

STDMETHODIMP CTCSRNotifySink::QueryInterface (REFIID riid, LPVOID *ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRNotifySinkW)) {
      *ppv = (LPVOID) this;
	AddRef();
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CTCSRNotifySink::AddRef (void)
{
   // normally this increases a reference count, but this object
   // is going to be freed as soon as the app is freed, so it doesn't
   // matter
   return 1;
}

STDMETHODIMP_(ULONG) CTCSRNotifySink::Release (void)
{
   // normally this releases a reference count, but this object
   // is going to be freed when the application is freed so it doesnt
   // matter
   return 1;
};


STDMETHODIMP CTCSRNotifySink::AttribChanged(DWORD dwID)
{
   return NOERROR;
}

STDMETHODIMP CTCSRNotifySink::Interference(QWORD qwStart, QWORD qwEnd, DWORD dwID)
{
   // log
   if (m_pTelControl->m_pLog) {
      WCHAR szTemp[64];
      swprintf (szTemp, L"Interference = %d", (int) dwID);
      m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), szTemp, 3);
   }

   m_pTelControl->OnInterference (dwID);

   return NOERROR;
}

STDMETHODIMP CTCSRNotifySink::Sound(QWORD qwTime,QWORD qwEnd)
{
   return NOERROR;
}

STDMETHODIMP CTCSRNotifySink::UtteranceBegin(QWORD qwTime)
{
   // log
   if (m_pTelControl->m_pLog)
      m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), L"UtteranceBegin", 3);

   return NOERROR;
}

STDMETHODIMP CTCSRNotifySink::UtteranceEnd(QWORD qwStart, QWORD qwEnd)
{
   // log
   if (m_pTelControl->m_pLog)
      m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), L"UtteranceEnd", 3);

   return NOERROR;
}

STDMETHODIMP CTCSRNotifySink::VUMeter(QWORD qwTime, WORD wLevel)
{

   if (m_pTelControl->m_dwTimeOut && (GetTickCount() > m_pTelControl->m_dwTimeOut) ) {
      // kill timer
      m_pTelControl->m_dwTimeOut = 0;
      m_pTelControl->m_DTMFcount = 0;  // reset DTMF

      // stop speech recognition
      m_pTelControl->StopSR();

      // log this
      if (m_pTelControl->m_pLog)
         m_pTelControl->m_pLog->Text (m_pTelControl->GetControlName(), L"OnNoAnswer", 1);

      // increate haven't answered tim
      m_pTelControl->m_dwNoAnswer++;

      if (m_pTelControl->m_dwNoAnswer >= m_pTelControl->m_dwNoAnswerMax)
         // hasn't answered N times in a row, so abort
         m_pTelControl->DoFinish (TCR_NORESPONSE, NULL, 0);
      else
         // call no answer
         m_pTelControl->OnNoAnswer();
   }

   return NOERROR;
}



/*************************************************************************
CTCSTTTTSQueueNotifySink - Notification sink from the TTS queue. The application
   should not override these, but should override some of the OnXXX calls.

   ::Start - Call OnTTSStart.
   ::Stop - Call OnTTSStop
   ::SpeakID - Call OnTTSSpeakID
   ::Error - Call OnTTSError
   ::Bookmark - Call OnTTSBookmark
*/
CTCSTTTTSQueueNotifySink::CTCSTTTTSQueueNotifySink(CTelControlFramework* pCtrl)
{
    m_pTelControl = pCtrl;
}

CTCSTTTTSQueueNotifySink::~CTCSTTTTSQueueNotifySink()
{
    // Intentionally left blank
}


STDMETHODIMP CTCSTTTTSQueueNotifySink::QueryInterface (REFIID riid, LPVOID *ppv)
{
    *ppv = NULL;

    /* always return our IUnkown for IID_IUnknown */
    if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISTTTSQueueNotifySink)) {
        *ppv = (LPVOID) this;
	AddRef();
        return NOERROR;
    }

    // otherwise, cant find
    return E_NOINTERFACE;
}

STDMETHODIMP_ (ULONG) CTCSTTTTSQueueNotifySink::AddRef (void)
{
   // normally this increases a reference count, but this object
   // is going to be freed as soon as the app is freed, so it doesn't
   // matter
   return 1;
}

STDMETHODIMP_(ULONG) CTCSTTTTSQueueNotifySink::Release (void)
{
   // normally this releases a reference count, but this object
   // is going to be freed when the application is freed so it doesnt
   // matter
   return 1;
};


STDMETHODIMP CTCSTTTTSQueueNotifySink::Start ()
{
   m_pTelControl->OnTTSStart();
   return NOERROR;
}

STDMETHODIMP CTCSTTTTSQueueNotifySink::Stop ()
{
   m_pTelControl->OnTTSStop();
   return NOERROR;
}

STDMETHODIMP CTCSTTTTSQueueNotifySink::SpeakID (DWORD dwID)
{
   m_pTelControl->OnTTSSpeakID(dwID);
   return NOERROR;
}

STDMETHODIMP CTCSTTTTSQueueNotifySink::Error (HRESULT hr)
{
   m_pTelControl->OnTTSError(hr);
   return NOERROR;
}

STDMETHODIMP CTCSTTTTSQueueNotifySink::Bookmark (DWORD dwMark)
{
   m_pTelControl->OnTTSBookmark(dwMark);
   return NOERROR;
}




/*************************************************************************
Description of member variables -

m_pszText - Control text from application. might be NULL.
m_pszDefaultText - Default text. Might be NULL.
m_pTextParse - Parse control with text from application. Might be NULL.
m_pDefaultTextParse - Parse control with text from default. Might be NULL.
m_dwDefaultTextResIDLoaded - Resource ID of the text currently loaded.

m_fStarted - TRUE if control started but not yet stopped.
m_fIsCompiled - TRUE if data is compiled
m_lang - language info
m_szRuleToActive - Rule to activate when we next get the change
m_szRuleActive - Currently active rule
m_fIsSRActive - TRUE if SR has rule activated, FALSE if not
m_fActivateWhenDoneTalking - TRUE if should activate when done talking
m_fFullDuplex - TRUE if full duplex connection with echo cancelling. FALSE otherwise
m_dwCurState - Current state number. Changed on a GoState() call.

m_dwTimeOut - TickCount at which point the time-out happens. 0 if no timer
m_dwNoAnswerTime - # of milliseconds before it says no answer
m_dwNoAnswer - Number of consecutives times that user has failed to answer
m_dwNoAnswerMax - Maximum number of times that this can happen before auto hang up

m_cRef - Reference count for the object
m_plObjCount - Where object count is incremented and decremented
m_hModule - Module handle of control's DLL

m_pISRNotifySink - SR notification sink
m_dwSRKey- notification sink key for unregistering
m_pISRGramNotifySink - SR Grammar notification sink
m_pISTTTSQueueNotifySink - TTS queue notify sink
m_pITelNotfiySink - TelInfo notify sink
m_fNotifySinkUsed - TRUE if the notification sinks are connected

m_pSRMode - SR mode
m_pGram - grammar object
m_pGramComp - compiler object
m_pQueue - TTS queue object
m_pLog - logging object
m_pTelInfo - telephony info object

m_pITelControlNotifySink - telephony control notify sink

m_pszPreSpeakItem - Item in [Promtps] to speak before speaking m_pszVerifySpeak
m_pszPostSpeakItem - Item in [Prompts] to speak after speaking m_pszVerifySpeak
m_pszVerifySpeak - String that is to be verified, like a series of digits.
m_dwOnCorrectState - State to go to if user says data is correct.
m_dwOnWrongState - State to go to if user says data is incorrect
m_fVerify - Will be TRUE if verification is indicated by the control file, FALSE if not
*/

