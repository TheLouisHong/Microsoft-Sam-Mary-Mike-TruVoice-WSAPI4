/******************************************************************************\
*       tabisrce.cpp
*
*       This module contains most of the UI handling for the engine and 
*       grammar property sheets.
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
#include "srdlg.h"
#include "srfunc.h"
#include "dlgfindm.h"
#include <mmsystem.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

int const GUIDSTRBUFSIZE = 64;
int const NSELBUF = 100;
int iSelBuf[NSELBUF];
DWORD dwListWordID=1;   // keeps appended word ID's unique
int       giResSel=-1;   // results object listbox current selection index
LPUNKNOWN gpResults;   // results object listbox current item data
TCHAR     gszResultsTxt[MAX_STRLEN];  // results object listbox string
DWORD     gdwSplitTime=1000;          // split time for ISRResMerge::Split

PTCHAR szDialogTitle[5]={
TEXT("SRTest General Dialog"),
TEXT("SRTest Lexicon Dialog"),
TEXT("SRTest General Training Dialog"),
TEXT("SRTest Microphone Training Dialog"),
TEXT("SRTest About Dialog")};

int const NGRAMORIGWORDS = 9;
PTCHAR szGramOrigWords[NGRAMORIGWORDS]={
TEXT("apple"),
TEXT("orange"),
TEXT("banana"),
TEXT("peach"),
TEXT("pear"),
TEXT("grapefruit"),
TEXT("watermellon"),
TEXT("bcdfghjklmnpqrstvwxyzbcdfghjklmn"),   // special case
TEXT("sdfghjklkjhgfdsdfghjklkjhgfdsdfg")};   // special case


/****************************************************************************

                           PROPERTY PAGE MAPS

****************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CTabISRCentral property page

IMPLEMENT_DYNCREATE(CTabISRCentral, CPropertyPage)

CTabISRCentral::CTabISRCentral() : CPropertyPage(CTabISRCentral::IDD)
{
   //{{AFX_DATA_INIT(CTabISRCentral)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
   
   m_bPosTracking = FALSE;
}

CTabISRCentral::~CTabISRCentral()
{
   // kill the position tracking if it's on...
   SetPosTimer( FALSE );
}

void CTabISRCentral::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRCentral)
   DDX_Control(pDX, IDC_USEARCHIVE, m_UseArchive);
   DDX_Control(pDX, IDC_TOFILEOUTPUT, m_ToFileOut);
   DDX_Control(pDX, IDC_POSITIONTRACK, m_PosTrack);
   DDX_Control(pDX, IDC_ENGCTLPAUSE, m_EngPaused);
   DDX_Control(pDX, IDC_POSGETOUTPUT, m_PosGetOutput);
   DDX_Control(pDX, IDC_LISTGRAMMARS, m_EnumGram);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRCentral, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRCentral)
   ON_BN_CLICKED(IDC_CENTGRAMLOAD, OnCentgramload)
   ON_LBN_DBLCLK(IDC_LISTGRAMMARS, OnDblclkListgrammars)
   ON_BN_CLICKED(IDC_CENTGRAMRELEASEALL, OnCentgramreleaseall)
   ON_BN_CLICKED(IDC_ENGCTLPAUSE, OnEngctlpause)
   ON_WM_TIMER()
   ON_BN_CLICKED(IDC_POSITIONTRACK, OnPositiontrack)
   ON_BN_CLICKED(IDC_CENTMODEGET, OnCentmodeget)
   ON_BN_CLICKED(IDC_GRAMRESCAN, OnGramrescan)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabISRGrammar property page

IMPLEMENT_DYNCREATE(CTabISRGrammar, CPropertyPage)

CTabISRGrammar::CTabISRGrammar() : CPropertyPage(CTabISRGrammar::IDD)
{
   //{{AFX_DATA_INIT(CTabISRGrammar)
   //}}AFX_DATA_INIT
}

CTabISRGrammar::~CTabISRGrammar()
{
}

void CTabISRGrammar::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRGrammar)
   DDX_Control(pDX, IDC_RULEACTIVELIST, m_RuleActiveList);
   DDX_Control(pDX, IDC_GRAMWND, m_GramWnd);
   DDX_Control(pDX, IDC_GRAMPAUSE, m_GramAutoPause);
   DDX_Control(pDX, IDC_GRAMRULE, m_GramRuleList);
   DDX_Control(pDX, IDC_BOOKMARKEDIT, m_BookMarkEdit);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRGrammar, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRGrammar)
   ON_BN_CLICKED(IDC_GRAMTRAIN, OnGramtrain)
   ON_BN_CLICKED(IDC_GRAMTRAINPHRASE, OnGramtrainphrase)
   ON_BN_CLICKED(IDC_GRAMTRAINQUERY, OnGramtrainquery)
   ON_BN_CLICKED(IDC_GRAMDETERIORGET, OnGramdeteriorget)
   ON_BN_CLICKED(IDC_GRAMBOOKMARK, OnGrambookmark)
   ON_BN_CLICKED(IDC_GRAMDETERIORSET, OnGramdeteriorset)
   ON_BN_CLICKED(IDC_GRAMARCHIVE, OnGramarchive)
   ON_BN_CLICKED(IDC_GRAMACTIVATEBUT, OnGramactivate)
   ON_BN_CLICKED(IDC_GRAMDEACTIVATEGRAM, OnGramdeactivategram)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabISRGramCFG property page

IMPLEMENT_DYNCREATE(CTabISRGramCFG, CPropertyPage)

CTabISRGramCFG::CTabISRGramCFG() : CPropertyPage(CTabISRGramCFG::IDD)
{
   //{{AFX_DATA_INIT(CTabISRGramCFG)
   //}}AFX_DATA_INIT
}

CTabISRGramCFG::~CTabISRGramCFG()
{
}

void CTabISRGramCFG::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRGramCFG)
   DDX_Control(pDX, IDC_GRAMLISTNAME, m_GramListName);
   DDX_Control(pDX, IDC_GRAMLISTQUERY, m_GramListQuery);
   DDX_Control(pDX, IDC_CFGORIGLIST, m_GramOrigWords);
   DDX_Control(pDX, IDC_GRAMLINKNAME, m_GramLinkName);
   DDX_Control(pDX, IDC_GRAMCFGLIST, m_GramCFGWords);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabISRGramCFG, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRGramCFG)
   ON_BN_CLICKED(IDC_GRAMLINKQUERY, OnGramlinkquery)
   ON_BN_CLICKED(IDC_GRAMCFGLISTSET, OnGramcfglistset)
   ON_BN_CLICKED(IDC_GRAMLISTQUERY, OnGramlistquery)
   ON_BN_CLICKED(IDC_GRAMCFGLISTREMOVE, OnGramcfglistremove)
   ON_BN_CLICKED(IDC_GRAMCFGLISTAPPEND, OnGramcfglistappend)
   ON_BN_CLICKED(IDC_GRAMCUSTOM, OnGramcustom)
   ON_CBN_CLOSEUP(IDC_GRAMLISTNAME, OnCloseupGramlistname)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabISRGramDict property page

IMPLEMENT_DYNCREATE(CTabISRGramDict, CPropertyPage)

CTabISRGramDict::CTabISRGramDict() : CPropertyPage(CTabISRGramDict::IDD)
{
   //{{AFX_DATA_INIT(CTabISRGramDict)
   //}}AFX_DATA_INIT
}

CTabISRGramDict::~CTabISRGramDict()
{
}

void CTabISRGramDict::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRGramDict)
   DDX_Control(pDX, IDC_LIKELYWORDS, m_GramLikelyWords);
   DDX_Control(pDX, IDC_GRAMAFTEREDIT, m_GramBeforeEdit);
   DDX_Control(pDX, IDC_GRAMBEFOREEDIT, m_GramAfterEdit);
   DDX_Control(pDX, IDC_GRAMDICTHINT, m_GramDictHint);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRGramDict, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRGramDict)
   ON_BN_CLICKED(IDC_GRAMDICTWORDS, OnGramdictwords)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabISRAttr property page

IMPLEMENT_DYNCREATE(CTabISRAttr, CPropertyPage)

CTabISRAttr::CTabISRAttr() : CPropertyPage(CTabISRAttr::IDD)
{
   //{{AFX_DATA_INIT(CTabISRAttr)
   //}}AFX_DATA_INIT
}

CTabISRAttr::~CTabISRAttr()
{
}

void CTabISRAttr::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRAttr)
   DDX_Control(pDX, IDC_ATTRSPEAKER, m_Speaker);
   DDX_Control(pDX, IDC_ATTRIBECHO, m_Echo);
   DDX_Control(pDX, IDC_ATTRTIMEOUTI, m_TimeOutI);
   DDX_Control(pDX, IDC_ATTRTIMEOUTC, m_TimeOutC);
   DDX_Control(pDX, IDC_ATTRCPU, m_CPU);
   DDX_Control(pDX, IDC_ATTRMIC, m_Mic);
   DDX_Control(pDX, IDC_ATTRENERGY, m_EnergyFlr);
   DDX_Control(pDX, IDC_ATTRTHRESH, m_Thresh);
   DDX_Control(pDX, IDC_ATTRGAIN, m_AttrGain);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabISRAttr, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRAttr)
   ON_EN_KILLFOCUS(IDC_ATTRGAIN, OnKillfocusAttrgain)
   ON_EN_KILLFOCUS(IDC_ATTRTHRESH, OnKillfocusAttrthresh)
   ON_EN_KILLFOCUS(IDC_ATTRENERGY, OnKillfocusAttrenergy)
   ON_EN_KILLFOCUS(IDC_ATTRMIC, OnKillfocusAttrmic)
   ON_EN_KILLFOCUS(IDC_ATTRCPU, OnKillfocusAttrcpu)
   ON_EN_KILLFOCUS(IDC_ATTRTIMEOUTI, OnKillfocusAttrtimeouti)
   ON_EN_KILLFOCUS(IDC_ATTRTIMEOUTC, OnKillfocusAttrtimeoutc)
   ON_BN_CLICKED(IDC_ATTRIBECHO, OnAttribecho)
   ON_EN_KILLFOCUS(IDC_ATTRSPEAKER, OnKillfocusAttrspeaker)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabISRDialogs property page

IMPLEMENT_DYNCREATE(CTabISRDialogs, CPropertyPage)

CTabISRDialogs::CTabISRDialogs() : CPropertyPage(CTabISRDialogs::IDD)
{
   //{{AFX_DATA_INIT(CTabISRDialogs)
	//}}AFX_DATA_INIT
}

CTabISRDialogs::~CTabISRDialogs()
{
}

void CTabISRDialogs::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRDialogs)
	DDX_Control(pDX, IDC_TRAINTEXT, m_TrainText);
   DDX_Control(pDX, IDC_DLGTITLEOPT, m_DlgTitleOpt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRDialogs, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRDialogs)
   ON_BN_CLICKED(IDC_ISRDLGGENERAL, OnIsrdlggeneral)
   ON_BN_CLICKED(IDC_ISRDLGLEX, OnIsrdlglex)
   ON_BN_CLICKED(IDC_ISRDLGTRAINGEN, OnIsrdlgtraingen)
   ON_BN_CLICKED(IDC_ISRDLGTRAINMIC, OnIsrdlgtrainmic)
   ON_BN_CLICKED(IDC_ISRDLGABOUT, OnIsrdlgabout)
	ON_BN_CLICKED(IDC_TRAINPHRASE, OnTrainphrase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabISRNotify property page

IMPLEMENT_DYNCREATE(CTabISRNotify, CPropertyPage)

CTabISRNotify::CTabISRNotify() : CPropertyPage(CTabISRNotify::IDD)
{
   //{{AFX_DATA_INIT(CTabISRNotify)
   //}}AFX_DATA_INIT
}

CTabISRNotify::~CTabISRNotify()
{
}

void CTabISRNotify::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRNotify)
   DDX_Control(pDX, IDC_NOTIFYATTR, m_Attr);
   DDX_Control(pDX, IDC_NOTIFYVMETER, m_VMeter);
   DDX_Control(pDX, IDC_NOTIFYUTTR, m_Utter);
   DDX_Control(pDX, IDC_NOTIFYSND, m_Sound);
   DDX_Control(pDX, IDC_NOTIFYINTERFER, m_Interfere);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRNotify, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRNotify)
   ON_BN_CLICKED(IDC_NOTIFYATTR, OnNotifyattr)
   ON_BN_CLICKED(IDC_NOTIFYINTERFER, OnNotifyinterfer)
   ON_BN_CLICKED(IDC_NOTIFYSND, OnNotifysnd)
   ON_BN_CLICKED(IDC_NOTIFYUTTR, OnNotifyuttr)
   ON_BN_CLICKED(IDC_NOTIFYVMETER, OnNotifyvmeter)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTabISRGramNotify property page

IMPLEMENT_DYNCREATE(CTabISRGramNotify, CPropertyPage)

CTabISRGramNotify::CTabISRGramNotify() : CPropertyPage(CTabISRGramNotify::IDD)
{
   //{{AFX_DATA_INIT(CTabISRGramNotify)
   //}}AFX_DATA_INIT
}

CTabISRGramNotify::~CTabISRGramNotify()
{
}

void CTabISRGramNotify::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRGramNotify)
   DDX_Control(pDX, IDC_GRAMNOTBOOKMARK, m_Bookmark);
   DDX_Control(pDX, IDC_GRAMNOTPAUSED, m_Paused);
   DDX_Control(pDX, IDC_GRAMNOTUNARC, m_Unarchive);
   DDX_Control(pDX, IDC_GRAMNOTREEVAL, m_ReEval);
   DDX_Control(pDX, IDC_GRAMNOTPHRASHYP, m_PhraseHyp);
   DDX_Control(pDX, IDC_GRAMNOTPHRASE, m_Phrase);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRGramNotify, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRGramNotify)
   ON_BN_CLICKED(IDC_GRAMNOTBOOKMARK, OnGramnotbookmark)
   ON_BN_CLICKED(IDC_GRAMNOTPAUSED, OnGramnotpaused)
   ON_BN_CLICKED(IDC_GRAMNOTPHRASE, OnGramnotphrase)
   ON_BN_CLICKED(IDC_GRAMNOTPHRASHYP, OnGramnotphrashyp)
   ON_BN_CLICKED(IDC_GRAMNOTREEVAL, OnGramnotreeval)
   ON_BN_CLICKED(IDC_GRAMNOTUNARC, OnGramnotunarc)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabISRResAud property page

IMPLEMENT_DYNCREATE(CTabISRResAud, CPropertyPage)

CTabISRResAud::CTabISRResAud() : CPropertyPage(CTabISRResAud::IDD)
{
   //{{AFX_DATA_INIT(CTabISRResAud)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}

CTabISRResAud::~CTabISRResAud()
{
}

void CTabISRResAud::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRResAud)
   DDX_Control(pDX, IDC_AUDIOFMTTXT, m_AudioFmtTxt);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRResAud, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRResAud)
   ON_BN_CLICKED(IDC_AUDIOGETWAVE, OnAudiogetwave)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTabISRResBasic property page

IMPLEMENT_DYNCREATE(CTabISRResBasic, CPropertyPage)

CTabISRResBasic::CTabISRResBasic() : CPropertyPage(CTabISRResBasic::IDD)
{
   //{{AFX_DATA_INIT(CTabISRResBasic)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}

CTabISRResBasic::~CTabISRResBasic()
{
}

void CTabISRResBasic::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRResBasic)
   DDX_Control(pDX, IDC_PHRASEGETTXT, m_PhraseGetTxt);
   DDX_Control(pDX, IDC_FLAGSGETTXT, m_FlagsGetTxt);
   DDX_Control(pDX, IDC_TIMEENDTXT, m_TimeEndTxt);
   DDX_Control(pDX, IDC_TIMEBEGINTXT, m_TimeBeginTxt);
   DDX_Control(pDX, IDC_RESOBJSELECT, m_ResSelection);
   DDX_Control(pDX, IDC_IDENTIFYTXT, m_ResIdentifyTxt);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabISRResBasic, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRResBasic)
   ON_BN_CLICKED(IDC_RESIDENTIFY, OnResidentify)
   ON_BN_CLICKED(IDC_RESTIMEGET, OnRestimeget)
   ON_LBN_KILLFOCUS(IDC_RESOBJSELECT, OnKillfocusResobjselect)
   ON_BN_CLICKED(IDC_FLAGSGET, OnFlagsget)
   ON_BN_CLICKED(IDC_PHRASEGET, OnPhraseget)
   ON_LBN_SELCHANGE(IDC_RESOBJSELECT, OnSelchangeResobjselect)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTabISRResCorr property page

IMPLEMENT_DYNCREATE(CTabISRResCorr, CPropertyPage)

CTabISRResCorr::CTabISRResCorr() : CPropertyPage(CTabISRResCorr::IDD)
{
   //{{AFX_DATA_INIT(CTabISRResCorr)
   //}}AFX_DATA_INIT
}

CTabISRResCorr::~CTabISRResCorr()
{
}

void CTabISRResCorr::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRResCorr)
   DDX_Control(pDX, IDC_CORRCONF, m_CorrConfidList);
   DDX_Control(pDX, IDC_CORRVALIDATE, m_CorrValidateList);
   DDX_Control(pDX, IDC_CORRPHRASE, m_CorrPhraseEdit);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRResCorr, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRResCorr)
   ON_BN_CLICKED(IDC_CORRVALID, OnCorrvalid)
   ON_BN_CLICKED(IDC_CORRCORR, OnCorrcorr)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTabISRResEval property page

IMPLEMENT_DYNCREATE(CTabISRResEval, CPropertyPage)

CTabISRResEval::CTabISRResEval() : CPropertyPage(CTabISRResEval::IDD)
{
   //{{AFX_DATA_INIT(CTabISRResEval)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}

CTabISRResEval::~CTabISRResEval()
{
}

void CTabISRResEval::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRResEval)
   DDX_Control(pDX, IDC_REEVALTXT, m_ReEvalTxt);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabISRResEval, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRResEval)
   ON_BN_CLICKED(IDC_REEVALUATE, OnReevaluate)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTabISRResGraph property page

IMPLEMENT_DYNCREATE(CTabISRResGraph, CPropertyPage)

CTabISRResGraph::CTabISRResGraph() : CPropertyPage(CTabISRResGraph::IDD)
{
   //{{AFX_DATA_INIT(CTabISRResGraph)
	//}}AFX_DATA_INIT
}

CTabISRResGraph::~CTabISRResGraph()
{
}

void CTabISRResGraph::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRResGraph)
	DDX_Control(pDX, IDC_GRAPHOUTPUT, m_GraphOutput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRResGraph, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRResGraph)
   ON_BN_CLICKED(IDC_BESTPATHPH, OnBestpathph)
   ON_BN_CLICKED(IDC_BESTPATHW, OnBestpathw)
   ON_BN_CLICKED(IDC_PATHSCOREW, OnPathscorew)
   ON_BN_CLICKED(IDC_PATHSCOREPH, OnPathscoreph)
   ON_BN_CLICKED(IDC_GETPHNODE, OnGetphnode)
   ON_BN_CLICKED(IDC_GETWNODE, OnGetwnode)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTabISRResMem property page

IMPLEMENT_DYNCREATE(CTabISRResMem, CPropertyPage)

CTabISRResMem::CTabISRResMem() : CPropertyPage(CTabISRResMem::IDD)
{
   //{{AFX_DATA_INIT(CTabISRResMem)
   //}}AFX_DATA_INIT
}

CTabISRResMem::~CTabISRResMem()
{
}

void CTabISRResMem::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRResMem)
   DDX_Control(pDX, IDC_MEMGETTXT, m_MemGetTxt);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRResMem, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRResMem)
   ON_BN_CLICKED(IDC_MEMGET, OnMemget)
   ON_BN_CLICKED(IDC_LOCKSET, OnLockset)
   ON_BN_CLICKED(IDC_LOCKGET, OnLockget)
   ON_BN_CLICKED(IDC_MEMFREE, OnMemfree)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabISRResMerge property page

IMPLEMENT_DYNCREATE(CTabISRResMerge, CPropertyPage)

CTabISRResMerge::CTabISRResMerge() : CPropertyPage(CTabISRResMerge::IDD)
{
   //{{AFX_DATA_INIT(CTabISRResMerge)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}

CTabISRResMerge::~CTabISRResMerge()
{
}

void CTabISRResMerge::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRResMerge)
   DDX_Control(pDX, IDC_RESMERGETXT, m_ResMergeTxt);
   DDX_Control(pDX, IDC_RESSPLITEDIT, m_ResSplitEdit);
   DDX_Control(pDX, IDC_RESSPLITTXT, m_ResSplitTxt);
   DDX_Control(pDX, IDC_RESMERGESEL, m_ResMergeSel);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRResMerge, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRResMerge)
   ON_BN_CLICKED(IDC_RESMERGE, OnResmerge)
   ON_BN_CLICKED(IDC_RESSPLIT, OnRessplit)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabISRResModGui property page

IMPLEMENT_DYNCREATE(CTabISRResModGui, CPropertyPage)

CTabISRResModGui::CTabISRResModGui() : CPropertyPage(CTabISRResModGui::IDD)
{
   //{{AFX_DATA_INIT(CTabISRResModGui)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}

CTabISRResModGui::~CTabISRResModGui()
{
}

void CTabISRResModGui::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRResModGui)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRResModGui, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRResModGui)
   ON_BN_CLICKED(IDC_MODGUIHIDE, OnModguihide)
   ON_BN_CLICKED(IDC_MODGUIMOVE, OnModguimove)
   ON_BN_CLICKED(IDC_MODGUISHOW, OnModguishow)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabISRResSpeak property page

IMPLEMENT_DYNCREATE(CTabISRResSpeak, CPropertyPage)

CTabISRResSpeak::CTabISRResSpeak() : CPropertyPage(CTabISRResSpeak::IDD)
{
   //{{AFX_DATA_INIT(CTabISRResSpeak)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}

CTabISRResSpeak::~CTabISRResSpeak()
{
}

void CTabISRResSpeak::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRResSpeak)
   DDX_Control(pDX, IDC_SPKRCONF, m_SpkrValConf);
   DDX_Control(pDX, IDC_SPKRCORRCONF, m_SpkrCorrConf);
   DDX_Control(pDX, IDC_CORRNAME, m_CorrName);
   DDX_Control(pDX, IDC_SPKRIDTXT, m_SpkrIDTxt);
   DDX_Control(pDX, IDC_IDFORFREETXT, m_IDForFreeTxt);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabISRResSpeak, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRResSpeak)
   ON_BN_CLICKED(IDC_SPKRIDENTIFY, OnSpkridentify)
   ON_BN_CLICKED(IDC_IDFORFREE, OnIdforfree)
   ON_BN_CLICKED(IDC_SPKRCORR, OnSpkrcorr)
   ON_BN_CLICKED(IDC_SPKRVALIDATE, OnSpkrvalidate)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/****************************************************************************

                        PROPERTY PAGE MESSAGE HANDLERS

****************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CTabISRCentral message handlers

BOOL CTabISRCentral::OnSetActive()
{
   if( !CPropertyPage::OnSetActive() ) return FALSE;

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();

   return TRUE;
}

void CTabISRCentral::OnTimer(UINT nIDEvent) 
{
   if( nIDEvent == ID_TIMERPOSTRACK ) OnPositionget();
      
   CPropertyPage::OnTimer(nIDEvent);
}

void CTabISRCentral::OnCentmodeget() 
{
   SRMODEINFO SRInfo;
   HRESULT    hRes = gpEng->m_pISRCentral->ModeGet( &SRInfo );
   if( !CheckError( hRes, TEXT("Error calling ModeGet()") ) )
      DisplayModeInfo( &SRInfo );
}

void CTabISRCentral::EnumGrammars( void )
{
   int i;

   // clear the list box...
   m_EnumGram.ResetContent();

   // put the filenames in the listbox...
   for( i=0; i<gnGramFiles; i++ )
   {
      m_EnumGram.InsertString( i, gpGramFiles[i].szName );
   }
}

// Rescan the grammar path for grammar files and store them in the listbox.
void CTabISRCentral::OnGramrescan() 
{
   CGramPath DlgGramPath;
   if( DlgGramPath.DoModal() == IDCANCEL ) return;
   else if( !gszGramPath[0] )
   {
      MainOutput( TEXT("Invalid path specified.\n") );
      return;
   }

   ((CSRTestDlg *)AfxGetMainWnd())->EnumGramFiles();

   EnumGrammars();
}

/*
 The user pressed the grammar load button, create a new grammar object and it to the
 grammar list.
*/
void CTabISRCentral::OnCentgramload() 
{
   TCHAR buf[MAX_STRLEN];
   TCHAR szName[MAX_STRLEN];

   // get the grammar name, append a path...
   m_EnumGram.GetCurSel();
   m_EnumGram.GetText( m_EnumGram.GetCurSel(), szName );
   wsprintf( buf, TEXT("%s\\%s"), gszGramPath, szName );

   // create a new grammar object and make it the current one (m_pGram)...
   if( gpEng->CreateGrammarObject( buf, m_UseArchive.GetCheck() ) )
   {
      wsprintf( buf, TEXT("Failed to create new grammar object for engine mode.\n") );
      MainOutput( buf );
      MessageBox( buf, NULL, MB_OK );
      return;
   }

   MainOutput( TEXT("Grammar object created successfully.\n") );

   // add it to the listbox for user selection and select it,
   // note the new grammar object is now referenced by gpEng->m_pGram...
   ((CSRTestDlg *)AfxGetMainWnd())->m_ListGramStat.InsertString( gpEng->m_nLoadedGrams-1, szName );
   ((CSRTestDlg *)AfxGetMainWnd())->m_ListGramStat.SetCurSel( gpEng->m_nLoadedGrams-1 );

   // anable the grammar property sheet...
   ((CSRTestDlg *)AfxGetMainWnd())->m_PropGram->EnableWindow( TRUE );
}

