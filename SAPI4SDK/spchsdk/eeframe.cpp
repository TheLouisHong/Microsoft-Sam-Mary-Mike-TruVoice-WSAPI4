/****************************************************************
Frame.cpp - Framework code for functionality tests


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

/* If you want to test data logging then define USELOGGING in
   the makefile */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <initguid.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <dsound.h>
#include <time.h>
#include <spchwrap.h>
#include "frame.h"
#include "resource.h"

/*
 *   File:  mscsr.h
 *
 *   Description:  Constant and structure definitions specific to the 
 *                 Microsoft Continuous Speech Recognition Engine.
 *
 */

#ifndef __MSCSR_H__
#define __MSCSR_H__

// SRGARC_PHONEMESEGMENTATION
//    Returns a packed, DWORD (4-byte) aligned array of SRPHONESEG
//    structures that represent the phoneme segmentation for the
//    supplied arc.
DEFINE_GUID(SRGARC_PHONEMESEGMENTATION, 0xd05405b0, 0x1db1, 0x11d2, 0x94, 0x2, 0x0, 0xc0, 0x4f, 0x8e, 0xf4, 0x8f);


#endif /* __MSCSR_H__ */


/*******************************************************************
These includes are for the wave manipulation object in the SDK. The
wave manipulation object is used for the wave editor, and in the
accuracy test, it's used for downsampling
*/
// {8EE328E7-FDF4-11d0-8FAE-08002BE4E62A}
DEFINE_GUID(CLSID_WaveData, 
0x8ee328e7, 0xfdf4, 0x11d0, 0x8f, 0xae, 0x8, 0x0, 0x2b, 0xe4, 0xe6, 0x2a);

#undef   INTERFACE
#define  INTERFACE   IWaveData

// {8EE328E7-FDF4-11d0-8FAE-08002BE4E62A}
DEFINE_GUID(IID_IWaveData, 
0x8ee328e7, 0xfdf4, 0x11d0, 0x8f, 0xae, 0x8, 0x0, 0x2b, 0xe4, 0xe6, 0x2a);

