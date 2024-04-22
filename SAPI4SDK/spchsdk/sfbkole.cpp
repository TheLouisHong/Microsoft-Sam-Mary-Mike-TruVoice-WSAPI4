/******************************************************************************\
*       FdBkOle.cpp
*
*       Implementation of CFeedBackApp OLE functionality and
*       all CIVCmdNotifySink functionality
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
   of your software product and any modifications to the Software.
*/

#include "stdafx.h"
#include "FeedBack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern CFeedBackApp theApp;

BOOL CFeedBackApp::InitOle(void)
{
  //Initializes the Voice Commands Ole Interfaces.
	HRESULT hRes = 0;

	gpIVoiceCommand = NULL;

	if ( FAILED (CoInitialize(NULL)) ) return FALSE;

	//Create a Voice Commands Object
	if (CoCreateInstance (CLSID_VCmd, NULL, CLSCTX_LOCAL_SERVER, IID_IVoiceCmd, (LPVOID *) &gpIVoiceCommand) != S_OK)
		return ReleaseInterfaces();

	//Register
	hRes = gpIVoiceCommand->Register ("",&gVCmdNotifySink, IID_IVCmdNotifySink, VCMDRF_ALLMESSAGES, NULL);
	if (FAILED(hRes)) return ReleaseInterfaces();

  //get a pointer to the Voice Commands Attributes interface.
  hRes = gpIVoiceCommand->QueryInterface (IID_IVCmdAttributes, (LPVOID *) &gpIVCmdAttr);
  if (FAILED(hRes)) return ReleaseInterfaces();

  hRes = gpIVoiceCommand->QueryInterface (IID_IVCmdDialogs, (LPVOID *) & pIVCmdDialogs);
  if (FAILED(hRes)) return ReleaseInterfaces();
  
	return TRUE;
}

BOOL CFeedBackApp::VCmdState(void)
{
  //sets the state of voice commands at initialization and when attributes change
  
  DWORD state;
  if (gpIVCmdAttr->EnabledGet (&state)==NOERROR)
  {
    if (!state) //speech recognition is not enabled
    {
      m_pDlg->SetState (OFF);
    }
    else
    {
      if (gpIVCmdAttr->AwakeStateGet (&state) == NOERROR)
      {
        if (state) m_pDlg->SetState (LISTEN);  //speech recognition awake
        else m_pDlg->SetState (SLEEP);  //speech recognition asleep
      }
      else
      {
        return FALSE;
      }
    }
  }
  else
  {
    return FALSE;
  }
  return TRUE;
}

VCMDSTATES CFeedBackApp::GetVCmdState ()
{
  DWORD state;
  if (gpIVCmdAttr->EnabledGet (&state)==NOERROR)
  {
    if (!state) 
    {
      return (OFF);
    }
    else
    {
      if (gpIVCmdAttr->AwakeStateGet (&state) == NOERROR)
      {
        if (state) return (LISTEN);  //speech recognition awake
        else return (SLEEP);  //speech recognition asleep
      }
      else
      {
        return (OFF);
      }
    }
  }
  else
  {
    return (OFF);
  }
}
BOOL CFeedBackApp::SetVCmdState (VCMDSTATES state)
{
  int iVCmdState = GetVCmdState();
  
  if (state == iVCmdState) return (TRUE);
  if (state == LISTEN)
  {
    if (iVCmdState == OFF) gpIVCmdAttr->EnabledSet (TRUE);
    gpIVCmdAttr->AwakeStateSet(TRUE);
    return (TRUE);
  }
  else if (state == OFF) 
  {
    gpIVCmdAttr->EnabledSet (FALSE);
    return (TRUE);
  }
  else if (state == SLEEP)
  {
    if (iVCmdState == OFF) gpIVCmdAttr->EnabledSet (TRUE);
    HRESULT hRes = gpIVCmdAttr->AwakeStateSet(FALSE);
    return (TRUE);
  }
  else return (FALSE);
}

BOOL CFeedBackApp::EndOle(void)
{
	ReleaseInterfaces();
	
	//Free OLE2
	CoUninitialize();
	
	return TRUE;
}

BOOL CFeedBackApp::ReleaseInterfaces (void)
{
	//release interfaces
	if ( gpIVoiceCommand ) gpIVoiceCommand->Release();
	if ( gpIVCmdAttr     ) gpIVCmdAttr->Release();

	//set pointers to null
	gpIVoiceCommand = NULL;
	gpIVCmdAttr     = NULL;

	return FALSE;
}


