/* frame.h - Header files to access framework functionality */

#ifndef _FRAME_H_
#define _FRAME_H_


/********** some useful macros **********/
// use this macro to make sure both the SR engine & TTS engine can reroute audio
#define  NEEDAUDIO      if ((gTTSModeInfo.dwFeatures & TTSFEATURE_FIXEDAUDIO) || !gpWFEXTTS || \
   (gSRModeInfo.dwFeatures & SRFEATURE_FIXEDAUDIO) || !gpWFEXSR \
   ) {NotApplicable(); return;}

//use this to make sure the SR engine supports CFGs
#define  NEEDCFG        if (!(gSRModeInfo.dwGrammars & SRGRAM_CFG) || \
   (gSRModeInfo.dwSequencing != SRSEQUENCE_CONTINUOUS) \
   ) {Warning("This test needs to rout audio from TTS to SR to complete."); return;}

//use this to make sure the SR engine supports continuous/discrete dictation
#define  NEEDDICTATION  if (!(gSRModeInfo.dwGrammars & SRGRAM_DICTATION) \
   ) {NotApplicable(); return;}

//use this to make sure the SR engine supports continuous/discrete dictation
#define  NEEDLIMITEDDOMAIN  if (!(gSRModeInfo.dwGrammars & SRGRAM_LIMITEDDOMAIN) \
   ) {NotApplicable(); return;}

//use this to make sure the SR engine supports CFGs
#define  NEEDCONTINUOUS if (gSRModeInfo.dwSequencing != SRSEQUENCE_CONTINUOUS) \
   {NotApplicable(); return;}


// use this to require english
#define  NEEDENGLISH    if ((PRIMARYLANGID(gSRModeInfo.language.LanguageID) != LANG_ENGLISH) || \
   (PRIMARYLANGID(gTTSModeInfo.language.LanguageID) != LANG_ENGLISH) ) \
   {Warning("Since this test uses English, the English must be translated before the test will run."); return;}

// need a ttsfeature
#define  NEEDTTSFEATURE(x)      if (!(gTTSModeInfo.dwFeatures & (x)) \
   ) {NotApplicable(); return;}

// need a srfeature
#define  NEEDSRFEATURE(x)       if (!(gSRModeInfo.dwFeatures & (x)) \
   ) {NotApplicable(); return;}

// need a srinterface
#define  NEEDSRINTERFACE(x)       if (!(gSRModeInfo.dwInterfaces & (x)) \
   ) {NotApplicable(); return;}

// need a srinterface
#define  NEEDTTSINTERFACE(x)       if (!(gTTSModeInfo.dwInterfaces & (x)) \
   ) {NotApplicable(); return;}


// report an error and return, if there's an error
#define  CHECKFAILED(x)  if (hRes) {Failed(x); return;}


#define WCTMB(x,y)   WideCharToMultiByte (CP_ACP, 0, (x), -1, (y), sizeof(y), 0 ,0);
#define MBTWC(x,y)   MultiByteToWideChar (CP_ACP, 0, (x), -1, (y), sizeof(y) / 2);

extern HINSTANCE	ghInstance;

extern PCSREnum       gpSREnum;
extern PCTTSEnum      gpTTSEnum;

typedef void (CALLBACK* FUNCTESTPROC)(void);

#define TEST_REQUIRED			0x0001
#define TEST_OPTIONAL			0x0002

#define QUERY_READ		FALSE
#define QUERY_WRITE		TRUE


typedef struct {
   char           *pszTestName;
   char           *pszTestDescription;
	DWORD				dwTestType;
   FUNCTESTPROC   pFunction;
} FUNCTEST, *PFUNCTEST;


class CFuncSRNotifySink : public ISRNotifySinkW {
   public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ISRNotifySink
      STDMETHODIMP AttribChanged  (DWORD);
      STDMETHODIMP Interference   (QWORD, QWORD, DWORD);
      STDMETHODIMP Sound          (QWORD, QWORD);
      STDMETHODIMP UtteranceBegin (QWORD);
      STDMETHODIMP UtteranceEnd   (QWORD, QWORD);
      STDMETHODIMP VUMeter        (QWORD, WORD);
      STDMETHODIMP Training		 (DWORD);
      STDMETHODIMP Warning        (LPUNKNOWN);
      STDMETHODIMP Error	       (LPUNKNOWN);

      BOOL        m_fAttribChanged;
      DWORD       m_dwAttribChangedAttrib;
      BOOL        m_fInterference;
      DWORD       m_dwInterferenceValues;
      BOOL        m_fSound;
      BOOL        m_fUtteranceBegin;
      BOOL        m_fUtteranceEnd;
      BOOL        m_fVUMeter;
      WORD        m_wVUMeterValue;
      QWORD       m_qwVUMeterTime;
      DWORD       m_dwRefCount;
      BOOL        m_fNotification;
      QWORD       m_qwUtteranceEndStart;
      QWORD       m_qwUtteranceEndEnd;
      QWORD       m_qwUtteranceBeginTime;
		BOOL			m_fTraining;
		BOOL			m_fWarning;
		BOOL			m_fError;

};


