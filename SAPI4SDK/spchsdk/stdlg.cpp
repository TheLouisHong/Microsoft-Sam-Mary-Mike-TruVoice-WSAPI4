/******************************************************************************\
*       srdlg.cpp
*
*       An MFC application for testing the the Speech recognition API.  
*       This module contains most of the UI handling for the main dialog.
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

#include "srtest.h"
#include "srdlg.h"
#include "srfunc.h"
#include "dlgfindm.h"
#include "mmsystem.h"

#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

const int PROPSHEETOFFSET_X = 10;
const int PROPSHEETOFFSET_Y = 94;

const int MAX_LISTBOXLINES=300; // the number of lines output to notification listboxes
                                // before the listbox is cleared.

BOOL gfModeSelected = FALSE;

void EnumGramFiles();
PTCHAR LangIDtoString( WORD LangID );
WORD StringtoLangID( LPCTSTR str );
int ModeIndex( PSRMODEINFO pSRInfo );

HCURSOR waitCur,saveCur;

PISRENUMOBJ gpSREnum=NULL;               // encapsulated enumerator object
PISRENGOBJ  gpEng=NULL;                  // global pointer to current engine object
int         gnModeList=0;                // the number of loaded engine modes

int         gdwNmodes=0;                 // the total number of available (enumerated) modes
TCHAR       gszGramPath[MAX_STRLEN];     // the path where the grammar files live
GramName_t  *gpGramFiles;                // the enumerated grammar filenames
int         gnGramFiles=0;               // the total number of available grammar files
PSRMODEINFO gpSRInfoList;                // the enumerated list of available modes
SRMODEINFO  gSRSelect;                   // the mode selected by the user from the add mode dialog

// language test table, taken from WINNT.h.
// these are used for the find mode dialog...
PTCHAR Languages[MAX_LANGUAGES]={
TEXT("NEUTRAL"),TEXT("BULGARIAN"),TEXT("CHINESE"),TEXT("CROATIAN"),TEXT("CZECH"),TEXT("DANISH"),
TEXT("DUTCH"),TEXT("ENGLISH"),TEXT("FINNISH"),TEXT("FRENCH"),TEXT("GERMAN"),TEXT("GREEK"),
TEXT("HUNGARIAN"),TEXT("ICELANDIC"),TEXT("ITALIAN"),TEXT("JAPANESE"),TEXT("KOREAN"),
TEXT("NORWEGIAN"),TEXT("POLISH"),TEXT("PORTUGUESE"),TEXT("ROMANIAN"),TEXT("RUSSIAN"),
TEXT("SLOVAK"),TEXT("SLOVENIAN"),TEXT("SPANISH"),TEXT("SWEDISH"),TEXT("TURKISH")};

WORD LanguageID[MAX_LANGUAGES]={
LANG_NEUTRAL,LANG_BULGARIAN,LANG_CHINESE,LANG_CROATIAN,LANG_CZECH,LANG_DANISH,LANG_DUTCH,
LANG_ENGLISH,LANG_FINNISH,LANG_FRENCH,LANG_GERMAN,LANG_GREEK,LANG_HUNGARIAN,LANG_ICELANDIC,
LANG_ITALIAN,LANG_JAPANESE,LANG_KOREAN,LANG_NORWEGIAN,LANG_POLISH,LANG_PORTUGUESE,
LANG_ROMANIAN,LANG_RUSSIAN,LANG_SLOVAK,LANG_SLOVENIAN,LANG_SPANISH,LANG_SWEDISH,LANG_TURKISH};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_UnicodeType;
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
	DDX_Control(pDX, IDC_UNICODETYPE, m_UnicodeType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();
	
	// display the compiled type (unicode or ansi)...
#if !(defined(_UNICODE) || defined(UNICODE)) && !defined(_S_UNICODE)
	m_UnicodeType.SetWindowText("ANSI app, ANSI api");
#elif !(defined(_UNICODE) || defined(UNICODE)) && defined(_S_UNICODE)
	m_UnicodeType.SetWindowText("ANSI app, UNICODE api");
#elif (defined(_UNICODE) || defined(UNICODE)) && defined(_S_ANSI)
	m_UnicodeType.SetWindowText(TEXT("UNICODE app, ANSI api"));
#elif (defined(_UNICODE) || defined(UNICODE)) && !defined(_S_ANSI)
	m_UnicodeType.SetWindowText(TEXT("UNICODE app, UNICODE api"));
#endif
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// CSRTestDlg dialog

CSRTestDlg::CSRTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSRTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSRTestDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_PropEng = m_PropGram = m_PropRes = NULL;
}

void CSRTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSRTestDlg)
	DDX_Control(pDX, IDC_SELPROPRES, m_SelPropRes);
	DDX_Control(pDX, IDC_SELPROPGRAM, m_SelPropGram);
	DDX_Control(pDX, IDC_SELPROPENG, m_SelPropEng);
	DDX_Control(pDX, IDC_NOTIFYOUTPUT, m_NotifyOut);
	DDX_Control(pDX, IDC_GRAMNOTIFYOUTPUT, m_GramNotifyOut);
	DDX_Control(pDX, IDC_LISTGRAMSTAT, m_ListGramStat);
	DDX_Control(pDX, IDC_LISTENUMMODES, m_EnumList);
	DDX_Control(pDX, IDC_EDITMAIN, m_MainOutedit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSRTestDlg, CDialog)
	//{{AFX_MSG_MAP(CSRTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MODEADD, OnModeadd)
	ON_LBN_SELCHANGE(IDC_LISTGRAMSTAT, OnSelchangeListgramstat)
	ON_BN_CLICKED(IDC_SELPROPENG, OnSelpropeng)
	ON_LBN_SELCHANGE(IDC_LISTENUMMODES, OnSelchangeListenummodes)
	ON_LBN_DBLCLK(IDC_LISTGRAMSTAT, OnDblclkListgramstat)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_SELPROPGRAM, OnSelpropgram)
	ON_BN_CLICKED(IDC_SELPROPRES, OnSelpropres)
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSRTestDlg message handlers

/*
 Initialize property sheets, enumerate the available engine modes and grammar files.
*/
BOOL CSRTestDlg::OnInitDialog()
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
	
	// create the engine and grammar property sheets...
	m_PropEng = (CPropertySheet *) new CPropertySheet(IDS_PROPCAPTION);
	ASSERT( m_PropEng  );
	m_PropEng->AddPage( &m_Cent    );
	m_PropEng->AddPage( &m_Attrib  );
	m_PropEng->AddPage( &m_Dialogs );
	m_PropEng->AddPage( &m_Notify  );

	if( m_PropEng->Create( this, WS_SYSMENU|WS_POPUP|WS_TABSTOP, NULL ) == 0 )
		MainOutput(TEXT("\nError creating the Engine property sheet.\n"));

	m_PropGram = (CPropertySheet *) new CPropertySheet(IDS_PROPCAPTION);
	ASSERT( m_PropGram );
	m_PropGram->AddPage( &m_Gramcom    );
	m_PropGram->AddPage( &m_GramCFG    );
	m_PropGram->AddPage( &m_GramDict   );
	m_PropGram->AddPage( &m_GramNotify );

	if( m_PropGram->Create( this, WS_SYSMENU|WS_POPUP|WS_TABSTOP, NULL ) == 0 )
		MainOutput(TEXT("\nError creating the Grammar property sheet.\n"));

	// create the ISRRes property sheet...
	m_PropRes = (CPropertySheet *) new CPropertySheet(IDS_PROPCAPTION);
	ASSERT( m_PropRes );
	m_PropRes->AddPage( &m_ResBasic   );
	m_PropRes->AddPage( &m_ResAudio   );
	m_PropRes->AddPage( &m_ResCorr    );
	m_PropRes->AddPage( &m_ResEval    );
	m_PropRes->AddPage( &m_ResGraph   );
	m_PropRes->AddPage( &m_ResMem     );
	m_PropRes->AddPage( &m_ResMerge   );
	m_PropRes->AddPage( &m_ResModGUI  );
	m_PropRes->AddPage( &m_ResSpeak   );
	m_PropRes->AddPage( &m_ResScores  );

	if( m_PropRes->Create( this, WS_SYSMENU|WS_POPUP|WS_TABSTOP, NULL ) == 0 )
		MainOutput(TEXT("\nError creating the Results property sheet.\n"));

	// position the property sheets...
	RECT  rMain, r;
	GetClientRect( &rMain );
	m_PropEng->GetWindowRect( &r );
	ClientToScreen( &rMain );

	m_PropEng->MoveWindow( rMain.left+PROPSHEETOFFSET_X, rMain.top+PROPSHEETOFFSET_Y,
		r.right-r.left, r.bottom-r.top, TRUE );
	m_PropGram->MoveWindow( rMain.left+PROPSHEETOFFSET_X, rMain.top+PROPSHEETOFFSET_Y,
		r.right-r.left, r.bottom-r.top, TRUE );
	m_PropRes->MoveWindow( rMain.left+PROPSHEETOFFSET_X, rMain.top+PROPSHEETOFFSET_Y,
		r.right-r.left, r.bottom-r.top, TRUE );

	// disable property sheets and selection radio controls until
	// an engine is selected...
	m_PropEng->EnableWindow ( FALSE );
	m_PropGram->EnableWindow( FALSE );
	m_PropRes->EnableWindow ( FALSE );
	m_SelPropEng.EnableWindow ( FALSE );
	m_SelPropGram.EnableWindow( FALSE );
	m_SelPropRes.EnableWindow ( FALSE );
	CheckRadioButton( IDC_SELPROPENG, IDC_SELPROPRES, IDC_SELPROPENG );

	// enumerate the available grammar files...
	gszGramPath[0] = NULL;
	EnumGramFiles();

	waitCur = LoadCursor( NULL, IDC_WAIT );

	// scroll the mainoutput window to the last line visible...
	m_MainOutedit.LineScroll( m_MainOutedit.GetLineCount()-4, 0 );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSRTestDlg::OnMove(int x, int y) 
{
	RECT  r, rNew;
	CDialog::OnMove(x, y);

	if( !(m_PropEng) ) return;

	m_PropEng->GetWindowRect( &r );

	rNew.left = x + PROPSHEETOFFSET_X;
	rNew.top  = y + PROPSHEETOFFSET_Y;
	rNew.right= r.right - r.left;
	rNew.bottom = r.bottom - r.top;

	m_PropEng->MoveWindow( rNew.left, rNew.top,	rNew.right, rNew.bottom, TRUE );
	m_PropGram->MoveWindow( rNew.left, rNew.top,	rNew.right, rNew.bottom, TRUE );
	m_PropRes->MoveWindow( rNew.left, rNew.top,	rNew.right, rNew.bottom, TRUE );
}

void CSRTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSRTestDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags the minimized window.
HCURSOR CSRTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CSRTestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create SR enumerator object and initialize, return -1 if error...
	gpSREnum = new ISRENUMOBJ;
	if( gpSREnum == NULL ) return -1;

	if( gpSREnum->FInit() == FALSE )
	{
		delete gpSREnum;
      gpSREnum = 0;
		return -1;
	}

	return 0;
}

/*
 The OK button was selected on the main window, we're exiting the test app.
*/
void CSRTestDlg::OnOK() 
{
	CDialog::OnOK();
}

void CSRTestDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	PISRENGOBJ pEng;

	// delete the loaded engine mode objects...
	for( int i=0; i<gnModeList; i++ )
	{
		pEng = (PISRENGOBJ)m_EnumList.GetItemData( i );
		delete pEng;
	}

	gnModeList = 0;

	// clear the engine mode listbox and free the mode info and grammar file lists...
   if (m_EnumList)
	   m_EnumList.ResetContent();
   if (gpSRInfoList)
	   free( gpSRInfoList );
   if (gpGramFiles)
	   free( gpGramFiles );

	// delete the property sheets...
	if( m_PropEng  ) delete m_PropEng;
	if( m_PropGram ) delete m_PropGram;
	if( m_PropRes  ) delete m_PropRes;

	// delete the find and enumerator objects...
	if( gpSREnum) delete gpSREnum;
}

