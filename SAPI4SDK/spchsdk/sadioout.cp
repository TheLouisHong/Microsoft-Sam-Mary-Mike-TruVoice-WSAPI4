/************************************************************************
AudioOut.cpp - Code for audio output.

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
#include <dbt.h>
#include <winver.h>
#include <mmsystem.h>
#include <objbase.h>
#include <objerror.h>
#include <speech.h>
#include <mmreg.h>
#include "audio.h"

// Debug Settings
#ifdef _DEBUG
#define TRACE_BOOKMARKS    (1)
#else
#define TRACE_BOOKMARKS    (0)
#endif

// HACKHACK - this is ifdef'd out in mmddk.h
#define  DRV_QUERYDEVNODE     (DRV_RESERVED + 2)

/* Define state names for the state machine */
#define  AOSM_PBFLAG          0x80
#define  AOSM_UNINITIALIZED   0x00
#define  AOSM_IDLE            0x01
#define  AOSM_CLAIMED         0x02
#define  AOSM_PLAYING         0x03
#define  AOSM_PB_IDLE         (AOSM_IDLE | AOSM_PBFLAG) 
#define  AOSM_PB_CLAIMED      (AOSM_CLAIMED | AOSM_PBFLAG)

MMRESULT TTSWaveMapper(LPHWAVEOUT lphwo, PUINT puDeviceID, LPCWAVEFORMATEX lpwf,
                       DWORD dwCallback, DWORD dwInstance, DWORD fdwOpen);

// Global variables used for debugging
#if TRACE_BOOKMARKS
static int nBookMarks = 0;
static int nRetBookMarks = 0;
#endif /* TRACE_BOOKMARKS */

/*************************************************************************
AOutDeviceIDToName - This converts a device ID into a device-name string. We
   have to do this because MMSys may change device IDs on us at any time
   cause of PnP.

inputs
   DWORD    dwDeviceID - Device ID number, including WAVE_MAPPER. If this
               is WAVE_MAPPER then the string will be set to an empty string.
   PSTR     pszString - String that is filled in. Must be large enough for
               waveOutGetDevCaps() names.
returns
   BOOL - TRUE if succeeds.
*/
BOOL AOutDeviceIDToName(DWORD dwDeviceID, PSTR pszString)
{
   WAVEOUTCAPS     wc;

   if (dwDeviceID == WAVE_MAPPER) {
      pszString[0] = '\0';
      return (TRUE);
   }

   if (waveOutGetDevCaps(dwDeviceID, &wc, sizeof(WAVEOUTCAPS)))
      return (FALSE);

   strcpy(pszString, wc.szPname);
   return (TRUE);
} /* End of AOutDeviceIDToName() */

/*************************************************************************
AOutNameToDeviceID - This converts a name into a device ID. We
   have to do this because MMSys may change device IDs on us at any time
   cause of PnP.

inputs
   DWORD    dwDeviceID - Device ID number where the device was the last
               time, including WAVE_MAPPER. This value is only used for
               optimization so we don't have to search through everything.
   PSTR     pszString - String of the device.
returns
   DWORD - device ID. If it cant find a match then this returns
            a device ID value that is out of range.
*/
DWORD AOutNameToDeviceID(DWORD dwDeviceID, PSTR pszString)
{
   WAVEOUTCAPS wc;
   DWORD i, num;

   // First off, if wave_mapper then do nothing
   if (dwDeviceID == WAVE_MAPPER) {
      return (dwDeviceID);
   }

   // Check out the first device. If it works then go for it
   if (!waveOutGetDevCaps(dwDeviceID, &wc, sizeof(WAVEOUTCAPS))) {
      if (!strcmp (pszString, wc.szPname))
         return (dwDeviceID);
   }

   // Else, search through all of the devices & look for the name
   num = waveOutGetNumDevs();
   for (i = 0; i < num; i++)
      if (!waveOutGetDevCaps(i, &wc, sizeof(WAVEOUTCAPS)))
         if (!strcmp(pszString, wc.szPname))
            return (i);

   // else can't find
   return (dwDeviceID);
} /* End of AOutNameToDeviceID() */

/************************************************************************
HandleDeviceChange - Process the WM_DEVICECHANGE message.  

inputs
   pAOut - pointer to the Audio Output Object
   dwEvent - the specific event ID
   pDevBroadcastHdr - pointer to the device broadcast header

returns
   TRUE/FALSE - dependent on specific event.
*/
BOOL CAOut::HandleDeviceChange(DWORD dwEvent, PDEV_BROADCAST_HDR pDevBroadcastHdr)
{
   switch (dwEvent) {
      case DBT_DEVICEARRIVAL:
         // This message is sent when a device has been
         // inserted and is now available
         if (pDevBroadcastHdr->dbch_devicetype == DBT_DEVTYP_DEVNODE) {
            // Okay, a new device has arrived let's check and see if it's our
            // audio device.  First we have to re-determine the devnode of our
            // audio device.
            if (!waveOutMessage((HWAVEOUT) m_dwWaveDev, DRV_QUERYDEVNODE, (LPARAM) &m_dnDevNode, 0)) {
               if (((PDEV_BROADCAST_DEVNODE) pDevBroadcastHdr)->dbcd_devnode == m_dnDevNode) {

                  switch (m_nState) {
                     case AOSM_CLAIMED:
                        OpenAndPrepare();
                        break;
                          
                     case AOSM_PLAYING:
                     case AOSM_PB_IDLE:
                     case AOSM_PB_CLAIMED:
                        OpenAndPrepare();
                        while (TryToSendBufOut())
                           /* Loop body intentionally left blank */ ;
                        break;

                     default:
                        // Device wasn't open so clear the removal flag
                        m_fRemoved = FALSE;
                        break;
                  }
               }
            }
         }
         return (TRUE);

      case DBT_DEVICEQUERYREMOVE:
         // This message is sent to request permission to remove
         // a device.  Any application can deny this request and 
         // cancel the removal.
         switch (m_nState) {
            case AOSM_UNINITIALIZED:
            case AOSM_IDLE:
               // The device isn't open so its fine with us if
               // the user yanks the card.
               return (TRUE);
         }
         // Otherwise we are currently playing audio so fail the
         // request in hopes that the user will find a cleaner
         // way to shut us down then just yanking the card.
         return (FALSE);

      case DBT_DEVICEQUERYREMOVEFAILED:
         // This message is sent when a request to remove a device 
         // has been canceled.
         break;

      case DBT_DEVICEREMOVEPENDING:
         // This message is sent when the device is about to be
         // removed.  This message is the last chance for applications
         // and drivers to prepare for the removal of the device.
         break;

      case DBT_DEVICEREMOVECOMPLETE:
         // This message is sent when a device has been removed.
         if (m_dnDevNode && pDevBroadcastHdr->dbch_devicetype == DBT_DEVTYP_DEVNODE) {
            if (((PDEV_BROADCAST_DEVNODE) pDevBroadcastHdr)->dbcd_devnode == m_dnDevNode) {
               m_fRemoved = TRUE;
               switch (m_nState) {
                  case AOSM_CLAIMED:
                     // Here if the device is open and buffers are
                     // prepared, but we haven't been started.  All
                     // we have to do is close the device without
                     // taking a state transition.
                     CloseIt();
                     break;

                  case AOSM_PLAYING:
                  case AOSM_PB_IDLE:
                  case AOSM_PB_CLAIMED:
                     // Here if the device is open, buffers are prepared,
                     // and we are playing audio.  We'll reset the waveaudio
                     // device and queue all the buffers in our forced stop
                     // cache.  When all the buffers have been returned, we'll
                     // unprepare all the headers and close the device.  All of
                     // this is done without a state transition.
                     waveOutReset(m_hWaveOut);
                     m_fJustOpenedOrReset = TRUE;
                     m_fForceStop = TRUE;
                     break;
               }

               // Okay, the device has gone away so our devnode is now bogus
               m_dnDevNode = 0;
            }
         }
         return (TRUE);
   }
   return (TRUE);
} /* End of HandleDeviceChange() */

