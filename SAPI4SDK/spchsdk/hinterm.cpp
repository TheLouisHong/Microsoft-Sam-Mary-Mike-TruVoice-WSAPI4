/*****************************************************************************
*                           Interm.cpp
*  
*  This implements the DctPad window and communicates with the Dictation
*  object.
*  
Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
******************************************************************************/

#include "cmnhdr.h"
#include "REditCB.h"
#include "zmouse.h"

// defines


#define INTERM_BUTTONSIDE  20
#define IDM_TB_INTERM_DICTLISTEN        40073
#define IDC_INTERM_TOOLBAR              40074
#define IDC_INTERM_TEXTWND              40075

#define  WM_DCTPAD_DELETESELECTION  WM_USER+905
#define  WM_DCTPAD_SETSELECTION     WM_USER+906
#define  WM_APPCOMMAND              (WM_USER+910)

#define  NOTOOLBARBUTTONS     10
#define  IDBT_TB_OPEN         0
#define  IDBT_TB_SAVEAS       1
#define  IDBT_TB_CUT          2
#define  IDBT_TB_COPY         3
#define  IDBT_TB_PASTE        4
#define  IDBT_TB_LISTENOFF    5
#define  IDBT_TB_CUTALL       6
#define  IDBT_TB_PLAYAUDIO    7
#define  IDBT_TB_CORRECTION   8
#define  IDBT_TB_LISTENON     9
#define  IDBT_TB_NEW          10
#define  IDBT_TB_ADDWORD      11
#define  IDBT_TB_CAPITALIZE   12
#define  IDBT_TB_UTTERANCE    13
#define  IDBT_TB_LISTENDCTCMDON 14
#define  IDBT_TB_LISTENCMDON    15

#define  TIMER_SAVEUNDO       186

#define  STARTAPPS            13000 // for send to item
#define  MAXAPPS              100

#define  BASETIMERPERIOD               300   // Don't change this because a lot of stuff depends on it
#define  TRANSFERTIMERPERIOD           600
#define  TRANSFERTIMERCLICK            (TRANSFERTIMERPERIOD / BASETIMERPERIOD)

TCHAR  gszIntermOptions[] = TEXT("IntermOptions");
BOOL   gbRunningFirstTime = TRUE; 
TCHAR  gszDctPadClass[] = TEXT("DEMODICTATIONPAD");

static LRESULT CALLBACK IntermWndProc( HWND hWnd, UINT message, WPARAM wParam, 
							   LPARAM lParam);
static LRESULT CALLBACK IntermEditSubClassProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK IntermButtonSubClassProc(HWND, UINT, WPARAM, LPARAM);

HWND Init(HINSTANCE, BOOL);
BOOL ExitProc(BOOL);
void TellCorrectionOfSelection(void);
void MightDictateIntoNewWindow(void);
BOOL AskAtClose(int);
BOOL CheckforBoundary(DWORD);
void MoveCorrect(BOOL);
int MessageBoxonTop(DWORD, DWORD);
void SetTitle(void);
HRESULT SetDictStatus(DWORD);
HRESULT GetCorrectionStatus(BOOL&);
HRESULT SetCorrectionStatus(BOOL);
BOOL IsSpoken(DWORD, DWORD*, DWORD*);
int FindNextPrevAtom(DWORD, BOOL);
void ToggleCaptionAndAdjust(BOOL, BOOL);
BOOL AdjustSelection(LPDWORD, LPDWORD);
void FindWordBoundary (DWORD, DWORD*, DWORD*);
HRESULT DictTextSet(DWORD, DWORD, LPTSTR, DWORD);
HRESULT DictTextSelSet(DWORD, DWORD);
HRESULT DictTextRemove(DWORD, DWORD, DWORD);
HRESULT TransmitTypedLetter(WORD, DWORD, DWORD);
void CopyText(void);
void CutText (DWORD);
void PasteText (void);
HRESULT EmptyDctPad(DWORD);
void DoMouseWheel (WPARAM wParam, LPARAM lParam);
HRESULT InitializeDictation(BOOL);
void ReleaseDictation(void);
void CutAllToTheClipboard(void);
void CopyAllToTheClipboard(void);

static HINSTANCE    ghVersionDll = NULL;
static HINSTANCE    ghInstRichEd = NULL;

char                gszHelp[MAX_PATH + 32];
char                gszHelpProc4[MAX_PATH + 32];
INTERMOPTIONS  m_io; // options stored in registry
HINSTANCE m_hInst;
HWND m_hIntermWnd;
HWND m_hTextWnd;
WORD m_wCXFrame;
WORD m_wCYFrame;
WORD m_wCaption;
WORD m_wBorder;
WORD m_wXScreen;
WORD m_wYScreen;
WORD m_wYMenu;
HMENU m_hMenuSendTo;

WORD m_wInitialLeft;
WORD m_wInitialTop;
WORD m_wInitialWidth;
WORD m_wInitialHeight;
WORD m_wMinWidth;
WORD m_wMinHeight;

BOOL m_fSelectEntireWord;
BOOL m_fHidCorrection;   // if TRUE hid correction when minimized
BOOL m_fFileDirty;
BOOL m_fFileNew;
HMENU m_hLongIntermMenu;
HCURSOR m_hWaitCursor;
long m_lFontHeight;
BOOL m_bFontChanged;
BOOL m_bAdjustSelection;
char m_szOpenFile[260];
char m_szUntitled[64];

// toolbar
HWND m_hToolBar;
TBBUTTON m_tba[13];
WORD m_wTBHeight;
BOOL m_bHideToolBar;

// caption hiding
BOOL  m_bCaptureMove;
WORD  m_wWidthMove;
WORD  m_wHeightMove;
BOOL  m_bHideCaption;

// general purpose buffer
LPTSTR m_pszBuf;
TCHAR m_szAppName[CAPTIONBUFFERSIZE];
DWORD m_nSizeBuf;

// richedit stuff
WNDPROC m_OrigEditProc, m_OrigButtonProc, m_OrigToolBarProc;
HFONT m_hfSpoken;
BOOL m_bSpokenFont;
CHARFORMAT m_cfTyped, m_cfSpoken;
BOOL m_bDictation;
LOGFONT m_lf;
HICON m_hIDictOn, m_hIDictOff, m_hIDctPad, m_hIDictCmdOn, m_hICmdOn;
BOOL m_bKeyPressed;
BOOL m_bDontProcessTextChanged;

BOOL m_bProcessENSelChange;
DWORD m_dwRank;

BOOL m_fENChange;
BOOL m_fOpeningFile;
BOOL gfTimerSaveUndo = FALSE;


// globals

// Dictation Notification sink
class CIVDctNotifySink : public IVDctNotifySink2 {
    private:
    DWORD   m_cRef;
    HWND    m_hwnd;
    WORD    m_wLastVU;
    BOOL    m_fInUtterance;

    public:
    CIVDctNotifySink(void);
    ~CIVDctNotifySink(void);

    STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    STDMETHODIMP CommandBuiltIn       (PTSTR);
    STDMETHODIMP CommandOther         (PTSTR);
    STDMETHODIMP CommandRecognize     (DWORD, DWORD, DWORD, PVOID, PTSTR);
    STDMETHODIMP TextSelChanged       (void);
    STDMETHODIMP TextChanged          (DWORD);
    STDMETHODIMP TextBookmarkChanged  (DWORD);
    STDMETHODIMP PhraseStart          (void);
    STDMETHODIMP PhraseFinish         (DWORD, PSRPHRASE);
    STDMETHODIMP PhraseHypothesis     (DWORD, PSRPHRASE);
    STDMETHODIMP UtteranceBegin       (void);
    STDMETHODIMP UtteranceEnd         (void);
    STDMETHODIMP VUMeter              (WORD);
    STDMETHODIMP AttribChanged        (DWORD);
    STDMETHODIMP Interference         (DWORD);
    STDMETHODIMP Training             (DWORD);
    STDMETHODIMP Dictating            (PCSTR, BOOL);
    STDMETHODIMP Error                (LPUNKNOWN);
    STDMETHODIMP Warning              (LPUNKNOWN);
};
typedef CIVDctNotifySink * PCIVDctNotifySink;

HWND ghCorrectionWnd = NULL;

PCIVDctNotifySink gpVDctNotifySink;
PIVOICEDICTATION  gpIVoiceDictation = NULL;
PIVDCTTEXT        gpIVDctText = NULL;
PIVDCTGUI         gpIVDctGui = NULL;
PIVDCTDIALOGS     gpIVDctDialogs = NULL;
PIVDCTATTRIBUTES  gpIVDctAttributes = NULL;
PIATTRIBUTES      gpIAttributes = NULL;
PIVDCTCOMMANDSAPP gpIVDctCommandsApp = NULL;
PISRSPEAKER       gpIVDctSpeaker = NULL;
PIVDCTTEXTCACHE   gpIVDctTextCache = NULL;
PIVDCTGLOSSARY    gpIVDctGlossary = NULL;
PIVDCTCOMMANDS    gpIVDctCommands = NULL;
PISTLEXDLG        gpILexDlg = NULL;
BOOL              gfRank = FALSE;
int               gi = 0;
static DWORD      gdwlButtondownPos = 0xffffffff;
static BOOL       gfKeyUp = FALSE;
static BOOL       gfDontNotify = FALSE;
DWORD             m_dwEnabled = 0;

char *MSD_Version = "46FC730A-D849-11d0-AB8A-08002BE4E3B7";

static CRichEditOleCallback gRichEditOleCallback;
#define  WM_DOMENU         (WM_USER+91)
#define  WM_OPENDICTFILE   (WM_USER+96)

// prototypes

static BOOL InitApplication(HINSTANCE);
static BOOL InitInstance(HINSTANCE, int);
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void GetChangesFromVDct();
//static BOOL CheckSoundCards(void);
int MessageBoxonTop(DWORD, DWORD);

// globals

HINSTANCE           ghInst = NULL; 
static HMENU        ghFloatingMenu = NULL;
static HMENU        ghmenuResource = NULL;
HWND                ghIntermWnd = NULL;
static char         gszBuffer[BUFFER];

char                gszAppName[CAPTIONBUFFERSIZE];
UINT                guMSH_MOUSEWHEEL = 0;
LPSTR               gpszFile = NULL;

#define              NUMBIN      2
BOOL                gfUndoHasData[NUMBIN];
DWORD               gadwUndoSelStart[NUMBIN];  // undo selection start
DWORD               gadwUndoSelEnd[NUMBIN];    // undo selection end
DWORD               gadwUndoFirstVisible[NUMBIN]; // first visible line for undo
DWORD               gadwCheckSum[NUMBIN];       // check sum for the buffer, so we know if has changed
BOOL                gfProcessingUndo = FALSE;   // if true, processing undo, so ignore changes

DWORD               gdwUndoLatest = 0;         // which undo buffer has the lastest (dictated) data

#include "zmouse.h"

/*****************************************************************
GenerateCheckSum - Generates a checksum for the buffer .It's an easy-to-code
way of doing undo.

inputs
   none
returns
   DWORD - checksum
*/
DWORD GenerateCheckSum (void)
{
   DWORD dwLen;
   HRESULT hRes;
   hRes = gpIVDctText->Lock();
   if (hRes)
      return 0;

   hRes = gpIVDctText->TextLengthGet(&dwLen);
   if (hRes) {
      gpIVDctText->UnLock();
      return 0;
   }

   SDATA dData;
   char  *psz;
   hRes = gpIVDctText->TextGet(0, dwLen, &dData);
   if (hRes) {
      gpIVDctText->UnLock();
      return 0;
   }
   psz = (char*)dData.pData;


   gpIVDctText->UnLock();

   // check sum
   DWORD dwSum, i;
   for (dwSum = i = 0; i < dwLen; i++)
      dwSum += (DWORD) (BYTE) psz[i] * (i+1);

   CoTaskMemFree (psz);

   return dwSum;
}


/*****************************************************************
UndoRemember - This remembers the buffer so that it can be used
   for undo. It sets the gdwUndoLatest to the buffer with the latest

*/
HRESULT UndoRemember (void)
{
   // figre out which bin to use.
   // look at the checksum
   DWORD dwSum, dwBin;
   dwSum = GenerateCheckSum();

   if ((dwSum == gadwCheckSum[0]) && gfUndoHasData[0])
      // matches the first buffer
      dwBin = 0;
   else if ((dwSum == gadwCheckSum[1]) && gfUndoHasData[1])
      // mateches the 2nd buffer
      dwBin = 1;
   else
      // if got here, no mach, so undo to the oldest bin
      dwBin = !gdwUndoLatest;

   HRESULT hRes;
   hRes = gpIVDctText->Lock();
   if (hRes)
      return hRes;

   DWORD dwLen;
   hRes = gpIVDctText->TextLengthGet(&dwLen);
   if (hRes) {
      gpIVDctText->UnLock();
      return hRes;
   }

   // cache this
   hRes = gpIVDctTextCache->CopyToBin (0, dwLen, dwBin);
   if (hRes) {
      gpIVDctText->UnLock();
      return hRes;
   }

   // unlock
   gpIVDctText->UnLock();

   // store this
   SendMessage (m_hTextWnd, EM_GETSEL, (WPARAM)&gadwUndoSelStart[dwBin], (LPARAM)&gadwUndoSelEnd[dwBin]);
   gadwUndoFirstVisible[dwBin] = SendMessage (m_hTextWnd, EM_GETFIRSTVISIBLELINE, 0, 0);
   gfUndoHasData[dwBin] = TRUE;
   gadwCheckSum[dwBin] = GenerateCheckSum();

   gdwUndoLatest = dwBin;

   // done
   return NOERROR;
}


/*****************************************************************
RememberIfTimer - Remembers if the timer is going
*/
void RememberIfTimer (void)
{
   if (!gfTimerSaveUndo)
      return;

   // save the text for undo
   KillTimer (m_hIntermWnd, TIMER_SAVEUNDO);
   gfTimerSaveUndo = FALSE;

   UndoRemember();

}

/*****************************************************************
SaveCurrentToUndo - Saves the current state to undo
*/
HRESULT SaveCurrentToUndo (void)
{
   // if we changed text, but those changes haven't been saved then
   // do so now
   if (gfTimerSaveUndo) {
      // & kill the timer
      KillTimer (m_hIntermWnd, TIMER_SAVEUNDO);
      gfTimerSaveUndo = FALSE;
   }
   

   UndoRemember();

   return NOERROR;
}


/*****************************************************************
SetUndoTimer - Sets the timer so we have a lazy undo
*/
void SetUndoTimer(void)
{
   if (gfTimerSaveUndo)
      KillTimer (m_hIntermWnd, TIMER_SAVEUNDO);
   SetTimer (m_hIntermWnd, TIMER_SAVEUNDO, 5000 /* 5 sec*/, 0);
   gfTimerSaveUndo = TRUE;
}

/*****************************************************************
UndoRestore - Restores the undo buffer (and stores the current
   state back in undo, so can do redo)
*/
void UndoRestore (void)
{

   HRESULT hRes;
   hRes = SaveCurrentToUndo();
   if (hRes)
      return;

   // figure out which bin we're going to get data from
   DWORD dwBin;
   dwBin = !gdwUndoLatest;

   // first, cache the existing dwata
   DWORD dwUndoBin = dwBin;    // bin in which undo info stored, either 1 or 2. If 0, no undo
   DWORD dwUndoSelStart = gadwUndoSelStart[dwBin];  // undo selection start
   DWORD dwUndoSelEnd = gadwUndoSelEnd[dwBin];    // undo selection end
   DWORD dwUndoFirstVisible = gadwUndoFirstVisible[dwBin]; // first visible line for undo

   // if there's nothing to restore to then don't change
   if (dwBin > NUMBIN)
      return;
   if (!gfUndoHasData[dwBin])
      return;

   // restore
   hRes = gpIVDctText->Lock();
   if (hRes)
      return;

   DWORD dwLen;
   hRes = gpIVDctText->TextLengthGet(&dwLen);
   if (hRes) {
      gpIVDctText->UnLock();
      return;
   }

   // uncache this
   hRes = gpIVDctTextCache->PasteFromBin (0, dwLen, dwUndoBin);
   if (hRes) {
      gpIVDctText->UnLock();
      return;
   }

#ifdef _DEBUG
   hRes = gpIVDctText->TextLengthGet(&dwLen);
#endif


   hRes = gpIVDctText->TextSelSet(dwUndoSelStart, dwUndoSelEnd - dwUndoSelStart);

   // unlock
   gpIVDctText->UnLock();

   // restore the changes
   gfProcessingUndo = TRUE;
   GetChangesFromVDct();
   gfProcessingUndo = FALSE;

   // restore the line & selection
   // let the selection change message be sent
   DWORD dwCur;
   dwCur = SendMessage (m_hTextWnd, EM_GETFIRSTVISIBLELINE, 0, 0);
   SendMessage (m_hTextWnd, EM_LINESCROLL, 0, (long) dwUndoFirstVisible - (long) dwCur);

   // note that we're displaying the other one now
   gdwUndoLatest = dwBin;
}



/******************************************************************
TurnOnMode - Returns which mode we should turn on with, either
   VSRMODE_DCTONLY or VSRMODE_CMDANDDCT
*/
DWORD TurnOnMode (void)
{
   HKEY  hKey;
   RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\MSDictation", 0, KEY_READ, &hKey);
   if (!hKey)
      return VSRMODE_DCTONLY;

   DWORD d, d1, dwVal;
   dwVal = TRUE;
   d = REG_DWORD;
   d1 = sizeof(dwVal);

   RegQueryValueEx(hKey, "TurnOnMode", NULL, &d, (LPBYTE)&dwVal, &d1);
   RegCloseKey(hKey);

   return dwVal ? VSRMODE_DCTONLY : VSRMODE_CMDANDDCT;

}


/******************************************************************
SetDictationIcon - Displays the correction icon for dictation
*/
void SetDictationIcon (void)
{
   HICON hIcon;
   DWORD dwBitmap;
   switch (m_dwEnabled) {
      case VSRMODE_DCTONLY:
         hIcon = m_hIDictOn;
         dwBitmap = IDBT_TB_LISTENON;
         m_bDictation = TRUE;
         break;
      case VSRMODE_CMDANDDCT:
         hIcon = m_hIDictCmdOn;
         dwBitmap = IDBT_TB_LISTENDCTCMDON;
         m_bDictation = TRUE;
         break;
      case VSRMODE_CMDONLY:
         hIcon = m_hICmdOn;
         m_bDictation = TRUE;
         dwBitmap = IDBT_TB_LISTENCMDON;
         break;
      default:
         hIcon = m_hIDictOff;
         dwBitmap = IDBT_TB_LISTENOFF;
         m_bDictation = FALSE;
   }

   SendMessage(m_hIntermWnd, WM_SETICON, FALSE, (LPARAM)hIcon);
   SendMessage(m_hToolBar, TB_CHANGEBITMAP, IDM_TB_INTERM_DICTLISTEN, MAKELPARAM(dwBitmap, 0));
   SendMessage(m_hToolBar, TB_CHECKBUTTON, IDM_TB_INTERM_DICTLISTEN, MAKELONG(m_bDictation, 0));
   CheckMenuItem(m_hLongIntermMenu, IDM_INTERM_DICTLISTEN, MF_BYCOMMAND|(m_bDictation ? MF_CHECKED : MF_UNCHECKED));
   CheckMenuItem(m_hLongIntermMenu, IDM_INTERM_DICTNOTLISTEN, MF_BYCOMMAND|(!m_bDictation ? MF_CHECKED : MF_UNCHECKED));
}

