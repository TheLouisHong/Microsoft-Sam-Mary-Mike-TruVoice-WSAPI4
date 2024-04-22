/* Test3.cpp - Tests 200-299 */


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

// check handling of grammer with symbols as words
void CALLBACK Test200 (void)
{

   NEEDCFG;
   NEEDENGLISH;
   NEEDAUDIO;

   char  szSymbolGrammar[] =
      "[<Start>]\n"
      "<Start>=?\n" "<Start>=question mark\n"
  		"<Start>=.\n" "<Start>=point\n"
  		"<Start>=,\n" "<Start>=comma\n"
  		"<Start>=!\n" "<Start>=explanation point\n"
  		"<Start>=@\n" "<Start>=at\n"
  		"<Start>=#\n" "<Start>=number\n"
  		"<Start>=$\n" "<Start>=dollar sign\n"
  		"<Start>=%\n" "<Start>=percent\n"
  		"<Start>=^\n" "<Start>=cap\n"
  		"<Start>=&\n" "<Start>=and\n"
  		"<Start>=(\n" "<Start>=left parenthesis\n"
  		"<Start>=)\n" "<Start>=right parenthesis\n"
  		"<Start>=_\n" "<Start>=underscore\n"
  		"<Start>==\n" "<Start>=equals\n"
  		"<Start>=<\n" "<Start>=is less than\n"
  		"<Start>=>\n" "<Start>=is greater than\n"
  		"<Start>=;\n" "<Start>=semi colon\n"
  		"<Start>=:\n" "<Start>=colon\n"
		;
 
	char  *psz;

   psz = SpeakAndListen ("?", szSymbolGrammar);
   if (!psz || (stricmp(psz, "question mark") && stricmp(psz, "?")))
      Failed ("Didn't recognize question mark");

   psz = SpeakAndListen (".", szSymbolGrammar);
   if (!psz || (stricmp(psz, "point") && stricmp(psz, ".")))
      Failed ("Didn't recognize point");

   psz = SpeakAndListen (",", szSymbolGrammar);
   if (!psz || (stricmp(psz, "comma") && stricmp(psz, ",")))
      Failed ("Didn't recognize commaa");

   psz = SpeakAndListen ("!", szSymbolGrammar);
   if (!psz || (stricmp(psz, "explanation point") && stricmp(psz, "!")))
      Failed ("Didn't recognize explanation point");

   psz = SpeakAndListen ("@", szSymbolGrammar);
   if (!psz || (stricmp(psz, "at") && stricmp(psz, "@")))
      Failed ("Didn't recognize explanation @ as at");

   psz = SpeakAndListen ("#", szSymbolGrammar);
   if (!psz || (stricmp(psz, "number") && stricmp(psz, "#")))
      Failed ("Didn't recognize explanation # as number");

   psz = SpeakAndListen ("$", szSymbolGrammar);
   if (!psz || (stricmp(psz, "dollar sign") && stricmp(psz, "$")))
      Failed ("Didn't recognize $ as dollar sign");

   psz = SpeakAndListen ("%", szSymbolGrammar);
   if (!psz || (stricmp(psz, "percent") && stricmp(psz, "%")))
      Failed ("Didn't recognize % as percent");

   psz = SpeakAndListen ("^", szSymbolGrammar);
   if (!psz || (stricmp(psz, "cap") && stricmp(psz, "^")))
      Failed ("Didn't recognize ^ as cap");

   psz = SpeakAndListen ("&", szSymbolGrammar);
   if (!psz || (stricmp(psz, "and") && stricmp(psz, "&")))
      Failed ("Didn't recognize & as and");

   psz = SpeakAndListen ("(", szSymbolGrammar);
   if (!psz || (stricmp(psz, "left parenthesis") && stricmp(psz, "(")))
      Failed ("Didn't recognize ( as left parenthesis");

   psz = SpeakAndListen (")", szSymbolGrammar);
   if (!psz || (stricmp(psz, "right parenthesis") && stricmp(psz, ")")))
      Failed ("Didn't recognize ) as right parenthesis");

   psz = SpeakAndListen ("_", szSymbolGrammar);
   if (!psz || (stricmp(psz, "underscore") && stricmp(psz, "_")))
      Failed ("Didn't recognize _ as underscore");

   psz = SpeakAndListen ("=", szSymbolGrammar);
   if (!psz || (stricmp(psz, "equals") && stricmp(psz, "=")))
      Failed ("Didn't recognize = as equals");

   psz = SpeakAndListen ("<", szSymbolGrammar);
   if (!psz || (stricmp(psz, "is less than") && stricmp(psz, "<")))
      Failed ("Didn't recognize < as is less than");

   psz = SpeakAndListen (">", szSymbolGrammar);
   if (!psz || (stricmp(psz, "is greater than") && stricmp(psz, ">")))
      Failed ("Didn't recognize > as is greater than");

   psz = SpeakAndListen (";", szSymbolGrammar);
   if (!psz || (stricmp(psz, "semi colon") && stricmp(psz, ";")))
      Failed ("Didn't recognize ; as semi colon");

   psz = SpeakAndListen (":", szSymbolGrammar);
   if (!psz || (stricmp(psz, "colon") && stricmp(psz, ":")))
      Failed ("Didn't recognize : as colon");

	Passed();
}

// ISRNotifySink - check for crash on destroy if registered
void CALLBACK Test201 (void)
{
   NEEDAUDIO;
   NEEDCFG;
   NEEDENGLISH;
   NEEDSRINTERFACE (SRI_ISRSPEAKER);

   HRESULT  hRes;

   // register a secong notification sink
   CFuncSRNotifySink nsnew;
   DWORD dwKey;
   hRes = gpSRMode->Register (&nsnew, &dwKey);
   CHECKFAILED ("Register");

	// make sure doesn't crash if destroy engine before without unregistering
   DestroySREngine();

    // create a new engine
   CreateSREngine();

   // Passes if it hasn't crashed
   Passed();
}


