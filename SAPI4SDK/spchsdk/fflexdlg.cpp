/***************************************************************
LexDlg.cpp - Program to demonstrate the use of the microphone
   setup wizard.

Copyright (c) 1998 by Microsoft Corporation

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

#include <spchwrap.h>
#include "resource.h"
#include "ctools.h"

/* glbals */
CTTSEnum    *gpTTSEnum = NULL;
CSREnum     *gpSREnum = NULL;
CTTSMode    *gpTTSMode = NULL;
CSTLexDlg   *gpLexDlg = NULL;
char        gszModeName[128] = "";
PILEXPRONOUNCEW   gpLex = NULL;
PILEXPRONOUNCE2W   gpLex2 = NULL;
BOOL        fUsingSR = FALSE;       // TRUE if editing sr engine, FALSE if TTS
DWORD       gdwLex2Read=0, gdwLex2Write = 0;
LANGUAGEW   lang;
CSTPhoneConv *gpPhoneConv;
HINSTANCE   ghInstance;

/*************************************************************************
FindUniqueWords - Opens an ASCII text file, and populates a CList structure
   with the unique ascii words. The contents of the list elements are first
   a DWORD containing the word count, followed by the word string - null
   terminateds

inputs
   char     *pszFile - file
   CList    *pList - list to add to
returns
   BOOL - TRUE ifopened file, FALSE if can't open
*/
BOOL FindUniqueWords (char *pszFile, CList *pList)
{
   CAddBTree   tree;
   FILE  *f;
   f = fopen (pszFile, "rt");
   if (!f)
      return FALSE;

   char  szTemp[128];
   DWORD dwPosn;
   int   iRet;

   while (TRUE) {
      while (TRUE) {
         // not in a word, loop until get alphabetic character
         iRet = fgetc(f);
         if (iRet == EOF)
            break;

         if (isalpha(iRet))
            break;
      }
      if (iRet == EOF)
         break;

      // we're in a word
      szTemp[0] = (char) iRet;
      dwPosn = 1;

      while (TRUE) {
         iRet = fgetc(f);
         if (iRet == EOF)
            break;
         if (!isalpha(iRet) && (iRet != '\'') && (iRet != '’'))
            break;   // out of word

         // else add it
         if (dwPosn < sizeof(szTemp)-1)
            szTemp[dwPosn++] = (char) iRet;
      }

      // add the word tot he list if there is one
      szTemp[dwPosn] = 0;
      if (dwPosn) {
         // find the word in the list
         DWORD dwRet;
         if (dwRet = tree.Find (szTemp)) {
            // found
            DWORD *pdw;
            pdw = (DWORD*) pList->GetElem(dwRet-1);
            if (pdw)
               (*pdw)++;
         }
         else {
            // can't find
            BYTE  ab[sizeof(szTemp)+sizeof(DWORD)];
            DWORD *pdw;
            pdw = (DWORD*) ab;
            *pdw = 1;
            strcpy ((char*)ab + sizeof(DWORD), szTemp);

            pList->AddElem (ab, sizeof(DWORD)+strlen(szTemp)+1);
            tree.Add (szTemp, pList->GetNumElems());
         }
      }

      // break if EOF
      if (iRet == EOF)
         break;
   }

   fclose (f);
   return TRUE;
}


/*************************************************************************
SpeakPronunciation - speak the pronunciation in the window
*/
void SpeakPronunciation (HWND hWnd)
{
   // if we don't have a TTS engine that can speak IPA, then return
   if (!gpTTSMode)
      return;

   // speak this using TTS
   int iNum;
   iNum = SendDlgItemMessage (hWnd, IDC_PRONUNCIATIONS, LB_GETCURSEL, 0, 0);
   if (iNum == LB_ERR)
      return;

   // get the text
   char  szTemp[256];
   WCHAR szwPhone[256];
   szTemp[0] = 0;
   SendDlgItemMessage (hWnd, IDC_PRONUNCIATIONS, LB_GETTEXT, iNum, (LPARAM) szTemp);
   MultiByteToWideChar (CP_ACP, 0, szTemp, -1, szwPhone, sizeof(szwPhone)/2);

   // concvert this to IPA
   WCHAR szIPA[256];
   DWORD dw1, dw2;
   gpPhoneConv->ToIPA (szwPhone, &dw1, szIPA,
      sizeof(szIPA) / 2, &dw2);

   // speak the IPA
   gpTTSMode->AudioReset();
   SDATA s;
   s.pData = (PVOID) szIPA;
   s.dwSize = (wcslen(szIPA)+1)*2;

   gpTTSMode->TextData (CHARSET_IPAPHONETIC, 0, s, NULL, IID_ITTSBufNotifySinkW);
}