/*****************************************************************
DoAppCommand - If a command is spoken that's app specific, this
handles it.

inputs
   DWORD dwID - Command ID
   PCSTR psz - String
returns
   none
*/
void DoAppCommand (DWORD dwID, PCSTR psz)
{
   DWORD dwVal, dwStart, dwEnd, dwLine, dwTemp;

   dwVal = psz ? atoi(psz) : 1;

   switch (dwID) {
   case 10: // go to the (StartBegin) of the line
   case 11: // go to the end of the line
      {
         SendMessage (m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
         dwLine = SendMessage (m_hTextWnd, EM_LINEFROMCHAR, (dwID == 10) ? dwStart : dwEnd, 0);
         dwStart = dwEnd = SendMessage (m_hTextWnd, EM_LINEINDEX, (dwID == 10) ? dwLine : dwLine+1, 0);
         if (dwStart == (DWORD)-1L)
            dwStart = GetWindowTextLength(m_hTextWnd);
         if ((dwID == 11) && (dwStart >= 2) && (dwEnd != (DWORD)-1L) )
            dwStart -= 2;  // remove /r and /n
         SendMessage (m_hTextWnd, EM_SETSEL, dwStart, dwStart);
      }
      break;

   case 12: // go (DownUp) <NaturalA> (LineLines)
   case 13: // go up N lines
      {
         SendMessage (m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
         dwLine = SendMessage (m_hTextWnd, EM_LINEFROMCHAR, (dwID == 10) ? dwStart : dwEnd, 0);
         if (dwID == 12) {
            dwLine += dwVal;
         }
         else {
            if (dwLine >= dwVal)
               dwLine -= dwVal;
            else
               dwLine = 0;
         }
         dwStart = SendMessage (m_hTextWnd, EM_LINEINDEX, dwLine, 0);
         if (dwStart == (DWORD)-1L)
            dwStart = GetWindowTextLength(m_hTextWnd);
         SendMessage (m_hTextWnd, EM_SETSEL, dwStart, dwStart);
      }
      break;
   case 20: // scroll (DownUp) <NaturalA> pages
   //case 20: // scroll (DownUp) a page
   case 21: // scroll up
   case 22: // scroll (DownUp) <NaturalA> (LineLines)
   // case 22: // scroll (DownUp) a line
   case 23: // scroll up
      {
         WPARAM   wParam;
         switch (dwID) {
         case 20:
            wParam = SB_PAGEDOWN;
            break;
         case 21:
            wParam = SB_PAGEUP;
            break;
         case 22:
            wParam = SB_LINEDOWN;
            break;
         case 23:
            wParam = SB_LINEUP;
            break;
         }

         for (; dwVal; dwVal--)
            SendMessage (m_hTextWnd, EM_SCROLL, wParam, 0);
      }
      break;


   case 29: // select [opt] this line
   case 30: // select (LastPrevious) [opt] <NaturalA> (LineLines)
   case 31: // select next
      {
         SendMessage (m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
         dwLine = SendMessage (m_hTextWnd, EM_LINEFROMCHAR, (dwID == 31) ? dwEnd : dwStart, 0);

         // find the starting position
         switch (dwID) {
         case 29:
            // do nothing
            break;
         case 30:
            if (dwLine >= dwVal)
               dwLine -= dwVal;
            else
               dwLine = 0;
            break;
         case 31:
            // do nothing
            break;
         }
         dwStart = SendMessage (m_hTextWnd, EM_LINEINDEX, dwLine, 0);
         if (dwStart == (DWORD)-1L)
            dwStart = GetWindowTextLength(m_hTextWnd);

         // find the ending position
         dwLine += dwVal;
         dwEnd = SendMessage (m_hTextWnd, EM_LINEINDEX, dwLine, 0);
         if (dwEnd == (DWORD)-1L)
            dwEnd = GetWindowTextLength(m_hTextWnd);

         // set the selection
         SendMessage (m_hTextWnd, EM_SETSEL, dwStart, dwEnd);
      }
      break;

   case 32: // select the rest of (TheThis) line
   // case 32: // select to the end of (TheThis) line
   case 33: // select to the beginning of (TheThis) line
   // case 33: // select the first part of (TheThis) line
   // case 33: // select the start of (TheThis) line
      {
         SendMessage (m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
         dwLine = SendMessage (m_hTextWnd, EM_LINEFROMCHAR, (dwID == 32) ? dwEnd : dwStart, 0);

         // find the starting position
         switch (dwID) {
         case 32:
            dwLine++;
            break;
         case 33:
            // do nothing
            break;
         }
         if (dwID == 32)
            dwEnd = SendMessage (m_hTextWnd, EM_LINEINDEX, dwLine, 0);
         else
            dwStart = SendMessage (m_hTextWnd, EM_LINEINDEX, dwLine, 0);
         if (dwStart == (DWORD)-1L)
            dwStart = GetWindowTextLength(m_hTextWnd);
         if (dwEnd == (DWORD)-1L)
            dwEnd = GetWindowTextLength(m_hTextWnd);

         // set the selection
         SendMessage (m_hTextWnd, EM_SETSEL, dwStart, dwEnd);
      }
      break;

   case 34: // extend the selection by <NaturalA> (LineLines) [opt] (ToTheRightLeft)
   case 35: // to the left
   case 36: // reduce the selection by <NaturalA> (LineLines) [opt] (FromTheRightLeft)
   case 37: // to the left
      {
         SendMessage (m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
         dwTemp = ((dwID == 34) || (dwID == 36)) ? dwEnd : dwStart;
         dwLine = SendMessage (m_hTextWnd, EM_LINEFROMCHAR,
            dwTemp, 0);

         // find the starting position of where to extend to
         switch (dwID) {
         case 34:
         case 37:
            dwLine += dwVal;
            break;
         case 35:
         case 36:
            // if we're in the middle of a line then don't go back mroe
            if (dwTemp != (DWORD) SendMessage (m_hTextWnd, EM_LINEINDEX, dwLine, 0))
               dwVal--;

            if (dwLine >= dwVal)
               dwLine -= dwVal;
            else
               dwLine = 0;
            break;
         }
         if ((dwID == 34) || (dwID == 36))
            dwEnd = SendMessage (m_hTextWnd, EM_LINEINDEX, dwLine, 0);
         else
            dwStart = SendMessage (m_hTextWnd, EM_LINEINDEX, dwLine, 0);
         if (dwStart == (DWORD)-1L)
            dwStart = GetWindowTextLength(m_hTextWnd);
         if (dwEnd == (DWORD)-1L)
            dwEnd = GetWindowTextLength(m_hTextWnd);

         // make sure we don't eceed boundaries
         if ((dwID == 34) || (dwID == 36)) {
            if (dwEnd < dwStart)
               dwEnd = dwStart;
         }
         else {
            if (dwStart > dwEnd)
               dwStart = dwEnd;
         }

         // set the selection
         SendMessage (m_hTextWnd, EM_SETSEL, dwStart, dwEnd);
      }
      break;


   case 51: // create [opt] a new (FileDoc)
   case 55: // close (TheThis) (FileDoc)
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_FILENEW, 0);
      break;
   case 52: // open [opt] a [opt] existing (FileDoc)
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_FILEOPEN, 0);
      break;
   case 53: // save (TheThis) (FileDoc)
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_FILESAVE, 0);
      break;
   case 54: // save (TheThis) (FileDoc) (AsType)
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_FILESAVEAS, 0);
      break;
   case 56: // copy this
   // case 56: // copy [opt] the selection
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_EDITCOPY, 0);
      break;
   case 57: // cut this
   // case 57: // cut [opt] the selection
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_EDITCUT, 0);
      break;
   case 58: // paste this
   // case 58: // paste over [opt] the selection
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_EDITPASTE, 0);
      break;
   case 59: // change the font
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_SHOWSETFONT, 0);
      break;
   case 60: // undo [opt] that
   case 61: // redo [opt] that
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_EDITUNDO, 0);
      break;
   case 62: // send the text to the clipboard
      SendMessage (m_hIntermWnd, WM_COMMAND, ID_FILE_SENDTEXTTO_THECLIPBOARD, 0);
      break;
   case 63: // close dictation pad
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_EXIT, 0);
      break;
   case 64: // cut all
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_EDITCUTALL, 0);
      break;
   case 66: // help
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_HELPTOPICS, 0);
      break;
   case 69: // go to the microsoft speech research page
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_WEB_RESEARCH, 0);
      break;
   case 70: // go to the microsoft web site
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_WEB_HOME, 0);
      break;
   case 71: // tell me about dictation pad
   // case 71: // about dictation pad
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_HELPABOUT, 0);
      break;
   case 72: // list dictation commands
   // case 72: // what can I say
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_INTERM_WHATCANISAY, 0);
      break;

   case 80: // add a new word
      SendMessage (m_hIntermWnd, WM_COMMAND, IDM_ADDWORD, 0);
      break;
   }

   return;
}

/*****************************************************************
AreWeDictating - Returns TRUE if dictation is on, FALSE if it's off.

inputs
   nonre
returns
   BOOL - TRUE if on
*/
BOOL AreWeDictating (void)
{
   DWORD dwEnabled;
   if (!gpIVDctAttributes)
      return FALSE;
   if (!gpIVDctAttributes->ModeGet(&dwEnabled))
      m_dwEnabled = dwEnabled;

   m_bDictation = ((dwEnabled == VSRMODE_DCTONLY) || (dwEnabled == VSRMODE_CMDANDDCT) || (dwEnabled == VSRMODE_CMDONLY));
   return m_bDictation;
}


/*****************************************************************************
RunMSDict - Runs msdict so there's a control panel applet for sR
*/
BOOL RunMSDict (void)
{
   HKEY  hKey;
   BOOL  fRanMSDict = FALSE;

   RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Voice", 0, KEY_READ, &hKey);
   if (hKey) {
      DWORD d, d1;
      char  szTemp[256];
      d = REG_SZ;
      d1 = sizeof(szTemp) - 1;

      if (ERROR_SUCCESS == RegQueryValueEx(hKey, "Dictation", NULL, &d,  
         (LPBYTE)szTemp, &d1)) {
             HINSTANCE  h;
             h = ShellExecute(m_hIntermWnd, NULL, szTemp, "-nowarn", NULL, SW_SHOWNORMAL);
             fRanMSDict = (((DWORD)h) > 32);

      }
      RegCloseKey(hKey);
   }

   return fRanMSDict;
}

/*****************************************************************************
* Winmain
*
* The startup function
*
******************************************************************************/

int CALLBACK WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPSTR pszCmdLine, int nCmdShow)
{
   MSG msg;
   char szPath[MAX_PATH];
   HKEY hKey;

   // see if MSDict.exe has run yet for this user. If not, we should
   // let MSDict run, and then quit out of dctpad right away because
   // msdict will bring up a wizard to make sure everything is installed
   // properly, and eventually re-run dctpad.exe
   RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\MSDictation", 0, KEY_READ, &hKey);
   if (hKey)
      RegCloseKey (hKey);
   BOOL  fMSDictRun = hKey ? TRUE : FALSE;


   // Want to check and make sure MSDict is running
   // hacing msdict around ensures that the user can set options
   // such as which sr engine to use
   BOOL  fRanMSDict = FALSE;
   fRanMSDict = RunMSDict();

   // potentially stop dctpad right here if this is the first time msdict is run
   if (fRanMSDict && !fMSDictRun)
      return 0;

   // set help dir
   // GetCurrentDirectory returns c:\windows\desktop on win95
   LoadString(ghInst, IDS_MSDICTINSTALL, gszBuffer, BUFFER-1);
   RegOpenKeyEx(HKEY_LOCAL_MACHINE, gszBuffer, 0, KEY_READ, &hKey);
   if (hKey) {
      DWORD d, d1;
      d = REG_SZ;
      d1 = sizeof(gszHelp) - 1;

      LoadString(ghInst, IDS_INSTALLDIR, gszBuffer, BUFFER-1);
      if (ERROR_SUCCESS != RegQueryValueEx(hKey, gszBuffer, NULL, &d,  
                              (LPBYTE)gszHelp, &d1))
         GetCurrentDirectory(sizeof(gszHelp), gszHelp);

      RegCloseKey(hKey);
   }

   strcpy(gszHelpProc4, gszHelp);
   strcat(gszHelp, "\\msdict.hlp");
   strcat(gszHelpProc4, "\\msdict.hlp>proc4");
   

   if (!LoadString(hInstance, IDS_APPNAME, gszAppName, CAPTIONBUFFERSIZE-1))
      ExceptionHandler(MSDICT_E_LOADSTRING, 1, __FILE__, __LINE__, 0);
   // we wont check for any more LoadString failures

   
   // Register the mouse wheel message
   guMSH_MOUSEWHEEL = RegisterWindowMessage(MSH_MOUSEWHEEL);

   if (!InitInstance(hInstance, nCmdShow))
      return (FALSE); 

   HACCEL hAccel;
   hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_DICT_ACCELTABLE));

   // process cmd line parameter
   if (*pszCmdLine) {
      // check if its a fully qualified path
      if ((strstr(pszCmdLine, ":\\") == pszCmdLine + 1) ||
          (strlen(pszCmdLine) > 2 && 
            (pszCmdLine[0] == '\\' && pszCmdLine[1] == '\\')))
         PostMessage(ghIntermWnd, WM_COMMAND, IDM_INTERM_FILEOPEN, (LPARAM)pszCmdLine);
      else {
         GetCurrentDirectory(sizeof(szPath), szPath);
         strcat(szPath, "\\");
         strcat(szPath, pszCmdLine);
         PostMessage(ghIntermWnd, WM_COMMAND, IDM_INTERM_FILEOPEN, (LPARAM)szPath);
      }
   }

   // show the window
   ShowWindow(m_hIntermWnd, nCmdShow);


   while (GetMessage(&msg, NULL, 0, 0)) {
      if (!TranslateAccelerator(ghIntermWnd, hAccel, &msg)) {
   	   TranslateMessage(&msg);
         DispatchMessage(&msg); 
      }
   }

   if (ghVersionDll)
      FreeLibrary(ghVersionDll);
   if (ghInstRichEd)
      FreeLibrary(ghInstRichEd);


   return (msg.wParam); 
}


/*****************************************************************************
* InitInstance
*
* Loads the icons 
* Creates the main window
* Loads the commctrl library
* Calls the DctPad window initialization function
* Checks the sound cards
* Creates the task-bar menu
* Creates registry key if not already present
* Initializes OLE
* Checks for engines and sound cards the first time
*
******************************************************************************/

BOOL InitInstance(HINSTANCE hInstance, 
                  int nCmdShow)
{
   ghInst = hInstance; 

   // load the commctrl library
   InitCommonControls();

   // Initialize OLE
   if (FAILED(OleInitialize(NULL))) {
      ExceptionHandler(MSDICT_E_COINITIALIZE, 2, __FILE__, __LINE__, 0);
      return FALSE;
   }

	UINT uErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

   //return Error Mode to previous state
   SetErrorMode (uErrorMode);

   // Initialize DctPad window object
   ghIntermWnd = Init(hInstance, FALSE);
   if (!ghIntermWnd)
      return FALSE;

   return TRUE;
}


/*****************************************************************************
* ShutDown
*
* Cleans up
*
* bMode == TRUE if shutting down, FALSE if not
*
******************************************************************************/

void ShutDown(BOOL bMode)
{ 
   if (!bMode) {
      ReleaseDictation();
      OleUninitialize();
   }

} 


/***********************************************************8
CorrectionOnOff - Turns correction on/off.

inputs
   BOOL     fOn - If TRUE turn on
rturns
   void
*/

void CorrectionOnOff (BOOL fOn)
{
   SetCorrectionStatus(fOn);
}


/***********************************************************************
EndSessionProc - Sets dictation on/off.

inputs
   DWORD    dwMode - VSRMODE_XXX
   DWORD    dwRc - Resource ID to display as small text
returns
   void
*/


void EndSessionProc(void)
{
   ExitProc(TRUE);
   ShutDown(TRUE);
}





/*****************************************************************************
* ExceptionHandler
*
* Called when an exception occurs
*
* dwSevCode gives the severity of exception, 1 means it's better to restart
* Dictation but its a non fatal error and 2 means a fatal error.
*
******************************************************************************/

static DWORD gdwSevCode;
static DWORD gdwErrorCode;
static LPTSTR gpszFileName;
static DWORD gdwLine;

void ExceptionHandler(DWORD dwErrorCode, 
                      DWORD dwSevCode,
                      LPTSTR pszFileName, 
                      DWORD dwLine,
                      HRESULT hRes)
{
   static BOOL b = FALSE;
   
   if (b)
      return; // once we have put up a Fatal error dont put up any more

   if (2 == dwSevCode)
      b = TRUE;

   gdwErrorCode = dwErrorCode;
   gdwSevCode = dwSevCode;
   gpszFileName = pszFileName;
   gdwLine = dwLine;

   char szErr[256];

   ZeroMemory(szErr, 256);

   switch (gdwErrorCode) {
      case MSDICT_E_LOADSTRING:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_LOADSTRING, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_INITAPPLICATION:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_INITAPPLICATION, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_REGCREATEKEY:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_REGCREATEKEY, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_COINITIALIZE:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_COINITIALIZE, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_LOADVCMD:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_LOADVCMD, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;
      
      case MSDICT_E_OUTOFMEM:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_OUTOFMEM, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_ALREADYRUNNING:
         LoadString(ghInst, IDS_E_ALREADYRUNNING, szErr, 255);
      break;

      case MSDICT_E_LOADRICHED32DLL:
         LoadString(ghInst, IDS_E_LOADRICHED32DLL, szErr, 255);
      break;

      case MSDICT_E_VCMDQUIT:
         LoadString(ghInst, IDS_E_VCMDQUIT, szErr, 255);
      break;

      case MSDICT_E_VCMDCOCREATE:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_VCMDCOCREATE, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_VCMDQUERY:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_VCMDQUERY, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_VCMDREGISTER:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_VCMDREGISTER, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_VCMDACTIVATE:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_VCMDACTIVATE, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_VCMDCOGETCLASS:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_VCMDCOGETCLASS, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_VCMDCREATEINSTANCE:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_VCMDCREATEINSTANCE, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_VCMDSITEINFOSET:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_VCMDSITEINFOSET, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

      case MSDICT_E_NOSOUNDCARD:
         LoadString(ghInst, IDS_EXCEPTION_FATAL, szErr, 255);
         LoadString(ghInst, IDS_E_NOSOUNDCARD, szErr + strlen(szErr), 
                    255 - strlen(szErr));
      break;

   }               

   char szHRes[16];
   MSGBOXPARAMS msgBoxParams;

   sprintf(szHRes, " (%X)", hRes);
   strcat(szErr, szHRes);

	memset((void*)&msgBoxParams, 0, sizeof(MSGBOXPARAMS));
	msgBoxParams.cbSize = sizeof(MSGBOXPARAMS);
	msgBoxParams.hInstance = ghInst;

	msgBoxParams.lpszText = szErr;
	msgBoxParams.lpszCaption = gszAppName;
	msgBoxParams.dwStyle = MB_OK|MB_TOPMOST|MB_ICONERROR;
	msgBoxParams.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);

   MessageBoxIndirect(&msgBoxParams);
}



int MessageBoxonTop(HWND hWndOwner, DWORD dwIDText, DWORD dwStyle)
{
   char pszBuf[512];
   MSGBOXPARAMS msgBoxParams;

	memset((void*)&msgBoxParams, 0, sizeof(MSGBOXPARAMS));
   msgBoxParams.hwndOwner = hWndOwner;
	msgBoxParams.cbSize = sizeof(MSGBOXPARAMS);
	msgBoxParams.hInstance = ghInst;

	LoadString(ghInst, dwIDText, pszBuf, BUFFER - 1);
	msgBoxParams.lpszText = pszBuf;
	msgBoxParams.lpszCaption = gszAppName;
	msgBoxParams.dwStyle = dwStyle|MB_TOPMOST;
	msgBoxParams.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);

   return MessageBoxIndirect(&msgBoxParams);
}

/*******************************************************************
_IsWindowOK - Returns TRUE if the window is one that theuser
   might switch to (visible, etc.). FALSE if not

inputs
   HWND     hWnd - Window
returns
   BOOL - TRUE of OK
*/
BOOL _IsWindowOK (HWND hWnd)
{
   if (!hWnd)
      return FALSE;
   if (!IsWindowVisible(hWnd))
      return FALSE;
   if (!GetWindowTextLength(hWnd))
      return FALSE;

   TCHAR szTemp[256];
   GetWindowText (hWnd, szTemp, sizeof(szTemp)/sizeof(TCHAR));

   if (strstr(szTemp, "Progman"))
      return FALSE;

   if (strstr(szTemp, "Program Manager"))
      return FALSE;

   if (hWnd == m_hIntermWnd)
      return FALSE;

   if (strstr(szTemp, "tty"))
      return FALSE;

   // Dont switch to a child or owned window or transparent window or desktop.
   if (GetParent(hWnd) 
         || GetWindow(hWnd, GW_OWNER) 
         || (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TRANSPARENT))
      return FALSE;

   return TRUE;
}

/*****************************************************************
ListOfRunningApps - This adds the list of running application (excluding
   DctPad) to the menu.
*/
void ListOfRunningApps (HMENU hMenu, WORD wStartID)
{
   // loop through the window
   // #2099
   // First put the topmost visible window at the bottom of the z-order.  This
   // will ensure that we will cycle through all the windows.
   HWND hwndNext;
   HWND hwndDesktop = GetDesktopWindow();

   hwndNext = ::GetWindow(hwndDesktop, GW_CHILD);

   for (;;hwndNext = ::GetWindow(hwndNext, GW_HWNDNEXT))
   {
      char szTemp[256];

      // If we've reached the end of the z-order, this means we
      // have no visible windows.
      if (!hwndNext)
         break;

      if (!_IsWindowOK(hwndNext) || hwndNext == ghCorrectionWnd)
         continue;

      // add it
      GetClassName (hwndNext, szTemp, sizeof(szTemp));
      // dont add correction window
      if (!stricmp(szTemp, "CORRECTCLS") || !stricmp(szTemp, gszDctPadClass))
         continue;

      GetWindowText (hwndNext, szTemp, sizeof(szTemp));
      if (!szTemp[0])
         continue;

      AppendMenu (hMenu, MF_ENABLED | MF_STRING, wStartID++, szTemp);
   }

}


// should not call Init more than once

HWND Init(HINSTANCE hInst, BOOL fMicWizCancel)
{
	m_hInst = 0;
	m_hIntermWnd = 0;
	m_hTextWnd = 0;
   m_hLongIntermMenu = 0;
	m_wCXFrame = 0;
	m_wCYFrame = 0;
	m_wCaption = GetSystemMetrics(SM_CYCAPTION);
	m_wBorder = GetSystemMetrics(SM_CYBORDER);
	m_wXScreen = GetSystemMetrics(SM_CXSCREEN);
	m_wYScreen = GetSystemMetrics(SM_CYSCREEN);
   m_wYMenu = GetSystemMetrics(SM_CYMENU);
   m_fHidCorrection = FALSE;  // just in case was minimized
   m_hMenuSendTo = NULL;
   m_fSelectEntireWord = TRUE;
	m_nSizeBuf = BUFFER;
	m_hfSpoken = 0;
	m_bSpokenFont = TRUE;
   m_bKeyPressed = FALSE;
   memset(&m_cfTyped, 0, sizeof(CHARFORMAT));
	m_cfTyped.cbSize = sizeof(CHARFORMAT);
	// set the padding m_crf._wpadl too????
	memset(&m_cfSpoken, 0, sizeof(CHARFORMAT));
	m_cfSpoken.cbSize = sizeof(CHARFORMAT);
	m_bDictation = FALSE;

   m_bCaptureMove = FALSE;
   m_wWidthMove = 0;
   m_wHeightMove = 0;
   m_bHideCaption = FALSE;
   m_bHideToolBar = FALSE;
   m_wTBHeight = 0;
   m_hfSpoken = NULL;
   m_fFileDirty = FALSE;
   m_fFileNew = TRUE;
   m_fENChange = FALSE;
   m_fOpeningFile = FALSE;

	memset (&m_lf, 0, sizeof(m_lf));
   m_bFontChanged = FALSE;
   m_bProcessENSelChange = TRUE;
   m_bAdjustSelection = FALSE;
   m_dwRank = 1;
   m_hWaitCursor = LoadCursor(NULL, IDC_WAIT);

   static BOOL bFirstTime = TRUE;
   HGLOBAL hglbMem = NULL;
   LPSTR lpPaste = NULL;
   HRESULT hRes = 0;

	if (bFirstTime) {
      m_hInst = hInst;

      LoadString(hInst, IDS_APPNAME, m_szAppName, CAPTIONBUFFERSIZE-1);

		m_hIDictOn = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_DICTON), IMAGE_ICON, 16, 16, 0);
		m_hIDictCmdOn = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_DICTCMDON), IMAGE_ICON, 16, 16, 0);
		m_hICmdOn = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_CMDON), IMAGE_ICON, 16, 16, 0);
		m_hIDictOff = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_DICTOFF), IMAGE_ICON, 16, 16, 0);

      m_hLongIntermMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_INTERM_LONGMENU));

		WNDCLASS wcInterm;

      wcInterm.style         = CS_HREDRAW | CS_VREDRAW;	
      wcInterm.lpfnWndProc   = (WNDPROC)IntermWndProc;  
      wcInterm.cbClsExtra    = 0;                     
      wcInterm.cbWndExtra    = 4;	// for the edit window to avoid recursion on keydown
      wcInterm.hInstance     = hInst;             
      wcInterm.hIcon         = m_hIDictOff;
      wcInterm.hCursor       = LoadCursor(NULL, IDC_ARROW);
      wcInterm.hbrBackground = NULL;
      wcInterm.lpszMenuName  = MAKEINTRESOURCE(IDR_INTERM_LONGMENU);
      wcInterm.lpszClassName = gszDctPadClass;

		if (!RegisterClass(&wcInterm)) {
         ExceptionHandler(MSDICT_E_INITAPPLICATION, 2, __FILE__, __LINE__, 0);
			return FALSE;
      }
		
		if (!(ghInstRichEd = LoadLibrary("RICHED32.DLL"))) {
         ExceptionHandler(MSDICT_E_LOADRICHED32DLL, 2, __FILE__, __LINE__, 0);
			return FALSE;
      }

		m_pszBuf = (LPSTR)calloc(BUFFER, sizeof(TCHAR));
		if (!m_pszBuf) {
         ExceptionHandler(MSDICT_E_OUTOFMEM, 2, __FILE__, __LINE__, 0);
			return FALSE;
      }
	}

	DWORD dwcbData;
	HKEY hKey;

	dwcbData = sizeof(m_io);
   memset (&m_io, 0, sizeof(m_io));

   // default our intial settings
   RECT rcTaskBar;
   HWND hTaskBar = FindWindow(TEXT("Shell_TrayWnd"), NULL);

   GetWindowRect(hTaskBar, &rcTaskBar);

	if (rcTaskBar.left <= 0 && rcTaskBar.right < m_wXScreen)
		// taskbar is docked to left of screen
   	m_io.wLeft = (WORD)rcTaskBar.right;
   else
      m_io.wLeft = 0;

   //m_io.lf = m_lf;
   //m_io.wLeft = m_wXScreen / 4;
   m_io.wWidth = m_wXScreen / 2;
   m_io.wTop = m_wYScreen / 4;
   m_io.wHeight = m_wYScreen / 2;

   
   m_io.bHideCaption = FALSE;
   m_io.bHideToolBar = FALSE;
   m_io.fShow = FALSE;
   m_io.bOnTop = FALSE;
   m_io.showCmd = SW_HIDE;
   m_io.fSelectEntireWord = TRUE;

   // try to load in from regsitry
   // read the last position from registry
	LoadString(hInst, IDS_REGISTRY_SUBKEY, m_pszBuf, BUFFER-2);
   RegOpenKeyEx(HKEY_CURRENT_USER, m_pszBuf, 0, KEY_QUERY_VALUE, &hKey);
   if (hKey) {
   	if (ERROR_SUCCESS == RegQueryValueEx(hKey, (LPSTR)gszIntermOptions, 
	      NULL, NULL, (LPBYTE)&m_io, &dwcbData))
            gbRunningFirstTime = FALSE;
      RegCloseKey(hKey);
   }
   // sanity checks
   BOOL bSign = TRUE; // positive

   if (m_io.lf.lfHeight < 0) {
      m_io.lf.lfHeight = -m_io.lf.lfHeight;
      bSign = FALSE;
   }
   
   if (m_io.lf.lfHeight < 1)
      m_io.lf.lfHeight = 19;

   if (m_io.lf.lfHeight > 50)
      m_io.lf.lfHeight = 50;

   m_lFontHeight = m_io.lf.lfHeight;

   if (!bSign)
      m_io.lf.lfHeight = -m_io.lf.lfHeight;

   if (m_io.wWidth > m_wXScreen)
      m_io.wWidth = m_wXScreen;

   if (m_io.wHeight > m_wYScreen)
      m_io.wHeight = m_wYScreen;

   if (m_io.wLeft + m_io.wWidth > m_wXScreen)
      m_io.wLeft = m_wXScreen - m_io.wWidth;

   if (m_io.wTop + m_io.wHeight > m_wYScreen)
      m_io.wTop = m_wYScreen - m_io.wHeight;


   // initialize first time using the reg values from above
   if (bFirstTime) {
		bFirstTime = FALSE;

      // create the lexicon dialog
      hRes = CoCreateInstance (CLSID_STLexDlg, NULL, CLSCTX_ALL,
         IID_ISTLexDlg, (void**)&gpILexDlg);

      if (InitializeDictation(fMicWizCancel))
         goto fail;

      // Set the memory constraits to 10M, 10,000 words
      VDCTMEMORY  mem;
      memset (&mem, 0, sizeof(mem));
      mem.dwMaxRAM = 10000000;
      mem.dwMaxTime = 100000000; // ms
      mem.dwMaxWords = 10000;
      mem.fKeepAudio = TRUE;
      mem.fKeepCorrection = TRUE;
      mem.fKeepEval = TRUE;
      gpIVDctAttributes->MemorySet (&mem);
   }

   // min of 10 chars in the edit window

   m_wMinWidth = (WORD)(5*m_wCXFrame + 2*m_wBorder + 
					INTERM_BUTTONSIDE + 10*m_lFontHeight);
	m_wMinHeight = 4*m_wCYFrame + 2*m_wBorder + m_wCaption + m_wYMenu +
			(INTERM_BUTTONSIDE > (m_lFontHeight + 3*m_wCYFrame) ?
				INTERM_BUTTONSIDE : (m_lFontHeight + 3*m_wCYFrame));

   m_bHideToolBar = m_io.bHideToolBar;
   m_fSelectEntireWord = m_io.fSelectEntireWord;

   if (gbRunningFirstTime) {
    	m_lf.lfHeight = m_wCaption;
      m_lFontHeight = 19;
      m_lf.lfHeight = -19;
      m_lf.lfWeight = FW_NORMAL;  
      strcpy(m_lf.lfFaceName, "Times New Roman");
   } else
      CopyMemory(&m_lf, &(m_io.lf), sizeof(LOGFONT));

   m_hfSpoken = CreateFontIndirect(&m_lf);
   if (!m_hfSpoken)
	   return NULL;

   // change the text
   char  szTemp[128];
   if (AreWeDictating())
      LoadString(m_hInst, IDS_DCTPADLISTENING, szTemp, sizeof(szTemp));
   else
      LoadString(m_hInst, IDS_DCTPADNOTLISTENING, szTemp, sizeof(szTemp));
   
   m_hIntermWnd = CreateWindowEx(0, 
               TEXT("DEMODICTATIONPAD"), 
               szTemp,         
   			   WS_CLIPCHILDREN | WS_THICKFRAME	| WS_CAPTION | WS_SYSMENU | 
               WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
               CW_USEDEFAULT, CW_USEDEFAULT, 
	   			CW_USEDEFAULT, CW_USEDEFAULT,
               NULL,            
               m_hLongIntermMenu,            
               hInst,          
               NULL
        );

	if (!m_hIntermWnd) {
      ExceptionHandler(MSDICT_E_INITAPPLICATION, 2, __FILE__, __LINE__, 0);
		return NULL;
   }

