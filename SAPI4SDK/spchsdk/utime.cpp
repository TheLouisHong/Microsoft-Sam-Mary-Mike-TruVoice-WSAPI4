/************************************************************************
Time.cpp - Simple control to ask for a Time

   This includes the interface:
      IClassFactory
      IUnknown

Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <objbase.h>
#include <objerror.h>
#include <spchwrap.h>
#include "telctl.h"
#include "resource.h"

static WCHAR gszStart[] = L"AskTime";

PCWSTR CTime::GetControlName (void)
{
   return L"Time";
}

DWORD CTime::GetDefaultTextResID (LANGID langID)
{
   // since only have one, return the english one no matter what
   return IDR_TIME;
}

void CTime::GetCLSID (CLSID *pCLSID)
{
#ifdef BUILDREALCONTROL
   *pCLSID = CLSID_TimeControl;
#else
   *pCLSID = CLSID_SampleTimeControl;
#endif
}

void CTime::OnState (DWORD dwStateID)
{
   switch (dwStateID) {
   case 0:  // initialization
      TTSSpeak (L"Prompts", L"Main");
      ReadyToListenBeep();
      StartSRWhenReady (gszStart);   // only have one rule that care about
      break;
      
   case 1:  // called when a number is gotten but we're don't know if should verify
      // make sure there's an AM or PM on the end. If not, ask for the info
      {
         DWORD dwLen;
         BOOL  fAMPM = FALSE;
         dwLen = wcslen(m_szParse);
         if ((dwLen > 2) && (!_wcsicmp(m_szParse + (dwLen-1), L"M")))
            fAMPM = TRUE;

         if (!fAMPM) {
            DoState (3);
            break;
         }
      }

      if (m_fVerify && m_szParse[0]) {
         DoVerify (L"VerifyPre", L"VerifyPost", m_szParse,
            2, 0);
      }
      else
         DoState (2);   // verified
      break;

   case 2:  // called when everything has been verified
      DoFinish (m_dwRecog, m_szParse, (wcslen(m_szParse)+1)*2);
      break;

   case 3:  // ask if AM or PM
      DoVerify (L"InTheEvening", NULL, NULL, 4, 5);
      break;

   case 4:  // user answered yes that is in evening
      wcscat (m_szParse, L" PM");
      DoState(1);
      break;

   case 5:  // user answered no to "is that in the evening"
      wcscat (m_szParse, L" AM");
      DoState(1);
      break;
   }



}


void CTime::OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                               DWORD dwParseMemSize,
                                               PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult)
{
   // remember this
   m_dwRecog = dwParseID;
   if (pParseMem)
      wcscpy (m_szParse, (WCHAR*) pParseMem);
   else
      m_szParse[0] = 0;

   // relative times
   switch (dwParseID) {
   case 1:  // now
      {
         SYSTEMTIME  st;
         GetLocalTime (&st);
         int   iHour;
         BOOL  fPM;
         iHour = (int) (st.wHour % 12);
         if (!iHour)
            iHour = 12;
         fPM = (st.wHour >= 12) && (st.wHour < 24);
         swprintf (m_szParse, L"%d:%02d %s",
            (int) iHour, (int) st.wMinute,
            fPM ? L"PM" : L"AM");
         m_dwRecog = 0;

      }
      break;
   }

   // verify
   DoState(1);
}