/*************************************************************************
AOutWindowProc - Window to 'Monitor' the audio output..
*/

LRESULT CALLBACK AOutWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   PCAOut pAOut = (PCAOut) GetWindowLong(hWnd, 0);

#define WM_SPACECHANGE (WM_USER + 142)

   switch (uMsg) {
      case WM_CREATE:
         pAOut = (PCAOut) ((CREATESTRUCT *) lParam)->lpCreateParams;
         SetWindowLong(hWnd, 0, (DWORD) pAOut);
         return (0);

      case WM_CLOSE:
         // We don't want to close
         return (0);

      case WM_DEVICECHANGE:
         return (pAOut->HandleDeviceChange((DWORD) wParam, (PDEV_BROADCAST_HDR) lParam));

      case MM_WOM_OPEN:
         if (!pAOut->m_fRemoved)
            pAOut->NotifyAudioStart();
         else
            pAOut->m_fRemoved = FALSE;
         return (0);

      case MM_WOM_CLOSE:
         if (!pAOut->m_fRemoved)
            pAOut->NotifyAudioStop(IANSRSN_NODATA);
         return (0);

      case MM_WOM_DONE:
         pAOut->ReturnedBuffer((LPWAVEHDR) lParam);
         return (0);

      case WM_SPACECHANGE:
         pAOut->Notify();
         return (0);

      case WM_TIMER:
         if (pAOut->m_fIsTimerOn && pAOut->m_pBookMarkList->GetNumElems()) {
            QWORD qwTime = 0;

            // See what time it is
            pAOut->m_pAOutIAudio->PosnGet(&qwTime);

            // Flush out any bookmarks that are around
            pAOut->ReturnedBookMarks(qwTime);
         }
         return 0;

   } /* End of process message switch */
   return (DefWindowProc(hWnd, uMsg, wParam, lParam));
} /* End of AOutWindowProc() */



/************************************************************************
CAOut - Audio Dest class.
*/

CAOut::CAOut(LPUNKNOWN punkOuter, LPFNDESTROYED pfnDestroy)
{
   m_cRef            = 0;
   m_punkOuter       = punkOuter;
   m_pfnDestroy      = pfnDestroy;
   m_pAOutIAudioDest = NULL;
   m_pAOutIAudio     = NULL;
   m_pAOutIAudioMultiMediaDevice = NULL;
   m_pIADNS          = NULL;
   m_pCache          = NULL;
   m_pFSCache        = NULL;
   m_pWFEX           = NULL;
   m_dwWFEXSize      = NULL;
   m_fForceStop      = FALSE;

   m_bFlushing       = FALSE;
   m_qwTotal         = 0;
   m_dwWaveDev       = WAVE_MAPPER;
   AOutDeviceIDToName(m_dwWaveDev, m_szDeviceName);
   m_hWaveOut        = NULL;
   m_dnDevNode       = 0;
   m_hWndMonitor     = NULL;
   m_dwBufSize       = 0;
   m_fBufsOut        = 0;
   m_fRemoved        = FALSE;
   m_nState          = AOSM_UNINITIALIZED;
   m_fSentEmptyBuf   = FALSE;
   m_qwOffsetWaveOutPosn = 0;
   m_fIsTimerOn      = FALSE;
   m_fJustOpenedOrReset = FALSE;
   m_fDriverCrashOnPosn = FALSE;

   memset(m_ahBufGlobal, 0, sizeof(m_ahBufGlobal));
   memset(m_apBufWH, 0, sizeof(m_apBufWH));

   // Record the time of the object's inception for use in ToFileTime
   {
   SYSTEMTIME     st;
   m_msStartTime = GetCurrentTime();
   GetSystemTime(&st);
   SystemTimeToFileTime(&st, &m_ftStartTime);
   }

   // Start with last buffer at this time
   m_qwLastBufTime = 0;
   m_msLastBufTime = m_msStartTime;

   InitializeCriticalSection(&m_CritSec);
}


CAOut::~CAOut (void)
{
   DWORD i;


   // Return all outstanding bookmarks as flushed
   m_bFlushing = TRUE;
   ReturnedBookMarks(m_qwTotal);

   // If we have the wave device open then kill it
   CloseIt();

   // Free the contained interfaces
   if (m_pAOutIAudioDest)
      delete m_pAOutIAudioDest;
   if (m_pAOutIAudio)
      delete m_pAOutIAudio;
   if (m_pAOutIAudioMultiMediaDevice)
      delete m_pAOutIAudioMultiMediaDevice;
   if (m_pIADNS)
      m_pIADNS->Release();
   if (m_pCache)
      delete m_pCache;
   if (m_pBookMarkList)
      delete m_pBookMarkList;
   if (m_pWFEX)
      free(m_pWFEX);
   if (m_hWndMonitor)
      DestroyWindow(m_hWndMonitor);
   if (m_pFSCache)
      delete m_pFSCache;

   for (i = 0; i < AOUT_NUMBUF; i++) {
      if (m_ahBufGlobal[i]) {
         GlobalUnlock(m_ahBufGlobal[i]);
         GlobalFree(m_ahBufGlobal[i]);
      }
   }
   DeleteCriticalSection(&m_CritSec);
}


/*******************************************************************************
 *
 * CAOut::FInit
 *
 *    new CAOutIAudioDest
 *    new CAOutIIdentity
 *    new CCache (m_dwBufSize * AOUT_BUFSPERSEC * AOUT_CACHESIZE
 *
 *    CreateWindow(hWndMonitor)
 *
 *    alloc AOUT_NUMBUF m_buffs
 *
 ******************************************************************************/

BOOL CAOut::FInit(DWORD dwWaveDev, LPWAVEFORMATEX lpWFEX, DWORD dwWFEXSize)
{
   LPUNKNOWN pIUnknown = (LPUNKNOWN) this;
   WNDCLASS wc;

   if (NULL != m_punkOuter) 
      pIUnknown = m_punkOuter;

   // Allocate all of the contained interfaces
   m_pAOutIAudioDest = new CAOutIAudioDest(this, pIUnknown);
   m_pAOutIAudio = new CAOutIAudio(this, pIUnknown);
   m_pAOutIAudioMultiMediaDevice = new CAOutIAudioMultiMediaDevice(this, pIUnknown);

   // Create the montior window
   memset(&wc, 0, sizeof(WNDCLASS));
   wc.lpfnWndProc   = AOutWindowProc;
   wc.hInstance     = ghInstance;
   wc.lpszClassName = "TTSAudioOutClass";
   wc.cbWndExtra    = sizeof(long);
   RegisterClass(&wc);
   m_hWndMonitor = CreateWindow(wc.lpszClassName, "", 0, CW_USEDEFAULT, CW_USEDEFAULT,
                                CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, ghInstance, 
                                (VOID*) this);

   // Store the device away
   m_dwWaveDev = dwWaveDev;

   // Create a book mark list
   m_pBookMarkList = new CList();

   return (m_pAOutIAudioDest &&
           m_pAOutIAudio &&
           m_pAOutIAudioMultiMediaDevice &&
           m_pBookMarkList &&
           m_hWndMonitor);
} /* End of FInit() */



STDMETHODIMP CAOut::QueryInterface(REFIID riid, LPVOID *ppv)
{
   *ppv = NULL;

   // Always return our IUnkown for IID_IUnknown
   if (IsEqualIID(riid, IID_IUnknown)) 
      *ppv = (LPVOID) this;
   else if (IsEqualIID(riid, IID_IAudioDest))
      *ppv = m_pAOutIAudioDest;
   else if (IsEqualIID(riid, IID_IAudio))
      *ppv = m_pAOutIAudio;
   else if (IsEqualIID(riid, IID_IAudioMultiMediaDevice))
      *ppv = m_pAOutIAudioMultiMediaDevice;

   // update the reference count
   if (NULL != *ppv) {
      ((LPUNKNOWN) *ppv)->AddRef();
      return (NOERROR);
   }

   return (ResultFromScode(E_NOINTERFACE));
} /* End of QueryInterface() */