BOOL CFeedBackApp::ShowDlg (VCMDDLGS Dialog)
{
  HRESULT hRes;
  switch (Dialog)
  {
    case VCMD_AboutDlg:
      hRes = pIVCmdDialogs->AboutDlg (m_pDlg->m_hWnd, NULL);
      return TRUE;
  
    case VCMD_GeneralDlg:
      hRes = pIVCmdDialogs->GeneralDlg (m_pDlg->m_hWnd, NULL);
      return TRUE;
  
    case VCMD_TrainGeneralDlg:
      hRes = pIVCmdDialogs->TrainGeneralDlg (m_pDlg->m_hWnd, NULL);
      return TRUE;
  
    case VCMD_LexiconDlg:
      hRes = pIVCmdDialogs->LexiconDlg (NULL, NULL);  
      hRes = pIVCmdDialogs->LexiconDlg (m_pDlg->m_hWnd, NULL);  
      return TRUE;
  }
  
  return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
//  Voice Command notification objects
//
CIVCmdNotifySink::CIVCmdNotifySink (void)
{
		m_dwMsgCnt = 0;
}

CIVCmdNotifySink::~CIVCmdNotifySink (void)
{
// this space intentionally left blank
}

STDMETHODIMP CIVCmdNotifySink::QueryInterface (REFIID riid, LPVOID *ppv)
{
		*ppv = NULL;

	//always return our IUnkown for IID_IUnknown
	if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IVCmdNotifySink)) 
	{
		*ppv = (LPVOID) this;
		return NOERROR;
	}

	// otherwise, cant find
	return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CIVCmdNotifySink::AddRef (void)
{
	// normally this adds a reference count, but this object is going to be freed
	// when the application is freed so it doesnt matter
	return 1;
}

STDMETHODIMP_(ULONG) CIVCmdNotifySink::Release (void)
{
		// normally this releases a reference count, but this object is going to be freed
		// when the application is freed so it doesnt matter
		return 1;
};

STDMETHODIMP CIVCmdNotifySink::CommandRecognize(DWORD dwID, PVCMDNAME pName, DWORD dwFlags,
	 DWORD dwActionSize, PVOID pAction, DWORD dwNumLists, PTSTR pszListValues, PTSTR pszCommand)
{
  // Normally, handles notifications for commands that belong to our app.  This app
  // does not have a menu, so it will not receive any commands.
	return NOERROR;
}


STDMETHODIMP CIVCmdNotifySink::CommandOther(PVCMDNAME pName, PTSTR pszCommand)
{
  // Handle notifications which are for some other command not in out list.
  if (pName) 
  {
    //command belongs to another app
    m_pDlg->SetRecognizedCommand (pszCommand);
    m_pDlg->SetState(RECOG);
  }
  
  else 
  {
    //command does not belong to any apps
    m_pDlg->SetState(NOTREC);
  }
	return NOERROR;
}


STDMETHODIMP CIVCmdNotifySink::CommandStart(void)
{
  // Handle notification sent when recognition begins.
	m_pDlg->SetState(THINK);    
	return NOERROR;
}


STDMETHODIMP CIVCmdNotifySink::MenuActivate(PVCMDNAME pName, BOOL bActivate)
{
  // Handle notification sent when the menu is activated/deactivated.
	return NOERROR;
}


STDMETHODIMP CIVCmdNotifySink::AttribChanged(DWORD dwAttribute)
{
  // Handle notification sent when the attribute is changed.
  theApp.VCmdState();
  return NOERROR;
}


STDMETHODIMP CIVCmdNotifySink::UtteranceBegin(void)
{
  // Handle notification sent when the recognizer detects the beginning of an utterance.
	m_pDlg->SetState(THINK);  
	return NOERROR;
}


STDMETHODIMP CIVCmdNotifySink::UtteranceEnd()
{
  // Handle notification sent when the recognizer detects the end of an utterance.
	return NOERROR;
}

STDMETHODIMP CIVCmdNotifySink::VUMeter(WORD wLevel)
{
  // Handle notification sent to indicate amplitude of incoming audio data.
	return NOERROR;
}

STDMETHODIMP CIVCmdNotifySink::Interference(DWORD dwType)
{
  // Handle notification sent when the recognizer detects interference
	m_pDlg->SetState (THINK);
	return NOERROR;
}
