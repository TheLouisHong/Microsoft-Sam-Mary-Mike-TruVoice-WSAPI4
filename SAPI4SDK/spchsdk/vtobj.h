/******************************************************************************\
*       vtobj.h
*
*       Header file for CVTEngineObject, CTestNotify, and CVTTestObject
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////
//
// The VTOBJ "Library" consists of classes that support easily adding Voice-Text
// to MFC applications. Virtually all functionality is isolated into several
// classes and files to allow these files to be copied over to a new project
// directory and used from a default AppWizard application, with minimum impact
// on the wizard generated classes.
//
// NOTE: Comments and sources are formatted for tabs stops = 3.
//
// To reuse these classes, do the following:
//
// 1)  Create a new MFC application. These instructions assume a Dialog as Main Window
//     type application.
//
// 2)  Copy guidseg.cpp, vtpick.*, vtnot.cpp, vtobj.* from a previous project over
//     to the new project.
//
// 3)  Add the new .cpp files to your project, but on the project settings compiler
//     tab turn off pre-compiled headers for guidseg.cpp (leave the others alone).
//
// 4)  Copy the IDD_DIALOGENGSELECT and IDD_DIALOG_REGISTER dialogs from the old
//     project to the new project. These templates are used by the classes in ttspick.cpp.
//
// 5)  Copy the OLE and template stuff from the old stdafx.h to the new stdafx.h. Add
//     ole32.lib and uuid.lib to the libraries in your project (Link tab).
//
// 6)  Open ClassWizard and click "Add Class..." then "Import Class..." to add the
//     CPickTTSModeDlg and CRegisterDlg classes from the ttspick.* files.
//
// 7)  You should now be able to successfully compile your application with the new files.
//
// 8)  In InitInstance() of your application class, you will need to change the main dialog
//     variable to a pointer so that CoUninitialize can be called AFTER destroying the
//     main dialog. Bracket the creation and DoModal() call on the main dialog window with
//     calls to CoInitialize() and CoUninitialize(). For example:
//
//			// Initialize OLE	before creating CVTTestDlg
//			if (FAILED(CoInitialize(NULL)))
//				return FALSE;
//			...
//			CVTTestDlg* pDlg = new CVTTestDlg;
//			if(pDlg == NULL)
//				return FALSE;
//			m_pMainWnd = pDlg;
//
//			int nResponse = pDlg->DoModal();
//			...
//			delete pDlg;
//			m_pMainWnd = pDlg = NULL;
//			// Uninitialize OLE after destroying CVTTestDlg
//			CoUninitialize ();
//
//     NOTE: You will have to add #includes at the top of various .cpp files as necessary.
//
// 9)  Add Protected variables to your main dialog class to hold pointers to a test
//     and engine object. Set these to NULL in the	main dialog constructor and if they
//     are != NULL, delete them in the destructor. In this sample, the following are
//     added to CVTTestDlg:
//
//			CVTTestObject*	m_pVTTestObj;
//			CVTEngineObject*	m_pVTEngObj;
//
//     Note: You could have an array or list of CVTEngineObject*'s if you wish to have
//     more than one instance of a Voice-Text object alive at one time. 
//
// 10) In OnInitDialog() of your main dialog, do the following:
//
//        m_pVTTestObj = new CVTTestObject(LogOutput, NotifyOutput);
//
//     LogOutput and NotifyOutput can be NULL (default) if you don't require logging,
//     otherwise use this sample as a guideline to send the logging information to
//     edit controls in the application or, write functions to log to a file.
//
// 11) Create an instance of an engine and register a site For and example, see
//     the CVTTestDlg::DoRegister() function in this sample.
//
// 12) You can now use the public members of m_pVTEngObj to access the engines various
//     interfaces. For an example see the CTabIVoiceText::OnButtonSpeak() function
//     in this sample.                                                                                                       
//			                                                                                                                                
// 13) Things to notice:
//			* These classes use the macros in bufcvt.h to facilitate compilation in multiple flavors.
//			* A class for the IVTxtNotifySink object is encapsulated in the CVTEngineObject
//			  class. You can add any application specific behavior to the implementation.
//			* typedef void  (__cdecl*PLOGOUTPUT) (LPCTSTR pLogStr); defines the logging functions. It
//			  can write the string to OutputDebugString() or to controls in the application. See the
//			  project code for more details on this and other features. 
//

#define MAX_ENUMMODES 40		// Maximum number of TTS engine modes found in engine DLLs
										// registered on system under test.

class CVTEngineObject;			// forward reference for pointers

// Holds flags so that logging of each notification type can be individually controlled
typedef struct
{
	BOOL bAttribChanged;
	BOOL bVisual;
	BOOL bSpeak;
	BOOL bSpeakingStarted;
	BOOL bSpeakingDone;
} VTNOTFLGS, *PVTNOTFLGS;

// Holds information needed to register an engine. 
typedef struct
{
	TTSMODEINFO				ttsSelect;				// the mode selected by the user
	VTSITEINFO				siteInfo;				// site info used to register
	BOOL						bUseSiteStr;			// pass site string
	CString					siteStr;					// site string to use
	CString					modeNameStr;			// name of the selected mode
	BOOL						bRegNotifySink;		// register notification sink
	BOOL						bAllMsgsFlg;			// set VTXTF_ALLMESSAGES flag if TRUE
	BOOL						bUseSiteInfo;			// use VTSITEINFO when registering
	BOOL						bUseMapper;				// use WAVE_MAPPER as the device ID
} VTREGINFO, *PVTREGINFO;

// typedef for pointer to logging functions
typedef void  (__cdecl*PLOGOUTPUT) (LPCTSTR pLogStr);

//************************************************************************
// Notification object

class CTestNotify : public IVTxtNotifySink {
   private:
		CVTEngineObject* m_pVTEngObj;
		PLOGOUTPUT m_fpNotifyOutput;

		static void NullLog(LPCTSTR pLogStr) {/* don't do anything*/;}

   public:
      CTestNotify (CVTEngineObject* pTTSEngObj = NULL, PLOGOUTPUT fpNotifyOutput = NULL);
      ~CTestNotify (void);

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

	   // IVTxtNotifySink members
	   STDMETHOD (AttribChanged)    (THIS_ DWORD);
	   STDMETHOD (Visual)           (THIS_ WCHAR, SCHAR, DWORD, PTTSMOUTH);
	   STDMETHOD (Speak)            (THIS_ SCHAR*, SCHAR*, DWORD);
	   STDMETHOD (SpeakingStarted)  (THIS);
	   STDMETHOD (SpeakingDone)     (THIS);
   };