void CSRTestDlg::OnAbout() 
{
	CAboutDlg Dlg;
	
	Dlg.DoModal();
}

/*************************************************************************
EnumModes - Enumerates the engine modes and fills the SRMODEINFO array
            pointed to by pSRInfoList.

inputs:
	pSRInfoList - to be filled with SRMODEINFO structures

returns:
	the number of modes found.
*/
DWORD CSRTestDlg::EnumModes( PSRMODEINFO *pSRInfoList )
{
	DWORD dwNumModes;
	TCHAR buf[MAX_STRLEN];

	// enumerate the modes...
	if( !gpSREnum->EngModeEnum( pSRInfoList, &dwNumModes ) ) return 0;

	wsprintf( buf, TEXT("  %d mode(s) found:\n"), (int) dwNumModes );
	MainOutput( buf );

	return dwNumModes;
}

/*
 User pressed engine mode add button, display add mode dialog.
*/
void CSRTestDlg::OnModeadd() 
{
	CAddEngMode Dlg;
	
	// enumerate the available engine modes...
	gdwNmodes = (int)EnumModes( &gpSRInfoList );

	Dlg.DoModal();

   if (gfModeSelected)
      GetDlgItem(IDC_MODEADD)->EnableWindow(FALSE);
}

/*
 User selected an engine mode from the engine mode listbox, make it our current
 global engine pointer.
*/
void CSRTestDlg::OnSelchangeListenummodes() 
{
	POSITION    pos;
	PISRGRAMOBJ pGram;
	int index = m_EnumList.GetCurSel();

	// make the selected item our current engine object pointer...
	gpEng = (PISRENGOBJ)m_EnumList.GetItemData( index );

	// if no grammars are loaded, disable the grammar property sheet...
	if( gpEng->m_nLoadedGrams == 0 )
	{
		m_PropGram->EnableWindow( FALSE );
		m_PropRes->EnableWindow( FALSE );
	}
	else
	{
		m_PropGram->EnableWindow( TRUE );
		if( gpEng->m_pGram->m_ResList.GetCount() > 0 ) m_PropRes->EnableWindow( TRUE );
	}

	// reset grammar listbox and refill it with loaded grammars for this engine mode...
	((CSRTestDlg *)AfxGetMainWnd())->m_ListGramStat.ResetContent();
	for( int i=0; i<gpEng->m_nLoadedGrams; i++ )
	{
		pos = gpEng->m_GramList.FindIndex( i );
		pGram = (PISRGRAMOBJ)gpEng->m_GramList.GetAt( pos );
		((CSRTestDlg *)AfxGetMainWnd())->m_ListGramStat.InsertString( i, pGram->m_szName );
	}
	((CSRTestDlg *)AfxGetMainWnd())->m_ListGramStat.SetCurSel( 0 );

	// update the property page controls to reflect current engine status...
	InitPropPageSettingsEng();

	// make the newly highlighted grammar the current one...
	OnSelchangeListgramstat();
}

/*
 User selected the engine radio control, make the engine property sheet active/visible.
*/
void CSRTestDlg::OnSelpropeng() 
{
	m_PropRes->ShowWindow( SW_HIDE );
	m_PropGram->ShowWindow( SW_HIDE );
	m_PropEng->ShowWindow( SW_SHOW );
}

