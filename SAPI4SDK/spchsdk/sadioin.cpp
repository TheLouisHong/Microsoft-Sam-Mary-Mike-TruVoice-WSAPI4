/************************************************************************
AudioIn.cpp - Code for audio input.

Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <dbt.h>
#include <winver.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <objbase.h>
#include <objerror.h>
#include <speech.h>
#include "audio.h"

#define TRACE_UNDERFLOW    (0)

#ifndef DRV_QUERYDEVNODE
#define DRV_QUERYDEVNODE   (DRV_RESERVED + 2)
#endif /* DRV_QUERYDEVNODE */

#ifndef MM_DIAMOND
#define MM_DIAMOND         (0x90)
#endif /* MM_DIAMOND */

#ifndef MM_TOSHIBA
#define MM_TOSHIBA         (0x84)
#endif /* MM_TOSHIBA */

/*************************************************************************
Internal prototypes */


BOOL  FindLowPriDriver(HMIXER hMixer, MIXERLINE *pMixerLine, BOOL *fLowPriority, BOOL fTryLowPriority);
INT avFindVolControl(HMIXER hMixer, DWORD* lpMasterVol, DWORD *lpVolControl, MIXERLINE MixerLine);
LPMIXERCONTROL avFindControlOnLine(HMIXER hMixer, DWORD dwLineID, DWORD dwControlType);
DWORD avFindSelectedMuxLine(HMIXER hMixer, DWORD dwDestination, DWORD dwMuxID, DWORD cMultipleItems);
MMRESULT SRWaveMapper(LPHWAVEIN lphwi, PUINT puDeviceID, LPCWAVEFORMATEX lpwf,
                      DWORD dwCallback, DWORD dwInstance, DWORD fdwOpen);

/*************************************************************************
AInDeviceIDToName - This converts a device ID into a device-name string. We
   have to do this because MMSys may change device IDs on us at any time
   cause of PnP.

inputs
   DWORD    dwDeviceID - Device ID number, including WAVE_MAPPER. If this
               is WAVE_MAPPER then the string will be set to an empty string.
   PSTR     pszString - String that is filled in. Must be large enough for
               waveInGetDevCaps() names.
returns
   BOOL - TRUE if succede.
*/
BOOL AInDeviceIDToName(DWORD dwDeviceID, PSTR pszString)
{
   WAVEINCAPS wc;

   if (dwDeviceID == WAVE_MAPPER) {
      pszString[0] = '\0';
      return (TRUE);
   }

   if (waveInGetDevCaps(dwDeviceID, &wc, sizeof(WAVEINCAPS)))
      return (FALSE);

   strcpy(pszString, wc.szPname);
   return (TRUE);
} /* End of AInDeviceIDToName() */

/*************************************************************************
AInNameToDeviceID - This converts a name into a device ID. We
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
DWORD AInNameToDeviceID(DWORD dwDeviceID, PSTR pszString)
{
   WAVEINCAPS  wc;
   DWORD       i, num;

   // First off, if wave_mapper then do nothing
   if (dwDeviceID == WAVE_MAPPER) {
      return (dwDeviceID);
   }

   // Check out the first device. If it works then go for it
   if (!waveInGetDevCaps (dwDeviceID, &wc, sizeof(wc))) {
      if (!strcmp (pszString, wc.szPname))
         return (dwDeviceID);
   }

   // Else, search through all of the devices & look for the name
   num = waveInGetNumDevs();
   for (i = 0; i < num; i++)
      if (!waveInGetDevCaps (i, &wc, sizeof(wc)))
         if (!strcmp (pszString, wc.szPname))
            return (i);

   // else can't find
   return (dwDeviceID);
} /* End of AInNameToDeviceID() */

#if 0
void
DumpBroadcastHdr(PDEV_BROADCAST_HDR pDevBroadCastHdr)
{
   char buf[256];

   OutputDebugString("Broadcast Header:  (");
   switch (pDevBroadCastHdr->dbch_devicetype) {
      case DBT_DEVTYP_OEM:
         OutputDebugString("OEM)\r\n");
         wsprintf(buf, "dbco_identifier:  %x\r\n", ((PDEV_BROADCAST_OEM) pDevBroadCastHdr)->dbco_identifier);
         OutputDebugString(buf);
         wsprintf(buf, "dbco_suppfunc:  %x\r\n", ((PDEV_BROADCAST_OEM) pDevBroadCastHdr)->dbco_suppfunc);
         OutputDebugString(buf);
         break;

      case DBT_DEVTYP_DEVNODE:
         OutputDebugString("DEVNODE)\r\n");
         wsprintf(buf, "dbcd_devnode:  %x\r\n", ((PDEV_BROADCAST_DEVNODE) pDevBroadCastHdr)->dbcd_devnode);
         OutputDebugString(buf);
         break;

      case DBT_DEVTYP_VOLUME:
         OutputDebugString("VOLUME)\r\n");
         wsprintf(buf, "dbcv_unitmask:  %x\r\n", ((PDEV_BROADCAST_VOLUME) pDevBroadCastHdr)->dbcv_unitmask);
         OutputDebugString(buf);
         wsprintf(buf, "dbcv_flags:  %x\r\n", ((PDEV_BROADCAST_VOLUME) pDevBroadCastHdr)->dbcv_flags & 0xFFFF);
         OutputDebugString(buf);
         break;

      case DBT_DEVTYP_PORT:
         OutputDebugString("PORT)\r\n");
         wsprintf(buf, "dbcp_name:  %s\r\n", ((PDEV_BROADCAST_PORT) pDevBroadCastHdr)->dbcp_name);
         OutputDebugString(buf);
         break;

      case DBT_DEVTYP_NET:
         OutputDebugString("NET)\r\n");
         wsprintf(buf, "dbcn_resource:  %x\r\n", ((PDEV_BROADCAST_NET) pDevBroadCastHdr)->dbcn_resource);
         OutputDebugString(buf);
         wsprintf(buf, "dbcn_flags:  %x\r\n", ((PDEV_BROADCAST_NET) pDevBroadCastHdr)->dbcn_flags);
         OutputDebugString(buf);
         break;
   }
   return;
} /* End of DumpBroadcastHdr() */
#endif /* 0 */

