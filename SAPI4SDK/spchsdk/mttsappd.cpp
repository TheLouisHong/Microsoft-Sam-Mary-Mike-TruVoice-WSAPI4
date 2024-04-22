// TTSAPPDlg.cpp : implementation file
//
/*


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include "stdafx.h"
extern CTTSAPPApp theApp;

void PaintGender (BOOL fMale);


RECT gRectMouth;
HWND ghWndMain = NULL;

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

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
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
// CTTSAPPDlg dialog

CTTSAPPDlg::CTTSAPPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTTSAPPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTTSAPPDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTTSAPPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTTSAPPDlg)
	DDX_Control(pDX, IDC_CHECK_IPA, m_CButtonIPA);
	DDX_Control(pDX, IDC_CHECK_TAG, m_CButtonTag);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_CSliderCtrlVolume);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_CSliderCtrlSpeed);
	DDX_Control(pDX, IDC_SLIDER_PITCH, m_CSliderCtrlPitch);
	DDX_Control(pDX, IDC_PAUSE, m_CButtonPause);
	DDX_Control(pDX, IDC_EDIT_SPEEDMAX, m_CEditSpeedMax);
	DDX_Control(pDX, IDC_EDIT_SPEEDMIN, m_CEditSpeedMin);
	DDX_Control(pDX, IDC_EDIT_VOLUMEMAX, m_CEditVolumeMax);
	DDX_Control(pDX, IDC_EDIT_VOLUMEMIN, m_CEditVolumeMin);
	DDX_Control(pDX, IDC_EDIT_VOLUME, m_CEditVolume);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_CEditText);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_CEditSpeed);
	DDX_Control(pDX, IDC_EDIT_PITCHMIN, m_CEditPitchMin);
	DDX_Control(pDX, IDC_EDIT_PITCHMAX, m_CEditPitchMax);
	DDX_Control(pDX, IDC_EDIT_PITCH, m_CEditPitch);
	DDX_Control(pDX, IDC_COMBO_MODES, m_CComboBoxModes);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTTSAPPDlg, CDialog)
	//{{AFX_MSG_MAP(CTTSAPPDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_CBN_CLOSEUP(IDC_COMBO_MODES, OnCloseupComboModes)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_BN_CLICKED(IDC_BUTTON_TEXTDATA, OnButtonTextdata)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_INJECT, OnButtonInject)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTTSAPPDlg message handlers

BOOL CTTSAPPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

   ghWndMain = m_hWnd;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	InitTTS();
    if (EnumModes())
    {
        m_CComboBoxModes.SetCurSel(0);
        OnCloseupComboModes();
    }
	if( !m_pITTSCentral) {
		EndDialog(0);
		return (FALSE);
	}
    ResetSliders();
	m_pITTSCentral->Register((void*)m_pTestNotify, IID_ITTSNotifySink,
                                 &m_dwRegKey);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTTSAPPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTTSAPPDlg::OnPaint() 
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
HCURSOR CTTSAPPDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTTSAPPDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int ID = pScrollBar->GetDlgCtrlID();
	int temp = nPos;
	if (nSBCode == SB_ENDSCROLL) {
		temp = ((CSliderCtrl *) pScrollBar)->GetPos();
	}
	char szBuf[10];
    DWORD dwVolume;
	_itoa(temp,szBuf,10);
	switch (ID) {
	case IDC_SLIDER_PITCH:
        if (nSBCode == SB_ENDSCROLL) {
            m_pITTSAttributes->PitchSet(temp);
        }
		m_CEditPitch.SetWindowText(szBuf); break;
	case IDC_SLIDER_SPEED:
        if (nSBCode == SB_ENDSCROLL) {
            m_pITTSAttributes->SpeedSet(temp);
        }
		m_CEditSpeed.SetWindowText(szBuf); break;
	case IDC_SLIDER_VOLUME:
        if (nSBCode == SB_ENDSCROLL) {
            dwVolume = 0xffff * temp / 100;
            dwVolume |= dwVolume << 16;
            m_pITTSAttributes->VolumeSet(dwVolume);
        }
		m_CEditVolume.SetWindowText(szBuf); break;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTTSAPPDlg::OnCloseupComboModes() 
{
	HRESULT hRes;
	SetCursor(theApp.LoadStandardCursor(IDC_WAIT));
	if(m_pITTSAttributes) {
       m_pITTSAttributes->Release();
       m_pITTSAttributes= NULL;
    }
	if(m_pITTSCentral) {
       m_pITTSCentral->UnRegister(m_dwRegKey);
       m_pITTSCentral->Release();

       m_pITTSCentral= NULL;
    }
#ifdef DIRECTSOUND
    if (m_pIAD) {
		while (m_pIAD->Release());
		m_pIAD= NULL;
	}
#else // DIRECTSOUND
    if (m_pIMMD) {
		while (m_pIMMD->Release());
		m_pIMMD= NULL;
	}
#endif
	int i = m_CComboBoxModes.GetCurSel();

#ifdef DIRECTSOUND
	hRes = CoCreateInstance (CLSID_AudioDestDirect, NULL, CLSCTX_ALL, IID_IAudioDirect, (void**)&m_pIAD);
	if (FAILED(hRes))
	{
		MessageBox( TEXT("Can't find CLSID_AudioDestDirect"), NULL, MB_OK );
	   return; 	
	}

   // crreate direct sound stuff
   LPDIRECTSOUND lpDirectSound;
   hRes = CoCreateInstance (CLSID_DirectSound, NULL,
      CLSCTX_ALL, IID_IDirectSound, (LPVOID*) &lpDirectSound);
   if (hRes) {
		MessageBox( TEXT("Can't find IID_IDirectSound"), NULL, MB_OK );
	   return; 	
   }
   hRes = lpDirectSound->Initialize(NULL);
   if (hRes) {
		MessageBox( TEXT("Can't initialize DirectSound"), NULL, MB_OK );
	   return; 	
   }
   hRes = lpDirectSound->SetCooperativeLevel (m_hWnd, DSSCL_NORMAL);

   // tell the audio object about our stuff
   m_pIAD->Init ((PVOID) lpDirectSound,IID_IDirectSound);

    m_pIAD->AddRef();
    hRes = m_pITTSEnum->Select(m_GUIDModes[i], &m_pITTSCentral, m_pIAD);
#else // DIRECTSOUND
	hRes = CoCreateInstance (CLSID_MMAudioDest, NULL, CLSCTX_ALL, IID_IAudioMultiMediaDevice, (void**)&m_pIMMD);
	if (FAILED(hRes))
	{
		MessageBox( TEXT("Error creating AudioDest Object(CoCreateInstance), app will terminate."), NULL, MB_OK );
	   	
	}

    hRes = m_pIMMD->DeviceNumSet( 0XFFFFFFFF);

    m_pIMMD->AddRef();
    hRes = m_pITTSEnum->Select(m_GUIDModes[i], &m_pITTSCentral, m_pIMMD);
#endif // DIRECTSOUND
    if ( !FAILED(hRes)  ) {
        m_pITTSCentral->QueryInterface (IID_ITTSAttributes, (void**)&m_pITTSAttributes);
        ResetSliders();	
	    m_pITTSCentral->Register((void*)m_pTestNotify, IID_ITTSNotifySink,
                                 &m_dwRegKey);
	} else {
		MessageBox( TEXT("Error selecting a TTS engine (No sound card?), app will terminate."), NULL, MB_OK );
	}

   // get the gender
   TTSMODEINFO tm;
   m_pITTSCentral->ModeGet (&tm);
   PaintGender (tm.wGender == GENDER_MALE);
   GetDlgItem(IDC_MOUTHBOX)->InvalidateRect(NULL);

}

void CTTSAPPDlg::OnPause() 
{
	BOOL bPause = m_CButtonPause.GetCheck();
	if (bPause) {
		m_pITTSCentral->AudioPause();
	} else {
		m_pITTSCentral->AudioResume();
	}
}
BOOL CTTSAPPDlg::InitTTS(void)
{
	HRESULT hRes;

    m_pITTSCentral = NULL;
    m_pITTSEnum = NULL;
    m_pITTSAttributes = NULL;
    m_dwRegKey = 0xFFFFFFFF;
    m_pTestNotify = NULL;
    m_pTestBufNotify = NULL;
#ifdef DIRECTSOUND
    m_pIAD = NULL;
#else
    m_pIMMD = NULL;
#endif

	hRes = CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSEnum, (void**)&m_pITTSEnum);

	if (FAILED(hRes))
	{
		MessageBox( TEXT("Error creating TTSEnumerator (CoCreateInstance), app will terminate."), NULL, MB_OK );
	   	return FALSE;
	}

	if( (m_pTestNotify = new CTestNotify(this)) == NULL )
		MessageBox( TEXT("Error creating notify pointer."), TEXT("Warning"), MB_OK );

	if( (m_pTestBufNotify = new CTestBufNotify()) == NULL )
		MessageBox( TEXT("Error creating buf notify pointer."), TEXT("Warning"), MB_OK );

	return TRUE;
}
BOOL CTTSAPPDlg::TerminateTTS(void)
{
	if ( m_pITTSEnum ) {
        m_pITTSEnum->Release();
        m_pITTSEnum = NULL;
    }
	if ( m_pITTSAttributes ) {
        m_pITTSAttributes->Release();
        m_pITTSAttributes = NULL;
    }
	if ( m_pITTSCentral ) {
	    m_pITTSCentral->UnRegister(m_dwRegKey);
        m_pITTSCentral->Release();
        m_pITTSCentral = NULL;
    }

#ifdef DIRECTSOUND
	if ( m_pIAD ) {
        while (m_pIAD->Release());
        m_pIAD = NULL;
    }
#else
	if ( m_pIMMD ) {
        while (m_pIMMD->Release());
        m_pIMMD = NULL;
    }
#endif	

	if (m_pTestNotify ) {
        delete(m_pTestNotify);
        m_pTestNotify = NULL;
    }
	if (m_pTestBufNotify) {
        delete(m_pTestBufNotify);
        m_pTestBufNotify = NULL;
    }
	return TRUE;
}
BOOL CTTSAPPDlg::EnumModes(void)
{
	HRESULT hRes;
    PITTSENUM pClone1;
    TTSMODEINFO TTSModeInfo;
    DWORD dwNumTimes;
    int index = 0;

	hRes = m_pITTSEnum->Clone(&pClone1);
    if( FAILED(hRes) )
    {
        MessageBox (TEXT("Couldn't clone ITTSEnum state, aborting enumeration test"), TEXT("Error"), MB_OK );
      return 0;
    }

    hRes = pClone1->Next (1, &TTSModeInfo, &dwNumTimes);
    if( dwNumTimes == 0 ) return FALSE;

    while (dwNumTimes) { 
        if (TTSModeInfo.dwFeatures & TTSFEATURE_ANYWORD)
            m_CComboBoxModes.AddString(TTSModeInfo.szModeName);
        else {
           CString   sz;
           sz = TTSModeInfo.szModeName;
           sz += " (Can only speak specific words!)";
            m_CComboBoxModes.AddString(sz);
        }
        m_GUIDModes[index++] = TTSModeInfo.gModeID;
		hRes = pClone1->Next (1, &TTSModeInfo, &dwNumTimes);
    }

	pClone1->Release();
	pClone1=NULL;
	return TRUE;
}
/**********************************************************************
Paint.cpp - Paints the mouth in any shape to an HDC. This is not cached.
   There is another set of functions that cache the bitmaps

Copyright (c) 1995-1997 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

//#include <windows.h>
//#include <mmsystem.h>
//#include <objbase.h>
//#include <objerror.h>
//#include <ole2ver.h>
//#include <speech.h>

int   iCenterX;
int   iCenterY;
int   iMaxWidthX;
int   iMinWidthX;
int   iMaxHeightY;
int   iMaxTopLipHeight;
int   iMaxBottomLipHeight;
int   iMaxTopTeethHeight;
int   iMaxBottomTeethHeight;
BOOL  gfMale;

/**********************************************************************
Bezier - Takes an array of 4 points and fills another buffer in
   with an array of N points (depending upon the depth). Note that
   it fill never fill in the last point, so the application will need
   to do that.

inputs
   POINT    *paIn - Pointer to an array of 4 points
   POINT    *paOut - Points to fill in
   DWORD    dwDepth - If 0, only 3 points are filled in. If 1, 7 are.
               If 2, 15 are. Etc.
returns
   DWORD    - Number of points
*/
DWORD Bezier (POINT *paIn, POINT *paOut, DWORD dwDepth)
{
   if (!dwDepth) {
      // fill in 3 points and we're done
      memcpy (paOut, paIn, 3 * sizeof(POINT));
      return 3;
   }

   // else, subdivide
   DWORD dwTotal = 0;
   POINT Mid;
   POINT aNew[4];
   Mid.x = (paIn[1].x + paIn[2].x) / 2;
   Mid.y = (paIn[1].y + paIn[2].y) / 2;

   // first half
   aNew[0] = paIn[0];
   aNew[1].x = (paIn[0].x + paIn[1].x) / 2;
   aNew[1].y = (paIn[0].y + paIn[1].y) / 2;
   aNew[2].x = (Mid.x + paIn[1].x) / 2;
   aNew[2].y = (Mid.y + paIn[1].y) / 2;
   aNew[3] = Mid;
   dwTotal = Bezier (aNew, paOut, dwDepth-1);

   // second half
   aNew[0] = Mid;
   aNew[1].x = (Mid.x + paIn[2].x) / 2;
   aNew[1].y = (Mid.y + paIn[2].y) / 2;
   aNew[2].x = (paIn[3].x + paIn[2].x) / 2;
   aNew[2].y = (paIn[3].y + paIn[2].y) / 2;
   aNew[3] = paIn[3];
   dwTotal += Bezier (aNew, paOut + dwTotal, dwDepth-1);

   return dwTotal;
}

