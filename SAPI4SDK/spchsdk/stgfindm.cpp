/******************************************************************************\
*       dlgfindm.cpp : implementation file
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
#include "srtest.h"
#include "dlgfindm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

const int NFINDGRAMTYPES = 3;
const int NFINDFEATURES  = 11;

PTCHAR szFindGramTypes[NFINDGRAMTYPES]={TEXT("Context-Free"),TEXT("Dictation"),TEXT("Limited-Domain")};

PTCHAR szFindFeatures[NFINDFEATURES]={TEXT("Any Word"),TEXT("Grammar Links"),TEXT("Grammar Lists"),
TEXT("Mic-Independent"),TEXT("Speaker Independent"),TEXT("Multilingual"),TEXT("Optimized for PC"),
TEXT("Optimized for Phone"),TEXT("Phonetic Training"),TEXT("Word Training"),TEXT("CFG Wild Cards")};

/////////////////////////////////////////////////////////////////////////////
// CDlgFindMode dialog


CDlgFindMode::CDlgFindMode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFindMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFindMode)
	m_Mfg = _T("");
	m_Lang = _T("");
	m_ModeName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgFindMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFindMode)
	DDX_Control(pDX, IDC_FEATURES, m_Features);
	DDX_Control(pDX, IDC_GRAMMARS, m_GramType);
	DDX_Control(pDX, IDC_LISTSEQUENCE, m_Sequence);
	DDX_Text(pDX, IDC_EDITMFG, m_Mfg);
	DDX_Text(pDX, IDC_EDITLANG, m_Lang);
	DDX_Text(pDX, IDC_MODENAME, m_ModeName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFindMode, CDialog)
	//{{AFX_MSG_MAP(CDlgFindMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDlgFindMode message handlers

BOOL CDlgFindMode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int i;

	// insert the types of Grammars...
	for( i=0; i<NFINDGRAMTYPES; i++ )
	{
		m_GramType.AddString( szFindGramTypes[i] );
	}

	// insert the types of Grammars...
	for( i=0; i<NFINDFEATURES; i++ )
	{
		m_Features.AddString( szFindFeatures[i] );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFindMode::OnOK() 
{
	int i;
	int iSel[NFINDFEATURES];

	switch( m_Sequence.GetCurSel() )
	{
		case 0: m_iSequence = SRSEQUENCE_CONTINUOUS; break;
		case 1: m_iSequence = SRSEQUENCE_DISCRETE;   break;
		case 2: m_iSequence = SRSEQUENCE_WORDSPOT;   break;
	}

	for( m_iGramType=i=0; i<m_GramType.GetSelItems( NFINDGRAMTYPES, iSel ); i++ )
	{
		m_iGramType |= 1<<iSel[i];
	}

	m_Features.GetSelItems( NFINDFEATURES, iSel );

	for( m_iFeature=i=0; i<m_Features.GetSelItems( NFINDFEATURES, iSel ); i++ )
	{
		m_iFeature |= 1<<iSel[i];
	}

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CGramAddCustom dialog


CGramAddCustom::CGramAddCustom(CWnd* pParent /*=NULL*/)
	: CDialog(CGramAddCustom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGramAddCustom)
	m_GramAddEdit = _T("");
	//}}AFX_DATA_INIT
}


void CGramAddCustom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGramAddCustom)
	DDX_Text(pDX, IDC_GRAMADDCUSTEDIT, m_GramAddEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGramAddCustom, CDialog)
	//{{AFX_MSG_MAP(CGramAddCustom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGramAddCustom message handlers

void CGramAddCustom::OnOK() 
{
	CDialog::OnOK();
}