/************************************************************************
HandleDeviceChange - Process the WM_DEVICECHANGE message.

inputs
   pAIn - pointer to the Audio Input Object
   dwEvent - the specific event ID
   pDevBroadcastHdr - pointer to the device broadcast header

returns
   TRUE/FALSE - dependent on specific event.
*/
BOOL CAIn::HandleDeviceChange(DWORD dwEvent, PDEV_BROADCAST_HDR pDevBroadcastHdr)
{
   switch (dwEvent) {
      case DBT_DEVICEARRIVAL:
         // This message is sent when a device has been
         // inserted and is now available
         if (pDevBroadcastHdr->dbch_devicetype == DBT_DEVTYP_DEVNODE) {
            // Okay, a new device has arrived let's check and see if it's our
            // audio device.  First we have to re-determine the devnode of our
            // audio device.
            if (!waveInMessage((HWAVEIN) m_dwWaveDev, DRV_QUERYDEVNODE, (LPARAM) &m_dnDevNode, 0)) {
               if (((PDEV_BROADCAST_DEVNODE) pDevBroadcastHdr)->dbcd_devnode == m_dnDevNode) {

                  // Cool! Our audio device has come back and the application has not 
                  // stopped us yet.  So we'll open and prepare a new set of buffers
                  // and resume recording.

                  if (m_fClaimed) {
                     if (SUCCEEDED(OpenAndPrepare())) {
                        if (m_bRecording) {
                           while (TryToSendBufOut())
                              /* Loop body intentionally left blank */ ;
                           waveInStart(m_hWaveIn);
                        } /* End of we were recording */
                     } /* End of opened audio device */
                     else {
                        m_fRemoved = FALSE;
                     }
                  } /* End of device is claimed */
                  else {
                     m_fRemoved = FALSE;
                  }
               } /* End of device is our wavein device */
            } /* End of got our device's devnode */
         } /* End of broadcast type is a devnode */
         return (TRUE);

      case DBT_DEVICEQUERYREMOVE:
         // This message is sent to request permission to remove
         // a device.  Any application can deny this request and 
         // cancel the removal.  We'll deny the request if the
         // audio device has been claimed.
         if (m_fClaimed) {
            return (FALSE);
         }
         return (TRUE);

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

               // Reset and close the waveaudio device so that the device driver 
               // can clean-up.  We'll reopen/prepare if the device comes back
               // before the application calls stop.
               m_fRemoved = TRUE;
               if (m_fClaimed) {
                  waveInReset(m_hWaveIn);
                  CloseIt();
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
AInWindowProc - Window for the audio input..
*/

LRESULT CALLBACK AInWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   PCAIn pAIn = (PCAIn) GetWindowLong(hWnd, 0);
   int i;

#define WM_SPACECHANGE     (WM_USER + 142)
#define WM_OVERFLOW        (WM_USER + 145)

   switch (uMsg) {
      case WM_CREATE:
         pAIn = (PCAIn) ((CREATESTRUCT *) lParam)->lpCreateParams;
         SetWindowLong (hWnd, 0, (DWORD) pAIn);
         return (0);

      case WM_CLOSE:
         return (0);   // dont close

      case WM_DEVICECHANGE:
        return (pAIn->HandleDeviceChange((DWORD) wParam, (DEV_BROADCAST_HDR *) lParam));

      case WM_TIMER:
         // The timer is set when a TryToStart() was called, but 
         // it failed because the wave device was busy.  The timer 
         // is set to 10 times a second, and will try to open the 
         // wave-in device until it opens, or the device is stopped.
         if (!pAIn->TryToStart()) {
            // We succeded!!!
            KillTimer (hWnd, pAIn->m_uTimer);
            pAIn->m_uTimer = 0;
         }
         return (0);

      case MM_WIM_OPEN:
         if (!pAIn->m_fRemoved)
            pAIn->NotifyAudioStart();
         else
            pAIn->m_fRemoved = FALSE;
         return (0);

      case MM_WIM_CLOSE:
         pAIn->m_fIgnoreDataUntilNextClose = FALSE;
         if (!pAIn->m_fRemoved)
            pAIn->NotifyAudioStop(IANSRSN_NODATA);
         return (0);

      case MM_WIM_DATA:
         if (pAIn->m_fIgnoreDataUntilNextClose)
            return 0;

         for (i = 0; i < AIN_NUMBUF; i++) {
            if (pAIn->m_apBufWH[i] && pAIn->m_apBufWH[i] == (LPWAVEHDR) lParam) {
#if TRACE_UNDERFLOW
               int nPrevBuffer;
               nPrevBuffer = i == 0 ? AIN_NUMBUF - 1 : i - 1;
               if (pAIn->m_apBufWH[nPrevBuffer]->dwFlags & WHDR_DONE)
                  OutputDebugString("AudioSrc:  Buffer Underflow!\r\n");
#endif /* TRACE_UNDERFLOW */
               pAIn->ReturnedBuffer((LPWAVEHDR) lParam);
               return (0);
            }
         }
         return (0);

      case WM_SPACECHANGE:
         pAIn->Notify();
         return (0);

      case WM_OVERFLOW:
         pAIn->NotifyOverflow((DWORD) lParam);
         return (0);
   }
   return (DefWindowProc(hWnd, uMsg, wParam, lParam));
} /* End of AInWindowProc()  */


/************************************************************************
CCache::CCache - Create a cache object.

inputs
   DWORD dwCacheSize - Number of bytes to make the cache.
returns
   standard
*/

CCache::CCache(DWORD dwCacheSize)
{
   m_dwCacheSize = dwCacheSize;
   m_pData = NULL;
   m_dwStartOffset = 0;
   m_dwData = 0;
}


/************************************************************************
CCache::~CCache - Free up the cache object
*/

CCache::~CCache(void)
{
   if (m_pData)
      free(m_pData);
}


/************************************************************************
CCache::MakeSureTheresMemory - This makes sure that there's memory
   allocated for the object. If it fails (and theres no memory)
   then this returns a HRESULT.

returns
   HRESULT - 0 if there's memory. error if an error occured.
*/
HRESULT CCache::MakeSureTheresMemory(void)
{
   if (m_pData)
      return (NOERROR);

   m_pData = (BYTE*) malloc (m_dwCacheSize);
   return ((m_pData != NULL) ? NOERROR : ResultFromScode(E_OUTOFMEMORY));
} /* End of MakeSureTheresMemory() */


/************************************************************************
CCache::DataGet - This gets data from the beginning (oldest part) of
   the cache. It then removes that memory from the cache.

inputs
   PVOID    pMem - Memory to fill in
   DWORD    dwSize - Number of bytes to fill in.
   DWORD    *pdwCopied - Number of bytes actually copied.
returns
   HRESULT - Error value
*/

HRESULT CCache::DataGet(PVOID pMem, DWORD dwSize, DWORD *pdwCopied)
{
   HRESULT hRes;
   DWORD   dwTemp;

   if (hRes = this->MakeSureTheresMemory()) {
      *pdwCopied = 0;
      return (hRes);
   }

   if (dwSize > m_dwData) 
      dwSize = m_dwData;

   // since this is a circular buffer, copy the first half
   dwTemp = min(m_dwCacheSize - m_dwStartOffset, dwSize);
   memcpy (pMem, m_pData + m_dwStartOffset, dwTemp);
   pMem = (void*) ((BYTE*)pMem + dwTemp);

   // copy the second half if there was any
   if ((dwSize + m_dwStartOffset) >= m_dwCacheSize) {
      dwTemp = dwSize + m_dwStartOffset - m_dwCacheSize;
      memcpy(pMem, m_pData, dwTemp);
   }

   // update the values
   m_dwStartOffset = (m_dwStartOffset + dwSize) % m_dwCacheSize;
   m_dwData -= dwSize;
   *pdwCopied = dwSize;

   // done
   return (NOERROR);
} /* End of DataGet() */


/************************************************************************
CCache::DataAdd - This adds data to the end (newest part) of
   the cache.

inputs
   PVOID    pMem - Memory to copy over
   DWORD    dwSize - Number of bytes to copy over
   DWORD    *pdwCopied - Number of bytes actually copied.
returns
   HRESULT - Error value
*/
HRESULT CCache::DataAdd(PVOID pMem, DWORD dwSize, DWORD *pdwCopied)
{
   HRESULT  hRes;
   DWORD    dwTemp, dwCopyTo;

   if (hRes = this->MakeSureTheresMemory()) {
      *pdwCopied = 0;
      return (hRes);
   }

   if (dwSize > (m_dwCacheSize - m_dwData))
      dwSize = m_dwCacheSize - m_dwData;

   // since this is a circular buffer, copy the first half
   dwCopyTo = (m_dwStartOffset + m_dwData) % m_dwCacheSize;
   dwTemp   = ((dwSize + dwCopyTo) >= m_dwCacheSize) ? m_dwCacheSize - dwCopyTo
                                                     : dwSize;
   memcpy (m_pData + dwCopyTo, pMem, dwTemp);
   pMem = (void*) ((BYTE*)pMem + dwTemp);

   // copy the second half if there was any
   if ((dwSize + dwCopyTo) >= m_dwCacheSize) {
      dwTemp = dwCopyTo + dwSize - m_dwCacheSize;
      memcpy(m_pData, pMem, dwTemp);
   }

   // update the values
   m_dwData  += dwSize;
   *pdwCopied = dwSize;

   // done
   return (NOERROR);
} /* End of DataAdd() */


/************************************************************************
CCache::DataFlush - This flushes out any data in the queue.

inputs
   void
returns
   HRESULT - Error value
*/
HRESULT CCache::DataFlush(void)
{
   m_dwData = 0;
   return (NOERROR);
} /* End of DataFlush() */


/************************************************************************
CCache::Space - This returns the amount of free space in the queue.

inputs
   DWORD * pdwSpace - Filled in with the amount of free space.
returns
   HRESULT - Error value
*/
HRESULT CCache::Space(DWORD *pdwSpace)
{
   *pdwSpace = m_dwCacheSize - m_dwData;
   return (NOERROR);
} /* End of Space() */

/************************************************************************
CCache::Used - This returns the amount of used space in the queue.

inputs
   DWORD * pdwSpace - Filled in with the amount of used space.
returns
   HRESULT - Error value
*/
HRESULT CCache::Used(DWORD * pdwSpace)
{
   *pdwSpace = m_dwData;
   return (NOERROR);
} /* End of Used() */




/************************************************************************
CAIn - Audio Source class.
*/

CAIn::CAIn(LPUNKNOWN punkOuter, LPFNDESTROYED pfnDestroy)
{
   m_cRef             = 0;
   m_punkOuter        = punkOuter;
   m_pfnDestroy       = pfnDestroy;
   m_pAInIAudioSource = NULL;
   m_pAInIAudio       = NULL;
   m_pAInIAudioMultiMediaDevice = NULL;
   m_pIASNS           = NULL;
   m_pCache           = NULL;
   m_pWFEX            = NULL;
   m_dwWFEXSize       = NULL;
   m_qwTotal          = 0;
   m_dwWaveDev        = WAVE_MAPPER;
   AInDeviceIDToName(m_dwWaveDev, m_szDeviceName);
   m_bRecording       = 0;
   m_hWaveIn          = NULL;
   m_dnDevNode        = 0;
   m_hMixer           = NULL;
   m_hWndMonitor      = NULL;
   m_dwBufSize        = 0;
   m_fBufsOut         = 0;
   m_fLowPriority     = FALSE;
   m_uTimer           = 0;
   m_fFlushing        = FALSE;
   m_nResumeBufID     = -1;
   m_fRemoved         = FALSE;
   m_fClaimed         = FALSE;
   m_fAutoClaimed     = FALSE;
   m_fIgnoreDataUntilNextClose = FALSE;

   memset(m_ahBufGlobal, 0, sizeof(m_ahBufGlobal));
   memset(m_apBufWH, 0, sizeof(m_apBufWH));

   // Record the time of the object's inception for use in ToFileTime
   {
   SYSTEMTIME st;

   m_msStartTime = GetCurrentTime();
   GetSystemTime(&st);
   SystemTimeToFileTime(&st, &m_ftStartTime);
   }

   // Start with last buffer at this time
   m_qwLastBufTime = 0;
   m_msLastBufTime = m_msStartTime;

}


CAIn::~CAIn (void)
{
   int i;

   m_pIASNS = NULL;

   // Must close mixer before its wave device.
   if (m_hMixer) {
      mixerClose((HMIXER) m_hMixer);
      m_hMixer = NULL;
   }

   // Close the waveaudio device
   CloseIt();

   // release all the buffers
   for (i = 0; i < AIN_NUMBUF; i++) {
      if (m_ahBufGlobal[i]) {
         GlobalUnlock(m_ahBufGlobal[i]);
         GlobalFree(m_ahBufGlobal[i]);
      }
   }

   // Free the contained interfaces
   if (m_pAInIAudioSource)
      delete m_pAInIAudioSource;
   if (m_pAInIAudio)
      delete m_pAInIAudio;
   if (m_pAInIAudioMultiMediaDevice)
      delete m_pAInIAudioMultiMediaDevice;

   if (m_pCache) 
      delete m_pCache;
   if (m_pWFEX) 
      free(m_pWFEX);
   if (m_uTimer) 
      KillTimer(m_hWndMonitor, m_uTimer);
   if (m_hWndMonitor) 
      DestroyWindow(m_hWndMonitor);
}


BOOL CAIn::FInit(DWORD dwWaveDev, LPWAVEFORMATEX lpWFEX, DWORD dwWFEXSize)
{
   LPUNKNOWN   pIUnknown = (LPUNKNOWN) this;
   WNDCLASS    wc;

   if (NULL != m_punkOuter) 
      pIUnknown = m_punkOuter;

   // Allocate all of the contained interfaces

   m_pAInIAudioSource = new CAInIAudioSource(this, pIUnknown);
   m_pAInIAudio = new CAInIAudio(this, pIUnknown);
   m_pAInIAudioMultiMediaDevice = new CAInIAudioMultiMediaDevice(this, pIUnknown);

   // Create the monitor window

   memset(&wc, 0, sizeof(wc));
   wc.lpfnWndProc = AInWindowProc;
   wc.hInstance = ghInstance;
   wc.lpszClassName = "SRAudioInClass";
   wc.cbWndExtra = sizeof(long);
   RegisterClass(&wc);
   m_hWndMonitor = CreateWindow(wc.lpszClassName, "", 0, CW_USEDEFAULT, 
                                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                NULL, NULL, ghInstance, (VOID*) this);

   // Store the wave device and format away
   m_dwWaveDev = dwWaveDev;


   return (m_pAInIAudioSource &&
           m_pAInIAudio &&
           m_pAInIAudioMultiMediaDevice &&
           m_hWndMonitor);
} /* End of FInit() */



STDMETHODIMP CAIn::QueryInterface(REFIID riid, LPVOID *ppv)
{
   *ppv = NULL;

   /* always return our IUnknown for IID_IUnknown */
   if (IsEqualIID (riid, IID_IUnknown))
      *ppv = (LPVOID) this;
   else if (IsEqualIID(riid, IID_IAudioSource))
      *ppv = m_pAInIAudioSource;
   else if (IsEqualIID(riid, IID_IAudio))
      *ppv = m_pAInIAudio;
   else if (IsEqualIID(riid, IID_IAudioMultiMediaDevice))
      *ppv = m_pAInIAudioMultiMediaDevice;

   // update the reference count
   if (NULL != *ppv) {
      ((LPUNKNOWN) *ppv)->AddRef();
      return (NOERROR);
   }
   return (ResultFromScode(E_NOINTERFACE));
} /* End of QueryInterface() */

STDMETHODIMP_(ULONG) CAIn::AddRef(void)
{
   return (++m_cRef);
} /* End of AddRef() */

STDMETHODIMP_(ULONG) CAIn::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0 == m_cRef) {
      if (NULL != m_pfnDestroy) 
         (*m_pfnDestroy)();
      delete this;
   }
   return (cRefT);
} /* End of Release() */




