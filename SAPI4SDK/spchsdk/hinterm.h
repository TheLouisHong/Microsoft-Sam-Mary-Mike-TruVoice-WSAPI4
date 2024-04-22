#ifndef _INTERMH_
#define _INTERMH_

#include "zmouse.h"

extern HINSTANCE           ghInst;
extern HWND                ghIntermWnd;
extern UINT                guMSH_MOUSEWHEEL;
extern char                gszAppName[];

void ShutDown(BOOL bMode);


BOOL AreWeDictating (void);
void DictationOnOff (BOOL, DWORD);
void CorrectionOnOff (BOOL);
int MessageBoxonTop(HWND, DWORD, DWORD);

extern PIVOICEDICTATION      gpIVoiceDictation;
extern LPCLASSFACTORY        gpIVDctCF;
extern PIVDCTDIALOGS         gpIVDctDialogs;
extern PIVDCTATTRIBUTES      gpIVDctAttributes;
extern PIVDCTGUI             gpIVDctGui;
extern PIVDCTTEXT            gpIVDctText; 
extern PISRSPEAKER           gpIVDctSpeaker; 
extern PIVDCTTEXTCACHE       gpIVDctTextCache; 
extern PIVDCTGLOSSARY        gpIVDctGlossary;
extern PIVDCTCOMMANDS        gpIVDctCommands;
extern LPSTREAM              gpIMarshalStream;

BOOL IsDctPadEmpty(void);

typedef struct {
   WORD     wLeft;
   WORD     wTop;
   WORD     wWidth;
   WORD     wHeight;
   BOOL     fShow;
   BOOL     bHideCaption;
   BOOL     bHideToolBar;
   BOOL     bOnTop;
   LOGFONT  lf;
   DWORD    showCmd;
   BOOL     fSelectEntireWord;
   char     szDictFilesDir[MAX_PATH];
} INTERMOPTIONS, *PINTERMOPTIONS;

#endif