/**********************************************************************
Mirror - Takes a set of pointers and mirrors them about a vertical
   axis. The best way to send in points is to start at the middle top
   of the object, work around clockwise, and end at the middle bottom.

inputs
   int      iMirrorX - X line to mirror around
   POINT    *paBuf - Pointer to an array of points
   DWORD    dwNum - Number of points. This writes another dwNum points
               into the buffer
returns
   none
*/
void Mirror (int iMirrorX, POINT *paBuf, DWORD dwNum)
{
   DWORD i;

   for (i = 0; i < dwNum; i++) {
      paBuf[dwNum+i].x = iMirrorX - (paBuf[dwNum-i-1].x - iMirrorX);
      paBuf[dwNum+i].y = paBuf[dwNum-i-1].y;
   }
}

/**********************************************************************
CalcMaxParams - Given the client rect, this calculates some
   important max & min params
*/
void CalcMaxParams (RECT *pRect)
{
   iCenterX = (pRect->right + pRect->left) / 2;
   iCenterY = (pRect->top * 2 + pRect->bottom) / 3;
   iMaxWidthX = (int) ((pRect->right - pRect->left) * .45f);
   iMinWidthX = iMaxWidthX * 2 / 3;
   iMaxHeightY = iMaxWidthX * 2 / 3;
   iMaxTopLipHeight = iMaxWidthX / (gfMale ? 7 : 5);
   iMaxBottomLipHeight = iMaxWidthX / (gfMale ? 6 : 4);
   iMaxTopTeethHeight = iMaxWidthX / 6;
   iMaxBottomTeethHeight = iMaxWidthX / 8;

}