DECLARE_INTERFACE_ (IWaveData, IUnknown) {

   // IUnknown members
   STDMETHOD(QueryInterface)  (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
   STDMETHOD_(ULONG,AddRef)   (THIS) PURE;
   STDMETHOD_(ULONG,Release)  (THIS) PURE;

   // IWaveData members
   STDMETHOD (ChunkEnum) (THIS_ DWORD *, DWORD) PURE;
   STDMETHOD (ChunkDelete) (THIS_ DWORD) PURE;
   STDMETHOD (ChunkGet) (THIS_ DWORD, LPUNKNOWN*) PURE;
   STDMETHOD (ChunkAdd) (THIS_ DWORD, LPUNKNOWN) PURE;
   STDMETHOD (Clone) (THIS_ LPUNKNOWN*) PURE;
   STDMETHOD (MaxTimeGet) (THIS_ double*) PURE;
   STDMETHOD (LastModifiedGet) (THIS_ DWORD *) PURE;
   STDMETHOD (FromFile) (THIS_ PCWSTR) PURE;
   STDMETHOD (FromMemory) (THIS_ PVOID, DWORD) PURE;
   STDMETHOD (ToFile) (THIS_ PCWSTR) PURE;
   STDMETHOD (ToMemory) (THIS_ PVOID*, DWORD*) PURE;
   STDMETHOD (Copy) (THIS_ double, double, LPUNKNOWN*) PURE;
   STDMETHOD (Paste) (THIS_ double, double, LPUNKNOWN) PURE;
   STDMETHOD (ClearAll) (THIS) PURE;
   STDMETHOD (GetWFEX) (THIS_ WAVEFORMATEX **ppWFEX) PURE;
   };

typedef IWaveData * PIWAVEDATA;
typedef struct {
   double   fViewStart;
   double   fViewEnd;
   double   fSelStart;
   double   fSelEnd;
   double   fCaret;
} WAVEVIEWPOSN, *PWAVEVIEWPOSN;


#undef   INTERFACE
#define  INTERFACE   IWaveNotifySink

// {8EE328EE-FDF4-11d0-8FAE-08002BE4E62A}
DEFINE_GUID(IID_IWaveNotifySink, 
0x8ee328ee, 0xfdf4, 0x11d0, 0x8f, 0xae, 0x8, 0x0, 0x2b, 0xe4, 0xe6, 0x2a);

DECLARE_INTERFACE_ (IWaveNotifySink, IUnknown) {

   // IUnknown members
   STDMETHOD(QueryInterface)  (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
   STDMETHOD_(ULONG,AddRef)   (THIS) PURE;
   STDMETHOD_(ULONG,Release)  (THIS) PURE;

   // IWaveNotifySink members
   STDMETHOD (RequestViewChange) (THIS_ PWAVEVIEWPOSN) PURE;
   STDMETHOD (DataChanged) (THIS) PURE;
   STDMETHOD (RequestPlay) (THIS) PURE;
   STDMETHOD (PercentComplete) (THIS_ WORD) PURE;
   STDMETHOD (RequestShowClass) (THIS_ CLSID) PURE;
   };

typedef IWaveNotifySink * PIWAVENOTIFYSINK;



#undef   INTERFACE
#define  INTERFACE   IWaveFX

// {8EE328F3-FDF4-11d0-8FAE-08002BE4E62A}
DEFINE_GUID(IID_IWaveFX, 
0x8ee328f3, 0xfdf4, 0x11d0, 0x8f, 0xae, 0x8, 0x0, 0x2b, 0xe4, 0xe6, 0x2a);

DECLARE_INTERFACE_ (IWaveFX, IUnknown) {

   // IUnknown members
   STDMETHOD(QueryInterface)  (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
   STDMETHOD_(ULONG,AddRef)   (THIS) PURE;
   STDMETHOD_(ULONG,Release)  (THIS) PURE;

   // IWaveChunk members
   STDMETHOD (Dialog) (THIS_ HWND, PIWAVEDATA, double, double, DWORD, PVOID*, DWORD*) PURE;
   STDMETHOD (FX) (THIS_ HWND, PIWAVEDATA, double, double, DWORD, PVOID, DWORD, PIWAVENOTIFYSINK) PURE;
   };

typedef IWaveFX * PIWAVEFX;


// {8EE328F5-FDF4-11d0-8FAE-08002BE4E62A}
// The input data is a WAVEFORMATEX structure
DEFINE_GUID(CLSID_WaveFXFormat, 
0x8ee328f5, 0xfdf4, 0x11d0, 0x8f, 0xae, 0x8, 0x0, 0x2b, 0xe4, 0xe6, 0x2a);

// noise control
typedef struct {
   BOOL     fEntireWave;   // if TRUE affects entire wave, else just selection
   WORD     wNoiseAmp;  // noise amplitude, in 1/65536 of the total max amplitude
} FXNOISE, *PFXNOISE;

// {C8E64980-90EA-11d1-BEAC-006008317CE8}
// there is no input data right now
DEFINE_GUID(CLSID_WaveFXNoise, 
0xc8e64980, 0x90ea, 0x11d1, 0xbe, 0xac, 0x0, 0x60, 0x8, 0x31, 0x7c, 0xe8);



/********* End includes for wave manipulation data *********/

#define  NUMTEMPWAVE 10
#define  TEMPWAVENAME   "ft%d.wav"

#define  IDC_CODE    156
#define  MAXTESTS    1000

// test flags
#define  TEST_FAILED    0x0001
#define  TEST_PASSED    0x0002
#define  TEST_WARNING   0x0004
#define  TEST_NA        0x0008
#define  TEST_TESTED    0x0010

/* typedefs */
typedef struct {
   GUID     gSRMode;
   GUID     gTTSMode;
   WORD     wSRSpeed;
   WORD     wTTSSpeed;
   LANGID   lSR;
   LANGID   lTTS;
   BOOL     fPlayTTS;
   char     szLogFile[256];
} FRAMEOPTIONS, *PFRAMEOPTIONS;




/* globals */
HINSTANCE	ghInstance;
HWND		ghWndEditor = NULL;
HWND		ghWndCode = NULL;
FRAMEOPTIONS   gOpt;
DWORD          gdwMode = ID_TEST_STOPPED;
PCSREnum       gpSREnum = NULL;
PCTTSEnum      gpTTSEnum = NULL;
FILE           *gfLog = NULL;
DWORD          gdwCurTest = (DWORD) -1L;
DWORD          gaTestStatus[MAXTESTS];
BOOL           gfAbortTest = FALSE;
BOOL           gfWantToQuit = FALSE;
DWORD          gdwCurWaveID = 0; // current temp wave to write to
DWORD          gdwRangeStart, gdwRangeStop;
DWORD				gdwRandSeed = 0;
DWORD				gaTestScript[MAXTESTS];
DWORD          gdwThreadID = 0;

// sr
CSRMode        *gpSRMode;
CFuncSRWaveAudioNotifySink gSRWANS;
WAVEFORMATEX   *gpWFEXSR;  // wave format used by SR
DWORD          gdwWFEXSRSize;   // wave format used by SR
CFuncSRNotifySink gSRNS;   // sr notification sink
CFuncGramNotifySink gSRGramNS;   // sr notification sink
CFuncGramNotifySink gSRGramNSSecondary;   // sr notification sink
DWORD          gdwSRNSKey; // notification sinc key
SRMODEINFOW    gSRModeInfo;
PIAUDIOFILE    gpIAudioFileSR;
CSRGramComp    *gpSRGramComp;
CSRGramComp    *gpSRGramCompSecondary;
CSRResult      *gpSRResult;   // last result object from main gramcomp

// tts
CTTSMode       *gpTTSMode;
CFuncTTSWaveAudioNotifySink gTTSWANS;
WAVEFORMATEX   *gpWFEXTTS;  // wave format used by TTS
DWORD          gdwWFEXTTSSize;   // wave format used by TTS
CFuncTTSNotifySink gTTSNS;   // TTS notification sink
DWORD          gdwTTSNSKey; // notification sinc key
TTSMODEINFOW   gTTSModeInfo;
PIAUDIOFILE    gpIAudioFileTTS;
CFuncTTSBufNotifySink gTTSBufNS; // tts buffer notify sink


/*******************************************************************
RangeDlgProc - Select the test range

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK RangeDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      SetDlgItemInt (hWnd, IDC_START, 0, FALSE);
      SetDlgItemInt (hWnd, IDC_STOP, gdwTestElems-1, FALSE);
      break;   // continue on

   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
            gdwRangeStart = (DWORD) GetDlgItemInt(hWnd, IDC_START, NULL, FALSE);
            gdwRangeStop = (DWORD) GetDlgItemInt(hWnd, IDC_STOP, NULL, FALSE);
            // fall through
         case IDCANCEL:
            EndDialog (hWnd, LOWORD(wParam));
            return TRUE;

		 default:
			 return FALSE;
         };
      break;
   };

return FALSE;  // didn't handle
}

/*******************************************************************
CheckThread - Checks to make sure the callback thread is the
   same as the main thread. If not, reports an error.
*/
void CheckThread (void)
{
   DWORD dwID = GetCurrentThreadId();
   if (dwID != gdwThreadID)
      FAILED ("Callback coming from a different thread!");
}

/*******************************************************************
ParseScript - parses script text and dumps results in to array

inputs
   char  *psz - string
returns
   0 if success
*/

DWORD ParseScript( char *psz )  
{
	char *tokenPtr;
	tokenPtr = strtok( psz, ",");
	DWORD dwToken;
	DWORD dwCount = 0;

	while(tokenPtr != NULL) {
		// convert tokenPtr to number and assign to array
		// discard values out of range
		dwToken = atol(tokenPtr);
		if( dwToken >= 0 && dwToken < gdwTestElems ) {
			gaTestScript[dwCount] = dwToken;
			dwCount++;
		}
	
		// advance to next token
		tokenPtr = strtok(NULL, ",");
	
		// sanity check
		if( dwCount > MAXTESTS )
			return FALSE;
	}
	gaTestScript[dwCount] = 0xffffffff;		// indicates end of list

	return TRUE;
}


/*******************************************************************
ScriptDlgProc - get the script for the test run

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK ScriptDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      break;   // continue on
   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
				{
					char szScriptText[1024];
					GetDlgItemText(hWnd, IDC_SCRIPT, szScriptText, sizeof(szScriptText));

					// populate gdwTestScript[]
					if( !ParseScript( szScriptText ) ) {
						MessageBox(hWnd, "can't parse for some reason", "Script Parse Error", MB_OK);
						break;
					}
				}

            // fall through
         case IDCANCEL:
            EndDialog (hWnd, LOWORD(wParam));
            return TRUE;

		 default:
			 return FALSE;
         };
      break;
   };

return FALSE;  // didn't handle
}


/*******************************************************************
OutputLog - Writes the text string to the display window and the
log output.

inputs
   char  *psz - string
returns
   none
*/
void OutputLog (char *psz)
{
   if (!psz)
      return;

   // write out to the text file if we have one
   if (gfLog) {
      fputs (psz, gfLog);
      fflush (gfLog);
   }

   // append the text
   int   iSize;
   iSize = GetWindowTextLength(ghWndCode);

   // if the window text is > 10,000 characters then clear
   if (iSize > 15000) {
      SetWindowText (ghWndCode, psz);
      return;
   }

   SendMessage (ghWndCode, EM_SETSEL, iSize, iSize);
   SendMessage (ghWndCode, EM_REPLACESEL, FALSE, (LPARAM) psz);
}


/*******************************************************************
Information - Writes a string out to the log. If a hRes is available,
   it also writes the return result.

inputs
   char     *psz - String. Don't want a CR.
   HRESULT  hRes - Error. If -1 then doesn't display info
   BOOL     fCR - If TRUE, add a CR. (Default to TRUE)
returns
   none
*/
void Information (char *psz, HRESULT hRes, BOOL fCR)
{
   if (psz)
      OutputLog (psz);
   if (hRes != (HRESULT) -1L) {
      if (!hRes) {
         OutputLog (" - No error");
      }
      else if (hRes == E_NOTIMPL) {
         OutputLog (" - Not implemented");
      }
      else {
         char  szTemp[64];
         wsprintf (szTemp, " - Failed (%lx)", (DWORD) hRes);
         OutputLog (szTemp);
      }

   }

   if (fCR)
      OutputLog ("\r\n");
}

void Information(char *psz, BOOL fCR)
{
   Information (psz, (HRESULT)-1L, fCR);
}

/*******************************************************************
Failed - Writes an string to the log. It also sets a bit indicating
   that the given test has failed

inputs
   char     *psz - String. Don't want a CR.
   HRESULT  hRes - Error. If -1 then doesn't display info
   BOOL     fCR - If TRUE, add a CR. (Default to TRUE)
returns
   none
*/
void Failed (char *psz, HRESULT hRes, BOOL fCR)
{
   Information ("FAILED: ", (HRESULT)-1L, FALSE);
   Information (psz, hRes, fCR);
   if (gdwCurTest != (DWORD) -1L)
      gaTestStatus[gdwCurTest] |= TEST_FAILED;
}

/*******************************************************************
NotApplicable - Writes an string to the log. It also sets a bit indicating
   that the given test has NotApplicable

inputs
   char     *psz - String. Don't want a CR.
   HRESULT  hRes - Error. If -1 then doesn't display info
   BOOL     fCR - If TRUE, add a CR. (Default to TRUE)
returns
   none
*/
void NotApplicable (char *psz, HRESULT hRes, BOOL fCR)
{
   Information ("NOTAPPLICABLE: ", (HRESULT)-1L, FALSE);
   Information (psz, hRes, fCR);
   if (gdwCurTest != (DWORD) -1L)
      gaTestStatus[gdwCurTest] |= TEST_NA;
}

/*******************************************************************
Passed - Writes an string to the log. It also sets a bit indicating
   that the given test has passed

inputs
   char     *psz - String. Don't want a CR.
   HRESULT  hRes - Error. If -1 then doesn't display info
   BOOL     fCR - If TRUE, add a CR. (Default to TRUE)
returns
   none
*/
void Passed (char *psz, HRESULT hRes, BOOL fCR)
{
   // if we've already failed or had a warning, then didn't pass
   if ((gdwCurTest != (DWORD)-1L) &&
      (gaTestStatus[gdwCurTest] & (TEST_FAILED || TEST_WARNING)))
      return;

   Information ("PASSED: ", (HRESULT)-1L, FALSE);
   Information (psz, hRes, fCR);
   if (gdwCurTest != (DWORD) -1L)
      gaTestStatus[gdwCurTest] |= TEST_PASSED;
}

/*******************************************************************
Warning - Writes an string to the log. It also sets a bit indicating
   that the given test has a warning

inputs
   char     *psz - String. Don't want a CR.
   HRESULT  hRes - Error. If -1 then doesn't display info
   BOOL     fCR - If TRUE, add a CR. (Default to TRUE)
returns
   none
*/
void Warning (char *psz, HRESULT hRes, BOOL fCR)
{
   Information ("WARNING: ", (HRESULT)-1L, FALSE);
   Information (psz, hRes, fCR);
   if (gdwCurTest != (DWORD) -1L)
      gaTestStatus[gdwCurTest] |= TEST_WARNING;
}


/*******************************************************************
RunTest - This runs a specific test number, 0 based.

inputs
   DWORD    dwNum
returns
   none
*/
void RunTest (DWORD dwNum)
{
   // set some flags indicating the test
   gdwCurTest = dwNum;

   // write out a separator and the test number
   char  szTemp[256];
   Information ("");
   Information ("");
   wsprintf (szTemp, "*** Test #%d ***", (int) dwNum);
#ifdef _DEBUG
   OutputDebugString("SrFunc:  ");
   OutputDebugString(szTemp);
   OutputDebugString("\r\n");
#endif /* _DEBUG */
   Information (szTemp);
   Information (gaTest[dwNum].pszTestName);
   Information (gaTest[dwNum].pszTestDescription);
   Information ("");

   // clean out junk stuff lying around
   CleanSlate();

   // run the test
   if (gaTest[dwNum].pFunction)
      gaTest[dwNum].pFunction();
   else
      Warning ("No code found for the test");

   gaTestStatus[dwNum] |= TEST_TESTED;

   // do a quick message loop thta allows a user to quit
   MSG   msg;
   while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage (&msg);
      DispatchMessage (&msg);
   }

}


/***************************************************************
RunTestSuite - This is a syncrhonous function that runs the test
   suite. Because it has peekmessage loops in, it acts asynchronous.
   It will autoamtically stop if gfAbortTest is set to TRUE.
   If will shut down the test app if afWantToQuit is set to TRUE.

inputs
   DWORD    dwMode - Test mode ID_TEST_XXX
returns
   none
*/
void RunTestSuite (DWORD dwMode)
{
   if (dwMode == ID_TEST_STOPPED) {
      if (gdwMode == ID_TEST_STOPPED)
         return;

      // else want to stop
      gfAbortTest = TRUE;
      return;  // will eventually abort
   }

   if (gdwMode != ID_TEST_STOPPED)
      return;  // this shouldn't happen

   // if it's a range then ask what it is
   if (dwMode == ID_TEST_RANGE) {
      if (IDOK != DialogBox (ghInstance, MAKEINTRESOURCE(IDD_RANGE),
			ghWndEditor, (DLGPROC) RangeDlgProc)) {
         dwMode = ID_TEST_STOPPED;
         return;
      }
   }

	// if it's a script than ask for it
	if (dwMode == ID_TEST_SCRIPT) {
		if (IDOK != DialogBox (ghInstance, MAKEINTRESOURCE(IDD_SCRIPT),
			ghWndEditor, (DLGPROC) ScriptDlgProc)) {
			dwMode = ID_TEST_STOPPED;
			return;
		}
	}

   // turn abort switch to false before starting
   gfAbortTest = FALSE;

	// seed rand()
	if( gdwRandSeed == 0 ) 
		gdwRandSeed = GetTickCount();
	
	srand( gdwRandSeed );
	char buffer[40];
	_itoa( gdwRandSeed, buffer, 10 );
	
	Information ("Random seed used for tests:");
	Information (buffer);

   // print out some information about the test
   gdwCurTest = (DWORD) -1L;
   Information ("");
   Information ("==============================================================");
   Information ("Running new test suite");
   switch (dwMode) {
   case ID_TEST_UNTESTED:
      Information ("Test untested");
      break;
   case ID_TEST_ALLCASES:
      Information ("Test all cases");
      break;
   case ID_TEST_BACKWARDS:
      Information ("Test all cases (Backwards)");
      break;
	case ID_TEST_REQUIRED:
		Information ("Test only cases for minimum compliance");
		break;
   case ID_TEST_RANDOMONE:
      Information ("Test random - One pass");
      break;
   case ID_TEST_RANDOMLOOP:
      Information ("Test untested - Loop until aborted");
      break;
   case ID_TEST_RANGE:
      Information ("Test a range");
      break;
   case ID_TEST_SCRIPT:
      Information ("Test a script");
      break;
   }

   Information ("");
   Information ("Some tests require English. If you wish to test a non-English engine you should "
      "translate the strings, so that all tests will run.");
   Information ("");

   // message for user to request updates
   Information ("NOTE: If you find any inconsistencies between speech engines, which "
      "is not covered by the test suite, then please send E-mail describing the inconsistency "
      "to voicebug@microsoft.com. Microsoft may use this information to add a new test "
      "case to the functionality test.");

   // Want to start up SR & TTS engine
   HRESULT hRes;
   hRes = CreateSREngine();
   if (hRes) {
      Failed ("Can't create SR engine", hRes);
      goto quit;
   }

   hRes = CreateTTSEngine();
   if (hRes) {
      Failed ("Can't create TTS engine", hRes);
      goto quit;
   }

   // Want to give info on SR & TTS engine
   char  szTemp[256];
   gpSRMode->ModeGet (&gSRModeInfo);
   gpTTSMode->ModeGet (&gTTSModeInfo);
   WideCharToMultiByte (CP_ACP, 0, gSRModeInfo.szModeName, -1, szTemp, sizeof(szTemp), 0,0);
   Information ("SR mode name: ", (HRESULT)-1, FALSE);
   Information (szTemp);
   WideCharToMultiByte (CP_ACP, 0, gTTSModeInfo.szModeName, -1, szTemp, sizeof(szTemp), 0,0);
   Information ("TTS mode name: ", (HRESULT)-1, FALSE);
   Information (szTemp);


   // count the number of tests
   DWORD dwNum;
   dwNum = gdwTestElems;
   gdwMode = dwMode;

   // clear the fields as appropriate
   switch (gdwMode) {
   case ID_TEST_UNTESTED:
      // do nothing to clear
      break;
   case ID_TEST_ALLCASES:
   case ID_TEST_BACKWARDS:
	case ID_TEST_REQUIRED:
   case ID_TEST_RANDOMONE:
   case ID_TEST_RANDOMLOOP:
	case ID_TEST_SCRIPT:
   case ID_TEST_RANGE:
      memset (&gaTestStatus, 0, sizeof(gaTestStatus));
   }

runagain:
   // loop, different loop depending upon
   DWORD i;
   for (i = 0; (i < dwNum) && !gfAbortTest; i++) {
      // figure out which test to run
      DWORD dwTest;

      switch (gdwMode) {
      case ID_TEST_UNTESTED:
         if (gaTestStatus[i] & TEST_TESTED)
            continue;
         dwTest = i;
         break;

      case ID_TEST_ALLCASES:
         dwTest = i;
         break;

		case ID_TEST_REQUIRED:
         if (!(gaTest[i].dwTestType & TEST_REQUIRED))
				continue;
			dwTest = i;
			break;

      case ID_TEST_RANGE:
         // Allow testing a range of tests backwards
         if (gdwRangeStart <= gdwRangeStop) {
            dwTest = i;
            if ((dwTest < gdwRangeStart) || (dwTest > gdwRangeStop))
               continue;   // don't test that
         }
         else {
            dwTest = gdwRangeStart - i;
            if ((dwTest < gdwRangeStop) || (dwTest > gdwRangeStart))
               continue;   // don't test that
         }
         break;

		case ID_TEST_SCRIPT: 
			// test elements of array in order - 0xffffffff is the flag for the end of the script
			if( gaTestScript[i] == 0xffffffff ) {
				i = dwNum;
				continue; //done with run
			}

			// one last sanity check
			if( gaTestScript[i] < MAXTESTS)
				dwTest = gaTestScript[i];
		
			break;
			
      case ID_TEST_BACKWARDS:
         dwTest = dwNum - i - 1;
         break;

      case ID_TEST_RANDOMONE:
      case ID_TEST_RANDOMLOOP:
         // pick a random test
         dwTest = rand() % dwNum;

         // loop until we find one that's not been tested
         while (gaTestStatus[dwTest] & TEST_TESTED)
            dwTest = ((dwTest+1) % dwNum);
         break;
      }
		
      // run it
      //if (dwTest != 111 && dwTest != 68 && dwTest != 54)
         RunTest (dwTest);
   }

   // if we get here and we're supposed to do a random loop then
   // go back again
   if ((gdwMode == ID_TEST_RANDOMLOOP) && !gfAbortTest) {
      for (i = 0; i < dwNum; i++)
         gaTestStatus[i] &= ~TEST_TESTED;
      goto runagain;
   }




   // Want to shut down SR & TTS engine
   DestroySREngine();
   DestroyTTSEngine();

   // show which tests passed/failed/etc
   Information ("=========================== Test Results");
   Information ("");
   for (i = 0; i < dwNum; i++) {
      if (!(gaTestStatus[i] & TEST_TESTED) && (gdwMode != ID_TEST_RANDOMLOOP))
         continue;

      char  szTemp[512];
      char  *pszRes;
      if (gaTestStatus[i] & TEST_FAILED)
         pszRes = "FAILED";
      else if (gaTestStatus[i] & TEST_WARNING)
         pszRes = "Warning";
      else if (gaTestStatus[i] & TEST_PASSED)
         pszRes = "Passed";
      else if (gaTestStatus[i] & TEST_NA)
         pszRes = "Not Applicable";
      else
         pszRes = "???";

      wsprintf (szTemp, "Test %d: %s - %s", (int) i, pszRes,
         gaTest[i].pszTestName);
      Information (szTemp);
   }

quit:
   // finally
   Information ("=========================== Test Ended");
   if (gfAbortTest)
      Information ("(Aborted by user)");

   gfAbortTest = TRUE;
   gdwMode = ID_TEST_STOPPED;
   if (gfWantToQuit)
      PostMessage (ghWndEditor, WM_CLOSE, 0, 0);
}


/*******************************************************************
CloseLog - Closes the log file
*/
void CloseLog (void)
{
   if (gfLog)
      fclose (gfLog);
   gfLog = NULL;
}


/*******************************************************************
OpenLog - Opens the log file. returns TRUE if successful
*/
bool OpenLog (void)
{
   if (gfLog)
      CloseLog();

   if (!gOpt.szLogFile[0])
      return TRUE;  // nothing to open

   // open the log file and append onto the existing one
   if( gfLog = fopen(gOpt.szLogFile, "at") )
		return TRUE;
	else
		return FALSE;
}


/*******************************************************************
LoadOptions - Load options from the registry and fill in the global
*/
void LoadOptions (void)
{
   // default to
   memset (&gOpt, 0, sizeof(gOpt));
   gOpt.wSRSpeed = 256;
   gOpt.wTTSSpeed = 256;
   gOpt.fPlayTTS = TRUE;

   // open the registry
   DWORD dwType, dwNeeded;
   HKEY  hKey;
   PSTR  pszSRBuf = 0, pszTTSBuf = 0;

   // read speech engine mode names
   hKey = NULL;
   DWORD dwDisposition;
   RegCreateKeyEx( HKEY_CURRENT_USER,
     "Software\\Microsoft\\SpeechAPI\\FunctionalityTest",
      NULL, NULL, REG_OPTION_NON_VOLATILE,
      KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition );

   if (hKey) {
	   dwNeeded = sizeof(gOpt);
	   RegQueryValueEx(hKey, gszAppName, NULL, &dwType, (PBYTE) &gOpt, &dwNeeded);
   }

   RegCloseKey (hKey);
}


/*******************************************************************
SaveOptions - Save the options to the registry
*/
void SaveOptions (void)
{
   HKEY  hKey;
   PSTR  pszSRBuf = 0, pszTTSBuf = 0;

   // read speech engine mode names
   hKey = NULL;
   DWORD dwDisposition;
   RegCreateKeyEx( HKEY_CURRENT_USER,
     "Software\\Microsoft\\SpeechAPI\\FunctionalityTest",
      NULL, NULL, REG_OPTION_NON_VOLATILE,
      KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition );

   if (hKey) {
      // Write the registry entry for preferences
      RegSetValueEx( hKey, gszAppName, NULL, REG_BINARY,
          (PBYTE)&gOpt, sizeof(gOpt) );
   }

   RegCloseKey (hKey);
}



/*******************************************************************
SelectSRDlgProc - Select the speech recognition engine

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK SelectSRDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      {
         HRESULT hRes;

         // fill in the combo box
         gpSREnum->Reset();
         SRMODEINFOW mi;
         while (!(hRes = gpSREnum->Next(1, &mi, NULL))) {
            char  szTemp[256];
            int   iIndex;
            WideCharToMultiByte (CP_ACP, 0, mi.szModeName, -1, szTemp, sizeof(szTemp), 0,0);
            iIndex = SendDlgItemMessage (hWnd, IDC_ENGINE, CB_ADDSTRING,
               0, (LPARAM) szTemp);
            if (IsEqualGUID(gOpt.gSRMode, mi.gModeID)) {
               SendDlgItemMessage (hWnd, IDC_ENGINE, CB_SETCURSEL, iIndex, 0);
            }
         }

         // check the radio buttons
         CheckDlgButton (hWnd, IDC_RATE1,
            (gOpt.wSRSpeed == 0x100) ? BST_CHECKED : BST_UNCHECKED);
         CheckDlgButton (hWnd, IDC_RATE2,
            (gOpt.wSRSpeed == 0x200) ? BST_CHECKED : BST_UNCHECKED);
         CheckDlgButton (hWnd, IDC_RATE4,
            (gOpt.wSRSpeed == 0x400) ? BST_CHECKED : BST_UNCHECKED);
         CheckDlgButton (hWnd, IDC_RATE12,
            (gOpt.wSRSpeed == 0x80) ? BST_CHECKED : BST_UNCHECKED);
         CheckDlgButton (hWnd, IDC_RATE14,
            (gOpt.wSRSpeed == 0x40) ? BST_CHECKED : BST_UNCHECKED);

      }
      break;   // continue on

   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
            {
               // get the current selection and use that
               int   iRet;
               iRet = SendDlgItemMessage (hWnd, IDC_ENGINE, CB_GETCURSEL, 0, 0);
               if (iRet == CB_ERR)
                  gOpt.gSRMode = GUID_NULL;
               else {
                  SRMODEINFOW mi;
                  gpSREnum->Reset();
                  gpSREnum->Skip((DWORD) iRet);
                  gpSREnum->Next(1, &mi, NULL);

                  // Do error checks here, because we
                  // can't select certain engines

                  // make sure it supports continuous & dictation
                  if (gfNeedAudioSREngine &&
                     ( (mi.dwSequencing != SRSEQUENCE_CONTINUOUS) ||
                     !(mi.dwGrammars & SRGRAM_CFG)
                     )) {

                     MessageBox (hWnd,
                        "Error: The speech recognition engine must support CFGs.",
                        gszAppName, MB_OK);
                     break;
                  }

                  // make sure not fixed audio
                  if (mi.dwFeatures & SRFEATURE_FIXEDAUDIO) {
                     MessageBox (hWnd,
                        gfNeedAudioSREngine ?
                        "Error: The speech recognition engine cannot accept input from text-to-speech. "
                        "It must be able to do this."
                        :
                        "Warning: The speech recognition enging you've selected will not "
                        "work with all of the tests because it cannot accept audio from "
                        "the text-to-speech engine.",
                        gszAppName, MB_OK);
                     if (gfNeedAudioSREngine)
                        break;
                  }

                  gOpt.gSRMode = mi.gModeID;
                  gOpt.lSR = mi.language.LanguageID;

                  // make sure the languages match
                  if (gOpt.lTTS) {
                     if (PRIMARYLANGID(gOpt.lTTS) != PRIMARYLANGID(gOpt.lSR))
                        MessageBox (hWnd,
                           "Warning: The speech recognition language is different from the "
                           "text-to-speech language. They should be the same.",
                           gszAppName, MB_OK);
                     else if (SUBLANGID(gOpt.lTTS) != SUBLANGID(gOpt.lSR))
                        MessageBox (hWnd,
                           "Warning: The speech recognition sub-language is different from the "
                           "text-to-speech sub-language. This may cause recognition problems.",
                           gszAppName, MB_OK);

                  }

               }

               // see what rate is selected
               if (IsDlgButtonChecked(hWnd, IDC_RATE2))
                  gOpt.wSRSpeed = 0x200;
               else if (IsDlgButtonChecked(hWnd, IDC_RATE4))
                  gOpt.wSRSpeed = 0x400;
               else if (IsDlgButtonChecked(hWnd, IDC_RATE12))
                  gOpt.wSRSpeed = 0x80;
               else if (IsDlgButtonChecked(hWnd, IDC_RATE14))
                  gOpt.wSRSpeed = 0x40;
               else
                  gOpt.wSRSpeed = 0x100;

            }
            // continue on through
         case IDCANCEL:
            EndDialog (hWnd, LOWORD(wParam));
            return TRUE;

		 default:
			 return FALSE;
         };
      break;
   };

return FALSE;  // didn't handle
}


/*******************************************************************
SelectTTSDlgProc - Select the speech recognition engine

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK SelectTTSDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      {
         HRESULT hRes;

         // fill in the combo box
         gpTTSEnum->Reset();
         TTSMODEINFOW mi;
         while (!(hRes = gpTTSEnum->Next(1, &mi, NULL))) {
            char  szTemp[256];
            int   iIndex;

            // if we're listening for SR, then only allow preferred voices
            // to be selected, since non-preferred are often wierd/funky
            // sounding voices where sr doesn't work well
            if (!gfNeedAudioSREngine && !(mi.dwFeatures & TTSFEATURE_PREFERRED))
               continue;

            WideCharToMultiByte (CP_ACP, 0, mi.szModeName, -1, szTemp, sizeof(szTemp), 0,0);
            iIndex = SendDlgItemMessage (hWnd, IDC_ENGINE, CB_ADDSTRING,
               0, (LPARAM) szTemp);
            if (IsEqualGUID(gOpt.gTTSMode, mi.gModeID)) {
               SendDlgItemMessage (hWnd, IDC_ENGINE, CB_SETCURSEL, iIndex, 0);
            }
         }

         // check the radio buttons
         CheckDlgButton (hWnd, IDC_RATE1,
            (gOpt.wTTSSpeed == 0x100) ? BST_CHECKED : BST_UNCHECKED);
         CheckDlgButton (hWnd, IDC_RATE2,
            (gOpt.wTTSSpeed == 0x200) ? BST_CHECKED : BST_UNCHECKED);
         CheckDlgButton (hWnd, IDC_RATE4,
            (gOpt.wTTSSpeed == 0x400) ? BST_CHECKED : BST_UNCHECKED);
         CheckDlgButton (hWnd, IDC_RATE12,
            (gOpt.wTTSSpeed == 0x80) ? BST_CHECKED : BST_UNCHECKED);
         CheckDlgButton (hWnd, IDC_RATE14,
            (gOpt.wTTSSpeed == 0x40) ? BST_CHECKED : BST_UNCHECKED);

         // check the option for playing TTS
         CheckDlgButton (hWnd, IDC_PLAYTTS,
            gOpt.fPlayTTS ? BST_CHECKED : BST_UNCHECKED);
      }
      break;   // continue on

   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
            {
               // get the current selection and use that
               int   iRet;
               iRet = SendDlgItemMessage (hWnd, IDC_ENGINE, CB_GETCURSEL, 0, 0);
               if (iRet == CB_ERR)
                  gOpt.gTTSMode = GUID_NULL;
               else {
                  TTSMODEINFOW mi, miF;

                  memset (&miF, 0, sizeof(miF));
                  char  szTemp[256];
                  SendDlgItemMessage (hWnd, IDC_ENGINE, CB_GETLBTEXT, (WPARAM) iRet, (LPARAM) szTemp);
                  MultiByteToWideChar (CP_ACP, 0, szTemp, -1, miF.szModeName, sizeof(miF.szModeName)/2);

                  gpTTSEnum->Find(&miF, &mi);

                  // Do error checks here, because we
                  // can't select certain engines

                  // make sure not fixed audio
                  if (mi.dwFeatures & TTSFEATURE_FIXEDAUDIO) {
                     MessageBox (hWnd,
                        "Warning: The text-to-speech enging you've selected will not "
                        "work with all of the tests because it cannot send audio to "
                        "the speech recognition engine.",
                        gszAppName, MB_OK);
                  }

                  gOpt.gTTSMode = mi.gModeID;
                  gOpt.lTTS = mi.language.LanguageID;

                  // make sure the languages match
                  if (gOpt.lTTS) {
                     if (PRIMARYLANGID(gOpt.lTTS) != PRIMARYLANGID(gOpt.lSR))
                        MessageBox (hWnd,
                           "Warning: The speech recognition language is different from the "
                           "text-to-speech language. They should be the same.",
                           gszAppName, MB_OK);
                     else if (SUBLANGID(gOpt.lTTS) != SUBLANGID(gOpt.lSR))
                        MessageBox (hWnd,
                           "Warning: The speech recognition sub-language is different from the "
                           "text-to-speech sub-language. This may cause recognition problems.",
                           gszAppName, MB_OK);

                  }

               }

               // see what rate is selected
               if (IsDlgButtonChecked(hWnd, IDC_RATE2))
                  gOpt.wTTSSpeed = 0x200;
               else if (IsDlgButtonChecked(hWnd, IDC_RATE4))
                  gOpt.wTTSSpeed = 0x400;
               else if (IsDlgButtonChecked(hWnd, IDC_RATE12))
                  gOpt.wTTSSpeed = 0x80;
               else if (IsDlgButtonChecked(hWnd, IDC_RATE14))
                  gOpt.wTTSSpeed = 0x40;
               else
                  gOpt.wTTSSpeed = 0x100;

               // get the option for playing TTS
               gOpt.fPlayTTS = IsDlgButtonChecked(hWnd, IDC_PLAYTTS);

            }
            // continue on through
         case IDCANCEL:
            EndDialog (hWnd, LOWORD(wParam));
            return TRUE;

		 default:
			 return FALSE;
         };
      break;
   };

return FALSE;  // didn't handle
}

/*******************************************************************
LoggingDlgProc - Select the speech recognition engine

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK LoggingDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      SetDlgItemText (hWnd, IDC_LOG, gOpt.szLogFile);
      break;   // continue on

   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
            GetDlgItemText (hWnd, IDC_LOG, gOpt.szLogFile, sizeof(gOpt.szLogFile));
            if( !OpenLog() )
					MessageBox (hWnd, "Unable to create log file","Warning", MB_ICONWARNING);
            // fall through
         case IDCANCEL:
            EndDialog (hWnd, LOWORD(wParam));
            return TRUE;

		 default:
			 return FALSE;
         };
      break;
   };

return FALSE;  // didn't handle
}

/*******************************************************************
RandSeedDlgProc - Seed rand() with a specific value

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK RandSeedDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
		// set the initial value for the random seed
		SetDlgItemInt (hWnd, IDC_SEED, gdwRandSeed, FALSE);
      break;   // continue on
   case WM_COMMAND:
      switch (LOWORD(wParam))
         {
         case IDOK:
				gdwRandSeed = (DWORD) GetDlgItemInt (hWnd, IDC_SEED, NULL, FALSE);
	         // fall through
         case IDCANCEL:
            EndDialog (hWnd, LOWORD(wParam));
            return TRUE;

		 default:
			 return FALSE;
         };
      break;
   };

return FALSE;  // didn't handle
}


/********************************************************************
EditorWndProc - Editor window proc
*/
LONG CALLBACK EditorWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
   case WM_SETFOCUS:
      SetFocus (ghWndCode);
      break;
	case WM_CREATE:
		ghWndCode = CreateWindowEx (
			WS_EX_CLIENTEDGE,
			"EDIT",
			"",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | 
			ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
			1, 1, 1, 1,
			hwnd,
			(HMENU) IDC_CODE,
			ghInstance,
			NULL
			);
		break;
   case WM_MENUSELECT:
      {
      HMENU hMenu = (HMENU) lParam;
      if (!hMenu) break;

      BOOL  fSRTTS;
      fSRTTS = (!IsEqualGUID (gOpt.gSRMode, GUID_NULL) &&
         !IsEqualGUID (gOpt.gTTSMode, GUID_NULL));

      switch (LOWORD(wParam)) {
      case 0:
         // disable if no SR or TTS is selected
         EnableMenuItem (hMenu, ID_TEST_STOPPED,
            MF_BYCOMMAND |
            ((fSRTTS && (gdwMode != ID_TEST_STOPPED)) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
         EnableMenuItem (hMenu, ID_TEST_ALLCASES,
            MF_BYCOMMAND |
            ((fSRTTS && (gdwMode == ID_TEST_STOPPED)) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
         EnableMenuItem (hMenu, ID_TEST_BACKWARDS,
            MF_BYCOMMAND |
            ((fSRTTS && (gdwMode == ID_TEST_STOPPED)) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
         EnableMenuItem (hMenu, ID_TEST_UNTESTED,
            MF_BYCOMMAND |
            ((fSRTTS && (gdwMode == ID_TEST_STOPPED)) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
         EnableMenuItem (hMenu, ID_TEST_REQUIRED,
            MF_BYCOMMAND |
            ((fSRTTS && (gdwMode == ID_TEST_STOPPED)) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
         EnableMenuItem (hMenu, ID_TEST_RANDOMONE,
            MF_BYCOMMAND |
            ((fSRTTS && (gdwMode == ID_TEST_STOPPED)) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
         EnableMenuItem (hMenu, ID_TEST_RANDOMLOOP,
            MF_BYCOMMAND |
            ((fSRTTS && (gdwMode == ID_TEST_STOPPED)) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
         EnableMenuItem (hMenu, ID_TEST_RANGE,
            MF_BYCOMMAND |
            ((fSRTTS && (gdwMode == ID_TEST_STOPPED)) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
         EnableMenuItem (hMenu, ID_TEST_SCRIPT,
            MF_BYCOMMAND |
            ((fSRTTS && (gdwMode == ID_TEST_STOPPED)) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));

         // check the current mode
         CheckMenuItem (hMenu, ID_TEST_STOPPED,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_STOPPED) ? MF_CHECKED : MF_UNCHECKED));
         CheckMenuItem (hMenu, ID_TEST_ALLCASES,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_ALLCASES) ? MF_CHECKED : MF_UNCHECKED));
         CheckMenuItem (hMenu, ID_TEST_BACKWARDS,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_BACKWARDS) ? MF_CHECKED : MF_UNCHECKED));
         CheckMenuItem (hMenu, ID_TEST_UNTESTED,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_UNTESTED) ? MF_CHECKED : MF_UNCHECKED));
         CheckMenuItem (hMenu, ID_TEST_REQUIRED,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_RANDOMONE) ? MF_CHECKED : MF_UNCHECKED));
         CheckMenuItem (hMenu, ID_TEST_RANDOMONE,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_RANDOMONE) ? MF_CHECKED : MF_UNCHECKED));
         CheckMenuItem (hMenu, ID_TEST_RANDOMLOOP,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_RANDOMLOOP) ? MF_CHECKED : MF_UNCHECKED));
         CheckMenuItem (hMenu, ID_TEST_RANGE,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_RANGE) ? MF_CHECKED : MF_UNCHECKED));
         CheckMenuItem (hMenu, ID_TEST_SCRIPT,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_SCRIPT) ? MF_CHECKED : MF_UNCHECKED));
         break;
      case 1:
         // disable if we're in a non-stopped mode
         EnableMenuItem (hMenu, ID_OPTIONS_SR,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_STOPPED) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
         EnableMenuItem (hMenu, ID_OPTIONS_TTS,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_STOPPED) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
         EnableMenuItem (hMenu, ID_OPTIONS_LOGGING,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_STOPPED) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
			EnableMenuItem (hMenu, ID_OPTIONS_RANDSEED,
            MF_BYCOMMAND | ((gdwMode == ID_TEST_STOPPED) ? MF_ENABLED : (MF_DISABLED|MF_GRAYED)));
         break;
      }
      }
      break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_TEST_EXIT:
			PostMessage (hwnd, WM_CLOSE, 0, 0);
			break;
      case ID_OPTIONS_SR:
         DialogBox (ghInstance, MAKEINTRESOURCE(IDD_SELECTSR),
            ghWndEditor, (DLGPROC) SelectSRDlgProc);
         break;
      case ID_OPTIONS_TTS:
         DialogBox (ghInstance, MAKEINTRESOURCE(IDD_SELECTTTS),
            ghWndEditor, (DLGPROC) SelectTTSDlgProc);
         break;
      case ID_OPTIONS_LOGGING:
         DialogBox (ghInstance, MAKEINTRESOURCE(IDD_LOGGING),
            ghWndEditor, (DLGPROC) LoggingDlgProc);
         break;
		case ID_OPTIONS_RANDSEED:
			DialogBox (ghInstance, MAKEINTRESOURCE(IDD_RANDSEED),
            ghWndEditor, (DLGPROC) RandSeedDlgProc);
         break;
      case ID_TEST_STOPPED:
      case ID_TEST_ALLCASES:
      case ID_TEST_BACKWARDS:
		case ID_TEST_REQUIRED:
      case ID_TEST_UNTESTED:
      case ID_TEST_RANDOMONE:
      case ID_TEST_RANDOMLOOP:
		case ID_TEST_SCRIPT:
      case ID_TEST_RANGE:
			RunTestSuite(LOWORD(wParam));
			break;
      case ID_TEST_LIST:
         {
            DWORD i;
            char  szTemp[5120];
            for (i = 0; i < gdwTestElems; i++) {
               wsprintf (szTemp, "Test %d: %s - %s", (int) i, 
                  gaTest[i].pszTestName, gaTest[i].pszTestDescription);
               Information ("");
               Information (szTemp);
            }

         }
         break;
		}
		break;
	case WM_CLOSE:
      if (gdwMode != ID_TEST_STOPPED) {
         gfWantToQuit = TRUE;
         gfAbortTest = TRUE;
         return 0;
      }

		DestroyWindow (hwnd);
		return 0;
	case WM_SIZE:
		// move the editort and compiler window
		MoveWindow (ghWndCode, 0, 0,
			LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage (0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


/********************************************************************
WinMain - Main entry point
*/
int WINAPI WinMain( 
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	ghInstance = hInstance;
   gdwThreadID = GetCurrentThreadId();

   LoadOptions();
	if( !OpenLog() )
		MessageBox(NULL, "Unable to create log file","Warning", MB_ICONWARNING);

	// init ole
	CoInitialize(NULL);

   // create the SR & TTS enumerators
   gpSREnum = new CSREnum;
   gpTTSEnum = new CTTSEnum;
   gpSREnum->Init();
   gpTTSEnum->Init();

	// register the windows class
	WNDCLASS wc;
	char	szClass[] = "FunctionalityTest";
	memset (&wc, 0, sizeof(wc));
	wc.style = 0; //CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = EditorWndProc;
	wc.hInstance = ghInstance;
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE+1);
	wc.lpszMenuName = MAKEINTRESOURCE (IDR_MENU);
	wc.lpszClassName = szClass;
   wc.hIcon = NULL;
	RegisterClass (&wc);

	// create the window
	ghWndEditor = CreateWindowEx (
		WS_EX_WINDOWEDGE,
		szClass,
		gszAppName,
		WS_OVERLAPPEDWINDOW | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, ghInstance, NULL);
	if (!ghWndEditor)
		return -1;

	// message loop
    MSG msg;
	ShowWindow( ghWndEditor, nCmdShow );
	UpdateWindow( ghWndEditor );


	while( GetMessage(&msg, NULL, NULL, NULL) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

   
   // free up the objects
   if (gpSREnum)
      delete gpSREnum;
   if (gpTTSEnum)
      delete gpTTSEnum;
   DestroySREngine();
   DestroyTTSEngine();

	// finish with ole
	CoUninitialize ();

   SaveOptions();
   CloseLog();

   // delete all the temporary wave files
   DWORD i;
   for (i = 0; i < NUMTEMPWAVE; i++) {
      char  szTemp[256];
      wsprintf (szTemp, TEMPWAVENAME, (int) i);
      DeleteFile (szTemp);
   }

	// done
	return 0;
}



/*****************************************************************
CreateSREngine - Creates an instance of the speech recognition
engine and registers the global SR notify sink object

inputs
   none
returns
   HRESULT - error
*/
HRESULT CreateSREngine (void)
{
   // destroy the existing engine just in case
   DestroySREngine();

   // creaate
   gpSRMode = new CSRMode;
   if (!gpSRMode)
      return E_OUTOFMEMORY;
   
   // create the audio source from wave
   HRESULT hRes;
	hRes = CoCreateInstance(CLSID_AudioSourceFile, 
		NULL, CLSCTX_INPROC_SERVER, 
		IID_IAudioFile, 
		(PVOID *) &gpIAudioFileSR);
	if (hRes) return hRes;
   gSRWANS.m_qwPosn = 0;
	gpIAudioFileSR -> Register(&gSRWANS);
   gpIAudioFileSR->RealTimeSet (gOpt.wSRSpeed);

   // init the engine
   hRes = gpSRMode->InitAudioSourceObject(gpIAudioFileSR);
   if (hRes)
      return hRes;

   hRes = gpSRMode->Init (gOpt.gSRMode);

   if (hRes)
      return hRes;

   // get the wave format
   PIAUDIO  pIAudio;
   gpIAudioFileSR->QueryInterface (IID_IAudio, (PVOID*) &pIAudio);
   if (pIAudio) {
      SDATA s;
      hRes = pIAudio->WaveFormatGet (&s);
      pIAudio->Release();
      gpWFEXSR = (WAVEFORMATEX*) s.pData;
      gdwWFEXSRSize = s.dwSize;
      if (hRes) {
         Failed ("The speech recognition engine hasn't told the audio source what wave format to use.");
         return hRes;
      }
   }

   // Need to register
   hRes = gpSRMode->Register (&gSRNS, &gdwSRNSKey);
   if (hRes) {
      return hRes;
   }


   // it all worked
   return NOERROR;
}



/*****************************************************************
CreateTTSEngine - Creates an instance of the speech recognition
engine and registers the global SR notify sink object

inputs
   none
returns
   HRESULT - error
*/
HRESULT CreateTTSEngine (void)
{
   // destroy the existing engine just in case
   DestroyTTSEngine();

   // creaate
   gpTTSMode = new CTTSMode;
   if (!gpTTSMode)
      return E_OUTOFMEMORY;
   
   // create the audio source from wave
   HRESULT hRes;
	hRes = CoCreateInstance(CLSID_AudioDestFile, 
		NULL, CLSCTX_INPROC_SERVER, 
		IID_IAudioFile, 
		(PVOID *) &gpIAudioFileTTS);
	if (hRes) return hRes;
   gTTSWANS.m_qwPosn = 0;
	gpIAudioFileTTS -> Register(&gTTSWANS);
   gpIAudioFileTTS->RealTimeSet (gOpt.wTTSSpeed);

   // init the engine
   hRes = gpTTSMode->InitAudioDestObject(gpIAudioFileTTS);
   if (hRes)
      return hRes;
   hRes = gpTTSMode->Init (gOpt.gTTSMode);
   if (hRes)
      return hRes;

   // get the wave format
   PIAUDIO  pIAudio;
   gpIAudioFileTTS->QueryInterface (IID_IAudio, (PVOID*) &pIAudio);
   if (pIAudio) {
      SDATA s;
      hRes = pIAudio->WaveFormatGet (&s);
      pIAudio->Release();
      gpWFEXTTS = (WAVEFORMATEX*) s.pData;
      gdwWFEXTTSSize = s.dwSize;
      if (hRes) {
         Failed ("The text-to-speech engine hasn't told the audio source what wave format to use.");
         return hRes;
      }
   }

   // Need to register
   hRes = gpTTSMode->Register (&gTTSNS, &gdwTTSNSKey);
   if (hRes) {
      return hRes;
   }


   // it all worked
   return NOERROR;
}

/*****************************************************************************
DestroySREngine - Frees the SR engine and relevent memory.
*/
HRESULT DestroySREngine (void)
{
   // depending upon a random value, change the order of deletion
   BOOL  fUp;
   fUp = (rand()%2) ? TRUE : FALSE;

   int   iCount;
   for (iCount = (fUp ? 0 : 1); (iCount >= 0) && (iCount <= 1); iCount = iCount + (fUp ? 1 : -1)) {
      switch (iCount) {
      case 0:
         // get rid of the grammar
         ClearMainGrammar();
         break;
      case 1:
         // get rid of the grammar
         ClearSecondaryGrammar();
         break;
      }
   }

   if (gpIAudioFileSR) {
      gpIAudioFileSR->Flush();
      gpIAudioFileSR->Release();
   }
   gpIAudioFileSR = NULL;

   if (gpSRMode)
      delete gpSRMode;
   gpSRMode = NULL;

   if (gpWFEXSR)
      CoTaskMemFree (gpWFEXSR);
   gpWFEXSR = NULL;

   return NOERROR;
}


/*****************************************************************************
DestroyTTSEngine - Frees the TTS engine and relevent memory.
*/
HRESULT DestroyTTSEngine (void)
{
   if (gpIAudioFileTTS)
      gpIAudioFileTTS->Release();
   gpIAudioFileTTS = NULL;

   if (gpTTSMode)
      delete gpTTSMode;
   gpTTSMode = NULL;

   if (gpWFEXTTS)
      CoTaskMemFree (gpWFEXTTS);
   gpWFEXTTS = NULL;

   return NOERROR;
}



/*****************************************************************************
CFuncSRNotifySink - SR Notify sink
*/

STDMETHODIMP CFuncSRNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   CheckThread();

   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRNotifySinkW)) {
      *ppObj = (LPVOID) this;
      m_dwRefCount++;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CFuncSRNotifySink::AddRef (void)
{
   CheckThread();

   m_dwRefCount++;
   return 1;
}

STDMETHODIMP_ (ULONG) CFuncSRNotifySink::Release (void)
{
   CheckThread();

   m_dwRefCount--;
   return 1;
}

STDMETHODIMP CFuncSRNotifySink::AttribChanged (DWORD dwAttrib)
{
   CheckThread();

   m_fAttribChanged = TRUE;
   m_fNotification = TRUE;
   m_dwAttribChangedAttrib = dwAttrib;
   return NOERROR;
}

STDMETHODIMP CFuncSRNotifySink::Interference (QWORD qwStart, QWORD qwEnd, DWORD dwValue)
{
   CheckThread();

   m_fInterference = TRUE;
   m_fNotification = TRUE;
   m_dwInterferenceValues |= (1 << dwValue);
   return NOERROR;
}

STDMETHODIMP CFuncSRNotifySink::Sound (QWORD qwStart, QWORD qwEnd)
{
   CheckThread();

   m_fSound = TRUE;
   m_fNotification = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncSRNotifySink::UtteranceBegin (QWORD qwStart)
{
   CheckThread();

   m_fUtteranceBegin = TRUE;
   m_fNotification = TRUE;
   m_qwUtteranceBeginTime = qwStart;
   return NOERROR;
}

STDMETHODIMP CFuncSRNotifySink::UtteranceEnd (QWORD qwStart, QWORD qwEnd)
{
   CheckThread();

   m_fUtteranceEnd = TRUE;
   m_fNotification = TRUE;
   m_qwUtteranceEndStart = qwStart;
   m_qwUtteranceEndEnd = qwEnd;
   return NOERROR;
}


STDMETHODIMP CFuncSRNotifySink::VUMeter (QWORD qwTime, WORD wValue)
{
   CheckThread();

   m_fVUMeter = TRUE;
   m_wVUMeterValue = wValue;
   m_qwVUMeterTime = qwTime;
   m_fNotification = TRUE;

   // ignore this since there are too many
   return NOERROR;
}

STDMETHODIMP CFuncSRNotifySink::Training (DWORD dwVal)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fTraining = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncSRNotifySink::Warning (LPUNKNOWN pUnk)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fWarning = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncSRNotifySink::Error (LPUNKNOWN pUnk)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fError = TRUE;
   return NOERROR;
}


/*****************************************************************************
CFuncGramNotifySink - SR grammar notify sink
*/
STDMETHODIMP CFuncGramNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   CheckThread();

   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRGramNotifySinkW)) {
      *ppObj = (LPVOID) this;
      m_dwRefCount++;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CFuncGramNotifySink::AddRef (void)
{
   CheckThread();

   m_dwRefCount++;
   return 1;
}

STDMETHODIMP_ (ULONG) CFuncGramNotifySink::Release (void)
{
   CheckThread();

   m_dwRefCount--;
   return 1;
}

STDMETHODIMP CFuncGramNotifySink::BookMark (DWORD dwMark)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fBookMark = TRUE;
   m_dwBookMarkMark = dwMark;
   return NOERROR;
}

