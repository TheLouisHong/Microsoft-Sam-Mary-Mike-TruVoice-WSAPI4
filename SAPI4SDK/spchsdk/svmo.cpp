/***********************************************************************
Demo.Cpp - Code to quickly demo voice commands.



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
#include <initguid.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>

#include <speech.h>
#include "resource.h"

#define  TIMER_CHANGECOMMAND     (52)
#define  TIMER_CLEARRESULT       (53)

// Voice Command notifications
class CIVCmdNotifySink : public IVCmdNotifySink {
    private:
    DWORD   m_dwMsgCnt;
    HWND    m_hwnd;

    public:
    CIVCmdNotifySink(void);
    ~CIVCmdNotifySink(void);


    // IUnkown members that delegate to m_punkOuter
    // Non-delegating object IUnknown
    STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    // IVCmdNotifySink members
    STDMETHODIMP CommandRecognize (DWORD, PVCMDNAME, DWORD, DWORD, PVOID, 
                                   DWORD,PSTR, PSTR);
    STDMETHODIMP CommandOther     (PVCMDNAME, PSTR);
    STDMETHODIMP MenuActivate     (PVCMDNAME, BOOL);
    STDMETHODIMP UtteranceBegin   (void);
    STDMETHODIMP UtteranceEnd     (void);
    STDMETHODIMP CommandStart     (void);
    STDMETHODIMP VUMeter          (WORD);
    STDMETHODIMP AttribChanged    (DWORD);
    STDMETHODIMP Interference     (DWORD);
};
typedef CIVCmdNotifySink * PCIVCmdNotifySink;





/*************************************************************************
Globals */

HINSTANCE         ghInstance;                // instance handle
CIVCmdNotifySink  gVCmdNotifySink;
HWND              ghwndResultsDisplay = NULL;
HWND              ghwndDialog = NULL;
PIVOICECMD        gpIVoiceCommand = NULL;
PIVCMDDIALOGS     gpIVCmdDialogs = NULL;
PIVCMDMENU        gpIVCmdMenu = NULL;
char              *gpszCommands = NULL; // Commands
char              *gpszCurCommand = NULL;  // current command that looking at

char  gszDefaultSet[] = // default command set
   "Help\r\n"
   "Minimize window.\r\n"
   "Maximize window.\r\n"
   "What time is it?\r\n"
   "What day is it?\r\n"
   "Create a new file.\r\n"
   "Delete the current file\r\n"
   "Open a file\r\n"
   "Switch to Word.\r\n"
   "Switch to Excel.\r\n"
   "Switch to calculator.\r\n"
   "Change the background color.\r\n"
   "Go to sleep.\r\n"
   "Wake up.\r\n"
   "Print the document.\r\n"
   "Speak the text.\r\n"
   "Paste\r\n"
   "Copy\r\n";
 
 BOOL		bNonFatalShutDown = FALSE;
 int CheckNavigator(void);
 DWORD VCMDState(PIVCMDATTRIBUTES);

/*************************************************************************
BeginOLE - This begins the OLE and creates the voice commands object,
   registers with it, and creates a temporary menu.

inputs
   none
returns
   BOOL - TRUE if is succedes
*/

