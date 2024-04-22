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

char gszHelloWorldGrammar[] =
      "[<Start>]\n"
      "<Start>=hello world\n";

char gszHelloWorld2Grammar[] =
      "[<Start>]\n"
      "<Start>=hello world\n"
      "<Start>=goodbye\n";

char gszMultiExportGrammar[] =
      "[<Start>]\n"
      "<Start>=<Start1>\n"
      "<Start>=<Start2>\n"
      "[<Start1>]\n"
      "<Start1>=hello world\n"
      "<Start1>=this is a test\n"
      "[<Start2>]\n"
      "<Start2>=hello world\n"
      "<Start2>=goodbye\n"
      ;
char gszWordSpotGrammar[] =
      "[<Start>]\n"
      "<Start>= [0+] ... <keyword> [0+] ...\n"
      "<keyword>= long distance\n"
      "<keyword>= collect\n"
      "<keyword>= person to person\n"
      "<Start>=   ... Arizona\n"
      "<keyword>= Nebraska\n"
      "<keyword>= New York\n"
      "<Start>= Mister Smith ...\n"
      "<Start>= Misses Jones [0+] ...\n"
      "<keyword>= send mail ... Mister Jones\n"
      ;

char gszListGrammar[] =
      "[<Start>]\n"
      "<Start>=call <List1>\n"
      "<Start>=purchase <List2>\n"
      "<Start>=please look for the <List3>\n"
      "[Lists]\n"
      "=list1\n"
      "=list2\n"
      "=list3\n"
      "[List1]\n"
      "=Mark\n"
      "=George\n"
      "[List2]\n"
      "=a hat\n"
      "=32 hats\n"
      "=3 2 hats\n"
      "=some clothes\n"
      "=a pepperoni pizza\n"
      // list3 purposely left blank
      ;
     
char gszPronGrammar[] =
      "[<Start>]\n"
      "<Start>=#\\pound-sign\n"
      "<Start>=$$$\\dollar-sign\n"
      "<Start>=a\\hello-there\n"
      ;

char gszAllWorkGrammar[] =
      "[<Start>]\n"
      "<Start>=all work and no play makes jack a dull boy\n"
      ;
char gszAllWork2Grammar[] =
      "[<Start>]\n"
      "<Start>=[1+] <word>\n"
      "<word>=all\n"
      "<word>=work\n"
      "<word>=and\n"
      "<word>=no\n"
      "<word>=play\n"
      "<word>=makes\n"
      "<word>=jack\n"
      "<word>=a\n"
      "<word>=dull\n"
      "<word>=boy\n"
      ;
char gszAllWork[] = "all work and no play makes jack a dull boy";

char gszDictationGrammar[] =
      "[Grammar]\n"
      "type=dictation\n";

char gszDictationChunkGrammar[] =
      "[Grammar]\n"
      "type=dictation\n"
      "[Words]\n"
      "=hello\n"
      "=there\n"
      "[Topics]\n"
      "=E-mail\n"
      "=Legal\n"
      "[WordGroups]\n"
      "=Cities\n"
      "=States\n"
      "[Cities]\n"
      "=New York City\n"
      "=Boston\n"
      "[States]\n"
      "=New York\n"
      "=Washington\n"
      ;

char gszLimitedDomainGrammar[] =
      "[Grammar]\n"
      "type=limited-domain\n"
      "[Words]\n"
      "=hello\n"
      "=there\n"
      ;

char gszWordOrder1Grammar[] =
      "[<Start>]\n"
      "<Start>=this\n"
      "<Start>=is\n"
      "<Start>=a\n"
      "<Start>=test\n"
      "<Start>=this is a test\n"
      ;

char gszWordOrder2Grammar[] =
      "[<Start>]\n"
      "<Start>=is\n"
      "<Start>=this\n"
      "<Start>=a\n"
      "<Start>=test\n"
      "<Start>=this is a test\n"
      ;



// show information about the mode
void CALLBACK Test000 (void)
{
   SRMODEINFOW mi;
   HRESULT  hRes;
   char     szTemp[256];
   WCHAR    szwTemp[256];

   hRes = gpSRMode->ModeGet (&mi);
   if (hRes) {
      Failed ("ModeGet", hRes);
      return;
   }

   // print out statistics ar SRMode
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

   Information ("Sequencing: ", FALSE);
   switch (mi.dwSequencing) {
   case SRSEQUENCE_DISCRETE:
      Information ("Discrete");
      break;
   case SRSEQUENCE_CONTINUOUS:
      Information ("Continuous");
      break;
   case SRSEQUENCE_WORDSPOT:
      Information ("Word Spotting Only");
      break;
   case SRSEQUENCE_CONTCFGDISCDICT:
      Information ("Continuous CFG, Discrete dictation");
      break;
   }

   wsprintf (szTemp, "Max vocabulary words: %d", (int) mi.dwMaxWordsVocab);
   Information (szTemp);

   wsprintf (szTemp, "Max word states: %d", (int) mi.dwMaxWordsState);
   Information (szTemp);

   Information ("Grammars supported:", FALSE);
   if (mi.dwGrammars & SRGRAM_CFG)
      Information (" CFG", FALSE);
   if (mi.dwGrammars & SRGRAM_DICTATION)
      Information (" Dictation", FALSE);
   if (mi.dwGrammars & SRGRAM_LIMITEDDOMAIN)
      Information (" Limited-Domain", FALSE);
   Information ();

   Information ("Features:");
   if (mi.dwFeatures & SRFEATURE_INDEPSPEAKER)
      Information ("\tSpeaker Independent");
   if (mi.dwFeatures & SRFEATURE_INDEPMICROPHONE)
      Information ("\tMicrophone Independent");
   if (mi.dwFeatures & SRFEATURE_TRAINWORD)
      Information ("\tTraining is word based");
   if (mi.dwFeatures & SRFEATURE_TRAINPHONETIC)
      Information ("\tTraining is phoneme based");
   if (mi.dwFeatures & SRFEATURE_WILDCARD)
      Information ("\tSupport garbage word");
   if (mi.dwFeatures & SRFEATURE_ANYWORD)
      Information ("\tCan recognize any word (letter-to-sound rules)");
   if (mi.dwFeatures & SRFEATURE_PCOPTIMIZED)
      Information ("\tOptimized for the PC");
   if (mi.dwFeatures & SRFEATURE_PHONEOPTIMIZED)
      Information ("\tOptimized for the phone");
   if (mi.dwFeatures & SRFEATURE_GRAMLIST)
      Information ("\tSupports lists in grammars");
   if (mi.dwFeatures & SRFEATURE_GRAMLINK)
      Information ("\tSupports links in grammars");
   if (mi.dwFeatures & SRFEATURE_MULTILINGUAL)
      Information ("\tCan recognize more than one language in the mode");
   if (mi.dwFeatures & SRFEATURE_GRAMRECURSIVE)
      Information ("\tSupport left recursive grammars");
   if (mi.dwFeatures & SRFEATURE_IPAUNICODE)
      Information ("\tSupports IPA pronunciations");
   if (mi.dwFeatures & SRFEATURE_SINGLEINSTANCE)
      Information ("\tOnly supports one instance");
   if (mi.dwFeatures & SRFEATURE_THREADSAFE)
      Information ("\tThread safe from all threads");
   if (mi.dwFeatures & SRFEATURE_FIXEDAUDIO)
      Information ("\tDoesn't get audio from the audio source");
   if (mi.dwFeatures & SRFEATURE_IPAWORD)
      Information ("\tCan specify custom pronunciations for CFGs or words");
   if (mi.dwFeatures & SRFEATURE_SAPI4)
      Information ("\tSAPI4 compliant");


   Information ("Interfaces:");
   if (mi.dwInterfaces & SRI_ILEXPRONOUNCE)
      Information ("\tILexPronounce");
   if (mi.dwInterfaces & SRI_ISRATTRIBUTES)
      Information ("\tISRAttributes");
   if (mi.dwInterfaces & SRI_ISRCENTRAL)
      Information ("\tISRCentral");
   if (mi.dwInterfaces & SRI_ISRDIALOGS)
      Information ("\tISRDialogs");
   if (mi.dwInterfaces & SRI_ISRDIALOGS2)
      Information ("\tISRDialogs2");
   if (mi.dwInterfaces & SRI_ISRGRAMCOMMON)
      Information ("\tISRGramCommon");
   if (mi.dwInterfaces & SRI_ISRGRAMCFG)
      Information ("\tISRGramCFG");
   if (mi.dwInterfaces & SRI_ISRGRAMDICTATION)
      Information ("\tISRGramDictation");
   if (mi.dwInterfaces & SRI_ISRGRAMINSERTIONGUI)
      Information ("\tISRGramInsertionGUI");
   if (mi.dwInterfaces & SRI_ISRESBASIC)
      Information ("\tISRResBasic");
   if (mi.dwInterfaces & SRI_ISRESMERGE)
      Information ("\tISRResMerge");
   if (mi.dwInterfaces & SRI_ISRESAUDIO)
      Information ("\tISRResAudio");
   if (mi.dwInterfaces & SRI_ISRESCORRECTION)
      Information ("\tISRResCorrection");
   if (mi.dwInterfaces & SRI_ISRESEVAL)
      Information ("\tISRResEval");
   if (mi.dwInterfaces & SRI_ISRESGRAPH)
      Information ("\tISRResGraph");
   if (mi.dwInterfaces & SRI_ISRESMEMORY)
      Information ("\tISRResMemory");
   if (mi.dwInterfaces & SRI_ISRESMODIFYGUI)
      Information ("\tISRResModifyGUI");
   if (mi.dwInterfaces & SRI_ISRESSPEAKER)
      Information ("\tISRResSpeaker");
   if (mi.dwInterfaces & SRI_ISRSPEAKER)
      Information ("\tISRSpeaker");
   if (mi.dwInterfaces & SRI_ISRSPEAKER2)
      Information ("\tISRSpeaker2");
   if (mi.dwInterfaces & SRI_ISRESSCORES)
      Information ("\tISRResScores");
   if (mi.dwInterfaces & SRI_ISRESAUDIOEX)
      Information ("\tISRResAudioEx");
   if (mi.dwInterfaces & SRI_ISRGRAMLEXPRON)
      Information ("\tISRGramLexPron");
   if (mi.dwInterfaces & SRI_ISRRESGRAPHEX)
      Information ("\tISRResGraphEx");
   if (mi.dwInterfaces & SRI_ILEXPRONOUNCE2)
      Information ("\tILexPronounce2");
   if (mi.dwInterfaces & SRI_IATTRIBUTES)
      Information ("\tIAttributes");

   wsprintf (szTemp, "Engine features: %lx", (int) mi.dwEngineFeatures);
   Information (szTemp);


   // print out statistics about the wave format
   Information();
   Information ("Wave format information");
   wsprintf (szTemp, "Primary wave format: %d", (int) gpWFEXSR->wFormatTag);
   Information (szTemp);
   if (gpWFEXSR->wFormatTag == WAVE_FORMAT_PCM)
      Information ("\t(This is PCM data)");
   wsprintf (szTemp, "Channels: %d", (int) gpWFEXSR->nChannels);
   Information (szTemp);
   wsprintf (szTemp, "Samples per sec.: %d", (int) gpWFEXSR->nSamplesPerSec);
   Information (szTemp);
   wsprintf (szTemp, "Avg. bytes per sec.: %d", (int) gpWFEXSR->nAvgBytesPerSec);
   Information (szTemp);
   wsprintf (szTemp, "Block size: %d", (int) gpWFEXSR->nBlockAlign);
   Information (szTemp);
   wsprintf (szTemp, "Bits per sample: %d", (int) gpWFEXSR->wBitsPerSample);
   Information (szTemp);

   Passed ();


}

void CALLBACK Test001 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDCFG;
   NEEDAUDIO;
   NEEDENGLISH;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=hello world\n"
      );
   CHECKFAILED ("Load grammar");

   // see if we hear anything. We don't care if it's the right thing
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   
   ClearMainGrammar();
   Passed ();
}


//- Look at modeinfo and make sure all necessary parameters are filled in,
//and that the requisite interfaces are there
// - Make sure that the ansi interfaces are also supported

void CALLBACK Test002 (void)
{
   BOOL  fPassed = TRUE;

   // make sure teh guids are not null
   if (IsEqualGUID (gSRModeInfo.gEngineID, GUID_NULL)) {
      fPassed = FALSE;
      Failed ("SRModeInfo.gEngineID guid is NULL");
   }

   if (IsEqualGUID (gSRModeInfo.gModeID, GUID_NULL)) {
      fPassed = FALSE;
      Failed ("SRModeInfo.gModeID guid is NULL");
   }


   // make sure the strings have data
   if (!gSRModeInfo.szMfgName[0]) {
      fPassed = FALSE;
      Failed ("SRModeInfo.szMfgName is empty");
   }
   if (!gSRModeInfo.szProductName[0]) {
      fPassed = FALSE;
      Failed ("SRModeInfo.szProductName is empty");
   }
   if (!gSRModeInfo.szModeName[0]) {
      fPassed = FALSE;
      Failed ("SRModeInfo.szModeName is empty");
   }

   // make sure there's a langauge
   if (!gSRModeInfo.language.LanguageID) {
      fPassed = FALSE;
      Failed ("SRModeInfo - Not language specified");
   }

   // make sure it specifies the sequencing
   switch (gSRModeInfo.dwSequencing) {
   case SRSEQUENCE_DISCRETE:
   case SRSEQUENCE_CONTINUOUS:
   case SRSEQUENCE_WORDSPOT:
   case SRSEQUENCE_CONTCFGDISCDICT:
      break;
   default:
      fPassed = FALSE;
      Failed ("SRModeInfo.dwSequencing - Invalid");
   }

   // max words
   if (!gSRModeInfo.dwMaxWordsVocab) {
      fPassed = FALSE;
      Failed ("SRModeInfo.dwMaxWordsVocab is 0");
   }
   if (!gSRModeInfo.dwMaxWordsState) {
      fPassed = FALSE;
      Failed ("SRModeInfo.dwMaxWordsState is 0");
   }

   // make sure it's got valid grammars
#define  SRGRAM_ALL     (SRGRAM_CFG | SRGRAM_DICTATION |SRGRAM_LIMITEDDOMAIN)
   if (!(gSRModeInfo.dwGrammars & SRGRAM_ALL)) {
      fPassed = FALSE;
      Failed ("SRModeInfo.dwGrammars - Doesn't support any grammar states");
   }
   if (!(gSRModeInfo.dwGrammars & SRGRAM_CFG))
      Warning ("SRModeInfo.dwGrammars - Doesn't support CFGs. Most applications use CFGs.");
   if (gSRModeInfo.dwGrammars & ~SRGRAM_ALL)
      Warning ("SRModeInfo.dwGrammars - Supporting unknown grammars");

#define SRFEATURE_ALL      0x0003ffff
   if (gSRModeInfo.dwFeatures & ~SRFEATURE_ALL)
      Warning ("SRModeInfo.dwFeatures - Supporting unknown features");
   if (!(gSRModeInfo.dwFeatures & (SRFEATURE_PCOPTIMIZED | SRFEATURE_PHONEOPTIMIZED)))
      Warning ("SRModeInfo.dwFeatures - Doesn't support PCOptimized or PhoneOptimized");
   if (gSRModeInfo.dwFeatures & SRFEATURE_FIXEDAUDIO)
      Warning ("SRModeInfo.dwFeatures - Engine had fixed audio, so some tests may not work.");
   if (!(gSRModeInfo.dwFeatures & SRFEATURE_GRAMLIST))
      Information ("SRModeInfo.dwFeatures - It's highly recommended that engines support lists.");
   if (!(gSRModeInfo.dwFeatures & SRFEATURE_WILDCARD))
      Information ("SRModeInfo.dwFeatures - It's highly recommended that engines support word spotting.");
   if (!(gSRModeInfo.dwFeatures & SRFEATURE_ANYWORD))
      Information ("SRModeInfo.dwFeatures - It's highly recommended that engines support speaking any word.");
   if (!(gSRModeInfo.dwFeatures & SRFEATURE_INDEPSPEAKER))
      Information ("SRModeInfo.dwFeatures - It's highly recommended that engines be speaker independent.");
   if (!(gSRModeInfo.dwFeatures & SRFEATURE_TRAINPHONETIC))
      Information ("SRModeInfo.dwFeatures - It's highly recommended that engines have phonetic training.");

   if (gSRModeInfo.dwFeatures & SRFEATURE_SINGLEINSTANCE)
      Failed ("\tSRModeInfo.dwFeatures - Only supports one instance.");

   // interfaces
#define SRI_ALL 0x03ffffff
   if (gSRModeInfo.dwInterfaces & ~SRI_ALL)
      Warning ("SRModeInfo.dwInterfaces - Supporting unknown interfaces");
   if (!(gSRModeInfo.dwInterfaces & SRI_ISRATTRIBUTES)) {
      fPassed = FALSE;
      Failed ("SRModeInfo.dwFeatures - Must support ISRAttributes");
   }
   if (!(gSRModeInfo.dwInterfaces & SRI_ISRCENTRAL)) {
      fPassed = FALSE;
      Failed ("SRModeInfo.dwFeatures - Must support ISRCentral");
   }
   if (!(gSRModeInfo.dwInterfaces & SRI_ISRDIALOGS)) {
      fPassed = FALSE;
      Failed ("SRModeInfo.dwFeatures - Must support ISRDialogs");
   }
   if (!(gSRModeInfo.dwInterfaces & SRI_ISRGRAMCOMMON)) {
      fPassed = FALSE;
      Failed ("SRModeInfo.dwFeatures - Must support ISRGramCommon");
   }
   if (gSRModeInfo.dwGrammars & SRGRAM_CFG) {
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRGRAMCFG)) {
         fPassed = FALSE;
         Failed ("SRModeInfo.dwFeatures - Must support ISRGramCFG");
      }
   }
   else {
      if ((gSRModeInfo.dwInterfaces & SRI_ISRGRAMCFG))
         Warning ("SRModeInfo.dwFeatures - Supporting ISRGramCFG, but doesn't support CFG");
   }
   if (gSRModeInfo.dwGrammars & SRGRAM_DICTATION) {
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRGRAMDICTATION)) {
         fPassed = FALSE;
         Failed ("SRModeInfo.dwFeatures - Must support ISRGramDictation");
      }
   }
   else {
      if ((gSRModeInfo.dwInterfaces & SRI_ISRGRAMDICTATION))
         Warning ("SRModeInfo.dwFeatures - Supporting ISRGramDictation, but doesn't support dictation");
   }

// results objects 
#define SRI_RESALL (SRI_ISRESBASIC | SRI_ISRESMERGE | SRI_ISRESAUDIO | SRI_ISRESCORRECTION | \
SRI_ISRESEVAL | SRI_ISRESGRAPH | SRI_ISRESMEMORY | SRI_ISRESMODIFYGUI | SRI_ISRESSPEAKER | \
SRI_ISRESSCORES | SRI_ISRESAUDIOEX | SRI_ISRRESGRAPHEX ) 

   if (gSRModeInfo.dwInterfaces & SRI_RESALL) {
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRESBASIC)) {
         fPassed = FALSE;
         Failed ("SRModeInfo.dwInterfaces - If supports results objects, must support ISRResBasic");
      }
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRESMERGE))
         Information ("SRModeInfo.dwInterfaces - Suggest support ISRResMerge");
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRESAUDIO) && !(gSRModeInfo.dwInterfaces & SRI_ISRESAUDIOEX))
         Information ("SRModeInfo.dwInterfaces - Suggest support ISRResAudio or ISRResAudioEx");
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRESCORRECTION))
         Information ("SRModeInfo.dwInterfaces - Suggest support ISRResCorrection");
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRESEVAL))
         Information ("SRModeInfo.dwInterfaces - Suggest support ISRResEval");
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRESMEMORY))
         Information ("SRModeInfo.dwInterfaces - Suggest support ISRResMemory");
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRESMODIFYGUI))
         Information ("SRModeInfo.dwInterfaces - Suggest support ISRResModifyGUI");
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRESSPEAKER))
         Information ("SRModeInfo.dwInterfaces - Suggest support ISRResSpeaker");
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRESSCORES))
         Information ("SRModeInfo.dwInterfaces - Suggest support ISRResScores");
      if (!(gSRModeInfo.dwInterfaces & SRI_ISRESGRAPH) && !(gSRModeInfo.dwInterfaces & SRI_ISRRESGRAPHEX))
         Information ("SRModeInfo.dwInterfaces - Suggest support ISRResGraph or SRResGraphEx");
   }
	else
		Information ("SRModeInfor.dwInterfaces - Suggest support results objects");

   if (fPassed)
      Passed();
}


