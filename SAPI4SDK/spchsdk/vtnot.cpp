/******************************************************************************\
*       vtnot.cpp
*
*       Implements functionality VTxtNotifySink interface created in
*       CVTEngineObject. This notification sink is called by the
*       Voice-Text object to inform the application of engine activity
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

#include <mmsystem.h>
#include <tchar.h>
#include <speech.h>

#include "resource.h"
#include <bufcvt.h>
#include "vtobj.h"

/*************************************************************************
CTestNotify - Notification object.
*/

// Constructor
CTestNotify::CTestNotify (CVTEngineObject* pVTEngObj, PLOGOUTPUT fpNotifyOutput)
{
	m_pVTEngObj = pVTEngObj;
	if(fpNotifyOutput)
		m_fpNotifyOutput = fpNotifyOutput;
	else
		m_fpNotifyOutput = NullLog;
}

CTestNotify::~CTestNotify (void)
{
// this space intentionally left blank
}

// IUnknown::QueryInterface() implementation
STDMETHODIMP CTestNotify::QueryInterface (REFIID riid, LPVOID *ppv)
{
	*ppv = NULL;

	/* always return our IUnkown for IID_IUnknown */
	if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IVTxtNotifySink))
	{
		*ppv = (LPVOID) this;
		return S_OK;
	}

	// otherwise, cant find
	return ResultFromScode (E_NOINTERFACE);
}

// IUnknown::AddRef() stub
STDMETHODIMP_ (ULONG) CTestNotify::AddRef (void)
{
	// normally this increases a reference count, but this object
	// is going to be freed when the engine object is freed so it
	// doesn't matter
	return 1;
}

// IUnknown::Release() stub
STDMETHODIMP_(ULONG) CTestNotify::Release (void)
{
	// normally this releases a reference count, but this object
	// is going to be freed when the engine object is freed so it
	// doesn't matter
	return 1;
}

// Log AttribChanged() notification, as well as type of attrib
STDMETHODIMP CTestNotify::AttribChanged(DWORD dwAttribID)
{
	if(!m_pVTEngObj)
		return  NOERROR;

	if(m_pVTEngObj->m_notifyFlgs.bAttribChanged)
	{
		TCHAR  buf[BUFLEN];
		CString idStr;
		switch (dwAttribID)
		{
		case IVTNSAC_DEVICE:
		 	idStr = TEXT("IVTNSAC_DEVICE");
			break;
		case IVTNSAC_ENABLED:
		 	idStr = TEXT("IVTNSAC_ENABLED");
			break;
		case IVTNSAC_SPEED:
		 	idStr = TEXT("IVTNSAC_SPEED");
			break;
		case IVTNSAC_VOLUME:
		 	idStr = TEXT("IVTNSAC_VOLUME");
			break;
		case IVTNSAC_TTSMODE:
		 	idStr = TEXT("IVTNSAC_TTSMODE");
			break;
		default:
		 	idStr = TEXT("ERROR");
			break;
		}
		_stprintf( buf, TEXT("AttribChanged notification: %s"), idStr);
		m_fpNotifyOutput( buf );
	}

   return NOERROR;
}

// Log SpeakingStarted() notification
STDMETHODIMP CTestNotify::SpeakingStarted()
{
	if(!m_pVTEngObj)
		return  NOERROR;

	if(m_pVTEngObj->m_notifyFlgs.bSpeakingStarted)
	{
		m_fpNotifyOutput(TEXT("SpeakingStarted notification."));
	}

   return NOERROR;
}

// Log SpeakingDone() notification
STDMETHODIMP CTestNotify::SpeakingDone()
{
	if(!m_pVTEngObj)
		return  NOERROR;

	if(m_pVTEngObj->m_notifyFlgs.bSpeakingDone)
	{
		m_fpNotifyOutput(TEXT("SpeakingDone notification."));
	}

   return NOERROR;
}

// Log Visual() notification
STDMETHODIMP CTestNotify::Visual(WCHAR cIPAPhoneme,
				SCHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth)
{
	if(!m_pVTEngObj)
		return  NOERROR;

	if(m_pVTEngObj->m_notifyFlgs.bVisual)
	{
		m_fpNotifyOutput(TEXT("Visual notification."));
	}

   return NOERROR;
}

// Log Speak() notification
STDMETHODIMP CTestNotify::Speak(SCHAR* pszText, SCHAR* aszApplication, DWORD dwType)
{
	if(!m_pVTEngObj)
		return  NOERROR;

	if(m_pVTEngObj->m_notifyFlgs.bSpeak)
	{
		m_fpNotifyOutput(TEXT("Speak notification."));
	}

   return NOERROR;
}