int   iTopLipAboveTeeth, iBottomLipBelowTeeth;
int   iMouthWidthX, iMouthHeightY;
int   iTopLipHeight, iBottomLipHeight;
int   iMidLipWidthX;
int   iUpturn;
double fTension;
double fTongueDown, fTongueUp;

/**********************************************************************
CalcCurParams - Given the mouth shape, this calcaultes current parameters.
*/
void CalcCurParams (PTTSMOUTH pMouth)
{
   // how much are the teeth showing
   iTopLipAboveTeeth = iMaxTopTeethHeight * pMouth->bTeethUpperVisible / 256;
   iBottomLipBelowTeeth = iMaxBottomTeethHeight * pMouth->bTeethLowerVisible / 256;

   // how wide and high are the teeth
   iMouthWidthX = iMinWidthX + (iMaxWidthX - iMinWidthX) * pMouth->bMouthWidth / 256;
   iMouthHeightY = iMaxHeightY * pMouth->bMouthHeight / 256;
   if (pMouth->bMouthUpturn >= 0x80)
      iUpturn = -(iMouthWidthX * ((int) pMouth->bMouthUpturn - 0x80) / 128 / 10);
   else
      iUpturn = -(iMouthWidthX * ((int) pMouth->bMouthUpturn - 0x80) / 128 / 20);

   // how thick are the lips. Assume are of lip is constant?
   iTopLipHeight = iMaxTopLipHeight;
   iBottomLipHeight = iMaxBottomLipHeight;
   iMidLipWidthX = iMouthWidthX / 2;
   fTension = pMouth->bLipTension / 255.0;

   // tongue
   fTongueDown = fTongueUp = 0;
   if (pMouth->bTonguePosn < 0x80)
      fTongueDown = (0x80 - (double) pMouth->bTonguePosn) / 128.0;
   else // tongue > 0x80
      fTongueUp = ((double)pMouth->bTonguePosn - 0x80) / 128.0;
}


