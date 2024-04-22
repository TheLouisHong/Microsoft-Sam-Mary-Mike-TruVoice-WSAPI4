/*****************************************************************
Test1.cpp - Tests 0 - 99.
*/

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

// show information about the mode
void CALLBACK Test000 (void)
{
   TTSMODEINFOW mi;
   HRESULT  hRes;
   char     szTemp[256];
   WCHAR    szwTemp[256];

   hRes = gpTTSMode->ModeGet (&mi);
   if (hRes) {
      Failed ("ModeGet", hRes);
      return;
   }

   // print out statistics ar TTSMode
   StringFromGUID2 (mi.gEngineID, szwTemp, sizeof(szwTemp)/2);
   WCTMB (szwTemp, szTemp);
   Information ("Engine ID: ", FALSE);
   Information (szTemp);

   StringFromGUID2 (mi.gModeID, szwTemp, sizeof(szwTemp)/2);
   WCTMB (szwTemp, szTemp);
   Information ("Mode ID: ", FALSE);
   Information (szTemp);

   WCTMB (mi.szMfgName, szTemp);
   Information ("Manufacturer Name: ", FALSE);
   Information (szTemp);

   WCTMB (mi.szProductName, szTemp);
   Information ("Product Name: ", FALSE);
   Information (szTemp);

   WCTMB (mi.szModeName, szTemp);
   Information ("Mode Name: ", FALSE);
   Information (szTemp);

   Information ("Language: LangID = ", FALSE);
   wsprintf (szTemp, "%lx", (DWORD)mi.language.LanguageID);
   Information (szTemp, FALSE);
   Information (", Dialect = ", FALSE);
   WCTMB (mi.language.szDialect, szTemp);
   Information (szTemp);

   WCTMB (mi.szSpeaker, szTemp);
   Information (szTemp);
   Information ("Speaker: ", FALSE);

   WCTMB (mi.szStyle, szTemp);
   Information ("Style: ", FALSE);
   Information (szTemp);

   Information ("Gender: ", FALSE);
   switch (mi.wGender) {
   case GENDER_NEUTRAL:
      Information ("Neutral");
      break;
   case GENDER_MALE:
      Information ("Male");
      break;
   case GENDER_FEMALE:
      Information ("Female");
      break;
   default:
      Information ("Unknown");
   }

   Information ("Age: ", FALSE);
   switch (mi.wAge) {
   case TTSAGE_BABY:
      Information ("Baby");
      break;
   case TTSAGE_TODDLER:
      Information ("Toddler");
      break;
   case TTSAGE_CHILD:
      Information ("Child");
      break;
   case TTSAGE_ADOLESCENT:
      Information ("Adolescent");
      break;
   case TTSAGE_ADULT:
      Information ("Adult");
      break;
   case TTSAGE_ELDERLY:
      Information ("Elderly");
      break;
   default:
      Information ("Unknown");
   }

   Information ("Features:");
   if (mi.dwFeatures & TTSFEATURE_ANYWORD)
      Information ("\tCan speak any word (letter-to-sound rules)");
   if (mi.dwFeatures & TTSFEATURE_VOLUME)
      Information ("\tSupports volume control");
   if (mi.dwFeatures & TTSFEATURE_SPEED)
      Information ("\tSupports speaking rate");
   if (mi.dwFeatures & TTSFEATURE_PITCH)
      Information ("\tSupports pitch modification");
   if (mi.dwFeatures & TTSFEATURE_TAGGED)
      Information ("\tSupport tagged text");
   if (mi.dwFeatures & TTSFEATURE_IPAUNICODE)
      Information ("\tSupports IPA pronunciations");
   if (mi.dwFeatures & TTSFEATURE_VISUAL)
      Information ("\tLip-sync notifications");
   if (mi.dwFeatures & TTSFEATURE_WORDPOSITION)
      Information ("\tWord position sync notifications");
   if (mi.dwFeatures & TTSFEATURE_PCOPTIMIZED)
      Information ("\tOptimized for the PC");
   if (mi.dwFeatures & TTSFEATURE_PHONEOPTIMIZED)
      Information ("\tOptimized for the phone");
   if (mi.dwFeatures & TTSFEATURE_FIXEDAUDIO)
      Information ("\tDoesn't get audio from the audio source");
   if (mi.dwFeatures & TTSFEATURE_SINGLEINSTANCE)
      Information ("\tOnly supports one instance");
   if (mi.dwFeatures & TTSFEATURE_THREADSAFE)
      Information ("\tThread safe from all threads");
   if (mi.dwFeatures & TTSFEATURE_IPATEXTDATA)
      Information ("\tSupport IPA phonemes from text-data");
   if (mi.dwFeatures & TTSFEATURE_PREFERRED)
      Information ("\tSupports a preferred voice");
   if (mi.dwFeatures & TTSFEATURE_TRANSPLANTED)
      Information ("\tSupports transplanted prosody");
   if (mi.dwFeatures & TTSFEATURE_SAPI4)
      Information ("\tIs SAPI4 compliant");


   Information ("Interfaces:");
   if (mi.dwInterfaces & TTSI_ILEXPRONOUNCE)
      Information ("\tILexPronounce");
  if (mi.dwInterfaces & TTSI_ITTSATTRIBUTES)
      Information ("\tIITTSAttributes");
   if (mi.dwInterfaces & TTSI_ITTSCENTRAL)
      Information ("\tITTSCentral");
   if (mi.dwInterfaces & TTSI_ITTSDIALOGS)
      Information ("\tITTSDialogs");
   if (mi.dwInterfaces & TTSI_ATTRIBUTES)
      Information ("\tIAttributes");
   if (mi.dwInterfaces & TTSI_ILEXPRONOUNCE2)
      Information ("\tILexPronounce2");
 

   wsprintf (szTemp, "Engine features: %lx", (int) mi.dwEngineFeatures);
   Information (szTemp);


   // print out statistics about the wave format
   Information();
   Information ("Wave format information");
   wsprintf (szTemp, "Primary wave format: %d", (int) gpWFEXTTS->wFormatTag);
   Information (szTemp);
   if (gpWFEXTTS->wFormatTag == WAVE_FORMAT_PCM)
      Information ("\t(This is PCM data)");
   wsprintf (szTemp, "Channels: %d", (int) gpWFEXTTS->nChannels);
   Information (szTemp);
   wsprintf (szTemp, "Samples per sec.: %d", (int) gpWFEXTTS->nSamplesPerSec);
   Information (szTemp);
   wsprintf (szTemp, "Avg. bytes per sec.: %d", (int) gpWFEXTTS->nAvgBytesPerSec);
   Information (szTemp);
   wsprintf (szTemp, "Block size: %d", (int) gpWFEXTTS->nBlockAlign);
   Information (szTemp);
   wsprintf (szTemp, "Bits per sample: %d", (int) gpWFEXTTS->wBitsPerSample);
   Information (szTemp);

   Passed ();


}


void CALLBACK Test001 (void)
{
   // checks to make sure the engine claims to support this
   NEEDENGLISH;

   char  *psz;
   psz = SpeakAndListen ( "Hello world",
      "[<Start>]\n"
      "<Start>=hello world\n"
      );
   if (!psz)
      return;

   // if didn't recognize hello world then worry
   if (stricmp(psz, "hello world")) {
      Warning ("Didn't recognize hello world");
   }
   else {
      Passed();
   }
}


//- Look at modeinfo and make sure all necessary parameters are filled in,
//and that the requisite interfaces are there
// - Make sure that the ansi interfaces are also supported

void CALLBACK Test002 (void)
{
   BOOL  fPassed = TRUE;

   // make sure teh guids are not null
   if (IsEqualGUID (gTTSModeInfo.gEngineID, GUID_NULL)) {
      fPassed = FALSE;
      Failed ("TTSModeInfo.gEngineID guid is NULL");
   }

   if (IsEqualGUID (gTTSModeInfo.gModeID, GUID_NULL)) {
      fPassed = FALSE;
      Failed ("TTSModeInfo.gModeID guid is NULL");
   }


   // make sure the strings have data
   if (!gTTSModeInfo.szMfgName[0]) {
      fPassed = FALSE;
      Failed ("TTSModeInfo.szMfgName is empty");
   }
   if (!gTTSModeInfo.szProductName[0]) {
      fPassed = FALSE;
      Failed ("TTSModeInfo.szProductName is empty");
   }
   if (!gTTSModeInfo.szModeName[0]) {
      fPassed = FALSE;
      Failed ("TTSModeInfo.szModeName is empty");
   }
   if (!gTTSModeInfo.szSpeaker[0]) {
      fPassed = FALSE;
      Failed ("TTSModeInfo.szSpeaker is empty");
   }
   if (!gTTSModeInfo.szStyle[0]) {
      fPassed = FALSE;
      Failed ("TTSModeInfo.szStyle is empty");
   }

   // make sure there's a langauge
   if (!gTTSModeInfo.language.LanguageID) {
      fPassed = FALSE;
      Failed ("TTSModeInfo - Not language specified");
   }

   switch (gTTSModeInfo.wGender) {
   case GENDER_NEUTRAL:
   case GENDER_MALE:
   case GENDER_FEMALE:
      break;
   default:
      Failed ("Unknown gender");
   }

   switch (gTTSModeInfo.wAge) {
   case TTSAGE_BABY:
   case TTSAGE_TODDLER:
   case TTSAGE_CHILD:
   case TTSAGE_ADOLESCENT:
   case TTSAGE_ADULT:
   case TTSAGE_ELDERLY:
      break;
   default:
      Warning ("Non-standard age");
   }

#define TTSFEATURE_ALL      0x0001ffff
   if (gTTSModeInfo.dwFeatures & ~TTSFEATURE_ALL)
      Warning ("TTSModeInfo.dwFeatutres - Supporting unknown features");
   if (!(gTTSModeInfo.dwFeatures & (TTSFEATURE_PCOPTIMIZED | TTSFEATURE_PHONEOPTIMIZED)))
      Warning ("TTSModeInfo.dwFeatures - Doesn't support PCOptimized or PhoneOptimized");
   if (gTTSModeInfo.dwFeatures & TTSFEATURE_FIXEDAUDIO)
      Warning ("TTSModeInfo.dwFeatures - Engine had fixed audio, so some tests may not work.");
   if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_TAGGED))
      Failed ("TTSModeInfo.dwFeatures - Engine must support tagged text.");

   // warnings for some features because we recommend that they're implemented
   if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_ANYWORD))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine be modified so it can speak any work");
   if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_VOLUME))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine be modified so it can control volume");
   if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_SPEED))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine be modified so it can control speed");
   if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_PITCH))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine be modified so it can control pitch");
   if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_IPAUNICODE))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine be modified so it can control Unicode IPA");
	if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_VISUAL))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine be modified so it provides lip sync");
   if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_WORDPOSITION))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine be modified so it notifies on word position");
   if ((gTTSModeInfo.dwFeatures & TTSFEATURE_SINGLEINSTANCE))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine be modified so it can support multiple instances");
   if ((gTTSModeInfo.dwFeatures & TTSFEATURE_THREADSAFE))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine be modified so it is thread safe");
   if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_IPATEXTDATA))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine supports IPA in TextData calls");
   if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_PREFERRED))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine supports a preferred voice");
   if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_TRANSPLANTED))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine supports transplanted prosody");
   if (!(gTTSModeInfo.dwFeatures & TTSFEATURE_SAPI4))
      Warning ("TTSModeInfo.dwFeatures - Suggest that the engine be modified to support SAPI4 features");


   // interfaces
#define TTSI_ALL 0x003f
   if (gTTSModeInfo.dwInterfaces & ~TTSI_ALL)
      Warning ("TTSModeInfo.dwInterfaces - Supporting unknown interfaces");
   if (!(gTTSModeInfo.dwInterfaces & TTSI_ITTSATTRIBUTES)) {
      fPassed = FALSE;
      Failed ("TTSModeInfo.dwInterfaces - Must support ITTSAttributes");
   }
   if (!(gTTSModeInfo.dwInterfaces & TTSI_ITTSCENTRAL)) {
      fPassed = FALSE;
      Failed ("TTSModeInfo.dwInterfaces - Must support ITTSCentral");
   }
   if (!(gTTSModeInfo.dwInterfaces & TTSI_ITTSDIALOGS)) {
      fPassed = FALSE;
      Failed ("TTSModeInfo.dwInterfaces - Must support ITTSDialogs");
   }

// warnings for features that are recommened
   if (!(gTTSModeInfo.dwInterfaces & TTSI_ILEXPRONOUNCE)) {
      fPassed = FALSE;
      Warning ("TTSModeInfo.dwInterfaces - recommended support ILexPronounce");
   }
   if (!(gTTSModeInfo.dwInterfaces & TTSI_IATTRIBUTES)) {
      fPassed = FALSE;
      Warning ("TTSModeInfo.dwInterfaces - recommended support IAttributes");
   }
   if (!(gTTSModeInfo.dwInterfaces & TTSI_ILEXPRONOUNCE2)) {
      fPassed = FALSE;
      Warning ("TTSModeInfo.dwInterfaces - recommended support ILexPronounce2");
   }

   if (fPassed)
      Passed();
}