#ifdef USENULLWINDOW
   hRes = gpIVoiceDictation->Activate(NULL);
#else
   hRes = gpIVoiceDictation->Activate(m_hIntermWnd);
#endif

   if (hRes)
      ExceptionHandler(MSDICT_E_VCMDACTIVATE, 2, __FILE__, __LINE__, hRes);

   // load the grammar with the commands from the resource
   HRSRC hRsrc;
   HGLOBAL hRMem;
   PCWSTR pMem;
   DWORD dwSize;
   hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_COMMANDS),
                      "TEXT");
   hRMem = LoadResource(NULL, hRsrc);
   pMem = (PWSTR)LockResource(hRMem);
   dwSize = SizeofResource(NULL, hRsrc);
   hRes = gpIVDctCommandsApp->FromMemory((void*) pMem, dwSize);
   FreeResource(hRsrc);

#ifdef USENULLWINDOW
   hRes = gpIVDctCommandsApp->Activate(NULL,0);
#else
   hRes = gpIVDctCommandsApp->Activate(m_hIntermWnd,0);
#endif


   // toolbar button
   memset((void*)m_tba, 0, 13*sizeof(TBBUTTON));

   #define TBASSIGN(x, a, b, c, d, e, f) \
      m_tba[x].iBitmap=a;\
      m_tba[x].idCommand=b;\
      m_tba[x].fsState=c;\
      m_tba[x].fsStyle=d;\
      m_tba[x].dwData=e;\
      m_tba[x].iString=f;

   // create the toolbar
   // when running first time dictation is off
   if (m_bDictation) {
      TBASSIGN(0, IDBT_TB_LISTENON, IDM_TB_INTERM_DICTLISTEN, TBSTATE_ENABLED, TBSTYLE_CHECK, 0, 0L);
   } else {
      TBASSIGN(0, IDBT_TB_LISTENOFF, IDM_TB_INTERM_DICTLISTEN, TBSTATE_ENABLED, TBSTYLE_CHECK, 0, 0L);
   }
//   TBASSIGN(1, IDBT_TB_CORRECTION, IDM_MSDICT_CORRECT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0L);
   TBASSIGN(1, IDBT_TB_CAPITALIZE, IDM_INTERM_CAPITALIZE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0L);
   TBASSIGN(2, IDBT_TB_ADDWORD, IDM_ADDWORD, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0L);
   TBASSIGN(3, 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0L);
   TBASSIGN(4, IDBT_TB_NEW, IDM_INTERM_FILENEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0L);
   TBASSIGN(5, IDBT_TB_OPEN, IDM_INTERM_FILEOPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0L);
   TBASSIGN(6, IDBT_TB_SAVEAS, IDM_INTERM_FILESAVEAS, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0L);
   TBASSIGN(7, 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0L);
   TBASSIGN(8, IDBT_TB_CUT, IDM_INTERM_EDITCUT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0L);
   TBASSIGN(9, IDBT_TB_COPY, IDM_INTERM_EDITCOPY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0L);
   TBASSIGN(10, IDBT_TB_PASTE, IDM_INTERM_EDITPASTE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0L);
   TBASSIGN(11, IDBT_TB_CUTALL, IDM_INTERM_EDITCUTALL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0L);

   m_hToolBar = CreateToolbarEx(m_hIntermWnd, WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS,
      IDC_INTERM_TOOLBAR, 12, m_hInst, IDB_INTERM_TOOLBAR, m_tba, 12,
      0, 0, 0, 0, sizeof(TBBUTTON));
   if (m_hToolBar) {
      RECT r;
      GetWindowRect(m_hToolBar, &r);
      m_wTBHeight = (WORD)(r.bottom - r.top);
      
      // make the DctPad atleast as wide as the tool bar (menu)
      // part of sanity check to avoid weird dimensions after crashes
      // which corrupt registry

      if (m_io.wWidth < NOTOOLBARBUTTONS*m_wTBHeight)
         MoveWindow(m_hIntermWnd, m_io.wLeft, m_io.wTop, NOTOOLBARBUTTONS*m_wTBHeight, 
	   			m_io.wHeight, TRUE);

      HWND hWndTT = (HWND)SendMessage(m_hToolBar, TB_GETTOOLTIPS, 0, 0);
      if (hWndTT)
         SendMessage(hWndTT, TTM_ACTIVATE, TRUE, 0);
   }
   else {
      m_wTBHeight = 0;
      ExceptionHandler(MSDICT_E_INITAPPLICATION, 1, __FILE__, __LINE__, 0);
   }

   // adjust the dimensions of interm wnd
   ToggleCaptionAndAdjust(m_bHideCaption, m_bHideToolBar);


   // dictation enable/disable
   if (::AreWeDictating()) {
      CheckMenuItem(m_hLongIntermMenu, IDM_INTERM_DICTLISTEN, 
            MF_BYCOMMAND|MF_CHECKED);
   } else {
      CheckMenuItem(m_hLongIntermMenu, IDM_INTERM_DICTNOTLISTEN, 
            MF_BYCOMMAND|MF_CHECKED);
   }
   

   // disable Edit - cut at startup
   EnableMenuItem(m_hLongIntermMenu, IDM_INTERM_EDITCUT, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
   EnableMenuItem(m_hLongIntermMenu, IDM_INTERM_EDITCOPY, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
   SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCUT, MAKELONG(FALSE, 0));

   // check the clipboard. If its empty disable the Paste
	OpenClipboard(NULL);

	hglbMem = GetClipboardData(CF_TEXT);

   if (hglbMem)
   	lpPaste = (LPSTR)GlobalLock(hglbMem);

   if (!hglbMem || (lpPaste && !strlen(lpPaste))) {
      EnableMenuItem(m_hLongIntermMenu, IDM_INTERM_EDITPASTE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
      SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITPASTE, MAKELONG(FALSE, 0));
	}

   if (hglbMem)
      GlobalUnlock(hglbMem);
	CloseClipboard();

   SendMessage(m_hTextWnd, WM_SETFONT, (WPARAM)m_hfSpoken, MAKELPARAM(TRUE, 0));

	m_OrigEditProc = (WNDPROC)SetWindowLong(m_hTextWnd, GWL_WNDPROC, (LONG)IntermEditSubClassProc);
   // set the Richedit ole call back which disables drag/drop
   SendMessage(m_hTextWnd, EM_SETOLECALLBACK, 0,(LPARAM)&gRichEditOleCallback);

   
   SendMessage(m_hTextWnd, EM_SETEVENTMASK, 0, ENM_CHANGE|ENM_UPDATE|ENM_SELCHANGE|ENM_SCROLL);

   // get the correction window handle
   ghCorrectionWnd = FindWindow("CORRECTCLS", NULL);

   if (gbRunningFirstTime) {
      hRes = gpIVDctAttributes->ModeSet(VSRMODE_DISABLED);
      m_dwEnabled = VSRMODE_DISABLED;
      if (hRes)
         ExceptionHandler(MSDICT_E_VCMDACTIVATE, 2, __FILE__, __LINE__, hRes);
   } else {

   }
   SetDictationIcon();

   // Since initialization usually takes a lot of memory,
   // allow the system to free up
   SetProcessWorkingSetSize (GetCurrentProcess(), (DWORD) -1, (DWORD) -1);

   LoadString(m_hInst, IDS_UNSAVEDFILE, m_szUntitled, sizeof(m_szUntitled) - 1);
   strcpy(m_szOpenFile, m_io.szDictFilesDir);
   strcat(m_szOpenFile, "\\");
   strcat(m_szOpenFile, m_szUntitled);

   // enable/disable the add word based upon whether the add-word
   // dialog claims to support
   BOOL  fSupport;
   fSupport = FALSE;
   if (gpILexDlg) {
      if (!gpILexDlg->LexDlg (NULL, NULL, STLD_TEST, 1033,
         gpIVDctAttributes, NULL, NULL, NULL, NULL))
         fSupport = TRUE;
   }
   if (!fSupport) {
      EnableMenuItem(m_hLongIntermMenu, IDM_ADDWORD, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
      SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_ADDWORD, MAKELONG(FALSE, 0));
	}

   gfDontNotify = FALSE;

   ShowWindow (m_hIntermWnd, SW_SHOW);

   return m_hIntermWnd;

fail:
   if (m_pszBuf) {
     free(m_pszBuf);
     m_pszBuf = NULL;
   }

   ReleaseDictation();

   return FALSE;
}


HRESULT InitializeDictation(BOOL fMicWizCancel)
{
   HRESULT hRes = NULL;
   DWORD dwException = MSDICT_E_LOADVCMD;

   hRes = CoCreateInstance(CLSID_VDct, NULL, CLSCTX_LOCAL_SERVER, IID_IVoiceDictation, (LPVOID *)&gpIVoiceDictation);
   if (hRes) {
      dwException = MSDICT_E_VCMDCREATEINSTANCE;
      goto failInitDictation;
   }

   // Get the text object
   hRes = gpIVoiceDictation->QueryInterface(
      IID_IVDctText, (LPVOID *)&gpIVDctText);
   if (hRes) {
      dwException = MSDICT_E_VCMDQUERY;
      goto failInitDictation;
   }

   hRes = gpIVoiceDictation->QueryInterface(
        IID_IVDctGUI, (LPVOID *)&gpIVDctGui);
   if (hRes) {
      dwException = MSDICT_E_VCMDQUERY;
      goto failInitDictation;
   }

   hRes = gpIVoiceDictation->QueryInterface(
        IID_IVDctDialogs, (LPVOID *)&gpIVDctDialogs);
   if (hRes) {
      dwException = MSDICT_E_VCMDQUERY;
      goto failInitDictation;
   }

   hRes = gpIVoiceDictation->QueryInterface(
        IID_IVDctAttributes, (LPVOID *)&gpIVDctAttributes);
   if (hRes) {
      dwException = MSDICT_E_VCMDQUERY;
      goto failInitDictation;
   }

   hRes = gpIVoiceDictation->QueryInterface(
        IID_IAttributes, (LPVOID *)&gpIAttributes);
   if (hRes) {
      dwException = MSDICT_E_VCMDQUERY;
      goto failInitDictation;
   }

   hRes = gpIVoiceDictation->QueryInterface(
        IID_IVDctCommandsApp, (LPVOID *)&gpIVDctCommandsApp);
   if (hRes) {
      dwException = MSDICT_E_VCMDQUERY;
      goto failInitDictation;
   }

   hRes = gpIVoiceDictation->QueryInterface(
         IID_ISRSpeaker, (LPVOID *)&gpIVDctSpeaker);
   if (hRes) {
      dwException = MSDICT_E_VCMDQUERY;
      goto failInitDictation;
   }
   
   hRes = gpIVoiceDictation->QueryInterface(
         IID_IVDctTextCache, (LPVOID *)&gpIVDctTextCache);
   if (hRes) {
      dwException = MSDICT_E_VCMDQUERY;
      goto failInitDictation;
   }
   
   hRes = gpIVoiceDictation->QueryInterface(
         IID_IVDctGlossary, (LPVOID *)&gpIVDctGlossary);
   if (hRes) {
      dwException = MSDICT_E_VCMDQUERY;
      goto failInitDictation;
   }
         
   hRes = gpIVoiceDictation->QueryInterface(
         IID_IVDctCommands, (LPVOID *)&gpIVDctCommands);
   if (hRes) {
      dwException = MSDICT_E_VCMDQUERY;
      goto failInitDictation;
   }

   DWORD dwCount;
   dwCount = 0;
retry:
   gpVDctNotifySink = new CIVDctNotifySink();
   if (!gpVDctNotifySink)
       goto failInitDictation;

   hRes = gpIVoiceDictation->Register(m_szAppName, NULL, NULL, 
         "", gpVDctNotifySink, IID_IVDctNotifySink, VCMDRF_ALLMESSAGES);
   if (hRes && (dwCount < 60)) {
      // failed, so wait a second & retry
      Sleep (1000);
      dwCount++;
      goto retry;
   }

   if (hRes) {
      dwException = MSDICT_E_VCMDREGISTER;
      goto failInitDictation;
   }


   return hRes;

failInitDictation:
   ExceptionHandler(dwException, 1, __FILE__, __LINE__, hRes);
   ReleaseDictation();
   return hRes;
}



// checks for and fixes differences between DctPad and internal buffer
//
// hWnd == hWnd of richedit in DctPad

HRESULT FixSynchProblem(HWND hWnd)
{
   char sz[256];
   DWORD dwStart, dwEnd;
   DWORD d1, d2, d3;
   TEXTRANGE tr;
   SDATA dData;
   HRESULT hRes;

   d3 = 0;

   hRes = gpIVDctText->Lock();
   if (hRes)
      return hRes;

   hRes = gpIVDctText->TextLengthGet(&d3);
   if (hRes) {
      gpIVDctText->UnLock();
      return hRes;
   }

   d2 = GetWindowTextLength(hWnd);
   if (d3 < d2)
      d3 = d2;

   if (d3) {
      ZeroMemory(sz, sizeof(sz));
      
      hRes = gpIVDctText->TextSelGet(&dwStart, &dwEnd);
      if (hRes) {
         gpIVDctText->UnLock();
         return hRes;
      }

      dwEnd += dwStart;

      // examine 127 chars before and after dwStart (total 255 chars)
      if (dwStart <= 127)
         d1 = 0;
      else
         d1 = dwStart - 127;

      if (dwStart + 128 > d3)
         d2 = d3;
      else
         d2 = dwStart + 127;

      tr.lpstrText = sz;
      tr.chrg.cpMin = d1;
      tr.chrg.cpMax = d2;

      SendMessage(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);

      hRes = gpIVDctText->TextGet(d1, d2 - d1, &dData);
      if (hRes) {
         gpIVDctText->UnLock();
         return hRes;
      }

      if (strcmp(sz, (LPSTR)(dData.pData))) {
         m_bProcessENSelChange = FALSE;
         SendMessage(hWnd, EM_SETSEL, d1, d2);
         SendMessage(hWnd, EM_REPLACESEL, FALSE, (LPARAM)(dData.pData));
         SendMessage(hWnd, EM_SETSEL, dwStart, dwEnd);
         m_bProcessENSelChange = TRUE;
      }

      gpIVDctText->UnLock();
      if (dData.pData)
         CoTaskMemFree(dData.pData);
   }
   else {
      // at least need to unlock
      gpIVDctText->UnLock();
   }

   return NOERROR;
}


void ReleaseDictation(void)
{
   HRESULT hRes;

   if (gpIVDctText) {
      DictTextRemove(0, 0x0fffffff, NULL);
      gpIVDctText->Release();
      gpIVDctText = NULL;
   }

   if (gpIVDctGui) {
      gpIVDctGui->Release();
     	gpIVDctGui = NULL;
   }

   if (gpIVDctDialogs) {
      gpIVDctDialogs->Release();
     	gpIVDctDialogs = NULL;
   }

   if (gpIVDctAttributes) {
      gpIVDctAttributes->Release();
     	gpIVDctAttributes = NULL;
   }

   if (gpIAttributes) {
      gpIAttributes->Release();
     	gpIAttributes = NULL;
   }

   if (gpILexDlg) {
      gpILexDlg->Release();
     	gpILexDlg = NULL;
   }

   if (gpIVDctCommandsApp) {
      gpIVDctCommandsApp->Release();
     	gpIVDctCommandsApp = NULL;
   }

   if (gpIVDctSpeaker) {
      gpIVDctSpeaker->Release();
      gpIVDctSpeaker = NULL;
   }

   if (gpIVDctTextCache) {
      gpIVDctTextCache->Release();
      gpIVDctTextCache = NULL;
   }

   if (gpIVDctGlossary) {
      gpIVDctGlossary->Release();
      gpIVDctGlossary = NULL;
   }

   if (gpIVDctCommands) {
      gpIVDctCommands->Release();
      gpIVDctCommands = NULL;
   }

   if (gpIVoiceDictation) {
      hRes = gpIVoiceDictation->Release();
      gpIVoiceDictation = NULL;
   }
}


/*****************************************************************************
*
*  ExitProc
*
*  Input
*     bMode == TRUE if windows is shutting down
*     bMode == FALSE otherwise
*
******************************************************************************/

BOOL ExitProc(BOOL bMode)
{
   HKEY hKey;
	DWORD dwcbData = sizeof(m_io);
   DWORD dwDisposition;

	// write to the registry
	LoadString(m_hInst, IDS_REGISTRY_SUBKEY, m_pszBuf, BUFFER-1);
	RegCreateKeyEx(HKEY_CURRENT_USER, 
		m_pszBuf, 0,
		NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
		NULL, &hKey, &dwDisposition);
	if (hKey) {
      WINDOWPLACEMENT wp;
      wp.length = sizeof(wp);
      m_io.fShow = m_hIntermWnd && IsWindowVisible(m_hIntermWnd);

      GetWindowPlacement(m_hIntermWnd, &wp);
   
      if (m_io.fShow) {
         // ShowWindow(m_hIntermWnd, SW_HIDE);
         m_io.wLeft = (WORD)wp.rcNormalPosition.left;
		   m_io.wTop = (WORD)wp.rcNormalPosition.top;
		   m_io.wWidth = wp.rcNormalPosition.right - wp.rcNormalPosition.left+1;
		   m_io.wHeight = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top+1;
      }

      m_io.fSelectEntireWord = m_fSelectEntireWord;
      // m_io.bHideCaption = m_bHideCaption;
      m_io.bHideToolBar = m_bHideToolBar;
      //m_io.lf = m_lf;
      CopyMemory(&(m_io.lf), &m_lf, sizeof(LOGFONT));
      m_io.showCmd = wp.showCmd;

      RegSetValueEx(hKey, (LPCTSTR)gszIntermOptions, 0, REG_BINARY, 
			(CONST BYTE*)&m_io, dwcbData);
      RegCloseKey (hKey);
	};

   if (m_pszBuf) {
		free(m_pszBuf);
		m_pszBuf = NULL;
	}

   return TRUE;
}


void SetTitle(void)
{
   char szTemp[256];

   LPSTR pFile = strrchr(m_szOpenFile, '\\');
   if (!pFile)
      pFile = m_szOpenFile;
   else
      pFile++;

   LoadString(m_hInst, m_bDictation ? IDS_DCTPADLISTENING : IDS_DCTPADNOTLISTENING, 
      szTemp, sizeof(szTemp));
   if (stricmp(pFile, m_szUntitled)) {
      strcat(szTemp, " - ");
      strcat(szTemp, pFile);
      if (4 < strlen(szTemp) && '.' == szTemp[strlen(szTemp) - 4])
         szTemp[strlen(szTemp) - 4] = 0;
   }
 	SetWindowText(m_hIntermWnd, szTemp);
}


HRESULT SetDictStatus(DWORD dwMode)
{
   HRESULT hRes = NULL;
   HCURSOR hCursorSave;

   hCursorSave = SetCursor(m_hWaitCursor);

   gfDontNotify = TRUE;

   hRes = gpIVDctAttributes->ModeSet(dwMode);
   m_dwEnabled = dwMode;
   if (hRes) {
      if ((hRes == VCMDERR_INVALIDMODE) && (dwMode == VSRMODE_CMDANDDCT)) {
         hRes = gpIVDctAttributes->ModeSet(VSRMODE_DCTONLY);
         m_dwEnabled = VSRMODE_DCTONLY;
         if (hRes)
            goto retSetDictStatus;
      }
      else
         goto retSetDictStatus;
   }

   if (dwMode == VSRMODE_DISABLED)
      FixSynchProblem(m_hTextWnd);

   // Set the dictation icon
   SetDictationIcon();

   // Since initialization usually takes a lot of memory,
   // allow the system to free up
   SetProcessWorkingSetSize (GetCurrentProcess(), (DWORD) -1, (DWORD) -1);


retSetDictStatus:
   SetCursor(hCursorSave);
   if (!hRes) {
      m_bDictation = (dwMode != VSRMODE_DISABLED);
      SetTitle();
   }

   if (0x800706BF == hRes)
      // vcmd has quit under us
      ExceptionHandler(MSDICT_E_VCMDQUIT, 2, __FILE__, __LINE__, hRes);

   if (hRes)
      ExceptionHandler(MSDICT_E_VCMDACTIVATE, 2, __FILE__, __LINE__, hRes);

   return hRes;
}


