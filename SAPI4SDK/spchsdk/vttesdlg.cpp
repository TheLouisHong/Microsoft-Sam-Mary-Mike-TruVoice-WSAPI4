/******************************************************************************\
*       vttesdlg.cpp
*
*       Implements functionality of the main CVTTestDlg application dialog
*       and the CAboutDlg dialog.
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/

#include "stdafx.h"
#include <limits.h>
#include "resource.h"
#include "tabdlgs.h"
#include <bufcvt.h>
#include "vtobj.h"

#include "vttest.h"
#include "vttesdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// Global function that directs log output to an edit control on the main dialog
void LogOutput(LPCTSTR pLogStr)
{
	((CVTTestDlg*) AfxGetApp()->m_pMainWnd)->MainOutput(pLogStr);
}

// Global function that directs log output to an edit control on the
// notification tab (CPropertyPage)	on the main dialog
void LogNotifyOutput(LPCTSTR pLogStr)
{
	((CVTTestDlg*) AfxGetApp()->m_pMainWnd)->m_tabIVTxtNotifySink.NotifyOutput(pLogStr);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// CVTTestDlg dialog

// Constructor	- initialize a few member variables
CVTTestDlg::CVTTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVTTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVTTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPropSheet = NULL;
	m_pVTTestObj = NULL;  
	m_pVTEngObj = NULL;

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// Destructor - delete any remaining engine or test objects (this releases
// all OLE interfaces) and delete the CPropertySheet (releases the property
// pages).
CVTTestDlg::~CVTTestDlg()
{
	if(m_pVTEngObj)
		delete m_pVTEngObj;
	if(m_pVTTestObj)
		delete m_pVTTestObj;
	if(m_pPropSheet)
		delete m_pPropSheet;
}

void CVTTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVTTestDlg)
	DDX_Control(pDX, IDC_BUTTON_SPEAK, m_buttonSpeakAlias);
	DDX_Control(pDX, IDC_EDIT_MODE, m_editModeCtrl);
	DDX_Control(pDX, IDC_EDIT_MAINOUTPUT, m_editMainOutput);
	DDX_Control(pDX, IDC_ENABLE_MSGS, m_buttonStatus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVTTestDlg, CDialog)
	//{{AFX_MSG_MAP(CVTTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SPEAK, OnButtonSpeakAlias)
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVTTestDlg message handlers

// Do standard MFC stuff and add Property Sheet, CVTTestObject,
// and register an engine (creates the Voice-Text object and
// calls IVoiceText::Register() with user selected options. 
BOOL CVTTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}
	
	// Create Property Sheet
	m_pPropSheet = (CPropertySheet *) new CPropertySheet(IDS_PROPCAPTION);
	if(m_pPropSheet)
	{
		m_pPropSheet->AddPage((CPropertyPage*) &m_tabIVoiceText);
		m_pPropSheet->AddPage((CPropertyPage*) &m_tabIVTxtAttributes);
		m_pPropSheet->AddPage((CPropertyPage*) &m_tabIVTxtDialogs);
		m_pPropSheet->AddPage((CPropertyPage*) &m_tabIVTxtNotifySink);

		if( m_pPropSheet->Create( this, WS_SYSMENU|WS_POPUP, NULL ) == 0 )
		{
			MainOutput(TEXT("Error creating the Engine property sheet."));
		}

		// Position the property sheet on the main dialog relative to the
		// upper left coordinate of the 	IDC_STATICPROPPOS	static groupbox
		CRect rSheet, rPos;
		m_pPropSheet->GetWindowRect(&rSheet);
		GetDlgItem(IDC_STATICPROPPOS)->GetWindowRect(&rPos);

		rPos.top += 15; // shift down and to the right
		rPos.left += 10;
		rPos.right= rPos.left + (rSheet.right - rSheet.left);
		// shorten the height by 30 since we don't have buttons...
		rPos.bottom = rPos.top + (rSheet.bottom - rSheet.top);
		// move the property sheet into position
		m_pPropSheet->MoveWindow( &rPos, TRUE );
	}


	// Enable logging to the main output window in the dialog
	((CButton*)GetDlgItem(IDC_ENABLE_MSGS))->SetCheck(1);

	// Add a default phrase to the main edit control on the IVoiceText tab
	m_tabIVoiceText.m_editSpeakData.SetWindowText(TEXT( \
"Edit this paragraph to speak something else. Click the \"High Priority\" \
button to append a phrase to the current buffer. Click the \"Very High Priority\" \
button to interrupt the current speech." \
																		));

	// Create a test object
	m_pVTTestObj = new CVTTestObject(LogOutput, LogNotifyOutput);

	// Create an engine and register it with user defined parameters
	DoRegister();

	m_pPropSheet->ShowWindow(SW_SHOW);

	// Set scroll position in the main logging edit control
	m_editMainOutput.LineScroll(m_editMainOutput.GetLineCount() - 4);
	
	// make sure the "IVoiceText::Speak() shortcut" button on the main dialog has focus
	m_buttonSpeakAlias.SetFocus();		
	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CVTTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVTTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVTTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVTTestDlg::MainOutput(LPCTSTR pLogStr)
{
	if(((CButton*)GetDlgItem(IDC_ENABLE_MSGS))->GetCheck() == 1)
	{
		m_editMainOutput.ReplaceSel(pLogStr);
		m_editMainOutput.ReplaceSel(TEXT("\r\n"));
	}
}

// Create a new engine object (releasing any existing engine) and
// register the application to use voice-text on a site specified
// by the user. U/I for choosing parameters for Register() is
// brought up by functions called in CreateEngine()
void CVTTestDlg::DoRegister()
{
	DWORD result;

	if(m_pVTEngObj)
	{
		// nuke this engine so we can create a new one
		m_pVTTestObj->ReleaseEngine(m_pVTEngObj);
	}

	// Create engine and bring up UI for Register()
	result = m_pVTTestObj->CreateEngine(m_pVTEngObj);
	if(result == ERROR_SUCCESS)
	{
		// display engine mode name specified in m_regInfo
		if(m_pVTTestObj->m_regInfo.bUseSiteInfo)
		{
			LOCALLOC(modeName, APISTRLEN(m_pVTTestObj->m_regInfo.ttsSelect.szModeName));
			LOCS2ACVT(modeName, (APICAST*) &(m_pVTTestObj->m_regInfo.ttsSelect.szModeName));
			m_editModeCtrl.SetWindowText(modeName);
			LOCFREE(modeName);
		}
		else
		{
			m_editModeCtrl.SetWindowText(TEXT("Voice-Text used its database or default."));
		}
	}
	else
	{
		m_editModeCtrl.SetWindowText(TEXT("Error Registering Site."));
	}
	m_buttonSpeakAlias.SetFocus();		
}


// Calls CTabIVoiceText::OnButtonSpeak() (same as pressing Speak button
// on IVoiceText tab)
void CVTTestDlg::OnButtonSpeakAlias()
{
	m_tabIVoiceText.DoSpeakShortcut();                                                                                                      
}

void CVTTestDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	if(m_pPropSheet) // OnInitDialog has been called
	{
		// Position the property sheet on the main dialog relative to the
		// upper left coordinate of the 	IDC_STATICPROPPOS	static groupbox
		CRect rSheet, rPos;
		m_pPropSheet->GetWindowRect(&rSheet);
		GetDlgItem(IDC_STATICPROPPOS)->GetWindowRect(&rPos);

		rPos.top += 15; // shift down and to the right
		rPos.bottom = rPos.top + (rSheet.bottom - rSheet.top);
		rPos.left += 10;
		rPos.right= rPos.left + (rSheet.right - rSheet.left);
		// move the property sheet into position
		m_pPropSheet->MoveWindow( &rPos, TRUE );
	}
}