// - Look through the interfaces claimed and make sure they exist
void CALLBACK Test003 (void)
{
   LPUNKNOWN   pUnk;

   if (gTTSModeInfo.dwInterfaces & TTSI_ILEXPRONOUNCE) {
      if (!gpTTSMode->m_pILexPronounce) 
         Failed ("Doesn't support ILexPronounce like it claims.");

      gpTTSMode->m_pITTSCentral->QueryInterface (IID_ILexPronounceA, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ILexPronounce though it claims the interface is supported.");
   }
   else {
      if (gpTTSMode->m_pILexPronounce)
         Warning ("Supports ILexPronounce even though TTSMODEINFO claims otherwise.");
   }

   if (gTTSModeInfo.dwInterfaces & TTSI_ILEXPRONOUNCE2) {
      if (!gpTTSMode->m_pILexPronounce2) 
         Failed ("Doesn't support ILexPronounce2 like it claims.");

      gpTTSMode->m_pITTSCentral->QueryInterface (IID_ILexPronounce2A, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ILexPronounce2 though it claims interface is supported.");
   }
   else {
      if (gpTTSMode->m_pILexPronounce2)
         Warning ("Supports ILexPronounce2 even though TTSMODEINFO claims otherwise.");
   }


   if (gTTSModeInfo.dwInterfaces & TTSI_ITTSATTRIBUTES) {
      if (!gpTTSMode->m_pITTSAttributes) 
         Failed ("Doesn't support ITTSAttributes like it claims.");

      gpTTSMode->m_pITTSCentral->QueryInterface (IID_ITTSAttributesA, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ITTSAttributes.");
   }
   else {
      if (gpTTSMode->m_pITTSAttributes)
         Warning ("Supports ITTSAttributes even though TTSMODEINFO claims otherwise.");
   }

   if (gTTSModeInfo.dwInterfaces & TTSI_IATTRIBUTES) {
      if (!gpTTSMode->m_pIAttributes) 
         Failed ("Doesn't support IAttributes like it claims.");

      gpTTSMode->m_pITTSCentral->QueryInterface (IID_IAttributesA, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of IAttributes.");
   }
   else {
      if (gpTTSMode->m_pIAttributes)
         Warning ("Supports IAttributes even though TTSMODEINFO claims otherwise.");
   }


   if (gTTSModeInfo.dwInterfaces & TTSI_ITTSCENTRAL) {
      if (!gpTTSMode->m_pITTSCentral) 
         Failed ("Doesn't support ITTSCentral like it claims.");

      gpTTSMode->m_pITTSCentral->QueryInterface (IID_ITTSCentralA, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ITTSCentral.");
   }
   else {
      if (gpTTSMode->m_pITTSCentral)
         Warning ("Supports ITTSCentral even though TTSMODEINFO claims otherwise.");
   }

   if (gTTSModeInfo.dwInterfaces & TTSI_ITTSDIALOGS) {
      if (!gpTTSMode->m_pITTSDialogs) 
         Failed ("Doesn't support ITTSDialogs like it claims.");

      gpTTSMode->m_pITTSCentral->QueryInterface (IID_ITTSDialogsA, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ITTSDialogs.");
   }
   else {
      if (gpTTSMode->m_pITTSDialogs)
         Warning ("Supports ITTSDialogs even though TTSMODEINFO claims otherwise.");
   }

   Passed();
}

// - If have PHONEOPTIMIZED, make sure default rate is 8 kHz 16-bit

void CALLBACK Test004 (void)
{
   if ((gTTSModeInfo.dwFeatures & TTSFEATURE_PHONEOPTIMIZED) &&
      !(gTTSModeInfo.dwFeatures & TTSFEATURE_PCOPTIMIZED))
   {
      // phone optimized by defaukt
   if ((gpWFEXTTS->wFormatTag != WAVE_FORMAT_PCM) ||
      (gpWFEXTTS->nChannels != 1) ||
      (gpWFEXTTS->nSamplesPerSec != 8000) ||
      (gpWFEXTTS->nAvgBytesPerSec != 16000) ||
      (gpWFEXTTS->nBlockAlign != 2) ||
      (gpWFEXTTS->wBitsPerSample != 16))
      Failed ("Telephone-optimized TTS engines must support 16-bit 8 kHz mono.");
   else
      Passed();
   }
   else {
      // not phone optimized, so not applicable
      NotApplicable();
   }
}

// - Make sure the enumerator works - can be re-enumerated and get the same results,
// along with skip, etc.
void CALLBACK Test005 (void)
{
   TTSMODEINFOW ami[200];  // large number of mode infos
   DWORD       dwModes;
   HRESULT     hRes;

   gpTTSEnum->Reset();
   dwModes = 0;
   memset (ami, 0, sizeof(ami));
   gpTTSEnum->Next(200, ami, &dwModes);
   
   // try stepping through individually and comparing, skipping
   // occasionally
   DWORD i, dwSkip;
   CTTSEnum *pNew;
   pNew = gpTTSEnum->Clone();
   if (!pNew) {
      Failed ("Can't clone enumerator");
      return;
   }
	// use all possible skip values
	for( dwSkip = 0; dwSkip < (dwModes - 1); dwSkip++)
	{
	   pNew->Reset();
		// skip off each mode
		for (i = 0; i < dwModes; i++) {
			// skip if possible
			if( dwModes > (i + dwSkip) ) {
				pNew->Skip(dwSkip);
				i += dwSkip;
			}
			else {
			// attempt skip anyway, should fail
				hRes = pNew->Skip(dwSkip + 1);
				if( hRes != E_INVALIDARG && hRes != E_UNEXPECTED && hRes != S_FALSE )
					Failed ("no or wrong error on skip out of range", hRes);

				// make sure it's still set to the right element
				pNew->Reset();
				pNew->Skip( i );
			}
		
			// get and compare
			TTSMODEINFOW mi;
			memset (&mi, 0, sizeof(mi));
			hRes = pNew->Next (1, &mi, NULL);
			if (FAILED(hRes))
				Failed ("Next returned an error", hRes);

			// compare the two results
			if ( !IsEqualGUID(mi.gEngineID, ami[i].gEngineID) ||
				wcscmp(mi.szMfgName, ami[i].szMfgName) ||
				wcscmp(mi.szProductName, ami[i].szProductName) ||
				!IsEqualGUID(mi.gModeID, ami[i].gModeID) ||
				wcscmp(mi.szModeName, ami[i].szModeName) ||
				(mi.language.LanguageID != ami[i].language.LanguageID) ||
				wcscmp(mi.language.szDialect, ami[i].language.szDialect) ||
				(mi.dwFeatures != ami[i].dwFeatures) ||
				(mi.dwInterfaces != ami[i].dwInterfaces) ||
				(mi.dwEngineFeatures != ami[i].dwEngineFeatures)
				)
				Failed ("Modes are different");
		}
	}

	// make sure enumerator is reference counting correctly
	// if the COM release was not done right it may crash here
	if( gpTTSEnum ) {
		delete gpTTSEnum;  
   
		// create it again
		gpTTSEnum = new CTTSEnum;
		gpTTSEnum->Init();
	}

	hRes = DestroyTTSEngine();
   CHECKFAILED ("DestroyTTSEngine");

   hRes = CreateTTSEngine();
   CHECKFAILED ("CreateTTSEngine");

   if (pNew)
      delete pNew;

   Passed();
}


// - Close & reload the engine
void CALLBACK Test006 (void)
{
   HRESULT  hRes;
   hRes = DestroyTTSEngine();
   CHECKFAILED ("DestroyTTSEngine");

   hRes = CreateTTSEngine();
   CHECKFAILED ("CreateTTSEngine");

   Passed();
}

// - Close & releoad the engine while listening/talking
void CALLBACK Test007 (void)
{
   HRESULT     hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;

   // speak
   hRes = SpeakToWave ("This is a test\\mrk=5\\ of a sentence for shut-down",
      NULL, NULL, &gTTSBufNS.m_fBookMark);
   if (hRes != S_FALSE)
      Failed ("Didn't abort at bookmark");


   hRes = DestroyTTSEngine();
   CHECKFAILED ("DestroyTTSEngine");

   hRes = CreateTTSEngine();
   CHECKFAILED ("CreateTTSEngine");

   Passed ();
}


// - Reference counting on the audio source/destination object
void CALLBACK Test008 (void)
{
   HRESULT     hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;

   // pull the audio object out of the clutches of the TTS engine
   // We'll addref it, so it should have a reference count if
   // 3 after destroy engine is called. One from this, and two from
   // the C++ wrapper
   LPUNKNOWN   pUnk;
   pUnk = gpTTSMode->m_pUnkAudio;
   pUnk->AddRef();
   gpTTSMode->m_pUnkAudio = NULL;

   hRes = DestroyTTSEngine();
   CHECKFAILED ("DestroyTTSEngine");

   // check the refcount
   DWORD dwRef;
   dwRef = pUnk->Release();
   while (pUnk->Release());   // release until

   hRes = CreateTTSEngine();
   CHECKFAILED ("CreateTTSEngine");

   if (dwRef != 1)
      Failed ("Audio object not completely released.");
   else
      Passed();
}

// - Reference counting on the main notification sinks, when register & unregister
void CALLBACK Test009 (void)
{
   HRESULT     hRes;

   // destroy the current TTSnotify sink
   hRes = gpTTSMode->UnRegister (gdwTTSNSKey);
   CHECKFAILED ("UnRegister");

   // set the ref count flag
   gTTSNS.m_dwRefCount = 0;

   // regsiter
   hRes = gpTTSMode->Register (&gTTSNS, &gdwTTSNSKey);
   CHECKFAILED ("Register");

   // make sure that the ref count incremented
   if (!gTTSNS.m_dwRefCount)
      Failed ("ITTSCentral::Register didn't increase the reference count");

   // unregister
   hRes = gpTTSMode->UnRegister (gdwTTSNSKey);
   CHECKFAILED ("UnRegister");

   // make sure the ref count is 0
   if (gTTSNS.m_dwRefCount)
      Failed ("ITTSCentral::UnRegister didn't release all the references");

   // reregsiter
   hRes = gpTTSMode->Register (&gTTSNS, &gdwTTSNSKey);
   CHECKFAILED ("Register");

   Passed();
}

// - Reference counting on the main notification sinks, when register & close down
void CALLBACK Test010 (void)
{
   HRESULT     hRes;

   // destroy the current TTSnotify sink
   hRes = gpTTSMode->UnRegister (gdwTTSNSKey);
   CHECKFAILED ("UnRegister");

   // set the ref count flag
   gTTSNS.m_dwRefCount = 0;

   // regsiter
   hRes = gpTTSMode->Register (&gTTSNS, &gdwTTSNSKey);
   CHECKFAILED ("Register");

   // make sure that the ref count incremented
   if (!gTTSNS.m_dwRefCount)
      Failed ("ITTSCentral::Register didn't increase the reference count");

   // destroy the engine
   hRes = DestroyTTSEngine();
   CHECKFAILED ("DestroyTTSEngine");

   // make sure the ref count is 0
   if (gTTSNS.m_dwRefCount)
      Failed ("ITTSCentral::UnRegister didn't release all the references");

   // create the engine
   hRes = CreateTTSEngine();
   CHECKFAILED ("CreateTTSEngine");

   Passed();
}


// - Reference counting on the buffer notification sinks, when start using them properly,
// and shut down after they should be released
void CALLBACK Test011 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;

   // set count to 0
   gTTSBufNS.m_dwRefCount = 0;

   // speak
   hRes = gpTTSMode->Speak (L"Hello world", TRUE, &gTTSBufNS);
   CHECKFAILED ("Speak");

   if (!gTTSBufNS.m_dwRefCount)
      Failed ("Speak-call didn't increase reference count");

   // audio reset
   hRes = gpTTSMode->AudioReset();
   CHECKFAILED ("AudioReset");

   WaitForEvent (60.0, &gTTSBufNS.m_fTextDataDone);

   if (gTTSBufNS.m_dwRefCount)
      Failed ("Audio-Reset didn't realease reference count");

   Passed ();
}


// - Reference counting on the buffer notification sinks, when start using them properly,
// and shut down after they should be released
void CALLBACK Test012 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;

   gTTSBufNS.m_dwRefCount = 0;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToWave ("Hello world");
   CHECKFAILED ("SpeakToWave");

   if (gTTSBufNS.m_dwRefCount)
      Failed ("When finished speaking didn't realease reference count");

   Passed ();
}


// - Close the engine while talking, and make sure reference counting is good
void CALLBACK Test013 (void)
{
   HRESULT     hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;

   // set reference counts
   gTTSBufNS.m_dwRefCount = 0;

   // destroy the current TTSnotify sink
   hRes = gpTTSMode->UnRegister (gdwTTSNSKey);
   CHECKFAILED ("UnRegister");

   // set the ref count flag
   gTTSNS.m_dwRefCount = 0;

   // regsiter
   hRes = gpTTSMode->Register (&gTTSNS, &gdwTTSNSKey);
   CHECKFAILED ("Register");

   // speak
   hRes = SpeakToWave ("This is a test\\mrk=5\\ of a sentence for shut-down",
      NULL, NULL, &gTTSBufNS.m_fBookMark);
   if (hRes != S_FALSE)
      Failed ("Didn't abort at bookmark");


   hRes = DestroyTTSEngine();
   CHECKFAILED ("DestroyTTSEngine");

   // make sure they're 0
   if (gTTSBufNS.m_dwRefCount)
      Failed ("Buffer notification sink not released on shut down");
   if (gTTSNS.m_dwRefCount)
      Failed ("Main notification sink not released on shut down.");

   hRes = CreateTTSEngine();
   CHECKFAILED ("CreateTTSEngine");

   Passed ();
}

// - Make sure don't get notifications to main notification sink after it's been unregisterd
void CALLBACK Test014 (void)
{
   HRESULT     hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToWave ("This is a test\\mrk=2\\ of a sentence for shut-down.",
      FALSE, NULL, &gTTSBufNS.m_fBookMark);
   if (hRes != S_FALSE)
      Failed ("Didn't abort at bookmark");

   // release the main notification sink
   hRes = gpTTSMode->UnRegister (gdwTTSNSKey);
   CHECKFAILED ("UnRegister");

   // stop the audio
   hRes = gpTTSMode->AudioReset();
   CHECKFAILED ("AudioReset");

   // wait for a sec
   BOOL *pf;
   pf = WaitForEvent (1.0, &gTTSNS.m_fNotification);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gTTSNS.m_fNotification) {
      // we shouldn't have gotten this
      Failed ("Got an ITTSNotifySink notification after unregistered notification sink");
   }

   // reregister
   hRes = gpTTSMode->Register (&gTTSNS, &gdwTTSNSKey);
   CHECKFAILED ("Register");

   Passed ();
}

// - Make sure don't get notifications to buffer or main notify sink when shut down
void CALLBACK Test015 (void)
{
   HRESULT     hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;

   // speak
   hRes = SpeakToWave ("This is a test\\mrk=5\\ of a sentence for shut-down",
      NULL, NULL, &gTTSBufNS.m_fBookMark);
   if (hRes != S_FALSE)
      Failed ("Didn't abort at bookmark");

   hRes = DestroyTTSEngine();
   CHECKFAILED ("DestroyTTSEngine");

   // set main SN to null
   gTTSBufNS.m_fNotification = FALSE;
   gTTSNS.m_fNotification = FALSE;

   // wait for a second to see if anything is called
   BOOL  *pf;
   pf = WaitForEvent (1.0);
   if (gTTSBufNS.m_fNotification)
      Failed ("ITTSBufNotifySink called after shut down");
   if (gTTSNS.m_fNotification)
      Failed ("ITTSNotifySink called after shut down.");

   hRes = CreateTTSEngine();
   CHECKFAILED ("CreateTTSEngine");

   Passed ();
}

// - Make sure audio object is not started up until it's supposed to (activate or text data)
// - Make sure the audio object is stopped as soon as it's supposed to (deactivate, or reset,
// text-data done)

void CALLBACK Test016 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;

   BOOL  *pf;

   // create a wave
   hRes = GenerateWaveName();
   CHECKFAILED ("GenerateWaveName");

   // make sure no audio's leaking from it now
   pf = WaitForEvent (1.0, &gTTSWANS.m_fPosn);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gTTSWANS.m_fPosn)
      Failed ("TTS engine has audio device open even when not active");

   // speak
   hRes = gpTTSMode->Speak (L"Hello world.\\mrk=2\\ This is\\mrk=3\\ a test.",
      TRUE, &gTTSBufNS);
   CHECKFAILED ("Speak");

   // wait for the bookmark
   pf = WaitForEvent (100.0, &gTTSBufNS.m_fBookMark);
   if (pf == &gfAbortTest)
      return;
   else if (pf != &gTTSBufNS.m_fBookMark)
      Failed ("Didn't get bookmark");

   // pause it
   hRes = gpTTSMode->AudioPause();
   CHECKFAILED ("Pause");

   // make sure no audio's leaking from it now
   pf = WaitForEvent (1.0, &gTTSWANS.m_fPosn);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gTTSWANS.m_fPosn)
      Failed ("TTS engine has audio device open even when not active");

   // resume
   hRes = gpTTSMode->AudioResume();
   CHECKFAILED ("Resume");

   // wait for the bookmark
   pf = WaitForEvent (100.0, &gTTSBufNS.m_fBookMark);
   if (pf == &gfAbortTest)
      return;
   else if (pf != &gTTSBufNS.m_fBookMark)
      Failed ("Didn't get bookmark");

   // do a reset
   hRes = gpTTSMode->AudioReset();
   CHECKFAILED ("AudioReset");

   // make sure no audio's leaking from it now
   pf = WaitForEvent (1.0, &gTTSWANS.m_fPosn);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gTTSWANS.m_fPosn)
      Failed ("TTS engine has audio device open even when not active");


   // second part of this test - pause, speak, resume, when done speaking
   // make sure no audio
   // pause

   // pause it
   hRes = gpTTSMode->AudioPause();
   CHECKFAILED ("Pause");

   // speak
   hRes = gpTTSMode->Speak (L"Last sentence.");
   CHECKFAILED ("Speak");

   // make sure no audio's leaking from it now
   pf = WaitForEvent (1.0, &gTTSWANS.m_fPosn);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gTTSWANS.m_fPosn)
      Failed ("TTS engine has audio device open even when not active");

   // resume
   hRes = gpTTSMode->AudioResume();
   CHECKFAILED ("Resume");

   // wait for the bookmark
   pf = WaitForEvent (100.0, &gTTSNS.m_fAudioStop);
   if (pf == &gfAbortTest)
      return;
   else if (pf != &gTTSNS.m_fAudioStop)
      Failed ("Audio didn't stop");

   // make sure no audio's leaking from it now
   pf = WaitForEvent (1.0, &gTTSWANS.m_fPosn);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gTTSWANS.m_fPosn)
      Failed ("TTS engine has audio device open even when not active");


   Passed ();
}

// - Can have more than one main notification sink, and both get the message
void CALLBACK Test017 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;

   // register a secong notification sink
   CFuncTTSNotifySink nsnew;
   DWORD dwKey;
   hRes = gpTTSMode->Register (&nsnew, &dwKey);
   CHECKFAILED ("Register");
   nsnew.m_fAudioStart = nsnew.m_fAudioStop = FALSE;
   gTTSNS.m_fAudioStart = gTTSNS.m_fAudioStop = FALSE;

   // speak
   hRes = SpeakToWave ("Hello world");
   CHECKFAILED ("SpeakToWave");

   // unregister the notification sink
   hRes = gpTTSMode->UnRegister (dwKey);
   CHECKFAILED ("Register");

   // make sure that all 4 got hit
   if (
      !nsnew.m_fAudioStart || !nsnew.m_fAudioStop ||
      !gTTSNS.m_fAudioStart || !gTTSNS.m_fAudioStop)
      Failed ("Didn't get audio start/stop on both notification sinks.");

   Passed ();
}


// If doesn't support 22 kHz then warn may not work with DirectX
void CALLBACK Test018 (void)
{
   // checks to make sure the engine claims to support this
   NEEDAUDIO;

   if (gTTSModeInfo.dwFeatures & TTSFEATURE_PCOPTIMIZED)
   {
      // PC optimized
   if ((gpWFEXTTS->wFormatTag != WAVE_FORMAT_PCM) ||
      (gpWFEXTTS->nChannels != 1) ||
      (gpWFEXTTS->nSamplesPerSec != 22050) ||
      (gpWFEXTTS->nAvgBytesPerSec != 44100) ||
      (gpWFEXTTS->nBlockAlign != 2) ||
      (gpWFEXTTS->wBitsPerSample != 16))
      Warning ("TTS may not work well with DirectSound applications "
         "because it doesn't default to 22 kHz 16-bit mono.");
   else
      Passed();
   }
   else {
      // not phone optimized, so not applicable
      NotApplicable();
   }

}

// Make sure ttsfeature anyword supported
void CALLBACK Test019 (void)
{
   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDCFG;
   NEEDTTSFEATURE(TTSFEATURE_ANYWORD);

   // make up a random word, send it to the recognizer, and
   // see if it recognizes
   HRESULT hRes;
   hRes = VerifyRecognition ("helllo there",
      "[<Start>]\n"
      "<Start>=helllo there\n"
      "<Start>=h. e. l. l. l. o. there\n"
      );
   CHECKFAILED ("VerifyRecognition");
   Passed();
}


// - Make sure TTSFEATURE_VOLUME is true
void CALLBACK Test020 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE(TTSFEATURE_VOLUME);

   // see if it works
   DWORD dwVal;
   HRESULT  hRes;
   hRes = gpTTSMode->VolumeGet (&dwVal);
   CHECKFAILED ("SpeedGet failed");

   hRes = gpTTSMode->VolumeSet (dwVal);
   CHECKFAILED ("SpeedSet failed");

   Passed();
}

// - Make sure TTSFEATURE_SPEED is true
void CALLBACK Test021 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE(TTSFEATURE_SPEED);

   // see if it works
   DWORD dwVal;
   HRESULT  hRes;
   hRes = gpTTSMode->SpeedGet (&dwVal);
   CHECKFAILED ("SpeedGet failed");

   hRes = gpTTSMode->SpeedSet (dwVal);
   CHECKFAILED ("SpeedSet failed");

   Passed();
}


// - Make sure TTSFEAUTRE_PITCH is true
void CALLBACK Test022 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE(TTSFEATURE_PITCH);

   // see if it works
   WORD dwVal;
   HRESULT  hRes;
   hRes = gpTTSMode->PitchGet (&dwVal);
   CHECKFAILED ("PitchGet failed");

   hRes = gpTTSMode->PitchSet (dwVal);
   CHECKFAILED ("PitchSet failed");

   Passed();
}