STDMETHODIMP CFuncGramNotifySink::Paused (void)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fPaused = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncGramNotifySink::PhraseFinish (DWORD dwFlags, QWORD qwStart,
                                                   QWORD qwEnd, PSRPHRASEW pSRPhrase,
                                                   LPUNKNOWN pUnk)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fPhraseFinish = TRUE;
   m_dwPhraseFinishFlags = dwFlags;
   m_szPhraseFinishText[0] = 0;
   m_dwPhraseFinishNumWords = 0;
   m_qwPhraseFinishStart = qwStart;
   m_qwPhraseFinishEnd = qwEnd;
   if (pSRPhrase)
      memcpy (m_abPhraseFinishPhrase, pSRPhrase, pSRPhrase->dwSize);
   else
      memset (m_abPhraseFinishPhrase, 0, sizeof(m_abPhraseFinishPhrase));

   // store the result object away
   if (gpSRResult)
      delete gpSRResult;
   gpSRResult = NULL;
   if (pUnk) {
      gpSRResult = new CSRResult;
      if (gpSRResult)
         gpSRResult->Init(pUnk);
   }

   // for debug purposes show what recognized
   // print out the results
	PSRWORDW  pSRWord, pSRMax;
   Information ("PhraseFinish: ", FALSE);
   // NOTE: We're testing to make sure that it's both recognized and it's in this grammar
	if (!pSRPhrase || !(dwFlags & ISRNOTEFIN_THISGRAMMAR) || !(dwFlags & ISRNOTEFIN_RECOGNIZED) ) {
		Information ("[Unrecognized]");
		return NOERROR;
	}

	// write out a string with the results
	// loop through all of the words and display them
	pSRMax = (PSRWORDW) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
	pSRWord = (PSRWORDW) (pSRPhrase->abWords);
   char  szTemp[1000];

	szTemp[0] = 0;

	while (pSRWord < pSRMax)
	{
      // store this away
      m_abPhraseFinishWordNum[m_dwPhraseFinishNumWords++] = pSRWord->dwWordNum;

		// add word
		char	szaTemp[256];
		WideCharToMultiByte (CP_ACP,0, pSRWord->szWord, -1, szaTemp, sizeof(szaTemp),
			NULL, NULL);

      if (pSRWord != (PSRWORDW) (pSRPhrase->abWords))
		   strcat (szTemp, " ");
		strcat (szTemp, szaTemp);

		pSRWord = (PSRWORDW) ((BYTE*) pSRWord + pSRWord->dwSize);
	};
   strcpy (m_szPhraseFinishText, szTemp);
	Information (szTemp);


	return NOERROR;

}


