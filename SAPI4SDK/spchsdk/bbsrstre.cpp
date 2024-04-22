/**********************************************************************

SRStress.cpp - Stress test application

Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <initguid.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <dsound.h>
#include <spchwrap.h>
#include <process.h>
#include "resource.h"
#include "SRStress.h"

void _cdecl StressThread (PVOID pData);

#define  MAXMODES       100    // maximum # of modes a user can select
#define  MAXINSTANCES   100    // maximum # of engine instances we can have
#define  MAXFILES       200    // maximum number of grammar files
#define  MAXNAMES       100     // maximum number of random speaker names



class CStress;

class CStressNotifySink : public ISRNotifySinkW {
   public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ISRNotifySink
      STDMETHODIMP AttribChanged  (DWORD);
      STDMETHODIMP Interference   (QWORD, QWORD, DWORD);
      STDMETHODIMP Sound          (QWORD, QWORD);
      STDMETHODIMP UtteranceBegin (QWORD);
      STDMETHODIMP UtteranceEnd   (QWORD, QWORD);
      STDMETHODIMP VUMeter        (QWORD, WORD);

      CStress     *m_pStress;
};

class CStressGramNotifySink: public ISRGramNotifySinkW {
   public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ISRNotifySink
	   STDMETHODIMP BookMark       (DWORD);
	   STDMETHODIMP Paused         (void);
	   STDMETHODIMP PhraseFinish   (DWORD, QWORD, QWORD, PSRPHRASEW, LPUNKNOWN);
	   STDMETHODIMP PhraseHypothesis(DWORD, QWORD, QWORD, PSRPHRASEW, LPUNKNOWN);
	   STDMETHODIMP PhraseStart    (QWORD);
	   STDMETHODIMP ReEvaluate     (LPUNKNOWN);
	   STDMETHODIMP Training       (DWORD);
	   STDMETHODIMP UnArchive      (LPUNKNOWN);

      CStress     *m_pStress;
};

class CStressWaveAudioNotifySink : public IAudioFileNotifySink {
public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // IAudioSourceNotifySink members
      STDMETHODIMP FileBegin      (DWORD);
      STDMETHODIMP FileEnd        (DWORD);
      STDMETHODIMP QueueEmpty     (void);
      STDMETHODIMP Posn           (QWORD, QWORD);

      CStress     *m_pStress;
};



#define     MAXGRAM     100
#define     MAXRESULT   500
#define     TIMERID     4289

BOOL StatusDialog (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WCHAR * GenerateRandomText (DWORD dwNumBytes = 0);

class CStress {
public:
   CStress (DWORD dwNum);
   ~CStress (void);
   void DoThread (void);
   void FreeAll (void);
   BOOL DoDialog (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
   void Output (char *psz, BOOL fClear = FALSE);
   void Output (HRESULT hRes, BOOL fClear = FALSE);
   DWORD Random (DWORD dwRange);
   BOOL OnlyIfDifficult (void);
   BOOL OnlyIfVeryDifficult (void);
   WCHAR *RandomSpeaker (void);
   void OnTimer (void);
   HRESULT MakeSureHaveEngine (void);
   HRESULT MakeSureHaveGrammar (DWORD dwNum);
   HRESULT MakeSureHaveResult (DWORD dwNum);
   void MadeCall (void);
   void StressEngine (void);
   void StressOnCallback (void);
   void StressGrammar (DWORD dwNum);
   void StressResult (DWORD dwNum);
   DWORD RandomSRWORD (BYTE *pBuf);

	// variables
	DWORD		      m_dwThreadNum;		// thread number. 0 is the first one. Each one follows
	CStressNotifySink *m_pNS;		// notification sink
	CStressGramNotifySink *m_pGNS;	// notification sink
	CStressWaveAudioNotifySink *m_pWANS;	// notification sink
	CSRMode		   *m_pMode;			// speech recognition mode
   PIAUDIOFILE    m_pIWaveAudio; // audio object for wave.
	PCSRGramComp   m_apGramComp[MAXGRAM];  // grammar object. Exclusive with m_apGram
   PCSRGram       m_apGram[MAXGRAM];      // grammar object. Exclusive with m_apGramComp
   PCSRResult     m_apResult[MAXRESULT];  // results objects
   HWND           m_hWnd;           // window handle for the dialog
   DWORD          m_dwTimerElapse;  // number of MS between timer

   DWORD          m_dwPhrases;      // total phrase count that heard
   QWORD          m_qwTotalAudio;   // total audio passed through
   QWORD          m_qwMaxAudio;     // max audio from this object
   QWORD          m_qwFunctionCalls;// total number of function calls

   FILE           *m_file;           // file to log to
};

#define		DIFFICULTY_EASY		0
#define		DIFFICULTY_MEDIUM		1
#define		DIFFICULTY_HARD		2


CSREnum     *pEnum = NULL;
GUID        gagMode[MAXMODES];
DWORD       gdwNumModes = 0;  // number of modes used
DWORD       gdwDifficulty = 0;   //0 = easy, 1 = medium, 2 = hard
BOOL			gfNonRandomString = FALSE;		// if true then generate string of form 'aaa...'
BOOL			gfNoTags = FALSE;					// if true then no tags are imbedded in strings
HINSTANCE   ghInstance;
DWORD       gdwNumFiles = 0;  // number of files
char        gszFileName[MAXFILES][256];
char        gszWaveFile[256];
WCHAR       gszwWaveFile[256];
WCHAR       gaszSpeakerNames[MAXNAMES][128];
BOOL        gfUseMultimedia;  // set to TRUE if we use the multimedia for the first thread
BOOL			gfGenerateLogFile;	// set to TRUE if log file is to be generated.
DWORD       gdwNumThreads;    // number of threads to create
HANDLE      gahEventShutDownNow[MAXINSTANCES];  // event signaling thread should shut down
HANDLE      gahEventIsShutDown[MAXINSTANCES];   // event signaling that thread has shut down
BOOL			gfTestEngine = TRUE;		// set to TRUE if test the engine test cases
BOOL			gfTestGrammar = TRUE;		// set to TRUE if test the Grammar object cases
BOOL			gfTestResult = TRUE;		// set to TRUE if test the result cases
BOOL			gfEngineCases[MAXENGINECASES];		// lists engine cases to test
BOOL			gfGrammarCases[MAXGRAMMARCASES];		// lists Grammar cases to test
BOOL			gfResultCases[MAXRESULTCASES];		// lists results cases to test


/*******************************************************
ProduceTag - This produces a tag in the specified
   memory and NULL-terminates it.

inputs
   char     *psz - String. At least 32 characters
   DWORD    dwType - 0 for easy, 1 for medium, 2 for hard
*/
void ProduceTag (WCHAR *psz, DWORD dwType)
{
   int iVal;
   iVal = rand()% 8;
   if (dwType == 0)
      iVal = min(4, iVal);

   switch (iVal) {
   case 0:
      wcscpy (psz, L"\\rst\\");
      break;
   case 1:
      swprintf (psz, L"\\vol=%d\\", (int) rand() % 65535);
      break;
   case 2:
      swprintf (psz, L"\\pau=%d\\", (int) rand() % 5000);
      break;
   case 3:
      swprintf (psz, L"\\pit=%d\\", (int) rand() % 500);
      break;
   case 4:
      swprintf (psz, L"\\spd=%d\\", (int) rand() % 500);
      break;
   case 5:
      swprintf (psz, L"\\mrk=%d\\", (int) rand());
      break;
   case 6:
      swprintf (psz, L"\\pro=%d\\", (int) rand());
      break;
   case 7:
      wcscpy (psz, L"\\emp\\");
      break;
   default:
      // error: unbounded
      wcscpy (psz, L"\\");
   }

}


/*******************************************************
GenerateCharacter - Generates a random character.

inputs
   DWORD dwType - 0 for alphanumeric and space,
      1 for < 128, 2 for any
*/
WCHAR GenerateCharacter (DWORD dwType)
{
   int   iVal;
   iVal = rand() % 9;
   if (dwType == 0)
      iVal = min(3, iVal);
   else if (dwType = 1)
      iVal = min(6, iVal);

	// if no random characters are to be generated the use a string of 'a'
	if (gfNonRandomString) 
		return 'a';
	
	switch (iVal) {
   case 0:
      // space
      return ' ';
   case 1:
   case 2:
   case 3:
      // lower case letter
      return (WCHAR) (rand() % 26) + 'a';
   case 4:
      // upper case letter
      return (WCHAR) (rand() % 26) + 'A';
   case 5:
      // number
      return (WCHAR) (rand() % 10) + '0';
   case 6:
      // anything < 128
      return (WCHAR) (rand() % 127) + 1;
   case 7:
      // anything
      return (WCHAR) (rand() % 255) + 1;
   default:
      // anything
      return (WCHAR) (rand() % 65536) + 1;
   }
}



/*******************************************************
FillBufWithText - Fills a buffer with text and terminates
   with a NULL.

inputs
   DWORD dwType - 0 for alphanumeric and space,
      1 for < 128, 2 for any
   WCHAR     *pBuf - Buffer to fill
   DWORD    dwNumChars - Number of characters. Including NULL
*/
void FillBufWithText (DWORD dwType, WCHAR *pBuf, DWORD dwNumChars)
{
   DWORD i;
   for (i = 0; (i+1) < dwNumChars; i++)
      pBuf[i] = GenerateCharacter (dwType);
   if (dwNumChars)
      pBuf[dwNumChars-1] = 0;
   else
      pBuf[0] = 0;
}


/*******************************************************
GenerateRandomText - Generates random text. The returned
   memory must be freed with free(). dwSize = # of bytes
*/
WCHAR * GenerateRandomText (DWORD dwSize)
{
   if (!dwSize) {
      // figure out how much
      int   iVal;
      iVal = rand() % 6;
      if (gdwDifficulty == DIFFICULTY_EASY)
         iVal = iVal % 2;
      else if (gdwDifficulty == DIFFICULTY_MEDIUM)
         iVal = iVal = iVal % 4;

      switch (iVal) {
      case 0:
         dwSize = (DWORD) (rand() % 10) + 2;
         break;
      case 1:
         dwSize = (DWORD) (rand() % 100) + 2;
         break;
      case 2:
         dwSize = (DWORD) (rand() % 1000) + 2;
         break;
      case 3:
         dwSize = (DWORD) (rand() % 10000) + 2;
         break;
      default:
         dwSize = (DWORD) (rand() % 100000) + 2;
         break;
      }
   }
   if (dwSize < 2)
      dwSize = 2;

   // Figure out what type
   DWORD dwType;
   dwType = (DWORD) (rand() % (gdwDifficulty + 1));

   // Fill it in
   WCHAR *pTemp;
   pTemp = (WCHAR*) malloc (dwSize);
   if (!pTemp)
      return NULL;
   FillBufWithText (dwType, pTemp, dwSize / 2);

   // Consider putting in tags
  	if (!gfNoTags ) {
		BOOL  fTags;
		fTags = !(rand() % 4);
		if (fTags) {
			DWORD dwPosn = (DWORD) rand() % 256;
			while ((dwPosn + 64) < (dwSize/2)) {
				ProduceTag (pTemp + dwPosn, gdwDifficulty);

				// Get rid of the null termination
				pTemp[dwPosn + wcslen(pTemp+dwPosn)] = GenerateCharacter(gdwDifficulty);

				dwPosn += ((DWORD) rand() % 256);
			}
		}
	}

   return pTemp;
}



/**********************************************************************
Dialogs to get data from the user
*/