// - Make sure buffer notify sinks called in the right order (when two buffers)
void CALLBACK Test023 (void)
{
   // arrange for the output buffer
   HRESULT hRes;
   hRes = GenerateWaveName();
   CHECKFAILED ("GenerateWaveName");

   // create a new notification sink
   CFuncTTSBufNotifySink   BNS;

   // set some flags to 0
   BNS.m_fTextDataStarted = BNS.m_fTextDataDone = BNS.m_fBookMark = 0;
   gTTSBufNS.m_fTextDataStarted = gTTSBufNS.m_fTextDataDone = gTTSBufNS.m_fBookMark = 0;

   // speak to both buffers
   hRes = gpTTSMode->Speak (L"First buffer.", TRUE, &gTTSBufNS);
   CHECKFAILED ("Speak");
   hRes = gpTTSMode->Speak (L"Second \\mrk=1\\ buffer.", TRUE, &BNS);
   CHECKFAILED ("Speak");

   // wait until text data starts for gTTSBusNS
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gTTSBufNS.m_fTextDataStarted);
   if (pf == &gfAbortTest)
      return;
   else if (!pf)
      Failed ("Failed to get expected message.");
   if (BNS.m_fTextDataStarted || BNS.m_fTextDataDone || BNS.m_fBookMark ||
      gTTSBufNS.m_fTextDataDone || gTTSBufNS.m_fBookMark) {
      Failed ("Got message before expected");
		return;
	}

   // wait for a data end for gTTSBufNS
   pf = WaitForEvent (60.0, &gTTSBufNS.m_fTextDataDone);
   if (pf == &gfAbortTest)
      return;
   else if (!pf)
      Failed ("Failed to get expected message.");
   if (BNS.m_fTextDataDone || BNS.m_fBookMark)
      Failed ("Got message before expected");

   // wait for a text data start for BNS
   if (!BNS.m_fTextDataStarted) {
      pf = WaitForEvent (60.0, &BNS.m_fTextDataStarted);
      if (pf == &gfAbortTest)
         return;
      else if (!pf)
         Failed ("Failed to get expected message.");
      if (BNS.m_fTextDataDone || BNS.m_fBookMark) {
         Failed ("Got message before expected");
			return;
		}
   }

   // wait for a bookmark for BNS
	if (!BNS.m_dwBookMarkMark) {
		pf = WaitForEvent (60.0, &BNS.m_fBookMark);
		if (pf == &gfAbortTest)
			return;
		else if (!pf)
			Failed ("Failed to get expected message.");
		if (BNS.m_fTextDataDone)
			Failed ("Got message before expected");
	}

   // wait for a text data done for BNS
   if (!BNS.m_fTextDataDone) {
		pf = WaitForEvent (60.0, &BNS.m_fTextDataDone);
		if (pf == &gfAbortTest)
			return;
		else if (!pf)
			Failed ("Failed to get expected message.");
	}

   Passed();
}

// - Make sure TTSFEATURE_VISUAL is true
void CALLBACK Test024 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDTTSFEATURE (TTSFEATURE_VISUAL);

   // set flag so we know if we got notified
   gTTSNS.m_fVisual = FALSE;

   HRESULT hRes;
   hRes = SpeakToWave ("Hello there");
   CHECKFAILED ("SpeakToWave");

   if (gTTSNS.m_fVisual)
      Passed();
   else
      Failed("Didn't get visual notification");
}

// - Make sure TTSFEATURE_WORDPOSITION is true
void CALLBACK Test025 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDTTSFEATURE (TTSFEATURE_WORDPOSITION);

   // set flag so we know if we got notified
   gTTSBufNS.m_fWordPosition = FALSE;

   HRESULT hRes;
   hRes = SpeakToWave ("Hello there");
   CHECKFAILED ("SpeakToWave");

   if (gTTSBufNS.m_fWordPosition)
      Passed();
   else
      Failed("Didn't get visual notification");
}

// - Text data with just \pau=1000\ should play audio
void CALLBACK Test026 (void)
{
   NEEDAUDIO;

   HRESULT hRes;
   DWORD dwUsed;
   hRes = SpeakToWave ("\\pau=1000\\", NULL, &dwUsed);
   CHECKFAILED ("SpeakToWave");

   float fTime;
   fTime = (float)dwUsed / (float)gpWFEXTTS->nAvgBytesPerSec;

   if ((fTime < .99) || (fTime > 1.01))
      Failed ("Pause of 1 second didn't work");
   else
      Passed();
}

// - Test pause between words
void CALLBACK Test027 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   HRESULT hRes;
   DWORD dwUsed, dwUsed2;
   hRes = SpeakToWave ("Hello. Hello.", NULL, &dwUsed);
   CHECKFAILED ("SpeakToWave");
   hRes = SpeakToWave ("Hello. \\pau=10000\\Hello.", NULL, &dwUsed2);
   CHECKFAILED ("SpeakToWave");

   float fTime;
   fTime = (float)(dwUsed2-dwUsed) / (float)gpWFEXTTS->nAvgBytesPerSec;

   if ((fTime < 9.9) || (fTime > 10.1))
      Failed ("Pause of 10 seconds didn't work");
   else
      Passed();
}

// - Test pause after word
void CALLBACK Test028 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   HRESULT hRes;
   DWORD dwUsed, dwUsed2;
   hRes = SpeakToWave ("Hello.", NULL, &dwUsed);
   CHECKFAILED ("SpeakToWave");
   hRes = SpeakToWave ("Hello. \\pau=10000\\", NULL, &dwUsed2);
   CHECKFAILED ("SpeakToWave");

   float fTime;
   fTime = (float)(dwUsed2-dwUsed) / (float)gpWFEXTTS->nAvgBytesPerSec;

   if ((fTime < 9.9) || (fTime > 10.1))
      Failed ("Pause of 10 seconds didn't work");
   else
      Passed();
}

// - Test pause before word
void CALLBACK Test029 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   HRESULT hRes;
   DWORD dwUsed, dwUsed2;
   hRes = SpeakToWave ("Hello.", NULL, &dwUsed);
   CHECKFAILED ("SpeakToWave");
   hRes = SpeakToWave ("\\pau=10000\\Hello.", NULL, &dwUsed2);
   CHECKFAILED ("SpeakToWave");

   float fTime;
   fTime = (float)(dwUsed2-dwUsed) / (float)gpWFEXTTS->nAvgBytesPerSec;

   if ((fTime < 9.9) || (fTime > 10.1))
      Failed ("Pause of 10 seconds didn't work");
   else
      Passed();
}

// - Bookmark at beginning or end of text, or by itself
void CALLBACK Test030 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   HRESULT hRes;

   // bookmark at the beginning of text
   gTTSBufNS.m_fBookMark = FALSE;
   hRes = SpeakToWave ("\\mrk=1\\Hello.");
   CHECKFAILED ("SpeakToWave");
   if (!gTTSBufNS.m_fBookMark)
      Failed ("Bookmark at beginning of text not set.");

   // bookmark at the end of text
   gTTSBufNS.m_fBookMark = FALSE;
   hRes = SpeakToWave ("Hello.\\mrk=1\\");
   CHECKFAILED ("SpeakToWave");
   if (!gTTSBufNS.m_fBookMark)
      Failed ("Bookmark at end of text not set.");

   // bookmark by itself
   gTTSBufNS.m_fBookMark = FALSE;
   hRes = SpeakToWave ("\\mrk=1\\");
   CHECKFAILED ("SpeakToWave");
   if (!gTTSBufNS.m_fBookMark)
      Failed ("Bookmark by itself not set.");
   Passed();
}

// - Send in an empty string
void CALLBACK Test031 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   HRESULT hRes;

   // empty string
   hRes = SpeakToWave ("");
   CHECKFAILED ("SpeakToWave");

   // empty string
   hRes = SpeakToWave (" ");
   CHECKFAILED ("SpeakToWave");

   // empty string
   hRes = SpeakToWave ("\t");
   CHECKFAILED ("SpeakToWave");

   Passed();
}


// - String with random Unicode characters & make sure speaks (along with some valid chars)

// - String with known good characters and see how long. Stich a non-speakable character
// in the middle and make sure it takes approximately the same amount of time or longer. It
// can't just quit speaking part way through.
void CALLBACK Test032 (void)
{
   NEEDAUDIO;

   // come up with a string to speak
   WCHAR szTemp[256];
   memset (szTemp, 0, sizeof(szTemp));
   wcscpy (szTemp, L"Hello there. This is a rather long text block "
      L"that I want to use to implant a random character.");

   HRESULT hRes;
   DWORD dwUsed, dwUsed2;
   hRes = SpeakToWave (szTemp, NULL, &dwUsed);
   CHECKFAILED ("SpeakToWave");

   // stick a random Unicode character in
   szTemp[rand() % wcslen(szTemp)] = max((WORD) rand(),1);

   hRes = SpeakToWave (szTemp, NULL, &dwUsed2);
   CHECKFAILED ("SpeakToWave");

   if ((dwUsed2 + 1000) < dwUsed)
      Failed ("Aborted speaking because of a bad character.");
   else
      Passed();
}

//- Send in a string with bookmarks and make sure they're hit
void CALLBACK Test033 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   // arrange for the output buffer
   HRESULT hRes;
   hRes = GenerateWaveName();
   CHECKFAILED ("GenerateWaveName");

   // create a new notification sink
   CFuncTTSBufNotifySink   BNS;

   // speak to both buffers
   hRes = gpTTSMode->Speak (
      L"\\mrk=1\\This might be \\mrk=2\\ a test \\mrk=3\\"
      L"of a te\\mrk=4000000000\\xt another mark\\mrk=5\\.",
      TRUE, &gTTSBufNS);
   gTTSNS.m_fAudioStop = 0;
   CHECKFAILED ("Speak");

   BOOL  *pf;
   // wait until first bookmark, 1
   pf = WaitForEvent (60.0, &gTTSBufNS.m_fBookMark);
   if (pf == &gfAbortTest)
      return;
   else if (!pf)
      Failed ("Failed to get expected message.");
   if (gTTSBufNS.m_dwBookMarkMark != 1)
      Failed ("Wrong bookmark");

   // wait until first bookmark, 2
   pf = WaitForEvent (60.0, &gTTSBufNS.m_fBookMark);
   if (pf == &gfAbortTest)
      return;
   else if (!pf)
      Failed ("Failed to get expected message.");
   if (gTTSBufNS.m_dwBookMarkMark != 2)
      Failed ("Wrong bookmark");

   // wait until first bookmark, 3
   pf = WaitForEvent (60.0, &gTTSBufNS.m_fBookMark);
   if (pf == &gfAbortTest)
      return;
   else if (!pf)
      Failed ("Failed to get expected message.");
   if (gTTSBufNS.m_dwBookMarkMark != 3)
      Failed ("Wrong bookmark");

   // wait until first bookmark, 4
   pf = WaitForEvent (60.0, &gTTSBufNS.m_fBookMark);
   if (pf == &gfAbortTest)
      return;
   else if (!pf)
      Failed ("Failed to get expected message.");
   if (gTTSBufNS.m_dwBookMarkMark != 4000000000)
      Failed ("Wrong bookmark");

   // wait until first bookmark, 5
   pf = WaitForEvent (60.0, &gTTSBufNS.m_fBookMark);
   if (pf == &gfAbortTest)
      return;
   else if (!pf)
      Failed ("Failed to get expected message.");
   if (gTTSBufNS.m_dwBookMarkMark != 5)
      Failed ("Wrong bookmark");

   if (!gTTSNS.m_fAudioStop) {
      pf = WaitForEvent (60.0, &gTTSNS.m_fAudioStop);
      if (pf == &gfAbortTest)
         return;
      else if (!pf)
         Failed ("Failed to get expected message.");
   }


   Passed();

}

// test some standard text normalization
void CALLBACK Test034 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   char  *psz;

	// - Phone numbers
   psz = SpeakAndListen ("The phone number is 623-9834",
      "[<Start>]\n"
      "<Start>=the phone number is <Alt>\n"
      "<Alt>=six two three [opt] dash nine eight three four\n"
      );
   if (!psz || stricmp(psz, "the phone number is six two three nine eight three four"))
      Warning ("Phone number");

   psz = SpeakAndListen ("The phone number is 716-623-9834",
      "[<Start>]\n"
      "<Start>=the phone number is <Alt>\n"
      "<Alt>=seven one six [opt] dash six two three [opt] dash nine eight three four\n"
      "<Alt>=area code seven one six six two three nine eight three four\n"
      );
   if (!psz || (stricmp(psz, "the phone number is six two three nine eight three four") &&
      stricmp(psz, "the phone number is area code seven one six six two three nine eight three four")))
      Warning ("Phone number");

   psz = SpeakAndListen ("The phone number is (716)623-9834",
      "[<Start>]\n"
      "<Start>=the phone number is <Alt>\n"
      "<Alt>=seven one six [opt] dash six two three [opt] dash nine eight three four\n"
      "<Alt>=left parenthesis seven one six right parenthesis six two three [opt] dash nine eight three four\n"
      "<Alt>=area code seven one six six two three nine eight three four\n"
      );
   if (!psz || (stricmp(psz, "the phone number is six two three nine eight three four") &&
      stricmp(psz, "the phone number is area code seven one six six two three nine eight three four")))
      Warning ("Phone number");

	// - Numbers
   psz = SpeakAndListen ("516",
      "[<Start>]\n"
      "<Start>=five hundred sixteen\n"
      "<Start>=five one six\n"
      );
   if (!psz || stricmp(psz, "five hundred sixteen"))
      Warning ("Number");

	// - Dates
   psz = SpeakAndListen ("1/9/97",
      "[<Start>]\n"
      "<Start>=one [opt] slash nine [opt] slash [opt] ninety seven\n"
      "<Start>=January ninth [opt] nineteen ninety seven\n"
      "<Start>=January ninth [opt] ninety seven\n"
      );
   if (!psz || (stricmp(psz, "January ninth nineteen ninety seven") && stricmp(psz, "January ninth ninety seven")))
      Warning ("Date");

   psz = SpeakAndListen ("Feb-1",
      "[<Start>]\n"
      "<Start>=feb [opt] dash one\n"
      "<Start>=February first\n"
      );
   if (!psz || stricmp(psz, "February first"))
      Warning ("Date");

	// - Mr., Mrs.
   psz = SpeakAndListen ("Mr. Smith",
      "[<Start>]\n"
      "<Start>=mister smith\n"
      "<Start>=m. r. smith\n"
      "<Start>=mir smith\n"
      );
   if (!psz || stricmp(psz, "mister smith"))
      Warning ("Mr.");

   psz = SpeakAndListen ("Mrs. Smith",
      "[<Start>]\n"
      "<Start>=missus smith\n"
      "<Start>=m. r. s. smith\n"
      "<Start>=mirs smith\n"
      );
   if (!psz || stricmp(psz, "missus smith"))
      Warning ("Mrs.");

	// - Street vs. saint
   psz = SpeakAndListen ("St. John St.",
      "[<Start>]\n"
      "<Start>=<St> john <St>\n"
      "<St>=saint\n"
      "<St>=street\n"
      );
   if (!psz || stricmp(psz, "saint john street"))
      Warning ("Saint vs. Street");

   // - All caps "MICROSOFT" vs. "Microsoft"
   psz = SpeakAndListen ("MICROSOFT",
      "[<Start>]\n"
      "<Start>=Microsoft\n"
      "<Start>=M. I. C. R. O. S. O. F. T.\n"
      );
   if (!psz || stricmp(psz, "Microsoft"))
      Warning ("Disambiguation of capitalized words vs. acronyms");


   Passed();
}

// test bookmark tag
void CALLBACK Test035 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   HRESULT hRes;
   DWORD dwUsed, dwUsed2;
   hRes = SpeakToWave ("Hello there. Th\\mrk=1\\is is a test.", NULL, &dwUsed);
   CHECKFAILED ("SpeakToWave");
   hRes = SpeakToWave ("Hello there. This is a test.", NULL, &dwUsed2);
   CHECKFAILED ("SpeakToWave");

   if (dwUsed != dwUsed2)
      Failed ("Bookmark tag changed the duration of text");
   else
      Passed();
}

// test a comment tag
void CALLBACK Test036 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   HRESULT hRes;
   DWORD dwUsed, dwUsed2;
   hRes = SpeakToWave ("Hello there.", NULL, &dwUsed);
   CHECKFAILED ("SpeakToWave");
   hRes = SpeakToWave ("Hello\\com=\"my comment\"\\ there.", NULL, &dwUsed2);
   CHECKFAILED ("SpeakToWave");

   if (dwUsed != dwUsed2)
      Failed ("Comment tag changed the duration of text");
   else
      Passed();
}

// test junk tag
void CALLBACK Test037 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   HRESULT hRes;
   DWORD dwUsed, dwUsed2;
   hRes = SpeakToWave ("Hello there. This is a test.", NULL, &dwUsed);
   CHECKFAILED ("SpeakToWave");
   hRes = SpeakToWave ("Hello there. This is a test.", NULL, &dwUsed2);
   CHECKFAILED ("SpeakToWave");
   if (dwUsed != dwUsed2)
      Warning ("Duration of audio changes even with the same text.");
   hRes = SpeakToWave ("Hello there. Th\\comjunke=my comment\\is is a test.", NULL, &dwUsed);
   CHECKFAILED ("SpeakToWave");

   if (dwUsed != dwUsed2)
      Failed ("Junk tag changed the duration of text");
   else
      Passed();
}

// invalid bookmark
void CALLBACK Test038 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   gTTSBufNS.m_fBookMark = FALSE;

   HRESULT hRes;
   hRes = SpeakToWave ("Hello there.\\mrk=junk\\ This is a test.");
   CHECKFAILED ("SpeakToWave");

   if (gTTSBufNS.m_fBookMark)
      Failed ("Junk bookmark got parsed as a real bookmark.");
   else
      Passed();
}