STDMETHODIMP CFuncGramNotifySink::PhraseHypothesis (DWORD dwFlags, QWORD qwStart,
                                                   QWORD qwEnd, PSRPHRASEW pSRPhrase,
                                                   LPUNKNOWN pUnk)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fPhraseHypothesis = TRUE;
   m_qwPhraseHypothesisStart = qwStart;
   m_qwPhraseHypothesisEnd = qwEnd;

   // store the result object away
   if (gpSRResult)
      delete gpSRResult;
   gpSRResult = NULL;
   if (pUnk) {
      gpSRResult = new CSRResult;
      if (gpSRResult)
         gpSRResult->Init(pUnk);
   }

   return NOERROR;
}


STDMETHODIMP CFuncGramNotifySink::PhraseStart (QWORD qwTime)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fPhraseStart = TRUE;
   m_qwPhraseStartTime = qwTime;
   return NOERROR;
}

STDMETHODIMP CFuncGramNotifySink::ReEvaluate (LPUNKNOWN pUnk)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fReEvaluate = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncGramNotifySink::Training (DWORD dwVal)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fTraining = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncGramNotifySink::UnArchive (LPUNKNOWN pUnk)
{
   CheckThread();

   m_fUnArchive = TRUE;
   m_fNotification = TRUE;
   return NOERROR;
}





