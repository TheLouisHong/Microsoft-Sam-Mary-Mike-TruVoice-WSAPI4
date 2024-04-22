/******************************************************************************\
*       srfunc.h
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/

#include "gramfunc.h"

typedef struct
{
	LPUNKNOWN lpResult;           // result object pointer
	TCHAR     szDesc[MAX_STRLEN]; // description appearing in results listbox
} Result_t, *PResult_t;

typedef struct
{
	TCHAR szName[MAX_STRLEN]; // description appearing in results listbox
} CFGName_t, *PCFGName_t;

class CISREngineObject;
class CISRGrammarObject;
class CISREnumObject;

typedef CISREngineObject ISRENGOBJ, *PISRENGOBJ;
typedef CISREnumObject ISRENUMOBJ, *PISRENUMOBJ;
typedef CISRGrammarObject ISRGRAMOBJ, *PISRGRAMOBJ;


/************************************************************************
Notification object */
class CEngNotify : public ISRNotifySink
{
   private:

   public:
      CEngNotify( CISREngineObject *pEngObj );
      ~CEngNotify();

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef();
      STDMETHODIMP_(ULONG) Release();

      // ISRNotifySink
	   STDMETHODIMP AttribChanged  (DWORD);
	   STDMETHODIMP Interference   (QWORD, QWORD, DWORD);
	   STDMETHODIMP Sound          (QWORD, QWORD);
	   STDMETHODIMP UtteranceBegin (QWORD);
	   STDMETHODIMP UtteranceEnd   (QWORD, QWORD);
	   STDMETHODIMP VUMeter        (QWORD, WORD);

protected:
       CISREngineObject *m_pEngObj;
       DWORD             m_dwRefCnt;
};

typedef CEngNotify * PCEngNotify;

class CGramNotify : public ISRGramNotifySink
{
   public:
      CGramNotify( CISRGrammarObject *pGramObj, BOOL bUseShare );
      ~CGramNotify();

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef();
      STDMETHODIMP_(ULONG) Release();

      // ISRNotifySink
	   STDMETHODIMP BookMark         (DWORD);
	   STDMETHODIMP Paused           ();
	   STDMETHODIMP PhraseFinish     (DWORD, QWORD, QWORD, PSRPHRASE, LPUNKNOWN);
	   STDMETHODIMP PhraseHypothesis (DWORD, QWORD, QWORD, PSRPHRASE, LPUNKNOWN);
	   STDMETHODIMP PhraseStart      (QWORD);
	   STDMETHODIMP ReEvaluate       (LPUNKNOWN);
	   STDMETHODIMP Training         (DWORD);
	   STDMETHODIMP UnArchive        (LPUNKNOWN);

	   CISRGrammarObject *m_pGramObj;      // pointer to grammar object

protected:
       DWORD m_dwRefCnt;
	   BOOL	 m_bUseShare;
};

typedef CGramNotify * PCGramNotify;

class CISRGrammarObject
{
public:
	CISRGrammarObject( BOOL bUseShare );
	~CISRGrammarObject();
	int GrammarInit( PISRCENTRAL pISRCentral, PTCHAR szFilename, BOOL bArchive );
	int GrammarReadFile(PTCHAR lpFilename, PSDATA pData);

	BOOL EnumCFGExportedRules();
	BOOL EnumCFGLists();

	TCHAR             m_szName[MAX_NAMELEN]; // grammar file name (path not included)
	PISRGRAMCOMMON    m_pISRGramCommon;      // grammar interface ptr
	PISRGRAMCFG       m_pISRGramCFG;         // grammar cfg interface ptr
	PISRGRAMDICTATION m_pISRGramDict;        // grammar dictation interface ptr
	PCGramNotify      m_pGramNotify;         // grammar notification object ptr
	ISRGramCFG_t      m_Cfg;                 // cfg data
	ISRGramDict_t     m_Dict;                // dictation data
	ISRGramNotFlag_t  m_Not;                 // notification filter flags
	BOOL              m_bActive;             // grammar active status flag