// invalid pause
void CALLBACK Test039 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   HRESULT hRes;
   DWORD dwUsed, dwUsed2;
   hRes = SpeakToWave ("Hello there. This is a test.", NULL, &dwUsed);
   CHECKFAILED ("SpeakToWave");
   hRes = SpeakToWave ("Hello there. \\pau=junk\\ This is a test.", NULL, &dwUsed2);
   CHECKFAILED ("SpeakToWave");

   if (dwUsed != dwUsed2)
      Failed ("Invalid pause tag changed the duration of text");
   else
      Passed();

   hRes = SpeakToWave ("Hello there. \\pau=0\\ This is a test.", NULL, &dwUsed2);
   CHECKFAILED ("SpeakToWave");

   if (dwUsed != dwUsed2)
      Failed ("Invalid pause tag changed the duration of text");
   else
      Passed();
}

// - Pitch tag actually changes pitch attribute
void CALLBACK Test040 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDTTSFEATURE (TTSFEATURE_PITCH);

   HRESULT hRes;

   // set the pitch
   hRes = gpTTSMode->PitchSet(160);
   CHECKFAILED ("PitchSet");

   // set flag so no if got attribute changed
   gTTSNS.m_fAttribChanged = FALSE;

   hRes = SpeakToWave ("Hello there. \\pit=150\\This is a test.");
   CHECKFAILED ("SpeakToWave");

   if (!gTTSNS.m_fAttribChanged || (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_PITCH))
      Failed ("Didn't get attrbute changed notification");

   WORD  wVal;
   hRes = gpTTSMode->PitchGet(&wVal);
   if (wVal != 150)
      Failed ("PitchSet didn't change attribute");

   Passed();
}

// - Pitch - invalid
void CALLBACK Test041 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDTTSFEATURE (TTSFEATURE_PITCH);

   HRESULT hRes;

   // set the pitch
   hRes = gpTTSMode->PitchSet(160);
   CHECKFAILED ("PitchSet");

   // set flag so no if got attribute changed
   gTTSNS.m_fAttribChanged = FALSE;

   hRes = SpeakToWave ("Hello there. \\pit=junk\\This is a test.");
   CHECKFAILED ("SpeakToWave");

   if (gTTSNS.m_fAttribChanged && (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_PITCH))
      Failed ("Didn't ignore bogus attrbute changed notification");

   // make sure pitch is within 10% of set value
   WORD  wVal;
   hRes = gpTTSMode->PitchGet(&wVal);
   if( LOWORD(wVal) != 160 )
      Failed ("PitchSet didn't ignore bogus tag");

   Passed();
}



// - Volume tag actually changes volume attribute
void CALLBACK Test042 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDTTSFEATURE (TTSFEATURE_VOLUME);

   HRESULT hRes;

   // set the volume
   hRes = gpTTSMode->VolumeSet(60000);
   CHECKFAILED ("VolumeSet");

   // set flag so no if got attribute changed
   gTTSNS.m_fAttribChanged = FALSE;

   hRes = SpeakToWave ("Hello there. \\vol=50000\\This is a test.");
   CHECKFAILED ("SpeakToWave");

   if (!gTTSNS.m_fAttribChanged || (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_VOLUME))
      Failed ("Didn't get attrbute changed notification");

	// check that within 5% of set value
   DWORD  wVal;
   hRes = gpTTSMode->VolumeGet(&wVal);

	// check within reasonable range for engines that do not use incrementes on 1
   if (LOWORD(wVal) < 45000 || LOWORD(wVal) > 55000)
      Failed ("VolumeSet didn't change attribute");

   Passed();
}

// - Volume - invalid
void CALLBACK Test043 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDTTSFEATURE (TTSFEATURE_VOLUME);

   HRESULT hRes;

   // set the volume
   hRes = gpTTSMode->VolumeSet(60000);
   CHECKFAILED ("VolumeSet");

   // set flag so no if got attribute changed
   gTTSNS.m_fAttribChanged = FALSE;

   hRes = SpeakToWave ("Hello there. \\vol=junk\\This is a test.");
   CHECKFAILED ("SpeakToWave");

   if (gTTSNS.m_fAttribChanged && (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_VOLUME))
      Failed ("Didn't ignore bogus attrbute changed notification");

   DWORD  wVal;
   hRes = gpTTSMode->VolumeGet(&wVal);
   if (LOWORD(wVal) < 55000 || LOWORD(wVal) > 65000)
      Failed ("VolumeSet didn't ignore bogus tag");

   Passed();
}


// - Speed tag actually changes speed attribute
void CALLBACK Test044 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDTTSFEATURE (TTSFEATURE_SPEED);

   HRESULT hRes;

   // set the speed
   hRes = gpTTSMode->SpeedSet(160);
   CHECKFAILED ("SpeedSet");

   // set flag so no if got attribute changed
   gTTSNS.m_fAttribChanged = FALSE;

   hRes = SpeakToWave ("Hello there. \\spd=120\\This is a test.");
   CHECKFAILED ("SpeakToWave");

   if (!gTTSNS.m_fAttribChanged || (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_SPEED))
      Failed ("Didn't get attrbute changed notification");

   DWORD  wVal;
   hRes = gpTTSMode->SpeedGet(&wVal);
   if (LOWORD(wVal) < 110 || LOWORD(wVal) > 130)
      Failed ("SpeedSet didn't change attribute");

   Passed();
}

// - Speed - invalid
void CALLBACK Test045 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDTTSFEATURE (TTSFEATURE_SPEED);

   HRESULT hRes;

   // set the speed
   hRes = gpTTSMode->SpeedSet(160);
   CHECKFAILED ("SpeedSet");

   // set flag so no if got attribute changed
   gTTSNS.m_fAttribChanged = FALSE;

   hRes = SpeakToWave ("Hello there. \\spd=junk\\This is a test.");
   CHECKFAILED ("SpeakToWave");

   if (gTTSNS.m_fAttribChanged && (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_SPEED))
      Failed ("Didn't ignore bogus attrbute changed notification");

   DWORD  wVal;
   hRes = gpTTSMode->SpeedGet(&wVal);
   if (wVal != 160)
      Failed ("SpeedSet didn't ignore bogus tag");

   Passed();
}


// makes sure that the speed change works within a sentence
void CALLBACK Test046 (void)
{
   NEEDAUDIO;
   NEEDENGLISH;

   HRESULT hRes;
   DWORD dwUsed, dwUsed2;
   hRes = SpeakToWave ("\\spd=120\\Hello there \\spd=120\\dude\\spd=120\\, this is a test.", NULL, &dwUsed);
   CHECKFAILED ("SpeakToWave");
   hRes = SpeakToWave ("\\spd=120\\Hello there \\spd=180\\dude\\spd=120\\, this is a test.", NULL, &dwUsed2);
   CHECKFAILED ("SpeakToWave");

   if (dwUsed == dwUsed2)
      Failed ("Speed tag ignored within a sentence");
   else
      Passed();

}

// - two backslashes in tagged text
void CALLBACK Test047 (void)
{
   // checks to make sure the engine claims to support this
   NEEDENGLISH;
   NEEDAUDIO;

   char  *psz;
   psz = SpeakAndListen ( "He said \\\\",
      "[<Start>]\n"
      "<Start>=he said [opt] back-slash\n"
      );
   if (!psz)
      return;

   // if didn't recognize hello world then worry
   if (stricmp(psz, "he said back-slash")) {
      Failed ("Didn't recognize back-slash");
   }
   else {
      Passed();
   }
}

// - PitchGet/Set - Make sure that set value & that can get same
void CALLBACK Test048 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE (TTSFEATURE_PITCH);

   HRESULT hRes;
   WORD  wPitch, wPitch2;
   hRes = gpTTSMode->PitchGet(&wPitch);
   CHECKFAILED("PitchGet");

   // increase by 10
   gTTSNS.m_fAttribChanged = 0;
   hRes = gpTTSMode->PitchSet (wPitch+10);
   CHECKFAILED("PitchSet");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (!gTTSNS.m_fAttribChanged ||
      (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_PITCH))
      Failed ("Didn't get pitch change notification");

   // check
   hRes = gpTTSMode->PitchGet(&wPitch2);
   CHECKFAILED("PitchGet");
   if ((wPitch+10) != wPitch2)
      Failed ("Didn't remember the pitch changes.");

   // restore
   hRes = gpTTSMode->PitchSet (wPitch);
   CHECKFAILED("PitchSet");

   Passed();
}


// - PitchGet/Set - Test PITCH_MAX and PITCH_MIN
void CALLBACK Test049 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE (TTSFEATURE_PITCH);

   HRESULT hRes;
   WORD  wPitch, wPitch2;
   hRes = gpTTSMode->PitchGet(&wPitch);
   CHECKFAILED("PitchGet");

   // minimum
   gTTSNS.m_fAttribChanged = 0;
   hRes = gpTTSMode->PitchSet (TTSATTR_MINPITCH);
   CHECKFAILED("PitchSet");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (!gTTSNS.m_fAttribChanged ||
      (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_PITCH))
      Failed ("Didn't get pitch change notification");

   // check
   hRes = gpTTSMode->PitchGet(&wPitch2);
   CHECKFAILED("PitchGet");
   if (wPitch2 == TTSATTR_MINPITCH)
      Failed ("Doesn't handle minimum/maximum pitch");

   // maximum
   hRes = gpTTSMode->PitchSet (TTSATTR_MAXPITCH);
   CHECKFAILED("PitchSet");
   hRes = gpTTSMode->PitchGet(&wPitch2);
   CHECKFAILED("PitchGet");
   if (wPitch2 == TTSATTR_MAXPITCH)
      Failed ("Doesn't handle minimum/maximum pitch");


   // restore
   hRes = gpTTSMode->PitchSet (wPitch);
   CHECKFAILED("PitchSet");

   Passed();
}

// - PitchGet/Set - Test invalid values
void CALLBACK Test050 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE (TTSFEATURE_PITCH);

   HRESULT hRes;
   WORD  wPitch, wPitch2;
   hRes = gpTTSMode->PitchGet(&wPitch);
   CHECKFAILED("PitchGet");

   // way too low
   gTTSNS.m_fAttribChanged = 0;
   hRes = gpTTSMode->PitchSet (2);
   if (!hRes)
      Failed ("PitchSet didn't error out when value too low.");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (gTTSNS.m_fAttribChanged &&
      (gTTSNS.m_dwAttribChangedAttrib == TTSNSAC_PITCH))
      Failed ("Got pitch change notification when invalid param.");

   // check
   hRes = gpTTSMode->PitchGet(&wPitch2);
   CHECKFAILED("PitchGet");
   if (wPitch != wPitch2)
      Failed ("Pitch changed.");

   // too high
   hRes = gpTTSMode->PitchSet (20000);
   if (!hRes)
      Failed ("PitchSet didn't error out when value too low.");
   hRes = gpTTSMode->PitchGet(&wPitch2);
   CHECKFAILED("PitchGet");
   if (wPitch != wPitch2)
      Failed ("Pitch changed.");

   // restore
   hRes = gpTTSMode->PitchSet (wPitch);
   CHECKFAILED("PitchSet");

   // null parameters
   hRes = gpTTSMode->PitchGet (NULL);
   if (!hRes)
      Failed ("Didn't error out when passed a NULL parameter");

   Passed();
}

// - SpeedGet/Set - Make sure that set value & that can get same
void CALLBACK Test051 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE (TTSFEATURE_SPEED);

   HRESULT hRes;
   DWORD  wSpeed, wSpeed2;
   hRes = gpTTSMode->SpeedGet(&wSpeed);
   CHECKFAILED("SpeedGet");

   // increase by 10
   gTTSNS.m_fAttribChanged = 0;
   hRes = gpTTSMode->SpeedSet (wSpeed+10);
   CHECKFAILED("SpeedSet");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (!gTTSNS.m_fAttribChanged ||
      (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_SPEED))
      Failed ("Didn't get Speed change notification");

   // check
   hRes = gpTTSMode->SpeedGet(&wSpeed2);
   CHECKFAILED("SpeedGet");
   if ((wSpeed+10) != wSpeed2)
      Failed ("Didn't remember the Speed changes.");

   // restore
   hRes = gpTTSMode->SpeedSet (wSpeed);
   CHECKFAILED("SpeedSet");

   Passed();
}


// - SpeedGet/Set - Test SPEED_MAX and SPEED_MIN
void CALLBACK Test052 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE (TTSFEATURE_SPEED);

   HRESULT hRes;
   DWORD  wSpeed, wSpeed2;
   hRes = gpTTSMode->SpeedGet(&wSpeed);
   CHECKFAILED("SpeedGet");

   // minimum
   gTTSNS.m_fAttribChanged = 0;
   hRes = gpTTSMode->SpeedSet (TTSATTR_MINSPEED);
   CHECKFAILED("SpeedSet");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (!gTTSNS.m_fAttribChanged ||
      (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_SPEED))
      Failed ("Didn't get Speed change notification");

   // check
   hRes = gpTTSMode->SpeedGet(&wSpeed2);
   CHECKFAILED("SpeedGet");
   if (wSpeed2 == TTSATTR_MINSPEED)
      Failed ("Doesn't handle minimum/maximum Speed");

   // maximum
   hRes = gpTTSMode->SpeedSet (TTSATTR_MAXSPEED);
   CHECKFAILED("SpeedSet");
   hRes = gpTTSMode->SpeedGet(&wSpeed2);
   CHECKFAILED("SpeedGet");
   if (wSpeed2 == TTSATTR_MAXSPEED)
      Failed ("Doesn't handle minimum/maximum Speed");


   // restore
   hRes = gpTTSMode->SpeedSet (wSpeed);
   CHECKFAILED("SpeedSet");

   Passed();
}

// - SpeedGet/Set - Test invalid values
void CALLBACK Test053 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE (TTSFEATURE_SPEED);

   HRESULT hRes;
   DWORD  wSpeed, wSpeed2;
   hRes = gpTTSMode->SpeedGet(&wSpeed);
   CHECKFAILED("SpeedGet");

   // way too low
   gTTSNS.m_fAttribChanged = 0;
   hRes = gpTTSMode->SpeedSet (2);
   if (!hRes)
      Failed ("SpeedSet didn't error out when value too low.");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (gTTSNS.m_fAttribChanged &&
      (gTTSNS.m_dwAttribChangedAttrib == TTSNSAC_SPEED))
      Failed ("Got Speed change notification when invalid param.");

   // check
   hRes = gpTTSMode->SpeedGet(&wSpeed2);
   CHECKFAILED("SpeedGet");
   if (wSpeed != wSpeed2)
      Failed ("Speed changed.");

   // too high
   hRes = gpTTSMode->SpeedSet (20000);
   if (!hRes)
      Failed ("SpeedSet didn't error out when value too low.");
   hRes = gpTTSMode->SpeedGet(&wSpeed2);
   CHECKFAILED("SpeedGet");
   if (wSpeed != wSpeed2)
      Failed ("Speed changed.");

   // restore
   hRes = gpTTSMode->SpeedSet (wSpeed);
   CHECKFAILED("SpeedSet");

   // null parameters
   hRes = gpTTSMode->SpeedGet (NULL);
   if (!hRes)
      Failed ("Didn't error out when passed a NULL parameter");

   Passed();
}

// - VolumeGet/Set - Make sure that set value & that can get same
void CALLBACK Test054 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE (TTSFEATURE_VOLUME);

   HRESULT hRes;
   DWORD  wVolume, wVolume2;
   hRes = gpTTSMode->VolumeGet(&wVolume);
   CHECKFAILED("VolumeGet");

   // set value
   gTTSNS.m_fAttribChanged = 0;
   hRes = gpTTSMode->VolumeSet (55000);
   CHECKFAILED("VolumeSet");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (!gTTSNS.m_fAttribChanged ||
      (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_VOLUME))
      Failed ("Didn't get Volume change notification");

   // check
   hRes = gpTTSMode->VolumeGet(&wVolume2);
   CHECKFAILED("VolumeGet");
	if (LOWORD(wVolume2) > 60000 || LOWORD(wVolume2) < 55000)
		Failed ("Didn't remember the Volume changes for left channel.");
	
	if( HIWORD(wVolume2) > 60000  || HIWORD(wVolume2) < 55000 )
      Failed ("Didn't remember the Volume changes for right channel.");

   // restore
   hRes = gpTTSMode->VolumeSet (wVolume);
   CHECKFAILED("VolumeSet");

   Passed();
}


// - VolumeGet/Set - Test Volume_MAX and Volume_MIN
void CALLBACK Test055 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE (TTSFEATURE_VOLUME);

   HRESULT hRes;
   DWORD  wVolume, wVolume2;
   hRes = gpTTSMode->VolumeGet(&wVolume);
   CHECKFAILED("VolumeGet");

   // minimum
   gTTSNS.m_fAttribChanged = 0;
   hRes = gpTTSMode->VolumeSet (TTSATTR_MINVOLUME);
   CHECKFAILED("VolumeSet");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (!gTTSNS.m_fAttribChanged ||
      (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_VOLUME))
      Failed ("Didn't get Volume change notification");

   // check
   hRes = gpTTSMode->VolumeGet(&wVolume2);
   CHECKFAILED("VolumeGet");
   if (wVolume2 != 0)
      Failed ("Doesn't handle minimum/maximum Volume");

   // maximum
   hRes = gpTTSMode->VolumeSet (TTSATTR_MAXVOLUME);
   CHECKFAILED("VolumeSet");
   hRes = gpTTSMode->VolumeGet(&wVolume2);
   CHECKFAILED("VolumeGet");
   if (wVolume2 != 0xffffffff)
      Failed ("Doesn't handle minimum/maximum Volume");


   // restore
   hRes = gpTTSMode->VolumeSet (wVolume);
   CHECKFAILED("VolumeSet");

   Passed();
}

// - VolumeGet/Set - Test invalid values
void CALLBACK Test056 (void)
{
   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE (TTSFEATURE_VOLUME);

   HRESULT hRes;

   // null parameters
   hRes = gpTTSMode->VolumeGet (NULL);
   if (!hRes)
      Failed ("Didn't error out when passed a NULL parameter");

   Passed();
}