/*****************************************************************************
CFuncSRWaveAudioNotifySink - Wave audio notify sink that inficates how
	much audio has been processed.
*/

STDMETHODIMP CFuncSRWaveAudioNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IAudioFileNotifySink)) {
      *ppObj = (LPVOID) this;
      m_dwRefCount++;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CFuncSRWaveAudioNotifySink::AddRef (void)
{
   m_dwRefCount++;
   return 1;
}

STDMETHODIMP_ (ULONG) CFuncSRWaveAudioNotifySink::Release (void)
{
   m_dwRefCount--;
   return 1;
}

STDMETHODIMP CFuncSRWaveAudioNotifySink::FileBegin      (DWORD dwVal)
{
   m_fNotification = TRUE;
   m_fFileBegin = TRUE;
   // Do nothing
   return NOERROR;
}

STDMETHODIMP CFuncSRWaveAudioNotifySink::FileEnd        (DWORD dwVal)
{
   m_fNotification = TRUE;
   m_fFileEnd = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncSRWaveAudioNotifySink::QueueEmpty     (void)
{
   m_fNotification = TRUE;
   m_fQueueEmpty = TRUE;
   // do nothing
   return NOERROR;
}

STDMETHODIMP CFuncSRWaveAudioNotifySink::Posn (QWORD qwProcessed, QWORD qwLeft)
{
   m_fNotification = TRUE;
   m_fPosn = TRUE;
   m_qwPosn = qwProcessed;
   return NOERROR;
}


/*****************************************************************************
CFuncTTSNotifySink - TTS Notify sink
*/

STDMETHODIMP CFuncTTSNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   CheckThread();

   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITTSNotifySinkW)) {
      *ppObj = (LPVOID) this;
      m_dwRefCount++;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CFuncTTSNotifySink::AddRef (void)
{
   CheckThread();

   m_dwRefCount++;
   return 1;
}

STDMETHODIMP_ (ULONG) CFuncTTSNotifySink::Release (void)
{
   CheckThread();

   m_dwRefCount--;
   return 1;
}

STDMETHODIMP CFuncTTSNotifySink::AttribChanged (DWORD dwAttrib)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fAttribChanged = TRUE;
   m_dwAttribChangedAttrib = dwAttrib;
   return NOERROR;
}

