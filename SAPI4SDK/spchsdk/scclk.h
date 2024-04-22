/******************************************************************************\
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <mmsystem.h>
#include <time.h>

#ifndef _SPEECH_
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <speech.h>
#endif  // _SPEECH_


const UINT MAXFILENAME= 256;
const UINT MAXSTRLEN=   256;
const UINT MAXCITYLEN=  100;

const UINT RECOG_GLOBAL= 0;
const UINT RECOG_WINDOW= 1;
const UINT RECOG_OFF=    3;

const UINT DIBSIZE=    128;
const UINT IDM_ACTUAL=  20;
const UINT IDM_STRETCH= 21;

const UINT SRTXT_UTTER= 0;
const UINT SRTXT_CMD=   1;
const UINT SRTXT_RESULT=2;
const UINT SRTXT_TIME=  3;

const UINT WM_USERSPEAK=WM_USER+10;
const UINT ID_CLKTIMER =1;
const UINT ID_WINTITLETIMER=2;

const UINT ID_CLKOPTBUT=800;
const UINT ID_SROPTBUT= 801;


typedef struct
{
	time_t t; 
	int    hours, minutes, seconds;
	int    pm;
} ClkTime_t;

typedef struct
{
	int x,y,xsize,ysize;
} PosSize_t;

typedef struct
{
	char  szStrTitle[20];
	char  szStrOut[80];
	POINT pos;
	RECT  rBox;
} SRTextOut;

typedef struct
{
	long lTmBias;
	char szCity[MAXCITYLEN];
} CityList_t;

extern char   *szWinTitle;
extern HWND   ghWndMain;
extern HINSTANCE hinst;
extern BOOL   bWidthFlag;
extern POINT  scrnsize;
extern RECT   rClient;
extern POINT  TextPos;
extern int    nCmds;
extern TEXTMETRIC TxtInfo;
extern SRTextOut srTxt[];
extern HPEN blkpen, whtpen, ltpen, dkpen;
extern int BitsPerPixel;
extern POINT BorderSize;
extern BOOL  bVMeterFlag;
extern TIME_ZONE_INFORMATION TimeZoneInfo;
extern UINT WM_REG_SPEECHSTARTED;
extern UINT WM_REG_SPEECHENDED;



LONG CALLBACK MainWndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
HRESULT InitState (char *pszCmdFileName);
void TextOutSR( HDC hdc, int index );
void GetClkTime( HWND hwnd, long TmBias, ClkTime_t *clktime );
HRESULT SpeakString( char *szSpeakStr );
HRESULT ActivateSR( int Flag );
void ActivateSRdialog();
void PaintVMeter( WORD wLevel );
void ToggleView( HWND hwnd );
