/**********************************************************************

TTSStress.cpp - Stress test application

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
#include "tsstress.h"

void _cdecl StressThread (PVOID pData);

#define  MAXMODES       100    // maximum # of modes a user can select
#define  MAXINSTANCES   100    // maximum # of engine instances we can have

class CStress;


class CStressNotifySink : public ITTSNotifySinkW {
   public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ITTSNotifySink
      STDMETHODIMP AttribChanged  (DWORD);
      STDMETHODIMP AudioStart (QWORD);
      STDMETHODIMP AudioStop (QWORD);
      STDMETHODIMP Visual (QWORD, WCHAR, WCHAR, DWORD, PTTSMOUTH);

      CStress     *m_pStress;
};

class CStressBufNotifySink: public ITTSBufNotifySinkW {
   public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ITTSNotifySink
      STDMETHODIMP TextDataDone (QWORD, DWORD);
      STDMETHODIMP TextDataStarted (QWORD);
      STDMETHODIMP BookMark (QWORD, DWORD);  
      STDMETHODIMP WordPosition (QWORD, DWORD);

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



#define     TIMERID     4289

BOOL CALLBACK StatusDialog (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
   void MadeCall (void);
   void StressEngine (void);
   void StressOnCallback (void);

	// variables
	DWORD		      m_dwThreadNum;		// thread number. 0 is the first one. Each one follows
   DWORD          m_dwWaveNum;      // wave number to write to disk with
	CStressNotifySink *m_pNS;		// notification sink
	CStressBufNotifySink *m_pBNS;	// notification sink
	CStressWaveAudioNotifySink *m_pWANS;	// notification sink
	CTTSMode		   *m_pMode;			// speech recognition mode
   PIAUDIOFILE    m_pIWaveAudio; // audio object for wave.
   HWND           m_hWnd;           // window handle for the dialog
   DWORD          m_dwTimerElapse;  // number of MS between timer

   DWORD          m_dwTextSendOut;      // total phrase count that heard
   QWORD          m_qwTotalAudio;   // total audio passed through
   QWORD          m_qwMaxAudio;     // max audio from this object
   QWORD          m_qwFunctionCalls;// total number of function calls
   WCHAR          m_szWave[256];    // wave name, gerneate from the wave number

   FILE           *m_file;           // file to log to
};


#define     MAXNAMES    100


CTTSEnum     *pEnum = NULL;
GUID        gagMode[MAXMODES];
DWORD       gdwNumModes = 0;  // number of modes used
DWORD       gdwDifficulty = 0;   //0 = easy, 1 = medium, 2 = hard
BOOL			gfNonRandomStrings = FALSE;	// true if to always use 'aaaa...'
BOOL			gfNoTags = FALSE;					// true if no tags are to be used in text
HINSTANCE   ghInstance;
WCHAR       gaszSpeakerNames[MAXNAMES][128];
BOOL        gfUseMultimedia;  // set to TRUE if we use the multimedia for the first thread
BOOL			gfGenerateLogFile;	// set to TRUE if we generate and populate log file
DWORD       gdwNumThreads;    // number of threads to create
HANDLE      gahEventShutDownNow[MAXINSTANCES];  // event signaling thread should shut down
HANDLE      gahEventIsShutDown[MAXINSTANCES];   // event signaling that thread has shut down
BOOL			gfEngineCases[MAXENGINECASES];		// lists engine cases to test


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
   iVal = rand() % 13;

	// possible context tags
	wchar_t * pszCtx[8] = {L"Address",L"C",L"Document",L"E-Mail",L"Spreadsheet",
			L"Unknown",L"Normal",L"Unknown"};
	// possible part of speech tags
	wchar_t * pszPrt[16] = {L"Abbr",L"Adj",L"Adv",L"Card",L"Conj",L"Cont",L"Det",L"Interj",
		L"N",L"Ord",L"Prep",L"Pron",L"Prop",L"Punct",L"Quant",L"V"}; 
	// possible languages
	wchar_t * pszLang[10] = {L"English",L"French",L"German",L"Spanish",L"Italian",L"Japanese",
		L"Cantonese",L"Mandorin",L"Portugese",L"Russian"};
	// some names
	wchar_t * pszName[10] = {L"Mike",L"Bill",L"Dave",L"Mark",L"Doug",L"Eric",L"Scott",L"Mary",
		L"Gloria",L"Tammy"};
	// possible ages
	wchar_t * pszAge[6] = {L"Baby",L"Toddler",L"Child",L"Adolescent",L"Adult",L"Elderly"};
	// possible styles
	wchar_t * pszStyle[5] = {L"Business",L"Casual",L"Computer",L"Excited",L"Singsong"};
		

	// 0-6 are required tags so use them even on easy tests
   if (dwType == 0)
      iVal = min(6, iVal);

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
		wcscpy (psz, L"\\com=\"this is a comment\"\\");
		break;
   case 7:
      swprintf (psz, L"\\pro=%d\\", (int) rand());
      break;
   case 9:
		// ctx: pick random context
		int iCtx;
		iCtx = rand() % 8;
		swprintf (psz, L"\\ctx=\"%s\"\\",pszCtx[iCtx]);
      break;
	case 10:
		wcscpy (psz, L"\\prn=tomato=tomaato\\");
		break;
	case 11:
		// prt: pick random context
		int iPrt;
		iPrt = rand() % 16;
		swprintf (psz, L"\\prt=\"%s\"\\",pszPrt[iPrt]);
      break;
	case 12:
		{
			//vce
			int iVce;
			iVce = rand() % 7;
			switch (iVce) {
			case 0:
				// language
				int iLang;
				iLang = rand() % 10;
				swprintf (psz, L"\\vce=Language=%s\\",pszLang[iLang]);
				break;
			case 1:
				// Accent
				iLang = rand() % 10;
				swprintf (psz, L"\\vce=Accent=%s\\",pszLang[iLang]);
				break;
			case 2:
				// Dialect
				iLang = rand() % 10;
				swprintf (psz, L"\\vce=Dialect=%s\\",pszLang[iLang]);
				break;
			case 3:
				// Gender
				int iGender;
				iGender = rand() % 3;
				switch (iGender) {
				case 0:
					wcscpy (psz, L"\\vce=Gender=Male\\");
					break;
				case 1:
					wcscpy (psz, L"\\vce=Gender=Female\\");
					break;
				case 2:
					wcscpy (psz, L"\\vce=Gender=Neutral\\");
					break;
				}
				break;
			case 4:
				// speaker name
				int iName;
				iName = rand() % 10;
				swprintf (psz, L"\\vce=Speaker=%s\\",pszName[iName]);
				break;
			case 5:
				// age
				int iAge;
				iAge = rand() % 6;
				swprintf (psz, L"\\vce=Age=%s\\",pszAge[iAge]);
				break;
			case 6:
				// style
				int iStyle;
				iStyle = rand() % 5;
				swprintf (psz, L"\\vce=Style=%s\\",pszStyle[iStyle]);
				break;
			}
		}
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

	if (gfNonRandomStrings)
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
      if (gdwDifficulty == 0)
         iVal = iVal % 2;
      else if (gdwDifficulty == 1)
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
	if (!gfNoTags) {
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
      TTSMODEINFOW TTSInfo;

      pEnum->Reset();
      while (!pEnum->Next(1, &TTSInfo, NULL)) {
         char  szTemp[128];
         WideCharToMultiByte (CP_ACP, 0, TTSInfo.szModeName, -1,
            szTemp, sizeof(szTemp), 0, 0);
         SendDlgItemMessage (hWnd, IDC_MODE, LB_ADDSTRING,
            0, (LPARAM) szTemp);
      }

      // check the dialog item button
      CheckDlgButton (hWnd, IDC_EASY, BST_CHECKED);
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
            TTSMODEINFOW TTSInfo;

            for (i = 0; i < dwCount; i++) {
               pEnum->Reset();
               pEnum->Skip(aItemNum[i]);
               pEnum->Next(1, &TTSInfo, NULL);
               gagMode[i] = TTSInfo.gModeID;
            }

         }

         // figure out if easy/medium/hard
         if (IsDlgButtonChecked (hWnd, IDC_MEDIUM))
            gdwDifficulty = 1;
         else if (IsDlgButtonChecked (hWnd, IDC_HARD))
            gdwDifficulty = 2;
         else
            gdwDifficulty = 0;

			// use tags?
			gfNoTags = (IsDlgButtonChecked (hWnd, IDC_CHECK_NOTAGS)) ? TRUE : FALSE;

			// use random strings?
			gfNonRandomStrings = (IsDlgButtonChecked (hWnd, IDC_CHECK_NONRANDOM_STRINGS)) ? TRUE : FALSE;


         // warn the user if difficulty is set to high
         if (gdwDifficulty >= 2) {
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
		// optional radio button is on by default
		CheckRadioButton (hWnd, IDC_RADIO_REQUIRED, IDC_RADIO_OPTIONAL, IDC_RADIO_OPTIONAL);
		
			// populate list box
		{
			for( DWORD dwCount=0; dwCount < MAXENGINECASES; dwCount++) {
	   		SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_ADDSTRING,
				  0, (LPARAM) gaEngineCases[dwCount].pszCaseName);
			}
   		SendDlgItemMessage (hWnd, IDC_LIST_ENGINE, LB_SELITEMRANGE, TRUE, MAKELPARAM(0, MAXENGINECASES -1)); 
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
		}
		return TRUE;

	case IDOK:
		{
			BOOL bCaseSelected = FALSE;
			int iListIndex;
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

			// make sure something was selected to test
			if( !bCaseSelected ) {
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
            _beginthread (StressThread, 0, (LPVOID) i);
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

   pEnum = new CTTSEnum;
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
   if (IDOK != DialogBox (ghInstance, MAKEINTRESOURCE(IDD_INTRO3), NULL, (DLGPROC) Intro3DlgProc))
      goto shutdown;
	if (IDOK != DialogBox (ghInstance, MAKEINTRESOURCE(IDD_TESTLIST), NULL, (DLGPROC) TestListDlgProc))
      goto shutdown;

   // unload the enumerator
   if (pEnum)
      delete pEnum;
   pEnum = NULL;

   // do the tests
   DialogBox (ghInstance, MAKEINTRESOURCE(IDD_MAINTHREAD), NULL, (DLGPROC) MainThreadDlgProc);

   // tell the user everything's unloaded
   CoFreeUnusedLibraries();

   MessageBox (NULL, "All ofthe engines have been unloaded.", "Stress", MB_OK);

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

   char  szTemp[256];
	if (gfGenerateLogFile) {
		wsprintf (szTemp, "c:\\ttsstress%d.txt", (int) dwNum);
		m_file = fopen(szTemp, "wt");
	}
	else
		m_file = NULL;

   m_dwThreadNum = dwNum;

	// I'm not too worried about these running out of memory
	m_pNS = new CStressNotifySink;
	m_pNS->m_pStress = this;
	m_pBNS = new CStressBufNotifySink;
	m_pBNS->m_pStress = this;
	m_pWANS = new CStressWaveAudioNotifySink;
	m_pWANS->m_pStress = this;

   // wipe out some pointers to TTS objects
   m_pMode = NULL;
   m_pIWaveAudio = NULL;

   // misc
   m_hWnd = NULL;
   m_dwTextSendOut = 0;
   m_qwTotalAudio = m_qwMaxAudio = 0;
   m_qwFunctionCalls = 0;

   // initialize the random function
   srand (GetTickCount());

   // gerneate a random wave number
   DWORD i;
   for (i = 0; i <= m_dwThreadNum; i++)
      m_dwWaveNum = (WORD) rand();
   swprintf (m_szWave, L"c:\\w%d.wav", (int) m_dwWaveNum);

   // generate a random timer elapse
   switch (gdwDifficulty) {
   case 0:  // easy
      m_dwTimerElapse = 800 + Random(400);
      break;
   case 1:  // medium
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
   if(m_pIWaveAudio)
	   m_pIWaveAudio->Release();

   // free all the objects
   FreeAll();

   // free the notification sinks
   if (m_pNS)
      delete m_pNS;
   if (m_pBNS)
      delete m_pBNS;
   if (m_pWANS)
      delete m_pWANS;

   // delete the wave file
   char  szTemp[256];
   wsprintf (szTemp, "c:\\w%d.wav", (int) m_dwWaveNum);
   DeleteFile (szTemp);

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

//   return 0;
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
	reoognition, this is the TTSMode, several grammars, and several results
	objects.
*/
void CStress::FreeAll (void)
{
   // If we're using the wave-file, we need to store the old amount
   // parsed away, so that when the new wave-file is stared, we'll just
   // be incrementing the old value
   m_qwTotalAudio += m_qwMaxAudio;
   m_qwMaxAudio = 0;

   // close the output file if it's there
   if (m_pIWaveAudio)
      m_pIWaveAudio->Set (NULL, 1);

#ifdef TESTREFCOUNT
   DWORD dwRet;
   PITTSCENTRALW   pITTSCentral;
   pITTSCentral = m_pMode->m_pITTSCentral;
   dwRet = pITTSCentral->AddRef();
#endif

   if (m_pMode)
      delete m_pMode;

#ifdef TESTREFCOUNT
   // dwRet should return 0, because it'll be the last thing
   // in the object released
   dwRet = pITTSCentral->Release();
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

		// show the start time
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
BOOL CALLBACK StatusDialog (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
   PCWSTR   paszNames[10] = {L"Mike", L"Larry", L"Jim", L"Bob", L"Doug",
      L"Fil", L"Yunus", L"John", L"Bill", L"Greg"};

   switch(Random(gdwDifficulty+1)) {
   case 0:
      return (WCHAR*) paszNames[Random(10)];
   case 1:
      return gaszSpeakerNames[Random(MAXNAMES / 4)];
   default:
      return gaszSpeakerNames[Random(MAXNAMES)];
   }
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


}


/*****************************************************************************
CStress::MakeSureHaveEngine - This makes sure that we have a TTS engine. If there
	iesn't one, a new one is randomly generated.
*/
HRESULT CStress::MakeSureHaveEngine (void)
{
   HRESULT  hRes;
   // if already have one all done
   if (m_pMode)
      return NOERROR;

   // allocate object
   m_pMode = new CTTSMode;
   if (!m_pMode)
      return E_OUTOFMEMORY;

   // If it's not the first thread, or if it is the first thread
   // and the global flag is NOT set for using the wave device, then use
   // wave file
   if (m_dwThreadNum || !gfUseMultimedia) {
   // Create the wave file object and initialize it
	   hRes = CoCreateInstance(CLSID_AudioDestFile, 
		   NULL, CLSCTX_INPROC_SERVER, 
		   IID_IAudioFile, 
		   (PVOID *) &m_pIWaveAudio);
	   if (hRes) goto alldone;
	   m_pIWaveAudio -> Register(m_pWANS);

      // add the file
      m_pIWaveAudio->Set (m_szWave, 1);

      m_pMode->InitAudioDestObject(m_pIWaveAudio);
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
CStressNotifySink - TTS Notify sink
*/

STDMETHODIMP CStressNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITTSNotifySinkW)) {
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
   wsprintf (szTemp, "ITTSNotifySink::Attribute %d changed\r\n", dwAttrib);
   m_pStress->Output (szTemp);
   return NOERROR;
}

STDMETHODIMP CStressNotifySink::AudioStart (QWORD qwStart)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ITTSNotifySink::AudioStart\r\n");
   m_pStress->Output (szTemp);
   return NOERROR;
}