STDMETHODIMP_(ULONG) CAOut::AddRef(void)
{
   return (++m_cRef);
} /* End of AddRef() */



STDMETHODIMP_(ULONG) CAOut::Release(void)
{
   ULONG    cRefT;

   cRefT = --m_cRef;
   if (0 == m_cRef) {
      // inform destroy that object is going away
      if (NULL != m_pfnDestroy) 
         (*m_pfnDestroy)();
      delete this;
   }

   return (cRefT);
} /* End of Release() */



/* 
 *  This trys to open and prepare the audio output device.  If it can't then
 *  it returns an error.  It does NOT continue to retry indefinately.
 */
STDMETHODIMP CAOut::OpenAndPrepare(void)
{
   MMRESULT    mm;
   SCODE       er;
   DWORD       i;
   LPWAVEHDR   pWH;

   // Stop a sound playing via sndPlaySound (if any)
   // sndPlaySound(NULL, 0);

   m_dwWaveDev = AOutNameToDeviceID(m_dwWaveDev, m_szDeviceName);
   mm = (m_dwWaveDev == WAVE_MAPPER)
      ? TTSWaveMapper(&m_hWaveOut, (PUINT) &m_dwWaveDev, m_pWFEX,
                      (DWORD) m_hWndMonitor, NULL, CALLBACK_WINDOW)
      : waveOutOpen  (&m_hWaveOut, m_dwWaveDev, m_pWFEX,
                      (DWORD) m_hWndMonitor, NULL, CALLBACK_WINDOW);
   AOutDeviceIDToName(m_dwWaveDev, m_szDeviceName);

   if (!mm) {
      m_fJustOpenedOrReset = TRUE;
      if (!m_fIsTimerOn)
         SetTimer (m_hWndMonitor, WAVEOUTTIMER, WAVEOUTTIMERMS, NULL);
      m_fIsTimerOn = TRUE;
   }
   if (mm) {
      m_hWaveOut = NULL;
      switch (mm) {
         case MMSYSERR_BADDEVICEID: 
            er = AUDERR_WAVEDEVNOTSUPPORTED; 
            break;

         case MMSYSERR_ALLOCATED: 
            er = AUDERR_WAVEDEVICEBUSY; 
            break;

         case MMSYSERR_NOMEM: 
            er = E_OUTOFMEMORY; 
            break;

         case MMSYSERR_NOTENABLED:
            er = AUDERR_WAVENOTENABLED;
            break;

         case MMSYSERR_NODRIVER: 
            er = AUDERR_NODRIVER;
            break;

         default:
            er = AUDERR_WAVEFORMATNOTSUPPORTED; 
            break;
      }
      return (ResultFromScode(er));
   }

   // Here we determine the devnode of the audio device we just opened.  This
   // is saved so that we can determine if the audio device is removed while
   // we are using it (plug-n-play).  This message require the user to pass
   // in the device ID of the device not a valid handle.
   waveOutMessage((HWAVEOUT) m_dwWaveDev, DRV_QUERYDEVNODE, (LPARAM) &m_dnDevNode, 0); 

   // Prepare all of the headers
   for (i = 0; i < AOUT_NUMBUF; i++) {
      pWH = m_apBufWH[i];
      memset(pWH, 0, sizeof(WAVEHDR));
      pWH->lpData = (char *) (pWH + 1);
      pWH->dwBufferLength = m_dwBufSize;
      pWH->dwUser = i;
      mm = waveOutPrepareHeader(m_hWaveOut, pWH, sizeof(WAVEHDR));

      if (mm) {
         if (!waveOutClose(m_hWaveOut))
            m_hWaveOut = NULL;
         if (m_fIsTimerOn) {
            KillTimer (m_hWndMonitor, WAVEOUTTIMER);
            m_fIsTimerOn = FALSE;
         }
         return (ResultFromScode(E_OUTOFMEMORY));
      }
   }
   return (MMSYSERR_NOERROR);
} /* End of OpenAndPrepare() */



/*
 *  This trys to start the audio output dest.
 */

STDMETHODIMP CAOut::TryToStart(void)
{
   MMRESULT    mm;

   // Open the device and prepare headers
   if ((mm = OpenAndPrepare()) != MMSYSERR_NOERROR)
       return (mm);

   // Send as much data as possible to the device
   while (this->TryToSendBufOut())
      /* Loop body intentionally left blank */ ;

   return (NOERROR);
} /* End of TryToStart() */


/* This trys to send a buffer out to be played. It returns TRUE
   if successful */

BOOL CAOut::TryToSendBufOut(void)
{
   DWORD    dwMMBufID;     // Multimedia buffer to use
   DWORD    dwMMBufMask;   // Mask of the buffer to use
   DWORD    dwUsed;        // Amount of data in the main cache
   DWORD    dwFSUsed;      // Amount of data in the forced stop cache
   PSTR     pMMDataBuf;    // Pointer to the waveaudio data buffer
   DWORD    dwMMBufSize;   // Amount of space left in the waveaudio data buffer
   BOOL     fFreedSpace;   // TRUE if we have removed some data from the primary cache

   // First let's see if there is any data to queue
   // We check both the forced stop cache
   if (FAILED(m_pFSCache->Used(&dwFSUsed)))
      return (FALSE);            // Cache failure

   // and the normal data cache
   if (FAILED(m_pCache->Used(&dwUsed)))
      return (FALSE);            // Cache failure

   // Verify that there is data to play
   if (!dwFSUsed && !dwUsed)
      return (FALSE);            // Both caches are empty

   // Second, let's see if there is an available wave data buffer
   for (dwMMBufID = 0, dwMMBufMask = 1; dwMMBufID < AOUT_NUMBUF; dwMMBufID++, dwMMBufMask <<= 1) {
      if (!(m_fBufsOut & dwMMBufMask)) 
         break;
   }
   if (dwMMBufID == AOUT_NUMBUF)
      return (FALSE);                           // All buffers in use, wait till later
   pMMDataBuf = m_apBufWH[dwMMBufID]->lpData;   // Initialize pointer to chosen buffer
   dwMMBufSize = m_dwBufSize;                   // Initialize MM buffer to empty

   // Okay, we have some data to play and a buffer to put it in
   // so now we copy the data from the cache(s) into the buffer.
   // First we satisfy the need for data from the forced stop cache
   if (dwFSUsed) {
      // Here we try and completely fill the MM buffer, dwFSUsed will be set
      // to the number of bytes actually copied into the buffer.
      if (FAILED(m_pFSCache->DataGet(pMMDataBuf, dwMMBufSize, &dwFSUsed)))
         return (FALSE);               // Problem getting data

      // Now we adjust the buffer pointer and free space count based on
      // how much data we just received.
      pMMDataBuf += dwFSUsed;
      dwMMBufSize -= dwFSUsed;
   } /* End of data in the forced stop cache */

   // Reset an internal flag that reflects whether we have removed data
   // from the primary cache or not.  If we don't remove any data from
   // the primary cache, then we don't want to tell the user that cache
   // space has changed.
   fFreedSpace = FALSE;

   // Okay, now if we have data in the primary cache and we also have space
   // left in the MM buffer then pack it in after the forced stop data (if any).
   if (dwUsed && dwMMBufSize) {
      // Here we try and completely fill (whats left of) the MM buffer, dwUsed 
      // will be set to the number of bytes actually copied into the buffer.
      if (FAILED(m_pCache->DataGet(pMMDataBuf, dwMMBufSize, &dwUsed))) {
         // We had a problem getting data from the main cache.  If we
         // have data from the forced stop cache the try and play it
         // anyway, otherwise just fail
         if (!dwFSUsed)
            return (FALSE);
      }
      else {
         dwMMBufSize -= dwUsed;        // Compute how many bytes remaining
         fFreedSpace = TRUE;           // Okay, to signal a space change
      }
   } /* End of data in the primary cache */
            
   // Now we update the WAVEHDR so that the buffer can be queued
   m_apBufWH[dwMMBufID]->dwFlags &= ~WHDR_DONE;      
   m_apBufWH[dwMMBufID]->dwBufferLength = m_dwBufSize - dwMMBufSize;

   // Attach a bookmark to it.  This is computed by taking the total 
   // number of bytes sent via DataSet() and subtracting off the number 
   // of bytes waiting to be played (still in the caches).
   m_pFSCache->Used(&dwFSUsed);
   m_pCache->Used(&dwUsed);
   m_aqwBufTime[dwMMBufID] = m_qwTotal - dwFSUsed - dwUsed;

   // Now queue the buffer
   if (waveOutWrite(m_hWaveOut, m_apBufWH[dwMMBufID], sizeof(WAVEHDR)))
      return (FALSE);      // Failed to add buffer

   // If this is the first time we're writing to an opened file then
   // remember the position
   if (m_fJustOpenedOrReset) {
      m_fJustOpenedOrReset = FALSE;
      m_qwOffsetWaveOutPosn = m_aqwBufTime[dwMMBufID] -
         m_apBufWH[dwMMBufID]->dwBufferLength;
   }

   // Flag the buffer as in use
   m_fBufsOut |= dwMMBufMask;

   // If some cache space in the primary cache has freed up then send 
   // a lazy notification to the application's notification sink (if any).
   if (fFreedSpace)
      PostMessage(m_hWndMonitor, WM_SPACECHANGE, 0, 0);

   // Signal that we successfully queued a buffer for playback
   return (TRUE);
} /* End of TryToSendBufOut() */