STDMETHODIMP CFuncTTSNotifySink::AudioStart (QWORD qwStart)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fAudioStart = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncTTSNotifySink::AudioStop (QWORD qwStart)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fAudioStop = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncTTSNotifySink::Visual (QWORD qwTime, WCHAR wc1, WCHAR wc2, DWORD dwFlags, PTTSMOUTH pMouth)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fVisual = TRUE;
   if (pMouth)
      m_VisualMouth = *pMouth;
   else
      memset (&m_VisualMouth, 0, sizeof(m_VisualMouth));
   m_cVisualIPA = wc1;
   return NOERROR;
}




/*****************************************************************************
CFuncTTSBufNotifySink - TTS buffer notify sink
*/
STDMETHODIMP CFuncTTSBufNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   CheckThread();

   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITTSBufNotifySinkW)) {
      *ppObj = (LPVOID) this;
      m_dwRefCount++;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CFuncTTSBufNotifySink::AddRef (void)
{
   CheckThread();

   m_dwRefCount++;
   return 1;
}

STDMETHODIMP_ (ULONG) CFuncTTSBufNotifySink::Release (void)
{
   CheckThread();

   m_dwRefCount--;
   return 1;
}

STDMETHODIMP CFuncTTSBufNotifySink::BookMark (QWORD qwTime, DWORD dwMark)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fBookMark = TRUE;
   m_dwBookMarkMark = dwMark;
   return NOERROR;
}


STDMETHODIMP CFuncTTSBufNotifySink::TextDataDone (QWORD qwTime, DWORD dwMark)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fTextDataDone = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncTTSBufNotifySink::TextDataStarted (QWORD qwTime)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fTextDataStarted = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncTTSBufNotifySink::WordPosition (QWORD qwTime, DWORD dwVal)
{
   CheckThread();

   m_fNotification = TRUE;
   m_fWordPosition = TRUE;
   m_dwWordPositionVal = dwVal;
   return NOERROR;
}


/*****************************************************************************
CFuncTTSWaveAudioNotifySink - Wave audio notify sink that inficates how
	much audio has been processed.
*/

STDMETHODIMP CFuncTTSWaveAudioNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IAudioFileNotifySink)) {
      *ppObj = (LPVOID) this;
      m_dwRefCount++;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CFuncTTSWaveAudioNotifySink::AddRef (void)
{
   m_dwRefCount++;
   return 1;
}

STDMETHODIMP_ (ULONG) CFuncTTSWaveAudioNotifySink::Release (void)
{
   m_dwRefCount--;
   return 1;
}

STDMETHODIMP CFuncTTSWaveAudioNotifySink::FileBegin      (DWORD dwVal)
{
   m_fNotification = TRUE;
   m_fFileBegin = TRUE;
   // Do nothing
   return NOERROR;
}

STDMETHODIMP CFuncTTSWaveAudioNotifySink::FileEnd        (DWORD dwVal)
{
   m_fNotification = TRUE;
   m_fFileEnd = TRUE;
   return NOERROR;
}

STDMETHODIMP CFuncTTSWaveAudioNotifySink::QueueEmpty     (void)
{
   m_fNotification = TRUE;
   m_fQueueEmpty = TRUE;
   // do nothing
   return NOERROR;
}

STDMETHODIMP CFuncTTSWaveAudioNotifySink::Posn (QWORD qwProcessed, QWORD qwLeft)
{
   m_fNotification = TRUE;
   m_fPosn = TRUE;
   m_qwPosn = qwProcessed;
   return NOERROR;
}




/*****************************************************************
WaitForEvent - Loops in a peekmessage loop and waits until one
   of the BOOL values has been set to TRUE, or until a time-out
   has occurred, or until gfAbortTest is true.

inputs
   float    fTimeOut - Time out in seconds
   BOOL     *pf1 through pf5- Bool to wait on
returns
   BOOL * - Pointer to the value that was set to true, or
            &gfAbortTest if that was set, or NULL if timed out
*/
BOOL *WaitForEvent (float fTimeOut, BOOL *pf1, BOOL *pf2, BOOL *pf3,
                    BOOL *pf4, BOOL *pf5)
{
   // set the wait values to FALSE
   if (pf1)
      *pf1 = NULL;
   if (pf2)
      *pf2 = NULL;
   if (pf3)
      *pf3 = NULL;
   if (pf4)
      *pf4 = NULL;
   if (pf5)
      *pf5 = NULL;

   // wait
   DWORD dwTimeOut;
   dwTimeOut = GetTickCount() + (DWORD) (fTimeOut * 1000);
   while (TRUE) {
      // should we quit?
      if (gfAbortTest)
         return &gfAbortTest;
      if (pf1 && *pf1)
         return pf1;
      if (pf2 && *pf2)
         return pf2;
      if (pf3 && *pf3)
         return pf3;
      if (pf4 && *pf4)
         return pf4;
      if (pf5 && *pf5)
         return pf5;
      if (GetTickCount() > dwTimeOut)
         return NULL;


      // peek message
      MSG   msg;
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
         TranslateMessage (&msg);
         DispatchMessage (&msg);
      }

      // sleep awhile
      Sleep (25);
   }
}

