/*******************************************************
test.cpp - main code.



Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/


#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <tchar.h>
#include <speech.h>
#include <spchtel.h>
#include "resource.h"
#include "link.h"

extern HINSTANCE ghInstance;
extern HWND ghWndEditor;
extern PITELCONTROL gpITelControl;
extern PITELINFO gpITelInfo;

// used for enumerating telephony controls from the registry
#define VALUENAMELEN    128
#define VALUEDATALEN    64

typedef struct {
    char    szName[VALUENAMELEN];
    GUID    gCtrlID;
} TELCONTROLINFO, *PTELCONTROLINFO;

static  _TCHAR  gszName[VALUENAMELEN];
static  GUID    ggCtrlID;



/* typedefs */
/* globals */
static   PISRCENTRAL        gpISRCentral = NULL;
static   PISRATTRIBUTES     gpISRAttributes = NULL;  // attributes
static   PISRGRAMCOMMON     gpISRGramCommon = NULL;
static   HWND               ghWndTest = NULL;

class CTestTelNotify : public ITelControlNotifySink {
    private:

    public:
        CTestTelNotify (void);
        ~CTestTelNotify (void);

        // IUnkown members that delegate to m_punkOuter
        // Non-delegating object IUnknown
        STDMETHODIMP            QueryInterface (REFIID, LPVOID FAR *);
        STDMETHODIMP_(ULONG)    AddRef(void);
        STDMETHODIMP_(ULONG)    Release(void);

        // ITelNotifySink
	    STDMETHODIMP Finish     (DWORD, PVOID, DWORD);
	    STDMETHODIMP Info       (DWORD, PVOID, DWORD);
};
typedef CTestTelNotify *PCTestTelNotify;

CTestTelNotify  gTestTelNotify;


/*************************************************************************
CTestTelNotify - Notification object. All this this will do is display
   its notification on the main window.
*/
CTestTelNotify::CTestTelNotify (void)
{
    // this space intentionally left blank
}

CTestTelNotify::~CTestTelNotify (void)
{
    // this space intentionally left blank
}

STDMETHODIMP CTestTelNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
    *ppv = NULL;

    /* always return our IUnkown for IID_IUnknown */
    if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITelControlNotifySink)) {
        *ppv = (LPVOID) this;
        return NOERROR;
    }

    // otherwise, cant find

    return E_NOINTERFACE;
}

STDMETHODIMP_ (ULONG) CTestTelNotify::AddRef (void)
{
    // normally this increases a reference count, but this object
    // is going to be freed as soon as the app is freed, so it doesn't
    // matter
    return 1;
}

STDMETHODIMP_(ULONG) CTestTelNotify::Release (void)
{
    // normally this releases a reference count, but this object
    // is going to be freed when the application is freed so it doesnt
    // matter
    return 1;
};


STDMETHODIMP CTestTelNotify::Finish(DWORD dwReturnInfo, PVOID pMemReturnInfo,
                                    DWORD dwSizeMem)
{
    PSTR pszBuf = NULL, pszTemp = NULL;

    if ((pMemReturnInfo && dwSizeMem) || dwReturnInfo) {
        if (dwSizeMem) {
            pszTemp = (PSTR) malloc(dwSizeMem + 1);
            if (!pszTemp)
                goto FinishExit;

            WideCharToMultiByte(CP_ACP, NULL, (PWSTR)pMemReturnInfo, -1, pszTemp,
                                dwSizeMem, NULL, NULL);
        }

        pszBuf = (PSTR) malloc(dwSizeMem + 50);
        if (!pszBuf) {
            free(pszTemp);
            goto FinishExit;
        }

        if (dwReturnInfo && dwSizeMem)
            wsprintf(pszBuf, "The telephony control return \"%d - %s\"", pszTemp, dwReturnInfo);
        if (dwReturnInfo && (!dwSizeMem))
            wsprintf(pszBuf, "The telephony control return \"%d\"", dwReturnInfo);
        if ((!dwReturnInfo) && dwSizeMem)
            wsprintf(pszBuf, "The telephony control return \"%s\"", pszTemp);
    }

    if (pszBuf) {
        MessageBox(NULL, pszBuf, "Result", MB_OK);
        free(pszBuf);
    }

    if (pszTemp)
       free (pszTemp);

FinishExit:
    PostMessage(ghWndTest, WM_COMMAND, IDCANCEL, 0);
    return NOERROR;
}

STDMETHODIMP CTestTelNotify::Info(DWORD dwReturnInfo, PVOID pMemReturnInfo,
                                  DWORD dwSizeMem)
{
    return NOERROR;
}