/*
 User double-clicked on a grammar name in the listbox.
*/
void CTabISRCentral::OnDblclkListgrammars() 
{
   OnCentgramload();
}

/*
 User pressed the release all grammar button.
*/
void CTabISRCentral::OnCentgramreleaseall() 
{
   gpEng->DeleteAllGrammars();

   // clear the grammar listbox...
   ((CSRTestDlg *)AfxGetMainWnd())->m_ListGramStat.ResetContent();
}

/*
 User pressed the engine pause button.
*/
void CTabISRCentral::OnEngctlpause() 
{
   TCHAR buf[MAX_STRLEN];
   HRESULT hRes;

   if( !gpEng )
   {
      MessageBox(TEXT("No Engine mode loaded yet."),NULL,MB_OK);
      return;
   }

   gpEng->m_bPaused = m_EngPaused.GetCheck();

   // Pause the engine based on current state...
   if( gpEng->m_bPaused )
   {
      hRes = gpEng->m_pISRCentral->Pause();
      if( hRes ) wsprintf( buf, TEXT("\nError Pausing recognition engine (%x).\n"), hRes );
      else wsprintf( buf, TEXT("\nEngine recognition paused.\n"));
   }
   else
   {
      hRes = gpEng->m_pISRCentral->Resume();
      if( hRes ) wsprintf( buf, TEXT("\nError Resuming recognition engine (%x)\n."));
      else wsprintf( buf, TEXT("\nEngine recognition resumed.\n"));
   }

   MainOutput( buf );
}

