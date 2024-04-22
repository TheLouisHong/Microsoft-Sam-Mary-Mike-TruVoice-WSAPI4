/************************************************************************
Audio.h - Definitions, classes, and prototypes for SR manager DLL.
Copyright 1994-1995 by Microsoft corporation. All rights reserved.
*/


#ifndef _AUDIO_H_
#define	_AUDIO_H_

#include "audioinc.h"
/************************************************************************
Random stuff
*/

#ifndef _DESTROY_
#define _DESTROY_
typedef void (FAR PASCAL *LPFNDESTROYED) (void);
#endif 

#include "ctools.h"

// two defines lifted from last year's ...driver\mixtest\iwaveio.h:

#define DRVMSG_USER        0x4000
#define WIDM_LOWPRIORITY   (DRVMSG_USER+0x93)

#define SNDSYS_MIXERLINE_LOWPRIORITY  1
#define MIXERCONTROLERROR       (0xffff)

#define AV_MINVOL   0x00000000
#define AV_MAXVOL   0x0000ffff

extern LONG        gEnumObjectCount;    // number of objects existing
extern HINSTANCE   ghInstance;             // DLL library instance


/************************************************************************
structures & defines
*/

// attaches bookmarks to audio streams

typedef struct
   {
   QWORD    qwCurrTime;
   DWORD    dwMarkID;
   } BOOKMARK, *PBOOKMARK;





/************************************************************************
CacheObject - This is a FIFO buffer cache. The cache is circular.
*/

class CCache {

   private:

      DWORD          m_dwCacheSize;   // Cache size (in bytes)
      BYTE           *m_pData;         // pointer to the data buffer
      DWORD          m_dwStartOffset;  // starting offset of cache
      DWORD          m_dwData;         // amount of valid data
      HRESULT MakeSureTheresMemory (void);

   public:

      CCache (DWORD dwCacheSize);
      ~CCache (void);
      HRESULT DataGet (PVOID, DWORD, DWORD *);
      HRESULT DataAdd (PVOID, DWORD, DWORD *);
      HRESULT DataFlush (void);
      HRESULT Space (DWORD *);
      HRESULT Used (DWORD *);
   };

typedef CCache * PCCache;






/************************************************************************
Audio Output Object
*/

// IAudio interface

class CAOutIAudio : public IAudio {

   private:

      ULONG          m_cRef;           // interface reference count
      LPVOID         m_pObj;           // Back pointer to the object
      LPUNKNOWN      m_punkOuter;      // Controlling unknown for delegation

   public:

      CAOutIAudio (LPVOID, LPUNKNOWN);
      ~CAOutIAudio (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// IAudio

	   STDMETHODIMP Flush          (void) ;
	   STDMETHODIMP LevelGet       ( DWORD *) ;
	   STDMETHODIMP LevelSet       ( DWORD) ;
	   STDMETHODIMP PassNotify     ( PVOID, IID) ;
	   STDMETHODIMP PosnGet        ( PQWORD) ;
	   STDMETHODIMP Claim          (void) ;
	   STDMETHODIMP UnClaim        (void) ;
	   STDMETHODIMP Start          (void) ;
	   STDMETHODIMP Stop           (void) ;
	   STDMETHODIMP TotalGet       ( PQWORD) ;
		STDMETHODIMP ToFileTime		( PQWORD, FILETIME *) ;
	   STDMETHODIMP WaveFormatGet  ( PSDATA) ;
	   STDMETHODIMP WaveFormatSet  ( SDATA) ;	  
   };

typedef CAOutIAudio * PCAOutIAudio;

// IAudioDest interface

class CAOutIAudioDest : public IAudioDest {

   private:

      ULONG          m_cRef;           // interface reference count
      LPVOID         m_pObj;           // Back pointer to the object
      LPUNKNOWN      m_punkOuter;      // Controlling unknown for delegation

   public:

      CAOutIAudioDest (LPVOID, LPUNKNOWN);
      ~CAOutIAudioDest (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// IAudioDest

	   STDMETHODIMP FreeSpace      (DWORD *, BOOL *);
	   STDMETHODIMP DataSet        (PVOID, DWORD);
	   STDMETHODIMP BookMark       (DWORD);
   };

typedef CAOutIAudioDest * PCAOutIAudioDest;




// IAudioMultiMediaDevice interface

class CAOutIAudioMultiMediaDevice : public IAudioMultiMediaDevice {

