/******************************************************************************
*       This module contains functions which handle interaction with the Voice 
*       Command and Voice Messages interface.
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#ifndef _SPEECH_
#define INITGUID
#define INC_OLE2
#endif  // _SPEECH_

#include "srclk.h"
#include "vcmd.h"

BOOL ReleaseInterfaces( const char *szMsg );
long GetCityTimeBias( char *pCity );

char *pszVoiceQuestion="Speech is turned off, you can turn this on using Feedback.Exe or Microsoft Voice.\nPress Yes after you've turned speech on, No to exit, or Cancel to continue.";

const int gnCmds = 6;	// the number of available commands for recognition
char *gszCmds=
"Toggle view\nClock Options\nS.R. Options\nWhat is the date\nWhat time is it\nWhat time is it in <CITYNAME>";
CityList_t *gCityList;
int gnCities=0;

PCIVCmdNotifySink gpVCmdNotifySink = NULL;
PCIVTxtNotifySink gpVTxtNotifySink = NULL;
PIVOICECMD        gpIVoiceCommand  = NULL;
PIVCMDDIALOGS     gpIVCmdDialogs   = NULL;
PIVCMDMENU        gpIVCmdMenu      = NULL;
PIVCMDATTRIBUTES  gpIVCmdAttr      = NULL;
PIVOICETEXT		  gpIVTxt          = NULL;
PIVTXTATTRIBUTES  gpIVTxtAttr      = NULL;
UINT              uTimerID = 0;
DWORD             gdwShowReEnable;

/*
 Invoke the Speech Recognition general dialog.
*/
void ActivateSRdialog()
{
	HRESULT hRes = gpIVCmdDialogs->GeneralDlg( ghWndMain, "Voice Clock" );

   if (hRes)
      MessageBox (ghWndMain, "The speech recognition engine you're using doesn't support this feature.",
      "Clock", MB_OK);
}

/*
 Begin OLE and create the interface pointers and an empty menu object.

returns
   BOOL - TRUE if is succedes, FALSE if error.
*/
BOOL BeginOLE()
{
	HRESULT  hRes;
	VCMDNAME VcmdName;
	LANGUAGE Language;

	SetMessageQueue(96);

	if( FAILED(CoInitialize(NULL)) )
		return ReleaseInterfaces( "CoInitialize() failed." );

	// Create the voice commands object
	if(CoCreateInstance(CLSID_VCmd, NULL, CLSCTX_LOCAL_SERVER, IID_IVoiceCmd, (LPVOID *)&gpIVoiceCommand) != S_OK)
		return ReleaseInterfaces("Error in CoCreateInstance() for VCmd interface." );

	// Get the dialogs interface pointer...
	hRes = gpIVoiceCommand->QueryInterface( IID_IVCmdDialogs, (LPVOID FAR *)&gpIVCmdDialogs );
	if( FAILED(hRes) ) return ReleaseInterfaces( "QueryInterface() failed to obtain VCmd Dialogs interface." );

	// Get the attributes interface pointer...
	hRes = gpIVoiceCommand->QueryInterface( IID_IVCmdAttributes, (LPVOID FAR *)&gpIVCmdAttr );
	if( FAILED(hRes) ) return ReleaseInterfaces( "QueryInterface() failed to obtain VCmd Attributes interface." );

	// Create/Register VCmd notification sink...
	gpVCmdNotifySink = new CIVCmdNotifySink;
	if( gpVCmdNotifySink == NULL )
		return ReleaseInterfaces( "Out of memory for Voice notification object." );

	hRes = gpIVoiceCommand->Register( "", gpVCmdNotifySink, IID_IVCmdNotifySink, VCMDRF_ALLMESSAGES, NULL );
	if( FAILED(hRes) )
		return ReleaseInterfaces( "Failed to Register() Voice notification sink." );

	// Initialize command menu set variables...
	lstrcpy(VcmdName.szApplication, "SR Clock Demo");
	lstrcpy(VcmdName.szState, "Main");
	Language.LanguageID = LANG_ENGLISH;
	lstrcpy (Language.szDialect, "US English");

	// Create an empty command menu set...
	hRes = gpIVoiceCommand->MenuCreate( &VcmdName, &Language, VCMDMC_CREATE_TEMP, &gpIVCmdMenu );
	if( FAILED(hRes) ) return ReleaseInterfaces( "Failed to create a voice command set with MenuCreate()" );

	// Create a voice text object...
	if(CoCreateInstance(CLSID_VTxt, NULL, CLSCTX_LOCAL_SERVER, IID_IVoiceText, (LPVOID *)&gpIVTxt) != S_OK)
		return ReleaseInterfaces("Error in CoCreateInstance() for Text-To-Speech interface." );

	// Get the attributes interface pointer...
	hRes = gpIVTxt->QueryInterface( IID_IVTxtAttributes, (LPVOID FAR *)&gpIVTxtAttr );
	if( FAILED(hRes) ) return ReleaseInterfaces( "QueryInterface() failed to obtain VTxt Attributes interface." );

	// Create/Register VTxt notification sink...
	gpVTxtNotifySink = new CIVTxtNotifySink;
	if( gpVTxtNotifySink == NULL )
		return ReleaseInterfaces( "Out of memory for VTxt notification object." );

	hRes = gpIVTxt->Register( NULL, "SRClock", gpVTxtNotifySink, IID_IVTxtNotifySink, VTXTF_ALLMESSAGES, NULL);
	if( FAILED(hRes) )
		return ReleaseInterfaces( "Failed to Register() VTxt notification sink." );

	return TRUE;
}

