#include "spchwrap.h"
#include "link.h"
#include "process.h"

#define     ENGINETEXT      512

/***********************************************
 Global variables
 */
static char     gszClass[] = "Telephone Line";
static char     gszExtension[] = ".stl";
static char     gszLogNumber[64];
static char     gszPath[512];
static char     gszSREngine[ENGINETEXT];
static char     gszTTSEngine[ENGINETEXT];
static char     gszDefaultPath[] = "c:\\program files\\microsoft speech sdk";
static LONG     gdwNumber;
static BOOL     gfUseLogging = FALSE, gfFirstTime = TRUE;
#ifdef RHETOREX
static BOOL     gfRhetorex = FALSE; // set to true if using full duplex rhetorex driver, which needs special work
#endif // RHETOREX

// used for enumerating telephony engines from the registry
#define VALUENAMELEN    256

typedef struct {
    char    szName[VALUENAMELEN];
    GUID    gModeID;
} ENGINEINFO, *PENGINEINFO;


HRESULT GetLineInfo (DWORD, LPLINEINFO, HLINEAPP);
void __cdecl LineThread(LPVOID);

static HANDLE  ghThreadInitEvent;

/**********************************************************************
 These numbers have to remain the same.
 They match the numbers used in spchtel.dll where the resources reside.
 */
#define IDD_PCCALLDIALOG                104
#define IDD_TAPICALLDIALOG              105
#define IDD_LOGGINGDIALOG               107
#define IDD_CHOOSEENGINEDIALOG          108
#define IDC_EDITLOGFILE                 1002
#define IDC_BUTTON1                     1001
#define IDC_BUTTON2                     1002
#define IDC_BUTTON3                     1003
#define IDC_BUTTON4                     1004
#define IDC_BUTTON5                     1005
#define IDC_BUTTON6                     1006
#define IDC_BUTTON7                     1007
#define IDC_BUTTON8                     1008
#define IDC_BUTTON9                     1009
#define IDC_BUTTON10                    1010
#define IDC_BUTTON11                    1011
#define IDC_BUTTON12                    1012
#define IDC_CONNECT                     1013
#define IDC_LINES                       1014
#define IDC_STATUS                      1015
#define IDC_HANGUP                      1015
#define IDC_USELOGGING                  1016
#define IDC_LOGFILENUMBER               1017
#define IDC_LOGGINGCHECK                1018
#define IDC_SRENGINE                    1019
#define IDC_TTSENGINE                   1020




/************************************************************************
    There can only be one TAPI object because of the callback. When we
    initialize TAPI we pass a callback function, which uses the following
    global variable pointer to pass on the callback.
 */
static PCTELMAINTAPI14 gpObj = NULL;

/*******************************************************************
 TAPI callback function
 */
static void CALLBACK LineCallback(DWORD hDevice, DWORD dwMsg, DWORD dwInst,
                                  DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
{
    // pass it on to the TAPI object if one there
    if (gpObj)
        gpObj->Callback(hDevice, dwMsg, dwInst, dwParam1, dwParam2, dwParam3);
}


/********************************************************************
 LoggingDlgProc - This dialog is for getting information about logging
                  such as the directory where to save log files, etc.
 */
BOOL CALLBACK LoggingDlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    static PCTELMAIN pTMObj = NULL;

    switch( msg ) {
        case WM_INITDIALOG:
            pTMObj = (PCTELMAINPC) lParam;
            SetWindowText(GetDlgItem(hwnd, IDC_EDITLOGFILE), gszPath);
            wsprintf(gszLogNumber, "%d", gdwNumber);
            SetWindowText(GetDlgItem(hwnd, IDC_LOGFILENUMBER), gszLogNumber);
            CheckDlgButton(hwnd, IDC_LOGGINGCHECK, 
                           gfUseLogging ? BST_CHECKED : BST_UNCHECKED);
            if (!gfUseLogging) {
                EnableWindow(GetDlgItem(hwnd, IDC_EDITLOGFILE), FALSE);
                EnableWindow(GetDlgItem(hwnd, IDC_LOGFILENUMBER), FALSE);
            }
            return FALSE;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDCANCEL:
                    // gszPath[0] = 0;
                    EndDialog(hwnd, IDCANCEL);
                    return TRUE;

                case IDOK:
                    gfUseLogging = IsDlgButtonChecked(hwnd, IDC_LOGGINGCHECK) &
                                   BST_CHECKED;

                    GetWindowText(GetDlgItem(hwnd, IDC_EDITLOGFILE),
                                  gszPath, sizeof(gszPath));

                    GetWindowText(GetDlgItem(hwnd, IDC_LOGFILENUMBER),
                                  gszLogNumber, sizeof(gszLogNumber));
                    if (gszLogNumber[0])
                        gdwNumber = atol(gszLogNumber);

                    EndDialog(hwnd, IDOK);
                    return TRUE;

                case IDC_LOGGINGCHECK:
                    if (IsDlgButtonChecked(hwnd, IDC_LOGGINGCHECK) & BST_CHECKED) {
                        EnableWindow(GetDlgItem(hwnd, IDC_EDITLOGFILE), TRUE);
                        EnableWindow(GetDlgItem(hwnd, IDC_LOGFILENUMBER), TRUE);
                    }
                    else {
                        EnableWindow(GetDlgItem(hwnd, IDC_EDITLOGFILE), FALSE);
                        EnableWindow(GetDlgItem(hwnd, IDC_LOGFILENUMBER), FALSE);
                    }

                    return TRUE;
            }
            return FALSE;
    }

    return FALSE;
}



/*******************************************************************
 EnableButtons - Enables and disables buttons in the PC phone call
                 emulator.
 */
void EnableButtons(HWND hwnd, BOOL fEnable)
{
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON2), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON3), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON4), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON5), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON6), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON7), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON8), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON9), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON10), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON11), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON12), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_HANGUP), fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_CONNECT), !fEnable);
    EnableWindow(GetDlgItem(hwnd, IDC_USELOGGING), !fEnable);
}


/********************************************************************
 PCDlgProc - PC call dialog procedure. This dialog emulates a telephone
             call on the PC (using the PC speakers and microphone). Used
             for testing purposes.
 */
BOOL CALLBACK PCDlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    static PCTELMAIN pPCObj = NULL;

    switch( msg ) {
        case WM_INITDIALOG:
            pPCObj = (PCTELMAINPC) lParam;
            pPCObj->UseLogging();
            return FALSE;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDCANCEL:
                    // abort call and end dialog
                    if (pPCObj)
                        pPCObj->Abort();

                    EndDialog(hwnd, IDCANCEL);
                    return TRUE;

                case IDC_HANGUP:
                    if (pPCObj)
                        pPCObj->Abort();

                    return TRUE;

                case IDC_CONNECT:
                    if (pPCObj) {
                        EnableButtons(hwnd, TRUE);
                        pPCObj->Callback();
                        EnableButtons(hwnd, FALSE);
                    }

                    return TRUE;

                case IDC_USELOGGING:
                    // user has requested the logging dialog
                    if (pPCObj) {
                        // pPCObj->UseLogging();
                        pPCObj->CallLoggingDialog(hwnd);
                    }

                    return TRUE;

                case IDC_BUTTON1:
                case IDC_BUTTON2:
                case IDC_BUTTON3:
                case IDC_BUTTON4:
                case IDC_BUTTON5:
                case IDC_BUTTON6:
                case IDC_BUTTON7:
                case IDC_BUTTON8:
                case IDC_BUTTON9:
                case IDC_BUTTON10:
                case IDC_BUTTON11:
                case IDC_BUTTON12:
                    if (pPCObj) {
                        pPCObj->GotDTMF(LOWORD(wParam) - IDC_BUTTON1);
                    }
                    
                    return TRUE;
            }
            return FALSE;
    }

    return FALSE;
}


/**************************************************************
 UpdateStatus - Updates the status bar for the TAPI dialog
 */
void UpdateStatus(HWND hwnd, DWORD dwCtl, DWORD dwState)
{
    switch (dwState) {
        case CALLSTATE_INITIALIZING:
            SetDlgItemText(hwnd, dwCtl, "Initializing...");
            break;

        case CALLSTATE_CONNECTING:
            SetDlgItemText(hwnd, dwCtl, "Connecting");
            break;

        case CALLSTATE_INPROGRESS:
            SetDlgItemText(hwnd, dwCtl, "Call in progress");
            break;

        case CALLSTATE_DISCONNECTING:
            SetDlgItemText(hwnd, dwCtl, "Disconnecting");
            break;

        case CALLSTATE_DISABLED:
            SetDlgItemText(hwnd, dwCtl, "Disabled");
            break;

        case CALLSTATE_IDLE:
            SetDlgItemText(hwnd, dwCtl, "Idle");
            break;
        case LINECALLSTATE_RINGBACK:
            SetDlgItemText(hwnd, dwCtl, "Ringing party...");
            break;
        case LINECALLSTATE_BUSY:
            SetDlgItemText(hwnd, dwCtl, "Busy.");
            break;
    }
}

/********************************************************************
 TapiDlgProc - TAPI call dialog procedure. This is the dialog for
               handling real live telephone calls.
 */