/**********************************************************************
DrawLips - Draws the lips on the HDC (after all the basic parameters
   are caclulated

inputs
   HDC      hdc - DC
returns
   none
*/
void DrawLips (HDC hdc)
{
   static HBRUSH redBrushMale = NULL;
   static HBRUSH redBrushFemale = NULL;
   if (!redBrushMale){
      redBrushMale = CreateSolidBrush (COLORREF(
         // RGB(184,80,80) )); //a solid skin brush - too red
         RGB(199,100,90) ));
      redBrushFemale = CreateSolidBrush (COLORREF(
         RGB(206,28, 28) )); //a solid skin brush
   }

   HBRUSH oldBrush;
   oldBrush = (HBRUSH) SelectObject(hdc, gfMale ? redBrushMale : redBrushFemale);

   int nLeft = iCenterX - iMouthWidthX;

   POINT aBuf[128];
   POINT aBez[4];
   DWORD dwCur;

#define  SETPOINT(a,b,c)   aBez[a].x = (b); aBez[a].y = (c)
#define  INTERPF(f,a,b)     ((1-(f)) * (a) + (f) * (b))
#define  INTERP(f,a,b)     ((int) INTERPF(f,a,b))
   // bottom lip
   int   iBottomLip = iCenterY + iMouthHeightY + iBottomLipBelowTeeth ;
   int   iBottomLipBottom = iCenterY + iMouthHeightY + iBottomLipBelowTeeth+ iBottomLipHeight;
   iBottomLip = INTERP(INTERPF(fTension, 0, .9),
      iBottomLip,
      iCenterY);
   iBottomLipBottom = INTERP(INTERPF(fTension, 0, .8),
      iBottomLipBottom,
      iCenterY+ iBottomLipHeight);
   dwCur = 0;

   // top of the bottom lip
   SETPOINT (0,
      iCenterX,
      iBottomLip);
   SETPOINT (1,
      INTERP(INTERPF(fTension, .6, .8), nLeft, iCenterX),
      INTERP(1.0, iCenterY, iBottomLip) + iUpturn / 3);
   SETPOINT (2,
      INTERP(INTERPF(fTension, .2, .5), nLeft, iCenterX),
      INTERP(INTERPF(fTension, .33, .1), iCenterY, iBottomLip) + iUpturn * 2 / 3);
   SETPOINT (3,
      nLeft,
      INTERP(0, iCenterY, iBottomLip) + iUpturn);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // bottom of the bottom lip
   SETPOINT (0,
      nLeft,
      INTERP(0, iCenterY, iBottomLipBottom) + iUpturn);
   SETPOINT (1,
      INTERP(INTERPF(fTension, .2, .4), nLeft, iCenterX),
      INTERP(INTERPF(fTension, .33, .2), iCenterY, iBottomLipBottom) + iUpturn * 2 / 3);
   SETPOINT (2,
      INTERP(INTERPF(fTension, .6, .8), nLeft, iCenterX),
      INTERP(1.0, iCenterY, iBottomLipBottom) + iUpturn / 3);
   SETPOINT (3,
      iCenterX,
      iBottomLipBottom);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // mirror & draw bottom lip
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);

   // top lip
   dwCur = 0;
   int iTopLip = iCenterY - iTopLipAboveTeeth - iTopLipHeight;
   int iTopLipBottom = iCenterY - iTopLipAboveTeeth;

   // bend in upper lip
   SETPOINT (0,
      iCenterX,
      iTopLip);
   SETPOINT (1,
      INTERP(.95, nLeft, iCenterX),
      iTopLip);
   SETPOINT (2,
      INTERP(.9, nLeft, iCenterX),
      iTopLip-iTopLipHeight/10);
   SETPOINT (3,
      INTERP(.8, nLeft, iCenterX),
      iTopLip-iTopLipHeight/8);
   dwCur += Bezier (aBez, aBuf + dwCur, 2);

   // from bend to left edge
   SETPOINT (0,
      INTERP(.8, nLeft, iCenterX),
      iTopLip-iTopLipHeight/8);
   SETPOINT (1,
      INTERP(INTERPF(fTension,.5,.6), nLeft, iCenterX),
      INTERP(.6, iCenterY,iTopLip) + iUpturn / 3);
   SETPOINT (2,
      INTERP(INTERPF(fTension,.2,.3), nLeft, iCenterX),
      INTERP(.3, iCenterY, iTopLip) + iUpturn * 2 / 3);
   SETPOINT (3,
      nLeft,
      iCenterY + iUpturn);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // from left edge to center lower
   SETPOINT (0,
      nLeft,
      iCenterY + iUpturn);
   SETPOINT (1,
      INTERP (INTERPF(fTension, .33, .5), nLeft, iCenterX),
      INTERP (INTERPF(fTension, .5, 0), iCenterY, iTopLipBottom) + iTopLipHeight / 8 +
      iUpturn * 2 / 3);
   SETPOINT (2,
      INTERP (INTERPF(fTension, .66, .8), nLeft, iCenterX),
      INTERP (INTERPF(fTension, .75, 0), iCenterY, iTopLipBottom) + iUpturn / 3);
   SETPOINT (3,
      iCenterX,
      INTERP (INTERPF(fTension, 1.0, .4), iCenterY, iTopLipBottom) + iTopLipHeight / 6);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // mirror & draw top lip
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);

   SelectObject (hdc, oldBrush);

   DeleteObject (redBrushMale);
   DeleteObject (redBrushFemale);
   redBrushMale = 0;
   redBrushFemale = 0;
}