/*************************************************************************
UpdateWordDisplay - Updates the list to show which words are displayed.

inputs
   HWND     hWnd - window
*/
void UpdateWordDisplay (HWND hWnd)
{
   // erase the entries in the list box
   SendDlgItemMessage (hWnd, IDC_PRONUNCIATIONS, LB_RESETCONTENT, 0, 0);

   // get the string
   int iNum;
   iNum = SendDlgItemMessage (hWnd, IDC_WORDS, LB_GETCURSEL, 0, 0);
   if (iNum == LB_ERR) {
      // disable everything
      EnableWindow (GetDlgItem(hWnd, IDC_PRONUNCIATIONS), FALSE);
      EnableWindow (GetDlgItem(hWnd, IDC_EDIT), FALSE);
      SetDlgItemText (hWnd, IDC_INLEX, "No word");
      return;
   }

   // enable everything
   EnableWindow (GetDlgItem(hWnd, IDC_PRONUNCIATIONS), TRUE);
   EnableWindow (GetDlgItem(hWnd, IDC_EDIT), TRUE);

   // get the word text and convert to unciode
   WCHAR    szWord[512];
   char     szaWord[512];
   szaWord[0] = 0;
   SendDlgItemMessage (hWnd, IDC_WORDS, LB_GETTEXT, iNum, (LPARAM) szaWord);
   MultiByteToWideChar (CP_ACP, 0, szaWord, -1, szWord, sizeof(szWord)/2);

   HRESULT hRes;
   DWORD i, dwNeeded;
   VOICEPARTOFSPEECH pos;
   WCHAR    szIPA[256];
   // loop through all the pronunciations, using only onws that can write
   if (gpLex2) {
      for (i = 0; ; i++) {
         hRes = gpLex2->GetFrom (gdwLex2Write, CHARSET_IPAPHONETIC,
            szWord, (WORD) i, szIPA, sizeof(szIPA), &dwNeeded,
            &pos, NULL, 0, NULL);
         if (hRes)
            break;

         // convert this and add it to the list box
         DWORD dw1, dw2;
         WCHAR szwTemp[256];
         char szaTemp[256];
         gpPhoneConv->FromIPA (szIPA, &dw1, szwTemp,
            sizeof(szwTemp) / 2, &dw2);
         WideCharToMultiByte (CP_ACP, 0,
            szwTemp, -1, szaTemp, sizeof(szaTemp),
            0, 0);
         SendDlgItemMessage (hWnd, IDC_PRONUNCIATIONS, LB_ADDSTRING, 0, (LPARAM) szaTemp);

         // this is in the lexicon
         SetDlgItemText (hWnd, IDC_INLEX, "Yes");
      }
   }

   // if there are no pronucniations there, and ilex2, then get from the read section
   if (!SendDlgItemMessage (hWnd, IDC_PRONUNCIATIONS, LB_GETCOUNT, 0,0) && gpLex2) {
      // this is in the lexicon
      SetDlgItemText (hWnd, IDC_INLEX, "No");

      for (i = 0; ; i++) {
         hRes = gpLex2->GetFrom (gdwLex2Read, CHARSET_IPAPHONETIC,
            szWord, (WORD) i, szIPA, sizeof(szIPA), &dwNeeded,
            &pos, NULL, 0, NULL);
         if (hRes)
            break;

         // convert this and add it to the list box
         DWORD dw1, dw2;
         WCHAR szwTemp[256];
         char szaTemp[256];
         gpPhoneConv->FromIPA (szIPA, &dw1, szwTemp,
            sizeof(szwTemp) / 2, &dw2);
         WideCharToMultiByte (CP_ACP, 0,
            szwTemp, -1, szaTemp, sizeof(szaTemp),
            0, 0);
         SendDlgItemMessage (hWnd, IDC_PRONUNCIATIONS, LB_ADDSTRING, 0, (LPARAM) szaTemp);
      }
   }


   // if there are no pronunciations then use ILexPronounce
   if (!SendDlgItemMessage (hWnd, IDC_PRONUNCIATIONS, LB_GETCOUNT, 0,0)) {
      // this is in the lexicon
      SetDlgItemText (hWnd, IDC_INLEX, "Unknown");

      for (i = 0; ; i++) {
         hRes = gpLex->Get (CHARSET_IPAPHONETIC,
            szWord, (WORD) i, szIPA, sizeof(szIPA), &dwNeeded,
            &pos, NULL, 0, NULL);
         if (hRes)
            break;

         // convert this and add it to the list box
         DWORD dw1, dw2;
         WCHAR szwTemp[256];
         char szaTemp[256];
         gpPhoneConv->FromIPA (szIPA, &dw1, szwTemp,
            sizeof(szwTemp) / 2, &dw2);
         WideCharToMultiByte (CP_ACP, 0,
            szwTemp, -1, szaTemp, sizeof(szaTemp),
            0, 0);
         SendDlgItemMessage (hWnd, IDC_PRONUNCIATIONS, LB_ADDSTRING, 0, (LPARAM) szaTemp);
      }
   }

   // set the cur sel
   SendDlgItemMessage (hWnd, IDC_PRONUNCIATIONS, LB_SETCURSEL, 0, (LPARAM) 0);

   // Play tts
   SpeakPronunciation (hWnd);
}