class CFuncGramNotifySink: public ISRGramNotifySinkW {
   public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ISRNotifySink
	   STDMETHODIMP BookMark       (DWORD);
	   STDMETHODIMP Paused         (void);
	   STDMETHODIMP PhraseFinish   (DWORD, QWORD, QWORD, PSRPHRASEW, LPUNKNOWN);
	   STDMETHODIMP PhraseHypothesis(DWORD, QWORD, QWORD, PSRPHRASEW, LPUNKNOWN);
	   STDMETHODIMP PhraseStart    (QWORD);
	   STDMETHODIMP ReEvaluate     (LPUNKNOWN);
	   STDMETHODIMP Training       (DWORD);
	   STDMETHODIMP UnArchive      (LPUNKNOWN);

      BOOL        m_fBookMark;
      DWORD       m_dwBookMarkMark;
      BOOL        m_fPaused;
      BOOL        m_fPhraseFinish;
      BOOL        m_fPhraseHypothesis;
      BOOL        m_fPhraseStart;
      BOOL        m_fReEvaluate;
      BOOL        m_fTraining;
      BOOL        m_fUnArchive;
      DWORD       m_dwPhraseFinishFlags;
      char        m_szPhraseFinishText[1000];
      DWORD       m_abPhraseFinishWordNum[100];
      DWORD       m_dwPhraseFinishNumWords;
      QWORD       m_qwPhraseFinishStart;
      QWORD       m_qwPhraseFinishEnd;
      BYTE        m_abPhraseFinishPhrase[10000];
      QWORD       m_qwPhraseStartTime;
      QWORD       m_qwPhraseHypothesisStart;
      QWORD       m_qwPhraseHypothesisEnd;
      DWORD       m_dwRefCount;
      BOOL        m_fNotification;
};

class CFuncSRWaveAudioNotifySink : public IAudioFileNotifySink {
public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // IAudioSourceNotifySink members
      STDMETHODIMP FileBegin      (DWORD);
      STDMETHODIMP FileEnd        (DWORD);
      STDMETHODIMP QueueEmpty     (void);
      STDMETHODIMP Posn           (QWORD, QWORD);

      BOOL        m_fFileBegin;
      BOOL        m_fFileEnd;
      BOOL        m_fQueueEmpty;
      BOOL        m_fPosn;
      DWORD       m_dwRefCount;
      BOOL        m_fNotification;
      QWORD       m_qwPosn;
};

class CFuncTTSWaveAudioNotifySink : public IAudioFileNotifySink {
public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // IAudioSourceNotifySink members
      STDMETHODIMP FileBegin      (DWORD);
      STDMETHODIMP FileEnd        (DWORD);
      STDMETHODIMP QueueEmpty     (void);
      STDMETHODIMP Posn           (QWORD, QWORD);

      BOOL        m_fFileBegin;
      BOOL        m_fFileEnd;
      BOOL        m_fQueueEmpty;
      BOOL        m_fPosn;
      DWORD       m_dwRefCount;
      BOOL        m_fNotification;
      QWORD       m_qwPosn;
};

class CFuncTTSNotifySink : public ITTSNotifySinkW {
   public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ITTSNotifySink
      STDMETHODIMP AttribChanged  (DWORD);
      STDMETHODIMP AudioStart (QWORD);
      STDMETHODIMP AudioStop (QWORD);
      STDMETHODIMP Visual (QWORD, WCHAR, WCHAR, DWORD, PTTSMOUTH);

      BOOL        m_fAttribChanged;
      DWORD       m_dwAttribChangedAttrib;
      BOOL        m_fAudioStart;
      BOOL        m_fAudioStop;
      BOOL        m_fVisual;
      TTSMOUTH    m_VisualMouth;
      WCHAR       m_cVisualIPA;
      DWORD       m_dwRefCount;
      BOOL        m_fNotification;
};

class CFuncTTSBufNotifySink: public ITTSBufNotifySinkW {
   public:
      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ITTSNotifySink
      STDMETHODIMP TextDataDone (QWORD, DWORD);
      STDMETHODIMP TextDataStarted (QWORD);
      STDMETHODIMP BookMark (QWORD, DWORD);  
      STDMETHODIMP WordPosition (QWORD, DWORD);

      BOOL        m_fTextDataDone;
      BOOL        m_fTextDataStarted;
      BOOL        m_fBookMark;
      DWORD       m_dwBookMarkMark;
      BOOL        m_fWordPosition;
      DWORD       m_dwWordPositionVal;
      DWORD       m_dwRefCount;
      BOOL        m_fNotification;
};