   private:

      ULONG          m_cRef;           // interface reference count
      LPVOID         m_pObj;           // Back pointer to the object
      LPUNKNOWN      m_punkOuter;      // Controlling unknown for delegation

   public:

      CAOutIAudioMultiMediaDevice (LPVOID, LPUNKNOWN);
      ~CAOutIAudioMultiMediaDevice (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// IAudioMultiMediaDevice

	 	STDMETHODIMP CustomMessage  (UINT, SDATA);
		STDMETHODIMP DeviceNumGet	(DWORD*);
	   STDMETHODIMP DeviceNumSet   (DWORD);
   };

typedef CAOutIAudioMultiMediaDevice * PCAOutIAudioMultiMediaDevice;



#define  AOUT_NUMBUF     (32)   // # of buffers
#define  AOUT_BUFSPERSEC (16)  // # of buffers per second
//#define  AOUT_NUMBUF     (4)   // # of buffers
//#define  AOUT_BUFSPERSEC (2)  // # of buffers per second

#define  AOUT_CACHESIZE  (2)   // in seconds


#define  WAVEOUTTIMER      89    // timer ID for the wave out position timer
#define  WAVEOUTTIMERMS    (1000 / 30) // pause between timer ticks

// CAOut class - Audio Out class

class CAOut : public IUnknown {

// interfaces are friends

   friend class CAOutIAudioDest;
   friend class CAOutIAudio;
   friend class CAOutIAudioMultiMediaDevice;

   protected:

      ULONG                   m_cRef;              // reference count (# interfaces opened)
      LPUNKNOWN               m_punkOuter;         // controlling unknown for aggregation
      LPFNDESTROYED           m_pfnDestroy;        // function call on closure
      PCAOutIAudioMultiMediaDevice m_pAOutIAudioMultiMediaDevice;   // Enumeration interface class
      PIAUDIODESTNOTIFYSINK   m_pIADNS;            // notify sink

      PCCache        m_pCache;         // cache object
      PCCache        m_pFSCache;       // Forced Stop cache object
      LPWAVEFORMATEX m_pWFEX;          // wave format EX
      DWORD          m_dwWFEXSize;     // size of waveformatex
      QWORD          m_qwTotal;        // total amount played out
      DWORD          m_dwWaveDev;      // wave device number
      CHAR           m_szDeviceName[MAXPNAMELEN]; // device name
      HWAVEOUT       m_hWaveOut;       // wave out handle
      DWORD          m_dnDevNode;      // Device node of the waveout device
      BOOL           m_bFlushing;      // TRUE during a flush operation
      DWORD          m_dwBufSize;      // buffer size (in bytes)
      HGLOBAL        m_ahBufGlobal[AOUT_NUMBUF];   // global memory for the buffers
      LPWAVEHDR      m_apBufWH[AOUT_NUMBUF];       // buffer memory, from global
      QWORD          m_aqwBufTime[AOUT_NUMBUF];    // for bookmarks
      DWORD          m_msStartTime;    // Time-stamp when object started,in milliseconds since windows started
      FILETIME       m_ftStartTime;    // Time-stamp when object started, in FILETIME units
      QWORD          m_qwLastBufTime;  // Time-stamp of last returned buffer, in data-rate time-stamp
      DWORD          m_msLastBufTime;  // Time-stamp of last returned buffer, in milliseonds since windows
      int            m_nState;         // Current state of the audio output object
      BOOL           m_fSentEmptyBuf;  // True if the last free space notification was cause the cache is empty

   public:

      HWND           m_hWndMonitor;    // monitoring window

      CAOut (LPUNKNOWN, LPFNDESTROYED);
      ~CAOut (void);

      BOOL FInit (DWORD, LPWAVEFORMATEX, DWORD);

// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      STDMETHODIMP   TryToStart (void);      // Try playing but don't retry
      STDMETHODIMP   OpenAndPrepare (void);  // Open the wave device and prepare the headers
      BOOL TryToSendBufOut();                // Pick a buf & send it
      void ReturnedBuffer(LPWAVEHDR);        // a buffer is returned
      void ReturnedBookMarks(QWORD qwTime);  // Looks for BookMarks to notify
      INT qwTimeCmp(QWORD a, QWORD b);       // { -1 (a < b), 0, 1 } - compares 2 qwTimes
      void Notify(void);                     // change-of-cache note to app
      void NotifyAudioStop(WORD);            // change-of-cache note to app
      void NotifyAudioStart(void);           // change-of-cache note to app
      void CloseIt(void);                    // close the device
      BOOL HandleDeviceChange(DWORD dwEvent, PDEV_BROADCAST_HDR pDevBroadcastHdr);

