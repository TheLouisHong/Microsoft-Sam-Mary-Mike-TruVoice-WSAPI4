/************************************************************************
Record.cpp - Simple control to ask for a Recording

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

static WCHAR gszSettings[] = L"Settings";
static WCHAR gszPlaybackString[] = L"<Record-Control-Prompt>";

PCWSTR CRecord::GetControlName (void)
{
   return L"Record";
}

DWORD CRecord::GetDefaultTextResID (LANGID langID)
{
   // since only have one, return the english one no matter what
   return IDR_RECORD;
}

void CRecord::GetCLSID (CLSID *pCLSID)
{
#ifdef BUILDREALCONTROL
   *pCLSID = CLSID_RecordControl;
#else
   *pCLSID = CLSID_SampleRecordControl;
#endif
}

void CRecord::OnState (DWORD dwStateID)
{
   switch (dwStateID) {
   case 0:  // initialization
      // if we already have a wave then clear it out
      if (m_pWAV) {
         CoTaskMemFree (m_pWAV);
         m_pWAV = NULL;
      }

      TTSSpeak (L"Prompts", L"Main");
      RecordingBeep();
      m_fRecording = FALSE;
      // we catch the TTS done
      break;
       
   case 1:  // called when a number is gotten but we're don't know if should verify
      if (m_fVerify && m_pWAV) {
         // pass the wave file to the playback object
         m_pQueue->WaveAddFromMemory (gszPlaybackString, m_pWAV, m_dwWAVSize);

         DoVerify (L"VerifyPre", L"VerifyPost", gszPlaybackString,
            2, 0);
      }
      else
         DoState (2);   // verified
      break;

   case 2:  // called when everything has been verified

      // remove the wave from the playback queue
      m_pQueue->WaveRemove (gszPlaybackString);

      DoFinish (0, m_pWAV, m_pWAV ? m_dwWAVSize : 0);
      if (m_pWAV) {
         CoTaskMemFree (m_pWAV);
         m_pWAV = NULL;
      }
      break;

   }
}


CRecord::CRecord (void)
{
   m_pWAV = NULL;
   m_dwWAVSize = 0;
   m_fRecording = FALSE;
   m_pISTRecord = NULL;
}

CRecord::~CRecord (void)
{
   if (m_pWAV) {
      CoTaskMemFree (m_pWAV);
      m_pWAV = NULL;
   }
   if (m_pISTRecord) {
      m_pISTRecord->Stop(0);
      m_pISTRecord->Release();
   }
}



void CRecord::OnAbort (void)
{
   if (m_dwCurState == 0) {
      StopAllMedia();
      if (m_pISTRecord)
         m_pISTRecord->Stop(0);
      m_fRecording = FALSE;
      if (!m_pWAV && m_pISTRecord)
         m_pISTRecord->GetWAV (&m_pWAV, &m_dwWAVSize);
      DoFinish (TCR_ABORT, m_pWAV, m_pWAV ? m_dwWAVSize : 0);
      if (m_pWAV) {
         CoTaskMemFree (m_pWAV);
         m_pWAV = NULL;
      }
   }
   else
      CTelControlFramework::OnAbort();
}

void CRecord::OnDTMF (WORD wDTMF)
{
   if (m_dwCurState == 0) {
      // if we're recording then stop and finish because the user
      // hit DTMF. If we're not recording then ignore
      if (m_fRecording) {
         StopAllMedia();
         if (m_pISTRecord)
            m_pISTRecord->Stop(250);   // cut out DTMF sound
         m_fRecording = FALSE;
         if (!m_pWAV && m_pISTRecord)
            m_pISTRecord->GetWAV (&m_pWAV, &m_dwWAVSize);
         DoFinish (0, m_pWAV, m_pWAV ? m_dwWAVSize : 0);
         if (m_pWAV) {
            CoTaskMemFree (m_pWAV);
            m_pWAV = NULL;
         }
      }
   }
   else
      CTelControlFramework::OnDTMF(wDTMF);
}

void CRecord::OnTTSStop (void)
{
   if (m_dwCurState == 0) {
      // start up record
      if (!m_pISTRecord)
         return;
      m_pISTRecord->Start();
      m_fRecording = TRUE;

      // wait
   }
   else
      CTelControlFramework::OnTTSStop();
}

STDMETHODIMP CRecord::Start (PITELCONTROLNOTIFYSINK pS)
{
   HRESULT  hRes;

   // try to create the record object
   if (!m_pISTRecord) {
      hRes = CoCreateInstance(CLSID_STRecord, NULL, CLSCTX_ALL,
         IID_ISTRecord, (void**)&m_pISTRecord);
      if (hRes)
         return hRes;

      // create the audio source
      LPUNKNOWN   lpUnk;
      hRes = m_pTelInfo->AudioSourceCreate(&lpUnk);
      if (hRes) {
         m_pISTRecord->Release();
         m_pISTRecord = NULL;
         return hRes;
      }

      hRes = m_pISTRecord->Init (lpUnk, 8000, &m_NS);
      lpUnk->Release();
      if (hRes)
         return hRes;
   }

   // initialize the parameters
   STRECORDPARAM  rp;
   rp.dwBetweenUtt = GetValue (gszSettings, L"BetweenUtt", 500);
   rp.dwInitialBuf = GetValue (gszSettings, L"InitialBuf", 5000);
   rp.dwReallocBuf = GetValue (gszSettings, L"ReallocBuf", 1000);
   rp.dwMaxBuf = GetValue (gszSettings, L"MaxBuf", 10000);
   rp.dwNoAnswer = GetValue (gszSettings, L"NoAnswerTime", 10) * 1000;
   m_pISTRecord->ParamsSet (&rp);

   m_NS.m_pRecord = this;

   // call the normal start
   return CTelControlFramework::Start(pS);
}



/*************************************************************************
CCtlSTRecordNotify - Notification object. All this this will do is display
   its notification on the main window.
*/
CCtlSTRecordNotify::CCtlSTRecordNotify (void)
{
// this space intentionally left blank
}

