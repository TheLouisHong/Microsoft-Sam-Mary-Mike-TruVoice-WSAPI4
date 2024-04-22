/******************************************************************************\
*       srfunc.cpp
*
*       This module contains most of the engine interface implementation code.
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

#include <mmsystem.h>

/*
#ifndef _SPEECH_
#define INITGUID
#define INC_OLE2
#endif  // _SPEECH_
*/

#include "srtest.h"
#include "srdlg.h"
#include "srfunc.h"



CISREnumObject::CISREnumObject()
{
   m_pISREnum = NULL;
   m_pISRFind = NULL;
}

CISREnumObject::~CISREnumObject()
{
   if( m_pISRFind ) m_pISRFind->Release();
   if( m_pISREnum ) m_pISREnum->Release();
   m_pISRFind = NULL;
   m_pISREnum = NULL;
}

BOOL CISREnumObject::FInit()
{
   // start speech recognition mode enumerator...
   HRESULT hRes = CoCreateInstance( CLSID_SREnumerator, NULL, CLSCTX_ALL, IID_ISREnum, (void**)&m_pISREnum );

   if( FAILED(hRes)  || !m_pISREnum)
   {
      m_pISREnum = NULL;
      MessageBox( NULL, TEXT("Error creating SREnumerator (CoCreateInstance), app will terminate."), NULL, MB_OK );
         return FALSE;
   }

   ASSERT( m_pISREnum );

   // obtain the find interface...
   hRes = m_pISREnum->QueryInterface(IID_ISRFind, (void**)&m_pISRFind);
   if( FAILED(hRes) )
   {
      m_pISRFind = NULL;
      MessageBox( NULL, TEXT("Error creating SRFind interface pointer."), TEXT("Error"), MB_OK );
      return FALSE;
   }

   ASSERT( m_pISRFind );

   return TRUE;
}

BOOL CISREnumObject::EngModeEnum( PSRMODEINFO *pSRInfoList, DWORD *dwNumFound )
{
   SRMODEINFO SRTmp, *p;
   PISRENUM   pClone;
   HRESULT    hRes;
   DWORD      i=0;

   // create an enumerator object...
   hRes = m_pISREnum->Clone( &pClone );
   if( CheckError( hRes, TEXT("Error in ISREnum::Clone()") ) ) return FALSE;

   // enumerate the modes...
   p = (PSRMODEINFO)malloc( 0 );
   while( (hRes = pClone->Next( 1, &SRTmp, dwNumFound )) == S_OK )
   {
      CheckError( hRes, TEXT("Error calling ISREnum::Next()") );
      if( dwNumFound > 0 )
      {
         p = (PSRMODEINFO)realloc( p, sizeof(SRMODEINFO) * (i+1) );
         memcpy( p+i, &SRTmp, sizeof(SRMODEINFO) );
         i++;
      }
      else
         MainOutput( TEXT("Error in ISREnum::Next, hResult ok but no element returned\n") );
   }

   *pSRInfoList = p;
   *dwNumFound = i;

   CheckError( pClone->Release(), TEXT("Error releasing cloned pointer") );

   return TRUE;
}

void CISREnumObject::EngModeFind( SRMODEINFO *pSRInfo, SRMODEINFO *pSRSelect )
{
   // do the find using the ISRFind interface...
   HRESULT hRes = m_pISRFind->Find( pSRInfo, NULL, pSRSelect );
   CheckError( hRes, TEXT("Error calling ISRFind::Find()") );

   // set flag so we know which select to use...
   m_bFindFlag = TRUE;
}

/*************************************************************************
CISREngineObject - Speech recognition engine object.
*/
CISREngineObject::CISREngineObject()
{
   m_pISRAttributes = NULL;
   m_pISRDialogs    = NULL;
   m_pISRDialogs2    = NULL;
   m_pISRCentral    = NULL;
   m_pSRShare       = NULL;

   m_bUseShare      = FALSE;
   m_bPaused        = FALSE;
   m_bHasArchive    = FALSE;
   m_pGram          = NULL;
   m_nLoadedGrams   = 0;

   m_Not.bAttrChange= TRUE;
   m_Not.bInterfere = TRUE;
   m_Not.bSound     = TRUE;
   m_Not.bUtter     = TRUE;
   m_Not.bVMeter    = FALSE;
}

BOOL CISREngineObject::FInit( PSRMODEINFO pModeInfo )
{
   // create an engine notification sink...
   if( (m_pEngNotify = new CEngNotify( this )) == NULL )
   {
      MessageBox( NULL, TEXT("Error creating engine notification object."), TEXT("Warning"), MB_OK );
      return FALSE;
   }

   // store the mode information...
   memcpy( &m_ModeInfo, pModeInfo, sizeof(SRMODEINFO) );

   return TRUE;
}