/* This closes the wave device */
void CAOut::CloseIt(void)
{
   DWORD    i;
   MMRESULT mmr;

   EnterCriticalSection(&m_CritSec);

   // Reset the sent an empty buffer notification
   m_fSentEmptyBuf = FALSE;

   if (m_hWaveOut) {
      waveOutReset(m_hWaveOut);
      m_fJustOpenedOrReset = TRUE;
      for (i = 0; i < AOUT_NUMBUF; i++) {
         if (m_ahBufGlobal[i])
             mmr = waveOutUnprepareHeader(m_hWaveOut, m_apBufWH[i], sizeof(WAVEHDR));
      }

      if (!waveOutClose(m_hWaveOut))
         m_hWaveOut = NULL;
      if (m_fIsTimerOn) {
         KillTimer (m_hWndMonitor, WAVEOUTTIMER);
         m_fIsTimerOn = FALSE;
      }
   }
   
   LeaveCriticalSection(&m_CritSec);
} /* End of CloseIt() */

/* This deals with a returned buffer returned by MM_WOM_DONE */

void CAOut::ReturnedBuffer(LPWAVEHDR pWH)
{
   DWORD dwCopied;

   // Turn off the bit representing this buffer
   m_fBufsOut &= ~(1 << pWH->dwUser);

//   ASSERT(m_nState != AOSM_UNINITIALIZED);

   // Do right thing based on our current state
   switch (m_nState) {
      case AOSM_IDLE:
      case AOSM_CLAIMED:
         // Here if the user called Stop()
         // Add the buffer to the forced stop cache. We'll play this
         // data first when we get a Start() command.
         if (!m_bFlushing)
            m_pFSCache->DataAdd(pWH->lpData, pWH->dwBufferLength, &dwCopied);
         if (!m_fBufsOut && m_bFlushing) {
            m_bFlushing = FALSE;
         }
         return;

      case AOSM_PLAYING:
      case AOSM_PB_IDLE:
      case AOSM_PB_CLAIMED:
         // Here if we are still playing data
         if (!m_bFlushing) {
            if (m_fRemoved || m_fForceStop) {
               // Special case, either the user removed the audio device while we 
               // were playing or they called Stop() after calling UnClaim().  So
               // we'll just cache the data and save it in case the device comes
               // back or they Claim() and Start() us again.
               m_pFSCache->DataAdd(pWH->lpData, pWH->dwBufferLength, &dwCopied);
               break;
            }

            // Keep a record of what (real-time) time it is, and what QWORD we just got
            // returned, since this was just recorded
            m_qwLastBufTime = m_aqwBufTime[pWH->dwUser];
            m_msLastBufTime = GetCurrentTime();

            // Send bookmark notifications for those in this buffer
            //if (!m_fIsTimerOn)
               ReturnedBookMarks(m_aqwBufTime[pWH->dwUser]);
         
            // Send out any queued data until can't send out anymore
            while (TryToSendBufOut())
               /* Loop body intentionally left blank */ ;
         }
         break;
   }

   // If one or more buffers need to be returned then just wait for 'em
   if (m_fBufsOut)
      return;
   
   // Okay we got all the buffers back so reset the forced stop flag
   // and the flush in progress flag
   m_fForceStop = FALSE;
   if (m_bFlushing) {
       m_bFlushing = FALSE;
   }

   // If the device was removed by the user then close it and exit
   if (m_fRemoved) {
      CloseIt();
      return;
   }

   // If were still playing and just ran out of data then return
   if (m_nState == AOSM_PLAYING)
      return;

   // Here if no buffers are out and we are in a PB state
   // First we set the state variable to the non-PB version
   m_nState &= ~AOSM_PBFLAG;

   // Now we reset the audio device 
   waveOutReset(m_hWaveOut);
   m_fJustOpenedOrReset = TRUE;

   // And close the device if it hasn't been claimed
   if (m_nState == AOSM_IDLE)
      CloseIt();
} /* End of ReturnedBuffer() */


void CAOut::ReturnedBookMarks(QWORD qwTime)
{
   PCList      pbmList = m_pBookMarkList;
   PBOOKMARK   pbm;

   while(pbmList->GetNumElems()) {
      pbm = (PBOOKMARK) pbmList->GetElem(0);
      if (qwTimeCmp(pbm->qwCurrTime, qwTime) <= 0) {  // if this one's time is up . . .
         if (m_pIADNS) {
            //  . . and there's a notification interface
#if TRACE_BOOKMARKS
            char dbgbuf[2048];
            ++nRetBookMarks;
            sprintf(dbgbuf, "<<< Returned Bookmark #%d (%d) Time - %I64x\r\n", pbm->dwMarkID, nRetBookMarks, pbm->qwCurrTime);
            OutputDebugString(dbgbuf);
#endif /* TRACE_BOOKMARKS */
            m_pIADNS->BookMark(pbm->dwMarkID, m_bFlushing);   // notify.
         }
#if TRACE_BOOKMARKS
         else {
            OutputDebugString("Warning - Need to return BookMark but no NS registered\r\n");
         }
#endif /* TRACE_BOOKMARKS */
         pbmList->RemoveElem(0);                            // Remove book mark from list.
      }
      else
         break;
   }
} /* End of ReturnedBookMarks() */


INT CAOut::qwTimeCmp(QWORD a, QWORD b)
{
   if (a > b)
      return 1;
   else if (a < b)
      return -1;
   else
      return 0;
} /* End of qwTimeCmp() */


/* This sends a change-of-notify message to the user-code.
   If no user code is specified then nothing is sent */

void CAOut::Notify(void)
{
   DWORD dwFree;
   DWORD dwUsed;

   // Notify the application
   if (m_pIADNS) {
      m_pCache->Space(&dwFree);
      // Only send the notification if there is some free space
      if (dwFree) {
         // Find out how much space is used
         m_pCache->Used(&dwUsed);

         // Abort if the cache is empty and we already told the app          
         if (!dwUsed && m_fSentEmptyBuf)
            return;

         // Flag the notification type (empty or not empty)
         m_fSentEmptyBuf = !dwUsed;

         // Send the notification
         m_pIADNS->FreeSpace(dwFree, FALSE);
      }
   }
} /* End of Notify() */

void CAOut::NotifyAudioStart(void)
{
   if (m_pIADNS) {
      m_pIADNS->AudioStart();
   }
} /* End of NotifyAudioStart() */

void CAOut::NotifyAudioStop(WORD wReason)
{
   if (m_pIADNS) {
      m_pIADNS->AudioStop(wReason);
   }
} /* End of NotifyAudioStop() */