HRESULT GetCorrectionStatus(BOOL& b)
{
   DWORD dwFlags = 0;
   HRESULT hRes = 0;

   hRes = gpIVDctGui->FlagsGet(&dwFlags);
   b = dwFlags & VDCTGUIF_VISIBLE;

   return hRes;
}


HRESULT SetCorrectionStatus(BOOL b)
{
   DWORD dwFlags = 0;
   HRESULT hRes = 0;

   hRes = gpIVDctGui->FlagsGet(&dwFlags);
   if (hRes)
      goto retSetCorrectionStatus;

   if (b) {
      hRes = gpIVDctGui->FlagsSet(dwFlags | VDCTGUIF_VISIBLE);
      TellCorrectionOfSelection();
   } else
      hRes = gpIVDctGui->FlagsSet(dwFlags & (~VDCTGUIF_VISIBLE));

retSetCorrectionStatus:
   if (0x800706BF == hRes)
      // vcmd has quit under us
      ExceptionHandler(MSDICT_E_VCMDQUIT, 2, __FILE__, __LINE__, hRes);
   return hRes;
}


/******************************************************************
TellCorrectionOfSelection - This sends a message to the correction
   window telling it where the current selection is.

inputs
   none
returns
   none
*/

VOID TellCorrectionOfSelection(void)
{
   DWORD dwStart, dwEnd;

   if (!m_hIntermWnd || !IsWindowVisible(m_hIntermWnd))
      return;

   // tell the dictation object what text is visible
   RECT  r;
   HRESULT hRes;
   GetClientRect (m_hTextWnd, &r);
   POINT p;
   p.x = r.left;
   p.y = r.top;
   DWORD dwCharStart, dwCharEnd;
   dwCharStart = LOWORD(SendMessage (m_hTextWnd, EM_CHARFROMPOS, 0, (LPARAM) &p));
   p.x = r.right;
   p.y = r.bottom;
   dwCharEnd = LOWORD(SendMessage (m_hTextWnd, EM_CHARFROMPOS, 0, (LPARAM) &p));
   if (gpIAttributes) {
      hRes = gpIAttributes->DWORDSet (VDCTATTR_VISIBLETEXTSTART, dwCharStart);
      hRes = gpIAttributes->DWORDSet (VDCTATTR_VISIBLETEXTCHARS, dwCharEnd - dwCharStart);
   }


   // Don't cancel out if no correction window gui, beause dragon
   // has a hypothesis window that's shown
   // gpIVDctGui->FlagsGet(&dwStart);
   // if (!(dwStart & VDCTGUIF_VISIBLE))
   //   return;

   // If the window is minimized then don't move correction
   if (GetWindowStyle(m_hIntermWnd) & WS_MINIMIZE)
      return;

   // find the current selection
	SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM) &dwStart, (LPARAM)&dwEnd);

   // Ask for the curor position
   POINT pS, pE;
   SendMessage(m_hTextWnd, EM_POSFROMCHAR, (WPARAM) &pS, dwStart);
   ClientToScreen (m_hTextWnd, &pS);

   if (dwEnd != dwStart) {
      SendMessage(m_hTextWnd, EM_POSFROMCHAR, (WPARAM) &pE, dwEnd+1);
      ClientToScreen (m_hTextWnd, &pE);
   }
   else
      pE = pS;

   //put this in a rectangle
   r.left = pS.x;
   r.right = pE.x;
   r.top = pS.y;

   // if the caret is scrolled down correction window may go way below the
   // DctPad. So check
   RECT rSP;
   GetWindowRect(m_hTextWnd, &rSP);
   (r.top > rSP.bottom) ? (r.top = rSP.bottom) : NULL;

   r.bottom = r.top + m_lFontHeight;

   // send it down
   if (gpIVDctGui != NULL && 0x800706BF == gpIVDctGui->SetSelRect (&r)) {
      // vcmd has quit under us
      ExceptionHandler(MSDICT_E_VCMDQUIT, 2, __FILE__, __LINE__, 0x800706BF);
   }
}

/******************************************************************
DoMouseWheel - Accepts the mouse-wheel message and acts on it.

inputs
   WPARAM      wParam - If + or - then moving
   LPARAM      lParam - ignored for now
returns
   none
*/

VOID DoMouseWheel (WPARAM wParam, LPARAM lParam)
{
   // If the mouse wheel button is down then scroll up/down
   if (GetKeyState(VK_MBUTTON) < 0) {
      WORD  vk;
      vk = ((signed)wParam > 0) ? VK_UP : VK_DOWN;
      UINT uScanCode = MapVirtualKey(vk, 0);
      PostMessage(m_hTextWnd, WM_KEYDOWN, vk, (LPARAM)(1 | (uScanCode << 16)));
      PostMessage(m_hTextWnd, WM_KEYUP, vk, (LPARAM)(1 | (uScanCode << 16) | (3 << 30)));
      return;
   }

   DWORD dwStart, dwEnd;

   SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

   BOOL bDctPadEmpty = TRUE;

   if (GetWindowTextLength(m_hTextWnd))
      bDctPadEmpty = FALSE;
   if (bDctPadEmpty)
      return;

   if ((signed)wParam > 0) {
      DWORD dwNewStart = (DWORD)FindNextPrevAtom(dwStart, FALSE);
      SendMessage(m_hTextWnd, EM_SETSEL, dwNewStart, dwStart);
   } else {
      if (dwStart == (DWORD)GetWindowTextLength(m_hTextWnd) && dwEnd == dwStart)
         return;

      DWORD dwNewEnd = (DWORD)FindNextPrevAtom(dwEnd, TRUE);
      SendMessage(m_hTextWnd, EM_SETSEL, dwEnd, dwNewEnd);
   }

   TellCorrectionOfSelection();
   return;
}


BOOL AskAtClose(int) 
{
   DWORD d;

   //if (!(GetWindowTextLength(m_hTextWnd) && SendMessage(m_hTextWnd, EM_GETMODIFY, 0,0))) {
   //   CutAllToTheClipboard();
   //   return TRUE;
   //}

   LPSTR pFile = strrchr(m_szOpenFile, '\\');
   if (!pFile)
      pFile = m_szOpenFile;
   else
      pFile++;

   if (stricmp(pFile, m_szUntitled)) {
      // legit file
      char szTemp[256];

      if (m_fFileDirty) {
         char szTemp1[256];

         if (stricmp(pFile, m_szUntitled)) {
            LoadString(ghInst, IDS_SAVECURFILE, szTemp, sizeof(szTemp) - 1);
            sprintf(szTemp1, szTemp, pFile);
         } else
            LoadString(ghInst, IDS_SAVENEWFILE, szTemp1, sizeof(szTemp1) - 1);

         d = MessageBox(m_hIntermWnd, szTemp1, gszAppName, MB_YESNOCANCEL|MB_ICONQUESTION);
      
         if (IDYES == d)
            SendMessage(m_hIntermWnd, WM_COMMAND, IDM_INTERM_FILESAVE, 0);
         else if (IDCANCEL == d)
            return FALSE;
      }
      EmptyDctPad(VDCT_TEXTCLEAN);
      GetWindowText(m_hIntermWnd, szTemp, sizeof(szTemp) - 1);
      pFile = strchr(szTemp, '-');
      if (pFile)
         *--pFile = 0;

      SetWindowText(m_hIntermWnd, szTemp);

      strcpy(m_szOpenFile, m_io.szDictFilesDir);
      strcat(m_szOpenFile, "\\");
      strcat(m_szOpenFile, m_szUntitled);
      
      m_fFileNew = TRUE;
      m_fFileDirty = FALSE;

      return TRUE;
   }

   if (!GetWindowTextLength(m_hTextWnd))
      return TRUE;

   d = MessageBoxonTop(IDS_TRANSFERTEXT, MB_ICONQUESTION|MB_YESNOCANCEL|MB_TOPMOST|MB_APPLMODAL);
   switch(d) {
      case IDYES:
         CutAllToTheClipboard();
         return TRUE;

      case IDNO:
         EmptyDctPad(0);
         m_fFileDirty = FALSE;
         SendMessage(m_hIntermWnd, WM_COMMAND, IDM_INTERM_FILENEW, 0);
         return TRUE;

      case IDCANCEL:
         return FALSE;
   }

   return TRUE;
}


/******************************************************************************
*
*  MessageBoxonTop - Puts up a topmost messagebox
*
******************************************************************************/

int MessageBoxonTop(DWORD dwIDText, DWORD dwStyle)
{
   MSGBOXPARAMS msgBoxParams;
	memset((void*)&msgBoxParams, 0, sizeof(MSGBOXPARAMS));
   msgBoxParams.hwndOwner = m_hIntermWnd;
	msgBoxParams.cbSize = sizeof(MSGBOXPARAMS);
	msgBoxParams.hInstance = m_hInst;

	LoadString(m_hInst, dwIDText, m_pszBuf, BUFFER - 2);
	msgBoxParams.lpszText = m_pszBuf;
	msgBoxParams.lpszCaption = m_szAppName;
	msgBoxParams.dwStyle = dwStyle;
	msgBoxParams.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);

   return MessageBoxIndirect(&msgBoxParams);
}


/******************************************************************************
*
* IsSpoken
*
* returns true if the word at dwPoint is spoken, else returns FALSE
*
******************************************************************************/

BOOL IsSpoken(DWORD dwPoint, 
                                DWORD* pdwOutLeft, 
                                DWORD* pdwOutRight)
{
   LPUNKNOWN lpUnknown = NULL;
   DWORD dwLeft, dwRight;

   gpIVDctText->ResultsGet(dwPoint, NULL, &dwLeft, &dwRight, &lpUnknown);
   if (lpUnknown) {
      // spoken word
      lpUnknown->Release();
      if (pdwOutLeft)
         *pdwOutLeft = dwLeft;
      if (pdwOutRight)
         *pdwOutRight = dwRight;
      return TRUE;
   }
   else
      return FALSE;
}


/******************************************************************************
*
* FindNextPrevAtom - Called only in word mode
* finds the next word or character (depending on the word to be spoken or
* typed) to the right or left (bRightLeft) and returns the position.
*
******************************************************************************/

int FindNextPrevAtom(DWORD dwPoint, 
                                       BOOL bRightLeft)
{
   DWORD dwSpeakStart, dwSpeakEnd;

   if (!m_fSelectEntireWord)
      return -1;

   DWORD dwLength = GetWindowTextLength(m_hTextWnd);

   // EM_FINDWORDBREAK (with WB_MOVEWORDLEFT and WB_MOVEWORDRIGHT) is a
   // finer grain for word manouver than ResultsGet For ex: sometimes the
   // engine passes up A. which is treated by ResultsGet as single word but
   // EM_FINDWORDBREAK would find two words in it.
   // exception : when more than one punctation follow they are treated as one

   if (bRightLeft) { // right
      if (dwPoint >= (DWORD)GetWindowTextLength(m_hTextWnd))
         return -1;
      
      // if we are on word spacing

      if (SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_ISDELIMITER, dwPoint)) {
         // check if the next word is spoken if so jump to it or just go char at a time
         DWORD dwNext = SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_MOVEWORDRIGHT, dwPoint);
         if (IsSpoken(dwNext, NULL, NULL))
            return dwNext;
         else
            return (dwPoint+1 <= dwLength ? dwPoint+1 : dwLength);
      }

      if (IsSpoken(dwPoint, &dwSpeakStart, &dwSpeakEnd)) {
         if (SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_ISDELIMITER, dwSpeakStart + dwSpeakEnd))
            return SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_MOVEWORDRIGHT, dwSpeakStart + dwSpeakEnd);
         else
            return dwSpeakStart + dwSpeakEnd;
      }

      // typed word
      // if the next characters are \r\n then jump over them
      SDATA dData;
      HRESULT hRes;
      if (dwPoint + 2 < dwLength && !gpIVDctText->Lock()) {
         hRes = gpIVDctText->TextGet(dwPoint, 2, &dData);

         if (!hRes && dData.pData) {
            if (((LPSTR)(dData.pData))[0] == '\r' && ((LPSTR)(dData.pData))[1] == '\n') {
               CoTaskMemFree(dData.pData);
               gpIVDctText->UnLock();
               return dwPoint+2;
            }
            CoTaskMemFree(dData.pData);
         }
         gpIVDctText->UnLock();
      }

      return (dwPoint+1 <= dwLength ? dwPoint+1 : dwLength);
   }

   // left

   if (!dwPoint)
      return 0;

   // check if the first word starting to the left of dwPoint is spoken
   // if yes jump to it if not move char at a time
   DWORD dwPrev;
   if (!SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_ISDELIMITER, dwPoint-1))
      dwPrev = dwPoint - 1;
   else {
      dwPrev = SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_LEFTBREAK, dwPoint);
      if (dwPrev)
         dwPrev--;
   }

   if (IsSpoken(dwPrev, &dwSpeakStart, &dwSpeakEnd))
      return dwSpeakStart;

   // typed word
   // if the previous characters are \r\n then jump over them
   SDATA dData;
   HRESULT hRes;
   if (dwPoint-2 && !gpIVDctText->Lock()) {
      hRes = gpIVDctText->TextGet(dwPoint-2, 2, &dData);

      if (!hRes && dData.pData) {
         if (((LPSTR)(dData.pData))[0] == '\r' && ((LPSTR)(dData.pData))[1] == '\n') {
            CoTaskMemFree(dData.pData);
            gpIVDctText->UnLock();

            return dwPoint-2;
         }
         CoTaskMemFree(dData.pData);
      }
      gpIVDctText->UnLock();
   }

   return dwPoint-1 >= 0 ? dwPoint-1 : 0;
}
      

/******************************************************************************
* FindWordBoundary - Given a point in the text, this finds the left
*   and right boundaries that select the entire word it's on.
*
* inputs
*   DWORD    dwPoint - Point
*   DWORD    *pdwLeft - Filled in with the left pointer
*   DWORD    *pdwRight - Filled in with the right pointer
* returns
*   none
******************************************************************************/

void FindWordBoundary (DWORD dwPoint,
                                         DWORD *pdwLeft,
                                         DWORD *pdwRight)
{
   LPUNKNOWN lpUnknown = NULL;

   gpIVDctText->ResultsGet(dwPoint, NULL, pdwLeft, pdwRight, &lpUnknown);
   if (lpUnknown) {
      // spoken word
      lpUnknown->Release();
      *pdwRight += *pdwLeft;
   }
   else {
      *pdwLeft = dwPoint;
      *pdwRight = dwPoint;
   }

}


/******************************************************************************
*
* AdjustSelection - This adjust the selection
*   so that it surrounds the highlighted word. It should only be called
*   if m_fSelectEntire word is set.

  returns TRUE if sent another EN_SELCHANGE message, FALSE if didn't
*
******************************************************************************/

BOOL AdjustSelection (LPDWORD lpdwStart, LPDWORD lpdwEnd)
{
   // get the selection
   DWORD  dwStart, dwEnd;
   DWORD dwLeft, dwRight;
   BOOL  fReturn = FALSE;
   SendMessage (m_hTextWnd, EM_GETSEL, (WPARAM) &dwStart, (LPARAM) &dwEnd);

   // see what we'd select around for both the left and
   // right edges, and adjust around
   DWORD dwTemp;
   dwLeft = dwStart;
   dwRight = dwEnd;
   if (!CheckforBoundary(dwStart)) {
      FindWordBoundary (dwStart, &dwLeft, &dwTemp);
      if (dwTemp < dwEnd) {
         if (!CheckforBoundary(dwEnd))
            FindWordBoundary (dwEnd, &dwTemp, &dwRight);
      }
      else
         dwRight = dwTemp;
   }
   else {
      if (dwStart == dwEnd)
         goto RETURNADJUST;

      if (!CheckforBoundary(dwEnd))
         FindWordBoundary (dwEnd, &dwTemp, &dwRight);
   }

   m_bProcessENSelChange = FALSE;
   if ((0xffffffff != gdwlButtondownPos && dwStart < gdwlButtondownPos) || gfKeyUp) {
      if ((dwRight != dwStart) || (dwLeft != dwEnd)) {
         SendMessage (m_hTextWnd, EM_SETSEL, dwRight, dwLeft);
         fReturn = TRUE;
      }
   }
   else {
      if ((dwLeft != dwStart) || (dwRight != dwEnd)) {
         SendMessage (m_hTextWnd, EM_SETSEL, dwLeft, dwRight);
         fReturn = TRUE;
      }
   }

RETURNADJUST:   
   gfKeyUp = FALSE;
   gdwlButtondownPos = 0xffffffff;
   m_bProcessENSelChange = TRUE;

   *lpdwStart = dwLeft;
   *lpdwEnd = dwRight;

   return fReturn;
}


/******************************************************************************
*
* ToggleCaptionAndAdjust
*
* Depending on the value of b this function hides the caption, menu
* and adjusts the sizes of text window and also recalculates the min and 
* max height/length values
*
******************************************************************************/

WORD gwaIntermParams[5]; // caption height, menu height, TB height, 
                     // X position of textwnd (client coord), borders

void ToggleCaptionAndAdjust(BOOL fIn, BOOL bToolBar)
{
   DWORD dwStyle = GetWindowLong(m_hIntermWnd, GWL_STYLE);

   RECT r;
   GetWindowRect(m_hIntermWnd, &r);
   int nHeightAdjust = r.bottom - r.top + 1;
   int nWidthAdjust = r.right - r.left + 1;

   if (!m_bHideCaption) {
      gwaIntermParams[0] = m_wCaption;
      gwaIntermParams[1] = m_wYMenu;
      gwaIntermParams[3] = m_wCXFrame;
      gwaIntermParams[4] = 2*m_wBorder;
      nHeightAdjust -= gwaIntermParams[0] + gwaIntermParams[1] + gwaIntermParams[4];
      nWidthAdjust -= gwaIntermParams[4];
   }
   else {
      // caption and menu are hidden
      gwaIntermParams[0] = 0;
      gwaIntermParams[1] = 0;
      gwaIntermParams[3] = 2*m_wCXFrame + INTERM_BUTTONSIDE;
      gwaIntermParams[4] = 0;
   }

   if (m_bHideToolBar)
      // toolbar is hidden
      gwaIntermParams[2] = 0;
   else {
      gwaIntermParams[2] = m_wTBHeight;
      nHeightAdjust -= gwaIntermParams[2];
   }

   dwStyle |= (WS_CAPTION|WS_BORDER);
   SetWindowLong(m_hIntermWnd, GWL_STYLE, dwStyle);
   SetMenu(m_hIntermWnd, m_hLongIntermMenu);
   gwaIntermParams[0] = m_wCaption;
   gwaIntermParams[1] = m_wYMenu;
   gwaIntermParams[3] = m_wCXFrame;
   gwaIntermParams[4] = 2*m_wBorder;
   m_bHideCaption = FALSE;

   if (bToolBar) {
      // hide toolbar
      ShowWindow(m_hToolBar, SW_HIDE);
      gwaIntermParams[2] = 0;
      m_bHideToolBar = TRUE;
   }
   else {
      // show toolbar
      ShowWindow(m_hToolBar, SW_SHOW);
      gwaIntermParams[2] = m_wTBHeight;
      m_bHideToolBar = FALSE;
   }

   m_wMinWidth = (WORD)(3*m_wCXFrame + gwaIntermParams[3] + 10*m_lFontHeight);
   m_wMinHeight = 4*m_wCYFrame + gwaIntermParams[0] + gwaIntermParams[1] + 
                  gwaIntermParams[2] + gwaIntermParams[4] +
                  (INTERM_BUTTONSIDE > (m_lFontHeight + 3*m_wCYFrame) ?
	                INTERM_BUTTONSIDE : (m_lFontHeight + 3*m_wCYFrame));

   if (m_bFontChanged) {
      if (nHeightAdjust - 4*m_wCYFrame < m_lFontHeight + 3*m_wCXFrame)
         nHeightAdjust = m_lFontHeight + 3*m_wCXFrame + 4*m_wCYFrame;
      m_bFontChanged = FALSE;
   }

   nHeightAdjust += gwaIntermParams[0] + gwaIntermParams[1] + 
                    gwaIntermParams[4] + gwaIntermParams[2];
   nWidthAdjust += gwaIntermParams[4];

   MoveWindow(m_hIntermWnd, r.left, r.top, nWidthAdjust, nHeightAdjust, TRUE);

   GetClientRect(m_hIntermWnd, &r);
   MoveWindow(m_hTextWnd, gwaIntermParams[3], gwaIntermParams[2] + m_wCYFrame,
      r.right - r.left - m_wCXFrame - gwaIntermParams[3],
      r.bottom - r.top - gwaIntermParams[2] - 2*m_wCYFrame, TRUE);

   GetWindowRect(m_hIntermWnd, &r);
   MoveWindow(m_hIntermWnd, r.left, r.top, r.right - r.left,
      r.bottom - r.top + 1, TRUE);
   MoveWindow(m_hIntermWnd, r.left, r.top, r.right - r.left,
      r.bottom - r.top - 1, TRUE);
   return;
}


/******************************************************************************
*
* CheckforBoundary
*
* Returns true if dwPos is before after or in between the words, else returns FALSE
*
******************************************************************************/

BOOL CheckforBoundary(DWORD dwPos)
{
   DWORD dwLine = SendMessage(m_hTextWnd, EM_LINEFROMCHAR, dwPos, 0);

   if ((dwPos == (DWORD)SendMessage(m_hTextWnd, EM_LINEINDEX, dwLine, 0) || // is caret at start of line
      SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_ISDELIMITER, dwPos-1) || // at start of word
      SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_ISDELIMITER, dwPos) || // at end of word
      dwPos == (DWORD)GetWindowTextLength(m_hTextWnd))) // at end of all text
      return TRUE;

   SDATA dData;
   BOOL f = FALSE;

   // is dwPos at end of a line before \r\n ?
   if (!gpIVDctText->Lock()) {
      if (!(gpIVDctText->TextGet(dwPos, 2, &dData)) && dData.pData) {
         if (((LPSTR)(dData.pData))[0] == '\r' && ((LPSTR)(dData.pData))[1] == '\n')
            f = TRUE;
         CoTaskMemFree(dData.pData);
      }
      gpIVDctText->UnLock();
   }

   return f;
}


/******************************************************************************
*
* DictTextSet
*
* Wrapper for TextSet
*
******************************************************************************/

HRESULT DictTextSet(DWORD dwStart, 
                                      DWORD dwNumChars, 
                                      LPSTR pszBuf, 
                                      DWORD dwReason)
{
   HRESULT hRes = gpIVDctText->Lock();

   if (NOERROR == hRes) {
      gpIVDctText->TextSet(dwStart, dwNumChars, pszBuf, dwReason);
      gpIVDctText->UnLock();
   }

   return hRes;
}


/******************************************************************************
*
* DictTextSelSet
*
* Wrapper for TextSelSet
*
******************************************************************************/

HRESULT DictTextSelSet(DWORD dwStart, 
                                         DWORD dwNumChars)
{
   HRESULT hRes = gpIVDctText->Lock();

   if (NOERROR == hRes) {
      gpIVDctText->TextSelSet (dwStart, dwNumChars);
      gpIVDctText->UnLock();
   }

   return hRes;
}


/******************************************************************************
*
* DictTextRemove
*
* Wrapper for TextRemove
*
******************************************************************************/