/**********************************************************************
DrawSkin - Draws the lips on the HDC (after all the basic parameters
   are caclulated

inputs
   HDC      hdc - DC
returns
   none
*/
void DrawSkin (HDC hdc)
{
   static HBRUSH skinBrush = NULL;
   if (!skinBrush) {
      skinBrush = CreateSolidBrush (COLORREF(RGB(199,143,103))); //a solid skin brush
   }

   HBRUSH oldBrush = (HBRUSH) SelectObject(hdc, skinBrush);
   HPEN   oldPen = (HPEN) SelectObject(hdc, GetStockObject(NULL_PEN));

   int nLeft = iCenterX - iMouthWidthX;

   POINT aBuf[128];
   POINT aBez[4];
   DWORD dwCur;

   // bottom lip
   int   iBottomLip = iCenterY + iMouthHeightY + iBottomLipBelowTeeth ;
   int   iBottomLipBottom = iCenterY + iMouthHeightY + iBottomLipBelowTeeth+ iBottomLipHeight;
   int iTopLip = iCenterY - iTopLipAboveTeeth - iTopLipHeight;
   int iTopLipBottom = iCenterY - iTopLipAboveTeeth;
   iBottomLip = INTERP(INTERPF(fTension, 0, .9),
      iBottomLip,
      iCenterY);
   iBottomLipBottom = INTERP(INTERPF(fTension, 0, .8),
      iBottomLipBottom,
      iCenterY+ iBottomLipHeight);
   dwCur = 0;

   // bottom of the bottom lip
   SETPOINT (0,
      iCenterX,
      iBottomLipBottom);
   SETPOINT (1,
      INTERP(INTERPF(fTension, .6, .8), nLeft, iCenterX),
      INTERP(1.0, iCenterY, iBottomLipBottom) + iUpturn / 3);
   SETPOINT (2,
      INTERP(INTERPF(fTension, .2, .4), nLeft, iCenterX),
      INTERP(INTERPF(fTension, .33, .2), iCenterY, iBottomLipBottom) + iUpturn * 2 / 3);
   SETPOINT (3,
      nLeft,
      INTERP(0, iCenterY, iBottomLipBottom) + iUpturn);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // from bend to left edge
   SETPOINT (0,
      nLeft,
      iCenterY + iUpturn);
   SETPOINT (1,
      INTERP(INTERPF(fTension,.2,.3), nLeft, iCenterX),
      INTERP(.3, iCenterY, iTopLip) + iUpturn * 2 / 3);
   SETPOINT (2,
      INTERP(INTERPF(fTension,.5,.6), nLeft, iCenterX),
      INTERP(.6, iCenterY,iTopLip) + iUpturn / 3);
   SETPOINT (3,
      INTERP(.8, nLeft, iCenterX),
      iTopLip-iTopLipHeight/8);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // bend in upper lip
   SETPOINT (0,
      INTERP(.8, nLeft, iCenterX),
      iTopLip-iTopLipHeight/8);
   SETPOINT (1,
      INTERP(.9, nLeft, iCenterX),
      iTopLip);
   SETPOINT (2,
      INTERP(.95, nLeft, iCenterX),
      iTopLip);
   SETPOINT (3,
      iCenterX,
      iTopLip);
   dwCur += Bezier (aBez, aBuf + dwCur, 2);

   // put the last point in & then draw a large box around
   aBuf[dwCur] = aBez[3];
   dwCur++;
   aBuf[dwCur].x = iCenterX;
   aBuf[dwCur].y = -10;
   dwCur++;
   aBuf[dwCur].x = -10;
   aBuf[dwCur].y = -10;
   dwCur++;
   aBuf[dwCur].x = -10;
   aBuf[dwCur].y = 1000;
   dwCur++;

   // mirror & draw bottom lip
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);


   SelectObject (hdc, oldBrush);
   SelectObject (hdc, oldPen);

   DeleteObject (skinBrush);
   skinBrush = 0;
}


