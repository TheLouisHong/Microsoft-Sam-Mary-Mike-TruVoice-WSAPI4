/******************************************************************************\
*       vtpick.cpp
*
*       Implements functionality of pick dialog used to select TTS
*       engine modes and register dialog used to set up for calling
*       IVoiceText::Register()
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

#include <speech.h>
#include <tchar.h>
#include <bufcvt.h>

#include "resource.h"
#include "vtobj.h"
#include "vtpick.h"

#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// helper function in this module...
LPTSTR LangIDtoString( WORD LangID );

#define MAX_LANGUAGES 27

// language text & ID tables, created from WINNT.h defines and used by LangIDtoString()
LPTSTR Languages[MAX_LANGUAGES]={
TEXT("NEUTRAL"),TEXT("BULGARIAN"),TEXT("CHINESE"),TEXT("CROATIAN"),TEXT("CZECH"),
TEXT("DANISH"),TEXT("DUTCH"),TEXT("ENGLISH"),TEXT("FINNISH"),
TEXT("FRENCH"),TEXT("GERMAN"),TEXT("GREEK"),TEXT("HUNGARIAN"),TEXT("ICELANDIC"),
TEXT("ITALIAN"),TEXT("JAPANESE"),TEXT("KOREAN"),TEXT("NORWEGIAN"),
TEXT("POLISH"),TEXT("PORTUGUESE"),TEXT("ROMANIAN"),TEXT("RUSSIAN"),TEXT("SLOVAK"),
TEXT("SLOVENIAN"),TEXT("SPANISH"),TEXT("SWEDISH"),TEXT("TURKISH")};

WORD LanguageID[MAX_LANGUAGES]={
LANG_NEUTRAL,LANG_BULGARIAN,LANG_CHINESE,LANG_CROATIAN,LANG_CZECH,LANG_DANISH,LANG_DUTCH,
LANG_ENGLISH,LANG_FINNISH,LANG_FRENCH,LANG_GERMAN,LANG_GREEK,LANG_HUNGARIAN,LANG_ICELANDIC,
LANG_ITALIAN,LANG_JAPANESE,LANG_KOREAN,LANG_NORWEGIAN,LANG_POLISH,LANG_PORTUGUESE,
LANG_ROMANIAN,LANG_RUSSIAN,LANG_SLOVAK,LANG_SLOVENIAN,LANG_SPANISH,LANG_SWEDISH,LANG_TURKISH};

/////////////////////////////////////////////////////////////////////////////
// CPickTTSModeDlg dialog


CPickTTSModeDlg::CPickTTSModeDlg(PTTSMODEINFO pttsSelect, CVTTestObject* pVTTestObject, CWnd* pParent)
	: CDialog(CPickTTSModeDlg::IDD, pParent)
{
	m_pVTTestObject = pVTTestObject;
	m_pttsSelect = pttsSelect;
	//{{AFX_DATA_INIT(CPickTTSModeDlg)
	//}}AFX_DATA_INIT
}


void CPickTTSModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPickTTSModeDlg)
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


BEGIN_MESSAGE_MAP(CPickTTSModeDlg, CDialog)
	//{{AFX_MSG_MAP(CPickTTSModeDlg)
	ON_LBN_DBLCLK(IDC_LISTENUMMODES1, OnDblclkListenummodes1)
	ON_LBN_SELCHANGE(IDC_LISTENUMMODES1, OnSelchangeListenummodes1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRegisterDlg dialog


CRegisterDlg::CRegisterDlg(PVTENGOBJ pVTEngObj, CVTTestObject* pVTTestObject, CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDlg::IDD, pParent)
{
	m_pVTEngObj = pVTEngObj;
	m_pVTTestObject = pVTTestObject;
	//{{AFX_DATA_INIT(CRegisterDlg)
	m_editDWDevice = 0;
	m_editDWSpeed = 0;
	//}}AFX_DATA_INIT
}


void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegisterDlg)
	DDX_Control(pDX, IDC_STATIC_MODENAME, m_staticModeName);
	DDX_Control(pDX, IDC_STATIC_WPM, m_staticWPM);
	DDX_Control(pDX, IDC_STATIC_GMODEID, m_staticgModeID);
	DDX_Control(pDX, IDC_STATIC_DWSPEED, m_staticdwSpeed);
	DDX_Control(pDX, IDC_STATIC_DWDEVICE, m_staticdwDevice);
	DDX_Control(pDX, IDC_EDIT_MODENAME, m_editctrlModeName);
	DDX_Control(pDX, IDC_BUTTON_SETMODE, m_buttonSetMode);
	DDX_Control(pDX, IDC_BUTTON_SPEEDMIN, m_buttonSpeedMin);
	DDX_Control(pDX, IDC_BUTTON_SPEEDMAX, m_buttonSpeedMax);
	DDX_Control(pDX, IDC_EDIT_DEVICEID, m_editctrlDeviceID);
	DDX_Control(pDX, IDC_RADIO_DEVICEID, m_radioDeviceID);
	DDX_Control(pDX, IDC_RADIO_MAPPER, m_radioMapper);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_editctrlSpeed);
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnable);
	DDX_Control(pDX, IDC_CHECK_NOTIFY, m_checkNotify);
	DDX_Control(pDX, IDC_EDIT_SITE, m_editSite);
	DDX_Text(pDX, IDC_EDIT_DEVICEID, m_editDWDevice);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_editDWSpeed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialog)
	//{{AFX_MSG_MAP(CRegisterDlg)
	ON_BN_CLICKED(IDC_BUTTON_SETMODE, OnButtonSetMode)
	ON_BN_CLICKED(IDC_BUTTON_SPEEDMAX, OnButtonSpeedMax)
	ON_BN_CLICKED(IDC_BUTTON_SPEEDMIN, OnButtonSpeedMin)
	ON_BN_CLICKED(IDC_RADIO_SITENULL, OnRadioSiteNULL)
	ON_BN_CLICKED(IDC_RADIO_SITEINFO, OnRadioSiteInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPickTTSModeDlg message handlers


// Create a TTS Enumerator object and stuff info in list box
BOOL CPickTTSModeDlg::OnInitDialog() 
{
	LOCALLOC(str, TTSI_NAMELEN);
	int i;

	CDialog::OnInitDialog();
	
	if (m_pVTTestObject->CreateEnumerator() == ERROR_SUCCESS)
	{
		// store the enumerated modes in the list box...
		for( i=0; i<m_pVTTestObject->m_nmodes; i++ )
		{
			LOCS2ACVT(str, (m_pVTTestObject->m_aTTSInfo)[i].szModeName);
			m_EnumList.InsertString( i, str );
		}

		// select the first mode and display it...
		m_EnumList.SetCurSel( 0 );
		OnSelchangeListenummodes1();
	}
	else
	{
		m_EnumList.InsertString( 0, TEXT("No Engine Modes Available.") );
	}

	LOCFREE(str);

	return TRUE;  // return TRUE unless you set the focus to a control
}

// Update static controls to display selected mode's TTSMODEINFO
void CPickTTSModeDlg::OnSelchangeListenummodes1() 
{
	if (m_pVTTestObject->m_nmodes == 0)
		return;

#define LENENGSTR 128
	TCHAR appStr[LENENGSTR];

	*m_pttsSelect = (m_pVTTestObject->m_aTTSInfo)[m_EnumList.GetCurSel()];

	S2ACVTEX(appStr, m_pttsSelect->szModeName, LENENGSTR);
	m_EnumOut1.SetWindowText( appStr );

	S2ACVTEX(appStr, m_pttsSelect->szMfgName, LENENGSTR);
	m_EnumOut2.SetWindowText( appStr );

	_stprintf( appStr, TEXT("%s"), LangIDtoString( m_pttsSelect->language.LanguageID ) );
	m_EnumOut3.SetWindowText( appStr );

	S2ACVTEX(appStr, m_pttsSelect->szProductName, LENENGSTR);
	m_EnumOut4.SetWindowText( appStr );

	OLECHAR oleStr[80];
	// Display Mode GUID
	StringFromGUID2(m_pttsSelect->gModeID, oleStr, sizeof(oleStr));
#if defined(_S_UNICODE)
	// do it the easy way, OLECHAR same width as API chars...
	S2ACVTEX(appStr, oleStr, LENENGSTR);
	m_EnumOut5.SetWindowText( appStr );
#else
	// we don't have macros for OLECHAR (always WCHAR) to app width
#if (defined(_UNICODE) || defined(UNICODE))
	// Unicode app is easy
	m_EnumOut5.SetWindowText(oleStr);
#else
	// ANSI app needs conversion, TCHAR = CHAR
	WideCharToMultiByte(CP_ACP, 0, (WCHAR*) &oleStr, -1, (CHAR*) &appStr, LENENGSTR, NULL, NULL);
	m_EnumOut5.SetWindowText( appStr );
#endif // (defined(_UNICODE) || defined(UNICODE))
#endif // defined(_S_UNICODE)

	// Display Engine GUID
	StringFromGUID2(m_pttsSelect->gEngineID, oleStr, sizeof(oleStr));
#if defined(_S_UNICODE)
	S2ACVTEX(appStr, oleStr, LENENGSTR);
	m_EnumOut6.SetWindowText( appStr );
#else
	// we don't have macros for OLECHAR (always WCHAR) to app width
#if (defined(_UNICODE) || defined(UNICODE))
	// Unicode app is easy
	m_EnumOut6.SetWindowText(oleStr);
#else
	// ANSI app needs conversion, TCHAR = CHAR
	WideCharToMultiByte(CP_ACP, 0, (WCHAR*) &oleStr, -1, (CHAR*) &appStr, LENENGSTR, NULL, NULL);
	m_EnumOut6.SetWindowText( appStr );
#endif // (defined(_UNICODE) || defined(UNICODE))
#endif // defined(_S_UNICODE)

	S2ACVTEX(appStr, m_pttsSelect->szSpeaker, LENENGSTR);
	m_EnumOut7.SetWindowText( appStr );

	S2ACVTEX(appStr, m_pttsSelect->szStyle, LENENGSTR);
	m_EnumOut8.SetWindowText( appStr );

	switch(m_pttsSelect->wGender)
	{
		case GENDER_FEMALE:
			_stprintf(appStr, TEXT("%s"), TEXT("GENDER_FEMALE"));
			break;
		case GENDER_MALE:
			_stprintf(appStr, TEXT("%s"), TEXT("GENDER_MALE"));
			break;
		case GENDER_NEUTRAL:
			_stprintf(appStr, TEXT("%s"), TEXT("GENDER_NEUTRAL"));
			break;
		default:
			_stprintf(appStr, TEXT("%s"), TEXT("Error!"));
	}
	m_EnumOut9.SetWindowText( appStr );

	switch(m_pttsSelect->wAge)
	{
		case TTSAGE_BABY:
			_stprintf(appStr, TEXT("%s"), TEXT("TTSAGE_BABY"));
			break;
		case TTSAGE_TODDLER:
			_stprintf(appStr, TEXT("%s"), TEXT("TTSAGE_TODDLER"));
			break;
		case TTSAGE_CHILD:
			_stprintf(appStr, TEXT("%s"), TEXT("TTSAGE_CHILD"));
			break;
		case TTSAGE_ADOLESCENT:
			_stprintf(appStr, TEXT("%s"), TEXT("TTSAGE_ADOLESCENT"));
			break;
		case TTSAGE_ADULT:
			_stprintf(appStr, TEXT("%s"), TEXT("TTSAGE_ADULT"));
			break;
		case TTSAGE_ELDERLY:
			_stprintf(appStr, TEXT("%s"), TEXT("TTSAGE_ELDERLY"));
			break;
		default:
			_stprintf(appStr, TEXT("%s"), TEXT("Error!"));
	}
	m_EnumOut10.SetWindowText(appStr);

	_stprintf( appStr, TEXT("%0lx (flag)"), (DWORD) m_pttsSelect->dwFeatures );
	m_EnumOut11.SetWindowText( appStr );

	_stprintf( appStr, TEXT("%0lx (flag)"), (DWORD) m_pttsSelect->dwInterfaces );
	m_EnumOut12.SetWindowText( appStr );

	_stprintf( appStr, TEXT("%0lx (flag)"), (DWORD) m_pttsSelect->dwEngineFeatures );
	m_EnumOut13.SetWindowText( appStr );
}

// same as clicking OK button
void CPickTTSModeDlg::OnDblclkListenummodes1() 
{
	OnOK();
}

// default implementation
void CPickTTSModeDlg::OnOK() 
{
	CDialog::OnOK();
}

/************************************************************************************
 Helper function
************************************************************************************/

