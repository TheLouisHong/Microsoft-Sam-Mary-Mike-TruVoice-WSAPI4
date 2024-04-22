/******************************************************************************\
*       vttesdlg.h
*
*       Header file for CVTTestDlg (main application dialog)
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
// CVTTestDlg dialog

class CVTTestDlg : public CDialog
{
// Construction
public:
	CVTTestDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CVTTestDlg();

	void MainOutput(LPCTSTR pLogStr);
	void DoRegister();

	// Property Sheet/Pages
	CPropertySheet*			m_pPropSheet;
	CTabIVoiceText				m_tabIVoiceText;
	CTabIVTxtAttributes		m_tabIVTxtAttributes;
	CTabIVTxtDialogs			m_tabIVTxtDialogs;
	CTabIVTxtNotifySink		m_tabIVTxtNotifySink;

	CVTTestObject*	m_pVTTestObj;			// variables to hold test and engine objects
	CVTEngineObject*	m_pVTEngObj;

// Dialog Data
	//{{AFX_DATA(CVTTestDlg)
	enum { IDD = IDD_VTTEST_DIALOG };
	CButton	m_buttonSpeakAlias;
	CEdit	m_editModeCtrl;
	CEdit	m_editMainOutput;
	CButton	m_buttonStatus;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVTTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVTTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSpeakAlias();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern inline void LogOutput(LPCTSTR pLogStr);
extern inline void LogNotifyOutput(LPCTSTR pLogStr);
