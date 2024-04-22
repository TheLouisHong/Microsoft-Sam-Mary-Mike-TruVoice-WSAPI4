/******************************************************************************\
*       srdlg.h : header file
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/
#include "tabisrce.h"

LPSTR StripPath( LPSTR p );
PWCHAR StripPath( PWCHAR pw );

/////////////////////////////////////////////////////////////////////////////
// CSRTestDlg dialog

class CSRTestDlg : public CDialog
{
	friend class CAddEngMode;

// Construction
public:
	CSRTestDlg(CWnd* pParent = NULL);	// standard constructor

	void MainOutput(PTCHAR str);
	void NotifyOutput(const PTCHAR str);
	void GramNotifyOutput(const PTCHAR str);
	void EnumGrammars();
	void InitPropPageSettingsEng();
	void InitPropPageSettingsGram();
	void InitPropPageSettingsRes();
	DWORD EnumModes( PSRMODEINFO *pSRInfoList );
	void CSRTestDlg::EnumGramFiles();

	CPropertySheet    *m_PropEng;
	CPropertySheet    *m_PropGram;
	CPropertySheet    *m_PropRes;
	CTabISRCentral    m_Cent;
	CTabISRAttr       m_Attrib;
	CTabISRDialogs    m_Dialogs;
	CTabISRNotify     m_Notify;
	CTabISRGrammar    m_Gramcom;
	CTabISRGramCFG    m_GramCFG;
	CTabISRGramDict   m_GramDict;
	CTabISRGramNotify m_GramNotify;
	CTabISRResAud     m_ResAudio;
	CTabISRResBasic   m_ResBasic;
	CTabISRResScores  m_ResScores;
	CTabISRResCorr    m_ResCorr;
	CTabISRResEval    m_ResEval;
	CTabISRResGraph   m_ResGraph;
	CTabISRResMem     m_ResMem;
	CTabISRResMerge   m_ResMerge;
	CTabISRResModGui  m_ResModGUI;
	CTabISRResSpeak   m_ResSpeak;

// Dialog Data
	//{{AFX_DATA(CSRTestDlg)
	enum { IDD = IDD_SRTEST_DIALOG };
	CButton	m_SelPropGram;
	CButton	m_SelPropEng;
	CButton	m_SelPropRes;
	CEdit	m_NotifyOut;
	CEdit	m_GramNotifyOut;
	CListBox	m_ListGramStat;
	CListBox	m_EnumList;
	CEdit	m_MainOutedit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSRTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSRTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnModeadd();
	afx_msg void OnSelchangeListgramstat();
	afx_msg void OnSelpropeng();
	afx_msg void OnSelchangeListenummodes();
	afx_msg void OnDblclkListgramstat();
	afx_msg void OnAbout();
	virtual void OnOK();
	afx_msg void OnSelpropgram();
	afx_msg void OnSelpropres();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CAddEngMode dialog

class CAddEngMode : public CDialog
{
// Construction
public:
	CAddEngMode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddEngMode)
	enum { IDD = IDD_DIALOGENGSELECT };
	CStatic	m_DeviceIDTxt;
	CComboBox	m_DeviceID;
	CButton	m_UseShare;
	CStatic	m_EnumOut7;
	CStatic	m_EnumOut13;
	CStatic	m_EnumOut12;
	CStatic	m_EnumOut11;
	CStatic	m_EnumOut10;
	CStatic	m_EnumOut9;
	CStatic	m_EnumOut8;
	CStatic	m_EnumOut6;
	CStatic	m_EnumOut3;
	CStatic	m_EnumOut1;
	CStatic	m_EnumOut5;
	CStatic	m_EnumOut4;
	CStatic	m_EnumOut2;
	CListBox	m_EnumList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddEngMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	PISRCENTRAL SelectMode( PSRMODEINFO psrResult );

	// Generated message map functions
	//{{AFX_MSG(CAddEngMode)
	afx_msg void OnFindcriteria();
	afx_msg void OnDblclkListenummodes1();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListenummodes1();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CGramPath dialog

class CGramPath : public CDialog
{
// Construction
public:
	CGramPath(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGramPath)
	enum { IDD = IDD_DLGGRAMPATH };
	CEdit	m_GramPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGramPath)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGramPath)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
