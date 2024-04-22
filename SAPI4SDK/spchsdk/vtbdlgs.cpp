/******************************************************************************\
*       tabdlgs.cpp
*
*       Implements functionality of CPropertyPage derived classes displayed
*       as tabs on the main application window.
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
#include <bufcvt.h>
#include "vtobj.h"

class CVTTestDlg;
class CVTTestObject;  
class CVTEngineObject;

#include "resource.h"
#include "vtpick.h"
#include "tabdlgs.h"

#include "vttesdlg.h"

/////////////////////////////////////////////////////////////////////////////
// CTabIVoiceText property page

IMPLEMENT_DYNCREATE(CTabIVoiceText, CPropertyPage)

CTabIVoiceText::CTabIVoiceText() : CPropertyPage(CTabIVoiceText::IDD)
{
	//{{AFX_DATA_INIT(CTabIVoiceText)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTabIVoiceText::~CTabIVoiceText()
{
}

void CTabIVoiceText::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabIVoiceText)
	DDX_Control(pDX, IDC_EDIT_TAGS, m_editTagData);
	DDX_Control(pDX, IDC_ENGCTLPAUSE, m_buttonCheckPaused);
	DDX_Control(pDX, IDC_EDIT_SPEAKDATA, m_editSpeakData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabIVoiceText, CPropertyPage)
	//{{AFX_MSG_MAP(CTabIVoiceText)
	ON_BN_CLICKED(IDC_BUTTON_SPEAK, OnButtonSpeak)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStopSpeaking)
	ON_BN_CLICKED(IDC_BUTTON_HIGH, OnButtonHigh)
	ON_BN_CLICKED(IDC_BUTTON_VERYHIGH, OnButtonVeryHigh)
	ON_BN_CLICKED(IDC_BUTTON_FF, OnButtonFastForward)
	ON_BN_CLICKED(IDC_BUTTON_REWIND, OnButtonRewind)
	ON_BN_CLICKED(IDC_ENGCTLPAUSE, OnCheckPauseResume)
	ON_BN_CLICKED(IDC_BUTTON_RELENUM, OnButtonReleaseEnum)
	ON_BN_CLICKED(IDC_REGISTER, OnRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTabIVTxtDialogs property page

IMPLEMENT_DYNCREATE(CTabIVTxtDialogs, CPropertyPage)

CTabIVTxtDialogs::CTabIVTxtDialogs() : CPropertyPage(CTabIVTxtDialogs::IDD)
{
	//{{AFX_DATA_INIT(CTabIVTxtDialogs)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTabIVTxtDialogs::~CTabIVTxtDialogs()
{
}

void CTabIVTxtDialogs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabIVTxtDialogs)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabIVTxtDialogs, CPropertyPage)
	//{{AFX_MSG_MAP(CTabIVTxtDialogs)
	ON_BN_CLICKED(IDC_VTGENERALDLG, OnVTGeneralDlg)
	ON_BN_CLICKED(IDC_VTLEXICONDLG, OnVTLexiconDlg)
	ON_BN_CLICKED(IDC_VTTRANSLATEDLG, OnVTTranslateDlg)
	ON_BN_CLICKED(IDC_VTABOUTDLG, OnVTAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTabIVTxtAttributes property page

IMPLEMENT_DYNCREATE(CTabIVTxtAttributes, CPropertyPage)

CTabIVTxtAttributes::CTabIVTxtAttributes() : CPropertyPage(CTabIVTxtAttributes::IDD)
{
	//{{AFX_DATA_INIT(CTabIVTxtAttributes)
	//}}AFX_DATA_INIT
}

CTabIVTxtAttributes::~CTabIVTxtAttributes()
{
}

void CTabIVTxtAttributes::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabIVTxtAttributes)
	DDX_Control(pDX, IDC_EDIT_MODEGET, m_editTTSMode);
	DDX_Control(pDX, IDC_EDIT_DEVICE, m_editDeviceID);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_editSpeed);
	DDX_Control(pDX, IDC_EDIT_ISSPEAKING, m_editIsSpeaking);
	DDX_Control(pDX, IDC_EDIT_ENABLED, m_editEnabled);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabIVTxtAttributes, CPropertyPage)
	//{{AFX_MSG_MAP(CTabIVTxtAttributes)
	ON_BN_CLICKED(IDC_BUTTON_ISSPEAKING, OnButtonIsSpeaking)
	ON_BN_CLICKED(IDC_BUTTON_SPEEDGET, OnButtonSpeedGet)
	ON_BN_CLICKED(IDC_BUTTON_SPEEDSET, OnButtonSpeedSet)
	ON_BN_CLICKED(IDC_BUTTON_DEVICEGET, OnButtonDeviceGet)
	ON_BN_CLICKED(IDC_BUTTON_DEVICESET, OnButtonDeviceSet)
	ON_BN_CLICKED(IDC_BUTTON_ENABLEDGET, OnButtonEnabledGet)
	ON_BN_CLICKED(IDC_BUTTON_ENABLEDSET, OnButtonEnabledSet)
	ON_BN_CLICKED(IDC_BUTTON_TTSMODEGET, OnButtonTTSModeGet)
	ON_BN_CLICKED(IDC_BUTTON_SELECTEDMODE, OnButtonSetSelectedMode)
	ON_BN_CLICKED(IDC_BUTTON_BADMODE, OnButtonSetBadMode)
	ON_BN_CLICKED(IDC_BUTTON_SPEEDMIN, OnButtonSpeedMin)
	ON_BN_CLICKED(IDC_BUTTON_SPEEDMAX, OnButtonSpeedMax)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CTabIVTxtNotifySink property page

IMPLEMENT_DYNCREATE(CTabIVTxtNotifySink, CPropertyPage)

CTabIVTxtNotifySink::CTabIVTxtNotifySink() : CPropertyPage(CTabIVTxtNotifySink::IDD)
{
	//{{AFX_DATA_INIT(CTabIVTxtNotifySink)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTabIVTxtNotifySink::~CTabIVTxtNotifySink()
{
}

void CTabIVTxtNotifySink::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabIVTxtNotifySink)
	DDX_Control(pDX, IDC_NOTIFYVISUAL, m_visual);
	DDX_Control(pDX, IDC_NOTIFYSPEAKSTART, m_speakStart);
	DDX_Control(pDX, IDC_NOTIFYSPEAKDONE, m_speakDone);
	DDX_Control(pDX, IDC_NOTIFYSPEAK, m_speak);
	DDX_Control(pDX, IDC_NOTIFYATTRIBCHANGED, m_attribChanged);
	DDX_Control(pDX, IDC_NOTIFYOUTPUT, m_editNotifications);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabIVTxtNotifySink, CPropertyPage)
	//{{AFX_MSG_MAP(CTabIVTxtNotifySink)
	ON_BN_CLICKED(IDC_NOTIFYATTRIBCHANGED, OnNotifyAttribChanged)
	ON_BN_CLICKED(IDC_NOTIFYSPEAK, OnNotifySpeak)
	ON_BN_CLICKED(IDC_NOTIFYSPEAKDONE, OnNotifySpeakDone)
	ON_BN_CLICKED(IDC_NOTIFYSPEAKSTART, OnNotifySpeakStart)
	ON_BN_CLICKED(IDC_NOTIFYVISUAL, OnNotifyVisual)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTabIVTxtNotifySink message handlers

// Write notification strings to edit control
void CTabIVTxtNotifySink::NotifyOutput(LPCTSTR pLogStr)
{
	if(m_editNotifications.m_hWnd == NULL)
		return; // edit control isn't created until tab is selected

	m_editNotifications.ReplaceSel(pLogStr);
	m_editNotifications.ReplaceSel(TEXT("\r\n"));
}

// Refresh the attributes check boxes when notify tab is clicked
BOOL CTabIVTxtNotifySink::OnSetActive() 
{
	if(!CPropertyPage::OnSetActive())
		return FALSE;

	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return TRUE;
	}

	m_attribChanged.SetCheck(pMainDlg->m_pVTEngObj->m_notifyFlgs.bAttribChanged);
	m_speak.SetCheck(pMainDlg->m_pVTEngObj->m_notifyFlgs.bSpeak);
	m_speakStart.SetCheck(pMainDlg->m_pVTEngObj->m_notifyFlgs.bSpeakingStarted);
	m_speakDone.SetCheck(pMainDlg->m_pVTEngObj->m_notifyFlgs.bSpeakingDone);
	m_visual.SetCheck(pMainDlg->m_pVTEngObj->m_notifyFlgs.bVisual);
	return TRUE;
}

// Update m_pVTEngObj->m_notifyFlgs with state of m_attribChanged checkbox
void CTabIVTxtNotifySink::OnNotifyAttribChanged() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	pMainDlg->m_pVTEngObj->m_notifyFlgs.bAttribChanged = m_attribChanged.GetCheck() == 1;
}

// Update m_pVTEngObj->m_notifyFlgs with state of m_speak checkbox
void CTabIVTxtNotifySink::OnNotifySpeak() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	pMainDlg->m_pVTEngObj->m_notifyFlgs.bSpeak = m_speak.GetCheck() == 1;
}

// Update m_pVTEngObj->m_notifyFlgs with state of m_speakDone checkbox
void CTabIVTxtNotifySink::OnNotifySpeakDone() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	pMainDlg->m_pVTEngObj->m_notifyFlgs.bSpeakingDone = m_speakDone.GetCheck() == 1;
}

// Update m_pVTEngObj->m_notifyFlgs with state of m_speakStart checkbox
void CTabIVTxtNotifySink::OnNotifySpeakStart() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	pMainDlg->m_pVTEngObj->m_notifyFlgs.bSpeakingStarted = m_speakStart.GetCheck() == 1;
}

// Update m_pVTEngObj->m_notifyFlgs with state of m_visual checkbox
void CTabIVTxtNotifySink::OnNotifyVisual() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	pMainDlg->m_pVTEngObj->m_notifyFlgs.bVisual = m_visual.GetCheck() == 1;
}


/////////////////////////////////////////////////////////////////////////////
// CTabIVTxtDialogs message handlers

// Call GeneralDlg() after checking for support
void CTabIVTxtDialogs::OnVTGeneralDlg() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtDialogs)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtDialogs->GeneralDlg(NULL, NULL);
	if(SUCCEEDED(hRes))
	{
		hRes = pMainDlg->m_pVTEngObj->m_pIVTxtDialogs->GeneralDlg(AfxGetMainWnd()->m_hWnd, NULL);
	}
	else
	{
		LogOutput(TEXT("GeneralDlg() not supported on this engine."));
	}
}

// Call LexiconDlg() after checking for support
void CTabIVTxtDialogs::OnVTLexiconDlg() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if (!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtDialogs)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtDialogs->LexiconDlg(NULL, NULL);
	if(SUCCEEDED(hRes))
	{
		hRes = pMainDlg->m_pVTEngObj->m_pIVTxtDialogs->LexiconDlg(AfxGetMainWnd()->m_hWnd, NULL);
	}
	else
	{
		LogOutput(TEXT("LexiconDlg() not supported on this engine."));
	}
}

// Call TranslateDlg() after checking for support
void CTabIVTxtDialogs::OnVTTranslateDlg() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if (!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtDialogs)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtDialogs->TranslateDlg(NULL, NULL);
	if(SUCCEEDED(hRes))
	{
		hRes = pMainDlg->m_pVTEngObj->m_pIVTxtDialogs->TranslateDlg(AfxGetMainWnd()->m_hWnd, NULL);
	}
	else
	{
		LogOutput(TEXT("TranslateDlg() not supported on this engine."));
	}
}

// Call AboutDlg() after checking for support
void CTabIVTxtDialogs::OnVTAboutDlg() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if (!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtDialogs)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtDialogs->AboutDlg(NULL, NULL);
	if(SUCCEEDED(hRes))
	{
		hRes = pMainDlg->m_pVTEngObj->m_pIVTxtDialogs->AboutDlg(AfxGetMainWnd()->m_hWnd, NULL);
	}
	else
	{
		LogOutput(TEXT("AboutDlg() not supported on this engine."));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTabIVoiceText message handlers

// Call Speak() with normal priority and optional tag string logging failure
void CTabIVoiceText::OnButtonSpeak() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVoiceText)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	CString speakStr;
	CString tagStr;
	m_editSpeakData.GetWindowText(speakStr);
	m_editTagData.GetWindowText(tagStr);

	APIALLOC(pOutStr, speakStr.GetLength());
	APIA2SCVT(pOutStr, speakStr);

	HRESULT hRes;
	if(tagStr.GetLength() != 0)
	{
		APIALLOC(pOutTag, tagStr.GetLength());
		APIA2SCVT(pOutTag, tagStr);
		hRes = pMainDlg->m_pVTEngObj->m_pIVoiceText->Speak(pOutStr, 0, pOutTag);
		APIFREE(pOutTag);                                                                                                      
	}
	else
	{
		hRes = pMainDlg->m_pVTEngObj->m_pIVoiceText->Speak(pOutStr, 0, NULL);
	}
	                                                                                                      
	if(FAILED(hRes))
	{
		TCHAR buf[BUFLEN];
		_stprintf(buf, TEXT("IVoiceText::Speak() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}

	APIFREE(pOutStr);
}

// Call StopSpeaking() logging failure
void CTabIVoiceText::OnButtonStopSpeaking() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVoiceText)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVoiceText->StopSpeaking();
	if(FAILED(hRes))
	{
		TCHAR buf[BUFLEN];
		_stprintf(buf, TEXT("IVoiceText::StopSpeaking() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
}

// Call Speak(...VTXTSP_HIGH...) with a canned phrase
void CTabIVoiceText::OnButtonHigh() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVoiceText)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	CString speakStr = TEXT("This was spoken with high priority.");

	APIALLOC(pOutStr, speakStr.GetLength());
	APIA2SCVT(pOutStr, speakStr);

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVoiceText->Speak(pOutStr, VTXTSP_HIGH, NULL);
	if(FAILED(hRes))
	{
		TCHAR buf[BUFLEN];
		_stprintf(buf, TEXT("IVoiceText::Speak(...,VTXTSP_HIGH,...) failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}

	APIFREE(pOutStr);                                                                                                      
}

// Call Speak(...VTXTSP_VERYHIGH...) with a canned phrase
void CTabIVoiceText::OnButtonVeryHigh() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVoiceText)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	CString speakStr = TEXT("This was spoken with very high priority.");

	APIALLOC(pOutStr, speakStr.GetLength());
	APIA2SCVT(pOutStr, speakStr);

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVoiceText->Speak(pOutStr, VTXTSP_VERYHIGH, NULL);
	if(FAILED(hRes))
	{
		TCHAR buf[BUFLEN];
		_stprintf(buf, TEXT("IVoiceText::Speak(...,VTXTSP_VERYHIGH,...) failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}

	APIFREE(pOutStr);                                                                                                      
}

// Call AudioFastForward() logging failure
void CTabIVoiceText::OnButtonFastForward() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVoiceText)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVoiceText->AudioFastForward();
	if(FAILED(hRes))
	{
		TCHAR buf[BUFLEN];
		_stprintf(buf, TEXT("IVoiceText::AudioFastForward() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
}

// Call AudioRewind() logging failure
void CTabIVoiceText::OnButtonRewind() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVoiceText)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVoiceText->AudioRewind();
	if(FAILED(hRes))
	{
		TCHAR buf[BUFLEN];
		_stprintf(buf, TEXT("IVoiceText::AudioRewind() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
}

// Call AudioAPause() logging result
void CTabIVoiceText::OnCheckPauseResume() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVoiceText)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	TCHAR tmpstr[80];
	HRESULT hRes;

	// Pause the engine based on current checkbox state...
	if(m_buttonCheckPaused.GetCheck())
	{
		hRes = pMainDlg->m_pVTEngObj->m_pIVoiceText->AudioPause();
		if(FAILED(hRes)) _stprintf( tmpstr, TEXT("Error Pausing audio."));
		else _stprintf( tmpstr, TEXT("Audio paused."));
	}
	else
	{
		hRes = pMainDlg->m_pVTEngObj->m_pIVoiceText->AudioResume();
		if(FAILED(hRes)) _stprintf( tmpstr, TEXT("Error Resuming audio."));
		else _stprintf( tmpstr, TEXT("Audio resumed."));
	}

	LogOutput(tmpstr);
}

// Call m_pVTTestObj->ReleaseEnumerator() and log success
// (i.e. there was an enumerator to release)
void CTabIVoiceText::OnButtonReleaseEnum() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTTestObj)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	if(pMainDlg->m_pVTTestObj->ReleaseEnumerator() == ERROR_SUCCESS)
	{
		LogOutput(TEXT("TTS Enumerator object released!"));
	}
}

// Call IVoiceText::Register() via routine in CVTTestDlg dialog
void CTabIVoiceText::OnRegister() 
{
	GetMainDlg()->DoRegister();
}

/////////////////////////////////////////////////////////////////////////////
// CTabIVTxtAttributes message handlers

// Call IsSpeaking() and display BOOL return in edit control
void CTabIVTxtAttributes::OnButtonIsSpeaking() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtAttributes)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	TCHAR buf[BUFLEN];
	BOOL fSpeaking;

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtAttributes->IsSpeaking(&fSpeaking);
	if(FAILED(hRes))
	{
		_stprintf(buf, TEXT("IVTxtAttributes::IsSpeaking() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
	else
	{
		m_editIsSpeaking.SetSel( 0, -1 );
		m_editIsSpeaking.ReplaceSel( fSpeaking ? TEXT("TRUE") : TEXT("FALSE"));
	}
}

// Call SpeedGet() and display return in edit control
void CTabIVTxtAttributes::OnButtonSpeedGet() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtAttributes)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	TCHAR buf[BUFLEN];
	DWORD dwSpeed;

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtAttributes->SpeedGet(&dwSpeed);
	if(FAILED(hRes))
	{
		_stprintf(buf, TEXT("IVTxtAttributes::SpeedGet() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
	else
	{
		_stprintf( buf, TEXT("%ld"), (DWORD) dwSpeed );
		m_editSpeed.SetSel( 0, -1 );
		m_editSpeed.ReplaceSel( buf );
	}
}

// Call SpeedSet() with the value from the edit control
void CTabIVTxtAttributes::OnButtonSpeedSet() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtAttributes)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	TCHAR buf[BUFLEN];
	DWORD dwSpeed;
	m_editSpeed.GetWindowText((TCHAR*) buf, BUFLEN);

	_stscanf( buf, TEXT("%ld"), (DWORD*) &dwSpeed );

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtAttributes->SpeedSet(dwSpeed);
	if(FAILED(hRes))
	{
		_stprintf(buf, TEXT("IVTxtAttributes::SpeedSet() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
	else
	{
		OnButtonSpeedGet();
	}
}


// Stuff TTSATTR_MINSPEED in edit control
void CTabIVTxtAttributes::OnButtonSpeedMin() 
{
	TCHAR buf[BUFLEN];
	_stprintf( buf, TEXT("%ld"), (DWORD) TTSATTR_MINSPEED );
	m_editSpeed.SetSel( 0, -1 );
	m_editSpeed.ReplaceSel( buf );
}

// Stuff TTSATTR_MAXSPEED in edit control
void CTabIVTxtAttributes::OnButtonSpeedMax() 
{
	TCHAR buf[BUFLEN];
	_stprintf( buf, TEXT("%ld"), (DWORD) TTSATTR_MAXSPEED );
	m_editSpeed.SetSel( 0, -1 );
	m_editSpeed.ReplaceSel( buf );
}


// Call DeviceGet() and display return in edit control
void CTabIVTxtAttributes::OnButtonDeviceGet() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtAttributes)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	TCHAR buf[BUFLEN];
	DWORD dwDeviceID;

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtAttributes->DeviceGet(&dwDeviceID);
	if(FAILED(hRes))
	{
		_stprintf(buf, TEXT("IVTxtAttributes::DeviceGet() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
	else
	{
		_stprintf( buf, TEXT("%ld"), (DWORD) dwDeviceID );
		m_editDeviceID.SetSel( 0, -1 );
		m_editDeviceID.ReplaceSel( buf );
	}
}

// Call DeviceSet() with the value from the edit control
void CTabIVTxtAttributes::OnButtonDeviceSet() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtAttributes)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	TCHAR buf[BUFLEN];
	DWORD dwDeviceID;
	m_editDeviceID.GetWindowText((TCHAR*) buf, BUFLEN);

	_stscanf( buf, TEXT("%ld"), (DWORD*) &dwDeviceID );

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtAttributes->DeviceSet(dwDeviceID);
	if(FAILED(hRes))
	{
		_stprintf(buf, TEXT("IVTxtAttributes::DeviceSet() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
	else
	{
		OnButtonDeviceGet();
	}
}

// Call EnabledGet() and display return in edit control
void CTabIVTxtAttributes::OnButtonEnabledGet() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtAttributes)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	TCHAR buf[BUFLEN];
	DWORD dwEnabled;

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtAttributes->EnabledGet(&dwEnabled);
	if(FAILED(hRes))
	{
		_stprintf(buf, TEXT("IVTxtAttributes::EnabledGet() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
	else
	{
		_stprintf( buf, TEXT("%ld"), (DWORD) dwEnabled );
		m_editEnabled.SetSel( 0, -1 );
		m_editEnabled.ReplaceSel( buf );
	}
}

// Call EnabledSet() with the value from the edit control
void CTabIVTxtAttributes::OnButtonEnabledSet() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtAttributes)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	TCHAR buf[BUFLEN];
	DWORD dwEnabled;
	m_editEnabled.GetWindowText((TCHAR*) buf, BUFLEN);

	_stscanf( buf, TEXT("%ld"), (DWORD*) &dwEnabled );

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtAttributes->EnabledSet(dwEnabled);
	if(FAILED(hRes))
	{
		_stprintf(buf, TEXT("IVTxtAttributes::EnabledSet() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
	else
	{
		OnButtonEnabledGet();
	}
}

// Get (TTSModeGet() call) and display current mode GUID
void CTabIVTxtAttributes::OnButtonTTSModeGet() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtAttributes)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	TCHAR buf[BUFLEN];
	GUID gVoice;

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtAttributes->TTSModeGet(&gVoice);
	if(FAILED(hRes))
	{
		_stprintf(buf, TEXT("IVTxtAttributes::TTSModeGet() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
	else
	{
#define APPSTRLEN 256
#define GUIDSTRLEN 80
		TCHAR appGUIDStr[GUIDSTRLEN];
		OLECHAR oleStr[GUIDSTRLEN];

		// Display GUID we got
		StringFromGUID2(gVoice, oleStr, sizeof(oleStr));
#if defined(_S_UNICODE)
		// do it the easy way, OLECHAR same width as API chars...
		S2ACVTEX(appGUIDStr, oleStr, GUIDSTRLEN);
#else
		// we don't have macros for OLECHAR (always WCHAR) to app width
#if (defined(_UNICODE) || defined(UNICODE))
		// Unicode app - just copy to app buffer
		wcscpy(appGUIDStr, oleStr);
#else
		// ANSI app needs conversion, TCHAR = CHAR
		WideCharToMultiByte(CP_ACP, 0, (WCHAR*) &oleStr, -1, (CHAR*) &appGUIDStr, GUIDSTRLEN, NULL, NULL);
#endif // (defined(_UNICODE) || defined(UNICODE))
#endif // defined(_S_UNICODE)

		m_editTTSMode.SetSel( 0, -1 );
		m_editTTSMode.ReplaceSel(appGUIDStr);
	}
}

// Use TTS Picker dialog to get valid mode GUID and call TTSModeSet()
void CTabIVTxtAttributes::OnButtonSetSelectedMode() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtAttributes)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}


	// bring up mode selection dialog
	CPickTTSModeDlg pickDlg(&(pMainDlg->m_pVTTestObj->m_regInfo.ttsSelect), pMainDlg->m_pVTTestObj);
	
	int ret = pickDlg.DoModal();
	if(ret == IDOK)
	{
		pMainDlg->m_pVTTestObj->m_regInfo.siteInfo.gModeID = pMainDlg->m_pVTTestObj->m_regInfo.ttsSelect.gModeID;

		TCHAR* pStrBuf = pMainDlg->m_pVTTestObj->m_regInfo.modeNameStr.GetBuffer(BUFLEN);
		S2ACVTEX(pStrBuf, pMainDlg->m_pVTTestObj->m_regInfo.ttsSelect.szModeName, BUFLEN);
		pMainDlg->m_pVTTestObj->m_regInfo.modeNameStr.ReleaseBuffer();
	}
	else
	{
		GUID gZero;
		memset(&gZero, 0, sizeof(gZero));
		pMainDlg->m_pVTTestObj->m_regInfo.siteInfo.gModeID = gZero;

		pMainDlg->m_pVTTestObj->m_regInfo.modeNameStr.LoadString(IDS_NOMODE);
	}

	TCHAR buf[BUFLEN];

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtAttributes->TTSModeSet(
														pMainDlg->m_pVTTestObj->m_regInfo.siteInfo.gModeID);
	if(FAILED(hRes))
	{
		_stprintf(buf, TEXT("IVTxtAttributes::TTSModeSet() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
	else
	{
		OnButtonTTSModeGet();
	}
}

// Call TTSModeSet() with a bad (zero) GUID
void CTabIVTxtAttributes::OnButtonSetBadMode() 
{
	CVTTestDlg* pMainDlg = GetMainDlg();
	if(!pMainDlg->m_pVTEngObj || !pMainDlg->m_pVTEngObj->m_pIVTxtAttributes)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}

	TCHAR buf[BUFLEN];
	GUID gVoice;
	// zero out GUID to provide a bad value
	memset(&gVoice, 0, sizeof(gVoice));

	HRESULT hRes = pMainDlg->m_pVTEngObj->m_pIVTxtAttributes->TTSModeSet(gVoice);
	if(FAILED(hRes))
	{
		_stprintf(buf, TEXT("IVTxtAttributes::TTSModeSet() failed with hRes = %lX.\r\n"), (DWORD) hRes);
		pMainDlg->MainOutput(buf);
	}
	else
	{
		OnButtonTTSModeGet();
	}
}

// Refresh the attributes when tab is clicked
BOOL CTabIVTxtAttributes::OnSetActive() 
{
	if(!CPropertyPage::OnSetActive())
		return FALSE;

	OnButtonTTSModeGet();
	OnButtonEnabledGet();
	OnButtonDeviceGet();
	OnButtonSpeedGet();
	OnButtonIsSpeaking();
	return TRUE;
}