// - RealTimeGet/Set - Make sure that set value & that can get same
void CALLBACK Test057 (void)
{
   HRESULT hRes;
   DWORD  wRealTime, wRealTime2;
   hRes = gpTTSMode->RealTimeGet(&wRealTime);
   if (hRes == TTSERR_NOTSUPPORTED) {
      NotApplicable();
      return;
   }
   CHECKFAILED("RealTimeGet");

   // set value
   gTTSNS.m_fAttribChanged = 0;
   hRes = gpTTSMode->RealTimeSet (50);
   CHECKFAILED("RealTimeSet");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (!gTTSNS.m_fAttribChanged ||
      (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_REALTIME))
      Failed ("Didn't get RealTime change notification");

   // check
   hRes = gpTTSMode->RealTimeGet(&wRealTime2);
   CHECKFAILED("RealTimeGet");
   if (50 != wRealTime2)
      Failed ("Didn't remember the RealTime changes.");

   // restore
   hRes = gpTTSMode->RealTimeSet (wRealTime);
   CHECKFAILED("RealTimeSet");

   Passed();
}

// - RealTimeGet/Set - Test invalid values
void CALLBACK Test058 (void)
{
   HRESULT hRes;

   // null parameters
   hRes = gpTTSMode->RealTimeGet (NULL);
   if (!hRes)
      Failed ("Didn't error out when passed a NULL parameter");

   Passed();
}


// - Check that words/minute is valied
void CALLBACK Test059 (void)
{
   HRESULT hRes;

   NEEDENGLISH;
   NEEDAUDIO;
   NEEDTTSFEATURE (TTSFEATURE_SPEED);

   char  sz400Words[] =
		"A glottal stop in English has been regarded as a signal which the speaker"
		" produces to indicate a boundary between two words when the second word starts"
		" with a stressed vowel. Studies have not been done extensively, because its"
		" use seems to offer considerable speaker and situation dependent options."
		" Certain researchers have suggested that phonological sequences across lexical"
		" boundaries is the principal factor which determines whether the presence of a"
		" glottal stop is obligatory. The Wessex people must have appeared on Salisbury"
		" plain about 1700 B.C.. They were a highly organized and industrious people, but"
		" perhaps not belligerent. In their graves are daggers and bows as well as"
		" ornaments, but some of their ostensible weapons at closer inspection seem to"
		" have been more probably only ceremonial symbols like our West Point and"
		" Annapolis parade swords. There is evidence that the Wessex folk were concerned"
		" less with war than with arts and enjoyments of peace. Their chiefs that is,"
		" were so concerned. The Wessex people themselves, along with other possibly"
		" subject peoples, may have made the profits which the rulers put to good use in"
		" their trading. Only the chieftains were preserved for afterlife. The ordinary"
		" folk left no trace. Those rulers were great lords and international financiers."
		" Using the surplus wealth accumulated by the toilers, they bartered old"
		" necessities and new luxuries all the way from the Baltic to the Mediterranean."
		" Among their momentoes are blue faience of Egypt, axes from Ireland, a Baltic"
		" amber disc bound with gold in the Cretan fashion, Scottish jet necklaces and"
		" ingenious arrowshaft straighteners, delicate incense cups and tiny bowls"
		" decorated in the style of Normandy. Those Wessex lords lived in busy splendor,"
		" and went into their resting places with their martial and civil pomp around"
		" them. Our result demonstrates that glottal stops seem to be almost entirely"
		" optional for speakers. However, an informal inquiry has revealed that MB's"
		" reading is the easiest and most pleasant to listen to among five speakers. And"
		" we can see the reason. This speaker uses glottal stops rather frequently on"
		" content words, uses pauses instead of glottal stops where grammatical breaks"
		" occur, and above all reads in an articulated manner. This total passage has"
		" an overall average word length of just over five letters, and a period or"
		" comma approximately every tenth word, and contains exactly 400 words counting"
		" the numbers in their full word forms.";

   // get the minimum and maximum speed
   DWORD dwMin, dwMax,dwStart;
   hRes = gpTTSMode->SpeedGet (&dwStart);
   CHECKFAILED ("SpeedGet");
   hRes = gpTTSMode->SpeedSet (TTSATTR_MINSPEED);
   CHECKFAILED ("SpeedSet");
   hRes = gpTTSMode->SpeedGet (&dwMin);
   CHECKFAILED ("SpeedGet");
   hRes = gpTTSMode->SpeedSet (TTSATTR_MAXSPEED);
   CHECKFAILED ("SpeedSet");
   hRes = gpTTSMode->SpeedGet (&dwMax);
   CHECKFAILED ("SpeedGet");

   // divide this into 10 parts & try
   DWORD dwDecr = (dwMax - dwMin) / 10;
   DWORD dwSpeed, dwUsed;
   DWORD dwCalcWPM;
  
	// decrement down to minimum
   for (dwSpeed = dwMax; dwSpeed >= dwMin; dwSpeed -= dwDecr) {

	   hRes = gpTTSMode->SpeedSet(dwSpeed);
      CHECKFAILED ("SpeedSet");

      hRes = SpeakToWave (sz400Words, NULL, &dwUsed);
      CHECKFAILED ("SpeakToWave");

		if( dwUsed == 0 )
			Failed ("no audio spoken");
		else {
			// calculate the WPM
			dwCalcWPM = 400 * 60 / (dwUsed / gpWFEXTTS->nAvgBytesPerSec);

			// print this out
			char  szTemp[128];
			wsprintf (szTemp, "SpeedSet=%d WPM. Real rate = %d WPM",
				(int) dwSpeed, (int) dwCalcWPM);
			Information (szTemp);

			// if this is off by more than 30% then error
			if ( (dwCalcWPM < (dwSpeed - dwSpeed/3.33333)) ||
				(dwCalcWPM > (dwSpeed + dwSpeed/3.33333)) )
				Failed ("Speaking rate was more than 30% different from expected.");
		}

		// to prevent possible rollover
		if( dwSpeed < dwDecr )
		break;
   } 

   // restore the original speed
   hRes = gpTTSMode->SpeedSet (dwStart);
   CHECKFAILED ("SpeedSet");
   Passed();
}


// - AudioReset - Make sure stops playing and clears all buffers.
void CALLBACK Test060 (void)
{
   HRESULT hRes;

   NEEDENGLISH;
   NEEDAUDIO;

   // generate the wave to send to
   hRes = GenerateWaveName ();
   CHECKFAILED ("GenerateWaveName");

   // create a second buffer notification sync
   // create a new notification sink
   CFuncTTSBufNotifySink   BNS;

   // set some flags to 0
   BNS.m_fTextDataStarted = BNS.m_fTextDataDone = BNS.m_fBookMark = 0;
   BNS.m_dwRefCount = 0;
   gTTSBufNS.m_fTextDataStarted = gTTSBufNS.m_fTextDataDone = gTTSBufNS.m_fBookMark = 0;


   // speak to both buffers
   hRes = gpTTSMode->Speak (L"First \\mrk=1\\ buffer to speak.", TRUE, &gTTSBufNS);
   CHECKFAILED ("Speak");
   hRes = gpTTSMode->Speak (L"Second buffer.", TRUE, &BNS);
   CHECKFAILED ("Speak");

   // wait till there's a bookmark
   BOOL  *pf;
   pf = WaitForEvent (60, &gTTSBufNS.m_fBookMark);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("Didn't get expected bookmark");
      return;
   }


   // audio reset
   hRes = gpTTSMode->AudioReset();
   CHECKFAILED ("AudioReset");

   // make sure the second buffer is released, and that we don't
   // get any text started/stopped
   gTTSWANS.m_fPosn = FALSE;
   WaitForEvent (1.0);
   if (gTTSWANS.m_fPosn)
      Failed ("Audio kept on playing after an audio reset");
   if (BNS.m_dwRefCount)
      Failed ("Didn't release second buffer notification sync");
   if (BNS.m_fTextDataStarted)
     Failed ("Engine claims it started second buffer even though aborted");
   if (!BNS.m_fTextDataDone)
      Failed ("Didn't send TextDataDone for second buffer on AudioReset");


   Passed();
}

// - Inject - Try all reasonable tags and make sure they actually work. The easitst
//	ones are pitch, volume, speed, and make sure the pitchget, volumeget, and
//	speechget attributes reflect this.
void CALLBACK Test061 (void)
{
   HRESULT hRes;

   NEEDENGLISH;
   NEEDAUDIO;

   // generate the wave to send to
   hRes = GenerateWaveName ();
   CHECKFAILED ("GenerateWaveName");

   // send in a lot of text to speak
   DWORD i;
   for (i = 0; i < 50; i++) {
      hRes = gpTTSMode->Speak (L"All work and no play makes jack a dull boy.");
      CHECKFAILED ("Speak");
   }

   for (i = 0; i < 8; i++) {
      // wait a second and then do a test
      if (&gfAbortTest == WaitForEvent(1.0))
         return;

      DWORD dwAttrib;
      WCHAR *psz;

      // which test
      switch (i) {
      case 0:  // volume
         psz = L"\\vol=34000\\";
         dwAttrib = TTSNSAC_VOLUME;
         break;
      case 1:  // speed
         psz = L"\\spd=140\\";
         dwAttrib = TTSNSAC_SPEED;
         break;
      case 2:  // pitch
         psz = L"\\pit=140\\";
         dwAttrib = TTSNSAC_PITCH;
         break;
      case 3:  // two tags
         psz = L"\\spd=140\\\\vol=34000\\";
         dwAttrib = 0;
         break;
      case 4:  // random text - expect ignroed
         psz = L"hello there";
         break;
      case 5:  // arbitrary tag
         psz = L"\\com=\"test\"\\hello there";
         break;
      case 6:  // two tags
         psz = L"\\rst\\";
         break;
      case 7:  // two tags
         psz = L"\\hello";
         break;
      }

      gTTSNS.m_fAttribChanged = FALSE;

      hRes = gpTTSMode->Inject(psz);
      CHECKFAILED ("Inject");

      // wait half a second
      WaitForEvent(.5);

      // if we're supposed to get an attribute changed notification
      // then make sire
      if (dwAttrib) {
         if (!gTTSNS.m_fAttribChanged ||
            (gTTSNS.m_dwAttribChangedAttrib != dwAttrib))
            Failed ("Didn't get expected attribute change notification.");
      }

      // make sure the value is set right
      switch (i) {
      case 0:  // volume
         if (LOWORD(gpTTSMode->VolumeGet()) < 32000 || LOWORD(gpTTSMode->VolumeGet()) > 36000)
            Failed ("Tag didn't properly change setting");
         break;
      case 1:  // speed
         if (gpTTSMode->SpeedGet() < 120 || gpTTSMode->SpeedGet() > 160)
            Failed ("Tag didn't properly change setting");
         break;
      case 2:  // pitch
         if (gpTTSMode->PitchGet() < 120 || gpTTSMode->PitchGet() > 160)
            Failed ("Tag didn't properly change setting");
         break;
      case 3:  // two tags
         if (gpTTSMode->SpeedGet() < 120 || gpTTSMode->SpeedGet() > 160)
            Failed ("Tag didn't properly change setting");
         if (gpTTSMode->PitchGet() < 120 || gpTTSMode->SpeedGet() > 160)
            Failed ("Tag didn't properly change setting");
         break;
      }
   }

   hRes = gpTTSMode->AudioReset();
   CHECKFAILED("AudioReset");

   Passed();
}

// - Inject - Try all reasonable tags and make sure they actually work. The easitst
//	ones are pitch, volume, speed, and make sure the pitchget, volumeget, and
//	speechget attributes reflect this.
void CALLBACK Test062 (void)
{
   HRESULT hRes;

   // null parameters
   hRes = gpTTSMode->Inject(NULL);
   if (!hRes)
      Failed ("Inject didn't detect NULL parameter");

   Passed();
}