// - Look through the interfaces claimed and make sure they exist
void CALLBACK Test003 (void)
{
   HRESULT hRes;
   LPUNKNOWN   pUnk;

   if (gSRModeInfo.dwInterfaces & SRI_ILEXPRONOUNCE) {
      if (!gpSRMode->m_pILexPronounce) 
         Failed ("Doesn't support ILexPronounce like it claims.");
 
      gpSRMode->m_pISRCentral->QueryInterface (IID_ILexPronounceA, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ILexPronounce though it claims to support the interface.");
   }
   else {
      if (gpSRMode->m_pILexPronounce)
         Warning ("Supports ILexPronounce even though SRMODEINFO claims otherwise.");
	  else 
		 Information("Doesn't support LexPronounce");
   }


   if (gSRModeInfo.dwInterfaces & SRI_ISRATTRIBUTES) {
      if (!gpSRMode->m_pISRAttributes) 
         Failed ("Doesn't support ISRAttributes like it claims.");
       
      gpSRMode->m_pISRCentral->QueryInterface (IID_ISRAttributesA, (LPVOID*) &pUnk);
     if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ISRAttributes.");
       
   }
   else {
      if (gpSRMode->m_pISRAttributes)
         Warning ("Supports ISRAttributes even though SRMODEINFO claims otherwise.");
   }

   if (gSRModeInfo.dwInterfaces & SRI_ISRCENTRAL) {
      if (!gpSRMode->m_pISRCentral) 
         Failed ("Doesn't support ISRCentral like it claims.");
         
      gpSRMode->m_pISRCentral->QueryInterface (IID_ISRCentralA, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ISRCentral.");
         
   }
   else {
      if (gpSRMode->m_pISRCentral)
         Warning ("Supports ISRCentral even though SRMODEINFO claims otherwise.");
   }

   if (gSRModeInfo.dwInterfaces & SRI_ISRDIALOGS) {
      if (!gpSRMode->m_pISRDialogs) 
         Failed ("Doesn't support ISRDialogs like it claims.");
         
      gpSRMode->m_pISRCentral->QueryInterface (IID_ISRDialogsA, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ISRDialogs.");
         
   }
   else {
      if (gpSRMode->m_pISRDialogs)
         Warning ("Supports ISRDialogs even though SRMODEINFO claims otherwise.");
   }

   if (gSRModeInfo.dwInterfaces & SRI_ISRDIALOGS2) {
      if (!gpSRMode->m_pISRDialogs2) 
         Failed ("Doesn't support ISRDialogs2 like it claims.");
         
      gpSRMode->m_pISRCentral->QueryInterface (IID_ISRDialogs2A, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ISRDialogs2.");
         
   }
   else {
      if (gpSRMode->m_pISRDialogs2)
         Warning ("Supports ISRDialogs2 even though SRMODEINFO claims otherwise.");
   }

   if (gSRModeInfo.dwInterfaces & SRI_ISRSPEAKER) {
      if (!gpSRMode->m_pISRSpeaker) 
         Failed ("Doesn't support ISRSpeaker like it claims.");
         
      gpSRMode->m_pISRCentral->QueryInterface (IID_ISRSpeakerA, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ISRSpeaker.");
         
   }
   else {
      if (gpSRMode->m_pISRSpeaker)
         Warning ("Supports ISRSpeaker even though SRMODEINFO claims otherwise.");
   }

   if (gSRModeInfo.dwInterfaces & SRI_ISRSPEAKER2) {
      if (!gpSRMode->m_pISRSpeaker2) 
         Failed ("Doesn't support ISRSpeaker2 like it claims.");
         
      gpSRMode->m_pISRCentral->QueryInterface (IID_ISRSpeaker2A, (LPVOID*) &pUnk);
      if (pUnk)
         pUnk->Release();
      else 
         Failed ("Doesn't support ANSI version of ISRSpeaker2.");
         
   }
   else {
      if (gpSRMode->m_pISRSpeaker2)
         Warning ("Supports ISRSpeaker2 even though SRMODEINFO claims otherwise.");
   }

   // load a grammar in so we can test if grammars support what they claim
   NEEDCFG;
   NEEDAUDIO;
   NEEDENGLISH;

   if (gSRModeInfo.dwGrammars & SRGRAM_CFG) {
      // See if SR hears it
      hRes = SetMainGrammar (
         "[<Start>]\n"
         "<Start>=hello world\n"
         );
      CHECKFAILED ("Load grammar");

      if (gSRModeInfo.dwInterfaces & SRI_ISRGRAMCOMMON) {
         if (!gpSRGramComp->m_pISRGramCommon) 
            Failed ("Doesn't support ISRGramCommon like it claims.");
            
         gpSRGramComp->m_pISRGramCommon->QueryInterface (IID_ISRGramCommonA, (LPVOID*) &pUnk);
         if (pUnk)
            pUnk->Release();
         else 
            Failed ("Doesn't support ANSI version of ISRGramCommon.");
      }
      else {
         if (gpSRGramComp->m_pISRGramCommon)
            Warning ("Supports ISRGramCommon even though SRMODEINFO claims otherwise.");
      }

		if (gSRModeInfo.dwInterfaces & SRI_ILEXPRONOUNCE2) {
			if (!gpSRMode->m_pILexPronounce2) {
				// check if supported off grammer object
				gpSRGramComp->m_pISRGramCommon->QueryInterface (IID_ILexPronounce2W, (LPVOID*) &pUnk);
				if (pUnk)
					pUnk->Release();
				else 
					Failed ("Doesn't support UNICODE version of ILexPronouce2");
			}

			gpSRMode->m_pISRCentral->QueryInterface (IID_ILexPronounce2A, (LPVOID*) &pUnk);
			if (pUnk)
				pUnk->Release();
			else {
				// check if supported off grammer object
				gpSRGramComp->m_pISRGramCommon->QueryInterface (IID_ILexPronounce2A, (LPVOID*) &pUnk);
				if (pUnk)
					pUnk->Release();
				else 
					Failed ("Doesn't support ANSI version of ILexPronounce2");
			}
		}
		else {
			if (gpSRMode->m_pILexPronounce2)
				Warning ("Supports ILexPronounce2 even though SRMODEINFO claims otherwise.");
			else 
				Information ("Doesn't support LexPronounce2");
		}

		if (gSRModeInfo.dwInterfaces & SRI_ISRGRAMCFG) {
         if (!gpSRGramComp->m_pISRGramCFG) 
            Failed ("Doesn't support ISRGramCFG like it claims.");

		 gpSRGramComp->m_pISRGramCommon->QueryInterface (IID_ISRGramCFGA, (LPVOID*) &pUnk);
         if (pUnk)
            pUnk->Release();
         else 
            Failed ("Doesn't support ANSI version of ISRGramCFG.");
      }
      else {
         if (gpSRGramComp->m_pISRGramCFG)
            Warning ("Supports ISRGramCFG even though SRMODEINFO claims otherwise.");
      }


      if (gSRModeInfo.dwInterfaces & SRI_ISRGRAMINSERTIONGUI) {
         if (!gpSRGramComp->m_pISRGramInsertionGUI) 
            Failed ("Doesn't support ISRGramInsertionGUI like it claims.");
      }
      else {
         if (gpSRGramComp->m_pISRGramInsertionGUI)
            Warning ("Supports ISRGramInsertionGUI even though SRMODEINFO claims otherwise.");
      }


      ClearMainGrammar();
   }

   // Dictation
   if (gSRModeInfo.dwGrammars & SRGRAM_DICTATION) {
      // See if SR hears it
      hRes = SetMainGrammar (
         "[Grammar]\n"
         "type=dictation\n"
         );
      CHECKFAILED ("Load grammar");

      if (gSRModeInfo.dwInterfaces & SRI_ISRGRAMCOMMON) {
         if (!gpSRGramComp->m_pISRGramCommon) 
            Failed ("Doesn't support ISRGramCommon like it claims.");

         gpSRGramComp->m_pISRGramCommon->QueryInterface (IID_ISRGramCommonA, (LPVOID*) &pUnk);
         if (pUnk)
            pUnk->Release();
         else 
            Failed ("Doesn't support ANSI version of ISRGramCommon.");
      }
      else {
         if (gpSRGramComp->m_pISRGramCommon)
            Warning ("Supports ISRGramCommon even though SRMODEINFO claims otherwise.");
      }

      if (gSRModeInfo.dwInterfaces & SRI_ISRGRAMDICTATION) {
         if (!gpSRGramComp->m_pISRGramDictation) 
            Failed ("Doesn't support ISRGramDictation like it claims.");

         gpSRGramComp->m_pISRGramCommon->QueryInterface (IID_ISRGramDictationA, (LPVOID*) &pUnk);
         if (pUnk)
            pUnk->Release();
         else 
            Failed ("Doesn't support ANSI version of ISRGramDictation.");
      }
      else {
         if (gpSRGramComp->m_pISRGramDictation)
            Warning ("Supports ISRGramDictation even though SRMODEINFO claims otherwise.");
      }


      if (gSRModeInfo.dwInterfaces & SRI_ISRGRAMINSERTIONGUI) {
         if (!gpSRGramComp->m_pISRGramInsertionGUI) 
            Failed ("Doesn't support ISRGramInsertionGUI like it claims.");
      }
      else {
         if (gpSRGramComp->m_pISRGramInsertionGUI)
            Warning ("Supports ISRGramInsertionGUI even though SRMODEINFO claims otherwise.");
      }


      ClearMainGrammar();
   }

   DWORD i;
   for (i = 0; i < 2; i++) {
      switch (i) {
      case 0:
         if  (gSRModeInfo.dwGrammars & SRGRAM_DICTATION)
            hRes = SetMainGrammar (
               "[Grammar]\n"
               "type=dictation\n"
               );
         else
            continue;
      case 1:
         if (gSRModeInfo.dwGrammars & SRGRAM_CFG)
            hRes = SetMainGrammar (
               "[<Start>]\n"
               "<Start>=hello world\n"
               );
         else
            continue;
      }

      CHECKFAILED ("Load grammar");

      // speak Hello worlds to speech recognition
      hRes = SpeakToSR ("Hello world");
      CHECKFAILED ("SpeakToSR");

      // see if we hear anything. We don't care if it's the right thing
      hRes = WaitForPhraseFinish();
      CHECKFAILED ("WaitForPhraseFinish");

      // Capture results object & test it out
      if (gpSRResult) {
         if (gSRModeInfo.dwInterfaces & SRI_ISRESBASIC) {
            if (!gpSRResult->m_pISRResBasic) 
               Failed ("Doesn't support ISRResBasic like it claims.");

            gpSRResult->m_pISRResBasic->QueryInterface (IID_ISRResBasicA, (LPVOID*) &pUnk);
            if (pUnk)
               pUnk->Release();
            else 
               Failed ("Doesn't support ANSI version of ISRResBasic.");
         }
         else {
            if (gpSRResult->m_pISRResBasic)
               Warning ("Supports ISRResBasic even though SRMODEINFO claims otherwise.");
         }

         if (gSRModeInfo.dwInterfaces & SRI_ISRESMERGE) {
            if (!gpSRResult->m_pISRResMerge) 
               Failed ("Doesn't support ISRResMerge like it claims.");
         }
         else {
            if (gpSRResult->m_pISRResMerge)
               Warning ("Supports ISRResMerge even though SRMODEINFO claims otherwise.");
         }

         if (gSRModeInfo.dwInterfaces & SRI_ISRESAUDIO) {
            if (!gpSRResult->m_pISRResAudio) 
               Failed ("Doesn't support ISRResAudio like it claims.");
         }
         else {
            if (gpSRResult->m_pISRResAudio)
               Warning ("Supports ISRResAudio even though SRMODEINFO claims otherwise.");
         }

         // don't require isrrescorrection for cfgs if the engine
         // has specified cfgs, because SAPI doesn't require this
         BOOL fCanWarn = ((i == 1) && (gSRModeInfo.dwGrammars & SRGRAM_DICTATION));

         if (gSRModeInfo.dwInterfaces & SRI_ISRESCORRECTION) {
            if (!gpSRResult->m_pISRResCorrection) {
               if (!fCanWarn)
                  Failed ("Doesn't support ISRResCorrection like it claims.");
	         }
				else {
					Information("Doesn't support ISRResCorrection interfaces for CFG grammars");
					gpSRResult->m_pISRResCorrection->QueryInterface (IID_ISRResCorrectionA, (LPVOID*) &pUnk);
					if (pUnk)
						pUnk->Release();
					else {
						if (!fCanWarn)
							Failed ("Doesn't support ANSI version of ISRResCorrection.");
					}
				}
         }
         else {
            if (gpSRResult->m_pISRResCorrection)
               Warning ("Supports ISRResCorrection even though SRMODEINFO claims otherwise.");
         }

         if (gSRModeInfo.dwInterfaces & SRI_ISRESEVAL) {
            if (!gpSRResult->m_pISRResEval) 
               Failed ("Doesn't support ISRResEval like it claims.");
         }
         else {
            if (gpSRResult->m_pISRResEval)
               Warning ("Supports ISRResEval even though SRMODEINFO claims otherwise.");
         }

         if (gSRModeInfo.dwInterfaces & SRI_ISRESGRAPH) {
            if (!gpSRResult->m_pISRResGraph) 
               Failed ("Doesn't support ISRResGraph like it claims.");

            gpSRResult->m_pISRResGraph->QueryInterface (IID_ISRResGraphA, (LPVOID*) &pUnk);
            if (pUnk)
               pUnk->Release();
            else 
               Failed ("Doesn't support ANSI version of ISRResGraph.");
         }
         else {
            if (gpSRResult->m_pISRResGraph)
               Warning ("Supports ISRResGraph even though SRMODEINFO claims otherwise.");
         }

         if (gSRModeInfo.dwInterfaces & SRI_ISRESMEMORY) {
            if (!gpSRResult->m_pISRResMemory) 
               Failed ("Doesn't support ISRResMemory like it claims.");
         }
         else {
            if (gpSRResult->m_pISRResMemory)
               Warning ("Supports ISRResMemory even though SRMODEINFO claims otherwise.");
         }

         if (gSRModeInfo.dwInterfaces & SRI_ISRESMODIFYGUI) {
            if (!gpSRResult->m_pISRResModifyGUI) 
               Failed ("Doesn't support ISRResModifyGUI like it claims.");
         }
         else {
            if (gpSRResult->m_pISRResModifyGUI)
               Warning ("Supports ISRResModifyGUI even though SRMODEINFO claims otherwise.");
         }

         if (gSRModeInfo.dwInterfaces & SRI_ISRESSPEAKER) {
            if (!gpSRResult->m_pISRResSpeaker) 
               Failed ("Doesn't support ISRResSpeaker like it claims.");

		    gpSRResult->m_pISRResSpeaker->QueryInterface (IID_ISRResSpeakerA, (LPVOID*) &pUnk);
            if (pUnk)
               pUnk->Release();
            else 
               Failed ("Doesn't support ANSI version of ISRResSpeaker.");
         }
         else {
            if (gpSRResult->m_pISRResSpeaker)
               Warning ("Supports ISRResSpeaker even though SRMODEINFO claims otherwise.");
         }

         if (gSRModeInfo.dwInterfaces & SRI_ISRESSCORES) {
            if (!gpSRResult->m_pISRResScores) 
               Failed ("Doesn't support ISRResScores like it claims.");

		    gpSRResult->m_pISRResScores->QueryInterface (IID_ISRResScoresA, (LPVOID*) &pUnk);
            if (pUnk)
               pUnk->Release();
            else 
               Failed ("Doesn't support ANSI version of ISRResScores.");
         }
         else {
            if (gpSRResult->m_pISRResScores)
               Warning ("Supports ISRResScores even though SRMODEINFO claims otherwise.");
         }

         if (gSRModeInfo.dwInterfaces & SRI_ISRESAUDIOEX) {
            if (!gpSRResult->m_pISRResAudioEx) 
               Failed ("Doesn't support ISRResAudioEx like it claims.");
			   // note: there is no ANSI version of IResGraphEx specified
         }
         else {
            if (gpSRResult->m_pISRResAudioEx)
               Warning ("Supports ISRResAudioEx even though SRMODEINFO claims otherwise.");
         }
      }
      else {
         // no result object
         if (gSRModeInfo.dwInterfaces & SRI_RESALL) {
            Warning ("SRMODEINFO claims to support results objects, but unable to get a results object.");
         }
      }

      ClearMainGrammar();
      }

	Passed();
}



//- If claim SRModeHas SRFEATURE_WORD or SRFEATURE_TRAINPHONETIC,
//test to make sure the dialog is supported. And, if dialog supported,
//make sure has one of features claimed.

void CALLBACK Test004 (void)
{
   BOOL  fPassed = TRUE;
   HRESULT hRes;

   BOOL  fTrain, fDlg;
   fTrain = (gSRModeInfo.dwFeatures & (SRFEATURE_TRAINWORD | SRFEATURE_TRAINPHONETIC)) ? TRUE : FALSE;
   hRes = gpSRMode->TrainGeneralDlg (NULL);
   fDlg = hRes ? FALSE : TRUE;

   if (fTrain && !fDlg)
      Failed ("SRMODEINFO claims training supported, but it doesn't seem to be.");
   else if (!fTrain && fDlg)
      Warning ("SRMODEINFO claims not to support training, but it seems to be supported.");
   else
      Passed();
}

// - If have SRFEATURE_WILDCARD, make sure can load wildcard, and vice
// versa.

void CALLBACK Test005 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDCFG;
   NEEDAUDIO;
   NEEDENGLISH;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("say good morning now.");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=... good morning ...\n"
      );
   if (hRes) {
      // if we're not supposed to support wildcards then no big deal
      if (!(gSRModeInfo.dwFeatures & SRFEATURE_WILDCARD)) {
         NotApplicable();
         return;
      }
   }
   else {
      if (!(gSRModeInfo.dwFeatures & SRFEATURE_WILDCARD))
         Warning ("SRMODEINFO claims that word spotting is not supported, but it GrammarLoad doesn't complain.");
   }
   CHECKFAILED ("Load grammar");

   // see if we hear anything. We don't care if it's the right thing
   hRes = WaitForPhraseFinish();
   if (hRes) {
      // if we're not supposed to support wildcards then no big deal
      if (!(gSRModeInfo.dwFeatures & SRFEATURE_WILDCARD)) {
         NotApplicable();
         return;
      }
   }
   CHECKFAILED ("WaitForPhraseFinish");
   
   ClearMainGrammar();

   // see if we got a recognition result
   if (gSRGramNS.m_szPhraseFinishText[0]) {
      // got result
      if (gSRModeInfo.dwFeatures & SRFEATURE_WILDCARD)
         Passed();
      else
         Warning ("SRMODEINFO claims that word spotting is not supported, but it seems to work.");

   }
   else {
      // got no result
      if (gSRModeInfo.dwFeatures & SRFEATURE_WILDCARD)
         Warning ("Can't determine if word spotting works.");
      else
         Passed();

   }

}


// - If have SRFEATURE_ANYWORD, make sure can load & recognize anyword.
// And vice versa.

void CALLBACK Test006 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDCFG;
   NEEDAUDIO;
   NEEDENGLISH;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("gobble-gooble");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=gobble-gooble\n"
      );
   if (hRes) {
      // if we're not supposed to support wildcards then no big deal
      if (!(gSRModeInfo.dwFeatures & SRFEATURE_ANYWORD)) {
         NotApplicable();
         return;
      }
   }
   else {
      if (!(gSRModeInfo.dwFeatures & SRFEATURE_ANYWORD))
         Warning ("SRMODEINFO claims that recognition of any word is not supported, but it GrammarLoad doesn't complain.");
   }
   CHECKFAILED ("Load grammar");

   // see if we hear anything. We don't care if it's the right thing
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   
   ClearMainGrammar();

   // see if we got a recognition result
   if (gSRGramNS.m_szPhraseFinishText[0]) {
      // got result
      if (gSRModeInfo.dwFeatures & SRFEATURE_ANYWORD)
         Passed();
      else
         Warning ("SRMODEINFO claims that recognition of any word is not supported, but it seems to work.");

   }
   else {
      // got no result
      if (gSRModeInfo.dwFeatures & SRFEATURE_ANYWORD)
         Warning ("Can't determine if recognition of any-word works.");
      else
         Passed();

   }

}


// - If have PHONEOPTIMIZED, make sure default rate is 8 kHz 16-bit

void CALLBACK Test007 (void)
{
   if ((gSRModeInfo.dwFeatures & SRFEATURE_PHONEOPTIMIZED) &&
      !(gSRModeInfo.dwFeatures & SRFEATURE_PCOPTIMIZED))
   {
      // phone optimized by defaukt
   if ((gpWFEXSR->wFormatTag != WAVE_FORMAT_PCM) ||
      (gpWFEXSR->nChannels != 1) ||
      (gpWFEXSR->nSamplesPerSec != 8000) ||
      (gpWFEXSR->nAvgBytesPerSec != 16000) ||
      (gpWFEXSR->nBlockAlign != 2) ||
      (gpWFEXSR->wBitsPerSample != 16))
      Failed ("Telephone-optimized SR engines must support 16-bit 8 kHz mono.");
   else
      Passed();
   }
   else {
      // not phone optimized, so not applicable
      NotApplicable();
   }
}



// - If have SRFEATURE_GRAMLIST, make sure list can be set and recognized,
//   and vice versa
void CALLBACK Test008 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDCFG;
   NEEDAUDIO;
   NEEDENGLISH;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("My name is Fred.");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=my name is <ListName>\n"
      "[Lists]\n"
      "=ListName\n"
      "[ListName]\n"
      "=Fred\n"
      "=Patrick\n"
      );
   if (hRes) {
      // if we're not supposed to support wildcards then no big deal
      if (!(gSRModeInfo.dwFeatures & SRFEATURE_GRAMLIST)) {
         NotApplicable();
         return;
      }
   }
   else {
      if (!(gSRModeInfo.dwFeatures & SRFEATURE_GRAMLIST))
         Warning ("SRMODEINFO claims that lists are not supported, but it GrammarLoad doesn't complain.");
   }
   CHECKFAILED ("Load grammar");

   // see if we hear anything. We don't care if it's the right thing
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   
   ClearMainGrammar();

   // see if we got a recognition result
   if (!stricmp(gSRGramNS.m_szPhraseFinishText, "my name is Fred")) {
      // got result
      if (gSRModeInfo.dwFeatures & SRFEATURE_GRAMLIST)
         Passed();
      else
         Warning ("SRMODEINFO claims that lists are not supported, but it seems to work.");

   }
   else {
      // got no result
      if (gSRModeInfo.dwFeatures & SRFEATURE_GRAMLIST)
         Warning ("Can't determine if lists work.");
      else
         Passed();

   }

}