/**********************************************************************
DrawTeeth - Draws the lips on the HDC (after all the basic parameters
   are caclulated

inputs
   HDC      hdc - DC
returns
   none
*/
void DrawTeeth (HDC hdc)
{
   static HBRUSH whiteBrush = NULL;
   if (!whiteBrush) {
      whiteBrush = CreateSolidBrush (COLORREF(RGB(244,245,216))); //a solid skin brush
   }

   HBRUSH oldBrush = (HBRUSH) SelectObject(hdc, whiteBrush);

   int nLeft = iCenterX - iMaxWidthX;

   POINT aBuf[128];
   POINT aBez[4];
   DWORD dwCur;

   // bottom teeth
   int   iBottomTeeth = iCenterY + iMouthHeightY ;
   int   iBottomTeethBottom = iCenterY + iMouthHeightY + iMaxBottomTeethHeight;
   dwCur = 0;

   // top of the bottom tooth
   SETPOINT (0,
      iCenterX,
      iBottomTeeth);
   SETPOINT (1,
      INTERP(.6, nLeft, iCenterX),
      INTERP(1.0, iCenterY, iBottomTeeth));
   SETPOINT (2,
      INTERP(.2, nLeft, iCenterX),
      INTERP(.33, iCenterY, iBottomTeeth) );
   SETPOINT (3,
      nLeft,
      INTERP(0, iCenterY, iBottomTeeth) );
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // switch over
   SETPOINT (0,
      nLeft,
      INTERP(0, iCenterY, iBottomTeeth) );
   SETPOINT (1,
      nLeft,
      INTERP(0, iCenterY, iBottomTeeth) );
   SETPOINT (2,
      nLeft,
      iCenterY + iMaxBottomTeethHeight );
   SETPOINT (3,
      nLeft,
      iCenterY + iMaxBottomTeethHeight );
   dwCur += Bezier (aBez, aBuf + dwCur, 2);

   // bottom of the bottom lip
   SETPOINT (0,
      nLeft,
      iCenterY + iMaxBottomTeethHeight);
   SETPOINT (1,
      INTERP(.2, nLeft, iCenterX),
      INTERP(.33, iCenterY + iMaxBottomTeethHeight, iBottomTeethBottom) );
   SETPOINT (2,
      INTERP(.6, nLeft, iCenterX),
      INTERP(1.0, iCenterY + iMaxBottomTeethHeight, iBottomTeethBottom) );
   SETPOINT (3,
      iCenterX,
      iBottomTeethBottom);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // mirror & draw bottom teeth
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);




   // top teeth
   dwCur = 0;

   // front teeth
   SETPOINT (0,
      iCenterX,
      iCenterY + iMaxTopTeethHeight / 6);
   SETPOINT (1,
      INTERP(.85, nLeft, iCenterX),
      iCenterY + iMaxTopTeethHeight / 6);
   SETPOINT (2,
      INTERP(.8, nLeft, iCenterX),
      iCenterY);
   SETPOINT (3,
      INTERP(.8, nLeft, iCenterX),
      iCenterY);
   dwCur += Bezier (aBez, aBuf + dwCur, 1);

   // to edge of mouth
   SETPOINT (0,
      INTERP(.75, nLeft, iCenterX),
      iCenterY);
   SETPOINT (1,
      INTERP(.5, nLeft, iCenterX),
      iCenterY - iMaxTopTeethHeight / 10);
   SETPOINT (2,
      INTERP(.25, nLeft, iCenterX),
      iCenterY - iMaxTopTeethHeight / 6);
   SETPOINT (3,
      nLeft,
      iCenterY - iMaxTopTeethHeight / 3);
   dwCur += Bezier (aBez, aBuf + dwCur, 1);

   // up
   SETPOINT (0,
      nLeft,
      iCenterY - iMaxTopTeethHeight / 3);
   SETPOINT (1,
      nLeft,
      iCenterY - iMaxTopTeethHeight / 3);
   SETPOINT (2,
      nLeft,
      iCenterY - iMaxTopTeethHeight);
   SETPOINT (3,
      nLeft,
      iCenterY - iMaxTopTeethHeight);
   dwCur += Bezier (aBez, aBuf + dwCur, 1);

   // back over
   SETPOINT (0,
      nLeft,
      iCenterY - iMaxTopTeethHeight);
   SETPOINT (1,
      nLeft,
      iCenterY - iMaxTopTeethHeight);
   SETPOINT (2,
      iCenterX,
      iCenterY - iMaxTopTeethHeight);
   SETPOINT (3,
      iCenterX,
      iCenterY - iMaxTopTeethHeight);
   dwCur += Bezier (aBez, aBuf + dwCur, 1);

   // mirror & draw top teeth
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);


   SelectObject (hdc, oldBrush);

   DeleteObject (whiteBrush);
   whiteBrush = 0;
}


/**********************************************************************
DrawTongue - Draws the lips on the HDC (after all the basic parameters
   are caclulated

inputs
   HDC      hdc - DC
returns
   none
*/
void DrawTongue (HDC hdc)
{
   static HBRUSH tongueBrush = NULL;
   if (!tongueBrush) {
      tongueBrush = CreateSolidBrush (COLORREF(RGB(180,79,61))); //a solid skin brush
   }

   HBRUSH oldBrush = (HBRUSH) SelectObject(hdc, tongueBrush);

   int nLeft = iCenterX - iMaxWidthX * 3 / 5;

   POINT aBuf[128];
   POINT aBez[4];
   DWORD dwCur;

   // bottom teeth
   int   iBottomTeeth = iCenterY + iMouthHeightY ;
   int   iBottomTeethBottom = iCenterY + iMouthHeightY + iMaxBottomTeethHeight;
   int   iTongueTop = iCenterY + iMaxTopTeethHeight / 2;
   int   iTongueBottom = iTongueTop + iMaxTopTeethHeight * 5 / 2;
   int   iTopTeeth = iCenterY - 2* iMaxTopTeethHeight;
   dwCur = 0;

   // top of the tongue. This has a little bump
   SETPOINT (0,
      iCenterX,
      INTERP(fTongueUp, iTongueTop, iTopTeeth) );
   SETPOINT (1,
      INTERP(.8, nLeft, iCenterX),
      INTERP(fTongueUp, INTERP(1.1, iTongueBottom, iTongueTop), iTopTeeth));
   SETPOINT (2,
      nLeft,
      INTERP(.8, iTongueBottom, iTongueTop) );
   SETPOINT (3,
      nLeft,
      INTERP(.5, iTongueBottom, iTongueTop) );
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // bottom of tongue
   SETPOINT (0,
      nLeft,
      INTERP(.5, iTongueBottom, iTongueTop) );
   SETPOINT (1,
      nLeft,
      INTERP(.3, iTongueBottom, iTongueTop) );
   int   iInterp = INTERP(fTongueDown, iTongueBottom, iBottomTeethBottom);
   SETPOINT (2,
      INTERP(.5, nLeft, iCenterX),
      max(iTongueBottom, iInterp) );
   SETPOINT (3,
      iCenterX,
      max(iTongueBottom, iInterp) );
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // mirror & draw tongue
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);


   SelectObject (hdc, oldBrush);

   DeleteObject (tongueBrush);
   tongueBrush = 0;
}


