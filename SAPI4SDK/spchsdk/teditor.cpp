/********************************************************************
Editor.cpp - telephony control editor


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <speech.h>
#include <spchtel.h>
#include "resource.h"

void EnableEditAndMenu(HWND hwnd, UINT uEnable, BOOL fEnable);

extern HRESULT Test (void);
extern HRESULT GetCtrlID (GUID *);

/* defines */

#define IDC_CODE		0x2000


/* globals */
HINSTANCE   ghInstance;
HWND        ghWndEditor = NULL;
HWND        ghWndCode = NULL;
HWND        ghWndCompile = NULL;
BOOL        gfDirty = FALSE;
BOOL        gfIsOutOfSync = TRUE;
BOOL        gfCompiled = FALSE;
char        gszName[256] = "";
char        gszAppName[] = "Telephony Control Editor";

PITELINFO       gpITelInfo;
PITELCONTROL    gpITelControl = NULL;

/********************************************************************
SetTitle - Sets the title with the file name
*/
void SetTitle (void)
{
    if (gszName[0]) {
        char    szTemp[256];
        wsprintf (szTemp, "%s - %s", gszName, gszAppName);
        SetWindowText (ghWndEditor, szTemp);
    }
    else
        SetWindowText (ghWndEditor, gszAppName);
}


/********************************************************************
SendTextToCompiler - If the text in the edit buffer is dirty then
        this sets the text in the grammar compiler object. This will,
        of course, require a recompile
*/
void SendTextToCompiler (void)
{
    if (!gfIsOutOfSync)
    	return;
    gfIsOutOfSync = FALSE;
    gfCompiled = FALSE;

    CHAR	*paTemp;
    WCHAR	*pwTemp;
    DWORD	dwaSize, dwwSize;

    dwaSize = GetWindowTextLength(ghWndCode) + 1;
    paTemp = (CHAR*) malloc(dwaSize);
    if (!paTemp)
        return;

    dwwSize = dwaSize * 2;
    pwTemp = (WCHAR*) malloc(dwwSize);
    if (!pwTemp) {
        free (paTemp);
        return;
    }

    GetWindowText (ghWndCode, paTemp, dwaSize);

    dwwSize = 2 * MultiByteToWideChar (CP_ACP, 0, paTemp, -1,
                                       pwTemp, dwwSize / 2);

    if (gpITelControl)
        gpITelControl->FromMemory(pwTemp, dwwSize);

    free (paTemp);
    free (pwTemp);
}

/********************************************************************
ReallySaveIt - Save it.
*/
BOOL ReallySaveIt ()
{
    HRESULT hRes;

    SendTextToCompiler();
    WCHAR	szTemp[256];
    MultiByteToWideChar (CP_ACP,0, gszName, -1, szTemp, sizeof(szTemp)/2);

    // depending upon the ending of the file
    DWORD dwOffset;
    dwOffset = strlen (gszName);
    if (dwOffset >= 4)
        dwOffset -= 4;
    else
        dwOffset = 0;

    if (!stricmp(gszName + dwOffset, ".txt")) {
        // get the data
        LPWSTR  lpStr;
        DWORD   dwSize;

        if (gpITelControl) {
            hRes = gpITelControl->TextGet (&lpStr, &dwSize);
            if (hRes || !lpStr) {
		        MessageBox (ghWndEditor, "Can't save the file.", NULL, MB_OK);
                return FALSE;
            }

            // write as unicode

            /* open the file */
            FILE *f;
            f = fopen (gszName, "wb");
            if (!f) {
		        MessageBox (ghWndEditor, "Can't save the file as text.", NULL, MB_OK);
                CoTaskMemFree (lpStr);
                return FALSE;
            }
            WCHAR szHeader = 0xfeff;
            fwrite (&szHeader, 2, 1, f);
            fwrite (lpStr, 2, wcslen(lpStr), f);
            fclose (f);
            CoTaskMemFree (lpStr);
        }
    }
    else {
        if (gpITelControl) {
            if (gpITelControl->ToFile (szTemp)) {
		        MessageBox (ghWndEditor, "Can't save the file.", NULL, MB_OK);
		        return FALSE;
            }
        }
    }

    gfDirty = FALSE;
    SetTitle ();

    return TRUE;
}

