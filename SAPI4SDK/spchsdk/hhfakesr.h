/************************************************************************
FakeSR.h - Header for fake engine code.

Copyright 1994 by Microsoft corporation.All rights reserved.
*/

/************************************************************************
Random stuff
*/

#include "ctools.h"

#ifndef _DESTROY_
#define _DESTROY_
#define  ASSERT(x)      // NOTE: Do a real assert
typedef void (FAR PASCAL *LPFNDESTROYED) (void);
#endif 

extern DWORD       gEngObjectCount;    // number of objects existing
extern HINSTANCE   ghInstance;             // DLL library instance





class CGramFormatBrowse {
   private:
      PVOID      m_pMem;
      DWORD      m_dwMemSize;

   public:
      CGramFormatBrowse (void);
      ~CGramFormatBrowse (void);

      BOOL   Init (PVOID, DWORD);
      PVOID FindChunk (DWORD dwChunkID, DWORD * pdwSize);
      DWORD   GetGrammarFormat (void);
      DWORD   GetGrammarFlags (void);
   };
typedef CGramFormatBrowse * PCGramFormatBrowse;



/************************************************************************
Class factory object
*/

class CEngineClassFactory : public IClassFactory {
   protected:
      ULONG          m_cRef;  // Reference count on class object

   public:
      CEngineClassFactory(void);
      ~CEngineClassFactory(void);

      // IUnknown menebers
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // SRManagerClassFactory members
      STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
      STDMETHODIMP         LockServer (BOOL);
   };

typedef CEngineClassFactory FAR * PCEngineClassFactory;






/************************************************************************
Enumerator Object
*/
// CEngISRFind interface

// CEngISREnum interface

class CEngISREnumW : public ISREnumW {

   private:

      ULONG          m_cRef;        // interface reference count
      LPVOID         m_pObj;        // Back pointer to the object
      LPUNKNOWN      m_punkOuter;   // Controlling unknown for delegation

   public:

      CEngISREnumW (LPVOID, LPUNKNOWN);
      ~CEngISREnumW (void);

      FInit(LPVOID, LPUNKNOWN);

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ISRFind
      STDMETHODIMP Next (ULONG, PSRMODEINFOW, ULONG *);
      STDMETHODIMP Skip (ULONG);
      STDMETHODIMP Reset (void);
      STDMETHODIMP Clone (ISREnumW * FAR *);
      STDMETHODIMP Select (GUID, PISRCENTRALW *, LPUNKNOWN);
   };

typedef CEngISREnumW * PCEngISREnumW;


class CEngISREnumA : public ISREnumA {

   private:

      LPVOID         m_pObjA;        // Back pointer to the (A..) object

   public:

      CEngISREnumA (LPVOID, LPUNKNOWN);
      ~CEngISREnumA (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ISRFind

      STDMETHODIMP Next (ULONG, PSRMODEINFOA, ULONG *);
      STDMETHODIMP Skip (ULONG);
      STDMETHODIMP Reset (void);
      STDMETHODIMP Clone (ISREnumA * FAR *);
      STDMETHODIMP Select (GUID, PISRCENTRALA *, LPUNKNOWN);
   };

typedef CEngISREnumA * PCEngISREnumA;



// CEngineEnum class - SR engine enumeration class

class CEngineEnum : public IUnknown {

// interfaces are friends

   friend class CEngISREnumW;
   friend class CEngISREnumA;

   protected:
      ULONG          m_cRef;              // reference count (# interfaces opened)
      LPUNKNOWN      m_punkOuter;         // controlling unknown for aggregation
      LPFNDESTROYED  m_pfnDestroy;        // function call on closure
      PCEngISREnumW  m_pEngISREnumW;      // Enumeration inteerface class
      PCEngISREnumA  m_pEngISREnumA;      // Enumeration inteerface class
      DWORD          dwCurrentEnumPosn;   // Current enumeration position for next, skip, point in the list
   // CList          list;                // linked list of engines

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
Grammar object
*/
// ISRGrammar interface for engines

class CGrmISRGramCommonW : public ISRGramCommonW {

   private:

      ULONG          m_cRef;        // interface reference count
      LPVOID         m_pObj;        // Back pointer to the object
      LPUNKNOWN      m_punkOuter;   // Controlling unknown for delegation

   public:

      CGrmISRGramCommonW (LPVOID, LPUNKNOWN);
      ~CGrmISRGramCommonW (void);

      FInit(LPVOID, LPUNKNOWN);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ISRAttributes members
      STDMETHODIMP Archive (BOOL, PVOID, DWORD, DWORD *);
      STDMETHODIMP Activate (HWND, BOOL, PCWSTR);
      STDMETHODIMP BookMark (QWORD, DWORD);
      STDMETHODIMP Context (PWSTR, PWSTR);
      STDMETHODIMP Deactivate (PCWSTR);
      STDMETHODIMP ListSet(DWORD, SDATA);
      STDMETHODIMP DeteriorationGet (DWORD *, DWORD *, DWORD *);
      STDMETHODIMP DeteriorationSet (DWORD, DWORD, DWORD);
      STDMETHODIMP TrainDlg (HWND, PCWSTR);
      STDMETHODIMP TrainPhrase (DWORD, PSDATA);
      STDMETHODIMP TrainQuery (DWORD *);
   };
typedef CGrmISRGramCommonW * PCGrmISRGramCommonW;


class CGrmISRGramCommonA : public ISRGramCommonA {

   private:

      LPVOID         m_pObjA;        // Back pointer to the (A..) object

   public:

      CGrmISRGramCommonA (LPVOID, LPUNKNOWN);
      ~CGrmISRGramCommonA (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ISRAttributes members

      STDMETHODIMP Archive (BOOL, PVOID, DWORD, DWORD *);
      STDMETHODIMP Activate (HWND, BOOL, PCSTR);
      STDMETHODIMP BookMark (QWORD, DWORD);
      STDMETHODIMP Context (PSTR, PSTR);
      STDMETHODIMP Deactivate (PCSTR);
      STDMETHODIMP ListSet(DWORD, SDATA);
      STDMETHODIMP DeteriorationGet (DWORD *, DWORD *, DWORD *);
      STDMETHODIMP DeteriorationSet (DWORD, DWORD, DWORD);
      STDMETHODIMP TrainDlg (HWND, PCSTR);
      STDMETHODIMP TrainPhrase (DWORD, PSDATA);
      STDMETHODIMP TrainQuery (DWORD *);
   };
typedef CGrmISRGramCommonA * PCGrmISRGramCommonA;



class CGrmISRGramCFGW : public ISRGramCFGW {

   private:

      ULONG          m_cRef;        // interface reference count
      LPVOID         m_pObj;        // Back pointer to the object
      LPUNKNOWN      m_punkOuter;   // Controlling unknown for delegation

   public:

      CGrmISRGramCFGW (LPVOID, LPUNKNOWN);
      ~CGrmISRGramCFGW (void);

      FInit(LPVOID, LPUNKNOWN);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ISRAttributes members

      STDMETHODIMP LinkQuery      (PCWSTR, BOOL *);
      STDMETHODIMP ListAppend     (PCWSTR, SDATA);
      STDMETHODIMP ListGet        (PCWSTR, PSDATA);
      STDMETHODIMP ListRemove     (PCWSTR, SDATA);
      STDMETHODIMP ListSet        (PCWSTR, SDATA);
      STDMETHODIMP ListQuery      (PCWSTR, BOOL *);
   };

typedef CGrmISRGramCFGW * PCGrmISRGramCFGW;


class CGrmISRGramCFGA : public ISRGramCFGA {

   private:

      LPVOID         m_pObjA;        // Back pointer to the (A..) object

   public:

      CGrmISRGramCFGA (LPVOID, LPUNKNOWN);
      ~CGrmISRGramCFGA (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ISRAttributes members

      STDMETHODIMP LinkQuery      (PCSTR, BOOL *);
      STDMETHODIMP ListAppend     (PCSTR, SDATA);
      STDMETHODIMP ListGet        (PCSTR, PSDATA);
      STDMETHODIMP ListRemove     (PCSTR, SDATA);
      STDMETHODIMP ListSet        (PCSTR, SDATA);
      STDMETHODIMP ListQuery      (PCSTR, BOOL *);
   };

typedef CGrmISRGramCFGA * PCGrmISRGramCFGA;



// CGrm class - Grammar

class CGrm : public IUnknown {
   // interfaces are friends
   friend class CGrmISRGramCFGW;
   friend class CGrmISRGramCommonW;
   friend class CMode;