/*
 Closes up OLE and frees everything else.
*/
BOOL EndOLE()
{
	// free our list of cities/time-zone bias data...
	free( gCityList );

	ReleaseInterfaces( "" );

	// Free up all of OLE
	CoUninitialize();

	return TRUE;
}

/*
 Release the Voice command interfaces.
*/
BOOL ReleaseInterfaces( const char *szMsg )
{
	// if a msg is specified, display it...
	if( *szMsg ) MessageBox( NULL, szMsg, NULL, MB_OK);

	if( gpIVTxt         )
      gpIVTxt->Release();
	if( gpIVTxtAttr         )
      gpIVTxtAttr->Release();
	if( gpIVCmdMenu     )
      gpIVCmdMenu->Release();
	if( gpIVCmdAttr     )
      gpIVCmdAttr->Release();
	if( gpIVCmdDialogs  )
      gpIVCmdDialogs->Release();
	if( gpIVoiceCommand )
      gpIVoiceCommand->Release();
	gpIVTxt          = NULL;
	gpIVCmdMenu      = NULL;
	gpIVCmdAttr      = NULL;
	gpIVCmdDialogs   = NULL;
	gpIVoiceCommand  = NULL;

	return FALSE;
}  

/*
 Scan memory and find the next command.

inputs
   CHAR     *pszMemory - Memory to start looking at
   PCHAR    *pBegin - Filled in with a pointer to the
         beginning of the command string.
   DWORD    *pdwSize - Filled in with the number of bytes in
         the string (excluding any NULL termination)
returns
   CHAR * - The next place that NextCommand should be called from,
         or NULL if no command string was found.
*/
CHAR * NextCommand (CHAR *pszMemory, PCHAR *pBegin, DWORD *pdwSize)
{
	DWORD i;

	for( ;; )
	{
		// try to find a non-newline
		while ((*pszMemory == '\n') || (*pszMemory == '\r'))
		{
			if( *pszMemory == '\0' ) return NULL;
			pszMemory++;
		}

		// Try to find a new-line
		for (i = 0; (pszMemory[i] != '\n') && (pszMemory[i] != '\r') && (pszMemory[i] != '\0'); i++);
		if( !i )
		{
			if( !pszMemory[i] ) return NULL;   // end
			pszMemory++;
			continue;   // try again
		}

		// Else, we've found a string
		*pBegin = pszMemory;
		*pdwSize = i;
		return pszMemory + i;
	}
}