/*************************************************************************
ScanFile - Scans a file and places its words in the list

inputs
   char     *pszFile - file
   HWND     hWnd - window handle of dialog to place in
   DWORD    dwCount - Words must occur this many times or more to keep
   DWORD    dwKeep - 0=all words, 1=only new words, 2=only words in lexicon already
   HWND     hWndStatus - Static text window to write status in
returns
   BOOL - TRUE if succede
*/
BOOL ScanFile (char *pszFile, HWND hWnd, DWORD dwCount, DWORD dwKeep, HWND hWndStatus)
{
   CList list;

   if (!FindUniqueWords(pszFile, &list)) {
      MessageBox (hWnd, "Can't open the specified file.", NULL, MB_OK);
      return FALSE;
   }

   // clear the current contents
   SendDlgItemMessage (hWnd, IDC_WORDS, LB_RESETCONTENT, 0, 0);

   // add the words
   DWORD i, dwNum;
   dwNum = list.GetNumElems();
   for (i =0; i < dwNum; i++) {
      DWORD *pdw;
      char  *psz;
      pdw = (DWORD*) list.GetElem (i);
      if (!pdw)
         continue;

      psz = (char*) (pdw+1);

      // if the count's too low ignroe
      if (*pdw < dwCount)
         continue;

      // if we're not doing any checks the accept
      if (dwKeep == 0)
         goto accept;

      // filter according to if already in lex
      if (!gpLex2)
         continue;   // can't filter this because need lex2

      // show the word
      if (hWndStatus) {
         char  szDisplay[512];
         wsprintf (szDisplay, "%d%% - %s", (int) (i * 100 / dwNum), psz);
         SetWindowText (hWndStatus, szDisplay);
         UpdateWindow (hWndStatus);
      }

      HRESULT hRes;
      WCHAR szWord[256];
      MultiByteToWideChar (CP_ACP, 0, psz, -1, szWord, sizeof(szWord)/2);
      DWORD dwNeeded;
      VOICEPARTOFSPEECH pos;
      WCHAR    szIPA[256];
      hRes = gpLex2->GetFrom (gdwLex2Write, CHARSET_IPAPHONETIC,
         szWord, 0, szIPA, sizeof(szIPA), &dwNeeded,
         &pos, NULL, 0, NULL);
      if (dwKeep == 1) {
         // keep words not in the lexicon
         if (!hRes)
            continue;
      }
      else {
         // keep only words in the lexicon
         if (hRes)
            continue;
      }


accept:
      SendDlgItemMessage (hWnd, IDC_WORDS, LB_ADDSTRING, 0, (LPARAM) psz);
   }


   // set some parameters
   SendDlgItemMessage (hWnd, IDC_WORDS, LB_SETCURSEL, 0, 0);
   UpdateWordDisplay (hWnd);


   return TRUE;
}


