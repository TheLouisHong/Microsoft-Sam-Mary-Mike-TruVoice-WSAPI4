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
#include "answer.h"


/* globals */
HINSTANCE           ghInstance;
PCTELMAINTAPIAnswer gpTapiObj;
PCTELMAINPCAnswer   gpPCObj;

PCTELMAIN           gpTelObj;

DWORD               gdwCallType = 0, gdwFileNum;
char                gszDirectory[DIRSIZE];

char                *gpszName[PEOPLECOUNT] = {"Jill", "Larry", "Alison",
                                             "Elliot", "Jennifer"};

WCHAR               *gpwszName[PEOPLECOUNT] = {L"Jill", L"Larry", L"Alison",
                                               L"Elliot", L"Jennifer"};

WCHAR               *gpwszPass[PEOPLECOUNT] = {L"12225", L"04025", L"03168",
                                               L"07318", L"11239"};


#define         CALLTYPE_TAPI       0x00000001
#define         CALLTYPE_PC         0x00000002


HRESULT CTelMainTAPIAnswer::CreateLineObject(PCCALL *ppCall)
{
    *ppCall = new CCallAnswer();
    if (!(*ppCall))
        return E_OUTOFMEMORY;
    else {
        return NOERROR;
    }
}


HRESULT CTelMainPCAnswer::CreateLineObject(PCCALL *ppCall)
{
    *ppCall = new CCallAnswer();
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

    // asnwer after 4 rings
    gpTelObj->m_dwAnswerAfterRings = 4;

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
                            gpTapiObj = new CTelMainTAPIAnswer();
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
                            gpPCObj = new CTelMainPCAnswer();
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



BOOL GetFileNumber()
{
    HKEY  hKey;
    DWORD dwType, dwDisposition, dwSize;

    // get the next available number to build message file names from the registry
    LONG status = RegOpenKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\speechapi\\AnswerMachine",
                                NULL, KEY_ALL_ACCESS, &hKey );

    if (status == ERROR_SUCCESS) {
        dwSize = sizeof(DWORD);

        // get the message file number from the registry
        status = RegQueryValueEx( hKey, "FileNumber", NULL, &dwType,
                                  (PBYTE)&gdwFileNum, &dwSize );

        if ((status != ERROR_SUCCESS) || (dwType != REG_DWORD))
            gdwFileNum = 1;

        RegCloseKey(hKey);
    }

    else if (status == ERROR_FILE_NOT_FOUND) {

        // start from scratch
        status = RegCreateKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\SpeechAPI\\AnswerMachine",
                                 NULL, NULL, REG_OPTION_NON_VOLATILE,
                                 KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition );

        if (status != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return FALSE;
        }

        // start a new number
        dwSize = sizeof(DWORD);
        gdwFileNum = 1;
        status = RegSetValueEx( hKey, "FileNumber", NULL, REG_DWORD,
                                (PBYTE)&gdwFileNum, dwSize );

        RegCloseKey(hKey);
        if (status != ERROR_SUCCESS)
            return FALSE;
    }

    return TRUE;
}



BOOL SaveFileNumber()
{
    HKEY  hKey;
    DWORD dwSize;

    // get the next available number to build log file names from the registry
    LONG status = RegOpenKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\speechapi\\AnswerMachine",
                                NULL, KEY_ALL_ACCESS, &hKey );

    if (status == ERROR_SUCCESS) {
        // save the file number to the registry
        dwSize = sizeof(DWORD);
        status = RegSetValueEx( hKey, "FileNumber", NULL, REG_DWORD,
                                (PBYTE)&gdwFileNum, dwSize );

        RegCloseKey(hKey);
    }

    return TRUE;
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
    DWORD i;

    CoInitialize(NULL);
    ghInstance = hInstance;

    DWORD dwLen = GetModuleFileName(hInstance, gszDirectory, DIRSIZE);
    if (!dwLen)
        return -1;

    while (dwLen--) {
        if (gszDirectory[dwLen] == '\\') {
            gszDirectory[dwLen+1] = 0;
            break;
        }
    }

    // create master message directory
    lstrcat(gszDirectory, "Messages\\");
    BOOL fCreate = CreateDirectory(gszDirectory, NULL);

    // now create the sub directories for the individuals
    char szBuf[DIRSIZE];
    for (i = 0; i < PEOPLECOUNT; i++) {
        ZeroMemory(szBuf, DIRSIZE);
        CopyMemory(szBuf, gszDirectory, lstrlen(gszDirectory));
        lstrcat(szBuf, gpszName[i]);
        CreateDirectory(szBuf, NULL);
    }

    GetFileNumber();

    DialogBox(ghInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL,
              (DLGPROC)MainDlgProc);

    SaveFileNumber();

    // done
    CoUninitialize();
    return 0;
}