BOOL *WaitForEvent (BOOL *pf1, BOOL *pf2, BOOL *pf3,
                    BOOL *pf4, BOOL *pf5)
{
   return WaitForEvent (1000.0, pf1, pf2, pf3, pf4, pf5);
}


/*********************************************************************
ReSample - This loads in a .wav file and makes sure
   that it has the right data format (sampling rate - basically). If
   it doens't it asks the user if they wish to change the file. If the
   user presses yes then the file is converted to the right format.

   This function uses a COM object provided in the SDK by the wave
   editor.

inputs
   char    *pszaWave - Wave file
   BOOL     fAddNoise - If TRUE, add noise
returns
   HRESULT - error
*/
HRESULT ReSample (char *pszaWave, BOOL fAddNoise)
{
   WAVEFORMATEX   *pWFEX;
   DWORD dwWFEXSize;
   WCHAR pszWave[256];
   MultiByteToWideChar (CP_ACP, 0, pszaWave, -1, pszWave, sizeof(pszWave)/2);

   pWFEX = gpWFEXSR;
   dwWFEXSize = gdwWFEXSRSize;

   PIWAVEFX   pIWaveFX = NULL;
   PIWAVEFX   pIWaveFXNoise = NULL;
   PIWAVEDATA pIWaveData = NULL;
   HRESULT hRes = NOERROR;

   pIWaveData = NULL;


   // Create TTS transform
   hRes = CoCreateInstance (CLSID_WaveFXFormat, NULL, CLSCTX_ALL, IID_IWaveFX, (void**)&pIWaveFX);
   if (hRes) {
      return E_FAIL;
   }

   hRes = CoCreateInstance (CLSID_WaveFXNoise, NULL, CLSCTX_ALL, IID_IWaveFX, (void**)&pIWaveFXNoise);
   if (hRes) {
      pIWaveFX->Release();
      return E_FAIL;
   }

   // Create wave data
   hRes = CoCreateInstance (CLSID_WaveData, NULL, CLSCTX_ALL, IID_IWaveData, (void**)&pIWaveData);
   if (hRes) {
      pIWaveFXNoise->Release();
      pIWaveFX->Release();
      return E_FAIL;
   }


   // load it in
   hRes = pIWaveData->FromFile (pszWave);
   if (hRes) {
      // no file here
      goto theend;
   }

   PWAVEFORMATEX  pOld;
   pOld = NULL;
   pIWaveData->GetWFEX(&pOld);
   if (!pOld) {
      return E_FAIL;
   }
   if (!memcmp(pOld, (PVOID) pWFEX, sizeof(PCMWAVEFORMAT))) {
      // same format
      hRes = NOERROR;
      goto noise;
   }
   
   // else we need to downsample
   // do the FX
   hRes = pIWaveFX->FX(ghWndEditor, pIWaveData,
                     0, 0, 0,
                     pWFEX, dwWFEXSize, NULL);
   if (hRes)
      goto theend;
   if (!hRes) {
      // write out the file. First convert file name to unicode
      pIWaveData->ToFile(pszWave);

   }

noise:
   // add some noise so SR doesn't complain
   if (fAddNoise) {
      FXNOISE  fn;
      fn.fEntireWave = TRUE;
      fn.wNoiseAmp = 128;
      hRes = pIWaveFXNoise->FX(ghWndEditor, pIWaveData,
                        0, 0, 0,
                        &fn, sizeof(fn), NULL);
   }

   // write out the file. First convert file name to unicode
   hRes = pIWaveData->ToFile(pszWave);

theend:
   // Free up TTS transform
   if (pIWaveFX)
      pIWaveFX->Release();
   if (pIWaveFXNoise)
      pIWaveFXNoise->Release();
   pIWaveFX = NULL;
   if (pIWaveData)
      pIWaveData->Release();
   pIWaveData = NULL;


   return hRes;

}

/****************************************************************
GenerateWaveName - Generates a wave name. Then, tells the TTS to
   use it.

inputs
   char  *psz - Filled with the wave name.
returns
   HRESULT - error
*/
HRESULT GenerateWaveName (char *psz)
{
   // chose a temporary wave name
   char  szTemp[256];
   WCHAR szwTemp[256];
   gdwCurWaveID = (gdwCurWaveID + 1) % NUMTEMPWAVE;
   wsprintf (szTemp, TEMPWAVENAME, gdwCurWaveID);
   if (psz)
      strcpy (psz, szTemp);
   MBTWC (szTemp, szwTemp);

   // tell the tts audio object to use this
   HRESULT hRes;
   hRes = gpIAudioFileTTS->Set (szwTemp, 1);
   if (hRes) {
      Failed ("TTS's IWaveAudio::Set", hRes);
      return hRes;
   }

   return NOERROR;
}


/****************************************************************
SpeakToWave - Speaks text-to-speech to a wave file.
   It:
      - picks a temporary wave file name
      - sends text to the TTS engine
      - waits for an audio stop notification
      - clears out the temporary wave file name from the TTS object
      - does a sndplaysound() if the user has requested to hear the audio

inputs
   char  *pszSpeak - Text to speak
   char  *pszFile - Filled in with the file name
   DWORD *pdwUsed - Filled in with the number of bytes written
   BOOL  *pfAbort - Abort case - such as a bookmark
   BOOL  fIPA - If TRUE, this is an IPA string
returns
   HRESULT - error
*/

HRESULT SpeakToWave (char *pszSpeak, char *pszFile,
                     DWORD *pdwUsed, BOOL *pfAbort)
{
   // send the text to the TTS engine
   WCHAR szHuge[100000];
   MultiByteToWideChar (CP_ACP, 0, pszSpeak, -1, szHuge,
      sizeof(szHuge)/2);

   return SpeakToWave (szHuge, pszFile, pdwUsed, pfAbort);
}

HRESULT SpeakToWave (WCHAR *szHuge, char *pszFile,
                     DWORD *pdwUsed, BOOL *pfAbort,
                     BOOL fIPA)
{
   // chose a temporary wave name
   char  szTemp[256];
   HRESULT hRes;
   hRes = GenerateWaveName (szTemp);
   if (hRes)
      return hRes;
   if (pszFile)
      strcpy (pszFile, szTemp);

   // get the position
   QWORD qwLast;
   PIAUDIO pia;
   gpIAudioFileTTS->QueryInterface (IID_IAudio, (PVOID*) &pia);
   pia->TotalGet(&qwLast);
   // send the text to the TTS engine
   if (fIPA) {
      SDATA sd;
      sd.pData = szHuge;
      sd.dwSize = (wcslen(szHuge)+1)*2;
      hRes = gpTTSMode->TextData (CHARSET_IPAPHONETIC, TTSDATAFLAG_TAGGED,
         sd, &gTTSBufNS);
   }
   else
      hRes = gpTTSMode->Speak (szHuge, TRUE, &gTTSBufNS);

   // print out that we speak this
   char  szaHuge[100000];
   WCTMB (szHuge, szaHuge);
   Information ("Speak: ", FALSE);
   Information (szaHuge);

   // wait for the audio stop notification
	BOOL *pf;
	pf = WaitForEvent (&gTTSNS.m_fAudioStop, pfAbort);

   hRes = NOERROR;
   if (!pf) {
	    Failed ("Speaking text to TTS timed out");
	    hRes = E_FAIL;
	    // fall on through
   }
   else if (pf == &gfAbortTest) {
	   Failed ("User aborted");
	   hRes = E_FAIL;
	   // fall on through
   }
   else if (pf == pfAbort) {
	   hRes = S_FALSE;
	   // fall on through
   }

   // it stopped, clear out the temporary wave
   gpIAudioFileTTS->Set (NULL, 1);

   // if we're supposed to play this then do so
   if (gOpt.fPlayTTS) {
      sndPlaySound (NULL, 0);
      sndPlaySound (szTemp, SND_ASYNC);
   }

   // how much data was written
   QWORD qwTemp;
   pia->TotalGet(&qwTemp);
   pia->Release();
   if (pdwUsed)
      *pdwUsed = (DWORD) (qwTemp - qwLast);

   // all done
   return hRes;
}




/****************************************************************
SpeakToSR - Speaks text-to-speech to the speech recognition engine.
   It:
      - picks a temporary wave file name
      - inserts some extra silence before/after the text
      - sends text to the TTS engine
      - waits for an audio stop notification
      - clears out the temporary wave file name from the TTS object
      - resamples the audio to the sR engine's format
      - adds this to the speech recognition engine
      - does a sndplaysound() if the user has requested to hear the audio

inputs
   char  *pszSpeak - Text to speak
   BOOL  fSilence - If TRUE, add lots of silence to beginning & end
   BOOL  fIPA - If TRUE, this is an IPA string (pszSpeak is really WCHAR)
   BOOL  fAddNoise - if TRUE (default), add noise, else not
returns
   HRESULT - error
*/
HRESULT SpeakToSR (char *pszSpeak, BOOL fSilence, BOOL fIPA, BOOL fAddNoise)
{
   // chose a temporary wave name
   char  szTemp[256];
   WCHAR szwTemp[256];
   HRESULT hRes;
   hRes = GenerateWaveName (szTemp);
   if (hRes)
      return hRes;
   MBTWC (szTemp, szwTemp);


   // send the text to the TTS engine
   WCHAR szHuge[100000];
   if (fSilence)
      wcscpy (szHuge, L"\\Pau=1000\\");
   else
      szHuge[0] = 0;
   if (fIPA)
      wcscpy (szHuge + wcslen(szHuge), (WCHAR*) pszSpeak);
   else
      MultiByteToWideChar (CP_ACP, 0, pszSpeak, -1, szHuge + wcslen(szHuge),
         sizeof(szHuge)/2);
   if (fSilence)
      wcscat (szHuge, L"\\Pau=4000\\");
   if (fIPA) {
      SDATA sd;
      sd.pData = szHuge;
      sd.dwSize = (wcslen(szHuge)+1)*2;
      hRes = gpTTSMode->TextData (CHARSET_IPAPHONETIC, TTSDATAFLAG_TAGGED,
         sd, &gTTSBufNS);
   }
   else
      hRes = gpTTSMode->Speak (szHuge, TRUE, &gTTSBufNS);

   // print out that we speak this
   Information ("Speak: ", FALSE);
   if (fIPA)
      Information ("IPA characters");
   else
      Information (pszSpeak);

   // wait for the audio stop notification
   BOOL *pf;
   pf = WaitForEvent (&gTTSNS.m_fAudioStop);
   if (!pf) {
      Failed ("Speaking text to TTS timed out");
      hRes = E_FAIL;
      gpIAudioFileTTS->Set (NULL, 1);
      return hRes;
   }
   else if (pf == &gfAbortTest) {
      Failed ("User aborted");
      hRes = E_FAIL;
      gpIAudioFileTTS->Set (NULL, 1);
      return hRes;
   };

   // it stopped, clear out the temporary wave
   gpIAudioFileTTS->Set (NULL, 1);

   // resample it
   hRes = ReSample (szTemp, fAddNoise);
   if (hRes) {
      Failed ("ReSampling to engine's file format failed", hRes);
      return hRes;
   }


   // add it to the speech recognition engine's queue
   hRes =gpIAudioFileSR->Add (szwTemp, 1);
   if (hRes) {
      Failed ("Add wave to SR's audio source", hRes);
      return hRes;
   }

   // if we're supposed to play this then do so
   if (gOpt.fPlayTTS) {
      sndPlaySound (NULL, 0);
      sndPlaySound (szTemp, SND_ASYNC);
   }

   // all done
   return NOERROR;
}