/*************************************************************************
LexDlgProc - Dialog procedure to modify the lexicon.

*/
BOOL CALLBACK ScanFileDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
      {
         CheckDlgButton (hWnd, IDC_ALLWORDS, BST_CHECKED);
         SetDlgItemText (hWnd, IDC_TIMES, "1");

         // if there's no ILex2 supported then disable the options for
         // only in the dictionary, because we can't tell withouth ILex2
         if (!gpLex2) {
            EnableWindow(GetDlgItem(hWnd, IDC_NEWWORDS), FALSE);
            EnableWindow(GetDlgItem(hWnd, IDC_OLDWORDS), FALSE);
         }
      }
      break;

   case WM_CLOSE:
      EndDialog (hWnd, IDCANCEL);
      return TRUE;

   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDOK:
         {
         // get the file name & stuff
         char szTemp[256];
         DWORD dwCount, dwKeep;
         GetDlgItemText (hWnd, IDC_FILE, szTemp, sizeof(szTemp));
         dwCount = GetDlgItemInt (hWnd, IDC_TIMES, NULL, FALSE);
         if (IsDlgButtonChecked(hWnd, IDC_ALLWORDS))
            dwKeep = 0;
         else if (IsDlgButtonChecked(hWnd, IDC_NEWWORDS))
            dwKeep = 1;
         else
            dwKeep = 2;

         HCURSOR  hCur;
         hCur = SetCursor(LoadCursor(NULL, IDC_WAIT));
         if (!ScanFile (szTemp, GetParent(hWnd), dwCount, dwKeep, GetDlgItem(hWnd, IDC_STATUS))) {
            SetCursor (hCur);
            return TRUE;   // error
         }
         SetCursor (hCur);

         EndDialog (hWnd, IDOK);
         }
         return TRUE;
      case IDCANCEL:
         EndDialog (hWnd, IDCANCEL);
         return TRUE;
      }
   };

   return FALSE;
}


/*************************************************************************
AddAll - Add all words not already in the lexicon

inputs
   HWND  hWnd - dialog that contains the list
*/
void AddAll (HWND hWnd)
{
   HWND  hWndList = GetDlgItem (hWnd, IDC_WORDS);

   if (IDYES != MessageBox (hWnd, "This will add any words from the list that are not "
      "already in the lexicon, into the lexicon. Are you sure you want to do this?",
      "Lexicon", MB_YESNO))
      return;

   if (!gpLex2)
      return;

   DWORD i, dwCount;
   HRESULT  hRes;
   char  szTemp[256];
#define  MAXPRON  10
   WCHAR szIPA[MAXPRON][256];
   DWORD dwNumPron;
   dwCount = SendMessage (hWndList, LB_GETCOUNT, 0, 0);
   for (i = 0; i < dwCount; i++) {
      szTemp[0] = 0;
      SendMessage (hWndList, LB_GETTEXT, i, (LPARAM) szTemp);

      WCHAR szWord[256];
      DWORD dwNeeded;
      VOICEPARTOFSPEECH pos;
      MultiByteToWideChar (CP_ACP, 0, szTemp, -1, szWord, sizeof(szWord)/2);

      // check one pronunciation to make sure there's nothing there
      hRes = gpLex2->GetFrom (gdwLex2Write, CHARSET_IPAPHONETIC,
         szWord, 0, szIPA[0], sizeof(szIPA[0]), &dwNeeded,
         &pos, NULL, 0, NULL);
      if (!hRes)
         continue; // already has pronunciation

      // loop through all the pronunciations we can read
      for (dwNumPron = 0; dwNumPron < MAXPRON; dwNumPron++) {
         hRes = gpLex2->GetFrom (gdwLex2Read, CHARSET_IPAPHONETIC,
            szWord, (WORD) dwNumPron, szIPA[dwNumPron], sizeof(szIPA[0]), &dwNeeded,
            &pos, NULL, 0, NULL);
         if (hRes)
            break;
      }

      // add these in then
      DWORD j;
      for (j = 0; j < dwNumPron; j++) {
         hRes = gpLex2->AddTo (gdwLex2Write, CHARSET_IPAPHONETIC,
            szWord, szIPA[j], VPS_UNKNOWN, NULL, NULL);
      }
   }

   // update the display window for the word
   UpdateWordDisplay (hWnd);
}