// - Make sure the enumerator works - can be re-enumerated and get the same results,
// along with skip, etc.
void CALLBACK Test009 (void)
{
   SRMODEINFOW ami[200];  // large number of mode infos
   DWORD       dwModes;
   HRESULT     hRes;

   gpSREnum->Reset();
   dwModes = 0;
   memset (ami, 0, sizeof(ami));
   gpSREnum->Next(200, ami, &dwModes);
   
   // try stepping through individually and comparing, skipping
   // occasionally
   DWORD i;
   CSREnum *pNew;
   pNew = gpSREnum->Clone();
   if (!pNew) {
      Failed ("Can't clone enumerator");
      return;
   }
   pNew->Reset();
   for (i = 0; i < dwModes; i++) {
      if (rand() %2) {
         // skip
         pNew->Skip(1);
      }
      else {
         // get and compare
         SRMODEINFOW mi;
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
            (mi.dwSequencing != ami[i].dwSequencing) ||
            (mi.dwMaxWordsVocab != ami[i].dwMaxWordsVocab) ||
            (mi.dwMaxWordsState != ami[i].dwMaxWordsState) ||
            (mi.dwGrammars != ami[i].dwGrammars) ||
            (mi.dwFeatures != ami[i].dwFeatures) ||
            (mi.dwInterfaces != ami[i].dwInterfaces) ||
            (mi.dwEngineFeatures != ami[i].dwEngineFeatures)
            )
            Failed ("Modes are different");

      }
   }

   if (pNew)
      delete pNew;

   Passed();
}



// - Close & reload the engine
void CALLBACK Test010 (void)
{
   HRESULT  hRes;
   hRes = DestroySREngine();
   CHECKFAILED ("DestroySREngine");

   hRes = CreateSREngine();
   CHECKFAILED ("CreateSREngine");

   Passed();
}


// - Close & releoad the engine while listening/talking
void CALLBACK Test011 (void)
{
   HRESULT     hRes;

   // checks to make sure the engine claims to support this
   NEEDCFG;
   NEEDAUDIO;
   NEEDENGLISH;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("This is a test of a sentence for shut-down.");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=this is a test of a sentence for shut-down\n"
      );
   CHECKFAILED ("SetMainGrammar");


   // see if we hear anything. We don't care if it's the right thing
   BOOL  *pf;

   // activate the grammar
   hRes = gpSRGramComp->Activate();
   if (hRes) {
      Failed ("Activate", hRes);
      return;
   }

   // wait for the event
   pf = WaitForEvent (60.0, &gSRNS.m_fUtteranceBegin);
   if (pf == NULL) {
      Failed ("Never recognized utterance");
      return;
   }
   else if (pf != &gSRNS.m_fUtteranceBegin)
      return;

   // else, it got an utterance begin, so quit right now

   // clear the audio
   ClearSRAudio();

   hRes = DestroySREngine();
   CHECKFAILED ("DestroySREngine");

   hRes = CreateSREngine();
   CHECKFAILED ("CreateSREngine");

   Passed ();
}


// - Reference counting on the audio source/destination object
void CALLBACK Test012 (void)
{
   HRESULT     hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;

   // Pull the audio object out of the clutches of the SR engine
   // We'll AddRef it, so it should have a reference count of
   // 2 after destroy engine is called. One from this, and one from
   // the C++ wrapper.  The Reference Count flow is:
   //    CoCreateInstance():        1
   //    InitAudioSourceObject():   2
   //    SR Engine:                 X
   //    Test012 AddRef():          X + 1
   //    DestroySREngine():           
   //       SR Engine:              3
   //       Release:                2
   //    Test012 Release()          1* This is the value of dwRef
   //    Test012 Release()          0

   LPUNKNOWN   pUnk;
   pUnk = gpSRMode->m_pUnkAudio;
   pUnk->AddRef();
   gpSRMode->m_pUnkAudio = NULL;

   hRes = DestroySREngine();
   CHECKFAILED ("DestroySREngine");

   // check the refcount
   DWORD dwRef;
   dwRef = pUnk->Release();
   while (pUnk->Release());   // release until

   hRes = CreateSREngine();
   CHECKFAILED ("CreateSREngine");

   if (dwRef != 1)
      Failed ("Audio object not completely released.");
   else
      Passed();
}

// - Reference counting on the main notification sinks, when register & unregister
void CALLBACK Test013 (void)
{
   HRESULT     hRes;

   // destroy the current srnotify sink
   hRes = gpSRMode->UnRegister (gdwSRNSKey);
   CHECKFAILED ("UnRegister");

   // set the ref count flag
   gSRNS.m_dwRefCount = 0;

   // regsiter
   hRes = gpSRMode->Register (&gSRNS, &gdwSRNSKey);
   CHECKFAILED ("Register");

   // make sure that the ref count incremented
   if (!gSRNS.m_dwRefCount)
      Failed ("ISRCentral::Register didn't increase the reference count");

   // unregister
   hRes = gpSRMode->UnRegister (gdwSRNSKey);
   CHECKFAILED ("UnRegister");

   // make sure the ref count is 0
   if (gSRNS.m_dwRefCount)
      Failed ("ISRCentral::UnRegister didn't release all the references");

   // reregsiter
   hRes = gpSRMode->Register (&gSRNS, &gdwSRNSKey);
   CHECKFAILED ("Register");

   Passed();
}



// - Reference counting on the main notifiation sinks, when register & close down
void CALLBACK Test014 (void)
{
   HRESULT     hRes;

   // destroy the current srnotify sink
   hRes = gpSRMode->UnRegister (gdwSRNSKey);
   CHECKFAILED ("UnRegister");

   // set the ref count flag
   gSRNS.m_dwRefCount = 0;

   // regsiter
   hRes = gpSRMode->Register (&gSRNS, &gdwSRNSKey);
   CHECKFAILED ("Register");

   // make sure that the ref count incremented
   if (!gSRNS.m_dwRefCount)
      Failed ("ISRCentral::Register didn't increase the reference count");

   // destroy the engine
   hRes = DestroySREngine();
   CHECKFAILED ("DestroySREngine");

   // make sure the ref count is 0
   if (gSRNS.m_dwRefCount)
      Failed ("ISRCentral::UnRegister didn't release all the references");

   // create the engine
   hRes = CreateSREngine();
   CHECKFAILED ("CreateSREngine");

   Passed();
}


// - Reference counting on the grammar/buffer notification sinks, when start using them properly,
// and shut down after they should be released
void CALLBACK Test015 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDCFG;
   NEEDAUDIO;
   NEEDENGLISH;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // clear the main grammar
   ClearMainGrammar();

   // set the ref count to 0
   gSRGramNS.m_dwRefCount = 0;

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=hello world\n"
      );
   CHECKFAILED ("Load grammar");

   // make sure the ref count is no 0
   if (!gSRGramNS.m_dwRefCount)
      Failed ("ISRCentral::GrammarLoad didn't increase the reference count");

   // see if we hear anything. We don't care if it's the right thing
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   
   ClearMainGrammar();

   // make sure the ref count it 0
   if (gSRGramNS.m_dwRefCount)
      Failed ("Release gramamr object didn't reduce the reference count");

   Passed ();
}


// - Reference counting on the grammar/buffer notification sinks, when start using and
// then shut down while still using
void CALLBACK Test016 (void)
{
   HRESULT     hRes;

   // checks to make sure the engine claims to support this
   NEEDCFG;
   NEEDAUDIO;
   NEEDENGLISH;

   // clear the main grammar
   ClearMainGrammar();

   // set the ref count to 0
   gSRGramNS.m_dwRefCount = 0;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("This is a test of a sentence for shut-down.");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=this is a test of a sentence for shut-down\n"
      );
   CHECKFAILED ("SetMainGrammar");


   // make sure the ref count is no 0
   if (!gSRGramNS.m_dwRefCount)
      Failed ("ISRCentral::GrammarLoad didn't increase the reference count");

   // see if we hear anything. We don't care if it's the right thing
   BOOL  *pf;

   // activate the grammar
   hRes = gpSRGramComp->Activate();
   if (hRes) {
      Failed ("Activate", hRes);
      return;
   }

   // wait for the event
   pf = WaitForEvent (60.0, &gSRNS.m_fUtteranceBegin);
   if (pf == NULL) {
      Failed ("Never recognized utterance");
      return;
   }
   else if (pf != &gSRNS.m_fUtteranceBegin)
      return;

   // else, it got an utterance begin, so quit right now

   // clear the audio
   ClearSRAudio();

   hRes = DestroySREngine();
   CHECKFAILED ("DestroySREngine");

   // make sure the ref count it 0
   if (gSRGramNS.m_dwRefCount)
      Failed ("Release gramamr object didn't reduce the reference count");

   hRes = CreateSREngine();
   CHECKFAILED ("CreateSREngine");

   Passed ();
}

// - Make sure don't get notifications to main notification sink after it's been unregisterd
void CALLBACK Test017 (void)
{
   HRESULT     hRes;

   // checks to make sure the engine claims to support this
   NEEDCFG;
   NEEDAUDIO;
   NEEDENGLISH;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("This is a test of a sentence for shut-down.");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=this is a test of a sentence for shut-down\n"
      );
   CHECKFAILED ("SetMainGrammar");


   // see if we hear anything. We don't care if it's the right thing
   BOOL  *pf;

   // activate the grammar
   hRes = gpSRGramComp->Activate();
   if (hRes) {
      Failed ("Activate", hRes);
      return;
   }

   // wait for the event
   pf = WaitForEvent (60.0, &gSRNS.m_fUtteranceBegin);
   if (pf == NULL) {
      Failed ("Never recognized utterance");
      return;
   }
   else if (pf != &gSRNS.m_fUtteranceBegin)
      return;

   // else, it got an utterance begin

   // release the main notification sink
   hRes = gpSRMode->UnRegister (gdwSRNSKey);
   CHECKFAILED ("UnRegister");

   // wait until we get a phrase finish, or a notification comes into
   // the main notification sink
   pf = WaitForEvent (60.0, &gSRNS.m_fNotification, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gSRNS.m_fNotification) {
      // we shouldn't have gotten this
      Failed ("Got an ISRNotifySink notification after unregistered notification sink");
   }

   // shut down this grammar
   ClearSRAudio();
   ClearMainGrammar();

   // reregister
   hRes = gpSRMode->Register (&gSRNS, &gdwSRNSKey);
   CHECKFAILED ("Register");

   Passed ();
}


// - Make sure that don't get notifiations to the main notification sink after it's been
// shut down.
void CALLBACK Test018 (void)
{
   HRESULT     hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("This is a test of a sentence for shut-down.");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=this is a test of a sentence for shut-down\n"
      );
   CHECKFAILED ("SetMainGrammar");


   // see if we hear anything. We don't care if it's the right thing
   BOOL  *pf;

   // activate the grammar
   hRes = gpSRGramComp->Activate();
   if (hRes) {
      Failed ("Activate", hRes);
      return;
   }

   // wait for the event
   gSRNS.m_fUtteranceBegin = FALSE;
   pf = WaitForEvent (60.0, &gSRNS.m_fUtteranceBegin);
   if (pf == NULL) {
      Failed ("Never recognized utterance");
      return;
   }
   else if (pf != &gSRNS.m_fUtteranceBegin)
      return;

   // else, it got an utterance begin

   // destroy the sr engine
   hRes = DestroySREngine();
   CHECKFAILED ("DestroySREngine");


   // wait for a few seconds. We shouldn't get any callbacks
   pf = WaitForEvent (10.0, &gSRNS.m_fNotification, &gSRGramNS.m_fNotification);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gSRNS.m_fNotification) {
      // we shouldn't have gotten this
      Failed ("Got an ISRNotifySink notification after unregistered notification sink");
   }
   else if (pf == &gSRGramNS.m_fNotification) {
      // we shouldn't have gotten this
      Failed ("Got an ISRGramNotifySink notification after unregistered notification sink");
   }

   // start up again
   hRes = CreateSREngine();
   CHECKFAILED ("CreateSREngine");

   Passed ();
}

// - Make sure don't get notifications to the grammar/buffer notification sink after
// start using, let it end by closing grammar
void CALLBACK Test019 (void)
{
   HRESULT     hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("This is a test of a sentence for shut-down.");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=this is a test of a sentence for shut-down\n"
      );
   CHECKFAILED ("SetMainGrammar");


   // see if we hear anything. We don't care if it's the right thing
   BOOL  *pf;

   // activate the grammar
   hRes = gpSRGramComp->Activate();
   if (hRes) {
      Failed ("Activate", hRes);
      return;
   }

   // wait for the event
   pf = WaitForEvent (60.0, &gSRNS.m_fUtteranceBegin);
   if (pf == NULL) {
      Failed ("Never recognized utterance");
      return;
   }
   else if (pf != &gSRNS.m_fUtteranceBegin)
      return;

   // else, it got an utterance begin

   // release the grammar object
   ClearMainGrammar();

   // make sure no notifications come in
   pf = WaitForEvent (10.0, &gSRGramNS.m_fNotification);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gSRGramNS.m_fNotification) {
      // we shouldn't have gotten this
      Failed ("Got an ISRGramNotifySink notification after grammar was released");
   }

   // shut down this grammar
   ClearSRAudio();
   ClearMainGrammar();

   Passed ();
}



// - Make sure audio object is not started up until it's supposed to (activate or text data)
// - Make sure the audio object is stopped as soon as it's supposed to (deactivate, or reset,
// text-data done)

void CALLBACK Test020 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=hello world\n"
      );
   CHECKFAILED ("Load grammar");


   // make sure it's not listening yet
   BOOL  *pf;
   pf = WaitForEvent (10.0, &gSRWANS.m_fPosn);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gSRWANS.m_fPosn)
      Failed ("SR engine has audio device open even when not active");

   // activate the grammar
   hRes = gpSRGramComp->Activate();
   if (hRes) {
      Failed ("Activate", hRes);
      return;
   }

   // wait for the event
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;

   // deactivate the grammar
   hRes = gpSRGramComp->Deactivate();

   pf = WaitForEvent (10.0, &gSRWANS.m_fPosn);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gSRWANS.m_fPosn)
      Failed ("SR engine has audio device open even when not active");

   // clear the audio
   ClearSRAudio();

   ClearMainGrammar();
   Passed ();
}


// - Can have more than one main notification sink, and both get the message
void CALLBACK Test021 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // register a secong notification sink
   CFuncSRNotifySink nsnew;
   DWORD dwKey;
   hRes = gpSRMode->Register (&nsnew, &dwKey);
   CHECKFAILED ("Register");
   nsnew.m_fUtteranceBegin = nsnew.m_fUtteranceEnd = FALSE;
   gSRNS.m_fUtteranceBegin = gSRNS.m_fUtteranceEnd = FALSE;

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=hello world\n"
      );
   if (hRes)
   {
      Failed("Load grammar");
      goto Cleanup;
   }

   // see if we hear anything. We don't care if it's the right thing
   hRes = WaitForPhraseFinish();
   if (hRes)
   {
      Failed("WaitForPhraseFinish");
      goto Cleanup;
   }

Cleanup:
   ClearMainGrammar();

   // unregister the notification sink
   hRes = gpSRMode->UnRegister (dwKey);
   CHECKFAILED ("Register");

   // make sure that all 4 got hit
   if (
      !nsnew.m_fUtteranceBegin || !nsnew.m_fUtteranceEnd ||
      !gSRNS.m_fUtteranceBegin || !gSRNS.m_fUtteranceEnd)
      Failed ("Didn't get utterance begin/end on both notification sinks.");

   Passed ();
}

// If doesn't support 22 kHz then warn may not work with DirectX
void CALLBACK Test022 (void)
{
   // checks to make sure the engine claims to support this
   NEEDAUDIO;

   if (gSRModeInfo.dwFeatures & SRFEATURE_PCOPTIMIZED)
   {
      // PC optimized
   if ((gpWFEXSR->wFormatTag != WAVE_FORMAT_PCM) ||
      (gpWFEXSR->nChannels != 1) ||
      (gpWFEXSR->nSamplesPerSec != 22050) ||
      (gpWFEXSR->nAvgBytesPerSec != 44100) ||
      (gpWFEXSR->nBlockAlign != 2) ||
      (gpWFEXSR->wBitsPerSample != 16))
      Information ("The recognizer may not work well with DirectSound applications "
         "because it doesn't default to 22 kHz 16-bit mono.");
   else
      Passed();
   }
   else {
      // not phone optimized, so not applicable
      NotApplicable();
   }

}


// - Make sure utterance begin/end and phrase start/finish notification sinks come in right order
void CALLBACK Test023 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (
      "[<Start>]\n"
      "<Start>=hello world\n"
      );
   CHECKFAILED ("Load grammar");

   BOOL  *pf;

   // activate the grammar
   hRes = gpSRGramComp->Activate();
   if (hRes) {
      Failed ("Activate", hRes);
      return;
   }

   // set all wait flags to 0
   gSRGramNS.m_fPhraseFinish = gSRGramNS.m_fPhraseStart = gSRGramNS.m_fPhraseHypothesis = 0;
   gSRNS.m_fUtteranceBegin = gSRNS.m_fUtteranceEnd = 0;

   // wait for utterance begin
   pf = WaitForEvent (10.0, &gSRNS.m_fUtteranceBegin, &gSRGramNS.m_fPhraseStart);
   if (pf == &gfAbortTest) return;
   if (!pf) {
      Failed ("Didn't get utterancebegin");
      return;
   }
   if (gSRGramNS.m_fPhraseFinish || gSRNS.m_fUtteranceEnd) {
      Failed ("Got notifications out of sync");
      return;
   }

   // if we haven't already gotten a phrase start then wait for that
   if (!gSRGramNS.m_fPhraseStart) {
      pf = WaitForEvent (10.0, &gSRGramNS.m_fPhraseStart);
      if (pf == &gfAbortTest) return;
      if (!pf) {
         Failed ("Didn't get phrase start");
         return;
      }
      if (gSRGramNS.m_fPhraseFinish) {
         Failed ("Got notifications out of sync");
         return;
      }
   }

   // if we haven't already gotten an utterance begin then wait for that
   if (!gSRNS.m_fUtteranceBegin) {
      pf = WaitForEvent (10.0, &gSRNS.m_fUtteranceBegin);
      if (pf == &gfAbortTest) return;
      if (!pf) {
         Failed ("Didn't get utterance begin");
         return;
      }
      if (gSRNS.m_fUtteranceEnd) {
         Failed ("Got notifications out of sync");
         return;
      }
   }

   // wait for an utterance end
   pf = WaitForEvent (30.0, &gSRNS.m_fUtteranceEnd, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest) return;
   if (!pf) {
      Failed ("Didn't get utterance end");
      return;
   }

   // if didn't get a phrase finish then wait for that
   if (!gSRGramNS.m_fPhraseFinish) {
      pf = WaitForEvent (10.0, &gSRGramNS.m_fPhraseFinish);
      if (pf == &gfAbortTest) return;
      if (!pf) {
         Failed ("Didn't get phrase finish");
         return;
      }
      if (gSRGramNS.m_fPhraseStart) {
         Failed ("Got notifications out of sync");
         return;
      }

   }

   // if didn't get an utterance end then wait for that
   if (!gSRNS.m_fUtteranceEnd) {
      pf = WaitForEvent (10.0, &gSRNS.m_fUtteranceEnd);
      if (pf == &gfAbortTest) return;
      if (!pf) {
         Failed ("Didn't get phrase finish");
         return;
      }
      if (gSRNS.m_fUtteranceBegin) {
         Failed ("Got notifications out of sync");
         return;
      }
   }

   // wait around  few seconds to make sure we don't get hypothesis
   pf = WaitForEvent (5.0, &gSRGramNS.m_fPhraseHypothesis);
   if (pf == &gfAbortTest) return;
   if (pf== &gSRGramNS.m_fPhraseHypothesis) {
      Failed ("Got a notification out of sync");
   }

  // deactivate the grammar
   hRes = gpSRGramComp->Deactivate();

   // clear the audio
   ClearSRAudio();

   ClearMainGrammar();
   Passed ();
}