/*
 Update the engine attribute controls to reflect current engine status.
*/
void CSRTestDlg::InitPropPageSettingsEng()
{
	if( !gpEng ) return;

	HRESULT hRes;

	if( m_Cent.m_hWnd )
	{
		m_Cent.m_EngPaused.SetCheck( gpEng->m_bPaused );
	}

	// ISRAttributes controls...

	if( m_Attrib.m_hWnd )
	{
		TCHAR buf[MAX_STRLEN];
		DWORD dwSize, dwTmp=0, dwTmp1=0;
		WORD  wTmp=0;
		BOOL  bTmp;

		APIALLOC(str, MAX_STRLEN);

		hRes = gpEng->m_pISRAttributes->AutoGainEnableGet( &dwTmp );
      if( FAILED(hRes) ) wsprintf( buf, (hRes == E_NOTIMPL) ? TEXT("Not Implemented") : TEXT("Error") );
		else wsprintf( buf, TEXT("%ld"), dwTmp );
		m_Attrib.m_AttrGain.SetSel( (DWORD)0xFFFF0000, 0 );
		m_Attrib.m_AttrGain.ReplaceSel( buf );

		dwTmp=0;
		hRes = gpEng->m_pISRAttributes->ThresholdGet( &dwTmp );
		if( FAILED(hRes) ) wsprintf( buf, (hRes == E_NOTIMPL) ? TEXT("Not Implemented") : TEXT("Error") );
		else wsprintf( buf, TEXT("%ld"), dwTmp );
		m_Attrib.m_Thresh.SetSel( (DWORD)0xFFFF0000, 0 );
		m_Attrib.m_Thresh.ReplaceSel( buf );

		wTmp=0;
		hRes = gpEng->m_pISRAttributes->EnergyFloorGet( &wTmp );
		if( FAILED(hRes) ) wsprintf( buf, (hRes == E_NOTIMPL) ? TEXT("Not Implemented") : TEXT("Error") );
		else wsprintf( buf, TEXT("%u"), wTmp );
		m_Attrib.m_EnergyFlr.SetSel( (DWORD)0xFFFF0000, 0 );
		m_Attrib.m_EnergyFlr.ReplaceSel( buf );

		memset( str, 0, APISIZEOFBUF(str) );
		hRes = gpEng->m_pISRAttributes->MicrophoneGet( str, MAX_STRLEN, &dwSize );
		if( FAILED(hRes) ) wsprintf( buf, (hRes == E_NOTIMPL) ? TEXT("Not Implemented") : TEXT("Error") );
		else APIS2ACVT( buf, str );
		m_Attrib.m_Mic.SetSel( (DWORD)0xFFFF0000, 0 );
		m_Attrib.m_Mic.ReplaceSel( buf );

		memset( str, 0, APISIZEOFBUF(str) );
		hRes = gpEng->m_pISRAttributes->SpeakerGet( str, MAX_STRLEN, &dwSize );
		if( FAILED(hRes) ) wsprintf( buf, (hRes == E_NOTIMPL) ? TEXT("Not Implemented") : TEXT("Error") );
		else APIS2ACVT( buf, str );
		m_Attrib.m_Speaker.SetSel( (DWORD)0xFFFF0000, 0 );
		m_Attrib.m_Speaker.ReplaceSel( buf );

		dwTmp = 0;
		hRes = gpEng->m_pISRAttributes->RealTimeGet( &dwTmp );
		if( FAILED(hRes) ) wsprintf( buf, (hRes == E_NOTIMPL) ? TEXT("Not Implemented") : TEXT("Error") );
		else wsprintf( buf, TEXT("%ld"), dwTmp );
		m_Attrib.m_CPU.SetSel( (DWORD)0xFFFF0000, 0 );
		m_Attrib.m_CPU.ReplaceSel( buf );

		dwTmp = dwTmp1 = 0;
		hRes = gpEng->m_pISRAttributes->TimeOutGet( &dwTmp, &dwTmp1 );
		if( FAILED(hRes) ) wsprintf( buf, (hRes == E_NOTIMPL) ? TEXT("Not Implemented") : TEXT("Error") );
		else wsprintf( buf, TEXT("%ld"), dwTmp1 );
		m_Attrib.m_TimeOutC.SetSel( (DWORD)0xFFFF0000, 0 );
		m_Attrib.m_TimeOutC.ReplaceSel( buf );
		if( FAILED(hRes) ) wsprintf( buf, (hRes == E_NOTIMPL) ? TEXT("Not Implemented") : TEXT("Error") );
		else wsprintf( buf, TEXT("%ld"), dwTmp );
		m_Attrib.m_TimeOutI.SetSel( (DWORD)0xFFFF0000, 0 );
		m_Attrib.m_TimeOutI.ReplaceSel( buf );

		bTmp = FALSE;
		hRes = gpEng->m_pISRAttributes->EchoGet( &bTmp );
		m_Attrib.m_Echo.SetCheck( bTmp );

		APIFREE( str );
	}

	if( m_Notify.m_hWnd )
	{
		m_Notify.m_Attr.SetCheck( gpEng->m_Not.bAttrChange );
		m_Notify.m_Interfere.SetCheck( gpEng->m_Not.bInterfere );
		m_Notify.m_Utter.SetCheck( gpEng->m_Not.bUtter );
		m_Notify.m_Sound.SetCheck( gpEng->m_Not.bSound );
		m_Notify.m_VMeter.SetCheck( gpEng->m_Not.bVMeter );
	}
}

/*
 User selected the grammar radio control, make the grammar property sheet active/visible.
*/
void CSRTestDlg::OnSelpropgram() 
{
	if( gpEng->m_nLoadedGrams == 0 )
	{
		MessageBox( TEXT("There are no grammars loaded for this engine mode, please load a grammar."), NULL, MB_OK );
		CheckRadioButton( IDC_SELPROPENG, IDC_SELPROPRES, IDC_SELPROPENG );
		return;
	}

	// update the property page controls to reflect current grammar status...
	InitPropPageSettingsGram();

	m_PropRes->ShowWindow( SW_HIDE );
	m_PropEng->ShowWindow( SW_HIDE );
	m_PropGram->ShowWindow( SW_SHOW );
}

/*
 User selected a grammar from the grammar listbox, make it our current grammar pointer.
*/
void CSRTestDlg::OnSelchangeListgramstat() 
{
	// if there are no grammars loaded yet...
	if( gpEng->m_nLoadedGrams < 1 ) return;

	// get the index of the highlighted grammar...
	int index = m_ListGramStat.GetCurSel();

	// retrieve the pointer to the selected grammar from the grammar list...
	POSITION pos = gpEng->m_GramList.FindIndex( index );
	gpEng->m_pGram = (PISRGRAMOBJ)gpEng->m_GramList.GetAt( pos );

	// update the property page controls to reflect current grammar status...
	InitPropPageSettingsGram();

	// update the results object list as well...
	InitPropPageSettingsRes();
}

/*
 User double-clicked on a grammar in the grammar listbox.  Toggle the grammar active
 status in addition to the processing already done in OnSelChangeListgramstat().
*/
void CSRTestDlg::OnDblclkListgramstat() 
{
	// note the OnSelChangeListgramstat() function has just been called
	// by MFC prior to this so we should have a valid grammar pointer...
	ASSERT( gpEng->m_pGram );  // (doesn't mean it's the right one though)

	// give the grammar property sheet the focus so they can see
	// that the grammar activate state is changing...
	CheckRadioButton( IDC_SELPROPENG, IDC_SELPROPRES, IDC_SELPROPGRAM );
	OnSelpropgram();

	// toggle the activation of this grammar.

	// atoggle the activate state of the grammar...
	if( gpEng->m_pGram->m_bActive )
		m_Gramcom.OnGramdeactivategram();
	else
		m_Gramcom.OnGramactivate();
}