/************************************************************************
CAOutIAudioDest - AOutIAudioDest interface for TTS Manager
*/

CAOutIAudioDest::CAOutIAudioDest (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef = 0;
   m_pObj = pObj;
   m_punkOuter = punkOuter;
}



CAOutIAudioDest::~CAOutIAudioDest (void)
{
// THis space intentionally left blank
}



STDMETHODIMP CAOutIAudioDest::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return m_punkOuter->QueryInterface(riid,ppv);
}



STDMETHODIMP_ (ULONG) CAOutIAudioDest::AddRef(void)
{
   ++m_cRef;
   return m_punkOuter->AddRef();
}



STDMETHODIMP_(ULONG) CAOutIAudioDest::Release(void)
{
   --m_cRef;
   return m_punkOuter->Release();
}

STDMETHODIMP CAOutIAudioDest::BookMark(DWORD dwMarkID)
{
   PCAOut   pCAOut = (PCAOut) m_pObj;
   BOOKMARK bm;
   PCList   pbmList = pCAOut->m_pBookMarkList;


   // Validate that the call is allowed
   if (pCAOut->m_nState == AOSM_UNINITIALIZED)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   pCAOut->m_pAOutIAudio->TotalGet(&bm.qwCurrTime);
   bm.dwMarkID = dwMarkID;
   if (!pbmList->AddElem((PVOID) &bm, sizeof(BOOKMARK))) {
#if TRACE_BOOKMARKS
      OutputDebugString("BookMark Set - FAILED\r\n");
#endif /* TRACE_BOOKMARKS */
      return (E_FAIL);
   }

#if TRACE_BOOKMARKS
   char dbgbuf[2048];

   ++nBookMarks;
   sprintf(dbgbuf, ">>> Set Bookmark #%d (%d) Time - %I64x\r\n", dwMarkID, nBookMarks, bm.qwCurrTime);
   OutputDebugString(dbgbuf);
#endif /* TRACE_BOOKMARKS */

   return (NOERROR);
} /* End of BookMark() */



STDMETHODIMP CAOutIAudioDest::DataSet(PVOID pBuffer, DWORD dwSize)
{
   PCAOut   pCAOut = (PCAOut) m_pObj;
   HRESULT  hRes;
   PQWORD   pQW = &pCAOut->m_qwTotal;
   DWORD    dwCopied;

   // Validate parameters
   if (!pBuffer) 
      return (ResultFromScode(AUDERR_INVALIDPARAM));

   // Allow empty cache notifications
   pCAOut->m_fSentEmptyBuf = FALSE;

   switch (pCAOut->m_nState) {
      case AOSM_UNINITIALIZED:
         return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

      case AOSM_IDLE:
      case AOSM_PB_IDLE:
         return (ResultFromScode(AUDERR_NOTCLAIMED));
   }

   // Here if we allow DataSet() calls
   // Determine if there is enough space in cache
   hRes = pCAOut->m_pCache->Space(&dwCopied);
   if (hRes || dwCopied < dwSize) 
      return (ResultFromScode(AUDERR_NOTENOUGHDATA));

   // Yes, so put it in the cache
   hRes = pCAOut->m_pCache->DataAdd(pBuffer, dwSize, &dwCopied);

   // increase the total bytes played, doing carry
   *pQW = *pQW + dwCopied;

   // If we have been started, then try and send out the data
   if (pCAOut->m_nState == AOSM_PLAYING) {
      // send out any buffers that we can
      while (pCAOut->TryToSendBufOut())
         /* Loop body intentionally left blank */ ;
   }

   return (hRes);
} /* End of DataSet() */



STDMETHODIMP CAOutIAudioDest::FreeSpace(DWORD *pdwBytes, BOOL *pfEOF)
{
   PCAOut pCAOut = (PCAOut) m_pObj;

   // Validate input parameter
   if (pdwBytes == NULL)  
      return (AUDERR_INVALIDPARAM);

   // Validate that the call is allowed
   if (pCAOut->m_nState == AOSM_UNINITIALIZED)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   if (pfEOF)
      *pfEOF = FALSE;
   HRESULT hRes;
   hRes = pCAOut->m_pCache->Space(pdwBytes);

   return hRes;
} /* End of FreeSpace() */

/************************************************************************
CAOutIAudioMultiMediaDevice - AOutIAudioMultiMediaDevice interface for TTS Manager
*/

CAOutIAudioMultiMediaDevice::CAOutIAudioMultiMediaDevice (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef = 0;
   m_pObj = pObj;
   m_punkOuter = punkOuter;
}



CAOutIAudioMultiMediaDevice::~CAOutIAudioMultiMediaDevice (void)
{
// THis space intentionally left blank
}



STDMETHODIMP CAOutIAudioMultiMediaDevice::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return m_punkOuter->QueryInterface(riid,ppv);
}



STDMETHODIMP_ (ULONG) CAOutIAudioMultiMediaDevice::AddRef(void)
{
   ++m_cRef;
   return m_punkOuter->AddRef();
}



STDMETHODIMP_(ULONG) CAOutIAudioMultiMediaDevice::Release(void)
{
   --m_cRef;
   return m_punkOuter->Release();
}


STDMETHODIMP CAOutIAudioMultiMediaDevice::CustomMessage(UINT uMessage, SDATA sData)
{
   PCAOut pCAOut = (PCAOut) m_pObj;

   // Validate that the call is allowed
   if (pCAOut->m_nState == AOSM_UNINITIALIZED)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   if (pCAOut->m_hWaveOut != NULL)
      waveOutMessage(pCAOut->m_hWaveOut, uMessage, (DWORD) sData.dwSize, (DWORD) sData.pData);

   return (NOERROR);
} /* End of CustomMessage() */


STDMETHODIMP CAOutIAudioMultiMediaDevice::DeviceNumGet(DWORD *pdwDevice)
{
   PCAOut pCAOut = (PCAOut) m_pObj;

   if (!pdwDevice)
      return (ResultFromScode(AUDERR_INVALIDPARAM));

   *pdwDevice = AOutNameToDeviceID(pCAOut->m_dwWaveDev, pCAOut->m_szDeviceName);
   return (NOERROR);
} /* End of DeviceNumGet() */


STDMETHODIMP 
CAOutIAudioMultiMediaDevice::DeviceNumSet(DWORD dwDevice)
{
   PCAOut pCAOut = (PCAOut) m_pObj;

   // First check and see if we have a non-NULL wave handle.  If so we
   // assume that its valid and that we are currently playing audio. Thus
   // we return an error code.
   if (pCAOut->m_hWaveOut)
      return (ResultFromScode(AUDERR_WAVEDEVICEBUSY));

   // Check and see if the device ID specified is valid for the
   // current system comfiguration.  If its not then we return an error.
   if (dwDevice >= waveOutGetNumDevs() && dwDevice != WAVE_MAPPER)
      return (ResultFromScode(AUDERR_BADDEVICEID));

   // Check and see if the user has already specified the waveformat for
   // this object instance.  If so we must validate that the chosen device
   // supports the configured waveformat.
   if (pCAOut->m_pWFEX) {
      MMRESULT mm;
      
      // If the caller specified wavemapper as the device ID to set to then we
      // will look for an available device and save it's device ID for later use.
      // In no case will we ever save the special value of WAVE_MAPPER into 
      // m_dwWaveDev since this could cause problems later on (i.e. we succeed here
      // but fail later because the device we found here is now in use.
      if (pCAOut->m_dwWaveDev == WAVE_MAPPER)
         mm = TTSWaveMapper(NULL, (PUINT) &dwDevice, pCAOut->m_pWFEX,
                            NULL, NULL, WAVE_FORMAT_QUERY);
      else
         mm = waveOutOpen(NULL, dwDevice, pCAOut->m_pWFEX, NULL, NULL, 
                          WAVE_FORMAT_QUERY);
      
      // If we failed to open the requested device, then tell the user that the
      // configured waveformat is not supported by the device.
      if (mm)
         return (ResultFromScode(AUDERR_WAVEFORMATNOTSUPPORTED));

      // We shouldn't allow synchronous devices to be used 
      // Save/Update the device capabilities
      waveOutGetDevCaps(pCAOut->m_dwWaveDev, &pCAOut->m_wc, sizeof(WAVEOUTCAPS));
      if (pCAOut->m_wc.dwSupport & WAVECAPS_SYNC)
         return (AUDERR_SYNCNOTALLOWED);
   }

   // Save both the device ID and the name
   pCAOut->m_dwWaveDev = dwDevice;
   AOutDeviceIDToName(pCAOut->m_dwWaveDev, pCAOut->m_szDeviceName);

   return (NOERROR);
} /* End of DeviceNumSet() */