/*************************************************************************
RemoveAll - remove all words already in the lexicon

inputs
   HWND  hWnd - dialog that contains the list
*/
void RemoveAll (HWND hWnd)
{
   HWND  hWndList = GetDlgItem (hWnd, IDC_WORDS);

   if (IDYES != MessageBox (hWnd, "This will remove all the words in the list from the lexicon. "
      "Deleting common words could degrade the performance of the speech engine. "
      "Are you sure you want to do this?",
      "Lexicon", MB_YESNO))
      return;

   if (!gpLex2)
      return;

   DWORD i, dwCount;
   HRESULT  hRes;
   char  szTemp[256];
#define  MAXPRON  10
   WCHAR szIPA[MAXPRON][256];
   DWORD dwNumPron;
   dwCount = SendMessage (hWndList, LB_GETCOUNT, 0, 0);
   for (i = 0; i < dwCount; i++) {
      szTemp[0] = 0;
      SendMessage (hWndList, LB_GETTEXT, i, (LPARAM) szTemp);

      WCHAR szWord[256];
      DWORD dwNeeded;
      VOICEPARTOFSPEECH pos;
      MultiByteToWideChar (CP_ACP, 0, szTemp, -1, szWord, sizeof(szWord)/2);

      // loop through all the pronunciations we can read
      for (dwNumPron = 0; dwNumPron < MAXPRON; dwNumPron++) {
         hRes = gpLex2->GetFrom (gdwLex2Write, CHARSET_IPAPHONETIC,
            szWord, (WORD) dwNumPron, szIPA[dwNumPron], sizeof(szIPA[0]), &dwNeeded,
            &pos, NULL, 0, NULL);
         if (hRes)
            break;
      }

      // remove these
      DWORD j;
      for (j = 0; j < dwNumPron; j++) {
         hRes = gpLex2->RemoveFrom (gdwLex2Write, szWord, (WORD) j);
      }
   }

   // update the display window for the word
   UpdateWordDisplay (hWnd);
}

/*************************************************************************
LexDlgProc - Dialog procedure to modify the lexicon.

*/
BOOL CALLBACK LexDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
      {
         // Add a few fake words
         SendDlgItemMessage (hWnd, IDC_WORDS, LB_ADDSTRING, 0, (LPARAM) "Hello");

         // select the first word
         SendDlgItemMessage (hWnd, IDC_WORDS, LB_SETCURSEL, 0, 0);

         // update the display window for the word
         UpdateWordDisplay (hWnd);

         // if ILex2, then can't tell if word really there, so don't allow add/remove all
         if (!gpLex2) {
            EnableWindow(GetDlgItem(hWnd, IDC_ADDALL), FALSE);
            EnableWindow(GetDlgItem(hWnd, IDC_REMOVEALL), FALSE);
         }
      }
      break;

   case WM_CLOSE:
      EndDialog (hWnd, IDOK);
      return TRUE;

   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDC_WORDS:
         if (HIWORD(wParam) == LBN_SELCHANGE)
            UpdateWordDisplay (hWnd);
         break;
      case IDC_SCAN:
         DialogBox (ghInstance, MAKEINTRESOURCE(IDD_SCANFILE),
            hWnd, (DLGPROC) ScanFileDlgProc);
         break;
      case IDC_PRONUNCIATIONS:
         if (HIWORD(wParam) == LBN_SELCHANGE)
            SpeakPronunciation(hWnd);
         break;
      case IDC_EDIT:
         {
            // get the string
            int iNum;
            iNum = SendDlgItemMessage (hWnd, IDC_WORDS, LB_GETCURSEL, 0, 0);
            if (iNum == LB_ERR)
               return TRUE;

            // get the word text and convert to unciode
            WCHAR    szWord[512];
            char     szaWord[512];
            szaWord[0] = 0;
            SendDlgItemMessage (hWnd, IDC_WORDS, LB_GETTEXT, iNum, (LPARAM) szaWord);
            MultiByteToWideChar (CP_ACP, 0, szaWord, -1, szWord, sizeof(szWord)/2);

            // pull up the standard lexicon dialog
            // force the user to edit
            // if using tts, don't allow them to add/remove words or rename
            gpLexDlg->LexDlg(hWnd, szWord,
               (fUsingSR ? STLD_CHANGEPRONADDS : (STLD_DISABLEPRONADDREMOVE | STLD_DISABLERENAME))
               | STLD_FORCEEDIT, lang.LanguageID,
               gpLex, gpTTSMode ? gpTTSMode->m_pITTSCentral : NULL);

            // refresh the word display because it may be changed
            UpdateWordDisplay (hWnd);
         }
         break;
      case IDC_ADDALL:
         AddAll (hWnd);
         break;
      case IDC_REMOVEALL:
         RemoveAll (hWnd);
         break;
      case IDC_EDIT2:
         {
            // get the string
            int iNum;
            iNum = SendDlgItemMessage (hWnd, IDC_WORDS, LB_GETCURSEL, 0, 0);
            if (iNum == LB_ERR)
               return TRUE;

            // get the word text and convert to unciode
            WCHAR    szWord[512];
            char     szaWord[512];
            szaWord[0] = 0;
            SendDlgItemMessage (hWnd, IDC_WORDS, LB_GETTEXT, iNum, (LPARAM) szaWord);
            MultiByteToWideChar (CP_ACP, 0, szaWord, -1, szWord, sizeof(szWord)/2);

            // pull up the standard lexicon dialog
            // force the user to edit
            // if using tts, don't allow them to add/remove words or rename
            gpLexDlg->LexDlg(hWnd, NULL,
               (fUsingSR ? STLD_CHANGEPRONADDS : (STLD_DISABLEPRONADDREMOVE | STLD_DISABLERENAME))
               , lang.LanguageID,
               gpLex, gpTTSMode ? gpTTSMode->m_pITTSCentral : NULL);

            // refresh the word display because it may be changed
            UpdateWordDisplay (hWnd);
         }
         break;
      }
   };

   return FALSE;
}