BOOL CALLBACK TapiDlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    static  PCTELMAINTAPI14 pTapiObj = NULL;
    DWORD   i, dwNumLines, dwLine, dwState;
    char    szBuf[256];

    switch( msg ) {
        case WM_INITDIALOG:
            pTapiObj = (PCTELMAINTAPI14) lParam;

            ((PCTELMAIN)pTapiObj)->UseLogging();

            // tell the TAPI object about us, and get the number
            // of lines supported
            dwNumLines = 0;
            if (pTapiObj) {
                pTapiObj->SetDlgHWND(hwnd);
                pTapiObj->GetNumLines(&dwNumLines);
            }

            // propogate the drop down combo box with line information
            if (dwNumLines) {
                for (i = 0; i < dwNumLines; i++) {
                    wsprintf(szBuf, "Line %d", i+1);
                    SendDlgItemMessage(hwnd, IDC_LINES, CB_ADDSTRING, 0,
                                       (LONG)szBuf);
                }

                SendDlgItemMessage(hwnd, IDC_LINES, CB_SETCURSEL, 0, 0);
                if (pTapiObj) {
                    pTapiObj->GetState(0, &dwState);
                    UpdateStatus(hwnd, IDC_STATUS, dwState);
                }
            }

            return FALSE;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK:
                case IDCANCEL:
                    // abort call and end dialog
                    if (pTapiObj)
                        pTapiObj->Abort();

                    EndDialog(hwnd, IDCANCEL);
                    return TRUE;

                case IDC_LINES:
                    // get the status and display for line requested
                    if (HIWORD(wParam) == CBN_SELENDOK) {
                        dwLine = SendDlgItemMessage(hwnd, IDC_LINES, CB_GETCURSEL, 0, 0);
                        if (pTapiObj) {
                            pTapiObj->GetState(dwLine, &dwState);
                            UpdateStatus(hwnd, IDC_STATUS, dwState);
                        }

                        return TRUE;
                    }
                    break;

                case IDC_USELOGGING:
                    // user has requested the logging dialog
                    if (pTapiObj) {
                        // ((PCTELMAIN)pTapiObj)->UseLogging();
                        ((PCTELMAIN)pTapiObj)->CallLoggingDialog(hwnd);
                    }

                    return TRUE;
            }

            return FALSE;

        case WM_TELCONTROL_CALLSTATE:
            // The TAPI object is updating the statu of a line.
            // If this is the current line being displayed, we
            // will update the status bar.
            dwLine = SendDlgItemMessage(hwnd, IDC_LINES, CB_GETCURSEL, 0, 0);
            if (dwLine == wParam)
                UpdateStatus(hwnd, IDC_STATUS, (DWORD)lParam);
            return TRUE;
    }

    return FALSE;
}



/********************************************************************
 LineThread - Procedure that is called when creating a line thread.
 */
void __cdecl LineThread(LPVOID pLineObj)
{
    ((PCTELLINETAPI14)pLineObj)->ThreadLoop();
//    return 0;
}


/********************************************************************
 LineWndProc - Line window proc. This the window procedure for the 
               hidden window that gets created for each line supported
 */
LONG CALLBACK LineWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg ) {
        case WM_DESTROY:
            return 0;

        case TM_RUN:
            ((PCTELLINETAPI14) lParam)->Go();
            break;

        case TM_ABORT:
            ((PCTELLINETAPI14) lParam)->AbortControl();
            break;

        case TM_DTMF:
            ((PCTELLINETAPI14) lParam)->SendDTMF(wParam);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}



// TAPI line object
//***************************************************************************
//***************************************************************************
//***************************************************************************
#ifdef STRICT
CTelLineTAPI14::CTelLineTAPI14(PVOID hInst, DWORD dwLine,
                               LPLINEINFO pLineInfo)
#else
CTelLineTAPI14::CTelLineTAPI14(HINSTANCE hInst, DWORD dwLine,
                               LPLINEINFO pLineInfo)
#endif
{
    m_hInst         = (HINSTANCE) hInst;
    m_li            = *pLineInfo;
    m_dwLine        = dwLine;

    m_hThread       = NULL;
    m_dwThreadID    = 0;

    m_pITelInfo     = NULL;
    m_hWnd          = NULL;
    m_hAppWnd       = NULL;
    m_eFreeForAnotherCall = CreateEvent (NULL, TRUE, TRUE, NULL);
}

CTelLineTAPI14::~CTelLineTAPI14()
{
   if (m_eFreeForAnotherCall)
      CloseHandle (m_eFreeForAnotherCall);
}


/********************************************************************
 Init - Gets the wavein and waveout devices from TAPI, and starts a
        new thread for the line
 */
#ifdef STRICT
HRESULT CTelLineTAPI14::Init(PVOID hwnd, PVOID pTelMain, PCCALL pCall)
#else
HRESULT CTelLineTAPI14::Init(HWND hwnd, PVOID pTelMain, PCCALL pCall)
#endif
{
    DWORD dwErr;

    // object for sending control done notifications

    m_pTelMain = pTelMain;
    m_pCall = pCall;


    //
    // the following code is for getting the wave device associated with
    // the line
    //

    // technically, this is not the correct way using TAPI to get the wave
    // devices. The official way to do so is to wait until after a call
    // comes in. However, most telephony cards support the coded method, of getting
    // the wave device from the line, rather than getting it from the call
    // object. If the code acquired the device ID from the call object, this
    // code would fail on most TSPIs. Unforuntaly, getting the wave device
    // from the line precludes ISDN boards from working

    VARSTRING *pvsVarStr = (VARSTRING *) new char[sizeof(VARSTRING) + 64];
    if (!pvsVarStr)
        return E_OUTOFMEMORY;

    pvsVarStr->dwTotalSize = sizeof(VARSTRING) + 64;
    dwErr = lineGetID(m_li.hLine, 0, 0, LINECALLSELECT_LINE, pvsVarStr, "wave/in");
    if (dwErr != 0) {
        lineClose(m_li.hLine);
        return E_FAIL;
    }

    // get wavein
    m_dwWaveIn = (DWORD) *((BYTE *)pvsVarStr+sizeof(VARSTRING));

#ifdef RHETOREX
    // if special rhetorex board then wave in is one better
    // NOTE - Shouldn't assume auatomatically
    if (gfRhetorex)
       m_dwWaveIn++;
#endif // RHETOREX

    pvsVarStr->dwTotalSize = sizeof(VARSTRING) + 64;
    dwErr = lineGetID(m_li.hLine, 0, 0, LINECALLSELECT_LINE, pvsVarStr, "wave/out");
    if (dwErr != 0) {
        lineClose(m_li.hLine);
        return E_FAIL;
    }
    // get waveout
    m_dwWaveOut = (DWORD) *((BYTE *)pvsVarStr+sizeof(VARSTRING));

    // create a handle for it
    m_hThread = CreateEvent(NULL, TRUE, FALSE, NULL);
    // create a thread for this line
    m_dwThreadID = (DWORD) _beginthread (LineThread, 0, this);
//    m_hThread = CreateThread(NULL, 0, LineThread, this, 0, &m_dwThreadID);
    if (!m_dwThreadID) {
        return E_OUTOFMEMORY;
    }


#ifdef _DEBUG
    char szBuf[256];
    wsprintf(szBuf, "Line %d thread created, ID = %x\n", m_dwLine, m_dwThreadID);
    OutputDebugString(szBuf);
#endif

    return NOERROR;
}


/********************************************************************
 ThreadLoop - Main thread loop for the line. It will initialize OLE,
              create the telephony information object and set it up,
              create a hidden window, then fall into a message loop.

              When it receives the TM_STOP message it will fall out
              of the message loop, clean up, and return.
 */