HRESULT DictTextRemove(DWORD dwStart, 
                                         DWORD dwNumChars, 
                                         DWORD dwReason)
{
   HRESULT hRes = gpIVDctText->Lock();

   if (NOERROR == hRes) {
      gpIVDctText->TextRemove(dwStart, dwNumChars, dwReason);
      gpIVDctText->UnLock();
   }

   SetUndoTimer();

   return hRes;
}


/******************************************************************************
*
* TransmitTypedLetter
*
* Transmits what has been typed to the vDict internal buffer
*
******************************************************************************/

HRESULT TransmitTypedLetter(WORD wChar,
                                              DWORD dwStart, 
                                              DWORD dwEnd)
{
   char szText[2];
   szText[0] = (char)wChar;
   szText[1] = 0;

   // delete selection if any
   if (dwEnd > dwStart)
      DictTextRemove(dwStart, dwEnd - dwStart, 0);

   // remove this
   DictTextSelSet(dwStart, 0);

   SetUndoTimer();

   return DictTextSet(dwStart, 0, szText, 0);
}


/******************************************************************************
*
* EmptyDctPad - Deletes the text in DctPad
*
******************************************************************************/

HRESULT EmptyDctPad(DWORD dwFlags)
{
   m_bProcessENSelChange = FALSE;
   SetWindowText(m_hTextWnd, NULL);
   m_bProcessENSelChange = TRUE;
   TellCorrectionOfSelection();
   SendMessage(m_hTextWnd, EM_SETMODIFY, FALSE, 0);
   gfUndoHasData[0] = gfUndoHasData[1] = FALSE;
   gdwUndoLatest = 0;
   UndoRemember ();
   return DictTextRemove(0, 0x0FFFFFFF, dwFlags);
}


/******************************************************************************
*
* CIntermedaiteWnd::CopyText - Copies only text (no RTF) to the
*  clipboard. Also have CutText(), and PasteText()
*
******************************************************************************/

void CopyText()
{
   if (!OpenClipboard(m_hIntermWnd))
	   return;
   EmptyClipboard();

   DWORD dwStart, dwEnd;

   SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
   if (dwStart >= dwEnd) {
	   CloseClipboard();
	   return;
   }

   // if we're copying it all then turn off the modify
   BOOL  fCopyAll;
   fCopyAll = (dwStart == 0) && ((int) dwEnd >= GetWindowTextLength(m_hTextWnd));
   if (fCopyAll)
      SendMessage (m_hTextWnd, EM_SETMODIFY, FALSE, 0);

   HGLOBAL hglbMem;
   TCHAR   *lpstrCopy;
   hglbMem = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, 
	   (dwEnd - dwStart + 2)*sizeof(TCHAR));
   if (!hglbMem) {
	   CloseClipboard();
	   return;
   }

   lpstrCopy = (LPSTR)GlobalLock(hglbMem);
   SendMessage(m_hTextWnd, EM_GETSELTEXT, 0, (LPARAM)lpstrCopy);
   lpstrCopy[dwEnd - dwStart + 1] = '\0';
   SetClipboardData(CF_TEXT, hglbMem);
   CloseClipboard();

}


/******************************************************************************
*
* CutText - Cuts the selection
*
******************************************************************************/

void CutText(DWORD dwFlags)
{
   // save undo information
   SaveCurrentToUndo();
   SetUndoTimer();

   CopyText();

   DWORD dwStart, dwEnd;
   SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
   // if we're copying it all then turn off the modify
   BOOL  fCopyAll;
   fCopyAll = (dwStart == 0) && ((int)dwEnd >= GetWindowTextLength(m_hTextWnd));

   if (dwEnd > dwStart) {
      DictTextRemove(dwStart, dwEnd - dwStart, dwFlags);
      TellCorrectionOfSelection();
   }
   SendMessage(m_hTextWnd, EM_REPLACESEL, TRUE, (LPARAM)"");

   if (fCopyAll)
      SendMessage (m_hTextWnd, EM_SETMODIFY, FALSE, 0);

}


/******************************************************************************
*
* PasteText - Pastes the selection
*
******************************************************************************/

void PasteText()
{
   if (!IsClipboardFormatAvailable(CF_TEXT))
	   return;
   if (!OpenClipboard(m_hIntermWnd))
	   return;

   // save undo information
   SaveCurrentToUndo();
   SetUndoTimer();

   HGLOBAL hglbMem;
   TCHAR   *lpstrCopy;
   hglbMem = GetClipboardData(CF_TEXT);
   if (!hglbMem) {
	   CloseClipboard();
	   return;
   }

   lpstrCopy = (LPSTR)GlobalLock(hglbMem);
   if (!lpstrCopy) {
	   CloseClipboard();
	   return;
   }

   DWORD dwStart, dwEnd;
   SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
   SendMessage(m_hTextWnd, EM_REPLACESEL, TRUE, (LPARAM)lpstrCopy);

   if (hglbMem && lpstrCopy) {
      // remove this
      DictTextSelSet(dwStart, dwEnd - dwStart);
      DictTextSet(dwStart, dwEnd-dwStart, lpstrCopy, NULL);
      TellCorrectionOfSelection();
   }

   if (hglbMem)
      GlobalUnlock(hglbMem);


   CloseClipboard();
}

/********************************************************************
CopyAllToTheClipboard - Copys all of the text onto the clipboard.
*/
void CopyAllToTheClipboard(void)
{
   // If the correction window is visible temporarily hide it
   DWORD dwFlags;
   gpIVDctGui->FlagsGet(&dwFlags);
   if (dwFlags & VDCTGUIF_VISIBLE)
      gpIVDctGui->FlagsSet(dwFlags & (~VDCTGUIF_VISIBLE));


   DWORD dwStart, dwEnd;
   SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
   SendMessage(m_hTextWnd, EM_SETSEL, 0, (LPARAM)-1);
   SendMessage(m_hTextWnd, WM_COPY, 0, 0);
   SendMessage(m_hTextWnd, EM_SETSEL, dwStart, dwEnd);

   // Undo the hide
   if (dwFlags & VDCTGUIF_VISIBLE)
      gpIVDctGui->FlagsSet(dwFlags);
}

/********************************************************************
CutAllToTheClipboard - Cuts all of the text onto the clipboard.
*/
void CutAllToTheClipboard(void)
{

   // If the correction window is visible temporarily hide it
   DWORD dwFlags;
   gpIVDctGui->FlagsGet(&dwFlags);
   if (dwFlags & VDCTGUIF_VISIBLE)
      gpIVDctGui->FlagsSet(dwFlags & (~VDCTGUIF_VISIBLE));

   // we gwt here only if there's a selection
   SendMessage(m_hTextWnd, EM_SETSEL, 0, (LPARAM)-1);
   SendMessage(m_hTextWnd, WM_CUT, VDCT_TEXTCLEAN, 0);

   // no more selection so disable cut
   SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCUT, MAKELONG(FALSE, 0));
   SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCOPY, MAKELONG(FALSE, 0));
   // enable paste
   SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITPASTE, MAKELONG(TRUE, 0));

   // Undo the hide
   if (dwFlags & VDCTGUIF_VISIBLE)
      gpIVDctGui->FlagsSet(dwFlags);

   // go back to 'Untitled' document
   m_fFileDirty = FALSE;
   // SendMessage(m_hIntermWnd, WM_COMMAND, IDM_INTERM_FILENEW, 0);
}

/*******************************************************************8
SendCtrlVToApp - Sends a control-V to the specified window.
*/
void SendCtrlVToApp (char *pszTitle)
{
   HWND hWndStart = NULL;

   // Find the window
   hWndStart = GetWindow(GetDesktopWindow(), GW_CHILD);
   if (!hWndStart)
      return;

   // loop through
   for (;;hWndStart = ::GetWindow(hWndStart, GW_HWNDNEXT))
   {
      // If we've reached the end of the z-order, this means we
      // have no visible windows.
      if (!hWndStart)
         return;

      if (!_IsWindowOK(hWndStart))
         continue;

      // See if the strings compare
      TCHAR    szTemp[256];
      GetWindowText (hWndStart, szTemp, sizeof(szTemp)/sizeof(TCHAR));

      if (!strcmp(szTemp, pszTitle))
         break;
   }

   // Switch to the window
   if (IsIconic(hWndStart))
      ShowWindow(hWndStart, SW_RESTORE);
   SetForegroundWindow (hWndStart);

   // Send ctrl-V
   BYTE bVK = (BYTE) VkKeyScan ('V');
   keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), 0, 0);
   keybd_event(bVK, MapVirtualKey(bVK, 0), 0, 0);
   keybd_event(bVK, MapVirtualKey(bVK, 0), KEYEVENTF_KEYUP, 0);
   keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), KEYEVENTF_KEYUP, 0);

   return;
}

LRESULT About(HWND hDlg,
                       UINT message,        
                       WPARAM uParam,       
                       LPARAM lParam)
{
   // change this
   LPSTR   lpVersion;
   DWORD   dwVerInfoSize;
   DWORD   dwVerHnd;
   UINT    uVersionLen;
   WORD    wRootLen;
   BOOL    bRetCode;
   char    szFullPath[256];
   char    szResult[256];
   char    szGetName[256];

   switch (message) {
           case WM_INITDIALOG:  

                   // Get version information from the application
                   GetModuleFileName (ghInst, szFullPath, sizeof(szFullPath));

                   dwVerInfoSize = 0;

                   if (!ghVersionDll)
                      ghVersionDll = LoadLibrary ("version.dll");

                   if (ghVersionDll) {
                      typedef BOOL (APIENTRY GETFILEVERSIONINFOSIZE) (LPSTR, LPDWORD);
                      typedef GETFILEVERSIONINFOSIZE FAR* PGETFILEVERSIONINFOSIZE;
                      PGETFILEVERSIONINFOSIZE pGetFileVersionInfoSize = 
                         (PGETFILEVERSIONINFOSIZE)GetProcAddress(ghVersionDll, 
                           "GetFileVersionInfoSizeA");
                      if (pGetFileVersionInfoSize)
                        dwVerInfoSize = pGetFileVersionInfoSize(szFullPath, &dwVerHnd);
                   }
                   
                   if (dwVerInfoSize) {
                           // If we were able to get the information, process it:
                           LPSTR   lpstrVffInfo;
                           HANDLE  hMem;
                           hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
                           lpstrVffInfo  = (char*)GlobalLock(hMem);

                           if (ghVersionDll) {
                              typedef BOOL (APIENTRY GETFILEVERSIONINFO) (LPSTR,
                                 DWORD, DWORD, LPVOID);
                              typedef GETFILEVERSIONINFO FAR* PGETFILEVERSIONINFO;
                              PGETFILEVERSIONINFO pGetFileVersionInfo = 
                                 (PGETFILEVERSIONINFO)GetProcAddress(ghVersionDll,
                                    "GetFileVersionInfoA");
                              if (pGetFileVersionInfo)
                                 pGetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpstrVffInfo);
                           }

                           lstrcpy(szGetName, "\\StringFileInfo\\040904B0\\ProductVersion");
                           wRootLen = lstrlen(szGetName);

                           // Walk through the dialog items that we want to replace:
                           uVersionLen   = 0;
                           lpVersion     = NULL;
                           bRetCode      =  0;

                           if (ghVersionDll) {
                             typedef BOOL (APIENTRY VERQUERYVALUE) (const LPVOID,
                                LPSTR, LPVOID*, PUINT);
                             typedef VERQUERYVALUE FAR* PVERQUERYVALUE;
                             PVERQUERYVALUE pVerQueryValue = 
                                (PVERQUERYVALUE)GetProcAddress(ghVersionDll,
                                 "VerQueryValueA");
                             if (pVerQueryValue)
                                bRetCode = pVerQueryValue((LPVOID)lpstrVffInfo,
                                   (LPSTR)szGetName,
                                   (void**)&lpVersion,
                                   (unsigned int*)&uVersionLen); // For MIPS strictness
                           }

                           if ( bRetCode && uVersionLen && lpVersion) {
                                  // Replace dialog item text with version info
                                  lstrcpy(szResult, lpVersion);
                                  SetDlgItemText(hDlg, IDC_PRODUCTVERSION, szResult);
                           }

                           GlobalUnlock(hMem);
                           GlobalFree(hMem);
                   } // if (dwVerInfoSize)
                   return (TRUE);

           case WM_COMMAND:                      // message: received a command
                   if (LOWORD(uParam) == IDOK        // "OK" box selected?
                   || LOWORD(uParam) == IDCANCEL) {  // System menu close command?
                           EndDialog(hDlg, TRUE);        // Exit the dialog
                           return (TRUE);
                   }
                   break;
   }
   return (FALSE); // Didn't process the message

   lParam; // This will prevent 'unused formal parameter' warnings
}