CISREngineObject::~CISREngineObject()
{
   // release the sr interfaces...
   if( m_pISRCentral && m_dwKey ) m_pISRCentral->UnRegister( m_dwKey );
   if( m_pISRAttributes ) m_pISRAttributes->Release();
   if( m_pISRDialogs    ) m_pISRDialogs->Release();
   if( m_pISRDialogs2    ) m_pISRDialogs2->Release();

   // Release the ISRCentral Interface.  It is important to release the
   // ISRCentral Interface even when using the sharing object.
   m_pISRCentral->Release();

   // detach or release depending on whether share was used...
   if( m_pSRShare )
   {
      m_pSRShare->Detach( m_qwInstID );
      m_pSRShare->Release();
   }

   DeleteAllGrammars();
}

/*
 Select engine mode.

 returns TRUE for succes, FALSE for failure
*/
BOOL CISREngineObject::SelectMode( PISRENUMOBJ pSREnum )
{
   HRESULT  hRes;
   TCHAR    buf[MAX_STRLEN];

   // if we want to either use a shared mode or share this one...
   if( m_bUseShare )
   {
       SRSHARE srShare;
       ULONG   uNum;
      GUID    guid;

       // get the speech recognition share enumerator...
      hRes = CoCreateInstance( CLSID_SRShare, NULL, CLSCTX_ALL, IID_IEnumSRShare, (void**)&m_pSRShare);
      if( FAILED(hRes) )
      {
         MainOutput(TEXT("\nWarning - Failed to create mode Share object in CISREngineObject::SelectMode.\n"));
         MessageBox( NULL, TEXT("Error obtaining share object."), TEXT("Warning"), MB_OK );
         m_pSRShare = NULL;
         return FALSE;
      }

      // look for the mode among the currently shared ones...
      while( TRUE )
      {
         memset( &srShare, 0, sizeof(SRSHARE) );
         hRes = m_pSRShare->Next( 1, &srShare, &uNum );
         if( hRes == 1 || CheckError( hRes, TEXT("Error in ISRShare::Next") ) ) break;

         // if match...
         if( srShare.srModeInfo.gModeID == m_ModeInfo.gModeID ) break;
       }

      memset( &guid, 0, sizeof(guid) );

      // if the guid is 0, then create a shared mode...
      if (memcmp( &srShare.srModeInfo.gModeID, &guid, sizeof(GUID) ) == 0)
      {
         hRes = m_pSRShare->New(m_DeviceID, m_ModeInfo.gModeID, (PISRCENTRAL *)&m_pISRCentral, &m_qwInstID );
         if( CheckError( hRes, TEXT("Error in ISRShare::New") ) )
            return (BOOL)(m_pISRCentral=NULL);
      }
      // otherwise share the one we found...
      else
      {
         hRes = m_pSRShare->Share( srShare.qwInstanceID, (PISRCENTRAL *)&m_pISRCentral );
         if( CheckError( hRes, TEXT("Error in ISRShare::Share") ) )
            return (BOOL)(m_pISRCentral=NULL);
         m_qwInstID = srShare.qwInstanceID;
      }
   }
   // otherwise select using the multimedia object...
   else
   {
      PIAUDIOMULTIMEDIADEVICE pIAudioSource;

      // Create the Multimedia Audio Source Object
      hRes = CoCreateInstance(CLSID_MMAudioSource, NULL, CLSCTX_ALL, 
                              IID_IAudioMultiMediaDevice, (void **) &pIAudioSource);
      if (CheckError(hRes, TEXT("CoCreateInstance for mm device failed in EngModeSelect()."))) {
         MessageBox( NULL, buf, NULL, MB_OK );
         return (FALSE);
      }

      // Set the specified device id
      hRes = pIAudioSource->DeviceNumSet(m_DeviceID);
      CheckError(hRes, TEXT("Error in IAudioMultimediaDevice::DeviceNumSet()."));

      // Create an instrumented audio source and pass the multumedia object into it
      IAudioSourceInstrumented *pIAudioInst = NULL;
      hRes = CoCreateInstance(CLSID_InstAudioSource, NULL, CLSCTX_ALL,
                              IID_IAudioSourceInstrumented, (void **) &pIAudioInst);
      if (SUCCEEDED(hRes)) {
         if (SUCCEEDED(hRes = pIAudioInst->AudioSource(pIAudioSource))) {
            pIAudioSource->Release();
            pIAudioInst->RegistrySet(L"Software\\Voice\\InstrumentedAudio");
            pIAudioSource = (PIAUDIOMULTIMEDIADEVICE) pIAudioInst;
         }
      }

      // Select now...

      // if the mode was found using the ISRFind interface then use the Find::Select()...
      if (pSREnum->m_bFindFlag)
         hRes = pSREnum->m_pISRFind->Select(m_ModeInfo.gModeID, &m_pISRCentral, (LPUNKNOWN) pIAudioSource);
      else   // otherwise use the ISREnum interface...
         hRes = pSREnum->m_pISREnum->Select(m_ModeInfo.gModeID, &m_pISRCentral, (LPUNKNOWN) pIAudioSource);

      if (CheckError(hRes, TEXT("Error in Find/Enum::Select"))) {
         m_pISRCentral = NULL;
         return (FALSE);
      }

      pIAudioSource->Release();
   }

   ASSERT( m_pISRCentral );

   hRes = m_pISRCentral->QueryInterface (IID_ISRAttributes, (void**)&m_pISRAttributes);
   CheckError( hRes, TEXT("Error in QueryInterface for ISRAttributes") );

   m_pISRCentral->QueryInterface (IID_ISRDialogs, (void**)&m_pISRDialogs);
   CheckError( hRes, TEXT("Error in QueryInterface for ISRDialogs") );

   m_pISRCentral->QueryInterface (IID_ISRDialogs2, (void**)&m_pISRDialogs2);

   // register the notification object...
   hRes = m_pISRCentral->Register (m_pEngNotify, IID_ISRNotifySink, &m_dwKey);
   CheckError( hRes, TEXT("Error Registering engine notification class") );

   return (BOOL)(m_pISRCentral != NULL);
}

