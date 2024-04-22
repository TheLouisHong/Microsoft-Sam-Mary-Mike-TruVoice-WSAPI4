/******************************************************************************\
*       Dialogs,cpp
*
*       Source file for FEEDBACK.EXE, a demonstration of the Microsoft
*       Speech Application Programming Interface.
*
*       Implementation of CSplashDlg, CFeedBackDlg, COptionsDlg, and CAboutDlg
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include "stdafx.h"
#include "FeedBack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern CFeedBackApp theApp;

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
//
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();
	
	SetDlgItemInt (IDC_BLDNUM, BUILD, FALSE);
	
	if (m_bSupTray)
	{
		SetDlgItemText (IDC_UITYPE, "Windows 95");
	}
	else
	{
		SetDlgItemText (IDC_UITYPE, "Windows 3.1");
	}
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFeedBackDlg dialog

CFeedBackDlg::CFeedBackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFeedBackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeedBackDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

   m_VCMDstate = UNINIT;
}

void CFeedBackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeedBackDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFeedBackDlg, CDialog)
	//{{AFX_MSG_MAP(CFeedBackDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_HIDE, OnHide)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_CONFIG, OnConfig)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
  ON_MESSAGE(WM_NOTIFYICON, OnTaskBar)
  ON_MESSAGE(WM_NOTIFYAPP, OnConfigMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeedBackDlg message handlers

/******************************************************************************\
* The following two message handlers, OnTaskBar() and OnConfigMsg(), are used  *
* to process a mouse click on the taskbar icon.  OnTaskBar() processes the     *
* actual click, showing the main dialog if the user clicks the left mouse      *
* button, and sending a message to create the config dialog if the user clicks *
* the right mouse button.                                                      *
*                                                                              *
* This method may seem overly-complicated.  However, it is necessary to avoid  *
* OLE problems.  OnTaskBar() is an input-synchronous call.  If the application *
* were to create the configuration dialog box within that call, the dialog box *
* would also be within the synchronous scope.  Because the dialog box must     *
* call into the Speech API, it can't be within synchronous scope, or it will   *
* receive the RPC_E_CANTCALLOUT_ININPUTSYNCCALL error message, returned when   *
* an outgoing call is made while the application is dispatching an input-      *
* synchronous call. (see winerror.h)                                           *
*                                                                              *
* The easiest way around this problem is to post a message in OnTaskBar(), and *
* then create the dialog in that message's message handler.  This will work    *
* because PostMessage() is asynchronous, while SendMessage(), which the task   *
* bar uses, is synchronous.                                                    *
\******************************************************************************/

LRESULT CFeedBackDlg::OnTaskBar(WPARAM wParam, LPARAM lParam)
{
  switch (lParam)
  {
    case WM_LBUTTONDOWN:
      ShowWindow (SW_SHOWNORMAL);
      break;
    
    case WM_RBUTTONDOWN:
      PostMessage (WM_NOTIFYAPP, wParam, lParam);
      break;

    default:
      break;
  }
  return NOERROR;
}

LRESULT CFeedBackDlg::OnConfigMsg(WPARAM wParam, LPARAM lParam)
{
  OnConfig();
  return NOERROR;
}

BOOL CFeedBackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow(NULL);
  ShowWindow (SW_HIDE);

  // Add "About..." and "Config..." menu items to system menu.

	// Check that IDM_CONFIGDLG is in the system command range.
	ASSERT((IDM_CONFIGDLG & 0xFFF0) == IDM_CONFIGDLG);
	ASSERT(IDM_CONFIGDLG < 0xF000);

	// Check that IDM_ABOUTBOX is in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString *szString = new CString;

  szString->LoadString(IDS_CONFIGDLG);
	if (!szString->IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_CONFIGDLG, *szString);
	}

  szString->LoadString(IDS_ABOUTBOX);
	if (!szString->IsEmpty())
	{
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, *szString);
	}
  
  //Disable the Size system menu item
  pSysMenu->EnableMenuItem (pSysMenu->GetMenuItemID (2), MF_GRAYED);


  //Disable the Maximize system menu item
  pSysMenu->EnableMenuItem (pSysMenu->GetMenuItemID (4), MF_GRAYED);
    
  //Create the Tray Feedback Class
  theApp.m_pTray = new CTrayFeedBack;
  m_bSupTray = theApp.m_pTray->InitTray(theApp.m_hInstance, m_hWnd);
  
  if (!m_bSupTray)
  {
    delete theApp.m_pTray;
    theApp.m_pTray = NULL;
		theApp.m_pIcon = new CIcons;
    theApp.m_pIcon->InitIcons (theApp.m_hInstance);

  };

	delete szString;
  
  theApp.VCmdState();

  ShowWindow (SW_HIDE);
  UpdateIcon();

	return TRUE;
}

void CFeedBackDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) OnAbout();
  else if ((nID & 0xFFF0) == IDM_CONFIGDLG) OnConfig();
	else CDialog::OnSysCommand(nID, lParam);
}

void CFeedBackDlg::OnDestroy()
{
  CDialog::OnDestroy();
}