void CTabISRCentral::OnPositiontrack() 
{
   SetPosTimer( m_PosTrack.GetCheck() );
}

void CTabISRCentral::SetPosTimer( int flag )
{
   if( flag )
   {
      SetTimer( ID_TIMERPOSTRACK, 1000, NULL );
      m_bPosTracking = TRUE;
   }
   else
   {
      if( m_bPosTracking ) KillTimer( ID_TIMERPOSTRACK );
      m_bPosTracking = FALSE;
   }
}

void CALLBACK EXPORT CTabISRCentral::OnPositionget() 
{
   TCHAR  buf[MAX_STRLEN];
   QWORD qTimestamp=0;
   FILETIME Ftime;

   HRESULT hRes = gpEng->m_pISRCentral->PosnGet( &qTimestamp );
   if( FAILED(hRes) ) wsprintf( buf, TEXT("Error PosnGet() (%lx)"), hRes );
   else _stprintf( buf, TEXT("Pos: %I64u"), qTimestamp );
   m_PosGetOutput.SetWindowText( buf );

   hRes = gpEng->m_pISRCentral->ToFileTime( &qTimestamp, &Ftime );
   if( FAILED(hRes) ) wsprintf( buf, TEXT("Error ToFileTime() (%lx)"), hRes );
   else _stprintf( buf,TEXT("FileTime: %lx %lx"), Ftime.dwHighDateTime, Ftime.dwLowDateTime );
   
   m_ToFileOut.SetWindowText( buf );
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRAttr message handlers

BOOL CTabISRAttr::OnSetActive()
{
   if( !CPropertyPage::OnSetActive() ) return FALSE;

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();

   return TRUE;
}

void CTabISRAttr::OnKillfocusAttrgain() 
{
   TCHAR buf[MAX_STRLEN];
   HRESULT hRes;

   m_AttrGain.GetWindowText( buf, sizeof(buf) );

   gpEng->m_Attr.dwAGainEnable = _ttol( buf );
   hRes = gpEng->m_pISRAttributes->AutoGainEnableSet( gpEng->m_Attr.dwAGainEnable );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("AutogainEnableSet not Implemented") : TEXT("Error calling AutoGainEnableSet()") );

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();
}

void CTabISRAttr::OnKillfocusAttrthresh() 
{
   TCHAR buf[MAX_STRLEN];
   HRESULT hRes;

   m_Thresh.GetWindowText( buf, sizeof(buf) );

   gpEng->m_Attr.dwThresh = _ttol( buf );
   hRes = gpEng->m_pISRAttributes->ThresholdSet( gpEng->m_Attr.dwThresh );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("ThresholdSet not Implemented") : TEXT("Error calling ThresholdSet()") );

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();
}

void CTabISRAttr::OnKillfocusAttrenergy() 
{
   TCHAR buf[MAX_STRLEN];
   HRESULT hRes;

   m_EnergyFlr.GetWindowText( buf, sizeof(buf) );

   gpEng->m_Attr.wEnergyFlr = (WORD)_ttol( buf );
   hRes = gpEng->m_pISRAttributes->EnergyFloorSet( gpEng->m_Attr.wEnergyFlr );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("EnergyfloorSet not Implemented") : TEXT("Error calling EnergyFloorSet()") );

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();
}

void CTabISRAttr::OnKillfocusAttrmic() 
{
   HRESULT hRes;

   m_Mic.GetWindowText( gpEng->m_Attr.szMicName, sizeof(gpEng->m_Attr.szMicName) );

   APIALLOC( speechbuf, MAX_STRLEN );
   APIA2SCVT( speechbuf, gpEng->m_Attr.szMicName );
   hRes = gpEng->m_pISRAttributes->MicrophoneSet( speechbuf );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("MicrophoneSet not Implemented") : TEXT("Error calling MicrophoneSet()") );
   APIFREE( speechbuf );

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();
}

void CTabISRAttr::OnKillfocusAttrspeaker() 
{
   HRESULT hRes;

   m_Speaker.GetWindowText( gpEng->m_Attr.szSpeakName, sizeof(gpEng->m_Attr.szSpeakName) );

   APIALLOC( speechbuf, _tcslen(gpEng->m_Attr.szSpeakName) );
   APIA2SCVT( speechbuf, gpEng->m_Attr.szSpeakName );
   hRes = gpEng->m_pISRAttributes->SpeakerSet( speechbuf );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("SpeakerSet not Implemented") : TEXT("Error calling SpeakerSet()") );
   APIFREE( speechbuf );

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();
}

void CTabISRAttr::OnKillfocusAttrcpu() 
{
   TCHAR buf[MAX_STRLEN];
   HRESULT hRes;

   m_CPU.GetWindowText( buf, sizeof(buf) );

   gpEng->m_Attr.dwRealTime = _ttol( buf );
   hRes = gpEng->m_pISRAttributes->RealTimeSet( gpEng->m_Attr.dwRealTime );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("RealTimeSet not Implemented") : TEXT("Error calling RealTimeSet()") );

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();
}

void CTabISRAttr::OnKillfocusAttrtimeoutc() 
{
   TCHAR buf[MAX_STRLEN];
   HRESULT hRes;

   m_TimeOutC.GetWindowText( buf, sizeof(buf) );

   gpEng->m_Attr.dwTimeOutC = _ttol( buf );
   hRes = gpEng->m_pISRAttributes->TimeOutSet( gpEng->m_Attr.dwTimeOutI, gpEng->m_Attr.dwTimeOutC );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("TimeOutSet not Implemented") : TEXT("Error calling TimeOutSet()") );

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();
}

void CTabISRAttr::OnKillfocusAttrtimeouti() 
{
   TCHAR buf[MAX_STRLEN];
   HRESULT hRes;

   m_TimeOutI.GetWindowText( buf, sizeof(buf) );

   gpEng->m_Attr.dwTimeOutI = _ttol( buf );
   hRes = gpEng->m_pISRAttributes->TimeOutSet( gpEng->m_Attr.dwTimeOutI, gpEng->m_Attr.dwTimeOutC );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("TimeOutset not Implemented") : TEXT("Error calling TimeOutSet()") );

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();
}

void CTabISRAttr::OnAttribecho() 
{
   HRESULT hRes;

   gpEng->m_Attr.bEcho = IsDlgButtonChecked( IDC_ATTRIBECHO );
   hRes = gpEng->m_pISRAttributes->EchoSet( gpEng->m_Attr.bEcho );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("EchoSet not Implemented") : TEXT("Error calling EchoSet()") );

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRDialogs message handlers

BOOL CTabISRDialogs::OnSetActive()
{
   if( !CPropertyPage::OnSetActive() ) return FALSE;

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();

   return TRUE;
}

void CTabISRDialogs::OnIsrdlggeneral() 
{
   HWND hWnd = m_hWnd;
   HRESULT hRes;

   if( m_DlgTitleOpt.GetCheck() )
   {
      APIALLOC( speechtitle, MAX_STRLEN );
      APIA2SCVT( speechtitle, szDialogTitle[0] );
      hRes = gpEng->m_pISRDialogs->GeneralDlg( hWnd, speechtitle );
      APIFREE( speechtitle );
   }
   else
      hRes = gpEng->m_pISRDialogs->GeneralDlg( hWnd, NULL );

   DWORD scode = SCODE_CODE( hRes );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("GeneralDlg not Implemented") : TEXT("Error calling GeneralDlg()") );
}

void CTabISRDialogs::OnIsrdlglex() 
{
   HWND hWnd = m_hWnd;
   HRESULT hRes;

   if( m_DlgTitleOpt.GetCheck() )
   {
      APIALLOC( speechtitle, MAX_STRLEN );
      APIA2SCVT( speechtitle, szDialogTitle[1] );
      hRes = gpEng->m_pISRDialogs->LexiconDlg( hWnd, speechtitle );
      APIFREE( speechtitle );
   }
   else
      hRes = gpEng->m_pISRDialogs->LexiconDlg( hWnd, NULL);

   DWORD scode = SCODE_CODE( hRes );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("LexiconDlg not Implemented") : TEXT("Error calling LexiconDlg()") );
}

void CTabISRDialogs::OnIsrdlgtraingen() 
{
   HWND hWnd = m_hWnd;
   HRESULT hRes;

   if( m_DlgTitleOpt.GetCheck() )
   {
      APIALLOC( speechtitle, MAX_STRLEN );
      APIA2SCVT( speechtitle, szDialogTitle[2] );
      hRes = gpEng->m_pISRDialogs->TrainGeneralDlg( hWnd, speechtitle );
      APIFREE( speechtitle );
   }
   else
      hRes = gpEng->m_pISRDialogs->TrainGeneralDlg( hWnd, NULL);

   DWORD scode = SCODE_CODE( hRes );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("TrainGeneralDlg not Implemented") : TEXT("Error calling TrainGeneralDlg()") );
}

void CTabISRDialogs::OnIsrdlgtrainmic() 
{
   HWND hWnd = m_hWnd;
   HRESULT hRes;

   if( m_DlgTitleOpt.GetCheck() )
   {
      APIALLOC( speechtitle, MAX_STRLEN );
      APIA2SCVT( speechtitle, szDialogTitle[3] );
      hRes = gpEng->m_pISRDialogs->TrainMicDlg( hWnd, speechtitle );
      APIFREE( speechtitle );
   }
   else
      hRes = gpEng->m_pISRDialogs->TrainMicDlg( hWnd, NULL);

   DWORD scode = SCODE_CODE( hRes );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("TrainMicDlg not Implemented") : TEXT("Error calling TrainMicDlg()") );
}

void CTabISRDialogs::OnIsrdlgabout() 
{
   HWND hWnd = m_hWnd;
   HRESULT hRes;

   if( m_DlgTitleOpt.GetCheck() )
   {
      APIALLOC( speechtitle, MAX_STRLEN );
      APIA2SCVT( speechtitle, szDialogTitle[4] );
      hRes = gpEng->m_pISRDialogs->AboutDlg( hWnd, speechtitle );
      APIFREE( speechtitle );
   }
   else
      hRes = gpEng->m_pISRDialogs->AboutDlg( hWnd, NULL );

   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("AboutDlg not Implemented") : TEXT("Error calling AboutDlg()") );
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRNotify message handlers

BOOL CTabISRNotify::OnSetActive()
{
   if( !CPropertyPage::OnSetActive() ) return FALSE;

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();

   return TRUE;
}

void CTabISRNotify::OnNotifyattr() 
{
   gpEng->m_Not.bAttrChange = IsDlgButtonChecked( IDC_NOTIFYATTR );
}

void CTabISRNotify::OnNotifyinterfer() 
{
   gpEng->m_Not.bInterfere = IsDlgButtonChecked( IDC_NOTIFYINTERFER );
}

void CTabISRNotify::OnNotifysnd() 
{
   gpEng->m_Not.bSound = IsDlgButtonChecked( IDC_NOTIFYSND );
}

void CTabISRNotify::OnNotifyuttr() 
{
   gpEng->m_Not.bUtter = IsDlgButtonChecked( IDC_NOTIFYUTTR );
}

void CTabISRNotify::OnNotifyvmeter() 
{
   gpEng->m_Not.bVMeter = IsDlgButtonChecked( IDC_NOTIFYVMETER );
}