// - PosnGet - Make sure return approximately same time as audio dest.
// - ToFileTime - Make sure returns approximately same value as audio source
void CALLBACK Test063 (void)
{
   HRESULT hRes;

   NEEDAUDIO;

   // get the interface for the audio
   PIAUDIO  pIAudio;
   hRes = gpIAudioFileTTS->QueryInterface (IID_IAudio, (LPVOID*) &pIAudio);
   CHECKFAILED ("IID_IAudio");

   // posnget
   QWORD qwTime, qwTime2;
   hRes = gpTTSMode->PosnGet(&qwTime);
   if (hRes)
      Failed ("PosnGet", hRes);
   hRes = pIAudio->PosnGet(&qwTime2);
   if (hRes)
      Failed ("PosnGet", hRes);
   if (qwTime != qwTime2)
      Failed ("Engine not returning the same time as the audio object");

   // to file time
   FILETIME ft, ft2;
   HRESULT  hRes2;
   hRes = gpTTSMode->ToFileTime (&qwTime, &ft);
   hRes2 = pIAudio->ToFileTime (&qwTime, &ft2);
   if (hRes != hRes2)
      Failed ("ToFileTime", hRes);
   if (!hRes && memcmp(&ft, &ft2, sizeof(ft)))
      Failed ("Engine not returning the same file time as audio object");

   // to file time, #2
   qwTime -= 5000;
   hRes = gpTTSMode->ToFileTime (&qwTime, &ft);
   hRes2 = pIAudio->ToFileTime (&qwTime, &ft2);
   if (hRes != hRes2)
      Failed ("ToFileTime", hRes);
   if (!hRes && memcmp(&ft, &ft2, sizeof(ft)))
      Failed ("Engine not returning the same file time as audio object");

   // invalid params
   hRes = gpTTSMode->PosnGet(NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   hRes = gpTTSMode->ToFileTime (0, NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   
   pIAudio->Release();
   Passed();
}


// - Register - Invalid params
void CALLBACK Test064 (void)
{
   HRESULT hRes;
   DWORD dwVal;

   // bad IID
   hRes = gpTTSMode->Register (&gTTSBufNS, IID_IAudio, &dwVal);
   if (!hRes)
      Failed ("Didn't detect bad interface");

   // no key
   hRes = gpTTSMode->Register (&gTTSNS, IID_ITTSNotifySink, NULL);
   if (!hRes)
      Failed ("Didn't detect NULL parameter.");

   // null interface
   hRes = gpTTSMode->Register (NULL, IID_ITTSNotifySink, &dwVal);
   if (!hRes)
      Failed ("Didn't detect NULL parameter.");


   Passed();
}

// - TextData - Invalid params
void CALLBACK Test065 (void)
{
   HRESULT hRes;
   SDATA sd;
   WCHAR sz[] = L"Testing.";

   sd.pData = (PVOID) sz;
   sd.dwSize = sizeof(sz);

	// set some flags to 0
   gTTSBufNS.m_fTextDataStarted = gTTSBufNS.m_fTextDataDone = 0;

   // bad character set
   hRes = gpTTSMode->TextData ((VOICECHARSET)52,
      0, sd, &gTTSBufNS, IID_ITTSBufNotifySink);
   if (!hRes)
      Failed ("Didn't detect invalid character set");

   // bad IID
   hRes = gpTTSMode->TextData (CHARSET_TEXT,
      0, sd, &gTTSNS, IID_ITTSNotifySink);
   if (!hRes)
      Failed ("Didn't detect invalid notification sink");

   // null text data
   sd.pData = NULL;
   hRes = gpTTSMode->TextData (CHARSET_TEXT,
      0, sd, &gTTSBufNS, IID_ITTSBufNotifySink);
   if (hRes != NOERROR && gTTSBufNS.m_fTextDataStarted && gTTSBufNS.m_fTextDataDone )
      Failed ("NOERROR and Start and Done notification sinks expected on NULL data");

   Passed();
}



// - Phoneme - Invalid param.
void CALLBACK Test066 (void)
{
   HRESULT hRes;
   SDATA sd;
   SDATA sdp;
   WCHAR sz[] = L"Testing.";

   sd.pData = (PVOID) sz;
   sd.dwSize = sizeof(sz);

   // bad character set
   hRes = gpTTSMode->Phoneme ((VOICECHARSET)52,
      0, sd, &sdp);
   if (!hRes)
      Failed ("Didn't detect invalid character set");

   // null phoneme return
   hRes = gpTTSMode->Phoneme (CHARSET_TEXT,
      0, sd, NULL);
   if (!hRes)
      Failed ("Didn't detect null data");

   // null text data
   sd.pData = NULL;
   hRes = gpTTSMode->Phoneme (CHARSET_TEXT,
      0, sd, &sdp);
   if (!hRes)
      Failed ("Didn't detect null data");

   Passed();
}

// - AboutDlg - Check to see if it exists
// - GeneralDlg- Check to see if it exists
// - LexiconDlg - Check to see if it exists
// - TrnslateDlg - Check to see if it exsists
void CALLBACK Test067 (void)
{
   HRESULT hRes;

   hRes = gpTTSMode->AboutDlg(NULL);
   if (hRes)
      Failed ("AboutDlg missing", hRes);

   hRes = gpTTSMode->GeneralDlg(NULL);
   if (hRes)
      Failed ("GeneralDlg missing", hRes);

   hRes = gpTTSMode->LexiconDlg(NULL);
   if (hRes)
      Warning ("LexiconDlg missing. Lexicon dialog is recommended.", hRes);

   hRes = gpTTSMode->TranslateDlg(NULL);
	// information because TranslateDlg is optional
   if (hRes)
      Information ("TranslateDlg missing (optional)", hRes);

   Passed();
}



// - Rst - make sure it resets parameters
void CALLBACK Test068 (void)
{
   HRESULT hRes;

   // destroy the TTS engine first
   hRes =DestroyTTSEngine();
   CHECKFAILED ("DestroyTTSEngine");

   // create it again
   hRes = CreateTTSEngine();
   CHECKFAILED ("CreateTTSEngine");

   // get the pitch, volume, and speed parameters
   WORD  wPitch;
   DWORD dwVolume, dwSpeed;
   hRes = gpTTSMode->PitchGet(&wPitch);
   CHECKFAILED ("PitchGet");
   hRes = gpTTSMode->SpeedGet (&dwSpeed);
   CHECKFAILED ("SpeedGet");
   hRes = gpTTSMode->VolumeGet (&dwVolume);
   CHECKFAILED ("VolumeGet");

   // change the values
   hRes = gpTTSMode->PitchSet(wPitch + 10);
   CHECKFAILED ("PitchSet");
   hRes = gpTTSMode->SpeedSet (dwSpeed + 10);
   CHECKFAILED ("SpeedSet");
   hRes = gpTTSMode->VolumeSet (34000);
   CHECKFAILED ("VolumeSet");

   // reset
   hRes = gpTTSMode->Inject (L"\\rst\\");
   CHECKFAILED ("Inject");

   // make sure the values are the same
   if (wPitch != gpTTSMode->PitchGet())
      Failed ("Pitch not restored");
   if (dwSpeed != gpTTSMode->SpeedGet())
      Failed ("Speed not restored");
   if (dwVolume != gpTTSMode->VolumeGet())
      Failed ("Volume not restored");

   Passed();
}


//- Make sure no arbitrary silence at beginning/end of audio.
void CALLBACK Test069 (void)
{
   HRESULT hRes;

   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   // make sure support PCM
   if (
      (gpWFEXTTS->wFormatTag != WAVE_FORMAT_PCM) ||
      (gpWFEXTTS->nChannels != 1) ||
      (gpWFEXTTS->wBitsPerSample != 16) )
   {
      NotApplicable ("Not PCM data");
      return;
   }

   // speak to SR
   char  szFile[128];
   hRes = SpeakToWave ("Hello world.", szFile);
   CHECKFAILED ("SpeakToSR");


   // open the file and find the data chunk
   HMMIO hFileRead;

	// open and make sure can support wave format
	hFileRead = mmioOpen( szFile, NULL, MMIO_READ | MMIO_ALLOCBUF );
	if(!hFileRead) return;
	MMCKINFO mminfopar, mminfosub;
	// search for wave type...
	mminfopar.fccType = mmioFOURCC('W','A','V','E');
	if( mmioDescend( hFileRead, (LPMMCKINFO)&mminfopar, NULL, MMIO_FINDRIFF ) ) {
		mmioClose (hFileRead, 0);
		return;
	}
	
	// get the total wave data size (mminfo.cksize)...
	mminfosub.ckid = mmioFOURCC('d','a','t','a');
	if( mmioDescend( hFileRead, &mminfosub, &mminfopar, MMIO_FINDCHUNK ) ) {
		mmioClose (hFileRead, 0);
		return;
	}

   // read in the data
   short *pMem;
   pMem = (short*) malloc (mminfosub.cksize);
   if (!pMem)
      return;

   mmioRead (hFileRead, (HPSTR) pMem, mminfosub.cksize);
   
   mmioClose (hFileRead, 0);

   // check the silence at the beginning
   DWORD i;
   for (i = 0; i < mminfosub.cksize / 2; i++)
      if (pMem[i])
         break;
   if ((i*2) > (gpWFEXTTS->nAvgBytesPerSec / 4))
      Warning ("More than 1/4 second of silence at the beginning of the utterance.");

   // check the silence at the end
   for (i = mminfosub.cksize / 2 - 1; i < mminfosub.cksize / 2; i--)
      if (pMem[i])
         break;
   if ((mminfosub.cksize - i*2) > (gpWFEXTTS->nAvgBytesPerSec/2))
      Warning ("More than 1/2 second of silence at the end of the utterance.");


   free (pMem);
   Passed();
}


// - Cause TTS not to like wave format & fail on create.  Also test ref count on audio obect.
void CALLBACK Test070 (void)
{
   HRESULT hRes;

   NEEDAUDIO;

   // creaate
   CTTSMode TTSMode;
   PIAUDIOFILE pIAudioFileTTS;
   
   // create the audio source from wave
	hRes = CoCreateInstance(CLSID_AudioDestFile, 
		NULL, CLSCTX_INPROC_SERVER, 
		IID_IAudioFile, 
		(PVOID *) &pIAudioFileTTS);
	if (hRes) return;
   gTTSWANS.m_qwPosn = 0;
	pIAudioFileTTS -> Register(&gTTSWANS);
   pIAudioFileTTS->RealTimeSet (0x100);

   // Wave format that won't work
   WAVEFORMATEX   wfex;
   wfex.wFormatTag = 638;

   PIAUDIO  pAudio;
   pIAudioFileTTS->QueryInterface (IID_IAudio, (PVOID*) &pAudio);
   if (pAudio) {
      SDATA    s;
      s.pData = &wfex;
      s.dwSize = sizeof(wfex);
      pAudio->WaveFormatSet (s);
      pAudio->Release();
   }

   // init the engine
   hRes = TTSMode.InitAudioDestObject(pIAudioFileTTS);
   if (hRes)
   {
       if (pIAudioFileTTS->Release() != 0)
	       Failed ("TTS engine didn't release the audio object on an error");
      return;
   }
   // get the ref count
   DWORD dwCount;
   dwCount= pIAudioFileTTS->AddRef();

   hRes = TTSMode.Init (gTTSModeInfo.gModeID);
   if (!hRes)
      Failed ("TTS Engine didn't test the wave device");

   // make sure the count's right
   if (pIAudioFileTTS->Release() != (dwCount-1))
      Failed ("TTS engine didn't release the audio object");

   if (pIAudioFileTTS->Release() != 1)
	       Failed ("Audio object leak");
   Passed();

}



// Make sure that bookmarks are not sent after an audio reset
void CALLBACK Test071 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;

   BOOL  *pf;

   // create a wave
   hRes = GenerateWaveName();
   CHECKFAILED ("GenerateWaveName");

   // speak
   hRes = gpTTSMode->Speak (L"Hello world.\\mrk=2\\ This is a test to see if the \\mrk=3\\ is sent.",
      TRUE, &gTTSBufNS);
   CHECKFAILED ("Speak");

   // wait for the bookmark
   pf = WaitForEvent (100.0, &gTTSBufNS.m_fBookMark);
   if (pf == &gfAbortTest)
      return;
   else if (pf != &gTTSBufNS.m_fBookMark)
      Failed ("Didn't get bookmark");

   gTTSBufNS.m_fBookMark = FALSE;

   // do a reset
   hRes = gpTTSMode->AudioReset();
   CHECKFAILED ("AudioReset");

   // make sure there's no bookmark
   pf = WaitForEvent (1.0);
   if (gTTSBufNS.m_fBookMark)
      Failed ("Engine sent a bookmark after an AudioReset");

   Passed ();
}



// - WordPosition - Make sure get right values (approx)
void CALLBACK Test072 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE (TTSFEATURE_WORDPOSITION);
   NEEDAUDIO;
   NEEDENGLISH;

   BOOL  *pf;

   // create a wave
   hRes = GenerateWaveName();
   CHECKFAILED ("GenerateWaveName");

   WCHAR psz[] = L"All work and no play makes jack a dull boy.";

   // speak
   hRes = gpTTSMode->Speak (psz, TRUE, &gTTSBufNS);
   CHECKFAILED ("Speak");

   // set flag for word position
   gTTSBufNS.m_fWordPosition = FALSE;

   // wait for the audio done
   pf = WaitForEvent (100.0, &gTTSNS.m_fAudioStop);
   if (pf == &gfAbortTest)
      return;
   else if (pf != &gTTSNS.m_fAudioStop)
      Failed ("Didn't get audio stopped");

   if (!gTTSBufNS.m_fWordPosition)
      Failed ("Didn't get a word position notification");
   else if (gTTSBufNS.m_dwWordPositionVal < (sizeof(psz) - 12))
      Failed ("Didn't get a word position notification for the last word. "
         "It may be notifying in characters instead of bytes.");
      
   Passed ();
}


// - Visual - make sure it's called, and make sure the
// last shape is a mouth closed
void CALLBACK Test073 (void)
{
   HRESULT hRes;
   CSTPhoneConv   pconv;

   // checks to make sure the engine claims to support this
   NEEDTTSFEATURE (TTSFEATURE_VISUAL);
   NEEDAUDIO;
   NEEDENGLISH;

   // initialize the phoneme converter
   pconv.Init();
   pconv.SetByLang(gTTSModeInfo.language.LanguageID);

   BOOL  *pf;

   // create a wave
   hRes = GenerateWaveName();
   CHECKFAILED ("GenerateWaveName");

   WCHAR psz[] = L"All work and no play makes jack a dull boy.";

   // speak
   hRes = gpTTSMode->Speak (psz, TRUE, &gTTSBufNS);
   CHECKFAILED ("Speak");

   BOOL  fGotVisual = FALSE;
   BOOL  fGotNonZeroIPA = FALSE;
   BOOL  fMouthEverOpen = FALSE;

   while (TRUE) {
      // wait for the audio done
      pf = WaitForEvent (10.0, &gTTSNS.m_fVisual);
      if (pf == &gfAbortTest)
         return;
      else if (!pf)
         break;

      // got a visual
      fGotVisual = TRUE;

      // analyze the visual

      // make sure it's IPA
      WCHAR szIPA[2];
      WCHAR szReadable[30];
      szIPA[0] = gTTSNS.m_cVisualIPA;
      szIPA[1] = 0;
      DWORD dwInvalid, dwNeed;
      if (szIPA[0]) {
         fGotNonZeroIPA = TRUE;
         hRes = pconv.FromIPA (szIPA, &dwInvalid,
            szReadable, sizeof(szReadable)/2, &dwNeed);
         if (hRes || (dwInvalid != (DWORD)-1L))
            Warning ("IPA phoneme may be invalid for the language");
      }

      // see if the mouth is open
      if (gTTSNS.m_VisualMouth.bMouthHeight)
         fMouthEverOpen = TRUE;

   }

   if (!fGotVisual)
      Failed ("Didn't send any visual");
   else {
      // if all IPA 0 then error
      if (!fGotNonZeroIPA)
         Failed ("All IPA phonemes were set to 0");

      // it the mouth was never open then error
      if (!fMouthEverOpen)
         Failed ("The mouth never opened");

      // if the last phoneme is not 0 then error
      if (gTTSNS.m_cVisualIPA)
         Failed ("Last IPA phoneme should be 0");

      // if the mouth is open now then fail
      if (gTTSNS.m_VisualMouth.bMouthHeight)
         Failed ("Last mouth shape should be closed");

   }

      
   Passed ();
}

// - Send in IPA phonemes and make sure it's speaking the right words

// - Send in invalid IPA phonemes and make sure it still speaks, although might take slightly
// less time. Shouldn't just stop speaking.
void CALLBACK Test074 (void)
{
   HRESULT hRes;
   CSTPhoneConv   pconv;
   pconv.Init();
   pconv.SetByLang(gTTSModeInfo.language.LanguageID);

   // checks to make sure the engine claims to support this
#ifndef _DEBUG
   NEEDTTSFEATURE (TTSFEATURE_IPATEXTDATA);
#endif
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   WCHAR szTemp[256];

   // convert to an IPA string
   DWORD dwFirst;
   DWORD dwNeed;
   hRes = pconv.ToIPA (L"hhaxl1ow dhehr",
      &dwFirst, szTemp, sizeof(szTemp)/2, &dwNeed);
   CHECKFAILED ("ToIPA");

   // speak it out
   hRes = SpeakToSR ((char*) szTemp, TRUE, TRUE);
   CHECKFAILED ("SpeakToSR");

   // recognize this
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<start>=hello\n"
      "<start>=there\n"
      "<start>=hello there\n"
      "<start>=this is a junk sentence\n"
      "<start>=make sure it doesn't recognize this\n"
      "<start>=goodbye\n"
      );
   CHECKFAILED ("SetMainGrammar");

   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   // wait for a phrase finish
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't recognize anything.");
      return;
   }

   // else recognized, make sure it's hello world
   if (stricmp(gSRGramNS.m_szPhraseFinishText, "hello there"))
      Failed ("Didn't recognize hello there");

   ClearMainGrammar();
   ClearSRAudio();

   Passed();
}


// Experiment to see if we can use SR scores to test the quality
// of text-to-speech voices
void CALLBACK Test075 (void)
{
   NEEDSRINTERFACE (SRI_ISRESSCORES);
   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   // score sum
   long  lTotal = 0;
   long  lCount = 0;
   char  szTemp[5000];

   PSTR apszSpeak[] = {
      "This is a test sentence.", "this is a test sentence",
      "All work and no play makes jack a dull boy.", "all work and no play makes jack a dull boy",
      "Four score and seven years ago.", "four score and seven years ago",
      "six two three four eight nine seven.", "six two three four eight nine seven",
      "Text-to-speech gets recognized well.", "text to speech gets recognized well",
      "Floodwaters began receding from the stricken outback.", "Floodwaters began receding from the stricken outback", 
      "An airforce transport plane dropped food and milk overnight.", "An airforce transport plane dropped food and milk overnight",
      "Police evacuated more than one thousand people from their homes.", "Police evacuated more than one thousand people from their homes", 
      "The Katherine river peaked at a record sixty five point six feet.", "The Katherine river peaked at a record sixty five point six feet", 
      "Mark thinks this is good enough for now.", "Mark thinks this is good enough for now"
   };

   DWORD i;
   for (i = 0; i < (sizeof(apszSpeak) / (2*sizeof(char*))); i++) {
      char  *pszSpeak, *pszRecog;

      pszSpeak = apszSpeak[i*2];
      pszRecog = apszSpeak[i*2+1];

      wsprintf (szTemp,
         "[<Start>]\n"
         "<start>=%s\n",
         pszRecog);

      // get a resutls object
      PCSRResult  pRes;
      pRes = GetResultsObject(pszSpeak, szTemp);
      if (!pRes) {
         Warning ("Couldn't get results object");
         continue;
      }

      // get the score
      HRESULT  hRes;
      long  lTemp;
      hRes = pRes->GetPhraseScore (0, &lTemp);
      if (hRes)
         Warning ("Couldn't get score");
      else {
         lCount++;
         lTotal += lTemp;

         wsprintf (szTemp, "Phrase score = %d", (int) lTemp);
         Information (szTemp);
      }



      // free the object
      delete pRes;
   }

   if (lCount)
      lTotal /= lCount;

   Information (
      "The acoustic score can be used to compare text-to-speech engine "
      "voice quality against once another - Or at least how much the "
      "recognizer likes the voice. However, scores can only be compared "
      "if they were produced by the same recognizer, and with the same "
      "set of input sentences."
      );
   wsprintf (szTemp, "Average Score = %d", (int) lTotal);
   Information (szTemp);

   Passed();
}