      BOOL           m_fForceStop;           // If TRUE then doing a foreced stop
#if 0
      BOOL           m_fTimerOn;             // If TRUE then timer is on
#endif /* 0 */
      BOOL           m_fRemoved;             // TRUE if device has been removed
      DWORD          m_fBufsOut;             // bit-wise flag indicating which buffers out
      WAVEOUTCAPS    m_wc;                   // device capabilities
      CRITICAL_SECTION m_CritSec;
      QWORD          m_qwOffsetWaveOutPosn;  // offset to add to waveOutGetPosition
      BOOL           m_fIsTimerOn;           // TRUE if the timer for waveOutGetPosition is on
      BOOL           m_fJustOpenedOrReset;   // TRUE if just opened or reset
      PCAOutIAudioDest        m_pAOutIAudioDest;   // Enumeration interface class
      PCAOutIAudio            m_pAOutIAudio;   // Enumeration interface class
      PCList         m_pBookMarkList;  // List of active bookmarks
      BOOL           m_fDriverCrashOnPosn;   // TRUE if known that driver crashes on audio out posn
   };

typedef CAOut * PCAOut;



/************************************************************************
Audio Input Object
*/
// IAudio interface

class CAInIAudio : public IAudio {

   private:

      ULONG          m_cRef;           // interface reference count
      LPVOID         m_pObj;           // Back pointer to the object
      LPUNKNOWN      m_punkOuter;      // Controlling unknown for delegation

   public:

      CAInIAudio (LPVOID, LPUNKNOWN);
      ~CAInIAudio (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// IAudio

	   STDMETHODIMP Flush          (void) ;
	   STDMETHODIMP LevelGet       ( DWORD *) ;
	   STDMETHODIMP LevelSet       ( DWORD) ;
	   STDMETHODIMP PassNotify     ( PVOID, IID) ;
	   STDMETHODIMP PosnGet        ( PQWORD) ;
	   STDMETHODIMP Claim          (void) ;
	   STDMETHODIMP UnClaim        (void) ;
	   STDMETHODIMP Start          (void) ;
	   STDMETHODIMP Stop           (void) ;
	   STDMETHODIMP TotalGet       ( PQWORD) ;
		STDMETHODIMP ToFileTime		( PQWORD, FILETIME *) ;
	   STDMETHODIMP WaveFormatGet  ( PSDATA) ;
	   STDMETHODIMP WaveFormatSet  ( SDATA) ;	  
   };

typedef CAInIAudio * PCAInIAudio;

// IAudioSource interface

class CAInIAudioSource : public IAudioSource {

   private:

      ULONG          m_cRef;        // interface reference count
      LPVOID         m_pObj;        // Back pointer to the object
      LPUNKNOWN      m_punkOuter;   // Controlling unknown for delegation

   public:

      CAInIAudioSource (LPVOID, LPUNKNOWN);
      ~CAInIAudioSource (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// IAudioSource

      STDMETHODIMP DataAvailable  (DWORD *, BOOL *);
      STDMETHODIMP DataGet        (PVOID, DWORD, DWORD *);
   };


typedef CAInIAudioSource * PCAInIAudioSource;




// IAudioMultiMediaDevice interface

class CAInIAudioMultiMediaDevice : public IAudioMultiMediaDevice {

   private:

      ULONG          m_cRef;           // interface reference count
      LPVOID         m_pObj;           // Back pointer to the object
      LPUNKNOWN      m_punkOuter;      // Controlling unknown for delegation

   public:

      CAInIAudioMultiMediaDevice (LPVOID, LPUNKNOWN);
      ~CAInIAudioMultiMediaDevice (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// IAudioMultiMediaDevice

	 	STDMETHODIMP CustomMessage  (UINT, SDATA);
		STDMETHODIMP DeviceNumGet	(DWORD*);
	   STDMETHODIMP DeviceNumSet   (DWORD);
   };

typedef CAInIAudioMultiMediaDevice * PCAInIAudioMultiMediaDevice;



#define  AIN_NUMINITBUF (4)    // # of initial buffers
#define  AIN_NUMBUF     (32)   // # of buffers
#define  AIN_BUFSPERSEC (16)  // # of buffers per second
#define  AIN_CACHESIZE  (2)   // in seconds

// CAIn class - Audio In class

class CAIn : public IUnknown {

// interfaces are friends