/*******************************************************
Intro1DlgProc - Do the testing.

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK Intro1DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_CLOSE:
      EndDialog (hWnd, IDCANCEL);
      break;

   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDOK:
      case IDCANCEL:
         EndDialog (hWnd, LOWORD(wParam));
         break;
      }
      break;
   }

   return FALSE;  // didn't handle
}



BOOL CALLBACK Intro2DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
      {
      SRMODEINFOW SRInfo;

      pEnum->Reset();
      while (!pEnum->Next(1, &SRInfo, NULL)) {
         char  szTemp[128];
         WideCharToMultiByte (CP_ACP, 0, SRInfo.szModeName, -1,
            szTemp, sizeof(szTemp), 0, 0);
         SendDlgItemMessage (hWnd, IDC_MODE, LB_ADDSTRING,
            0, (LPARAM) szTemp);
      }

      // check the dialog item button
      CheckDlgButton (hWnd, IDC_HARD, BST_CHECKED);
      }
      break;

   case WM_CLOSE:
      EndDialog (hWnd, IDCANCEL);
      break;

   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDOK:
         // make sure that one or more engines is selected
         {
            // figure out how many are selected
            DWORD dwCount;
            int   aItemNum[MAXMODES];
            dwCount = SendDlgItemMessage (hWnd, IDC_MODE, LB_GETSELCOUNT, 0,0);
            if (!dwCount) {
               MessageBox (hWnd, "You must select at least one engine mode.", NULL, MB_OK);
               break;
            }
            SendDlgItemMessage (hWnd, IDC_MODE, LB_GETSELITEMS, dwCount, (LPARAM) aItemNum);

            // get the guids
            gdwNumModes = dwCount;
            DWORD i;
            SRMODEINFOW SRInfo;

            for (i = 0; i < dwCount; i++) {
               pEnum->Reset();
               pEnum->Skip(aItemNum[i]);
               pEnum->Next(1, &SRInfo, NULL);
               gagMode[i] = SRInfo.gModeID;
            }

         }

         // figure out if easy/medium/hard
         if (IsDlgButtonChecked (hWnd, IDC_MEDIUM))
            gdwDifficulty = DIFFICULTY_MEDIUM;
         else if (IsDlgButtonChecked (hWnd, IDC_HARD))
            gdwDifficulty = DIFFICULTY_HARD;
         else
            gdwDifficulty = DIFFICULTY_EASY;

			// use tags in text?
			gfNoTags = (IsDlgButtonChecked (hWnd, IDC_CHECK_NOTAGS)) ? TRUE : FALSE;

			// use random strings?
			gfNonRandomString = (IsDlgButtonChecked (hWnd, IDC_CHECK_NONRANDOM_STRING)) ? TRUE : FALSE;
			

         // warn the user if difficulty is set to high
         if (gdwDifficulty >= DIFFICULTY_HARD) {
            MessageBox (hWnd, "Because the Very Difficult setting does stress tests on "
               "pronunciation, accuracy may degrade after running the test.",
               "Warning", MB_OK);
         }

         // fall on through
      case IDCANCEL:
         EndDialog (hWnd, LOWORD(wParam));
         break;
      }
      break;
   }

   return FALSE;  // didn't handle
}



/*******************************************************
Intro3DlgProc - Do the testing.

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK Intro3DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
      SetDlgItemText (hWnd, IDC_GRAMMARDIR, "c:\\program files\\microsoft speech sdk\\sr");
      SetDlgItemText (hWnd, IDC_INPUTWAVE, "Fill this in");
      CheckDlgButton (hWnd, IDC_FIRSTTOSOUNDCARD, BST_CHECKED);
      CheckDlgButton (hWnd, IDC_CHECK_LOGFILE, BST_CHECKED);
      SetDlgItemText (hWnd, IDC_THREADS, "1");
      break;

   case WM_CLOSE:
      EndDialog (hWnd, IDCANCEL);
      break;

   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDOK:
         {
            // enumerate the directory and get the files
            char  szDir[256], szSearch[256];
            GetDlgItemText (hWnd, IDC_GRAMMARDIR, szDir, sizeof(szDir));
            wsprintf (szSearch, "%s\\*.*", szDir);
            HANDLE   hFile;
            WIN32_FIND_DATA   fd;
            gdwNumFiles = 0;
            hFile = FindFirstFile (szSearch, &fd);
            if (hFile != INVALID_HANDLE_VALUE) {
               while (TRUE) {
                  DWORD dwLen;
                  dwLen = strlen(fd.cFileName);
                  if ((dwLen >= 4) && (gdwNumFiles < MAXFILES) && (
                     (!stricmp(fd.cFileName + (dwLen - 4), ".txt")) ||
                     (!stricmp(fd.cFileName + (dwLen - 4), ".srg")) ||
                     (!stricmp(fd.cFileName + (dwLen - 4), ".grm")) )) {
                     // add the name

                     wsprintf (gszFileName[gdwNumFiles++], "%s\\%s", szDir, fd.cFileName);
                  }

                  // else go on
                  if (!FindNextFile (hFile, &fd))
                     break;
               }
               FindClose (hFile);
            }

            // if no files then error
            if (!gdwNumFiles) {
               MessageBox (hWnd, "There are no grammar files in the directory.", NULL, MB_OK);
               return TRUE;   // don't exit
            }

            // make sure the wave exists
            GetDlgItemText (hWnd, IDC_INPUTWAVE, gszWaveFile, sizeof(gszWaveFile));
            MultiByteToWideChar (CP_ACP, 0, gszWaveFile, -1, gszwWaveFile, sizeof(gszwWaveFile)/2);
            FILE  *f;
            f = fopen (gszWaveFile, "rb");
            if (f)
               fclose (f);
            if (!f) {
               MessageBox (hWnd, "The wave file does not exist.", NULL, MB_OK);
               return TRUE;
            }

            // remember if check box checked
            gfUseMultimedia = IsDlgButtonChecked (hWnd, IDC_FIRSTTOSOUNDCARD);
				gfGenerateLogFile = IsDlgButtonChecked (hWnd, IDC_CHECK_LOGFILE);

            // get the number of threads
            gdwNumThreads = GetDlgItemInt(hWnd, IDC_THREADS, NULL, FALSE);
            if (!gdwNumThreads) {
               MessageBox (hWnd, "You must have at least one thread.", NULL, MB_OK);
               return TRUE;
            }
            if (gdwNumThreads > MAXINSTANCES) {
               MessageBox (hWnd, "The test application can't handle that many threads.", NULL, MB_OK);
               return TRUE;
            }
         }
         // fall through to cancel
      case IDCANCEL:
         EndDialog (hWnd, LOWORD(wParam));
         break;
      }
      break;
   }

   return FALSE;  // didn't handle
}

/*******************************************************
TestListDlgProc - Indicate which cases to stress

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK TestListDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
		// required radio button is on by default
		CheckRadioButton (hWnd, IDC_RADIO_REQUIRED, IDC_RADIO_OPTIONAL, IDC_RADIO_OPTIONAL);
		
		// all boxes are checked by default
		CheckDlgButton (hWnd, IDC_CHECK_ENGINE, BST_CHECKED);
		CheckDlgButton (hWnd, IDC_CHECK_GRAMMAR, BST_CHECKED);
		CheckDlgButton (hWnd, IDC_CHECK_RESULT, BST_CHECKED);
		
			// populate list boxs
		{
			for( DWORD dwCount=0; dwCount < MAXENGINECASES; dwCount++) {
	   		SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_ADDSTRING,
				  0, (LPARAM) gaEngineCases[dwCount].pszCaseName);
			}
   		SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXENGINECASES -1)); 

			for( dwCount=0; dwCount < MAXGRAMMARCASES; dwCount++) {
	   		SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_ADDSTRING,
				  0, (LPARAM) gaGrammarCases[dwCount].pszCaseName);
			}
			SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXGRAMMARCASES -1)); 
			
			for( dwCount=0; dwCount < MAXRESULTCASES; dwCount++) {
	   		SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_ADDSTRING,
				  0, (LPARAM) gaResultCases[dwCount].pszCaseName);
			}
			SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXRESULTCASES -1)); 
		}

		break;
   case WM_CLOSE:
      EndDialog (hWnd, IDCANCEL);
      break;

   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      		case IDC_ENGINE_ALLNONE:
			if( SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_GETSEL,0,0) )
				SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_SELITEMRANGE, FALSE, MAKELPARAM(0, MAXENGINECASES -1));
			else
				SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXENGINECASES -1));
			return TRUE;
		case IDC_GRAMMAR_ALLNONE:
			if( SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_GETSEL,0,0) )
				SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_SELITEMRANGE, FALSE, MAKELPARAM(0, MAXGRAMMARCASES -1));
			else
				SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXGRAMMARCASES -1));
			return TRUE;
		case IDC_RESULT_ALLNONE:
			if( SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_GETSEL,0,0) ) 
				SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_SELITEMRANGE, FALSE, MAKELPARAM(0, MAXRESULTCASES -1));
			else
				SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXRESULTCASES -1));
			return TRUE;	
		case IDC_RADIO_REQUIRED:
			CheckRadioButton (hWnd, IDC_RADIO_REQUIRED, IDC_RADIO_OPTIONAL, IDC_RADIO_REQUIRED);

				// populate list boxs with only required tests
			{
				// first clear list box then fill with required cases only
				SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_RESETCONTENT, 0, 0 ); 

				for( DWORD dwCount=0; dwCount < MAXENGINECASES; dwCount++) {
	   			if(gaEngineCases[dwCount].dwCaseType == TEST_REQUIRED)
						SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_ADDSTRING,
							0, (LPARAM) gaEngineCases[dwCount].pszCaseName);
				}
   			SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXENGINECASES -1)); 


				// first clear list box then fill with required cases only
				SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_RESETCONTENT, 0, 0 ); 
				
				for( dwCount=0; dwCount < MAXGRAMMARCASES; dwCount++) {
					if (gaGrammarCases[dwCount].dwCaseType == TEST_REQUIRED)
	   				SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_ADDSTRING,
							0, (LPARAM) gaGrammarCases[dwCount].pszCaseName);
				}
				SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXGRAMMARCASES -1)); 
				
				// first clear list box then fill with required cases only
				SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_RESETCONTENT, 0, 0 ); 

				for( dwCount=0; dwCount < MAXRESULTCASES; dwCount++) {
					if (gaResultCases[dwCount].dwCaseType == TEST_REQUIRED)
	   				SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_ADDSTRING,
							0, (LPARAM) gaResultCases[dwCount].pszCaseName);
				}
				SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXRESULTCASES -1)); 
			}
			return TRUE;
		case IDC_RADIO_OPTIONAL:
			CheckRadioButton (hWnd, IDC_RADIO_REQUIRED, IDC_RADIO_OPTIONAL, IDC_RADIO_OPTIONAL);
				// populate list boxs with all items
			{
				// first clear list box then fill with everything
				SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_RESETCONTENT, 0, 0 ); 

				for( DWORD dwCount=0; dwCount < MAXENGINECASES; dwCount++) {
	   			SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_ADDSTRING,
					  0, (LPARAM) gaEngineCases[dwCount].pszCaseName);
				}
   			SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXENGINECASES -1)); 


				// first clear list box then fill with everything
				SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_RESETCONTENT, 0, 0 ); 
				
				for( dwCount=0; dwCount < MAXGRAMMARCASES; dwCount++) {
	   			SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_ADDSTRING,
					  0, (LPARAM) gaGrammarCases[dwCount].pszCaseName);
				}
				SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXGRAMMARCASES -1)); 
				
				// first clear list box then fill with everything
				SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_RESETCONTENT, 0, 0 ); 

				for( dwCount=0; dwCount < MAXRESULTCASES; dwCount++) {
	   			SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_ADDSTRING,
					  0, (LPARAM) gaResultCases[dwCount].pszCaseName);
				}
				SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXRESULTCASES -1)); 
			}
			return TRUE;
		case IDC_CHECK_ENGINE:
			// if not checked then disable list
			if( IsDlgButtonChecked (hWnd, IDC_CHECK_ENGINE)) 
				EnableWindow(GetDlgItem(hWnd, IDC_LIST_ENGINE), TRUE); 
			else
				EnableWindow(GetDlgItem(hWnd, IDC_LIST_ENGINE), FALSE);
			return TRUE;
		case IDC_CHECK_GRAMMAR:
			// if not checked then disable list
			if( IsDlgButtonChecked (hWnd, IDC_CHECK_GRAMMAR)) 
				EnableWindow(GetDlgItem(hWnd, IDC_LIST_GRAMMAR), TRUE);
			else
				EnableWindow(GetDlgItem(hWnd, IDC_LIST_GRAMMAR), FALSE);
			return TRUE;
		case IDC_CHECK_RESULT:
			// if not checked then disable list
			if( IsDlgButtonChecked (hWnd, IDC_CHECK_RESULT)) 
				EnableWindow(GetDlgItem(hWnd, IDC_LIST_RESULT), TRUE);
			else
				EnableWindow(GetDlgItem(hWnd, IDC_LIST_RESULT), FALSE);
			return TRUE;
		case IDOK:
			{
				BOOL bCaseSelected = FALSE;
				int iListIndex;
					// figure out if Engine, Grammar, or Result
				if (IsDlgButtonChecked (hWnd, IDC_CHECK_ENGINE)) {
					gfTestEngine = TRUE;
					// check which cases are selected
					for( DWORD dwCount=0; dwCount<MAXENGINECASES;dwCount++) {
						// check each case to see if it's on list and if selected
						iListIndex = SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_FINDSTRING, -1, 
							(LPARAM) gaEngineCases[dwCount].pszCaseName);
						if( iListIndex != LB_ERR ) {
							if( SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_GETSEL, iListIndex,0) ) {
								gaEngineCases[dwCount].fRun = TRUE;
								bCaseSelected = TRUE;
							}
							else
								gaEngineCases[dwCount].fRun = FALSE;
						}
						else
							gaEngineCases[dwCount].fRun = FALSE;
					}
				}
				else 
					gfTestEngine = FALSE;
         
				if (IsDlgButtonChecked (hWnd, IDC_CHECK_GRAMMAR)) {
					gfTestGrammar = TRUE;
					// check which cases are selected
					for( DWORD dwCount=0; dwCount<MAXGRAMMARCASES;dwCount++) {
						// check each case to see if it's on list and if selected
						iListIndex = SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_FINDSTRING, -1, 
							(LPARAM) gaGrammarCases[dwCount].pszCaseName);
						if( iListIndex != LB_ERR ) {
							if( SendDlgItemMessage (hWnd, IDC_LIST_GRAMMAR, LB_GETSEL, iListIndex,0) ) {
								gaGrammarCases[dwCount].fRun = TRUE;
								bCaseSelected = TRUE;
							}
							else
								gaGrammarCases[dwCount].fRun = FALSE;
						}
						else
							gaGrammarCases[dwCount].fRun = FALSE;
					}
				}
				else
					gfTestGrammar = FALSE;
				
				if (IsDlgButtonChecked (hWnd, IDC_CHECK_RESULT)) {
					gfTestResult = TRUE;
					// check which cases are selected
					for( DWORD dwCount=0; dwCount<MAXRESULTCASES;dwCount++) {
						// check each case to see if it's on list and if selected
						iListIndex = SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_FINDSTRING, -1, 
							(LPARAM) gaResultCases[dwCount].pszCaseName);
						if( iListIndex != LB_ERR ) {
							if( SendDlgItemMessage (hWnd, IDC_LIST_RESULT, LB_GETSEL, iListIndex,0) ) {
								gaResultCases[dwCount].fRun = TRUE;
								bCaseSelected = TRUE;
							}
							else
								gaResultCases[dwCount].fRun = FALSE;
						}
						else
							gaResultCases[dwCount].fRun = FALSE;
					}
				}
				else
					gfTestResult = FALSE;
			
				// make sure that something is picked to test
				if( !(gfTestEngine | gfTestGrammar | gfTestResult) || bCaseSelected == FALSE) {
					MessageBox (hWnd, "You must select something to test.", NULL, MB_OK);
					return TRUE;
				}
			}
		case IDCANCEL:
         EndDialog (hWnd, LOWORD(wParam));
         break;
      }
      break;
   }

   return FALSE;  // didn't handle
}


/*******************************************************
MainThreadDlgProc - Do the testing.

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK MainThreadDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
      {
         DWORD i;
         // Fill in the speaker names so we don't generate too many
         // speakers
         for (i = 0; i < MAXNAMES; i++) {
            FillBufWithText (rand() % 8, gaszSpeakerNames[i], sizeof(gaszSpeakerNames[i])/2);
         }

         // create two events for each thread, one so we can
         // inform them of shut-down, and the other so we can
         // know if they have shut down
         for (i = 0; i < gdwNumThreads; i++) {
            gahEventShutDownNow[i] = CreateEvent (NULL, TRUE, FALSE, NULL);
            gahEventIsShutDown[i] = CreateEvent (NULL, TRUE, FALSE, NULL);
         }

         // create all the threads
         // DWORD dwThread;
         for (i = 0; i < gdwNumThreads; i++)
            _beginthread (StressThread, 0, (LPVOID)i);
//            CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) StressThread, (LPVOID) i, 0, &dwThread);
		}
      break;

   case WM_CLOSE:
      PostMessage (hWnd, WM_COMMAND, IDCANCEL, 0);
      return TRUE;

   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDOK:
      case IDCANCEL:
         {
         HCURSOR hCursor = SetCursor (LoadCursor(NULL, IDC_WAIT));

         // set all the events
         DWORD i;
         for (i = 0; i < gdwNumThreads; i++)
            SetEvent (gahEventShutDownNow[i]);

         // wait for all the events
         for (i = 0; i < gdwNumThreads; i++)
            WaitForSingleObject (gahEventIsShutDown[i], INFINITE);

         // free all the events
         for (i = 0; i < gdwNumThreads; i++) {
            CloseHandle (gahEventShutDownNow[i]);
            CloseHandle (gahEventIsShutDown[i]);
         }

         SetCursor (hCursor);

         EndDialog (hWnd, LOWORD(wParam));
         }
         return TRUE;
      }
      break;
   }

   return FALSE;  // didn't handle
}






/**********************************************************************
WinMain dialog
*/
int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
   LPSTR lpszCmdLine, int nCmdShow)
{
   ghInstance = hInstance;

   CoInitialize(NULL);

   pEnum = new CSREnum;
   if (!pEnum)
      return -1;
   if (pEnum->Init()) {
      MessageBox (NULL, "Can't initialize the enumerator.", NULL, MB_OK);
      return -1;
   }

   // ask for info
   if (IDOK != DialogBox (ghInstance, MAKEINTRESOURCE(IDD_INTRO1), NULL, (DLGPROC) Intro1DlgProc))
      goto shutdown;
   if (IDOK != DialogBox (ghInstance, MAKEINTRESOURCE(IDD_INTRO2), NULL, (DLGPROC) Intro2DlgProc))
      goto shutdown;
	if (IDOK != DialogBox (ghInstance, MAKEINTRESOURCE(IDD_TESTLIST), NULL, (DLGPROC) TestListDlgProc))
      goto shutdown;
   if (IDOK != DialogBox (ghInstance, MAKEINTRESOURCE(IDD_INTRO3), NULL, (DLGPROC) Intro3DlgProc))
      goto shutdown;
	
   // unload the enumerator
   if (pEnum)
      delete pEnum;
   pEnum = NULL;

   // do the tests
   DialogBox (ghInstance, MAKEINTRESOURCE(IDD_MAINTHREAD), NULL, (DLGPROC) MainThreadDlgProc);

   // tell the user everything's unloaded
   CoFreeUnusedLibraries();

   MessageBox (NULL, "All the engines have been unloaded.", "Stress", MB_OK);

shutdown:
   if (pEnum)
      delete pEnum;

   CoUninitialize();

   return 0;
}