// Return a descriptive string given the language ID
LPTSTR LangIDtoString( WORD LangID )
{
	int i;
	for( i=0; i<MAX_LANGUAGES; i++ )
		if( (LangID & 0xFF) == LanguageID[i] ) return Languages[i];

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CRegisterDlg message handlers

// Bring up dialog to allow user to select TTS mode
void CRegisterDlg::OnButtonSetMode() 
{
	// bring up pick TTS mode dialog
	CPickTTSModeDlg pickDlg(&(m_pVTTestObject->m_regInfo.ttsSelect), m_pVTTestObject);
	
	int ret = pickDlg.DoModal();
	if(ret == IDOK)
	{
		// save selected GUID and convert szModeName string
		m_pVTTestObject->m_regInfo.siteInfo.gModeID = m_pVTTestObject->m_regInfo.ttsSelect.gModeID;

		TCHAR* pStrBuf = m_pVTTestObject->m_regInfo.modeNameStr.GetBuffer(BUFLEN);
		S2ACVTEX(pStrBuf, m_pVTTestObject->m_regInfo.ttsSelect.szModeName, BUFLEN);
		m_pVTTestObject->m_regInfo.modeNameStr.ReleaseBuffer();
	}
	else
	{
		// save a zero'ed out GUID and get informative message
		GUID gZero;
		memset(&gZero, 0, sizeof(gZero));
		m_pVTTestObject->m_regInfo.siteInfo.gModeID = gZero;

		m_pVTTestObject->m_regInfo.modeNameStr.LoadString(IDS_NOMODE);
	}
	m_editctrlModeName.SetWindowText(m_pVTTestObject->m_regInfo.modeNameStr);
}

// Initialize dialog controls with data from m_pVTTestObject->m_regInfo
BOOL CRegisterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//	pszSite
	m_editSite.SetWindowText(m_pVTTestObject->m_regInfo.siteStr);
	if(m_pVTTestObject->m_regInfo.bUseSiteStr)
	{
		CheckRadioButton(IDC_RADIO_SITESTRNULL, IDC_RADIO_SITESTREDIT, IDC_RADIO_SITESTREDIT);
	}
	else
	{
		CheckRadioButton(IDC_RADIO_SITESTRNULL, IDC_RADIO_SITESTREDIT, IDC_RADIO_SITESTRNULL);
	}

	// pNotifyInterface
	if(m_pVTTestObject->m_regInfo.bRegNotifySink)
	{
		m_checkNotify.SetCheck(1);
	}
	else
	{
		m_checkNotify.SetCheck(0);
	}

	// dwFlags
	if(m_pVTTestObject->m_regInfo.bAllMsgsFlg)
	{
		CheckRadioButton(IDC_RADIO_ALLMSGSFLG, IDC_RADIO_ZEROFLG, IDC_RADIO_ALLMSGSFLG);
	}
	else
	{
		CheckRadioButton(IDC_RADIO_ALLMSGSFLG, IDC_RADIO_ZEROFLG, IDC_RADIO_ZEROFLG);
	}

	// pSiteInfo
	if(m_pVTTestObject->m_regInfo.bUseSiteInfo)
	{
		CheckRadioButton(IDC_RADIO_SITENULL, IDC_RADIO_SITEINFO, IDC_RADIO_SITEINFO);
		OnRadioSiteInfo();
	}
	else
	{
		CheckRadioButton(IDC_RADIO_SITENULL, IDC_RADIO_SITEINFO, IDC_RADIO_SITENULL);
		OnRadioSiteNULL();
	}

	// m_pVTTestObject->m_regInfo.siteInfo.dwDevice 
	m_editDWDevice = m_pVTTestObject->m_regInfo.siteInfo.dwDevice;
	UpdateData(FALSE);
	if(m_pVTTestObject->m_regInfo.bUseMapper)
	{
		CheckRadioButton(IDC_RADIO_MAPPER, IDC_RADIO_DEVICEID, IDC_RADIO_MAPPER);
	}
	else
	{
		CheckRadioButton(IDC_RADIO_MAPPER, IDC_RADIO_DEVICEID, IDC_RADIO_DEVICEID);
	}

	// m_pVTTestObject->m_regInfo.siteInfo.dwEnable 
	if(m_pVTTestObject->m_regInfo.siteInfo.dwEnable)
	{
		m_checkEnable.SetCheck(1);
	}
	else
	{
		m_checkEnable.SetCheck(0);
	}

	// m_pVTTestObject->m_regInfo.siteInfo.dwSpeed 
	m_editDWSpeed = m_pVTTestObject->m_regInfo.siteInfo.dwSpeed;
	UpdateData(FALSE);

	//m_regInfo.siteInfo.gModeID 
	m_pVTTestObject->m_regInfo.siteInfo.gModeID = m_pVTTestObject->m_regInfo.ttsSelect.gModeID;
	m_editctrlModeName.SetWindowText(m_pVTTestObject->m_regInfo.modeNameStr);

	return TRUE;  // return TRUE unless you set the focus to a control
}