   friend class CAInIAudioSource;
   friend class CAInIAudio;
   friend class CAInIAudioMultiMediaDevice;
   friend LRESULT CALLBACK AInWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

   protected:

      ULONG                   m_cRef;             // reference count (# interfaces opened)
      LPUNKNOWN               m_punkOuter;        // controlling unknown for aggregation
      LPFNDESTROYED           m_pfnDestroy;       // function call on closure
      PCAInIAudioSource       m_pAInIAudioSource; // Enumeration inteerface class
      PCAInIAudio             m_pAInIAudio;   // Enumeration interface class
      PCAInIAudioMultiMediaDevice m_pAInIAudioMultiMediaDevice;   // Enumeration interface class
      PIAUDIOSOURCENOTIFYSINK m_pIASNS;           // notify sink

      PCCache        m_pCache;      // cache object
      LPWAVEFORMATEX m_pWFEX;       // wave format EX
      DWORD          m_dwWFEXSize;  // size of waveformatex
      QWORD          m_qwTotal;     // total amount played out
      DWORD          m_dwWaveDev;   // wave device ID
      CHAR           m_szDeviceName[MAXPNAMELEN]; // device name
      BOOL           m_bRecording;  // TRUE if recording
      HWAVEIN        m_hWaveIn;     // wave in handle
      DWORD          m_dnDevNode;   // Device node of wave device
      HMIXER         m_hMixer;      // input mixer handle (or NULL if none)
      DWORD          m_dwMasterVolCtrlID; // Control ID for the master volume
      DWORD          m_dwVolCtrlID; // Control ID for the volume
      MIXERLINE      m_gMixerLine;
      HWND           m_hWndMonitor; // monitoring window
      BOOL           m_fLowPriority;   // whether this is a low priority in dev.

      DWORD          m_dwBufSize;   // buffer size (in bytes)
      DWORD          m_fBufsOut;    // bit-wise flag indicating which buffers out
      HGLOBAL        m_ahBufGlobal[AIN_NUMBUF]; // global memory for the buffers
      LPWAVEHDR      m_apBufWH[AIN_NUMBUF]; // buffer memory, from global
      DWORD          m_msStartTime;    // Time-stamp when object started,in milliseconds since windows started
      FILETIME       m_ftStartTime;    // Time-stamp when object started, in FILETIME units
      QWORD          m_qwLastBufTime;  // Time-stamp of last returned buffer, in data-rate time-stamp
      DWORD          m_msLastBufTime;  // Time-stamp of last returned buffer, in milliseonds since windows
      BOOL           m_fClaimed;       // TRUE if the wavein device has been claimed
      BOOL           m_fAutoClaimed;   // TRUE if we claimed the device for the application
      BOOL           m_fFlushing;      // True if we are flushing audio data
      int            m_nResumeBufID;   // Buffer ID to resume recording with

   public:

      CAIn (LPUNKNOWN, LPFNDESTROYED);
      ~CAIn (void);

      BOOL FInit (DWORD, LPWAVEFORMATEX, DWORD);

// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      STDMETHODIMP TryToStart(void);      // Try to start recording. Dont retry if fails
      STDMETHODIMP OpenAndPrepare(void);  // Open the wave device and prepare the headers
      BOOL TryToSendBufOut();             // Pick a buffer and send it out
      void ReturnedBuffer(LPWAVEHDR);     // Called when a buffer is returned
      void Notify(void);                  // Send change-of-cache notification to app
      void CloseIt(void);                 // Closes the wave device
      void NotifyAudioStop(WORD);         // audio stopped note to app
      void NotifyAudioStart(void);        // audio started note to app
      void NotifyOverflow(DWORD);         // change-of-cache note to app
      BOOL HandleDeviceChange(DWORD dwEvent, PDEV_BROADCAST_HDR pDevBroadcastHdr);
      UINT m_uTimer;                      // TRUE if timer is going
      BOOL m_fRemoved;                    // TRUE if device has been removed

      BOOL  m_fIgnoreDataUntilNextClose;  // just did a reset and then close, so ignore all returning data
   };

typedef CAIn * PCAIn;




#endif // _AUDIO_H_