/*************************************************************************
EngineDlgProc - Dialog procedure to select the engine.

  This fills gpLex and/or gpLex2 with a valid lex, if IDOK. If also
  fills gpTTSMode if this can speak out IPA.
*/
BOOL CALLBACK EngineDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
   case WM_INITDIALOG:
      {
         // enumerate through all the TTS engines with ILexPronounce
         TTSMODEINFOW   tmi;

         while (!gpTTSEnum->Next(&tmi)) {
            if (!(tmi.dwInterfaces & TTSI_ILEXPRONOUNCE))
               continue;

            char  szTemp[256];
            WideCharToMultiByte (CP_ACP, 0, tmi.szModeName, -1,
               szTemp, sizeof(szTemp), 0, 0);
            SendDlgItemMessage (hWnd, IDC_ENGINE, LB_ADDSTRING, 0, (LPARAM) szTemp);
         }

         // enumerate through all the SR engines with ILexPronounce
         SRMODEINFOW   smi;

         while (!gpSREnum->Next(&smi)) {
            if (!(smi.dwInterfaces & SRI_ILEXPRONOUNCE))
               continue;

            char  szTemp[256];
            WideCharToMultiByte (CP_ACP, 0, smi.szModeName, -1,
               szTemp, sizeof(szTemp), 0, 0);
            SendDlgItemMessage (hWnd, IDC_ENGINE, LB_ADDSTRING, 0, (LPARAM) szTemp);
         }

         // are there any items
         if (!SendDlgItemMessage (hWnd, IDC_ENGINE, LB_GETCOUNT, 0, 0)) {
            // no engines
            SendDlgItemMessage (hWnd, IDC_ENGINE, LB_ADDSTRING, 0, (LPARAM) "No engines");
            EnableWindow(GetDlgItem(hWnd, IDOK), FALSE);
         }

         // set a selection
         SendDlgItemMessage (hWnd, IDC_ENGINE, LB_SETCURSEL, 0, 0);
      }
      break;
   case WM_COMMAND:
      switch (LOWORD(wParam)) {
      case IDOK:
         {
         // copy the string
         SendDlgItemMessage (hWnd, IDC_ENGINE, LB_GETTEXT,
            SendDlgItemMessage (hWnd, IDC_ENGINE, LB_GETCURSEL, 0, 0),
            (LPARAM) gszModeName);

         // see if it's a SR engine or a TTS engine
         TTSMODEINFOW   tmi, tf;
         SRMODEINFOW    smi, sf;
         memset (&tmi, 0, sizeof(tmi));
         memset (&smi, 0, sizeof(smi));
         memset (&tf, 0, sizeof(tf));
         memset (&sf, 0, sizeof(sf));
         MultiByteToWideChar (CP_ACP, 0, gszModeName, -1, tmi.szModeName,
            sizeof(tmi.szModeName));
         MultiByteToWideChar (CP_ACP, 0, gszModeName, -1, smi.szModeName,
            sizeof(smi.szModeName));

         gpTTSEnum->Find (&tmi, &tf);
         gpSREnum->Find (&smi, &sf);

         if (!wcsicmp(tmi.szModeName, tf.szModeName)) {
            // found tts
            CTTSMode  *pMode;

            pMode = new CTTSMode;
            pMode->Init(tf.gModeID);
            if (pMode->m_pILexPronounce) {
               // support ILexPronounce, so keep this
               gpLex = pMode->m_pILexPronounce;
               gpLex2 = pMode->m_pILexPronounce2;
               if (gpLex)
                  gpLex->AddRef();
               if (gpLex2)
                  gpLex2->AddRef();

               // if this supports IPA text data speaking then keep it,
               // se we speak in the same voice
               if (tf.dwFeatures & TTSFEATURE_IPATEXTDATA)
                  gpTTSMode = pMode;
               else
                  delete pMode;

               fUsingSR = FALSE;
               lang = tf.language;

               // done
               EndDialog (hWnd, IDOK);
               return TRUE;
            }

            // TTS doesn't support ILexPronounce even though it claims to
            MessageBox (hWnd, "The text-to-speech engine doesn't support ILexPronounce even though it claims to.", NULL, MB_OK);
            return TRUE;
         }

         // else, found SR
         CSRMode  mode;
         PCSRGramComp pSRGram;
         CSRGramNotifySink *pgns;
         pgns = new CSRGramNotifySink;

         mode.Init(sf.gModeID);
         fUsingSR = TRUE;
         lang = sf.language;

         WCHAR *psz;
         psz = 
            (sf.dwGrammars & SRGRAM_DICTATION) ?
            L"[Grammar]\n"
            L"type=dictation\n"
            :
            L"[<Start>]\n"
            L"<Start>=hello\n";
         pSRGram = mode.GrammarFromMemory (pgns, psz, (wcslen(psz)+1)*2);

         // if the grammar has ILexPronounce then ask if the user
         // wants to use dictation
         if (pSRGram && pSRGram->m_pILexPronounce) {
            if (IDYES != MessageBox (hWnd, "Do you wish to modify the dictation vocabulary? (If you press "
               "No, you will modify the pronunciations for context-free grammars.)",
               "Lexicon", MB_YESNO)) {
               delete pSRGram;
               pSRGram = NULL;
            }
         }

         // if we have a non-null grammar that has a lexicon then use that,
         // else use the main sr mode
         if (pSRGram && pSRGram->m_pILexPronounce) {
            gpLex = pSRGram->m_pILexPronounce;
            gpLex2 = pSRGram->m_pILexPronounce2;
         }
         else if (mode.m_pILexPronounce) {
            gpLex = mode.m_pILexPronounce;
            gpLex2 = mode.m_pILexPronounce2;
         }
         else {
            // doens't support ILexPronounce at all
            // TTS doesn't support ILexPronounce even though it claims to
            MessageBox (hWnd, "The speech recognition engine doesn't support ILexPronounce even though it claims to.", NULL, MB_OK);
            return TRUE;
         }
         if (gpLex)
            gpLex->AddRef();
         if (gpLex2)
            gpLex2->AddRef();

         if (pSRGram)
            delete pSRGram;

         EndDialog (hWnd, IDOK);
         return TRUE;
         }
      case IDCANCEL:
         EndDialog (hWnd, IDCANCEL);
         return TRUE;
      }
      break;
   };

   return FALSE;
}