/************************************************************************
CAOutIAudio - AOutIAudio interface for TTS Manager
*/

CAOutIAudio::CAOutIAudio (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef = 0;
   m_pObj = pObj;
   m_punkOuter = punkOuter;
}


CAOutIAudio::~CAOutIAudio (void)
{
   if (((PCAOut) m_pObj)->m_pIADNS) {
       ((PCAOut) m_pObj)->m_pIADNS->Release();
       ((PCAOut) m_pObj)->m_pIADNS = NULL;
   }
}



STDMETHODIMP CAOutIAudio::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return m_punkOuter->QueryInterface(riid,ppv);
}



STDMETHODIMP_ (ULONG) CAOutIAudio::AddRef(void)
{
   ++m_cRef;
   return m_punkOuter->AddRef();
}



STDMETHODIMP_(ULONG) CAOutIAudio::Release(void)
{
   --m_cRef;
   return m_punkOuter->Release();
}


STDMETHODIMP CAOutIAudio::Flush(void)
{
   PCAOut   pCAOut = (PCAOut) m_pObj;
   MMRESULT mm;

   // Validate that the call is allowed
   switch (pCAOut->m_nState) {
      case AOSM_UNINITIALIZED:
         return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

      case AOSM_IDLE:
         pCAOut->m_pCache->DataFlush();
         pCAOut->m_pFSCache->DataFlush();
         pCAOut->m_bFlushing = TRUE;
         pCAOut->ReturnedBookMarks(pCAOut->m_qwTotal);
         pCAOut->m_bFlushing = FALSE;
         return (NOERROR);
   }

   // Here if we are playing audio
   pCAOut->m_bFlushing = TRUE;
   pCAOut->m_pCache->DataFlush();
   pCAOut->m_pFSCache->DataFlush();
   pCAOut->ReturnedBookMarks(pCAOut->m_qwTotal);
   if (!pCAOut->m_fBufsOut)
      pCAOut->m_bFlushing = FALSE;
      
   mm = waveOutReset(pCAOut->m_hWaveOut);
   pCAOut->m_fJustOpenedOrReset = TRUE;

   switch (mm) {
      case MMSYSERR_NOERROR:        return NOERROR;
      case MMSYSERR_INVALHANDLE:    return AUDERR_INVALIDHANDLE;
      case MMSYSERR_HANDLEBUSY:     return AUDERR_HANDLEBUSY;
      default:                      return E_FAIL;
   }

   return (NOERROR);
} /* End of Flush() */



STDMETHODIMP CAOutIAudio::LevelGet(DWORD *pdwLevel)
{
   PCAOut pCAOut = (PCAOut) m_pObj;
   HWAVEOUT hWaveOut;

   if (pdwLevel == NULL)
      return (AUDERR_INVALIDPARAM);

   // Validate that the call is allowed
   if (pCAOut->m_nState == AOSM_UNINITIALIZED)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   hWaveOut = (pCAOut->m_hWaveOut == NULL) ? (HWAVEOUT) pCAOut->m_dwWaveDev : pCAOut->m_hWaveOut;
   if (waveOutGetVolume(hWaveOut, pdwLevel))
      return (ResultFromScode(AUDERR_NOTSUPPORTED));

   return (NOERROR);
} /* End of LevelGet() */



STDMETHODIMP CAOutIAudio::LevelSet(DWORD dwLevel)
{
   PCAOut pCAOut = (PCAOut) m_pObj;
   HWAVEOUT hWaveOut;

   // Validate that the call is allowed
   if (pCAOut->m_nState == AOSM_UNINITIALIZED)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   hWaveOut = (pCAOut->m_hWaveOut == NULL) ? (HWAVEOUT) pCAOut->m_dwWaveDev : pCAOut->m_hWaveOut;
   if (waveOutSetVolume(hWaveOut, dwLevel))
      return (ResultFromScode(AUDERR_NOTSUPPORTED));

   return (NOERROR);
} /* End of LevelSet() */



STDMETHODIMP CAOutIAudio::PassNotify(PVOID pIADNS, IID iid)
{
   PCAOut pCAOut = (PCAOut) m_pObj;

#ifdef _DEBUG
#define GUIDSTRBUFSIZE  64
   OLECHAR IIDGUIDStrW[GUIDSTRBUFSIZE];
   CHAR IIDGUIDStrA[GUIDSTRBUFSIZE];
 
   StringFromGUID2(iid, IIDGUIDStrW, GUIDSTRBUFSIZE);
   WideCharToMultiByte(CP_ACP, 0, IIDGUIDStrW, -1, IIDGUIDStrA, GUIDSTRBUFSIZE, NULL, NULL);
#endif /* _DEBUG */

   // If the IID is not for an audio-dest notify sink then error
   if (pIADNS != NULL) {
      if (!IsEqualIID(iid, IID_IAudioDestNotifySink))
         return (ResultFromScode(AUDERR_INVALIDNOTIFYSINK));

      // AddRef() the user's notification sink
      ((PIAUDIODESTNOTIFYSINK) pIADNS)->AddRef();
   }

   // If we already had a notification sink then release it
   if (pCAOut->m_pIADNS)
      pCAOut->m_pIADNS->Release();

   // Use the new notification sink (NULL is allowed)
   pCAOut->m_pIADNS = (PIAUDIODESTNOTIFYSINK) pIADNS;

   return (NOERROR);
} /* End of PassNotify() */


STDMETHODIMP CAOutIAudio::PosnGet(PQWORD pqWord)
{
   PCAOut pCAOut = (PCAOut) m_pObj;
   DWORD dwUsed;
   DWORD dwFSUsed;

   // Validate the parameter
   if (pqWord == NULL)
      return (AUDERR_INVALIDPARAM);

   // Validate that the call is allowed
   if (pCAOut->m_nState == AOSM_UNINITIALIZED)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   // If the wave device is open then just ask it
   if (pCAOut->m_hWaveOut && !pCAOut->m_fDriverCrashOnPosn) {
      MMTIME   mmTime;
      mmTime.wType = TIME_BYTES;
      waveOutGetPosition (pCAOut->m_hWaveOut, &mmTime, sizeof(mmTime));
      *pqWord = pCAOut->m_qwOffsetWaveOutPosn + mmTime.u.cb;
      return NOERROR;
   }

   // If we use m_qwTotal, it's accurate to within 1/16 of a second
   // This is computed by taking the total number of bytes sent via
   // DataSet() and subtracting off the number of bytes waiting to
   // be played.
   pCAOut->m_pCache->Used(&dwUsed);
   pCAOut->m_pFSCache->Used(&dwFSUsed);

   *pqWord = pCAOut->m_qwTotal - dwUsed - dwFSUsed;
   return (NOERROR);
} /* End of PosnGet() */


// Function:  Claim()
//
// Description:  Claim() will open the audio device and prepare all the
//               wave headers/buffers for playing.