/*
 Take a block of memory containing command strings and convert it into a list of VCMDCOMMAND
 structures.

inputs
   pszMemory       - NULL terminated string. Commands are separated by \n or \r.
   pData           - This is filled in with a pointer to memory and size for the vcmdcommand
                     structure. The memory must be freed by the caller with free().   
   *pdwNumCommands - Filled with the number of commands
*/
BOOL GetCommands(char *pszMemory, PSDATA pData, DWORD *pdwNumCommands)
{
	PSTR pTemp;
	DWORD dwTotal, dwSize, dwSizeDesc, dwSizeCat;
	DWORD dwSizeCmd;
	PVCMDCOMMAND pCmd, pCmdNew;
	CHAR    *pszBegin;
	DWORD   dwCmdSize;
	DWORD   dwCmds = 0;
	DWORD   dwCount = 1;
	char    szCat[] = "Main";

	dwTotal = dwSize = 0;

	pTemp = (PSTR)malloc(0);
	if( !pTemp ) return FALSE;

	pCmd = (PVCMDCOMMAND)pTemp;
	for( ;; )
	{
		pszMemory = NextCommand (pszMemory, &pszBegin, &dwCmdSize);
		if( !pszMemory ) break;   // no more

		// size of header
		dwSize = sizeof(VCMDCOMMAND);

		// get command length
		dwSizeCmd = (dwCmdSize + 1);

		// doubleword align
		dwSizeCmd += 3;
		dwSizeCmd &= (~3);
		dwSize += dwSizeCmd;

		// get description length
		dwSizeDesc = (dwCmdSize + 1);

		// doubleword align
		dwSizeDesc += 3;
		dwSizeDesc &= (~3);
		dwSize += dwSizeDesc;

		// get category length
		dwSizeCat = lstrlen(szCat) + 1;

		// doubleword align
		dwSizeCat += 3;
		dwSizeCat &= (~3);
		dwSize += dwSizeCat;

		// action indicator
		dwSize += sizeof(DWORD);

		// accumulate total size
		dwTotal += dwSize;

		// reallocate enough memory to hold this command
		pTemp = (PSTR)realloc((PVOID)pCmd, dwTotal);

		// fill in the new command
		pCmd = (PVCMDCOMMAND)pTemp;
		pTemp += (dwTotal-dwSize);
		pCmdNew = (PVCMDCOMMAND)pTemp;
		memset (pCmdNew, 0, dwSize);

		pCmdNew->dwSize = dwSize;
		pCmdNew->dwFlags = 0;
		pCmdNew->dwAction = (DWORD)(pCmdNew->abData-(PBYTE)pTemp);
		pCmdNew->dwActionSize = sizeof(DWORD);

		// point past header to begin of data
		pTemp += (pCmdNew->abData-(PBYTE)pTemp);

		// action index
		*(DWORD *)pTemp = dwCount++;
		pTemp += sizeof(DWORD);

		// command
		pCmdNew->dwCommand = (DWORD)((PBYTE)pTemp - (PBYTE)pCmdNew);
		strncpy(pTemp, pszBegin, dwCmdSize);
		pTemp += dwSizeCmd;

		// description
		pCmdNew->dwDescription = (DWORD)((PBYTE)pTemp - (PBYTE)pCmdNew);
		strncpy(pTemp, pszBegin, dwCmdSize);
		pTemp += dwSizeDesc;

		// category
		pCmdNew->dwCategory = (DWORD)((PBYTE)pTemp - (PBYTE)pCmdNew);
		strcpy(pTemp, szCat);

		// point to next pointers
		dwCmds++;
	}

	pData->pData = (PVOID)pCmd;
	pData->dwSize = dwTotal;
	*pdwNumCommands = dwCmds;
	return TRUE;
}