#if 0
   // see if it's a SR engine or a TTS engine
   TTSMODEINFOW   tmi, tf;
   SRMODEINFOW    smi, sf;
   memset (&tmi, 0, sizeof(tmi));
   memset (&smi, 0, sizeof(smi));
   memset (&tf, 0, sizeof(tf));
   memset (&sf, 0, sizeof(sf));
   MultiByteToWideChar (CP_ACP, 0, gszModeName, -1, tmi.szModeName,
      sizeof(tmi.szModeName));
   MultiByteToWideChar (CP_ACP, 0, gszModeName, -1, smi.szModeName,
      sizeof(smi.szModeName));

   gpTTSEnum->Find (&tmi, &tf);
   gpSREnum->Find (&smi, &sf);

   if (!wcsicmp(tmi.szModeName, tf.szModeName)) {
      // found tts
      CTTSMode  mode;

      mode.Init(tf.gModeID);
retesttts:
      hRes = lexDlg.LexDlg(NULL, NULL,
         STLD_DISABLEPRONADDREMOVE | STLD_DISABLERENAME, tf.language.LanguageID,
         mode.m_pITTSCentral, mode.m_pITTSCentral);

      if (IDYES == MessageBox (NULL, "Do you want to modify another word?",
         "Lexicon Editor", MB_YESNO))
         goto retesttts;

   }
   else {
      // found SR
      CSRMode  mode;
      PCSRGramComp pSRGram;
      CSRGramNotifySink *pgns;
      pgns = new CSRGramNotifySink;

      mode.Init(sf.gModeID);
      WCHAR *psz;
      psz = 
         (sf.dwGrammars & SRGRAM_DICTATION) ?
         L"[Grammar]\n"
         L"type=dictation\n"
         :
         L"[<Start>]\n"
         L"<Start>=hello\n";
      pSRGram = mode.GrammarFromMemory (pgns, psz, (wcslen(psz)+1)*2);

retest:

// #define TESTCASE
#ifdef TESTCASE
      hRes = lexDlg.LexDlg(NULL, L"hello", STLD_CHANGEPRONADDS | STLD_FORCEEDIT, sf.language.LanguageID,
         pSRGram->m_pISRGramCommon);
#else
      hRes = lexDlg.LexDlg(NULL, NULL, STLD_CHANGEPRONADDS, sf.language.LanguageID,
         pSRGram->m_pISRGramCommon);
#endif // TESTCASE

      // if it failed, then see if main grammar object supports
      if (hRes)
         hRes = lexDlg.LexDlg(NULL, NULL, STLD_CHANGEPRONADDS, sf.language.LanguageID,
            mode.m_pISRCentral);

      if (IDYES == MessageBox (NULL, "Do you want to modify another word?",
         "Lexicon Editor", MB_YESNO))
         goto retest;

      delete pSRGram;
   }