HRESULT CTelLineTAPI14::ThreadLoop()
{
    // init OLE
    CoInitialize(NULL);

    // Create the telephony control information object
    HRESULT	hRes;
    hRes = CoCreateInstance (CLSID_TelInfo, NULL, CLSCTX_INPROC_SERVER,
                             IID_ITelInfo, (void**)&m_pITelInfo);
    if (hRes) {
        MessageBox (NULL,
                    "Can't load the telephony information."
                    "You must have the 4.0 version of the API installed.",
                    NULL, MB_OK);
       SetEvent (m_hThread);

        return E_FAIL;
    }

    // tell the information object what type of object and give the
    // wave devices
    m_pITelInfo->TypeSet(INFOTYPE_TAPI20);
    m_pITelInfo->WaveDeviceSet(m_dwWaveIn, m_dwWaveOut);


    // set the TAPI HLINE
    m_pITelInfo->DWORDSet (TELDWORD_HLINE, (DWORD) m_li.hLine);

    // keep this off because on most telephony cards there's
    // either no beep playede even though they're supposed to,
    // or, there are stability bugs with the beep
    // set a flag indicating that we should use tapi beeps
    // m_pITelInfo->DWORDSet (TELDWORD_UseTAPIBeep, TRUE);

#ifdef RHETOREX
    // If special rhetorex board then full duplex should be on
    // NOTE: Shouldn't assume this automatically
    if (gfRhetorex) {
          m_pITelInfo->DWORDSet (TELDWORD_FullDuplex, TRUE);
          m_pITelInfo->DWORDSet (TELDWORD_EchoCancel, TRUE);
    }
#endif // RHETOREX

    // let the app do it's own QuickCreate if it wants to use certain
    // engines, etc.   The default is at the end of this file
    if (m_pCall->DoQuickCreate(m_pITelInfo, m_hInst) != NOERROR) {
        m_pITelInfo->Release();
        m_pITelInfo = NULL;

        SetEvent (m_hThread);

        return E_OUTOFMEMORY;
    }

    // the app can initialize anything it needs for calls
    if (m_pCall->Init(m_pITelInfo) != NOERROR) {
        m_pITelInfo->Release();
        m_pITelInfo = NULL;
        SetEvent (m_hThread);

        return E_OUTOFMEMORY;
    }

    // if the speech recognition engine only supports one instance
    // then set # of lines to 1
    LPUNKNOWN  pUnkSR;
    pUnkSR = NULL;
    m_pITelInfo->ObjectGet(TELOBJ_SPEECHRECOG, &pUnkSR);
    if (pUnkSR) {
       PISRCENTRALW  pSR;
       pSR = NULL;
       pUnkSR->QueryInterface (IID_ISRCentralW, (PVOID*) &pSR);
       if (pSR) {
          SRMODEINFOW   mi;
          pSR->ModeGet (&mi);

          // I can set the number of lines to 1 here because
          // this case can only happen on the first line
          if (mi.dwFeatures & SRFEATURE_SINGLEINSTANCE)
             ((PCTELMAINTAPI14)m_pTelMain)->m_dwNumLines = 1;

          pSR->Release();
       }
       pUnkSR->Release();
    }


    // create the window
    m_hWnd = CreateWindowEx (WS_EX_WINDOWEDGE, gszClass, NULL, NULL,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, m_hInst, NULL);
    if (!m_hWnd) {
        SetEvent (m_hThread);

        return E_OUTOFMEMORY;
    }

    // we're ready to go into our message loop here so lets tell the main
    // guy, and signal that we have initialized
    SetEvent(ghThreadInitEvent);
    ((PCTELMAINTAPI14)m_pTelMain)->LineNotify(m_dwLine, WM_TELCONTROL_INITDONE);

    // message loop
    MSG msg;
    while(GetMessage(&msg, NULL, NULL, NULL)) {
        if (msg.message == TM_STOP)
            break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Here the app can clean up
    m_pCall->Shutdown();


    // release the information object
    if (m_pITelInfo) {
        m_pITelInfo->Release();
        m_pITelInfo = NULL;
    }

    // destroy the window
    if (m_hWnd) {
        DestroyWindow(m_hWnd);
        m_hWnd = NULL;
    }

    // uninit OLE
    CoUninitialize();

    SetEvent (m_hThread);

    return NOERROR;
}


HRESULT CTelLineTAPI14::MakeCall(LPCSTR number,DWORD countrycode)
{
	long retval;

	retval=lineMakeCall(m_li.hLine,&m_li.hCall,number,countrycode,NULL);
	if(retval < 0)
		return(retval);
	else
		return NOERROR;
}

HRESULT CTelLineTAPI14::DropCall()
{	
	lineDrop(m_li.hCall,NULL,0);
	return NOERROR;
}

// set the current hcall
HRESULT CTelLineTAPI14::SetHCALL(HCALL hCall)
{	
	m_li.hCall = hCall;
	return NOERROR;
}

/********************************************************************
 Go - This function is called when a phone call is initiated, and it
      will pass control the CCall object that the application developer
      supplies.
 */
HRESULT CTelLineTAPI14::Go()
{
    PISTLOG pILog = NULL;
    char    szBuf[64];
    char    szLogFileName[256];
    WCHAR   wszLogFileName[256];
    BOOL    fInc;
    LPUNKNOWN  pUnk = NULL;
    HRESULT hRes;

    // if logging, tell the logging object the file information
    m_pITelInfo->ObjectGet(TELOBJ_LOGGING, &pUnk);
    if (pUnk) {
         pUnk->QueryInterface (IID_ISTLog, (PVOID*)&pILog);
         pUnk->Release();
    }

    if (gfUseLogging) {
        wsprintf(szBuf, "%s%06d", "L", gdwNumber);
        fInc = InterlockedIncrement(&gdwNumber);

        // create log file name
        strcpy(szLogFileName, gszPath);
        int iLen;
        iLen = strlen(szLogFileName);
        if (!iLen || (szLogFileName[iLen-1] != L'\\'))
           strcat (szLogFileName, "\\");
        strcat(szLogFileName, szBuf);
        strcat(szLogFileName, gszExtension);

        // tell the logging object the log file name
        wszLogFileName[0] = 0;
        MultiByteToWideChar(CP_ACP, NULL, szLogFileName, -1, wszLogFileName,
                            sizeof(wszLogFileName));
        if (pILog)
            pILog->ToFile(wszLogFileName);
    }
    else {
        // if we aren't using logging, make sure the logging object knows
        // this by setting the log file to NULL
        if (pILog)
            pILog->ToFile(NULL);
    }

    // set an event so that we won't abort here
    // Aborting TAPI when there are still wave devices open is bad
    // for some TAPI cards
    ResetEvent (m_eFreeForAnotherCall);

    // Tell the speech recognizer if we're full duplex or not
    {
       DWORD   dwEcho, dwDuplex;
       dwEcho = dwDuplex = FALSE;
       m_pITelInfo->DWORDGet(TELDWORD_FullDuplex, &dwDuplex);
       m_pITelInfo->DWORDGet(TELDWORD_EchoCancel, &dwEcho);

       // get the SR engine
       pUnk = NULL;
       m_pITelInfo->ObjectGet(TELOBJ_SPEECHRECOG, &pUnk);
       if (pUnk) {
            PISRATTRIBUTES pISRAttributes = NULL;
            pUnk->QueryInterface (IID_ISRAttributes, (PVOID*)&pISRAttributes);
            pUnk->Release();

            if (pISRAttributes) {
               pISRAttributes->EchoSet (dwDuplex && dwEcho);
               pISRAttributes->Release();
            }
       }
    }

    // store the hcall away
    m_pITelInfo->DWORDSet (TELDWORD_HCALL, (DWORD) m_li.hCall);

    // This is the callback into the application supplied phone call logic
    m_pCall->DoPhoneCall();

    hRes = m_pITelInfo->ObjectGet(TELOBJ_TTSQUEUE, &pUnk);
    if (!hRes) 
    {
	CSTTTSQueue	q;
	q.Init (pUnk);
	pUnk->Release();
	q.WaitUntilDoneSpeaking();
    }
    // do a peekmessage loop until there are no more messages
    // wait in message loop
    MSG msg;
    while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE) )
    {
	   TranslateMessage(&msg);
	   DispatchMessage(&msg);
    }

    ((PCTELMAINTAPI14)m_pTelMain)->LineNotify(m_dwLine, WM_TELCONTROL_CALLDONE);

    // Set an event so we know that we can abort
    SetEvent (m_eFreeForAnotherCall);

    // turn logging off after the call
    if (pILog) {
       pILog->ToFile(NULL);
       pILog->Release();
    }

    return NOERROR;

}


/********************************************************************
 NewCall - Called by the TAPI object, it will post a message to the
           line object which will eventually get to the Go function.

           This is done so that we run the call logic in the line
           thread. This function is called on the main application
           thread.
 */
HRESULT CTelLineTAPI14::NewCall()
{
    PostMessage(m_hWnd, TM_RUN, 0, (LONG) this);
    return NOERROR;
}


/********************************************************************
 Stop - Close the TAPI line and stop the line thread. It will wait
        until the thread goes away before returning to call (which
        means lines will be shut down synchronously).
 */
HRESULT CTelLineTAPI14::Stop()
{

    lineClose(m_li.hLine);

    if (m_hThread) {
        PostMessage(m_hWnd, TM_STOP, 0, 0L);

        WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
    }
    return NOERROR;
}


/********************************************************************
 AbortControl - Stop the call.
 */
HRESULT CTelLineTAPI14::AbortControl()
{
    if (m_pITelInfo)
        m_pITelInfo->SendAbort(0);

    return NOERROR;
}


/********************************************************************
 Abort - Called by the TAPI object, it will post a message to the
         line object which will eventually get to the AbortControl
         function.

         This is done so that we call the information object in the
         line thread. This function is called on the main application
         thread.
 */
HRESULT CTelLineTAPI14::Abort()
{
    PostMessage(m_hWnd, TM_ABORT, 0, (LONG) this);

    // wait until the line returns and indicates that we're no
    // longer in a phone call
    if (m_eFreeForAnotherCall)
       WaitForSingleObject (m_eFreeForAnotherCall, INFINITE);

    return NOERROR;
}


/********************************************************************
 SendDTMF - Sends the application a DTMF code.
 */
HRESULT CTelLineTAPI14::SendDTMF(WCHAR wcKey)
{
    if (m_pITelInfo)
        m_pITelInfo->SendDTMF(wcKey);

    return NOERROR;
}


/********************************************************************
 GotDTMF - Called by the TAPI object, it will post a message to the
           line object which will eventually get to the SendDTMF
           function.

           This is done so that we call the information object in the
           line thread. This function is called on the main application
           thread.
 */
HRESULT CTelLineTAPI14::GotDTMF(WCHAR wcKey)
{
    PostMessage(m_hWnd, TM_DTMF, wcKey, (LONG) this);
    return NOERROR;
}




// TAPI Main object
//***************************************************************************
//***************************************************************************
//***************************************************************************
CTelMainTAPI14::CTelMainTAPI14()
{
    m_hLineApp      = NULL;
    m_pdwAddr       = NULL;
    m_dwNumLines    = 0;

    m_lpLineInfo    = NULL;
    m_iReq          = 0;
    m_hAppWnd       = 0;

    m_dwState       = CALLSTATE_IDLE;
    m_hDlgWnd       = NULL;
    m_dwAnswerAfterRings = 1;
    m_dwMaxLines = 1000;   // arbitrarily high number
}

CTelMainTAPI14::~CTelMainTAPI14()
{
    DWORD i;

    // get rid of all the line objects
    for (i = 0; i < m_dwNumLines; i++) {
        if (m_lpLineInfo[i].pTelLine)
            delete m_lpLineInfo[i].pTelLine;
    }

    if (m_lpLineInfo) 
        FREE (m_lpLineInfo);

    if (m_pdwAddr) 
        FREE (m_pdwAddr);
}


/********************************************************************
 Init - Registers the window class for the hidden window used by the
        line threads. Initializes TAPI, gets the number of lines, and
        initializes each line.
 */
#ifdef STRICT
HRESULT CTelMainTAPI14::Init (PVOID hWnd, PVOID hInst)
#else
HRESULT CTelMainTAPI14::Init (HWND hWnd, HINSTANCE hInst)
#endif
{
    DWORD dwLine, errCode, tc = GetTickCount(), dwReturn = E_FAIL;
    PCCALL pAppCall;

    m_hInst = (HINSTANCE) hInst;
    m_hAppWnd = (HWND) hWnd;

    // register the windows class
    WNDCLASS    wc;
    memset (&wc, 0, sizeof(wc));
    wc.style = 0;
    wc.lpfnWndProc = LineWndProc;
    wc.hInstance = (HINSTANCE) hInst;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = gszClass;
    wc.hIcon = NULL;
    RegisterClass (&wc);

    // set up the initialization event
    ghThreadInitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!ghThreadInitEvent)
        return E_FAIL;

    PCTELLINETAPI14 pLineObj;

    // make sure the callback function can call us back
    gpObj = (PCTELMAINTAPI14)this;
    errCode = lineInitialize(&m_hLineApp, (HINSTANCE) hInst, (LINECALLBACK)LineCallback,
                             NULL, &m_dwNumLines);
    if (errCode != 0) {
        if (errCode == LINEERR_REINIT) {
            // keep trying until the user cancels
            // or we stop getting LINEERR_REINIT
            while ((errCode = lineInitialize(&m_hLineApp, (HINSTANCE) hInst,NULL, NULL,
                                             &m_dwNumLines)) != 0) {
                // error out if 5 seconds have passed
                if(GetTickCount() > 5000 + tc)
                    return E_FAIL;
            }
        }
        else
            return E_FAIL;
	}

    // no more than maxlines
    m_dwNumLines = min(m_dwNumLines, m_dwMaxLines);

    // allocate buffer for storing LINEINFO for all of the available lines
    // always allocate space for at least one line
    if ( m_dwNumLines == 0 )
        return E_FAIL;
    else {
        m_pdwAddr = (DWORD *) ALLOC(sizeof(DWORD) * m_dwNumLines);
        m_lpLineInfo = (LPLINEINFO) ALLOC(sizeof(LINEINFO) * m_dwNumLines);
        memset (m_pdwAddr, 0, sizeof(DWORD) * m_dwNumLines);
        memset (m_lpLineInfo, 0, sizeof(LINEINFO) * m_dwNumLines);
    }

    // if no space was set aside...
    if ( m_lpLineInfo == NULL || m_pdwAddr == NULL ) {
        dwReturn = E_OUTOFMEMORY;
        goto tapiinit_exit;
    }