/* 
 *  This trys to open and prepare the audio input source.  If it can't then
 *  it returns an error.  It does NOT continue to retry indefinately.
 */
STDMETHODIMP CAIn::OpenAndPrepare(void)
{
   MMRESULT    mm;
   DWORD       i;
   LPWAVEHDR   pWH;
   UINT        nMix = mixerGetNumDevs();
   UINT        nIn  = waveInGetNumDevs();

   m_dwWaveDev = AInNameToDeviceID (m_dwWaveDev, m_szDeviceName);
   mm = (m_dwWaveDev == WAVE_MAPPER)

      ? SRWaveMapper(&m_hWaveIn, (PUINT) &m_dwWaveDev, m_pWFEX,
                     (DWORD) m_hWndMonitor, NULL, CALLBACK_WINDOW)

      : waveInOpen  (&m_hWaveIn, m_dwWaveDev, m_pWFEX,
                     (DWORD) m_hWndMonitor, NULL, CALLBACK_WINDOW);
   AInDeviceIDToName (m_dwWaveDev, m_szDeviceName);

   if (mm) {
      m_hWaveIn = NULL;
      switch (mm) {
         case MMSYSERR_BADDEVICEID: return ResultFromScode(AUDERR_BADDEVICEID);
         case MMSYSERR_ALLOCATED  : return ResultFromScode(AUDERR_WAVEDEVICEBUSY);
         case MMSYSERR_NOMEM      : return ResultFromScode(E_OUTOFMEMORY);
         case MMSYSERR_NODRIVER   : return ResultFromScode(AUDERR_NODRIVER);
         case MMSYSERR_NOTENABLED : return ResultFromScode(AUDERR_WAVENOTENABLED);
         default                  : return ResultFromScode(AUDERR_WAVEFORMATNOTSUPPORTED);
      }
   }

   // Here we determine the devnode of the audio device we just opened.  This
   // is saved so that we can determine if the audio device is removed while
   // we are using it (plug-n-play).  This message require the user to pass
   // in the device ID of the device not a valid handle.
   waveInMessage((HWAVEIN) m_dwWaveDev, DRV_QUERYDEVNODE, (LPARAM) &m_dnDevNode, 0); 

   // Set this into low priority mode if we can, we don't care if it succeeds or not
   if (m_fLowPriority)
      waveInMessage(m_hWaveIn, WIDM_LOWPRIORITY, 0, 0);

   // Prepare all of the headers
   m_fBufsOut = 0;

   // Start the initial buffers here, then after the waveInStart, send down the
   // remaining buffers

   for (i = 0; i < AIN_NUMBUF; i++) {
      pWH = m_apBufWH[i];
      memset(pWH, 0, sizeof(WAVEHDR));
      pWH->lpData = (char *) (pWH + 1);
      pWH->dwBufferLength = m_dwBufSize;
      pWH->dwUser = i;
      mm = waveInPrepareHeader(m_hWaveIn, pWH, sizeof(WAVEHDR));

      if (mm) {
         if (waveInClose(m_hWaveIn) == 0)
            m_hWaveIn = NULL;
         switch (mm) {
            case MMSYSERR_INVALHANDLE: return ResultFromScode(AUDERR_INVALIDHANDLE);
            case MMSYSERR_NOMEM:       return ResultFromScode(E_OUTOFMEMORY);
            case MMSYSERR_HANDLEBUSY:  return ResultFromScode(AUDERR_HANDLEBUSY);
            default:                   return ResultFromScode(E_FAIL);
         }
      }
   }

   return (MMSYSERR_NOERROR);
} /* End of OpenAndPrepare() */


/*
 *  This will to start the audio input source.
 */
STDMETHODIMP CAIn::TryToStart(void)
{
   HRESULT hRes;

   // HACKHACK:  If the claim count is zero then claim the device 
   //            on behalf of the user.
   if (!m_fClaimed) {
      if (FAILED(hRes = m_pAInIAudio->Claim()))
         return (hRes);
      m_fAutoClaimed = TRUE;
   }

   // At this point, a call to Claim() has succeeded (either the
   // application called Claim() followed by Start() or we called
   // Claim() on behalf of an app in response to a Start() call.
   // Since Claim() has succeeded, we have a valid waveformat, 
   // the audio device is open, and all the buffers have been 
   // prepared.  Thus this will be our last pass through TryToStart().

   // Give all of the buffers to the waveaudio device
   while (this->TryToSendBufOut())
      /* This loop body intentionally left blank */ ;

   // Start the waveaudio device recording
   waveInStart(m_hWaveIn);

   // Flag that we are recording
   m_bRecording = TRUE;

   // Tell the caller about our success
   return (MMSYSERR_NOERROR);
} /* End of TryToStart() */


/*
 *  Try to send a buffer out to be recorded.
 *  Return TRUE if successful.
 */
BOOL CAIn::TryToSendBufOut(void)
{
   DWORD    i, fFlag;

   // find a free one
   for (i = 0; i < AIN_NUMBUF; i++) {
      fFlag = 1L << i;
      if (!(fFlag & m_fBufsOut)) 
         break;
   }
   if (i >= AIN_NUMBUF) 
      return (FALSE);

   m_apBufWH[i]->dwFlags &= ~WHDR_DONE;
   m_apBufWH[i]->dwBytesRecorded = 0;

   // send it out
   if (waveInAddBuffer(m_hWaveIn, m_apBufWH[i], sizeof(WAVEHDR)))
      return (FALSE);

   // make note that it's sent out
   m_fBufsOut |= fFlag;

   return (TRUE);
} /* End of TryToSendBufOut() */


/* This closes the wave device */
void CAIn::CloseIt(void)
{
   DWORD i;

   if (m_hWaveIn) {
      // We have an open waveaudio device
      // First reset the audio device to keep the driver from puking
      waveInReset(m_hWaveIn);

      // Now unprepare all of the wave headers.
      for (i = 0; i < AIN_NUMBUF; i++)
         if (m_ahBufGlobal[i])
            waveInUnprepareHeader(m_hWaveIn, m_apBufWH[i], sizeof(WAVEHDR));

      // Flag that all the buffers have been returned
      m_fBufsOut = 0;

      // The close the device
      m_fIgnoreDataUntilNextClose = TRUE;
      if (!waveInClose(m_hWaveIn))
         m_hWaveIn = NULL;

      // Turn off any flushing
      m_fFlushing = FALSE;
   }
} /* End of CloseIt() */


/* This deals with a returned buffer returned by MM_WIM_DATA */