/**********************************************************************
Stress test for each thread
*/

/*****************************************************************************
CStress::CStress - Constructor that initializes variables. It also initializaes
	the seed for srand(). It also does a CoInitialize().

inputs
	DWORD	dwNum - Thread number. 0 is the first one. The thread numbers
					are unique.
*/
CStress::CStress (DWORD dwNum)
{
   CoInitialize(NULL);

	if (gfGenerateLogFile) {
		char  szTemp[256];
		wsprintf (szTemp, "c:\\srstress%d.txt", (int) dwNum);
		m_file = fopen(szTemp, "wt");
	}
	else
		m_file = NULL;

	m_dwThreadNum = dwNum;

	// I'm not too worried about these running out of memory
	m_pNS = new CStressNotifySink;
	m_pNS->m_pStress = this;
	m_pGNS = new CStressGramNotifySink;
	m_pGNS->m_pStress = this;
	m_pWANS = new CStressWaveAudioNotifySink;
	m_pWANS->m_pStress = this;

   // wipe out some pointers to SR objects
   m_pMode = NULL;
   m_pIWaveAudio = NULL;
   memset (m_apGram, 0, sizeof(m_apGram));
   memset (m_apGramComp, 0, sizeof(m_apGramComp));
   memset (m_apResult, 0, sizeof(m_apResult));

   // misc
   m_hWnd = NULL;
   m_dwPhrases = 0;
   m_qwTotalAudio = m_qwMaxAudio = 0;
   m_qwFunctionCalls = 0;

   // initialize the random function
   srand (GetTickCount());

   // generate a random timer elapse
   switch (gdwDifficulty) {
   case DIFFICULTY_EASY:  // easy
      m_dwTimerElapse = 800 + Random(400);
      break;
   case DIFFICULTY_MEDIUM:  // medium
      m_dwTimerElapse = 80 + Random(40);
      break;
   default: // difficult:
      m_dwTimerElapse = 8 + Random(4);
      break;
   }
}



/*****************************************************************************
CStress::~CStress - Destructor. This should not be called until the dialog
	created by DoThread has returned. This frees all the objects, and
	uninitializes ole.
*/
CStress::~CStress (void)
{
   if (m_pIWaveAudio)
	   m_pIWaveAudio->Release();

   // free all the objects
   FreeAll();

   // free the notification sinks
   if (m_pNS)
      delete m_pNS;
   if (m_pGNS)
      delete m_pGNS;
   if (m_pWANS)
      delete m_pWANS;
   CoUninitialize();

   if (m_file)
      fclose (m_file);
}


/*****************************************************************************
ThreadFunction - Thread callback function. The DWORD parameter is the thread
   number.
*/
void _cdecl StressThread (PVOID pData)
{
   DWORD dwParam = (DWORD) pData;

   // create the stress object
   {
      CStress  *pStress;
      pStress = new CStress(dwParam);
      pStress->DoThread();
      delete pStress;
   }

   // set an event that we're done
   SetEvent (gahEventIsShutDown[dwParam]);

   // return 0;
}


/*****************************************************************************
CStress::DoThread - This does all the call functions of the thread. It is assumed
	to be called from the other thread.
	- Creates a dialog box to show the information in
	- Runs the dialog
	- When the dialog returns (because the global quit flag has been set)
		this returns.
*/
void CStress::DoThread (void)
{
   DialogBoxParam (ghInstance, MAKEINTRESOURCE(IDD_INSTANCETHREAD),
      NULL, (DLGPROC) StatusDialog, (LPARAM) this);
}



/*****************************************************************************
CStress::FreeAll - Frees all objects know about by the object. For speech
	reoognition, this is the SRMode, several grammars, and several results
	objects.
*/
void CStress::FreeAll (void)
{
   DWORD i;

   for (i = 0; i < MAXRESULT; i++) {
      if (m_apResult[i]) 
         delete m_apResult[i];
      m_apResult[i] = 0;
   }
   for (i = 0; i < MAXGRAM; i++) {
      if (m_apGram[i])
         delete m_apGram[i];
      if (m_apGramComp[i])
         delete m_apGramComp[i];
      m_apGram[i] = 0;
      m_apGramComp[i] = 0;
   }

   // If we're using the wave-file, we need to store the old amount
   // parsed away, so that when the new wave-file is stared, we'll just
   // be incrementing the old value
   m_qwTotalAudio += m_qwMaxAudio;
   m_qwMaxAudio = 0;

#ifdef TESTREFCOUNT
   DWORD dwRet;
   PISRCENTRALW   pISRCentral;
   pISRCentral = m_pMode->m_pISRCentral;
   dwRet = pISRCentral->AddRef();
#endif

   if (m_pMode)
      delete m_pMode;

#ifdef TESTREFCOUNT
   // dwRet should return 0, because it'll be the last thing
   // in the object released
   dwRet = pISRCentral->Release();
#endif
   m_pMode = NULL;
   // we don't need to delete the wave audio since that's deleted with the mode
}


/*****************************************************************************
CStress::DoDialog - The dialog box function, once it determines which stress
	object is to be used, calls this. This is the dialog box function specific
	to the stress object.
*/
BOOL CStress::DoDialog (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
      // store some info away
      m_hWnd = hWnd;

      // set the title
      char  szTemp[128];
      wsprintf (szTemp, "Stress test thread %d", (int) m_dwThreadNum + 1);
      SetWindowText (hWnd, szTemp);

      // output that we've intialize
      Output ("Initialized", TRUE);

      // set the timer
      SetTimer (hWnd, TIMERID, m_dwTimerElapse, 0);

		// display the start time
		SYSTEMTIME  st;
		GetLocalTime (&st);
		wsprintf (szTemp, "%d:%02d:%02d",
			(int) st.wHour, (int) st.wMinute, (int) st.wSecond);
		SetDlgItemText (m_hWnd, IDC_STARTTIME, szTemp);

		// show the current thread id
		DWORD dwID;
		dwID = GetCurrentThreadId();
		wsprintf (szTemp, "%d", dwID);
		SetDlgItemText (m_hWnd, IDC_THREADID, szTemp);


      break;

   case WM_TIMER:
      // check to see if the quit-now even is on
      if (WaitForSingleObject (gahEventShutDownNow[m_dwThreadNum], 0) == WAIT_OBJECT_0) {
         KillTimer (m_hWnd, TIMERID);
         EndDialog (hWnd, IDOK);
         return TRUE;
      }

      // do the timer function
      OnTimer();

      break;

   case WM_CLOSE:
      // trap this
      return TRUE;

   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDCANCEL:
         // trap this
         return TRUE;
      }
      break;
   }

   // else didn't know what to do
   return FALSE;
}

/*****************************************************************************
StatusDialog - Callback procedure for the status dialog box.
*/
BOOL StatusDialog (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   CStress  *pStress;

   switch (uMsg) {
   case WM_INITDIALOG:
      // store this away
      SetWindowLong (hWnd, DWL_USER, lParam);
      pStress = (CStress*) lParam;
      return pStress->DoDialog (hWnd, uMsg, wParam, lParam);
      break;
   default:
      pStress = (CStress*) GetWindowLong (hWnd, DWL_USER);
      if (pStress)
         return pStress->DoDialog (hWnd, uMsg, wParam, lParam);
      break;
   }

   return FALSE;
}

/*****************************************************************************
CStress::Output - Outputs a string to the dialog box's main edit window so
	the user can see status information.
*/
void CStress::Output (char *psz, BOOL fClear)
{
   if (m_file) {
      fputs (psz, m_file);
      fflush (m_file);
   }

   HWND hWnd = GetDlgItem (m_hWnd, IDC_OUTPUT);

   // ignore the fclear for now

   // append the text
   int   iSize;
   iSize = GetWindowTextLength(hWnd);

   // if the window text is > 10,000 characters then clear
   if (iSize > 10000) {
      SetWindowText (hWnd, psz);
      return;
   }

   SendMessage (hWnd, EM_SETSEL, iSize, iSize);
   SendMessage (hWnd, EM_REPLACESEL, FALSE, (LPARAM) psz);
}

void CStress::Output (HRESULT hRes, BOOL fClear)
{
   if (!hRes) {
      Output (" - No error\r\n", fClear);
   }
   else if (hRes == E_NOTIMPL) {
      Output (" - Not implemented\r\n", fClear);
   }
   else {
      char  szTemp[64];
      wsprintf (szTemp, " - Failed=%x\r\n", (unsigned int) hRes);
      Output (szTemp, fClear);
   }
}


/*****************************************************************************
CStress::Random - Generates a random number from 0 to N
*/
DWORD CStress::Random (DWORD dwRange)
{
   return (DWORD) rand() % dwRange;
}

BOOL CStress::OnlyIfDifficult (void)
{
   return (Random(gdwDifficulty+1) >= 1);
}

BOOL CStress::OnlyIfVeryDifficult (void)
{
   return (Random(gdwDifficulty+1) >= 2);
}