/*
 Read the clock timezone info file and build the recognition command list from that.

inputs
   char     *pszState - State name
returns
   HRESULT - error
*/
HRESULT InitState (char *pszCmdFileName)
{
	HRESULT  hRes;
	SDATA    data;
	DWORD    dwNum, dwStart;
	char     *pszCities, *pPos;
	char     *p, buf[MAXSTRLEN];
	FILE     *fp;
	DWORD    dwTotal=0, len;
	int      iState;
   iState = -1;

	// register our speech state messages...
	// this code is here to show you how to register these messages,
	// this app currently does not implement them completely...
	WM_REG_SPEECHSTARTED = RegisterWindowMessage( "WM_SPEECHSTARTED" );
	WM_REG_SPEECHENDED   = RegisterWindowMessage( "WM_SPEECHENDED" );
	if( WM_REG_SPEECHSTARTED == 0 || WM_REG_SPEECHENDED == 0 )
		MessageBox( NULL, "Can't register messages for obtaining the speech state.", "Warning", MB_OK );

	// add the recognizable cmds...
	if( !GetCommands(gszCmds, &data, &dwNum) )
	{
		MessageBox( NULL, "Out of memory during VCMD initialization.", NULL, MB_OK );
		return E_OUTOFMEMORY;
	}
	hRes = gpIVCmdMenu->Add (dwNum, data, &dwStart);
	free (data.pData);
	if( FAILED(hRes) )
	{
		MessageBox( NULL, "Error adding commands to menu.", NULL, MB_OK );
		return hRes;
	}

	// open the city/time-zone data file...
	if( !(fp = fopen( pszCmdFileName, "r" )) )
	{
      // try opening one directory above
      char  szTemp[256];
      wsprintf (szTemp, "..\\%s", pszCmdFileName);

	   if( !(fp = fopen( szTemp, "r" )) )
	   {
		   MessageBox( NULL, "Time zone data file not found (srclk.dat). It needs to be in the same directory as SRCLK.Exe.", NULL, MB_OK );
		   return E_FAIL;
      }
	}

	// allocate memory for the city/time bias list...
	gCityList = (CityList_t *)malloc( 0 );
	gnCities = 0;

	// initialize temporary sdata structure for the ListSet function...
	pszCities = pPos = (char *)malloc( 0 );

	// build the lists based on the data in the file...
	while( fgets( buf, MAXSTRLEN, fp ) )
	{
		// null-terminate the city name...
		p = strchr( buf, ',' );
		*p = 0;

		// add the info to our global list...
		gCityList = (CityList_t *)realloc( (PVOID)gCityList, sizeof(CityList_t)*(gnCities+1) );
		if( gCityList == NULL )
		{
			MessageBox( NULL, "Out of memory during VCMD initialization.", NULL, MB_OK );
			return E_OUTOFMEMORY;
		}

		gCityList[gnCities].lTmBias = atol(p+1) * -60;
		strcpy( gCityList[gnCities].szCity, buf );

		// now add the city name to the temporary SDATA list...
		len = strlen(buf)+1;
		pszCities = (char *)realloc( (PVOID)pszCities, dwTotal+len );
		if( pszCities == NULL )
		{
			MessageBox( NULL, "Out of memory during VCMD initialization.", NULL, MB_OK );
			return E_OUTOFMEMORY;
		}

		strncpy( pszCities+dwTotal, buf, len );
		dwTotal += len;

		gnCities++;
	}

	// close the file...
	fclose( fp );

	// add our list of cities to the VCMD menu...
	data.dwSize = dwTotal;
	data.pData  = pszCities;
	hRes = gpIVCmdMenu->ListSet( "CITYNAME", gnCities, data );
	free (data.pData);
	if( FAILED(hRes) )
	{
		MessageBox( NULL, "Error in ListSet during VCMD initialization.", NULL, MB_OK );
		return hRes;
	}

	// enable the VCMD and VTXT state and activate the menu,
	// note that this affects the entire site which means all voice-aware apps that
	// are running at this point...
	if( iState == 2 )
	{
		// VCMD and VCMD state...
		if( gpIVCmdAttr->EnabledSet(1) || gpIVCmdAttr->AwakeStateSet(1) )
			MessageBox( NULL, "Error enabling TTS during VCMD initialization.", NULL, MB_OK );
		
		// TTS...
		if( gpIVTxtAttr->EnabledSet(1) )
			MessageBox( NULL, "Error enabling TTS during VCMD initialization.", NULL, MB_OK );
	}

	// begin in global recognition state...
	if( ActivateSR( RECOG_GLOBAL ) )
		MessageBox( NULL, "Error activating the SR command menu.", NULL, MB_OK );

	return hRes;
}

/*
 Activate the recognition mode for off, recognize in window, or recognize globally.
*/
HRESULT ActivateSR( int Flag )
{
	HRESULT hRes;

	// turn it off...
	hRes = gpIVCmdMenu->Deactivate();

	// now turn it on if user wants to recognize no matter what window is active...
	if( Flag == RECOG_GLOBAL )
		hRes = gpIVCmdMenu->Activate( NULL, 0 );

	// or if the user just wants it to recognize while it's window is active...
	else if( Flag == RECOG_WINDOW )
		hRes = gpIVCmdMenu->Activate( ghWndMain, 0 );

	return hRes;
}

/*
 Send the string out to be spoken by the Voice Text interface.
*/
HRESULT SpeakString( char *szSpeak )
{
	HRESULT hRes;

	if( *szSpeak == NULL ) return 0;

	// temporarily disable SR to speak some text.  We'll enable it again in the
	// VTxt notification SpeakingDone() function. Note that if the wave driver
	// being used supports low-priority mode we don't have to do this (nor would
	// we need to implement the IVTxt notification interface)...
	gpIVCmdAttr->EnabledGet( &gdwShowReEnable );
	gpIVCmdAttr->EnabledSet( FALSE );

	hRes = gpIVTxt->Speak( szSpeak, VTXTSP_NORMAL, NULL );

	// if we failed to start tts, then enable sr to be listening again...
	if( FAILED(hRes) && gdwShowReEnable) gpIVCmdAttr->EnabledSet( TRUE );

	return hRes;
}

