/************************************************************************
Xtension.cpp - Simple control to ask for an extension

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
#include <objbase.h>
#include <objerror.h>
#include <spchwrap.h>
#include "telctl.h"
#include "resource.h"

static WCHAR gszStart[] = L"AskDigits";

PCWSTR CExtension::GetControlName (void)
{
   return L"Extension";
}

DWORD CExtension::GetDefaultTextResID (LANGID langID)
{
   // since only have one, return the english one no matter what
   return IDR_EXTENSION;
}

void CExtension::GetCLSID (CLSID *pCLSID)
{
#ifdef BUILDREALCONTROL
   *pCLSID = CLSID_ExtensionControl;
#else
   *pCLSID = CLSID_SampleExtensionControl;
#endif
}

void CExtension::OnState (DWORD dwStateID)
{
   switch (dwStateID) {
   case 0:  // initialization
      // start out with no digits
      m_szCurrentDigits[0] = 0;

      TTSSpeak (L"Prompts", L"Main");
      ReadyToListenBeep();
      StartSRWhenReady (gszStart);   // only have one rule that care about
      break;
      
   case 1:  // too few digits
      TTSSpeak (L"Prompts", L"TooFewDigits");
      ReadyToListenBeep();
      StartSRWhenReady (gszStart);   // only have one rule that care about
      break;

   case 2:  // too many digits
      TTSSpeak (L"Prompts", L"TooManyDigits");
      ReadyToListenBeep();
      StartSRWhenReady (gszStart);   // only have one rule that care about
      break;

   case 3:  // user just pressed/spoke on digit, there may be more
      ReadyToListenBeep();
      StartSRWhenReady (gszStart);   // only have one rule that care about
      break;

   case 4:  // called when a number is gotten but we're don't know if should verify
      if (m_fVerify) {
         // separate all of the digits by a space so TTS reads out properly
         DWORD i;
         for (i = 0; ; i++) {
            m_szSpaceDigits[i*2] = m_szCurrentDigits[i];
            if (!m_szCurrentDigits[i])
               break;
            m_szSpaceDigits[i*2+1] = L' ';
         }
         DoVerify (L"VerifyPre", L"VerifyPost", m_szSpaceDigits,
            5, 0);
      }
      else
         DoState (5);   // verified
      break;

   case 5:  // called when everything has been verified
      DoFinish (0, m_szCurrentDigits, (wcslen(m_szCurrentDigits)+1)*2);
      break;
   }

}


void CExtension::OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                               DWORD dwParseMemSize,
                                               PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult)
{
   // if it's DTMF or just one digit then append onto existing digits
   WCHAR wDigit = 0;
   DWORD dwLenDigits;
   if (dwParseID && (dwParseID <= 10)) {
      if (dwParseID == 10) // 0
         wDigit = '0';
      else
         wDigit = (WCHAR) dwParseID + '0';
   }
   PCWSTR psz;
   psz = (PCWSTR) pParseMem;
   if (pParseMem && iswdigit (psz[0]) && !psz[1])
      wDigit = psz[0];
   if (wDigit) {
      // append onto the current string
      dwLenDigits = wcslen(m_szCurrentDigits);
      if (dwLenDigits < (MAXDIGITS-1)) {
         m_szCurrentDigits[dwLenDigits] = wDigit;
         m_szCurrentDigits[dwLenDigits+1] = 0;
      }
      dwLenDigits++;
   }

   // if there are too few or too many digits then error
   DWORD dwLen = pParseMem ? wcslen ((PCWSTR)pParseMem) : 0;
   DWORD dwNeeded = GetValue (L"Settings", L"NumDigits");

   // if the user just spoke a single digit, then:
   if (wDigit) {
      // if we have enough digits then we're done
      // see about verification
      if (dwLenDigits >= dwNeeded) {
         DoState (4);
         return;
      }

      // else, go for another digit
      DoState (3);
      return;
   }

   // if the user spoke a bunch of digits then wipe out the existing list
   m_szCurrentDigits[0] = 0;

   if (dwLen < dwNeeded) {
      // too few digits
      DoState (1);
      return;
   }
   else if (dwLen > dwNeeded) {
      DoState (2);
      return;
   }

   // return the parse result
   // We can use the recognition result to return
   wcscpy (m_szCurrentDigits, (PCWSTR) pParseMem);
   DoState (4);
}

void CExtension::OnNoAnswer (void)
{
   // special case. If we're in state 3 then re-ask state 1, otherwise default
   if (m_dwCurState == 3) {
      DoState (1);
      return;
   }

   // else default
   CTelControlFramework::OnNoAnswer();
}