/*
 Update the grammar property sheet with the current grammar properties.
*/
void CSRTestDlg::InitPropPageSettingsGram()
{
	POSITION pos;
	int      i;

	CursorWait;

	if( !gpEng || !gpEng->m_pGram || gpEng->m_nLoadedGrams == 0 ) return;

	ASSERT( gpEng->m_pGram->m_pISRGramCommon );

	// grammar activation state...

	if( m_Gramcom.m_hWnd )
	{
		PTCHAR pRule;

		m_Gramcom.m_GramRuleList.ResetContent();
		m_Gramcom.m_GramRuleList.AddString( TEXT("NULL") );
		for( i=0; i<gpEng->m_pGram->m_RuleList.GetCount(); i++ )
		{
			pos = gpEng->m_pGram->m_RuleList.FindIndex( i );
			pRule = (PTCHAR)gpEng->m_pGram->m_RuleList.GetAt( pos );
			m_Gramcom.m_GramRuleList.AddString( pRule );
		}

		m_Gramcom.m_RuleActiveList.ResetContent();
		for( i=0; i<gpEng->m_pGram->m_ActiveRules.GetCount(); i++ )
		{
			pos = gpEng->m_pGram->m_ActiveRules.FindIndex( i );
			pRule = (PTCHAR)gpEng->m_pGram->m_ActiveRules.GetAt( pos );
			m_Gramcom.m_RuleActiveList.AddString( pRule );
		}

		m_Gramcom.m_GramRuleList.SetCurSel( 0 );
		m_Gramcom.m_GramAutoPause.SetCurSel( 0 );
		m_Gramcom.m_GramWnd.SetCurSel( 0 );
		m_Gramcom.m_RuleActiveList.SetCurSel( 0 );
	}

	// ISR GrammarCFG Interface status...

	if( m_GramCFG.m_hWnd )
	{
		PCFGName_t pListName;

		// put the enumerated grammar list names into the combobox...
		m_GramCFG.m_GramListName.ResetContent();
		for( i=0; i<gpEng->m_pGram->m_GramList.GetCount(); i++ )
		{
			pos = gpEng->m_pGram->m_GramList.FindIndex( i );
			pListName = (PCFGName_t)gpEng->m_pGram->m_GramList.GetAt( pos );
			m_GramCFG.m_GramListName.AddString( pListName->szName );
		}
		m_GramCFG.m_GramListName.SetCurSel( gpEng->m_pGram->m_szListNameIndex );
	}

	if( m_GramNotify.m_hWnd )
	{
		m_GramNotify.m_Bookmark.SetCheck( gpEng->m_pGram->m_Not.bBookmark );
		m_GramNotify.m_Paused.SetCheck( gpEng->m_pGram->m_Not.bPaused );
		m_GramNotify.m_Phrase.SetCheck( gpEng->m_pGram->m_Not.bPhrase );
		m_GramNotify.m_PhraseHyp.SetCheck( gpEng->m_pGram->m_Not.bPhraseHyp );
		m_GramNotify.m_ReEval.SetCheck( gpEng->m_pGram->m_Not.bReEval );
		m_GramNotify.m_Unarchive.SetCheck( gpEng->m_pGram->m_Not.bUnarchive );
	}

	CursorGo;
}

void CSRTestDlg::InitPropPageSettingsRes()
{
	CursorWait;

	if( m_ResBasic.m_hWnd )
	{
		Result_t *pResult;
		POSITION pos;

		// add the results objects to the results selection listbox...
		m_ResBasic.m_ResSelection.ResetContent();
		for( DWORD i=0; i< gpEng->m_pGram->m_NumResults; i++ )
		{
			pos = gpEng->m_pGram->m_ResList.FindIndex( i );
			pResult = gpEng->m_pGram->m_ResList.GetAt( pos );
			m_ResBasic.m_ResSelection.AddString( pResult->szDesc );
			m_ResBasic.m_ResSelection.SetItemData( i, (DWORD)pResult->lpResult );
		}
	}

	CursorGo;
}

/*
 Display diagnostics to the bottom output edit control.
*/
void CSRTestDlg::MainOutput(PTCHAR str) 
{
	TCHAR str1[MAX_STRLEN], *p1;

	// for every '\n', insert a '\r', it needs both...
	p1 = str1;
	while( *str )
	{
		if( *str == '\n' ) *p1++ = '\r';
		*p1++ = *str++;
	}
	*p1 = 0;

	m_MainOutedit.ReplaceSel( (LPCTSTR)str1 );
}

/*
 Display engine notification information to the left output window.
*/
void CSRTestDlg::NotifyOutput(const PTCHAR str) 
{
	if( m_NotifyOut.GetLineCount() > MAX_LISTBOXLINES )
		m_NotifyOut.SetSel( (DWORD)0xFFFF0000, 0);	 // select all

	m_NotifyOut.ReplaceSel( (LPCTSTR)str );
}

/*
 Display grammar notification information to the right output window.
*/
void CSRTestDlg::GramNotifyOutput(const PTCHAR str) 
{
	if( m_GramNotifyOut.GetLineCount() > MAX_LISTBOXLINES )
		m_GramNotifyOut.SetSel( (DWORD)0xFFFF0000, 0);	 // select all

	m_GramNotifyOut.ReplaceSel( (LPCTSTR)str );
}

/////////////////////////////////////////////////////////////////////////////
// CAddEngMode dialog


CAddEngMode::CAddEngMode(CWnd* pParent /*=NULL*/)
	: CDialog(CAddEngMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddEngMode)
	//}}AFX_DATA_INIT
}


void CAddEngMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddEngMode)
	DDX_Control(pDX, IDC_DEVICEIDTXT, m_DeviceIDTxt);
	DDX_Control(pDX, IDC_DEVICEID, m_DeviceID);
	DDX_Control(pDX, IDC_USESHARE, m_UseShare);
	DDX_Control(pDX, IDC_ENUMOUTPUT7A, m_EnumOut7);
	DDX_Control(pDX, IDC_ENUMOUTPUT13A, m_EnumOut13);
	DDX_Control(pDX, IDC_ENUMOUTPUT12A, m_EnumOut12);
	DDX_Control(pDX, IDC_ENUMOUTPUT11A, m_EnumOut11);
	DDX_Control(pDX, IDC_ENUMOUTPUT10A, m_EnumOut10);
	DDX_Control(pDX, IDC_ENUMOUTPUT9A, m_EnumOut9);
	DDX_Control(pDX, IDC_ENUMOUTPUT8A, m_EnumOut8);
	DDX_Control(pDX, IDC_ENUMOUTPUT6A, m_EnumOut6);
	DDX_Control(pDX, IDC_ENUMOUTPUT3A, m_EnumOut3);
	DDX_Control(pDX, IDC_ENUMOUTPUT1A, m_EnumOut1);
	DDX_Control(pDX, IDC_ENUMOUTPUT5A, m_EnumOut5);
	DDX_Control(pDX, IDC_ENUMOUTPUT4A, m_EnumOut4);
	DDX_Control(pDX, IDC_ENUMOUTPUT2A, m_EnumOut2);
	DDX_Control(pDX, IDC_LISTENUMMODES1, m_EnumList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddEngMode, CDialog)
	//{{AFX_MSG_MAP(CAddEngMode)
	ON_BN_CLICKED(IDC_FINDCRITERIA, OnFindcriteria)
	ON_LBN_DBLCLK(IDC_LISTENUMMODES1, OnDblclkListenummodes1)
	ON_LBN_SELCHANGE(IDC_LISTENUMMODES1, OnSelchangeListenummodes1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAddEngMode message handlers

BOOL CAddEngMode::OnInitDialog() 
{
	CDialog::OnInitDialog();

	TCHAR buf[SRMI_NAMELEN];
	int i;

	// clear the ISRFind flag...
	gpSREnum->m_bFindFlag = 0;

	// stuff the enumerated modes in the list box to allow user to choose...
	for( i=0; i<gdwNmodes; i++ )
	{
		S2ACVTEX( buf, gpSRInfoList[i].szModeName, SRMI_NAMELEN );
		m_EnumList.InsertString( i, buf );
	}

	// select the first mode and display it...
	m_EnumList.SetCurSel( 0 );
	OnSelchangeListenummodes1();

	// enumerate amd list the available device IDs...
	m_DeviceID.ResetContent();
	m_DeviceID.AddString(TEXT("WAVE_MAPPER"));
	for( i=0; i<(int)waveInGetNumDevs(); i++ )
	{
		wsprintf( buf, TEXT("%d"), i );
		m_DeviceID.AddString( buf );
	}
	m_DeviceID.SetCurSel( 0 );	// default to the wave_mapper


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
 The user just pressed the Find button in the mode selection dialog.
 Activate the find mode dialog.
*/
void CAddEngMode::OnFindcriteria() 
{
	CDlgFindMode Dlg;
	TCHAR  buf[SRMI_NAMELEN];
	int index=-1;	// -1 means no mode matches
	static SRMODEINFO srInfo;
	static int firsttime=1;

	if( firsttime )
	{
		memset( &srInfo, 0, sizeof(srInfo) );
		srInfo.language.LanguageID = LANG_ENGLISH;
		firsttime = 0;
	}

	// init the dialog strings...
	Dlg.m_Lang = LangIDtoString( srInfo.language.LanguageID );
	S2ACVTEX( buf, srInfo.szMfgName, SRMI_NAMELEN );
	Dlg.m_Mfg = buf;
	S2ACVTEX( buf, srInfo.szModeName, SRMI_NAMELEN );
	Dlg.m_ModeName = buf;

	Dlg.DoModal();

	// store the members that the user selected as criteria for the find...
	srInfo.language.LanguageID = StringtoLangID( (LPCTSTR)Dlg.m_Lang );
	A2SCVTEX( srInfo.szMfgName,  Dlg.m_Mfg, SRMI_NAMELEN );
	A2SCVTEX( srInfo.szModeName, Dlg.m_ModeName, SRMI_NAMELEN );
	srInfo.dwSequencing = Dlg.m_iSequence;
	srInfo.dwGrammars   = Dlg.m_iGramType;
	srInfo.dwFeatures   = Dlg.m_iFeature;

	// do the find using the ISRFind interface...
	gpSREnum->EngModeFind( &srInfo, &gSRSelect );

	// get the index of the mode it found,
	if( (index = ModeIndex( &gSRSelect )) == -1 )
		MessageBox( TEXT("Mode based on criteria not found."), TEXT("Warning"), MB_OK );
	else
		m_EnumList.SetCurSel( index );

	OnSelchangeListenummodes1();
}

/*
 Display the detailed information about the mode that is currently highlighted
 by the user in the mode select list control.
*/
void CAddEngMode::OnSelchangeListenummodes1() 
{
	TCHAR str[SRMI_NAMELEN], buf[SRMI_NAMELEN];

	gSRSelect = gpSRInfoList[ m_EnumList.GetCurSel() ];

	S2ACVTEX( buf, gSRSelect.szModeName, SRMI_NAMELEN );
	m_EnumOut1.SetWindowText( buf );

	S2ACVTEX( buf, gSRSelect.szMfgName, SRMI_NAMELEN );
	m_EnumOut2.SetWindowText( buf );

	wsprintf( str, TEXT("%s"), LangIDtoString( gSRSelect.language.LanguageID ) );
	m_EnumOut3.SetWindowText( str );

	S2ACVTEX( buf, gSRSelect.szProductName, SRMI_NAMELEN );
	m_EnumOut4.SetWindowText( buf );

	wsprintf( str, TEXT("%x %x %x %x%x%x%x%x%x%x%x"),
		gSRSelect.gModeID.Data1,gSRSelect.gModeID.Data2,gSRSelect.gModeID.Data3,
		gSRSelect.gModeID.Data4[0],gSRSelect.gModeID.Data4[1],
		gSRSelect.gModeID.Data4[2],gSRSelect.gModeID.Data4[3],
		gSRSelect.gModeID.Data4[4],gSRSelect.gModeID.Data4[5],
		gSRSelect.gModeID.Data4[6],gSRSelect.gModeID.Data4[7]);
	m_EnumOut5.SetWindowText( str );

	wsprintf( str, TEXT("%x %x %x %x%x%x%x%x%x%x%x"),
		gSRSelect.gEngineID.Data1,gSRSelect.gEngineID.Data2,gSRSelect.gEngineID.Data3,
		gSRSelect.gEngineID.Data4[0],gSRSelect.gEngineID.Data4[1],
		gSRSelect.gEngineID.Data4[2],gSRSelect.gEngineID.Data4[3],
		gSRSelect.gEngineID.Data4[4],gSRSelect.gEngineID.Data4[5],
		gSRSelect.gEngineID.Data4[6],gSRSelect.gEngineID.Data4[7]);
	m_EnumOut6.SetWindowText( str );

	if( gSRSelect.dwSequencing == SRSEQUENCE_DISCRETE )
		wsprintf( str, TEXT("DISCRETE") );
	else if( gSRSelect.dwSequencing == SRSEQUENCE_CONTINUOUS )
		wsprintf( str, TEXT("CONTINUOUS") );
	else if( gSRSelect.dwSequencing == SRSEQUENCE_WORDSPOT )
		wsprintf( str, TEXT("WORDSPOT") );
	else
		wsprintf( str, TEXT("UNDEFINED") );
	m_EnumOut7.SetWindowText( str );

	wsprintf( str, TEXT("%ld"), gSRSelect.dwMaxWordsVocab );
	m_EnumOut8.SetWindowText( str );

	wsprintf( str, TEXT("%ld"), gSRSelect.dwMaxWordsState );
	m_EnumOut9.SetWindowText( str );

	wsprintf( str, TEXT("%0lx (flag)"), gSRSelect.dwGrammars );
	m_EnumOut10.SetWindowText( str );

	wsprintf( str, TEXT("%0lx (flag)"), gSRSelect.dwFeatures );
	m_EnumOut11.SetWindowText( str );

	wsprintf( str, TEXT("%0lx (flag)"), gSRSelect.dwInterfaces );
	m_EnumOut12.SetWindowText( str );

	wsprintf( str, TEXT("%0lx (flag)"), gSRSelect.dwEngineFeatures );
	m_EnumOut13.SetWindowText( str );
}

/*
 The user just double-clicked a mode in the mode selection list control.
*/
void CAddEngMode::OnDblclkListenummodes1() 
{
	OnOK();
}

/*
 The user just selected an engine mode by pressing the ok button in the mode selection
 dialog.  Select the mode and init the grammar file selection list box.
*/
void CAddEngMode::OnOK() 
{
	PISRENGOBJ pEng;
	TCHAR buf[MAX_STRLEN], buf1[SRMI_NAMELEN];

	CursorWait;

	// create a new engine object...
	pEng = new ISRENGOBJ;
	if( pEng == NULL )
	{
		_stprintf( buf, TEXT("Error creating engine object.\n") );
		MainOutput( buf );
		MessageBox( buf, TEXT("Error"), MB_OK );
		return;
	}

	if( pEng->FInit( &gSRSelect ) == FALSE )
	{
		_stprintf( buf, TEXT("Error in Engine object FInit.\n") );
		MainOutput( buf );
		MessageBox( buf, TEXT("Error"), MB_OK );
		return;
	}

	// set mode sharing flag...
	pEng->m_bUseShare = m_UseShare.GetCheck();

	// get device ID (used only if sharing not enabled)...
	pEng->m_DeviceID = m_DeviceID.GetCurSel() - 1;   // (a value of -1 is WAVE_MAPPER)

	// associate an engine mode for new object and add it to the engine listbox...
	if( pEng->SelectMode( gpSREnum ) )
	{
		// add the mode to the mode selection list box on the main dialog...
		S2ACVTEX( buf1, gSRSelect.szModeName, SRMI_NAMELEN );
		((CSRTestDlg *)AfxGetMainWnd())->m_EnumList.InsertString( gnModeList, buf1 );
		((CSRTestDlg *)AfxGetMainWnd())->m_EnumList.SetCurSel( gnModeList );

		// associate our object pointer with the listbox item,
		// we'll let the listbox be our list of engine mode pointers...
		((CSRTestDlg *)AfxGetMainWnd())->m_EnumList.SetItemData( gnModeList, (DWORD)pEng );

		// now store the grammar filenames in the ISRCentral prop sheet,
		// just do this when we create the first engine mode...
		if( gnModeList == 0 )
			((CTabISRCentral *)((CSRTestDlg *)AfxGetMainWnd())->m_PropEng->GetPage(0))->EnumGrammars();

		// make the newly added mode the current global pointer (gpEng)...
		((CSRTestDlg *)AfxGetMainWnd())->OnSelchangeListenummodes();

		// enable the property sheet and selection radio buttons...
		((CSRTestDlg *)AfxGetMainWnd())->m_PropEng->EnableWindow( TRUE );
		((CSRTestDlg *)AfxGetMainWnd())->m_SelPropEng.EnableWindow( TRUE );
		((CSRTestDlg *)AfxGetMainWnd())->m_SelPropGram.EnableWindow( TRUE );

		// enable the results property sheet only if the engine supports results objects...
		if( gSRSelect.dwInterfaces & SRI_ISRESBASIC )
			((CSRTestDlg *)AfxGetMainWnd())->m_SelPropRes.EnableWindow( TRUE );

		// make the ISRCentral property sheet visible...
		((CSRTestDlg *)AfxGetMainWnd())->OnSelpropeng();

		gnModeList++;

      gfModeSelected = TRUE;

	}

	CursorGo;

	CDialog::OnOK();
}

/************************************************************************************
 Helper functions
************************************************************************************/

/*
 Enumerate the available grammar files and store them in the grammar listbox in the
 ISRCentral property page.

 The path of the grammar files are taken from 'grammarpath' in registry key:

    HKEY_CURRENT_USER\\Software\\Microsoft\\srtest\\Settings

 If they aren't specified there than a dialog is presented asking for the path.
*/
void CSRTestDlg::EnumGramFiles()
{
	TCHAR buf[MAX_STRLEN], namespec[MAX_STRLEN];
	char namespecA[MAX_STRLEN];
	_finddata_t fdata;
	long hfile;

	gnGramFiles = 0;

	// get the path for the grammar files from the registry,
	// see CSRTestApp::InitInstance() for more info...
	if( gszGramPath[0] == NULL )
	{
		CString GramPathC = AfxGetApp()->GetProfileString( TEXT("Settings"), TEXT("GrammarPath"), NULL );
		_tcscpy( gszGramPath, GramPathC );
	}

	// try to enumerate some grammar files...
	if( gszGramPath[0] )
	{
		_stprintf( namespec, TEXT("%s\\*.grm"), gszGramPath );

		Unicode2Char( namespecA, namespec, sizeof(namespecA) );

		hfile = _findfirst( namespecA, &fdata );
	}	   

	// if that didn't work then give the user a dialog for entering the dir...
	if( gszGramPath[0] == NULL || hfile == -1 )
	{
		CGramPath DlgGramPath;
		DlgGramPath.DoModal();

		if( !gszGramPath[0] )
		{
			MainOutput( TEXT("No grammar path specified.\n") );
			return;
		}
	}

	// enumerate the grammar files in the directory specified...
	_stprintf( namespec, TEXT("%s\\*.grm"), gszGramPath );

	// loop thru files, if there aren't any tell user and leave...
	Unicode2Char( namespecA, namespec, sizeof(namespecA) );

	if( (hfile = _findfirst( namespecA, &fdata )) == -1 )
	{
		_stprintf( buf, TEXT("Grammar files not found in %s.\n"), gszGramPath );
		MessageBox( buf, TEXT("Error"), MB_OK );
		MainOutput( buf );
		gszGramPath[0] = 0;
		return;
	}

	// files exist, add this path to the registry for next time...
	AfxGetApp()->WriteProfileString( TEXT("Settings"), TEXT("GrammarPath"), gszGramPath );

	// enumerate the grammar files...
	gpGramFiles = (GramName_t *)malloc( sizeof(GramName_t) );
	Char2Unicode( gpGramFiles->szName, _strlwr(fdata.name), MAX_STRLEN );
	gnGramFiles++;

	while( _findnext( hfile, &fdata ) == 0 )
	{
		gpGramFiles = (GramName_t *)realloc( gpGramFiles, sizeof(GramName_t)*(gnGramFiles+1) );
		Char2Unicode( gpGramFiles[gnGramFiles].szName, _strlwr(fdata.name), MAX_STRLEN );
		gnGramFiles++;
	}

	_findclose( hfile );
}

/*
 Return a pointer to just the filename from a path and filename.  Ansi version.

 Input:  p - a filename including the path.
*/
LPSTR StripPath( LPSTR p )
{
    int n = strlen( p ) - 1;
    while( n )
    {
        if( *(p+n) == '\\' || *(p+n) == ':' ) return p+n+1;
        n--;
    }
    return p;
}

/*
 Unicode version.
*/
PWCHAR StripPath( PWCHAR p )
{
    int n = (wcslen( p ) - 1) * 2;
	char *pa = (char *)p;
    while( n )
    {
        if( *(pa+n) == '\\' || *(pa+n) == ':' ) return (PWCHAR)(pa+n+2);
        n -= 2;
    }
    return (PWCHAR)pa;
}

/*
 Return a language string given a language ID.

 Input:  LangID - language ID.
*/
PTCHAR LangIDtoString( WORD LangID )
{
	int i;
	for( i=0; i<MAX_LANGUAGES; i++ )
		if( (LangID & 0xFF) == LanguageID[i] ) return Languages[i];

	return NULL;
}

/*
 Return a language ID given a language string.

 Input:  str - language string.
*/
WORD StringtoLangID( LPCTSTR str )
{
	int i;
	for( i=0; i<MAX_LANGUAGES; i++ )
		if( _tcsicmp(str,Languages[i]) == 0 ) return LanguageID[i];

	return NULL;
}

/* Return the index of the given mode, the mode GUID is unique */
int ModeIndex( PSRMODEINFO pSRInfo )
{
	int i;

	for( i=0; i<gdwNmodes; i++ )
	{
		if( memcmp( &pSRInfo->gModeID, &gpSRInfoList[i].gModeID, sizeof(GUID) ) == 0 ) return i;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CGramPath dialog


CGramPath::CGramPath(CWnd* pParent /*=NULL*/)
	: CDialog(CGramPath::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGramPath)
	//}}AFX_DATA_INIT
}


void CGramPath::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGramPath)
	DDX_Control(pDX, IDC_GRAMPATH, m_GramPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGramPath, CDialog)
	//{{AFX_MSG_MAP(CGramPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGramPath message handlers

BOOL CGramPath::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// get the current directory information if we don't have one...
	if( gszGramPath[0] == 0 )
	{
		GetCurrentDirectory( sizeof(gszGramPath), gszGramPath );
	}
		
	m_GramPath.ReplaceSel( gszGramPath );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGramPath::OnOK() 
{
	m_GramPath.GetWindowText( gszGramPath, sizeof(gszGramPath) );

	CDialog::OnOK();
}

/*
 User selected the results radio control, make the results property sheet active/visible.
*/
void CSRTestDlg::OnSelpropres() 
{
	if( gpEng == NULL || gpEng->m_nLoadedGrams == 0 )
	{
		MessageBox( TEXT("You have no grammar loaded, therefore no results to process."), NULL, MB_OK );
		CheckRadioButton( IDC_SELPROPENG, IDC_SELPROPRES, IDC_SELPROPENG );
		return;
	}
	
	if( gpEng->m_pGram->m_ResList.GetCount() < 1 )
	{
		MessageBox( TEXT("You have no results objects to process."), TEXT("Warning"), MB_OK );
	}

	m_PropGram->ShowWindow( SW_HIDE );
	m_PropEng->ShowWindow( SW_HIDE );
	m_PropRes->ShowWindow( SW_SHOW );

	m_ResBasic.OnSetActive();
	InitPropPageSettingsRes();
}