/**************************************************************************
 *  Voice Command notification objects
 **************************************************************************/

CIVCmdNotifySink::CIVCmdNotifySink (void)
{
    m_dwRefCnt = 0;
}

CIVCmdNotifySink::~CIVCmdNotifySink (void)
{
}

STDMETHODIMP CIVCmdNotifySink::QueryInterface (REFIID riid, LPVOID *ppv)
{
	*ppv = NULL;

	// always return our IUnkown for IID_IUnknown
	if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IVCmdNotifySink)) {
		*ppv = (LPVOID) this;
		AddRef();
		return NOERROR;
	}

	// otherwise, cant find
	return E_NOINTERFACE;
}

STDMETHODIMP_ (ULONG) CIVCmdNotifySink::AddRef (void)
{
	return ++m_dwRefCnt;
}

STDMETHODIMP_(ULONG) CIVCmdNotifySink::Release (void)
{
	if( --m_dwRefCnt == 0 )
	{
		// reference count is 0, delete the object...
		delete this;
		return 0;
	}
	return m_dwRefCnt;
}

/*
 Handle recognition notification.

 Inputs:
 dwID         - Id of this instance of Voice command object.
 pName        - 
 dwFlags      -
 dwActionSize -
 pAction      - The index of the command in the command list (1 based).
 dwNumLists   -
 pszListValue - 
 pszCommand   - 
*/
STDMETHODIMP CIVCmdNotifySink::CommandRecognize(DWORD dwID, PVCMDNAME pName, DWORD dwFlags,
   DWORD dwActionSize, PVOID pAction, DWORD dwNumLists, PTSTR pszListValues, PTSTR pszCommand)
{
	// indicate recognition status to user by changing the window title back to normal...
	SetWindowText( ghWndMain, szWinTitle );

	// write the recognized command to the screen...
	strcpy( srTxt[SRTXT_CMD].szStrOut, pszCommand );
	TextOutSR( NULL, SRTXT_CMD );

	switch( *(DWORD *)pAction )
	{
		// user said "toggle view" (first cmd), toggle it and return...
		case 1:
			ToggleView( ghWndMain );
			break;

		// user wants the clock options...
		case 2:
			PostMessage( ghWndMain, WM_COMMAND, ID_CLKOPTBUT, NULL );
			break;

		// user wants the SR options...
		case 3:
			PostMessage( ghWndMain, WM_COMMAND, ID_SROPTBUT, NULL );
			break;

		// user wants today's date...
		case 4:
			PostMessage( ghWndMain, WM_USERSPEAK, TRUE, (LPARAM)TimeZoneInfo.Bias );
			break;

		// user wants the local time...
		case 5:
			PostMessage( ghWndMain, WM_USERSPEAK, FALSE, (LPARAM)TimeZoneInfo.Bias );
			break;

		// user wants the time somewhere else.  Do a lookup on the city
		// to obtain the timezone bias value...
		default:
			long lTmBias = GetCityTimeBias(pszListValues);
			PostMessage( ghWndMain, WM_USERSPEAK, FALSE, (LPARAM)lTmBias );
			break;
	}

	return NOERROR;
}

long GetCityTimeBias( char *pCity )
{
	long lTmBias = TimeZoneInfo.Bias;

	for( int i=0; i<gnCities; i++ )
	{
		if( !strcmp( pCity, gCityList[i].szCity ) ) lTmBias = gCityList[i].lTmBias;
	}

	return lTmBias;
}

/*
 Handle notifications which are for some other command not in out list.
*/
STDMETHODIMP CIVCmdNotifySink::CommandOther(PVCMDNAME pName, PTSTR pszCommand)
{
	// put a '??' in the window title to tell the user we didn't understand the command...
	char buf[30];
	wsprintf( buf, "%s ?", szWinTitle );
	SetWindowText( ghWndMain, buf );

	strcpy( srTxt[SRTXT_CMD].szStrOut, "??" );
	TextOutSR( NULL, SRTXT_CMD );

	return NOERROR;
}