// - Load grammar (CFG) with hwnd=NULL and make sure works
void CALLBACK Test024 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("Load grammar");

   // see if we hear anything. We don't care if it's the right thing
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   
   ClearMainGrammar();
   Passed ();
}


// - Load grammar (CFG) with hwnd=app and make sure it works when app has focus. Switch away
// and make sure it works the way it should. Make sure audio not used when no focus.
void CALLBACK Test025 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // set the focus away from this window
   Information ("Making sure the window doesn't have the focus "
      "so nothing it listening for it.");

   HWND hwndTemp;
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);

   WaitForEvent (1.0);

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("Load grammar");

   // activate the grammar and make sure it's not really active
   hRes = gpSRGramComp->Activate (ghWndEditor);
   CHECKFAILED ("Activate");

   // make sure no audio is getting sucked
   BOOL *pf;
   pf = WaitForEvent (10.0, &gSRWANS.m_fPosn);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gSRWANS.m_fPosn)
      Failed ("SR engine has audio device open even when it's active for a different window");

   // give this window the focus
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);

   // see if we hear anything. We don't care if it's the right thing
   // wait for the event
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf)
      Failed ("Never got a PhraseFinish");

   // send some more text
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // set the focus away
   Information ("Making sure the window doesn't have the focus "
      "so nothing it listening for it.");
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);

   // wait 1/2 second to give the recognizer time to respond to
   // the window change. Then, make sure we're not sucking down adudio
   pf = WaitForEvent (.5);

   // see if is getting data from audio
   pf = WaitForEvent (5, &gSRWANS.m_fPosn);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gSRWANS.m_fPosn)
      Failed ("SR engine has audio device open even when it's active for a different window");

   // deactivate
   hRes = gpSRGramComp->Deactivate ();
   CHECKFAILED ("Deactivate");

   // give this window the focus
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);

   
   ClearSRAudio();
   ClearMainGrammar();
   Passed ();
}

// - Load grammar (CFG) with hwn=badhwnd and make sure its not recognized
void CALLBACK Test026 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("Load grammar");

   // find a window that's not readl
   HWND  hWnd;
   while (TRUE) {
      hWnd = (HWND) rand();
      if (!IsWindow(hWnd))
         break;
   }

   // activate
   hRes = gpSRGramComp->Activate (hWnd);
   if (!hRes)
      Warning ("Didn't error out invalid window");

   ClearSRAudio();
   ClearMainGrammar();

   Passed ();
}


// - Load grammar (dictation) with hwnd=NULL and make sure works
void CALLBACK Test027 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("Load grammar");

   // see if we hear anything. We don't care if it's the right thing
   hRes = WaitForPhraseFinish();
   CHECKFAILED ("WaitForPhraseFinish");
   
   ClearMainGrammar();
   Passed ();
}


// - Load grammar (dictation) with hwnd=app and make sure it works when app has focus. Switch away
// and make sure it works the way it should. Make sure audio not used when no focus.
void CALLBACK Test028 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;

   // set the focus away from this window
   Information ("Making sure the window doesn't have the focus "
      "so nothing it listening for it.");
   HWND hwndTemp;
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);

   WaitForEvent (1.0);

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("Load grammar");

   // activate the grammar and make sure it's not really active
   hRes = gpSRGramComp->Activate (ghWndEditor);
   CHECKFAILED ("Activate");

   // make sure no audio is getting sucked
   BOOL *pf;
   pf = WaitForEvent (10.0, &gSRWANS.m_fPosn);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gSRWANS.m_fPosn)
      Failed ("SR engine has audio device open even when it's active for a different window");

   // give this window the focus
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);

   // see if we hear anything. We don't care if it's the right thing
   // wait for the event
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf)
      Failed ("Never got a PhraseFinish");

   // send some more text
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // set the focus away
   Information ("Making sure the window doesn't have the focus "
      "so nothing it listening for it.");
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);

   // wait 1/2 second to give the recognizer time to respond to
   // the window change. Then, make sure we're not sucking down adudio
   pf = WaitForEvent (.5);

   // see if is getting data from audio
   pf = WaitForEvent (5, &gSRWANS.m_fPosn);
   if (pf == &gfAbortTest)
      return;
   else if (pf == &gSRWANS.m_fPosn)
      Failed ("SR engine has audio device open even when it's active for a different window");

   // deactivate
   hRes = gpSRGramComp->Deactivate ();
   CHECKFAILED ("Deactivate");

   // give this window the focus
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);

   
   ClearSRAudio();
   ClearMainGrammar();
   Passed ();
}

// - Load grammar (dictation) with hwn=badhwnd and make sure its not recognized
void CALLBACK Test029 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // See if SR hears it
   hRes = SetMainGrammar (gszDictationGrammar);
   CHECKFAILED ("Load grammar");

   // find a window that's not readl
   HWND  hWnd;
   while (TRUE) {
      hWnd = (HWND) rand();
      if (!IsWindow(hWnd))
         break;
   }

   // activate
   hRes = gpSRGramComp->Activate (hWnd);
   if (!hRes)
      Warning ("Didn't error out invalid window");
   
   ClearSRAudio();
   ClearMainGrammar();

   Passed ();
}


// - Load CFG with hwdn=null and dictation with hwnd=NULL, send in text. make sure the
// results go to either cfg or dictation. Make sure that if say command, most of the time
// it goes to cfg, and that never get the potential output of a cfg recognized by dictation
void CALLBACK Test030 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;
   NEEDCFG;

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate();
   CHECKFAILED ("Activate");

   // pass in the first utterance
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // if the dictation grammar isn't finished, wait another second for that
   if (!gSRGramNSSecondary.m_fPhraseFinish) {
      pf = WaitForEvent (1.0, &gSRGramNSSecondary.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Dictation Recognition timed out so can't perform test.");
      }
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both dictation & CFG have recognized results");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("CFG got recognition result");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Dictation grammar got recognition. CFG should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }
}

// - Load CFG with hwdn=null and dictation with hwnd=NULL, send in text. make sure the
// results go to either cfg or dictation. Make sure that if say command, most of the time
// it goes to cfg, and that never get the potential output of a cfg recognized by dictation
void CALLBACK Test031 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;
   NEEDCFG;

   // pass in the first utterance
   hRes = SpeakToSR ("this is a test period");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate();
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // if the dictation grammar isn't finished, wait another second for that
   if (!gSRGramNSSecondary.m_fPhraseFinish) {
      pf = WaitForEvent (1.0, &gSRGramNSSecondary.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Dictation Recognition timed out so can't perform test.");
      }
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both dictation & CFG have recognized results");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("Dictation got recognition result");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("CFG grammar got recognition. dictation should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }
}

// - Load CFG with hwdn=app and dictation with hwnd=app, send in text. make sure the
// results go to either cfg or dictation. Make sure that if say command, most of the time
// it goes to cfg, and that never get the potential output of a cfg recognized by dictation
void CALLBACK Test032 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;
   NEEDCFG;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // pass in the first utterance
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(ghWndEditor);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // if the dictation grammar isn't finished, wait another second for that
   if (!gSRGramNSSecondary.m_fPhraseFinish) {
      pf = WaitForEvent (1.0, &gSRGramNSSecondary.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Dictation Recognition timed out so can't perform test.");
      }
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both dictation & CFG have recognized results");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("CFG got recognition result");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Dictation grammar got recognition. CFG should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }
}

// - Load CFG with hwdn=null and dictation with hwnd=app, send in text. make sure the
// results go to either cfg or dictation. Make sure that if say command, most of the time
// it goes to cfg, and that never get the potential output of a cfg recognized by dictation
void CALLBACK Test033 (void)
{
   HRESULT hRes;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;
   NEEDCFG;

   // pass in the first utterance
   hRes = SpeakToSR ("this is a test period");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(ghWndEditor);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // if the dictation grammar isn't finished, wait another second for that
   if (!gSRGramNSSecondary.m_fPhraseFinish) {
      pf = WaitForEvent (1.0, &gSRGramNSSecondary.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Dictation Recognition timed out so can't perform test.");
      }
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both dictation & CFG have recognized results");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("Dictation got recognition result");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("CFG grammar got recognition. dictation should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }
}

// - Load CFG with hwdn=app and dictation with hwnd=null, send in text. make sure the
// results go to either cfg or dictation. Make sure that if say command, most of the time
// it goes to cfg, and that never get the potential output of a cfg recognized by dictation
void CALLBACK Test034 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;
   NEEDCFG;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // pass in the first utterance
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // if the dictation grammar isn't finished, wait another second for that
   if (!gSRGramNSSecondary.m_fPhraseFinish) {
      pf = WaitForEvent (1.0, &gSRGramNSSecondary.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Dictation Recognition timed out so can't perform test.");
      }
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both dictation & CFG have recognized results");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("CFG got recognition result");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Dictation grammar got recognition. CFG should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }
}

// - Load CFG with hwdn=app and dictation with hwnd=null, send in text. make sure the
// results go to either cfg or dictation. Make sure that if say command, most of the time
// it goes to cfg, and that never get the potential output of a cfg recognized by dictation
void CALLBACK Test035 (void)
{
   HRESULT hRes;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;
   NEEDCFG;

   // pass in the first utterance
   hRes = SpeakToSR ("this is a test period");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // if the dictation grammar isn't finished, wait another second for that
   if (!gSRGramNSSecondary.m_fPhraseFinish) {
      pf = WaitForEvent (1.0, &gSRGramNSSecondary.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Dictation Recognition timed out so can't perform test.");
      }
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both dictation & CFG have recognized results");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("Dictation got recognition result");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("CFG grammar got recognition. dictation should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }
}


// - Load CFG with hwdn=app and dictation with hwnd=null, send in text. make sure the
// results go to either cfg or dictation. Make sure that if say command, most of the time
// it goes to cfg, and that never get the potential output of a cfg recognized by dictation
void CALLBACK Test036 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;
   NEEDCFG;

   // set the focus
   HWND hwndTemp;
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);

   // pass in the first utterance
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNSSecondary.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("Dictation Recognition timed out so can't perform test.");
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("Dictation got recognition result");
   }
   else if (!gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Dictation should have gotten the result.");
   }

   // give this window the focus
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);
}


// - Load CFG with hwdn=app and identical cfg with hwnd=null.
// Set hwnd to active window, and expect focus
void CALLBACK Test037 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // pass in the first utterance
   hRes = SpeakToSR ("Hello world.");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // if the other cfg grammar isn't finished, wait another second for that
   if (!gSRGramNSSecondary.m_fPhraseFinish) {
      pf = WaitForEvent (1.0, &gSRGramNSSecondary.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Second CFG timed out so can't perform test.");
      }
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both CFGs have recognized results");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("App CFG got recognition result");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("NULL CFG grammar got recognition. App CFG should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }

   // set the focus
   SetForegroundWindow (ghWndEditor);

}

// - Load CFG with hwdn=app and identical cfg with hwnd=null.
// Set random window to active window, and expect NULL CFG to win
void CALLBACK Test038 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   // set the focus
   HWND hwndTemp;
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);

   // pass in the first utterance
   hRes = SpeakToSR ("Hello world.");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;
   gSRGramNS.m_dwPhraseFinishFlags = ISRNOTEFIN_RECOGNIZED;
   gSRGramNS.m_szPhraseFinishText[0] = 0;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNSSecondary.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both CFGs have recognized results");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("NULL CFG got recognition result");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("App CFG grammar got recognition. NULL CFG should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }

   // set the focus
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);

}

// - Load CFG with hwdn=app and overlapping cfg with hwnd=null.
// Set hwnd to active window, and expect app grammar to get
void CALLBACK Test039 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // pass in the first utterance
   hRes = SpeakToSR ("Hello world.");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszHelloWorld2Grammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // if the other cfg grammar isn't finished, wait another second for that
   if (!gSRGramNSSecondary.m_fPhraseFinish) {
      pf = WaitForEvent (1.0, &gSRGramNSSecondary.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Second CFG timed out so can't perform test.");
      }
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both CFGs have recognized results");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("App CFG got recognition result");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("NULL CFG grammar got recognition. App CFG should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }

   // set the focus
   SetForegroundWindow (ghWndEditor);

}

// - Load CFG with hwdn=app and identical cfg with hwnd=null.
// Set random window to active window, and expect NULL CFG to win
void CALLBACK Test040 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // set the focus
   HWND hwndTemp;
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);

   // pass in the first utterance
   hRes = SpeakToSR ("Hello world.");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszHelloWorld2Grammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;
   gSRGramNS.m_dwPhraseFinishFlags = ISRNOTEFIN_RECOGNIZED;
   gSRGramNS.m_szPhraseFinishText[0] = 0;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNSSecondary.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both CFGs have recognized results");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("NULL CFG got recognition result");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("App CFG grammar got recognition. NULL CFG should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }

   // set the focus
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);

}

// - Load CFG with hwdn=app and overlapping cfg with hwnd=null.
// Set hwnd to active window, and expect NULL grammar to get
void CALLBACK Test041 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // pass in the first utterance
   hRes = SpeakToSR ("goodbye.");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszHelloWorld2Grammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // if the other cfg grammar isn't finished, wait another second for that
   if (!gSRGramNSSecondary.m_fPhraseFinish) {
      pf = WaitForEvent (1.0, &gSRGramNSSecondary.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Second CFG timed out so can't perform test.");
      }
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both CFGs have recognized results");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("NULL CFG got recognition result");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("App CFG grammar got recognition. NULL CFG should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }

   // set the focus
   SetForegroundWindow (ghWndEditor);

}

// - Load dictation with hwdn=app and overlapping cfg with hwnd=null.
// Set hwnd to active window, and expect app grammar to get
void CALLBACK Test042 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // pass in the first utterance
   hRes = SpeakToSR ("This is a test period");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");
   hRes = SetSecondaryGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // if the other cfg grammar isn't finished, wait another second for that
   if (!gSRGramNSSecondary.m_fPhraseFinish) {
      pf = WaitForEvent (1.0, &gSRGramNSSecondary.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Second dictation timed out so can't perform test.");
      }
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both dictations have recognized results");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("App Dictation got recognition result");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("NULL Dictation grammar got recognition. App Dictation should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }

   // set the focus
   SetForegroundWindow (ghWndEditor);

}

// - Load dictation with hwdn=app and overlapping cfg with hwnd=null.
// Set random window to active window, and expect NULL grammar to get
void CALLBACK Test043 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;

   // set the focus
   HWND hwndTemp;
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);

   // pass in the first utterance
   hRes = SpeakToSR ("This is a test period");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");
   hRes = SetSecondaryGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;
   gSRGramNS.m_szPhraseFinishText[0] = 0;
   gSRGramNS.m_dwPhraseFinishFlags = ISRNOTEFIN_RECOGNIZED;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNSSecondary.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("Dictation Recognition timed out so can't perform test.");
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both dictations have recognized results");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("NULL Dictation got recognition result");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("App Dictation grammar got recognition. NULL Dictation should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }

   // set the focus
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);

}

// - Load two cfgs at different points, and make sure get recognized
// properly
void CALLBACK Test044 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // pass in the first utterance
   hRes = SpeakToSR ("hello world");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszHelloWorld2Grammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(NULL);
   CHECKFAILED ("Activate");

   // wait until get notification from first grammar
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // activate the next one and speak the same thing
   hRes = gpSRGramCompSecondary->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = SpeakToSR ("hello world");
   CHECKFAILED ("SpeakToSR");


   // wait until I get a notification of recognition

   // if the other cfg grammar isn't finished, wait another second for that
   pf = WaitForEvent (60.0, &gSRGramNSSecondary.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("Second CFG timed out so can't perform test.");
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both CFGs have recognized results");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("App CFG got recognition result");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("NULL CFG grammar got recognition. App CFG should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }

   // set the focus
   SetForegroundWindow (ghWndEditor);

}



// Make sure window switching works
void CALLBACK Test045 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // pass in the first utterance
   hRes = SpeakToSR ("hello world");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszHelloWorldGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszHelloWorld2Grammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // wait until get notification from first grammar
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   // give another window the focus
   HWND hwndTemp;
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);

   // activate the next one and speak the same thing
   hRes = SpeakToSR ("hello world");
   CHECKFAILED ("SpeakToSR");


   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;
   gSRGramNS.m_dwPhraseFinishFlags= ISRNOTEFIN_RECOGNIZED;
   gSRGramNS.m_szPhraseFinishText[0] = 0;

   // wait until I get a notification of recognition

   // if the other cfg grammar isn't finished, wait another second for that
   pf = WaitForEvent (60.0, &gSRGramNSSecondary.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("Second CFG timed out so can't perform test.");
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both CFGs have recognized results");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("App CFG got recognition result");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("NULL CFG grammar got recognition. App CFG should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }

   // set the focus
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);

}


// - Load two cfgs at different points, and make sure get recognized
// properly
void CALLBACK Test046 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDCFG;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // pass in the first utterance
   hRes = SpeakToSR ("goodbye");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszMultiExportGrammar);
   CHECKFAILED ("MultiExport grammar");

   // activate both
   hRes = gpSRGramComp->Activate(NULL, L"Start1");
   CHECKFAILED ("Activate");
   hRes = gpSRGramComp->Activate(ghWndEditor, L"Start2");
   CHECKFAILED ("Activate");

   // wait until get notification from grammar
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }

   if (stricmp(gSRGramNS.m_szPhraseFinishText, "goodbye"))
      Failed ("Expected to recognize goodbye");

   // minimize the window
   HWND hwndTemp;
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);

   // speak again, this time we expect "this is a test" or unrecognized
   hRes = SpeakToSR ("this is a test");
   CHECKFAILED ("SpeakToSR");
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("CFG Recognition timed out so can't perform test.");
   }
   if (stricmp(gSRGramNS.m_szPhraseFinishText, "this is a test"))
      Failed ("Expected to recognize this is a test");

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();

   // set the focus
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);

   Passed();
}


// - Send a random buffer of data to the grammarload function and make sure sr can detect
void CALLBACK Test047 (void)
{
   HRESULT hRes;

   // fill a buffer with random memory
   BYTE  abMem[10000];
   DWORD dwSize;
   dwSize = rand() % sizeof(abMem);
   DWORD i;
   for (i = 0; i < dwSize; i++)
      abMem[i] = (BYTE) rand();

   // make sure the engine returns failure
   LPUNKNOWN   pUnk;

   // cfg
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_CFG,
      abMem, dwSize, &gSRGramNS, &pUnk);
   if (!hRes)
      Failed ("Grammar load didn't detect bogus data.");

   // dictation
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_DICTATION,
      abMem, dwSize, &gSRGramNS, &pUnk);
   if (!hRes)
      Failed ("Grammar load didn't detect bogus data.");

   // limited-domain
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_LIMITEDDOMAIN,
      abMem, dwSize, &gSRGramNS, &pUnk);
   if (!hRes)
      Failed ("Grammar load didn't detect bogus data.");

   // cfg
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_CFGNATIVE,
      abMem, dwSize, &gSRGramNS, &pUnk);
   if (!hRes)
      Failed ("Grammar load didn't detect bogus data.");

   // cfg
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_DICTATIONNATIVE,
      abMem, dwSize, &gSRGramNS, &pUnk);
   if (!hRes)
      Failed ("Grammar load didn't detect bogus data.");

   // cfg
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_LIMITEDDOMAINNATIVE,
      abMem, dwSize, &gSRGramNS, &pUnk);
   if (!hRes)
      Failed ("Grammar load didn't detect bogus data.");

   Passed();
}

// - Make sure that language ID is checked by engine - warning if not
void CALLBACK Test048 (void)
{
   HRESULT hRes;

   NEEDCFG;

   hRes = SetMainGrammar (
      "[Grammar]\n"
      "LangID=9999\n"   // bogus language
      "[<Start>]\n"
      "<Start>=hello\n"
      );
   ClearMainGrammar();

   if (!hRes)
      Failed ("GrammarLoad doesn't seem to be looking at language chunk");
   else
      Passed();

}