BOOL CTabISRNotify::OnInitDialog() 
{
   CPropertyPage::OnInitDialog();
   
   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsEng();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRGrammar message handlers

BOOL CTabISRGrammar::OnInitDialog() 
{
   CPropertyPage::OnInitDialog();
   
   m_BookMarkEdit.ReplaceSel( TEXT("0") );

   m_GramAutoPause.SetCurSel( 0 );
   m_GramWnd.SetCurSel( 0 );

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTabISRGrammar::OnSetActive()
{
   if( !CPropertyPage::OnSetActive() ) return FALSE;

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsGram();

   return TRUE;
}

void CTabISRGrammar::OnGramactivate() 
{
   HRESULT hRes;
   BOOL    fAutoPause = FALSE;
   HWND    hWndGram = NULL;
   TCHAR   buf[MAX_STRLEN], pausebuf[MAX_STRLEN], *prulebuf;
   PTCHAR  pRuleBuf;

   if( !gpEng->m_pGram )
   {
      MainOutput(TEXT("Error, grammar pointer invalid!\n"));
      return;
   }

   APIALLOC( pRuleBufAPI, MAX_STRLEN );

   // get the selected rule, autopause, and hwnd parameters

   // store the rule name...
   pRuleBuf = new TCHAR[MAX_STRLEN];
   m_GramRuleList.GetLBText( m_GramRuleList.GetCurSel(), pRuleBuf );

   APIA2SCVT( pRuleBufAPI, pRuleBuf );

   // allow the user to send an actual NULL param...
   if( _tcscmp( pRuleBuf, TEXT("NULL") ) == 0 )
      prulebuf = NULL;
   else
      prulebuf = (PTCHAR)pRuleBufAPI;

   if( m_GramAutoPause.GetCurSel() == 1 ) fAutoPause = TRUE;
   if( m_GramWnd.GetCurSel() == 1 ) hWndGram = ((CSRTestDlg *)AfxGetMainWnd())->m_hWnd;

   // activate the grammar using the user-specified rule and parameters...
   hRes = gpEng->m_pGram->m_pISRGramCommon->Activate( hWndGram, fAutoPause, (const APICAST*)prulebuf );
   if( FAILED(hRes) )
   {
      delete pRuleBuf;
      APIFREE( pRuleBufAPI );
      _stprintf( buf, TEXT("\nError activating grammar (%x).\n"), hRes );
   }
   else
   {
      // add rule name to list of rule names for this grammar...
      gpEng->m_pGram->m_ActiveRules.AddTail( pRuleBuf );
      m_GramAutoPause.GetLBText( m_GramAutoPause.GetCurSel(), pausebuf );
      _stprintf( buf, TEXT("Grammar %s activated, hwnd: %x, pause: %s, rule: %s, \n"),
          gpEng->m_pGram->m_szName, hWndGram, pausebuf, pRuleBuf );

      // update the active rule listbox...
      ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsGram();
   }

   MainOutput( buf );

   APIFREE( pRuleBufAPI );
}

void CTabISRGrammar::OnGramdeactivategram() 
{
   HRESULT  hRes;
   POSITION pos;
   TCHAR    buf[MAX_STRLEN], RuleBuf[MAX_STRLEN], *prulebuf;
   int      iCurSel;

   // get the active rule selection by the user...
   iCurSel = m_RuleActiveList.GetCurSel();
   m_RuleActiveList.GetText( iCurSel, RuleBuf );

   APIALLOC( pRuleBufAPI, MAX_STRLEN );
   APIA2SCVT( pRuleBufAPI, RuleBuf );

   // allow the user to send an actual NULL param...
   if( _tcscmp( RuleBuf, TEXT("NULL") ) == 0 )
      prulebuf = NULL;
   else
      prulebuf = (PTCHAR)pRuleBufAPI;

   hRes = gpEng->m_pGram->m_pISRGramCommon->Deactivate( (const APICAST*)prulebuf );
   if( FAILED(hRes) )
      _stprintf( buf, TEXT("\nError deactivating grammar. (%x)\n"), hRes );
   else
   {
      _stprintf( buf, TEXT("Grammar %s, rule %s deactivated\n"), gpEng->m_pGram->m_szName, RuleBuf );

      // remove/delete the rule from the listbox and Cptrlist...
      pos = gpEng->m_pGram->m_ActiveRules.FindIndex( iCurSel );
      prulebuf = (PTCHAR)gpEng->m_pGram->m_ActiveRules.GetAt( pos );
      gpEng->m_pGram->m_ActiveRules.RemoveAt( pos );
      delete prulebuf;

      // update the active rule listbox...
      ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsGram();
   }

   MainOutput( buf );

   APIFREE( pRuleBufAPI );
}

void CTabISRGrammar::OnGramarchive() 
{
   HRESULT hRes;
   TCHAR   buf[MAX_STRLEN];

   // get the size of the buffer needed for the archive call...
   hRes = gpEng->m_pGram->m_pISRGramCommon->Archive( FALSE, NULL, 0, &gpEng->m_pGram->m_Archive.dwSize );
   if( hRes != SRERR_NOTENOUGHDATA )
   {
      CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("Archive not Implemented") : TEXT("Error getting size required in Archive()") );
      return;
   }

   // if we allocate the memory ok, then make the archive call,
   // this memory is freed in the mrammar destructor...
   gpEng->m_pGram->m_Archive.pData = (LPSTR)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, gpEng->m_pGram->m_Archive.dwSize );
   if( gpEng->m_pGram->m_Archive.pData )
   {
      hRes = gpEng->m_pGram->m_pISRGramCommon->Archive( FALSE, gpEng->m_pGram->m_Archive.pData,
         gpEng->m_pGram->m_Archive.dwSize, &gpEng->m_pGram->m_Archive.dwSize );
      if( CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("Archive not Implemented") : TEXT("Error calling Archive()" ) ) ) return;

      wsprintf( buf, TEXT("Archived %lu bytes."), gpEng->m_pGram->m_Archive.dwSize );
   }
   else
   {
      wsprintf( buf, TEXT("Error allocating %lu bytes, archive aborted."), gpEng->m_pGram->m_Archive.dwSize );
      gpEng->m_pGram->m_Archive.dwSize = 0;
   }

   MainOutput( buf );
}

void CTabISRGrammar::OnGramdeteriorget() 
{
   HRESULT hRes;
   DWORD dwMem, dwTime, dwObj;
   TCHAR buf[MAX_STRLEN];

   hRes = gpEng->m_pGram->m_pISRGramCommon->DeteriorationGet( &dwMem, &dwTime, &dwObj );
   if( CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("DeteriorationGet not Implemented") : TEXT("Error calling DeteriorationGet()") ) ) return;

   _stprintf( buf, TEXT("DeteriorationGet successful, Mem: %u, Time: %u, Obj: %u"), dwMem, dwTime, dwObj );
   MainOutput( buf );
}

void CTabISRGrammar::OnGramdeteriorset() 
{
   HRESULT hRes;
   DWORD dwMem=0xffff, dwTime=60, dwObj=10;
   int error=0;

   hRes = gpEng->m_pGram->m_pISRGramCommon->DeteriorationSet( dwMem, dwTime, dwObj );
   if( CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("DeteriorationSet not Implemented") : TEXT("Error calling DesteriorationSet()") ) ) return;

   for( dwMem=0; dwMem<100; dwMem += 1000 )
   {
      for( dwTime=0; dwTime<1000; dwTime++ )
      {
         for( dwObj=0; dwObj<100; dwObj++ )
         {
            if( gpEng->m_pGram->m_pISRGramCommon->DeteriorationSet( dwMem, dwTime, dwObj ) ) error = 1;
         }
      }
   }
}

void CTabISRGrammar::OnGrambookmark() 
{
   TCHAR buf[MAX_STRLEN];
   QWORD qTime;
   DWORD dwID=1;

   m_BookMarkEdit.GetWindowText( buf, sizeof(buf) );
   _stscanf( buf, TEXT("%I64u"), &qTime );

   HRESULT hRes = gpEng->m_pGram->m_pISRGramCommon->BookMark( qTime, dwID );
   if( CheckError( hRes, TEXT("Error calling BookMark()") ) ) return;

   _stprintf( buf,TEXT("BookMark Set at %I64x, id=%lx\n"), qTime, dwID );
   MainOutput( buf );
}

void CTabISRGrammar::OnGramtrain() 
{
   HRESULT hRes;

   hRes = gpEng->m_pGram->m_pISRGramCommon->TrainDlg( m_hWnd, NULL );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("TrainDlg not Implemented") : TEXT("Error calling TrainDlg()") );
}

void CTabISRGrammar::OnGramtrainphrase() 
{
   SDATA Data;
   HRESULT hRes;
   TCHAR buf[MAX_STRLEN];

   hRes = gpEng->m_pGram->m_pISRGramCommon->TrainPhrase( SRTQEX_RECOMMENDED, &Data );
   if( FAILED(hRes) )
      _stprintf( buf, (hRes == E_NOTIMPL) ? TEXT("TrainPhrase (%x). (%x) not Implemented") : TEXT("Error calling TrainPhrase( %x ). (%x)\n"), SRTQEX_RECOMMENDED, hRes );
   else
   {
      _stprintf( buf, TEXT("TrainPhrase( %x ) success.\n"), SRTQEX_RECOMMENDED );
      CoTaskMemFree( Data.pData );
   }

   MainOutput( buf );

   hRes = gpEng->m_pGram->m_pISRGramCommon->TrainPhrase( SRTQEX_REQUIRED, &Data );
   if( FAILED(hRes) )
      _stprintf( buf, (hRes == E_NOTIMPL) ? TEXT("TrainPhrase (%x).(%x) not Implemented") : TEXT("Error calling TrainPhrase( %x ). (%x)\n"), SRTQEX_REQUIRED, hRes );
   else
   {
      _stprintf( buf, TEXT("TrainPhrase( %x ) success.\n"), SRTQEX_REQUIRED );
      CoTaskMemFree( Data.pData );
   }

   MainOutput( buf );
}

void CTabISRGrammar::OnGramtrainquery() 
{
   HRESULT hRes;
   DWORD   dwTrainFlag;
   TCHAR   buf[MAX_STRLEN];

   hRes = gpEng->m_pGram->m_pISRGramCommon->TrainQuery( &dwTrainFlag );
   if( FAILED(hRes) )
      _stprintf( buf, (hRes == E_NOTIMPL) ? TEXT("TrainQuery(%x) not Implemented") : TEXT("Error calling TrainQuery() (%x)\n"), hRes );
   else
      _stprintf( buf, TEXT("TrainQuery successful, result: %x\n"), dwTrainFlag );

   MainOutput( buf );
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRGramCFG message handlers

BOOL CTabISRGramCFG::OnInitDialog() 
{
   CPropertyPage::OnInitDialog();

   // give the user a default set of original list words that can be used to set a list...
   for( int i=0; i<NGRAMORIGWORDS; i++ ) m_GramOrigWords.AddString( szGramOrigWords[i] );

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTabISRGramCFG::OnSetActive()
{
   if( !CPropertyPage::OnSetActive() ) return FALSE;

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsGram();

   // clear the added words list box...
   m_GramCFGWords.ResetContent();

   if( gpEng->m_pGram && m_GramListName.GetWindowText( gpEng->m_pGram->m_Cfg.szListName, MAX_STRLEN ) )
      DisplayGramList( gpEng->m_pGram->m_Cfg.szListName );

   m_GramListQuery.SetFocus();      

   return TRUE;
}

void CTabISRGramCFG::OnCloseupGramlistname() 
{
   gpEng->m_pGram->m_szListNameIndex = m_GramListName.GetCurSel();
}

void CTabISRGramCFG::OnGramlinkquery() 
{
   HRESULT hRes;
   TCHAR *p;

   if( m_GramLinkName.GetWindowText( gpEng->m_pGram->m_Cfg.szLinkName, MAX_STRLEN ) == 0 )
   {
      MessageBox( TEXT("You must first specify a link name."), NULL, MB_OK );
      return;
   }

   APIALLOC( speechbuf, MAX_STRLEN );
   APIA2SCVT( speechbuf, gpEng->m_pGram->m_Cfg.szLinkName );

   hRes = gpEng->m_pGram->m_pISRGramCFG->LinkQuery( speechbuf, &gpEng->m_pGram->m_Cfg.bLinkQuery );
   APIFREE( speechbuf );

   if( CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("Link Query not Implemented") : TEXT("Error calling LinkQuery()") ) ) return;

   p = gpEng->m_pGram->m_Cfg.bLinkQuery ? TEXT("Contains Link\n") : TEXT("Link not found\n");
   MainOutput( p );
}

void CTabISRGramCFG::OnGramlistquery() 
{
   if( m_GramListName.GetWindowText( gpEng->m_pGram->m_Cfg.szListName, MAX_STRLEN ) == 0 )
   {
      MessageBox( TEXT("You must first specify a list name."), NULL, MB_OK );
      return;
   }

   DisplayGramList( gpEng->m_pGram->m_Cfg.szListName );
}

void CTabISRGramCFG::DisplayGramList( PTCHAR szListName )
{
   HRESULT hRes;

   if( *szListName == NULL ) return;

   // clear the added words list box...
   m_GramCFGWords.ResetContent();

   APIALLOC( speechbuf, MAX_STRLEN );
   APIA2SCVT( speechbuf, szListName );
   hRes = gpEng->m_pGram->m_pISRGramCFG->ListQuery( speechbuf, &gpEng->m_pGram->m_Cfg.bListQuery );
   APIFREE( speechbuf );

   if( CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("ListQuery not Implemented") : TEXT("Error calling ListQuery()") ) ) return;

   if( gpEng->m_pGram->m_Cfg.bListQuery )
      OnGramcfglistget();
   else
      MainOutput( TEXT("List has no words set.\n") );
}

