/************************************************************************
YesNo.cpp - Simple yes/no control

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

PCWSTR CYesNo::GetControlName (void)
{
   return L"Simple Yes/No";
}

DWORD CYesNo::GetDefaultTextResID (LANGID langID)
{
   // since only have one, return the english one no matter what
   return IDR_YESNO;
}

void CYesNo::GetCLSID (CLSID *pCLSID)
{
#ifdef BUILDREALCONTROL
   *pCLSID = CLSID_YesNoControl;
#else
   *pCLSID = CLSID_SampleYesNoControl;
#endif
}

void CYesNo::OnState (DWORD dwStateID)
{
   switch (dwStateID) {
   case 0:  // initialization
      TTSSpeak (L"Prompts", L"Main");
      ReadyToListenBeep();
      StartSRWhenReady (NULL);   // only have one rule that care about
      break;
   }

}


void CYesNo::OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                               DWORD dwParseMemSize,
                                               PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult)
{
   // return the parse result
   // We can use the recognition result to return
   DoFinish (dwParseID, NULL, 0);
}