BOOL BeginOLE (void)
{
   DWORD    dwVer; // OLE version
   HRESULT  hRes;
   VCMDNAME VcmdName; // Command Name
   LANGUAGE Language; // language to use
   PIVCMDATTRIBUTES  pIVCmdAttributes;
    

   gpIVoiceCommand = NULL;
   gpIVCmdDialogs = NULL;
   gpIVCmdMenu = NULL;

// Initialize OLE

   SetMessageQueue(96);
   dwVer = CoBuildVersion();

   if (rmm != HIWORD(dwVer)) return FALSE;         // error

   if (FAILED(CoInitialize(NULL))) return FALSE;

// Create the voice commands object
if (CoCreateInstance(CLSID_VCmd,
   NULL, 
   CLSCTX_LOCAL_SERVER, 
   IID_IVoiceCmd, 
   (LPVOID *)&gpIVoiceCommand) != S_OK)
      goto fail;

// Get the dialogs object
hRes = gpIVoiceCommand->QueryInterface(
   IID_IVCmdDialogs, (LPVOID FAR *)&gpIVCmdDialogs);
if (hRes)
   goto fail;

// Register
hRes = gpIVoiceCommand->Register("", &gVCmdNotifySink,
      IID_IVCmdNotifySink, VCMDRF_ALLMESSAGES, NULL);
if (hRes)
   goto fail;

//The following code checks for a navigator app and 
//checks the state of voice commands

hRes = gpIVoiceCommand->QueryInterface(
   IID_IVCmdAttributes, (LPVOID FAR *)&pIVCmdAttributes);
if (pIVCmdAttributes) 
{
	pIVCmdAttributes->EnabledSet( TRUE );
  	pIVCmdAttributes->AwakeStateSet( TRUE );

   pIVCmdAttributes->Release();
};

// Create a menu object
lstrcpy (VcmdName.szApplication, "Voice Commands Demo");
lstrcpy (VcmdName.szState, "Main");
Language.LanguageID = LANG_ENGLISH;
lstrcpy (Language.szDialect, "US English");
hRes = gpIVoiceCommand->MenuCreate( &VcmdName,
     &Language,
     VCMDMC_CREATE_TEMP,
     &gpIVCmdMenu
     );
if (hRes)
   goto fail;

return TRUE;


   // else failed

fail:
   if (gpIVoiceCommand)
      gpIVoiceCommand->Release();
   if (gpIVCmdDialogs)
      gpIVCmdDialogs->Release();
   if (gpIVCmdMenu)
      gpIVCmdMenu->Release();
   gpIVoiceCommand = NULL;
   gpIVCmdDialogs = NULL;
   gpIVCmdMenu = NULL;
  
   return FALSE;
}


/*************************************************************************
EndOLE - This closes up the OLE and frees everything else.

inputs
   none
returns
   BOOL - TRUE if succede
*/

BOOL EndOLE (void)
{
// Free the interfacesd
   if (gpIVoiceCommand)
      gpIVoiceCommand->Release();
   if (gpIVCmdDialogs)
      gpIVCmdDialogs->Release();
   if (gpIVCmdMenu)
      gpIVCmdMenu->Release();
   gpIVoiceCommand = NULL;
   gpIVCmdDialogs = NULL;
   gpIVCmdMenu = NULL;

// Free up all of OLE

   CoUninitialize ();

   return TRUE;
}


/****************************************************************
NextCommand - This looks in the memory and finds the next command.

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
CHAR * NextCommand (CHAR *pszMemory, PCHAR *pBegin,
   DWORD *pdwSize)
{
DWORD i;

for( ;; ) {
   // try to find a non-newline
   while ((*pszMemory == '\n') || (*pszMemory == '\r')) {
      if (*pszMemory == '\0')
         return NULL;
      pszMemory++;
      };

   // Try to find a new-line
   for (i = 0;
      (pszMemory[i] != '\n') && (pszMemory[i] != '\r') && (pszMemory[i] != '\0');
      i++);
   if (!i) {
      if (!pszMemory[i])
         return NULL;   // end
      pszMemory++;
      continue;   // try again
      };

   // Else, we've found a string
   *pBegin = pszMemory;
   *pdwSize = i;
   return pszMemory + i;
   };
}



/*****************************************************************
GetCommands - Takes a block of memory containg command strings and
   converts it into a list of VCMDCOMMAND structures.

inputs
   char     *pszMemory - NULL terminated sting. Commands are
               separated by \n or \r.
   PSDATA   pData - This is filled in with a pointer to memory and
               size for the vcmdcommand structure. The memory
               must be freed by the caller with free().   
   DWORD    *pdwNumCommands - Filled with the number of commands
 */