void CTabISRGramCFG::OnGramcustom() 
{
   CGramAddCustom DlgAddCust;

   if( DlgAddCust.DoModal() == IDOK )
   {
      m_GramOrigWords.AddString( DlgAddCust.m_GramAddEdit );
   }
}

void CTabISRGramCFG::OnGramcfglistget() 
{
   HRESULT  hRes;
   SDATA    Data;
   DWORD    cnt=0, nline=0;
   TCHAR    buf[MAX_STRLEN];
   PSRWORD  pSRWord;


   if( m_GramListName.GetLBText( gpEng->m_pGram->m_szListNameIndex, gpEng->m_pGram->m_Cfg.szListName ) == 0 )
   {
      MessageBox( TEXT("Grammar contains no lists."), NULL, MB_OK );
      return;
   }

   APIALLOC( speechbuf, MAX_STRLEN );
   APIA2SCVT( speechbuf, gpEng->m_pGram->m_Cfg.szListName );

   hRes = gpEng->m_pGram->m_pISRGramCFG->ListGet( speechbuf, &Data );
   APIFREE( speechbuf);
   if( CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("ListGet not Implemented") : TEXT("Error calling ListGet()") ) )
   {
      CoTaskMemFree( Data.pData );
      return;
   }

   // clear, then display the grammar list if a name has been entered...
   m_GramCFGWords.ResetContent();

   // copy the new list to the list control...
   pSRWord = (PSRWORD)Data.pData;
   while( cnt < Data.dwSize )
   {
      // convert a line from the struct...
      S2ACVTEX( buf, pSRWord->szWord, MAX_STRLEN );

      // copy to list box...
      m_GramCFGWords.InsertString( nline++, buf );

      cnt += pSRWord->dwSize;

      // next SRWORD struct...
      pSRWord = (PSRWORD) ( (char *)Data.pData + cnt );
   }

   // free memory...
   CoTaskMemFree( Data.pData );
}

void CTabISRGramCFG::OnGramcfglistset() 
{
   HRESULT  hRes;
   SDATA    Data;
   int      size, i;
   DWORD    *pdw;
   TCHAR    buf[MAX_STRLEN];

   if( m_GramListName.GetLBText( gpEng->m_pGram->m_szListNameIndex, gpEng->m_pGram->m_Cfg.szListName ) == 0 )
   {
      MessageBox( TEXT("Grammar contains no lists."), NULL, MB_OK );
      return;
   }

   // setup the grammar list

   // get the starting/ending characters selected by the user in the edit control...
   if( m_GramOrigWords.GetSelCount() == 0 )
   {
      MessageBox( TEXT("You must first select the words you want to set in the listbox to the right."), NULL, MB_OK );
      return;
   }

   // build the SRWORD structs from the selected text.

   Data.dwSize = 0;
   if( !(Data.pData  = (char *)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 1 )) )
   {
      MainOutput( TEXT("Error in HeapAlloc(), ListSet aborted.") );
      return;
   }

   // loop through each line...
   for( i=0; i<m_GramOrigWords.GetSelItems( NSELBUF, iSelBuf ); i++ )
   {
      // get the next line from the listbox and compute the SRWORD size,
      // note, convert to the speech data type...
      m_GramOrigWords.GetText( iSelBuf[i], buf );
      size = sizeof(SRWORD) + sizeof(APICAST)*(_tcslen(buf) + 1);
      if( size%4 ) size += (4 - size%4);

      // allocate memory and build the new structure entry,
      if( !(Data.pData = (char *)HeapReAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, Data.pData, Data.dwSize+size )) )
      {
         MainOutput( TEXT("Error in HeapReAlloc(), ListSet aborted.") );
         return;
      }

      // point past the last structure...
      pdw  = (DWORD *) ( (char *)Data.pData + Data.dwSize );
      *pdw++ = size;            // SRWORD.dwSize
      *pdw++ = dwListWordID++;   // SRWORD.dwWordNum
      A2SCVTEX( (APICAST *)pdw, buf, _tcslen(buf)+1 );

      // accumulate total...
      Data.dwSize += size;
   }

   // make SR api call...
   APIALLOC( speechbuf, MAX_STRLEN );
   APIA2SCVT( speechbuf, gpEng->m_pGram->m_Cfg.szListName );
   hRes = gpEng->m_pGram->m_pISRGramCFG->ListSet( speechbuf, Data );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("ListSet not Implemented") : TEXT("Error calling ListSet()") );

   // free memory...
   APIFREE( speechbuf );
   HeapFree( GetProcessHeap(), NULL, Data.pData );

   // now go get the list and copy it to the list box...
   OnGramcfglistget();

   // deselect the original words...
   m_GramOrigWords.SetSel( -1, FALSE );
}

void CTabISRGramCFG::OnGramcfglistappend() 
{
   HRESULT  hRes;
   SDATA    Data;
   int      size, i;
   DWORD    *pdw;
   TCHAR    buf[MAX_STRLEN];

   if( m_GramListName.GetLBText( gpEng->m_pGram->m_szListNameIndex, gpEng->m_pGram->m_Cfg.szListName ) == 0 )
   {
      MessageBox( TEXT("Grammar contains no lists."), NULL, MB_OK );
      return;
   }

   // setup the grammar list

   // get the lines selected by the user in the edit control...
   if( m_GramOrigWords.GetSelCount() == 0 )
   {
      MessageBox( TEXT("You must first select the words you want to append in the edit control to the right."), NULL, MB_OK );
      return;
   }

   // build the SRWORD structs from the selected text.

   Data.dwSize = 0;
   if( !(Data.pData  = (char *)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 1 )) )
   {
      MainOutput( TEXT("Error in HeapAlloc(), ListAppend aborted.") );
      return;
   }

   // loop through each line...
   for( i=0; i < m_GramOrigWords.GetSelItems( NSELBUF, iSelBuf ); i++ )
   {
      // get the next line from the edit control and compute the SRWORD size,
      // make sure we DWORD align...
      // note, convert to the speech data type...
      m_GramOrigWords.GetText( iSelBuf[i], buf );
      size = sizeof(SRWORD) + sizeof(APICAST)*(_tcslen(buf) + 1);
      if( size%4 ) size += (4 - size%4);

      // allocate memory and build the new structure entry,
      if( !(Data.pData = (char *)HeapReAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, Data.pData, Data.dwSize+size )) )
      {
         MainOutput( TEXT("Error in HeapReAlloc(), ListAppend aborted.") );
         return;
      }

      // point past the last structure...
      pdw  = (DWORD *) ( (char *)Data.pData + Data.dwSize );
      *pdw++ = size;            // SRWORD.dwSize
      *pdw++ = dwListWordID++;   // SRWORD.dwWordNum
      A2SCVTEX( (APICAST *)pdw, buf, _tcslen(buf)+1 );

      // accumulate total...
      Data.dwSize += size;
   }

   // make SR api call...
   APIALLOC( speechbuf, MAX_STRLEN );
   APIA2SCVT( speechbuf, gpEng->m_pGram->m_Cfg.szListName );
   hRes = gpEng->m_pGram->m_pISRGramCFG->ListAppend( speechbuf, Data );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("ListAppend not Implemented") : TEXT("Error calling ListAppend()") );

   // free memory...
   APIFREE( speechbuf );
   HeapFree( GetProcessHeap(), NULL, Data.pData );

   // now go get the list and copy it to the list box...
   OnGramcfglistget();

   // deselect the original words...
   m_GramOrigWords.SetSel( -1, FALSE );
}

void CTabISRGramCFG::OnGramcfglistremove() 
{
   HRESULT  hRes;
   SDATA    Data;
   int      size, i;
   DWORD    *pdw;
   TCHAR    buf[MAX_STRLEN];

   if( m_GramListName.GetLBText( gpEng->m_pGram->m_szListNameIndex, gpEng->m_pGram->m_Cfg.szListName ) == 0 )
   {
      MessageBox( TEXT("You must first specify a list name."), NULL, MB_OK );
      return;
   }

   // build the SRWORD structs from the selected text.

   // get the lines selected by the user in the listbox...
   if( m_GramCFGWords.GetSelCount() == 0 )
   {
      MessageBox( TEXT("You must first select the words you want to append in the listbox to the left."), NULL, MB_OK );
      return;
   }

   Data.dwSize = 0;
   if( !(Data.pData  = (char *)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 1 )) )
   {
      MainOutput( TEXT("Error in HeapAlloc(), ListRemove aborted.") );
      return;
   }

   // loop through each line...

   for( i=0; i < m_GramCFGWords.GetSelItems( NSELBUF, iSelBuf ); i++ )
   {
      // get the next selected line from the list box and compute the SRWORD size,
      // note, convert to the speech data type...
      m_GramCFGWords.GetText( iSelBuf[i], buf );
      size = sizeof(SRWORD) + sizeof(APICAST)*(_tcslen(buf) + 1);
      if( size%4 ) size += (4 - size%4);

      // allocate memory and build the new structure entry,
      if( !(Data.pData = (char *)HeapReAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, Data.pData, Data.dwSize+size )) )
      {
         MainOutput( TEXT("Error in HeapReAlloc(), ListRemove aborted.") );
         return;
      }

      // point past the last structure...
      pdw  = (DWORD *) ( (char *)Data.pData + Data.dwSize );
      *pdw++ = size;   // SRWORD.dwSize
      *pdw++ = 0;      // SRWORD.dwWordNum (ListRemove doesn't look at this)
      A2SCVTEX( (APICAST *)pdw, buf, _tcslen(buf)+1 );

      // accumulate total...
      Data.dwSize += size;
   }

   // make SR api call...
   APIALLOC( speechbuf, MAX_STRLEN );
   APIA2SCVT( speechbuf, gpEng->m_pGram->m_Cfg.szListName );

   hRes = gpEng->m_pGram->m_pISRGramCFG->ListRemove( speechbuf, Data );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("ListRemove not Implemented") : TEXT("Error calling ListRemove()") );

   // free memory...
   APIFREE( speechbuf );
   HeapFree( GetProcessHeap(), NULL, Data.pData );

   // deselect the original words...
   m_GramCFGWords.SetSel( -1, FALSE );

   // now go get update the list box...
   OnGramcfglistget();
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRGramDict message handlers