// - AutoGainEnableSet & Get - See if set value to min and max correctly
void CALLBACK Test202 (void)
{
   // get the current value
   HRESULT  hRes;
   DWORD    dwVal, dwMin, dwMax;
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

   // set a min values
   hRes = gpSRMode->AutoGainEnableSet (SRATTR_MINAUTOGAIN);
   CHECKFAILED ("AutoGainEnableSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_AUTOGAINENABLE))
      Failed ("Didn't get AttribChanged notification");

   // verify
	hRes = gpSRMode->AutoGainEnableGet(&dwMin);
	if( hRes )
		Failed ("Failed to return AutoGainEnableGet",hRes);
	else {
	   if ( gpSRMode->AutoGainEnableGet() != dwMin )
			Failed ("Didn't recall min value");
	}

   // set to max value
   hRes = gpSRMode->AutoGainEnableSet (SRATTR_MAXAUTOGAIN);
   CHECKFAILED ("AutoGainEnableSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_AUTOGAINENABLE))
      Failed ("Didn't get AttribChanged notification");
	
	// verify
	hRes = gpSRMode->AutoGainEnableGet(&dwMax);
	if( hRes )
		Failed ("Didn't recall min value");
	else {
		if (gpSRMode->AutoGainEnableGet() != dwMax )
			Failed ("Didn't recall max value");
	}

   // out of range
	if( dwMin > 0 ) {
		hRes = gpSRMode->AutoGainEnableSet (dwMin - 1);
		if( !hRes )
			Failed ("Didn't error out when set invalid value");
	}

	if( dwMax < (DWORD) -1 ) {
		hRes = gpSRMode->AutoGainEnableSet (dwMax + 1);
		if( !hRes )
			Failed ("Didn't error out when set invalid value");
	}

   // return to original value
   hRes = gpSRMode->AutoGainEnableSet (dwVal);
   CHECKFAILED ("AutoGainEnableSet");
   if (dwVal != gpSRMode->AutoGainEnableGet())
      Failed ("Didn't recall previous value");

   Passed();
}

// - EnergyFloorSet/Get - Set to min/max magic numbers.
void CALLBACK Test203 (void)
{
   // get the current value
   HRESULT  hRes;
   WORD     wVal, wMinVal, wMaxVal;
   hRes = gpSRMode->EnergyFloorGet(&wVal);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable ("EnergyFloor not supported");
      return;
   }
   else
      CHECKFAILED ("EnergyFloorGet");
 
   // check to see if the attribute changed
   gSRNS.m_fAttribChanged = FALSE;

   // get min values
   hRes = gpSRMode->EnergyFloorSet (SRATTR_MINENERGYFLOOR);
   CHECKFAILED ("EnergyFloorSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_ENERGYFLOOR))
      Failed ("Didn't get AttribChanged notification");

   // verify
   hRes = gpSRMode->EnergyFloorGet(&wMinVal);
	if (gpSRMode->EnergyFloorGet() != wMinVal)
      Failed ("Didn't recall previous value");

	// new value
   hRes = gpSRMode->EnergyFloorSet (SRATTR_MAXENERGYFLOOR);
   CHECKFAILED ("EnergyFloorSet");

	hRes = gpSRMode->EnergyFloorGet(&wMaxVal);
   if (gpSRMode->EnergyFloorGet() != wMaxVal)
      Failed ("Didn't recall previous value");

	// sanity check min and max
	if( wMinVal >= wMaxVal )
		Failed ("Min is >= Max");

	// check for out of range
	if( wMinVal > 0 ) {
		hRes = gpSRMode->EnergyFloorSet (wMinVal - 1);
			if( !hRes )
				Failed ("Failed to error on out of range value");
	}

	if( wMaxVal < (WORD) -1 ) {
		hRes = gpSRMode->EnergyFloorSet (wMaxVal + 1);
			if( !hRes )
				Failed ("Failed to error on out of range value");
	}

   // return to original value
   hRes = gpSRMode->EnergyFloorSet (wVal);
   CHECKFAILED ("EnergyFloorSet");
   if (wVal != gpSRMode->EnergyFloorGet())
      Failed ("Didn't recall previous value");

   Passed();
}


// - RealTimeGet/Set - Make sure that set default values 
void CALLBACK Test204 (void)
{
   HRESULT hRes;
   DWORD  dwMinTime, dwMaxTime, dwRealTime;
   hRes = gpSRMode->RealTimeGet(&dwRealTime);
   if (hRes == SRERR_NOTSUPPORTED) {
      NotApplicable();
      return;
   }
   CHECKFAILED("RealTimeGet");

   // set value to minimum
   gSRNS.m_fAttribChanged = 0;
   hRes = gpSRMode->RealTimeSet (SRATTR_MINREALTIME );
   CHECKFAILED("RealTimeSet");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_REALTIME))
      Failed ("Didn't get RealTime change notification");

   hRes = gpSRMode->RealTimeGet (&dwMinTime);
   CHECKFAILED("RealTimeSet");

   // set value to maximum
   gSRNS.m_fAttribChanged = 0;
   hRes = gpSRMode->RealTimeSet (SRATTR_MAXREALTIME);
   CHECKFAILED("RealTimeSet");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_REALTIME))
      Failed ("Didn't get RealTime change notification");

   hRes = gpSRMode->RealTimeGet (&dwMaxTime);
   CHECKFAILED("RealTimeSet");

	// sanity check of min and max values
	if( dwMinTime >= dwMaxTime )
		Failed ("MinRealTime was >= MaxRealTime" );

	// check for out of range
   // Since SRATTR_MINREALTIME is defined as 0, dwMinTime - 1 is
   // always valid when dwMinTime is 1.  So we only test the MINREALTIME
   // boundary if it is something other than 1.
	if( dwMinTime > 1 ) {
		hRes = gpSRMode->RealTimeSet (dwMinTime - 1);
		if( !hRes )
			Failed ("Failed to return error for value out of range");
	}

   // Since SRATTR_MAXREALTIME is defined as 0xffffffff, dwMaxTime + 1 is
   // always valid when dwMaxTime is 0xfffffffe.  So we only test the MAXREALTIME
   // boundary if it is something less than 0xfffffffe.
	if( dwMaxTime < SRATTR_MAXREALTIME - 1 ) {
		hRes = gpSRMode->RealTimeSet (dwMaxTime + 1);
		if( !hRes )
			Failed ("Failed to return error for value out of range");
	}

   // restore
   hRes = gpSRMode->RealTimeSet (dwRealTime);
   CHECKFAILED("RealTimeSet");

   Passed();
}