CCtlSTRecordNotify::~CCtlSTRecordNotify (void)
{
// this space intentionally left blank
}

STDMETHODIMP CCtlSTRecordNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
   *ppv = NULL;

   /* always return our IUnkown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISTRecordNotifySink))
      *ppv = (LPVOID) this;

   // otherwise, cant find

   return ppv ? NOERROR : E_NOINTERFACE;
}

STDMETHODIMP_ (ULONG) CCtlSTRecordNotify::AddRef (void)
{
// normally this increases a reference count, but this object
// is going to be freed as soon as the app is freed, so it doesn't
// matter
return 1;
}

STDMETHODIMP_(ULONG) CCtlSTRecordNotify::Release (void)
{
// normally this releases a reference count, but this object
// is going to be freed when the application is freed so it doesnt
// matter
return 1;
};


STDMETHODIMP CCtlSTRecordNotify::AutoStop(void)
{
   // free up any exsiting data we may have
   if (m_pRecord->m_pWAV) {
      CoTaskMemFree (m_pRecord->m_pWAV);
      m_pRecord->m_pWAV = NULL;
   }

   // get the new data
   m_pRecord->m_pISTRecord->GetWAV (&m_pRecord->m_pWAV, &m_pRecord->m_dwWAVSize);

   // if this value is not NULL, then we got answeer. Play
   // a recognized beep and go to state 2
   if (m_pRecord->m_pWAV) {
      m_pRecord->m_dwNoAnswer = 0;
      m_pRecord->RecognizedBeep();
      m_pRecord->DoState (1);
      return NOERROR;
   }

   // else, it's unrecognized. count the number of times unrecognized
   m_pRecord->m_dwNoAnswer++;
   if (m_pRecord->m_dwNoAnswer >= m_pRecord->m_dwNoAnswerMax)
      // hasn't answered N times in a row, so abort
      m_pRecord->DoFinish (TCR_NORESPONSE, NULL, 0);
   else {
      // play unrecognized beep
      m_pRecord->UnrecognizedBeep();

      // call no answer
      m_pRecord->OnNoAnswer();
   }


   return NOERROR;
}

STDMETHODIMP CCtlSTRecordNotify::VU(WORD wLevel)
{
   // ignore this
   return NOERROR;
}