BOOL CTabISRGramDict::OnInitDialog() 
{
   CPropertyPage::OnInitDialog();

   // if the pointer is ok, then init the controls
   // otherwise disable the controls...
   if( gpEng->m_pGram && gpEng->m_pGram->m_pISRGramDict )
      ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsGram();
   else
   {
      m_GramLikelyWords.EnableWindow( FALSE );
      m_GramDictHint.EnableWindow( FALSE );
      m_GramBeforeEdit.EnableWindow( FALSE );
      m_GramAfterEdit.EnableWindow( FALSE );
   }
   
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTabISRGramDict::OnSetActive()
{
   if( !CPropertyPage::OnSetActive() ) return FALSE;

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsGram();

   return TRUE;
}

void CTabISRGramDict::OnGramdictwords() 
{
   MessageBox( TEXT("The GramDictation test code is not currently implemented."), NULL, MB_OK );
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRGramNotify message handlers

BOOL CTabISRGramNotify::OnSetActive()
{
   if( !CPropertyPage::OnSetActive() ) return FALSE;

   ((CSRTestDlg *)AfxGetMainWnd())->InitPropPageSettingsGram();

   return TRUE;
}

void CTabISRGramNotify::OnGramnotbookmark() 
{
   gpEng->m_pGram->m_Not.bBookmark = m_Bookmark.GetCheck();
}

void CTabISRGramNotify::OnGramnotpaused() 
{
   gpEng->m_pGram->m_Not.bPaused = m_Paused.GetCheck();
}

void CTabISRGramNotify::OnGramnotphrase() 
{
   gpEng->m_pGram->m_Not.bPhrase = m_Phrase.GetCheck();
}

void CTabISRGramNotify::OnGramnotphrashyp() 
{
   gpEng->m_pGram->m_Not.bPhraseHyp = m_PhraseHyp.GetCheck();
}

void CTabISRGramNotify::OnGramnotreeval() 
{
   gpEng->m_pGram->m_Not.bReEval = m_ReEval.GetCheck();
}

void CTabISRGramNotify::OnGramnotunarc() 
{
   gpEng->m_pGram->m_Not.bUnarchive = m_Unarchive.GetCheck();
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRResAud message handlers

void CTabISRResAud::OnAudiogetwave() 
{
   PISRRESAUDIO pISRResAudio;
   TCHAR        buf[MAX_STRLEN];
   SDATA        sData;
   HRESULT hRes;

   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
   return;

   hRes = gpResults->QueryInterface(IID_ISRResAudio, (void**)&pISRResAudio);
   if( CheckError( hRes, TEXT("ISRResAudio not supported.") )) return;

   hRes = pISRResAudio->GetWAV( &sData );
   if( !CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("GetWAV not Implemented") : TEXT("Error in GetWAV.") ))
   {
      _stprintf( buf, TEXT("Received data, size=%ld\n"), sData.dwSize );
      m_AudioFmtTxt.SetWindowText( buf );
      CoTaskMemFree( sData.pData );
   }
   
   pISRResAudio->Release();
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRResBasic message handlers

BOOL CTabISRResBasic::OnSetActive()
{
   if (!CPropertyPage::OnSetActive()) 
      return (FALSE);

   if (m_ResSelection.GetCount() > 0) {
      if (giResSel == -1) {
         m_ResSelection.SetCurSel(giResSel=0);
         gpResults = (LPUNKNOWN) m_ResSelection.GetItemData(giResSel);
         m_ResSelection.GetText(giResSel, gszResultsTxt);
      }
   }

   return (TRUE);
}

void CTabISRResBasic::OnKillfocusResobjselect() 
{
   giResSel = m_ResSelection.GetCurSel();
   gpResults = (LPUNKNOWN) m_ResSelection.GetItemData(giResSel);
   m_ResSelection.GetText(giResSel, gszResultsTxt);
}

void CTabISRResBasic::OnSelchangeResobjselect() 
{
}

void CTabISRResBasic::OnFlagsget() 
{
   PISRRESBASIC pISRResBasic;
   DWORD        dwFlags;
   HRESULT      hRes;

   if (IsBadReadPtr(gpResults, sizeof(gpResults)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResBasic, (PVOID *) &pISRResBasic);
   if (CheckError(hRes, TEXT("ISRResBasic not supported."))) 
      return;

   hRes = pISRResBasic->FlagsGet(0, &dwFlags);
   if (!CheckError(hRes, (hRes == E_NOTIMPL) ? TEXT("FlagsGet not Implemented") : TEXT("Error in FlagsGet()."))) {
      TCHAR buf[MAX_STRLEN];

      _stprintf(buf, TEXT("%x"), dwFlags);
      m_FlagsGetTxt.SetWindowText(buf);
   }
   
   pISRResBasic->Release();
}

void CTabISRResBasic::OnResidentify() 
{
   PISRRESBASIC pISRResBasic;
   GUID         guid;
   HRESULT      hRes;
      
   if (IsBadReadPtr(gpResults, sizeof(gpResults)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResBasic, (PVOID *) &pISRResBasic);
   if (CheckError(hRes, TEXT("ISRResBasic not supported.")))
      return;

   hRes = pISRResBasic->Identify(&guid);
   if (!CheckError(hRes, (hRes == E_NOTIMPL) ? TEXT("Identify not Implemented") : TEXT("Error in Identify()."))) {
      OLECHAR ResultObjectGUIDStr[GUIDSTRBUFSIZE];

      // Get a textual representation of the GUID and display it
      StringFromGUID2(guid, ResultObjectGUIDStr, GUIDSTRBUFSIZE);
#ifdef _UNICODE
      m_ResIdentifyTxt.SetWindowText(ResultObjectGUIDStr);
#else
      CHAR buf[GUIDSTRBUFSIZE];

      WideCharToMultiByte(CP_ACP, 0, ResultObjectGUIDStr, -1, buf, GUIDSTRBUFSIZE, NULL, NULL);
      m_ResIdentifyTxt.SetWindowText(buf);
#endif /* _UNICODE */
   }

   // Release the interface pointer
   pISRResBasic->Release();
}

void CTabISRResBasic::OnRestimeget() 
{
   PISRRESBASIC pISRResBasic;
   QWORD        qwBegin, qwEnd;
   HRESULT      hRes;
   
   if (IsBadReadPtr(gpResults, sizeof(gpResults)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResBasic, (PVOID *) &pISRResBasic);
   if (CheckError(hRes, TEXT("ISRResBasic not supported."))) 
      return;

   hRes = pISRResBasic->TimeGet( &qwBegin, &qwEnd );
   if (!CheckError(hRes, (hRes == E_NOTIMPL) ? TEXT("TimeGet not Implemented") : TEXT("Error in TimeGet()."))) {
      TCHAR  buf[MAX_STRLEN];

      _stprintf(buf, TEXT("Begin: %I64x"), qwBegin);
      m_TimeBeginTxt.SetWindowText(buf);
      _stprintf(buf, TEXT("End: %I64x"), qwEnd);
      m_TimeEndTxt.SetWindowText(buf);
   }
   
   pISRResBasic->Release();
}

void CTabISRResBasic::OnPhraseget() 
{
   PISRRESBASIC pISRResBasic;
   DWORD        dwSizeNeeded;
   PSRPHRASE    pSRPhrase;
   TCHAR        buf[MAX_STRLEN], bufA[MAX_STRLEN];
   HRESULT      hRes;
   DWORD        dwRank = 1;

   if (IsBadReadPtr(gpResults, sizeof(gpResults)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResBasic, (PVOID *) &pISRResBasic);
   if (CheckError(hRes, TEXT("ISRResBasic not supported")))
      return;
   for (;;) {
      // Determine how large of a buffer we need
      hRes = pISRResBasic->PhraseGet(dwRank, NULL, 0, &dwSizeNeeded);
      if (hRes == SRERR_VALUEOUTOFRANGE) {
         // Rank ID too high
         pISRResBasic->Release();
         if (dwRank == 1)
            m_PhraseGetTxt.SetWindowText(TEXT("PhraseGet:  No alternatives"));
         return;
      }
      if ((hRes != SRERR_NOTENOUGHDATA) &&
        ((hRes != S_OK) || (dwSizeNeeded != 0))) {
         CheckError(hRes, TEXT("Error in PhraseGet()."));
         pISRResBasic->Release();
         return;
      }

      // now allocate space and get the data...
      pSRPhrase = (PSRPHRASE) malloc(dwSizeNeeded);
      pSRPhrase->dwSize = dwSizeNeeded;
      hRes = pISRResBasic->PhraseGet(dwRank, (PSRPHRASE) pSRPhrase, dwSizeNeeded, &dwSizeNeeded);
      if (CheckError(hRes, TEXT("Error in PhraseGet()."))) {
         pISRResBasic->Release();
         free(pSRPhrase);
         return;
      }

      Phrase2Str(pSRPhrase, (char *) bufA, (APICHARSIZE == 2));   
      S2ACVTEX(buf, (APICAST *) bufA, MAX_STRLEN);
      m_PhraseGetTxt.SetWindowText(buf);
      free(pSRPhrase);
      ++dwRank;
   }
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRResCorr message handlers

BOOL CTabISRResCorr::OnInitDialog() 
{
   CPropertyPage::OnInitDialog();

   m_CorrConfidList.SetCurSel(1);
   m_CorrValidateList.SetCurSel(1);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
} /* End of CTabISRResCorr::OnInitDialog() */

BOOL CTabISRResCorr::OnSetActive()
{
   if (!CPropertyPage::OnSetActive()) 
      return FALSE;

   TCHAR buf[MAX_STRLEN];

   ((CSRTestDlg *) AfxGetMainWnd())->m_ResBasic.m_ResSelection.GetText(giResSel, buf);
   m_CorrPhraseEdit.SetWindowText(buf);

   return (TRUE);
} /* End of CTabISRResCorr::OnSetActive() */

void CTabISRResCorr::OnCorrcorr() 
{
   // Get ISRResCorrection interface pointer
   PISRRESCORRECTION pISRResCorrection;
   HRESULT hResult;
      
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;

   hResult = gpResults->QueryInterface(IID_ISRResCorrection, (PVOID *) &pISRResCorrection);
   if (CheckError(hResult, TEXT("ISRResCorrection not supported.")))
      return;

   // Determine confidence setting
   WORD wConfidence;
   wConfidence = m_CorrConfidList.GetCurSel() == 1 ? SRCORCONFIDENCE_VERY : SRCORCONFIDENCE_SOME;

   // Get text of corrected word
   TCHAR buf[MAX_STRLEN];
   m_CorrPhraseEdit.GetWindowText(buf, MAX_STRLEN);

   // Convert corrected word text into correct character set
   APIALLOC(speechbuf, MAX_STRLEN);
   APIA2SCVT(speechbuf, buf);

   // Create an SRPhrase structure from the corrected word
   PSRPHRASE pSRPhrase;
   Str2Phrase((TCHAR*) speechbuf, &pSRPhrase);

   // Free character set conversion buffer
   APIFREE(speechbuf);

   // Tell engine about correction
   hResult = pISRResCorrection->Correction(pSRPhrase, wConfidence);
   CheckError(hResult, (hResult == E_NOTIMPL) ? TEXT("Correction not Implemented") : TEXT("Error in Correction()."));

   // Free SRPhrase structure
    free(pSRPhrase);

   // Release the ISRResCorrection interface
   pISRResCorrection->Release();
} /* End of CTabISRResCorr::OnCorrcorr() */

void CTabISRResCorr::OnCorrvalid() 
{
   // Get ISRResCorrection interface pointer
   PISRRESCORRECTION pISRResCorrection;
   HRESULT hResult;
      
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;

   hResult = gpResults->QueryInterface(IID_ISRResCorrection, (PVOID *) &pISRResCorrection);
   if (CheckError(hResult, TEXT("ISRResCorrection not supported.")))
      return;

   // Determine confidence setting
   WORD wConfidence;
   wConfidence = m_CorrConfidList.GetCurSel() == 1 ? SRCORCONFIDENCE_VERY : SRCORCONFIDENCE_SOME;

   // Tell the engine its doing good work!
   hResult = pISRResCorrection->Validate(wConfidence);
   CheckError(hResult, TEXT("Error in Validate()."));
   
   // Release the ISRResCorrection interface
   pISRResCorrection->Release();
} /* End of CTabISRResCorr::OnCorrvalid() */

/////////////////////////////////////////////////////////////////////////////
// CTabISRResEval message handlers

void CTabISRResEval::OnReevaluate() 
{
   PISRRESEVAL pISRResEval;
   BOOL        fChanged;
   HRESULT hRes;

   // Validate the gpResults value;
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResEval, (void**)&pISRResEval);
   if( CheckError( hRes, TEXT("ISRResEvaluate not supported.") )) return;

   hRes = pISRResEval->ReEvaluate( &fChanged );
   if( !CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("ReEvalutate not Implemented") : TEXT("Error in ReEvaluate().")) )
   {
      if( fChanged ) m_ReEvalTxt.SetWindowText( TEXT("pfChanged = TRUE") );   
      else m_ReEvalTxt.SetWindowText( TEXT("pfChanged = FALSE") );   
   }

   pISRResEval->Release();
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRResGraph message handlers

void CTabISRResGraph::OnBestpathph() 
{
   PISRRESGRAPH pISRResGraph;
   DWORD        dwBytesNeeded;
   DWORD        *padwPath;
   TCHAR        buf[MAX_STRLEN];
   HRESULT hRes;

   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResGraph, (void**)&pISRResGraph);
   if( CheckError( hRes, TEXT("ISRResGraph not supported.") )) return;

   // get the size required and allocate it...
   hRes = pISRResGraph->BestPathPhoneme( 0, NULL, 0, &dwBytesNeeded );
   if( CheckError( hRes, TEXT("Error in BestPathPhoneme(1).")) ) return;

   if( (padwPath = (DWORD *)malloc( dwBytesNeeded )) == NULL )
   {
      wsprintf( buf, TEXT("Out of memory for DWORD array in BestPathPhoneme.\n") );
      MainOutput( buf );
      return;
   }
   hRes = pISRResGraph->BestPathPhoneme( 0, padwPath, dwBytesNeeded, NULL );
   if( CheckError( hRes, TEXT("Error in BestPathPhoneme(2).")) )
   {
      free( padwPath );
      return;
   }
   m_GraphOutput.SetSel( 0, -1, TRUE );
   m_GraphOutput.Clear();
   for( DWORD i=0; i<dwBytesNeeded/sizeof(DWORD); i++ )
   {
      wsprintf( buf, TEXT("Phoneme: %ld\r\n"), padwPath[i] );
      m_GraphOutput.ReplaceSel( buf );
   }

   free( padwPath );

   pISRResGraph->Release();
}

void CTabISRResGraph::OnBestpathw() 
{
   PISRRESGRAPH pISRResGraph;
   DWORD        dwBytesNeeded;
   DWORD        *padwPath;
   TCHAR        buf[MAX_STRLEN];
   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResGraph, (void**) &pISRResGraph);
   if (CheckError(hRes, TEXT("ISRResGraph not supported."))) 
      return;

   // Determine how large of a buffer we need...
   hRes = pISRResGraph->BestPathWord(0, NULL, 0, &dwBytesNeeded);
   if (hRes != SRERR_NOTENOUGHDATA) {
      if (CheckError(hRes, TEXT("Error in BestPathWord(1)."))) 
         return;
   }

   // Allocate a buffer of the required size...
   if ((padwPath = (DWORD *) malloc(dwBytesNeeded)) == NULL) {
      wsprintf(buf, TEXT("Out of memory for DWORD array in BestPathWord.\n"));
      MainOutput(buf);
      return;
   }
   
   // And call BestPathWord to get the best path
   hRes = pISRResGraph->BestPathWord(0, padwPath, dwBytesNeeded, &dwBytesNeeded);
   if (CheckError(hRes, TEXT("Error in BestPathWord(2)."))) {
      free(padwPath);
      return;
   }
   m_GraphOutput.SetSel(0, -1, TRUE);
   m_GraphOutput.Clear();
   m_GraphOutput.ReplaceSel(TEXT("BestPathWord: "));
   DWORD i;
   DWORD dwNumNodes = dwBytesNeeded / sizeof(DWORD);
   for (i = 0; i < dwNumNodes; i++ ) {
      if (i == dwNumNodes - 1)
         wsprintf(buf, TEXT("%d\r\n"), padwPath[i]);
      else
         wsprintf(buf, TEXT("%d, "), padwPath[i]);
      m_GraphOutput.ReplaceSel(buf);
   }

   free(padwPath);

   pISRResGraph->Release();
}

void CTabISRResGraph::OnGetphnode() 
{
   TCHAR            buf[MAX_STRLEN];
   SRRESPHONEMENODE Node;
   PISRRESGRAPH     pISRResGraph;

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResGraph, (void**)&pISRResGraph);
   if( CheckError( hRes, TEXT("ISRResGraph not supported.") )) return;

   SCHAR cEngine;
   WCHAR cIPA;
   pISRResGraph->GetPhonemeNode( 1, &Node, &cIPA, (APICAST *)&cEngine );
   if( !CheckError( hRes, TEXT("Error in GetPhonemeNode.") ) )
   {
      wsprintf( buf, TEXT("cIPA: %d, cEngine: %d"), cIPA, cEngine );
      m_GraphOutput.SetWindowText( buf );
   }

   pISRResGraph->Release();
   return;
}

void CTabISRResGraph::OnGetwnode() 
{
   SRRESWORDNODE Node;
   DWORD         dwBytesNeeded;
   PISRRESGRAPH  pISRResGraph;
   PSRWORD       pSRWord;
   TCHAR         buf[MAX_STRLEN];
   TCHAR         WordBuf[MAX_STRLEN];

   HRESULT hRes;
   
   // Validate the Results Object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;

   // Get the ISRResGraph interface pointer
   hRes = gpResults->QueryInterface(IID_ISRResGraph, (void**) &pISRResGraph);
   if (CheckError(hRes, TEXT("ISRResGraph not supported.\r\n"))) 
      return;

   // Prompt the user for the word node to fetch

   // Determine how many bytes are needed to store the SRWORD structure
   hRes = pISRResGraph->GetWordNode(1, &Node, NULL, 0, &dwBytesNeeded);
   if (CheckError(hRes, TEXT("Error in GetWordNode\r\n."))) 
      return;

   // Allocate the required memory
   pSRWord   = (PSRWORD) malloc(dwBytesNeeded);

   // Now fetch the node and SRWORD structure
   pISRResGraph->GetWordNode(1, &Node, pSRWord, dwBytesNeeded, &dwBytesNeeded);
   if (!CheckError(hRes, TEXT("Error in GetPhonemeNode.\r\n"))) {
      // Clear the display window
      m_GraphOutput.SetSel(0, -1, TRUE);
      m_GraphOutput.Clear();

      // Display the SRWORD structure
      Word2Str(pSRWord, WordBuf);
      _stprintf(buf, TEXT("SRWORD:  %x, %x, \"%s\"\r\n"), pSRWord->dwSize, pSRWord->dwWordNum, WordBuf);
      m_GraphOutput.ReplaceSel(buf);

      // Display the SRRESWORDNODE structure
      _stprintf(buf, TEXT("dwNextWordNode:  %d\r\n"), Node.dwNextWordNode);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("dwUpAlternateWordNode:  %d\r\n"), Node.dwUpAlternateWordNode);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("dwDownAlternateWordNode:  %d\r\n"), Node.dwDownAlternateWordNode);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("dwPreviousWordNode:  %d\r\n"), Node.dwPreviousWordNode);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("dwPhonemeNode:  %d\r\n"), Node.dwPhonemeNode);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("qwStartTime:  %I64x\r\n"), Node.qwStartTime);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("qwEndTime:  %I64x\r\n"), Node.qwEndTime);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("dwWordScore:  %x\r\n"), Node.dwWordScore);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("wVolume:  %x\r\n"), Node.wVolume);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("wPitch:  %x\r\n"), Node.wPitch);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("pos:  %x\r\n"), Node.pos);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("dwCFGParse:  %x\r\n"), Node.dwCFGParse);
      m_GraphOutput.ReplaceSel(buf);
      _stprintf(buf, TEXT("dwCue:  %x\r\n"), Node.dwCue);
      m_GraphOutput.ReplaceSel(buf);
   }

   free(pSRWord);
   pISRResGraph->Release();
}