/*****************************************************************************
CStress::RandomSpeaker - Generates a random speaker name. The returned
   memory should NOT be freed.
*/
WCHAR *CStress::RandomSpeaker (void)
{
   PCWSTR   paszNames[11] = {L"Mike", L"Larry", L"Jim", L"Bob", L"Doug",
      L"Fil", L"Yunus", L"John", L"Bill", L"Greg", L"aaaa"};

	// if no random strings the return a name of 'aaaa'
	if (gfNonRandomString)
		return (WCHAR*) paszNames[10];

   switch(Random(gdwDifficulty+1)) {
   case DIFFICULTY_EASY:
      return (WCHAR*) paszNames[Random(10)];
   case DIFFICULTY_MEDIUM:
      return gaszSpeakerNames[Random(MAXNAMES / 4)];
   default:
      return gaszSpeakerNames[Random(MAXNAMES)];
   }
}
/*****************************************************************************
CStress::RandomSRWORD - Generates a random SRWORD data and puts it
   in pBuf. Returns the number of bytes used.
*/
DWORD CStress::RandomSRWORD (BYTE *pBuf)
{
   PSRWORDW pW;
   pW = (PSRWORDW) pBuf;
   FillBufWithText (Random(gdwDifficulty+1), (WCHAR*) (pW + 1), Random(100));
   pW->dwWordNum = (DWORD) rand();
   pW->dwSize = sizeof(SRWORDW) + (wcslen(pW->szWord) + 1) * 2;
   pW->dwSize = (pW->dwSize + 3) & ~0x03; // dword aligh
   return pW->dwSize;
}


/*****************************************************************************
CStress::OnTimer - This is called in the event of a timer message coming in,
	and handles the stress calls necessary.
*/
void CStress::OnTimer (void)
{
   // on a timer, clear the output window
   Output ("", TRUE);

   // makre sure we have an engine
   if (MakeSureHaveEngine()) {
      Output ("Error: Create engine failed.\r\n");
      return;  // erorr
   }

   // make a call to the engine
   while (!Random(2)) {
      StressEngine();
   }

   // consider destroying it
   if (!Random(10000)) {
      Output ("Error: Free engine.\r\n");
      FreeAll();
      return;
   }

   // pick a random grammar and make sure we have that
   DWORD i;
   while (!Random(2)) {
      i = Random(MAXGRAM);
      switch (gdwDifficulty) {
      case DIFFICULTY_EASY:
         // no more than 5 grammars
         i = i % 5;
         break;
      case DIFFICULTY_MEDIUM:
         // no more than 10 grammars
         i = i % 10;
         break;
      default:
         // do nothing
         break;
      }

      if (!MakeSureHaveGrammar(i)) {
         StressGrammar (i);

         // consider destroying it
         if (!Random(1000)) {
            if (m_apGram[i])
               delete m_apGram[i];
            if (m_apGramComp[i])
               delete m_apGramComp[i];
            m_apGram[i] = 0;
            m_apGramComp[i] = 0;
         }
      }
   }

   // find a random results object
   while (!Random(3)) {
      i = Random(MAXRESULT);
      if (!MakeSureHaveResult(i)) {
         StressResult (i);

         // consdier destroying it
         if (!Random(10)) {
            if (m_apResult[i])
               delete m_apResult[i];
            m_apResult[i] = 0;
         }
      }
   }

}


/*****************************************************************************
CStress::MakeSureHaveEngine - This makes sure that we have a SR engine. If there
	isn't one, a new one is randomly generated.
*/
HRESULT CStress::MakeSureHaveEngine (void)
{
   HRESULT  hRes;
   // if already have one all done
   if (m_pMode)
      return NOERROR;

   // allocate object
   m_pMode = new CSRMode;
   if (!m_pMode)
      return E_OUTOFMEMORY;

   // If it's not the first thread, or if it is the first thread
   // and the global flag is NOT set for using the wave device, then use
   // wave file
   if (m_dwThreadNum || !gfUseMultimedia) {
   // Create the wave file object and initialize it
	   hRes = CoCreateInstance(CLSID_AudioSourceFile, 
		   NULL, CLSCTX_INPROC_SERVER, 
		   IID_IAudioFile, 
		   (PVOID *) &m_pIWaveAudio);
	   if (hRes) goto alldone;
	   m_pIWaveAudio -> Register(m_pWANS);

      // add the file
      m_pIWaveAudio->Add (gszwWaveFile, 1);

      m_pMode->InitAudioSourceObject(m_pIWaveAudio);
   }

   // initialize
alldone:
   hRes = m_pMode->Init (gagMode[Random(gdwNumModes)]);
   if (hRes) {
      delete m_pMode;
      m_pMode = NULL;
      return hRes;
   }

   // Need to register
   DWORD dwKey;
   hRes = m_pMode->Register (m_pNS, &dwKey);
   if (hRes) {
      delete m_pMode;
      m_pMode = NULL;
      return hRes;
   }

   // we have a mode
   return NOERROR;
}


/*****************************************************************************
CStress::MakeSureHaveGrammar - This makes sure that we have a grammar object
	to test on. If there isn't one then a new one is randomly created.
*/
HRESULT CStress::MakeSureHaveGrammar (DWORD dwNum)
{
   // see if we already have one
   if (m_apGram[dwNum] || m_apGramComp[dwNum])
      return NOERROR;   // have one

   // if not, load in the file
   DWORD dwFile;
   HRESULT  hRes;
   PBYTE    pGrmMem;
   DWORD    dwGrmMem;
   pGrmMem = 0;
   dwFile = Random(gdwNumFiles);

   // if the file is a grammar file then load the binary
   DWORD dwLen;
   dwLen = strlen(gszFileName[dwFile]);
   if ((dwLen > 4) && (!stricmp(gszFileName[dwFile] + (dwLen-4), ".grm"))) {
      FILE  *f;
      f = fopen (gszFileName[dwFile], "rb");
      fseek (f, 0, SEEK_END);
      dwGrmMem = (DWORD) ftell(f);
      fseek (f, 0, 0);
      pGrmMem = (PBYTE) malloc (dwGrmMem);
      if (pGrmMem) {
         fread (pGrmMem, 1, dwGrmMem, f);
      }
      fclose (f);
   }

   // Once in awhile try making up random garbage and loading
   // that in as a grammar
   if (!pGrmMem && OnlyIfVeryDifficult()) {
      pGrmMem = (PBYTE) GenerateRandomText();
      dwGrmMem = wcslen((WCHAR*) pGrmMem) * 2;
   }

   // If it ends in .grm then we need to take a different rout
   if (pGrmMem) {
      SRGRMFMT fmt;
      switch (Random(6)) {
      case 0:
         fmt = SRGRMFMT_CFGNATIVE;
         break;
      case 1:
         fmt = SRGRMFMT_LIMITEDDOMAINNATIVE;
         break;
      case 2:
         fmt = SRGRMFMT_DICTATIONNATIVE;
         break;
      case 3:
         fmt = SRGRMFMT_CFG;
         break;
      case 4:
         fmt = SRGRMFMT_LIMITEDDOMAIN;
         break;
      case 5:
         fmt = SRGRMFMT_DICTATION;
         break;
      }

      m_apGram[dwNum] = m_pMode->GrammarLoad (fmt, pGrmMem, dwGrmMem,
         m_pGNS);
      free (pGrmMem);
      if (!m_apGram[dwNum]) {
         char  szTemp[512];
         wsprintf (szTemp, "Error: Can't load %s.\r\n", gszFileName[dwFile]);
         Output (szTemp);
         return E_FAIL;
      }
   }
   else {   // it's a grammar file

      // assume it's a .txt or .srg, so we can load it with the grammar compiler
      // object
      WCHAR szw[256];
      MultiByteToWideChar (CP_ACP, 0, gszFileName[dwFile], -1, szw, sizeof(szw)/2);
      m_apGramComp[dwNum] = m_pMode->GrammarFromFile (
         OnlyIfVeryDifficult() ? NULL : m_pGNS, szw);
      MadeCall();
      if (!m_apGramComp[dwNum]) {
         char  szTemp[512];
         wsprintf (szTemp, "Error: Can't load %s.\r\n", gszFileName[dwFile]);
         Output (szTemp);
         return E_FAIL;
      }

      // The wrapper automatically compiles and loads

      // acitvate it while we're at it, so that we've got a better than normal
      // chance of something actually listening to the audio
      hRes = m_apGramComp[dwNum]->Activate();
      MadeCall();
      if (hRes) {
         char  szTemp[512];
         wsprintf (szTemp, "Error: Can't activate %s.\r\n", gszFileName[dwFile]);
         Output (szTemp);
         return hRes;
      }

   }
   return NOERROR;
}



/*****************************************************************************
CStress::MakeSureHaveResult - This makes sure we have a results object. If
	not it returns an error
*/
HRESULT CStress::MakeSureHaveResult (DWORD dwNum)
{
   // we can't create results on the fly, so this will have to do
   return m_apResult[dwNum] ? NOERROR : E_FAIL;
}


/*****************************************************************************
CStress::MadeCall - Call this when a function call is made.
*/
void CStress::MadeCall (void)
{
   // finally, increate the tick count of functions
   char  szTemp[64];
   m_qwFunctionCalls++;
   sprintf (szTemp, "%I64i", m_qwFunctionCalls);
   SetDlgItemText (m_hWnd, IDC_FUNCTIONCALLS, szTemp);

   // show the time of this last call
   SYSTEMTIME  st;
   GetLocalTime (&st);
   wsprintf (szTemp, "%d:%02d:%02d",
      (int) st.wHour, (int) st.wMinute, (int) st.wSecond);
   SetDlgItemText (m_hWnd, IDC_LASTCALL, szTemp);
}


/*****************************************************************************
CStressNotifySink - SR Notify sink
*/

STDMETHODIMP CStressNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRNotifySinkW)) {
      *ppObj = (LPVOID) this;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CStressNotifySink::AddRef (void)
{
   return 1;
}

STDMETHODIMP_ (ULONG) CStressNotifySink::Release (void)
{
   return 1;
}

STDMETHODIMP CStressNotifySink::AttribChanged (DWORD dwAttrib)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRNotifySink::Attribute %d changed\r\n", dwAttrib);
   m_pStress->Output (szTemp);
   return NOERROR;
}

STDMETHODIMP CStressNotifySink::Interference (QWORD qwStart, QWORD qwEnd, DWORD dwValue)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRNotifySink::Interference %d\r\n", dwValue);
   m_pStress->Output (szTemp);
   return NOERROR;
}

STDMETHODIMP CStressNotifySink::Sound (QWORD qwStart, QWORD qwEnd)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRNotifySink::Sound\r\n");
   m_pStress->Output (szTemp);
   return NOERROR;
}

STDMETHODIMP CStressNotifySink::UtteranceBegin (QWORD qwStart)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRNotifySink::UtteranceBegin\r\n");
   m_pStress->Output (szTemp);
   return NOERROR;
}

STDMETHODIMP CStressNotifySink::UtteranceEnd (QWORD qwStart, QWORD qwEnd)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRNotifySink::UtteranceEnd\r\n");
   m_pStress->Output (szTemp);
   return NOERROR;
}


STDMETHODIMP CStressNotifySink::VUMeter (QWORD qwTime, WORD wValue)
{
   m_pStress->StressOnCallback();

   // ignore this since there are too many
   return NOERROR;
}




/*****************************************************************************
CStressGramNotifySink - SR grammar notify sink
*/
STDMETHODIMP CStressGramNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRGramNotifySinkW)) {
      *ppObj = (LPVOID) this;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CStressGramNotifySink::AddRef (void)
{
   return 1;
}

STDMETHODIMP_ (ULONG) CStressGramNotifySink::Release (void)
{
   return 1;
}

STDMETHODIMP CStressGramNotifySink::BookMark (DWORD dwMark)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRGramNotifySink::BookMark %d\r\n", dwMark);
   m_pStress->Output (szTemp);
   return NOERROR;
}

STDMETHODIMP CStressGramNotifySink::Paused (void)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRGramNotifySink::Paused\r\n");
   m_pStress->Output (szTemp);

   // resume this right away so we're not always ending up paused
   HRESULT  hRes;
   hRes = m_pStress->m_pMode->Resume();
   if (hRes)
      m_pStress->Output ("Error: Pause after resume failed.\r\n");

   return NOERROR;
}