/*
 Create a new grammar object for this engine mode.
*/
int CISREngineObject::CreateGrammarObject( PTCHAR szFileName, BOOL bArchive )
{
   PISRGRAMOBJ pGram;

   // create a grammar object...
   pGram = new ISRGRAMOBJ( m_bUseShare );
   if( pGram == NULL ) return -1;

   // initialize the grammar...
   if( pGram->GrammarInit( m_pISRCentral, szFileName, bArchive ) )
   {
      MainOutput(TEXT("GrammarInit() failed, grammar object not created. The engine may not support the grammar type."));
      delete pGram;
      return -2;
   }

   // add it to the current grammar list for this engine mode...
   m_GramList.AddTail( pGram );

   // make it the current grammar...
   m_pGram = pGram;

   m_nLoadedGrams++;

   return 0;
}

/*
 Delete a grammar object specified by a pointer to it.
*/
void CISREngineObject::DeleteGrammarObject( PISRGRAMOBJ pGram )
{
   ASSERT( pGram != NULL );

   if( pGram == NULL || m_nLoadedGrams < 1 ) return;

   // remove the grammar pointer from the grammar list...
   POSITION pos = m_GramList.Find( pGram, NULL );
   m_GramList.RemoveAt( pos );

   // now delete the grammar object...
   delete m_pGram;
   m_pGram = NULL;

   m_nLoadedGrams--;
}

/*
 Release all the grammars associated with this engine mode.
*/
void CISREngineObject::DeleteAllGrammars()
{
   POSITION pos;

   for( int i=0; i<m_nLoadedGrams; i++ )
   {
      // get the pointer...
      pos = m_GramList.FindIndex( i );
      m_pGram = m_GramList.GetAt( pos );

      // delete it...
      delete m_pGram;
      m_pGram = NULL;
   }

   m_GramList.RemoveAll();

   m_pGram = NULL;
   m_nLoadedGrams = 0;
}

/*************************************************************************
CISRGrammarObject - Grammar object.
*/
CISRGrammarObject::CISRGrammarObject( BOOL bUseShare )
{
   if( (m_pGramNotify = new CGramNotify( this, bUseShare )) == NULL )
      MessageBox( NULL, TEXT("Error creating grammar notification object."), TEXT("Warning"), MB_OK );

   // inititalize member variables...
   m_pISRGramCommon = NULL;
   m_pISRGramCFG    = NULL;
   m_pISRGramDict   = NULL;
   m_NumResults     = 0;
   memset( m_szName, 0, sizeof(m_szName) );
   memset( &m_Cfg, 0, sizeof(m_Cfg) );
   memset( &m_Dict, 0, sizeof(m_Dict) );

   m_bActive        = FALSE;
   m_bUseShare       = bUseShare;
   m_Cfg.bLinkQuery = FALSE;
   m_Cfg.bListQuery = FALSE;

   // init grammar notification flags...
   m_Not.bPhrase    = TRUE;
   m_Not.bBookmark  = TRUE;
   m_Not.bPaused    = TRUE;
   m_Not.bPhraseHyp = FALSE;
   m_Not.bReEval    = TRUE;
   m_Not.bUnarchive = TRUE;
   m_Not.bTraining  = TRUE;

   m_hGramInfo      = NULL;
   m_Archive.pData  = NULL;
}

