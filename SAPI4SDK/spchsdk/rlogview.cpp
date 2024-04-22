/***********************************************************************
LogView.Cpp - Code that opens a log file and allows users to view them.


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
#include <mmsystem.h>
#include <objbase.h>
#include <initguid.h>
#include <objerror.h>
#include <speech.h>
#include "ctools.h"
#include "logfile.h"
#include "resource.h"

HINSTANCE   ghInstance = NULL;
CLogFile    gLF;  // log view object
CList       gListInBox;
DWORD       gdwCurPosn = 0;   // current element looking at

#define  MAX_DETAIL     4

/*************************************************************************
ShowInfo - Based on the current selection in the list box, this shows
   the info for that section.

inputs
   HWND  hWnd - Dialog window
returns
   none
*/
void ShowInfo (HWND hWnd)
{
   // get the current selection
   DWORD dwCurSel;
   dwCurSel = SendDlgItemMessage (hWnd, IDC_MESSAGES, LB_GETCURSEL, 0, 0);

   // look this up and find it in the list
   PDWORD   pdw;
   pdw = (PDWORD) gListInBox.GetElem (dwCurSel);
   if (!pdw)
      return;

   // find the element number from this
   DWORD dwElem;
   dwElem = *pdw;
   gdwCurPosn = dwElem;

   // start filling in the info
   char  szHuge[256*256];  // really huge buffer for conversions
   PCWSTR   pszW;

   // source
   pszW = gLF.GetSource(dwElem);
   if (pszW) {
      szHuge[0] = 0;
      WideCharToMultiByte (CP_ACP, 0, pszW, -1,
         szHuge, sizeof(szHuge), 0, 0);
      SetDlgItemText (hWnd, IDC_SOURCE, szHuge);
   }

   // message
   pszW = gLF.GetMessage(dwElem);
   if (pszW) {
      szHuge[0] = 0;
      WideCharToMultiByte (CP_ACP, 0, pszW, -1,
         szHuge, sizeof(szHuge), 0, 0);
      SetDlgItemText (hWnd, IDC_MESSAGE, szHuge);

      // enable/disable the play audio button
      char  szUtt[] = "utter";
      char  szPhrase[] = "phrase";
      char  szWave[] = "wave";
      BOOL  fEnable = FALSE;
      fEnable |= !_strnicmp(szHuge, szUtt, strlen(szUtt));
      fEnable |= !_strnicmp(szHuge, szPhrase, strlen(szPhrase));
      fEnable |= !_strnicmp(szHuge, szWave, strlen(szWave));
      EnableWindow (GetDlgItem (hWnd, IDC_PLAY), fEnable );
   }

   // detail
   WORD  wDetail;
   wDetail = gLF.GetDetail(dwElem);
   wsprintf (szHuge, "%d", (int) wDetail);
   SetDlgItemText (hWnd, IDC_DETAIL, szHuge);

   // time
   FILETIME ft; //, lft;
   SYSTEMTIME  st;
   if (gLF.GetFileTime(dwElem, &ft)) {
      // FileTimeToLocalFileTime (&ft, &lft);
      FileTimeToSystemTime (&ft, &st);
      wsprintf (szHuge, "%d:%02d:%02d on %d/%d/%d",
         (int)st.wHour, (int)st.wMinute, (int)st.wSecond,
         (int)st.wMonth, (int)st.wDay, (int)st.wYear);
      SetDlgItemText (hWnd, IDC_TIME, szHuge);
   }

   // memory
   char  *pCur;
   szHuge[0] = '\0';
   pCur = szHuge;
   PBYTE pMem;
   DWORD dwSize;
   pMem = (PBYTE) gLF.GetData (dwElem, &dwSize);
   if (pMem && dwSize) {
      DWORD i;
      for (i = 0; i < dwSize; i++) {
         wsprintf (pCur, "%02x ", (int) pMem[i]);
         pCur += strlen(pCur);
      }
   }
   SetDlgItemText (hWnd, IDC_DATA, szHuge);

}

/*************************************************************************
FillListBox - Fills in the list box with a new list of elements depending
   upon the detail level.

inputs
   HWND     hWndList - list window
   WORD     wDetail - Detail level to use
returns
   none
*/
void FillListBox (HWND hWndList, WORD wDetail)
{
   // clear out the list box
   SendMessage (hWndList, LB_RESETCONTENT, 0, 0);
   SendMessage (hWndList, LB_INITSTORAGE, gLF.GetNumElems(),
      gLF.GetNumElems() * 100);
   while (gListInBox.RemoveElem(0));

   // fill it in
   DWORD i, dwMax;
   dwMax = gLF.GetNumElems();
   for (i = 0; i < dwMax; i++) {
      WORD  wD;
      wD = gLF.GetDetail(i);
      if (wD > wDetail)
         continue;   // not viewing with that much detail

      // get the name and convert
      char  szMessage[256];
      szMessage[0] = 0;
      WideCharToMultiByte (CP_ACP, 0, gLF.GetMessage(i), -1,
         szMessage, sizeof(szMessage), 0, 0);
      SendMessage (hWndList, LB_ADDSTRING, 0, (LPARAM) szMessage);
      gListInBox.AddElem (&i, sizeof(i));
   }

   // Select item 0, and display properties
   SendMessage (hWndList, LB_SETCURSEL, 0, 0);
   ShowInfo(GetParent(hWndList));
}