// Get user edited data from dialog and stuff in m_pVTTestObject->m_regInfo
void CRegisterDlg::OnOK() 
{
	// Copy data from selected edit controls into m_ data members
	UpdateData(TRUE);

	//	pszSite
	m_editSite.GetWindowText(m_pVTTestObject->m_regInfo.siteStr); // save the string
	m_pVTTestObject->m_regInfo.bUseSiteStr = (GetCheckedRadioButton(IDC_RADIO_SITESTRNULL, IDC_RADIO_SITESTREDIT)
												== IDC_RADIO_SITESTREDIT);
	// pNotifyInterface
	m_pVTTestObject->m_regInfo.bRegNotifySink = (m_checkNotify.GetCheck() == 1);

	// dwFlags
	m_pVTTestObject->m_regInfo.bAllMsgsFlg = (GetCheckedRadioButton(IDC_RADIO_ALLMSGSFLG, IDC_RADIO_ZEROFLG)
												== IDC_RADIO_ALLMSGSFLG);

	// pSiteInfo
	m_pVTTestObject->m_regInfo.bUseSiteInfo = (GetCheckedRadioButton(IDC_RADIO_SITENULL, IDC_RADIO_SITEINFO)
												== IDC_RADIO_SITEINFO);

	if(m_pVTTestObject->m_regInfo.bUseSiteInfo)
	{
		GUID gZero;
		memset(&gZero, 0, sizeof(gZero));

		if(IsEqualGUID(m_pVTTestObject->m_regInfo.siteInfo.gModeID, gZero))
		{
			MessageBox(TEXT("You must select a mode if you wish to pass the VTSITEINFO structure to IVoiceText::Register()"),
							TEXT("Incomplete VTSITEINFO Structure"), MB_ICONSTOP);
		 	return;
		}
	}
	//m_regInfo.siteInfo.dwDevice 
	m_pVTTestObject->m_regInfo.bUseMapper = (GetCheckedRadioButton(IDC_RADIO_MAPPER, IDC_RADIO_DEVICEID)
												== IDC_RADIO_MAPPER);
	m_pVTTestObject->m_regInfo.siteInfo.dwDevice = m_editDWDevice;

	//m_regInfo.siteInfo.dwEnable 
	m_pVTTestObject->m_regInfo.siteInfo.dwEnable = (m_checkEnable.GetCheck() == 1);

	//m_regInfo.siteInfo.dwSpeed
	m_pVTTestObject->m_regInfo.siteInfo.dwSpeed = m_editDWSpeed;
	 
	//m_regInfo.siteInfo.gModeID 
	m_pVTTestObject->m_regInfo.siteInfo.gModeID = m_pVTTestObject->m_regInfo.ttsSelect.gModeID;
		
	CDialog::OnOK();
}