#ifdef RHETOREX
    gfRhetorex = TRUE;  // note - shouldn't set this by default
#endif //RHETOREX 

    // This code assumes that there's only one call possible per line
    // While this works correctly for most cards, it does not work
    // for ISDN. A developer sufficiently skilled in TAPI and SAPI
    // can modify this code to create two telephony info objects
    // per line, and handle ISDN

    // fill m_lpLineInfo[] and open each line
    for ( dwLine = 0; dwLine < m_dwNumLines; ++dwLine ) {
//    for ( dwLine = 0; dwLine < 1; ++dwLine ) {
        // skip remaining processing for this if it didn't open

#ifdef RHETOREX
        // NOTE - Shouldn't necessarily skip this line if odd, because
        // may not be rheoriex or may be more than one card in
        // default to disabled line
       if (gfRhetorex) {
          if (dwLine % 2)
             continue;
       }
#endif // RHETOREX

        m_lpLineInfo[dwLine].dwState = CALLSTATE_DISABLED;

        if ( GetLineInfo( dwLine, &m_lpLineInfo[dwLine], m_hLineApp ) != ERR_NONE )
            continue; 

        m_pdwAddr [dwLine] = m_lpLineInfo[dwLine].nAddr;
        m_lpLineInfo[dwLine].hApp = m_hLineApp;
        m_lpLineInfo[dwLine].dwLineReplyNum = 0;

        // open the line with this application as the owner
        errCode = lineOpen(m_hLineApp, dwLine, &(m_lpLineInfo[dwLine].hLine),
                           m_lpLineInfo[dwLine].dwAPIVersion, 0, (DWORD) this,
                           LINECALLPRIVILEGE_OWNER,
                           LINEMEDIAMODE_INTERACTIVEVOICE | LINEMEDIAMODE_AUTOMATEDVOICE,
                           NULL);
        if (errCode)
            continue;

        // set status messages so we know if the phone is ringing
        lineSetStatusMessages(m_lpLineInfo[dwLine].hLine, 0x000FFFFF, 0x000000FF );

        // initial status is INITIALIZING
        m_lpLineInfo[dwLine].dwState = CALLSTATE_INITIALIZING;

        // create the line object
        pLineObj = new CTelLineTAPI14(hInst, dwLine, &m_lpLineInfo[dwLine]);
        if (!pLineObj)
            return E_OUTOFMEMORY;

        // this is where we call back into the application to get a CCall
        // object that has the logic for the phone call
        pAppCall = NULL;
        CreateLineObject(&pAppCall);
        m_lpLineInfo[dwLine].pAppCall = pAppCall;

        // initialize the line object and wait until it is done
        // initializing before moving on to next line
        if (pAppCall) {
            ResetEvent(ghThreadInitEvent);
            if (pLineObj->Init(m_hAppWnd, this, pAppCall) != NOERROR)
                return E_OUTOFMEMORY;
            WaitForSingleObject(ghThreadInitEvent, INFINITE);
        }
        else {
            lineClose(m_lpLineInfo[dwLine].hLine);
            delete pLineObj;
        }

        // remember our line object
        m_lpLineInfo[dwLine].pTelLine = (PCTELLINE) pLineObj;
    }

    dwReturn = NOERROR;

tapiinit_exit:
    return dwReturn;
}


/********************************************************************
 LineNotify - Called from a line object to notify us about the state
              of a phone call
 */
void CTelMainTAPI14::LineNotify(DWORD dwLine, DWORD dwNotification)
{
    if (dwNotification == WM_TELCONTROL_INITDONE) {
        m_lpLineInfo[dwLine].dwState = CALLSTATE_IDLE;
        if (m_hDlgWnd)
            PostMessage(m_hDlgWnd, WM_TELCONTROL_CALLSTATE, dwLine, CALLSTATE_IDLE);
    }
    else if (dwNotification == WM_TELCONTROL_CALLDONE) {
        CallDone(dwLine);
        m_lpLineInfo[dwLine].dwState = CALLSTATE_DISCONNECTING;
        if (m_hDlgWnd)
            PostMessage(m_hDlgWnd, WM_TELCONTROL_CALLSTATE, dwLine, CALLSTATE_DISCONNECTING);
    }
}


/********************************************************************
 Shutdown - Saves logging file information to the registry, closes all
            the phone lines, and shuts down TAPI.
 */
HRESULT CTelMainTAPI14::Shutdown()
{
    HKEY  hKey;
    DWORD i, dwSize;

    // write out the log number and directory for persistence
    LONG status = RegOpenKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\SpeechAPI\\TelephonyLogging",
                                NULL, KEY_ALL_ACCESS, &hKey );

    if (status == ERROR_SUCCESS) {
        // save log number used for building log file names
        dwSize = sizeof(DWORD);
        status = RegSetValueEx( hKey, "LogNumber", NULL, REG_DWORD,
                                (PBYTE)&gdwNumber, dwSize );

        // save logging on indicator
        status = RegSetValueEx( hKey, "LoggingOn", NULL, REG_DWORD,
                                (PBYTE)&gfUseLogging, dwSize );

        // save log file directory
        dwSize = lstrlen(gszPath) + 1;
        status = RegSetValueEx( hKey, "LogFileDirectory", NULL, REG_SZ,
                                (PBYTE)gszPath, dwSize );

        RegCloseKey(hKey);
    }

    // stop all the lines
    for (i = 0; i < m_dwNumLines; i++) {
        if (m_lpLineInfo[i].pTelLine)
            m_lpLineInfo[i].pTelLine->Stop();

        if (m_lpLineInfo[i].pAppCall)
            delete m_lpLineInfo[i].pAppCall;
    }

    // shutdown TAPI
    if (m_hLineApp) {
        lineShutdown(m_hLineApp);
        m_hLineApp = NULL;
    }

    return NOERROR;
}


/********************************************************************
 CallDialog - Runs the dialog that controls the TAPI lines. The dialog
              is in spchtel.dll, so we need to load it to run the dialog.

              The reason it is not in here is because this is a static
              library, and cannot have resources.
 */
HRESULT CTelMainTAPI14::CallDialog()
{
    PSTR        pszBuf;
    DWORD       dwNeeded, dwType;
    HINSTANCE   hInst;
    LONG        status;
    HKEY        hKey;

	// open the key
    hKey = NULL;
	status = RegOpenKeyEx(HKEY_CLASSES_ROOT,
                          "CLSID\\{F9D18BF8-E0ED-11d0-AB8B-08002BE4E3B7}\\InprocServer32",
                          0, KEY_ALL_ACCESS, &hKey);
    if (status != ERROR_SUCCESS)
        goto CDError;

	dwNeeded = 0;
	status = RegQueryValueEx(hKey, NULL, NULL, &dwType, NULL, &dwNeeded);
	if (dwNeeded) {
	    pszBuf = (PSTR) ALLOC(dwNeeded);
	    if (pszBuf) {
            // get the full path of spchtel.dll
    		status = RegQueryValueEx(hKey, NULL, NULL, &dwType,
                                    (PBYTE)pszBuf, &dwNeeded);
		    if (status != ERROR_SUCCESS)
                goto CDError;
	    }
        else
            goto CDError;
	}
    else
        goto CDError;

    RegCloseKey(hKey);
    hKey = NULL;

    // load spchtel.dll
    hInst = LoadLibrary(pszBuf);
    if (hInst == NULL)
        goto CDError;

    // call the TAPI dialog
    DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_TAPICALLDIALOG), m_hAppWnd,
                   (DLGPROC) TapiDlgProc, (LONG)this);

    FREE (pszBuf);
    FreeLibrary(hInst);
    return NOERROR;

CDError:
    if (pszBuf)
        FREE (pszBuf);

    if (hKey)
        RegCloseKey(hKey);

    return E_FAIL;
}


/********************************************************************
 CallDone - Call is over so let's drop it.
 */
HRESULT CTelMainTAPI14::CallDone(DWORD dwLine)
{
    if (m_lpLineInfo[dwLine].hCall) {
        lineDrop(m_lpLineInfo[dwLine].hCall, NULL, 0);
        m_lpLineInfo[dwLine].hCall = NULL;
    }

    return NOERROR;
}


/********************************************************************
 GetLineFromCall - Gets a line number given a handle to a call.
 */
HLINE GetLineFromCall(HCALL hCall)
{
    HLINE hLine;
    LINECALLINFO lci, *plci;

    lci.dwTotalSize = sizeof(LINECALLINFO);
    lineGetCallInfo(hCall, &lci);
    if (lci.dwNeededSize > lci.dwTotalSize) {
        plci = (LINECALLINFO *) ALLOC(lci.dwNeededSize);
        if (!plci)
            return (HLINE) -1;

        plci->dwTotalSize = lci.dwNeededSize;
        lineGetCallInfo(hCall, plci);
        hLine = plci->hLine;
        FREE(plci);
    }
    else {
        hLine = lci.hLine;
    }

    return hLine;
}

HRESULT CTelMainTAPI14::Callback()
{
    return NOERROR;
}



/********************************************************************
 Callback - Function called from the TAPI callback function
 */
