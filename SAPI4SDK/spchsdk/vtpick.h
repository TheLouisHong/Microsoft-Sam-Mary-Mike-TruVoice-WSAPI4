/******************************************************************************\
*       vtpick.h
*
*       Header file for CPickTTSModeDlg and CRegisterDlg
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
// CPickTTSModeDlg dialog

class CPickTTSModeDlg : public CDialog
{
// Construction
public:
	CPickTTSModeDlg(PTTSMODEINFO pttsSelect, CVTTestObject* pVTTestObject = NULL, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CPickTTSModeDlg)
	enum { IDD = IDD_DIALOGENGSELECT };
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
	//{{AFX_VIRTUAL(CPickTTSModeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CVTTestObject* m_pVTTestObject;
	PTTSMODEINFO m_pttsSelect;

	// Generated message map functions
	//{{AFX_MSG(CPickTTSModeDlg)
	afx_msg void OnDblclkListenummodes1();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListenummodes1();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CRegisterDlg dialog

class CRegisterDlg : public CDialog
{
// Construction
public:
	CRegisterDlg(PVTENGOBJ pVTEngObj, CVTTestObject* pVTTestObject, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegisterDlg)
	enum { IDD = IDD_DIALOG_REGISTER };
	CStatic	m_staticModeName;
	CStatic	m_staticWPM;
	CButton	m_staticgModeID;
	CButton	m_staticdwSpeed;
	CButton	m_staticdwDevice;
	CEdit	m_editctrlModeName;
	CButton	m_buttonSetMode;
	CButton	m_buttonSpeedMin;
	CButton	m_buttonSpeedMax;
	CEdit	m_editctrlDeviceID;
	CButton	m_buttonSelectMode;
	CButton	m_radioDeviceID;
	CButton	m_radioMapper;
	CEdit	m_editctrlSpeed;
	CButton	m_checkEnable;
	CButton	m_checkNotify;
	CEdit	m_editSite;
	DWORD	m_editDWDevice;
	DWORD	m_editDWSpeed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CVTTestObject* m_pVTTestObject;
	PVTENGOBJ m_pVTEngObj;

	// Generated message map functions
	//{{AFX_MSG(CRegisterDlg)
	afx_msg void OnButtonSetMode();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonSpeedMax();
	afx_msg void OnButtonSpeedMin();
	afx_msg void OnRadioSiteNULL();
	afx_msg void OnRadioSiteInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
