/************************************************************************
FakeTTS.h - Header for fake engine code.

Copyright 1994 by Microsoft corporation.All rights reserved.
*/

/************************************************************************
Random stuff
*/

#include "ctools.h"

#ifndef _DESTROY_
#define _DESTROY_
typedef void (FAR PASCAL *LPFNDESTROYED) (void);
#endif 

extern DWORD       gEngObjectCount;    // number of objects existing
extern HINSTANCE   ghInstance;         // DLL library instance

/************************************************************************
Class factory object
*/

class CEngineClassFactory : public IClassFactory {

   protected:

      ULONG    m_cRef;  // Reference count on class object

   public:

      CEngineClassFactory(void);
      ~CEngineClassFactory(void);

// IUnknown menebers

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// TTSManagerClassFactory members

      STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
      STDMETHODIMP         LockServer (BOOL);
   };

typedef CEngineClassFactory FAR * PCEngineClassFactory;



/************************************************************************
Enumerator Object
*/

// CEngITTSEnum interface

class CEngITTSEnumW : public ITTSEnumW {

   private:

      ULONG          m_cRef;        // interface reference count
      LPVOID         m_pObj;        // Back pointer to the object
      LPUNKNOWN      m_punkOuter;   // Controlling unknown for delegation

   public:

      CEngITTSEnumW (LPVOID, LPUNKNOWN);
      ~CEngITTSEnumW (void);

      FInit(LPVOID, LPUNKNOWN);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ITTSFind

      STDMETHODIMP Next (ULONG, PTTSMODEINFOW, ULONG *);
      STDMETHODIMP Skip (ULONG);
      STDMETHODIMP Reset (void);
      STDMETHODIMP Clone (ITTSEnumW * FAR *);
      STDMETHODIMP Select (GUID, PITTSCENTRALW *, LPUNKNOWN);
   };

typedef CEngITTSEnumW * PCEngITTSEnumW;


class CEngITTSEnumA : public ITTSEnumA {

   private:

      LPVOID         m_pObjA;        // Back pointer to the (A..) object

   public:

      CEngITTSEnumA (LPVOID, LPUNKNOWN);
      ~CEngITTSEnumA (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ITTSFind

      STDMETHODIMP Next (ULONG, PTTSMODEINFOA, ULONG *);
      STDMETHODIMP Skip (ULONG);
      STDMETHODIMP Reset (void);
      STDMETHODIMP Clone (ITTSEnumA * FAR *);
      STDMETHODIMP Select (GUID, PITTSCENTRALA *, LPUNKNOWN);
   };

typedef CEngITTSEnumA * PCEngITTSEnumA;



// CEngineEnum class - TTS engine enumeration class

class CEngineEnum : public IUnknown {

// interfaces are friends

   friend class CEngITTSEnumW;
   friend class CEngITTSEnumA;

   protected:

      ULONG           m_cRef;              // reference count (# I.F.'s opened)
      LPUNKNOWN       m_punkOuter;         // controlling unknown for aggregation
      LPFNDESTROYED   m_pfnDestroy;        // function call on closure
      PCEngITTSEnumW  m_pEngITTSEnumW;     // Enumeration interface class
      PCEngITTSEnumA  m_pEngITTSEnumA;     // Enumeration interface class
      DWORD           dwCurrentEnumPosn;   // Current position for next, skip

   public:

      CEngineEnum (LPUNKNOWN, LPFNDESTROYED);
      ~CEngineEnum (void);

      BOOL FInit (void);

// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);
   };

typedef CEngineEnum * PCEngineEnum;



/************************************************************************
TTS-mode object
*/


/* Mode notification object */

class CModeNotify : public IAudioDestNotifySink {

   private:

      void *         m_pObj;        // CMode object

   public:

      CModeNotify (void * pObj);
      ~CModeNotify (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// IAudioDestNotifySink

      STDMETHODIMP AudioStop   (WORD);
      STDMETHODIMP AudioStart   (void);
      STDMETHODIMP FreeSpace  (DWORD, BOOL);
      STDMETHODIMP BookMark   (DWORD, BOOL);
   };

typedef CModeNotify * PCModeNotify;



// IAttributesW interface for engines
// A CModeITTSAttributesA inteface is not needed yet but given anyway.

class CModeITTSAttributesW : public ITTSAttributesW {

   private:

      ULONG          m_cRef;        // interface reference count
      LPVOID         m_pObj;        // Back pointer to the object
      LPUNKNOWN      m_punkOuter;   // Controlling unknown for delegation

   public:

      CModeITTSAttributesW (LPVOID, LPUNKNOWN);
      ~CModeITTSAttributesW (void);

      FInit(LPVOID, LPUNKNOWN);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ITTSAttributes members