/**********************************************************************
PaintMouth - Given a mouth shape and HDC, this draws it.

inputs
   TTSMOUTH    *pTTSMouth - mouth shape
   HDC         hdc - drawing context
   RECT        *pRect - rectangle to draw in
returns
   none
*/
void PaintMouth (TTSMOUTH *pTTSMouth, HDC hdc, RECT *pRect)
{
   static HBRUSH blackBrush = NULL;
   if (!blackBrush){
      blackBrush = CreateSolidBrush (COLORREF(RGB(101,37,39))); //a solid skin brush
   }

   CalcMaxParams (pRect);
   CalcCurParams (pTTSMouth);

   // back of mouth
   FillRect(hdc, pRect, blackBrush);
   DrawTongue (hdc);
   DrawTeeth (hdc);
   DrawSkin (hdc);
   DrawLips (hdc);

   DeleteObject (blackBrush);
   blackBrush = 0;

   return;
}

/**********************************************************************
PaintGender - Specified the Gender
*/
void PaintGender (BOOL fMale)
{
   gfMale = fMale;
}

/*************************************************************************
CTestNotify - Notification object.
*/
CTestNotify::CTestNotify (CTTSAPPDlg *pCTTSAPPDlg)
{
    m_pCTTSAPPDlg = pCTTSAPPDlg;
}

CTestNotify::~CTestNotify (void)
{
// this space intentionally left blank
}

STDMETHODIMP CTestNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
	*ppv = NULL;

	/* always return our IUnknown for IID_IUnknown */
	if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITTSNotifySink))
	{
		*ppv = (LPVOID) this;
		return S_OK;
	}

	// otherwise, cant find
	return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CTestNotify::AddRef (void)
{
	// normally this increases a reference count, but this object
	// is going to be freed as soon as the app is freed, so it doesn't
	// matter
	return 1;
}

STDMETHODIMP_(ULONG) CTestNotify::Release (void)
{
	// normally this releases a reference count, but this object
	// is going to be freed when the application is freed so it doesnt
	// matter
	return 1;
}

STDMETHODIMP CTestNotify::AttribChanged (DWORD dwAttribID)
{
   m_pCTTSAPPDlg->UpdateSliders();
   return NOERROR;
}

STDMETHODIMP CTestNotify::AudioStart (QWORD qTimeStamp)
{
   return NOERROR;
}

STDMETHODIMP CTestNotify::AudioStop (QWORD qTimeStamp)
{
   return NOERROR;
}

/********************************************************************
PaintToDC - Paints the mouth to a DC

inputs
   HDC dc
*/
void PaintToDC (HDC hdc, RECT *pRectClient, PTTSMOUTH pTTSMouth)
{
   // Cache & draw onto a bitmap
   HDC      hdcTemp;
   HBITMAP  hBmp;
   hdcTemp = CreateCompatibleDC (hdc);
   hBmp = CreateCompatibleBitmap (hdc, pRectClient->right, pRectClient->bottom);
   SelectObject (hdcTemp, hBmp);

   PaintMouth (pTTSMouth, hdcTemp, pRectClient);
   BitBlt(
      hdc,
      0, 0, pRectClient->right, pRectClient->bottom,
      hdcTemp,
      0, 0,
      SRCCOPY);
   DeleteDC (hdcTemp);
   DeleteObject (hBmp);
}


STDMETHODIMP CTestNotify::Visual (QWORD qTimeStamp, CHAR cIPAPhoneme,
				CHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth)
{
    if (!pTTSMouth)
       return NOERROR;

    RECT r;
    HWND hWndMouth;
    hWndMouth = ::GetDlgItem (m_pCTTSAPPDlg->m_hWnd, IDC_MOUTHBOX);
    HDC hdc = GetDC(hWndMouth);
    ::GetClientRect(hWndMouth, &r);
    PaintToDC (hdc, &r, pTTSMouth);

    ReleaseDC (hWndMouth, hdc);

   return NOERROR;
}

/*************************************************************************
CTestBufNotify - Notification object.
*/
CTestBufNotify::CTestBufNotify (void)
{
// this space intentionally left blank
}

CTestBufNotify::~CTestBufNotify (void)
{
// this space intentionally left blank
}

STDMETHODIMP CTestBufNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
	*ppv = NULL;

	/* always return our IUnknown for IID_IUnknown */
	if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ITTSBufNotifySink))
	{
		*ppv = (LPVOID) this;
		return S_OK;
	}

	// otherwise, cant find
	return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CTestBufNotify::AddRef (void)
{
	// normally this increases a reference count, but this object
	// is going to be freed as soon as the app is freed, so it doesn't
	// matter
	return 1;
}

STDMETHODIMP_(ULONG) CTestBufNotify::Release (void)
{
	// normally this releases a reference count, but this object
	// is going to be freed when the application is freed so it doesnt
	// matter
	return 1;
}

STDMETHODIMP CTestBufNotify::BookMark (QWORD qTimeStamp, DWORD dwMarkNum)
{
   return NOERROR;
}

STDMETHODIMP CTestBufNotify::TextDataDone (QWORD qTimeStamp, DWORD dwFlags)
{
   return NOERROR;
}

STDMETHODIMP CTestBufNotify::TextDataStarted (QWORD qTimeStamp)
{
   return NOERROR;
}

STDMETHODIMP CTestBufNotify::WordPosition (QWORD qTimeStamp, DWORD dwByteOffset)
{
   return NOERROR;
}

BOOL CTTSAPPDlg::DestroyWindow() 
{
	TerminateTTS();
	return CDialog::DestroyWindow();
}

