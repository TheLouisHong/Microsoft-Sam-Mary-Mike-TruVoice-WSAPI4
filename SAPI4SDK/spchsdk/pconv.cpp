/***********************************************************************
PConv.Cpp - Code to demo the phoneme converter.



Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/


#define UNICODE
#define _UNICODE

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include <initguid.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <speech.h>
#include "pconv.h"


/*************************************************************************
Globals */

HINSTANCE         ghInstance;                // instance handle
PISTPHONECONV     gpISTPhoneConv = NULL;

/*******************************************************************
HexToIPA - Converts a string containing hex numbers into IPA.

inputs
   WCHAR    *pszIn - input
   WCHAR    *pszIPA - Output with IPA
   DWORD    dwMax - Maximum number of characters
returns
   none
*/
void HexToIPA (WCHAR *pszIn, WCHAR *pszIPA, DWORD dwMax)
{
   while (TRUE) {
      // repeat until non numeric
      while (*pszIn && iswspace(*pszIn))
         pszIn++;
      if (!(*pszIn))
         break;

      // if we've reached max then stop
      if (!dwMax)
         break;

      // find the number
      *pszIPA = 0;
      while (TRUE) {
         if ((*pszIn >= L'0') && (*pszIn <= L'9'))
            *pszIPA = (*pszIPA * 16) + (*pszIn - L'0');
         else if ((*pszIn >= L'A') && (*pszIn <= L'F'))
            *pszIPA = (*pszIPA * 16) + (*pszIn - L'A' + 10);
         else if ((*pszIn >= L'a') && (*pszIn <= L'f'))
            *pszIPA = (*pszIPA * 16) + (*pszIn - L'a' + 10);
         else
            break;   // not a number
         pszIn++;
      }

      pszIPA++;
      dwMax--;
   }

   *pszIPA = 0;
}
/*************************************************************************
LangDialogProc
*/
BOOL CALLBACK LangDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
      case WM_INITDIALOG:
         {
            // set the current text
            WCHAR szTemp[64];
            wsprintf (szTemp, L"%d", (DWORD) LANGIDFROMLCID (GetThreadLocale()));
            SetDlgItemText (hWnd, IDC_LANGID, szTemp);
         }
         return TRUE;
      case WM_COMMAND:
         switch (LOWORD(wParam)) {
            case IDOK:
               {
                  // get the current text
                  WCHAR szTemp[64];
                  GetDlgItemText (hWnd, IDC_LANGID, szTemp, 64);
                  LANGID   l;
                  l = (LANGID) wcstoul(szTemp, NULL, 10);
                  if (gpISTPhoneConv->SetByLang(l))
                     MessageBox (hWnd, L"Can't set that language!", NULL, MB_OK);
                  else
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
TextDialogProc
*/
BOOL CALLBACK TextDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg) {
      case WM_INITDIALOG:
         {
            // set the current text
            SDATA d;
            if (gpISTPhoneConv->Get(&d)) {
               MessageBox(hWnd, L"Can't get the text!", NULL, MB_OK);
               return TRUE;
            }

            SetDlgItemText (hWnd, IDC_TEXT, (LPCWSTR) d.pData);
            CoTaskMemFree (d.pData);
         }
         return TRUE;
      case WM_COMMAND:
         switch (LOWORD(wParam)) {
            case IDOK:
               {
                  // get the current text
#define  HUGENUM        10000
                  WCHAR szTemp[HUGENUM];
                  GetDlgItemText (hWnd, IDC_TEXT, szTemp, HUGENUM);
                  if (gpISTPhoneConv->SetByText(szTemp))
                     MessageBox (hWnd, L"Can't set the text!", NULL, MB_OK);
                  else
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
#define  STRINGSIZE     256

   switch (uMsg) {
      case WM_INITDIALOG:
         {
         // Use a unicode font that can show IPA
         HFONT hFont;
         LOGFONT  lf;
         memset (&lf, 0, sizeof(lf));
         lf.lfHeight = 16;
         wcscpy (lf.lfFaceName, L"Lucida Sans Unicode");
         hFont = ::CreateFontIndirect (&lf);

         SendDlgItemMessage (hWnd, IDC_IPA_SYMBOLS, WM_SETFONT, (WPARAM) hFont, TRUE);
         }
         return TRUE;
      case WM_COMMAND:
         switch (LOWORD(wParam)) {
            case IDC_TOIPA:
               {
                  WCHAR szIn[STRINGSIZE];
                  WCHAR szOut[STRINGSIZE];
                  HRESULT  hRes;
                  DWORD dwInvalid = (DWORD) -1;
                  DWORD dwNeeded = 0;

                  GetDlgItemText (hWnd, IDC_TEXT, szIn, STRINGSIZE);
                  hRes = gpISTPhoneConv->ToIPA(szIn, &dwInvalid,
                     szOut, STRINGSIZE, &dwNeeded);
                  if (hRes)
                     return TRUE;

                  // Write out IPA characters to special box
                  SetDlgItemText (hWnd, IDC_IPA_SYMBOLS, szOut);

                  // set text
                  WCHAR szNum[STRINGSIZE];
                  szNum[0] = 0;
                  DWORD i;
                  for (i = 0; szOut[i]; i++)
                     wsprintf (szNum + wcslen(szNum), L"%x ", (DWORD) szOut[i]);
                  SetDlgItemText (hWnd, IDC_IPA, szNum);

                  // if error, set cursor to error point
                  if (dwInvalid != (DWORD) -1) {
                     HWND  hCtl;
                     hCtl = GetDlgItem(hWnd, IDC_TEXT);
                     SetFocus(hCtl);
                     SendMessage (hCtl, EM_SETSEL, dwInvalid, dwInvalid);
                  }

               }
               return TRUE;
            case IDC_FROMIPA:
               {
                  WCHAR szIn[STRINGSIZE];
                  WCHAR szOut[STRINGSIZE];
                  WCHAR szIPA[STRINGSIZE];
                  HRESULT  hRes;
                  DWORD dwInvalid = (DWORD) -1;
                  DWORD dwNeeded = 0;

                  GetDlgItemText (hWnd, IDC_IPA, szIn, STRINGSIZE);
                  HexToIPA (szIn, szIPA, STRINGSIZE);

                  // Write out IPA characters to special box
                  SetDlgItemText (hWnd, IDC_IPA_SYMBOLS, szIPA);

                  hRes = gpISTPhoneConv->FromIPA(szIPA, &dwInvalid,
                     szOut, STRINGSIZE, &dwNeeded);
                  if (hRes)
                     return TRUE;

                  // set text
                  SetDlgItemText (hWnd, IDC_TEXT, szOut);

                  // if error, set cursor to error point
                  if (dwInvalid != (DWORD) -1) {
                     HWND  hCtl;
                     hCtl = GetDlgItem(hWnd, IDC_IPA_SYMBOLS);
                     SetFocus(hCtl);
                     SendMessage (hCtl, EM_SETSEL, dwInvalid, dwInvalid);
                  }

               }
               return TRUE;
            case IDC_SETBYLANG:
               DialogBox (ghInstance, MAKEINTRESOURCE(IDD_LANGUAGE),
                  NULL, (DLGPROC) LangDialogProc);
               return TRUE;
            case IDC_SETBYTEXT:
               DialogBox (ghInstance, MAKEINTRESOURCE(IDD_TEXT),
                  NULL, (DLGPROC) TextDialogProc);
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
winmain - Windows main code.
*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
   CoInitialize (NULL);

   ghInstance = hInstance;

   // create the phoneme converter
   HRESULT hRes;
   hRes = CoCreateInstance(CLSID_STPhoneConv, NULL,
      CLSCTX_ALL, IID_ISTPhoneConv,
      (void**)&gpISTPhoneConv);
   if (hRes) {
      MessageBox(NULL, L"Can't create CLSID_STPhoneConv. Do you have 3.0 installed?", NULL, MB_OK);
      return -1;
   }


   DialogBox (hInstance, MAKEINTRESOURCE(IDD_MAIN),
      NULL, (DLGPROC) DialogProc);

   gpISTPhoneConv->Release();

   CoUninitialize ();
   return 0;
}