BOOL GetCommands(char *pszMemory, PSDATA pData, DWORD *pdwNumCommands)
{
    PSTR pTemp;
    DWORD dwTotal, dwSize, dwSizeDesc, dwSizeCat;
    DWORD dwSizeCmd;
    PVCMDCOMMAND pCmd, pCmdNew;
    CHAR    *pszBegin;
    DWORD   dwCmdSize;
    DWORD   dwCmds = 0;  // Current count
    DWORD   dwCount = 1; // Command number
    char    szCat[] = "Main";

    dwTotal = dwSize = 0;

    pTemp = (PSTR)malloc(0);
    if (!pTemp)
        return FALSE;

    pCmd = (PVCMDCOMMAND)pTemp;
    for( ;; ) {
        pszMemory = NextCommand (pszMemory, &pszBegin, &dwCmdSize);
        if (!pszMemory)
            break;   // no more

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
        pCmdNew->dwCommandText = NULL;

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

        // we just added another command
        dwCmds++;
    }

    pData->pData = (PVOID)pCmd;
    pData->dwSize = dwTotal;
    *pdwNumCommands = dwCmds;
    return TRUE;
}


/************************************************************************
UseCommands - This accepts a NULL-termaintes string with commands
   separated by new-lines and loads them into the voice-menu object,
   replacing any old commands.

inputs
   char     *pszCommands - String.
   PIVCMDMENU  pMenu - Menu
returns
   HRESULT - error
*/
HRESULT UseCommands (char *pszCommands, PIVCMDMENU pMenu)
{
HRESULT  hRes;
SDATA    data;
DWORD    dwNum, dwStart;

hRes = pMenu->Deactivate ();
if (hRes) return hRes;

hRes = pMenu->Num (&dwNum);
if (hRes) return hRes;

if (dwNum)
   hRes = pMenu->Remove (1, dwNum, VCMD_BY_POSITION);
if (hRes) return hRes;

if (!GetCommands(pszCommands, &data, &dwNum))
   return ResultFromScode (E_OUTOFMEMORY);

hRes = pMenu->Add (dwNum, data, &dwStart);
if (hRes) return hRes;

// free memory
free (data.pData);

hRes = pMenu->Activate(ghwndDialog, 0);
return hRes;
}

/****************************************************************************
*	CheckNavigator:
*
*	Checks the registry entries to see if a navigator application
*	has been installed on the machine. If the Navigator is installed
*	CheckNavigator returns its state(0 [not running], 1 [running]) else if no 
*	navigator is found it returns -1.
****************************************************************************/
int CheckNavigator(void)
{
	HKEY  hKey;
	DWORD dwType=REG_DWORD, dwSize=sizeof(DWORD), dwVal;

	if( RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Voice", 0, KEY_READ, &hKey) != ERROR_SUCCESS )
		return -1;
	
	if( RegQueryValueEx (hKey, "UseSpeech", 0, &dwType, (LPBYTE)&dwVal, &dwSize) != ERROR_SUCCESS )
		return -1;
	
	RegCloseKey (hKey);

	return (int)dwVal;
}

/****************************************************************************
*	VCMDState:
*
*	Determines what listening state Voice Commands is in. Returns a int 
*	specifing a state( 0 [not listening state], 1 [sleep state], 2 [listening state]) 
*	or in case of error returns 3.
****************************************************************************/

DWORD VCMDState(PIVCMDATTRIBUTES pIVCmdAttributes)
{
 	DWORD dwAwake, dwEnabled;

	dwAwake = dwEnabled = 0;
	if((FAILED(pIVCmdAttributes->EnabledGet(&dwEnabled))) || (FAILED(pIVCmdAttributes->AwakeStateGet(&dwAwake))))
		 return 3;// function failed
	else
	{
		if(dwEnabled == 0)
			return 0; //not listening state
		else if(dwEnabled == 1 && dwAwake == 0)
			return 1; //sleep state
		else
			return 2; //listening state
	}
}

/**************************************************************************
 *  Voice Command notification objects
 **************************************************************************/

CIVCmdNotifySink::CIVCmdNotifySink (void)
{
    m_dwMsgCnt = 0;
}

CIVCmdNotifySink::~CIVCmdNotifySink (void)
{
// this space intentionally left blank
}

STDMETHODIMP CIVCmdNotifySink::QueryInterface (REFIID riid, LPVOID *ppv)
{
    *ppv = NULL;

    /* always return our IUnkown for IID_IUnknown */
    if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IVCmdNotifySink)) {
        *ppv = (LPVOID) this;
        return NOERROR;
    }

    // otherwise, cant find
    return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CIVCmdNotifySink::AddRef (void)
{
    // normally this increases a reference count, but this object
    // is going to be freed as soon as the app is freed, so it doesn't
    // matter
    return 1;
}