// - make sure dictation grammars work
void CALLBACK Test049 (void)
{

   NEEDENGLISH;
   NEEDAUDIO;
   NEEDDICTATION;
   NEEDCONTINUOUS;

   char  *psz;
   psz = SpeakAndListen ("This is a test of dictation period",
      gszDictationGrammar);
   if (!psz)
      Failed ("Doesn't support dictation grammars");
   else
      Passed();

}

// - make sure that limtied domain grammars work
void CALLBACK Test050 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDLIMITEDDOMAIN;
   NEEDCONTINUOUS;

   char  *psz;
   psz = SpeakAndListen ("hello there",
      gszLimitedDomainGrammar);
   if (!psz)
      Failed ("Doesn't support limited-domain grammars");
   else
      Passed();

}


// - make a simple grammar with a bunch of 1-word alternatives and make sure goes
void CALLBACK Test051 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   char  *psz;
   psz = SpeakAndListen ("welcome",
      "[<Start>]\n"
      "<Start>=hello\n"
      "<Start>=welcome\n"
      "<Start>=goodbye\n"
      "<Start>=farewell\n"
      );
   if (!psz || stricmp(psz, "welcome"))
      Failed ("Doesn't support CFG grammars");
   else
      Passed();

}


// - make a simple grammar with a bunch of N-word alternatives and make sure goes (duplicate
// source words in some of the alternatives)
void CALLBACK Test052 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   char  *psz;
   psz = SpeakAndListen ("welcome home",
      "[<Start>]\n"
      "<Start>=hello there\n"
      "<Start>=welcome home\n"
      "<Start>=welcome back\n"
      "<Start>=goodbye now\n"
      "<Start>=farewell to thee\n"
      );
   if (!psz || stricmp(psz, "welcome home"))
      Failed ("Doesn't support CFG grammars");
   else
      Passed();

}


// - make a grammar with rules, and have the rules called
void CALLBACK Test053 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   char  *psz;
   psz = SpeakAndListen ("item two",
      "[<Start>]\n"
      "<Start>=item <0..9>\n"
      );
   if (!psz || stricmp(psz, "item two"))
      Failed ("Doesn't support CFG grammars");
   else
      Passed();

}

// - make sure that all the default grammars in the cfg compiler work - phone number ,etc.
void CALLBACK Test054 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   char  *psz;
   DWORD dwCompleteTimeOut;		// complete utterance time out in milliseconds
   DWORD dwIncompleteTimeOut;	// incomplete utterance time out in milliseconds

   // Save the original values for latter restoration
   // - ignore error return since we aren't testing this and we going to proceed
   //   even if the call fails.
   gpSRMode->TimeOutGet(&dwIncompleteTimeOut, &dwCompleteTimeOut);
   // Set the complete phrase timeout to 500 to avoid premature phrase segmentation
   gpSRMode->TimeOutSet(500, 500);
   
	// phone numbers
	// set flag to true if one get's recognized
   psz = SpeakAndListen ("call six two three four five two seven",
      "[<Start>]\n"
      "<Start>=call <PhoneNumber>\n"
      );
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "call six two three four five two seven"))
      Warning ("Didn't recognize a CFG propertly");

   psz = SpeakAndListen ("call one eight nine zero three four six",
      "[<Start>]\n"
      "<Start>=call <PhoneNumber>\n"
      );
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "call one eight nine zero three four six"))
      Warning ("Didn't recognize a CFG propertly");

   psz = SpeakAndListen ("call nine six seven zero one five two",
      "[<Start>]\n"
      "<Start>=call <PhoneNumber>\n"
      );
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "call nine six seven zero one five two"))
      Warning ("Didn't recognize a CFG propertly");

   // year
   psz = SpeakAndListen ("nineteen ninety seven","[<Start>]\n" "<Start>=<Year>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "nineteen ninety seven"))
      Warning ("Didn't recognize a CFG propertly");

   psz = SpeakAndListen ("eighteen twelve","[<Start>]\n""<Start>=<Year>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "eighteen twelve"))
      Warning ("Didn't recognize a CFG propertly");
	
	psz = SpeakAndListen ("seventeen seventy six","[<Start>]\n" "<Start>=<Year>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "seventeen seventy six"))
      Warning ("Didn't recognize a CFG propertly");
	
	 // date
   psz = SpeakAndListen ("january first nineteen sixty nine","[<Start>]\n" "<Start>=<Date>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "january first nineteen sixty nine"))
      Warning ("Didn't recognize a CFG propertly");

   psz = SpeakAndListen ("april twentieth fourteen fifty six",
      "[<Start>]\n"
      "<Start>=<Date>\n"
      );
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "april twentieth fourteen fifty six"))
      Warning ("Didn't recognize a CFG propertly");

   psz = SpeakAndListen ("november tenth eighteen forty seven","[<Start>]\n" "<Start>=<Date>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "november tenth eighteen forty seven"))
      Warning ("Didn't recognize a CFG propertly");

   // fraction
   psz = SpeakAndListen ("fifty two eighteenths","[<Start>]\n" "<Start>=<Fraction>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "fifty two eighteenths"))
      Warning ("Didn't recognize a CFG propertly");

   psz = SpeakAndListen ("one third","[<Start>]\n" "<Start>=<Fraction>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "one third"))
      Warning ("Didn't recognize a CFG propertly");

   psz = SpeakAndListen ("one and four fifths","[<Start>]\n" "<Start>=<Fraction>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "one and four fifths"))
      Warning ("Didn't recognize a CFG propertly");

   // natural number
   psz = SpeakAndListen ("one hundred million","[<Start>]\n" "<Start>=<Natural>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "one hundred million"))
      Warning ("Didn't recognize a CFG propertly");

   psz = SpeakAndListen ("six thousand four hundred and two","[<Start>]\n" "<Start>=<Natural>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "six thousand four hundred and two"))
      Warning ("Didn't recognize a CFG propertly");

   psz = SpeakAndListen ("three hundred and fifty seven thousand",
      "[<Start>]\n"
      "<Start>=<Natural>\n"
      );
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "three hundred and fifty seven thousand"))
      Warning ("Didn't recognize a CFG propertly");

   // ordinal number
   psz = SpeakAndListen ("fifty third","[<Start>]\n" "<Start>=<Ordinal>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "fifty third"))
      Warning ("Didn't recognize a CFG propertly");

   psz = SpeakAndListen ("seventy sixth","[<Start>]\n" "<Start>=<Ordinal>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "seventy sixth"))
      Warning ("Didn't recognize a CFG propertly");

   psz = SpeakAndListen ("tenth","[<Start>]\n" "<Start>=<Ordinal>\n");
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "tenth"))
      Warning ("Didn't recognize a CFG propertly");

   // floating point number
   psz = SpeakAndListen ("sixteen point eight nine","[<Start>]\n" "<Start>=<Float>\n" );
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "sixteen point eight nine"))
      Warning ("Didn't recognize a CFG properly");

   psz = SpeakAndListen ("three point one four one five","[<Start>]\n" "<Start>=<Float>\n" );
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "three point one four one five"))
      Warning ("Didn't recognize a CFG properly");

   psz = SpeakAndListen ("twenty point two six","[<Start>]\n" "<Start>=<Float>\n" );
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "twenty point two six"))
      Warning ("Didn't recognize a CFG properly");


   // multiple digits
   psz = SpeakAndListen ("six two seven","[<Start>]\n" "<Start>=<Digits>\n" );
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "six two seven"))
      Warning ("Didn't recognize a CFG properly");

   psz = SpeakAndListen ("one five four","[<Start>]\n" "<Start>=<Digits>\n" );
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "one five four"))
      Warning ("Didn't recognize a CFG properly");

   psz = SpeakAndListen ("eight nine six","[<Start>]\n" "<Start>=<Digits>\n" );
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Failed ("Rejected utterance");
   else if (stricmp(psz, "eight nine six"))
      Warning ("Didn't recognize a CFG properly");

	Passed();
   // Reset the original values
   gpSRMode->TimeOutSet(dwIncompleteTimeOut, dwCompleteTimeOut);
}



// - test that really long words work
void CALLBACK Test055 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   char  *psz;
   psz = SpeakAndListen ("this-is-a-really-long-word",
      "[<Start>]\n"
      "<Start>=this-is-a-really-long-word\n"
      "<Start>=this hyphen is hyphen a hyphen really hyphen long hyphen word\n"
      "<Start>=this dash is dash a dash really dash long dash word\n"
      );
   if (!psz || stricmp(psz, "this-is-a-really-long-word"))
      Failed ("Doesn't support CFG grammars");
   else
      Passed();

}

// - test the garbage words work at the beginning, middle, end of a phrase. Also, multiple
// places.
void CALLBACK Test056 (void)
{
   NEEDENGLISH;
   NEEDCFG;

   // if doesn't compain about loading word-spotting grammar then
   // warning
   HRESULT hRes;
   hRes = SetMainGrammar (gszWordSpotGrammar);
   ClearMainGrammar();
   if (!hRes && !(gSRModeInfo.dwFeatures & SRFEATURE_WILDCARD)) {
      Information ("SRMODEINFO claims that word spotting is not supported, but it's possible to load a word spotting grammar");
      return;
   }

   NEEDAUDIO;
   NEEDSRFEATURE (SRFEATURE_WILDCARD);

   char *psz;

   psz = SpeakAndListen ("could you help me make a long distance call please thank you",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "long distance"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("long distance",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "long distance"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("long distance call",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "long distance"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("Make it collect to my mother thanks.",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "collect"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("make it collect please",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "collect"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("we need to make a person to person call",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "person to person"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("I want to fly to Arizona.",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "Arizona"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("I want to fly to Arizona right now.",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (!psz[0])
      Passed();   // supposed to reject this one
   else if (stricmp(psz, "Arizona"))
      Warning ("Misrecognized the command as something else.");
   else
      Failed ("Didn't recognize a CFG properly.");

   psz = SpeakAndListen ("Arizona",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "Arizona"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("send me to New York.",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "New York"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("Mister Smith could you come over here.",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "Mister Smith"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("Misses Jones could you come over here.",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "Misses Jones"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("Misses Jones",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "Misses Jones"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();

   psz = SpeakAndListen ("computer, I want to send mail by way of Mister Jones now",
      gszWordSpotGrammar);
   if (!psz)
      Failed ("Couldn't load CFG");
   else if (stricmp(psz, "send mail Mister Jones"))
      Warning ("Didn't recognize a CFG properly");
   else
      Passed();
}

// - test that really long words work
void CALLBACK Test057 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   char  *psz;
   psz = SpeakAndListen ("1 3 5",
      "[<Start>]\n"
      "<Start>=1 3 5\n"
      "<Start>=1 8 5\n"
      "<Start>=2 3 8\n"
      );
   if (!psz || stricmp(psz, "1 3 5"))
      Failed ("Doesn't support CFG grammars with numbers");
   else
      Passed();

   psz = SpeakAndListen ("32",
      "[<Start>]\n"
      "<Start>=3 2\n"
      "<Start>=32\n"
      "<Start>=89\n"
      "<Start>=57\n"
      );
   if (!psz || stricmp(psz, "32"))
      Failed ("Doesn't support CFG grammars with numbers");
   else
      Passed();
}

// - Test lists
void CALLBACK Test058 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;
   NEEDSRFEATURE (SRFEATURE_GRAMLIST);

   char  *psz;

   // lists with single words
   psz = SpeakAndListen ("call mark",
      gszListGrammar);
   if (!psz || stricmp(psz, "call mark"))
      Failed ("Doesn't support CFG grammars with numbers");
   else
      Passed();

   // lists with multiple words
   psz = SpeakAndListen ("purchase a pepperoni pizza",
      gszListGrammar);
   if (!psz || stricmp(psz, "purchase a pepperoni pizza"))
      Failed ("Doesn't support CFG grammars with numbers");
   else
      Passed();

   // lists with multiple words
   psz = SpeakAndListen ("purchase 32 hats",
      gszListGrammar);
   if (!psz || stricmp(psz, "purchase 32 hats"))
      Failed ("Doesn't support CFG grammars with numbers");
   else
      Passed();

   // lists with unfilled entries
   psz = SpeakAndListen ("please look for the",
      gszListGrammar);
   if (!psz)
      Failed ("Doesn't support CFG grammars with numbers");
   else if (stricmp(psz, "please look for the"))
      Passed ();
   else
      Failed ("Recognized a phrase with an empty list");

}

// - test that lists work if load, and then reload a different set
void CALLBACK Test059 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;
   NEEDSRFEATURE (SRFEATURE_GRAMLIST);

   HRESULT  hRes;
   hRes = SetMainGrammar (gszListGrammar);
   CHECKFAILED ("SetMainGrammar");

   // speak text to text-to-speech
   hRes = SpeakToSR ("purchase a pepperoni pizza");
   CHECKFAILED ("SpeakToSR");

   // activate
   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   BOOL *pf;

   // wait for phrase finish
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get recognition result");
      return;
   }

   if (stricmp(gSRGramNS.m_szPhraseFinishText, "purchase a pepperoni pizza"))
      Failed ("Didn't recognize list entry");

   // set a new list
   PVOID pMem;
   DWORD dwSize;
   pMem = ConstructList (L"a new car\0some fencing\0", &dwSize);
   hRes = gpSRGramComp->ListSet (L"list2", pMem, dwSize);
   CHECKFAILED ("ListSet");

   // make sure the old list entry is no longer recognized
   hRes = SpeakToSR ("purchase a pepperoni pizza");
   CHECKFAILED ("SpeakToSR");
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get recognition result");
      return;
   }
   if (!stricmp(gSRGramNS.m_szPhraseFinishText, "purchase a pepperoni pizza"))
      Failed ("Recognized a list that was just removed");

   // make sure the new entry is recognized
   hRes = SpeakToSR ("purchase a new car");
   CHECKFAILED ("SpeakToSR");
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get recognition result");
      return;
   }
   if (stricmp(gSRGramNS.m_szPhraseFinishText, "purchase a new car"))
      Failed ("Didn't recognize new list entry");


   // set another new list, make sure it works with a hyphen
   PVOID pMem2;
   pMem2 = ConstructList (L"-\0an old book\0", &dwSize);
   hRes = gpSRGramComp->ListSet (L"list2", pMem2, dwSize);
   CHECKFAILED ("ListSet");

   // make sure the old list entry is no longer recognized
   hRes = SpeakToSR ("purchase a new car or some fencing");
   CHECKFAILED ("SpeakToSR");
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get recognition result");
      return;
   }
   if (!stricmp(gSRGramNS.m_szPhraseFinishText, "purchase a new car or some fencing"))
      Failed ("Recognized a list that was just removed");

   // make sure the new entry is recognized
   hRes = SpeakToSR ("purchase an old book");
   CHECKFAILED ("SpeakToSR");
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't get recognition result");
      return;
   }
   if (stricmp(gSRGramNS.m_szPhraseFinishText, "purchase an old book"))
      Failed ("Didn't recognize new list entry");

   // finally
   ClearMainGrammar();
   ClearSRAudio();

   Passed();

}


// - dictaion - try with topic chunks
// - dictation - try with suggeste word chunks
void CALLBACK Test060 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDDICTATION;
   NEEDCONTINUOUS;

   char  *psz;
   psz = SpeakAndListen ("Let me talk into dictation and hope that it recognizes properly.",
      gszDictationChunkGrammar);
   if (!psz || !psz[0])
      Failed ("Doesn't support dictation");
   else
      Passed();

}

// - discrete dictation - make sure works
void CALLBACK Test061 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDDICTATION;

   HRESULT hRes;
   hRes = SpeakToSR ("welcome\\pau=4000\\ to\\pau=4000\\ this\\pau=4000\\ demonstration \\pau=4000\\period");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszDictationChunkGrammar);
   CHECKFAILED ("gszDictationChunkGrammar");

   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   // expect 5 utterances
   DWORD i;
   BOOL  *pf;
   for (i = 0; i < 5; i++) {
      pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Didn't get expected utterance");
         break;
      }
   }

   // clear
   ClearSRAudio();
   ClearMainGrammar();

   Passed();

}



// - activate a rule other than <Start> and make sure it works
// - case sensativity check in activationg
void CALLBACK Test062 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   HRESULT hRes;
   hRes = SpeakToSR ("goodbye");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszMultiExportGrammar);
   CHECKFAILED ("gszMultiExportGrammar");

   hRes = gpSRGramComp->Activate(NULL, L"Start2");
   CHECKFAILED ("Activate case sensative");
   hRes = gpSRGramComp->Deactivate(L"stART2");
   CHECKFAILED ("Deactivate case insensative");

   hRes = gpSRGramComp->Activate(NULL, L"sTaRt2");
   CHECKFAILED ("Activate case insensative");

   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("Didn't get expected utterance");
   }
   else if (stricmp(gSRGramNS.m_szPhraseFinishText, "goodbye"))
      Failed ("Didn't recognize right phrase.");

   // clear
   ClearSRAudio();
   ClearMainGrammar();

   Passed();

}


// - Load CFG with hwdn=app and identical cfg with hwnd=null.
// Set hwnd to active window, and expect focus
void CALLBACK Test063 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;
   NEEDCONTINUOUS;

   // set the focus
   SetForegroundWindow (ghWndEditor);

   // pass in the first utterance
   hRes = SpeakToSR ("Hello world, this is a test of continuous dictation period");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszDictationGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("Dictation Recognition timed out so can't perform test.");
   }

   // if the other Dictation grammar isn't finished, wait another second for that
   if (!gSRGramNSSecondary.m_fPhraseFinish) {
      pf = WaitForEvent (1.0, &gSRGramNSSecondary.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;
      else if (pf == NULL) {
         Failed ("Second Dictation timed out so can't perform test.");
      }
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both Dictations have recognized results");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("App Dictation got recognition result");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("NULL Dictation grammar got recognition. App Dictation should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }

   // set the focus
   SetForegroundWindow (ghWndEditor);

}

// - Load Dictation with hwdn=app and identical Dictation with hwnd=null.
// Set random window to active window, and expect NULL Dictation to win
void CALLBACK Test064 (void)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   NEEDAUDIO;
   NEEDENGLISH;
   NEEDDICTATION;
   NEEDCONTINUOUS;

   // set the focus
   HWND hwndTemp;
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);

   // pass in the first utterance
   hRes = SpeakToSR ("Hello world, this is a test of continuous dictation period");
   CHECKFAILED ("SpeakToSR");

   // load the gramamrs
   hRes = SetMainGrammar(gszDictationGrammar);
   CHECKFAILED ("Hello world grammar");
   hRes = SetSecondaryGrammar(gszDictationGrammar);
   CHECKFAILED ("Dictation grammar");

   // activate both
   hRes = gpSRGramComp->Activate(ghWndEditor);
   CHECKFAILED ("Activate");
   hRes = gpSRGramCompSecondary->Activate(NULL);
   CHECKFAILED ("Activate");

   // set flags for both phrase finish to false
   gSRGramNS.m_fPhraseFinish = gSRGramNSSecondary.m_fPhraseFinish = FALSE;
   gSRGramNS.m_dwPhraseFinishFlags = ISRNOTEFIN_RECOGNIZED;
   gSRGramNS.m_szPhraseFinishText[0] = 0;

   // wait until I get a notification of recognition
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNSSecondary.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (pf == NULL) {
      Failed ("Dictation Recognition timed out so can't perform test.");
   }

   // stop both of these
   ClearSRAudio();
   ClearMainGrammar();
   ClearSecondaryGrammar();

   // make sure the flags are set properly
#define  FLAGS_BOTH     (ISRNOTEFIN_RECOGNIZED | ISRNOTEFIN_THISGRAMMAR)
   if ((gSRGramNSSecondary.m_dwPhraseFinishFlags & FLAGS_BOTH) != FLAGS_BOTH)
      Failed ("Invalid flags");
   if ((gSRGramNS.m_dwPhraseFinishFlags & FLAGS_BOTH) != ISRNOTEFIN_RECOGNIZED)
      Failed ("Invalid flags");

   // compare the results
   if (gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("Both Dictations have recognized results");
   }
   else if (!gSRGramNS.m_szPhraseFinishText[0] && gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Passed ("NULL Dictation got recognition result");
   }
   else if (gSRGramNS.m_szPhraseFinishText[0] && !gSRGramNSSecondary.m_szPhraseFinishText[0]) {
      Failed ("App Dictation grammar got recognition. NULL Dictation should have");
   }
   else {
      Failed ("Neither grammar got recognized");
   }

   // set the focus
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);

}

