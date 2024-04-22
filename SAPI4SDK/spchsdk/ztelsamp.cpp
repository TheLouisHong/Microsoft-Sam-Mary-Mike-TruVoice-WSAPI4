/********************************************************************
TelSamp.cpp - Sample telephony application

Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <objbase.h>
#include <objerror.h>
#include <initguid.h>
#include <dsound.h>
#include <speech.h>
#include <spchwrap.h>
#include "resource.h"
#include "pizza.h"

//extern HRESULT Test (void);
//extern HRESULT GetCtrlID (GUID *);


/* globals */
//HWND            ghWnd, ghWndButton;
HINSTANCE       ghInstance;
PCTELMAINTAPIPIZZA gpTapiObj;
PCTELMAINPCPIZZA   gpPCObj;

PCTELMAIN       gpTelObj;

DWORD           gdwCallType = 0;


/* child window identifiers */
//#define     IDB_CLOSEBUTTON     0x00008280


#define         CALLTYPE_TAPI       0x00000001
#define         CALLTYPE_PC         0x00000002


HRESULT CTelMainTAPIPizza::CreateLineObject(PCCALL *ppCall)
{
    *ppCall = new CCallPizza();
    if (!(*ppCall))
        return E_OUTOFMEMORY;
    else {
        return NOERROR;
    }
}


HRESULT CTelMainPCPizza::CreateLineObject(PCCALL *ppCall)
{
    *ppCall = new CCallPizza();
    if (!(*ppCall))
        return E_OUTOFMEMORY;
    else {
        return NOERROR;
    }
}


/***************************************************************
StartMonitor - Starts the monitoring process
*/
void StartMonitor(HWND hwnd)
{
    HRESULT hr;

    SetDlgItemText(hwnd, IDC_TAPISTATE, "Initializing...");
    hr = gpTelObj->Init(hwnd, ghInstance);
    if (hr)
        goto SMError;

    gpTelObj->CallDialog();
    SetDlgItemText(hwnd, IDC_TAPISTATE, "");

    gpTelObj->Shutdown();
    return;

SMError:
    SetDlgItemText(hwnd, IDC_TAPISTATE, "");
    MessageBox(NULL, "Couldn't initiate monitor", "Error", MB_OK);
    gpTelObj->Shutdown();
}



/********************************************************************
MainDlgProc - Main window procedure
*/
BOOL CALLBACK MainDlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg ) {
        case WM_INITDIALOG:
           CheckDlgButton (hwnd, IDC_USEPC, BST_CHECKED);
           break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDCANCEL:
                    EndDialog(hwnd, IDCANCEL);
                    return TRUE;

                case IDC_SELECTENGINES:
                    ChooseEngineDialog(hwnd);
                    break;

                case IDC_MAKECALL:
                    if (IsDlgButtonChecked(hwnd, IDC_USETAPI))
                       gdwCallType = CALLTYPE_TAPI;
                    else
                       gdwCallType = CALLTYPE_PC;

                    if (gdwCallType == CALLTYPE_TAPI) {
                        if (!gpTapiObj) {
                            gpTapiObj = new CTelMainTAPIPizza();
                            if (!gpTapiObj)
                                break;

                            gpTelObj = gpTapiObj;
                            StartMonitor(hwnd);

                            delete gpTapiObj;
                            gpTapiObj = NULL;
                        }
                    }
                    else if (gdwCallType == CALLTYPE_PC) {
                        if (!gpPCObj) {
                            gpPCObj = new CTelMainPCPizza();
                            if (!gpPCObj)
                                break;

                            gpTelObj = gpPCObj;
                            StartMonitor(hwnd);

                            delete gpPCObj;
                            gpPCObj = NULL;
                        }
                    }
                    else
                        MessageBox(hwnd, "Must select call type", "Error", MB_OK);

                    return TRUE;
            }
            return FALSE;
    }
    return FALSE;
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

    CoInitialize(NULL);
    DialogBox(ghInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL,
              (DLGPROC)MainDlgProc);

    // done
    CoUninitialize();
    return 0;
}
