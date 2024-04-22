/***************************************************************
MicWiz.cpp - Program to demonstrate the use of the microphone
   setup wizard.

Copyright (c) 1997-1998 by Microsoft Corporation

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


/*************************************************************************
winmain - Windows main code.
*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
   BOOL  fAsk = TRUE;

   if (!stricmp(lpszCmdLine, "/n"))
      fAsk = FALSE;

   CoInitialize(NULL);

   // create the microphone wizard
   HRESULT hRes;
   PISTMICWIZARD   pISTMicWizard = NULL;
   hRes = CoCreateInstance(CLSID_STMicWizard, NULL,
      CLSCTX_ALL, IID_ISTMicWizard,
      (void**)&pISTMicWizard);
   if (hRes) {
      MessageBox(NULL, "Can't create CLSID_STMicWizard. Do you have 3.0 installed?", NULL, MB_OK);
      goto closeup;
   }

   // see if it's been used before
   MICWIZARDINFO  wi;
   hRes = pISTMicWizard->InfoGet(&wi);
   if (hRes)
      goto closeup;

   if (wi.fHasRunMicWizard) {
      if (!fAsk)
         goto closeup;
      if (IDYES != MessageBox (NULL,
         "You've already checked your microphone. Do you want to check it again?",
         "Microphone Setup Wizard", MB_YESNO))
         goto closeup;
   }

   pISTMicWizard->Wizard(NULL, STMWU_DICTATION, WAVE_MAPPER, 16000, STMWU_CNC);

closeup:
   if (pISTMicWizard)
      pISTMicWizard->Release();
   CoUninitialize ();

   return 0;
}