void CAIn::ReturnedBuffer(LPWAVEHDR pWH)
{
   DWORD dwDataLost = 0;      // Number of bytes of audio data lost
   DWORD dwRec;               // Number of bytes written to the cache
   DWORD dwUsed;              // Number of bytes of audio data in the cache

   // Make sure the data is block aligned
   if (pWH->dwBytesRecorded % m_pWFEX->nBlockAlign)
      pWH->dwBytesRecorded -= (pWH->dwBytesRecorded % m_pWFEX->nBlockAlign);

   if (m_fFlushing) {
      // Here if the user has called flush and we haven't seen all
      // the buffers from the driver yet.  If this is the first
      // buffer returned since the call to flush, then we save it's
      // ID, ignore it's contents, and requeue it.  We will ignore
      // all subsequent buffers until we see this buffer again.  This
      // ensures that any data the driver was holding at the time of
      // the flush will be discarded.
      if (m_nResumeBufID == -1)
         m_nResumeBufID = pWH->dwUser;
      else if ((DWORD) m_nResumeBufID == pWH->dwUser)
         m_fFlushing = FALSE;
   } 

   // If we're recording (and not flushing) than put the data into the cache
   if (m_bRecording && !m_fFlushing) {
      // Pack in as much data as will fit
      m_pCache->DataAdd(pWH->lpData, pWH->dwBytesRecorded, &dwRec);

      // Check for data loss.  We set the variable dwDataLost to the
      // count of audio data bytes lost and check the value just before
      // exiting.  If it's non-zero then we call the notification sink
      // directly since lazy notifications of data loss don't allow
      // applications to always respond/recover correctly.  This is
      // deferred until function exit time to avoid CAIn object reentrency
      // problems.
      if (pWH->dwBytesRecorded > dwRec)
         dwDataLost = pWH->dwBytesRecorded - dwRec;

      // Tell the app that new data has arrived
      PostMessage(m_hWndMonitor, WM_SPACECHANGE, 0, 0);
   }      

   // Keep a record of what (real-time) time it is, and what QWORD we just got
   // returned, since this was just recorded
   m_pCache->Used(&dwUsed);
   m_qwLastBufTime = m_qwTotal + dwUsed;
   m_msLastBufTime = GetCurrentTime();

   // Turn off bit representing this buffer
   m_fBufsOut &= (~(1 << pWH->dwUser));
   
   // If we are still recording then try and requeue the buffer
   if (m_bRecording) {
      pWH->dwBytesRecorded = 0;     // Reset the count of bytes recorded
      pWH->dwFlags &= ~WHDR_DONE;   // Clear the done flag
      if (waveInAddBuffer(m_hWaveIn, pWH, sizeof(WAVEHDR)) == MMSYSERR_NOERROR)
         m_fBufsOut |= (1 << pWH->dwUser);
   }

   // Check for no buffers queued for recording, either we arn't
   // recording or all of the waveInAddBuffer() calls have failed.
   if (!m_fBufsOut && !m_fClaimed)
      CloseIt();

   // Finally, if we lost any data then call the notification sink
   // right now since this information isn't very useful if it's 
   // moldy.
   if (dwDataLost)
      this->NotifyOverflow(dwDataLost);
} /* End of ReturnedBuffer() */


/* This sends a change-of-notify message to the user-code.
   If no user code is specified then nothing is sent.  */

void CAIn::Notify(void)
{
   DWORD dwRes;

   if (m_pIASNS) {
      m_pCache->Used(&dwRes);
      // Only send a DataAvailable notification if there is
      // some data available.
      if (dwRes) {
         m_pIASNS->DataAvailable(dwRes, FALSE);
      }
   }
} /* End of Notify() */


void CAIn::NotifyAudioStart(void)
{
   if (m_pIASNS) {
      m_pIASNS->AudioStart();
   }
} /* End of NotifyAudioStart() */

void CAIn::NotifyAudioStop(WORD wReason)
{
   if (m_pIASNS) {
      m_pIASNS->AudioStop(wReason);
   }
} /* End of NotifyAudioStop() */

void CAIn::NotifyOverflow(DWORD dwSize)
{
   if (m_pIASNS) {
      m_pIASNS->Overflow(dwSize);
   }
} /* End of NotifyAudioOverFlow() */


/************************************************************************
CAInIAudioMultiMediaDevice - AInIAudioMultiMediaDevice interface for SR Manager
*/

CAInIAudioMultiMediaDevice::CAInIAudioMultiMediaDevice(LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef = 0;
   m_pObj = pObj;
   m_punkOuter = punkOuter;
}


CAInIAudioMultiMediaDevice::~CAInIAudioMultiMediaDevice(void)
{
   // This space intentionally left blank
}


STDMETHODIMP CAInIAudioMultiMediaDevice::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return (m_punkOuter->QueryInterface(riid,ppv));
}


STDMETHODIMP_(ULONG) CAInIAudioMultiMediaDevice::AddRef(void)
{
   ++m_cRef;
   return (m_punkOuter->AddRef());
}


STDMETHODIMP_(ULONG) CAInIAudioMultiMediaDevice::Release(void)
{
   --m_cRef;
   return (m_punkOuter->Release());
}


STDMETHODIMP CAInIAudioMultiMediaDevice::CustomMessage(UINT uMessage, SDATA sData)
{
   PCAIn pCAIn = (PCAIn) m_pObj;


   if (!pCAIn->m_pWFEX)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   if (pCAIn->m_hWaveIn != NULL) {
      if (sData.dwSize)
         waveInMessage(pCAIn->m_hWaveIn, uMessage, (DWORD) sData.dwSize, (DWORD) sData.pData);
      else
         waveInMessage(pCAIn->m_hWaveIn, uMessage, 0, 0);
   }

   return (NOERROR);
} /* End of CustomMessage() */


STDMETHODIMP CAInIAudioMultiMediaDevice::DeviceNumGet(DWORD *pdwDevice)
{
   PCAIn pCAIn = (PCAIn) m_pObj;


   if (!pdwDevice)
      return (ResultFromScode(AUDERR_INVALIDPARAM));
   *pdwDevice = AInNameToDeviceID(pCAIn->m_dwWaveDev, pCAIn->m_szDeviceName);
   return (NOERROR);
} /* End of DeviceNumGet() */


STDMETHODIMP 
CAInIAudioMultiMediaDevice::DeviceNumSet(DWORD dwDevice)
{
   PCAIn pCAIn = (PCAIn) m_pObj;


   // First check and see if we have a non-NULL wave handle.  If so we
   // assume that its valid and that we are currently recording audio. Thus
   // we return an error code.
   if (pCAIn->m_hWaveIn)
      return (ResultFromScode(AUDERR_WAVEDEVICEBUSY));

   // Check and see if the device ID specified is valid for the
   // current system comfiguration.  If its not then we return an error.
   if (dwDevice >= waveInGetNumDevs() && dwDevice != WAVE_MAPPER)
      return (ResultFromScode(AUDERR_BADDEVICEID));

   // Check and see if the user has already specified the waveformat for
   // this object instance.  If so we must validate that the chosen device
   // supports the configured waveformat.
   if (pCAIn->m_pWFEX) {
      MMRESULT mm;

      // If the caller specified wavemapper as the device ID to set to then we
      // will look for an available device and save it's device ID for later use.
      // In no case will we ever save the special value of WAVE_MAPPER into 
      // m_dwWaveDev since this could cause problems later on (i.e. we succeed here
      // but fail later because the device we found here is now in use.
      if (pCAIn->m_dwWaveDev == WAVE_MAPPER)
         mm = SRWaveMapper(NULL, (PUINT) &dwDevice, pCAIn->m_pWFEX,
                           NULL, NULL, WAVE_FORMAT_QUERY | CALLBACK_NULL);
      else 
         mm = waveInOpen(NULL, (UINT) dwDevice, pCAIn->m_pWFEX,
                         NULL, NULL, WAVE_FORMAT_QUERY | CALLBACK_NULL);

      // If we failed to open the requested device, then tell the user that the
      // configured waveformat is not supported by the device.
      if (mm)
         return (ResultFromScode(AUDERR_WAVEFORMATNOTSUPPORTED));
   }

   // Save both the device ID and the name
   pCAIn->m_dwWaveDev = dwDevice;
   AInDeviceIDToName(pCAIn->m_dwWaveDev, pCAIn->m_szDeviceName);

   return (NOERROR);
} /* End of DeviceNumSet() */


/************************************************************************
CAInIAudioSource - AInIAudioSource interface for SR Manager
*/

CAInIAudioSource::CAInIAudioSource (LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef = 0;
   m_pObj = pObj;
   m_punkOuter = punkOuter;
}


CAInIAudioSource::~CAInIAudioSource (void)
{
   // This space intentionally left blank
}


STDMETHODIMP CAInIAudioSource::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return (m_punkOuter->QueryInterface(riid, ppv));
} /* End of QueryInterface() */


STDMETHODIMP_ (ULONG) CAInIAudioSource::AddRef(void)
{
   ++m_cRef;
   return (m_punkOuter->AddRef());
} /* End of AddRef() */


STDMETHODIMP_(ULONG) CAInIAudioSource::Release(void)
{
   --m_cRef;
   return (m_punkOuter->Release());
} /* End of Release() */


STDMETHODIMP CAInIAudioSource::DataAvailable(DWORD *pdwBytes, BOOL *pfEOF)
{
   PCAIn pCAIn = (PCAIn) m_pObj;


   if (!pCAIn->m_pWFEX)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   if (!pdwBytes) 
      return (ResultFromScode(AUDERR_INVALIDPARAM));

   // check pointer for non-NULL value before setting
   if (pfEOF)
      *pfEOF = FALSE;

   // So that returns the proper # of bytes left - what's wrong with:
   return (pCAIn->m_pCache->Used(pdwBytes));
} /* End of DataAvailable() */


STDMETHODIMP CAInIAudioSource::DataGet(PVOID pBuffer, DWORD dwGetSize, DWORD *pdwCopied)
{
   PCAIn    pCAIn = (PCAIn) m_pObj;
   PQWORD   pQW = &pCAIn->m_qwTotal;
   HRESULT  hRes;

   if (!pCAIn->m_pWFEX)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   if (!pdwCopied || !pBuffer) 
      return (ResultFromScode(AUDERR_INVALIDPARAM));

   // get it from the cache
   hRes = pCAIn->m_pCache->DataGet(pBuffer, dwGetSize, pdwCopied);

   // increase the total bytes recorded, doing carry
   *pQW = *pQW + *pdwCopied;

   return (hRes);
} /* End of DataGet() */