// - english dictation - make sure recognizer .\period, .\comma, etc.
void CALLBACK Test065 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDDICTATION;
   NEEDCONTINUOUS;

   char  *psz;

   // period
   psz = SpeakAndListen ("This is a sentence period",
      gszDictationChunkGrammar);
   if (psz) CharLower (psz);
   if (!psz || !strstr(psz, ".\\period"))
      Warning ("Didn't recognize the punctuation");
   else
      Passed();

   // comma
   psz = SpeakAndListen ("he said comma hello",
      gszDictationChunkGrammar);
   if (psz) CharLower (psz);
   if (!psz || !strstr(psz, ",\\comma"))
      Warning ("Didn't recognize the punctuation");
   else
      Passed();

   // exclamation-point
   psz = SpeakAndListen ("This is a sentence exclamation-point",
      gszDictationChunkGrammar);
   if (psz) CharLower (psz);
   if (!psz || !strstr(psz, "!\\exclamation-point"))
      Warning ("Didn't recognize the punctuation");
   else
      Passed();

   // question-mark
   psz = SpeakAndListen ("Is this a sentence question-mark",
      gszDictationChunkGrammar);
   if (psz) CharLower (psz);
   if (!psz || !strstr(psz, "?\\question-mark"))
      Warning ("Didn't recognize the punctuation");
   else
      Passed();

   // new paragraph
   psz = SpeakAndListen ("This is a sentence period new-paragraph",
      gszDictationChunkGrammar);
   if (psz) CharLower (psz);
   if (!psz || !strstr(psz, "new-paragraph"))
      Warning ("Didn't recognize the punctuation");
   else
      Passed();

   // semicolon
   psz = SpeakAndListen ("This is a sentence semicolon",
      gszDictationChunkGrammar);
   if (psz) CharLower (psz);
   if (!psz || !strstr(psz, ";\\semicolon"))
      Warning ("Didn't recognize the punctuation");
   else
      Passed();

   // colon
   psz = SpeakAndListen ("This is a sentence colon",
      gszDictationChunkGrammar);
   if (psz) CharLower (psz);
   if (!psz || !strstr(psz, ":\\colon"))
      Warning ("Didn't recognize the punctuation");
   else
      Passed();

   // quote
   psz = SpeakAndListen ("He said quote This is a sentence period quote",
      gszDictationChunkGrammar);
   if (psz) CharLower (psz);
   if (!psz || !strstr(psz, "\"\\quote"))
      Warning ("Didn't recognize the punctuation");
   else
      Passed();

	   // dash
   psz = SpeakAndListen ("His number is two dash five dash three",
      gszDictationChunkGrammar);
   if (psz) CharLower (psz);
   if (!psz || !strstr(psz, "-\\hyphen"))
      Warning ("Didn't recognize the punctuation");
   else
      Passed();


}

// - cfg - if send in a word with a backslash, then should use the pronunciation from the
// second half. This pronunciation could be text
void CALLBACK Test066 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   char  *psz;

   // pound-sign
   psz = SpeakAndListen ("pound-sign",
      gszPronGrammar);
   if (!psz || stricmp(psz, "#\\pound-sign"))
      Failed ("Didn't recognize the word");
   else
      Passed();

   // pound-sign
   psz = SpeakAndListen ("hello there",
      gszPronGrammar);
   if (!psz || stricmp(psz, "a\\hello-there"))
      Failed ("Didn't recognize the word");
   else
      Passed();

   // pound-sign
   psz = SpeakAndListen ("dollar-sign",
      gszPronGrammar);
   if (!psz || stricmp(psz, "$$$\\dollar-sign"))
      Failed ("Didn't recognize the word");
   else
      Passed();
}

// - test that phrasefinish comes back with the same word IDs as passed into the grammar
// - test that phrasefinish comes back with the same word IDs that passed into the cfg, if
// there's more than one grammar active and listening at a time.
void CALLBACK Test067 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   char  *psz;
   HRESULT  hRes;

   // set focus away from this window & load/activate the secondary grammar
   HWND hwndTemp;
   hwndTemp = GetNextWindow(ghWndEditor, GW_HWNDPREV);
   SetForegroundWindow(hwndTemp);
   hRes = SetSecondaryGrammar (gszWordOrder1Grammar);
   CHECKFAILED ("SetSecondaryGrammar");
   hRes = gpSRGramCompSecondary->Activate (ghWndEditor);
   CHECKFAILED ("Activate");
   
   // speak in the word numbers
   psz = SpeakAndListen ("this is a test",
      gszWordOrder2Grammar);
   ClearSecondaryGrammar();

   if (!psz || stricmp(psz, "this is a test"))
      Failed ("Didn't recognize the word");
   else {
      // given that we know how the grammar compiler works, we
      // know what we should recognize
      if (
         (gSRGramNS.m_abPhraseFinishWordNum[0] != 2) ||
         (gSRGramNS.m_abPhraseFinishWordNum[1] != 1) ||
         (gSRGramNS.m_abPhraseFinishWordNum[2] != 3) ||
         (gSRGramNS.m_abPhraseFinishWordNum[3] != 4)
         )
         Failed ("Recognizer misordered word numbers");
      else
         Passed();
   }

   // reverse the grammars, and expect different numbering
   hRes = SetSecondaryGrammar (gszWordOrder2Grammar);
   CHECKFAILED ("SetSecondaryGrammar");
   hRes = gpSRGramCompSecondary->Activate (ghWndEditor);
   CHECKFAILED ("Activate");
   
   // speak in the word numbers
   psz = SpeakAndListen ("this is a test",
      gszWordOrder1Grammar);
   ClearSecondaryGrammar();

   if (!psz || stricmp(psz, "this is a test"))
      Failed ("Didn't recognize the word");
   else {
      // given that we know how the grammar compiler works, we
      // know what we should recognize
      if (
         (gSRGramNS.m_abPhraseFinishWordNum[0] != 1) ||
         (gSRGramNS.m_abPhraseFinishWordNum[1] != 2) ||
         (gSRGramNS.m_abPhraseFinishWordNum[2] != 3) ||
         (gSRGramNS.m_abPhraseFinishWordNum[3] != 4)
         )
         Failed ("Recognizer misordered word numbers");
      else
         Passed();
   }

   // restore the window back
   ShowWindow (ghWndEditor, SW_RESTORE);
   SetForegroundWindow (ghWndEditor);

}


// - try a very huge (5000 word fan-out) grammar and make sure it can be loaded. warning
// if recognition doesn't happen
void CALLBACK Test068 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   char  szGrammar[100000];
   char  szTemp[256];

   // construct a really big grammar
   strcpy (szGrammar,
      "[<Start>]\n");
   DWORD i;
   for (i = 0; i < 5000; i++) {
      wsprintf (szTemp, "<Start>=%d\n", (int) i);
      strcat (szGrammar, szTemp);
   }


   char  *psz;

   // pound-sign
   psz = SpeakAndListen ("4000",
      szGrammar);

   // we're happy as long as it didn't crash
   if (!psz)
      Warning ("Didn't load grammar");
   else
      Passed();

}




// - AutoGainEnableSet & Get - See if set value & then restore value works
// - AutoGainEnableSet&Get - Invalid parameters.
void CALLBACK Test069 (void)
{
   // get the current value
   HRESULT  hRes;
   DWORD    dwVal;
   hRes = gpSRMode->AutoGainEnableGet(&dwVal);
   if (hRes == SRERR_NOTSUPPORTED) {
      Warning ("AutoGainEnable not supported");
      return;
   }
   else
      CHECKFAILED ("AutoGainEnableGet");
   if (dwVal > 100)
      Failed ("Value out of range");

   // check to see if the attribute changed
   gSRNS.m_fAttribChanged = FALSE;

   // set a new values
   hRes = gpSRMode->AutoGainEnableSet (0);
   CHECKFAILED ("AutoGainEnableSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_AUTOGAINENABLE))
      Failed ("Didn't get AttribChanged notification");

   // verify
   if (0 != gpSRMode->AutoGainEnableGet())
      Failed ("Didn't recall previous value");

   // new value
   hRes = gpSRMode->AutoGainEnableSet (99);
   CHECKFAILED ("AutoGainEnableSet");
   if (99 != gpSRMode->AutoGainEnableGet())
      Failed ("Didn't recall previous value");


   // out of range
   hRes = gpSRMode->AutoGainEnableSet (101);
   if (!hRes)
      Failed ("Didn't error out when set invalid value");
   if (99 != gpSRMode->AutoGainEnableGet())
      Failed ("Didn't recall previous value");

   // invalid get param
   hRes = gpSRMode->AutoGainEnableGet (NULL);
   if (!hRes)
      Failed ("Didn't detect NULL parameter");

   // return to original value
   hRes = gpSRMode->AutoGainEnableSet (dwVal);
   CHECKFAILED ("AutoGainEnableSet");
   if (dwVal != gpSRMode->AutoGainEnableGet())
      Failed ("Didn't recall previous value");

   Passed();
}

// - EchoSet/Get - Set to TRUE/FALSE and see that engine remembers.
// - EchoSet/Get - Invalid parameters.
void CALLBACK Test070 (void)
{
   // get the current value
   HRESULT  hRes;
   BOOL    fEcho;
   hRes = gpSRMode->EchoGet(&fEcho);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable ("Echo not supported");
      return;
   }
   else
      CHECKFAILED ("EchoGet");

   // check to see if the attribute changed
   gSRNS.m_fAttribChanged = FALSE;

   // set a new values
   hRes = gpSRMode->EchoSet (!fEcho);
   CHECKFAILED ("EchoSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_ECHO))
      Failed ("Didn't get AttribChanged notification");

   // verify
   if (!fEcho != gpSRMode->EchoGet())
      Failed ("Didn't recall previous value");

   // new value
   hRes = gpSRMode->EchoSet (fEcho);
   CHECKFAILED ("EchoSet");
   if (fEcho != gpSRMode->EchoGet())
      Failed ("Didn't recall previous value");


   // invalid get param
   hRes = gpSRMode->EchoGet (NULL);
   if (!hRes)
      Failed ("Didn't detect NULL parameter");

   Passed();
}

// - EnergyFloorSet/Get - Set to value and see if the engine remembers.
// - EnergyFloorSet/Get - Invalid parameters.
void CALLBACK Test071 (void)
{
   // get the current value
   HRESULT  hRes;
   WORD     wVal;
   hRes = gpSRMode->EnergyFloorGet(&wVal);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable ("EnergyFloor not supported");
      return;
   }
   else
      CHECKFAILED ("EnergyFloorGet");
 
   // check to see if the attribute changed
   gSRNS.m_fAttribChanged = FALSE;

   // set a new values
   hRes = gpSRMode->EnergyFloorSet (25);
   CHECKFAILED ("EnergyFloorSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_ENERGYFLOOR))
      Failed ("Didn't get AttribChanged notification");

   // verify
   if (25 != gpSRMode->EnergyFloorGet())
      Failed ("Didn't recall previous value");

   // new value
   hRes = gpSRMode->EnergyFloorSet (60);
   CHECKFAILED ("EnergyFloorSet");
   if (60 != gpSRMode->EnergyFloorGet())
      Failed ("Didn't recall previous value");


   // invalid get param
   hRes = gpSRMode->EnergyFloorGet (NULL);
   if (!hRes)
      Failed ("Didn't detect NULL parameter");

   // return to original value
   hRes = gpSRMode->EnergyFloorSet (wVal);
   CHECKFAILED ("EnergyFloorSet");
   if (wVal != gpSRMode->EnergyFloorGet())
      Failed ("Didn't recall previous value");

   Passed();
}

// - MicrophoneGet/Set - Set to value and see if the engine remembers.
// - MicrophoneGet/Set - Invalid parameters.
void CALLBACK Test072 (void)
{
   // get the current value
   HRESULT  hRes;
   WCHAR    szMic[256], szMic2[256];
   DWORD    dwNeeded;
   hRes = gpSRMode->MicrophoneGet(szMic, sizeof(szMic), &dwNeeded);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable ("Microphone not supported");
      return;
   }
   else
      CHECKFAILED ("MicrophoneGet");

   // check to see if the attribute changed
   gSRNS.m_fAttribChanged = FALSE;

   // set a new values
   WCHAR szGoodOne[] = L"A very good one";
   hRes = gpSRMode->MicrophoneSet (szGoodOne);
   CHECKFAILED ("MicrophoneSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_MICROPHONE))
      Failed ("Didn't get AttribChanged notification");

   // verify
   hRes = gpSRMode->MicrophoneGet(szMic2, sizeof(szMic2), &dwNeeded);
   CHECKFAILED ("MicrophoneSet");
   if (wcscmp(L"A very good one", szMic2))
      Failed ("Didn't recall previous value");

   // make sure it requires the right size
   hRes = gpSRMode->MicrophoneGet (NULL, 0, &dwNeeded);
   if (hRes != E_INVALIDARG)
      CHECKFAILED ("MicrophoneGet");
   if (dwNeeded != sizeof(szGoodOne))
      Warning ("Size-needed is not set optimally");
   hRes = gpSRMode->MicrophoneGet (szMic2, sizeof(szGoodOne)-2, &dwNeeded);
   if (hRes != E_INVALIDARG)
      CHECKFAILED ("MicrophoneGet");
   if (dwNeeded != sizeof(szGoodOne))
      Warning ("Size-needed is not set optimally");

   // out of range
   hRes = gpSRMode->MicrophoneSet (
      L"This is a very long microphone string and I just want "
      L"to make sure that it doesn't crash the speech engien "
      L"when it gets such a long string.");

   // invalid get param
   hRes = gpSRMode->MicrophoneGet (NULL, 0, NULL);
   if (!hRes)
      Failed ("Didn't detect NULL parameter");

   // return to original value
   hRes = gpSRMode->MicrophoneSet (szMic);
   CHECKFAILED ("MicrophoneSet");
   hRes = gpSRMode->MicrophoneGet(szMic2, sizeof(szMic2), &dwNeeded);
   CHECKFAILED ("MicrophoneSet");
   if (wcscmp(szMic, szMic2))
      Failed ("Didn't recall previous value");

   Passed();
}


//- RealTimeGet/Set - Set to value and see if the engine remembers.
//- RealTimeGet/Set - Invalid parameters.
void CALLBACK Test073 (void)
{
   // get the current value
   HRESULT  hRes;
   DWORD    dwVal;
   DWORD    dwMinVal;
   DWORD    dwMaxVal;
   DWORD    dwTestValue;

   hRes = gpSRMode->RealTimeGet(&dwVal);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable ("RealTime not supported");
      return;
   }
   else
      CHECKFAILED ("RealTimeGet");

   // Determine the range of settings allowed
   hRes = gpSRMode->RealTimeSet(SRATTR_MINREALTIME);
   CHECKFAILED ("RealTimeSet");
   hRes = gpSRMode->RealTimeGet(&dwMinVal);
   CHECKFAILED ("RealTimeGet");
   hRes = gpSRMode->RealTimeSet(SRATTR_MAXREALTIME);
   CHECKFAILED ("RealTimeSet");
   hRes = gpSRMode->RealTimeGet(&dwMaxVal);
   CHECKFAILED ("RealTimeGet");

   // check to see if the attribute changed
   gSRNS.m_fAttribChanged = FALSE;

   // set a new in range value
   dwTestValue = (dwMaxVal - dwMinVal) / 2 + dwMinVal;
   hRes = gpSRMode->RealTimeSet(dwTestValue);

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_REALTIME))
      Failed ("Didn't get AttribChanged notification");

   // verify
   if (dwTestValue != gpSRMode->RealTimeGet())
      Failed ("Didn't recall previous value");

   // Try a different value
   dwTestValue = (dwMaxVal - dwMinVal) / 3 + dwMinVal;
   hRes = gpSRMode->RealTimeSet(dwTestValue);
   CHECKFAILED ("RealTimeSet");
   if (dwTestValue != gpSRMode->RealTimeGet())
      Failed ("Didn't recall previous value");

   // invalid get param
   hRes = gpSRMode->RealTimeGet (NULL);
   if (!hRes)
      Failed ("Didn't detect NULL parameter");

   // return to original value
   hRes = gpSRMode->RealTimeSet (dwVal);
   CHECKFAILED ("RealTimeSet");
   if (dwVal != gpSRMode->RealTimeGet())
      Failed ("Didn't recall previous value");

   Passed();
}