void CTabISRResGraph::OnPathscoreph() 
{
   const int nNodes=5;
   DWORD aNodes[nNodes];
   LONG  lScore;
   TCHAR buf[MAX_STRLEN];
   PISRRESGRAPH pISRResGraph;

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   // Get the ISRResGraph interface pointer
   hRes = gpResults->QueryInterface(IID_ISRResGraph, (void **) &pISRResGraph);
   if (CheckError(hRes, TEXT("ISRResGraph not supported."))) 
      return;

   // prompt the user to enter a path
   for (int i = 0; i < nNodes; i++ ) 
      aNodes[i] = i + 1;

   // Determine the path's score and display it
   hRes = pISRResGraph->PathScorePhoneme(aNodes, nNodes, &lScore);
   if (!CheckError(hRes, TEXT("Error in PathScorePhoneme."))) {
      wsprintf(buf, TEXT("Score = %ld\n"), lScore);
      m_GraphOutput.SetWindowText(buf);
   }
   pISRResGraph->Release();
}

void CTabISRResGraph::OnPathscorew() 
{
   const int nNodes=5;
   DWORD aNodes[nNodes];
   LONG  lScore;
   TCHAR buf[MAX_STRLEN];
   PISRRESGRAPH pISRResGraph;

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;

   // Get the ISRResGraph interface pointer
   hRes = gpResults->QueryInterface(IID_ISRResGraph, (void **) &pISRResGraph);
   if (CheckError(hRes, TEXT("ISRResGraph not supported."))) 
      return;

   //  Prompt the user to enter a path
   for (int i = 0; i < nNodes; i++) 
      aNodes[i] = i + 1;

   // Determine the path's score and display it
   hRes = pISRResGraph->PathScoreWord(aNodes, nNodes, &lScore);
   if (!CheckError(hRes, TEXT("Error in PathScoreWord."))) {
      wsprintf(buf, TEXT("Score = %ld\n"), lScore);
      m_GraphOutput.SetWindowText(buf);
   }

   pISRResGraph->Release();
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRResMem message handlers

void CTabISRResMem::OnMemget() 
{
   // Abort if the user hasn't selected a results object yet
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;

   // Get the ISRResMemory Interface pointer
   HRESULT hResult;
   PISRRESMEMORY pISRResMemory;

   hResult = gpResults->QueryInterface(IID_ISRResMemory, (PVOID *) &pISRResMemory);
   if (CheckError(hResult, TEXT("ISRResMemory not supported.")))
      return;
   
   // Determine the kind of information stored and how much memory
   // the Result Object consumes.
   DWORD dwKind;
   DWORD dwMemory;

   hResult = pISRResMemory->Get(&dwKind, &dwMemory);
   if (!CheckError(hResult, TEXT("Error in Get."))) {
      TCHAR buf[MAX_STRLEN];

      // Update all the kinds of information check boxes
      CheckDlgButton(IDC_SRRESAUDIO, dwKind & SRRESMEMKIND_AUDIO);
      CheckDlgButton(IDC_SRRESCORRECTION, dwKind & SRRESMEMKIND_CORRECTION);
      CheckDlgButton(IDC_SRRESEVAL, dwKind & SRRESMEMKIND_EVAL);
      CheckDlgButton(IDC_SRRESPHONEMEGRAPH, dwKind & SRRESMEMKIND_PHONEMEGRAPH);
      CheckDlgButton(IDC_SRRESWORDGRAPH, dwKind & SRRESMEMKIND_WORDGRAPH);

      // Display how many bytes are used
      _stprintf(buf, TEXT("%#lx bytes"), dwMemory);
      m_MemGetTxt.SetWindowText(buf);
   }

   // Release the ISRResMemory interface pointer
   pISRResMemory->Release();
} /* End of CTabISRResMem::OnMemget() */

void CTabISRResMem::OnMemfree() 
{
   // Abort if the user hasn't selected a results object yet
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;

   // Get the ISRResMemory Interface pointer
   HRESULT hResult;
   PISRRESMEMORY pISRResMemory;

   hResult = gpResults->QueryInterface(IID_ISRResMemory, (PVOID *) &pISRResMemory);
   if (CheckError(hResult, TEXT("ISRResMemory not supported.")))
      return;

   // Determine the kinds of memory the user wants freed
   DWORD dwKind = 0;

   dwKind |= IsDlgButtonChecked(IDC_SRRESAUDIO) ? SRRESMEMKIND_AUDIO : 0;
   dwKind |= IsDlgButtonChecked(IDC_SRRESCORRECTION) ? SRRESMEMKIND_CORRECTION : 0;
   dwKind |= IsDlgButtonChecked(IDC_SRRESEVAL) ? SRRESMEMKIND_EVAL : 0;
   dwKind |= IsDlgButtonChecked(IDC_SRRESPHONEMEGRAPH) ? SRRESMEMKIND_PHONEMEGRAPH : 0;
   dwKind |= IsDlgButtonChecked(IDC_SRRESWORDGRAPH) ? SRRESMEMKIND_WORDGRAPH : 0;
   
   // Free the requested types of memory
   hResult = pISRResMemory->Free(dwKind);
   CheckError(hResult, TEXT("Error in Get."));

   // Release the ISRResMemory interface pointer
   pISRResMemory->Release();
} /* End of CTabISRResMem::OnMemfree() */

void CTabISRResMem::OnLockget() 
{
   // Abort if the user hasn't selected a results object yet
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;

   // Get the ISRResMemory Interface pointer
   HRESULT hResult;
   PISRRESMEMORY pISRResMemory;

   hResult = gpResults->QueryInterface(IID_ISRResMemory, (PVOID *) &pISRResMemory);
   if (CheckError(hResult, TEXT("ISRResMemory not supported.")))
      return;

   // Get the Result Object's lock status
   BOOL fLock;

   hResult = pISRResMemory->LockGet(&fLock);
   if (!CheckError(hResult, TEXT("Error in Get."))) {
      CheckDlgButton(IDC_SRRESLOCKED, fLock);
   }

   // Release the ISRResMemory interface pointer
   pISRResMemory->Release();
} /* End of CTabISRResMem::OnLockget() */

void CTabISRResMem::OnLockset() 
{
   // Abort if the user hasn't selected a results object yet
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;

   // Get the ISRResMemory Interface pointer
   HRESULT hResult;
   PISRRESMEMORY pISRResMemory;

   hResult = gpResults->QueryInterface(IID_ISRResMemory, (PVOID *) &pISRResMemory);
   if (CheckError(hResult, TEXT("ISRResMemory not supported.")))
      return;

   // Get the LOCKED check box setting
   BOOL fLock = IsDlgButtonChecked(IDC_SRRESLOCKED);

   hResult = pISRResMemory->LockSet( fLock );
   CheckError(hResult, TEXT("Error in Get."));

   // Release the ISRResMemory interface pointer
   pISRResMemory->Release();
} /* End of CTabISRResMem::OnLockset() */

/////////////////////////////////////////////////////////////////////////////
// CTabISRResMerge message handlers

BOOL CTabISRResMerge::OnSetActive()
{
   if( !CPropertyPage::OnSetActive() ) return FALSE;

   TCHAR buf[MAX_STRLEN];

   // fill the combobox with the same text data as the ISRResBasic listbox...
   m_ResMergeSel.ResetContent();
   for( int i=0; i<((CSRTestDlg *)AfxGetMainWnd())->m_ResBasic.m_ResSelection.GetCount(); i++ )
   {
      ((CSRTestDlg *)AfxGetMainWnd())->m_ResBasic.m_ResSelection.GetText( giResSel, buf );
      m_ResMergeSel.AddString( buf );
   }

   wsprintf( buf, TEXT("%ld"), gdwSplitTime );
   m_ResSplitEdit.SetWindowText( buf );

   return TRUE;
}

void CTabISRResMerge::OnResmerge() 
{
   PISRRESMERGE pISRResMerge;
   LPUNKNOWN    pResults, pResultsNew;

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResMerge, (void**)&pISRResMerge);
   if( CheckError( hRes, TEXT("ISRResMerge not supported.") )) return;

   // get the pIunknown from the ISRResBasic listbox data...
   int index = m_ResMergeSel.GetCurSel();
   pResults = (LPUNKNOWN)((CSRTestDlg *)AfxGetMainWnd())->m_ResBasic.m_ResSelection.GetItemData( index );

   hRes = pISRResMerge->Merge( pResults, &pResultsNew );
   if( !CheckError( hRes, TEXT("Error in Merge.") ) )
   {
      TCHAR buf[MAX_STRLEN];
      wsprintf( buf, TEXT("pResultsNew: %lx, "), (DWORD)pResultsNew );
      m_ResMergeTxt.SetWindowText( buf );
   }

   pISRResMerge->Release();
}