// - ThreasholdGet/Set - get set and get of min/max
void CALLBACK Test205 (void)
{
   // get the current value
   HRESULT  hRes;
   DWORD    dwVal,dwMinVal,dwMaxVal;
   hRes = gpSRMode->ThresholdGet(&dwVal);
   if (hRes == SRERR_NOTSUPPORTED) {
      Warning ("Threshold not supported");
      return;
   }
   else
      CHECKFAILED ("ThresholdGet");
   if (dwVal > 100)
      Failed ("Value out of range");

   // dwVal should be zero since we set the threshold to zero 
   // whenever we create an engine so that rejection won't
   // interfere with testing.  However, here we want it set
   // to the expected default of 50 before testing the min/max
   // boundary conditions.
   if (dwVal == 0) {
      hRes = gpSRMode->ThresholdSet (50);
      CHECKFAILED ("ThresholdSet");
   }

   // check to see if the attribute changed
   gSRNS.m_fAttribChanged = FALSE;

   // set a min value
   hRes = gpSRMode->ThresholdSet (SRATTR_MINTHRESHOLD);
   CHECKFAILED ("ThresholdSet");

   WaitForEvent(1.0);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_THRESHOLD))
      Failed ("Didn't get AttribChanged notification");

	hRes = gpSRMode->ThresholdGet(&dwMinVal);
   CHECKFAILED ("ThresholdGet");

   // verify
   if (gpSRMode->ThresholdGet() != dwMinVal)
      Failed ("Didn't recall previous value");

   // set a max value
   hRes = gpSRMode->ThresholdSet (SRATTR_MAXTHRESHOLD);
   CHECKFAILED ("ThresholdSet");

	hRes = gpSRMode->ThresholdGet(&dwMaxVal);
   CHECKFAILED ("ThresholdGet");

	if (gpSRMode->ThresholdGet() != dwMaxVal)
      Failed ("Didn't recall previous value");

	// sanity check min and max
	if( dwMinVal >= dwMaxVal )
		Failed ("Min is >= to Max");

   // out of range tests
	if( dwMinVal > 1 ) {
		hRes = gpSRMode->ThresholdSet (dwMinVal - 1);
		if (!hRes)
			Failed ("Didn't error out when set invalid value");
	}

	if( dwMaxVal < (DWORD) -1 ) {
		hRes = gpSRMode->ThresholdSet (dwMaxVal + 1);
		if (!hRes)
			Failed ("Didn't error out when set invalid value");
	}

   // return to original value
   hRes = gpSRMode->ThresholdSet (dwVal);
   CHECKFAILED ("ThresholdSet");
   if (dwVal != gpSRMode->ThresholdGet())
      Failed ("Didn't recall previous value");

   Passed();
}

