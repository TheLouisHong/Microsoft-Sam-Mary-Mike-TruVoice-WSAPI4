/******************************************************************************\
*       vtobj.cpp
*
*       Implements functionality of the CVTEngineObject and CVTTestObject classes
*
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/

// vtobj.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>
#include <tchar.h>

#include "resource.h"

#include <speech.h>
#include <bufcvt.h>

#include "vtobj.h"
#include "vtpick.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////
// CVTEngineObject

// Constructor initializes defaults
CVTEngineObject::CVTEngineObject(PLOGOUTPUT fpAPIOutput,	PLOGOUTPUT fpNotifyOutput)
{
	m_pIVoiceText = NULL;	
	m_pIVTxtAttributes = NULL;
	m_pIVTxtDialogs = NULL;	
	m_pTestNotify = NULL;
	m_notifyFlgs.bAttribChanged =
		m_notifyFlgs.bVisual =
		m_notifyFlgs.bSpeak =
		m_notifyFlgs.bSpeakingStarted =
		m_notifyFlgs.bSpeakingDone = TRUE;

	if (fpAPIOutput)
		m_fpAPIOutput = fpAPIOutput;
	else
		m_fpAPIOutput = NullLog;

	if (fpNotifyOutput)
		m_fpNotifyOutput = fpNotifyOutput;
	else
		m_fpNotifyOutput = NullLog;

	// Construct a notification object
	m_pTestNotify = new CTestNotify(this, m_fpNotifyOutput);
}

// Destructor releases Voice-Text interfaces and deletes notification object
CVTEngineObject::~CVTEngineObject()
{
	if( m_pIVTxtAttributes ) m_pIVTxtAttributes->Release();
	if( m_pIVTxtDialogs    ) m_pIVTxtDialogs->Release();
	if( m_pIVoiceText      ) m_pIVoiceText->Release();
	if( m_pTestNotify      ) delete m_pTestNotify;
}

/////////////////////////////////////////////////////////////////////////////////
// CVTTestObject

// Constructor initializes member data and structures
CVTTestObject::CVTTestObject(PLOGOUTPUT fpLogOut, PLOGOUTPUT fpNotifyOut)
{
	if (fpLogOut)
		m_fpLogOut = fpLogOut;
	else
		m_fpLogOut = NullLog;

	if (fpNotifyOut)
		m_fpNotifyOut = fpNotifyOut;
	else
		m_fpLogOut = NullLog;

	m_pITTSEnum = NULL;
	m_nmodes = 0;
	memset(&m_aTTSInfo, 0, sizeof(m_aTTSInfo));

	// initialize VTREGINFO struct with defaults
	memset(&m_regInfo.ttsSelect, 0, sizeof(m_regInfo.ttsSelect));
	memset(&m_regInfo.siteInfo, 0, sizeof(m_regInfo.siteInfo));
	m_regInfo.siteInfo.dwEnable = TRUE;
	m_regInfo.siteInfo.dwSpeed = 150;
	m_regInfo.bUseSiteInfo = FALSE;
	m_regInfo.bUseSiteStr = FALSE;
	m_regInfo.bRegNotifySink = TRUE;
	m_regInfo.bAllMsgsFlg = TRUE;
	m_regInfo.bUseMapper = TRUE;
	m_regInfo.modeNameStr.LoadString(IDS_NOMODE);
}

// destructor releases the TTS enumerator object if it exists
CVTTestObject::~CVTTestObject()
{
	ReleaseEnumerator();
}

// Clone the TTS enumerator object and do some tests on the clone
void CVTTestObject::CloneAndTest(PITTSENUM pTTSEnum)
{
	TTSMODEINFO aTTSInfo[MAX_ENUMMODES];
	HRESULT     hRes = S_OK;
	DWORD       dwNumTimes, i;
	TCHAR       szStr[(TTSI_NAMELEN * 2) + 50],szMfg[TTSI_NAMELEN],szName[TTSI_NAMELEN];
	PITTSENUM	pClone = NULL;
	int j;
	int nskip = 3;

	if(FAILED(pTTSEnum->Clone(&pClone)))
	{
		m_fpLogOut(TEXT("pTTSEnum->Clone() failed."));
		return;
	}

	while (hRes == S_OK)
	{
		// enumerate 2 items
		hRes = pClone->Next(2, aTTSInfo, &dwNumTimes);
		_stprintf (szStr, TEXT("  %d mode(s) found:"), (int) dwNumTimes);
		m_fpLogOut(szStr);

		j=0;
		for (i = 0; i < dwNumTimes; i++, j++)
		{
			S2ACVTEX(szName, aTTSInfo[j].szModeName, APPMAXSTRLEN(szName))
			S2ACVTEX(szMfg, aTTSInfo[j].szMfgName, APPMAXSTRLEN(szMfg))
			_stprintf( szStr, TEXT("    Mode %2d = %s, %s"), j+1, szMfg, szName );
			m_fpLogOut(szStr);
		}

		if(SUCCEEDED(hRes))
		{
		// skip nskip
		j += nskip;
		hRes = pClone->Skip( nskip );
		if( FAILED(hRes) )
			m_fpLogOut(TEXT("  Enum->Skip() FAILED()."));
		else
			m_fpLogOut(TEXT("  Enum->Skip() SUCCEEDED()."));
		}
	}

	// Release the clone
	if(pClone->Release())
		m_fpLogOut(TEXT("pClone->Release() returned > 0 inside CloneAndTest()."));
}

// Enumerate modes in Clone of enumerator (for testing) and stuff in aTTSInof as well as log
DWORD CVTTestObject::InitEnumModes(PITTSENUM pITTSEnum, PTTSMODEINFO aTTSInfo, int maxnmodes )
{
	PITTSENUM   pClone;
	HRESULT     hRes;
	DWORD       dwTotalModes,dwNumTimes,i;
	TCHAR       szStr[BUFLEN];

	// Clone an enumerator object and use it to enumerate the modes
	hRes = pITTSEnum->Clone (&pClone);
	if( FAILED(hRes) )
	{
		m_fpLogOut(TEXT("Couldn't clone ITTSEnum state, aborting enumeration test"));
		return 0;
	}

	// Enumerate up to maxnmodes modes and stuff into aTTSInfo array.
	// Actual number of modes enumerated returned in dwNumTimes
	hRes = pClone->Next (maxnmodes, aTTSInfo, &dwNumTimes);

	_stprintf (szStr, TEXT("  %d mode(s) found:"), (int) dwNumTimes);
	m_fpLogOut(szStr);
	if( dwNumTimes == 0 ) return 0;

	for (i = 0; i < dwNumTimes; i++)
	{
		DisplayModeInfo( &aTTSInfo[i] );
	}

	dwTotalModes = dwNumTimes;	 // remember the total number of modes

	// reset
	if( FAILED((hRes = pClone->Reset())) )
	{
		m_fpLogOut(TEXT("Enum->Reset() failed."));
		pClone->Release();
		return dwTotalModes;
	}

	// clone the clone and test it...
 	CloneAndTest(pClone);

	// Release the first clone
	if( pClone->Release() )
		m_fpLogOut(TEXT("pClone->Release() returned > 0."));

	return dwTotalModes;
}

// Log TTSMODEINFO information
void CVTTestObject::DisplayModeInfo( PTTSMODEINFO pTTSInfo )
{
	TCHAR szTemp[80];
	TCHAR str[128];

	m_fpLogOut(TEXT("\r\nMode info follows:"));
	S2ACVTEX(szTemp, pTTSInfo->szMfgName, APPMAXSTRLEN(szTemp) + 1)
	_stprintf( str, TEXT("  Mfg = %s"), szTemp );
	m_fpLogOut(str);
	S2ACVTEX(szTemp, pTTSInfo->szModeName, APPMAXSTRLEN(szTemp) + 1)
	_stprintf( str, TEXT("  ModeName = %s"), szTemp );
	m_fpLogOut(str);
	S2ACVTEX(szTemp, pTTSInfo->szProductName, APPMAXSTRLEN(szTemp) + 1)
	_stprintf( str, TEXT("  ProductName = %s"), szTemp );
	m_fpLogOut(str);
	_stprintf( str, TEXT("  LangID = %4x"), pTTSInfo->language.LanguageID );
	m_fpLogOut(str);
	_stprintf( str, TEXT("  gModeID (GUID) = %x %x %x %x%x%x%x%x%x%x%x"),
		pTTSInfo->gModeID.Data1,pTTSInfo->gModeID.Data2,pTTSInfo->gModeID.Data3,
		pTTSInfo->gModeID.Data4[0],pTTSInfo->gModeID.Data4[1],
		pTTSInfo->gModeID.Data4[2],pTTSInfo->gModeID.Data4[3],
		pTTSInfo->gModeID.Data4[4],pTTSInfo->gModeID.Data4[5],
		pTTSInfo->gModeID.Data4[6],pTTSInfo->gModeID.Data4[7]);
	m_fpLogOut(str);
}

// Create and Register the Voice-Text object
DWORD CVTTestObject::RegisterEngine(CVTEngineObject* pVTEngObj)
{
	HRESULT		hRes;

	if(!pVTEngObj)
		return 0;

	if (pVTEngObj->m_pIVoiceText)
	{
		m_fpLogOut(TEXT("Don't call RegisterEngine on same engine object twice."));
		return 0;
	}

	// Create the Voice-Text object then Register it...
	hRes = CoCreateInstance(CLSID_VTxt, NULL, CLSCTX_ALL, IID_IVoiceText, (void**) &(pVTEngObj->m_pIVoiceText));
	if(FAILED(hRes))
	{
		TCHAR buf[BUFLEN];
		_stprintf(buf, TEXT("CoCreateInstance failed with HRESULT=%lX creating CLSID_VTxt object!"), (DWORD) hRes); 
	 	m_fpLogOut(buf);

		if (pVTEngObj->m_pIVoiceText != NULL)
		{
			m_fpLogOut(TEXT("CoCreateInstance failed but didn't set pVTEngObj->m_pIVoiceText == NULL."));
			pVTEngObj->m_pIVoiceText = NULL;
		}
		return 0;
	}

	// Call IVoiceText::Register() using information from m_regInfo
	CString appName = AfxGetAppName();
	APIALLOC(pszApplication, appName.GetLength());
	APIA2SCVT(pszApplication, appName);

	APIALLOC(pszSite, m_regInfo.siteStr.GetLength());
	APIA2SCVT(pszSite, m_regInfo.siteStr);

	//Register the engine so we can use the interfaces
	hRes = pVTEngObj->m_pIVoiceText->Register(
										m_regInfo.bUseSiteStr ? pszSite : NULL,
										pszApplication,
										m_regInfo.bRegNotifySink ? pVTEngObj->m_pTestNotify : NULL,
										IID_IVTxtNotifySink,
										m_regInfo.bAllMsgsFlg ? VTXTF_ALLMESSAGES : 0,
										m_regInfo.bUseSiteInfo ? &(m_regInfo.siteInfo)	: NULL
															);
	APIFREE(pszApplication);
	APIFREE(pszSite);
	if(FAILED(hRes))
	{
		TCHAR buf[BUFLEN];
		_stprintf(buf, TEXT("m_pIVoiceText->Register() failed with HRESULT=%lX!"), (DWORD) hRes); 
	 	m_fpLogOut(buf);

		pVTEngObj->m_pIVoiceText->Release();
		pVTEngObj->m_pIVoiceText = NULL;
		return 0;
	}

	// QueryInterface() for the rest of the interfaces
	hRes = pVTEngObj->m_pIVoiceText->QueryInterface(IID_IVTxtAttributes,
														(void**)&(pVTEngObj->m_pIVTxtAttributes));
	if( FAILED(hRes) )
		m_fpLogOut(TEXT("Error getting IID_IVTxtAttributes interface."));

	hRes = pVTEngObj->m_pIVoiceText->QueryInterface(IID_IVTxtDialogs,
														(void**)&(pVTEngObj->m_pIVTxtDialogs));
	if( FAILED(hRes) )
		m_fpLogOut(TEXT("Error getting IID_IVTxtDialogs interface."));

	// Display GUID of engine mode we got
	if(pVTEngObj->m_pIVTxtAttributes)
	{
		GUID gotThisGUID;
		pVTEngObj->m_pIVTxtAttributes->TTSModeGet(&gotThisGUID);

#define APPSTRLEN 256
#define GUIDSTRLEN 80
		TCHAR appStr[APPSTRLEN];
		TCHAR appGUIDStr[GUIDSTRLEN];
		OLECHAR oleStr[GUIDSTRLEN];

		// Display GUID we got
		StringFromGUID2(gotThisGUID, oleStr, sizeof(oleStr));
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

		_stprintf(appStr, TEXT("Registered Mode GUID: %s"), appGUIDStr);
		m_fpLogOut(appStr);
	}

	return (DWORD) pVTEngObj->m_pIVoiceText;
}

// Create a TTS Enumerator object and fill in m_aTTSInfo array with mode info
DWORD CVTTestObject::CreateEnumerator()
{
  	HRESULT hRes;

	if(m_pITTSEnum)
		return ERROR_SUCCESS; // we've already got one

	hRes = CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_ALL, IID_ITTSEnum, (void**)& m_pITTSEnum);

	if (FAILED(hRes))
	{
		m_fpLogOut(TEXT("Error creating TTSEnumerator (CoCreateInstance)."));
		return hRes;
	}

	// enumerate the TTS modes filling in m_aTTSInfo and test enumeration interface...
	m_nmodes = (int)InitEnumModes(m_pITTSEnum, m_aTTSInfo, MAX_ENUMMODES);

	m_fpLogOut(TEXT("\r\n========= Finished testing enumerator object =========\r\n"));

	return ERROR_SUCCESS;
}

// Create an engine object and notification sink object.
// Note that actual Voice-Text engine object is created
// by selecting options via CRegisterDlg and calling
// RegisterEngine() to actually instantiate the engine
// and QueryInterface() to get the various interfaces.
DWORD CVTTestObject::CreateEngine(PVTENGOBJ& pVTEngObj)
{
	pVTEngObj = new CVTEngineObject(m_fpLogOut, m_fpNotifyOut);
	if(!pVTEngObj || !pVTEngObj->m_pTestNotify)
	{
		if(pVTEngObj)
			delete pVTEngObj;
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	CRegisterDlg regDlg(pVTEngObj, this);

	if(regDlg.DoModal() == IDOK)
	{
		TCHAR str[80];
		AfxGetApp()->BeginWaitCursor();

		// Create and Register the engine...
		if(RegisterEngine(pVTEngObj))
		{
			// log the mode to the mode selection list box on the main dialog...
			S2ACVTEX(str, m_regInfo.ttsSelect.szModeName, APPMAXSTRLEN(str) + 1);
			m_fpLogOut(str);
		}
		else
		{
			ReleaseEngine(pVTEngObj);
		}

		AfxGetApp()->EndWaitCursor();
	}
	else
	{
		ReleaseEngine(pVTEngObj);
	}

	if(pVTEngObj)
		return ERROR_SUCCESS;
	else
		return E_FAIL;
}

// Release Voice-Text engine/interfaces (released in engine object destructor)
DWORD CVTTestObject::ReleaseEngine(PVTENGOBJ& pVTEngObj)
{
	if (pVTEngObj == NULL)
		return ERROR_INVALID_PARAMETER;

	delete pVTEngObj;
	pVTEngObj = NULL;

	return ERROR_SUCCESS;
}

// Call Release() on enumerator object and zero out array of TTSMODEINFO
DWORD CVTTestObject::ReleaseEnumerator()
{
	if (!m_pITTSEnum)
	{
		return E_FAIL;
	}

	m_pITTSEnum->Release();
	m_pITTSEnum = NULL;
	m_nmodes = 0;
	memset(&m_aTTSInfo, 0, sizeof(m_aTTSInfo));

	return ERROR_SUCCESS;
}