   protected:
      ULONG          m_cRef;     // reference count (# interfaces opened)
      LPUNKNOWN      m_punkOuter;   // controlling unknown for aggregation
      LPFNDESTROYED  m_pfnDestroy;  // function call on closure
      VOID*          m_pCMode;   // SR mode which grammar uses
      PCGrmISRGramCommonW m_pGrmISRGramCommonW; // Grammar class
      PCGrmISRGramCFGW  m_pGrmISRGramCFGW;  // IIdentity class
      PCList         m_pList;       // List containing grammar words
      DWORD          m_dwGramActive;   // number of active grammars
      QWORD          m_qwBegin, m_qwEnd;   // begin & end of utterance
      PISRGRAMNOTIFYSINKW m_NotifyW;   // notification sink
      BOOL            m_fHooked;      // TRUE if have have done AddRef() to ISRCentral

      void Utterance (QWORD qTimeBegin, QWORD qTimeEnd);        // An utterance was completed

   public:
      CGrm (LPUNKNOWN, LPFNDESTROYED, void *);
      ~CGrm (void);
      BOOL FInit (SRGRMFMT, SDATA, PISRGRAMNOTIFYSINKW);

      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      HWND           hWndMain;      // main window
      HWND           hWndEdit;      // edit control window
      BOOL RecognizeFromString (PSTR pStr);
      BOOL           m_bActive;     // TRUE if is active
   };
   
typedef CGrm * PCGrm;



/************************************************************************
SR-mode object
*/

/* Mode notification object */
class CModeNotify : public IAudioSourceNotifySink {
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

      // ISRNotifySink
      STDMETHODIMP AudioStop      (WORD);
      STDMETHODIMP AudioStart      (void);
      STDMETHODIMP DataAvailable  (DWORD, BOOL);
      STDMETHODIMP Overflow       (DWORD);
   };
typedef CModeNotify * PCModeNotify;



// IAttributes interface for engines

class CModeISRAttributesW : public ISRAttributesW {

   private:

      ULONG          m_cRef;        // interface reference count
      LPVOID         m_pObj;        // Back pointer to the object
      LPUNKNOWN      m_punkOuter;   // Controlling unknown for delegation

   public:

      CModeISRAttributesW (LPVOID, LPUNKNOWN);
      ~CModeISRAttributesW (void);

      FInit(LPVOID, LPUNKNOWN);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ISRAttributes members

      STDMETHODIMP AutoGainEnableGet (DWORD *);
      STDMETHODIMP AutoGainEnableSet (DWORD);
      STDMETHODIMP EchoGet           (BOOL *);
      STDMETHODIMP EchoSet           (BOOL);
      STDMETHODIMP EnergyFloorGet    (WORD *);
      STDMETHODIMP EnergyFloorSet    (WORD);
      STDMETHODIMP MicrophoneGet     (PWSTR, DWORD, DWORD *);
      STDMETHODIMP MicrophoneSet     (PCWSTR);
      STDMETHODIMP RealTimeGet       (DWORD *);
      STDMETHODIMP RealTimeSet       (DWORD);
      STDMETHODIMP SpeakerGet        (PWSTR, DWORD, DWORD *);
      STDMETHODIMP SpeakerSet        (PCWSTR);
      STDMETHODIMP TimeOutGet        (DWORD *, DWORD *);
      STDMETHODIMP TimeOutSet        (DWORD, DWORD);
      STDMETHODIMP ThresholdGet      (DWORD *);
      STDMETHODIMP ThresholdSet      (DWORD);
   };

typedef CModeISRAttributesW * PCModeISRAttributesW;


class CModeISRAttributesA : public ISRAttributesA {

   private:

      LPVOID         m_pObjA;        // Back pointer to the (A..) object

   public:

      CModeISRAttributesA (LPVOID, LPUNKNOWN);
      ~CModeISRAttributesA (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ISRAttributes members

      STDMETHODIMP AutoGainEnableGet (DWORD *);
      STDMETHODIMP AutoGainEnableSet (DWORD);
      STDMETHODIMP EchoGet           (BOOL *);
      STDMETHODIMP EchoSet           (BOOL);
      STDMETHODIMP EnergyFloorGet    (WORD *);
      STDMETHODIMP EnergyFloorSet    (WORD);
      STDMETHODIMP MicrophoneGet     (PSTR, DWORD, DWORD *);
      STDMETHODIMP MicrophoneSet     (PCSTR);
      STDMETHODIMP RealTimeGet       (DWORD *);
      STDMETHODIMP RealTimeSet       (DWORD);
      STDMETHODIMP SpeakerGet        (PSTR, DWORD, DWORD *);
      STDMETHODIMP SpeakerSet        (PCSTR);
      STDMETHODIMP TimeOutGet        (DWORD *, DWORD *);
      STDMETHODIMP TimeOutSet        (DWORD, DWORD);
      STDMETHODIMP ThresholdGet      (DWORD *);
      STDMETHODIMP ThresholdSet      (DWORD);
   };

typedef CModeISRAttributesA * PCModeISRAttributesA;



// ISRCentral interface for engines

class CModeISRCentralW : public ISRCentralW {

   private:

      ULONG          m_cRef;        // interface reference count
      LPVOID         m_pObj;        // Back pointer to the object
      LPUNKNOWN      m_punkOuter;   // Controlling unknown for delegation

   public:

      CModeISRCentralW (LPVOID, LPUNKNOWN);
      ~CModeISRCentralW (void);

      FInit(LPVOID, LPUNKNOWN);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ISRCentral members

      STDMETHODIMP ModeGet        (PSRMODEINFOW);
      STDMETHODIMP GrammarLoad    (SRGRMFMT, SDATA, PVOID, IID, LPUNKNOWN *);
      STDMETHODIMP Pause          (void);
      STDMETHODIMP PosnGet        (PQWORD);
      STDMETHODIMP Resume         (void);
      STDMETHODIMP ToFileTime     (PQWORD, FILETIME *);
      STDMETHODIMP Register       (PVOID, IID, DWORD*);
      STDMETHODIMP UnRegister     (DWORD);
   };

typedef CModeISRCentralW * PCModeISRCentralW;


class CModeISRCentralA : public ISRCentralA {

   private:

      LPVOID         m_pObjA;        // Back pointer to the (A..) object

   public:

      CModeISRCentralA (LPVOID, LPUNKNOWN);
      ~CModeISRCentralA (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ISRCentral members

      STDMETHODIMP ModeGet        (PSRMODEINFOA);
      STDMETHODIMP GrammarLoad    (SRGRMFMT, SDATA, PVOID, IID, LPUNKNOWN *);
      STDMETHODIMP Pause          (void);
      STDMETHODIMP PosnGet        (PQWORD);
      STDMETHODIMP Resume         (void);
      STDMETHODIMP ToFileTime     (PQWORD, FILETIME *);
      STDMETHODIMP Register       (PVOID, IID, DWORD*);
      STDMETHODIMP UnRegister     (DWORD);
   };

typedef CModeISRCentralA * PCModeISRCentralA;



// ISRDialogs interface for engines

class CModeISRDialogsW : public ISRDialogsW {

   private:

      ULONG          m_cRef;        // interface reference count
      LPVOID         m_pObj;        // Back pointer to the object
      LPUNKNOWN      m_punkOuter;   // Controlling unknown for delegation

   public:

      CModeISRDialogsW (LPVOID, LPUNKNOWN);
      ~CModeISRDialogsW (void);

      FInit(LPVOID, LPUNKNOWN);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ISRDialogs members

      STDMETHODIMP AboutDlg (HWND, PCWSTR);
      STDMETHODIMP GeneralDlg (HWND, PCWSTR);
      STDMETHODIMP LexiconDlg (HWND, PCWSTR);
      STDMETHODIMP TrainMicDlg (HWND, PCWSTR);
      STDMETHODIMP TrainGeneralDlg (HWND, PCWSTR);
   };

typedef CModeISRDialogsW * PCModeISRDialogsW;


class CModeISRDialogsA : public ISRDialogsA {