STDMETHODIMP_(ULONG) CIVCmdNotifySink::Release (void)
{
    // normally this releases a reference count, but this object
    // is going to be freed when the application is freed so it doesnt
    // matter
    return 1;
};


STDMETHODIMP CIVCmdNotifySink::CommandRecognize(DWORD dwID, PVCMDNAME pName,
   DWORD dwFlags, DWORD dwActionSize, PVOID pAction, DWORD dwNumLists,
   PSTR pszListValues, PSTR pszCommand)
{
// This is called when a recognition occurs for the current application

if (!ghwndResultsDisplay)
   return NOERROR;

SetWindowText (ghwndResultsDisplay,
   pszCommand ? pszCommand : "[Unrecognized]");

// Kill the timer & restart it
if (ghwndDialog) {
   KillTimer (ghwndDialog, TIMER_CLEARRESULT);
   SetTimer (ghwndDialog, TIMER_CLEARRESULT, 2000, NULL);
   };

return NOERROR;
}


STDMETHODIMP CIVCmdNotifySink::CommandOther(PVCMDNAME pName, PSTR pszCommand)
{
// This is called when a recognition occurs for another application,
// or an unknown recognition occurs

if (!ghwndResultsDisplay)
   return NOERROR;

SetWindowText (ghwndResultsDisplay,
   pszCommand ? pszCommand : "[Unrecognized]");

// Kill the timer & restart it
if (ghwndDialog) {
   KillTimer (ghwndDialog, TIMER_CLEARRESULT);
   SetTimer (ghwndDialog, TIMER_CLEARRESULT, 2000, NULL);
   };

    
return NOERROR;
}

STDMETHODIMP CIVCmdNotifySink::MenuActivate(PVCMDNAME pName, BOOL bActive)
{
// Called when a menu is activated or deactivated. We don't care

    return NOERROR;
}

STDMETHODIMP CIVCmdNotifySink::AttribChanged(DWORD dwAttribute)
{
// Called when an attribute changes. We don't care.
    return NOERROR;
}

STDMETHODIMP CIVCmdNotifySink::Interference(DWORD dwType)
{
// Called when audio interference is happening. We don't care.
    return NOERROR;
}

STDMETHODIMP CIVCmdNotifySink::CommandStart(void)
{
// Called when SR starts processing a command. We don't care.
    return NOERROR;
}

STDMETHODIMP CIVCmdNotifySink::UtteranceBegin(void)
{
// Called when an utterance begins. We don't care.
    return NOERROR;
}

STDMETHODIMP CIVCmdNotifySink::UtteranceEnd()
{
// Called when an utterance finishes. We don't care
    return NOERROR;
}

STDMETHODIMP CIVCmdNotifySink::VUMeter(WORD wLevel)
{
// Called for VU meter notifications. We don't care
    return NOERROR;
}