/************************************************************************
CAInIAudio - AInIAudio interface for SR Manager
*/

CAInIAudio::CAInIAudio(LPVOID pObj, LPUNKNOWN punkOuter)
{
   m_cRef = 0;
   m_pObj = pObj;
   m_punkOuter = punkOuter;
}


CAInIAudio::~CAInIAudio(void)
{
   PCAIn pCAIn = (PCAIn) m_pObj;

   if (pCAIn->m_pIASNS) {
      pCAIn->m_pIASNS->Release();
      pCAIn->m_pIASNS = NULL;
   }
}


STDMETHODIMP CAInIAudio::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
   return (m_punkOuter->QueryInterface(riid, ppv));
} /* End of QueryInterface() */


STDMETHODIMP_(ULONG) CAInIAudio::AddRef(void)
{
   ++m_cRef;
   return (m_punkOuter->AddRef());
} /* End of AddRef() */


STDMETHODIMP_(ULONG) CAInIAudio::Release(void)
{
   --m_cRef;
   return (m_punkOuter->Release());
} /* End of Release() */

STDMETHODIMP CAInIAudio::Flush(void)
{
   PCAIn    pCAIn = (PCAIn) m_pObj;
   MMRESULT mm;

   if (!pCAIn->m_pWFEX)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   mm = MMSYSERR_NOERROR;
   if (pCAIn->m_bRecording && pCAIn->m_hWaveIn) {
      pCAIn->m_fFlushing = TRUE;
      pCAIn->m_nResumeBufID = -1;
      mm = waveInReset(pCAIn->m_hWaveIn);
   }

   switch (mm) {
      case MMSYSERR_NOERROR:        break;
      case MMSYSERR_INVALHANDLE:    return AUDERR_INVALIDHANDLE;
      case MMSYSERR_HANDLEBUSY:     return AUDERR_HANDLEBUSY;
      default:                      return E_FAIL;
   }

   // flush the queue
   pCAIn->m_pCache->DataFlush();

   // Restart the audio device recording
   if (pCAIn->m_bRecording && pCAIn->m_hWaveIn) {
      mm = waveInStart(pCAIn->m_hWaveIn);

      switch (mm) {
         case MMSYSERR_NOERROR:        break;
         case MMSYSERR_INVALHANDLE:    return AUDERR_INVALIDHANDLE;
         case MMSYSERR_HANDLEBUSY:     return AUDERR_HANDLEBUSY;
         default:                      return E_FAIL;
      }
   }

   return (NOERROR);
} /* End of Flush() */


STDMETHODIMP CAInIAudio::LevelGet(DWORD *pdwLevel)
{
   PCAIn pCAIn = (PCAIn) m_pObj;
   MMRESULT                      mm;
   MIXERCONTROLDETAILS           MixerControlDetails;
   MIXERCONTROLDETAILS_UNSIGNED  Level;

   if (pdwLevel == NULL)
      return (AUDERR_INVALIDPARAM);

   // initiailize
   *pdwLevel = 0;

   if (!pCAIn->m_pWFEX)
      return (AUDERR_NEEDWAVEFORMAT);

   if (pCAIn->m_hMixer == NULL) 
      return (AUDERR_NOTSUPPORTED);

   memset(&MixerControlDetails, 0, sizeof(MIXERCONTROLDETAILS));
   MixerControlDetails.cbStruct    = sizeof(MIXERCONTROLDETAILS);
   MixerControlDetails.dwControlID = pCAIn->m_dwVolCtrlID;
   MixerControlDetails.cChannels   = pCAIn->m_pWFEX->nChannels;
   MixerControlDetails.cbDetails   = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
   MixerControlDetails.paDetails   = &Level;

   // use opened mixer
   mm = mixerGetControlDetails((HMIXEROBJ) pCAIn->m_hMixer, &MixerControlDetails,
                               MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE);
   switch (mm) {
      case MMSYSERR_NOERROR:
         break;

      case MMSYSERR_BADDEVICEID:
         return (AUDERR_BADDEVICEID);

      case MMSYSERR_INVALHANDLE:
         return (AUDERR_INVALIDHANDLE);

      case MMSYSERR_INVALFLAG:
      case MMSYSERR_INVALPARAM:
      case MIXERR_INVALCONTROL:
         return (E_FAIL);

      case MMSYSERR_NODRIVER:
         return (AUDERR_NODRIVER);

      default:
         return (AUDERR_WAVEFORMATNOTSUPPORTED);
   }

   *pdwLevel = Level.dwValue;

   return(NOERROR);
} /* End of CAInIAudio::LevelGet() */


STDMETHODIMP CAInIAudio::LevelSet(DWORD dwLevel)
{
   PCAIn pCAIn = (PCAIn) m_pObj;
   MMRESULT             mm;
   MIXERCONTROLDETAILS  MixerControlDetails;
   MIXERCONTROLDETAILS_UNSIGNED  Level;

   if (!pCAIn->m_pWFEX)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   if (pCAIn->m_hMixer == NULL) 
      return (ResultFromScode(AUDERR_NOTSUPPORTED));

   // Hack, only use the left channel (mono)
   dwLevel = LOWORD(dwLevel);
   if (dwLevel < AV_MINVOL) dwLevel = AV_MINVOL;
   if (dwLevel > AV_MAXVOL) dwLevel = AV_MAXVOL;

   Level.dwValue = dwLevel;
   memset(&MixerControlDetails, 0, sizeof(MIXERCONTROLDETAILS));
   MixerControlDetails.cbStruct    = sizeof(MIXERCONTROLDETAILS);
   MixerControlDetails.dwControlID = pCAIn->m_dwVolCtrlID;
   MixerControlDetails.cChannels   = pCAIn->m_pWFEX->nChannels;
   MixerControlDetails.cbDetails   = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
   MixerControlDetails.paDetails   = &Level;

   for (int i = 0; i < 2; i++) {
      if (i) {
         if (!(pCAIn->m_dwMasterVolCtrlID))
            break;
         else
            MixerControlDetails.dwControlID = pCAIn->m_dwMasterVolCtrlID;
      }

      mm = mixerSetControlDetails((HMIXEROBJ) pCAIn->m_hMixer, &MixerControlDetails,
                                  MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE);
      switch (mm) {
         case MMSYSERR_NOERROR:
            break;

         case MMSYSERR_BADDEVICEID: 
            return (ResultFromScode(AUDERR_BADDEVICEID));

         case MMSYSERR_INVALHANDLE: 
            return (ResultFromScode(AUDERR_INVALIDHANDLE));

         case MMSYSERR_INVALFLAG: 
            return (ResultFromScode(AUDERR_INVALIDFLAG));

         case MMSYSERR_INVALPARAM: 
            return (ResultFromScode(AUDERR_INVALIDPARAM));

         case MMSYSERR_NODRIVER: 
            return (ResultFromScode(AUDERR_NODRIVER));

         case MIXERR_INVALCONTROL: 
            return (ResultFromScode(AUDERR_INVALIDPARAM));

         default: 
            return (ResultFromScode(AUDERR_WAVEFORMATNOTSUPPORTED));
      }
   }

   return(NOERROR);
} /* End of LevelSet() */


STDMETHODIMP CAInIAudio::PassNotify(PVOID pIADNS, IID iid)
{
   PCAIn pCAIn = (PCAIn) m_pObj;

#ifdef _DEBUG
#define GUIDSTRBUFSIZE  64
   OLECHAR IIDGUIDStrW[GUIDSTRBUFSIZE];
   CHAR IIDGUIDStrA[GUIDSTRBUFSIZE];
 
   StringFromGUID2(iid, IIDGUIDStrW, GUIDSTRBUFSIZE);
   WideCharToMultiByte(CP_ACP, 0, IIDGUIDStrW, -1, IIDGUIDStrA, GUIDSTRBUFSIZE, NULL, NULL);
#endif /* _DEBUG */
   // If the IID is not for an audio-source notify sink then error
   if (pIADNS != NULL) {
      if (!IsEqualIID(iid, IID_IAudioSourceNotifySink))
         return (AUDERR_INVALIDNOTIFYSINK);
      ((PIAUDIOSOURCENOTIFYSINK) pIADNS)->AddRef();
   }

   if (pCAIn->m_pIASNS)
      pCAIn->m_pIASNS->Release();
   pCAIn->m_pIASNS = (PIAUDIOSOURCENOTIFYSINK) pIADNS;

   return (NOERROR);
} /* End of PassNotify() */


STDMETHODIMP CAInIAudio::PosnGet(PQWORD pqWord)
{
   PCAIn pCAIn = (PCAIn) m_pObj;
   DWORD dwUsed;

   if (pqWord == NULL)
      return (AUDERR_INVALIDPARAM);

   if (!pCAIn->m_pWFEX)
      return (ResultFromScode (AUDERR_NEEDWAVEFORMAT));

   // If we use m_qwTotal, it's accurate to within 1/16 of a second
   pCAIn->m_pCache->Used(&dwUsed);

   *pqWord = pCAIn->m_qwTotal + dwUsed;

   return (NOERROR);
} /* End of PosnGet() */

//              Implement Claim/UnClaim, and remove PowerDown/PowerUp.
//              Claim/UnClaim will occupy the position of PowerDown/PowerUp
//              so there is no problem with backward compatability and
//              vtables.
//
STDMETHODIMP CAInIAudio::Claim(void)
{
   PCAIn pCAIn = (PCAIn) m_pObj;
   HRESULT  hRes;

   // First, verify that the user has configured a valid
   // waveformat for the target device.
   if (!pCAIn->m_pWFEX)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   // If the device has already been claimed let the app know
   if (pCAIn->m_fClaimed)
      return (ResultFromScode(AUDERR_ALREADYCLAIMED));

   // Otherwise, open the device and prepare all the headers
   if ((hRes = pCAIn->OpenAndPrepare()) == MMSYSERR_NOERROR) {
      // Set the claim flag
      pCAIn->m_fClaimed = TRUE;

      // Reset the auto-claimed flag
      pCAIn->m_fAutoClaimed = FALSE;
   }

   return (hRes);
} /* End of Claim() */