CISRGrammarObject::~CISRGrammarObject()
{
   int        i;
   POSITION   pos;
   PResult_t  pResult;
   PTCHAR     pName;

   // free the extra grammar info...
   GrammarClose(m_hGramInfo);
   for (i = 0; i < m_RuleList.GetCount(); i++) {
      pos = m_RuleList.FindIndex(i);
      pName = (PTCHAR) m_RuleList.GetAt(pos);
      delete pName;
   }
   m_RuleList.RemoveAll();

   for(i = 0; i < m_GramList.GetCount(); i++) {
      pos = m_GramList.FindIndex(i);
      pName = (PTCHAR) m_GramList.GetAt(pos);
      delete pName;
   }
   m_GramList.RemoveAll();

   for (i = 0; i < m_ActiveRules.GetCount(); i++) {
      pos = m_ActiveRules.FindIndex(i);
      pName = (PTCHAR) m_ActiveRules.GetAt(pos);
      delete pName;
   }
   m_ActiveRules.RemoveAll();

   // the spec (for GrammarLoad) says the grammar will be deactivated
   // by the engine so I don't have to call Deactivate here...

   // release/free results objects and interfaces...
   for (i = 0; i < (int) m_NumResults; i++ ) {
      pos = m_ResList.FindIndex(i);
      pResult = m_ResList.GetAt(pos);

      // release the unknown interface...
      pResult->lpResult->Release();

      // now free the result object...
      free(pResult);
   }

   // Release grammar interfaces...
   if (m_pISRGramCommon)
      m_pISRGramCommon->Release();
   if (m_pISRGramCFG)
      m_pISRGramCFG->Release();
   if (m_pISRGramDict)
      m_pISRGramDict->Release();

   // remove all of the results pointers...
   if (m_NumResults) 
      m_ResList.RemoveAll();
   m_NumResults = 0;

   // free archived data buffer if there is one...
   if (m_Archive.pData)  
      HeapFree(GetProcessHeap(), NULL , m_Archive.pData);
}

/*
 Initialize the grammar object by loading a grammar via the specified ISRCentral
 interface, then obtain the member pointers to the other interfaces.
*/
int CISRGrammarObject::GrammarInit(PISRCENTRAL pISRCentral, 
                           PTCHAR szFilename, 
                           BOOL bArchive)
{
   SDATA     dGramData;
   HRESULT   hRes;
   LPUNKNOWN lpUnk;
   PSRHEADER pSRHeader;

   if (GrammarReadFile(szFilename, &dGramData)) {
      MainOutput (TEXT("\nError opening/reading grammar file.\n"));
      return -1;
   }

   // Get access to the grammar file's header
   pSRHeader = (PSRHEADER) dGramData.pData;

   // parse the grammar info...
   m_hGramInfo = GrammarOpen(szFilename);
   EnumCFGExportedRules();
   EnumCFGLists();

   // store the name...
   _tcscpy(m_szName, StripPath(szFilename));

   // obtain interfaces...
   if (bArchive) {
      free(dGramData.pData);

      // if there are no grammars loaded, then we can't get to any archived data...
      if (gpEng->m_pGram == NULL) {
         MessageBox(NULL, TEXT("No archived grammar data available to use"), NULL, MB_OK);
         return -1;
      }

      hRes = pISRCentral->GrammarLoad(
         SRGRMFMT_CFGNATIVE, 
         gpEng->m_pGram->m_Archive,
         (PVOID)m_pGramNotify,
         IID_ISRGramNotifySink,
         &lpUnk);
      if (CheckError(hRes, TEXT("GrammarLoad using archived data failed. The engine may not support it.")))
         return -1;
   }
   else {
      hRes = pISRCentral->GrammarLoad(
         (SRGRMFMT) pSRHeader->dwType, 
         dGramData,
         (PVOID) m_pGramNotify, 
         IID_ISRGramNotifySink,
         &lpUnk);
         free(dGramData.pData);
      if(CheckError(hRes, TEXT("GrammarLoad failed. The engine may not support the grammar format.")))
         return -1;
   }

   hRes = lpUnk->QueryInterface(IID_ISRGramCommon, (LPVOID *) &m_pISRGramCommon);
   if (CheckError(hRes, TEXT("Error obtaining ISRGramCommon interface."))) {
      lpUnk->Release();
      return -2;
   }
   
   // Get Context-Free Grammar Interface
   hRes = lpUnk->QueryInterface(IID_ISRGramCFG, (LPVOID *) &m_pISRGramCFG);
   CheckError(hRes, TEXT("No ISRGramCFG interface."));

   // Get Dictation Grammar Interface
   hRes = lpUnk->QueryInterface(IID_ISRGramDictation, (LPVOID *) &m_pISRGramDict);
   CheckError(hRes, TEXT("No ISRGramDictation interface."));

   // release the IUnknown...
   lpUnk->Release();

   return 0;
}