STDMETHODIMP CStressNotifySink::AudioStop (QWORD qwStart)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ITTSNotifySink::AudioStop\r\n");
   m_pStress->Output (szTemp);
   return NOERROR;
}

STDMETHODIMP CStressNotifySink::Visual (QWORD qwTime, WCHAR wc1, WCHAR wc2, DWORD dwFlags, PTTSMOUTH pMouth)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ITTSNotifySink::Visual\r\n");
   m_pStress->Output (szTemp);
   return NOERROR;
}




/*****************************************************************************
CStressBufNotifySink - TTS buffer notify sink
*/
STDMETHODIMP CStressBufNotifySink::QueryInterface (REFIID riid, LPVOID *ppObj)
{
   *ppObj = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITTSBufNotifySinkW)) {
      *ppObj = (LPVOID) this;
      return NOERROR;
   }

   // otherwise, cant find

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CStressBufNotifySink::AddRef (void)
{
   return 1;
}

STDMETHODIMP_ (ULONG) CStressBufNotifySink::Release (void)
{
   return 1;
}

STDMETHODIMP CStressBufNotifySink::BookMark (QWORD qwTime, DWORD dwMark)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ITTSBufNotifySink::BookMark %d\r\n", dwMark);
   m_pStress->Output (szTemp);
   return NOERROR;
}