	BOOL			  m_bUseShare;			 // Is the engine marshalled through the sharing object?
	// m_bUseShare determines what kind of NotifySInk is needed.

	HGRAMMAR          m_hGramInfo;           // handle for extra grammar info
	int               m_szListNameIndex;     // currently selected grammar list name
	SDATA             m_Archive;             // archived data buffer

	DWORD             m_NumResults;          // number of results object pointers in list
	CList<PResult_t, PResult_t>  m_ResList;  // list of results object pointers
	CPtrList          m_RuleList;            // list of exported grammar rule names
	CPtrList          m_ActiveRules;         // list of active grammar rules
	CPtrList          m_GramList;            // list of grammar list names
};

class CISREngineObject
{
public:
	CISREngineObject();
	~CISREngineObject();

	BOOL CISREngineObject::FInit( PSRMODEINFO pModeInfo );
	BOOL CISREngineObject::SelectMode( PISRENUMOBJ pSREnum );
	int CreateGrammarObject( PTCHAR szFileName, BOOL bArchive );
	void DeleteGrammarObject( PISRGRAMOBJ pGram );
	void DeleteAllGrammars();

	PISRCENTRAL       m_pISRCentral;     // engine interface
	PISRATTRIBUTES    m_pISRAttributes;  // attributes
	PISRDIALOGS       m_pISRDialogs;     // dialogs
	PISRDIALOGS2      m_pISRDialogs2;     // dialogs
	PCEngNotify       m_pEngNotify;      // notification object for engine
	DWORD             m_dwKey;           // key to free notification

	PIENUMSRSHARE     m_pSRShare;        // mode share interface
	QWORD             m_qwInstID;        // SRShare instance ID
	BOOL              m_bUseShare;       // use sharing for mode
	BOOL              m_bFindFlag;       // we used find interface to obtain this mode
	BOOL              m_bPaused;         // engine paused status flag
	ISRAttr_t		  m_Attr;            // engine attribute data
	ISREngNotFlag_t   m_Not;             // notification status flags
	SRMODEINFO        m_ModeInfo;        // mode information
	BOOL              m_bHasArchive;     // TRUE if this mode supports the archive function

	DWORD             m_DeviceID;        // wave device ID (if not sharing)
	int               m_nLoadedGrams;    // the number of loaded grammars
	PISRGRAMOBJ       m_pGram;           // pointer to current grammar object
	CList<PISRGRAMOBJ, PISRGRAMOBJ>	m_GramList;   // list of loaded grammar objects
};

class CISREnumObject
{

friend CISREngineObject;

public:
	CISREnumObject();
	~CISREnumObject();

	BOOL FInit();
	BOOL TerminateSR();
	void EngModeFind( PSRMODEINFO pSRInfo, PSRMODEINFO pSRSelect );
	BOOL EngModeEnum( PSRMODEINFO *pSRInfoList, DWORD *dwNumFound );
	BOOL m_bFindFlag;           // indicates if ISRFind was used

protected:
	PISRENUM      m_pISREnum;   // mode enumerator interface
	PISRFIND      m_pISRFind;   // mode find interface
};


extern ISRENUMOBJ SREnum;   // encapsulated enumerator object
extern PISRENGOBJ gpEng;    // global pointer to current engine object


// helper funtions...
void MainOutput( PTCHAR msg );
int CheckError( HRESULT hRes, PTCHAR prestr );
void DisplayModeInfo( PSRMODEINFO pSRInfo );
void Phrase2Str( PSRPHRASE pSRPhrase, char *buf, BOOL fUnicode );
BOOL Str2Phrase(TCHAR *buf, PSRPHRASE *ppSRPhrase);
void Word2Str( PSRWORDA pSRWordA, char *buf );
void Word2Str( PSRWORDW pSRWordW, PWCHAR bufW );
void Word2Str( PSRWORDW pSRWordW, char *buf );
void Word2Str( PSRWORDA pSRWordA, PWCHAR bufW );