typedef CTestNotify * PCTestNotify;

//************************************************************************
// CVTEngineObject

// Engine object groups interface variables and does cleanup in destructor
class CVTEngineObject
{
public:
	PIVOICETEXT				m_pIVoiceText;				// primary Voice-Text pointer
	PIVTXTATTRIBUTES	 	m_pIVTxtAttributes;		// attributes interface pointer
	PIVTXTDIALOGS			m_pIVTxtDialogs;			// dialogs interface pointer
	PCTestNotify			m_pTestNotify;     		// notification object
	VTNOTFLGS				m_notifyFlgs;				// pointer to notification flags struct

	CVTEngineObject(PLOGOUTPUT fpAPIOutput = NULL,
					PLOGOUTPUT fpNotifyOutput = NULL);
	~CVTEngineObject();

protected:
	PLOGOUTPUT				m_fpAPIOutput;
	PLOGOUTPUT				m_fpNotifyOutput;

	static void NullLog(LPCTSTR pLogStr) {/* don't do anything*/;}
};

typedef CVTEngineObject VTENGOBJ, *PVTENGOBJ;

//************************************************************************
// CTTSTestObject

// Test object has member functions to create and release TTS enumerator and
// CVTEngineObject objects, and register a site for the engine 
class CVTTestObject
{
public:
	CVTTestObject(PLOGOUTPUT fpLogOut = NULL, PLOGOUTPUT m_fpNotifyOut = NULL);
	~CVTTestObject();

	DWORD CreateEnumerator();
	PITTSENUM GetEnumerator() {return m_pITTSEnum;}
	DWORD ReleaseEnumerator();
	DWORD CreateEngine(PVTENGOBJ& pVTEngObj);
	DWORD RegisterEngine(CVTEngineObject* pVTEngObj);
	DWORD ReleaseEngine(PVTENGOBJ& pVTEngObj);

	PITTSENUM				m_pITTSEnum;					// pointer to enumerator object
	int						m_nmodes;						// the total number of enumerated modes
	TTSMODEINFO				m_aTTSInfo[MAX_ENUMMODES];
 	VTREGINFO				m_regInfo;						// info needed to call IVoiceText::Register()

protected:
	PLOGOUTPUT				m_fpLogOut;						// main logging function
	PLOGOUTPUT				m_fpNotifyOut;					// notification logging function

	void DisplayModeInfo(PTTSMODEINFO pTTSInfo);
	void CloneAndTest(PITTSENUM pTTSEnum);
	DWORD InitEnumModes(PITTSENUM pITTSEnum, PTTSMODEINFO aTTSInfo, int maxnmodes);
	static void NullLog(LPCTSTR pLogStr) {/* don't do anything*/;}
};