STDMETHODIMP CAInIAudio::UnClaim(void)
{
   PCAIn pCAIn = (PCAIn) m_pObj;

   // Validate that the device has been initialized
   if (!pCAIn->m_pWFEX)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   // Must have claimed the device to unclaim
   if (!pCAIn->m_fClaimed)
      return (ResultFromScode(AUDERR_NOTCLAIMED));
   
   // Reset the claim flag
   pCAIn->m_fClaimed = FALSE;

   // close the device 
   pCAIn->CloseIt();

   return (NOERROR);
} /* End of UnClaim() */



STDMETHODIMP CAInIAudio::Start(void)
{
   PCAIn pCAIn = (PCAIn) m_pObj;
   HRESULT hRes;

   // Tell user if the device is already recording
   if (pCAIn->m_bRecording)
      return (ResultFromScode(AUDERR_ALREADYSTARTED));

   // Try and start recording, this will claim the audio device 
   // if the application hasn't already called Claim().
   hRes = pCAIn->TryToStart();

   // If the waveaudio device is in use by some other application
   // then we will wait until it frees up.
   if (hRes == AUDERR_WAVEDEVICEBUSY) {
      pCAIn->m_bRecording = TRUE;
      pCAIn->m_uTimer = SetTimer(pCAIn->m_hWndMonitor, 1, 200, NULL);
      return (NOERROR);
   }

   return (hRes);
} /* End of Start() */


STDMETHODIMP CAInIAudio::Stop(void)
{
   PCAIn pCAIn = (PCAIn) m_pObj;

   if (!pCAIn->m_pWFEX)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   if (pCAIn->m_hWaveIn)
      waveInReset(pCAIn->m_hWaveIn);
   pCAIn->m_bRecording = FALSE;

   // Kill the start timer if we're still trying to start
   if (pCAIn->m_uTimer)
      KillTimer(pCAIn->m_hWndMonitor, pCAIn->m_uTimer);

   // Unclaim the device if we claimed it for the application
   if (pCAIn->m_fAutoClaimed)
      UnClaim();

   return (NOERROR);
} /* End of Stop() */



STDMETHODIMP CAInIAudio::ToFileTime(PQWORD pqWord, FILETIME *pFT)
{
   PCAIn       pCAIn = (PCAIn) m_pObj;
   _int64      iByteDelta, iMSDelta;
   DWORD       dwMSTime;
   QWORD       qwFT;

   // error checks
   if (pqWord == NULL || pFT == NULL)
      return (AUDERR_INVALIDPARAM);

   if (!pCAIn->m_pWFEX)
      return ResultFromScode (AUDERR_NEEDWAVEFORMAT);

   // Find the difference between the last buffer that we saw returned (giving
   // us a correlation between qw-Time and millisecon-time. and find the differece.
   iByteDelta = (_int64) *pqWord - (_int64) pCAIn->m_qwLastBufTime;

   // This is a delta (in bytes) from the time. Convert this to milliseconds
   iMSDelta = (iByteDelta * 1000) / pCAIn->m_pWFEX->nAvgBytesPerSec;

   // Add this to the time of the last buffer (in milliseconds) & figure out what time
   // this happened. How many millseconds after the inception of the audio object
   dwMSTime = (DWORD) ((pCAIn->m_msLastBufTime - pCAIn->m_msStartTime) + (long) iMSDelta);

   // Convert this to a FILETIME scale, where 100 ns is 1 unit. 1 ms = 1,000,000 
   // ns => 1 ms = 10,000 units
   qwFT = (QWORD) dwMSTime * 10000;

   // add this to the starting file-time & ship it out
   *((QWORD *)pFT) = qwFT + *((QWORD*) &(pCAIn->m_ftStartTime));

   // done
   return (NOERROR);
} /* End of ToFileTime() */


STDMETHODIMP CAInIAudio::TotalGet(PQWORD pqWord)
{
   PCAIn pCAIn = (PCAIn) m_pObj;

   // Validate parameter
   if (!pqWord) 
      return (ResultFromScode(AUDERR_INVALIDPARAM));

   // Verify that a waveformat has been set
   if (!pCAIn->m_pWFEX)
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   *pqWord = pCAIn->m_qwTotal;
   return (NOERROR);
} /* End of TotalGet() */


STDMETHODIMP CAInIAudio::WaveFormatGet(PSDATA pData)
{
   PCAIn pCAIn = (PCAIn) m_pObj;
   LPMALLOC pMalloc;
   HRESULT hRes;

   // Validate the parameter
   if (!pData) 
      return (ResultFromScode(AUDERR_INVALIDPARAM));

   // Validate that a waveformat has been set
   if (!pCAIn->m_pWFEX) 
      return (ResultFromScode(AUDERR_NEEDWAVEFORMAT));

   if (hRes = CoGetMalloc(MEMCTX_TASK, &pMalloc)) 
      return (ResultFromScode(hRes));
   pData->pData = (PVOID) (pMalloc->Alloc(pCAIn->m_dwWFEXSize));
   pMalloc->Release();
   if (!pData->pData) 
      return (ResultFromScode(E_OUTOFMEMORY));
   pData->dwSize = pCAIn->m_dwWFEXSize;
   memcpy(pData->pData, pCAIn->m_pWFEX, pCAIn->m_dwWFEXSize);
   return (NOERROR);
} /* End of WaveFormatGet() */