void CTabISRResMerge::OnRessplit() 
{
   PISRRESMERGE pISRResMerge;
   LPUNKNOWN    pIUnkLeft, pIUnkRight;
   TCHAR        buf[MAX_STRLEN];

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResMerge, (void**)&pISRResMerge);
   if( CheckError( hRes, TEXT("ISRResMerge not supported.") )) return;

   m_ResSplitEdit.GetWindowText( buf, sizeof(buf) );
//   sscanf( buf, TEXT("%ld"), &gdwSplitTime );
   _stscanf( buf, TEXT("%ld"), &gdwSplitTime );

   hRes = pISRResMerge->Split( gdwSplitTime, &pIUnkLeft, &pIUnkRight );
   if( !CheckError( hRes, TEXT("Error in Split.") ) )
   {
      wsprintf( buf, TEXT("pIUnkLeft: %lx, pIUnkRight: %lx"), (DWORD)pIUnkLeft, (DWORD)pIUnkRight );
      m_ResSplitTxt.SetWindowText( buf );
   }

   pISRResMerge->Release();
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRResModGui message handlers

void CTabISRResModGui::OnModguihide() 
{
   PISRRESMODIFYGUI pISRResModifyGUI;

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResModifyGUI, (void**)&pISRResModifyGUI);
   if( CheckError( hRes, TEXT("ISRResModifyGUI not supported.") )) return;

   hRes = pISRResModifyGUI->Hide();
   CheckError( hRes, TEXT("Error in Hide.") );

   pISRResModifyGUI->Release();
}

void CTabISRResModGui::OnModguimove() 
{
   PISRRESMODIFYGUI pISRResModifyGUI;
   RECT             r;

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResModifyGUI, (void**)&pISRResModifyGUI);
   if( CheckError( hRes, TEXT("ISRResModifyGUI not supported.") )) return;

   r.left = r.top = 0;
   r.right = r.bottom = 100;
   hRes = pISRResModifyGUI->Move( &r );
   CheckError( hRes, TEXT("Error in Hide.") );

   pISRResModifyGUI->Release();
}

void CTabISRResModGui::OnModguishow() 
{
   PISRRESMODIFYGUI pISRResModifyGUI;
   HWND             hFakeWnd =  ((CWnd *)AfxGetMainWnd())->m_hWnd;

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResModifyGUI, (void**)&pISRResModifyGUI);
   if( CheckError( hRes, TEXT("ISRResModifyGUI not supported.") )) return;

   hRes = pISRResModifyGUI->Show( hFakeWnd );
   CheckError( hRes, TEXT("Error in Show.") );

   pISRResModifyGUI->Release();
}

/////////////////////////////////////////////////////////////////////////////
// CTabISRResSpeak message handlers

void CTabISRResSpeak::OnSpkridentify() 
{
   PISRRESSPEAKER pISRResSpeaker;
   TCHAR          szName[MAX_STRLEN], buf[MAX_STRLEN];
   DWORD          dwRank=0, dwNameSize=MAX_STRLEN, dwNeeded;
   LONG           lScore;

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResSpeaker, (void**)&pISRResSpeaker);
   if( CheckError( hRes, TEXT("ISRResSpeaker not supported.") )) return;

   APIALLOC( speechbuf, dwNameSize );
   hRes = pISRResSpeaker->Identify( dwRank, speechbuf, dwNameSize, &dwNeeded, &lScore );
   if( !CheckError( hRes, TEXT("Error in Identify.") ) )
   {
      APIS2ACVT( szName, speechbuf );
      wsprintf( buf, TEXT("Name:%s, Score:%ld"), szName, lScore );
      m_SpkrIDTxt.SetWindowText( buf );
   }

   APIFREE( speechbuf );
   pISRResSpeaker->Release();
}

void CTabISRResSpeak::OnIdforfree() 
{
   PISRRESSPEAKER pISRResSpeaker;
   BOOL           fFree;

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResSpeaker, (void**)&pISRResSpeaker);
   if( CheckError( hRes, TEXT("ISRResSpeaker not supported.") )) return;

   hRes = pISRResSpeaker->IdentifyForFree( &fFree );
   if( !CheckError( hRes, TEXT("Error in IdentifyForFree.") ) )
   {
      if( fFree == TRUE ) m_IDForFreeTxt.SetWindowText( TEXT("True") );
      else m_IDForFreeTxt.SetWindowText( TEXT("False") );
   }

   pISRResSpeaker->Release();
}

void CTabISRResSpeak::OnSpkrcorr() 
{
   PISRRESSPEAKER pISRResSpeaker;
   TCHAR          buf[MAX_STRLEN];
   WORD           wConfidence=SRCORCONFIDENCE_SOME;

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResSpeaker, (void**)&pISRResSpeaker);
   if( CheckError( hRes, TEXT("ISRResSpeaker not supported.") )) return;

   m_CorrName.GetWindowText( buf, sizeof(buf) );
   APIALLOC( speechbuf, MAX_STRLEN );
   APIA2SCVT( speechbuf, buf );

   if( m_SpkrCorrConf.GetCurSel() == 1 ) wConfidence=SRCORCONFIDENCE_VERY;

   hRes = pISRResSpeaker->Correction( speechbuf, wConfidence );
   CheckError( hRes, TEXT("Error in Confidence.") );

   pISRResSpeaker->Release();
   APIFREE( speechbuf );
}

void CTabISRResSpeak::OnSpkrvalidate() 
{
   PISRRESSPEAKER pISRResSpeaker;
   WORD           wConfidence=SRCORCONFIDENCE_SOME;
   if( m_SpkrValConf.GetCurSel() == 1 ) wConfidence=SRCORCONFIDENCE_VERY;

   HRESULT hRes;
   
   // Validate the results object pointer
   if (IsBadReadPtr(gpResults, sizeof(LPUNKNOWN)))
      return;
   
   hRes = gpResults->QueryInterface(IID_ISRResSpeaker, (void**)&pISRResSpeaker);
   if( CheckError( hRes, TEXT("ISRResSpeaker not supported.") )) return;

   hRes = pISRResSpeaker->Validate( wConfidence );
   CheckError( hRes, TEXT("Error in Validate.") );

   pISRResSpeaker->Release();
}

/* End of tabisrce.cpp */
/////////////////////////////////////////////////////////////////////////////
// CTabISRResScores property page

IMPLEMENT_DYNCREATE(CTabISRResScores, CPropertyPage)

CTabISRResScores::CTabISRResScores() : CPropertyPage(CTabISRResScores::IDD)
{
   //{{AFX_DATA_INIT(CTabISRResScores)
   //}}AFX_DATA_INIT
}

CTabISRResScores::~CTabISRResScores()
{
}

void CTabISRResScores::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTabISRResScores)
   DDX_Control(pDX, IDC_WORDSCORETEXT, m_WordScoreText);
   DDX_Control(pDX, IDC_PHRASESCORETEXT, m_PhraseScoreText);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabISRResScores, CPropertyPage)
   //{{AFX_MSG_MAP(CTabISRResScores)
   ON_BN_CLICKED(IDC_GETWORDSCORES, OnGetwordscores)
   ON_BN_CLICKED(IDC_GETPHRASESCORE, OnGetphrasescore)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabISRResScores message handlers

void CTabISRResScores::OnGetwordscores() 
{
   PISRRESSCORES pISRResScores;
   long *piScores;
   DWORD dwScoreSize;
   DWORD dwScoreNeeded;
   HRESULT hRes;
   DWORD dwRank = 1;
   TCHAR buf[100], tbuf[7];
   DWORD i;

   if (IsBadReadPtr(gpResults, sizeof(gpResults))) {
      return;
   }

   hRes = gpResults -> QueryInterface(IID_ISRResScores, (PVOID *) &pISRResScores);

   if (CheckError(hRes, TEXT("ISRResScores not supported.")))
      return;

   for (;;) {

      hRes = pISRResScores -> GetWordScores(dwRank, NULL, 0, &dwScoreNeeded);

      if (SRERR_VALUEOUTOFRANGE == hRes) {
         pISRResScores -> Release();
         if (1 == dwRank)
            m_WordScoreText.SetWindowText(TEXT("GetPhraseScore: No alternatives"));
         return;
      }

      if (E_NOTIMPL == hRes) {
         pISRResScores -> Release();
         m_WordScoreText.SetWindowText(TEXT("GetPhraseScore: Not implemented"));
         return;
      }

      if (FAILED(hRes) && (SRERR_NOTENOUGHDATA != hRes)) {
         pISRResScores -> Release();
         m_WordScoreText.SetWindowText(TEXT("GetWordScore: unexpected error"));
         return;
      }

      piScores = new long[dwScoreNeeded / sizeof(long)];

      if (NULL == piScores) {
         pISRResScores -> Release();
         m_WordScoreText.SetWindowText(TEXT("Error: Out of memory."));
         return;
      }

      dwScoreSize = dwScoreNeeded;
      hRes = pISRResScores -> GetWordScores(dwRank, piScores, dwScoreSize, &dwScoreNeeded);

      buf[0] = _T('\0');

      for (i = 0; (i * sizeof(long)) < dwScoreSize; ++i) {
         _stprintf(&tbuf[0], _T("%d "), piScores[i]);
         _tcscat(&buf[0], &tbuf[0]);
      }

      delete piScores;

      m_WordScoreText.SetWindowText( buf );
      ++dwRank;
   }
}

void CTabISRResScores::OnGetphrasescore() 
{
   PISRRESSCORES pISRResScores;
   long iScore;
   HRESULT hRes;
   DWORD dwRank = 0;
   TCHAR buf[5];

   if (IsBadReadPtr(gpResults, sizeof(gpResults))) {
      return;
   }

   hRes = gpResults -> QueryInterface(IID_ISRResScores, (PVOID *) &pISRResScores);

   if (CheckError(hRes, TEXT("ISRResScores not supported.")))
      return;

   m_WordScoreText.SetWindowText(TEXT("GetPhraseScore: Alternatives"));

   for (;;) {

      hRes = pISRResScores -> GetPhraseScore(dwRank, &iScore);

      if (hRes) {
         pISRResScores -> Release();
         if (dwRank <= 1)
            m_WordScoreText.SetWindowText(TEXT("GetPhraseScore: No alternatives"));
         return;
      }

      _stprintf(&buf[0], _T("%d"), iScore);
      if (!dwRank)
         m_PhraseScoreText.SetWindowText( buf );
      ++dwRank;
   }
}

void CTabISRDialogs::OnTrainphrase() 
{
   HWND hWnd = m_hWnd;
   HRESULT hRes;

   if (!gpEng->m_pISRDialogs2) {
      MessageBox (TEXT("ISRDialogs2 not supported"));
      return;
   }

   TCHAR buf[512];
   m_TrainText.GetWindowText (buf, 512);

   if( m_DlgTitleOpt.GetCheck() )
   {
      APIALLOC( speechtitle, MAX_STRLEN );
      APIA2SCVT( speechtitle, szDialogTitle[3] );
      hRes = gpEng->m_pISRDialogs2->TrainPhrasesDlg( hWnd, speechtitle, buf );
      APIFREE( speechtitle );
   }
   else
      hRes = gpEng->m_pISRDialogs2->TrainPhrasesDlg( hWnd, NULL, buf );

   DWORD scode = SCODE_CODE( hRes );
   CheckError( hRes, (hRes == E_NOTIMPL) ? TEXT("TrainPhrasesDlg not Implemented") : TEXT("Error calling TrainPhrasesDlg()") );
}