PCLink ListControlsGet (HWND hwnd, DWORD dwCtrlID)
{
// defines are for re-setting Reg...(...) certain args used as both in's & out's:

    HKEY            hKeyTelCtrls;
    PCLink          pList = new CLink();

    DWORD           dwData;
    DWORD           dwType;
    DWORD           dwValueNameLen;
    CHAR            szValueName[VALUENAMELEN];
    INT             iValue;
    HRESULT         hRes;
    LONG            lRes;
    CHAR            szValue[VALUEDATALEN];
    WCHAR           wszValue[VALUEDATALEN];
    GUID            guid;

    TELCONTROLINFO  tci;

    // Set our key
    lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\SpeechAPI\\TelephonyControls",
                        0, KEY_READ, &hKeyTelCtrls);

    // empty list OK
    if (lRes != ERROR_SUCCESS)
        return pList;

    // Loop through values.
    iValue         = 0;
    dwValueNameLen = VALUENAMELEN;
    dwData         = VALUEDATALEN;

    while ((hRes = RegEnumValue(hKeyTelCtrls, iValue, szValueName,
                                &dwValueNameLen, NULL, &dwType,
                                (unsigned char *) szValue, &dwData))
                                == ERROR_SUCCESS) {

        if (dwType == REG_SZ && dwValueNameLen <= VALUENAMELEN) {

            MultiByteToWideChar(CP_ACP, 0, szValue, -1, wszValue,
                                sizeof(wszValue) / sizeof(WCHAR));

            CLSIDFromString((unsigned short *)wszValue, &guid);

            SendDlgItemMessage(hwnd, dwCtrlID, CB_ADDSTRING, 0, (LONG) szValueName);

            memset(&tci, 0, sizeof(tci));
            memcpy(&tci.gCtrlID, &guid, sizeof(guid));
            memcpy(&tci.szName, szValueName, dwValueNameLen);
            pList->AddElem(&tci, sizeof(tci));

            dwValueNameLen = VALUENAMELEN;
            dwData         = VALUEDATALEN;
        }

        iValue++;
    }

    RegCloseKey(hKeyTelCtrls);
    return pList;
}


/*******************************************************
_TestDlgProc - Do the testing.

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK _TestDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_INITDIALOG:
            // remember this window
            ghWndTest = hWnd;
            if (gpITelControl)
                gpITelControl->Start(&gTestTelNotify);

        break;   // continue on

     case WM_CLOSE:
       gpITelInfo->SendAbort(0);
       return TRUE;


    case WM_COMMAND:
        switch (LOWORD(wParam)) {
            case IDOK:
            case IDCANCEL:
                EndDialog (hWnd, IDCANCEL);
                return TRUE;

            case IDC_BUTTON1:
                gpITelInfo->SendDTMF(L'1');
                return TRUE;

            case IDC_BUTTON2:
                gpITelInfo->SendDTMF(L'2');
                return TRUE;

            case IDC_BUTTON3:
                gpITelInfo->SendDTMF(L'3');
                return TRUE;

            case IDC_BUTTON4:
                gpITelInfo->SendDTMF(L'4');
                return TRUE;

            case IDC_BUTTON5:
                gpITelInfo->SendDTMF(L'5');
                return TRUE;

            case IDC_BUTTON6:
                gpITelInfo->SendDTMF(L'6');
                return TRUE;

            case IDC_BUTTON7:
                gpITelInfo->SendDTMF(L'7');
                return TRUE;

            case IDC_BUTTON8:
                gpITelInfo->SendDTMF(L'8');
                return TRUE;

            case IDC_BUTTON9:
                gpITelInfo->SendDTMF(L'9');
                return TRUE;

            case IDC_BUTTON10:
                gpITelInfo->SendDTMF(L'*');
                return TRUE;

            case IDC_BUTTON11:
                gpITelInfo->SendDTMF(L'0');
                return TRUE;

            case IDC_BUTTON12:
                gpITelInfo->SendDTMF(L'#');
                return TRUE;

            case IDC_BUTTON13:
                gpITelInfo->SendAbort(0);
                return TRUE;

		    default:
			    return FALSE;
        };
        break;
    };

    return FALSE;  // didn't handle
}


/*******************************************************
_TestDlgProc - Do the testing.

standard dlg-proc inputs & outputs
*/
BOOL CALLBACK _GetIDDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static PCLink  pList;

    switch (uMsg) {
        case WM_INITDIALOG:
            // remember this window
            ghWndTest = hWnd;

            pList = ListControlsGet(hWnd, IDC_CONTROLLIST);
            SendDlgItemMessage (hWnd, IDC_CONTROLLIST, CB_SETCURSEL, 0, 0);
            gszName[0] = 0;
            break;   // continue on

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK:
                    GetDlgItemText(hWnd, IDC_CONTROLLIST, gszName, VALUENAMELEN);
                    if (pList) {
                        // loop through the list looking for the name
                        // so we can save the guid
                        PVOID pLink = NULL;
                        PTELCONTROLINFO ptci = NULL;

                        ZeroMemory(&ggCtrlID, sizeof(GUID));
                        while (TRUE) {
                            pLink = pList->GetNextElem(pLink, (PVOID *)&ptci);
                            if (!pLink)
                                break;

                            if (lstrcmp(gszName, ptci->szName) == 0) {
                                ggCtrlID = ptci->gCtrlID;
                                break;
                            }
                        }
                    }

                case IDCANCEL:
                    if (pList)
                        delete pList;

                    EndDialog (hWnd, IDCANCEL);
                    return TRUE;

		        default:
			        return FALSE;
            }
        break;
    }

return FALSE;  // didn't handle
}


/*******************************************************
Test - Do the test

inputs
returns
   HRESULT - error
*/
HRESULT Test (void)
{
    // pull up test dialog
    DialogBox (ghInstance, MAKEINTRESOURCE(IDD_CONTROLDIALOG),
               ghWndEditor, (DLGPROC) _TestDlgProc);

    return NOERROR;
}



/************************************************************
GetCtrlID - Gets the GUID of a control by presenting the user with a
            dialog box that gets propagated with information about
            all the controls in the registry

inputs
    GUID * - pointer to a guid that this function fills in
returns
    HRESULT - error
*/
HRESULT GetCtrlID (GUID *pgCtrlID)
{
    ZeroMemory (pgCtrlID, sizeof(GUID));

    DialogBox (ghInstance, MAKEINTRESOURCE(IDD_REGCONTROLS),
               ghWndEditor, (DLGPROC) _GetIDDlgProc);

    *pgCtrlID = ggCtrlID;
    return NOERROR;
}