STDMETHODIMP CStressGramNotifySink::PhraseFinish (DWORD dwFlags, QWORD qwStart,
                                                   QWORD qwEnd, PSRPHRASEW pSRPhrase,
                                                   LPUNKNOWN pUnk)
{
   m_pStress->StressOnCallback();

   char  szTemp[10000];
   wsprintf (szTemp, "ISRGramNotifySink::PhraseFinish\r\n");
   m_pStress->Output (szTemp);

   // increase the recognized phrase count
   m_pStress->m_dwPhrases++;
   wsprintf (szTemp, "%d", (int) m_pStress->m_dwPhrases);
   SetDlgItemText (m_pStress->m_hWnd, IDC_PHRASES, szTemp);

   // if there's a results object then keep it
   if (pUnk) {
      DWORD dwNum;
      dwNum = m_pStress->Random(MAXRESULT);
      if (m_pStress->m_apResult[dwNum])
         delete m_pStress->m_apResult[dwNum];
      m_pStress->m_apResult[dwNum] = new CSRResult;
      if (m_pStress->m_apResult[dwNum]) {
         HRESULT  hRes;
         hRes = m_pStress->m_apResult[dwNum]->Init(pUnk);
         if (hRes) {
            m_pStress->Output ("Error: Can't use result object.\r\n");
            delete m_pStress->m_apResult[dwNum];
            m_pStress->m_apResult[dwNum] = NULL;
         }
      }
   }

   // print out the results
	PSRWORDW  pSRWord, pSRMax;
   m_pStress->Output ("PhraseFinish: ");
	if (!pSRPhrase) {
		m_pStress->Output ("[Unrecognized]\r\n");
		return NOERROR;
	}

	// write out a string with the results
	// loop through all of the words and display them
	pSRMax = (PSRWORDW) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
	pSRWord = (PSRWORDW) (pSRPhrase->abWords);

	szTemp[0] = 0;

	while (pSRWord < pSRMax)
	{
		// add word
		char	szaTemp[256];
		WideCharToMultiByte (CP_ACP,0, pSRWord->szWord, -1, szaTemp, sizeof(szaTemp),
			NULL, NULL);

		strcat (szTemp, szaTemp);
		strcat (szTemp, " ");

		pSRWord = (PSRWORDW) ((BYTE*) pSRWord + pSRWord->dwSize);
	};
	m_pStress->Output (szTemp);
   m_pStress->Output ("\r\n");

	return NOERROR;

}


STDMETHODIMP CStressGramNotifySink::PhraseHypothesis (DWORD dwFlags, QWORD qwStart,
                                                   QWORD qwEnd, PSRPHRASEW pSRPhrase,
                                                   LPUNKNOWN pUnk)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRGramNotifySink::PhraseHypothesis\r\n");
   m_pStress->Output (szTemp);

   // if there's a results object then keep it
   if (pUnk) {
      DWORD dwNum;
      dwNum = m_pStress->Random(MAXRESULT);
      if (m_pStress->m_apResult[dwNum])
         delete m_pStress->m_apResult[dwNum];
      m_pStress->m_apResult[dwNum] = new CSRResult;
      if (m_pStress->m_apResult[dwNum]) {
         HRESULT  hRes;
         hRes = m_pStress->m_apResult[dwNum]->Init(pUnk);
         if (hRes) {
            m_pStress->Output ("Error: Can't use result object.\r\n");
            delete m_pStress->m_apResult[dwNum];
            m_pStress->m_apResult[dwNum] = NULL;
         }
      }
   }

   // print out the results
	PSRWORDW  pSRWord, pSRMax;
   m_pStress->Output ("PhraseHypothesis: ");
	if (!pSRPhrase) {
		m_pStress->Output ("[Unrecognized]\r\n");
		return NOERROR;
	}

	// write out a string with the results
	// loop through all of the words and display them
	pSRMax = (PSRWORDW) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
	pSRWord = (PSRWORDW) (pSRPhrase->abWords);

	szTemp[0] = 0;

	while (pSRWord < pSRMax)
	{
		// add word
		char	szaTemp[256];
		WideCharToMultiByte (CP_ACP,0, pSRWord->szWord, -1, szaTemp, sizeof(szaTemp),
			NULL, NULL);

		strcat (szTemp, szaTemp);
		strcat (szTemp, " ");

		pSRWord = (PSRWORDW) ((BYTE*) pSRWord + pSRWord->dwSize);
	};
	m_pStress->Output (szTemp);
   m_pStress->Output ("\r\n");


   return NOERROR;
}


STDMETHODIMP CStressGramNotifySink::PhraseStart (QWORD qwTime)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRGramNotifySink::PhraseStart\r\n");
   m_pStress->Output (szTemp);

   return NOERROR;
}

STDMETHODIMP CStressGramNotifySink::ReEvaluate (LPUNKNOWN pUnk)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRGramNotifySink::ReEvalutate\r\n");
   m_pStress->Output (szTemp);

   // if there's a results object then keep it
   if (pUnk) {
      DWORD dwNum;
      dwNum = m_pStress->Random(MAXRESULT);
      if (m_pStress->m_apResult[dwNum])
         delete m_pStress->m_apResult[dwNum];
      m_pStress->m_apResult[dwNum] = new CSRResult;
      if (m_pStress->m_apResult[dwNum]) {
         HRESULT  hRes;
         hRes = m_pStress->m_apResult[dwNum]->Init(pUnk);
         if (hRes) {
            m_pStress->Output ("Error: Can't use result object.\r\n");
            delete m_pStress->m_apResult[dwNum];
            m_pStress->m_apResult[dwNum] = NULL;
         }
      }
   }

   return NOERROR;
}

STDMETHODIMP CStressGramNotifySink::Training (DWORD dwVal)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRGramNotifySink::Training\r\n");
   m_pStress->Output (szTemp);

   return NOERROR;
}

STDMETHODIMP CStressGramNotifySink::UnArchive (LPUNKNOWN pUnk)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ISRGramNotifySink::ReEvalutate\r\n");
   m_pStress->Output (szTemp);

   // if there's a results object then keep it
   if (pUnk) {
      DWORD dwNum;
      dwNum = m_pStress->Random(MAXRESULT);
      if (m_pStress->m_apResult[dwNum])
         delete m_pStress->m_apResult[dwNum];
      m_pStress->m_apResult[dwNum] = new CSRResult;
      if (m_pStress->m_apResult[dwNum]) {
         HRESULT  hRes;
         hRes = m_pStress->m_apResult[dwNum]->Init(pUnk);
         if (hRes) {
            m_pStress->Output ("Error: Can't use result object.\r\n");
            delete m_pStress->m_apResult[dwNum];
            m_pStress->m_apResult[dwNum] = NULL;
         }
      }
   }

   return NOERROR;
}





/*****************************************************************************
CStressWaveAudioNotifySink - Wave audio notify sink that inficates how
	much audio has been processed.
*/

