/******************************************************************************\
*       dlgfindm.h : header file
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
// CDlgFindMode dialog

class CDlgFindMode : public CDialog
{
// Construction
public:
	CDlgFindMode(CWnd* pParent = NULL);   // standard constructor

	DWORD m_iFeature;
	DWORD m_iSequence;
	DWORD m_iGramType;

// Dialog Data
	//{{AFX_DATA(CDlgFindMode)
	enum { IDD = IDD_DLGFINDMODE };
	CListBox	m_Features;
	CListBox	m_GramType;
	CComboBox	m_Sequence;
	CString	m_Mfg;
	CString	m_Lang;
	CString	m_ModeName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFindMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFindMode)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CGramAddCustom dialog

class CGramAddCustom : public CDialog
{
// Construction
public:
	CGramAddCustom(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGramAddCustom)
	enum { IDD = IDD_GRAMCUSTOMADD };
	CString	m_GramAddEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGramAddCustom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGramAddCustom)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