// - SpeakerGet/Set - Set the value and see if it remembers.
// - SpeakerGet/Set - Try null value, and other invalid parameters.
void CALLBACK Test074 (void)
{
   // get the current value
   HRESULT  hRes;
   WCHAR    szMic[256], szMic2[256];
   DWORD    dwNeeded;
   hRes = gpSRMode->SpeakerGet(szMic, sizeof(szMic), &dwNeeded);
   if (hRes == SRERR_NOTSUPPORTED) {
      Warning ("Speaker not supported");
      return;
   }
   else
      CHECKFAILED ("SpeakerGet");

   // check to see if the attribute changed
   gSRNS.m_fAttribChanged = FALSE;

   // set a new values
   WCHAR szGoodOne[] = L"A test speaker";
   hRes = gpSRMode->SpeakerSet (szGoodOne);
   CHECKFAILED ("SpeakerSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_SPEAKER))
      Failed ("Didn't get AttribChanged notification");

   // verify
   hRes = gpSRMode->SpeakerGet(szMic2, sizeof(szMic2), &dwNeeded);
   CHECKFAILED ("SpeakerSet");
   if (wcscmp(szGoodOne, szMic2))
      Failed ("Didn't recall previous value");

   // make sure it requires the right size
   hRes = gpSRMode->SpeakerGet (NULL, 0, &dwNeeded);
   if (hRes != E_INVALIDARG && hRes != SRERR_NOTENOUGHDATA)
      CHECKFAILED ("SpeakerGet");
   if (dwNeeded != sizeof(szGoodOne))
      Warning ("Size-needed is not set optimally");
   hRes = gpSRMode->SpeakerGet (szMic2, sizeof(szGoodOne)-2, &dwNeeded);
   if (hRes != E_INVALIDARG  && hRes != SRERR_NOTENOUGHDATA)
      CHECKFAILED ("SpeakerGet");
   if (dwNeeded != sizeof(szGoodOne))
      Warning ("Size-needed is not set optimally");

   // out of range
   hRes = gpSRMode->SpeakerSet (
      L"This is a very long Speaker string and I just want "
      L"to make sure that it doesn't crash the speech engien "
      L"when it gets such a long string.");

   // make sure I can do a NULL speaker, to go back to default
   hRes = gpSRMode->SpeakerSet (L"");
   CHECKFAILED ("SpeakerSet");

   // invalid get param
   hRes = gpSRMode->SpeakerGet (NULL, 0, NULL);
   if (!hRes)
      Failed ("Didn't detect NULL parameter");

   // return to original value
   hRes = gpSRMode->SpeakerSet (szMic);
   CHECKFAILED ("SpeakerSet");
   hRes = gpSRMode->SpeakerGet(szMic2, sizeof(szMic2), &dwNeeded);
   CHECKFAILED ("SpeakerSet");
   if (wcscmp(szMic, szMic2))
      Failed ("Didn't recall previous value");

   Passed();
}


// - SpeakerGet/Set - Set the value, and then see if ISRSpeaker has the same value
// - SpeakerGet/Set - Set the speaker through ISRSpeaker, and see if SpeakerGet has the same value
void CALLBACK Test075 (void)
{
   NEEDSRINTERFACE (SRI_ISRSPEAKER);

   // get the current value
   HRESULT  hRes;
   DWORD    dwNeeded;
   WCHAR szSpeaker[256], szSpeaker2[256];
   
   hRes = gpSRMode->SpeakerGet(szSpeaker, sizeof(szSpeaker), &dwNeeded);
   CHECKFAILED ("SpeakerGet");
   hRes = gpSRMode->Query (szSpeaker2, sizeof(szSpeaker), &dwNeeded);
   if (hRes == SRERR_NOUSERSELECTED) {
      szSpeaker2[0] = 0;
      hRes = NOERROR;
   }
   CHECKFAILED ("Query");
   if (wcscmp(szSpeaker, szSpeaker2))
      Failed ("SpeakerGet is out of sync with ISRSpeaker::Query");

   // make a unique name up
   char  szTemp[64];
   WCHAR szUnique[64];
   wsprintf (szTemp, "s %d", (int) rand());
   MBTWC (szTemp, szUnique);

   // make sure it doesn't exist
   gpSRMode->Delete (szUnique);

   // set the speaker
   hRes = gpSRMode->SpeakerSet (szUnique);
   CHECKFAILED ("SpeakerSet");
   hRes = gpSRMode->Query (szSpeaker2, sizeof(szSpeaker2), &dwNeeded);
   CHECKFAILED ("Query");
   if (wcscmp(szUnique, szSpeaker2))
      Failed ("SpeakerGet is out of sync with ISRSpeaker::Query");

   // make a unique name up
   wsprintf (szTemp, "s %d", (int) rand());
   MBTWC (szTemp, szUnique);

   // make sure it doesn't exist
   gpSRMode->Delete (szUnique);

   // set the speaker
   hRes = gpSRMode->Select (szUnique, FALSE);
   CHECKFAILED ("Select");
   hRes = gpSRMode->SpeakerGet (szSpeaker2, sizeof(szSpeaker2), &dwNeeded);
   CHECKFAILED ("Query");
   if (wcscmp(szUnique, szSpeaker2))
      Failed ("SpeakerGet is out of sync with ISRSpeaker::Query");

   // restore the speaker
   hRes = gpSRMode->SpeakerSet (szSpeaker);
   CHECKFAILED ("SpeakerSet");

   Passed();

}



// - TimeOutGet/Set - Set and see if engine remembers.
// - TimeOutGet/Set - Invalid parameters
void CALLBACK Test076 (void)
{
   // get the current value
   HRESULT  hRes;
   DWORD    dwVal;
   DWORD    dwTestThreshold;
   hRes = gpSRMode->ThresholdGet(&dwVal);
   if (hRes == SRERR_NOTSUPPORTED) {
      Warning ("Threshold not supported");
      return;
   }
   else
      CHECKFAILED ("ThresholdGet");
   if (dwVal > 100)
      Failed ("Value out of range");

   // check to see if the attribute changed
   gSRNS.m_fAttribChanged = FALSE;
   dwTestThreshold = dwVal < 100 ? dwVal + 1 : 0;

   // set a new values
   hRes = gpSRMode->ThresholdSet (dwTestThreshold);
   CHECKFAILED ("ThresholdSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_THRESHOLD))
      Failed ("Didn't get AttribChanged notification");

   // verify
   if (dwTestThreshold != gpSRMode->ThresholdGet())
      Failed ("Didn't recall previous value");

   // new value
   hRes = gpSRMode->ThresholdSet (90);
   CHECKFAILED ("ThresholdSet");
   if (90 != gpSRMode->ThresholdGet())
      Failed ("Didn't recall previous value");


   // out of range
   hRes = gpSRMode->ThresholdSet (101);
   if (!hRes)
      Failed ("Didn't error out when set invalid value");
   if (90 != gpSRMode->ThresholdGet())
      Failed ("Didn't recall previous value");

   // invalid get param
   hRes = gpSRMode->ThresholdGet (NULL);
   if (!hRes)
      Failed ("Didn't detect NULL parameter");

   // return to original value
   hRes = gpSRMode->ThresholdSet (dwVal);
   CHECKFAILED ("ThresholdSet");
   if (dwVal != gpSRMode->ThresholdGet())
      Failed ("Didn't recall previous value");

   Passed();
}

// - ThresholdGet/Set - Have a grammar with a phrase, and a number of similar sounding test
// 	phrases to test the threshold. See if varying the threshhold affects the rejections
void CALLBACK Test077 (void)
{
   NEEDENGLISH;
   NEEDAUDIO;
   NEEDCFG;

   DWORD dwOrig;
   HRESULT hRes;
   // get the original value
   hRes = gpSRMode->ThresholdGet (&dwOrig);
   if (hRes) {
      NotApplicable ("ThresholdGet");
      return;
   }

#define  TESTREJECT(x)  \
      dwTotal++; \
      psz = SpeakAndListen (x, gszAllWorkGrammar, FALSE); \
      if (!psz) \
         dwRejected++;  \
      else if (!psz[0]) \
         dwRejected++; \

   // loop through some values
   DWORD dwThresh;
   DWORD dwLastRejected = 0;
   for (dwThresh = 0; dwThresh <= 100; dwThresh += 25) {
      // set the threshhold
      hRes = gpSRMode->ThresholdSet (dwThresh);
      CHECKFAILED ("ThresholdSet");

      // count the number of rejected phrases
      DWORD dwRejected = 0;
      DWORD dwTotal = 0;
      char  *psz;

      // the correct phrase
      TESTREJECT (gszAllWork);
      
      // special case for the correct phrase. Even with TTS
      // speaking, it shouldn't be rejected at setting of 50 or below
      if (!psz || (!psz[0] && (dwThresh <= 50)))
         Failed ("Correct response rejected");

      // try other prompts
      TESTREJECT ("all work and no play makes jack the dull boy");
      TESTREJECT ("call work and no play makes jack the dull boy");
      TESTREJECT ("call work and go play makes jack the dull boy");
      TESTREJECT ("call work and go play makes zack the dull boy");
      TESTREJECT ("call work and go stay makes zack the dull boy");
      TESTREJECT ("call her and go stay makes zack the dull boy");
      TESTREJECT ("call her then go stay makes zack the dull boy");
      TESTREJECT ("call her then go stay math zack the dull boy");
      TESTREJECT ("call her then go stay math zack the old boy");
      TESTREJECT ("call her then go stay math zack the old buoy");
      TESTREJECT ("this is a completely different sentence");
      TESTREJECT ("a shorter sentence");
      TESTREJECT ("short");
      TESTREJECT ("this is a rather longer sentence to see if the rejection works well on longer sentences or not");

      char  szTemp[256];
      wsprintf (szTemp, "*** Rejected %d out of %d at Rejection Setting = %d", (int) dwRejected, (int) dwTotal, (int) dwThresh);
      Information (szTemp);
      Information ();

      // if we're at 100% threshhold, then expect almost everything
      // to be rejected
      if ((dwThresh >= 95) && (dwRejected < (dwTotal-1) ))
         Warning ("At 100% rejection and it's not rejecting just about everything");

      // if the threshhold is near 0, then expect alsmot everything to be accepted
      if ((dwThresh <= 5) && (dwRejected*3 > dwTotal ))
         Warning ("At rejection setting 0 it's rejecting more sentences than expected.");

      // if we've rejected less than last time then error
      if (dwRejected < dwLastRejected)
         Failed ("Increased the rejection threshold and it rejected less");

      dwLastRejected = dwRejected;
   }


   // restore the original value
   hRes = gpSRMode->ThresholdSet (dwOrig);
   CHECKFAILED ("ThresholdSet");

   Passed();
}

// - TimeOutGet/Set - Set and see if engine remembers.
// - TimeOutGet/Set - Invalid parameters
void CALLBACK Test078 (void)
{
   // get the current value
   HRESULT  hRes;
   DWORD    dwValA, dwValB;
   hRes = gpSRMode->TimeOutGet(&dwValA, &dwValB);
   if (hRes == SRERR_NOTSUPPORTED) {
      Warning ("TimeOut not supported");
      return;
   }
   else
      CHECKFAILED ("TimeOutGet");

   // check to see if the attribute changed
   gSRNS.m_fAttribChanged = FALSE;

   // set a new values
   hRes = gpSRMode->TimeOutSet (750, 500);
   CHECKFAILED ("TimeOutSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_TIMEOUT))
      Failed ("Didn't get AttribChanged notification");

   // verify
   DWORD dwValA2, dwValB2;
   hRes = gpSRMode->TimeOutGet (&dwValA2, &dwValB2);
   CHECKFAILED ("TimeOutGet");
   if ((750 != dwValA2) || (500 != dwValB2))
      Failed ("Didn't recall previous value");

   // invalid get param
   hRes = gpSRMode->TimeOutGet (NULL, NULL);
   if (!hRes)
      Failed ("Didn't detect NULL parameter");
   hRes = gpSRMode->TimeOutGet (&dwValA2, &dwValB2);
   CHECKFAILED ("TimeOutGet");
   if ((750 != dwValA2) || (500 != dwValB2))
      Failed ("Didn't recall previous value");

   // return to original value
   hRes = gpSRMode->TimeOutSet (dwValA, dwValB);
   CHECKFAILED ("TimeOutSet");
   hRes = gpSRMode->TimeOutGet (&dwValA2, &dwValB2);
   CHECKFAILED ("TimeOutGet");
   if ((dwValA != dwValA2) || (dwValB != dwValB2))
      Failed ("Didn't recall previous value");

   Passed();
}


// - TimeOutGet/Set - Set to different values and see if it really does affect the time out.
//	Like it's supposd to.
void CALLBACK Test079 (void)
{
   NEEDENGLISH;
   NEEDCFG;
   NEEDAUDIO;

   // get the current value
   HRESULT  hRes;
   DWORD dwValA, dwValB;
   hRes = gpSRMode->TimeOutGet(&dwValA, &dwValB);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable ("TimeOut not supported");
      return;
   }
   else
      CHECKFAILED ("TimeOutGet");

   DWORD dwPause, dwGrammar, dwTimeOutIncomplete, dwTimeOutComplete;

   /* There is no need to check the whole matrix
    * simply test the boundary conditions. 
    */
   for (dwPause = 500; dwPause <= 2000; dwPause += 500)
   for (dwTimeOutIncomplete = dwPause - 250; dwTimeOutIncomplete <= dwPause + 250; dwTimeOutIncomplete += 500)
   for (dwTimeOutComplete = dwPause - 250; (dwTimeOutComplete <= dwTimeOutIncomplete); dwTimeOutComplete += 500)
   for (dwGrammar = 0; dwGrammar < 2; dwGrammar++)
   {
      char  szTemp[256];

      // The test is ill defined for these conditions and depends on how
      // engines handle silence which is not specified. The purpose of the
      // setting these timeouts is to control how long speakers have to
      // complete or wait for an utterance
      if ((dwPause < dwTimeOutComplete) && (dwGrammar == 0))
	 continue;

      // display info to the user
      wsprintf (szTemp,
         "Pause = %d, grammar=%s, incomplete time out=%d, complete time out=%d",
         (int) dwPause, dwGrammar ? "Any # of digits" : "Only 4 digits",
         (int) dwTimeOutIncomplete, (int) dwTimeOutComplete);
      Information ();
      Information (szTemp);

      // Update the engine to the current test settings
      gpSRMode->TimeOutSet(dwTimeOutIncomplete, dwTimeOutComplete);

      // load in the audio
      wsprintf (szTemp, "nine one eight two \\pau=%d\\ five six seven eight", (int) dwPause);
      hRes = SpeakToSR (szTemp);
      CHECKFAILED ("SpeakToSR");

      // set the grammar
      switch (dwGrammar) {
      case 0:
         // grammar with only 4 digits
         hRes = SetMainGrammar (
            "[<Start>]\n"
            "<Start>=<0..9> <0..9> <0..9> <0..9>\n"
            );
         break;
      case 1:
         // grammar with arbitrary # of digits
         hRes = SetMainGrammar (
            "[<Start>]\n"
            "<Start>=[1+] <0..9>\n"
            );
         break;
      }
      CHECKFAILED ("SetMainGrammar");

      // what should we expect? two phrase or one
      BOOL  fExpectTwoPhrases;
      BOOL  fExpectRejectFirst;
      DWORD dwTimeOutUsed;
      switch (dwGrammar) {
      case 0:
         // only four digits, so lookin at timeout complete
         dwTimeOutUsed = dwTimeOutComplete;
         break;
      case 1:
         // more than four digits, so lookin at time out incomplete
         dwTimeOutUsed = dwTimeOutIncomplete;
         break;
      }
      fExpectTwoPhrases = (dwTimeOutUsed < dwPause);
      fExpectRejectFirst = !fExpectTwoPhrases && (dwGrammar == 0);
         // will reject if 4-digit grammar hears 8 digits

      // activate
      hRes = gpSRGramComp->Activate();
      CHECKFAILED ("Activate");

      // wait for the first recognition result
      BOOL  *pf;
      pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;  // abort
      else if (!pf) {
         Failed ("Never got first utterance");
         goto nextinline;
      }

      // figure out if the first utterance is unrecognized,
      // has 3 spaces (4 words), or > 4 words
     
      DWORD dwSpaces;
      dwSpaces = 0;
      char  *psz;
      for (psz = gSRGramNS.m_szPhraseFinishText; *psz; psz++)
         if (*psz == ' ')
            dwSpaces++;
      DWORD dwRecog;
      if (!dwSpaces)
         dwRecog = 0;
      else if (dwSpaces < 4)
         dwRecog = 1;
      else
         dwRecog = 2;

      // wait for a second recognition result
      pf = WaitForEvent (30.0, &gSRGramNS.m_fPhraseFinish);
      if (pf == &gfAbortTest)
         return;  // abort

      // did we get the amount expected?
      if (!pf && fExpectTwoPhrases)
         Failed ("Expected to hear two phrases, but only got one");
      else if (pf && !fExpectTwoPhrases)
         Failed ("Expected to hear one phrase, but got two");
      else
         Information ("Got right # of phrases");
      if (fExpectRejectFirst && (dwRecog != 0))
         Information ("Engine accepted complete utterance after pause");
      else if (!fExpectRejectFirst && (dwRecog == 0))
         Failed ("Expected the first phrase to be valid, but it was rejected");

nextinline:
      // clear out the grammar and audio
      ClearMainGrammar();
      ClearSRAudio();
   }

   gpSRMode->TimeOutSet(dwValA, dwValB);

   // done
   Passed();
}



// - ModeGet - Invalid parameters.
void CALLBACK Test080 (void)
{
   HRESULT  hRes;

   hRes = gpSRMode->ModeGet(NULL);
   if (hRes)
      Passed();
   else
      Failed ("ModeGet didn't detect invalid parameters");
}

// - PosnGet - Make sure return approximately same time as audio dest.
// - ToFileTime - Make sure returns approximately same value as audio source
void CALLBACK Test081 (void)
{
   HRESULT hRes;

   NEEDAUDIO;

   // get the interface for the audio
   PIAUDIO  pIAudio;
   hRes = gpIAudioFileSR->QueryInterface (IID_IAudio, (LPVOID*) &pIAudio);
   CHECKFAILED ("IID_IAudio");

   // posnget
   QWORD qwTime, qwTime2;
   hRes = gpSRMode->PosnGet(&qwTime);
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
   hRes = gpSRMode->ToFileTime (&qwTime, &ft);
   hRes2 = pIAudio->ToFileTime (&qwTime, &ft2);
   if (hRes != hRes2)
      Failed ("ToFileTime", hRes);
   if (!hRes && memcmp(&ft, &ft2, sizeof(ft)))
      Failed ("Engine not returning the same file time as audio object");

   // to file time, #2
   qwTime -= 5000;
   hRes = gpSRMode->ToFileTime (&qwTime, &ft);
   hRes2 = pIAudio->ToFileTime (&qwTime, &ft2);
   if (hRes != hRes2)
      Failed ("ToFileTime", hRes);
   if (!hRes && memcmp(&ft, &ft2, sizeof(ft)))
      Failed ("Engine not returning the same file time as audio object");

   // invalid params
   hRes = gpSRMode->PosnGet(NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   hRes = gpSRMode->ToFileTime (0, NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");
   
   pIAudio->Release();
   Passed();
}


// - Register - Invalid params
void CALLBACK Test082 (void)
{
   HRESULT hRes;
   DWORD dwVal;

   // bad IID
   hRes = gpSRMode->Register (&gSRGramNS, IID_IAudio, &dwVal);
   if (!hRes)
      Failed ("Didn't detect bad interface");

   // no key
   hRes = gpSRMode->Register (&gSRNS, IID_ISRNotifySink, NULL);
   if (!hRes)
      Failed ("Didn't detect NULL parameter.");

   // null interface
   hRes = gpSRMode->Register (NULL, IID_ISRNotifySink, &dwVal);
   if (!hRes)
      Failed ("Didn't detect NULL parameter.");


   Passed();
}


// - AboutDlg - Check to see if it exists
// - GeneralDlg- Check to see if it exists
// - LexiconDlg - Check to see if it exists
// - TrnslateDlg - Check to see if it exsists
void CALLBACK Test083 (void)
{
   HRESULT hRes;

   hRes = gpSRMode->AboutDlg(NULL);
   if (hRes)
      Failed ("AboutDlg missing");

   hRes = gpSRMode->GeneralDlg(NULL);
   if (hRes)
      Failed ("GeneralDlg missing");

   hRes = gpSRMode->LexiconDlg(NULL);
   if (hRes)
      Warning ("LexiconDlg missing. Lexicon dialog is recommended.");

   hRes = gpSRMode->TrainGeneralDlg(NULL);
   if (hRes)
      Warning ("TrainGeneralDlg missing. TrainGeneralDlg is recommended.");

   hRes = gpSRMode->TrainMicDlg(NULL);
   if (hRes)
      Information ("TrainMicDlg missing.");

   Passed();
}



// - GrammarLoad - Invalid parameters
void CALLBACK Test084 (void)
{
   NEEDENGLISH;
   NEEDCFG;

   HRESULT hRes;

   // create a main grammar object using a default hello world
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   // pull out the data structure for the grammar so we have
   // some valid binary data
   PVOID pMem;
   DWORD dwMemSize;
   hRes = gpSRGramComp->GrammarDataGet (FALSE, &pMem, &dwMemSize);
   CHECKFAILED ("GrammarDataGet");

   // can free up that grammar now
   ClearMainGrammar();

   LPUNKNOWN   pUnk = NULL;
   SDATA s;
   s.pData = pMem;
   s.dwSize = dwMemSize;

   // valid test
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_CFG, s, &gSRGramNS,
      IID_ISRGramNotifySinkW, &pUnk);
   CHECKFAILED("GrammarLoad");
   if (pUnk) {
      pUnk->Release();
      pUnk = NULL;
   }

   // bad format
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_DICTATION, s, &gSRGramNS,
      IID_ISRGramNotifySinkW, &pUnk);
   if (!hRes)
      Failed ("Didn't detect bad format");
   if (pUnk) {
      pUnk->Release();
      pUnk = NULL;
   }

   // bad format
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_CFGNATIVE, s, &gSRGramNS,
      IID_ISRGramNotifySinkW, &pUnk);
   if (!hRes)
      Failed ("Didn't detect bad format");
   if (pUnk) {
      pUnk->Release();
      pUnk = NULL;
   }

   // bad format
   hRes = gpSRMode->GrammarLoad ((SRGRMFMT) 52, s, &gSRGramNS,
      IID_ISRGramNotifySinkW, &pUnk);
   if (!hRes)
      Failed ("Didn't detect bad format");
   if (pUnk) {
      pUnk->Release();
      pUnk = NULL;
   }

   // bad notification sink
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_CFG, s, &gTTSBufNS,
      IID_IAudio, &pUnk);
   if (!hRes)
      Failed ("Didn't detect bad notification sink");
   if (pUnk) {
      pUnk->Release();
      pUnk = NULL;
   }

   // bad notification sink
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_CFG, s, NULL,
      IID_ISRGramNotifySinkW, &pUnk);
   if (hRes)
      Failed ("Doesn't accept NULL notification");
   if (pUnk) {
      pUnk->Release();
      pUnk = NULL;
   }

   // bad repository for IUnk
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_CFG, s, &gSRGramNS,
      IID_ISRGramNotifySinkW, NULL);
   if (!hRes)
      Failed ("Didn't detect bad IUnknown");
   if (pUnk) {
      pUnk->Release();
      pUnk = NULL;
   }

   // bad data
   s.dwSize /= 2;
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_CFG, s, &gSRGramNS,
      IID_ISRGramNotifySinkW, &pUnk);
   if (!hRes)
      Failed ("Didn't detect bad data");
   if (pUnk) {
      pUnk->Release();
      pUnk = NULL;
   }

   // bad data
   s.pData = NULL;
   hRes = gpSRMode->GrammarLoad (SRGRMFMT_CFG, s, &gSRGramNS,
      IID_ISRGramNotifySinkW, &pUnk);
   if (!hRes)
      Failed ("Didn't detect bad data");
   if (pUnk) {
      pUnk->Release();
      pUnk = NULL;
   }

   // finally, free the data
   if (pMem)
      CoTaskMemFree (pMem);
	pMem = NULL;
   Passed();

}



// - Pause/Resume - Pause the audio part way through an utterance. Make sure that after call pause,
//	we don't get notifications from the SR engine. When call resume, restarts.
void CALLBACK Test085 (void)
{
   NEEDENGLISH;
   NEEDCFG;
   NEEDAUDIO;

   HRESULT hRes;

   // speak phrase
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // load the grammar
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   // speak & wait for phrase start
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseStart);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't hear speech");
      return;
   }

   // pause it
   hRes = gpSRMode->Pause();
   CHECKFAILED ("Pause");

   // wait and see if there's a phrase finish. there shouldn be
   pf = WaitForEvent (20.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gSRGramNS.m_fPhraseFinish) {
      Failed ("Pause didn't prevent phrase finish from being called");
      gpSRMode->Resume();
      return;
   }

   // else, resumte and wait
   hRes = gpSRMode->Resume();
   CHECKFAILED ("Resume");
   pf = WaitForEvent (20.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gSRGramNS.m_fPhraseFinish)
      Passed ("Got phraseFinished");
   else
      Failed ("Didn't get PhraseFinished");

   ClearMainGrammar();
   ClearSRAudio();
}