/*
 Handle notifiaction sent when recognition begins.
*/
STDMETHODIMP CIVCmdNotifySink::CommandStart(void)
{
	return NOERROR;
}

/*
 Handle notification sent when the menu is activated/deactivated.
*/
STDMETHODIMP CIVCmdNotifySink::MenuActivate(PVCMDNAME pName, BOOL bActivate)
{
	return NOERROR;
}

/*
 Handle notification sent when the attribute is changed.
*/
STDMETHODIMP CIVCmdNotifySink::AttribChanged(DWORD dwAttribute)
{
	return NOERROR;
}

/*
 Handle notification sent when the recognizer detects the beginning of an utterance.
*/
STDMETHODIMP CIVCmdNotifySink::UtteranceBegin(void)
{
	// indicate the status to the user by changing the window title...
	SetWindowText( ghWndMain, "Utter..." );

	// fill string for widw view mode...
	strcpy( srTxt[SRTXT_UTTER].szStrOut, "Begin" );
	TextOutSR( NULL, SRTXT_UTTER );
	
	return NOERROR;
}

/*
 Handle notification sent when the recognizer detects the end of an utterance.
*/
STDMETHODIMP CIVCmdNotifySink::UtteranceEnd()
{
	// if we're not in wide mode, indicate the status to the user
	// by changing the window title...
	SetWindowText( ghWndMain, "Utter End" );

	strcpy( srTxt[SRTXT_UTTER].szStrOut, "End" );
	TextOutSR( NULL, SRTXT_UTTER );

	return NOERROR;
}

/*
 Handle notification sent to indicate amplitude of incoming audio data.
*/
STDMETHODIMP CIVCmdNotifySink::VUMeter(WORD wLevel)
{
	// paint the view meter abse3d on data from notification...
	if( bVMeterFlag )
	{
		PaintVMeter( wLevel );
	}

	return NOERROR;
}

/*
 Handle notification sent when the recognizer detects interference
*/
STDMETHODIMP CIVCmdNotifySink::Interference(DWORD dwType)
{
	return NOERROR;
}

/**************************************************************************
 *  Voice Message notification object
 **************************************************************************/

/*
The only reason we need to implement this notification interface is because
we want to know when we're done speaking so we can enable speech recognition
mode.  This is because we probably have a half-duplex wave device which doesn't
allow SR and TTS simultaneously.  If we had a full-duplex wave device we wouldn't
need to do this.
*/
CIVTxtNotifySink::CIVTxtNotifySink ()
{
    m_dwRefCnt = 0;
}

CIVTxtNotifySink::~CIVTxtNotifySink (void)
{
}

STDMETHODIMP CIVTxtNotifySink::QueryInterface (REFIID riid, LPVOID *ppv)
{
	*ppv = NULL;

	// always return our IUnkown for IID_IUnknown
	if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IVTxtNotifySink))
	{
		*ppv = (LPVOID) this;
		AddRef();
		return NOERROR;
	}

	// otherwise, cant find
	return E_NOINTERFACE;
}

STDMETHODIMP_ (ULONG) CIVTxtNotifySink::AddRef (void)
{
	return ++m_dwRefCnt;
}

STDMETHODIMP_(ULONG) CIVTxtNotifySink::Release (void)
{
	if( --m_dwRefCnt == 0 )
	{
		// reference count is 0, delete the object...
		delete this;
		return 0;
	}
    return m_dwRefCnt;
}

STDMETHODIMP CIVTxtNotifySink::AttribChanged(DWORD dwAttribute)
{
	return NOERROR;
}

STDMETHODIMP CIVTxtNotifySink::SpeakingStarted(void)
{
	return NOERROR;
}

STDMETHODIMP CIVTxtNotifySink::SpeakingDone(void)
{
	// Enable speech recognition again.  Note that if the wave driver being used
	// supports low-priority mode or full-duplex we wouldn't have to do this...
   if (gdwShowReEnable)
	   gpIVCmdAttr->EnabledSet( TRUE );

	return NOERROR;
}

STDMETHODIMP CIVTxtNotifySink::Visual(WCHAR cIPAPhoneme, char cEnginePhoneme, DWORD dwFlags, PTTSMOUTH pTTSMouth)
{
	return NOERROR;
}

STDMETHODIMP CIVTxtNotifySink::Speak(PSTR pszMessage, PSTR pszApplication, DWORD dwTyep)
{
	return NOERROR;
}