// - TimeOutGet/Set - check min/max magic number setting.
void CALLBACK Test206 (void)
{
   // get the current value
   HRESULT  hRes;
	DWORD		dwValA, dwValB, dwValA2, dwValB2;
   DWORD    dwMinComVal, dwMinIncVal, dwMaxComVal, dwMaxIncVal;
   hRes = gpSRMode->TimeOutGet(&dwValA, &dwValB);
   if (hRes == SRERR_NOTSUPPORTED) {
      Warning ("TimeOut not supported");
      return;
   }
   else
      CHECKFAILED ("TimeOutGet");

   // check to see if the attribute changed
   gSRNS.m_fAttribChanged = FALSE;

   // set min values
   hRes = gpSRMode->TimeOutSet (SRATTR_MINTOINCOMPLETE, SRATTR_MINTOCOMPLETE);
   CHECKFAILED ("TimeOutSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_TIMEOUT))
      Failed ("Didn't get AttribChanged notification");

   hRes = gpSRMode->TimeOutGet (&dwMinIncVal, &dwMinComVal);
   CHECKFAILED ("TimeOutSet");

   // set max values
   hRes = gpSRMode->TimeOutSet (SRATTR_MAXTOINCOMPLETE, SRATTR_MAXTOCOMPLETE);
   CHECKFAILED ("TimeOutSet");

   WaitForEvent(.5);
   if (!gSRNS.m_fAttribChanged ||
      (gSRNS.m_dwAttribChangedAttrib != ISRNSAC_TIMEOUT))
      Failed ("Didn't get AttribChanged notification");

   hRes = gpSRMode->TimeOutGet (&dwMaxIncVal, &dwMaxComVal);
   CHECKFAILED ("TimeOutSet");

	// sanity check min and max
	if( dwMinIncVal >= dwMaxIncVal )
		Failed ("Min Incomplete >= max incomplete");
	if( dwMinComVal >= dwMaxComVal )
		Failed ("Min Complete >= max complete");

	// check out of range
	if( dwMinIncVal > 0 ) {
	   hRes = gpSRMode->TimeOutSet (dwMinIncVal - 1, SRATTR_MINTOCOMPLETE);
		 if( !hRes )
			 Failed ("No error when set out of range");
	}

	if( dwMaxIncVal < (DWORD) -1 ) {
	   hRes = gpSRMode->TimeOutSet (dwMaxIncVal + 1, SRATTR_MAXTOCOMPLETE);
		 if( !hRes )
			 Failed ("No error when set out of range");
	}

	if( dwMinComVal > 0 ) {
	   hRes = gpSRMode->TimeOutSet ( SRATTR_MINTOINCOMPLETE, dwMinComVal - 1);
		 if( !hRes )
			 Failed ("No error when set out of range");
	}

	if( dwMaxComVal < (DWORD) -1 ) {
	   hRes = gpSRMode->TimeOutSet ( SRATTR_MAXTOINCOMPLETE, dwMaxComVal + 1);
		 if( !hRes )
			 Failed ("No error when set out of range");
	}

   // return to original value
   hRes = gpSRMode->TimeOutSet (dwValA, dwValB);
   CHECKFAILED ("TimeOutSet");
   hRes = gpSRMode->TimeOutGet (&dwValA2, &dwValB2);
   CHECKFAILED ("TimeOutGet");
   if ((dwValA != dwValA2) || (dwValB != dwValB2))
      Failed ("Didn't recall previous value");

   Passed();
}


// - Commit
void CALLBACK Test207 (void)
{
   NEEDSRINTERFACE (SRI_ISRSPEAKER2);

   // select untrained speaker
   HRESULT  hRes;

   hRes = gpSRMode->Select(NULL);
   CHECKFAILED ("Select (NULL)");

   // make sure can't commit this
   hRes = gpSRMode->Commit();
   if (!hRes) {
      FAILED ("Should return error when commit default speaker");
      return;
   }

   // select new speaker
   hRes = gpSRMode->Select (L"Dummy");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Commit ();
   CHECKFAILED ("Commit");

   Passed();

}


// - Rename
void CALLBACK Test208 (void)
{
   NEEDSRINTERFACE (SRI_ISRSPEAKER2);

   HRESULT  hRes;

   // test for null errors
   hRes = gpSRMode->Rename (NULL, L"A");
   if (!hRes) {
      FAILED ("Rename didn't detect NULL.");
      return;
   }
   hRes = gpSRMode->Rename (L"B", NULL);
   if (!hRes) {
      FAILED ("Rename didn't detect NULL.");
      return;
   }
   hRes = gpSRMode->Rename (NULL, NULL);
   if (!hRes) {
      FAILED ("Rename didn't detect NULL.");
      return;
   }

   // select a user
   hRes = gpSRMode->Select (L"Dummy");
   CHECKFAILED ("Select");
   gpSRMode->Delete (L"Dummy2");
   hRes = gpSRMode->Rename (L"Dummy", L"Dummy2");
   CHECKFAILED ("Rename current user");
   WCHAR szTemp[256];
   DWORD dwNeeded;
   hRes = gpSRMode->Query (szTemp, sizeof(szTemp)/2, &dwNeeded);
   CHECKFAILED ("Query");
   if (!wcscmp(szTemp, L"Dummy2")) {
      FAILED ("When rename current user failed.");
   }

   // rename over an existing user and expect error
   hRes = gpSRMode->Select (L"Dummy2");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Select (L"Dummy");
   CHECKFAILED ("Select");
   hRes = gpSRMode->Rename (L"Dummy2", L"Dummy");
   if (!hRes) {
      FAILED ("Rename didn't detect renaming over existing user.");
      return;
   }

   // rename to non-existend user
   gpSRMode->Delete (L"Dummy3");
   hRes = gpSRMode->Rename (L"Dummy2", L"Dummy3");
   CHECKFAILED ("Rename");
   PVOID pMem = NULL;
   DWORD dwSize;
   hRes = gpSRMode->Read (L"Dummy3", &pMem, &dwSize);
   if (pMem)
      CoTaskMemFree (pMem);
	pMem = NULL;
   CHECKFAILED ("Read renamed user");

   Passed();

}

// - GetChangedInfo
void CALLBACK Test209 (void)
{
   NEEDSRINTERFACE (SRI_ISRSPEAKER2);

   HRESULT  hRes;
   FILETIME ft;
   BOOL     f;

   // test for null errors
   hRes = gpSRMode->GetChangedInfo (NULL, &ft);
   if (!hRes) {
      FAILED ("GetChangedInfo didn't detect NULL.");
      return;
   }
   hRes = gpSRMode->GetChangedInfo (&f, NULL);
   if (!hRes) {
      FAILED ("GetChangedIfno didn't detect NULL.");
      return;
   }
   hRes = gpSRMode->GetChangedInfo (NULL, NULL);
   if (!hRes) {
      FAILED ("GetChangedInfo didn't detect NULL.");
      return;
   }

   // no user
   hRes = gpSRMode->Select (NULL);
   CHECKFAILED ("Select");

   // make sure not changed into
   hRes = gpSRMode->GetChangedInfo (&f, &ft);
   CHECKFAILED ("GetChangedInfo");
   if (f)
      FAILED ("Said that default speaker changed even though don't expect");

   // delete user
   gpSRMode->Delete (L"Dummy");

   // new user
   hRes = gpSRMode->Select (L"Dummy");
   CHECKFAILED ("Select");
   hRes = gpSRMode->GetChangedInfo (&f, &ft);
   CHECKFAILED ("GetChangedInfo");
   if (f)
      FAILED ("Said that dummy speaker changed even though don't expect");

   Passed();

}

// - TrainPhrasesDlg - Check to see if it exists
void CALLBACK Test210 (void)
{
   NEEDSRINTERFACE (SRI_ISRDIALOGS2);

   HRESULT hRes;

   // null parameter
   hRes = gpSRMode->TrainPhrasesDlg (NULL, NULL, NULL);
   if (!hRes)
      FAILED ("Didn't detect NULL phrases");

   // see if it seems ok
   hRes = gpSRMode->TrainPhrasesDlg (NULL, NULL, L"Hello");
   CHECKFAILED ("TrainPhrasesDlg");

   Passed();
}

// ISRResMerge::Merge - invalid parameters
void CALLBACK Test211 (void)
{
	NEEDSRINTERFACE (SRI_ISRESMERGE);

	HRESULT hRes;
	NEEDCFG;
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
	// variable needed for ISRResMerge
	LPUNKNOWN pIUnknown = NULL;
	LPUNKNOWN pIUnkMerge = NULL;

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

	// test
	if (!gpSRResult) {
		Failed ("no result object");
		goto cleanup;
	}

	if (!(gpSRResult->m_pISRResMerge)) {
		Information ("Unable to test: gpSRResult->m_pISRResMerge is NULL");
		goto cleanup;
	}

	// first make sure it's supported
	hRes = gpSRResult->m_pISRResMerge->Merge(NULL, NULL);
	if (hRes == SRERR_NOTSUPPORTED) {
		Failed("Merge is not supported");
		goto cleanup;
	}

	// test second parameter for NULL
	hRes = gpSRResult->m_pISRResMerge->Merge(pIUnknown, NULL); 
	if (!hRes)
		Failed("Merge did not detect NULL second parameter");

	// test first paramter for NULL
	hRes = gpSRResult->m_pISRResMerge->Merge(NULL, &pIUnkMerge);
	if (!hRes)
		Failed("Merge did not detect NULL first parameter");


cleanup:

	// release it just it case it got set
	if( pIUnkMerge )
		pIUnkMerge->Release();

   ClearMainGrammar();
   ClearSRAudio();

	Passed();
}


// ISRResMerge::Split - invalid parameters
void CALLBACK Test212 (void)
{
	NEEDSRINTERFACE (SRI_ISRESMERGE);

	HRESULT hRes;
	NEEDCFG;
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
	// variable needed for ISRResMerge
	LPUNKNOWN pIUnkLeft = NULL;	
	LPUNKNOWN pIUnkRight = NULL;
	QWORD qwSplitTime = 10;

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

	// test
	if (!gpSRResult) {
		Failed ("no result object");
		goto cleanup;
	}

	if (!(gpSRResult->m_pISRResMerge)) {
		Information ("Unable to test: gpSRResult->m_pISRResMerge is NULL");
		goto cleanup;
	}

	// first make sure it's supported
	hRes = gpSRResult->m_pISRResMerge->Split(NULL, NULL, NULL);
	if (hRes == SRERR_NOTSUPPORTED) {
		Failed("Split is not supported");
		goto cleanup;
	}

	// first parameter is not checked or NULL as 0 is a valid time

	// test second parameter for NULL
	hRes = gpSRResult->m_pISRResMerge->Split(qwSplitTime, NULL, &pIUnkRight);
	if (!hRes)
		Failed("Split did not detect NULL second parameter");

	// test third parameter for NULL
	hRes = gpSRResult->m_pISRResMerge->Split(qwSplitTime, &pIUnkLeft, NULL);
	if (!hRes)
		Failed("Split did not detect NULL third parameter");


cleanup:

	// release anything that got set
	if( pIUnkRight )
		pIUnkRight->Release();
	if( pIUnkLeft )
		pIUnkLeft->Release();

   ClearMainGrammar();
   ClearSRAudio();

	Passed();
}

// Helper function to get start/end times of a Results Object
HRESULT GetCSRResultStartEndTimes(CSRResult *pRes, QWORD *pqwStartTime, QWORD *pqwEndTime)
{
   HRESULT hRes;
   SDATA sData;
   DWORD dwStartNode, dwEndNode;

   // get beginning timestamp though the first node
   if (FAILED(hRes = pRes->NodeStartGet (&dwStartNode))) {
		Failed ("NodeStartGet failed", hRes);
      return (hRes);
   }
   if (FAILED(hRes = pRes->DataGet(dwStartNode, SRGNODE_TIME, &sData))) {
		Failed ("DataGet failed to get time",hRes);
      return (hRes);
	}

   if (sData.pData) {
      *pqwStartTime = ( *((QWORD*) sData.pData));
      CoTaskMemFree (sData.pData);
		sData.pData = NULL;
   }

	// get the end time timestamp through the end node
   if (FAILED(hRes = pRes->NodeEndGet (&dwEndNode))) {
		Failed ("NodeEndGet failed", hRes);
      return (hRes);
	}

	// get the timestamp for the last node
   if (FAILED(hRes = pRes->DataGet(dwEndNode, SRGNODE_TIME, &sData))) {
		Failed ("DataGet failed to get time",hRes);
      return (hRes);
	}

   if (sData.pData) {
      *pqwEndTime = ( *((QWORD*) sData.pData));
      CoTaskMemFree (sData.pData);
		sData.pData = NULL;
   }

   return (NOERROR);
} /* End of GetCSRResultStartEndTimes() */

// ISRResMerge::Split - split's time stamps properly for begin and end
void CALLBACK Test213 (void)
{
	NEEDSRINTERFACE (SRI_ISRESMERGE);
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);

	HRESULT hRes;
	NEEDCFG;
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;
   
	// variables needed for ISRResMerge
	LPUNKNOWN pIUnkLeft = NULL;	
	LPUNKNOWN pIUnkRight = NULL;
	QWORD qwSplitTime;

	// variable needed for ISRResGraphEx
   DWORD dwStartNode, dwEndNode;
	SDATA sData;

	// misc variable for test
	QWORD qwTimeStampBegin;
	QWORD qwTimeStampEnd;
	QWORD qwPhraseBegin, qwPhraseEnd, qwTSBeginLeft, qwTSBeginRight, qwTSEndLeft, qwTSEndRight;
	CSRResult *pResultLeft = new CSRResult;
	CSRResult *pResultRight = new CSRResult;
	BOOL bLeftFail = FALSE;


  // recognize a results object
   PCSRResult  pRes;
   pRes = NULL;
   pRes = GetResultsObject (gszAllWork, gszAllWork2Grammar);
   if (!pRes) {
      NotApplicable();
      goto cleanup;
   }
	

	if (!(pRes->m_pISRResMerge)) {
		Information ("Unable to test: m_pISRResMerge is NULL");
		goto cleanup;
	}

	// first make sure it's supported
	hRes = pRes->m_pISRResMerge->Split(NULL, NULL, NULL);
	if (hRes == SRERR_NOTSUPPORTED) {
		Failed("Split is not supported");
		goto cleanup;
	}

	// get beginning timestamp though the first node
	hRes = pRes->NodeStartGet (&dwStartNode);
	if( hRes ) {
		Failed ("NodeStartGet failed", hRes);
		goto cleanup;
	}
	
	// get the timestamp for the first node
	sData.pData = NULL;
	hRes = pRes->DataGet(dwStartNode, SRGNODE_TIME, &sData);
	if( hRes ) {
		Failed ("DataGet failed to get time",hRes);
		goto cleanup;
	}

   if (sData.pData) {
      qwTimeStampBegin = ( *((QWORD*) sData.pData));
      CoTaskMemFree (sData.pData);
    	sData.pData = NULL;
   }

	// get the end time timestamp through the end node
	hRes = pRes->NodeEndGet (&dwEndNode);
	if( hRes ) {
		Failed ("NodeEndGet failed", hRes);
		goto cleanup;
	}

	// get the timestamp for the last node
	sData.pData = NULL;
	hRes = pRes->DataGet(dwEndNode, SRGNODE_TIME, &sData);
	if( hRes ) {
		Failed ("DataGet failed to get time",hRes);
		goto cleanup;
	}

   if (sData.pData) {
      qwTimeStampEnd = ( *((QWORD*) sData.pData));
      CoTaskMemFree (sData.pData);
     	sData.pData = NULL;
   }
	
	// try splitting before beginning
	if( qwTimeStampBegin > 0 ) {
		hRes = pRes->m_pISRResMerge->Split(qwTimeStampBegin - 1, &pIUnkLeft, &pIUnkRight);
		if (!(hRes == SRERR_VALUEOUTOFRANGE || hRes == SRERR_INVALIDPARAM))
			Failed ("expected SRERR_VALUEOUTOFRANGE from split before begin timestamp",hRes);
	}
	if( pIUnkLeft )
		pIUnkLeft->Release();
	if( pIUnkRight )
		pIUnkRight->Release();
	
	// try splitting after end
	if( qwTimeStampEnd < (QWORD) -1 ) {
		hRes = pRes->m_pISRResMerge->Split(qwTimeStampEnd + 1, &pIUnkLeft, &pIUnkRight);
		if (!(hRes == SRERR_VALUEOUTOFRANGE || hRes == SRERR_INVALIDPARAM))
			Failed ("unexpected result from split after end timestamp",hRes);
	}
	if( pIUnkLeft )
		pIUnkLeft->Release();
	if( pIUnkRight )
		pIUnkRight->Release();


	// try a valid split, at beginning
	qwSplitTime = qwTimeStampBegin;
	hRes = pRes->m_pISRResMerge->Split(qwSplitTime, &pIUnkLeft, &pIUnkRight);
	if (hRes)
		Failed ("unexpected error from valid split",hRes);

	// make sure left is NULL and right is not
	if( pIUnkLeft ) {
		Failed ("split on beginning word boudary should return left NULL pointer");
		pIUnkLeft->Release();
	}
	if( !pIUnkRight ) {
		Failed ("failed to return right result interface pointer");
		goto cleanup;
	}

	// make sure time stamps come out right
	// initialize the CResult object so that we can get the timestamp
	hRes = pResultRight->Init( pIUnkRight );
	if( hRes ) {
		Failed ("CResult::Init failed to right result", hRes);
		if( pIUnkRight )
			pIUnkRight->Release();
		goto cleanup;
	}
	// release now that we've initialized the results object
	if( pIUnkRight )
		pIUnkRight->Release();

   // Get phrase timestamps for the initial results object
	hRes = pRes->TimeGet( &qwPhraseBegin, &qwPhraseEnd );
	if ( hRes ) {
		Failed ("error on getting phrase timestamps",hRes);
		goto cleanup;
	}

	// get timestamp for right result object
	hRes = pResultRight->TimeGet( &qwTSBeginRight, &qwTSEndRight );
	if ( hRes ) {
		Failed ("error on getting right time stamp",hRes);
		goto cleanup;
	}

	// compare timestamps for integrity and make sure begin and end
	// are the same as original results object
	if( (qwTSBeginRight > qwTSEndRight ) || (qwTSBeginRight != qwPhraseBegin) || 
		(qwTSEndRight != qwPhraseEnd) )
			Failed("error in timestamp values");

	//
	// try a valid split, at the end
	qwSplitTime = qwTimeStampEnd;
	hRes = pRes->m_pISRResMerge->Split(qwSplitTime, &pIUnkLeft, &pIUnkRight);
	if (hRes)
		Failed ("unexpected error from valid split",hRes);

	// make sure right is NULL and left is not
	if( pIUnkRight ) {
		Failed ("split on end word boudary should return right NULL pointer");
		pIUnkRight->Release();
	}
	if( !pIUnkLeft ) {
		Failed ("failed to return left result interface pointer");
		goto cleanup;
	}

	// make sure time stamps come out right
	// initialize the CResult object so that we can get the timestamp
	hRes = pResultLeft->Init( pIUnkLeft );
	if( hRes ) {
		Failed ("CResult::Init failed to right result", hRes);
		if( pIUnkLeft )
			pIUnkLeft->Release();
		goto cleanup;
	}
	// release now that we've initialized the results object
	if( pIUnkLeft )
		pIUnkLeft->Release();

	// get timestamp for right result object
	hRes = pResultLeft->TimeGet( &qwTSBeginLeft, &qwTSEndLeft );
	if ( hRes ) {
		Failed ("error on getting left time stamp",hRes);
		goto cleanup;
	}

	// compare timestamps for integrity and make sure begin and end
	// are the same as original results object
	if( (qwTSBeginLeft > qwTSEndLeft) || (qwTSBeginLeft != qwPhraseBegin) || 
		(qwTSEndLeft != qwPhraseEnd) )
			Failed("error in timestamp values");



cleanup:

	// no reason to believe these won't be there, but check just in case
	if( pResultLeft )
		delete pResultLeft;
	if( pResultRight )
		delete pResultRight;

	// we should have cleaned this up as we went along but just in case not
	if (sData.pData)
		CoTaskMemFree (sData.pData);
	if (pRes)
		delete pRes;



   ClearMainGrammar();
   ClearSRAudio();

	Passed();
}