void CFeedBackDlg::OnPaint() 
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
		
		dc.DrawIcon(x, y, theApp.m_pIcon->m_Icons[m_VCMDstate]);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFeedBackDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CFeedBackDlg::OnHide() 
{
  ShowWindow (SW_HIDE);
	UpdateIcon ();
}


void CFeedBackDlg::SetRecognizedCommand (PSTR szCommand)
{
  if (m_bSupTray) theApp.m_pTray->SetRecognizedCommand (szCommand);
  else theApp.m_pIcon->SetRecognizedCommand (szCommand);
}


void CFeedBackDlg::UpdateIcon()
{
	if (m_bSupTray)
	{
		theApp.m_pTray->SetIcon(m_VCMDstate);
	}
	else 
	{
    if (IsIconic()) Invalidate(TRUE);
 	  SetWindowText (theApp.m_pIcon->m_strCaption[m_VCMDstate].szCaption);
 	} 
}


void CFeedBackDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == REC_TIMER)
	{
		theApp.VCmdState();
	}
	else
	{
		CDialog::OnTimer(nIDEvent);
	}
}


BOOL CFeedBackDlg::SetState(VCMDSTATES state)
{
	// if state is being set to same state it is already in, do not update
  if (m_VCMDstate == state) return TRUE;
	
	// if VCmds is asleep, don't process heard, etc. notifications.
	if (m_VCMDstate == SLEEP) 
	{
	  if ((state != LISTEN) && (state != OFF)) return TRUE;
  }

  if ((state == RECOG) || (state == NOTREC)) SetTimer (REC_TIMER, theApp.m_iDelay, NULL);

	m_VCMDstate=state;
	UpdateIcon();

	return TRUE;
}

void CFeedBackDlg::OnConfig() 
{
	CConfigDlg ConfigDlg;
  ConfigDlg.DoModal();
}

void CFeedBackDlg::OnAbout() 
{
  CAboutDlg dlgAbout;
  dlgAbout.m_bSupTray = m_bSupTray;
  dlgAbout.DoModal();
 }

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg dialog
CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplashDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplashDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplashDlg, CDialog)
	//{{AFX_MSG_MAP(CSplashDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSplashDlg message handlers

void CSplashDlg::Message (LPCTSTR message) 
{
  SetDlgItemText (IDC_MESSAGE, message);
}

BOOL CSplashDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CenterWindow();
	ShowWindow (SW_SHOW); 
  Message ("Initializing. . .");
	
  if (theApp.InitOle())
	{
		CDialog::OnOK();
	}
	else
	{
		CString sFailOle;
    sFailOle.LoadString (IDS_FAILOLE);
		MessageBox (sFailOle, "Initialization Error!", MB_ICONSTOP);
		CDialog::OnCancel();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog
// See FeedBack.h for the specification for this class
//
CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_BN_CLICKED(IDC_ENGINE_ABOUT, OnEngineAbout)
	ON_BN_CLICKED(IDC_ENGINE_CONFIGURE, OnEngineConfigure)
	ON_BN_CLICKED(IDC_RADIO_ASLEEP, OnRadioAsleep)
	ON_BN_CLICKED(IDC_RADIO_LISTEN, OnRadioListen)
	ON_BN_CLICKED(IDC_RADIO_OFF, OnRadioOff)
	ON_BN_CLICKED(IDC_TRAIN, OnTrain)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

void CConfigDlg::OnEngineAbout() 
{
  theApp.ShowDlg (VCMD_AboutDlg);
}

void CConfigDlg::OnTrain() 
{
  theApp.ShowDlg (VCMD_TrainGeneralDlg);
}

void CConfigDlg::OnEngineConfigure() 
{
  theApp.ShowDlg (VCMD_GeneralDlg);
}

void CConfigDlg::OnRadioAsleep() 
{
	iVCmdState = SLEEP;
}

void CConfigDlg::OnRadioListen() 
{
  iVCmdState = LISTEN;
	
}

void CConfigDlg::OnRadioOff() 
{
  iVCmdState = OFF;
}

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  iVCmdState = theApp.GetVCmdState();
  
  if (iVCmdState == LISTEN) CheckDlgButton (IDC_RADIO_LISTEN, 1);
  if (iVCmdState == SLEEP)  CheckDlgButton (IDC_RADIO_ASLEEP, 1);
  if (iVCmdState == OFF)    CheckDlgButton (IDC_RADIO_OFF, 1);

  UINT delaySecs=(UINT)(theApp.m_iDelay/1000);

  SetDlgItemInt (IDC_EDIT_DELAY, delaySecs, FALSE);
	
	ShowWindow(SW_SHOWNORMAL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::OnOK() 
{
  theApp.SetVCmdState (iVCmdState);	
	theApp.m_iDelay = 1000 * GetDlgItemInt (IDC_EDIT_DELAY, NULL, FALSE);

	CDialog::OnOK();
}

void CConfigDlg::OnButtonApply() 
{
  theApp.SetVCmdState (iVCmdState);		
}