void CTTSAPPDlg::OnButtonTextdata() 
{
    SDATA text;
    CString editStr;
    VOICECHARSET vcs;
    m_CEditText.GetWindowText(editStr);
    text.dwSize = strlen(editStr) + 1;
    text.pData = (CHAR *) malloc( text.dwSize);
    strcpy((CHAR*)text.pData, editStr);
	if (m_CButtonIPA.GetCheck()) {
		vcs = CHARSET_IPAPHONETIC;
	} else {
		vcs = CHARSET_TEXT;
	}
    m_pITTSCentral->TextData(vcs, m_CButtonTag.GetCheck(), text, NULL, IID_ITTSBufNotifySink);
	free(text.pData);
}

void CTTSAPPDlg::OnButtonReset() 
{
	m_pITTSCentral->AudioReset();
    m_CButtonPause.SetCheck(0);
	
}
void CTTSAPPDlg::ResetSliders()
{
	DWORD dwMin, dwMax, dwNor;
	WORD wMin, wMax, wNor;
   HRESULT  hRes;

    char szBuf[10];
	m_pITTSAttributes->PitchSet(TTSATTR_MINPITCH);
	hRes = m_pITTSAttributes->PitchGet(&wMin);
   if (hRes) {
       m_CEditPitchMin.SetWindowText("XXX");
       m_CEditPitchMax.SetWindowText("XXX");

       m_CEditPitchMin.EnableWindow(FALSE);
       m_CEditPitchMax.EnableWindow(FALSE);
       m_CEditPitch.EnableWindow(FALSE);
       m_CSliderCtrlPitch.EnableWindow(FALSE);
   }
   else {
      _itoa(wMin,szBuf,10); 
      m_CEditPitchMin.SetWindowText(szBuf);
      m_pITTSAttributes->PitchSet(TTSATTR_MAXPITCH);
      m_pITTSAttributes->PitchGet(&wMax);
      _itoa(wMax,szBuf,10); 
      m_CEditPitchMax.SetWindowText(szBuf);
      m_CSliderCtrlPitch.SetRange(wMin, wMax);

      m_CEditPitchMin.EnableWindow(TRUE);
      m_CEditPitchMax.EnableWindow(TRUE);
      m_CEditPitch.EnableWindow(TRUE);
      m_CSliderCtrlPitch.EnableWindow(TRUE);
   }
	
	m_pITTSAttributes->SpeedSet(TTSATTR_MINSPEED);
	hRes = m_pITTSAttributes->SpeedGet(&dwMin);
   if (hRes) {
      m_CEditSpeedMin.SetWindowText("XXX");
      m_CEditSpeedMax.SetWindowText("XXX");

      m_CEditSpeedMin.EnableWindow(FALSE);
      m_CEditSpeedMax.EnableWindow(FALSE);
      m_CEditSpeed.EnableWindow(FALSE);
      m_CSliderCtrlSpeed.EnableWindow(FALSE);
   }
   else {
      _itoa(dwMin,szBuf,10); 
      m_CEditSpeedMin.SetWindowText(szBuf);
      m_pITTSAttributes->SpeedSet(TTSATTR_MAXSPEED);
      m_pITTSAttributes->SpeedGet(&dwMax);
      _itoa(dwMax,szBuf,10); 
      m_CEditSpeedMax.SetWindowText(szBuf);
      m_CSliderCtrlSpeed.SetRange(dwMin, dwMax);

      m_CEditSpeedMin.EnableWindow(TRUE);
      m_CEditSpeedMax.EnableWindow(TRUE);
      m_CEditSpeed.EnableWindow(TRUE);
      m_CSliderCtrlSpeed.EnableWindow(TRUE);
   }
	
   m_CSliderCtrlVolume.SetRange(1,100); // display volume as 1-100	, not 0-65535
   m_CEditVolumeMin.SetWindowText("1");
   m_CEditVolumeMax.SetWindowText("100");
   m_CSliderCtrlVolume.SetPos(100);

   m_pITTSCentral->Inject("\\rst\\");
   hRes = m_pITTSAttributes->PitchGet(&wNor);
   if (hRes) {
      m_CEditPitch.SetWindowText("XXX");
   }
   else {
      m_CSliderCtrlPitch.SetPos(wNor);
      _itoa(wNor,szBuf,10); 
      m_CEditPitch.SetWindowText(szBuf);
   }
   hRes = m_pITTSAttributes->SpeedGet(&dwNor);
   if (hRes) {
      m_CEditSpeed.SetWindowText("XXX");
   }
   else {
      m_CSliderCtrlSpeed.SetPos(dwNor);
      _itoa(dwNor,szBuf,10); 
      m_CEditSpeed.SetWindowText(szBuf);
   }
   m_CEditVolume.SetWindowText("100");
	
}
void CTTSAPPDlg::UpdateSliders()
{
	DWORD dwNor;
	WORD wNor;
    char szBuf[10];

    
	m_pITTSAttributes->PitchGet(&wNor);
	m_CSliderCtrlPitch.SetPos(wNor);
    _itoa(wNor,szBuf,10); 
    m_CEditPitch.SetWindowText(szBuf);
	m_pITTSAttributes->SpeedGet(&dwNor);
	m_CSliderCtrlSpeed.SetPos(dwNor);
    _itoa(dwNor,szBuf,10); 
    m_CEditSpeed.SetWindowText(szBuf);
	m_pITTSAttributes->VolumeGet(&dwNor);
    dwNor &= 0xffff;
    dwNor /= 655;
    _itoa(dwNor,szBuf,10); 
    m_CEditVolume.SetWindowText(szBuf);
	m_CSliderCtrlVolume.SetPos(dwNor);
}

void CTTSAPPDlg::OnButtonDefault() 
{
    m_pITTSCentral->Inject("\\rst\\");
    UpdateSliders();	
}


void CTTSAPPDlg::OnButtonInject() 
{
	
    CString editStr;
    m_CEditText.GetWindowText(editStr);
    m_pITTSCentral->Inject(editStr);
}