int CISRGrammarObject::GrammarReadFile(PTCHAR lpFilename, PSDATA pData)
{
   HANDLE hFile;
   BOOL fRead;
   DWORD dwRead;
   
   hFile = CreateFile(lpFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
   if (hFile == INVALID_HANDLE_VALUE)
      return (-1);
   if ((pData->dwSize = GetFileSize(hFile, NULL)) == 0xFFFFFFFF) {
      CloseHandle(hFile);
      return (-1);
   }
   if ((pData->pData = (PSTR) malloc(pData->dwSize)) == NULL) {
      CloseHandle(hFile);
      return (-1);
   }

   fRead = ReadFile(hFile, pData->pData, pData->dwSize, &dwRead, NULL);
   CloseHandle(hFile);
   return (fRead ? 0 : -1);
} /* End of CISRGrammarObject::GrammarReadFile() */

BOOL CISRGrammarObject::EnumCFGExportedRules()
{
   DWORD       dwSize;
   PSRCFGXRULE psrCFGXRule;
   CHAR        szRuleNameA[MAX_STRLEN];
   PTCHAR      pszRuleName;

   if( m_hGramInfo->srHeader.dwType != SRHDRTYPE_CFG ) return (FALSE);
   if( !m_hGramInfo->psrckCFGExportRules ) return (FALSE);

   dwSize = m_hGramInfo->psrckCFGExportRules->dwChunkSize;
   psrCFGXRule = (PSRCFGXRULE) (((BYTE *) m_hGramInfo->psrckCFGExportRules) + sizeof(SRCHUNK));

   while( dwSize )
   {
      dwSize -= psrCFGXRule->dwSize;

      // if the grammar is unicode...
      if (m_hGramInfo->srHeader.dwFlags & SRHDRFLAG_UNICODE)
         WideCharToMultiByte(CP_ACP, 0, (PWCHAR)psrCFGXRule->szString, -1, szRuleNameA, MAX_STRLEN, NULL, NULL);
      else   // the grammar is ansi...
         strcpy( szRuleNameA, (PCHAR)psrCFGXRule->szString );

      pszRuleName = new TCHAR[MAX_STRLEN];
      Char2Unicode( pszRuleName, szRuleNameA, MAX_STRLEN );
      m_RuleList.AddTail( pszRuleName );

      psrCFGXRule = (PSRCFGXRULE) (((BYTE *) psrCFGXRule) + psrCFGXRule->dwSize);
   }
   return TRUE;
}

BOOL CISRGrammarObject::EnumCFGLists()
{
   DWORD      dwSize;
   PSRCFGLIST psrCFGList;
   CHAR        szListNameA[MAX_STRLEN];
   PTCHAR      pszListName;

   if( m_hGramInfo->srHeader.dwType != SRHDRTYPE_CFG ) return FALSE;
   if( !m_hGramInfo->psrckCFGLists ) return FALSE;

   dwSize = m_hGramInfo->psrckCFGLists->dwChunkSize;
   psrCFGList = (PSRCFGLIST) (((BYTE *) m_hGramInfo->psrckCFGLists) + sizeof(SRCHUNK));

   while (dwSize)
   {
      dwSize -= psrCFGList->dwSize;

      // if SRCFGLIST structure is in Unicode...
      if (m_hGramInfo->srHeader.dwFlags & SRHDRFLAG_UNICODE)
         WideCharToMultiByte(CP_ACP, 0, (PWCHAR)psrCFGList->szString, -1, szListNameA, MAX_STRLEN, NULL, NULL);
      else  // else ANSI...
         strcpy( szListNameA, (PCHAR)psrCFGList->szString );

      pszListName = new TCHAR[MAX_STRLEN];
      Char2Unicode( pszListName, szListNameA, MAX_STRLEN );
      m_GramList.AddTail( pszListName );

      psrCFGList = (PSRCFGLIST) (((BYTE *) psrCFGList) + psrCFGList->dwSize);
   }

   m_szListNameIndex = 0;

   return TRUE;
}

/*************************************************************************
CEngNotify - Notification object.
*/
CEngNotify::CEngNotify( CISREngineObject *pEngObj )
{
   m_pEngObj  = pEngObj;
   m_dwRefCnt = 0;
}

CEngNotify::~CEngNotify()
{
}

STDMETHODIMP CEngNotify::QueryInterface( REFIID riid, LPVOID *ppv )
{
   *ppv = NULL;

   // always return our IUnkown for IID_IUnknown...
   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_ISRNotifySink))
   {
      AddRef();
      *ppv = (LPVOID)this;
      return S_OK;
   }

   // otherwise, cant find...
   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CEngNotify::AddRef()
{
   return ++m_dwRefCnt;
}

STDMETHODIMP_(ULONG) CEngNotify::Release()
{
   if( --m_dwRefCnt == 0 )
   {
      delete this;
      return 0;
   }
   return m_dwRefCnt;
}

STDMETHODIMP CEngNotify::AttribChanged( DWORD dwAttribID )
{
   if( gpEng->m_Not.bAttrChange )
   {
      TCHAR  buf[MAX_STRLEN];
      wsprintf( buf, TEXT("Attribute: ID: %0x\r\n"), dwAttribID );
      ((CSRTestDlg *)AfxGetMainWnd())->NotifyOutput( buf );
   }

   return NOERROR;
}


STDMETHODIMP CEngNotify::Interference( QWORD qTimeStampBegin,QWORD qTimeStampEnd, DWORD dwType )
{
   if( gpEng->m_Not.bInterfere )
   {
      TCHAR buf[MAX_STRLEN];
      _stprintf( buf,TEXT("Interference %04x: beg: %I64x, end: %I64x\r\n"),
            dwType, qTimeStampBegin, qTimeStampEnd );
      ((CSRTestDlg *)AfxGetMainWnd())->NotifyOutput( buf );
   }

   return NOERROR;
}