      STDMETHODIMP PitchGet (WORD *);
      STDMETHODIMP PitchSet (WORD);
      STDMETHODIMP RealTimeGet (DWORD *);
      STDMETHODIMP RealTimeSet (DWORD);
      STDMETHODIMP SpeedGet (DWORD *);
      STDMETHODIMP SpeedSet (DWORD);
      STDMETHODIMP VolumeGet(DWORD *);
      STDMETHODIMP VolumeSet(DWORD);
   };

typedef CModeITTSAttributesW * PCModeITTSAttributesW;


class CModeITTSAttributesA : public ITTSAttributesA {

   private:

      LPVOID         m_pObjA;        // Back pointer to the (A..) object

   public:

      CModeITTSAttributesA (LPVOID, LPUNKNOWN);
      ~CModeITTSAttributesA(void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ITTSAttributes members

      STDMETHODIMP PitchGet (WORD *);
      STDMETHODIMP PitchSet (WORD);
      STDMETHODIMP RealTimeGet (DWORD *);
      STDMETHODIMP RealTimeSet (DWORD);
      STDMETHODIMP SpeedGet (DWORD *);
      STDMETHODIMP SpeedSet (DWORD);
      STDMETHODIMP VolumeGet(DWORD *);
      STDMETHODIMP VolumeSet(DWORD);
   };

typedef CModeITTSAttributesA * PCModeITTSAttributesA;



// ITTSCentral interface for engines

class CModeITTSCentralW : public ITTSCentralW {

   private:

      ULONG          m_cRef;        // interface reference count
      LPVOID         m_pObj;        // Back pointer to the object
      LPUNKNOWN      m_punkOuter;   // Controlling unknown for delegation

   public:

      CModeITTSCentralW (LPVOID, LPUNKNOWN);
      ~CModeITTSCentralW (void);

      FInit(LPVOID, LPUNKNOWN);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (THIS_ REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(THIS);
      STDMETHODIMP_(ULONG) Release(THIS);

// ITTSCentral members

      STDMETHODIMP Inject         (THIS_ PCWSTR);
      STDMETHODIMP ModeGet        (THIS_ PTTSMODEINFOW);
      STDMETHODIMP Phoneme        (THIS_ VOICECHARSET, DWORD, SDATA, PSDATA);
      STDMETHODIMP PosnGet        (THIS_ PQWORD);
      STDMETHODIMP TextData       (THIS_ VOICECHARSET, DWORD, SDATA, PVOID, IID);
      STDMETHODIMP ToFileTime     (THIS_ PQWORD, FILETIME *);
      STDMETHODIMP AudioPause     (THIS);
      STDMETHODIMP AudioResume    (THIS) ;
      STDMETHODIMP AudioReset     (THIS);
      STDMETHODIMP Register       (THIS_ PVOID, IID, DWORD*);
      STDMETHODIMP UnRegister     (THIS_ DWORD);
   };

typedef CModeITTSCentralW * PCModeITTSCentralW;


class CModeITTSCentralA : public ITTSCentralA {

   private:

      LPVOID         m_pObjA;        // Back pointer to the (A..) object

   public:

      CModeITTSCentralA (LPVOID, LPUNKNOWN);
      ~CModeITTSCentralA (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (THIS_ REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(THIS);
      STDMETHODIMP_(ULONG) Release(THIS);

// ITTSCentral members

      STDMETHODIMP Inject     (THIS_ PCSTR);
      STDMETHODIMP ModeGet    (THIS_ PTTSMODEINFOA);
      STDMETHODIMP Phoneme    (THIS_ VOICECHARSET, DWORD, SDATA, PSDATA);
      STDMETHODIMP PosnGet    (THIS_ PQWORD);
      STDMETHODIMP TextData   (THIS_ VOICECHARSET, DWORD, SDATA, PVOID, IID);
      STDMETHODIMP ToFileTime (THIS_ PQWORD, FILETIME *);
      STDMETHODIMP AudioPause (THIS);
      STDMETHODIMP AudioResume(THIS) ;
      STDMETHODIMP AudioReset (THIS);
      STDMETHODIMP Register   (THIS_ PVOID, IID, DWORD*);
      STDMETHODIMP UnRegister (THIS_ DWORD);
   };

typedef CModeITTSCentralA * PCModeITTSCentralA;



// ITTSDialogs interface for engines

class CModeITTSDialogsW : public ITTSDialogsW {

   private:

      ULONG          m_cRef;        // interface reference count
      LPVOID         m_pObj;        // Back pointer to the object
      LPUNKNOWN      m_punkOuter;   // Controlling unknown for delegation

   public:

      CModeITTSDialogsW (LPVOID, LPUNKNOWN);
      ~CModeITTSDialogsW (void);

      FInit(LPVOID, LPUNKNOWN);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (THIS_ REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(THIS);
      STDMETHODIMP_(ULONG) Release(THIS);

// ITTSDialogs members

      STDMETHODIMP AboutDlg      (THIS_ HWND, PCWSTR);
      STDMETHODIMP LexiconDlg    (THIS_ HWND, PCWSTR);
      STDMETHODIMP GeneralDlg    (THIS_ HWND, PCWSTR);
      STDMETHODIMP TranslateDlg  (THIS_ HWND, PCWSTR);
   };

typedef CModeITTSDialogsW * PCModeITTSDialogsW;


class CModeITTSDialogsA : public ITTSDialogsA {

   private:

      LPVOID         m_pObjA;        // Back pointer to the (A..) object

   public:

      CModeITTSDialogsA (LPVOID, LPUNKNOWN);
      ~CModeITTSDialogsA (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (THIS_ REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(THIS);
      STDMETHODIMP_(ULONG) Release(THIS);

// ITTSDialogs members

      STDMETHODIMP AboutDlg      (THIS_ HWND, PCSTR);
      STDMETHODIMP LexiconDlg    (THIS_ HWND, PCSTR);
      STDMETHODIMP GeneralDlg    (THIS_ HWND, PCSTR);
      STDMETHODIMP TranslateDlg  (THIS_ HWND, PCSTR);
   };

typedef CModeITTSDialogsA * PCModeITTSDialogsA;



// CMode class - TTS engine mode

class CMode : public IUnknown {

// interfaces are friends

   friend class CModeITTSAttributesW;
   friend class CModeITTSAttributesA;
   friend class CModeITTSCentralW;
   friend class CModeITTSCentralA;
   friend class CModeITTSDialogsW;
   friend class CModeITTSDialogsA;
   friend class CModeNotify;
   friend class CEngITTSEnumW;

   protected:

      ULONG                 m_cRef;                   // ref count (# I.F's opened)
      LPUNKNOWN             m_punkOuter;              // controlling unknown for aggr.
      LPFNDESTROYED         m_pfnDestroy;             // function call on closure
      PCModeITTSAttributesW m_pModeITTSAttributesW;   // Attreibutes class
      PCModeITTSAttributesA m_pModeITTSAttributesA;   // Attreibutes class
      PCModeITTSCentralW    m_pModeITTSCentralW;      // Central control class
      PCModeITTSCentralA    m_pModeITTSCentralA;      // Central control class
      PCModeITTSDialogsW    m_pModeITTSDialogsW;      // Dialogs class
      PCModeITTSDialogsA    m_pModeITTSDialogsA;      // Dialogs class
      LPUNKNOWN             m_pAudioIUnknown;         // unknown for the audio
      PIAUDIO               m_pAudioIAudio;           // to get audio information from
      PIAUDIODEST           m_pAudioIAudioDest;       // to get audio information from
      PCModeNotify          m_pNotify;                // notify sink object
      WORD                  m_wPitch;                 // Pitch
      DWORD                 m_dwRealTime;             // real-time value
      DWORD                 m_dwSpeed;                // Speed
      DWORD                 m_dwVolume;               // Volume
      TTSMODEINFOW          m_TTSModeInfo;            // Stores the selected mode

      // Current speaking state
      BOOL                  m_fClaimed;               // TRUE if wave device claimed
      BOOL                  m_fPaused;                // TRUE if app has called pause
      BOOL                  m_fDataPending;           // TRUE if data is pending

// The following pair of lists is a quick and dirty way to effect a
// list of Texts + associated IDs (as opposed to a list of structs
// which include a text and an ID each).  I.e. the lists are always
// maintained in parallel.

      PCList         m_pTextList;                  // list of text objects
      PCList         m_pIDList;                    // companion of TextList, lists ofIDs, use BUFLIST structre

// 'currently speaking' means speech is lined up to speak but not necessarily
// that sound is coming out the audio componant.

      BOOL           m_fSpeaking;                  // TRUE if currently speaking

      void           Output (PSTR);                // Output string to edit window
      BOOL           Get1Char       (CHAR *);      // 1 ch fm list to screen
      void           Print1Char     (HWND, CHAR);  // 1 ch fm list to screen
      STDMETHODIMP   DataAvailable  (DWORD, BOOL);
      HRESULT        SpeakIfNecessary(void);
      HRESULT        SendAudioIfCan (void);

   public:

      CMode (LPUNKNOWN, LPFNDESTROYED, LPUNKNOWN);

// NOTE: Need more stuff here

      ~CMode (void);

      BOOL FInit (void);

// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (THIS_ REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(THIS);
      STDMETHODIMP_(ULONG) Release(THIS);

      HWND           hWndMain;      // main window
      HWND           hWndEdit;      // edit control window
      PCList         m_pNotifyList;
   };

typedef CMode * PCMode;



// a couple local helper functions for converting in and out of an _int64:

void FileTimeToInt64(LPFILETIME pft, _int64 *pI);
void Int64ToFileTime(_int64 *pI, LPFILETIME pft);