/**************************************************************
GetChangesFromVDct - Get changes from the voice dictation object
   and display them. This includes the text and selection.
*/
void GetChangesFromVDct()
{
   HRESULT hr;
   DWORD   dwStart, dwEnd, dwOldStart, dwOldEnd;
   SDATA   dData;
   static  WORD nCount = 0;
   char    c = 0;

	// Called when text changes in the VTB
   // lock the text buffer so we know it doesn't change on us
   if (!gpIVDctText)
      return;

	hr = gpIVDctText->Lock();
   if (NOERROR != hr)
      return;

   // find out what has changed in the buffer
   hr = gpIVDctText->GetChanges(&dwStart, &dwEnd, &dwOldStart, &dwOldEnd);
   if (hr != NOERROR)
      goto getselchanges;

   // set a timer so that we store the undo information
   if (gfTimerSaveUndo)
      KillTimer (m_hIntermWnd, TIMER_SAVEUNDO);
   gfTimerSaveUndo = FALSE;
   if (!gfProcessingUndo && ((dwStart != dwEnd) || (dwOldStart != dwOldEnd))) {
      SetTimer (m_hIntermWnd, TIMER_SAVEUNDO, 5000 /* 5 sec*/, 0);
      gfTimerSaveUndo = TRUE;
   }


   ZeroMemory(&dData, sizeof(SDATA));

   // dictating to interm wnd (which may be transferred to an app)

   // reset the rank for VK_ESCAPE processing
   if (gfRank)
      gfRank = FALSE;
   else
      m_dwRank = 1;

   
   // allocate if the # of characters is less than 256, then
   // write code to do a diff on the text, ensuring that we
   // change the minimum amount of text
#define  TEXTCOMP    256
   char  szTemp[TEXTCOMP+1];
   BOOL  fComp;
   fComp = FALSE;
   if ((dwOldEnd != dwOldStart) && ((dwOldEnd - dwOldStart) < TEXTCOMP)) {
      TEXTRANGE   tr;
      tr.lpstrText = szTemp;
      tr.chrg.cpMin = dwOldStart;
      tr.chrg.cpMax = dwOldEnd;
      SendMessage(m_hTextWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
      fComp = TRUE;
   }

   // get the text changes
   dData.pData = NULL;
   if (dwEnd != dwStart) {
      hr = gpIVDctText->TextGet(dwStart, dwEnd-dwStart, &dData);
   }

   // see how far we can get before the characters are different
   // don't update the buffer if the text is the same
   DWORD i;
   BOOL  fNoChanges;
   fNoChanges = TRUE;
   i = 0;
   if (dData.pData) {
      if (!fComp) {
         fNoChanges = FALSE;
      }
      else
         for (i = 0; (i < (dwEnd - dwStart)) && (i < (dwOldEnd - dwOldStart)); i++)
            if ( ((char*)dData.pData)[i] != szTemp[i]) {
               fNoChanges = FALSE;
               break;
            }

   }

   // if got to the end and didn't find any changes, then we
   // should make sure that the buffers were of identical lenght.
   // If not, then there were changes, but they occurred after
   // the crrent buffer
   if (fNoChanges)
      fNoChanges = ((dwEnd - dwStart) == (dwOldEnd - dwOldStart));

   // put the text in the edit box
   m_bProcessENSelChange = FALSE;
   
   if (!fNoChanges) {
      SendMessage(m_hTextWnd, EM_SETSEL, (WPARAM)dwOldStart + i, (LPARAM)dwOldEnd);
      SendMessage(m_hTextWnd, EM_REPLACESEL, TRUE,
         (LPARAM) ((char*) dData.pData + i));
   }
   SendMessage(m_hTextWnd, EM_SCROLLCARET, 0, 0); 
   m_bProcessENSelChange = TRUE;


   // free the memory allocated for text changes
   if (dData.pData)
      CoTaskMemFree(dData.pData);
   nCount++;


   // fall on through to the selection getting code
getselchanges:
   DWORD d1, d2;
   HRESULT hRes;
   //char sz[256];

   hRes = gpIVDctText->TextSelGet(&d1, &d2);
   if (hRes)
      goto alldone;

   // only do stuff if the selection is different
   SendMessage (m_hTextWnd, EM_GETSEL, (WPARAM) &dwStart, (LPARAM) &dwEnd);
   if ((dwStart == d1) && (dwEnd == (d1+d2)))
      goto alldone;

   // get the new position
   m_bProcessENSelChange = FALSE;
   SendMessage (m_hTextWnd, EM_SETSEL, d1, d1+d2);
   m_bProcessENSelChange = TRUE;

alldone:
   TellCorrectionOfSelection();


   gpIVDctText->UnLock();

}




/******************************************************************************
*
* IntermWndProc
*
******************************************************************************/

LRESULT CALLBACK IntermWndProc(HWND hWnd, 
                               UINT message, 
                               WPARAM wParam, 
                               LPARAM lParam)
{
	static LPMINMAXINFO lpmmi;
	static HBRUSH hBrush = 0;
	static RECT rc;
	static HPEN hPBlack = 0, hPWhite = 0, hPGrey = 0;
	static HBRUSH hBrButton = 0;
	static LPDRAWITEMSTRUCT lpdis;
   static LPSTORAGE pIStgFile = NULL;
   static LPSTREAM pIStmHeader = NULL;
   static LPSTREAM pIStmText = NULL;
   static LPSTREAM pIStmVersion = NULL;
   char* pszStmHeader = "Header";
   char* pszStmText = "Text";
   char* pszStmVersion = "Version";

	switch (message) {
		case WM_CREATE:
		{
			m_hTextWnd = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_RIGHTSCROLLBAR, 
				TEXT("RICHEDIT"), NULL,
				WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP 
				| ES_MULTILINE  | ES_AUTOVSCROLL | WS_VSCROLL,
				2*m_wCXFrame + INTERM_BUTTONSIDE, 
				m_wCYFrame, 
				10, 10,
				hWnd, (HMENU) IDC_INTERM_TEXTWND, m_hInst, NULL);
			SetFocus(m_hTextWnd);
         TellCorrectionOfSelection();
         gpIVDctText->TextSelSet(0, 0);

			hBrush = GetSysColorBrush(COLOR_MENU);
			hPBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			hPGrey = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
			hPWhite = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNHIGHLIGHT));
			hBrButton = GetSysColorBrush(COLOR_BTNFACE);

         // Get the menu of the Send to
         m_hMenuSendTo = GetSubMenu(GetSubMenu (GetMenu(hWnd), 0), 0);


         gfUndoHasData[0] = gfUndoHasData[1] = FALSE;
         gdwUndoLatest = 0;
         UndoRemember ();

         return 0;
		}

      case WM_TIMER:
         // make sure not in process of shutting down
         if (!gpIVoiceDictation)
            return 0;

         if (wParam == TIMER_SAVEUNDO) {
            RememberIfTimer();
         }

         return 0;

      case WM_QUERYENDSESSION:
         // set this window to the top
         SetWindowPos(hWnd, HWND_TOPMOST,
              0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

         if (GetWindowTextLength(m_hTextWnd) && 
             SendMessage(m_hTextWnd, EM_GETMODIFY, 0,0)) {
            if (IDCANCEL == 
                  MessageBoxonTop(ghIntermWnd, IDS_SHUTDOWN, MB_OKCANCEL|MB_DEFBUTTON2))
               return FALSE;
            else
               return TRUE;
         } else
            return TRUE;

      case WM_ENDSESSION:
         if (wParam)
            EndSessionProc();
         return 0;

      case WM_APPCOMMAND:
         // make sure not in process of shutting down
         if (!gpIVoiceDictation)
            return 0;

         // an application specific command happened, do it
         DoAppCommand (wParam, (PCSTR) lParam);

         if (lParam)
            free ((PVOID) lParam);
         return 0;

      case WM_INITMENUPOPUP: 
         if (m_hMenuSendTo == (HMENU) wParam) {
            // it's the send to menu
            // Wipe out all except the top two items
            while (RemoveMenu(m_hMenuSendTo, 2, MF_BYPOSITION));

            // now fill it in
            ListOfRunningApps (m_hMenuSendTo, STARTAPPS);
            return 0;
         }


         switch (LOWORD(lParam)) {
#ifdef SHOWCORRECTIONWINDOW
            case 2: // Dictation
               // Correction window
               DWORD dwFlags;
               dwFlags = VDCTGUIF_VISIBLE;
               gpIVDctGui->FlagsGet(&dwFlags);
		         CheckMenuItem(m_hLongIntermMenu, IDM_MSDICT_CORRECT,
                  MF_BYCOMMAND| ((dwFlags & VDCTGUIF_VISIBLE) ? MF_CHECKED : MF_UNCHECKED));
               break;
#endif // SHOWCORRECTIONWINDOW
            case 3:
               CheckMenuItem(m_hLongIntermMenu, IDM_INTERN_SELECTENTIREWORD, 
                  MF_BYCOMMAND| (m_fSelectEntireWord ? MF_CHECKED : MF_UNCHECKED) );

               break;

            break;

            case 1: { // Edit
               DWORD dwStart, dwEnd;

               SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

               if (dwEnd > dwStart) {
                  EnableMenuItem(m_hLongIntermMenu, IDM_INTERM_EDITCUT, MF_BYCOMMAND|MF_ENABLED);
                  EnableMenuItem(m_hLongIntermMenu, IDM_INTERM_EDITCOPY, MF_BYCOMMAND|MF_ENABLED);
                  SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCUT, MAKELONG(TRUE, 0));
                  SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCOPY, MAKELONG(TRUE, 0));
               }
               else {
                  EnableMenuItem(m_hLongIntermMenu, IDM_INTERM_EDITCUT, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
                  EnableMenuItem(m_hLongIntermMenu, IDM_INTERM_EDITCOPY, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
                  SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCUT, MAKELONG(FALSE, 0));
                  SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCOPY, MAKELONG(FALSE, 0));
               }

               // check the clipboard. If its empty disable the Paste
               HGLOBAL hglbMem = NULL;
               LPSTR lpPaste = NULL;

	            OpenClipboard(NULL);

	            hglbMem = GetClipboardData(CF_TEXT);

               if (hglbMem)
   	            lpPaste = (LPSTR)GlobalLock(hglbMem);

               if (lpPaste && strlen(lpPaste)) {
                  EnableMenuItem(m_hLongIntermMenu, IDM_INTERM_EDITPASTE, 
                     MF_BYCOMMAND|MF_ENABLED);
                  SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITPASTE, MAKELONG(TRUE, 0));
	            }
               else {
                  EnableMenuItem(m_hLongIntermMenu, IDM_INTERM_EDITPASTE, 
                     MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
                  SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITPASTE, MAKELONG(FALSE, 0));
	            }

               GlobalUnlock(hglbMem);
	            CloseClipboard();
            }
         }

         return 0;

		// handle this so we can allow the user to drag the window without a
		// title bar...
		case WM_NCHITTEST:
			wParam = DefWindowProc(hWnd, message, wParam, lParam);
			if (m_bHideCaption && wParam == HTCLIENT)
				return HTCAPTION;
			else
				return wParam;

      case WM_USER+863: // text changed
      case WM_USER+865: // selecton changed;
         // make sure not in process of shutting down
         if (!gpIVoiceDictation)
            return 0;

         GetChangesFromVDct();
         return 0;

      case WM_USER+864: {
         if (gbRunningFirstTime)
            return 0;

         // set the dictation icon
         AreWeDictating();
         SetDictationIcon();

         SetTitle();

         return 0;
      }


      case EN_SAVECLIPBOARD:
         // save anything on the clipboard
         return 0;

      case WM_NOTIFY:
         switch ( ((LPNMHDR)lParam)->code) {
         case EN_SELCHANGE: {
            DWORD dwStart, dwEnd;
            SHORT wState;
            static DWORD sdwOldStart = (DWORD)-1L, sdwOldEnd = (DWORD)-1L;

            SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

            if (IDC_INTERM_TEXTWND != wParam || !(m_bProcessENSelChange)) {
               sdwOldStart = dwStart;
               sdwOldEnd = dwEnd;
               return 0;
            }

#ifdef _DEBUG
            OutputDebugString ("EN_SELCHANGE - check for L button\r\n");
#endif

            wState = GetKeyState(VK_LBUTTON);
            if (wState & 0x8000) {
               sdwOldStart = dwStart;
               sdwOldEnd = dwEnd;
               return 0;
            }

#ifdef _DEBUG
            OutputDebugString ("EN_SELCHANGE - L button\r\n");
#endif

            CHARRANGE chrg = ((SELCHANGE*)lParam)->chrg;
            if (chrg.cpMax > chrg.cpMin) {
               SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCUT, MAKELPARAM(TRUE, 0));
               SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCOPY, MAKELPARAM(TRUE, 0));
            }
            else {
               SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCUT, MAKELPARAM(FALSE, 0));
               SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCOPY, MAKELPARAM(FALSE, 0));
            }
            
            // if the selection hasn't changed since the last one then it's
            // a duplicate
            if ((sdwOldStart == dwStart) && (sdwOldEnd == dwEnd))
               return 0;
            sdwOldStart = dwStart;
            sdwOldEnd = dwEnd;

            // Potentially adjust the selection around a word
            if (m_fSelectEntireWord && m_bAdjustSelection) {
               AdjustSelection(&dwStart, &dwEnd);
               sdwOldStart = dwStart;
               sdwOldEnd = dwEnd;
            }

            m_bAdjustSelection = FALSE;
            DictTextSelSet(dwStart, dwEnd - dwStart);
            TellCorrectionOfSelection();
         }
         return 0;

         case TTN_NEEDTEXT:
           LPTOOLTIPTEXT lpToolTipText;
           lpToolTipText = (LPTOOLTIPTEXT) lParam;
           switch (wParam) {
           case IDM_INTERM_FILENEW:
              lpToolTipText->lpszText = (LPSTR) IDS_INTERM_TT_NEW;
              break;
           case IDM_ADDWORD:
              lpToolTipText->lpszText = (LPSTR) IDS_INTERM_TT_ADDWORD;
              break;
           case IDM_INTERM_FILEOPEN:
              lpToolTipText->lpszText = (LPSTR) IDS_INTERM_TT_OPEN;
              break;
           case IDM_INTERM_FILESAVEAS:
              lpToolTipText->lpszText = (LPSTR) IDS_INTERM_TT_SAVEAS;
              break;
           case IDM_INTERM_EDITCUT:
              lpToolTipText->lpszText = (LPSTR) IDS_INTERM_TT_CUT;
              break;
           case IDM_INTERM_EDITCUTALL:
              lpToolTipText->lpszText = (LPSTR) IDS_INTERM_TT_CUTALL;
              break;
           case IDM_INTERM_EDITCOPY:
              lpToolTipText->lpszText = (LPSTR) IDS_INTERM_TT_COPY;
              break;
           case IDM_INTERM_EDITPASTE:
              lpToolTipText->lpszText = (LPSTR) IDS_INTERM_TT_PASTE;
              break;
           case IDM_TB_INTERM_DICTLISTEN:
              lpToolTipText->lpszText = (LPSTR) IDS_INTERM_TT_LISTEN;
              break;
           case IDM_INTERM_CAPITALIZE:
              lpToolTipText->lpszText = (LPSTR) IDS_INTERM_TT_CAPITALIZE;
              break;
#ifdef SHOWCORRECTIONWINDOW
           case IDM_MSDICT_CORRECT:
              lpToolTipText->lpszText = (LPSTR) IDS_INTERM_TT_CORRECTION;
              break;
#endif // SHOWCORRECTIONWINDOW
           }
           lpToolTipText->hinst = m_hInst;
           break;
         }
         return 0;
         

		case WM_GETMINMAXINFO:
			lpmmi = (LPMINMAXINFO)lParam;
			lpmmi->ptMinTrackSize.x = m_wMinWidth;
			lpmmi->ptMinTrackSize.y = m_wMinHeight;
			return 0;

		case WM_SIZE: {
#if 0
         // if we minimized the hide the correction window
         if (wParam == SIZE_MINIMIZED) {
            DWORD dwFlags;
            gpIVDctGui->FlagsGet(&dwFlags);
            m_fHidCorrection = (dwFlags & VDCTGUIF_VISIBLE) != FALSE;
            CorrectionOnOff(FALSE);

         }
         else if (m_fHidCorrection) {
            m_fHidCorrection = FALSE;
            CorrectionOnOff(TRUE);
         }
#endif // 0

         DWORD x, y;
         x = m_wCXFrame;
         if (m_bHideCaption)
            x += INTERM_BUTTONSIDE + m_wCXFrame;
         y = m_wCYFrame;
         if (!(m_bHideToolBar))
            y += m_wTBHeight;

   	   MoveWindow(m_hTextWnd, x, y, LOWORD(lParam) - x - m_wCXFrame,
			   HIWORD(lParam) - y - m_wCYFrame, TRUE);

   	   MoveWindow(m_hToolBar, 0, 0, LOWORD(lParam),
			   m_wTBHeight, TRUE);

   	   MoveWindow(m_hToolBar, 0, 0, LOWORD(lParam),
			   m_wTBHeight, TRUE);

         TellCorrectionOfSelection();
         DWORD dwStart, dwEnd;
         SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
         DictTextSelSet(dwStart, dwEnd - dwStart);
      }
      return 0;

      case WM_MOVE: {
         TellCorrectionOfSelection();
      }
      break;   // left default handling

		case WM_ERASEBKGND: {			
			HBRUSH hbrTemp = (HBRUSH)SelectObject((HDC)wParam, hBrush);
			GetClipBox((HDC)wParam, &rc);
			PatBlt((HDC)wParam, rc.left, rc.top, rc.right - rc.left, 
					rc.bottom - rc.top, PATCOPY);
			SelectObject((HDC)wParam, hbrTemp);
			return TRUE;
		}

		case WM_SYSCOLORCHANGE:
			DeleteObject(hBrush);
			hBrush = GetSysColorBrush(COLOR_MENU);
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;

		case WM_DISPLAYCHANGE:
			m_wXScreen = LOWORD(lParam);
			m_wYScreen = HIWORD(lParam);
			m_wCXFrame = GetSystemMetrics(SM_CXFIXEDFRAME);
			m_wCYFrame = GetSystemMetrics(SM_CYFIXEDFRAME);
			m_wCaption = GetSystemMetrics(SM_CYCAPTION);
			m_wBorder = GetSystemMetrics(SM_CYBORDER);
         m_wYMenu = GetSystemMetrics(SM_CYMENU);
         // recalc the min height and width
         ToggleCaptionAndAdjust(m_bHideCaption, m_bHideToolBar);
			break;

		case WM_ACTIVATE: {
         static BOOL bToggled = FALSE;
         BYTE kbState[256];

			if (LOWORD(wParam) != WA_INACTIVE) {
  		      SetFocus(m_hTextWnd);

            // tell the correction window where we are
            TellCorrectionOfSelection();

            // DctPad does not support overwrite mode

            if (!GetKeyboardState((PBYTE)&kbState))
               return 0;

            if (kbState[VK_INSERT] & 0x01) {
               // Insert key is toggled we are in overwrite mode
               kbState[VK_INSERT] &= 0xFE;
               SetKeyboardState((LPBYTE)&kbState);
               bToggled = TRUE;
            }
			}
         else {
            if (bToggled) {
               // restore the insert/overwrite mode
               if (!GetKeyboardState((PBYTE)&kbState))
                  return 0;
               kbState[VK_INSERT] |= 0x01;
               SetKeyboardState((LPBYTE)&kbState);
               bToggled = FALSE;
            }
         }
      }
		return 0;
		
      case WM_USER + 200: {
         char szTemp[256];
         WCHAR wszTemp[260];
         char* pszTemp;
         BOOL fRet = TRUE;
         HRESULT hRes;
         DWORD d;
         ULARGE_INTEGER ui;
         char szPath[260];

         ZeroMemory(&ui, sizeof(ui));

         HCURSOR hCurSave = SetCursor(m_hWaitCursor);

         strcpy(szPath, (char*)lParam);

         LoadString(ghInst, IDS_TEXTFILEEXT, szTemp, sizeof(szTemp) - 1);
         if (!stricmp(szTemp + 1, (szPath + strlen(szPath) - 4))) {
            // save as text file

            int i = 0;

            HANDLE h = CreateFile(szPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
               FILE_ATTRIBUTE_NORMAL, NULL);
            if (!h)
               goto error200;

            i = GetWindowTextLength(m_hTextWnd);
            if (!i) {
               CloseHandle(h);
               goto continue200;
            }

            pszTemp = (char*)calloc(i + 1, sizeof(char));
            if (!pszTemp)
               goto error200;

            GetWindowText(m_hTextWnd, pszTemp, i + 1);
            if (!WriteFile(h, pszTemp, i, &d, 0)) {
               CloseHandle(h);
               free(pszTemp);
               pszTemp = NULL;
               goto error200;
            }

            free(pszTemp);
            pszTemp = NULL;

            CloseHandle(h);
            goto continue200;
         }

         // save as doc file
         MultiByteToWideChar(CP_ACP, NULL, szPath, -1, wszTemp, 260);
         hRes = StgOpenStorage(wszTemp, NULL, STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 
            NULL, 0, &pIStgFile);
         
         if (STG_E_FILENOTFOUND == hRes || STG_E_FILEALREADYEXISTS == hRes)
            hRes = StgCreateDocfile(wszTemp, STGM_CREATE | STGM_READWRITE | 
               STGM_SHARE_EXCLUSIVE, 0, &pIStgFile);

         if (hRes)
            goto error200;

         // save font
         MultiByteToWideChar(CP_ACP, NULL, pszStmHeader, -1, wszTemp, 260);
         hRes = pIStgFile->OpenStream(wszTemp, NULL, STGM_READWRITE | 
                     STGM_SHARE_EXCLUSIVE, NULL, &pIStmHeader);
         
         if (STG_E_FILENOTFOUND == hRes)
            hRes = pIStgFile->CreateStream(wszTemp, STGM_CREATE | STGM_READWRITE | 
                     STGM_SHARE_EXCLUSIVE, 0, 0, &pIStmHeader);

         if (hRes)
            goto error200;

         hRes = pIStmHeader->SetSize(ui);

         if (hRes)
            goto error200;

         hRes = pIStmHeader->Write(&(m_lf), sizeof(LOGFONT), NULL);
         if (hRes)
            goto error200;

         // save text
         hRes = gpIVoiceDictation->SessionSerialize(pIStgFile);
         if (hRes)
            goto error200;

         MultiByteToWideChar(CP_ACP, NULL, pszStmVersion, -1, wszTemp, 260);
         hRes = pIStgFile->OpenStream(wszTemp, NULL, STGM_READWRITE | 
                     STGM_SHARE_EXCLUSIVE, NULL, &pIStmVersion);
         
         if (STG_E_FILENOTFOUND == hRes)
            hRes = pIStgFile->CreateStream(wszTemp, STGM_CREATE | STGM_READWRITE | 
                     STGM_SHARE_EXCLUSIVE, 0, 0, &pIStmVersion);

         if (hRes)
            goto error200;

         hRes = pIStmVersion->SetSize(ui);

         if (hRes)
            goto error200;

         hRes = pIStmVersion->Write(MSD_Version, strlen(MSD_Version), NULL);
         if (hRes)
            goto error200;

         goto continue200;

      error200:
         fRet = FALSE;

      continue200:
         if (pIStgFile) {
            pIStgFile->Release();
            pIStgFile = NULL;
         }
         if (pIStmHeader) {
            pIStmHeader->Release();
            pIStmHeader = NULL;
         }
         if (pIStmText) {
            pIStmText->Release();
            pIStmText = NULL;
         }
         if (pIStmVersion) {
            pIStmVersion->Release();
            pIStmVersion = NULL;
         }

         SetCursor(hCurSave);
         if (fRet) {
            return TRUE;
         }

         pszTemp = strrchr((char*)lParam, '\\');
         if (pszTemp)
            pszTemp++;
         else
            pszTemp = (char*)lParam;
         LoadString(ghInst, IDS_ERRORWRITINGFILE, szTemp, sizeof(szTemp) - 1);
         sprintf((char*)wszTemp, szTemp, pszTemp);
         sprintf(szTemp, " (%X)", hRes);
         strcat((char*)wszTemp, szTemp);
         MessageBox(hWnd, (char*)wszTemp, gszAppName, MB_OK|MB_ICONERROR);

         // delete the file if it has been created
         DeleteFile((LPSTR)lParam);

         return FALSE;
      }

		case WM_COMMAND: {
         switch (LOWORD(wParam)) {
            case IDM_INTERM_CAPITALIZE:
               if (!(gpIVDctText->Lock())) {
                  gpIVDctText->FX(VDCTFX_TOGGLEFIRST);
                  gpIVDctText->UnLock();
               }
               return 0;

            case IDC_INTERM_TEXTWND:
               if (EN_CHANGE == HIWORD(wParam)) {
                  if (m_fENChange)
                     return 0;

                  if (m_fOpeningFile) {
                     m_fOpeningFile = FALSE;
                     return 0;
                  }

                  // mark document dirty
                  m_fFileDirty = TRUE;

               }
               return 0;

            case IDM_INTERM_FILENEW: {
               char szTemp[256];
               char szTemp1[256];
               char *pszTemp;
               int i;

               LPSTR pFile = strrchr(m_szOpenFile, '\\');
               if (!pFile)
                  pFile = m_szOpenFile;
               else
                  pFile++;

               if (m_fFileDirty) {
                  if (stricmp(pFile, m_szUntitled)) {
                     LoadString(ghInst, IDS_SAVECURFILE, szTemp, sizeof(szTemp) - 1);
                     sprintf(szTemp1, szTemp, pFile);
                  } else
                     LoadString(ghInst, IDS_SAVENEWFILE, szTemp1, sizeof(szTemp1) - 1);

                  i = MessageBox(hWnd, szTemp1, gszAppName, MB_YESNOCANCEL|MB_ICONQUESTION);
                  
                  if (IDCANCEL == i)
                     return 0;

                  if (IDYES == i)
                     if (!SendMessage(hWnd, WM_COMMAND, IDM_INTERM_FILESAVE, 0))
                        return 0;
               }

               EmptyDctPad(VDCT_TEXTCLEAN);

               GetWindowText(hWnd, szTemp, sizeof(szTemp) - 1);
               pszTemp = strchr(szTemp, '-');
               if (pszTemp)
                  *--pszTemp = 0;

               SetWindowText(hWnd, szTemp);

               strcpy(m_szOpenFile, m_io.szDictFilesDir);
               strcat(m_szOpenFile, "\\");
               strcat(m_szOpenFile, m_szUntitled);
              
               gfUndoHasData[0] = gfUndoHasData[1] = FALSE;
               gdwUndoLatest = 0;
               UndoRemember ();

               m_fFileNew = TRUE;
               m_fFileDirty = FALSE;
            }
            break;

            case IDM_INTERM_FILEOPEN: {
               OPENFILENAME ofn;
               char szFilter[260];
               char szFileExt[32];
               char szFileTitle[128];
               char szFilePath[256];
               char* pszTemp;
               WCHAR wszTemp[260];
               HCURSOR hCurSave;
               HRESULT hRes;
               DWORD d = 0, d1;
               MSG msg;
               
               LPSTR pFile = strrchr(m_szOpenFile, '\\');
               if (!pFile)
                  pFile = m_szOpenFile;
               else
                  pFile++;

               while (PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE)) {
                  if (!IsDialogMessage(hWnd, &msg)) {
                     TranslateMessage(&msg);
                     DispatchMessage(&msg);
                  }
               }

               ZeroMemory(szFileTitle, sizeof(szFileTitle));
               ZeroMemory(szFilePath, sizeof(szFilePath));
               ZeroMemory(szFilter, sizeof(szFilter));
               LoadString(ghInst, IDS_DICTFILEEXT, szFileExt, sizeof(szFileExt) - 1);
               LoadString(ghInst, IDS_DICTFILES, szFilter, sizeof(szFilter) - 1);
               pszTemp = szFilter + strlen(szFilter) + 1;
               LoadString(ghInst, IDS_DICTFILEEXT, pszTemp, sizeof(szFilter) - 
                  (pszTemp - szFilter)*sizeof(char));
               pszTemp += strlen(pszTemp) + 1;
               LoadString(ghInst, IDS_TEXTFILES, pszTemp, sizeof(szFilter) - 
                  (pszTemp - szFilter)*sizeof(char));
               pszTemp += strlen(pszTemp) + 1;
               LoadString(ghInst, IDS_TEXTFILEEXT, pszTemp, sizeof(szFilter) - 
                  (pszTemp - szFilter)*sizeof(char));

               if (lParam && !IsWindow((HWND)lParam)) {
                  // msdict was started as msdict <filename>
                  strcpy(szFilePath, (char*)lParam);
                  pszTemp = strrchr(szFilePath, '\\');
                  strcpy(szFileTitle, pszTemp + 1);
                  if (gpszFile) {
                     // this came in when .msd file was double clicked when
                     // dictation was already running
                     free(gpszFile);
                     gpszFile = NULL;
                  }   
               } else {
                  d = strlen(pFile) - 4;
                  if (!(d < strlen(pFile) && '.' == pFile[d]))
                     d = 0;

                  ofn.lStructSize = sizeof (OPENFILENAME);
                  ofn.hwndOwner = hWnd;
                  ofn.hInstance = NULL;
                  ofn.lpstrFilter = szFilter;
                  ofn.lpstrCustomFilter = NULL;
                  ofn.nMaxCustFilter = 0L;

                  if (d)
                     ofn.nFilterIndex = pFile[d + 1] == 't' ? 2 : 1;
                  else
                     ofn.nFilterIndex = 1;

                  ofn.lpstrFile = szFilePath;
                  ofn.nMaxFile = sizeof(szFilePath) - 1;
                  ofn.lpstrFileTitle = szFileTitle;
                  ofn.nMaxFileTitle = sizeof(szFileTitle)-1;
                  ofn.lpstrInitialDir = m_io.szDictFilesDir;
                  ofn.lpstrTitle = NULL;
                  ofn.nFileOffset = 0;
                  ofn.nFileExtension = (WORD)d;
                  ofn.lpstrDefExt = szFileExt;
                  ofn.lpfnHook = NULL;
                  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | 
                  OFN_LONGNAMES | OFN_HIDEREADONLY | OFN_NOCHANGEDIR |
                     OFN_EXPLORER; // | OFN_ENABLEHOOK;

                  if (!GetOpenFileName (&ofn))
                     break;

                  pszTemp = strrchr(szFilePath, '\\');
               }

               while (PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE)) {
                  if (!IsDialogMessage(hWnd, &msg)) {
                     TranslateMessage(&msg);
                     DispatchMessage(&msg);
                  }
               }

               m_fFileNew = FALSE;

               ZeroMemory(m_io.szDictFilesDir, sizeof(m_io.szDictFilesDir));
               strncpy(m_io.szDictFilesDir, szFilePath, pszTemp - szFilePath);

               if (m_fFileDirty) {
                  // first save current file
                  char szTemp[256];

                  if (stricmp(pFile, m_szUntitled)) {
                     LoadString(ghInst, IDS_SAVECURFILE, szFilter, sizeof(szFilter)-1);
                     sprintf(szTemp, szFilter, pFile);
                  } else
                     LoadString(ghInst, IDS_SAVENEWFILE, szTemp, sizeof(szTemp) - 1);

                  d = ::MessageBox(hWnd, szTemp, gszAppName, MB_ICONQUESTION|MB_YESNOCANCEL);
                  if (IDCANCEL == (int)d)
                     return FALSE;
                  if (IDYES == (int)d)
                     if (!SendMessage(hWnd, WM_COMMAND, IDM_INTERM_FILESAVE, 0))
                        return FALSE;
               }

               hCurSave = SetCursor(m_hWaitCursor);

               if (4 < strlen(szFileTitle)) {
                  LoadString(ghInst, IDS_TEXTFILEEXT, (char*)wszTemp, sizeof(wszTemp) - 1);
                  if (!stricmp((char*)wszTemp + 1, (szFilePath + strlen(szFilePath) - 4))) {
                  
                     // open as text file
                     HANDLE h = CreateFile(szFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);
                     if (!h)
                        goto errorOpen;

                     hRes = TRUE;
                     d = 1;
                     d1 = 0;

                     EmptyDctPad(0);

                     // read in text
                     for (;;) {
                        ZeroMemory(szFilter, sizeof(szFilter));

                        hRes = ReadFile(h, szFilter, sizeof(szFilter) - 1, &d, NULL);
                        if (!d)
                           break;

                        m_bProcessENSelChange = FALSE;
                        SendMessage(m_hTextWnd, EM_SETSEL, 0x0FFFFFFF, 0x0FFFFFFF);
                        SendMessage(m_hTextWnd, EM_REPLACESEL, FALSE, (LPARAM)szFilter);
                        DictTextSet(d1, d, szFilter, 0);
                        d1 += d;
                        m_bProcessENSelChange = TRUE;
                     }

                     SendMessage(m_hTextWnd, EM_SETSEL, 0x0FFFFFFF, 0x0FFFFFFF);
                     DictTextSelSet(d1, 0);

                     CloseHandle(h);

                     // reset the DctPad font to default

                  	m_lf.lfHeight = m_wCaption;
                     m_lFontHeight = 19;

                     m_lf.lfHeight = -19;
	                  m_lf.lfWeight = FW_NORMAL;  
                     strcpy(m_lf.lfFaceName, "Times New Roman");
      
                     if (m_hfSpoken)
                        DeleteObject(m_hfSpoken);

                     m_hfSpoken = CreateFontIndirect(&(m_lf));

                  	SendMessage(m_hTextWnd, WM_SETFONT, (WPARAM)m_hfSpoken, 
                        MAKELPARAM(TRUE, 0));
                     
                     goto setCaption;
                  } else {
                     LoadString(ghInst, IDS_DICTFILEEXT, (char*)wszTemp, sizeof(wszTemp) - 1);
                     if (stricmp((char*)wszTemp + 1, (szFilePath + strlen(szFilePath) - 4))) {
                        LoadString(ghInst, IDS_INVALIDFILETYPE, szFilter, sizeof(szFilter) - 1);
                        sprintf((char*)wszTemp, szFilter, szFileTitle);
                        MessageBox(ghIntermWnd, (char*)wszTemp, gszAppName, MB_OK|MB_ICONEXCLAMATION);

                        goto continueOpen;
                     }
                  }
               }

               MultiByteToWideChar(CP_ACP, NULL, szFilePath, -1, wszTemp, 260);
               hRes = StgOpenStorage(wszTemp, NULL, STGM_READWRITE | 
                         STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, 0, &pIStgFile);

               if (STG_E_FILENOTFOUND == hRes) {
                  // szFileExit == *.msd
                  // check if this file ended with .msd otherwise tag .msd and try
                  if (strlen(szFileTitle) >= 4) {
                     if (!strcmp(szFileTitle + strlen(szFileTitle) - 4, szFileExt + 1))
                        goto errorOpen;
                  }
                  strcat(szFilePath, szFileExt+1);
                  strcat(szFileTitle, szFileExt+1);

                  MultiByteToWideChar(CP_ACP, NULL, szFilePath, -1, wszTemp, 260);
                  hRes = StgOpenStorage(wszTemp, NULL, STGM_READWRITE | 
                         STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, 0, &pIStgFile);
               }

               if (hRes && hRes != STG_E_FILEALREADYEXISTS)
                  goto errorOpen;

               if (hRes) {
                  // maybe created in explorer as a msdict file
                  // the file being opened is not a doc file
                  hRes = StgCreateDocfile(wszTemp, STGM_CREATE | STGM_READWRITE | 
                     STGM_SHARE_EXCLUSIVE, 0, &pIStgFile);
                  if (hRes)
                     goto errorOpen;
                  else {
                     m_fENChange = TRUE;
                     EmptyDctPad(0);
                     m_fENChange = FALSE;
                  }
               } else {
                  MultiByteToWideChar(CP_ACP, NULL, pszStmVersion, -1, wszTemp, 260);
                  hRes = pIStgFile->OpenStream(wszTemp, NULL, STGM_READWRITE | 
                           STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, &pIStmVersion);
                  if (hRes) {
                     if (hRes == STG_E_FILENOTFOUND) {
                        // maybe created in explorer as a msdict file
                        m_fENChange = TRUE;
                        EmptyDctPad(0);
                        m_fENChange = FALSE;
                        goto setCaption;
                     } else
                        goto errorOpen;
                  }

                  hRes = pIStmVersion->Read(szFilter, strlen(MSD_Version), NULL);
                  if (hRes)
                     goto errorOpen;

                  if (strncmp(szFilter, MSD_Version, strlen(MSD_Version)))
                     goto errorOpen;

                  MultiByteToWideChar(CP_ACP, NULL, pszStmHeader, -1, wszTemp, 260);
                  hRes = pIStgFile->OpenStream(wszTemp, NULL, STGM_READWRITE | 
                           STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, &pIStmHeader);
                  if (hRes)
                     goto errorOpen;

                  hRes = pIStmHeader->Read(&(m_lf), sizeof(LOGFONT), NULL);
                  if (hRes)
                     goto errorOpen;

                  if (m_hfSpoken)
                     DeleteObject(m_hfSpoken);

                  m_hfSpoken = CreateFontIndirect(&(m_lf));

               	SendMessage(m_hTextWnd, WM_SETFONT, (WPARAM)m_hfSpoken, 
                     MAKELPARAM(TRUE, 0));

                  m_fENChange = TRUE;
                  EmptyDctPad(0);
                  m_fENChange = FALSE;

                  // DeSerialize bookmarks, results objects, internal buffer, etc
                  m_fOpeningFile = TRUE;
                  hRes = gpIVoiceDictation->SessionDeserialize(pIStgFile);
                  if (hRes)
                     goto errorOpen;

                  gfUndoHasData[0] = gfUndoHasData[1] = FALSE;
                  gdwUndoLatest = 0;
                  UndoRemember ();
               }

            setCaption:

               m_fFileDirty = FALSE;

               GetWindowText(ghIntermWnd, szFilter, sizeof(szFilter) - 1);
               pszTemp = strstr(szFilter, " - ");
               if (pszTemp)
                  strcpy(pszTemp + 3, szFileTitle);
               else {
                  strcat(szFilter, " - ");
                  strcat(szFilter, szFileTitle);
               }
               if (4 < strlen(szFilter) && '.' == szFilter[strlen(szFilter) - 4])
                  szFilter[strlen(szFilter) - 4] = 0;
               SetWindowText(ghIntermWnd, szFilter);
               
               strcpy(m_szOpenFile, m_io.szDictFilesDir);
               strcat(m_szOpenFile, "\\");
               strcat(m_szOpenFile, szFileTitle);

               goto continueOpen;

            errorOpen:
               LoadString(ghInst, IDS_ERRORREADINGFILE, szFilter, sizeof(szFilter) - 1);
               sprintf(szFilePath, szFilter, szFileTitle);
               MessageBox(hWnd, szFilePath, gszAppName, MB_OK|MB_ICONERROR);

            continueOpen:
               if (pIStgFile) {
                  pIStgFile->Release();
                  pIStgFile = NULL;
               }
               if (pIStmHeader) {
                  pIStmHeader->Release();
                  pIStmHeader = NULL;
               }
               if (pIStmVersion) {
                  pIStmVersion->Release();
                  pIStmVersion = NULL;
               }
               if (pIStmText) {
                  pIStmText->Release();
                  pIStmText = NULL;
               }

               SetCursor(hCurSave);
            }
            return TRUE;

            case IDM_INTERM_FILESAVE: {
               if (!m_fFileDirty && !m_fFileNew)
                  return TRUE;
               
               LPSTR pFile = strrchr(m_szOpenFile, '\\');
               if (!pFile)
                  pFile = m_szOpenFile;
               else
                  pFile++;

               if (!strcmp(pFile, m_szUntitled)) {
                  if (!SendMessage(hWnd, WM_COMMAND, IDM_INTERM_FILESAVEAS, 0))
                     return FALSE;
               } else
                  SendMessage(hWnd, WM_USER + 200, 0, (LPARAM)m_szOpenFile);

               m_fFileDirty = FALSE;
               m_fFileNew = FALSE;
            }
            return TRUE;

            case IDM_INTERM_FILESAVEAS: {
               OPENFILENAME ofn;
               char szTemp[256];
               char szFileTitle[128];
               char szFilePath[256];
               char szFileExt[32];
               char* pszTemp;
               UINT i =0;
               MSG msg;
               
               LPSTR pFile = strrchr(m_szOpenFile, '\\');
               if (!pFile)
                  pFile = m_szOpenFile;
               else
                  pFile++;

               while (PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE)) {
                  if (!IsDialogMessage(hWnd, &msg)) {
                     TranslateMessage(&msg);
                     DispatchMessage(&msg);
                  }
               }

               ZeroMemory(szTemp, sizeof(szTemp));
               ZeroMemory(szFilePath, sizeof(szFilePath));
               LoadString(ghInst, IDS_DICTFILEEXT, szFileExt, sizeof(szFileExt) - 1);
               LoadString(ghInst, IDS_DICTFILES, szTemp, sizeof(szTemp) - 1);
               pszTemp = szTemp + strlen(szTemp) + 1;
               LoadString(ghInst, IDS_DICTFILEEXT, pszTemp, sizeof(szTemp) - 
                  (pszTemp - szTemp)*sizeof(char));
               pszTemp += strlen(pszTemp) + 1;
               LoadString(ghInst, IDS_TEXTFILES, pszTemp, sizeof(szTemp) - 
                  (pszTemp - szTemp)*sizeof(char));
               pszTemp += strlen(pszTemp) + 1;
               LoadString(ghInst, IDS_TEXTFILEEXT, pszTemp, sizeof(szTemp) - 
                  (pszTemp - szTemp)*sizeof(char));

               LoadString(ghInst, IDS_UNSAVEDFILE, szFileTitle, sizeof(szFileTitle)-1);

               if (!stricmp(szFileTitle, pFile)) {
                  // copy the first word from text
                  DWORD d1 = SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_RIGHTBREAK, 0);
                  DWORD d2 = SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_LEFT, d1);
                  DWORD d3;

                  TEXTRANGE tr;

                  if (d1 - d2 >= sizeof(szFileTitle) - 1)
                     d1 = d2 + sizeof(szFileTitle) - 2;

                  tr.chrg.cpMin = d2;
                  tr.chrg.cpMax = d1;
                  tr.lpstrText = szFileTitle;
                  if (0 != (d3 = SendMessage(m_hTextWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr)) &&
                      IsCharAlpha(szFileTitle[0])) {
                     for (UINT i = 1; i < d3; i++)
                        if (!IsCharAlpha(szFileTitle[i]))
                           szFileTitle[i] = '\0';   

                     strcpy(szFilePath, szFileTitle);
                  } else
                     strcpy(szFilePath, pFile);
               } else
                  strcpy(szFilePath, pFile);

               ZeroMemory(szFileTitle, sizeof(szFileTitle));

               i = strlen(szFilePath) - 4;
               if (!(i < strlen(szFilePath) && '.' == szFilePath[i]))
                  i = 0;

               ofn.lStructSize = sizeof(OPENFILENAME);
               ofn.hwndOwner = hWnd;
               ofn.hInstance = NULL;
               ofn.lpstrFilter = szTemp;
               ofn.lpstrCustomFilter = NULL;
               ofn.nMaxCustFilter = 0;
               if (i)
                  ofn.nFilterIndex = szFilePath[i + 1] == 't' ? 2 : 1;
               else
                  ofn.nFilterIndex = 1;

               ofn.lpstrFile = szFilePath;
               ofn.nMaxFile = sizeof(szFilePath) - 1;
               ofn.lpstrFileTitle = szFileTitle;
               ofn.nMaxFileTitle = sizeof(szFileTitle)-1;

               char szInitDir[256];
               ZeroMemory(szInitDir, 256);
               strncpy(szInitDir, m_szOpenFile, pFile - m_szOpenFile);

               ofn.lpstrInitialDir = szInitDir;
               ofn.lpstrTitle = NULL;
               ofn.nFileOffset = 0;
               ofn.nFileExtension = i;
               ofn.lpstrDefExt = szFileExt;
               ofn.lpfnHook = NULL;
               ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST |
                           OFN_LONGNAMES | OFN_HIDEREADONLY;

               if (!GetSaveFileName (&ofn))
                  return FALSE;

               while (PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE)) {
                  if (!IsDialogMessage(hWnd, &msg)) {
                     TranslateMessage(&msg);
                     DispatchMessage(&msg);
                  }
               }

               ZeroMemory(m_io.szDictFilesDir, sizeof(m_io.szDictFilesDir));
               pszTemp = strrchr(szFilePath, '\\');
               strncpy(m_io.szDictFilesDir, szFilePath, pszTemp - szFilePath);

               pszTemp = strrchr(szFileTitle, '.');

               if (2 == ofn.nFilterIndex && !pszTemp) {
                  LoadString(ghInst, IDS_TEXTFILEEXT, szFileExt, sizeof(szFileExt)-1);
                  // save as text file
                  if (pszTemp)
                     strcpy(pszTemp+1, szFileExt + 2);
                  else
                     strcat(pszTemp, szFileExt+1);
               }

               if (!SendMessage(hWnd, WM_USER + 200, 0, (LPARAM)szFilePath))
                  return FALSE;

               GetWindowText(ghIntermWnd, szTemp, sizeof(szTemp) - 1);
               pszTemp = strstr(szTemp, " - ");
               if (pszTemp)
                  strcpy(pszTemp + 3, szFileTitle);
               else {
                  strcat(szTemp, " - ");
                  strcat(szTemp, szFileTitle);
               }
               if (4 < strlen(szTemp) && '.' == szTemp[strlen(szTemp) - 4])
                  szTemp[strlen(szTemp) - 4] = 0;
               SetWindowText(ghIntermWnd, szTemp);
               
               strcpy(m_szOpenFile, m_io.szDictFilesDir);
               strcat(m_szOpenFile, "\\");
               strcat(m_szOpenFile, szFileTitle);

               m_fFileDirty = FALSE;

               return TRUE;
            }

            case IDM_INTERM_EXIT:
               SendMessage (hWnd, WM_CLOSE, 0, 0);
               return 0;

            case IDM_INTERM_EDITUNDO:
               UndoRestore();
               return 0;

            case IDM_INTERM_EDITCUT: {
               // we gwt here only if there's a selection
               SendMessage(m_hTextWnd, WM_CUT, 0, 0);

               // no more selection so disable cut
               SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCUT, MAKELONG(FALSE, 0));
               SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCOPY, MAKELONG(FALSE, 0));
               // enable paste
               SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITPASTE, MAKELONG(TRUE, 0));
               }
               return 0;

            case ID_FILE_SENDTEXTTO_THECLIPBOARD:
               CopyAllToTheClipboard();
               return 0;

            case IDM_INTERM_EDITCUTALL:
               CutAllToTheClipboard();
               return 0;

            case IDM_INTERM_EDITCOPY: {
               DWORD dwStart, dwEnd;
               SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
               if (dwEnd > dwStart) {
                  SendMessage(m_hTextWnd, WM_COPY, 0, 0);

                  // enable cut and paste
                  SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCUT, MAKELONG(TRUE, 0));
                  SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCOPY, MAKELONG(TRUE, 0));
                  SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITPASTE, MAKELONG(TRUE, 0));
                  return 0;
               }

               // empty the clipboard
               OpenClipboard(NULL);
               EmptyClipboard();
               CloseClipboard();

               // disable cut and paste
               SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCUT, MAKELONG(FALSE, 0));
               SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITCOPY, MAKELONG(FALSE, 0));
               SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDM_INTERM_EDITPASTE, MAKELONG(FALSE, 0));
               }
               return 0;

            case IDM_INTERM_EDITPASTE:
               SendMessage(m_hTextWnd, WM_PASTE, 0, 0);
               return 0;

            case IDM_INTERM_EDITDELETE: {
               UINT uScanCode = MapVirtualKey(VK_DELETE, 0);
               PostMessage(m_hTextWnd, WM_KEYDOWN, VK_DELETE, (LPARAM)(1 | (uScanCode << 16)));
               PostMessage(m_hTextWnd, WM_KEYUP, VK_DELETE, (LPARAM)(1 | (uScanCode << 16) | (3 << 30)));
               }
               return 0;

            case IDM_INTERM_EDITSELECTALL:
               SendMessage(m_hTextWnd, EM_SETSEL, 0, (LPARAM)-1);
               return 0;

            case IDM_TB_INTERM_DICTLISTEN:
               if (::AreWeDictating())
                  SetDictStatus (VSRMODE_DISABLED);
               else
                  SetDictStatus (TurnOnMode());

               return 0;

            case IDM_INTERM_DICTLISTEN:
               SetDictStatus (TurnOnMode());
               return 0;

            case IDM_INTERM_DICTNOTLISTEN:
               SetDictStatus (VSRMODE_DISABLED);
               return 0;

            case IDM_ADDWORD:
               {
               // Shut off SR & turn back on
               DWORD dwMode;
               gpIVDctAttributes->ModeGet(&dwMode);
               gpIVDctAttributes->ModeSet(VSRMODE_DISABLED);

               // If text is highlighted then in add text?
               char  szTemp[128];
               WCHAR szwTemp[128];
               DWORD dwStart, dwEnd;
               szTemp[0] = 0;
               SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
               if (dwEnd - dwStart < 64) {
                  TEXTRANGE tr;
                  memset (&tr, 0, sizeof(tr));
                  tr.lpstrText = szTemp;
                  tr.chrg.cpMin = dwStart;
                  tr.chrg.cpMax = dwEnd;

                  SendMessage(m_hTextWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);

                  char  *pTemp;
                  for (pTemp = szTemp; *pTemp; pTemp++)
                     if (isspace(*pTemp)) {
                        *pTemp = NULL;
                        break;
                     }
               }
               MultiByteToWideChar(CP_ACP, NULL, szTemp, -1, szwTemp, sizeof(szwTemp));


               if (gpILexDlg)
                  gpILexDlg->LexDlg (hWnd, szwTemp[0] ? szwTemp : NULL,
                     STLD_CHANGEPRONADDS, 1033 /* english*/,
                     gpIVDctAttributes, NULL, NULL, NULL, NULL);

               // restore the mode
               SetDictStatus (dwMode);
               }

               break;

            case IDM_MSDICT_CORRECT: {
               // Correction window
               DWORD dwFlags = VDCTGUIF_VISIBLE;
               gpIVDctGui->FlagsGet(&dwFlags);
               BOOL  fCorrection = (dwFlags & VDCTGUIF_VISIBLE) != FALSE;
               CorrectionOnOff(!fCorrection);
            }
            break;

            case IDM_INTERN_SELECTENTIREWORD: {
               m_fSelectEntireWord = !m_fSelectEntireWord;
               
               if (m_fSelectEntireWord && 
                     SendMessage(m_hTextWnd, EM_LINELENGTH, 0, 0)) {
                  DWORD dwStart, dwEnd;
                  // if theres a current selection, adjust it so that it covers entire words
                  AdjustSelection(&dwStart, &dwEnd);
                  DictTextSelSet(dwStart, dwEnd - dwStart);
               }
            }
   			break;

            case IDM_INTERM_HELPTOPICS:
               WinHelp(ghIntermWnd, gszHelpProc4, HELP_FINDER, 0);
            return 0;

            case IDM_INTERM_WHATCANISAY:
               WinHelp(ghIntermWnd, gszHelpProc4, HELP_CONTEXT, 250);
            return 0;

            case IDM_INTERM_WEB_RESEARCH:
            case IDM_INTERM_WEB_HOME: {
               char szBuffer[256];

               if (IDM_INTERM_WEB_RESEARCH == LOWORD(wParam))
                  LoadString(ghInst, IDS_MSRESEARCH, szBuffer, BUFFER-1);
               else
                  LoadString(ghInst, IDS_MSHOME, szBuffer, BUFFER-1);

               ShellExecute(hWnd, NULL, szBuffer, NULL, NULL, SW_SHOWNORMAL);

            }
            return 0;

            case IDM_INTERM_HELPABOUT:
               DialogBox(ghInst, MAKEINTRESOURCE(IDD_ABOUT),
                  hWnd, (DLGPROC)About); 
            return 0;

            case IDM_INTERM_SHOWSETFONT: {
               CHOOSEFONT cf;
               memset(&cf, 0, sizeof(CHOOSEFONT));

      			cf.lStructSize = sizeof(CHOOSEFONT);
		   		cf.hwndOwner = m_hIntermWnd;
   				cf.lpLogFont = &(m_lf);
				   cf.Flags = CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT;

               if (!ChooseFont(&cf))
                  return 0;

               if (m_hfSpoken)
                  DeleteObject(m_hfSpoken);

               m_hfSpoken = CreateFontIndirect(cf.lpLogFont);
               if (m_hfSpoken) {
                  SendMessage(m_hTextWnd, WM_SETFONT, (WPARAM)m_hfSpoken, MAKELPARAM(TRUE, 0));
                  // mark document dirty
                  m_fFileDirty = TRUE;

                  RECT r;
                  GetClientRect(m_hTextWnd, &r);
                  InvalidateRect(m_hTextWnd, &r, TRUE);
                  UpdateWindow(m_hTextWnd);
                  DeleteObject(m_hfSpoken);
                  SendMessage(m_hTextWnd, EM_SETSEL, 0x0FFFFFFF, 0x0FFFFFFF);
                  m_lf = *(cf.lpLogFont);
                  m_lFontHeight = m_lf.lfHeight;
                  if (m_lFontHeight < 0)
                     m_lFontHeight = -(m_lFontHeight);
                  // recalc the min height and width and resize the interm wnd
                  m_bFontChanged = TRUE;
                  ToggleCaptionAndAdjust(m_bHideCaption, m_bHideToolBar);
               }
            }
            return 0;

            default:
               if ((LOWORD(wParam) >= STARTAPPS) && (LOWORD(wParam) < (STARTAPPS + MAXAPPS))) {
                  // menu item
                  char  szTemp[256];
                  GetMenuString (m_hMenuSendTo, LOWORD(wParam),
                     szTemp, sizeof(szTemp),MF_BYCOMMAND);

                  // Cut it to the clipboard and send
                  CopyAllToTheClipboard();
                  SendCtrlVToApp(szTemp);
                  return 0;
               }
               break;
			}
		}
		return 0;

      case WM_MOUSEWHEEL:
         DoMouseWheel (wParam, 0);
         return 0;

      case WM_CLOSE: {
         if (!AskAtClose(IDS_TRANSFERTEXT))
            return 0;
         }
         ExitProc(FALSE);
         ShutDown(FALSE);
         DestroyWindow (hWnd);
         PostQuitMessage(0);
         return 0;

      case WM_DESTROY:
			DeleteObject(hBrush);
			// DeleteObject(hBrButton);
			DeleteObject(hPWhite);
			DeleteObject(hPGrey);
			DeleteObject(hPBlack);
	      DeleteObject(m_hfSpoken);
	      DestroyMenu(m_hLongIntermMenu);
		break;
      
      default:
         if (message == guMSH_MOUSEWHEEL) {
            DoMouseWheel(wParam, 0);
            return 0;
         };

         break;
	}	

   return DefWindowProc(hWnd, message, wParam, lParam);
}