   private:

      LPVOID         m_pObjA;        // Back pointer to the (A..) object

   public:

      CModeISRDialogsA (LPVOID, LPUNKNOWN);
      ~CModeISRDialogsA (void);

// IUnkown members that delegate to m_punkOuter
// Non-delegating object IUnknown

      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

// ISRDialogs members

      STDMETHODIMP AboutDlg        (HWND, PCSTR);
      STDMETHODIMP GeneralDlg      (HWND, PCSTR);
      STDMETHODIMP LexiconDlg      (HWND, PCSTR);
      STDMETHODIMP TrainMicDlg     (HWND, PCSTR);
      STDMETHODIMP TrainGeneralDlg (HWND, PCSTR);
   };

typedef CModeISRDialogsA * PCModeISRDialogsA;



// CMode class - SR engine mode

class CMode : public IUnknown {

// interfaces are friends

   friend class CModeISRAttributesW;
   friend class CModeISRAttributesA;
   friend class CModeISRCentralW;
   friend class CModeISRCentralA;
   friend class CModeISRDialogsW;
   friend class CModeISRDialogsA;
   friend class CGrm;
   friend class CGrmISRGramCommonW;
   friend class CGrmISRGramCommonA;
   friend class CGrmISRGramCRGW;
   friend class CGrmISRGramCRGA;
   friend class CModeNotify;
   friend class CEngISREnumW;
   friend class CEngISREnumA;

   protected:

      ULONG          m_cRef;     // reference count (# interfaces opened)
      LPUNKNOWN      m_punkOuter;   // controlling unknown for aggregation
      LPFNDESTROYED  m_pfnDestroy;  // function call on closure
      PCModeISRAttributesW m_pModeISRAttributesW;  // Attributes class
      PCModeISRAttributesA m_pModeISRAttributesA;  // Attributes class
      PCModeISRCentralW    m_pModeISRCentralW;     // Central control class
      PCModeISRCentralA    m_pModeISRCentralA;     // Central control class
      PCModeISRDialogsW    m_pModeISRDialogsW;     // Dialogs class
      PCModeISRDialogsA    m_pModeISRDialogsA;     // Dialogs class
      LPUNKNOWN            m_pAudioIUnknown;       // unknown for the audio
      PIAUDIO              m_pAudioIAudio;         // to get audio information from
      PIAUDIOSOURCE        m_pAudioIAudioSrc;      // to get audio information from
      DWORD          m_dwAutoGain;        // auto gain parameter
      DWORD          m_dwConfidence;      // confidence parameter (threshold)
      BOOL           m_fEcho;             // Echo cancellation on/off
      WORD           m_wFloor;            // Energy floor
      PSTR           m_pstrMicrophone;    // Microphone
      DWORD          m_dwRealTime;        // real time performance
      PSTR           m_pstrSpeaker;       // Speaker
      DWORD          m_dwTimeOutCompl;    // Time out complete
      DWORD          m_dwTimeOutIncompl;  // Time out incomplete
      PCList         m_pList;             // list of grammars
      PCModeNotify   m_pNotify;           // audio-source notification

      BOOL           m_fUtterance;        // TRUE if an utterance is taking place
      QWORD          m_qwUttStart;        // when utterance started

      SRMODEINFOW    m_SRModeInfo;        // Stores the selected mode

      void Output (PSTR);                 // Output a string to the edit window
      void GrammarRelease (PCGrm);        // Grammar calls to indicate that is releasing
      STDMETHODIMP DataAvailable  (DWORD, BOOL);

   public:

      CMode (LPUNKNOWN, LPFNDESTROYED,
         LPUNKNOWN);
          // NOTE: Need more stuff here
      ~CMode (void);

      BOOL FInit (void);

      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      HWND           hWndMain;      // main window
      HWND           hWndEdit;      // edit control window
      PCList         m_pNotifyList;
   };

typedef CMode * PCMode;