STDMETHODIMP CStressWaveAudioNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IAudioFileNotifySink)) {
      *ppObj = (LPVOID) this;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CStressWaveAudioNotifySink::AddRef (void)
{
   return 1;
}

STDMETHODIMP_ (ULONG) CStressWaveAudioNotifySink::Release (void)
{
   return 1;
}

STDMETHODIMP CStressWaveAudioNotifySink::FileBegin      (DWORD dwVal)
{
   // Do nothing
   return NOERROR;
}

STDMETHODIMP CStressWaveAudioNotifySink::FileEnd        (DWORD dwVal)
{
   // add the file again
   m_pStress->m_pIWaveAudio->Add (gszwWaveFile, 1);

   return NOERROR;
}

STDMETHODIMP CStressWaveAudioNotifySink::QueueEmpty     (void)
{
   // do nothing
   return NOERROR;
}

STDMETHODIMP CStressWaveAudioNotifySink::Posn (QWORD qwProcessed, QWORD qwLeft)
{
   // store this away
   m_pStress->m_qwMaxAudio = qwProcessed;

   // add the max and total together, and display
   char  szTemp[64];
   sprintf (szTemp, "%I64iK", (m_pStress->m_qwTotalAudio + m_pStress->m_qwMaxAudio) / 1000);
   SetDlgItemText (m_pStress->m_hWnd, IDC_DIGITALAUDIO, szTemp);
   
   return NOERROR;
}

/*****************************************************************************
CStress::StressOnCallback - Randomly stresses on a callback.
*/
void CStress::StressOnCallback (void)
{
   while (OnlyIfVeryDifficult()) {
      switch (Random(3)) {
      case 0:
         StressEngine();
         break;
      case 1:
         StressGrammar(Random(MAXGRAM));
         break;
      case 2:
         StressResult(Random(MAXRESULT));
         break;
      }
   }
}



/*****************************************************************************
CStress::StressEngine - This makes a call to the main engine and does a random
	function to stress it.
*/
void CStress::StressEngine (void)
{
   HRESULT  hRes;
   DWORD dwVal;
	DWORD		dwCaseToRun;
	
	dwCaseToRun = Random(MAXENGINECASES);

	if( !gfTestEngine )
		return;
	else {
		if ( !gaEngineCases[dwCaseToRun].fRun )
			return;
	}   

   if (!m_pMode)
      return;

	switch (dwCaseToRun) {
   case 0: // autogain enable get
      {
			hRes = m_pMode->AutoGainEnableGet(
				OnlyIfVeryDifficult() ? NULL : &dwVal);
         Output ("SRMode:AutoGainEnableGet");
         Output (hRes);
         break;
      }
   case 1: // autgainenableset
      {
         if (OnlyIfDifficult())
            dwVal = Random(65536);
         else
            dwVal = Random(100);
         hRes = m_pMode->AutoGainEnableSet (dwVal);
         Output ("SRMode:AutoGainEnableSet");
         Output (hRes);
         break;
      }
   case 2: // echoget
      {
         hRes = m_pMode->EchoGet(
            OnlyIfVeryDifficult() ? NULL : (BOOL*) &dwVal);
         Output ("SRMode:EchoGet");
         Output (hRes);
         break;
      }
   case 3: // echoset
      {
         if (OnlyIfDifficult())
            dwVal = Random(65536);
         else
            dwVal = Random(2);
         hRes = m_pMode->EchoSet ((BOOL) dwVal);
         Output ("SRMode:EchoSet");
         Output (hRes);
         break;
      }
   case 4: // energy floorget
      {
         WORD  wVal;
         hRes = m_pMode->EnergyFloorGet(
            OnlyIfVeryDifficult() ? NULL : &wVal);
         Output ("SRMode:EnergyFloorGet");
         Output (hRes);
         break;
      }
   case 5: // energy floor set
      {
         if (OnlyIfDifficult())
            dwVal = Random(65536);
         else
            dwVal = Random(100);
         hRes = m_pMode->EnergyFloorSet ((WORD) dwVal);
         Output ("SRMode:EnergyFloorSet");
         Output (hRes);
         break;
      }
   case 6: // microphoneget
      {
         WCHAR szMicrophone[128];

         hRes = m_pMode->MicrophoneGet(
            OnlyIfVeryDifficult() ? NULL : szMicrophone,
            OnlyIfDifficult() ? 0 : Random(sizeof(szMicrophone)),
            OnlyIfVeryDifficult() ? NULL : &dwVal);
         Output ("SRMode:MicrophoneGet");
         Output (hRes);
         break;
      }
   case 7: // microphone set
      {
         WCHAR *pTemp = GenerateRandomText (Random(50));
         hRes = m_pMode->MicrophoneSet (OnlyIfVeryDifficult() ? NULL : pTemp);
         if (pTemp)
            free (pTemp);
         Output ("SRMode:MicrophoneSet");
         Output (hRes);
         break;
      }
   case 8: // realtimeget
      {
         hRes = m_pMode->RealTimeGet(
            OnlyIfVeryDifficult() ? NULL : &dwVal);
         Output ("SRMode:RealTimeGet");
         Output (hRes);
         break;
      }
   case 9: // realtimeset
      {
         if (OnlyIfDifficult())
            dwVal = Random(65536);
         else
            dwVal = Random(100);
         hRes = m_pMode->RealTimeSet (dwVal);
         Output ("SRMode:RealTimeSet");
         Output (hRes);
         break;
      }
   case 10: // speakerget
      {
         WCHAR szMicrophone[128];

         hRes = m_pMode->SpeakerGet(
            OnlyIfVeryDifficult() ? NULL : szMicrophone,
            OnlyIfDifficult() ? 0 : Random(sizeof(szMicrophone)),
            OnlyIfVeryDifficult() ? NULL : &dwVal);
         Output ("SRMode:SpeakerGet");
         Output (hRes);
         break;
      }
   case 11: // speakerset
      {
         hRes = m_pMode->SpeakerSet (
            OnlyIfDifficult() ? NULL : RandomSpeaker());
         Output ("SRMode:SpeakerSet");
         Output (hRes);
         break;
      }
   case 12: // thresholdget
      {
         hRes = m_pMode->ThresholdGet(
            OnlyIfVeryDifficult() ? NULL : &dwVal);
         Output ("SRMode:ThresholdGet");
         Output (hRes);
         break;
      }
   case 13: // threadholdset
      {
         if (OnlyIfDifficult())
            dwVal = Random(65536);
         else
            dwVal = Random(100);
         hRes = m_pMode->ThresholdSet (dwVal);
         Output ("SRMode:ThresholdSet");
         Output (hRes);
         break;
      }
   case 14: //timeoutget
      {
         hRes = m_pMode->TimeOutGet(
            OnlyIfVeryDifficult() ? NULL : &dwVal,
            OnlyIfVeryDifficult() ? NULL : &dwVal);
         Output ("SRMode:TimeOutGet");
         Output (hRes);
         break;
      }
   case 15: // timeoutset
      {
         hRes = m_pMode->TimeOutSet (
            OnlyIfVeryDifficult() ? Random(65536) : Random(1000),
            OnlyIfVeryDifficult() ? Random(65536) : Random(1000));
         Output ("SRMode:TimeOutSet");
         Output (hRes);
         break;
      }
   case 16: // modeget
      {
         SRMODEINFOW  mi;
         hRes = m_pMode->ModeGet (
            OnlyIfVeryDifficult() ? NULL : &mi);
         Output ("SRMode:ModeGet");
         Output (hRes);
         break;
      }
   case 17: // pause
      {
         hRes = m_pMode->Pause();
         hRes = m_pMode->Resume();
         Output ("SRMode:Pause & Resume");
         Output (hRes);
         break;
      }
   case 18: // resume
      {
         hRes = m_pMode->Resume();
         Output ("SRMode:Resume");
         Output (hRes);
         break;
      }
   case 19: // posnget
      {
         QWORD qwVal;
         hRes = m_pMode->PosnGet (
            OnlyIfVeryDifficult() ? NULL : &qwVal);
         Output ("SRMode:PosnGet");
         Output (hRes);
         break;
      }
   case 20: // register & unregister
      {
         DWORD dwKey;
         hRes = m_pMode->Register (
            OnlyIfVeryDifficult() ? NULL : (PVOID) m_pNS,
            OnlyIfDifficult() ? IID_IAudioFile : IID_ISRNotifySink,
            OnlyIfVeryDifficult() ? NULL : &dwKey);
         Output ("SRMode:Register");
         Output (hRes);
         if (hRes)
            break;
         hRes = m_pMode->UnRegister (dwKey);
         Output ("SRMode:UnRegister");
         Output (hRes);
         break;
      }
   case 21: // tofiletime
      {
         FILETIME ft;
         DWORD dw[2];
         dw[0] = (DWORD) rand();
         dw[1] = (DWORD) rand();
         hRes = m_pMode->ToFileTime ((PQWORD) dw,
            OnlyIfVeryDifficult () ? NULL : &ft);
         Output ("SRMode:ToFileTime");
         Output (hRes);
         break;
      }
   case 22: // about dialog
      {
         WCHAR *psz = GenerateRandomText();
         hRes = m_pMode->AboutDlg (NULL, OnlyIfDifficult ? psz : NULL);
         if (psz)
            free (psz);
         Output ("SRMode:AboutDlg");
         Output (hRes);
         break;
      }
   case 23: // general dialog
      {
         WCHAR *psz = GenerateRandomText();
         hRes = m_pMode->GeneralDlg (NULL, OnlyIfDifficult ? psz : NULL);
         if (psz)
            free (psz);
         Output ("SRMode:GeneralDlg");
         Output (hRes);
         break;
      }
   case 24: // lexicon dialog
      {
         WCHAR *psz = GenerateRandomText();
         hRes = m_pMode->LexiconDlg (NULL, OnlyIfDifficult ? psz : NULL);
         if (psz)
            free (psz);
         Output ("SRMode:LexiconDlg");
         Output (hRes);
         break;
      }
   case 25: // traingeneral dialog
      {
         WCHAR *psz = GenerateRandomText();
         hRes = m_pMode->TrainGeneralDlg (NULL, OnlyIfDifficult ? psz : NULL);
         if (psz)
            free (psz);
         Output ("SRMode:TrainGeneralDlg");
         Output (hRes);
         break;
      }
   case 26: // trainmic dialog
      {
         WCHAR *psz = GenerateRandomText();
         hRes = m_pMode->TrainMicDlg (NULL, OnlyIfDifficult ? psz : NULL);
         if (psz)
            free (psz);
         Output ("SRMode:TrainMicDlg");
         Output (hRes);
         break;
      }
   case 27: // speaker - delete
      {
         hRes = m_pMode->Delete (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker());
         Output ("SRMode:Delete");
         Output (hRes);
         break;
      }
   case 28: // speaker - enum
      {
         PWSTR psz;
         DWORD dwSize;
         psz = NULL;
         dwSize = NULL;
         hRes = m_pMode->Enum (
            OnlyIfVeryDifficult() ? NULL : &psz,
            OnlyIfVeryDifficult() ? NULL : &dwSize);
         if (psz)
            CoTaskMemFree (psz);
         Output ("SRMode:Enum");
         Output (hRes);
         break;
      }
   case 29: // speaker - merge
      {
         PVOID pBuf;
         WCHAR *pMem;
         DWORD dwSize;
         pBuf = NULL;
         dwSize = 0;

         hRes = m_pMode->Read (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            OnlyIfVeryDifficult() ? NULL : &pBuf,
            OnlyIfVeryDifficult() ? NULL : &dwSize);
         Output ("SRMode:Read");
         Output (hRes);

         // consider doing random text instead of valid data
         pMem = NULL;
         if (!pBuf || OnlyIfVeryDifficult())
            pMem = GenerateRandomText();

         hRes = m_pMode->Merge (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            pMem ? (PVOID) pMem : pBuf,
            pMem ? (wcslen(pMem) * 2) : dwSize);
         Output ("SRMode:Merge");
         Output (hRes);

         if (pBuf)
            CoTaskMemFree (pBuf);
         if (pMem)
            free (pMem);
         break;
      }
   case 30: // speaker - new
      {
         hRes = m_pMode->New (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker());
         Output ("SRMode:New");
         Output (hRes);
         break;
      }
   case 31: // speaker - query
      {
         WCHAR szMicrophone[128];

         hRes = m_pMode->Query(
            OnlyIfVeryDifficult() ? NULL : szMicrophone,
            OnlyIfDifficult() ? 0 : Random(sizeof(szMicrophone)),
            OnlyIfVeryDifficult() ? NULL : &dwVal);
         Output ("SRMode:Query");
         Output (hRes);
         break;
      }
   case 32: // speaker - read
      {
         PVOID pBuf;
         DWORD dwSize;
         pBuf = NULL;
         dwSize = 0;

         hRes = m_pMode->Read (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            OnlyIfVeryDifficult() ? NULL : &pBuf,
            OnlyIfVeryDifficult() ? NULL : &dwSize);
         Output ("SRMode:Read");
         Output (hRes);
         if (pBuf)
            CoTaskMemFree (pBuf);
         break;
      }
   case 33: // speaker - revert
      {
         hRes = m_pMode->Revert (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker());
         Output ("SRMode:Revert");
         Output (hRes);
         break;
      }
   case 34: // speaker - select
      {
         hRes = m_pMode->Select (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            Random(2) ? TRUE : FALSE);
         Output ("SRMode:Select");
         Output (hRes);
         break;
      }
   case 35: // speaker - write
      {
         PVOID pBuf;
         WCHAR *pMem;
         DWORD dwSize;
         pBuf = NULL;
         dwSize = 0;

         hRes = m_pMode->Read (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            OnlyIfVeryDifficult() ? NULL : &pBuf,
            OnlyIfVeryDifficult() ? NULL : &dwSize);
         Output ("SRMode:Read");
         Output (hRes);

         // consider doing random text instead of valid data
         pMem = NULL;
         if (!pBuf || OnlyIfVeryDifficult())
            pMem = GenerateRandomText();

         hRes = m_pMode->Write (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            pMem ? (PVOID) pMem : pBuf,
            pMem ? (wcslen(pMem) * 2) : dwSize);
         Output ("SRMode:Merge");
         Output (hRes);

         if (pBuf)
            CoTaskMemFree (pBuf);
         if (pMem)
            free (pMem);
         break;
      }
   case 36: // ilexpronounce - add
      {
	      if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test

         VOICECHARSET   vcs;
         switch (Random(3)) {
         case 0:
            vcs = CHARSET_TEXT;
            break;
         case 1:
            vcs = CHARSET_IPAPHONETIC;
            break;
         case 2:
            vcs = CHARSET_ENGINEPHONETIC;
            break;
         }

         VOICEPARTOFSPEECH vp;
         vp = (VOICEPARTOFSPEECH) Random(17);

         WCHAR *szWord;
         WCHAR *szPron;
         WCHAR *szEI;
         szWord = GenerateRandomText();
         szPron = GenerateRandomText();
         szEI = GenerateRandomText();
         hRes = m_pMode->Add (vcs,
            Random(2) ? NULL : szWord,
            Random(2) ? NULL : szPron,
            vp,
            Random(2) ? NULL : (PVOID) szEI,
            Random(2) ? 0 : wcslen(szEI) * 2);
         if (szWord)
            free (szWord);
         if (szPron)
            free (szPron);
         if (szEI)
            free (szEI);

         Output ("SRMode:Add");
         Output (hRes);
         break;
      }
   case 37: // ilexpronounce - get
      {
	      if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test
			
			VOICECHARSET   vcs;
         switch (Random(3)) {
         case 0:
            vcs = CHARSET_TEXT;
            break;
         case 1:
            vcs = CHARSET_IPAPHONETIC;
            break;
         case 2:
            vcs = CHARSET_ENGINEPHONETIC;
            break;
         }

         WCHAR szPron[256];
         BYTE  abEI[256];
         VOICEPARTOFSPEECH ps;
         DWORD dwNeeded;
         WCHAR *pszText;
         pszText = GenerateRandomText();

         hRes = m_pMode->Get (vcs,
            (WCHAR*) Random(2) ? NULL : pszText,
            (WORD) Random(10),
            Random(2) ? NULL : szPron,
            Random(sizeof(szPron)),
            Random(2) ? NULL : &dwNeeded,
            Random(2) ? NULL : &ps,
            Random(2) ? NULL : abEI,
            (DWORD) Random(sizeof(abEI)),
            Random(2) ? NULL : &dwNeeded);
         if (pszText)
            free (pszText);

         Output ("SRMode:Get");
         Output (hRes);

         break;
      }
   case 38: // ilexpronounce - remove
      {
	      if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test

         WCHAR *pszText;
         pszText = GenerateRandomText();

         hRes = m_pMode->Remove (
            Random(2) ? NULL : pszText,
            (WORD) Random(10));
         if (pszText)
            free (pszText);

         Output ("SRMode:Remove");
         Output (hRes);

         break;
      }
	case 39:	// ilexpronounce2 - getfrom
		{
			if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test

			VOICECHARSET   vcs;
         switch (Random(3)) {
         case 0:
            vcs = CHARSET_TEXT;
            break;
         case 1:
            vcs = CHARSET_IPAPHONETIC;
            break;
         case 2:
            vcs = CHARSET_ENGINEPHONETIC;
            break;
         }

			DWORD	dwLexicon;
			switch (Random(5)) {
			case 0:
					dwLexicon = ILP2_LTS;
				break;
			case 1:
					dwLexicon = ILP2_BACKUP;
				break;
			case 2:
					dwLexicon = ILP2_USER;
				break;
			case 3:
					dwLexicon = ILP2_ACTIVE;
				break;
			case 4:			
					dwLexicon = rand();		// invalid lexicon, most of the time.
				break;
			}

         WCHAR szPron[256];
         BYTE  abEI[256];
         VOICEPARTOFSPEECH ps;
         DWORD dwNeeded;
         WCHAR *pszText;
         pszText = GenerateRandomText();

         hRes = m_pMode->GetFrom (dwLexicon,
				vcs,
            (WCHAR*) Random(2) ? NULL : pszText,
            (WORD) Random(10),
            Random(2) ? NULL : szPron,
            Random(sizeof(szPron)),
            Random(2) ? NULL : &dwNeeded,
            Random(2) ? NULL : &ps,
            Random(2) ? NULL : abEI,
            (DWORD) Random(sizeof(abEI)),
            Random(2) ? NULL : &dwNeeded);
         if (pszText)
            free (pszText);

         Output ("SRMode:GetFrom");
         Output (hRes);
 
			break;
		}
	case 40:	// ilexpronounce2 - addto
		{
			if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test
 
			VOICECHARSET   vcs;
         switch (Random(3)) {
         case 0:
            vcs = CHARSET_TEXT;
            break;
         case 1:
            vcs = CHARSET_IPAPHONETIC;
            break;
         case 2:
            vcs = CHARSET_ENGINEPHONETIC;
            break;
         }

			DWORD	dwLexicon;
			switch (Random(5)) {
			case 0:
					dwLexicon = ILP2_LTS;
				break;
			case 1:
					dwLexicon = ILP2_BACKUP;
				break;
			case 2:
					dwLexicon = ILP2_USER;
				break;
			case 3:
					dwLexicon = ILP2_ACTIVE;
				break;
			case 4:			
					dwLexicon = rand();		// invalid lexicon, most of the time.
				break;
			}
         
			VOICEPARTOFSPEECH vp;
         vp = (VOICEPARTOFSPEECH) Random(17);

         WCHAR *szWord;
         WCHAR *szPron;
         WCHAR *szEI;
         szWord = GenerateRandomText();
         szPron = GenerateRandomText();
         szEI = GenerateRandomText();
         hRes = m_pMode->AddTo (dwLexicon,
				vcs,
            Random(2) ? NULL : szWord,
            Random(2) ? NULL : szPron,
            vp,
            Random(2) ? NULL : (PVOID) szEI,
            Random(2) ? 0 : wcslen(szEI) * 2);
         if (szWord)
            free (szWord);
         if (szPron)
            free (szPron);
         if (szEI)
            free (szEI);

         Output ("SRMode:AddTo");
         Output (hRes);
         break;
		}
	case 41:	// ilexpronounce2 - removefrom
		{
			if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test

			DWORD	dwLexicon;
			switch (Random(5)) {
			case 0:
					dwLexicon = ILP2_LTS;
				break;
			case 1:
					dwLexicon = ILP2_BACKUP;
				break;
			case 2:
					dwLexicon = ILP2_USER;
				break;
			case 3:
					dwLexicon = ILP2_ACTIVE;
				break;
			case 4:			
					dwLexicon = rand();		// invalid lexicon, most of the time.
				break;
			}

         WCHAR *pszText;
         pszText = GenerateRandomText();

         hRes = m_pMode->RemoveFrom (dwLexicon,
            Random(2) ? NULL : pszText,
            (WORD) Random(10));
         if (pszText)
            free (pszText);

         Output ("SRMode:RemoveFrom");
         Output (hRes);

			break;
		}
	case 42:	// ilexpronounce2 - changespelling
		{
			if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test

			DWORD	dwLexicon;
			switch (Random(5)) {
			case 0:
					dwLexicon = ILP2_LTS;
				break;
			case 1:
					dwLexicon = ILP2_BACKUP;
				break;
			case 2:
					dwLexicon = ILP2_USER;
				break;
			case 3:
					dwLexicon = ILP2_ACTIVE;
				break;
			case 4:			
					dwLexicon = rand();		// invalid lexicon, most of the time.
				break;
			}

         WCHAR *pszTextOld;
			WCHAR *pszTextNew;
         pszTextOld = GenerateRandomText();
			pszTextNew = GenerateRandomText();

         hRes = m_pMode->ChangeSpelling (dwLexicon,
            Random(2) ? NULL : pszTextOld,
            Random(2) ? NULL : pszTextNew);

         if (pszTextOld)
         	free (pszTextOld);
			if (pszTextNew)
				free (pszTextNew);

         Output ("SRMode:ChangeSpelling");
         Output (hRes);

			break;
		}
	case 43:	// ilexpronounce2 - querylexicons
		{
			if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test
			
			DWORD dwLexicon;
				
         hRes = m_pMode->QueryLexicons (
				Random(2) ? NULL : Random(2),
				&dwLexicon);
	
         Output ("SRMode:QueryLexicons");
         Output (hRes);

			break;
		}
   }

   MadeCall();
}


/*****************************************************************************
CStress::StressGrammar - This calls a random function i nthe ggrammar and
	stresses it with a random function
*/
void CStress::StressGrammar (DWORD dwNum)
{
   HRESULT  hRes;
   CSRGram  *pGram;
	DWORD		dwCaseToRun;
	
	dwCaseToRun = Random(MAXGRAMMARCASES);

	if( !gfTestGrammar )
		return;
	else {
		if ( !gaGrammarCases[dwCaseToRun].fRun )
			return;
	}

   pGram = m_apGramComp[dwNum] ? m_apGramComp[dwNum] : m_apGram[dwNum];
   if (!pGram)
      return;

   switch (dwCaseToRun) {
   case 0: // Activate
      {
         hRes = pGram->Activate (
            Random(2) ? NULL : m_hWnd,
            Random(10) ? FALSE : TRUE,
            Random(2) ? NULL : RandomSpeaker());

         Output ("SRGram:Activate");
         Output (hRes);
         
         break;
      }
   case 1: // Archive
      {
         BYTE  abTemp[100000];
         DWORD dwSize = 0;

         hRes = pGram->Archive (
            Random(2) ? TRUE : FALSE,
            OnlyIfVeryDifficult() ? NULL : (PVOID) abTemp,
            Random(sizeof(abTemp)),
            OnlyIfVeryDifficult() ? NULL : &dwSize);

         Output ("SRGram:Archive");
         Output (hRes);

         if (hRes || !dwSize)
            break;   // don't try to unarchive

         // try to load

         // delete the old one
         DWORD dwNum;
         dwNum = Random(min(5, MAXGRAM));

         if (m_apGramComp[dwNum])
            delete m_apGramComp[dwNum];
         if (m_apGram[dwNum])
            delete m_apGram[dwNum];
         m_apGramComp[dwNum] = 0;
         m_apGram[dwNum] = 0;

         // grammar load this
         SRGRMFMT fmt;
         switch (Random(3)) {
         case 0:
            fmt = SRGRMFMT_CFGNATIVE;
            break;
         case 1:
            fmt = SRGRMFMT_LIMITEDDOMAINNATIVE;
            break;
         case 2:
            fmt = SRGRMFMT_DICTATIONNATIVE;
            break;
         }
         m_apGram[dwNum] = m_pMode->GrammarLoad (fmt,
            (PVOID) abTemp, dwSize,
            m_pGNS);

         break;
      }
   case 2: // BookMark
      {
         hRes = pGram->BookMark ((QWORD) rand(), Random(65536));

         Output ("SRGram:BookMark");
         Output (hRes);

         break;
      }
   case 3: // Deactivate
      {
         hRes = pGram->Deactivate (
            Random(2) ? NULL : RandomSpeaker());

         Output ("SRGram:Deactivate");
         Output (hRes);
         
         break;
      }
   case 4: // DeteriorationGet
      {
         DWORD dwVal;
         hRes = pGram->DeteriorationGet (
            OnlyIfDifficult() ? NULL : &dwVal,
            OnlyIfDifficult() ? NULL : &dwVal,
            OnlyIfDifficult() ? NULL : &dwVal);

         Output ("SRGram:DeteriorationGet");
         Output (hRes);
         
         break;
      }
   case 5: // DeteriorationSet
      {
         hRes = pGram->DeteriorationSet (
            Random (1000000),
            Random (1000000),
            Random (10000));

         Output ("SRGram:DeteriorationSet");
         Output (hRes);
         
         break;
      }
   case 6: // TrainDlg
      {
         WCHAR *psz = GenerateRandomText();
         hRes = pGram->TrainDlg (NULL, OnlyIfDifficult ? psz : NULL);
         if (psz)
            free (psz);
         Output ("SRGram:TrainDlg");
         Output (hRes);
         break;
      }
   case 7: // TrainPhrase
      {
         SDATA   Data;
         Data.pData =NULL;
         Data.dwSize = 0;
         hRes = pGram->TrainPhrase (Random(5),
            OnlyIfVeryDifficult() ? NULL : &Data);
         if (Data.pData)
            CoTaskMemFree (Data.pData);

         Output ("SRGram:TrainPhrase");
         Output (hRes);
         break;
      }
   case 8: // TrainQuery
      {
         DWORD dwVal;
         hRes = pGram->TrainQuery (
            OnlyIfVeryDifficult() ? NULL : &dwVal);

         Output ("SRGram:TrainQuery");
         Output (hRes);
         break;
      }
   case 9: // LinkeQuery
      {
         BOOL  fExist;
         hRes = pGram->LinkQuery (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            OnlyIfVeryDifficult() ? NULL : &fExist);


         Output ("SRGram:LinkQuery");
         Output (hRes);
         break;
      }
   case 10: // ListAppend
      {
         // generate a set of words
         BYTE  abTemp[100000];
         DWORD dwNumWords, i;
         DWORD dwCur;
         dwNumWords = Random(gdwDifficulty * 100 + 100);
         dwCur = 0;
         for (i = 0; i < dwNumWords; i++) {
            dwCur += RandomSRWORD (abTemp + dwCur);
         }

         // pass it in
         SDATA sd;
         sd.pData = (PVOID) abTemp;
         sd.dwSize = dwCur;

         // sometimes send in junk
         if (OnlyIfVeryDifficult())
            FillBufWithText (3, (WCHAR*) abTemp, sd.dwSize / 2);

         hRes = pGram->ListAppend (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            sd);

         Output ("SRGram:ListAppend");
         Output (hRes);
         break;
      }
   case 11: // ListGet
      {
         SDATA   Data;
         Data.pData =NULL;
         Data.dwSize = 0;
         hRes = pGram->ListGet (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            OnlyIfVeryDifficult() ? NULL : &Data);
         if (Data.pData)
            CoTaskMemFree (Data.pData);

         Output ("SRGram:ListGet");
         Output (hRes);
         break;
      }
   case 12: // ListQuery
      {
         BOOL  fExist;
         hRes = pGram->ListQuery (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            OnlyIfVeryDifficult() ? NULL : &fExist);


         Output ("SRGram:ListQuery");
         Output (hRes);
         break;
      }
   case 13: // ListRemove
      {
         // generate a set of words
         BYTE  abTemp[100000];
         DWORD dwNumWords, i;
         DWORD dwCur;
         dwNumWords = Random(gdwDifficulty * 100 + 100);
         dwCur = 0;
         for (i = 0; i < dwNumWords; i++) {
            dwCur += RandomSRWORD (abTemp + dwCur);
         }

         // pass it in
         SDATA sd;
         sd.pData = (PVOID) abTemp;
         sd.dwSize = dwCur;

         // sometimes send in junk
         if (OnlyIfVeryDifficult())
            FillBufWithText (3, (WCHAR*) abTemp, sd.dwSize / 2);

         hRes = pGram->ListRemove (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            sd);

         Output ("SRGram:ListRemove");
         Output (hRes);
         break;
      }
   case 14: // ListSet
      {
         // generate a set of words
         BYTE  abTemp[100000];
         DWORD dwNumWords, i;
         DWORD dwCur;
         dwNumWords = Random(gdwDifficulty * 100 + 100);
         dwCur = 0;
         for (i = 0; i < dwNumWords; i++) {
            dwCur += RandomSRWORD (abTemp + dwCur);
         }

         // pass it in
         SDATA sd;
         sd.pData = (PVOID) abTemp;
         sd.dwSize = dwCur;

         // sometimes send in junk
         if (OnlyIfVeryDifficult())
            FillBufWithText (3, (WCHAR*) abTemp, sd.dwSize / 2);

         hRes = pGram->ListSet (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            sd);

         Output ("SRGram:ListSet");
         Output (hRes);
         break;
      }
   case 15: // Context
      {
         WCHAR *pszPrior, *pszAfter;
         pszPrior = GenerateRandomText();
         pszAfter = GenerateRandomText();

         hRes = pGram->Context (
            Random(2) ? NULL : pszPrior,
            Random(2) ? NULL : pszAfter);

         if (pszPrior)
            free (pszPrior);
         if (pszAfter)
            free (pszAfter);


         Output ("SRGram:Context");
         Output (hRes);
         break;
      }
   case 16: // Hint
      {
         WCHAR *pszPrior;
         pszPrior = GenerateRandomText();

         hRes = pGram->Hint (
            OnlyIfVeryDifficult() ? NULL : pszPrior);

         if (pszPrior)
            free (pszPrior);


         Output ("SRGram:Hint");
         Output (hRes);
         break;
      }
   case 17: // Words
      {
         WCHAR *pszPrior;
         pszPrior = GenerateRandomText();

         hRes = pGram->Words (
            OnlyIfVeryDifficult() ? NULL : pszPrior);

         if (pszPrior)
            free (pszPrior);


         Output ("SRGram:Words");
         Output (hRes);
         break;
         break;
      }
   case 18: // Hide
      {
         hRes = pGram->Hide();


         Output ("SRGram:Hide");
         Output (hRes);
         break;
      }
   case 19: // Move
      {
         RECT  r;
         r.left = rand();
         r.right = rand();
         r.top = rand();
         r.bottom = rand();

         hRes = pGram->Move(r);


         Output ("SRGram:Move");
         Output (hRes);
         break;
      }
   case 20: // Show
      {
         hRes = pGram->Show(
            OnlyIfVeryDifficult() ? (HWND) Random(65536) : (Random(2) ? NULL : m_hWnd));

         Output ("SRGram:Show");
         Output (hRes);
         break;
      }
	case 21:	// ilexpronounce2 - getfrom
		{
			if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test

			VOICECHARSET   vcs;
         switch (Random(3)) {
         case 0:
            vcs = CHARSET_TEXT;
            break;
         case 1:
            vcs = CHARSET_IPAPHONETIC;
            break;
         case 2:
            vcs = CHARSET_ENGINEPHONETIC;
            break;
         }

			DWORD	dwLexicon;
			switch (Random(5)) {
			case 0:
					dwLexicon = ILP2_LTS;
				break;
			case 1:
					dwLexicon = ILP2_BACKUP;
				break;
			case 2:
					dwLexicon = ILP2_USER;
				break;
			case 3:
					dwLexicon = ILP2_ACTIVE;
				break;
			case 4:			
					dwLexicon = rand();		// invalid lexicon, most of the time.
				break;
			}

         WCHAR szPron[256];
         BYTE  abEI[256];
         VOICEPARTOFSPEECH ps;
         DWORD dwNeeded;
         WCHAR *pszText;
         pszText = GenerateRandomText();

         hRes = pGram->GetFrom (dwLexicon,
				vcs,
            (WCHAR*) Random(2) ? NULL : pszText,
            (WORD) Random(10),
            Random(2) ? NULL : szPron,
            Random(sizeof(szPron)),
            Random(2) ? NULL : &dwNeeded,
            Random(2) ? NULL : &ps,
            Random(2) ? NULL : abEI,
            (DWORD) Random(sizeof(abEI)),
            Random(2) ? NULL : &dwNeeded);
         if (pszText)
            free (pszText);

         Output ("SRGram:GetFrom");
         Output (hRes);
 
			break;
		}
	case 22:	// ilexpronounce2 - addto
		{
			if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test
 
			VOICECHARSET   vcs;
         switch (Random(3)) {
         case 0:
            vcs = CHARSET_TEXT;
            break;
         case 1:
            vcs = CHARSET_IPAPHONETIC;
            break;
         case 2:
            vcs = CHARSET_ENGINEPHONETIC;
            break;
         }

			DWORD	dwLexicon;
			switch (Random(5)) {
			case 0:
					dwLexicon = ILP2_LTS;
				break;
			case 1:
					dwLexicon = ILP2_BACKUP;
				break;
			case 2:
					dwLexicon = ILP2_USER;
				break;
			case 3:
					dwLexicon = ILP2_ACTIVE;
				break;
			case 4:			
					dwLexicon = rand();		// invalid lexicon, most of the time.
				break;
			}
         
			VOICEPARTOFSPEECH vp;
         vp = (VOICEPARTOFSPEECH) Random(17);

         WCHAR *szWord;
         WCHAR *szPron;
         WCHAR *szEI;
         szWord = GenerateRandomText();
         szPron = GenerateRandomText();
         szEI = GenerateRandomText();
         hRes = pGram->AddTo (dwLexicon,
				vcs,
            Random(2) ? NULL : szWord,
            Random(2) ? NULL : szPron,
            vp,
            Random(2) ? NULL : (PVOID) szEI,
            Random(2) ? 0 : wcslen(szEI) * 2);
         if (szWord)
            free (szWord);
         if (szPron)
            free (szPron);
         if (szEI)
            free (szEI);

         Output ("SRGram:AddTo");
         Output (hRes);
         break;
		}
	case 23:	// ilexpronounce2 - removefrom
		{
			if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test

			DWORD	dwLexicon;
			switch (Random(5)) {
			case 0:
					dwLexicon = ILP2_LTS;
				break;
			case 1:
					dwLexicon = ILP2_BACKUP;
				break;
			case 2:
					dwLexicon = ILP2_USER;
				break;
			case 3:
					dwLexicon = ILP2_ACTIVE;
				break;
			case 4:			
					dwLexicon = rand();		// invalid lexicon, most of the time.
				break;
			}

         WCHAR *pszText;
         pszText = GenerateRandomText();

         hRes = pGram->RemoveFrom (dwLexicon,
            Random(2) ? NULL : pszText,
            (WORD) Random(10));
         if (pszText)
            free (pszText);

         Output ("SRGram:RemoveFrom");
         Output (hRes);

			break;
		}
	case 24:	// ilexpronounce2 - changespelling
		{
			if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test

			DWORD	dwLexicon;
			switch (Random(5)) {
			case 0:
					dwLexicon = ILP2_LTS;
				break;
			case 1:
					dwLexicon = ILP2_BACKUP;
				break;
			case 2:
					dwLexicon = ILP2_USER;
				break;
			case 3:
					dwLexicon = ILP2_ACTIVE;
				break;
			case 4:			
					dwLexicon = rand();		// invalid lexicon, most of the time.
				break;
			}

         WCHAR *pszTextOld;
			WCHAR *pszTextNew;
         pszTextOld = GenerateRandomText();
			pszTextNew = GenerateRandomText();

         hRes = pGram->ChangeSpelling (dwLexicon,
            Random(2) ? NULL : pszTextOld,
            Random(2) ? NULL : pszTextNew);

         if (pszTextOld)
         	free (pszTextOld);
			if (pszTextNew)
				free (pszTextNew);

         Output ("SRGram:ChangeSpelling");
         Output (hRes);

			break;
		}
	case 25:	// ilexpronounce2 - querylexicons
		{
			if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // don't test
			
			DWORD dwLexicon;
				
         hRes = pGram->QueryLexicons (
				Random(2) ? NULL : Random(2),
				&dwLexicon);
	
         Output ("SRGram:QueryLexicons");
         Output (hRes);

			break;
		}
   }

   MadeCall();
}



/*****************************************************************************
CStress:StressResult - This calls a random fucntion in the results object
	and stresses it with a random function.
*/
void CStress::StressResult (DWORD dwNum)
{
   HRESULT  hRes;
   DWORD    dwVal;
   CSRResult  *pRes;
	DWORD		dwCaseToRun;
	
	dwCaseToRun = Random(MAXRESULTCASES);

	if( !gfTestResult )
		return;
	else {
		if ( !gaResultCases[dwCaseToRun].fRun )
			return;
	}

	
   pRes = m_apResult[dwNum];
   if (!pRes)
      return;

   switch (dwCaseToRun) {
   case 0: // GetWAV
      {
         SDATA sd;
         sd.pData = 0;
         hRes = pRes->GetWAV (OnlyIfVeryDifficult() ? NULL : &sd);
         if (sd.pData)
            CoTaskMemFree (sd.pData);

         Output ("SRResult:GetWAV");
         Output (hRes);
         break;
      }
   case 1: // FlagsGet
      {
         hRes = pRes->FlagsGet (Random(20),
            OnlyIfVeryDifficult() ? NULL : &dwVal);

         Output ("SRResult:FlagsGet");
         Output (hRes);
         break;
      }
   case 2: // Identify
      {
         GUID g;
         hRes = pRes->Identify (
            OnlyIfVeryDifficult() ? NULL : &g);

         Output ("SRResult:Identify");
         Output (hRes);
         break;
      }
   case 3: // PhraseGet
      {
         BYTE  abTemp[1000];
         hRes = pRes->PhraseGet (
            Random(20),
            OnlyIfDifficult() ? NULL : (PSRPHRASEW) abTemp,
            Random(sizeof(abTemp)),
            OnlyIfVeryDifficult() ? NULL : &dwVal);

         Output ("SRResult:PhraseGet");
         Output (hRes);
         break;
      }
   case 4: // TimeGet
      {
         QWORD qw1, qw2;
         hRes = pRes->TimeGet (
            OnlyIfVeryDifficult() ? NULL : &qw1,
            OnlyIfVeryDifficult() ? NULL : &qw2);

         Output ("SRResult:TimeGet");
         Output (hRes);
         break;
      }
   case 5: // Correction
      {
         // only if very difficult
         if (gdwDifficulty < DIFFICULTY_HARD)
            return;

         // generate a random phrase
         BYTE  abTemp[20000];
         DWORD dwNumWords = Random(50);
         DWORD i;
         DWORD dwTotal = 4;
         for (i = 0; i < dwNumWords; i++)
            dwTotal += RandomSRWORD(abTemp + dwTotal);
         ((DWORD*) abTemp)[0] = dwTotal;

         // send it down
         hRes = pRes->Correction ((PSRPHRASEW) abTemp,
            (WORD) Random(3));


         Output ("SRResult:Correction");
         Output (hRes);
         break;
      }
   case 6: // Validate
      {
         // only if very difficult
         if (gdwDifficulty < DIFFICULTY_HARD)
            return;

         // send it down
         hRes = pRes->Validate ((WORD) Random(3));


         Output ("SRResult:Validate");
         Output (hRes);
         break;
      }
   case 7: // ReEvaluate
      {
         BOOL  f;

         hRes = pRes->ReEvaluate (
            OnlyIfVeryDifficult() ? NULL : &f);

         Output ("SRResult:ReEvaluate");
         Output (hRes);
         break;
      }
   case 8: // BestPathPhoneme
      {
         DWORD dwPath[500];

         hRes = pRes->BestPathPhoneme (Random(20),
            OnlyIfVeryDifficult() ? NULL : dwPath,
            Random(500),
            OnlyIfVeryDifficult() ? NULL : &dwVal);

         Output ("SRResult:BestPathPhoneme");
         Output (hRes);
         break;
      }
   case 9: // BestPathWord
      {
         DWORD dwPath[50];

         hRes = pRes->BestPathWord (Random(20),
            OnlyIfVeryDifficult() ? NULL : dwPath,
            Random(50),
            OnlyIfVeryDifficult() ? NULL : &dwVal);

         Output ("SRResult:BestPathWord");
         Output (hRes);
         break;
      }
   case 10: // GetPhonemeNode
      {
         SRRESPHONEMENODE  n;
         WCHAR w1, w2;

         hRes = pRes->GetPhonemeNode ((DWORD)rand(),
            OnlyIfVeryDifficult() ? NULL : &n,
            OnlyIfVeryDifficult() ? NULL : &w1,
            OnlyIfVeryDifficult() ? NULL : &w2);

         Output ("SRResult:GetPhonemeNode");
         Output (hRes);
         break;
      }
   case 11: // GetWordNode
      {
         SRRESWORDNODE  n;
         BYTE  abTemp[200];

         hRes = pRes->GetWordNode ((DWORD)rand(),
            OnlyIfVeryDifficult() ? NULL : &n,
            OnlyIfVeryDifficult() ? NULL : (PSRWORDW) abTemp,
            Random(sizeof(abTemp)),
            OnlyIfVeryDifficult() ? NULL : &dwVal);

         Output ("SRResult:GetWordNode");
         Output (hRes);
         break;
      }
   case 12: // PathScorePhoneme
      {
         DWORD dwPath[100];
         DWORD i;
         LONG  lScore;
         for (i = 0; i < 100; i++)
            dwPath[i] = (DWORD) rand();

         hRes = pRes->PathScorePhoneme (
            OnlyIfVeryDifficult() ? NULL : dwPath,
            Random(100),
            OnlyIfVeryDifficult() ? NULL : &lScore);

         Output ("SRResult:PathScorePhoneme");
         Output (hRes);
         break;
      }
   case 13: // PathScoreWord
      {
         DWORD dwPath[100];
         DWORD i;
         LONG  lScore;
         for (i = 0; i < 100; i++)
            dwPath[i] = (DWORD) rand();

         hRes = pRes->PathScoreWord (
            OnlyIfVeryDifficult() ? NULL : dwPath,
            Random(100),
            OnlyIfVeryDifficult() ? NULL : &lScore);

         Output ("SRResult:PathScoreWord");
         Output (hRes);
         break;
      }
   case 14: // Free
      {
         hRes = pRes->Free ((DWORD) rand());


         Output ("SRResult:Free");
         Output (hRes);
         break;
      }
   case 15: // Get
      {
         DWORD dwKind;

         hRes = pRes->Get (
            OnlyIfVeryDifficult() ? NULL : &dwKind,
            OnlyIfVeryDifficult() ? NULL : &dwVal);

         Output ("SRResult:Get");
         Output (hRes);
         break;
      }
   case 16: // LockGet
      {
         BOOL f;

         hRes = pRes->LockGet (
            OnlyIfVeryDifficult() ? NULL : &f);

         Output ("SRResult:LockGet");
         Output (hRes);
         break;
      }
   case 17: // LockSet
      {
         hRes = pRes->LockSet ((BOOL) Random(3));

         Output ("SRResult:LockSet");
         Output (hRes);
         break;
      }
   case 18: // Merge
      {
         CSRResult   *pNew, *pUse;
         pUse = m_apResult[Random(MAXRESULT)];
         if (!pUse)
            return;
         pNew = pRes->Merge (pUse);
         hRes = 0;
         if (!pNew) {
            hRes = E_FAIL;

            // keep it around
            dwVal = Random(MAXRESULT);
            if (m_apResult[dwVal])
               delete m_apResult[dwVal];
            m_apResult[dwVal] = pNew;
         }

         Output ("SRResult:Merge");
         Output (hRes);
         break;
      }
   case 19: // Split
      {
         CSRResult   *pLeft, *pRight;
         pLeft = pRight = NULL;
         hRes = pRes->Split ((QWORD) rand(), &pLeft, &pRight);
         if (pLeft)
            delete pLeft;
         if (pRight)
            delete pRight;

         Output ("SRResult:Split");
         Output (hRes);
         break;
      }
   case 20: // Hide
      {
         hRes = pRes->Hide();


         Output ("SRResult:Hide");
         Output (hRes);
         break;
      }
   case 21: // Move
      {
         RECT  r;
         r.left = rand();
         r.right = rand();
         r.top = rand();
         r.bottom = rand();

         hRes = pRes->Move(
            OnlyIfVeryDifficult() ? NULL : &r);


         Output ("SRResult:Move");
         Output (hRes);
         break;
      }
   case 22: // Show
      {
         hRes = pRes->Show(
            OnlyIfVeryDifficult() ? (HWND) Random(65536) : (Random(2) ? NULL : m_hWnd));

         Output ("SRResult:Show");
         Output (hRes);
         break;
      }
   case 23: // GetPhraseScore
      {
         long  lScore;

         hRes = pRes->GetPhraseScore (
            Random(20),
            OnlyIfVeryDifficult() ? NULL : &lScore);

         Output ("SRResult:GetPhraseScore");
         Output (hRes);
         break;
      }
   case 24: // GetWordScores
      {
         long  alScore[100];
         hRes = pRes->GetWordScores (
            Random(20),
            OnlyIfVeryDifficult() ? NULL : alScore,
            Random(sizeof(alScore)),
            OnlyIfVeryDifficult() ? NULL : &dwVal);

         Output ("SRResult:GetWordScores");
         Output (hRes);
         break;
      }
   case 25: // SpeakerCorrection
      {
         if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // might mess up SR

         hRes = pRes->SpeakerCorrection (
            OnlyIfVeryDifficult() ? NULL : RandomSpeaker(),
            (WORD) Random(3));

         Output ("SRResult:SpeakerCorrection");
         Output (hRes);
         break;
      }
   case 26: // SpeakerIdentify
      {
         WCHAR szTemp[256];
         long  lScore;

         hRes = pRes->SpeakerIdentify (
            Random(20),
            OnlyIfVeryDifficult() ? NULL : szTemp,
            Random(sizeof(szTemp)),
            OnlyIfVeryDifficult() ? NULL : &dwVal,
            OnlyIfVeryDifficult() ? NULL : &lScore);

         Output ("SRResult:SpeakerIdentify");
         Output (hRes);
         break;
      }
   case 27: // SpeakerIdentifyForFree
      {
         BOOL f;
         hRes = pRes->SpeakerIdentifyForFree (
            OnlyIfVeryDifficult() ? NULL : &f);

         Output ("SRResult:SpeakerIdentifyForFree");
         Output (hRes);
         break;
      }
   case 28: // SpeakerValidate
      {
         if (gdwDifficulty < DIFFICULTY_HARD)
            return;  // might mess up SR

         hRes = pRes->SpeakerValidate (
            (WORD) Random(3));

         Output ("SRResult:SpeakerValidate");
         Output (hRes);
         break;
      }
   }

   MadeCall();
}








