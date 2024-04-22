/******************************************************************************\
*       tabisrce.h : header file
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CTabISRCentral dialog

class CTabISRCentral : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRCentral)

// Construction
public:
	CTabISRCentral();
	~CTabISRCentral();
	virtual BOOL OnSetActive();

	void EnumGrammars( void );
	void CALLBACK EXPORT OnPositionget();
	void SetPosTimer( int flag );

	BOOL m_bPosTracking;    // positon tracking flag

// Dialog Data
	//{{AFX_DATA(CTabISRCentral)
	enum { IDD = IDD_TABISRCENTRAL };
	CButton	m_UseArchive;
	CStatic	m_ToFileOut;
	CButton	m_PosTrack;
	CButton	m_EngPaused;
	CStatic	m_PosGetOutput;
	CListBox	m_EnumGram;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRCentral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRCentral)
	afx_msg void OnCentgramload();
	afx_msg void OnDblclkListgrammars();
	afx_msg void OnCentgramreleaseall();
	afx_msg void OnEngctlpause();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPositiontrack();
	afx_msg void OnCentmodeget();
	afx_msg void OnGramrescan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CTabISRGrammar dialog

class CTabISRGrammar : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRGrammar)

	friend class CSRTestDlg;

// Construction
public:
	CTabISRGrammar();
	~CTabISRGrammar();
	virtual BOOL OnSetActive();

// Dialog Data
	//{{AFX_DATA(CTabISRGrammar)
	enum { IDD = IDD_TABISRGRAMMAR };
	CListBox	m_RuleActiveList;
	CComboBox	m_GramWnd;
	CComboBox	m_GramAutoPause;
	CComboBox	m_GramRuleList;
	CEdit	m_BookMarkEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRGrammar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRGrammar)
	afx_msg void OnGramtrain();
	afx_msg void OnGramtrainphrase();
	afx_msg void OnGramtrainquery();
	afx_msg void OnGramdeteriorget();
	afx_msg void OnGrambookmark();
	afx_msg void OnGramdeteriorset();
	virtual BOOL OnInitDialog();
	afx_msg void OnGramarchive();
	afx_msg void OnGramactivate();
	afx_msg void OnGramdeactivategram();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRGramCFG dialog

class CTabISRGramCFG : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRGramCFG)

// Construction
public:
	CTabISRGramCFG();
	~CTabISRGramCFG();
	virtual BOOL OnSetActive();

// Dialog Data
	//{{AFX_DATA(CTabISRGramCFG)
	enum { IDD = IDD_TABISRGRAMCFG };
	CComboBox	m_GramListName;
	CButton	m_GramListQuery;
	CListBox	m_GramOrigWords;
	CEdit	m_GramLinkName;
	CListBox	m_GramCFGWords;
	//}}AFX_DATA

	void OnGramcfglistget();
	void DisplayGramList( PTCHAR szListName );

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRGramCFG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRGramCFG)
	afx_msg void OnGramlinkquery();
	afx_msg void OnGramcfglistset();
	afx_msg void OnGramlistquery();
	afx_msg void OnGramcfglistremove();
	virtual BOOL OnInitDialog();
	afx_msg void OnGramcfglistappend();
	afx_msg void OnGramcustom();
	afx_msg void OnCloseupGramlistname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRGramDict dialog

class CTabISRGramDict : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRGramDict)

// Construction
public:
	CTabISRGramDict();
	~CTabISRGramDict();
	virtual BOOL OnSetActive();

// Dialog Data
	//{{AFX_DATA(CTabISRGramDict)
	enum { IDD = IDD_TABISRGRAMDICT };
	CEdit	m_GramLikelyWords;
	CEdit	m_GramBeforeEdit;
	CEdit	m_GramAfterEdit;
	CComboBox	m_GramDictHint;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRGramDict)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRGramDict)
	afx_msg void OnGramdictwords();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRAttr dialog

class CTabISRAttr : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRAttr)

// Construction
public:
	CTabISRAttr();
	~CTabISRAttr();
	virtual BOOL OnSetActive();

// Dialog Data
	//{{AFX_DATA(CTabISRAttr)
	enum { IDD = IDD_TABISRATTRIB };
	CEdit	m_Speaker;
	CButton	m_Echo;
	CEdit	m_TimeOutI;
	CEdit	m_TimeOutC;
	CEdit	m_CPU;
	CEdit	m_Mic;
	CEdit	m_EnergyFlr;
	CEdit	m_Thresh;
	CEdit	m_AttrGain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRAttr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRAttr)
	afx_msg void OnKillfocusAttrgain();
	afx_msg void OnKillfocusAttrthresh();
	afx_msg void OnKillfocusAttrenergy();
	afx_msg void OnKillfocusAttrmic();
	afx_msg void OnKillfocusAttrcpu();
	afx_msg void OnKillfocusAttrtimeouti();
	afx_msg void OnKillfocusAttrtimeoutc();
	afx_msg void OnAttribecho();
	afx_msg void OnKillfocusAttrspeaker();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRNotify dialog

class CTabISRNotify : public CPropertyPage
{
	friend class CSRTestDlg;

	DECLARE_DYNCREATE(CTabISRNotify)

// Construction
public:
	CTabISRNotify();
	~CTabISRNotify();
	virtual BOOL OnSetActive();

// Dialog Data
	//{{AFX_DATA(CTabISRNotify)
	enum { IDD = IDD_TABISRNOTIFY };
	CButton	m_Attr;
	CButton	m_VMeter;
	CButton	m_Utter;
	CButton	m_Sound;
	CButton	m_Interfere;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRNotify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRNotify)
	afx_msg void OnNotifyattr();
	afx_msg void OnNotifyinterfer();
	afx_msg void OnNotifysnd();
	afx_msg void OnNotifyuttr();
	afx_msg void OnNotifyvmeter();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRGramNotify dialog

class CTabISRGramNotify : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRGramNotify)

// Construction
public:
	CTabISRGramNotify();
	~CTabISRGramNotify();
	virtual BOOL OnSetActive();

// Dialog Data
	//{{AFX_DATA(CTabISRGramNotify)
	enum { IDD = IDD_ISRGRAMNOTIFY };
	CButton	m_Bookmark;
	CButton	m_Paused;
	CButton	m_Unarchive;
	CButton	m_ReEval;
	CButton	m_PhraseHyp;
	CButton	m_Phrase;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRGramNotify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRGramNotify)
	afx_msg void OnGramnotbookmark();
	afx_msg void OnGramnotpaused();
	afx_msg void OnGramnotphrase();
	afx_msg void OnGramnotphrashyp();
	afx_msg void OnGramnotreeval();
	afx_msg void OnGramnotunarc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRDialogs dialog

class CTabISRDialogs : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRDialogs)

// Construction
public:
	CTabISRDialogs();
	~CTabISRDialogs();
	virtual BOOL OnSetActive();

// Dialog Data
	//{{AFX_DATA(CTabISRDialogs)
	enum { IDD = IDD_TABISRDIALOGS };
	CEdit	m_TrainText;
	CButton	m_DlgTitleOpt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRDialogs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRDialogs)
	afx_msg void OnIsrdlggeneral();
	afx_msg void OnIsrdlglex();
	afx_msg void OnIsrdlgtraingen();
	afx_msg void OnIsrdlgtrainmic();
	afx_msg void OnIsrdlgabout();
	afx_msg void OnTrainphrase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRResAud dialog

class CTabISRResAud : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRResAud)

// Construction
public:
	CTabISRResAud();
	~CTabISRResAud();

// Dialog Data
	//{{AFX_DATA(CTabISRResAud)
	enum { IDD = IDD_TABISRRESAUDIO };
	CStatic	m_AudioFmtTxt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRResAud)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRResAud)
	afx_msg void OnAudiogetwave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRResBasic dialog

class CTabISRResBasic : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRResBasic)

// Construction
public:
	CTabISRResBasic();
	~CTabISRResBasic();
	virtual BOOL OnSetActive();

// Dialog Data
	//{{AFX_DATA(CTabISRResBasic)
	enum { IDD = IDD_TABISRRESBASIC };
	CStatic	m_PhraseGetTxt;
	CStatic	m_FlagsGetTxt;
	CStatic	m_TimeEndTxt;
	CStatic	m_TimeBeginTxt;
	CListBox	m_ResSelection;
	CStatic	m_ResIdentifyTxt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRResBasic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRResBasic)
	afx_msg void OnResidentify();
	afx_msg void OnRestimeget();
	afx_msg void OnKillfocusResobjselect();
	afx_msg void OnFlagsget();
	afx_msg void OnPhraseget();
	afx_msg void OnSelchangeResobjselect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CTabISRResCorr dialog

class CTabISRResCorr : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRResCorr)

// Construction
public:
	CTabISRResCorr();
	~CTabISRResCorr();
	virtual BOOL OnSetActive();

// Dialog Data
	//{{AFX_DATA(CTabISRResCorr)
	enum { IDD = IDD_TABISRRESCORR };
	CComboBox	m_CorrConfidList;
	CComboBox	m_CorrValidateList;
	CEdit	m_CorrPhraseEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRResCorr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRResCorr)
	afx_msg void OnCorrvalid();
	afx_msg void OnCorrcorr();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CTabISRResMem dialog

class CTabISRResMem : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRResMem)

// Construction
public:
	CTabISRResMem();
	~CTabISRResMem();

// Dialog Data
	//{{AFX_DATA(CTabISRResMem)
	enum { IDD = IDD_TABISRRESMEM };
	CStatic	m_MemGetTxt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRResMem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRResMem)
	afx_msg void OnMemget();
	afx_msg void OnLockset();
	afx_msg void OnLockget();
	afx_msg void OnMemfree();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRResGraph dialog

class CTabISRResGraph : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRResGraph)

// Construction
public:
	CTabISRResGraph();
	~CTabISRResGraph();

// Dialog Data
	//{{AFX_DATA(CTabISRResGraph)
	enum { IDD = IDD_TABISRRESGRAPH };
	CEdit	m_GraphOutput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRResGraph)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRResGraph)
	afx_msg void OnBestpathph();
	afx_msg void OnBestpathw();
	afx_msg void OnPathscorew();
	afx_msg void OnPathscoreph();
	afx_msg void OnGetphnode();
	afx_msg void OnGetwnode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRResMerge dialog

class CTabISRResMerge : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRResMerge)

// Construction
public:
	CTabISRResMerge();
	~CTabISRResMerge();
	virtual BOOL OnSetActive();

// Dialog Data
	//{{AFX_DATA(CTabISRResMerge)
	enum { IDD = IDD_TABISRRESMERGE };
	CStatic	m_ResMergeTxt;
	CEdit	m_ResSplitEdit;
	CStatic	m_ResSplitTxt;
	CComboBox	m_ResMergeSel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRResMerge)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRResMerge)
	afx_msg void OnResmerge();
	afx_msg void OnRessplit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRResModGui dialog

class CTabISRResModGui : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRResModGui)

// Construction
public:
	CTabISRResModGui();
	~CTabISRResModGui();

// Dialog Data
	//{{AFX_DATA(CTabISRResModGui)
	enum { IDD = IDD_TABISRRESMODGUI };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRResModGui)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRResModGui)
	afx_msg void OnModguihide();
	afx_msg void OnModguimove();
	afx_msg void OnModguishow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRResSpeak dialog

class CTabISRResSpeak : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRResSpeak)

// Construction
public:
	CTabISRResSpeak();
	~CTabISRResSpeak();

// Dialog Data
	//{{AFX_DATA(CTabISRResSpeak)
	enum { IDD = IDD_TABISRRESSpeaker };
	CComboBox	m_SpkrValConf;
	CComboBox	m_SpkrCorrConf;
	CEdit	m_CorrName;
	CStatic	m_SpkrIDTxt;
	CStatic	m_IDForFreeTxt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRResSpeak)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRResSpeak)
	afx_msg void OnSpkridentify();
	afx_msg void OnIdforfree();
	afx_msg void OnSpkrcorr();
	afx_msg void OnSpkrvalidate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRResEval dialog

class CTabISRResEval : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRResEval)

// Construction
public:
	CTabISRResEval();
	~CTabISRResEval();

// Dialog Data
	//{{AFX_DATA(CTabISRResEval)
	enum { IDD = IDD_TABISRRESEVAL };
	CStatic	m_ReEvalTxt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRResEval)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRResEval)
	afx_msg void OnReevaluate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabISRResScores dialog

class CTabISRResScores : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabISRResScores)

// Construction
public:
	CTabISRResScores();
	~CTabISRResScores();

// Dialog Data
	//{{AFX_DATA(CTabISRResScores)
	enum { IDD = IDD_TABISRRESSCORES };
	CStatic	m_WordScoreText;
	CStatic	m_PhraseScoreText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabISRResScores)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabISRResScores)
	afx_msg void OnGetwordscores();
	afx_msg void OnGetphrasescore();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