// ISRResMerge::Merge - merges split results properly
void CALLBACK Test214 (void)
{
	NEEDSRINTERFACE (SRI_ISRESMERGE);
	NEEDSRINTERFACE (SRI_ISRRESGRAPHEX);

	HRESULT hRes;
	NEEDCFG;
   NEEDDICTATION;
   NEEDENGLISH;
   NEEDAUDIO;

	// variables needed for ISRResMerge
	LPUNKNOWN pIUnkLeft = NULL;	
	LPUNKNOWN pIUnkRight = NULL;
	QWORD qwSplitTime;

	// variable needed for ISRResGraphEx
   SDATA sdPath;
   sdPath.pData = NULL;

	// misc variable for test
	QWORD qwTSBeginLeft, qwTSBeginRight, qwTSEndLeft, qwTSEndRight;
	QWORD qwTSBeginMerge, qwTSEndMerge;
	CSRResult *pResultLeft = new CSRResult;
	CSRResult *pResultRight = new CSRResult;
	CSRResult *pResultMerge = new CSRResult;
	BOOL bLeftFail = FALSE;
   DWORD dwArcId;
   DWORD *pdwBestPath;
	DWORD dwNodeId;
	QWORD qwNodeTime;
   SDATA sdNode;

   // recognize a results object
   PCSRResult  pRes;
   pRes = NULL;
   pRes = GetResultsObject (gszAllWork, gszAllWork2Grammar);
   if (!pRes) {
      NotApplicable();
      return;
   }

	if (!(pRes->m_pISRResMerge)) {
		Information ("Unable to test: m_pISRResMerge is NULL");
		goto cleanup;
	}

	// first make sure it's supported
	hRes = pRes->m_pISRResMerge->Merge(NULL, NULL);
	if (hRes == SRERR_NOTSUPPORTED) {
		Failed("Merge is not supported");
		goto cleanup;
	}

	// get two adjacent results by splitting on a word boundary
	// find a word boundary with ISRResGraphEx
   hRes = pRes->BestPathEnum (0, NULL, 0, NULL, 0, FALSE, &sdPath);
   if (hRes) {
      Failed ("BestPathEnum");
      goto cleanup;
   }

	// pick an arbitrary word boundary about half way
   // First we get the Arc ID of the word
   pdwBestPath = (DWORD *) sdPath.pData;
   dwArcId = pdwBestPath[(sdPath.dwSize / sizeof(DWORD)) / 2];

   // Then we get that arc's left node (i.e. start time)
   hRes = pRes->NodeGet(dwArcId, &dwNodeId, FALSE);
	if ( hRes ) {
		Failed ("Failed to get a timestamp to split on",hRes);
		goto cleanup;
	}

   // Then we convert the node into a SAPI timestamp
   qwNodeTime = 0;
   sdNode.pData = NULL;
   pRes->DataGet (dwNodeId, SRGNODE_TIME, &sdNode);
   if (sdNode.pData) {
      qwNodeTime = ( *((QWORD*) sdNode.pData));
      CoTaskMemFree (sdNode.pData);
      sdNode.pData = NULL;
   }

	// split on this time
	qwSplitTime = qwNodeTime;
	hRes = pRes->m_pISRResMerge->Split(qwSplitTime, &pIUnkLeft, &pIUnkRight);
	if( hRes ) {
		Failed ("Failed to split on valid word boundary",hRes);
		goto cleanup;
	}

	// first check that they are still adjacent before attempting to merge
	// initialize the CResult object so that we can get the timestamp
	hRes = pResultLeft->Init( pIUnkLeft );
	if( hRes ) {
		Failed ("left CResult::Init failed");
		bLeftFail = TRUE;
	}
	if( pIUnkLeft )
		pIUnkLeft->Release();

	// initialize the CResult object so that we can get the timestamp
	hRes = pResultRight->Init( pIUnkRight );
	if( hRes || bLeftFail) {
		Failed ("right CResult::Init failed");
		if( pIUnkRight )
			pIUnkRight->Release();
		goto cleanup;
	}
	if( pIUnkRight )
		pIUnkRight->Release();

	// get the time stamps
   hRes = GetCSRResultStartEndTimes(pResultLeft, &qwTSBeginLeft, &qwTSEndLeft);
	if (hRes) {
		Failed( "Failed to get left time stamps",hRes);
		goto cleanup;
	}
   hRes = GetCSRResultStartEndTimes(pResultRight, &qwTSBeginRight, &qwTSEndRight);
	if (hRes) {
		Failed ("Failed to get right time stamps",hRes);
		goto cleanup;
	}

	// make sure they are adjacent
	if( qwTSEndLeft != qwTSBeginRight ) {
		Failed ("split results are not adjacent" );
		goto cleanup;
	}

	// sanity check of proper timestamp values
	if( (qwTSBeginLeft > qwTSEndLeft) || (qwTSBeginRight > qwTSEndRight) ||
			(qwTSEndLeft != qwNodeTime) || (qwTSBeginRight != qwNodeTime)) {
		Failed ("timestamps are invalid");
		goto cleanup;
	}

	// merge them
	hRes = pResultLeft->Merge (pResultRight, &pResultMerge);
	if (FAILED(hRes))
		Failed ("merge failed");
	else {
		// make sure begin and end timestamps are correct
      hRes = GetCSRResultStartEndTimes(pResultMerge, &qwTSBeginMerge, &qwTSEndMerge);
		if (hRes) {
			Failed( "Failed to get merge time stamps",hRes);
			goto cleanup;
		}

		if( qwTSBeginMerge != qwTSBeginLeft )
			Failed ("Merged Begin timpstamp value is incorrect");
		if( qwTSEndMerge != qwTSEndRight )
			Failed ("Merged End timestamp value is incorrect");
	}


cleanup:

	// no reason to believe these won't be there, but check just in case
   if( pResultLeft )
	   delete pResultLeft;
   if( pResultRight )
	   delete pResultRight;
   if( pResultMerge )
	   delete pResultMerge;

	// we should have cleaned this up as we went along but just in case not
	if (sdPath.pData)
		CoTaskMemFree (sdPath.pData);
	if (pRes)
		delete pRes;



   ClearMainGrammar();
   ClearSRAudio();


	Passed();
}