HRESULT CTelMainTAPI14::Callback(DWORD hDevice, DWORD dwMsg, DWORD dwCBInst,
                                 DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
{
    DWORD i;
    HLINE hLine;

    switch (dwMsg) {
        case LINE_REPLY:
           // we get this on some tapi boards that do not do immediate replies
           if (!dwParam1)
              break;
           for (i = 0; i < m_dwNumLines; i++)
              if (dwParam1 == m_lpLineInfo[i].dwLineReplyNum)
                 break;
           if (i >= m_dwNumLines)
              break;
           m_lpLineInfo[i].dwLineReplyNum = 0;  // so don't get again

           // if it failed then return
           if ((int)dwParam2 < 0)
              break; // failed

           //else succeded, so connect
           m_lpLineInfo[i].dwState = CALLSTATE_INPROGRESS;
           if (m_hDlgWnd)
                PostMessage(m_hDlgWnd, WM_TELCONTROL_CALLSTATE, i,
                            CALLSTATE_INPROGRESS);	   

           break;

        case LINE_LINEDEVSTATE:
           if (dwParam1 == LINEDEVSTATE_RINGING ) {
                // find the line
                hLine = (HLINE) hDevice;
                for (i = 0; i < m_dwNumLines; i++)
                    if (hLine == m_lpLineInfo[i].hLine)
                       break;
                if (i >= m_dwNumLines)
                   break;  // not a valid line

                // if we haven't had enough rings then break
                if (dwParam3 < m_dwAnswerAfterRings)
                   break;

                // answer the call
                m_iReq = lineAnswer(m_lpLineInfo[i].hCall, NULL, 0);
                if (m_iReq < 0)
                    return E_FAIL;
                if (m_iReq > 0) {
                   m_lpLineInfo[i].dwLineReplyNum = (DWORD) m_iReq;
                   break;
                }

                m_lpLineInfo[i].dwState = CALLSTATE_CONNECTING;
                if (m_hDlgWnd)
                      PostMessage(m_hDlgWnd, WM_TELCONTROL_CALLSTATE, i,
                                  CALLSTATE_CONNECTING);
           }
           break;

        case LINE_CALLSTATE:
            // call being offered, answer it
            if (dwParam1 == LINECALLSTATE_OFFERING) {
                // clear the number of rings to 0
                hLine = GetLineFromCall((HCALL) hDevice);
                if (hLine == (HLINE) -1)
                    return NOERROR;

                // tell the TAPI dialog about the state of this line
                for (i = 0; i < m_dwNumLines; i++) {
                    if (hLine == m_lpLineInfo[i].hLine) {
                        m_lpLineInfo[i].hCall = (HCALL) hDevice;
                        ((CTelLineTAPI14*) m_lpLineInfo[i].pTelLine)->SetHCALL(m_lpLineInfo[i].hCall);
                        break;
                    }
                }
            }
            // call connected, tell the line object so that it can call
            // the application supplied call logic
            else if (dwParam1 == LINECALLSTATE_CONNECTED) {
                hLine = GetLineFromCall((HCALL) hDevice);
                if (hLine == (HLINE) -1)
                    return E_FAIL;

                for (i = 0; i < m_dwNumLines; i++) {
                    if (hLine == m_lpLineInfo[i].hLine) {
                        m_lpLineInfo[i].hCall = (HCALL) hDevice;
                        ((CTelLineTAPI14*) m_lpLineInfo[i].pTelLine)->SetHCALL(m_lpLineInfo[i].hCall);

                        // tell TAPI we want DTMF keys
                        lineMonitorDigits((HCALL) hDevice, LINEDIGITMODE_DTMF);

                        // call the line object to initiate the conversation
                        m_lpLineInfo[i].pTelLine->NewCall();

                        // tell the TAPI dialog about the stat of this line
                        m_lpLineInfo[i].dwState = CALLSTATE_INPROGRESS;
                        if (m_hDlgWnd)
                            PostMessage(m_hDlgWnd, WM_TELCONTROL_CALLSTATE, i,
                                        CALLSTATE_INPROGRESS);
                        break;
                    }
                }
            }
            // call disconnected
            else if (dwParam1 == LINECALLSTATE_DISCONNECTED) {
                hLine = GetLineFromCall((HCALL) hDevice);
                if (hLine == (HLINE) -1)
                    return NOERROR;

               // tell the line object to abort
               for (i = 0; i < m_dwNumLines; i++) {
                  if (hLine == m_lpLineInfo[i].hLine) {
                       m_lpLineInfo[i].pTelLine->Abort();
                  }
               }

                // We don't need to drop the line since the abort
                // call just did this in its own thread

                // tell the TAPI dialog about the state of this line
                for (i = 0; i < m_dwNumLines; i++) {
                    if (hLine == m_lpLineInfo[i].hLine) {
                        m_lpLineInfo[i].dwState = CALLSTATE_DISCONNECTING;
                        if (m_hDlgWnd)
                            PostMessage(m_hDlgWnd, WM_TELCONTROL_CALLSTATE, i,
                                        CALLSTATE_DISCONNECTING);

                        break;
                    }
                }
            }
            // line going idle
            else if (dwParam1 == LINECALLSTATE_IDLE) {
                // tell the TAPI dialog about the state of this line
                hLine = GetLineFromCall((HCALL) hDevice);
                if (hLine != (HLINE) -1) {
                    for (i = 0; i < m_dwNumLines; i++) {
                        if (hLine == m_lpLineInfo[i].hLine) {
                            m_lpLineInfo[i].dwState = CALLSTATE_IDLE;
                            if (m_hDlgWnd)
                                PostMessage(m_hDlgWnd, WM_TELCONTROL_CALLSTATE, i,
                                            CALLSTATE_IDLE);

                            break;
                        }
                    }
                }

                // tell TAPI to get rid of all the resources for this call
                lineDeallocateCall((HCALL)hDevice);
            }
	    else if (dwParam1 == LINECALLSTATE_RINGBACK)
	    {
		   // tell the TAPI dialog about the state of this line
                hLine = GetLineFromCall((HCALL) hDevice);
                if (hLine != (HLINE) -1) {
                    for (i = 0; i < m_dwNumLines; i++) {
                        if (hLine == m_lpLineInfo[i].hLine) {
                            m_lpLineInfo[i].dwState = LINECALLSTATE_RINGBACK;
                            if (m_hDlgWnd)
                                PostMessage(m_hDlgWnd, WM_TELCONTROL_CALLSTATE, i,
                                           LINECALLSTATE_RINGBACK);

                            break;
                        }
                    }
		}

	    }
	    else if (dwParam1 == LINECALLSTATE_BUSY)
	    {
        	   // tell the TAPI dialog about the state of this line
                hLine = GetLineFromCall((HCALL) hDevice);
                if (hLine != (HLINE) -1) {
                    for (i = 0; i < m_dwNumLines; i++) {
                        if (hLine == m_lpLineInfo[i].hLine) {
                            m_lpLineInfo[i].dwState = LINECALLSTATE_BUSY;
                            if (m_hDlgWnd)
                                PostMessage(m_hDlgWnd, WM_TELCONTROL_CALLSTATE, i,
                                           LINECALLSTATE_BUSY);

                            break;
                        }
                    }
		}

	    }


            break;


        // got a DTMF key
        case LINE_MONITORDIGITS:
            hLine = GetLineFromCall((HCALL) hDevice);
            if (hLine == (HLINE) -1)
                return E_FAIL;

            // tell the line object we got a DTMF key so it could
            // notify the application
            for (i = 0; i < m_dwNumLines; i++) {
                if (hLine == m_lpLineInfo[i].hLine)
                    m_lpLineInfo[i].pTelLine->GotDTMF((WCHAR)dwParam1);
            }

            break;
    }

    return NOERROR;
}


/********************************************************************
 Abort - Call by the application if the user aborts the call
 */
HRESULT CTelMainTAPI14::Abort()
{
    DWORD i;

    for (i = 0; i < m_dwNumLines; i++) {
        // if this line has an active call, drop it
        if (m_lpLineInfo[i]. hCall) {

            // tel line object to abort the control
            if (m_lpLineInfo[i].pTelLine)
                m_lpLineInfo[i].pTelLine->Abort();

            // drop this line and indicate no call on this line
            lineDrop(m_lpLineInfo[i].hCall, NULL, 0);
            m_lpLineInfo[i].hCall = NULL;
        }
    }

    return NOERROR;
}


HRESULT CTelMainTAPI14::GotDTMF(DWORD dwKey)
{
    return NOERROR;
}





// Get line information    
//***************************************************************************
//***************************************************************************
//***************************************************************************
HRESULT GetLineInfo (DWORD dwLine, LPLINEINFO lpLineInfo, HLINEAPP hLineApp)
{
	DWORD errCode = 0;
	DWORD dwNeededSize = 0; 
	LINEEXTENSIONID ExtensionID;

	LPSTR pszLineName = NULL; 
	LPLINEDEVCAPS lpDevCaps = NULL;

    HRESULT hrReturn = E_FAIL;

    // need to get version information straight with TAPI
    errCode = lineNegotiateAPIVersion (hLineApp, dwLine, TAPI_VERSION_1_4,
                                       TAPI_VERSION_2_0,
                                       &(lpLineInfo->dwAPIVersion),
                                       &ExtensionID);
    if (errCode)
        goto error;

    // get the line device capabilities
    dwNeededSize = sizeof(LINEDEVCAPS);
    do {
        lpDevCaps = (LPLINEDEVCAPS) ALLOC (dwNeededSize);
        if (!lpDevCaps) {
			hrReturn = E_OUTOFMEMORY;
            goto error;
        }

        lpDevCaps->dwTotalSize = dwNeededSize;
        errCode = lineGetDevCaps (hLineApp, dwLine, lpLineInfo->dwAPIVersion,
                                  0, lpDevCaps);
        if (errCode)
			goto error;

        if (lpDevCaps->dwNeededSize <= lpDevCaps->dwTotalSize)
            break;

        dwNeededSize = lpDevCaps->dwNeededSize;
        FREE (lpDevCaps);
        lpDevCaps = NULL;
	} while ( TRUE );

    // save number of addresses and whether it is a voice line
    lpLineInfo->nAddr = lpDevCaps->dwNumAddresses;
    lpLineInfo->fVoiceLine =
        ( (lpDevCaps->dwMediaModes & LINEMEDIAMODE_INTERACTIVEVOICE) != 0 );

    //
    // get the line name
    //
    pszLineName = (LPSTR) ALLOC (MAXBUFSIZE);
    if (!pszLineName) {
        hrReturn = E_OUTOFMEMORY;
        goto error;
    }

    if (lpDevCaps->dwLineNameSize > 0) {
        if (lpDevCaps->dwLineNameSize > (MAXBUFSIZE - 1)) {
            strncpy (pszLineName, 
                     (LPSTR) lpDevCaps + lpDevCaps->dwLineNameOffset,
                     MAXBUFSIZE - 1);
            pszLineName[ MAXBUFSIZE - 1 ] = '\0';
        }
        else {
            lstrcpy(pszLineName, (LPSTR) lpDevCaps + lpDevCaps->dwLineNameOffset);
        }
    }
    else {
        wsprintf (pszLineName, "Line %d", dwLine);
    }

    lstrcpy(lpLineInfo->szLineName, pszLineName);
    lpLineInfo->dwPermanentLineID = lpDevCaps->dwPermanentLineID;

    hrReturn = NOERROR;
	
error:
    if (lpDevCaps)
        FREE (lpDevCaps);

    if (pszLineName)
        FREE (pszLineName);

    return hrReturn;
}



// PC line object
//***************************************************************************
//***************************************************************************
//***************************************************************************
#ifdef STRICT
CTelLinePC::CTelLinePC(PVOID hInst)
#else
CTelLinePC::CTelLinePC(HINSTANCE hInst)
#endif
{
    m_hInst     = (HINSTANCE) hInst;
    m_pCall     = NULL;
    m_pITelInfo = NULL;
    m_hAppWnd   = NULL;
}

CTelLinePC::~CTelLinePC()
{
    if (m_pITelInfo) {
        m_pITelInfo->Release();
        m_pITelInfo = NULL;
    }

    // uninit OLE
    CoUninitialize();
}

/********************************************************************
 Init - It will initialize OLE, create the telephony information object
        and set it up, and initialize the CCall object.
 */
#ifdef STRICT
HRESULT CTelLinePC::Init(PVOID hwnd, PVOID pTelMain, PCCALL pCall)
#else
HRESULT CTelLinePC::Init(HWND hwnd, PVOID pTelMain, PCCALL pCall)
#endif
{
    m_hAppWnd = (HWND) hwnd;
    m_pTelMain = pTelMain;
    m_pCall = pCall;

    // init OLE
    CoInitialize(NULL);

    // Create the telephony control information object
    HRESULT	hRes;
    hRes = CoCreateInstance (CLSID_TelInfo, NULL, CLSCTX_INPROC_SERVER,
                             IID_ITelInfo, (void**)&m_pITelInfo);
    if (hRes) {
        MessageBox (NULL,
                    "Can't load the telephony information."
                    "You must have the 4.0 version of the API installed.",
                    NULL, MB_OK);
        return E_FAIL;
    }

    // let the app do it's own QuickCreate if it wants to use certain
    // engines, etc.   The default is at the end of this file
    if (m_pCall->DoQuickCreate(m_pITelInfo, m_hInst) != NOERROR) {
        m_pITelInfo->Release();
        m_pITelInfo = NULL;
        return E_OUTOFMEMORY;
    }

    // initialize the CCall object
    if (m_pCall->Init(m_pITelInfo) != NOERROR) {
        delete m_pCall;
        m_pITelInfo->Release();
        return E_OUTOFMEMORY;
    }

    return NOERROR;
}

/********************************************************************
 NewCall - Just calls Go (simulating the way things are done in the
           TAPI line object).
 */
HRESULT CTelLinePC::NewCall()
{
    HRESULT hr = Go();
    return hr;
}


/********************************************************************
 Go - This function is called when a phone call is initiated, and it
      will pass control the CCall object that the application developer
      supplies.
 */
HRESULT CTelLinePC::Go()
{
    PISTLOG pILog;
    char    szBuf[64];
    char    szLogFileName[256];
    WCHAR   wszLogFileName[256];
    HRESULT hr;
    BOOL    fInc;

    // if logging, tell the logging object the file information
    if (gfUseLogging) {
        wsprintf(szBuf, "%s%06d", "L", gdwNumber);
        fInc = InterlockedIncrement(&gdwNumber);

        // create log file name
        strcpy(szLogFileName, gszPath);
        int iLen;
        iLen = strlen(szLogFileName);
        if (!iLen || (szLogFileName[iLen-1] != L'\\'))
           strcat (szLogFileName, "\\");
        strcat(szLogFileName, szBuf);
        strcat(szLogFileName, gszExtension);

        // get the address of the logging object
        hr = m_pITelInfo->ObjectGet(TELOBJ_LOGGING, (LPUNKNOWN *)&pILog);
        if (hr != NOERROR)
            return hr;

        // tell the logging object the log file name
        wszLogFileName[0] = 0;
        MultiByteToWideChar(CP_ACP, NULL, szLogFileName, -1, wszLogFileName,
                            sizeof(wszLogFileName));
        pILog->ToFile(wszLogFileName);
        pILog->Release();
    }
    else {
        // if we aren't using logging, make sure the logging object knows
        // this by setting the log file to NULL
        hr = m_pITelInfo->ObjectGet(TELOBJ_LOGGING, (LPUNKNOWN *)&pILog);
        if (hr != NOERROR)
            return hr;

        pILog->ToFile(NULL);
        pILog->Release();
    }

    // This is the callback into the application supplied phone call logic
    m_pCall->DoPhoneCall();

    // turn logging off after the call
    if (gfUseLogging) {
        hr = m_pITelInfo->ObjectGet(TELOBJ_LOGGING, (LPUNKNOWN *)&pILog);
        if (hr != NOERROR)
            return hr;

        pILog->ToFile(NULL);
        pILog->Release();
    }

    return NOERROR;
}

/********************************************************************
 Abort - Aborts the phone call
 */
HRESULT CTelLinePC::Abort()
{
    if (m_pITelInfo)
        m_pITelInfo->SendAbort(0);

    return NOERROR;
}

/********************************************************************
 GotDTMF - Sends a DTMF key to the user
 */
HRESULT CTelLinePC::GotDTMF(WCHAR wcKey)
{
    if (m_pITelInfo)
        m_pITelInfo->SendDTMF(wcKey);

    return NOERROR;
}




// PC Main object
//***************************************************************************
//***************************************************************************
//***************************************************************************
CTelMainPC::CTelMainPC()
{
    m_hInst         = NULL;
    m_pLine         = NULL;
    m_hAppWnd       = NULL;
    m_pAppCall      = NULL;
    m_dwAnswerAfterRings = 1;
    m_dwMaxLines = 1;
}

CTelMainPC::~CTelMainPC()
{
    if (m_pLine) {
        delete(m_pLine);
        m_pLine = NULL;
    }

    if (m_pAppCall) {
        m_pAppCall->Shutdown();
        delete m_pAppCall;
        m_pAppCall = NULL;
    }
}

/********************************************************************
 Init - Create a line object, get the CCall object from the application,
        and initialize the line object (has to be done after getting the
        CCall object from the application because this is needed by the
        line object).
 */
#ifdef STRICT
HRESULT CTelMainPC::Init (PVOID hWnd, PVOID hInst)
#else
HRESULT CTelMainPC::Init (HWND hWnd, HINSTANCE hInst)
#endif
{
    m_hAppWnd = (HWND) hWnd;
    m_hInst = (HINSTANCE) hInst;

    // create a PC line object
    m_pLine = new CTelLinePC(hInst);
    if (!m_pLine)
        return E_FAIL;

    // get call object from the application
    CreateLineObject(&m_pAppCall);
    if (!m_pAppCall) {
        delete m_pLine;
        m_pLine = NULL;
        return E_OUTOFMEMORY;
    }

    // initialize the line object
    if (m_pLine->Init(m_hAppWnd, this, m_pAppCall) != NOERROR) {
        delete m_pLine;
        m_pLine = NULL;
        return E_OUTOFMEMORY;
    }

    return NOERROR;
}


/********************************************************************
 Shutdown - Save log file information to the registry, shutdown the
            applications call object, and the PC line object
 */
HRESULT CTelMainPC::Shutdown()
{
    HKEY  hKey;
    DWORD dwSize;

    // write out the log number and directory for persistence
        dwSize = sizeof(DWORD);
    LONG status = RegOpenKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\SpeechAPI\\TelephonyLogging",
                                NULL, KEY_ALL_ACCESS, &hKey );

    if (status == ERROR_SUCCESS) {
        // save the log number used to build log file names
        dwSize = sizeof(DWORD);
        status = RegSetValueEx( hKey, "LogNumber", NULL, REG_DWORD,
                                (PBYTE)&gdwNumber, dwSize );

        // save logging on indicator
        status = RegSetValueEx( hKey, "LoggingOn", NULL, REG_DWORD,
                                (PBYTE)&gfUseLogging, dwSize );

        // save log file directory
        dwSize = lstrlen(gszPath) + 1;
        status = RegSetValueEx( hKey, "LogFileDirectory", NULL, REG_SZ,
                                (PBYTE)gszPath, dwSize );

        RegCloseKey(hKey);
    }

    // shutdown the applications call object, and delete it
    if (m_pAppCall) {
        m_pAppCall->Shutdown();
        delete m_pAppCall;
        m_pAppCall = NULL;
    }

    // delete the line object
    if (m_pLine) {
        delete(m_pLine);
        m_pLine = NULL;
    }

    return NOERROR;
}