STDMETHODIMP CAOutIAudio::Claim(void)
{
   PCAOut pCAOut = (PCAOut) m_pObj;
   HRESULT  hRes;

   // Validate that the call is allowed
   switch (pCAOut->m_nState) {
      case AOSM_UNINITIALIZED:
         return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

      case AOSM_IDLE:
         // Here if the wave format has been set, the
         // device is not claimed, and were not playing
         // any audio from a previous session.  Thus
         // we will open the waveaudio device and prepare
         // all of the buffers and headers.
         if ((hRes = pCAOut->OpenAndPrepare()) == MMSYSERR_NOERROR)
            pCAOut->m_nState = AOSM_CLAIMED;
         return (hRes);

      case AOSM_PB_IDLE:
         // Here if we are unclaimed but still playing audio
         // from a previous session.  Thus the waveaudio device
         // is already open and the buffers are prepared.
         pCAOut->m_nState = AOSM_PB_CLAIMED;
         return (NOERROR);
   }

   // Not one of the above states so tell the app that the device
   // has already been claimed.
   return (ResultFromScode(AUDERR_ALREADYCLAIMED));
} /* End of Claim() */

STDMETHODIMP CAOutIAudio::UnClaim(void)
{
   PCAOut pCAOut = (PCAOut) m_pObj;

   switch (pCAOut->m_nState) {
      case AOSM_UNINITIALIZED:
         return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

      case AOSM_CLAIMED:
         // Here if the device has been claimed, but
         // is currently not playing.  We just unprepare
         // the wave headers/buffers and close the device.
         pCAOut->CloseIt();
         pCAOut->m_nState = AOSM_IDLE;
         return (NOERROR);

      case AOSM_PLAYING:
      case AOSM_PB_CLAIMED:
         // Here if we are currently playing data. We'll
         // just flag that the device is not claimed and
         // wait for all the data to play before actually
         // closing the device.
         if (!pCAOut->m_fBufsOut) {
            // Special case, no buffers are out so we
            // can process the UnClaim() request right
            // now!
            pCAOut->CloseIt();
            pCAOut->m_nState = AOSM_IDLE;

            // If the device has been removed then simulate an AudioStop()
            // We also flush any queued data since we are pretending that
            // it all played.
            if (pCAOut->m_fRemoved) {
               Flush();
               pCAOut->NotifyAudioStop(IANSRSN_NODATA);
            }
         }
         else
            pCAOut->m_nState = AOSM_PB_IDLE;
         return (NOERROR);
   }

   // Here if the device has not been claimed
   return (ResultFromScode(AUDERR_NOTCLAIMED));
} /* End of UnClaim() */




STDMETHODIMP CAOutIAudio::Start(void)
{
   PCAOut pCAOut = (PCAOut) m_pObj;

   switch (pCAOut->m_nState) {
      case AOSM_UNINITIALIZED:
         return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

      case AOSM_CLAIMED:
         // Send as much data as possible to the device
         while (pCAOut->TryToSendBufOut())
            /* Loop body intentionally left blank */ ;

         // Reset the forced stop flag
         pCAOut->m_fForceStop = FALSE;
         pCAOut->m_nState = AOSM_PLAYING;
         return (NOERROR);

      case AOSM_PLAYING:
         return (ResultFromScode(AUDERR_ALREADYSTARTED));

      case AOSM_PB_CLAIMED:
         pCAOut->m_nState = AOSM_PLAYING;
         return (NOERROR);
   }

   // Here if the device is not claimed
   return (ResultFromScode(AUDERR_NOTCLAIMED));
} /* End of Start() */


STDMETHODIMP CAOutIAudio::Stop(void)
{
   PCAOut pCAOut = (PCAOut) m_pObj;

   switch (pCAOut->m_nState) {
      case AOSM_UNINITIALIZED:
         return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

      case AOSM_PB_IDLE:
      case AOSM_PB_CLAIMED:
         // Stop the audio from playing and get buffers back
         waveOutReset(pCAOut->m_hWaveOut);
         pCAOut->m_fJustOpenedOrReset = TRUE;

         // Flag that this is a forced stop
         pCAOut->m_fForceStop = TRUE;
         break;

      case AOSM_PLAYING:
         // Stop the audio from playing and get buffers back
         waveOutReset(pCAOut->m_hWaveOut);
         pCAOut->m_fJustOpenedOrReset = TRUE;

         // Flag that this is a forced stop
         pCAOut->m_fForceStop = TRUE;

         // Flag that we are no longer playing
         pCAOut->m_nState = AOSM_CLAIMED;
         break;
   }

   // Here if not playing
   return (NOERROR);
} /* End of Stop() */


STDMETHODIMP CAOutIAudio::ToFileTime(PQWORD pqWord, FILETIME *pFT)
{
   PCAOut pCAOut = (PCAOut) m_pObj;
   _int64 iByteDelta, iMSDelta;
   DWORD  dwMSTime;
   QWORD  qwFT;

   // error checks
   if (pqWord == NULL || pFT == NULL)
      return (AUDERR_INVALIDPARAM);

   // Validate that the call is allowed
   if (pCAOut->m_nState == AOSM_UNINITIALIZED)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   // Find the difference between the last buffer that we saw returned (giving
   // us a correlation between qw-Time and millisecond-time. and find the differece.
   iByteDelta = (_int64) *pqWord - (_int64) pCAOut->m_qwLastBufTime;

   // This is a delta (in bytes) from the time. Convert this to milliseconds
   iMSDelta = (iByteDelta * 1000) / pCAOut->m_pWFEX->nAvgBytesPerSec;

   // Add this to the time of the last buffer (in milliseconds) & figure out what time
   // this happened. How many millseconds after the inception of the audio object
   dwMSTime = (DWORD) ((pCAOut->m_msLastBufTime - pCAOut->m_msStartTime) + (long) iMSDelta);

   // Convert this to a FILETIME scale, where 100 ns is 1 unit. 1 ms = 1,000,000 ns => 1 ms = 10,000 units
   qwFT = (QWORD) dwMSTime * 10000;

   // add this to the starting file-time & ship it out
   *((QWORD *)pFT) = qwFT + *((QWORD*) &(pCAOut->m_ftStartTime));

   // done
   return (NOERROR);
} /* End of ToFileTime() */


STDMETHODIMP CAOutIAudio::TotalGet(PQWORD pqWord)
{
   PCAOut pCAOut = (PCAOut) m_pObj;

   // Validate the argument
   if (!pqWord) 
      return (ResultFromScode(AUDERR_INVALIDPARAM));

   // Validate that the call is allowed
   if (pCAOut->m_nState == AOSM_UNINITIALIZED)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   // Store the total bytes played
   *pqWord = pCAOut->m_qwTotal;

   return (NOERROR);
} /* End of TotalGet() */


STDMETHODIMP CAOutIAudio::WaveFormatGet(PSDATA pData)
{
   PCAOut   pCAOut = (PCAOut) m_pObj;
   LPMALLOC pMalloc;
   HRESULT  hRes;

   // Validate the argument
   if (!pData) 
      return (ResultFromScode(AUDERR_INVALIDPARAM));

   // Validate that the call is allowed
   if (pCAOut->m_nState == AOSM_UNINITIALIZED)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   // Allocate memory for returning the waveformat 
   if (hRes = CoGetMalloc(MEMCTX_TASK, &pMalloc)) 
      return (hRes);
   pData->pData = (PVOID) pMalloc->Alloc(pCAOut->m_dwWFEXSize);
   pMalloc->Release();
   if (!pData->pData) 
      return (ResultFromScode(E_OUTOFMEMORY));

   // Copy the waveformat into the buffer and return it
   pData->dwSize = pCAOut->m_dwWFEXSize;
   memcpy(pData->pData, pCAOut->m_pWFEX, pCAOut->m_dwWFEXSize);

   return (NOERROR);
} /* End of WaveFormatGet() */



