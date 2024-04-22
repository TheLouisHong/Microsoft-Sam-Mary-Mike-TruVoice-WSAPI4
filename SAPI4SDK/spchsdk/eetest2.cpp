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

// Lex2: AddTo pronunciations - invalid param
void CALLBACK Test100(void)
{
	NEEDTTSINTERFACE (TTSI_ILEXPRONOUNCE2);
	HRESULT  hRes;

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
	if( !(gpTTSMode->m_pILexPronounce2) ) 
		Information ("Unable to test: gpTTSMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpTTSMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		else {
			DWORD dwQueryLex = 0;
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
				if( dwQueryLex & dwLexicons ) {
					hRes = gpTTSMode->m_pILexPronounce2->AddTo(dwQueryLex,CHARSET_IPAPHONETIC,NULL,szPronounce,VPS_NOUN,NULL,0);
					if( hRes != E_INVALIDARG ) 
						Failed ("Failed to return E_INVALIDARG for add of NULL word in main object", hRes);
				}
				dwQueryLex = SETBIT(iCount);
			}
		}
	}

	Passed();
	return;
}

// Lex2: QueryLexicons - verify all writable lex's are read lex's.
void CALLBACK Test101(void)
{
	NEEDTTSINTERFACE (TTSI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	// out
	DWORD dwWriteLexicons = 0;
	DWORD dwReadLexicons = 0;

	// check main object
	if( !(gpTTSMode->m_pILexPronounce2) ) 
		Information ("Unable to test: gpTTSMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpTTSMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwWriteLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		hRes = gpTTSMode->m_pILexPronounce2->QueryLexicons(QUERY_READ,&dwReadLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		if( (dwWriteLexicons & dwReadLexicons) != dwWriteLexicons) 
			Failed ("write lexicons are not all read lexicons");
	}

	
	Passed();
	return;
}


// Lex2: ChangeSpelling - verify not applicable to TTS
void CALLBACK Test102(void)
{
	NEEDTTSINTERFACE (TTSI_ILEXPRONOUNCE2);
	HRESULT  hRes;

	WCHAR *pszOld = L"Hello";
	WCHAR *pszNew = L"Hello";
	DWORD dwLexicons = 0;

	// check main object
	if( !(gpTTSMode->m_pILexPronounce2) ) 
		Information ("Unable to test: gpTTSMode->m_pILexPronounce2 is NULL");
	else {
		hRes = gpTTSMode->m_pILexPronounce2->QueryLexicons(QUERY_WRITE,&dwLexicons);
		if( FAILED(hRes) ) {
			Warning( "unable to test main object: QueryLexicons failed", hRes);
		}
		else {
			DWORD dwQueryLex = 0;	
			// check each lexicon
			for( int iCount=0; iCount<8*sizeof(dwLexicons);iCount++) {
				if( dwQueryLex & dwLexicons ) {
						// pass NULL as value to change
					hRes = gpTTSMode->m_pILexPronounce2->ChangeSpelling(dwQueryLex, pszOld, pszNew);
					if( hRes != TTSERR_NOTSUPPORTED ) 
						Failed ("Failed to return TTSERR_NOTSUPPORTED for ChangeSpelling");
				}
				dwQueryLex = SETBIT(iCount);
			}

			// pass invalid number for lexicon
			hRes = gpTTSMode->m_pILexPronounce2->ChangeSpelling(-1, pszOld, pszNew);
			if( SUCCEEDED(hRes) ) 
				Failed ("Failed to return error with invalid param in main object");
		}
	}

	Passed();
	return;
}	

// - RealTimeGet/Set - Make sure that set default values 
void CALLBACK Test103 (void)
{
   HRESULT hRes;
   DWORD  wMinTime, wMaxTime, wRealTime;
   hRes = gpTTSMode->RealTimeGet(&wRealTime);
   if (hRes == TTSERR_NOTSUPPORTED) {
      NotApplicable();
      return;
   }
   CHECKFAILED("RealTimeGet");

   // set value to minimum
   gTTSNS.m_fAttribChanged = 0;
   hRes = gpTTSMode->RealTimeSet (TTSATTR_MINREALTIME );
   CHECKFAILED("RealTimeSet");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (!gTTSNS.m_fAttribChanged ||
      (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_REALTIME))
      Failed ("Didn't get RealTime change notification");

   hRes = gpTTSMode->RealTimeGet (&wMinTime);
   CHECKFAILED("RealTimeSet");

   // set value to maximum
   gTTSNS.m_fAttribChanged = 0;
   hRes = gpTTSMode->RealTimeSet (TTSATTR_MAXREALTIME);
   CHECKFAILED("RealTimeSet");

   // see if the attribute changed
   WaitForEvent (1.0);
   if (!gTTSNS.m_fAttribChanged ||
      (gTTSNS.m_dwAttribChangedAttrib != TTSNSAC_REALTIME))
      Failed ("Didn't get RealTime change notification");

   hRes = gpTTSMode->RealTimeGet (&wMaxTime);
   CHECKFAILED("RealTimeSet");

	// sanity check of min and max values
	if( wMinTime >= wMaxTime )
		Failed ("MinRealTime was >= MaxRealTime" );

	// check for out of range
	if( wMinTime > 0 ) {
		hRes = gpTTSMode->RealTimeSet (wMinTime - 1);
		if( hRes )
			Failed ("Failed to return error for value out of range");
	}

	if( wMaxTime > (WORD) -1 ) {
		hRes = gpTTSMode->RealTimeSet (wMaxTime + 1);
		if( hRes )
			Failed ("Failed to return error for value out of range");
	}

   // restore
   hRes = gpTTSMode->RealTimeSet (wRealTime);
   CHECKFAILED("RealTimeSet");

   Passed();
}


// - Phoneme - valid param
void CALLBACK Test104 (void)
{
   HRESULT hRes;
   SDATA sd;
   SDATA sdp;
	sdp.pData = NULL;
   WCHAR *psz[] = {L"Testing.",
		L"Test \\com=comment tag\\ tag",
		L"pause \\pau=1000\\ tag",
		L"volume \\vol=100\\ tag"};

	BOOL bEnginePhonetic = TRUE;
	BOOL bIPAPhonetic = TRUE;
	// first determine if uses enginephonetic
   sd.pData = (PVOID) psz[0];
   sd.dwSize = sizeof(psz[0]);
	hRes = gpTTSMode->Phoneme (CHARSET_ENGINEPHONETIC,NULL, sd, &sdp);
	if( hRes == TTSERR_NOTSUPPORTED )
		bEnginePhonetic = FALSE;
	if( sdp.pData ) {
		CoTaskMemFree( sdp.pData );
		sdp.pData = NULL;
	}

	if( !bEnginePhonetic && !bIPAPhonetic ) {
		Failed ("Phoneme not supported");
		return;
	}

	// first determine if uses ipaphonetic
	hRes = gpTTSMode->Phoneme (CHARSET_IPAPHONETIC,NULL, sd, &sdp);
	if( hRes == TTSERR_NOTSUPPORTED )
		bIPAPhonetic = FALSE;
	if( sdp.pData ) {
		CoTaskMemFree( sdp.pData );
		sdp.pData = NULL;
	}

   // just text
   sd.pData = (PVOID) psz[0];
   sd.dwSize = sizeof(psz[0]);
	if( bEnginePhonetic ) {
		hRes = gpTTSMode->Phoneme (CHARSET_ENGINEPHONETIC,NULL, sd, &sdp);
		if (hRes)
			Failed ("failed on valid data for enginephonetic");
		if( sdp.pData ) {
			CoTaskMemFree( sdp.pData );
			sdp.pData = NULL;
		}
	}
	if( bIPAPhonetic ) {
		hRes = gpTTSMode->Phoneme (CHARSET_IPAPHONETIC,NULL, sd, &sdp);
		if (hRes)
			Failed ("failed on valid data for ipaphonetic");
		if( sdp.pData ) {
			CoTaskMemFree( sdp.pData );
			sdp.pData = NULL;
		}
	}

	// with comment tag
   sd.pData = (PVOID) psz[1];
   sd.dwSize = sizeof(psz[1]);
	if( bEnginePhonetic ) {
		hRes = gpTTSMode->Phoneme (CHARSET_ENGINEPHONETIC,TTSDATAFLAG_TAGGED, sd, &sdp);
		if (hRes)
			Failed ("failed on valid data with comment tag for enginephonetic");
		if( sdp.pData ) {
			CoTaskMemFree( sdp.pData );
			sdp.pData = NULL;
		}
	}
	if( bIPAPhonetic ) {
		hRes = gpTTSMode->Phoneme (CHARSET_IPAPHONETIC,TTSDATAFLAG_TAGGED, sd, &sdp);
		if (hRes)
			Failed ("failed on valid data for ipaphonetic");
		if( sdp.pData ) {
			CoTaskMemFree( sdp.pData );
			sdp.pData = NULL;
		}
	}
	// with pause tag
   sd.pData = (PVOID) psz[2];
   sd.dwSize = sizeof(psz[2]);
	if( bEnginePhonetic ) {
		hRes = gpTTSMode->Phoneme (CHARSET_ENGINEPHONETIC,TTSDATAFLAG_TAGGED, sd, &sdp);
		if (hRes)
			Failed ("failed on valid data with pause tag for enginephonetic");
		if( sdp.pData ) {
			CoTaskMemFree( sdp.pData );
			sdp.pData = NULL;
		}
	}
	if( bIPAPhonetic ) {
		hRes = gpTTSMode->Phoneme (CHARSET_IPAPHONETIC,TTSDATAFLAG_TAGGED, sd, &sdp);
		if (hRes)
			Failed ("failed on valid data for ipaphonetic");
		if( sdp.pData ) {
			CoTaskMemFree( sdp.pData );
			sdp.pData = NULL;
		}
	}
	// with volume tag
   sd.pData = (PVOID) psz[3];
   sd.dwSize = sizeof(psz[3]);
	if( bEnginePhonetic ) {
		hRes = gpTTSMode->Phoneme (CHARSET_ENGINEPHONETIC,TTSDATAFLAG_TAGGED, sd, &sdp);
		if (hRes)
			Failed ("failed on valid data with pause tag for enginephonetic");
		if( sdp.pData ) {
			CoTaskMemFree( sdp.pData );
			sdp.pData = NULL;
		}
	}
	if( bIPAPhonetic ) {
		hRes = gpTTSMode->Phoneme (CHARSET_IPAPHONETIC,TTSDATAFLAG_TAGGED, sd, &sdp);
		if (hRes)
			Failed ("failed on valid data for ipaphonetic");
		if( sdp.pData ) {
			CoTaskMemFree( sdp.pData );
			sdp.pData = NULL;
		}
	}
   Passed();
}