/********************************************************************
 CallDialog - Runs the dialog that controls the PC phone emulator.
              The dialog is in spchtel.dll, so we need to load it to
              run the dialog.

              The reason it is not in here is because this is a static
              library, and cannot have resources.
 */
HRESULT CTelMainPC::CallDialog()
{
    PSTR        pszBuf;
    DWORD       dwNeeded, dwType;
    HINSTANCE   hInst;
    LONG        status;
    HKEY        hKey;

	// open the key
    hKey = NULL;
	status = RegOpenKeyEx(HKEY_CLASSES_ROOT,
                          "CLSID\\{F9D18BF8-E0ED-11d0-AB8B-08002BE4E3B7}\\InprocServer32",
                          0, KEY_ALL_ACCESS, &hKey);
    if (status != ERROR_SUCCESS)
        goto PCCDError;

	dwNeeded = 0;
	status = RegQueryValueEx(hKey, NULL, NULL, &dwType, NULL, &dwNeeded);
	if (dwNeeded) {
	    pszBuf = (PSTR) ALLOC(dwNeeded);
	    if (pszBuf) {
            // get the full path of spchtel.dll
    		status = RegQueryValueEx(hKey, NULL, NULL, &dwType,
                                     (PBYTE)pszBuf, &dwNeeded);
		    if (status != ERROR_SUCCESS)
                goto PCCDError;
	    }
        else
            goto PCCDError;
	}
    else
        goto PCCDError;

    RegCloseKey(hKey);
    hKey = NULL;

    // load spchtel.dll
    hInst = LoadLibrary(pszBuf);
    if (hInst == NULL)
        goto PCCDError;

    // run the dialog
    DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_PCCALLDIALOG), m_hAppWnd,
                   (DLGPROC) PCDlgProc, (LONG)this);

    FREE (pszBuf);
    FreeLibrary(hInst);
    return NOERROR;

PCCDError:
    if (pszBuf)
        FREE (pszBuf);

    if (hKey)
        RegCloseKey(hKey);

    return E_FAIL;
}


