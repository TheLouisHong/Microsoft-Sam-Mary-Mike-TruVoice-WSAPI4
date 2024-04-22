/************************************************************************
Grammar.cpp - Simple control to ask for n Grammar

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

static WCHAR gszStart[] = L"AskGrammar";

CGrammar::CGrammar()
{
   m_dwRecog = 0;     // recognition result
   m_dwParseSize = 0; // number of bytes in the parse
   m_pszParse = NULL;
}

CGrammar::~CGrammar()
{
   if (m_pszParse)
      free (m_pszParse);
}

PCWSTR CGrammar::GetControlName (void)
{
   return L"Grammar";
}

DWORD CGrammar::GetDefaultTextResID (LANGID langID)
{
   // since only have one, return the english one no matter what
   return IDR_GRAMMAR;
}

void CGrammar::GetCLSID (CLSID *pCLSID)
{
#ifdef BUILDREALCONTROL
   *pCLSID = CLSID_GrammarControl;
#else
   *pCLSID = CLSID_SampleGrammarControl;
#endif
}

void CGrammar::OnState (DWORD dwStateID)
{
   switch (dwStateID) {
   case 0:  // initialization
      TTSSpeak (L"Prompts", L"Main");
      ReadyToListenBeep();
      StartSRWhenReady (gszStart);   // only have one rule that care about
      break;
      
   case 1:  // called when a number is gotten but we're don't know if should verify
      if (m_fVerify && m_pszParse) {
         DoVerify (L"VerifyPre", L"VerifyPost", m_pszParse,
            2, 0);
      }
      else
         DoState (2);   // verified
      break;

   case 2:  // called when everything has been verified
      DoFinish (m_dwRecog, m_pszParse, m_dwParseSize);
      if (m_pszParse)
         free (m_pszParse);
      m_pszParse = NULL;
      break;

   }


}


void CGrammar::OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                               DWORD dwParseMemSize,
                                               PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult)
{
   // free up existing data
   if (m_pszParse)
      free (m_pszParse);
   m_pszParse = NULL;

   // remember this
   m_dwRecog = dwParseID;
   m_dwParseSize = dwParseMemSize;
   if (pParseMem) {
      m_pszParse = (PWSTR) malloc(dwParseMemSize);
      if (m_pszParse)
         memcpy (m_pszParse, pParseMem, dwParseMemSize);
   }

   // verify
   DoState(1);
}