STDMETHODIMP CAInIAudio::WaveFormatSet(SDATA pData)
{
   PVOID          pNew;
   PCAIn          pCAIn = (PCAIn) m_pObj;
   DWORD          i;
   MMRESULT       mm;
   HWAVEIN        hWaveInTemp;
   BOOL           fTryLowPriority = TRUE;

   // Validate the argument
   if (!pData.pData) 
      return (AUDERR_INVALIDPARAM);

   // Only allow an application to set the wave format once
   if (pCAIn->m_pWFEX)
      return (AUDERR_WAVEDEVICEBUSY);

   // Test the wave-format to see if it works with the wave device.
   // Should use wave-mapper also
   pCAIn->m_dwWaveDev = AInNameToDeviceID(pCAIn->m_dwWaveDev, pCAIn->m_szDeviceName);
   mm = (pCAIn->m_dwWaveDev == WAVE_MAPPER)
      ? SRWaveMapper(&hWaveInTemp, (UINT*) &pCAIn->m_dwWaveDev,
                     (LPWAVEFORMATEX) pData.pData,
                     NULL, NULL, WAVE_FORMAT_QUERY | CALLBACK_NULL)

      : waveInOpen(&hWaveInTemp, (UINT) pCAIn->m_dwWaveDev,
                   (LPWAVEFORMATEX) pData.pData,
                   NULL, NULL, WAVE_FORMAT_QUERY | CALLBACK_NULL);
   switch (mm) {
      case MMSYSERR_NOERROR: 
         break;

      case MMSYSERR_BADDEVICEID:
         return (AUDERR_BADDEVICEID);

      case MMSYSERR_ALLOCATED:
         return (AUDERR_WAVEDEVICEBUSY);

      case MMSYSERR_NOMEM:
         return (E_OUTOFMEMORY);

      case MMSYSERR_INVALPARAM:
         return (AUDERR_INVALIDPARAM);

      case WAVERR_BADFORMAT:
         return (AUDERR_WAVEFORMATNOTSUPPORTED);

      case MMSYSERR_NODRIVER:
         return (AUDERR_NODRIVER);

      default:
         return (AUDERR_NOTSUPPORTED);
   }

   AInDeviceIDToName(pCAIn->m_dwWaveDev, pCAIn->m_szDeviceName);

   pNew = malloc(pData.dwSize);
   if (!pNew) 
      return (E_OUTOFMEMORY);
   memcpy(pNew, pData.pData, pData.dwSize);

   pCAIn->m_pWFEX = (WAVEFORMATEX *) pNew;
   pCAIn->m_dwWFEXSize = pData.dwSize;

   // Figure out how large the cache has to be
   pCAIn->m_dwBufSize  =  pCAIn->m_pWFEX->nAvgBytesPerSec / AIN_BUFSPERSEC;
   pCAIn->m_dwBufSize -= (pCAIn->m_dwBufSize % 2);   // round off to nearest word
   pCAIn->m_dwBufSize -= (pCAIn->m_dwBufSize % pCAIn->m_pWFEX->nBlockAlign);
   if (!pCAIn->m_dwBufSize)
      pCAIn->m_dwBufSize = pCAIn->m_pWFEX->nBlockAlign;

   // Create the cache
   pCAIn->m_pCache = new CCache (pCAIn->m_dwBufSize * AIN_BUFSPERSEC * AIN_CACHESIZE);

   // allocate all of the buffers
   for (i = 0; i < AIN_NUMBUF; i++) {
      pCAIn->m_ahBufGlobal[i] = GlobalAlloc(GMEM_MOVEABLE, pCAIn->m_dwBufSize + sizeof(WAVEHDR));
      if (!pCAIn->m_ahBufGlobal[i]) {
         free(pNew);
         pCAIn->m_pWFEX = NULL;
         return (E_OUTOFMEMORY);
      }
      pCAIn->m_apBufWH[i] = (LPWAVEHDR) GlobalLock(pCAIn->m_ahBufGlobal[i]);

      if (!pCAIn->m_apBufWH[i]) {
         GlobalFree(pCAIn->m_ahBufGlobal[i]);
         pCAIn->m_ahBufGlobal[i] = NULL;
         free(pNew);
         pCAIn->m_pWFEX = NULL;
         return (E_OUTOFMEMORY);
      }
   }

   if (!pCAIn->m_pCache) {
      free(pNew);
      pCAIn->m_pWFEX = NULL;
      return (E_OUTOFMEMORY);
   }

   // Special cases:
   //    I) There's a bug in the windows NT 3.5 WSS driver. Thus, if we find this
   //       then don't use the low priority
   //
   //   II) The Diamond Telecommander card supports full-duplex so we don't want
   //       to use low priority.
   WAVEINCAPS wc;
   OSVERSIONINFO OSVersionInfo;

   // Get the operating system version information
   ZeroMemory(&OSVersionInfo, sizeof(OSVERSIONINFO));
   OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   if (!GetVersionEx(&OSVersionInfo))
      return (HRESULT_FROM_WIN32(GetLastError()));

   // Get the Audio device's capabilities info
   if (waveInGetDevCaps(pCAIn->m_dwWaveDev, &wc, sizeof(WAVEINCAPS)) != MMSYSERR_NOERROR)
      return (E_FAIL);

   switch (wc.wMid) {
      case MM_MICROSOFT:
         // Low priority doesn't work for the SoundBlaster-16 on Windows-NT 3.5
         // or the Window's Sound System
         if (OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT
         && OSVersionInfo.dwMajorVersion == 3
         && OSVersionInfo.dwMinorVersion == 50) {
            if (wc.wPid == MM_MSFT_WSS_NT_WAVEIN
            || wc.wPid == MM_MSFT_SB16_WAVEIN)
               fTryLowPriority = FALSE;
         }

         // Low priority doesnt work for SB16 in NT 40 either
         if (OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {
            if (wc.wPid == MM_MSFT_SB16_WAVEIN)
               fTryLowPriority = FALSE;
         }
         break;

      case MM_CREATIVE:
         // Low priority doesn't work for the SoundBlaster-16 on Windows-NT 3.5
         // or the Window's Sound System
         if (OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT
         && OSVersionInfo.dwMajorVersion == 3
         && OSVersionInfo.dwMinorVersion == 50) {
            if (wc.wPid == MM_CREATIVE_SBP16_WAVEIN)
               fTryLowPriority = FALSE;
         }
         break;

      case MM_MEDIAVISION:
         // Mediavision drivers don't do low priority correctly.
         // The mixer volume control does not adjust the low
         // priority record
         fTryLowPriority = FALSE;
         break;

      case MM_DIAMOND:
         if (wc.wPid == 0x13)
            fTryLowPriority = FALSE;
         break;

      case MM_TOSHIBA:
         if (wc.wPid == 0x01)             // CS4232 audio driver doesn't work with low priority
            fTryLowPriority = FALSE;
         break;
   } /* End of switch */

   // Read the registry
   HKEY  hKey = 0;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER,
      "Software\\Microsoft\\SpeechAPI", 0,
		KEY_READ, &hKey)) {
		
		// Found it. Get the value
		DWORD dwType, dwData, dwSize;
      dwSize = sizeof(dwData);
		if (ERROR_SUCCESS == RegQueryValueEx (hKey, "UseLowPriority",
         0, &dwType, (LPBYTE) &dwData, &dwSize)) {
            fTryLowPriority = (dwData ? TRUE : FALSE);
      }
		
      RegCloseKey(hKey);
	}

   // only open the mixer when start, close when shut down
   // Find mixer that is connected to input device - setup to talk to our m_hWndMonitor
   mm = mixerOpen(&pCAIn->m_hMixer, (UINT) pCAIn->m_dwWaveDev,
                  (DWORD) pCAIn->m_hWndMonitor, 0, CALLBACK_WINDOW | MIXER_OBJECTF_WAVEIN);

   // It's OK if you don't get a mixer - you just don't get volume control - m_hMixer == NULL
   if (mm) {
      // On certain drivers like 4.0 on Sounblaster16 there's no master volume for
      // recording control in which case the above mixerOpen call fails with MMSYSERR_NODRIVER
      // So we retry below with MIXER_OBJECTF_MIXER instead of MIXER_OBJECTF_WAVEIN

      mm = mixerOpen(&pCAIn->m_hMixer, (UINT) pCAIn->m_dwWaveDev,
                  (DWORD) pCAIn->m_hWndMonitor, 0, CALLBACK_WINDOW | MIXER_OBJECTF_MIXER);
      if (mm)      
         pCAIn->m_hMixer = NULL;
   }


   // Find the volume control

   if (pCAIn->m_hMixer) {
      UINT  waveInID = pCAIn->m_dwWaveDev;

      if (!FindLowPriDriver(pCAIn->m_hMixer, &pCAIn->m_gMixerLine,
                            &pCAIn->m_fLowPriority, fTryLowPriority)
      || avFindVolControl(pCAIn->m_hMixer, &pCAIn->m_dwMasterVolCtrlID, 
                  &pCAIn->m_dwVolCtrlID, pCAIn->m_gMixerLine)) {
         mixerClose(pCAIn->m_hMixer);
         pCAIn->m_hMixer = NULL;
      }
   }

   // if there's only master volume, make the default volume master volume

   if (!(pCAIn->m_dwVolCtrlID) && pCAIn->m_dwMasterVolCtrlID) {
      pCAIn->m_dwVolCtrlID = pCAIn->m_dwMasterVolCtrlID;
      pCAIn->m_dwMasterVolCtrlID = 0;
   }

   return (NOERROR);
} /* End of WaveFormatSet() */


/********************************************************************/

/*
      Finds first waveIn, giving you an ID, which an ordinary
      waveInOpen(..., WAVE_MAPPER, ...) would not.

      Arguments are same as waveInOpen except that the second is a
      POINTER to an ID - which gets returned - not an ID which the
      user specifies.

      NOTE: dwInstance must be sent in as NULL! - just like waveInOpen.
 */

MMRESULT SRWaveMapper(LPHWAVEIN lphwi, PUINT puDeviceID, LPCWAVEFORMATEX lpwf,
                      DWORD dwCallback, DWORD dwInstance, DWORD fdwOpen)
{
   UINT         n = waveInGetNumDevs();
   WAVEINCAPS   wiCaps;
   MMRESULT     mmr;
   LONG         lRes;
   HKEY         hKeyMapper = NULL;
   DWORD        dwNeeded, dwType, dwPref = 0;
   CHAR         pName[MAXPNAMELEN];
   
   if (lphwi)
      *lphwi = NULL;

   lRes = RegOpenKeyEx(HKEY_CURRENT_USER,
                       "Software\\Microsoft\\Multimedia\\Sound Mapper",
                       0, KEY_READ, &hKeyMapper);

   if (lRes != ERROR_SUCCESS)
      goto NoReg;

   dwNeeded = MAXPNAMELEN;
   memset(pName, 0, MAXPNAMELEN);

   // read Pname from the registry
   lRes = RegQueryValueEx(hKeyMapper, "Record", NULL, &dwType, (PBYTE)pName,
                          &dwNeeded);

   if (lRes != ERROR_SUCCESS)
      goto NoReg;

   // read Preferred setting from the registry
   lRes = RegQueryValueEx(hKeyMapper, "PreferredOnly", NULL, &dwType, (PBYTE)&dwPref,
                          &dwNeeded);

   if (lRes != ERROR_SUCCESS)
      // Don't error out, just assume FALSE
      dwPref = FALSE;

   // look for the device that is in the registry
   for (*puDeviceID = 0; *puDeviceID < n; (*puDeviceID)++) {
      memset(&wiCaps, 0, sizeof(wiCaps));
      mmr = waveInGetDevCaps(*puDeviceID, &wiCaps, sizeof(wiCaps));
      if (mmr)
         continue;

      if (!lstrcmpi(wiCaps.szPname, pName)) {
         if (waveInOpen(lphwi, *puDeviceID, lpwf, dwCallback, dwInstance, fdwOpen) == 0) {
            if (hKeyMapper)
               RegCloseKey (hKeyMapper);
            return MMSYSERR_NOERROR;
         }
         else
            break;
      }
   }


   // we get here if we either can't open the device that is in the registry
   // or we can't find the device that is in the regsitry or we can't open or
   // read from the registry
NoReg:
   if (hKeyMapper)
      RegCloseKey (hKeyMapper);
   if (!dwPref) {
      for (*puDeviceID = 0; *puDeviceID < n; (*puDeviceID)++)
         if (waveInOpen(lphwi, *puDeviceID, lpwf, dwCallback, dwInstance, fdwOpen) == 0)
            return MMSYSERR_NOERROR;
   }

   if (lphwi)
      *lphwi = NULL;

   *puDeviceID = (UINT) -1;
   return (MMSYSERR_NODRIVER);
} /* End of SRWaveMapper() */


/************************************************************************
**  Function: FindLowPriDriver
**
**  Purpose : Scans all installed drivers looking for one that support
**            low-priority.  If it finds one, it stores the MID/PID
**            in VOICEPIL.INI, sets gwWaveDriver and gfLowPri, and
**            returns TRUE.
**
**  Parms   : wMID - MID we're looking for a mixer with a voicein for
**                   0xFFFF if we don't care.
**            wPID - PID we're looking for.
**                   0xFFFF if we don't care.
**
**  Returns : TRUE on success.
**            FALSE on failure.
**
********************************************************************/