HRESULT CTelMainPC::CallDone(DWORD dwLine)
{
    return NOERROR;
}


HRESULT CTelMainPC::Callback(DWORD hDevice, DWORD dwMsg, DWORD dwCBInst,
                             DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
{
    return NOERROR;
}


/********************************************************************
 Callback - Tell the PC line object to start a new phone call
 */
HRESULT CTelMainPC::Callback()
{
    if (m_pLine)
        m_pLine->NewCall();

    return NOERROR;
}


/********************************************************************
 Abort - Tell the PC line object to abort the phone call
 */
HRESULT CTelMainPC::Abort()
{
    if (m_pLine)
        m_pLine->Abort();

    return NOERROR;
}


/********************************************************************
 Abort - Tell the PC line object a DTMF key was pushed on the phone
         simulator in the PC dialog box.
 */
HRESULT CTelMainPC::GotDTMF(DWORD dwKey)
{
    WCHAR wcKey[] = L"123456789*0#";

    if (m_pLine)
        m_pLine->GotDTMF(wcKey[dwKey]);

    return NOERROR;
}




/***************************************************************
 ***************************************************************
 CTelMain defaults
 */

/***************************************************************
 UseLogging - Gets logging information from the registry, including
              a logging indicator to tell whether to use logging or
              not. Defaults are used if there is not information in
              the registry yet.

              The defaults are: log number = 1, logging = off,
              log file directory = c:\program files\microsoft speech sdk
 */
HRESULT CTelMain::UseLogging()
{
    HKEY  hKey;
    DWORD dwType, dwDisposition, dwSize;
    BOOL  fRegDir = FALSE, fRegNum = FALSE;

    if (!gszLogNumber[0])
        fRegNum = TRUE;

    if (!gszPath[0]) {
        CopyMemory(gszPath, gszDefaultPath, sizeof(gszDefaultPath));
        fRegDir = TRUE;
    }


    // get the next available number to build log file names from the registry
    LONG status = RegOpenKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\speechapi\\TelephonyLogging",
                                NULL, KEY_ALL_ACCESS, &hKey );

    if (status == ERROR_SUCCESS) {
        dwSize = sizeof(DWORD);

        // get the log number from the registry
        if (fRegNum) {
            status = RegQueryValueEx( hKey, "LogNumber", NULL, &dwType,
                                      (PBYTE)&gdwNumber, &dwSize );

            if ((status != ERROR_SUCCESS) || (dwType != REG_DWORD))
                gdwNumber = 1;
        }

        // get the 'use log' indicator from the registry
        if (gfFirstTime) {
            status = RegQueryValueEx( hKey, "LoggingOn", NULL, &dwType,
                                      (PBYTE)&gfUseLogging, &dwSize );
            if (status != ERROR_SUCCESS)
                gfUseLogging = FALSE;

            gfFirstTime = FALSE;
        }

        // get the log file directory from the registry
        if (fRegDir) {
            dwSize = sizeof(gszPath);
            status = RegQueryValueEx( hKey, "LogFileDirectory", NULL, &dwType,
                                      (PBYTE)gszPath, &dwSize);

            if (status != ERROR_SUCCESS)
                CopyMemory(gszPath, gszDefaultPath, sizeof(gszDefaultPath));
        }

        RegCloseKey(hKey);
    }

    else if (status == ERROR_FILE_NOT_FOUND) {

        // start from scratch
        status = RegCreateKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\SpeechAPI\\TelephonyLogging",
                                 NULL, NULL, REG_OPTION_NON_VOLATILE,
                                 KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition );

        if (status != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return E_FAIL;
        }

        // start a new number
        dwSize = sizeof(DWORD);
        if (gdwNumber == 0)
            gdwNumber = 1;
        status = RegSetValueEx( hKey, "LogNumber", NULL, REG_DWORD,
                                (PBYTE)&gdwNumber, dwSize );

        // start a new 'use log' indicator
        dwSize = sizeof(DWORD);
        gfUseLogging = FALSE;
        status = RegSetValueEx( hKey, "LoggingOn", NULL, REG_DWORD,
                                (PBYTE)&gfUseLogging, dwSize );

        // new directory setting
        dwSize = lstrlen(gszPath) + 1;
        status = RegSetValueEx( hKey, "LogFileDirectory", NULL, REG_SZ,
                                (PBYTE)gszPath, dwSize );

        RegCloseKey(hKey);
        if (status != ERROR_SUCCESS)
            return E_FAIL;
    }

    return TRUE;
}


/********************************************************************
 CallLoggingDialog - Runs the dialog that gets the logging information.
                     The dialog is in spchtel.dll, so we need to load
                     it to run the dialog.

                     The reason it is not in here is because this is a
                     static library, and cannot have resources.
 */
#ifdef STRICT
HRESULT CTelMain::CallLoggingDialog(PVOID hwnd)
#else
HRESULT CTelMain::CallLoggingDialog(HWND hwnd)
#endif
{
    PSTR        pszBuf;
    DWORD       dwNeeded, dwType;
    HINSTANCE   hInst;
    LONG        status;
    HKEY        hKey;

	// open the key
    hKey = NULL;
	status = RegOpenKeyEx(HKEY_CLASSES_ROOT,
                          "CLSID\\{F9D18BF8-E0ED-11d0-AB8B-08002BE4E3B7}\\InprocServer32",
                          0, KEY_ALL_ACCESS, &hKey);
    if (status != ERROR_SUCCESS)
        goto CDError;

	dwNeeded = 0;
	status = RegQueryValueEx(hKey, NULL, NULL, &dwType, NULL, &dwNeeded);
	if (dwNeeded) {
	    pszBuf = (PSTR) ALLOC(dwNeeded);
	    if (pszBuf) {
            // get full path to spchtel.dll
    		status = RegQueryValueEx(hKey, NULL, NULL, &dwType,
					 (PBYTE)pszBuf, &dwNeeded);
		    if (status != ERROR_SUCCESS)
                goto CDError;
	    }
        else
            goto CDError;
	}
    else
        goto CDError;

    RegCloseKey(hKey);
    hKey = NULL;

    // load spchtel.dll
    hInst = LoadLibrary(pszBuf);
    if (hInst == NULL)
        goto CDError;

    // run the dialog
    DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_LOGGINGDIALOG), (HWND) hwnd,
                   (DLGPROC) LoggingDlgProc, (LONG)this);

    FREE (pszBuf);
    FreeLibrary(hInst);
    return NOERROR;

CDError:
    if (pszBuf)
        FREE (pszBuf);

    if (hKey)
        RegCloseKey(hKey);

    return E_FAIL;
}



/***************************************************************
 ***************************************************************
 CTelLine defaults
 */
HRESULT CTelLine::AbortControl()
{
    return NOERROR;
}

HRESULT CTelLine::Stop()
{
    return NOERROR;
}

HRESULT CTelLine::ThreadLoop()
{
    return NOERROR;
}

HRESULT CTelLine::SendDTMF(WCHAR wcKey)
{
    return NOERROR;
}


/***************************************************************
 ***************************************************************
 CCall defaults
 */
HRESULT CCall::Init(PITELINFO pITelInfo)
{
    return NOERROR;
}

HRESULT CCall::Shutdown()
{
    return NOERROR;
}

/****************************************************************
 DoQuickCreate - Calls the information objects QuickCreate function
                 telling it to use the defaults for the engines, and
                 to use logging.

                 If an application would like to do something different,
                 it can override this function.
 */
#ifdef STRICT
HRESULT CCall::DoQuickCreate(PITELINFO pITelInfo, PVOID hInst)
#else
HRESULT CCall::DoQuickCreate(PITELINFO pITelInfo, HINSTANCE hInst)
#endif
{
    SRMODEINFOW     srModeInfo, *psrMode;
    TTSMODEINFOW    ttsModeInfo, *pttsMode;
    HRESULT         hr;
    DWORD           dwSize, dwType;
    HKEY            hKey;

    ZeroMemory(&srModeInfo, sizeof(srModeInfo));
    ZeroMemory(&ttsModeInfo, sizeof(ttsModeInfo));

    // write out speech engine mode IDs
    LONG status = RegOpenKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\SpeechAPI\\TelephonyEngines",
                                NULL, KEY_ALL_ACCESS, &hKey );

    if (status == ERROR_FILE_NOT_FOUND)
        hr = pITelInfo->QuickCreate((HINSTANCE) hInst, NULL, NULL, NULL, TRUE);
    else {
        dwSize = sizeof(GUID);

        // get the SR mode ID from the registry
        status = RegQueryValueEx( hKey, "SR Mode", NULL, &dwType,
                                  (PBYTE)&srModeInfo.gModeID, &dwSize );

        if ((status == ERROR_SUCCESS) && (dwType == REG_BINARY))
            psrMode = &srModeInfo;
        else
            psrMode = NULL;

        // get the TTS mode ID from the registry
        status = RegQueryValueEx( hKey, "TTS Mode", NULL, &dwType,
                                  (PBYTE)&ttsModeInfo.gModeID, &dwSize );

        if ((status == ERROR_SUCCESS) && (dwType == REG_BINARY))
            pttsMode = &ttsModeInfo;
        else
            pttsMode = NULL;
        
        // close the key
        RegCloseKey (hKey);

        // call using the mode IDs we just got from the registry
        hr = pITelInfo->QuickCreate((HINSTANCE) hInst, psrMode, pttsMode, NULL, TRUE);
    }

    return hr;
}



// Choose engine dialog processing
//***************************************************************************
//***************************************************************************
//***************************************************************************

/********************************************************************
 SaveToRegistry - Saves the speech engines selected in the dialog to
                  the registry.

 */