/****************************************************************
SpeakAndListen - Speaks some audio to the text to speech engine,
   and has speech recognition listen for a response with the
   specified grammar.

inputs
   char     *pszSpeak - text to speak
   char     *pszGram - grammar
   BOOL     fFailIfTimeOut - If TRUE, then fail if the phrase
               times out. Else, just return NULL string, as though
               rejected.
returns
   char* - NULL if timed-out or otherwise failed (error will have
            been logged), pointer to a string. If the string is
            empty, it was unrecognized, else it's what's recognized
*/
char *SpeakAndListen (char*pszSpeak, char*pszGram, BOOL fFailIfTimeOut)
{
   HRESULT hRes;

   // checks to make sure the engine claims to support this
   if ((gTTSModeInfo.dwFeatures & TTSFEATURE_FIXEDAUDIO) || !gpWFEXTTS ||
      (gSRModeInfo.dwFeatures & SRFEATURE_FIXEDAUDIO) || !gpWFEXSR
      )
   {
      NotApplicable();
      return NULL;
   }

   //use this to make sure the SR engine supports CFGs
   if (!(gSRModeInfo.dwGrammars & SRGRAM_CFG) || 
      (gSRModeInfo.dwSequencing != SRSEQUENCE_CONTINUOUS) 
      )
   {
      NotApplicable();
      return NULL;
   }

   // speak Hello worlds to text-to-speech
   hRes = SpeakToSR (pszSpeak);
   if (hRes) {
      Failed ("SpeakToSR", hRes);
      return NULL;
   }

   // See if SR hears it
   hRes = SetMainGrammar (pszGram);
   if (hRes) {
      Failed ("Load grammar", hRes);
      return NULL;
   }

   // see if we hear anything. We don't care if it's the right thing
   hRes = WaitForPhraseFinish();
   if (hRes) {
      if (fFailIfTimeOut)
         Failed ("WaitForPhraseFinish", hRes);
      ClearMainGrammar();
      return NULL;
   }

   ClearMainGrammar();

   return gSRGramNS.m_szPhraseFinishText;
}

/****************************************************************
VeryifyRecognition - Speaks text to TTS and uses the speech
recognizer to verify that the desired result was recognized.
Returns hRes=NOERROR if recognized, else error
*/
HRESULT VerifyRecognition (char*pszSpeak, char*pszGram)
{
   char  *pRet;

   pRet = SpeakAndListen (pszSpeak, pszGram);
   if (!pRet)
      return E_FAIL;
   return stricmp(pszSpeak, pRet) ? E_FAIL : NOERROR;
}

/****************************************************************
ClearSRAudio - Clears the SR audio queue of all audio
*/
void ClearSRAudio (void)
{
   if (gpIAudioFileSR) {
      gpIAudioFileSR->Flush();
   }
}


/****************************************************************
WaitForPhraseFinish - This waits until the speech rcognizer responds
   with a phrase finish, or there's a time out, or there's a gfAbort.

   It activated, waits for a recognition or time out, deactivates,
   and then clears the sr audio queue.

inputs
   HWND - hWnd to activate with
returns
   HRESULT - No error if got a phrase, S_FALSE if timed out or there was an abort message
*/
HRESULT WaitForPhraseFinish (HWND hWnd)
{
   BOOL  *pf;

   // activate the grammar
   HRESULT  hRes;
   hRes = gpSRGramComp->Activate(hWnd);
   if (hRes) {
      Failed ("Activate", hRes);
      return FALSE;
   }

   // wait for the event
   pf = WaitForEvent (60.0, &gSRGramNS.m_fPhraseFinish);

   // deactivate the grammar
   hRes = gpSRGramComp->Deactivate();

   // clear the audio
   ClearSRAudio();

   if (pf == &gSRGramNS.m_fPhraseFinish)
      return NOERROR;
   else
      return S_FALSE;
}


/*******************************************************************
ClearMainGrammar - Clears the grammar if it already exists.
*/
void ClearMainGrammar (void)
{
   BOOL  fUp;
   fUp = (rand()%2) ? TRUE : FALSE;

   int   iCount;
   for (iCount = (fUp ? 0 : 1); (iCount >= 0) && (iCount <= 1); iCount = iCount + (fUp ? 1 : -1)) {
      switch (iCount) {
      case 0:
         if (gpSRGramComp)
            delete gpSRGramComp;
         gpSRGramComp = NULL;
         break;
      case 1:
         if (gpSRResult)
            delete gpSRResult;
         gpSRResult = NULL;
         break;
      }
   }

}


/*******************************************************************
ClearSecondaryGrammar - Clears the grammar if it already exists.
*/
void ClearSecondaryGrammar (void)
{
   BOOL  fUp;
   fUp = (rand()%2) ? TRUE : FALSE;

   int   iCount;
   for (iCount = (fUp ? 0 : 1); (iCount >= 0) && (iCount <= 1); iCount = iCount + (fUp ? 1 : -1)) {
      switch (iCount) {
      case 0:
         if (gpSRGramCompSecondary)
            delete gpSRGramCompSecondary;
         gpSRGramCompSecondary = NULL;
         break;
      case 1:
         if (gpSRResult)
            delete gpSRResult;
         gpSRResult = NULL;
         break;
      }
   }

}

/*******************************************************************
SetMainGrammar - Sets the main grammar, either based upon a unicode
   text string, or an ANSI text string, or a resource ID.

   If another main grammar already exists this clears it.

inputs
   none
returns
   HRRESULT - error
*/
HRESULT SetMainGrammar (PCWSTR psz)
{
   ClearMainGrammar();
   gpSRGramComp = gpSRMode->GrammarFromMemory (&gSRGramNS, (PVOID) psz, (wcslen(psz)+1)*2);
   if (!gpSRGramComp)
      return E_FAIL;
   return NOERROR;
}

HRESULT SetMainGrammar (PCSTR psz)
{
   ClearMainGrammar();
   gpSRGramComp = gpSRMode->GrammarFromMemory (&gSRGramNS, (PVOID) psz, strlen(psz)+1);
   if (!gpSRGramComp)
      return E_FAIL;
   return NOERROR;
}


HRESULT SetMainGrammar (DWORD dwRes)
{
   ClearMainGrammar();
   gpSRGramComp = gpSRMode->GrammarFromResource (&gSRGramNS, ghInstance, dwRes);
   if (!gpSRGramComp)
      return E_FAIL;
   return NOERROR;
}

HRESULT SetSecondaryGrammar (PCSTR psz)
{
   ClearSecondaryGrammar();
   gpSRGramCompSecondary = gpSRMode->GrammarFromMemory (&gSRGramNSSecondary, (PVOID) psz, strlen(psz)+1);
   if (!gpSRGramCompSecondary)
      return E_FAIL;
   return NOERROR;
}


/********************************************************************
CleanSlate - Cleans out all the grammars and the queued up audio
*/
void CleanSlate (void)
{
   ClearMainGrammar();
   ClearSecondaryGrammar();
   ClearSRAudio();
   gpTTSMode->AudioReset();

   // set the rejection threshhold
   if (gpSRMode)
      gpSRMode->ThresholdSet (0);
}


/********************************************************************
ConstructList - This takes a doublly-null termianted string and
   constructs data to be used in a list. It uses a temporary
   buffer, and returns a pointer to that

inputs
   WCHAR    *psz - Doublly null terminated
   DWORD    *pdwSize - fills in the size
returns
   PVOID - buffer - This is only valid until ConstructList is called again
*/
PVOID ConstructList (WCHAR *psz, DWORD *pdwSize)
{
   static BYTE bBuf[100000];  // temporary buffer


   // start the word
   PBYTE pCur = bBuf;
   int   iLen;
   while (iLen = wcslen(psz)) {
      PSRWORDW pWord;
      pWord = (PSRWORDW) pCur;
      pWord->dwSize = (iLen + 1) * 2 + sizeof(SRWORDW);
      pWord->dwSize = (pWord->dwSize + 3) & ~0x03;   // dword align
      pWord->dwWordNum = 0;
      wcscpy (pWord->szWord, psz);

      pCur += pWord->dwSize;
      psz += (iLen+1);
   }

   *pdwSize = (pCur - bBuf);
   return (PVOID) bBuf;
}



/*****************************************************************
GetResultsObjects - Speaks some audio to the recognizer so that
it gets a new results object.

inputs
   char     *pszSpeak - Text to speak. If this is NULL,
               a default is used.
   char     *pszGrammar - Text of the grammar to use. If this is NULL,
               the current Main grammar is used.
returns
   PCSRResults - Results object. The application must delete this.
            If this is NULL, no results object gotten.
*/
PCSRResult GetResultsObject (char *pszSpeak, char *pszGrammar)
{
   HRESULT hRes;
   if (pszGrammar) {
      hRes = SetMainGrammar (pszGrammar);
      if (hRes)
         return NULL;
   }

   // if there's already a results object cached the free it
   if (gpSRResult) {
      delete gpSRResult;
      gpSRResult = NULL;
   }

   // speak
   hRes = SpeakToSR (pszSpeak ? pszSpeak :
      "All work and no play makes Jack a dull boy.");
   if (hRes)
      return NULL;
   hRes = WaitForPhraseFinish();
   if (hRes)
      return NULL;

   if (gpSRResult) {
      PCSRResult  pTemp;
      pTemp = gpSRResult;
      gpSRResult = NULL;
      return pTemp;
   }
   else
      return NULL;
}


/********************************************************************
ConstructPhrase - This takes a doublly-null termianted string and
   constructs data to be used in a list. It uses a temporary
   buffer, and returns a pointer to that

inputs
   WCHAR    *psz - Doublly null terminated
   DWORD    *pdwSize - fills in the size
returns
   PVOID - buffer - This is only valid until ConstructList is called again
*/
PVOID ConstructPhrase (WCHAR *psz, DWORD *pdwSize)
{
   static BYTE bBuf[1000];  // temporary buffer


   // start the word
   PBYTE pCur = bBuf + sizeof(DWORD);
   int   iLen;
   while (iLen = wcslen(psz)) {
      PSRWORDW pWord;
      pWord = (PSRWORDW) pCur;
      pWord->dwSize = (iLen + 1) * 2 + sizeof(SRWORDW);
      pWord->dwSize = (pWord->dwSize + 3) & ~0x03;   // dword align
      pWord->dwWordNum = 0;
      wcscpy (pWord->szWord, psz);

      pCur += pWord->dwSize;
      psz += (iLen+1);
   }

   *pdwSize = (pCur - bBuf);
   *pdwSize = (*pdwSize + 3) & ~0x03;  // dwrod alight

   // store this in the buffer
   *((DWORD*)bBuf) = *pdwSize;

   return (PVOID) bBuf;
}



