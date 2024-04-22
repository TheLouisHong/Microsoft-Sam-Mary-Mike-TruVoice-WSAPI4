// TTSAPPDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////

class CTTSAPPDlg; 
/************************************************************************
Notification objects */
class CTestNotify : public ITTSNotifySink {
   private:
      CTTSAPPDlg *m_pCTTSAPPDlg; 
   public:
      CTestNotify (CTTSAPPDlg *m_pCTTSAPPDlg);
      ~CTestNotify (void);

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ITTSNotifySink
		STDMETHOD (AttribChanged)  (DWORD);
		STDMETHOD (AudioStart)     (QWORD);
		STDMETHOD (AudioStop)      (QWORD);
		STDMETHOD (Visual)         (QWORD, CHAR, CHAR, DWORD, PTTSMOUTH);
   };
typedef CTestNotify * PCTestNotify;

class CTestBufNotify : public ITTSBufNotifySink {
   private:

   public:
      CTestBufNotify (void);
      ~CTestBufNotify (void);

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ITTSNotifySink
	   STDMETHOD (BookMark)		   	(QWORD, DWORD);
	   STDMETHOD (TextDataDone)   	(QWORD, DWORD);
	   STDMETHOD (TextDataStarted)   (QWORD);
	   STDMETHOD (WordPosition)      (QWORD, DWORD);
   };
typedef CTestBufNotify * PCTestBufNotify;
// CTTSAPPDlg dialog

class CTTSAPPDlg : public CDialog
{
// Construction
public:
	CTTSAPPDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTTSAPPDlg)
	enum { IDD = IDD_TTSAPP_DIALOG };
	CButton	m_CButtonIPA;
	CButton	m_CButtonTag;
	CSliderCtrl	m_CSliderCtrlVolume;
	CSliderCtrl	m_CSliderCtrlSpeed;
	CSliderCtrl	m_CSliderCtrlPitch;
	CButton	m_CButtonPause;
	CEdit	m_CEditSpeedMax;
	CEdit	m_CEditSpeedMin;
	CEdit	m_CEditVolumeMax;
	CEdit	m_CEditVolumeMin;
	CEdit	m_CEditVolume;
	CEdit	m_CEditText;
	CEdit	m_CEditSpeed;
	CEdit	m_CEditPitchMin;
	CEdit	m_CEditPitchMax;
	CEdit	m_CEditPitch;
	CComboBox	m_CComboBoxModes;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTTSAPPDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTTSAPPDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCloseupComboModes();
	afx_msg void OnPause();
	afx_msg void OnButtonTextdata();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonInject();
	afx_msg void OnButtonDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// SAPI 
private:
    PITTSENUM m_pITTSEnum;
    PITTSCENTRAL    m_pITTSCentral;
    PITTSATTRIBUTES m_pITTSAttributes;
    PCTestNotify    m_pTestNotify;
    PCTestBufNotify  m_pTestBufNotify;
#ifdef DIRECTSOUND
    PIAUDIODIRECT    m_pIAD;
#else
    PIAUDIOMULTIMEDIADEVICE m_pIMMD;
#endif
    DWORD            m_dwRegKey; 
    GUID m_GUIDModes[50];

    BOOL InitTTS(void);
    BOOL TerminateTTS(void);
    BOOL EnumModes(void);
    void ResetSliders(void);
public:
    void UpdateSliders(void);
};
