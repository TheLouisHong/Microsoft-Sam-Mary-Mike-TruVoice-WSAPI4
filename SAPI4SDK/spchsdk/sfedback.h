/******************************************************************************\
*       Feedback.h
*
*       Source file for FEEDBACK.EXE, a demonstration of the Microsoft
*       Speech Application Programming Interface.
*
*       Header file for Dialogs.cpp, FeedBack.cpp and FdBkOle.cpp
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/


#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef _SPEECH_
#define INC_OLE2
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <speech.h>
#endif //_SPEECH_

#include "resource.h"
#include <stdio.h>

#ifndef WINSHELLAPI
  //
  //Tray definitions taken from shellapi.h
  //Included so that Visual C++ users can build
  //Without getting the Win95 SDK
  //
  typedef struct _NOTIFYICONDATA
  {
  	DWORD cbSize;
  	HWND hWnd;
  	UINT uID;

  	UINT uFlags;
	  UINT uCallbackMessage;
	  HICON hIcon;
	  char szTip[64];
  } NOTIFYICONDATA, *PNOTIFYICONDATA;


  #define NIM_ADD         0x00000000
  #define NIM_MODIFY	    0x00000001
  #define NIM_DELETE	    0x00000002
 
  #define NIF_MESSAGE	    0x00000001
  #define NIF_ICON	      0x00000002
  #define NIF_TIP		      0x00000004
  //
  // End Tray definitions taken from shellapi.h
  //
#endif //WINSHELLAPI

const UINT BUILD = 12;  //Build Version

const UINT WM_NOTIFYICON = (WM_USER+0x0ADA);
const UINT WM_NOTIFYAPP = (WM_USER+0x0ACE);


#define NUMSTATES         6

enum VCMDSTATES
{
  LISTEN          =  0,  //Constants used for setting 
  SLEEP           =  1,  //Voice Command States
  OFF             =  2,
  NOTREC          =  3,
  RECOG           =  4,
  THINK           =  5,
  UNINIT          =  6
};  

enum VCMDDLGS
{
  VCMD_AboutDlg        =  0,
  VCMD_GeneralDlg      =  1,
  VCMD_TrainGeneralDlg =  2,
  VCMD_LexiconDlg      =  3
};

#include "tray.h"     //Used for feedback in the windows tray under win95
#include "icons.h"    //Used for feedback on the desktop under windows NT

const UINT REC_TIMER = 10001;



/////////////////////////////////////////////////////////////////////////////
// CSplashDlg dialog
// See Dialogs.cpp for the implementation of this class
//
class CSplashDlg : public CDialog
{
// Construction
public:
	CSplashDlg(CWnd* pParent = NULL);   // standard constructor
  void Message (LPCTSTR message);     //Displays initialization messages.
	
	//{{AFX_DATA(CSplashDlg)
	enum { IDD = IDD_SPLASH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	/* Overrides */
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSplashDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CFeedBackDlg dialog
// see Dialogs.cpp for the implementation of this class
//
class CFeedBackDlg : public CDialog
{
// Construction
public:
	CFeedBackDlg(CWnd* pParent = NULL);     // standard constructor
	
	// Dialog Data
	BOOL m_bSupTray; //TRUE if Windows supports the Tray
	BOOL SetState (VCMDSTATES state);
  	void SetRecognizedCommand (PSTR szCommand);
  	CSplashDlg *m_pSplash;

	//{{AFX_DATA(CFeedBackDlg)
	enum { IDD = IDD_NOTIFY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFeedBackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	VCMDSTATES m_VCMDstate;
	void UpdateIcon (void);  
	// Generated message map functions
	//{{AFX_MSG(CFeedBackDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnTaskBar(WPARAM wParam, LPARAM lParam);
  	afx_msg LRESULT OnConfigMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHide();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHideByDefault();
	afx_msg void OnConfig();
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CIVCmdNotifySink Voice Command notifications
// see FdBkOle.cpp for the implementation of this class
//
class CIVCmdNotifySink : public IVCmdNotifySink {
private:
	DWORD   m_dwMsgCnt;

public:
	CIVCmdNotifySink(void);
	~CIVCmdNotifySink(void);
	CFeedBackDlg *m_pDlg;   //pointer to main dialog
  	HWND    m_hWnd;

	// IUnkown members that delegate to m_punkOuter
	// Non-delegating object IUnknown
	STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	// IVCmdNotifySink members
	STDMETHODIMP CommandRecognize (DWORD, PVCMDNAME, DWORD, DWORD, PVOID, DWORD, PSTR, PSTR);
	STDMETHODIMP CommandOther     (PVCMDNAME, PTSTR);
	STDMETHODIMP CommandStart     (void);
	STDMETHODIMP MenuActivate     (PVCMDNAME, BOOL);
	STDMETHODIMP UtteranceBegin   (void);
	STDMETHODIMP UtteranceEnd     (void);
	STDMETHODIMP VUMeter          (WORD);
	STDMETHODIMP AttribChanged    (DWORD);
	STDMETHODIMP Interference     (DWORD);
};


/////////////////////////////////////////////////////////////////////////////
// CFeedBackApp:
// See FeedBack.cpp for the implementation of this class
//   (OLE2 functions are implemented in FdBkOle.cpp)
//
class CFeedBackApp : public CWinApp
{
public:
	CFeedBackApp();
	NOTIFYICONDATA m_IconStr[NUMSTATES]; //array of icon data
	HINSTANCE m_hShell32;
	CFeedBackDlg *m_pDlg;
  	CSplashDlg *m_pSplashDlg;
  	CTrayFeedBack *m_pTray;
  	CIcons *m_pIcon;
  	UINT m_iDelay;

	BOOL Initialize (void);
	BOOL InitOle (void);            //Implemented in FdBkOle.cpp
	BOOL EndOle (void);             //Implemented in FdBkOle.cpp
  	BOOL VCmdState(void);           //Implemented in FdBkOle.cpp
  	BOOL SetVCmdState(VCMDSTATES);  //Implemented in FdBkOle.cpp
  	VCMDSTATES  GetVCmdState();     //Implemented in FdBkOle.cpp
  	BOOL ShowDlg (VCMDDLGS);        //Implemented in FdBkOle.cpp
  

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFeedBackApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFeedBackApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL ReleaseInterfaces (void);      //Implemented in FdBkOle.cpp
	CIVCmdNotifySink  gVCmdNotifySink;  //Voice Commands Notification Sink
	PIVOICECMD        gpIVoiceCommand;
	PIVCMDATTRIBUTES  gpIVCmdAttr;
  	PIVCMDDIALOGS     pIVCmdDialogs;
  	BOOL StartSpeech(void);
	void StopSpeech(void);
	int CheckNavigator(void);
};


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
// See Dialogs.cpp for the implementation of this class
//
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	BOOL m_bSupTray;
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

typedef CIVCmdNotifySink * PCIVCmdNotifySink;

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog
// See Dialogs.cpp for the implementation of this class
//
class CConfigDlg : public CDialog
{
// Construction
public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_DIALOG_CONFIGURE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  VCMDSTATES iVCmdState;
	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	afx_msg void OnEngineAbout();
	afx_msg void OnEngineConfigure();
	afx_msg void OnRadioAsleep();
	afx_msg void OnRadioListen();
	afx_msg void OnRadioOff();
	virtual BOOL OnInitDialog();
	afx_msg void OnTrain();
	virtual void OnOK();
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