/********************************************************************
SaveAs - Asks the user where to save the file. It then saves.

inputs
    none
returns
    BOOL - TRUE if all OK, FALSE if user cancelled
*/
BOOL SaveAs (void)
{
    OPENFILENAME	ofn;

    memset (&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = ghWndEditor;
    ofn.hInstance = ghInstance;
    ofn.lpstrFilter =
        "Telephony Control File (*.tpc)\0" "*.tpc\0"
        "Text File (*.txt)\0" "*.txt\0"
        "\0\0";
    ofn.lpstrFile = gszName;
    ofn.nMaxFile = sizeof(gszName);
    ofn.Flags = OFN_HIDEREADONLY; 
    ofn.lpstrDefExt = "tpc";

    if (!GetSaveFileName(&ofn))
        return FALSE;

    // fill in new file extension
    if (ofn.nFileExtension) {
        CHAR* pExt;
        switch (ofn.nFilterIndex) {
            case 2:
                pExt = "txt";
                break;

            case 1:
            default:
                pExt = "tpc";
        }
        strcpy (gszName + ofn.nFileExtension, pExt);
    }

    // Really save it
    return ReallySaveIt();
}



/********************************************************************
Save - Saves the file. If the file doens't have a name then it
    asks then it does a save-as.

inputs
    none
returns
    BOOL - TRUE if all OK, FALSE if user cancelled
*/
BOOL Save (void)
{
    if (!gszName[0])
        return SaveAs();

    return ReallySaveIt();
}


/********************************************************************
AskToSaveIfNecessary - Asks the user if they want to save any changes
    if the file is dirty. If it's not dirty they're not asked.

inputs
    none
returns
    BOOL - TRUE if everything OK, FALSE is not OK and pressed cancel
*/
BOOL AskToSaveIfNecessary (void)
{
    if (!gfDirty)
        return TRUE;

    int	iRet;
    iRet = MessageBox (ghWndEditor,
        "The file has been modified. Do you want to save it?",
        gszAppName,
        MB_YESNOCANCEL);

    if (iRet == IDNO)
        return TRUE;
    if (iRet == IDYES)
        return Save();

    // else cancelled
    return FALSE;
}


/********************************************************************
New - Clears out the buffer. If there's anything dirty, first
      asks the user if they want to save changes.

inputs
    none
returns
    BOOL - TRUE if everything OK, FALSE if is not OK
*/
BOOL New (void)
{
    GUID    guid;
    HRESULT hr;

    LPUNKNOWN pUnk;

    if (!AskToSaveIfNecessary())
        return FALSE;

    gfDirty = gfCompiled = FALSE;
    gfIsOutOfSync = TRUE;
    gszName[0] = 0;
    SetTitle();
    SetWindowText (ghWndCode, "");
    SetWindowText (ghWndCompile, "");

    hr = GetCtrlID(&guid);
    if (hr) {
        MessageBox(ghWndEditor, "Couldn't get control ID", "Error", MB_OK);
        return FALSE;
    }

    hr = CoCreateInstance(guid, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown,
                          (PVOID *)&pUnk);
    if (hr) {
        MessageBox(ghWndEditor, "Couldn't create control", "Error", MB_OK);
        return FALSE;
    }

    gpITelControl = (PITELCONTROL) pUnk;
    gpITelControl->ObjectSet(gpITelInfo);

    return TRUE;
}

/********************************************************************
ReallyOpenIt - Really opens the file.

returns
    BOOL - TRUE if everything OK, FALSE if cancel
*/
BOOL ReallyOpenIt (void)
{
    HRESULT hRes;
    WCHAR	szTemp[256];
    GUID    guid;

    LPUNKNOWN pUnk;
    HANDLE  hFile;

    // open the file and fail if it doens't exist
    hFile = CreateFile(gszName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return E_FAIL;

    // read in the first sizeof(GUID) bytes to see if this a control
    // or just a text file
    DWORD   dwRead;
    ReadFile (hFile, &guid, sizeof(GUID), &dwRead, NULL);
    CloseHandle (hFile);

    hRes = CoCreateInstance(guid, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown,
                            (PVOID *)&pUnk);
    if (hRes) {
        hRes = GetCtrlID(&guid);
        if (hRes) {
            MessageBox(ghWndEditor, "Couldn't get control ID", "Error", MB_OK);
            return FALSE;
        }

        hRes = CoCreateInstance(guid, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown,
                                (PVOID *)&pUnk);
        if (hRes) {
            MessageBox(ghWndEditor, "Couldn't create control", "Error", MB_OK);
            return FALSE;
        }
    }

    gpITelControl = (PITELCONTROL) pUnk;
    gpITelControl->ObjectSet(gpITelInfo);

    MultiByteToWideChar (CP_ACP,0, gszName, -1, szTemp, sizeof(szTemp)/2);
    hRes = gpITelControl->FromFile (szTemp);
    if (hRes) {
        MessageBox (ghWndEditor, "Can't open the file.", NULL, MB_OK);
        return FALSE;
    }

    // get the text, convert to ANSI, and send it
    SetWindowText (ghWndCode, "");
    SetWindowText (ghWndCompile, "");
    WCHAR   *pMem;
    DWORD   dwSize;

    hRes = gpITelControl->TextGet (&pMem, &dwSize);
    if (!hRes) {
        char    *pTemp;
        pTemp = (char*) malloc (dwSize);
        if (!pTemp)
            return FALSE;	// error

        WideCharToMultiByte (CP_ACP,0, pMem, -1, pTemp, dwSize, 0, NULL);
        SetWindowText (ghWndCode, pTemp);
        SendMessage (ghWndCode, EM_SETSEL, 0,0);
        free (pTemp);
        CoTaskMemFree (pMem);
    }


    gfDirty = FALSE;
    gfIsOutOfSync = FALSE;
    gpITelControl->IsCompiled(&gfCompiled);

    SetTitle ();
    EnableEditAndMenu(ghWndEditor, MF_BYCOMMAND | MF_ENABLED, TRUE);

    return TRUE;
}

/********************************************************************
Open - Opens a file. If another file is open it will ask the user
    if they want to save first.

inputs
    NONE
returns
    BOOL - TRUE if everything OK, FALSE if cancel
*/
BOOL Open (void)
{
    if (!AskToSaveIfNecessary())
        return FALSE;

    // 
    OPENFILENAME    ofn;

    memset (&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = ghWndEditor;
    ofn.hInstance = ghInstance;
    ofn.lpstrFilter =
        "Control or Text (*.tpc;*.txt)\0" "*.tpc;*.txt\0"
        "Telephony Control File (*.tpc)\0" "*.tpc\0"
        "Unicode text file (*.txt)\0" "*.txt\0"
        "\0\0";
    ofn.lpstrFile = gszName;
    ofn.nMaxFile = sizeof(gszName);
    ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST; 
    ofn.lpstrDefExt = "tpc";

    if (!GetOpenFileName(&ofn))
        return FALSE;

    // open it
    if (!ReallyOpenIt())
        return FALSE;

    return TRUE;
}


/********************************************************************
CompileTheGrammar - Compiles the grammar

inputs
    BOOL        fForce - If TRUE then it always re-compiles. If FALSE
                         if only recompiles if it's out of date.
returns
    BOOL - TRUE if compile succeded. FALSE if it failed
*/
BOOL CompileTheGrammar (BOOL fForce)
{
    // do we need to update the compiler with our status?
    SendTextToCompiler();

    if (!fForce && gfCompiled)
        return TRUE;	// already compiled

    // since we've already sent the text down, ask the grammar
    // compiler to compile
    HRESULT	hRes;
    WCHAR	*pRes;
    DWORD	dwSize;

    pRes = NULL;
    hRes = S_FALSE;
    if (gpITelControl)
        hRes = gpITelControl->Compile(&pRes, &dwSize);

    // show the text
    if (pRes) {
        char	*pTemp;

        pTemp = (char*) malloc (dwSize + 100);
        if (!pTemp)
            return FALSE;	// error

        WideCharToMultiByte (CP_ACP,0, pRes, -1, pTemp, dwSize, 0, NULL);
        SetWindowText (ghWndCompile, pTemp);

        if (hRes)
            strcat (pTemp, "\r\nCOMPILE FAILED!");

        free (pTemp);
        CoTaskMemFree (pRes);

        // Set selection at end
        SendMessage (ghWndCompile, EM_SETSEL, GetWindowTextLength(ghWndCompile),GetWindowTextLength(ghWndCompile));
        SendMessage (ghWndCompile, EM_SCROLLCARET, 0,0);
    }
    else
        SetWindowText (ghWndCompile, "");

    if (!hRes)
        gfCompiled = TRUE;

    return hRes ? FALSE : TRUE;
}


BOOL CALLBACK DlgDefTextProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    WCHAR   *pMem;
    DWORD   dwSize;
    HRESULT hRes;

    switch (msg) {
        case WM_INITDIALOG:
            if (gpITelControl) {
                hRes = gpITelControl->TextDefaultGet (&pMem, &dwSize);
                if (!hRes) {
                    if (dwSize) {
                        char    *pTemp;
                        pTemp = (char*) malloc (dwSize);
                        if (!pTemp)
                            return TRUE;    // error

                        WideCharToMultiByte (CP_ACP,0, pMem, -1, pTemp, dwSize, 0, NULL);
                        SetWindowText (GetDlgItem(hwnd, IDC_DEFAULTTEXT), pTemp);
                        SendMessage (GetDlgItem(hwnd, IDC_DEFAULTTEXT), EM_SETSEL, 0,0);
                        free (pTemp);
                        CoTaskMemFree (pMem);
                    }
                    else {
                        SetWindowText (GetDlgItem(hwnd, IDC_DEFAULTTEXT),
                                       "No default text");
                    }
                }
            }

            return FALSE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
                EndDialog(hwnd, IDOK);

            return TRUE;
    }

    return FALSE;
}




void EnableEditAndMenu(HWND hwnd, UINT uEnable, BOOL fEnable)
{
    EnableMenuItem(GetMenu(hwnd), IDM_FILE_SAVE, uEnable);
    EnableMenuItem(GetMenu(hwnd), IDM_FILE_SAVEAS, uEnable);
    EnableMenuItem(GetMenu(hwnd), IDM_FILE_EXIT, uEnable);
    EnableMenuItem(GetMenu(hwnd), IDM_GRAMMAR_COMPILE, uEnable);
    EnableMenuItem(GetMenu(hwnd), IDM_GRAMMAR_TEST, uEnable);
    EnableMenuItem(GetMenu(hwnd), IDM_DEFAULT, uEnable);
    EnableWindow(ghWndCode, fEnable);
}




/********************************************************************
EditorWndProc - Editor window proc
*/
LONG CALLBACK EditorWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg ) {
        case WM_SETFOCUS:
            SetFocus (ghWndCode);
            break;

        case WM_CREATE:
            ghWndCode = CreateWindowEx (
                WS_EX_CLIENTEDGE,
                "EDIT",
                "",
                WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | 
                ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN,
                1, 1, 1, 1,
                hwnd,
                (HMENU) IDC_CODE,
                ghInstance,
                NULL);
            ghWndCompile = CreateWindowEx (
                WS_EX_CLIENTEDGE,
                "EDIT",
                "",
                WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | 
                ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN |
                ES_READONLY,
                1, 1, 1, 1,
                hwnd,
                NULL,
                ghInstance,
                NULL);

            EnableEditAndMenu(hwnd, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED, FALSE);

            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDM_FILE_NEW:
                    if (New())
                        EnableEditAndMenu(hwnd, MF_BYCOMMAND | MF_ENABLED, TRUE);
                    break;

                case IDM_FILE_SAVE:
                    Save();
                    break;

                case IDM_FILE_SAVEAS:
                    SaveAs();
                    break;

                case IDM_FILE_OPEN:
                    if (Open())
                        EnableEditAndMenu(hwnd, MF_BYCOMMAND | MF_ENABLED, TRUE);
                    break;

                case IDM_FILE_EXIT:
                    PostMessage (hwnd, WM_CLOSE, 0, 0);
                    break;

                case IDM_GRAMMAR_COMPILE:
                    // Compile the grammar
                    if (!CompileTheGrammar(TRUE))
                        MessageBox (hwnd, "The compile failed.", NULL, MB_OK);
                    break;

                case IDM_DEFAULT:
                    DialogBox(ghInstance, MAKEINTRESOURCE(IDD_DEFAULTTEXT),
                              hwnd, (DLGPROC) DlgDefTextProc);
                    break;

                case IDM_GRAMMAR_TEST:
                    // Make sure the grammar is compiled
                    if (!CompileTheGrammar(FALSE)) {
                        MessageBox (hwnd, "The grammar compiled failed. You cannot test until it compiles.", NULL, MB_OK);
                        return 0;
                    }

                    // Run the test
                    {
                        HRESULT hRes;
                        hRes = Test();
                        if (hRes) {
                            char  szTemp[256];
                            wsprintf (szTemp, "The SR engine reported an error: 0x%x", (DWORD) hRes);
                            MessageBox (hwnd, szTemp, NULL, MB_OK);
                        }
                    }
                    break;

                case ID_HELP_HELP:
                    MessageBox (hwnd,
                    "The telephone control editor if for building telephony controls.\r\n"
                    "For information on the text format, look through the samples or "
                    "read the Grammar Compiler object documentation. (Located in the "
                    "Speech Tools section.)\r\n\r\n"
                    "Select New or Open to work on a telephony control",
                    "Help", MB_OK);
                    break;

                case IDC_CODE:
                    if (HIWORD(wParam) == EN_CHANGE) {
                        gfDirty = TRUE;
                        gfIsOutOfSync = TRUE;
                        gfCompiled = FALSE;
                    }
                    break;
            }
            break;

        case WM_CLOSE:
            // If dirty ask if want to save and if want to compile
            if (!AskToSaveIfNecessary())
                return 0;
            DestroyWindow (hwnd);
            return 0;

        case WM_SIZE:
            // move the editor and compiler window
            MoveWindow (ghWndCode, 0, 0,
                LOWORD(lParam), HIWORD(lParam) * 3 / 4, TRUE);
            MoveWindow (ghWndCompile, 0, HIWORD(lParam) * 3 / 4 + 3,
                LOWORD(lParam), max(0, (int) HIWORD(lParam) / 4 - 3), TRUE);
            return 0;

        case WM_DESTROY:
            PostQuitMessage (0);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/****************************************************************
 DoQuickCreate - Calls the information objects QuickCreate function
                 telling it to use the defaults for the engines, and
                 to use logging.

                 If an application would like to do something different,
                 it can override this function.
 */
HRESULT DoQuickCreate(PITELINFO pITelInfo, HINSTANCE hInst)
{
    SRMODEINFOW     srModeInfo, *psrMode;
    TTSMODEINFOW    ttsModeInfo, *pttsMode;
    HRESULT         hr;
    DWORD           dwSize, dwType;
    HKEY            hKey;

    ZeroMemory(&srModeInfo, sizeof(SRMODEINFO));
    ZeroMemory(&ttsModeInfo, sizeof(TTSMODEINFO));

    // write out speech engine mode IDs
    LONG status = RegOpenKeyEx( HKEY_CURRENT_USER,
                                "Software\\Microsoft\\SpeechAPI\\TelephonyEngines",
                                NULL, KEY_ALL_ACCESS, &hKey );

    if (status == ERROR_FILE_NOT_FOUND)
        hr = pITelInfo->QuickCreate(hInst, NULL, NULL, NULL, TRUE);
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

        // call using the mode IDs we just got from the registry
        hr = pITelInfo->QuickCreate(hInst, psrMode, pttsMode, NULL, TRUE);
    }

    if (hKey)
       RegCloseKey (hKey);

    return hr;
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

    // Set the current working directory
    if (!lpCmdLine || !lpCmdLine[0]) {
        char  szModule[256];
        char *pSlash;
        GetModuleFileName (hInstance, szModule, sizeof(szModule));
        for (pSlash = strchr(szModule,'\\'); pSlash && strchr(pSlash+1,'\\');
        pSlash = strchr(pSlash+1,'\\'));
        if (pSlash)
            *pSlash = 0;
        SetCurrentDirectory (szModule);
    }

    // register the windows class
    WNDCLASS    wc;
    char        szClass[] = "TEL Editor";

    memset (&wc, 0, sizeof(wc));
    wc.style = 0; //CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = EditorWndProc;
    wc.hInstance = ghInstance;
    wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE+1);
    wc.lpszMenuName = MAKEINTRESOURCE (IDM_EDITOR);
    wc.lpszClassName = szClass;
    wc.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_MAIN));
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    RegisterClass (&wc);

    // create the window
    ghWndEditor = CreateWindowEx (
        WS_EX_WINDOWEDGE,
        szClass,
        "TEL Editor",
        WS_OVERLAPPEDWINDOW | WS_SIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, ghInstance, NULL);
    if (!ghWndEditor)
        return -1;

    // init ole
    CoInitialize(NULL);

    // Create the grammar compiler object
    HRESULT	hRes;
    hRes = CoCreateInstance (CLSID_TelInfo, NULL, CLSCTX_INPROC_SERVER,
                             IID_ITelInfo, (void**)&gpITelInfo);
    if (hRes) {
        MessageBox (NULL, "Can't load the telephony information. You must have the 4.0 version of the API installed.\n", NULL, MB_OK);
        return 0;
    }

    DoQuickCreate(gpITelInfo, hInstance);

    // message loop
    MSG msg;
    SetTitle();
    ShowWindow( ghWndEditor, nCmdShow );
    UpdateWindow( ghWndEditor );

    // If there's a command line option then open up
    // the file immediately
    if (lpCmdLine && lpCmdLine[0]) {
        strcpy (gszName, lpCmdLine);
        ReallyOpenIt();
    }

    while( GetMessage(&msg, NULL, NULL, NULL) )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // finish with ole
    if (gpITelInfo)
        gpITelInfo->Release();
    if (gpITelControl)
        gpITelControl->Release();

    // done
    CoUninitialize ();
    return 0;
}