STDMETHODIMP CStressBufNotifySink::TextDataDone (QWORD qwTime, DWORD dwMark)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ITTSBufNotifySink::TextDataDone\r\n");
   m_pStress->Output (szTemp);
   return NOERROR;
}

STDMETHODIMP CStressBufNotifySink::TextDataStarted (QWORD qwTime)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ITTSBufNotifySink::TextDataStarted\r\n");
   m_pStress->Output (szTemp);
   return NOERROR;
}

STDMETHODIMP CStressBufNotifySink::WordPosition (QWORD qwTime, DWORD dwVal)
{
   m_pStress->StressOnCallback();

   char  szTemp[128];
   wsprintf (szTemp, "ITTSBufNotifySink::WordPosition\r\n");
   m_pStress->Output (szTemp);
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
   // Do nothing

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
      StressEngine();
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
	DWORD	dwSwitch;
	
   dwSwitch = Random(26);

	if ( !gaEngineCases[dwSwitch].fRun ) {
		return;
	}
   
   // Encourage text-data
   if (!Random(3))
      dwSwitch = 16; // textdata

   if (!m_pMode)
      return;

	switch (dwSwitch) {
   case 0: // pitchget
      {
         WORD  wVal;
         hRes = m_pMode->PitchGet(
            OnlyIfVeryDifficult() ? NULL : &wVal);
         Output ("TTSMode:PitchGet");
         Output (hRes);
         break;
      }
   case 1: // pitchset
      {
         hRes = m_pMode->PitchSet((WORD) rand());
         Output ("TTSMode:PitchSet");
         Output (hRes);
         break;
      }
   case 2: // realtimeget
      {
         hRes = m_pMode->RealTimeGet(
            OnlyIfVeryDifficult() ? NULL : &dwVal);
         Output ("TTSMode:RealTimeGet");
         Output (hRes);
         break;
      }
   case 3: // realtimeset
      {
         if (OnlyIfDifficult())
            dwVal = Random(65536);
         else
            dwVal = Random(100);
         hRes = m_pMode->RealTimeSet (dwVal);
         Output ("TTSMode:RealTimeSet");
         Output (hRes);
         break;
      }
   case 4: // speedget
      {
         hRes = m_pMode->SpeedGet(
            OnlyIfVeryDifficult() ? NULL : &dwVal);
         Output ("TTSMode:SpeedGet");
         Output (hRes);
         break;
      }
   case 5: // speedset
      {
         if (OnlyIfDifficult())
            dwVal = Random(65536);
         else
            dwVal = Random(300);
         hRes = m_pMode->SpeedSet (dwVal);
         Output ("TTSMode:SpeedSet");
         Output (hRes);
         break;
      }
   case 6: // volumeget
      {
         hRes = m_pMode->VolumeGet(
            OnlyIfVeryDifficult() ? NULL : &dwVal);
         Output ("TTSMode:VolumeGet");
         Output (hRes);
         break;
      }
   case 7: // volumeset
      {
         hRes = m_pMode->VolumeSet (Random(65536));
         Output ("TTSMode:VolumeSet");
         Output (hRes);
         break;
      }
   case 8: // AudioPause
      {
         // make this fairly rare
         if (Random(10))
            return;

         hRes = m_pMode->AudioPause ();
         Output ("TTSMode:AudioPause");
         Output (hRes);
         break;
      }
   case 9: // AudioReset
      {
         // make this fairly rare
         if (Random(10))
            return;

         hRes = m_pMode->AudioReset ();
         Output ("TTSMode:AudioReset");
         Output (hRes);
         break;
      }
   case 10: // AudioResume
      {
         hRes = m_pMode->AudioResume ();
         Output ("TTSMode:AudioResume");
         Output (hRes);
         break;
      }
   case 11: // Inject
      {
         if (OnlyIfDifficult()) {
            WCHAR *pTemp;
            pTemp = GenerateRandomText();
            hRes = m_pMode->Inject (pTemp);
            if (pTemp)
               free (pTemp);
         }
         else {
            WCHAR  szTemp[128];
            ProduceTag (szTemp, gdwDifficulty);

            hRes = m_pMode->Inject (szTemp);
         }
         Output ("TTSMode:Inject");
         Output (hRes);
         break;
      }
   case 12: // modeget
      {
         TTSMODEINFOW  mi;
         hRes = m_pMode->ModeGet (
            OnlyIfVeryDifficult() ? NULL : &mi);
         Output ("TTSMode:ModeGet");
         Output (hRes);
         break;
      }
   case 13: // phoneme
      {
         VOICECHARSET   vc;
         switch (Random(3)) {
         case 0:
            vc = CHARSET_ENGINEPHONETIC;
            break;
         case 1:
            vc = CHARSET_IPAPHONETIC;
            break;
         case 2:
            vc = CHARSET_TEXT;
            break;
         }

         DWORD dwFlags;
         dwFlags = Random(OnlyIfDifficult() ? 65536 : 2);

         SDATA sText;
         sText.pData = OnlyIfVeryDifficult() ? NULL : GenerateRandomText();
         sText.dwSize = sText.pData ? (wcslen((WCHAR*) sText.pData)+1)*2 : 0;
         if (sText.dwSize && OnlyIfVeryDifficult())
            sText.dwSize = Random(sText.dwSize);

         SDATA sRet;
         sRet.pData = NULL;

         // call the function
         hRes = m_pMode->Phoneme (vc, dwFlags, sText,
            OnlyIfVeryDifficult() ? NULL : &sRet);
         if (sText.pData)
            free (sText.pData);
         if (sRet.pData)
            CoTaskMemFree (sRet.pData);
         if (!hRes) {
            m_dwTextSendOut += sText.dwSize;
            char  szTemp[48];
            wsprintf (szTemp, "%d", (int) m_dwTextSendOut);
            SetDlgItemText (m_hWnd, IDC_PHRASES, szTemp);
         }

         Output ("TTSMode:Phoneme");
         Output (hRes);
         break;
      }
   case 14: // posnget
      {
         QWORD qwVal;
         hRes = m_pMode->PosnGet (
            OnlyIfVeryDifficult() ? NULL : &qwVal);
         Output ("TTSMode:PosnGet");
         Output (hRes);
         break;
      }
   case 15: // register & unregister
      {
         DWORD dwKey;
         hRes = m_pMode->Register (
            OnlyIfVeryDifficult() ? NULL : (PVOID) m_pNS,
            OnlyIfDifficult() ? IID_IAudioFile : IID_ITTSNotifySink,
            OnlyIfVeryDifficult() ? NULL : &dwKey);
         Output ("TTSMode:Register");
         Output (hRes);
         if (hRes)
            break;
         hRes = m_pMode->UnRegister (dwKey);
         Output ("TTSMode:UnRegister");
         Output (hRes);
         break;
      }
   case 16: // textdata
      {
         VOICECHARSET   vc;
         switch (Random(3)) {
         case 0:
            vc = CHARSET_ENGINEPHONETIC;
            break;
         case 1:
            vc = CHARSET_IPAPHONETIC;
            break;
         case 2:
            vc = CHARSET_TEXT;
            break;
         }

         DWORD dwFlags;
         dwFlags = Random(OnlyIfDifficult() ? 65536 : 2);

         SDATA sText;
         sText.pData = OnlyIfVeryDifficult() ? NULL : GenerateRandomText();
         sText.dwSize = sText.pData ? (wcslen((WCHAR*) sText.pData)+1)*2 : 0;
         if (sText.dwSize && OnlyIfVeryDifficult())
            sText.dwSize = Random(sText.dwSize);

         // call the function
         hRes = m_pMode->TextData (vc, dwFlags, sText,
            OnlyIfDifficult() ? NULL : m_pBNS,
            OnlyIfVeryDifficult() ? IID_IAudioFile : IID_ITTSBufNotifySinkW);
         if (sText.pData)
            free (sText.pData);
         if (!hRes) {
            m_dwTextSendOut += sText.dwSize;
            char  szTemp[48];
            wsprintf (szTemp, "%d", (int) m_dwTextSendOut);
            SetDlgItemText (m_hWnd, IDC_PHRASES, szTemp);
         }

         Output ("TTSMode:TextData");
         Output (hRes);
         break;
      }
   case 17: // tofiletime
      {
         FILETIME ft;
         DWORD dw[2];
         dw[0] = (DWORD) rand();
         dw[1] = (DWORD) rand();
         hRes = m_pMode->ToFileTime ((PQWORD) dw,
            OnlyIfVeryDifficult () ? NULL : &ft);
         Output ("TTSMode:ToFileTime");
         Output (hRes);
         break;
      }
   case 18: // about dialog
      {
         WCHAR *psz = GenerateRandomText();
         hRes = m_pMode->AboutDlg (NULL, OnlyIfDifficult ? psz : NULL);
         if (psz)
            free (psz);
         Output ("TTSMode:AboutDlg");
         Output (hRes);
         break;
      }
   case 19: // general dialog
      {
         WCHAR *psz = GenerateRandomText();
         hRes = m_pMode->GeneralDlg (NULL, OnlyIfDifficult ? psz : NULL);
         if (psz)
            free (psz);
         Output ("TTSMode:GeneralDlg");
         Output (hRes);
         break;
      }
   case 20: // lexicon dialog
      {
         WCHAR *psz = GenerateRandomText();
         hRes = m_pMode->LexiconDlg (NULL, OnlyIfDifficult ? psz : NULL);
         if (psz)
            free (psz);
         Output ("TTSMode:LexiconDlg");
         Output (hRes);
         break;
      }
   case 21: // translate dialog
      {
         WCHAR *psz = GenerateRandomText();
         hRes = m_pMode->TranslateDlg (NULL, OnlyIfDifficult ? psz : NULL);
         if (psz)
            free (psz);
         Output ("TTSMode:TranslateDlg");
         Output (hRes);
         break;
      }
   case 22: // ilexpronounce - add
      {
         if (gdwDifficulty < 2)
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

         Output ("TTSMode:Add");
         Output (hRes);
         break;
      }
   case 23: // ilexpronounce - get
      {
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

         Output ("TTSMode:Get");
         Output (hRes);

         break;
      }
   case 24: // ilexpronounce - remove
      {
         if (gdwDifficulty < 2)
            return;  // don't test

         WCHAR *pszText;
         pszText = GenerateRandomText();

         hRes = m_pMode->Remove (
            Random(2) ? NULL : pszText,
            (WORD) Random(10));
         if (pszText)
            free (pszText);

         Output ("TTSMode:Remove");
         Output (hRes);

         break;
      }
   case 25: // wipe out wave file
      {
         // add the file
         if (!m_pIWaveAudio)
            return;

         hRes = m_pIWaveAudio->Flush();
         hRes = m_pIWaveAudio->Set (m_szWave, 1);
         break;
      }
	}

   MadeCall();
}