// Lex: Get pronunciations - known valid word
// note: it is recommeded that ILexPronounce2::GetFrom be used to instead
void CALLBACK Test076(void)
{
	NEEDSRINTERFACE (TTSI_ILEXPRONOUNCE);	
   HRESULT  hRes;

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
	
	// access through main object
   do {
		hRes = gpTTSMode->Get(
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

	if (hRes == TTSERR_NOTSUPPORTED || hRes == TTSERR_INVALIDINTERFACE) {
      Warning ("Get not supported through TTSMode");
   }
	else {
  		if( hRes != LEXERR_INVALIDSENSE || wSense == 1)	// if failed on first try or other error then failed
			Failed ("Get failed with main object", hRes);
		if( wSense >= 64 )
			Failed ("Get does not support Sense parameter");
	}

	// cleanup
	free (pEngineInfo);
	Passed();

	return;
}

// Lex: Get pronunciations - get pronunciation of word not in lex
// note: it is recommended that ILexPronounce2::GetFrom be used instead
void CALLBACK Test077(void)
{
	NEEDSRINTERFACE (TTSI_ILEXPRONOUNCE);
	HRESULT  hRes;

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
	if( !(gpTTSMode->m_pILexPronounce) )
		Warning ("unable to test: gpTTSMode->m_pILexPronounce is NULL");
	else {
		hRes = gpTTSMode->m_pILexPronounce->Get(CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
			// check failure condition
		if( FAILED(hRes) && hRes != LEXERR_NOTINLEX ) 
			Failed ("Expected LEXERR_NOTINLEX error for word not in lex through main object", hRes);

			// check for NULL word
		hRes = gpTTSMode->m_pILexPronounce->Get(CHARSET_IPAPHONETIC,pszNULL,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
		if( hRes != TTSERR_INVALIDPARAM && hRes != LEXERR_INVALIDTEXTCHAR) 
			Failed ("Expected TTSERR_INVALIDPARAM for get of NULL word through main object", hRes);
	}

	// cleanup
	free (pEngineInfo);

	Passed();

	return;
}

// Lex: Get pronunciations - known valid word, check returns proper size when buffer too small.
// note: it is recommended that ILexPronounce2::GetFrom be used instead
void CALLBACK Test078(void)
{
	NEEDSRINTERFACE (TTSI_ILEXPRONOUNCE);	
	HRESULT  hRes;

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
	
	if( !(gpTTSMode->m_pILexPronounce) ) 
		Warning ("unable to test: gpTTSMode->m_pILexPronounce is NULL");
	else {
		hRes = gpTTSMode->m_pILexPronounce->Get(CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
		// check failure condition
		if( (hRes != LEXERR_NOTINLEX && hRes != LEXERR_ENGBUFTOOSMALL && hRes != LEXERR_PRNBUFTOOSMALL) || dwPronounceNeeded == 0) 
			Failed ("Main object Get did not indicate return buffer too small or PronounceNeeded not set");
	}
		 
	// cleanup
	free (pEngineInfo);

	Passed();

	return;
}

// Lex: Add pronunciations - valid word
// note: it is recommmeded that ILexPronounce2::AddTo be used instead
void CALLBACK Test079(void)
{
	NEEDSRINTERFACE (TTSI_ILEXPRONOUNCE);
	HRESULT  hRes;

	// in
	WCHAR *pszText = L"TestNown";
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F, 0x0, 0x0};  

	// test main object
	if( !(gpTTSMode->m_pILexPronounce) ) 
		Warning ("unable to test: gpTTSMode->m_pILexPronounce is NULL");
	else {
		// add valid word
		hRes = gpTTSMode->m_pILexPronounce->Add(CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
		if( FAILED(hRes) ) 
			Failed ("Main object failed to add valid word", hRes);
		else {
			//delete entry
			hRes = gpTTSMode->m_pILexPronounce->Remove(pszText, 0);
			if( FAILED(hRes) ) 
				Warning ("Passed although Remove returned error when trying to remove word added", hRes);
		}
	}
	
	Passed();

	return;
}

// Lex: Add pronunciations - known invalid word
// note: it is recommmeded that ILexPronounce2::AddTo be used instead
void CALLBACK Test080(void)
{
	NEEDSRINTERFACE (TTSI_ILEXPRONOUNCE);
	HRESULT  hRes;

	// in
	WCHAR *pszText = L"TestNown";
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  
	DWORD dwPronounceNeeded = 0;
	VOICEPARTOFSPEECH	PartOfSpeech = VPS_UNKNOWN;
	PVOID	pEngineInfo = malloc (1024);
	DWORD dwEngineInfoNeeded = 0;
	WORD wSense = 0;
	
	if( !(gpTTSMode->m_pILexPronounce) ) 
		Warning ("unable to test: gpTTSMode->m_pILexPronounce is NULL");
	else {		// try to add invalid word
		// NULL word
		hRes = gpTTSMode->m_pILexPronounce->Add(CHARSET_IPAPHONETIC,NULL,szPronounce,VPS_NOUN,NULL,0);
		if( hRes != TTSERR_INVALIDPARAM && hRes != LEXERR_INVALIDTEXTCHAR) 
			Failed ("wrong or no error return on adding invalid word through main object", hRes);

		// NULL pronunciations
		hRes = gpTTSMode->m_pILexPronounce->Add(CHARSET_IPAPHONETIC,pszText,NULL,VPS_NOUN,NULL,0);
		if( hRes != TTSERR_INVALIDPARAM && hRes != LEXERR_INVALIDPRONCHAR) 
			Failed ("wrong or no error return on adding invalid word through main object", hRes);
	
	}			
	
	//clean up
	free (pEngineInfo);
	
	Passed();

	return;
}

// Lex: Add pronunciations - valid word, get it
// note: it is recommmeded that ILexPronounce2::AddTo and GetFrom be used instead
void CALLBACK Test081(void)
{
	NEEDSRINTERFACE (TTSI_ILEXPRONOUNCE);
	HRESULT  hRes;

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


	if( !(gpTTSMode->m_pILexPronounce) )
		Warning ("unable to test: gpTTSMode->m_pILexPronounce is NULL");
	else {		// attempt to add word to main object
		hRes = gpTTSMode->m_pILexPronounce->Add(CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
		if( FAILED(hRes) ) {
			if( hRes == LEXERR_ALREADYINLEX ) {
				hRes = gpTTSMode->m_pILexPronounce->Remove(pszText,0);
				if( FAILED(hRes) )
					Failed ("Failed to remove word through main object", hRes);
			}
			else
				Warning ("Failed to add valid word in main object", hRes);
		}
		else	{
         WCHAR szPron[512];

			// get word that was added
			hRes = gpTTSMode->m_pILexPronounce->Get(CHARSET_IPAPHONETIC,pszText,wSense,szPron,sizeof(szPron),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
			if( FAILED(hRes) )
				Failed ("Failed to get added word in main object", hRes);

			// attempt to remove word that was added
			hRes = gpTTSMode->m_pILexPronounce->Remove(pszText, 0);
			if( FAILED(hRes) )
				Failed ("Failed to remove added word in main object", hRes);
		}
	}

	// clean up
	free (pEngineInfo);

	Passed();

	return;
}

// -  various tags in text
void CALLBACK Test082 (void)
{
   // checks to make sure the engine claims to support this
   NEEDENGLISH;
   NEEDAUDIO;

   char  *psz = NULL;
   
	// Chr tag - Angry, Happy
	psz = SpeakAndListen ( "\\Chr=\"Angry\"\\angry voice\\Chr=\"Normal\"\\",
		"[<Start>]\n"
		"<Start>=angry voice\n");
	if (!psz)
      Failed ("returned null");
	else		  // make sure matches
		if (stricmp(psz, "angry voice")) 
			Failed ("didn't match");
	
	psz = NULL;
	psz = SpeakAndListen ( "\\Chr=\"Happy\"\\happy voice\\Chr=\"Normal\"\\",
		"[<Start>]\n"
		"<Start>=happy voice\n");
	if (!psz)
      Failed ("returned null");
	else		  // make sure matches
		if (stricmp(psz, "happy voice")) 
			Failed ("didn't match");
	
	// Com tag
	psz = NULL;
	psz = SpeakAndListen ( "the \\Com=\"comment\"\\truth, the whole truth, \\Com=\"comment 2\"\\and nothing but the truth.",
      "[<Start>]\n"
      "<Start>=the truth the whole truth and nothing but the truth\n"
      );
	if (!psz)
      Failed ("returned null");
	else		  // make sure matches
		if (stricmp(psz, "the truth the whole truth and nothing but the truth")) 
			Failed ("didn't match");

		
	// Ctx tag - Numbers, Unknown
	psz = NULL;
	psz = SpeakAndListen ( "the herd is \\Ctx=\"Numbers\"\"\\125\\Ctx=\"unknown\"\\strong and growing",
      "[<Start>]\n"
      "<Start>=the herd is <alt> twenty five strong and growing\n"
		"<Alt>=one hundred\n"
		"<Alt>=one\n"
      );
	if (!psz)
      Failed ("returned null");
	else		  // make sure matches
		if( stricmp(psz, "the herd is one hundred twenty five strong and growing") && 
			stricmp(psz, "the herd is one twenty five strong and growing") ) 

			Failed ("didn't match");

   	
	// Emp tag
	psz = NULL;
	psz = SpeakAndListen ( "the \\Emp\\truth, the \\Emp\\whole truth, and nothing \\Emp\\but the truth.",
      "[<Start>]\n"
      "<Start>=the truth the whole truth and nothing but the truth\n"
      );
	if (!psz)
      Failed ("returned null");
	else		  // make sure matches
		if (stricmp(psz, "the truth the whole truth and nothing but the truth")) 
			Failed ("didn't match");

	// Pau tag
	psz = NULL;
	psz = SpeakAndListen ( "the \\Pau=100\\truth, the \\Pau=0\\whole truth, and nothing \\Pau=1\\but the truth.",
      "[<Start>]\n"
      "<Start>=the truth the whole truth and nothing but the truth\n"
      );
	if (!psz)
      Failed ("returned null");
	else		  // make sure matches
		if (stricmp(psz, "the truth the whole truth and nothing but the truth")) 
			Failed ("didn't match");

   Passed();
}

// IAttributes - DWORDGet/Set - TTSATTR_THREADPRIORITY (DWORD)
void CALLBACK Test083(void)
{
	NEEDSRINTERFACE (TTSI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwAttrib = 0;
	DWORD dwValue = 0;
	DWORD dwInvalidPriority;

	if( !(gpTTSMode->m_pIAttributes) ) {
		Warning ("unable to test: gpTTSMode->m_pIAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of TTSATTR_THREADPRIORITY attribute

	// get the current setting
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_THREADPRIORITY, &dwValue);
	if( FAILED(hRes) )
		Failed("Failed to get thread priority attribute value",hRes);
	
	// try to change attribute 
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_THREADPRIORITY, THREAD_PRIORITY_ABOVE_NORMAL);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_ABOVE_NORMAL )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_THREADPRIORITY, THREAD_PRIORITY_BELOW_NORMAL);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_BELOW_NORMAL )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_THREADPRIORITY, THREAD_PRIORITY_HIGHEST);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_HIGHEST )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_THREADPRIORITY, THREAD_PRIORITY_IDLE);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_IDLE )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_THREADPRIORITY, THREAD_PRIORITY_LOWEST);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_LOWEST )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_THREADPRIORITY, THREAD_PRIORITY_NORMAL);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_NORMAL )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_THREADPRIORITY, THREAD_PRIORITY_TIME_CRITICAL);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to get thread priority that it was set to",hRes);
	else {
		// double check that it got set correctly
		hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_TIME_CRITICAL )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	// check for invalid arguments handling
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_THREADPRIORITY, NULL);
	if( SUCCEEDED(hRes) || hRes != E_INVALIDARG)
		Failed("Failed to return correct error for invalid argument",hRes);

	// make sure it is rejected for invalid thread priorities
	int iCheckCount;
	iCheckCount = 0;
	do {
		dwInvalidPriority = rand();
		switch (dwInvalidPriority) {
		case THREAD_PRIORITY_IDLE:
		case THREAD_PRIORITY_LOWEST:
		case THREAD_PRIORITY_BELOW_NORMAL:
		case THREAD_PRIORITY_NORMAL:
		case THREAD_PRIORITY_ABOVE_NORMAL:
		case THREAD_PRIORITY_HIGHEST:
		case THREAD_PRIORITY_TIME_CRITICAL:
			break;
		default:  // if not a defined thread priority check for error
			{
				// check for invalid value only ten times, otherwise this would run a long time.
				hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_THREADPRIORITY, dwInvalidPriority);
				if( SUCCEEDED(hRes) || hRes != E_INVALIDARG) {
					Failed("Failed to return correct error for invalid argument",hRes);
					// don't check the rest in case there are lots of errors.				
					goto cleanup;
				}
				iCheckCount++;
			}
		}
	} while (iCheckCount < 100);

	// cleanup
cleanup:

	ClearMainGrammar();

	Passed();

	return;
}


// IAttributes - DWORDGet
void CALLBACK Test084(void)
{
	NEEDSRINTERFACE (TTSI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwAttrib = 0;
	DWORD dwValue = 0;
	DWORD dwTempValue = 0;
		
	if( !(gpTTSMode->m_pIAttributes) ) {
		Warning ("unable to test: gpTTSMode->m_pIAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of an invalid argument
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (NULL, &dwValue);
	if (hRes != E_INVALIDARG)
		Failed("m_pIAttributes->DWORDGet did not return invalid argument on NULL attrib",hRes);
		
	// try to get all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
		hRes = gpTTSMode->m_pIAttributes->DWORDGet (dwAttrib, &dwValue);
      if( FAILED(hRes) && hRes != E_INVALIDARG ) {
         // Ignore LevelGet errors since the dummy audio object doesn't support it
         if (dwAttrib != SRATTR_LEVEL)
			   Failed("DWORDGet Failed with unknown error",hRes);
      }

		// check the returned a value by called second time a making sure it's the same.
		if( SUCCEEDED(hRes)) {
			dwTempValue = dwValue + 1;
			hRes = gpTTSMode->m_pIAttributes->DWORDGet (dwAttrib, &dwTempValue);
			if( dwValue != dwTempValue )
				Failed( "DWORDGet Indicated success but did not return a value");
		}
	}


		// cleanup
cleanup:

	Passed();
	return;
}

// IAttributes - DWORDSet - try all values and make sure they work
void CALLBACK Test085(void)
{
	NEEDSRINTERFACE (TTSI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwAttrib = 0;
	DWORD dwOldValue = 0;
	DWORD dwValue = 0;

			
	if( !(gpTTSMode->m_pIAttributes) ) {
		Warning ("unable to test: gpTTSMode->m_pIAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of an invalid argument
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (NULL, dwValue);
	if (hRes != E_INVALIDARG)
		Failed("m_pIAttributes->DWORDSet did not return invalid argument on NULL attrib",hRes);
		
	// try to set all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
		hRes = gpTTSMode->m_pIAttributes->DWORDGet (dwAttrib, &dwValue);	
		if( SUCCEEDED(hRes) ) {
			// make it works first
			hRes = gpTTSMode->m_pIAttributes->DWORDSet (dwAttrib, dwValue);
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
				hRes = gpTTSMode->m_pIAttributes->DWORDSet (dwAttrib, dwValue);
				if( FAILED(hRes) )
					Failed( "failed to DWORDSet valid attribute",hRes);
				
				// change value slightly to insure it get's a value and that it's the correct value
				dwValue++;
				hRes = gpTTSMode->m_pIAttributes->DWORDGet (dwAttrib, &dwValue);
				if( FAILED(hRes) || (dwValue != dwOldValue + 1) )
					Failed( "DWORDSet failed to set value",hRes);

				// set back to the way we found it
				hRes = gpTTSMode->m_pIAttributes->DWORDSet (dwAttrib, dwOldValue);
				if( FAILED(hRes) )
					Failed( "unable to set attribute back to original value",hRes);
			}
		}

      if( FAILED(hRes) && hRes != E_INVALIDARG ) {
         // Ignore LevelSet errors since the dummy audio object doesn't support it
         if (dwAttrib != SRATTR_LEVEL)
			   Failed("DWORDSet Failed with unknown error",hRes);
      }
	}


		// cleanup
cleanup:

	Passed();
	return;
}



// IAttributes - StringGet
void CALLBACK Test086(void)
{
	NEEDSRINTERFACE (TTSI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwAttrib;
	WCHAR szString[1024];
	DWORD dwSize = 0;
	DWORD dwNeeded = 0;

	if( !(gpTTSMode->m_pIAttributes) ) {
		Warning ("unable to test: gpTTSMode->m_pIAttributes is NULL");
		goto cleanup;
	}

	// check proper handle of invalid parameter
	hRes = gpTTSMode->m_pIAttributes->StringGet (NULL, szString, dwSize, &dwNeeded);
	if (hRes != E_INVALIDARG)
		Failed("StringGet did not return invalid argument on NULL attrib",hRes);

		// try to set all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
		dwNeeded = 0;

		// get buffer size needed for string
		hRes = gpTTSMode->m_pIAttributes->StringGet (dwAttrib, szString, 0, &dwNeeded);

		// should return one of two condition, otherwise it's a failure
		if( !(hRes == E_INVALIDARG || hRes == E_BUFFERTOOSMALL) )
			Failed ("Failed to return correct hRes, expect E_INVALIDARG or E_BUFFERTOOSMALL", hRes);
	
			// check for handle of invalid parameter for valid attribs
		if( hRes == E_BUFFERTOOSMALL ) {
#define  FILL_CHARACTER    0xFB
				// check that recieved information in string when all params are valid
			_wcsnset(szString, FILL_CHARACTER, sizeof(szString));
			hRes = gpTTSMode->m_pIAttributes->StringGet (dwAttrib, szString, dwNeeded, &dwNeeded);
			if ( FAILED(hRes) )
				Failed ("Failed to get string",hRes);
			else {
				if( szString[0] == FILL_CHARACTER ) // string didn't get populated with data
					Failed ("String did not get populated with data");
			}
		}
				// check for invalid argument handling of each attribute
		// check for invalid parameter of szString
		hRes = gpTTSMode->m_pIAttributes->StringGet (dwAttrib, NULL, dwNeeded, &dwNeeded);
		if (SUCCEEDED(hRes))
			Failed("StringGet did not fail for a NULL szString param",hRes);

		// check invalid dwNeeded param
		hRes = gpTTSMode->m_pIAttributes->StringGet (dwAttrib, szString, 0, NULL);
		if (hRes != E_INVALIDARG)
			Failed("StringGet did not return invalid argument when dwNeeded pointer is NULL",hRes);
	}

	// cleanup
cleanup:

	Passed();
	return;
}

// IAttributes - StringSet
void CALLBACK Test087(void)
{
	NEEDSRINTERFACE (TTSI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwNeeded = 0;
	DWORD dwAttrib = 0;
	WCHAR szString[1024];

	if( !(gpTTSMode->m_pIAttributes) ) {
		Warning ("unable to test: gpTTSMode->m_pIAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of an invalid argument
	hRes = gpTTSMode->m_pIAttributes->StringSet (NULL, szString);
	if (hRes != E_INVALIDARG)
		Failed("m_pIAttributes->StringSet did not return invalid argument on NULL attrib",hRes);
	
		// check invalid szString param
	hRes = gpTTSMode->m_pIAttributes->StringSet (dwAttrib, NULL);
	if (hRes != E_INVALIDARG)
		Failed("StringGet did not return invalid argument when szString pointer is NULL",hRes);


	// try to set all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
		dwNeeded = 0;

		// get buffer size needed for string
		hRes = gpTTSMode->m_pIAttributes->StringGet (dwAttrib, szString, 0, &dwNeeded);

		// should return one of two condition, otherwise it's a failure
		if( !(hRes == E_INVALIDARG || hRes == E_BUFFERTOOSMALL) ) {
			Failed ("Failed to StringGet for this attrib, cannot complete test", hRes);
			continue;
		}
	
			// get current string for attribute
		if( hRes == E_BUFFERTOOSMALL ) {
				// check that recieved information in string when all params are valid
			_wcsnset(szString, FILL_CHARACTER, sizeof(szString));
			hRes = gpTTSMode->m_pIAttributes->StringGet (dwAttrib, szString, dwNeeded, &dwNeeded);
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
			hRes = gpTTSMode->m_pIAttributes->StringSet (dwAttrib, szString);
			if( FAILED(hRes) )
				Failed ("Failed to set string to the same value it already has",hRes);
		
		}
	}

		// cleanup
cleanup:
	
	Passed();
	return;
}

// IAttribute - MemoryGet
void CALLBACK Test088(void)
{
	NEEDSRINTERFACE (TTSI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwNeeded = 0;
	DWORD dwAttrib = 0;
	DWORD dwSize = 0;
	void * pMem = NULL;

	if( !(gpTTSMode->m_pIAttributes) ) {
		Warning ("unable to test: gpTTSMode->m_pIAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of an invalid argument
	hRes = gpTTSMode->m_pIAttributes->MemoryGet (NULL, &pMem, &dwSize);
	if( SUCCEEDED(hRes) )
		Failed ("Failed to return an error with NULL attribute");

	// try to set all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
	
		dwSize = 0;
		// check that returns correct error regardless of attribute supported or not
		hRes = gpTTSMode->m_pIAttributes->MemoryGet (dwAttrib, &pMem, &dwSize);
		if( !(SUCCEEDED(hRes) || hRes == E_INVALIDARG || hRes == E_WRONGTYPE || 
				hRes == E_ACCESSDENIED || hRes == E_OUTOFMEMORY) )
			Failed ("uncorrect return value", hRes);

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

	Passed();
	return;
}

// IAttribute - MemorySet
void CALLBACK Test089(void)
{
	NEEDSRINTERFACE (TTSI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwNeeded = 0;
	DWORD dwAttrib = 0;
	DWORD dwSize = 0;
	void * pMem = NULL;

	if( !(gpTTSMode->m_pIAttributes) ) {
		Warning ("unable to test: gpTTSMode->m_pIAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of an invalid argument
	hRes = gpTTSMode->m_pIAttributes->MemoryGet (NULL, &pMem, &dwSize);
	if( SUCCEEDED(hRes) )
		Failed ("Failed to return an error with NULL attribute");

	// try to set all possible SYSTEM attributes and make sure it success or returns the correct error
	for( dwAttrib=1; dwAttrib<0xffff; dwAttrib++) {
	
		dwSize = 0;
		// check that returns correct error regardless of attribute supported or not
		hRes = gpTTSMode->m_pIAttributes->MemoryGet (dwAttrib, &pMem, &dwSize);
		if( !(SUCCEEDED(hRes) || hRes == E_INVALIDARG || hRes == E_WRONGTYPE || 
				hRes == E_ACCESSDENIED || hRes == E_OUTOFMEMORY) )
			Failed ("uncorrect return value", hRes);

		if( SUCCEEDED(hRes)) {
			// if success check dwSize for value
			if( dwSize == 0 )
				Failed ("indicated success yet dwSize not set");

			// if success check that pointer points somewhere
			if( !pMem )
				Failed ("indicated success yet memory pointer not set");


			// try to set memory
			hRes = gpTTSMode->m_pIAttributes->MemorySet (dwAttrib, pMem, dwSize);
			if( !(SUCCEEDED(hRes) || hRes == E_INVALIDARG || hRes == E_WRONGTYPE || 
				hRes == E_ACCESSDENIED || hRes == E_OUTOFMEMORY) ) {

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

	Passed();
	return;
}

// IAttributes - DWORDGet/Set - TTSATTR_PITCH (DWORD)
void CALLBACK Test090(void)
{
	NEEDSRINTERFACE (TTSI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwAttrib = 0;
	DWORD dwValue = 0;
	DWORD dwOldValue = 0;
	DWORD dwMinMax = 0;

	if( !(gpTTSMode->m_pIAttributes) ) {
		Warning ("unable to test: gpTTSMode->m_pIAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of TTSATTR_PITCH attribute

	// get the current setting
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_PITCH, &dwOldValue);
	if( FAILED(hRes) )
		Failed("Failed to get pitch attribute value",hRes);
	
	// try to change attribute 
	dwValue = dwOldValue + 1;
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_PITCH, dwValue);
	if( FAILED(hRes) )
		Failed("Failed to set pitch attribute value",hRes);

	// make sure it changed
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_PITCH, &dwValue);
	if( FAILED(hRes) || dwValue != (dwOldValue + 1) )
		Failed("Failed to set pitch attribute value",hRes);


	// check for invalid arguments handling
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_PITCH, NULL);
	if( hRes != E_INVALIDARG)
		Failed("Failed to return correct error for invalid argument",hRes);

	// get min and set to below it
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_PITCH, TTSATTR_MINPITCH);
	if( FAILED( hRes ))
		Failed ("Failed to set minimum pitch", hRes);
	
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_PITCH, &dwMinMax);
	if( FAILED( hRes ))
		Failed ("Failed to get pitch",hRes);
	
	if( dwMinMax > 0 ) {
		hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_PITCH, dwMinMax - 1);
		if( hRes != TTSERR_VALUEOUTOFRANGE )
			Failed("Failed to return correct error for set of pitch below minimum",hRes);
	}

	// get max and set to above it
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_PITCH, TTSATTR_MAXPITCH);
	if( FAILED( hRes ))
		Failed ("Failed to set maximum pitch", hRes);
	
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_PITCH, &dwMinMax);
	if( FAILED( hRes ))
		Failed ("Failed to get pitch",hRes);
	
	// only 0xffffffff is max WORD size
	if( TTSATTR_MAXPITCH < 0xffffffff ) {
		hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_PITCH, dwMinMax + 1);
		if( hRes != TTSERR_VALUEOUTOFRANGE )
			Failed("Failed to return correct error for set of pitch above maximum",hRes);
	}

	// set pitch back to start value
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_PITCH, dwOldValue);
	if( FAILED(hRes) )
		Failed("Failed to set pitch attribute value",hRes);


	// cleanup
cleanup:

	ClearMainGrammar();

	Passed();

	return;
}

// IAttributes - DWORDGet/Set - TTSATTR_SPEED (DWORD)
void CALLBACK Test091(void)
{
	NEEDSRINTERFACE (TTSI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwAttrib = 0;
	DWORD dwValue = 0;
	DWORD dwOldValue = 0;
	DWORD dwMinMax = 0;

	if( !(gpTTSMode->m_pIAttributes) ) {
		Warning ("unable to test: gpTTSMode->m_pIAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of TTSATTR_SPEED attribute

	// get the current setting
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_SPEED, &dwOldValue);
	if( FAILED(hRes) )
		Failed("Failed to get speed attribute value",hRes);
	
	// try to change attribute 
	dwValue = 100;
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_SPEED, dwValue);
	if( FAILED(hRes) )
		Failed("Failed to set speed attribute value",hRes);

	// make sure it changed
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_SPEED, &dwValue);
	if( FAILED(hRes) || dwValue != 100 )
		Failed("Failed to set speed attribute value",hRes);


	// check for invalid arguments handling
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_SPEED, NULL);
	if( hRes != E_INVALIDARG)
		Failed("Failed to return correct error for invalid argument",hRes);

	// get min and set to below it
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_SPEED, TTSATTR_MINSPEED);
	if( FAILED( hRes ))
		Failed ("Failed to set minimum speed", hRes);
	
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_SPEED, &dwMinMax);
	if( FAILED( hRes ))
		Failed ("Failed to get speed",hRes);
	
	if( dwMinMax > 0 ) {
		hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_SPEED, dwMinMax - 1);
		if( hRes != TTSERR_VALUEOUTOFRANGE )
			Failed("Failed to return correct error for set of speed below minimum",hRes);
	}

	// get max and set to above it
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_SPEED, TTSATTR_MAXSPEED);
	if( FAILED( hRes ))
		Failed ("Failed to set maximum speed", hRes);
	
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_SPEED, &dwMinMax);
	if( FAILED( hRes ))
		Failed ("Failed to get speed",hRes);
	
	// only 0xffffffff is max WORD size
	if( TTSATTR_MAXSPEED < 0xffffffff ) {
		hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_SPEED, dwMinMax + 1);
		if( hRes != TTSERR_VALUEOUTOFRANGE )
			Failed("Failed to return correct error for set of speed above maximum",hRes);
	}

	// set speed back to start value
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_SPEED, dwOldValue);
	if( FAILED(hRes) )
		Failed("Failed to set speed attribute value",hRes);


	// cleanup
