/************************************************************************
Xtension.cpp - Simple control to ask for an PhoneNum

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

static WCHAR gszStart[] = L"AskPhoneNumber";
static WCHAR gsz3Digits[] = L"3Digits";
static WCHAR gsz4Digits[] = L"4Digits";

PCWSTR CPhoneNum::GetControlName (void)
{
   return L"PhoneNum";
}

DWORD CPhoneNum::GetDefaultTextResID (LANGID langID)
{
   // since only have one, return the english one no matter what
   return IDR_PHONENUM;
}

void CPhoneNum::GetCLSID (CLSID *pCLSID)
{
#ifdef BUILDREALCONTROL
   *pCLSID = CLSID_PhoneNumControl;
#else
   *pCLSID = CLSID_SamplePhoneNumControl;
#endif
}

void PutInSpaces (PCWSTR pIn, PWSTR pOut)
{
   DWORD i;
   for (i = 0; ; i++) {
      pOut[i*2] = pIn[i];
      if (!pIn[i])
         break;
      pOut[i*2+1] = L' ';
   }
}

void CPhoneNum::OnState (DWORD dwStateID)
{
   switch (dwStateID) {
   case 0:  // initialization
      // start out with no digits
      m_szCurrentDigits[0] = 0;
      m_szCombinedDigits[0] = 0;

      TTSSpeak (L"Prompts", L"Main");
      ReadyToListenBeep();
      StartSRWhenReady (gszStart);   // only have one rule that care about
      break;
      
   case 101:  // user just pressed/spoke on digit, there may be more
      ReadyToListenBeep();
      StartSRWhenReady (gszStart);   // only have one rule that care about
      break;

   case 1:  // called when a number is gotten but we're don't know if should verify
      if (m_fVerify) {
         DoVerify (L"VerifyPre", L"VerifyPost", m_szCurrentDigits,
            2, 3);
      }
      else
         DoState (2);   // verified
      break;

   case 2:  // called when everything has been verified
      DoFinish (0, m_szCurrentDigits, (wcslen(m_szCurrentDigits)+1)*2);
      break;

   case 3:  // called when verification of main phone number fails
      // it it's not a NorthAmerican phone number then go back to state 0,
      // else go to state 3
      {
         BOOL  fIsNA;
         DWORD i, dwRes;
         for (i = dwRes = 0; m_szCurrentDigits[i]; i++)
            if (iswdigit(m_szCurrentDigits[i]))
               dwRes |= (1L << i);

         fIsNA =
            (dwRes == 0xf7) || // 11110111
            (dwRes == 0xf77) || // 111101110111
            (dwRes == 0x3ddd);   // 11110111011101

         // if the first three digits start with 011 it's not
         if ((m_szCurrentDigits[0] == L'0') &&
            (m_szCurrentDigits[0] == L'1') &&
            (m_szCurrentDigits[0] == L'1'))
            fIsNA = FALSE;

         // Since we don't know how to ask european numbers,
         // just return to the main menu
         DoState (fIsNA ? 4 : 0);
      }
      break;

   case 4:  // North American. Ask the area code
      // start out with no digits
      m_szCombinedDigits[0] = 0;
      m_szCurrentDigits[0] = 0;

      TTSSpeak (L"Prompts", L"AskAreaCode");
      ReadyToListenBeep();
      StartSRWhenReady (gsz3Digits);   // only have one rule that care about
      break;
   case 104:  // user just pressed/spoke on digit, there may be more
      ReadyToListenBeep();
      StartSRWhenReady (gsz3Digits);   // only have one rule that care about
      break;


   case 5:  // North American. Verify the area code
      PutInSpaces (m_szCurrentDigits, m_szSpaceDigits);
      DoVerify (L"VerifyPre", L"VerifyPost", m_szSpaceDigits,
         6, 4);
      break;

   case 6:  // Firs time, North American. Ask the next three digits
      // copy the old digits over
      wcscat (m_szCombinedDigits, m_szCurrentDigits);
      wcscat (m_szCombinedDigits, L"-");
      // let this fall on through

   case 61:  // Firs time, North American. Ask the next three digits
      // start out with no digits
      m_szCurrentDigits[0] = 0;

      TTSSpeak (L"Prompts", L"AskNextThree");
      ReadyToListenBeep();
      StartSRWhenReady (gsz3Digits);   // only have one rule that care about
      break;

   case 106:  // user just pressed/spoke on digit, there may be more
      ReadyToListenBeep();
      StartSRWhenReady (gsz3Digits);   // only have one rule that care about
      break;

   case 7:  // North american. Verify the next three digits
      PutInSpaces (m_szCurrentDigits, m_szSpaceDigits);
      DoVerify (L"VerifyPre", L"VerifyPost", m_szSpaceDigits,
         8, 61);
      break;

   case 8:  // Firt time, North american. Ask the last four digits
      // copy the old digits over
      wcscat (m_szCombinedDigits, m_szCurrentDigits);
      wcscat (m_szCombinedDigits, L"-");
      // let this fall on through

   case 81:  // Firt time, North american. Ask the last four digits
      // start out with no digits
      m_szCurrentDigits[0] = 0;

      TTSSpeak (L"Prompts", L"AskLastFour");
      ReadyToListenBeep();
      StartSRWhenReady (gsz4Digits);   // only have one rule that care about
      break;

   case 108:  // user just pressed/spoke on digit, there may be more
      ReadyToListenBeep();
      StartSRWhenReady (gsz4Digits);   // only have one rule that care about
      break;

   case 9:  // North american. Verify the last four digits
      PutInSpaces (m_szCurrentDigits, m_szSpaceDigits);
      DoVerify (L"VerifyPre", L"VerifyPost", m_szSpaceDigits,
         10, 81);
      break;

   case 10: // After collect all of the digits, combine together & verify togehter
      // copy the old digits over
      wcscat (m_szCombinedDigits, m_szCurrentDigits);

      // copy all into m_szCurrentDigits
      wcscpy (m_szCurrentDigits, m_szCombinedDigits);

      DoState (1);   // re-verify
      break;
   }


}


void CPhoneNum::OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                               DWORD dwParseMemSize,
                                               PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult)
{
   DWORD dwLen;

   // if we had DTMF entered then add it to the list
   if ((dwParseID >= 1) && (dwParseID <= 12)) {
      // convert 10 to a 0 press
      if (dwParseID == 10)
         dwParseID = 0;

      // if the user presses pound or star then go onto the next state
      if (dwParseID > 10) {
         dwLen = wcslen(m_szCurrentDigits);
         switch (m_dwCurState) {
         case 0:
         case 101:
            DoState(dwLen ? 1 : 0);
            break;
         case 4:
         case 104:
            DoState(dwLen ? 5 : 4);
            break;
         case 6:
         case 61:
         case 106:
            DoState(dwLen ? 7 : 6);
            break;
         case 8:
         case 81:
         case 108:
            DoState(dwLen ? 9 : 8);
            break;
         default:
            // shouldn't get here
            DoState(1);
         }
         return;
      }

      // add it to the string
      dwLen = wcslen(m_szCurrentDigits);
      if ((dwLen+2 < MAXDIGITS) && ((dwLen + wcslen(m_szCombinedDigits) + 5 < MAXDIGITS))) {
         // only write out if we're not going to break the bank
         m_szCurrentDigits[dwLen] = L'0' + (WORD) dwParseID;
         m_szCurrentDigits[dwLen+1] =
            ((m_dwCurState == 0) || (m_dwCurState == 101)) ? L' ' : 0;
         m_szCurrentDigits[dwLen+2] = 0;
      }

      // If we're in a state where we're entering only 3 or 4
      // digits then, go onto the next state
      dwLen = wcslen(m_szCurrentDigits);
      switch (m_dwCurState) {

      // We're asking for 3 digits, so depending on how many digits
      // we have, go on
      case 4:
      case 104:
         DoState((dwLen < 3) ? 104 : 5);
         break;

      // Asking for 3 digits again, so check for length
      case 6:
      case 61:
      case 106:
         DoState((dwLen < 3) ? 106 : 7);
         break;

      // Now asking for 4 digits
      case 8:
      case 81:
      case 108:
         DoState((dwLen < 4) ? 108 : 9);
         break;

      // if the user types in state 0 (or just after typed), then
      // just play a beep
      case 0:
      case 101:
      default:
         DoState(101);
         break;
      }
      return;
   }

   // return the parse result
   // We can use the recognition result to return
   wcscpy (m_szCurrentDigits, (PCWSTR) pParseMem);
   switch (m_dwCurState) {
   case 0:
   case 101:
      DoState(1);
      break;
   case 4:
   case 104:
      DoState(5);
      break;
   case 6:
   case 61:
   case 106:
      DoState(7);
      break;
   case 8:
   case 81:
   case 108:
      DoState(9);
      break;
   default:
      // shouldn't get here
      DoState(1);
   }
}