HRESULT SaveToRegistry(PCLink pList)
{
    HKEY  hKey;
    DWORD dwSize, dwDisposition;

    PENGINEINFO peInfo;
    PVOID       pLink;

    // write out speech engine mode IDs and names
    LONG status = RegOpenKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\SpeechAPI\\TelephonyEngines",
                                NULL, KEY_ALL_ACCESS, &hKey );

    if (status == ERROR_FILE_NOT_FOUND) {
        // start from scratch
        status = RegCreateKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\SpeechAPI\\TelephonyEngines",
                                 NULL, NULL, REG_OPTION_NON_VOLATILE,
                                 KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition );

        if (status != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return E_FAIL;
        }
    }
    else if (status != ERROR_SUCCESS)
        return E_FAIL;

    pLink = NULL;
    while (TRUE) {
        peInfo = NULL;
        pLink = pList->GetNextElem(pLink, (PVOID *)&peInfo);
        if (peInfo) {
            if (lstrcmp(peInfo->szName, gszSREngine) == 0) {
                // save the SR Mode name
                dwSize = lstrlen(peInfo->szName) + 1;
                status = RegSetValueEx( hKey, "SR Name", NULL, REG_SZ,
                                        (PBYTE)peInfo->szName, dwSize );

                // save the SR Mode ID
                dwSize = sizeof(GUID);
                status = RegSetValueEx( hKey, "SR Mode", NULL, REG_BINARY,
                                        (PBYTE)&peInfo->gModeID, dwSize );
            }

            if (lstrcmp(peInfo->szName, gszTTSEngine) == 0) {
                // save the TTS Mode name
                dwSize = lstrlen(peInfo->szName) + 1;
                status = RegSetValueEx( hKey, "TTS Name", NULL, REG_SZ,
                                        (PBYTE)peInfo->szName, dwSize );

                // save the TTS Mode ID
                dwSize = sizeof(GUID);
                status = RegSetValueEx( hKey, "TTS Mode", NULL, REG_BINARY,
                                        (PBYTE)&peInfo->gModeID, dwSize );
            }
        }
        else
            break;
    }

    RegCloseKey(hKey);
    return NOERROR;
}


/********************************************************************
 FillBoxes - Propagates the drop down combo boxes with all the telephony
             capable speech engines on the system.

 Input - hwnd of dialog box that has the combo boxes
 */
PCLink FillBoxes(HWND hwnd)
{
    PISRENUM    pSREnum;
    PITTSENUM   pTTSEnum;
    HRESULT     hr;
    ULONG       dwFetched;
    PCLink      pList = new CLink();

    SRMODEINFO  srModeInfo;
    TTSMODEINFO ttsModeInfo;

    ENGINEINFO  eInfo;

    // SR enumerator object
    hr = CoCreateInstance(CLSID_SREnumerator, NULL, CLSCTX_INPROC_SERVER,
                          IID_ISREnum, (PVOID *)&pSREnum);
    if (hr != NOERROR)
        return NULL;

    while (TRUE) {
        hr = pSREnum->Next(1, &srModeInfo, &dwFetched);
        if (hr != NOERROR || dwFetched == 0)
            break;

        if (srModeInfo.dwFeatures & SRFEATURE_PHONEOPTIMIZED) {
            SendDlgItemMessage(hwnd, IDC_SRENGINE, CB_ADDSTRING, 0,
                               (LONG) srModeInfo.szModeName);

            memset(&eInfo, 0, sizeof(ENGINEINFO));
            memcpy(&eInfo.gModeID, &srModeInfo.gModeID, sizeof(GUID));
            memcpy(&eInfo.szName, srModeInfo.szModeName,
                                  lstrlen(srModeInfo.szModeName));
            pList->AddElem(&eInfo, sizeof(ENGINEINFO));
        }
    }

    pSREnum->Release();

    // TTS enumerator object
    hr = CoCreateInstance(CLSID_TTSEnumerator, NULL, CLSCTX_INPROC_SERVER,
                          IID_ITTSEnum, (PVOID *)&pTTSEnum);
    if (hr != NOERROR)
        return NULL;

    while (TRUE) {
        hr = pTTSEnum->Next(1, &ttsModeInfo, &dwFetched);
        if (hr != NOERROR || dwFetched == 0)
            break;

        if (ttsModeInfo.dwFeatures & TTSFEATURE_PHONEOPTIMIZED) {
            SendDlgItemMessage(hwnd, IDC_TTSENGINE, CB_ADDSTRING, 0,
                               (LONG) ttsModeInfo.szModeName);

            memset(&eInfo, 0, sizeof(ENGINEINFO));
            memcpy(&eInfo.gModeID, &ttsModeInfo.gModeID, sizeof(GUID));
            memcpy(&eInfo.szName, ttsModeInfo.szModeName,
                                  lstrlen(ttsModeInfo.szModeName));
            pList->AddElem(&eInfo, sizeof(ENGINEINFO));
        }
    }

    pTTSEnum->Release();

    return pList;
}




/********************************************************************
 SetCurrentSelection - Sets the current selection to whatever is in the
                       registry.

 Input - hwnd of dialog box that has the combo boxes
 */
void SetCurrentSelection(HWND hwnd)
{
    DWORD dwType, dwNeeded;
    HKEY  hKey;
    PSTR  pszSRBuf = 0, pszTTSBuf = 0;

    // read speech engine mode names
    LONG status = RegOpenKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\SpeechAPI\\TelephonyEngines",
                                NULL, KEY_ALL_ACCESS, &hKey );
    if (status != ERROR_SUCCESS) {
        SendDlgItemMessage (hwnd, IDC_SRENGINE, CB_SETCURSEL, 0, 0);
        SendDlgItemMessage (hwnd, IDC_TTSENGINE, CB_SETCURSEL, 0, 0);
        return;
    }


	dwNeeded = 0;
	status = RegQueryValueEx(hKey, "SR Name", NULL, &dwType, NULL, &dwNeeded);
	if (dwNeeded) {
	    pszSRBuf = (PSTR) ALLOC(dwNeeded);
	    if (pszSRBuf) {
            // get the SR mode name from the registry
            status = RegQueryValueEx( hKey, "SR Name", NULL, &dwType,
                                      (PBYTE)pszSRBuf, &dwNeeded );

            if (status != ERROR_SUCCESS) {
                FREE (pszSRBuf);
                pszSRBuf = 0;
            }
        }
    }

	dwNeeded = 0;
	status = RegQueryValueEx(hKey, "TTS Name", NULL, &dwType, NULL, &dwNeeded);
	if (dwNeeded) {
	    pszTTSBuf = (PSTR) ALLOC(dwNeeded);
	    if (pszTTSBuf) {
            // get the TTS mode name from the registry
            status = RegQueryValueEx( hKey, "TTS Name", NULL, &dwType,
                                      (PBYTE)pszTTSBuf, &dwNeeded );

            if (status != ERROR_SUCCESS) {
                FREE (pszTTSBuf);
                pszTTSBuf = 0;
            }
        }
    }

    RegCloseKey(hKey);

    SendDlgItemMessage (hwnd, IDC_SRENGINE, CB_SETCURSEL, 0, 0);
    if (pszSRBuf) {
        int iNum;
        iNum = SendDlgItemMessage(hwnd, IDC_SRENGINE, CB_FINDSTRINGEXACT, -1, (long)pszSRBuf);
        if (iNum >= 0)
           SendDlgItemMessage (hwnd, IDC_SRENGINE, CB_SETCURSEL, iNum, 0);
    }

    SendDlgItemMessage (hwnd, IDC_TTSENGINE, CB_SETCURSEL, 0, 0);
    if (pszTTSBuf) {
        int iNum;
        iNum = SendDlgItemMessage(hwnd, IDC_TTSENGINE, CB_FINDSTRINGEXACT, -1, (long)pszTTSBuf);
        if (iNum >= 0)
           SendDlgItemMessage (hwnd, IDC_TTSENGINE, CB_SETCURSEL, iNum, 0);
    }

    if (pszSRBuf)
        FREE(pszSRBuf);
    if (pszTTSBuf)
        FREE(pszTTSBuf);
}




/********************************************************************
 ChooseEngDlgProc - This dialog is for enumerating SR and TTS engines
                    that work over the phone, and letting the user pick
                    the one they want to use.
 */
BOOL CALLBACK ChooseEngDlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    static PCLink  pList = NULL;

    switch( msg ) {
        case WM_INITDIALOG:
            pList = FillBoxes(hwnd);
            SetCurrentSelection(hwnd);
            return FALSE;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDCANCEL:
                    if (pList)
                        delete pList;

                    EndDialog(hwnd, IDCANCEL);
                    return TRUE;

                case IDOK:
                    GetDlgItemText(hwnd, IDC_SRENGINE, gszSREngine, ENGINETEXT);
                    GetDlgItemText(hwnd, IDC_TTSENGINE, gszTTSEngine, ENGINETEXT);

                    if (pList) {
                        SaveToRegistry(pList);
                        delete pList;
                    }

                    EndDialog(hwnd, IDOK);
                    return TRUE;
            }
            return FALSE;
    }

    return FALSE;
}



/********************************************************************
 ChooseEngineDialog - Runs the dialog that gets the logging information.
                     The dialog is in spchtel.dll, so we need to load
                     it to run the dialog.

                     The reason it is not in here is because this is a
                     static library, and cannot have resources.

  Input - hwnd of window that owns dialog
 */
HRESULT ChooseEngineDialog(HWND hwnd)
{
    PSTR        pszBuf;
    DWORD       dwNeeded, dwType;
    HINSTANCE   hInst;
    LONG        status;
    HKEY        hKey;

	// open the key
    hKey = NULL;
	status = RegOpenKeyEx(HKEY_CLASSES_ROOT,
                          "CLSID\\{F9D18BF8-E0ED-11d0-AB8B-08002BE4E3B7}\\InprocServer32",
                          0, KEY_ALL_ACCESS, &hKey);
    if (status != ERROR_SUCCESS)
        goto CDError;

	dwNeeded = 0;
	status = RegQueryValueEx(hKey, NULL, NULL, &dwType, NULL, &dwNeeded);
	if (dwNeeded) {
	    pszBuf = (PSTR) ALLOC(dwNeeded);
	    if (pszBuf) {
            // get full path to spchtel.dll
    		status = RegQueryValueEx(hKey, NULL, NULL, &dwType,
					 (PBYTE)pszBuf, &dwNeeded);
		    if (status != ERROR_SUCCESS)
                goto CDError;
	    }
        else
            goto CDError;
	}
    else
        goto CDError;

    RegCloseKey(hKey);
    hKey = NULL;

    // load spchtel.dll
    hInst = LoadLibrary(pszBuf);
    if (hInst == NULL)
        goto CDError;

    // run the dialog
    DialogBox(hInst, MAKEINTRESOURCE(IDD_CHOOSEENGINEDIALOG), hwnd,
              (DLGPROC) ChooseEngDlgProc);

    FREE (pszBuf);
    FreeLibrary(hInst);
    return NOERROR;

CDError:
    if (pszBuf)
        FREE (pszBuf);

    if (hKey)
        RegCloseKey(hKey);

    return E_FAIL;
}