STDMETHODIMP CEngNotify::Sound( QWORD qTimeStampBegin, QWORD qTimeStampEnd )
{
   if( gpEng->m_Not.bSound )
   {
      TCHAR  buf[MAX_STRLEN];
      _stprintf( buf,TEXT("Sound: beg: %I64x, end: %I64x\r\n"), qTimeStampBegin, qTimeStampEnd );
      ((CSRTestDlg *)AfxGetMainWnd())->NotifyOutput( buf );
   }

   return NOERROR;
}

STDMETHODIMP CEngNotify::UtteranceBegin( QWORD qTimeStampBegin )
{
   if( gpEng->m_Not.bUtter )
   {
      TCHAR buf[MAX_STRLEN];
      _stprintf( buf,TEXT("Utterance Beg: %I64x\r\n"), qTimeStampBegin );
      ((CSRTestDlg *)AfxGetMainWnd())->NotifyOutput( buf );
   }

   return NOERROR;
}

STDMETHODIMP CEngNotify::UtteranceEnd( QWORD qTimeStampBegin,QWORD qTimeStampEnd )
{
   if( gpEng->m_Not.bUtter )
   {
      TCHAR  buf[MAX_STRLEN];
      _stprintf( buf,TEXT("Utterance End: %I64x\r\n"), qTimeStampEnd );
      ((CSRTestDlg *)AfxGetMainWnd())->NotifyOutput( buf );
   }

   return NOERROR;
}

STDMETHODIMP CEngNotify::VUMeter( QWORD qTimeStampBegin, WORD wLevel )
{
   if( gpEng->m_Not.bVMeter )
   {
      TCHAR buf[MAX_STRLEN];
      wsprintf( buf, TEXT("View meter: %04x\r\n"), wLevel );
      ((CSRTestDlg *)AfxGetMainWnd())->NotifyOutput( buf );
   }

   return NOERROR;
}


/*************************************************************************
CTestGramNotify - Notification object. All this this will do is display
   its notification on the main window.
*/
CGramNotify::CGramNotify( PISRGRAMOBJ pGramObj, BOOL bUseShare )
{
   m_pGramObj = pGramObj;
   m_dwRefCnt = 0;
   m_bUseShare = bUseShare;
}

CGramNotify::~CGramNotify()
{
}