STDMETHODIMP CAOutIAudio::WaveFormatSet(SDATA pData)
{
   PVOID    pNew;
   PCAOut   pCAOut = (PCAOut) m_pObj;
   DWORD    i;
   MMRESULT mm;

   // Validate the parameter
   if (!pData.pData) 
      return (ResultFromScode(AUDERR_INVALIDPARAM));

   // Validate that we can set the format
   if (pCAOut->m_nState != AOSM_UNINITIALIZED)
      return (ResultFromScode(AUDERR_WAVEDEVICEBUSY));

   // Validate that the device can support the format
   pCAOut->m_dwWaveDev = AOutNameToDeviceID(pCAOut->m_dwWaveDev, pCAOut->m_szDeviceName);
   mm = (pCAOut->m_dwWaveDev == WAVE_MAPPER)
      ? TTSWaveMapper(NULL, (UINT*) &pCAOut->m_dwWaveDev, (LPWAVEFORMATEX) pData.pData,
                    NULL, NULL, WAVE_FORMAT_QUERY)
      : waveOutOpen(NULL, pCAOut->m_dwWaveDev, (LPWAVEFORMATEX) pData.pData,
                    NULL, NULL, WAVE_FORMAT_QUERY);
   if (mm) {
      switch(mm) {
         case MMSYSERR_ALLOCATED:
            break;

         case MMSYSERR_BADDEVICEID:
            return (ResultFromScode(AUDERR_BADDEVICEID));

         case MMSYSERR_NOMEM: 
            return (ResultFromScode(AUDERR_NOTSUPPORTED));

         case WAVERR_BADFORMAT: 
            return (ResultFromScode(AUDERR_WAVEFORMATNOTSUPPORTED));

         case MMSYSERR_NODRIVER: 
            return (ResultFromScode(AUDERR_NODRIVER));

         default: 
            return (ResultFromScode(AUDERR_WAVEFORMATNOTSUPPORTED));
      }
   }

   // We shouldn't allow synchronous devices to be used 
   // Save the device capabilities
   waveOutGetDevCaps(pCAOut->m_dwWaveDev, &pCAOut->m_wc, sizeof(WAVEOUTCAPS));
   if (pCAOut->m_wc.dwSupport & WAVECAPS_SYNC)
      return (AUDERR_SYNCNOTALLOWED);

   // some sound cards have buggy wave get posn calls
   //switch (pCAOut->m_wc.wMid) {
   // IMPORTANT: Disable buggy waveOutPosn calls here
   //}

   AOutDeviceIDToName(pCAOut->m_dwWaveDev, pCAOut->m_szDeviceName);
   pNew = malloc (pData.dwSize);
   if (!pNew) 
      return (ResultFromScode(E_OUTOFMEMORY));
   memcpy(pNew, pData.pData, pData.dwSize);
   pCAOut->m_pWFEX = (WAVEFORMATEX *) pNew;
   pCAOut->m_dwWFEXSize = pData.dwSize;

   // Figure out how large the cache has to be
   pCAOut->m_dwBufSize  = pCAOut->m_pWFEX->nAvgBytesPerSec / AOUT_BUFSPERSEC;
   pCAOut->m_dwBufSize -= (pCAOut->m_dwBufSize % 2);   // round off to nearest word
   pCAOut->m_dwBufSize -= (pCAOut->m_dwBufSize % pCAOut->m_pWFEX->nBlockAlign);
   if (!pCAOut->m_dwBufSize) 
      pCAOut->m_dwBufSize = pCAOut->m_pWFEX->nBlockAlign;

   // Create the caches
   pCAOut->m_pCache   = new CCache(pCAOut->m_dwBufSize * AOUT_BUFSPERSEC * AOUT_CACHESIZE);
   pCAOut->m_pFSCache = new CCache(pCAOut->m_dwBufSize * AOUT_BUFSPERSEC * AOUT_CACHESIZE);

   // allocate all of the buffers
   for (i = 0; i < AOUT_NUMBUF; i++) {
      pCAOut->m_ahBufGlobal[i] = GlobalAlloc(GMEM_MOVEABLE, pCAOut->m_dwBufSize + sizeof(WAVEHDR));
      if (!pCAOut->m_ahBufGlobal[i]) {
         free(pNew);
         pCAOut->m_pWFEX = NULL;
         return (ResultFromScode(E_OUTOFMEMORY));
      }
      pCAOut->m_apBufWH[i] = (LPWAVEHDR) GlobalLock(pCAOut->m_ahBufGlobal[i]);
      if (!pCAOut->m_apBufWH[i]) {
         GlobalFree(pCAOut->m_ahBufGlobal[i]);
         pCAOut->m_ahBufGlobal[i] = NULL;
         free(pNew);
         pCAOut->m_pWFEX = NULL;
         return (ResultFromScode(E_OUTOFMEMORY));
      }
   }

   if (!pCAOut->m_pCache || !pCAOut->m_pFSCache) {
      free(pNew);
      pCAOut->m_pWFEX = NULL;
      return (ResultFromScode(E_OUTOFMEMORY));
   }

   // And change state
   pCAOut->m_nState = AOSM_IDLE;

   return (NOERROR);
} /* End of WaveFormatSet() */


/********************************************************************/

/*
      Finds first waveOut, giving you an ID, which an ordinary
      waveOutOpen(..., WAVE_MAPPER, ...) would not.

      Arguments are same as waveOutOpen except that the second is a
      POINTER to an ID - which gets returned - not an ID which the
      user specifies.

      NOTE: dwInstance must be sent in as NULL! - just like waveOutOpen.
 */

MMRESULT TTSWaveMapper(LPHWAVEOUT lphwo, PUINT puDeviceID, LPCWAVEFORMATEX lpwf,
                       DWORD dwCallback, DWORD dwInstance, DWORD fdwOpen)
{
   UINT         n = waveOutGetNumDevs();
   WAVEOUTCAPS  woCaps;
   MMRESULT     mmr;
   LONG         lRes;
   HKEY         hKeyMapper = NULL;
   DWORD        dwNeeded, dwType, dwPref = 0;
   CHAR         pName[MAXPNAMELEN];
   
   if (lphwo)
      *lphwo = NULL;

   lRes = RegOpenKeyEx(HKEY_CURRENT_USER,
                       "Software\\Microsoft\\Multimedia\\Sound Mapper",
                       0, KEY_READ, &hKeyMapper);

   if (lRes != ERROR_SUCCESS)
      goto NoReg;

   dwNeeded = MAXPNAMELEN;
   memset(pName, 0, MAXPNAMELEN);

   // read Pname from the registry
   lRes = RegQueryValueEx(hKeyMapper, "Playback", NULL, &dwType, (PBYTE)pName,
                          &dwNeeded);

   if (lRes != ERROR_SUCCESS)
      goto NoReg;

   // read Preferred setting from the registry
   lRes = RegQueryValueEx(hKeyMapper, "PreferredOnly", NULL, &dwType, (PBYTE)&dwPref,
                          &dwNeeded);

   if (lRes != ERROR_SUCCESS)
      dwPref = FALSE;

   // look for the device that is in the registry
   for (*puDeviceID = 0; *puDeviceID < n; (*puDeviceID)++) {
      memset(&woCaps, 0, sizeof(woCaps));
      mmr = waveOutGetDevCaps(*puDeviceID, &woCaps, sizeof(woCaps));
      if (mmr)
         continue;

      if (!lstrcmpi(woCaps.szPname, pName)) {
         if (waveOutOpen(lphwo, *puDeviceID, lpwf, dwCallback, dwInstance, fdwOpen) == 0) {
            if (hKeyMapper)
               RegCloseKey (hKeyMapper);
            return MMSYSERR_NOERROR;
         }
         else
            break;
      }
   }

// we get here if we either can't open the device that is in the registry
// or we can't find the device that is in the registry or we can't open or
// read from the registry
NoReg:
   if (hKeyMapper)
      RegCloseKey (hKeyMapper);

   if (!dwPref) {
      for (*puDeviceID = 0; *puDeviceID < n; (*puDeviceID)++)
         if (waveOutOpen(lphwo, *puDeviceID, lpwf, dwCallback, dwInstance, fdwOpen) == 0)
            return MMSYSERR_NOERROR;
   }

   // don't gp-fault if no wave-device
   if (lphwo)
      *lphwo = NULL;

   *puDeviceID = (UINT) -1;
   return MMSYSERR_NODRIVER;
}

/* End of audioout.cpp */