BOOL FindLowPriDriver(HMIXER hMixer, MIXERLINE *pMixerLine, BOOL *bLowPriority, BOOL fTryLowPriority)
{
   BOOL        fRet = FALSE;
   MIXERLINE   ml;
   int         k;

   pMixerLine->cbStruct = 0;
   *bLowPriority = FALSE;

   // First look for low pri
   if (fTryLowPriority) {
      for (k = 0; !fRet && k < 2; k++) {
         ml.cbStruct        = sizeof (ml);
         ml.dwComponentType = (k == 0) ?
                               MIXERLINE_COMPONENTTYPE_DST_VOICEIN :
                               MIXERLINE_COMPONENTTYPE_DST_WAVEIN;

         if (!mixerGetLineInfo((HMIXEROBJ) hMixer, &ml, MIXER_GETLINEINFOF_COMPONENTTYPE)) {
            if (ml.dwUser & SNDSYS_MIXERLINE_LOWPRIORITY) {
               *pMixerLine   = ml;
               fRet          = TRUE;
               *bLowPriority = TRUE;

               return TRUE;
            }
         }
      }
   }

   // If no low pri - take anything
   for (k = 0; !fRet && k < 2; k++) {
      ml.cbStruct        = sizeof (ml);
      ml.dwComponentType = (k == 0) ?
                            MIXERLINE_COMPONENTTYPE_DST_VOICEIN :
                            MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
      if (!*bLowPriority)
         ml.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;

      if (!mixerGetLineInfo((HMIXEROBJ) hMixer, &ml, MIXER_GETLINEINFOF_COMPONENTTYPE)) {
         *pMixerLine   = ml;
         fRet          = TRUE;
         *bLowPriority = FALSE;

         return TRUE;
      }
   }

   return fRet;
} /* End of FindLowPriDriver() */


/************************************************************************
**
**  Function: avFindVolControl
**
**  Purpose : Finds the volume control in the mixer we need to twiddle
**            with for voice commands.
**
**  Parms   : lpID - Where to stuff the mixer control ID for the volume
**                   control that handles voice input.
**
**  Returns : One of:   AV_FVC_ERR_NONE         - No error
**                      AV_FVC_ERR_NOMIXER      - No mixer for voice-in.
**                      AV_FVC_ERR_NOVOLORMUX   - No vol/mux at voice-in.
**                      AV_FVC_ERR_MULTIMUX     - Voice-in mux had > 1 source.
**                      AV_FVC_ERR_NOVOLATSOURCE- Source of mux had no vol.
**
************************************************************************/

INT avFindVolControl(HMIXER hMixer, DWORD *lpMasterVolControl, DWORD* lpVolControl,
                     MIXERLINE MixerLine)
{
   LPMIXERCONTROL lpmxctrl;
   DWORD          dwID;
   BOOL           f = FALSE;

   *lpMasterVolControl = 0;
   *lpVolControl = 0;

   // Did we find the line on startup?
   // First look for a master volume control on the line.
   lpmxctrl = avFindControlOnLine(hMixer, MixerLine.dwLineID, MIXERCONTROL_CONTROLTYPE_VOLUME);
   if (lpmxctrl) {
      *lpMasterVolControl = lpmxctrl->dwControlID;
      f = TRUE;
   }

   // No volume control, look for a mux.
   lpmxctrl = avFindControlOnLine(hMixer, MixerLine.dwLineID, MIXERCONTROL_CONTROLTYPE_MUX);
   if (!lpmxctrl) {
      // #3270: If didn't find a mux, look for a mixer (and then handle
      //        it the same as you would the mux.
      lpmxctrl = avFindControlOnLine(hMixer, MixerLine.dwLineID, MIXERCONTROL_CONTROLTYPE_MIXER);

      // HACKHACK - This was (!lpmxtrl && !f) then return notsupported
      if (!lpmxctrl) {
         if (f)
            return 0;
         else
            return AUDERR_NOTSUPPORTED;      // was AV_FVC_ERR_NOVOLORMUX;
      }
   }

   // Find line selected for mux.
   dwID = avFindSelectedMuxLine(hMixer, MixerLine.dwDestination, lpmxctrl->dwControlID, lpmxctrl->cMultipleItems);
   if (dwID == MIXERCONTROLERROR && !f)
      return AUDERR_NOTSUPPORTED;         // AV_FVC_ERR_MULTIMUX;

   // Get the volume control on the selected line.
   lpmxctrl = avFindControlOnLine(hMixer, dwID, MIXERCONTROL_CONTROLTYPE_VOLUME);
   if (!lpmxctrl)
	   return (f ? 0 : AUDERR_NOTSUPPORTED);         // AV_FVC_ERR_NOVOLATSOURCE;
   // if (f) // Don't think should be doing this
   //   return AUDERR_NOTSUPPORTED;         // AV_FVC_ERR_NOVOLATSOURCE;
   *lpVolControl = lpmxctrl->dwControlID;

   return 0;
} /* End of avFindVolControl() */


/************************************************************************
**
**  Function: avFindControlOnLine
**
**  Purpose : Given a mixer line ID, returns the first control of a
**            given type on that line.
**
**  Parms   : dwLineID      - Line ID we're examinging.
**            dwControlType - Control type we're looking for.
**
**  Returns : Pointer to a mixer control structure for the given
**            type of control (note: that the caller must make a
**            copy of this before calling avFindControlOnLine() again!
**            NULL if no control of that type is found.
**
************************************************************************/

LPMIXERCONTROL avFindControlOnLine(HMIXER hMixer, DWORD dwLineID, DWORD dwControlType)
{
   MIXERLINECONTROLS   mxlc;
   static MIXERCONTROL mxctrl;

   mxlc.cbStruct      = sizeof(mxlc);
   mxlc.dwLineID      = dwLineID;
   mxlc.dwControlType = dwControlType;
   mxlc.cControls     = 1;
   mxlc.cbmxctrl      = sizeof(MIXERCONTROL);
   mxlc.pamxctrl      = &mxctrl;

   if (mixerGetLineControls((HMIXEROBJ) hMixer, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE))
      return (NULL);

   return (&mxctrl);
} /* End of avFindControlOnLine() */


/************************************************************************
**
**  Function: avFindSelectedMuxLine
**
**  Purpose : Returns the line ID of the line that is selected on a
**            given mux.
**
**  Parms   : dwMuxID        - ID of the mux we're examining.
**            cMultipleItems - # of items on the mux (from MIXERCONTROL).
**
**  Returns : Line ID of selected item on MUX.
**            MIXERCONTROLERROR if more than one selected, none selected,
**            or error.
**
************************************************************************/

DWORD avFindSelectedMuxLine(HMIXER hMixer, DWORD dwDestination, DWORD dwMuxID, DWORD cMultipleItems)
{
   LPMIXERCONTROLDETAILS_BOOLEAN  lpMuxOnOff = NULL;
   LPMIXERCONTROLDETAILS_LISTTEXT lpSources  = NULL;
   DWORD                          dwRet      = (DWORD) AUDERR_NOTSUPPORTED;
   MIXERCONTROLDETAILS            mxcd;
   UINT                           nMux, i;
   MIXERLINE                      mxl;
   int                            SrcMic     = -1;
   int                            SrcLineIn  = -1;
   int                            SrcOther   = -1;

   // Get info on sources to mux.
   lpSources = (LPMIXERCONTROLDETAILS_LISTTEXT)
               malloc(sizeof(MIXERCONTROLDETAILS_LISTTEXT) * cMultipleItems);

   if (!lpSources)
      goto avfsmlDone;

   mxcd.cbStruct       = sizeof(mxcd);
   mxcd.dwControlID    = dwMuxID;
   mxcd.cChannels      = 1;
   mxcd.cMultipleItems = cMultipleItems;
   mxcd.cbDetails      = sizeof(MIXERCONTROLDETAILS_LISTTEXT);
   mxcd.paDetails      = lpSources;

   if (mixerGetControlDetails((HMIXEROBJ) hMixer, &mxcd, MIXER_GETCONTROLDETAILSF_LISTTEXT))
      goto avfsmlDone;

   // Get info on selected guys on mux.
   lpMuxOnOff = (LPMIXERCONTROLDETAILS_BOOLEAN)
                malloc(sizeof(MIXERCONTROLDETAILS_BOOLEAN) * cMultipleItems);

   if (!lpMuxOnOff)
      goto avfsmlDone;

   mxcd.cbStruct       = sizeof(mxcd);
   mxcd.dwControlID    = dwMuxID;
   mxcd.cChannels      = 1;
   mxcd.cMultipleItems = cMultipleItems;
   mxcd.cbDetails      = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
   mxcd.paDetails      = lpMuxOnOff;

   if (mixerGetControlDetails((HMIXEROBJ) hMixer, &mxcd, MIXER_GETCONTROLDETAILSF_VALUE))
      goto avfsmlDone;

   // Okay, find the source that we want to use
   // we will search the sources in the following order:
   //    1) Microphone
   //    2) Line-In
   //    3) Use first enabled source
   for (i = 0; i < cMultipleItems; i++) {
      mxl.cbStruct = sizeof(MIXERLINE);
      mxl.dwDestination = dwDestination;
      mxl.dwSource = i;
      mixerGetLineInfo((HMIXEROBJ) hMixer, &mxl, MIXER_GETLINEINFOF_SOURCE);

      switch (mxl.dwComponentType) {
         case MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE:
            SrcMic = i;
            break;

         case MIXERLINE_COMPONENTTYPE_SRC_LINE:
            SrcLineIn = i;
            break;

         default:
            SrcOther = i;
            break;
      }
   }

   if (SrcMic != -1)
      nMux = SrcMic;
   else if (SrcLineIn != -1)
      nMux = SrcLineIn;
   else if (SrcOther != -1)
      nMux = SrcOther;
   else
      goto avfsmlDone;

   // Selected guy maps to guy in LISTTEXT.
   dwRet = lpSources[nMux].dwParam1;

avfsmlDone:

   if (lpSources)
      free(lpSources);
   if (lpMuxOnOff)
      free(lpMuxOnOff);

   return (dwRet);
} /* End of avFindSelectedMuxLine() */

/* End of audioin.cpp */