// Set dwSpeed = TTSATTR_MAXSPEED and display in edit control
void CRegisterDlg::OnButtonSpeedMax() 
{
	m_editDWSpeed = m_pVTTestObject->m_regInfo.siteInfo.dwSpeed = TTSATTR_MAXSPEED;
	TCHAR buf[BUFLEN];
	_stprintf(buf, TEXT("%lu"), (DWORD) m_editDWSpeed);
	m_editctrlSpeed.SetWindowText(buf);
}

// Set dwSpeed = TTSATTR_MINSPEED and display in edit control
void CRegisterDlg::OnButtonSpeedMin() 
{
	m_editDWSpeed = m_pVTTestObject->m_regInfo.siteInfo.dwSpeed = TTSATTR_MINSPEED;
	TCHAR buf[BUFLEN];
	_stprintf(buf, TEXT("%lu"), (DWORD) m_editDWSpeed);
	m_editctrlSpeed.SetWindowText(buf);
}

// Disable the controls used to edit VTSITEINFO members
void CRegisterDlg::OnRadioSiteNULL() 
{
	// Enable site member controls
	m_radioMapper.EnableWindow(FALSE);
	m_radioDeviceID.EnableWindow(FALSE);
	m_editctrlDeviceID.EnableWindow(FALSE);
	m_checkEnable.EnableWindow(FALSE);
	m_editctrlSpeed.EnableWindow(FALSE);
	m_buttonSpeedMax.EnableWindow(FALSE);
	m_buttonSpeedMin.EnableWindow(FALSE);
	m_buttonSetMode.EnableWindow(FALSE);
	m_editctrlModeName.EnableWindow(FALSE);
	m_staticdwDevice.EnableWindow(FALSE);
	m_staticdwSpeed.EnableWindow(FALSE);
	m_staticgModeID.EnableWindow(FALSE);
	m_staticWPM.EnableWindow(FALSE);
	m_staticModeName.EnableWindow(FALSE);
}

// Enable the controls used to edit VTSITEINFO members
void CRegisterDlg::OnRadioSiteInfo() 
{
	// Disable site member controls
	m_radioMapper.EnableWindow(TRUE);
	m_radioDeviceID.EnableWindow(TRUE);
	m_editctrlDeviceID.EnableWindow(TRUE);
	m_checkEnable.EnableWindow(TRUE);
	m_editctrlSpeed.EnableWindow(TRUE);
	m_buttonSpeedMax.EnableWindow(TRUE);
	m_buttonSpeedMin.EnableWindow(TRUE);
	m_buttonSetMode.EnableWindow(TRUE);
	m_editctrlModeName.EnableWindow(TRUE);
	m_staticdwDevice.EnableWindow(TRUE);
	m_staticdwSpeed.EnableWindow(TRUE);
	m_staticgModeID.EnableWindow(TRUE);
	m_staticWPM.EnableWindow(TRUE);
	m_staticModeName.EnableWindow(TRUE);
}
