/******************************************************************************\
*       tabdlgs.h
*
*       Header file for the CPropertyPage classes (tabs on main window)
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
// CTabIVoiceText dialog

class CVTTestDlg;

class CTabIVoiceText : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabIVoiceText)

// Construction
public:
	CTabIVoiceText();
	~CTabIVoiceText();

	void DoSpeakShortcut() { OnButtonSpeak();}

	CVTTestDlg* GetMainDlg() {return (CVTTestDlg*) AfxGetApp()->m_pMainWnd;}

// Dialog Data
	//{{AFX_DATA(CTabIVoiceText)
	enum { IDD = IDD_TABIVOICETEXT };
	CEdit	m_editTagData;
	CButton	m_buttonCheckPaused;
	CEdit	m_editSpeakData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabIVoiceText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabIVoiceText)
	afx_msg void OnButtonSpeak();
	afx_msg void OnButtonStopSpeaking();
	afx_msg void OnButtonHigh();
	afx_msg void OnButtonVeryHigh();
	afx_msg void OnButtonFastForward();
	afx_msg void OnButtonRewind();
	afx_msg void OnCheckPauseResume();
	afx_msg void OnButtonReleaseEnum();
	afx_msg void OnRegister();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabIVTxtAttributes dialog

class CTabIVTxtAttributes : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabIVTxtAttributes)

// Construction
public:
	CTabIVTxtAttributes();
	~CTabIVTxtAttributes();

	BOOL OnSetActive();

	CVTTestDlg* GetMainDlg() {return (CVTTestDlg*) AfxGetApp()->m_pMainWnd;}

// Dialog Data
	//{{AFX_DATA(CTabIVTxtAttributes)
	enum { IDD = IDD_TABIVTATTRIB };
	CEdit	m_editTTSMode;
	CEdit	m_editDeviceID;
	CEdit	m_editSpeed;
	CEdit	m_editIsSpeaking;
	CEdit	m_editEnabled;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabIVTxtAttributes)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabIVTxtAttributes)
	afx_msg void OnButtonIsSpeaking();
	afx_msg void OnButtonSpeedGet();
	afx_msg void OnButtonSpeedSet();
	afx_msg void OnButtonDeviceGet();
	afx_msg void OnButtonDeviceSet();
	afx_msg void OnButtonEnabledGet();
	afx_msg void OnButtonEnabledSet();
	afx_msg void OnButtonTTSModeGet();
	afx_msg void OnButtonSetSelectedMode();
	afx_msg void OnButtonSetBadMode();
	afx_msg void OnButtonSpeedMin();
	afx_msg void OnButtonSpeedMax();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabIVTxtNotifySink dialog

class CTabIVTxtNotifySink : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabIVTxtNotifySink)

// Construction
public:
	CTabIVTxtNotifySink();
	~CTabIVTxtNotifySink();

	CVTTestDlg* GetMainDlg() {return (CVTTestDlg*) AfxGetApp()->m_pMainWnd;}
	void NotifyOutput(LPCTSTR pLogStr);

	BOOL OnSetActive(); 

// Dialog Data
	//{{AFX_DATA(CTabIVTxtNotifySink)
	enum { IDD = IDD_TABIVTNOTIFY };
	CButton	m_visual;
	CButton	m_speakStart;
	CButton	m_speakDone;
	CButton	m_speak;
	CButton	m_attribChanged;
	CEdit	m_editNotifications;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabIVTxtNotifySink)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabIVTxtNotifySink)
	afx_msg void OnNotifyAttribChanged();
	afx_msg void OnNotifySpeak();
	afx_msg void OnNotifySpeakDone();
	afx_msg void OnNotifySpeakStart();
	afx_msg void OnNotifyVisual();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CTabIVTxtDialogs dialog

class CTabIVTxtDialogs : public CPropertyPage
{
	DECLARE_DYNCREATE(CTabIVTxtDialogs)

// Construction
public:
	CTabIVTxtDialogs();
	~CTabIVTxtDialogs();

	CVTTestDlg* GetMainDlg() {return (CVTTestDlg*) AfxGetApp()->m_pMainWnd;}

// Dialog Data
	//{{AFX_DATA(CTabIVTxtDialogs)
	enum { IDD = IDD_TABIVTDIALOGS };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTabIVTxtDialogs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTabIVTxtDialogs)
	afx_msg void OnVTGeneralDlg();
	afx_msg void OnVTLexiconDlg();
	afx_msg void OnVTTranslateDlg();
	afx_msg void OnVTAboutDlg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