/*******************************************************************
DeleteWithSpace - Delete the selected text, including at least one
   space.

inputs
   HWND     hWnd - window handle
   DWORD    dwStart - start sel
   DWORD    dwEnd - end sel
returns
   none
*/
void DeleteWithSpace (HWND hWnd, DWORD dwStart, DWORD dwEnd)
{
   // include one space after the word
   char szBuf[8];
   TEXTRANGE tr;
   ZeroMemory(szBuf, sizeof(szBuf));
   tr.lpstrText = szBuf;

   if (dwStart) {
      // if we already include a leading or trailing space then 
      // dont do anything
      tr.chrg.cpMin = dwStart;
      tr.chrg.cpMax = dwStart + 1;
      SendMessage(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);

      tr.lpstrText  = szBuf + 2;
      tr.chrg.cpMin = dwEnd - 1;
      tr.chrg.cpMax = dwEnd;
      SendMessage(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);

      if (' ' != szBuf[0] && ' ' != szBuf[2]) {
         tr.lpstrText  = szBuf;
         tr.chrg.cpMin = dwStart - 1;
         tr.chrg.cpMax = dwStart;

         SendMessage(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
         if (' ' == szBuf[0])
            dwStart--;
         else if ('\n' == szBuf[0]) {
            // massage dwEnd
            tr.chrg.cpMin = dwEnd;
            tr.chrg.cpMax = dwEnd + 1;

            SendMessage(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
            if (' ' == szBuf[0])
               dwEnd++;
         }
      }
   } else {
      // massage dwEnd
      tr.chrg.cpMin = dwEnd;
      tr.chrg.cpMax = dwEnd + 1;

      SendMessage(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
      if (' ' == szBuf[0])
         dwEnd++;
   }

   m_bProcessENSelChange = FALSE;
   SendMessage(hWnd, EM_SETSEL, dwStart, dwEnd);
   SendMessage(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"");
   DictTextRemove(dwStart, dwEnd - dwStart, 0);
   DictTextSelSet(dwStart, 0);
   m_bProcessENSelChange = TRUE;
}

LRESULT CALLBACK IntermEditSubClassProc(HWND hWnd, UINT message, 
                                        WPARAM wParam, LPARAM lParam)
{
	static HGLOBAL hglbMem;
	static LPSTR lpstrCopy;
   static BOOL bOverwrite = FALSE;
   DWORD dwStart, dwEnd;
   UINT nCharCode;
   BOOL bTypableChar = FALSE;
   HRESULT hRes;

	switch (message) {
      case WM_MOUSEWHEEL:
         PostMessage(ghIntermWnd, WM_MOUSEWHEEL, wParam, lParam);
         return 0;

      case WM_VSCROLL:
         TellCorrectionOfSelection();
         break;

      case WM_CUT:
         // wParam == VDCT_TEXTCLEAN if the text being cut is good
         // and engine has to be told to adapt
         CutText(wParam);
         return 0;

      case WM_COPY:
         CopyText();
         return 0;

      case WM_PASTE:
         PasteText();
         return 0;

      case WM_DCTPAD_DELETESELECTION:
         SendMessage(hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
         if (!gpIVDctText->Lock()) {
            gpIVDctText->TextRemove(dwStart, dwEnd - dwStart, 0);
            m_bProcessENSelChange = FALSE;
            SendMessage(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"");
            TellCorrectionOfSelection();
            m_bProcessENSelChange = TRUE;
            gpIVDctText->TextSelSet(dwStart, 0);
            gpIVDctText->UnLock();
         }
      return 0;

      case WM_DCTPAD_SETSELECTION: {

         // make sure not in process of shutting down
         if (!gpIVoiceDictation)
            return 0;

         // Posted from VK_UP, HOME, DOWN.. Look there before using this msg elsewhere

         DWORD dwSpokenStart, dwSpokenEnd;
         BOOL b = FALSE;

         SendMessage(hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

         if (IsSpoken(dwStart, &dwSpokenStart, &dwSpokenEnd) &&
             SendMessage(hWnd, EM_LINEFROMCHAR, dwStart, 0) ==
             SendMessage(hWnd, EM_LINEFROMCHAR, dwSpokenStart, 0)) {

            if (dwStart > dwSpokenStart + (dwSpokenEnd/2))
               dwStart = dwSpokenStart + dwSpokenEnd;
            else
               dwStart = dwSpokenStart;
         }

         // The dict buffer is already locked when we get here
         //gpIVDctText->TextSelSet(dwStart, dwEnd - dwStart);

         SendMessage(m_hTextWnd, EM_SETSEL, dwStart, dwStart);

         gpIVDctText->TextSelSet(dwStart, 0);
         gpIVDctText->UnLock();
         m_bProcessENSelChange = TRUE;
         TellCorrectionOfSelection();
      }
      return 0;
      
      case WM_CHAR: {
         if (27 == wParam) // escape
            return 0;

//         if (FixSynchProblem(hWnd))
//            return 0;

         SHORT nCode = VkKeyScan(wParam);
         SendMessage(hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
         // ANSI character codes
         //if (wParam == 10 || // LF
         //    wParam == 13 || // CR
         if (wParam >= ' ' && wParam <= '~' ||
             wParam == '\t' ||
             wParam >= 130 && wParam <= 140 ||
             wParam >= 145 && wParam <= 156 ||
             wParam >= 159 && wParam <= 255)
            TransmitTypedLetter(wParam, dwStart, dwEnd);

         // richedit gives only CR, no LF
         
         if (VK_RETURN == nCode)
            // when we get carriage return stuff a line feed too
            DictTextSet(dwStart, 0, "\r\n", 0);
      }
      break;

		case WM_KEYDOWN: {
         if (VK_INSERT == wParam)
            return 0;

         SendMessage(m_hTextWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

         // if we are in overwrite mode and there's no current selection and one of
         // the typable chars is typed set sel to next char which would be overwritten

         nCharCode = MapVirtualKey(wParam, 2);
         // ANSI char codes
         if (LOWORD(nCharCode) == 10 || // LF
             LOWORD(nCharCode) == 13 || // CR 
             LOWORD(nCharCode) >= ' ' && LOWORD(nCharCode) <= '~' ||
             LOWORD(nCharCode) >= 130 && LOWORD(nCharCode) <= 140 ||
             LOWORD(nCharCode) >= 145 && LOWORD(nCharCode) <= 156 ||
             LOWORD(nCharCode) >= 159 && LOWORD(nCharCode) <= 255) {
            bTypableChar = TRUE;
         }

         BOOL bDctPadEmpty = TRUE;

         if (GetWindowTextLength(hWnd))
            bDctPadEmpty = FALSE;

         switch (wParam) {
            case VK_LEFT: {
               if (bDctPadEmpty)
                  return 0;
               if (!m_fSelectEntireWord)
                  break;

               DWORD dwNewStart = (DWORD)FindNextPrevAtom(dwStart, FALSE);

               if (0x8000 & GetKeyState(VK_SHIFT)) {
                  SendMessage(hWnd, EM_SETSEL, dwNewStart, dwEnd);
               }
               else {
                  // if there's a selection remove it
                     dwStart = dwNewStart;

                  SendMessage(hWnd, EM_SETSEL, dwStart, dwStart);
               }

               TellCorrectionOfSelection();
            }
            return 0;

            case VK_RIGHT: {
               if (bDctPadEmpty)
                  return 0;
               if (!(m_fSelectEntireWord) || 
                    (dwStart == (DWORD)GetWindowTextLength(hWnd) && dwEnd == dwStart))
                  break;

               DWORD dwNewEnd = (DWORD)FindNextPrevAtom(dwEnd, TRUE);

               if (0x8000 & GetKeyState(VK_SHIFT)) {
                  SendMessage(hWnd, EM_SETSEL, dwStart, dwNewEnd);
               }
               else {
                  //if (dwEnd > dwStart) {
                     //if (!IsSpoken(dwEnd, NULL, NULL))
                     //   dwEnd = dwNewEnd;
                  //}
                  //else
                     dwEnd = dwNewEnd;

                  SendMessage(hWnd, EM_SETSEL, dwEnd, dwEnd);
               }
               
               TellCorrectionOfSelection();
            }
            return 0;

            case VK_UP:
            case VK_DOWN:
            case VK_HOME:
               if (bDctPadEmpty)
                  return 0;

               if (!m_fSelectEntireWord)
                  break;

               if (0x8000 & GetKeyState(VK_SHIFT)) {
                  m_bAdjustSelection = TRUE;
                  break;

#if 0
                  SendMessage(hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
                  if (dwEnd > dwStart) {
                     // if we've got a key down, and the end of the selection
                     // if a \r or \n, then just break and elt this throug
                     // after getting rid of adjustselection
                     if (VK_DOWN == wParam) {
                        char  szTemp[3];
                        TEXTRANGE tr;
                        memset (&tr, 0, sizeof(tr));
                        tr.lpstrText = szTemp;
                        tr.chrg.cpMin = dwEnd-1;
                        tr.chrg.cpMax = dwEnd;

                        SendMessage(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
                        if ((szTemp[0] == '\n') || (szTemp[0] == '\r')) {
                           m_bAdjustSelection = FALSE;
                           break;
                        }
                     }

                     if (VK_UP == wParam)
                        SendMessage (m_hTextWnd, EM_SETSEL, dwEnd, dwStart);
                     else
                        SendMessage (m_hTextWnd, EM_SETSEL, dwStart, dwEnd);
                  }

                  if (VK_UP == wParam)
                     gfKeyUp = TRUE;

                  break;
#endif // 0
               }

               // Lock the dict buffer here so that it does not change on us between here
               // and the time we receive WM_DctPad_SETSELECTION
               if (!gpIVDctText->Lock()) {
                  DWORD dwStart, dwEnd;

                  SendMessage(hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

                  m_bProcessENSelChange = FALSE; // so that EN_SELCHANGE is not processed

                  PostMessage(hWnd, WM_DCTPAD_SETSELECTION, wParam, lParam);
               }
            break;
           
            case VK_BACK: {
               if (bDctPadEmpty)
                  return 0;

               if (dwEnd > dwStart) {
                  // if theres already a selection backspace only deletes it regradless of
                  // the mode we are in - word or character
                  DeleteWithSpace (hWnd, dwStart, dwEnd);
                  return 0;
               }

               if (!(m_fSelectEntireWord)) {
                  // character mode

                  if (1 == dwStart) {
                     DictTextRemove(0, 1, NULL);
                     break;
                  }

                  // if we are backspacing a new line delete 2 chars in internal buffer
                  SDATA dData;
                  if (dwStart - 1  && !gpIVDctText->Lock()) {
                     hRes = gpIVDctText->TextGet(dwStart - 2, 2, &dData);

                     if (!hRes && dData.pData) {
                        if (((LPSTR)(dData.pData))[0] == '\r' && ((LPSTR)(dData.pData))[1] == '\n')
                           gpIVDctText->TextRemove(dwStart-2, 2, 0);
                        else
                           gpIVDctText->TextRemove(dwStart-1, 1, NULL);

                        CoTaskMemFree(dData.pData);
                     }

                     gpIVDctText->UnLock();
                  }
                  break;
               }
               
               // word mode
               
               
               
               
               DWORD d1, d2;

               dwStart = (DWORD)FindNextPrevAtom(dwStart, FALSE);
               if (IsSpoken(dwStart, &d1, &d2)) {
                  dwStart = d1;

                  dwEnd = d1 + d2;
                  if (dwStart) {
                     DWORD dwLine = SendMessage(hWnd, EM_LINEFROMCHAR, dwStart, 0);
                     if (dwStart == (DWORD)SendMessage(hWnd, EM_LINEINDEX, dwLine, 0)) {
                        // we are at start of line and it is not first line
                        SDATA dData;
                        if (!gpIVDctText->Lock()) {
                           hRes = gpIVDctText->TextGet(dwStart-2, 2, &dData);

                           if (!hRes && dData.pData) {
                              if (((LPSTR)(dData.pData))[0] == '\r' && 
                                  ((LPSTR)(dData.pData))[1] == '\n') {
                                 // return - remove the spaces after the word being deleted
                                 dwEnd = SendMessage(hWnd, EM_FINDWORDBREAK, WB_MOVEWORDRIGHT, dwEnd);
                              } else
                                 // wordbreak - include the previous space
                                 dwStart--;

                              CoTaskMemFree(dData.pData);
                           }
                           gpIVDctText->UnLock();
                        }
                     }
                  } else 
                     dwEnd = SendMessage(hWnd, EM_FINDWORDBREAK, WB_MOVEWORDRIGHT, dwEnd);
               }

               if ((0x8000 & GetKeyState(VK_SHIFT)) && IsSpoken(dwStart, NULL, NULL)) {
                  // if SHIFT is pressed set the selection and exit

                  m_bProcessENSelChange = FALSE;
                  SendMessage(hWnd, EM_SETSEL, dwStart, dwEnd);
                  m_bProcessENSelChange = TRUE;

                  DictTextSelSet(dwStart, dwEnd - dwStart);
                  return 0;
               }

               DWORD dwLine = SendMessage(hWnd, EM_LINEFROMCHAR, dwStart, 0);

               if (dwStart != (DWORD)SendMessage(hWnd, EM_LINEINDEX, dwLine, 0) &&
                   SendMessage(hWnd, EM_FINDWORDBREAK, WB_ISDELIMITER, dwStart-1) &&
                   IsSpoken(dwStart, 0, 0)) {
                  // dwStart is not the start of line AND
                  // dwStart is at start of word

                  // delete the spaces till the the word preceding to dwStart

                  dwStart = (DWORD)FindNextPrevAtom(dwStart, FALSE);
                  if (IsSpoken(dwStart, &d1, &d2))
                     dwStart = d1+d2;
               }

               m_bProcessENSelChange = FALSE;
               SendMessage(hWnd, EM_SETSEL, dwStart, dwEnd);
               m_bProcessENSelChange = TRUE;

                  
               PostMessage(hWnd, WM_DCTPAD_DELETESELECTION, 0, 0);
            }
            return 0;

            case VK_DELETE: {
               if (bDctPadEmpty)
                  return 0;

               // if there's a selection, delete it and return
               if (dwEnd > dwStart) {
                  DeleteWithSpace (hWnd, dwStart, dwEnd);
                  return 0;
               }


               if (!(m_fSelectEntireWord)) {
                  // if we are deleting a new line delete 2 chars in internal buffer
                  SDATA dData;
                  if (!gpIVDctText->Lock()) {
                     hRes = gpIVDctText->TextGet(dwStart, 2, &dData);

                     if (!hRes && dData.pData) {
                        if (((LPSTR)(dData.pData))[0] == '\r' && ((LPSTR)(dData.pData))[1] == '\n')
                           gpIVDctText->TextRemove(dwStart, 2, 0);
                        else
                           gpIVDctText->TextRemove(dwStart, 1, NULL);

                        CoTaskMemFree(dData.pData);
                     }

                     gpIVDctText->UnLock();
                  }
                  break;
               }

               DWORD d1, d2, d3;

               d1 = (DWORD)GetWindowTextLength(hWnd);
               if (d1 == dwStart)
                  break;

               //if (SendMessage(hWnd, EM_FINDWORDBREAK, WB_ISDELIMITER, dwStart)) {
               //   if (dwStart + 1 <= d1)
               //      dwStart++;
               //}

               if (IsSpoken(dwStart, &d1, &d2)) {
                  SDATA dData;
                  BOOL fNewLine = FALSE;
                  BOOL fText = FALSE;
  
                  if (!gpIVDctText->Lock()) {
                     hRes = gpIVDctText->TextGet(dwStart, d1 + d2 - dwStart, &dData);

                     if (!hRes && dData.pData) {
                        for (DWORD d = 0; d < d1 + d2 - dwStart; d++) {
                           switch (((LPSTR)(dData.pData))[d]) {
                              case '\r':
                              case '\n':
                                 fNewLine = TRUE;
                                 break;

                              case ' ':
                                 break;

                              default:
                                 // text
                                 fText = TRUE;
                                 break;
                           }
                           if (fNewLine || fText)
                              break;
                        }
                        CoTaskMemFree(dData.pData);
                     }
                     gpIVDctText->UnLock();
                  }
                  
                  if (!fNewLine && !fText) {
                     dwStart = d1 + d2;
                     d1 = FindNextPrevAtom(d1, TRUE);
                     if (d1 > dwStart && IsSpoken(d1, &d2, &d3))
                        dwEnd = d2 + d3;
                     else
                        dwEnd = dwStart + 1;
                  } else {
                     dwStart = d1;
                     dwEnd = d1 + d2;

                     if (fText) {
                        d1 = FindNextPrevAtom(d1, FALSE);
                        if (IsSpoken(d1, &d2, &d3)) {
                           if (d1 < dwStart)
                              dwStart = d2+d3;
                        }
                        else if (SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_ISDELIMITER, d1))
                           dwStart = d1; // take atleast one preceding space

                        // if dwStart is at the start of the first sentence or 
                        // at the start of sentence and a new-line precedes dwStart the delete
                        // the spaces after dwEnd

                        if (!dwStart)
                           dwEnd = SendMessage(hWnd, EM_FINDWORDBREAK, WB_MOVEWORDRIGHT, dwEnd);
                        else {
                           SDATA dData;
                           if (!gpIVDctText->Lock()) {
                              hRes = gpIVDctText->TextGet(dwStart-2, 2, &dData);

                              if (!hRes && dData.pData) {
                                 if (((LPSTR)(dData.pData))[0] == '\r' && 
                                     ((LPSTR)(dData.pData))[1] == '\n') {
                                    dwEnd = SendMessage(hWnd, EM_FINDWORDBREAK, WB_MOVEWORDRIGHT, dwEnd);
                                 }

                                 CoTaskMemFree(dData.pData);
                              }
                              gpIVDctText->UnLock();
                           }
                        }
                     }
                  }
               } else {
                  if (SendMessage(m_hTextWnd, EM_FINDWORDBREAK, WB_ISDELIMITER, dwStart)) {
                     d1 = FindNextPrevAtom(dwStart, TRUE);
                     if (IsSpoken(d1, &d2, &d3)) {
                        dwEnd = d2+d3;
                        d1 = FindNextPrevAtom(dwStart, FALSE);
                        if (d1 < dwStart && IsSpoken(d1, &d2, &d3))
                           dwStart = d2 + d3;
                     }
                     else
                        dwEnd = d1;
                  }
                  else
                     dwEnd = FindNextPrevAtom(dwStart, TRUE);
               }

               m_bProcessENSelChange = FALSE;
               SendMessage(hWnd, EM_SETSEL, dwStart, dwEnd);
               m_bProcessENSelChange = TRUE;

               // WM_DctPad_DELETESELECTION does DicTextRemove and deletes the selection
               // in edit control
               PostMessage(hWnd, WM_DCTPAD_DELETESELECTION, 0, 0);
            }   
            return 0;
         }
      }
      break;

      case WM_MBUTTONDBLCLK:
         // Toggle showing/hiding the correction window
         // FixSynchProblem(hWnd);
         SendMessage(ghIntermWnd, WM_COMMAND, IDM_MSDICT_CORRECT, 0);
         return 0;

      case WM_RBUTTONDOWN:
      {   
         if (!GetWindowTextLength(hWnd))
             return 0;
         
         // FixSynchProblem(hWnd);

         // show the correction window
         DWORD dwFlags;
         if (!gpIVDctGui->FlagsGet(&dwFlags))
            if (!(dwFlags & VDCTGUIF_VISIBLE))
               gpIVDctGui->FlagsSet(dwFlags | VDCTGUIF_VISIBLE);

         DWORD   dwChar;
         POINT p;
         p.x = LOWORD(lParam);
         p.y = HIWORD(lParam);
         dwChar = SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM) &p);

         SendMessage(hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

         // if we're clicking within the existing sleection then don't
         // change it
         if ((dwChar >= dwStart) && (dwChar <= dwEnd))
            break;

         if (dwStart == dwEnd) {
            DWORD d1, d2;

            if (IsSpoken(dwStart, &d1, &d2)) {
               dwStart = d1;
               dwEnd = d1 + d2;
            }
         }

         if (dwChar < dwStart || dwChar > dwEnd)
            m_dwRank = 1;

         m_bAdjustSelection = TRUE;
         SendMessage(m_hTextWnd, EM_SETSEL, dwChar, dwChar);
      }
      break;

      case WM_LBUTTONUP:
      case WM_NCLBUTTONUP:
#ifdef _DEBUG
         OutputDebugString ("WM_LBUTTONUP\r\n");
#endif
         break;

      case WM_LBUTTONDOWN: {
         if (!GetWindowTextLength(hWnd))
             return 0;
         
         // FixSynchProblem(hWnd);

#ifdef _DEBUG
         OutputDebugString ("WM_LBUTTONDOWN\r\n");
#endif

         POINT p = {LOWORD(lParam), HIWORD(lParam)};
         gdwlButtondownPos = SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM) &p);
         m_bAdjustSelection = TRUE;

         SendMessage(hWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
         if (dwStart == dwEnd) {
            DWORD d1, d2;

            if (IsSpoken(dwStart, &d1, &d2)) {
               dwStart = d1;
               dwEnd = d1 + d2;
            }
         }

         if (gdwlButtondownPos < dwStart || gdwlButtondownPos > dwEnd)
            m_dwRank = 1;
      }
      break;
	}

	return CallWindowProc(m_OrigEditProc, hWnd, message, wParam, lParam);
}


/**************************************************************************
 *  Dictation Notification
 **************************************************************************/

CIVDctNotifySink::CIVDctNotifySink (void)
{
    m_cRef = 0;
    m_wLastVU = 0;
    m_fInUtterance = FALSE;
}

CIVDctNotifySink::~CIVDctNotifySink (void)
{
// this space intentionally left blank
}

STDMETHODIMP CIVDctNotifySink::QueryInterface (REFIID riid, LPVOID *ppv)
{
    *ppv = NULL;

    /* always return our IUnkown for IID_IUnknown */
    if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid,IID_IVDctNotifySink) ||
        IsEqualIID(riid,IID_IVDctNotifySink2)) {
        *ppv = (LPVOID) this;
        AddRef();
        return NOERROR;
    }

    // otherwise, cant find
    return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CIVDctNotifySink::AddRef (void)
{
   return ++m_cRef;
}

STDMETHODIMP_(ULONG) CIVDctNotifySink::Release (void)
{
   m_cRef--;

   if (!m_cRef)
      delete this;

   return m_cRef;
};


STDMETHODIMP CIVDctNotifySink::CommandBuiltIn(PTSTR pCommand)
{
   return NOERROR;
}


STDMETHODIMP CIVDctNotifySink::CommandOther(PTSTR pCommand)
{
    return NOERROR;
}


STDMETHODIMP CIVDctNotifySink::CommandRecognize(DWORD dwID, DWORD dwFlags,
                                                DWORD dwActionSize,
                                                PVOID pAction, PTSTR pCommand)
{
   // make sure not in process of shutting down
   if (!gpIVoiceDictation)
      return NOERROR;

   PVOID pMem = NULL;

   // allocate the action to memory if it's there
   if (pAction && dwActionSize)
      pMem = malloc (dwActionSize);
   if (pMem)
      memcpy (pMem, pAction, dwActionSize);

   PostMessage (m_hIntermWnd, WM_APPCOMMAND, dwID, (LPARAM) pMem);

   return NOERROR;
}


STDMETHODIMP CIVDctNotifySink::TextSelChanged(void)
{
   // make sure not in process of shutting down
   if (!gpIVoiceDictation)
      return NOERROR;

   // This is called when the selection for text in the VTB (virtual text box)
   // changes
   PostMessage(ghIntermWnd, WM_USER + 865, 0, 0);

#ifdef _DEBUG
   OutputDebugString ("TextSelChanged\r\n");
#endif


   return NOERROR;
}


STDMETHODIMP CIVDctNotifySink::TextChanged(DWORD dwReason)
{
   // make sure not in process of shutting down
   if (!gpIVoiceDictation)
      return NOERROR;

	// Called when text changes in the VTB
   PostMessage(m_hIntermWnd, WM_USER+863, dwReason, 0);
#ifdef _DEBUG
   OutputDebugString ("TextChanged\r\n");
#endif

   return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::TextBookmarkChanged(DWORD dwID)
{
// Called when a bookmark in the VTB changes
    return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::PhraseStart (void)
{
// Called when SR engine begins processing a phrase
    return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::PhraseFinish (DWORD dwFlag, PSRPHRASE pSRPhrase)
{
   if (!gpIVoiceDictation)
       return NOERROR;
    
// Called when SR engine is done processing a phrase
    return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::PhraseHypothesis (DWORD, PSRPHRASE)
{
// Called when SR engine has other possibilities for the recognition.
    return NOERROR;
}


STDMETHODIMP CIVDctNotifySink::AttribChanged(DWORD dwAttribute)
{
// Called when an attribute changes. Right now we only care about the
// enabled state of dictation.

    if (!gpIVoiceDictation || gfDontNotify) {
       gfDontNotify = FALSE;
       return NOERROR;
    }
       
    if (dwAttribute & IVCNSAC_ENABLED)
        PostMessage(m_hIntermWnd, WM_USER+864, 0, 0);
    return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::Interference(DWORD dwType)
{
// Called when audio interference is happening. We don't care.

   return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::Training(DWORD dwTrain)
{
// Called when training is happening
    return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::Dictating(PCSTR pszApp, BOOL fDictating)
{
// Called when an app starts or stops dictating
    return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::UtteranceBegin(void)
{
   // make sure not in process of shutting down
   if (!gpIVoiceDictation)
      return NOERROR;


   m_fInUtterance = TRUE;

   SendMessage(m_hToolBar, TB_CHANGEBITMAP, IDM_TB_INTERM_DICTLISTEN, MAKELPARAM(IDBT_TB_UTTERANCE, 0));
   return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::UtteranceEnd()
{
// Called when an utterance finishes. We don't care

   m_fInUtterance = FALSE;

   if (!gpIVoiceDictation)
       return NOERROR;

   AreWeDictating();
   SetDictationIcon();

   return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::VUMeter(WORD wLevel)
{
#define  VULEVEL  0x800 // if the VU is more than this then show utterance
   // make sure not in process of shutting down
   if (!gpIVoiceDictation)
      return NOERROR;


   // first of all, if in utterance already then do nothing
   if (m_fInUtterance) {
      m_wLastVU = wLevel;
      return NOERROR;
   }

   // if was quiet before, and now is loud, then update icon
   if ((m_wLastVU < VULEVEL) && (wLevel >= VULEVEL)) {
      SendMessage(m_hToolBar, TB_CHANGEBITMAP, IDM_TB_INTERM_DICTLISTEN, MAKELPARAM(IDBT_TB_UTTERANCE, 0));

      m_wLastVU = wLevel;
      return NOERROR;
   }

   if ((m_wLastVU >= VULEVEL) && (wLevel < VULEVEL)) {
      // simulate an utterance end, but since we've already got the
      // code written in utterance end, just call that
      UtteranceEnd();

      m_wLastVU = wLevel;
      return NOERROR;
   }

    return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::Error(LPUNKNOWN pUnk)
{
   return NOERROR;
}

STDMETHODIMP CIVDctNotifySink::Warning(LPUNKNOWN pUnk)
{
   return NOERROR;
}



/*****************************************************************************
*
* IsDctPadEmpty
*
* returns TRUE if there's text in DctPad, else FALSE
*
******************************************************************************/

BOOL IsDctPadEmpty(void)
{
   return (GetWindowTextLength(m_hTextWnd));
}