// 
void CALLBACK Test215 (void)
{
	NotApplicable();
   return;
}


// IAttributes - DWORDGet/Set - SRATTR_THREADPRIORITY (DWORD)
void CALLBACK Test216(void)
{

	NEEDSRINTERFACE (SRI_IATTRIBUTES);
	HRESULT  hRes;

	DWORD dwAttrib = 0;
	DWORD dwValue = 0;
	DWORD dwInvalidPriority = 0;

	if( !(gpSRMode->m_pIAttributes) ) {
		Warning ("unable to test: gpSRMode->m_pIAttributes is NULL");
		goto cleanup;
	}

	// check for proper handling of SRATTR_THREADPRIORITY attribute

	// get the current setting
	hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_THREADPRIORITY, &dwValue);
	if( FAILED(hRes) )
		Failed("Failed to get thread priority attribute value",hRes);
	
	// try to change attribute 
	hRes = gpSRMode->m_pIAttributes->DWORDSet (SRATTR_THREADPRIORITY, THREAD_PRIORITY_ABOVE_NORMAL);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_ABOVE_NORMAL )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpSRMode->m_pIAttributes->DWORDSet (SRATTR_THREADPRIORITY, THREAD_PRIORITY_BELOW_NORMAL);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_BELOW_NORMAL )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpSRMode->m_pIAttributes->DWORDSet (SRATTR_THREADPRIORITY, THREAD_PRIORITY_HIGHEST);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_HIGHEST )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpSRMode->m_pIAttributes->DWORDSet (SRATTR_THREADPRIORITY, THREAD_PRIORITY_IDLE);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_IDLE )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpSRMode->m_pIAttributes->DWORDSet (SRATTR_THREADPRIORITY, THREAD_PRIORITY_LOWEST);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_LOWEST )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpSRMode->m_pIAttributes->DWORDSet (SRATTR_THREADPRIORITY, THREAD_PRIORITY_NORMAL);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to set thread priority to valid value",hRes);
	else {
		// double check that it got set correctly
		hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_NORMAL )
			Failed("Failed to get thread priority that it was set to",hRes);
	}

	hRes = gpSRMode->m_pIAttributes->DWORDSet (SRATTR_THREADPRIORITY, THREAD_PRIORITY_TIME_CRITICAL);
	if( FAILED(hRes) && hRes != E_NOTIMPL)
		Failed("Failed to get thread priority that it was set to",hRes);
	else {
		// double check that it got set correctly
		hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_THREADPRIORITY, &dwValue);
		if( FAILED(hRes) || dwValue != THREAD_PRIORITY_TIME_CRITICAL )
			Failed("Failed to get thread priority that it was set to",hRes);
	}


	// check for invalid arguments handling
	hRes = gpSRMode->m_pIAttributes->DWORDGet (SRATTR_THREADPRIORITY, NULL);
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
				hRes = gpSRMode->m_pIAttributes->DWORDSet (SRATTR_THREADPRIORITY, dwInvalidPriority);
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