cleanup:

	ClearMainGrammar();

	Passed();

	return;
}


// IAttributes - DWORDGet/Set - TTSATTR_VOLUME (DWORD)
void CALLBACK Test092(void)
{
	NEEDSRINTERFACE (TTSI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwAttrib = 0;
	DWORD dwValue = 0;
	DWORD dwOldValue = 0;
	DWORD dwMinMax = 0;

	if( !(gpTTSMode->m_pIAttributes) ) {
		Warning ("unable to test: gpTTSMode->m_pIAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of TTSATTR_VOLUME attribute

	// get the current setting
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_VOLUME, &dwOldValue);
	if( FAILED(hRes) )
		Failed("Failed to get volume attribute value",hRes);
	
	// try to change attribute 
	dwValue = 100;
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_VOLUME, dwValue);
	if( FAILED(hRes) )
		Failed("Failed to set volume attribute value",hRes);

	// make sure it changed
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_VOLUME, &dwValue);
	if( FAILED(hRes) || LOWORD(dwValue) != 100 )
		Failed("Failed to get volume attribute value expected",hRes);


	// check for invalid arguments handling
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_VOLUME, NULL);
	if( hRes != E_INVALIDARG)
		Failed("Failed to return correct error for invalid argument",hRes);

	// get min and set to below it
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_VOLUME, TTSATTR_MINVOLUME);
	if( FAILED( hRes ))
		Failed ("Failed to set minimum volume", hRes);
	
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_VOLUME, &dwMinMax);
	if( FAILED( hRes ))
		Failed ("Failed to get volume",hRes);
	
	if( dwMinMax > 0 ) {
		hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_VOLUME, dwMinMax - 1);
		if( hRes != TTSERR_VALUEOUTOFRANGE )
			Failed("Failed to return correct error for set of volume below minimum",hRes);
	}

	// set left channel only
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_VOLUME, 60000);
	if( FAILED(hRes) )
		Failed ("Failed to set volume",hRes);

	// check that right channel is minimum or same as left
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_VOLUME, &dwValue);
	if( FAILED( hRes ) )
		Failed ("Failed to get volume",hRes);
	else {
		if ( HIWORD(dwValue) != dwMinMax ) {
			Information( "This engine does not support setting seperate left/right channels");
			if ( HIWORD(dwValue) != LOWORD(dwValue) )
				Failed("Failed to get volume attribute value expected",hRes);
		}
		else {
			Information( "This engine may support seperarte left/right channels");
			if ( HIWORD(dwValue != LOWORD(dwValue) ) )
				Failed ("Failed to get volume attribute value expected",hRes);
		}
	}

	// get max and set to above it
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_VOLUME, TTSATTR_MAXVOLUME);
	if( FAILED( hRes ))
		Failed ("Failed to set maximum volume", hRes);
	
	hRes = gpTTSMode->m_pIAttributes->DWORDGet (TTSATTR_VOLUME, &dwMinMax);
	if( FAILED( hRes ))
		Failed ("Failed to get volume",hRes);
	
	// only 0xffffffff is max WORD size
	if( TTSATTR_MAXVOLUME < 0xffffffff ) {
		hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_VOLUME, dwMinMax + 1);
		if( hRes != TTSERR_VALUEOUTOFRANGE )
			Failed("Failed to return correct error for set of volume above maximum",hRes);
	}

	// set volume back to start value
	hRes = gpTTSMode->m_pIAttributes->DWORDSet (TTSATTR_VOLUME, dwOldValue);
	if( FAILED(hRes) )
		Failed("Failed to set volume attribute value",hRes);


	// cleanup
cleanup:

	ClearMainGrammar();

	Passed();

	return;
}

// Lex2: GetFrom pronunciations - known valid word
void CALLBACK Test093(void)
{
	NEEDTTSINTERFACE (TTSI_ILEXPRONOUNCE2);	

   HRESULT  hRes;

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
	if( !(gpTTSMode->m_pILexPronounce2) ) 
		Information ("unable to test through main object gpTTSMode->m_pILexPronounce2 is NULL");
	else {
		// first get all lexicons that can read from
		hRes = gpTTSMode->m_pILexPronounce2->QueryLexicons(QUERY_READ,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test through main object, QueryLexicons failed", hRes);
		}
		else {
			// check each lexicon by shifting through each bit
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++)	{
				if( dwQueryLex & dwLexicons ) {
					do {		// check each sense
						hRes = gpTTSMode->m_pILexPronounce2->GetFrom(dwQueryLex,CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);		// sized needed of addition info if pEnginInfo is too small
						wSense++;
					} while ( !hRes && wSense < 64 );
					if( !(hRes == LEXERR_NOTINLEX && dwQueryLex == ILP2_USER))	{
						if( hRes != LEXERR_INVALIDSENSE || wSense == 1 ) 
							Failed ("GetFrom failed with main object", hRes);
						if( wSense >= 64 )
							Failed ("GetFrom does not support Sense parameter");
					}
					wSense = 0;
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}

	// cleanup
	free (pEngineInfo);
	
	Passed();
	return;
}

// Lex2: GetFrom pronunciations - get pronunciation of word not in LTS lex
void CALLBACK Test094(void)
{
	NEEDTTSINTERFACE (TTSI_ILEXPRONOUNCE2);
	HRESULT  hRes;

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
	if( !(gpTTSMode->m_pILexPronounce2) ) {
		Information ("unable to test: gpTTSMode->m_pILexPronounce2 is NULL");
	}
	else {
		hRes = gpTTSMode->m_pILexPronounce2->GetFrom(ILP2_LTS,CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
		// check failure condition
		if( FAILED(hRes) ) 
			Failed ("Did not return pronunciation of word not in lex", hRes);
	}
	
	// cleanup
	free (pEngineInfo);

	Passed();
	return;
}

// Lex2: GetFrom pronunciations - known valid word, check returns proper size when buffer too small.
void CALLBACK Test095(void)
{
	NEEDTTSINTERFACE (TTSI_ILEXPRONOUNCE2);	
	HRESULT  hRes;

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
	if( !(gpTTSMode->m_pILexPronounce2) ) 
		Information ("unable to test: gpTTSMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpTTSMode->m_pILexPronounce2->QueryLexicons(QUERY_READ,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test AddTo through main object, QueryLexicons failed", hRes);
		}
		else {
			// check each lexicon by shifting through each bit
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++)	{
				if( dwQueryLex & dwLexicons ) {
					hRes = gpTTSMode->m_pILexPronounce2->GetFrom(dwQueryLex,CHARSET_IPAPHONETIC,pszText,wSense,szPronounce,sizeof(szPronounce),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
					// check failure condition
					if( !(hRes == LEXERR_ENGBUFTOOSMALL || hRes == LEXERR_PRNBUFTOOSMALL || (hRes == LEXERR_NOTINLEX && dwQueryLex == ILP2_USER) || dwPronounceNeeded != 0) ) 
						Failed ("Main object GetFrom did not indicate return buffer too small or PronounceNeeded not set");
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}
		 
	// cleanup
	free (pEngineInfo);

	Passed();
	return;
}

// Lex2: AddTo pronunciations - attempt to AddTo and RemoveFrom LTS
void CALLBACK Test096(void)
{
	NEEDTTSINTERFACE (TTSI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	// in
	WCHAR *pszText = L"TestNown";
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  

	// test main object
	if( !(gpTTSMode->m_pILexPronounce2) ) 
		Information ("unable to test: gpTTSMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpTTSMode->m_pILexPronounce2->AddTo(ILP2_LTS,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
		if( SUCCEEDED(hRes) ) 
			Failed ("Main object returned no error on AddTo LTS lexicon");
		//try and delete an entry
		hRes = gpTTSMode->m_pILexPronounce2->RemoveFrom(ILP2_LTS,pszText, 0);
		if( SUCCEEDED(hRes) ) 
			Failed ("Main object returned no error on RemoveFrom LTS lexicon");
	}
		
	Passed();
	return;
}

// Lex2: AddTo pronunciations - valid word
void CALLBACK Test097(void)
{
	NEEDTTSINTERFACE (TTSI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	// in
	WCHAR *pszText = L"TestNown";
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  

	// out
	DWORD dwLexicons = 0;

	// test main object
	if( !(gpTTSMode->m_pILexPronounce2) ) 
		Information ("unable to test: gpTTSMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpTTSMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test AddTo through main object, QueryLexicons failed", hRes);
		}
		else {
			// check each lexicon by shifting through each bit
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++)	{
				if( dwQueryLex & dwLexicons ) {
					hRes = gpTTSMode->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
					if( FAILED(hRes) ) 
						Failed ("Main object failed to add valid word", hRes);
					else {
						//delete entry
						hRes = gpTTSMode->m_pILexPronounce2->RemoveFrom(dwQueryLex,pszText, 0);
						if( FAILED(hRes) ) {
							Warning ("Passed although Remove returned error when trying to remove word added", hRes);
						}
					}
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}
		
	Passed();
	return;
}

// Lex2: AddTo pronunciations - known invalid word, duplicate
void CALLBACK Test098(void)
{
	NEEDTTSINTERFACE (TTSI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	WCHAR *pszText = L"helow";
			// double NULL terminated, UNICODE for    hh eh l ow
	WCHAR szPronounce[6] = {0x68, 0x25B, 0x6C, 0x6F};  

	DWORD dwPronounceNeeded = 0;
	VOICEPARTOFSPEECH	PartOfSpeech = VPS_UNKNOWN;
	PVOID	pEngineInfo = malloc (14 * 64);
	DWORD dwEngineInfoNeeded = 0;
	DWORD	dwLexicons = 0;
	WORD wSense = 0;

   if( !(gpTTSMode->m_pILexPronounce2) ) 
		Information ("unable to test: gpTTSMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpTTSMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) 
			Warning ( "unable to test main object: QueryLexicons failed", hRes);
		else {
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
				if( dwQueryLex & dwLexicons ) {  //add same word twice, should get error second time
					hRes = gpTTSMode->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_ADJECTIVE,NULL,0);
					if( FAILED(hRes) ) 
						Failed("failed to add valid word through main object",hRes);
					else {
						hRes = gpTTSMode->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_ADJECTIVE,NULL,0);
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

	free (pEngineInfo);
	
	Passed();

	return;
}

// Lex2: AddTo pronunciations - valid word, get it
void CALLBACK Test099(void)
{
	NEEDTTSINTERFACE (TTSI_ILEXPRONOUNCE2);
	HRESULT  hRes;

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

	if( !(gpTTSMode->m_pILexPronounce2) ) {
		Information ("unable to test: gpTTSMode->m_pILexPronounce2 is NULL");
	}
	else {
		hRes = gpTTSMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		else {
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
				if( dwQueryLex & dwLexicons ) {
					hRes = gpTTSMode->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,pszText,szPronounce,VPS_NOUN,NULL,0);
					if( FAILED(hRes) ) 
						Warning ("Failed to add valid word in main object", hRes);
					else
					{
						WCHAR szPron[512];
						hRes = gpTTSMode->m_pILexPronounce2->GetFrom(dwQueryLex,CHARSET_IPAPHONETIC,pszText,wSense,szPron,sizeof(szPron),&dwPronounceNeeded,&PartOfSpeech,pEngineInfo,sizeof(pEngineInfo),&dwEngineInfoNeeded);
						if( FAILED(hRes) ) 
							Failed ("Failed to get added word in main object", hRes);
					}
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}
	

	// clean up
	free (pEngineInfo);

	Passed();
	return;
}