/*************************************************************************
DialogProc
*/
BOOL CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch (uMsg) {
   case WM_INITDIALOG:
      // Set intial scroll bar stuff
      SetScrollRange(GetDlgItem(hWnd, IDC_DETAILSCROLL), SB_CTL,
         1, MAX_DETAIL, FALSE);
      SetScrollPos(GetDlgItem(hWnd, IDC_DETAILSCROLL), SB_CTL,
         1, TRUE);

      // Fill message box with list
      FillListBox(GetDlgItem(hWnd, IDC_MESSAGES), 1);
      break;
   case WM_HSCROLL:
      {
      // Handle scroll bars
      HWND  hWndScroll = GetDlgItem(hWnd, IDC_DETAILSCROLL);
      int   iPos;
      iPos = GetScrollPos (hWndScroll, SB_CTL);
   
      switch (LOWORD(wParam)) {
      case SB_BOTTOM:
         iPos++;
         break;
      case SB_ENDSCROLL:
         return TRUE;   // do nothing
      case SB_LINELEFT:
      case SB_PAGELEFT:
         iPos--;
         break;
      case SB_LINERIGHT:
      case SB_PAGERIGHT:
         iPos++;
         break;
      case SB_THUMBTRACK:
         return TRUE;
      case SB_THUMBPOSITION:
         iPos = HIWORD(wParam);
         break;
      case SB_TOP:
         iPos = 1;
         break;
      }

      if (iPos < 1)
         iPos = 1;
      if (iPos > MAX_DETAIL)
         iPos = MAX_DETAIL;
      SetScrollPos (hWndScroll, SB_CTL, iPos, TRUE);
      FillListBox (GetDlgItem(hWnd, IDC_MESSAGES), (WORD) iPos);
      return TRUE;
      }  
   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDC_MESSAGES:
         ShowInfo(hWnd);
         break;
      case IDC_PLAY:
         // terminate the existing sound
         sndPlaySound (NULL, 0);

         // write the file
         if (!gLF.MakeWave (gdwCurPosn, "c:\\lvtemp.wav")) {
            MessageBox (hWnd, "Can't find anything to play.", NULL, MB_OK);
            return TRUE;
         }

         // play it
         sndPlaySound ("c:\\lvtemp.wav", SND_ASYNC | SND_NODEFAULT );
         break;
      }
      break;
   case WM_CLOSE:
      EndDialog(hWnd, IDOK);
      return TRUE;
   };

return FALSE;  // didn't handle
}

/********************************************************************
OpenALog - Opens a log file.

inputs
	char     *pszFile - File name to write into
returns
	BOOL - TRUE if everything OK, FALSE if cancel
*/
BOOL OpenALog (char *pszFile)
{
	OPENFILENAME	ofn;

	memset (&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
   ofn.lpstrTitle = "Select the log file to open.";
	ofn.hwndOwner = NULL;
	ofn.hInstance = ghInstance;
	ofn.lpstrFilter =
		"Log File (*.stl)\0" "*.stl\0"
		"\0\0";
	ofn.lpstrFile = pszFile;
	ofn.nMaxFile = 256;
	ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST; 
	ofn.lpstrDefExt = "stl";

	if (!GetOpenFileName(&ofn))
		return FALSE;

	// open it
	return TRUE;
}


/*************************************************************************
winmain - Windows main code.
*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
   ghInstance = hInstance; 
   char  szTemp[256];

   char  szModule[256];
   char *pSlash;
   GetModuleFileName (hInstance, szModule, sizeof(szModule));
   for (pSlash = strchr(szModule,'\\'); pSlash && strchr(pSlash+1,'\\');
      pSlash = strchr(pSlash+1,'\\'));
   if (pSlash)
      *pSlash = 0;
   SetCurrentDirectory (szModule);


   // load the file in
   if (!lpszCmdLine || !lpszCmdLine[0]) {
      szTemp[0] = 0;
      OpenALog (szTemp);
      lpszCmdLine = szTemp;
   }

   // open it
   if (!gLF.FileOpen (lpszCmdLine)) {
      char  szTemp[512];
      wsprintf (szTemp, "Can't open %s.", lpszCmdLine);
      MessageBox (NULL, szTemp, "LogView", MB_OK);
      return -1;
   }

   // Bring up the dialog box
   DialogBox (hInstance, MAKEINTRESOURCE(IDD_VIEW),
      NULL, (DLGPROC) DialogProc);

   // delete the temp file
   DeleteFile ("c:\\lvtemp.wav");
   return 0;
}