STDMETHODIMP CGramNotify::QueryInterface( REFIID riid, LPVOID *ppv )
{
   *ppv = NULL;

   // always return our IUnknown for IID_IUnknown...
   if (IsEqualIID (riid, IID_IUnknown) || 
      IsEqualIID (riid, IID_ISRGramNotifySink))
   {
      AddRef();
      *ppv = (LPVOID)this;
      return S_OK;
   }

   // otherwise, cant find...
   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CGramNotify::AddRef()
{
   return ++m_dwRefCnt;
}

STDMETHODIMP_(ULONG) CGramNotify::Release()
{
   if( --m_dwRefCnt == 0 )
   {
      delete this;
      return 0;
   }
   return m_dwRefCnt;
}

STDMETHODIMP CGramNotify::BookMark( DWORD dwID )
{
   if( m_pGramObj->m_Not.bBookmark )
   {
      TCHAR buf[MAX_STRLEN];
      _stprintf( buf,TEXT("BookMark (%s): id=%lx\r\n"), m_pGramObj->m_szName, dwID );
      ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( buf );
   }

   return NOERROR;
}

STDMETHODIMP CGramNotify::Paused()
{
   ((CSRTestDlg *)((CSRTestApp *)AfxGetApp())->m_pMainWnd)->m_Cent.m_EngPaused.SetCheck( gpEng->m_bPaused=TRUE );

   if( m_pGramObj->m_Not.bPaused )
   {
      TCHAR buf[MAX_STRLEN];
      _stprintf( buf,TEXT("Paused (%s)\r\n"), m_pGramObj->m_szName );
      ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( buf );
   }

   return NOERROR;
}

STDMETHODIMP CGramNotify::PhraseStart( QWORD qTimeStampBegin )
{
   if( m_pGramObj->m_Not.bPhrase )
   {
      TCHAR buf[MAX_STRLEN];
      _stprintf( buf,TEXT("Phrase start (%s): %I64x\r\n"), m_pGramObj->m_szName, qTimeStampBegin );
      ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( buf );
   }

   return NOERROR;
}

STDMETHODIMP CGramNotify::PhraseFinish( DWORD dwFlags, QWORD qTimeStampBegin,
   QWORD qTimeStampEnd, PSRPHRASE pSRPhrase, LPUNKNOWN lpResults )
{
   PSRWORD  pSRWord, pSRMax;
   TCHAR    buf[SRMI_NAMELEN];
   TCHAR    buf2[SRMI_NAMELEN];
   PResult_t pResult;

   if( m_pGramObj->m_Not.bPhrase )
   {
      TCHAR buf[MAX_STRLEN];
      _stprintf( buf,TEXT("Phrase finish (%s): %I64x, flags: %lx\r\n"), m_pGramObj->m_szName, qTimeStampEnd, dwFlags );
      ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( buf );
   }

   if( !pSRPhrase )
   {
      // Unrecongized utternace...
      ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( TEXT("Unrecognized.\r\n") );
      return NOERROR;
   }

   // else, we recognize something, so display it...
   ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( TEXT("Recognized: ") );

   // if a result object is available, create storage for it...
   if( lpResults )
   {
      pResult = (PResult_t)malloc( sizeof(Result_t) );
      pResult->lpResult = lpResults;
      pResult->szDesc[0] = 0;
//      if (!m_bUseShare)
         lpResults->AddRef();
   }

   // loop through all of the words and display them...
   pSRMax  = (PSRWORD) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
   pSRWord = (PSRWORD) (pSRPhrase->abWords);
   while( pSRWord < pSRMax )
   {
      // Display word...
      S2ACVTEX( buf, pSRWord->szWord, sizeof(buf)/2 );
      wsprintf( buf2, TEXT("%s "), buf );
      ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( buf2 );

      // store the phrase to display in the results list box...
      if( lpResults )
      {
#if (defined(UNICODE) || defined(_UNICODE))
         wcscat( pResult->szDesc, buf );
         wcscat( pResult->szDesc, TEXT(" ") );
#else
         strcat( pResult->szDesc, buf );
         strcat( pResult->szDesc, " " );
#endif
      }

      pSRWord = (PSRWORD) ((BYTE*) pSRWord + pSRWord->dwSize);
   }

   // end the line...
   ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( TEXT("\r\n") );

   // store the phrase in the results selection listbox...
   if (lpResults) {
      m_pGramObj->m_ResList.AddTail(pResult);
      m_pGramObj->m_NumResults++;

      // enable the results property sheet...
      ((CSRTestDlg *) AfxGetMainWnd())->m_PropRes->EnableWindow(TRUE);
   }

   return NOERROR;
}

STDMETHODIMP CGramNotify::PhraseHypothesis( DWORD dwFlags, QWORD qTimeStampBegin,
   QWORD qTimeStampEnd, PSRPHRASE pSRPhrase, LPUNKNOWN lpResults )
{
   if( m_pGramObj->m_Not.bPhraseHyp )
   {
      TCHAR buf[MAX_STRLEN];
      _stprintf( buf,TEXT("Phrase Hypothesis (%s): %I64x\r\n"), m_pGramObj->m_szName, qTimeStampBegin );
      ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( buf );
   }

   return NOERROR;
}

STDMETHODIMP CGramNotify::ReEvaluate( LPUNKNOWN lpUnk )
{
   if( m_pGramObj->m_Not.bReEval )
   {
      TCHAR buf[MAX_STRLEN];
      _stprintf( buf,TEXT("Reevaluate (%s)\r\n"), m_pGramObj->m_szName );
      ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( buf );
   }

   return NOERROR;
}

STDMETHODIMP CGramNotify::Training( DWORD dwTrain )
{
   if( m_pGramObj->m_Not.bTraining )
   {
      TCHAR buf[MAX_STRLEN];
      wsprintf( buf, TEXT("Training request (%s), flag=%lx.\r\n"), m_pGramObj->m_szName, dwTrain );
      ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( buf );
   }

   return NOERROR;
}

STDMETHODIMP CGramNotify::UnArchive( LPUNKNOWN lpUnk )
{
   if( m_pGramObj->m_Not.bUnarchive )
   {
      TCHAR buf[MAX_STRLEN];
      wsprintf( buf, TEXT("Unarchive (%s)\r\n"), m_pGramObj->m_szName );
      ((CSRTestDlg *)AfxGetMainWnd())->GramNotifyOutput( buf );
   }

   return NOERROR;
}

// Output to main edit window...
void MainOutput( PTCHAR str )
{
   ((CSRTestDlg *)((CSRTestApp *)AfxGetApp())->m_pMainWnd)->MainOutput(str);
}

void DisplayModeInfo( PSRMODEINFO pSRInfo )
{
   TCHAR buf[MAX_STRLEN], outstr[MAX_STRLEN];

   MainOutput(TEXT("\nMode info follows:\n"));
   S2ACVTEX( buf, pSRInfo->szMfgName, MAX_STRLEN );
   wsprintf( outstr, TEXT("  Mfg = %s\n"), buf );
   MainOutput( outstr );

   S2ACVTEX( buf, pSRInfo->szModeName, MAX_STRLEN );
   wsprintf( outstr, TEXT("  ModeName = %s\n"), buf );
   MainOutput( outstr );

   S2ACVTEX( buf, pSRInfo->szProductName, MAX_STRLEN );
   wsprintf( outstr, TEXT("  ProductName = %s\n"), buf );
   MainOutput( outstr );

   wsprintf( outstr, TEXT("  LangID = %4x\n"), pSRInfo->language.LanguageID );
   MainOutput( outstr );

   wsprintf( outstr, TEXT("  gModeID (GUID) = %x %x %x %x%x%x%x%x%x%x%x\n"),
      pSRInfo->gModeID.Data1,pSRInfo->gModeID.Data2,pSRInfo->gModeID.Data3,
      pSRInfo->gModeID.Data4[0],pSRInfo->gModeID.Data4[1],
      pSRInfo->gModeID.Data4[2],pSRInfo->gModeID.Data4[3],
      pSRInfo->gModeID.Data4[4],pSRInfo->gModeID.Data4[5],
      pSRInfo->gModeID.Data4[6],pSRInfo->gModeID.Data4[7]);
   MainOutput( outstr );
}

// check the return value, if error then send it to the main output window...
//
BOOL CheckError(HRESULT hRes, PTCHAR prestr)
{
   TCHAR buf[MAX_STRLEN];

   if (hRes) {
      wsprintf(buf, TEXT("%s (%lx).\n"), prestr, hRes);
      ((CSRTestDlg *)((CSRTestApp *)AfxGetApp())->m_pMainWnd)->MainOutput(buf);
      if (FAILED(hRes)) 
         return (TRUE);
   }

   return (FALSE);
}

/*
 Convert an SRPhrase structure to a string.
*/
void Phrase2Str( PSRPHRASE pSRPhrase, char *buf, BOOL fUnicode )
{
   char  *p=buf;
   DWORD len;

   // loop through all of the words...
   PSRWORD pSRWord, pSRMax;
   pSRMax  = (PSRWORD) ((BYTE*)pSRPhrase + pSRPhrase->dwSize);
   pSRWord = (PSRWORD) (pSRPhrase->abWords);
   while( pSRWord < pSRMax )
   {
      if( fUnicode ) len = wcslen((PWCHAR)pSRWord->szWord)*2;
      else len = strlen((char *)pSRWord->szWord);
      memcpy( p, pSRWord->szWord, len );
      p += len;
      *p++ = ' ';
      if( fUnicode ) *p++ = 0;

      pSRWord = (PSRWORD) ((BYTE*) pSRWord + pSRWord->dwSize);
   }
   if( fUnicode ) *(p-2) = 0;
   else *(p-1) = 0;
}

/*
 Convert a string to an SRPhrase structure.  Since the memory for the SRPhrase structure
 is alloc'ed here, it must be freed by the caller.
*/
BOOL Str2Phrase(TCHAR *buf, PSRPHRASE *ppSRPhrase)
{
   DWORD dwSRPhraseSize;

   // Allocate an SRPHRASE structure
   *ppSRPhrase = (PSRPHRASE) malloc(sizeof(SRPHRASE));
   dwSRPhraseSize = sizeof(SRPHRASE);

   // Split-up words and add on a new SRWORD structure
   TCHAR pWord[256];
   int nCount;
   int nIndex = 0;
   PSRWORD pSRWord;
   DWORD dwWordLen;
   DWORD dwWordLenAlgn;
   DWORD dwWordNum = 0;

   while (buf[nIndex]) {
      // Get a word
#ifdef _UNICODE
      nCount = swscanf(&buf[nIndex], L"%s", pWord);
#else
      nCount = sscanf(&buf[nIndex], "%s", pWord);
#endif /* _UNICODE */
      if (!nCount || nCount == EOF)
         break;
      nIndex += _tcslen(pWord);
      dwWordLen = (_tcslen(pWord) + 1) * sizeof(TCHAR);
      dwWordLenAlgn = (dwWordLen + 3) & ~3;
      *ppSRPhrase = (PSRPHRASE) realloc(*ppSRPhrase, dwSRPhraseSize + sizeof(SRWORD) + dwWordLenAlgn);
      pSRWord =  (PSRWORD) (((CHAR *) *ppSRPhrase) + dwSRPhraseSize);
      dwSRPhraseSize += sizeof(SRWORD) + dwWordLenAlgn;
      memcpy(pSRWord->szWord, pWord, dwWordLen);
      pSRWord->dwSize = sizeof(SRWORD) + dwWordLenAlgn;
      pSRWord->dwWordNum = ++dwWordNum;
   }

   // Update size of SRPHRASE structure
   (*ppSRPhrase)->dwSize = dwSRPhraseSize;

   return (TRUE);
}

/*
 Overloaded functions for converting an SRWORD to string.
*/
void Word2Str( PSRWORDA pSRWordA, char *buf )
{
   strcpy( buf, pSRWordA->szWord );
}

void Word2Str( PSRWORDW pSRWordW, PWCHAR bufW )
{
   wcscpy( bufW, pSRWordW->szWord );
}

void Word2Str( PSRWORDW pSRWordW, char *buf )
{
   char *p = (char *)pSRWordW->szWord;
   for( DWORD i=0; i<wcslen(pSRWordW->szWord)+1; i++ )
   {
      *buf++ = *p;
      p += 2;
   }
}

void Word2Str( PSRWORDA pSRWordA, PWCHAR bufW )
{
   char *p  = pSRWordA->szWord;
   char *pW = (char *)bufW;
   for( DWORD i=0; i<strlen(pSRWordA->szWord)+1; i++ )
   {
      *pW++ = *p++;
      *pW++ = 0;
   }
}

/* End of srfunc.cpp */