// - Pause/Resume - Call pause several times, and then resume once. Make sure it handles the
//	count.
void CALLBACK Test086 (void)
{
   NEEDENGLISH;
   NEEDCFG;
   NEEDAUDIO;

   HRESULT hRes;

   // speak phrase
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   // load the grammar
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   // speak & wait for phrase start
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseStart);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't hear speech");
      return;
   }

   // pause it
   hRes = gpSRMode->Pause();
   CHECKFAILED ("Pause");

   // pause it again
   hRes = gpSRMode->Pause();
   CHECKFAILED ("Pause");

   // resume it

   hRes = gpSRMode->Resume();
   CHECKFAILED ("Resume");

   // wait and see if there's a phrase finish. there shouldn be
   pf = WaitForEvent (20.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gSRGramNS.m_fPhraseFinish) {
      Failed ("Pause didn't prevent phrase finish from being called");
      gpSRMode->Resume();
      return;
   }

   // else, resumte and wait
   hRes = gpSRMode->Resume();
   CHECKFAILED ("Resume");
   pf = WaitForEvent (20.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gSRGramNS.m_fPhraseFinish)
      Passed ("Got phraseFinished");
   else
      Failed ("Didn't get PhraseFinished");

   ClearMainGrammar();
   ClearSRAudio();
}



// - Puase/Resume - Pause first, then send in audio, and see if processes.
void CALLBACK Test087 (void)
{
   NEEDENGLISH;
   NEEDCFG;
   NEEDAUDIO;

   HRESULT hRes;

   // speak phrase
   hRes = SpeakToSR ("Hello world");
   CHECKFAILED ("SpeakToSR");

   hRes = gpSRMode->Pause();
   CHECKFAILED ("Pause");

   // load the grammar
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   BOOL  *pf;

   // make sure we don't get a phrasestart
   pf = WaitForEvent (20.0, &gSRGramNS.m_fPhraseStart);
   if (pf == &gfAbortTest)
      return;
   else if (pf) {
      Failed ("Got a PhraseStart even though was paused");
      gpSRMode->Resume();
      return;
   }


   // resume
   hRes = gpSRMode->Resume();
   CHECKFAILED ("Resume");

   // speak & wait for phrase finish
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't hear speech");
      return;
   }


   ClearMainGrammar();
   ClearSRAudio();
   Passed();
}



// - Pause/Resume - Two utterances, and pause after first phrasefinish
void CALLBACK Test088 (void)
{
   NEEDENGLISH;
   NEEDCFG;
   NEEDAUDIO;

   HRESULT hRes;

   // speak phrase
   hRes = SpeakToSR ("Hello world. \\pau=4000\\ Hello world.");
   CHECKFAILED ("SpeakToSR");

   // load the grammar
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   // speak & wait for phrase finish
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't hear speech");
      return;
   }

   // pause it
   hRes = gpSRMode->Pause();
   CHECKFAILED ("Pause");

   // wait and see if there's a phrase start. there shouldn be
   pf = WaitForEvent (20.0, &gSRGramNS.m_fPhraseStart);
   if (pf == &gSRGramNS.m_fPhraseStart) {
      Failed ("Pause didn't prevent phrase start from being called");
      gpSRMode->Resume();
      return;
   }

   // else, resumte and wait
   hRes = gpSRMode->Resume();
   CHECKFAILED ("Resume");
   pf = WaitForEvent (30.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gSRGramNS.m_fPhraseFinish)
      Passed ("Got phraseFinished");
   else
      Failed ("Didn't get PhraseFinished");

   ClearMainGrammar();
   ClearSRAudio();
}




// - Pause/Resume - Two utterances, and pause after first phrasefinish
// Second also active
void CALLBACK Test089 (void)
{
   NEEDENGLISH;
   NEEDCFG;
   NEEDAUDIO;

   HRESULT hRes;

   // speak phrase
   hRes = SpeakToSR ("Hello world. \\pau=4000\\ Hello world.");
   CHECKFAILED ("SpeakToSR");

   // load the grammar
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = SetSecondaryGrammar (gszListGrammar);
   CHECKFAILED ("SetSecondaryGrammar");

   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   hRes = gpSRGramCompSecondary->Activate();
   CHECKFAILED ("Activate");

   // speak & wait for phrase finish
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't hear speech");
      return;
   }

   // pause it
   hRes = gpSRMode->Pause();
   CHECKFAILED ("Pause");

   // wait and see if there's a phrase start. there shouldn be
   pf = WaitForEvent (20.0, &gSRGramNS.m_fPhraseStart);
   if (pf == &gSRGramNS.m_fPhraseStart) {
      Failed ("Pause didn't prevent phrase start from being called");
      gpSRMode->Resume();
      return;
   }

   // else, resumte and wait
   hRes = gpSRMode->Resume();
   CHECKFAILED ("Resume");
   pf = WaitForEvent (30.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gSRGramNS.m_fPhraseFinish)
      Passed ("Got phraseFinished");
   else
      Failed ("Didn't get PhraseFinished");

   ClearMainGrammar();
   ClearSecondaryGrammar();
   ClearSRAudio();
}

// - Pause/Resume - Two utterances, and pause after first phrasefinish
// Second also active - dictation grammar
void CALLBACK Test090 (void)
{
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDCFG;
   NEEDAUDIO;

   HRESULT hRes;

   // speak phrase
   hRes = SpeakToSR ("Hello world. \\pau=4000\\ Hello world.");
   CHECKFAILED ("SpeakToSR");

   // load the grammar
   hRes = SetMainGrammar (gszHelloWorldGrammar);
   CHECKFAILED ("SetMainGrammar");

   hRes = SetSecondaryGrammar (gszDictationGrammar);
   CHECKFAILED ("SetSecondaryGrammar");

   hRes = gpSRGramComp->Activate();
   CHECKFAILED ("Activate");

   hRes = gpSRGramCompSecondary->Activate();
   CHECKFAILED ("Activate");

   // speak & wait for phrase finish
   BOOL  *pf;
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gfAbortTest)
      return;
   else if (!pf) {
      Failed ("Didn't hear speech");
      return;
   }

   // pause it
   hRes = gpSRMode->Pause();
   CHECKFAILED ("Pause");

   // wait and see if there's a phrase start. there shouldn be
   pf = WaitForEvent (20.0, &gSRGramNS.m_fPhraseStart);
   if (pf == &gSRGramNS.m_fPhraseStart) {
      Failed ("Pause didn't prevent phrase start from being called");
      gpSRMode->Resume();
      return;
   }

   // else, resumte and wait
   hRes = gpSRMode->Resume();
   CHECKFAILED ("Resume");
   pf = WaitForEvent (30.0, &gSRGramNS.m_fPhraseFinish);
   if (pf == &gSRGramNS.m_fPhraseFinish)
      Passed ("Got phraseFinished");
   else
      Failed ("Didn't get PhraseFinished");

   ClearMainGrammar();
   ClearSecondaryGrammar();
   ClearSRAudio();
}



// - Pause/Resume - Call resume without having called pause, and make sure errors.
void CALLBACK Test091 (void)
{
   HRESULT  hRes;

   hRes = gpSRMode->Resume();
   if (!hRes)
      Failed ("Resume() didn't return error when called, even when Pause not called");
   else
      Passed();
}


// - Speaker::Delete - Try with valid parameters & make sure really deletes
// - Speaker::Delete - Currently selected user
// - Speaker::Delete - Invalid paraameters
void CALLBACK Test092 (void)
{
   HRESULT  hRes;

   NEEDSRINTERFACE (SRI_ISRSPEAKER);

   // create  a new speaker
   hRes = gpSRMode->Select(L"1");
   CHECKFAILED ("Select");

   // switch to a different speaker
   hRes = gpSRMode->Select(L"2");
   CHECKFAILED ("Select");

   // delete the first one
   hRes = gpSRMode->Delete(L"1");
   CHECKFAILED ("Select");

   // make sure the user no longer exists
   WCHAR *pszOrig;
   WCHAR *psz;
   DWORD dwSize;
   hRes = gpSRMode->Enum(&pszOrig, &dwSize);
   CHECKFAILED ("Enum");
   for (psz = pszOrig; psz[0]; psz += (wcslen(psz)+1))
      if (!wcsicmp(L"1", psz))
         Failed ("User name still exists");
   CoTaskMemFree (pszOrig);
	pszOrig = NULL;

   // try deleting the current one
   hRes = gpSRMode->Delete(L"2");
   if (!hRes)
      Failed ("Didn't error when deleted selected speaker");

   // invalid parameters to delete
   hRes = gpSRMode->Delete (NULL);
   if (!hRes)
      Failed ("Didn't detect invalid parameters");

   // delete a speaker name that doesn't exist
   hRes = gpSRMode->Delete (L"This speaker does not exist");
   if (!hRes)
      Failed ("Didn't detect invalid parameters");

   // delete a very long spaker name
   hRes = gpSRMode->Delete (
      L"This is a very long speaker name that "
      L"might cause a GP fault to occurr in the recognizer "
      L"when it tries to parse it");
   if (!hRes)
      Failed ("Didn't detect invalid parameters");

   Passed();
}


// - Speaker::Enum - Test out
// - Speaker::Enum - Invalid parameters
void CALLBACK Test093 (void)
{
   HRESULT  hRes;

   NEEDSRINTERFACE (SRI_ISRSPEAKER);

   // add some users
   hRes = gpSRMode->Select(L"1");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Select(L"2");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Select(L"This is a really long name");
   CHECKFAILED ("Select");


   // make sure the users exist
   WCHAR *pszOrig;
   WCHAR *psz;
   DWORD dwSize;
   BOOL  fExist1 = FALSE;
   BOOL  fExist2 = FALSE;
   BOOL  fExistThis = FALSE;
   hRes = gpSRMode->Enum(&pszOrig, &dwSize);
   CHECKFAILED ("Enum");
   for (psz = pszOrig; psz[0]; psz += (wcslen(psz)+1)) {
      if (!wcscmp(L"1", psz)) {
         if (fExist1)
            Failed ("User name duplicated");
         fExist1 = TRUE;
      }
      if (!wcscmp(L"2", psz)) {
         if (fExist2)
            Failed ("User name duplicated");
         fExist2 = TRUE;
      }
      if (!wcscmp(L"This is a really long name", psz)) {
         if (fExistThis)
            Failed ("User name duplicated");
         fExistThis = TRUE;
      }
   }
   CoTaskMemFree (pszOrig);
   pszOrig = NULL;
   if (!fExist1 || !fExist2 || !fExistThis)
      Failed ("Not all users added are enumerated");

   // try invalid parameters
   hRes = gpSRMode->Enum(&pszOrig, NULL);
   if (!hRes) {
      Failed ("Didn't detect invalid param");
      if (pszOrig)
         CoTaskMemFree (pszOrig);
		pszOrig = NULL;
   }

   hRes = gpSRMode->Enum(NULL, &dwSize);
   if (!hRes) {
      Failed ("Didn't detect invalid param");
   }

   Passed();
}

// - Speaker::Merge - Valid parameterrs
// - SPeaker::Merge - Invalid parameters
void CALLBACK Test094 (void)
{
   HRESULT  hRes;

   NEEDSRINTERFACE (SRI_ISRSPEAKER);

   // add some users
   hRes = gpSRMode->Select(L"1");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Select(L"2");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Select(L"3");
   CHECKFAILED ("Select");

   // get the speaker data from 2
   PVOID pSpk;
   DWORD dwSize;
   hRes = gpSRMode->Read(L"1", &pSpk, &dwSize);
   CHECKFAILED ("Read");

   // try to merge this in with 2
   hRes = gpSRMode->Merge(L"2", pSpk, dwSize);
   if (hRes == SRERR_NOTSUPPORTED)
      Warning ("Doesn't support ISRSpeaker::Merge");
   else
      CHECKFAILED ("Merge");

   // try to merge in with 3, currently selected speaker
   hRes = gpSRMode->Merge(L"3", pSpk, dwSize);
   if (hRes == SRERR_NOTSUPPORTED)
      Warning ("Doesn't support ISRSpeaker::Merge");
   else
      CHECKFAILED ("Merge");

   // try to merge in with NULL name (should error out)
   hRes = gpSRMode->Merge(NULL, pSpk, dwSize);
   if (!hRes)
      Failed ("Shouldn't be able to merge in with default user");

   // try to merge in with non-existent speaker
   hRes = gpSRMode->Merge(L"This speaker does not exist", pSpk, dwSize);
   if (!hRes)
      Failed ("Shouldn't be able to merge in with non-existent speaker");

   // free the data
   CoTaskMemFree (pSpk);
	pSpk = NULL;

   // try some invalid params
   BYTE  abRandom[1000000];   // purposely leave uninitialized
   hRes = gpSRMode->Merge(L"1", abRandom, rand() % sizeof(abRandom));
   if (!hRes)
      Failed ("Shouldn't be able to merge in with random data");

   hRes = gpSRMode->Merge(L"1", abRandom, 0);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   hRes = gpSRMode->Merge(L"1", NULL, (DWORD) rand());
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   Passed();
}


//- Speaker::New - Valid parameters
//- SPeaker::New - Invalid parameters
void CALLBACK Test095 (void)
{
   HRESULT  hRes;

   NEEDSRINTERFACE (SRI_ISRSPEAKER);

   // add some users
   hRes = gpSRMode->Select(L"2");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Select(L"3");
   CHECKFAILED ("Select");

   // delete user one. Don't check the error in case it doesn't exist
   gpSRMode->Delete(L"1");

   // create a new one
   hRes = gpSRMode->New (L"1");
   CHECKFAILED ("New");

   // create a new 2 - should error
   hRes = gpSRMode->New (L"2");
   if (!hRes)
      Failed ("New didn't error out when overwriting an existing user");

   // make sure that 3 is still selected
   WCHAR szTemp[256];
   DWORD dwNeeded;
   hRes = gpSRMode->Query (szTemp, sizeof(szTemp), &dwNeeded);
   CHECKFAILED ("Query");
   if (wcscmp(szTemp, L"3"))
      Failed ("User 3 should still be selected");


   // try some invalid parameters
   hRes = gpSRMode->New (NULL);
   if (!hRes)
      Failed ("Allowed to create a new default user");

   hRes = gpSRMode->New (
      L"This is a very long speaker name for new that "
      L"might cause a GP fault to occurr in the recognizer "
      L"when it tries to parse it");
   // don't check the error - happy as lnog as it doesn't crash

   Passed();
}

// - Speaker::Query - Valid parameters
// - Speaker::Query - Invalid parameters
void CALLBACK Test096 (void)
{
   HRESULT  hRes;

   NEEDSRINTERFACE (SRI_ISRSPEAKER);

   // select "1"
   hRes = gpSRMode->Select (L"1");
   CHECKFAILED ("Select");

   // make sure get same value
   WCHAR psz[256];
   DWORD dwSize;
   hRes = gpSRMode->Query (psz, sizeof(psz), &dwSize);
   CHECKFAILED ("Query");
   if (wcscmp(L"1", psz))
      Failed ("Query doesn't have same speaker");

   // what if there's no user?
   hRes = gpSRMode->Select (NULL);
   CHECKFAILED ("Select");
   hRes = gpSRMode->Query (psz, sizeof(psz), &dwSize);
   if (hRes != SRERR_NOUSERSELECTED)
      Failed ("Expect ISRSpeaker::Query to return SRERR_NOUSERSELECTED");

   // select a longer name
   WCHAR szLongName[] = L"This is a really long name";
   hRes = gpSRMode->Select(szLongName);
   CHECKFAILED ("Select");

   // make sure get this
   hRes = gpSRMode->Query (psz, sizeof(psz), &dwSize);
   CHECKFAILED ("Query");
   if (wcscmp(szLongName, psz))
      Failed ("Query doesn't have same speaker");

   // don't provide enough space
   hRes = gpSRMode->Query (psz, sizeof(szLongName) - 1, &dwSize);
   if (!hRes)
      Failed ("ISRSpeaker::Query - Didn't detect buffer was too small");
   if (dwSize < sizeof(szLongName))
      Failed ("ISRSpeaker::Query - Didn't return right size needed");

   // try some invalid parameters
   hRes = gpSRMode->Query (NULL,  sizeof(psz), &dwSize);
   if (!hRes)
      Failed ("ISRSpeaker::Query - Didn't detect invalid parameter");
   hRes = gpSRMode->Query (NULL, sizeof(psz), NULL);
   if (!hRes)
      Failed ("ISRSpeaker::Query - Didn't detect invalid parameter");
   hRes = gpSRMode->Query (psz, 0, NULL);
   if (!hRes)
      Failed ("ISRSpeaker::Query - Didn't detect invalid parameter");

   Passed();
}


// - Speaker::Read - Valid parameters
// - SPeaker::read - Invalid parameters
void CALLBACK Test097 (void)
{
   HRESULT  hRes;

   NEEDSRINTERFACE (SRI_ISRSPEAKER);

   // select "1"
   hRes = gpSRMode->Select (L"1");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Select (L"2");
   CHECKFAILED ("Select");
   
   // delete user "3" just in case there
   gpSRMode->Delete (L"3");

   // get the user
   PVOID pMem;
   DWORD dwSize;
   hRes = gpSRMode->Read (L"1", &pMem, &dwSize);
   CHECKFAILED ("Read");
   CoTaskMemFree (pMem);
	pMem = NULL;

   // try again with "2"
   hRes = gpSRMode->Read (L"2", &pMem, &dwSize);
   CHECKFAILED ("Read");

   // make sure can write this data back
   hRes = gpSRMode->Write (L"3", pMem, dwSize);
   CHECKFAILED ("Write");

   // try writing onto 1
   hRes = gpSRMode->Write (L"1", pMem, dwSize);
   if (!hRes)
      Failed ("Should have errored out when writing over existing user");

   // free memory
   CoTaskMemFree (pMem);
	pMem = NULL;

   // invalid parameters
   hRes = gpSRMode->Read (L"This user does not exist", &pMem, &dwSize);
   if (!hRes)
      Failed ("Didn't fail on a non-existant user");
   hRes = gpSRMode->Read (L"This user does not exist", &pMem, NULL);
   if (!hRes)
      Failed ("Didn't fail on invalid parameter");
   hRes = gpSRMode->Read (L"This user does not exist", NULL, &dwSize);
   if (!hRes)
      Failed ("Didn't fail on invalid parameter");

   // make sure can read default user
   hRes = gpSRMode->Read (NULL, &pMem, &dwSize);
   CHECKFAILED ("Read");
   CoTaskMemFree (pMem);
	pMem = NULL;

   Passed();

}


// - Speaker::revert - Valid parameters
// - SPeaker::revert - Invalid parameters
void CALLBACK Test098 (void)
{
   HRESULT  hRes;

   NEEDSRINTERFACE (SRI_ISRSPEAKER);

   // select "1"
   hRes = gpSRMode->Select (L"1");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Select (L"2");
   CHECKFAILED ("Select");
   
   // try reverting
   hRes = gpSRMode->Revert (L"1");
   if (hRes == SRERR_NOTSUPPORTED) {
      Warning ("Revert not supported");
      return;
   }
   CHECKFAILED ("Revert");
   hRes = gpSRMode->Revert (L"2");
   CHECKFAILED ("Revert");

   // invalid params
   hRes = gpSRMode->Revert (NULL);
   if (!hRes)
      Failed ("Didn't check invalid param");
   hRes = gpSRMode->Revert (L"This user does not exist");
   if (!hRes)
      Failed ("Didn't check invalid param");

   Passed();
}


// - Speaker::Write - Valid parameterrs
// - SPeaker::Write - Invalid parameters
void CALLBACK Test099 (void)
{
   HRESULT  hRes;

   NEEDSRINTERFACE (SRI_ISRSPEAKER);

   // add some users
   hRes = gpSRMode->Select(L"1");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Select(L"2");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Select(L"3");
   CHECKFAILED ("Select");

   // get the speaker data from 2
   PVOID pSpk;
   DWORD dwSize;
   hRes = gpSRMode->Read(L"1", &pSpk, &dwSize);
   CHECKFAILED ("Read");

   // delete 2
   gpSRMode->Delete(L"2");

   // try to write this in with 2
   hRes = gpSRMode->Write(L"2", pSpk, dwSize);
   CHECKFAILED ("Write");

   // try to Write in with 3, currently selected speaker
   hRes = gpSRMode->Write(L"3", pSpk, dwSize);
   if (!hRes)
      Failed ("Should error out when overwriting existing speaker");

   // try to Write in with NULL name (should error out)
   hRes = gpSRMode->Write(NULL, pSpk, dwSize);
   if (!hRes)
      Failed ("Shouldn't be able to Write in with default user");

   // free the data
   CoTaskMemFree (pSpk);
	pSpk = NULL;
   gpSRMode->Delete (L"1");

   // try some invalid params
   BYTE  abRandom[1000000];   // purposely leave uninitialized
   hRes = gpSRMode->Write(L"1", abRandom, rand() % sizeof(abRandom));
   if (!hRes)
      Failed ("Shouldn't be able to Write in with random data");

   hRes = gpSRMode->Write(L"1", abRandom, 0);
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   hRes = gpSRMode->Write(L"1", NULL, (DWORD) rand());
   if (!hRes)
      Failed ("Didn't detect invalid parameter");

   Passed();
}