#endif // 0


/*************************************************************************
CreateTTSEngine - Creates a text-to-speech engine that can play the IPA
   audio properly.
*/
void CreateTTSEngine (void)
{
   if (gpTTSMode)
      return;  // all done

   // find one with the right features
   TTSMODEINFOW   mi1, mi2;
   memset (&mi1, 0, sizeof(mi1));
   memset (&mi2, 0, sizeof(mi2));
   mi1.dwFeatures = TTSFEATURE_ANYWORD | TTSFEATURE_IPATEXTDATA;
   mi1.language = lang;
   gpTTSEnum->Find (&mi1, &mi2);

   // make sure it supports that
   if (!(mi2.dwFeatures & TTSFEATURE_IPATEXTDATA))
      return;  // cant find anything

   // else, found it, so create it
   gpTTSMode = new CTTSMode;
   if (!gpTTSMode)
      return;
   if (gpTTSMode->Init (mi2.gModeID)) {
      // error
      delete gpTTSMode;
      gpTTSMode = NULL;
   }

}

/*************************************************************************
winmain - Windows main code.
*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
   HRESULT hRes;
   ghInstance = hInstance;

   CoInitialize(NULL);
   {

   // create the lexicon dialog
   gpLexDlg = new CSTLexDlg;
   hRes = gpLexDlg->Init();
   if (hRes) {
      MessageBox(NULL, "Can't create CLSID_STLexDlg. Do you have 4.0 installed?", NULL, MB_OK);
      goto closeup;
   }


   // craete the enumerators
   gpTTSEnum = new CTTSEnum;
   gpTTSEnum->Init();
   gpSREnum = new CSREnum;
   gpSREnum->Init();

   // find out which engine the user wants
   if (IDOK != DialogBox (hInstance, MAKEINTRESOURCE(IDD_ENGINE),
      NULL, (DLGPROC) EngineDlgProc))
      goto closeup;

   // get some flags if lex2 supported so we know what lexicons to modify
   if (gpLex2) {
      gpLex2->QueryLexicons(FALSE, &gdwLex2Read);
      gpLex2->QueryLexicons(TRUE, &gdwLex2Write);
   }

   // create a tts engine that can play audio
   CreateTTSEngine();

   // create the phoneme converter object so the phonemes can be understood
   gpPhoneConv = new CSTPhoneConv;
   if (gpPhoneConv) {
      gpPhoneConv->Init();
      gpPhoneConv->SetByLang (lang.LanguageID);
   }

   // main lexicon dialog
   DialogBox (hInstance, MAKEINTRESOURCE(IDD_LEX),
      NULL, (DLGPROC) LexDlgProc);

closeup:
   if (gpLex)
      gpLex->Release();
   if (gpLex2)
      gpLex2->Release();

   if (gpTTSMode)
      delete gpTTSMode;
   if (gpPhoneConv)
      delete gpPhoneConv;
   if (gpLexDlg)
      delete gpLexDlg;

   // free up the enumerators
   if (gpTTSEnum)
      delete gpTTSEnum;
   if (gpSREnum)
      delete gpSREnum;

   }
   CoUninitialize ();

   return 0;
}