extern BOOL           gfAbortTest;
extern HWND		ghWndEditor;

// sr
extern CSRMode        *gpSRMode;
extern CFuncSRWaveAudioNotifySink gSRWANS;
extern WAVEFORMATEX   *gpWFEXSR;  // wave format used by SR
extern DWORD          gdwWFEXSRSize;   // wave format used by SR
extern CFuncSRNotifySink gSRNS;   // sr notification sink
extern CFuncGramNotifySink gSRGramNS;   // sr notification sink
extern CFuncGramNotifySink gSRGramNSSecondary;   // sr notification sink
extern DWORD          gdwSRNSKey; // notification sinc key
extern SRMODEINFOW    gSRModeInfo;
extern PIAUDIOFILE    gpIAudioFileSR;
extern CSRGramComp    *gpSRGramComp;
extern CSRGramComp    *gpSRGramCompSecondary;
extern CSRResult      *gpSRResult;   // last result object from main gramcomp

// tts
extern CTTSMode       *gpTTSMode;
extern CFuncTTSWaveAudioNotifySink gTTSWANS;
extern WAVEFORMATEX   *gpWFEXTTS;  // wave format used by TTS
extern DWORD          gdwWFEXTTSSize;   // wave format used by TTS
extern CFuncTTSNotifySink gTTSNS;   // TTS notification sink
extern DWORD          gdwTTSNSKey; // notification sinc key
extern TTSMODEINFOW   gTTSModeInfo;
extern CFuncTTSBufNotifySink gTTSBufNS; // tts buffer notify sink
extern PIAUDIOFILE    gpIAudioFileTTS;


void Information (char *psz = NULL, HRESULT hRes = (HRESULT)-1, BOOL fCR = TRUE);
void Information(char *psz, BOOL fCR);
void Failed (char *psz = NULL, HRESULT hRes = (HRESULT)-1, BOOL fCR = TRUE);
void NotApplicable (char *psz = NULL, HRESULT hRes = (HRESULT)-1, BOOL fCR = TRUE);
void Passed (char *psz = NULL, HRESULT hRes = (HRESULT)-1, BOOL fCR = TRUE);
void Warning (char *psz = NULL, HRESULT hRes = (HRESULT)-1, BOOL fCR = TRUE);
PVOID ConstructList (WCHAR *psz, DWORD *pdwSize);
HRESULT CreateSREngine (void);
HRESULT CreateTTSEngine (void);
HRESULT DestroySREngine (void);
HRESULT DestroyTTSEngine (void);
BOOL *WaitForEvent (float fTimeOut, BOOL *pf1 = NULL, BOOL *pf2 = NULL, BOOL *pf3 = NULL,
                    BOOL *pf4 = NULL, BOOL *pf5 = NULL);
BOOL *WaitForEvent (BOOL *pf1 = NULL, BOOL *pf2 = NULL, BOOL *pf3 = NULL,
                    BOOL *pf4 = NULL, BOOL *pf5 = NULL);
HRESULT GenerateWaveName (char *psz = NULL);
HRESULT SpeakToSR (char *pszSpeak, BOOL fSilence = TRUE,
                   BOOL fIPA = FALSE, BOOL fAddNoise = TRUE);
HRESULT SpeakToWave (char *pszSpeak, char *pszFile = NULL,
                     DWORD *pdwUsed = NULL, BOOL *pfAbort = NULL);
HRESULT SpeakToWave (WCHAR *pszSpeak, char *pszFile = NULL,
                     DWORD *pdwUsed = NULL, BOOL *pfAbort = NULL,
                     BOOL fIPA = FALSE);
char *SpeakAndListen (char*pszSpeak, char*pszGram, BOOL fFailIfTimeOut = TRUE);
HRESULT VerifyRecognition (char*pszSpeak, char*pszGram);
HRESULT SetMainGrammar (PCWSTR psz);
HRESULT SetMainGrammar (PCSTR psz);
HRESULT SetSecondaryGrammar (PCSTR psz);
HRESULT SetMainGrammar (DWORD dwRes);
void ClearMainGrammar (void);
void ClearSecondaryGrammar (void);
void ClearSRAudio (void);
HRESULT WaitForPhraseFinish (HWND hWnd = NULL);
void CleanSlate (void);
PCSRResult GetResultsObject (char *pszSpeak = NULL, char *pszGrammar = NULL);
PVOID ConstructPhrase (WCHAR *psz, DWORD *pdwSize);

/******************************************************************
Externs that must be supplied elsewhere for framework to function */
extern char gszAppName[];
extern BOOL gfNeedAudioSREngine;
extern FUNCTEST gaTest[];
extern DWORD gdwTestElems;

#endif // _FRAME_H_