/*************************************************************************
ChangeProc
*/
BOOL CALLBACK ChangeProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      SetDlgItemText (hWnd, IDC_EDIT, gpszCommands);
      return FALSE;
   case WM_COMMAND:
         switch (LOWORD(wParam))
            {
            case IDOK:
               {
               char     *pszNew;
               DWORD    dwSize;
               // Throw out the old buffer & copy the
               // new one in. The set us to use it
               pszNew = (char*) malloc (dwSize =
                  GetWindowTextLength(GetDlgItem(hWnd, IDC_EDIT)) + 1);
               if (pszNew) {
                  GetDlgItemText (hWnd, IDC_EDIT, pszNew, dwSize);
                  free (gpszCommands);
                  gpszCommands = pszNew;
                  gpszCurCommand = pszNew;
                  if (UseCommands (gpszCommands, gpIVCmdMenu))
                     return 1;   // error

                  };
               EndDialog (hWnd, IDOK);
               }
               return TRUE;
            case IDCANCEL:
               EndDialog (hWnd, IDCANCEL);
               return TRUE;
            }
      break;
   };

return FALSE;  // didn't handle
}




/*************************************************************************
DialogProc
*/
BOOL CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      ghwndResultsDisplay = GetDlgItem (hWnd, IDC_HEARD);
      ghwndDialog = hWnd;

      if (UseCommands (gpszCommands, gpIVCmdMenu))
         return 1;   // error



      SetTimer (hWnd, TIMER_CHANGECOMMAND, 2000, NULL);
      PostMessage (hWnd, WM_TIMER, TIMER_CHANGECOMMAND, 0);
      return FALSE;
   case WM_COMMAND:
         switch (LOWORD(wParam))
            {
            case IDC_CHANGE:
               // Change commands dialog box
               DialogBox (ghInstance, MAKEINTRESOURCE(IDD_CHANGE),
                  hWnd, (DLGPROC) ChangeProc);
               return TRUE;
            case IDC_TRAIN:
               gpIVCmdDialogs->TrainGeneralDlg (hWnd, NULL);
               return TRUE;
            case IDCANCEL:
               EndDialog (hWnd, IDCANCEL);
               return TRUE;
            }
      break;
   case WM_TIMER:
      if (wParam == TIMER_CHANGECOMMAND) {
         char     *pszToDisplay;
         DWORD    dwSize;
         char     cTemp;

         // go to the next command
         if (!gpszCurCommand)
            gpszCurCommand = gpszCommands;
         gpszCurCommand = NextCommand (gpszCurCommand,
            &pszToDisplay, &dwSize);
         if (gpszCurCommand) {
            cTemp = pszToDisplay[dwSize];
            pszToDisplay[dwSize] = '\0';
            SetDlgItemText (hWnd, IDC_COMMAND, pszToDisplay);
            pszToDisplay[dwSize] = cTemp;
            };
         }
      else {
         // clear the static
         KillTimer (hWnd, TIMER_CLEARRESULT);
         SetDlgItemText (hWnd, IDC_HEARD, "");
         };
      return TRUE;
   case WM_DESTROY:
      KillTimer (hWnd, TIMER_CHANGECOMMAND);
      break;   // continue on
   };

return FALSE;  // didn't handle
}



/*************************************************************************
winmain - Windows main code.
*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
ghInstance = hInstance;

// try to begin ole

   if (!BeginOLE())
   {
      if(!bNonFatalShutDown)
      	MessageBox (NULL, "Can't open. OLE or a VoiceCommands call failed", NULL, MB_OK);
      return 1;
   }


// Create a menu out of the default
gpszCommands = (char*) malloc (strlen(gszDefaultSet)+1);
if (!gpszCommands)
   return 1;
strcpy (gpszCommands, gszDefaultSet);
gpszCurCommand = gpszCommands;

// Bring up the dialog box
   DialogBox (hInstance, MAKEINTRESOURCE(IDD_VCMD),
      NULL, (DLGPROC) DialogProc);

if (gpszCommands)
   free (gpszCommands);

// try to close ole
   if (!EndOLE())
      MessageBox (NULL, "Can't shut down OLE.", NULL, MB_OK);

   return 0;
}

