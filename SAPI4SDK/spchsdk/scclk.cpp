/******************************************************************************
*       SRCLK - A program which demonstrates the useage of the Voice Commands
*       and Voice Text interfaces of the Microsoft Speech API.
*
*       Recognizes:
*         "Toggle View"
*         "What is the date?"
*         "What time is it?"
*         "What time is it in <City>?".
*
*       SRClk uses srclk.dat for the city names and time zones bias values.
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
#include "srclk.h"
#include "clkhands.h"
#include "srclkdlg.h"
#include "showdib.h"
#include "resource.h"
#include <stdlib.h>
#include <assert.h>

const UINT MAX_OUTTEXT=      4;
const UINT TIMER_1SEC=       1000;
const UINT TIMER_5SEC=       5000;
const UINT TIMER_30SEC=      30000;
const UINT IDM_MENUWIDEVIEW= 1;
const UINT IDM_MENUCLKOPTS=  2;
const UINT IDM_MENUSROPTS=   3;
const UINT IDM_MENUABOUT=    4;

// functions local to this module only...
void InitSRTextOut( HWND hWnd );
void PaintTextBox( HDC hdc );
void PaintVMeterBox( HDC hdc );
void MoveBoxes();
void ProcessString( ClkTime_t *clktime, BOOL bAction );
void ClearTextRect( HDC hdc, int i );
void InitColors();
void DeleteColors();
void SpeakContinuousTime( ClkTime_t *clktime );
void InitSystemMenu( HWND hwnd );
void PaintVMeter( WORD wLevel );

HWND ghWndMain = NULL;				// global main window handle
HWND SROptions, ClkOptions;			// handles for options buttons
TIME_ZONE_INFORMATION TimeZoneInfo;	// local system time zone information

// registered messages for obtaining speech state changes...
UINT WM_REG_SPEECHSTARTED;
UINT WM_REG_SPEECHENDED;

//long TmBias[MAX_TMBIAS];			// list of time zone bias values

char       *szWinTitle="SRClk";		// main window title
char       *szCmdFileName="srclk.dat";	// file containing city/timezone info
BOOL       bWidthFlag=FALSE;        // flag controlling window width/view
TEXTMETRIC TxtInfo;                 // text font info
HINSTANCE  hinst;                   // app instance
POINT      scrnsize;                // dimensions of the physical screen
RECT       rClient, rWin, rVMeter;  // client, window, and view meter bounding rectangles
POINT      TextPos;	                // positions of text information in wide view
SRTextOut  srTxt[MAX_OUTTEXT];      // wide view text data
PosSize_t  SRButPos, ClkButPos;     // clock/sr options button positions
int        BitsPerPixel;            // color resolution of current video mode
int        SRFlag = RECOG_GLOBAL;   // controls recognition active status
BOOL       bWndOnTop = FALSE;       // control for keeping window topmost
BOOL       bWndTitleBar = TRUE;     // window title on/off
BOOL       bVMeterFlag = TRUE;	    // view meter on/off
int        TimeContOpt=IDC_RADIOTIMEOFF;  // Ctrl variable to speak the time continuously
POINT      OrigSize, CurSize, BorderSize; // window size structures

HBRUSH ltbrush, redbrush, grnbrush, ylwbrush, dkbrush;
HPEN   blkpen, whtpen, ltpen, dkpen, blupen, nulpen;

char *monthstr[]={
"January","February","March","April","May","June","July",
"August","September","October","November","December"};

/*
Main window procedure
*/
LONG CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int retval=0;
	ClkTime_t clktime;

	// check for registered speech messages,
	// we don't do anything with them but this is where you check them...
	if( message == WM_REG_SPEECHSTARTED )
	{
		// you would obtain your speech interfaces here...
		SetWindowText( hWnd, "Speech on." );
		SetTimer( hWnd, ID_WINTITLETIMER, TIMER_5SEC, NULL );
	}
	else if( message == WM_REG_SPEECHENDED )
	{
		// you would release your speech interfaces here...
		SetWindowText( hWnd, "Speech off." ); 
		SetTimer( hWnd, ID_WINTITLETIMER, TIMER_5SEC, NULL );
	}

	switch( message )
	{
		// initialize variables
		case WM_CREATE:

			ghWndMain = hWnd;

			// load/display the clock bitmap...
			if( LoadDIB( hWnd ) )
			{
				DestroyWindow(hWnd);
				break;
			}

			// get client and window rectangles and store the info...
			GetClientRect( hWnd, &rClient );
			GetWindowRect( hWnd, &rWin );
			OrigSize.x = CurSize.x = rWin.right - rWin.left;
			OrigSize.y = CurSize.y = rWin.bottom - rWin.top;

			// get our system time zone bias (and other info)...
			GetTimeZoneInformation( &TimeZoneInfo );

			// modify the system menu to contain our own items as well...
			InitSystemMenu( hWnd );

			// create our drawing pens/brushes...
			InitColors();

			// init our text strings and rectangles...
			InitSRTextOut( hWnd );

			// init the clock hands polygons...
			InitClkHands();

			// init OLE and the VCMD objects/interfaces...
			if( InitState( szCmdFileName ) )
			{
				DestroyWindow( hWnd );
				break;
			}

			// set timer to 30 seconds initially, we do this so that the clock hands
			// will be updated on time.
			// When the widw view is selected, the time updates every second to
			// draw the second hand...
			SetTimer( hWnd, ID_CLKTIMER, TIMER_30SEC, NULL );
			break;
			
		case WM_SIZE:
			rClient.right = LOWORD(lparam);
			rClient.bottom= HIWORD(lparam);
			MoveBoxes();
			break;

		case WM_PALETTECHANGED:
			if( wparam == (WPARAM)hWnd ) break;
			
		case WM_QUERYNEWPALETTE:
			return OnPaletteChanged( hWnd, wparam );

		// paint the clock face, hands, vumeter, and text strings...
		case WM_PAINT:
			GetClkTime( hWnd, TimeZoneInfo.Bias, &clktime );
			hdc = BeginPaint( hWnd, &ps );
			PaintClock( hWnd, hdc );
			DrawClkHands( hdc, &clktime, bWidthFlag );
			PaintVMeterBox( hdc );
			PaintTextBox( hdc );
			TextOutSR( hdc, -1 );
			EndPaint( hWnd, &ps );
			break;

		// the time is used when the user wants the time spoken in intervals,
		// also, the second hand is updated every second while in wide view mode...
		case WM_TIMER:
			if( wparam == ID_WINTITLETIMER )
			{
				SetWindowText( hWnd, szWinTitle );
				KillTimer( hWnd, ID_WINTITLETIMER );
				break;
			}
			hdc = GetDC( hWnd );
			GetClkTime( hWnd, TimeZoneInfo.Bias, &clktime );
			wsprintf( srTxt[SRTXT_TIME].szStrOut, "%2d:%02d:%02d  ", clktime.hours, clktime.minutes, clktime.seconds );
			TextOutSR( hdc, SRTXT_TIME );
			PaintClock( hWnd, hdc );
			DrawClkHands( hdc, &clktime, bWidthFlag );
			ReleaseDC( hWnd, hdc );
			SpeakContinuousTime( &clktime );
			break;

		// tell the user the time...
		case WM_USERSPEAK:
			// get the time based on where the user wants it
			// wparam contains a flag determining date or time
			// lpraram contains the TMBias value for the time...
			GetClkTime( hWnd, (long)lparam, &clktime );
			ProcessString( &clktime, (BOOL)wparam );

			TextOutSR( NULL, SRTXT_RESULT );
			SpeakString( srTxt[SRTXT_RESULT].szStrOut );
			break;

		// handle this so we can allow the user to drag the window without a
		// title bar...
		case WM_NCHITTEST:
			wparam = DefWindowProc(hWnd, message, wparam, lparam);
			if( !bWndTitleBar && wparam == HTCLIENT )
				return HTCAPTION;
			else
				return wparam;

		// restore the title bar if it's not there...
		case WM_NCLBUTTONDBLCLK:
			// if title bar, then no action...
			if( bWndTitleBar ) break;

			// otherwise put the title bar back by
			// falling thru to under WM_LBUTTONDBLCLK...

		// toggle the title bar on/off...
		case WM_LBUTTONDBLCLK:
			ToggleTitleBar( hWnd, bWndTitleBar=!bWndTitleBar );
			return TRUE;

		// an options button was pressed, present the appropriate dialog...
		case WM_COMMAND:
			switch( wparam )
			{
				case ID_CLKOPTBUT:
					ClockOptionsDialog( hWnd );
					break;

				case ID_SROPTBUT:
					ActivateSRdialog();
					break;
			}
			break;

		case WM_SYSCOMMAND:
			switch( wparam )
			{
				case IDM_MENUWIDEVIEW:
					ToggleView( hWnd );
					break;

				case IDM_MENUCLKOPTS:
					ClockOptionsDialog( hWnd );
					break;

				case IDM_MENUSROPTS:
					ActivateSRdialog();
					break;

				case IDM_MENUABOUT:
					AboutDialog( hWnd );
					break;
				default:
					return DefWindowProc(hWnd, message, wparam, lparam);
			}
			break;
			
			// bye...			
		case WM_DESTROY:
			KillTimer( hWnd, ID_CLKTIMER );
			FreeDIB();
			DeleteColors();
			PostQuitMessage( 0 );
			break;

		default:
			return DefWindowProc(hWnd, message, wparam, lparam);
    }
    return retval;
}

/*
 Get the current time using the time zone info (obtained from the data file)
 and and parse it.

 Input:

 hWnd    - main window handle
 TmBias  - Time zone bias of target city obtained from data file

 Output:

 clktime - filled with the current localized time
*/
void GetClkTime( HWND hWnd, long TmBias, ClkTime_t *clktime )
{
	time_t t;
	char str[MAXSTRLEN];

	time( &t );

	clktime->t = t;

	// add our current bias to get GMT, then subtract the bias
	// for the destination time zone read in from the cmd file...
	t = t + TimeZoneInfo.Bias*60 - TmBias*60;

	strcpy( str, ctime( &t ) );
	str[13] = str[16] = str[20] = 0;
		
	clktime->hours   = atoi( str+11 );
	clktime->minutes = atoi( str+14 );
	clktime->seconds = atoi( str+17 );
	clktime->pm = 0;

	if( clktime->hours == 0 )
		clktime->hours = 12;
	else if( clktime->hours > 12 )
	{
		clktime->pm = 1;
		clktime->hours -= 12;
	}
}

/*
 Initialize text strings for wide view and create the options buttons.

 Input:

 hWnd - main window handle
*/
void InitSRTextOut( HWND hWnd )
{
	int i;

	// init view meter box...
	rVMeter.left   = DIBSIZE + 10;
	rVMeter.right  = rVMeter.left + 6; 
	rVMeter.top    = TxtInfo.tmHeight/2;
	rVMeter.bottom = rClient.bottom - rVMeter.top;

	// init text title posiion...
	TextPos.x = rVMeter.right + 10;
	TextPos.y = TxtInfo.tmHeight;

	// fill each text string...
	strcpy( srTxt[0].szStrTitle, "Utterance:" );
	strcpy( srTxt[1].szStrTitle, "Recognized:" );
	strcpy( srTxt[2].szStrTitle, "Response:" );
	strcpy( srTxt[3].szStrTitle, "Local Time:" );

	// now init the rectangle size/position for each string...
	for( i=0; i<MAX_OUTTEXT; i++ )
	{
		srTxt[i].pos.x = TextPos.x + TxtInfo.tmAveCharWidth*13;
		if( i == 0 )
			srTxt[i].pos.y = TxtInfo.tmHeight/2;
		else
			srTxt[i].pos.y = srTxt[i-1].pos.y + TxtInfo.tmHeight + TxtInfo.tmHeight/3;
		srTxt[i].rBox.left  = srTxt[i].pos.x - 4;
		srTxt[i].rBox.right = rClient.right - TxtInfo.tmAveCharWidth;
		srTxt[i].rBox.top   = srTxt[i].pos.y - 1;
		srTxt[i].rBox.bottom= srTxt[i].pos.y + TxtInfo.tmHeight + 1;
	}

	// init the options button positions...
	SRButPos.xsize = ClkButPos.xsize = TxtInfo.tmAveCharWidth*18;
	SRButPos.ysize = ClkButPos.ysize = TxtInfo.tmHeight+8;

	ClkButPos.x = TextPos.x;
	ClkButPos.y = SRButPos.y = rClient.bottom - ClkButPos.ysize - ClkButPos.ysize/4;
	SRButPos.x = ClkButPos.x + ClkButPos.xsize + TxtInfo.tmAveCharWidth*2;

	// create the options buttons...
	ClkOptions = CreateWindow( "BUTTON", "Clock Options", WS_CHILD | BS_PUSHBUTTON,
					ClkButPos.x, ClkButPos.y, ClkButPos.xsize, ClkButPos.ysize,
					hWnd, (HMENU)ID_CLKOPTBUT, hinst, NULL );

	SROptions = CreateWindow( "BUTTON", "SR Options", WS_CHILD | BS_PUSHBUTTON,
					SRButPos.x, SRButPos.y, SRButPos.xsize, SRButPos.ysize,
					hWnd, (HMENU)ID_SROPTBUT, hinst, NULL );
}

/*
 Write out the information if we're in wide view.

 Input:

 hdc   - handle to device context
 index - index of the text string array, if this is < 0, then write all text strings
*/
void TextOutSR( HDC hdc, int index )
{
	int releaseflag=1, i;

	// if we're not in wide view then don't draw...
	if( !bWidthFlag ) return;

	if( hdc == NULL ) hdc = GetDC( ghWndMain );
	else releaseflag = 0;

	ClearTextRect( hdc, index );
	SetBkColor( hdc, RGB(196,196,196) );
	SetTextColor(hdc, RGB(0,0,127) );

	// if we're writing all the text strings...
	if( index < 0 )
	{
		for( i=0; i<MAX_OUTTEXT; i++ )
			TextOut( hdc, srTxt[i].pos.x, srTxt[i].pos.y, srTxt[i].szStrOut, strlen(srTxt[i].szStrOut) );
	}
	else	// othwerwise just write the one out...
		TextOut( hdc, srTxt[index].pos.x, srTxt[index].pos.y, srTxt[index].szStrOut, strlen(srTxt[index].szStrOut) );

	if( releaseflag ) ReleaseDC( ghWndMain, hdc );
}

/*
 Clear the text box and draw the bounding rectangle

 Input:

 hdc - handle to device context
*/
void PaintTextBox( HDC hdc )
{
	int i;

	if( !bWidthFlag ) return;

	SetBkColor( hdc, RGB(196,196,196) );
	for( i=0; i<MAX_OUTTEXT; i++ )
	{
		TextOut( hdc, TextPos.x, srTxt[i].pos.y, srTxt[i].szStrTitle, strlen(srTxt[i].szStrTitle) );
	}

	SelectObject( hdc, dkpen );
	MoveToEx( hdc, DIBSIZE, 0, NULL );
	LineTo( hdc, DIBSIZE, rClient.bottom );
	MoveToEx( hdc, srTxt[MAX_OUTTEXT-1].rBox.left, srTxt[MAX_OUTTEXT-1].rBox.bottom, NULL );
	LineTo( hdc, srTxt[0].rBox.left, srTxt[0].rBox.top );
	LineTo( hdc, srTxt[0].rBox.right, srTxt[0].rBox.top );
	SelectObject( hdc, whtpen );
	LineTo( hdc, srTxt[MAX_OUTTEXT-1].rBox.right, srTxt[MAX_OUTTEXT-1].rBox.bottom );
	LineTo( hdc, srTxt[MAX_OUTTEXT-1].rBox.left, srTxt[MAX_OUTTEXT-1].rBox.bottom );
}

/*
 Draw the view meter rectangle.

 Input:

 hdc - handle to device context
*/
void PaintVMeterBox( HDC hdc )
{
	if( !bWidthFlag ) return;

	SelectObject( hdc, dkpen );
	SelectObject( hdc, ltbrush );
	Rectangle( hdc, rVMeter.left, rVMeter.top, rVMeter.right+1, rVMeter.bottom+1 );

	SelectObject( hdc, whtpen );
	MoveToEx( hdc, rVMeter.left, rVMeter.bottom, NULL );
	LineTo( hdc, rVMeter.right, rVMeter.bottom );
	LineTo( hdc, rVMeter.right, rVMeter.top-1 );

	SelectObject( hdc, ltpen );
	MoveToEx( hdc, rVMeter.left, rVMeter.bottom-1, NULL );
	LineTo( hdc, rVMeter.right-1, rVMeter.bottom-1 );
	LineTo( hdc, rVMeter.right-1, rVMeter.top );
}

/*
 Move the text boxes and options buttons.
*/
void MoveBoxes()
{
	for( int i=0; i<MAX_OUTTEXT; i++ ) srTxt[i].rBox.right = rClient.right - TxtInfo.tmAveCharWidth;

	int tmp = (rClient.right - TextPos.x - SRButPos.xsize*2) / 3; 
	ClkButPos.x = TextPos.x + tmp;
	SRButPos.x = ClkButPos.x + SRButPos.xsize + tmp;
	MoveWindow( ClkOptions, ClkButPos.x, ClkButPos.y, ClkButPos.xsize, ClkButPos.ysize, FALSE );
	MoveWindow( SROptions, SRButPos.x, SRButPos.y, SRButPos.xsize, SRButPos.ysize, FALSE);
}

/*
 Fill the global text output string with appropriate time/date response.

 Input:

 clktime  - current localized time for target city
 dwAction - TRUE if the date is requested.  FALSE if the time is requested.
*/
void ProcessString( ClkTime_t *clktime, BOOL bAction )
{
	char *p;

	// if we need the date rather than time...
	if( bAction )
	{
		char buf1[MAXSTRLEN];
		// int day;
		_strdate( buf1 );


		// format the month first...
		wsprintf( srTxt[SRTXT_RESULT].szStrOut, "The date is %s ", buf1);

	}
	else	// otherwise just get the time...
	{
		wsprintf( srTxt[SRTXT_RESULT].szStrOut, "The time is %2d:%02d", clktime->hours, clktime->minutes );
		if( clktime->pm ) strcat( srTxt[SRTXT_RESULT].szStrOut, " P M");
	}

	// if the user wants the time somewhere else, say that as well...
	p = strstr( srTxt[SRTXT_CMD].szStrOut, " in " );
	if( p )	strcat( srTxt[SRTXT_RESULT].szStrOut, p );
}

/*
 Clear a single text box.

 Input:

 hdc - handle to device context
 i   - index of the text rectangle to clear. A -1 means do 'em all
*/
void ClearTextRect( HDC hdc, int i )
{
	SelectObject( hdc, ltbrush );
	SelectObject( hdc, ltpen );
	if( i < 0 ) i = 0;
	Rectangle( hdc, srTxt[i].rBox.left+1, srTxt[i].rBox.top+1, srTxt[i].rBox.right-1, srTxt[i].rBox.bottom-1 );
}

/*
 Create/Initialize the various pens and brushes we'll be using.
*/
void InitColors()
{
	ltbrush = (HBRUSH) GetStockObject( LTGRAY_BRUSH );
	dkbrush = (HBRUSH) GetStockObject( GRAY_BRUSH );
	redbrush= CreateSolidBrush( RGB(255,0,0) );
	grnbrush= CreateSolidBrush( RGB(0,255,0) );
	ylwbrush= CreateSolidBrush( RGB(255,255,0) );
	whtpen  = (HPEN) GetStockObject( WHITE_PEN );
	blkpen  = (HPEN) GetStockObject( BLACK_PEN );
	nulpen  = (HPEN) GetStockObject( NULL_PEN );
	ltpen   = CreatePen( PS_SOLID, 0, RGB(196,196,196) );
	dkpen   = CreatePen( PS_SOLID, 0, RGB(128,128,128) );
	blupen  = CreatePen( PS_SOLID, 0, RGB(0,0,196) );
}

/*
 Delete the pen/brush objects that we created.
*/
void DeleteColors()
{
	DeleteObject( ltpen    );
	DeleteObject( dkpen    );
	DeleteObject( blupen   );
	DeleteObject( grnbrush );
	DeleteObject( redbrush );
	DeleteObject( ylwbrush );
}

/*
 This function is called when the clock is speaking the time in intervals. 

 Input:

 clktime - contains the current localized time
*/
void SpeakContinuousTime( ClkTime_t *clktime )
{
	char buf[MAXSTRLEN];
	static int lastminute=0;

	if( TimeContOpt == IDC_RADIOTIMEOFF || clktime->minutes == lastminute ) return;

	// do this so we won't speak the same time within this minute...
	lastminute = clktime->minutes;

	switch( TimeContOpt )
	{
		case IDC_RADIOHOUR:
			if( clktime->minutes != 0 ) return;
			break;

		case IDC_RADIOHALFHR:
			if( clktime->minutes % 30 ) return;
			break;

		case IDC_RADIOQUARTHR:
			if( clktime->minutes % 15 ) return;
			break;

		case IDC_RADIO1MIN:
			lastminute = clktime->minutes;
			break;
	}

	wsprintf( buf, "The time is %d:%02d", clktime->hours, clktime->minutes );
	if( clktime->pm ) strcat( buf, " P M" );
	SpeakString( buf );
}

/*
 Modify the system menu to include special clock and SR commands.

 Input:

 hWnd - handle to main window
*/
void InitSystemMenu( HWND hWnd )
{
	HMENU hSysMenu = GetSystemMenu( hWnd, FALSE );
	AppendMenu( hSysMenu, MF_SEPARATOR, IDM_MENUABOUT, NULL );
	AppendMenu( hSysMenu, MF_STRING, IDM_MENUWIDEVIEW, "Toggle view" );
	AppendMenu( hSysMenu, MF_STRING, IDM_MENUCLKOPTS, "Clock options" );
	AppendMenu( hSysMenu, MF_STRING, IDM_MENUSROPTS, "SR options" );
	AppendMenu( hSysMenu, MF_STRING, IDM_MENUABOUT, "About SR Clock" );
}

/*
 Toggle the view between just the clock face or wide view.

 Input:

 hWnd - handle to main window
*/
void ToggleView( HWND hWnd )
{
	int showflag;
	PosSize_t sTmp;

	GetWindowRect( hWnd, &rWin );

	// toggle back to normal view...
	if( bWidthFlag )
	{
		showflag = SW_HIDE;
		sTmp.x     = rWin.left + ((rWin.right - rWin.left) - OrigSize.x ) / 2;
		sTmp.y     = rWin.top + ((rWin.bottom - rWin.top) - OrigSize.y ) / 2;
		sTmp.xsize = OrigSize.x;
		if( !bWndTitleBar ) sTmp.xsize -= BorderSize.x;

		CurSize.x = sTmp.xsize;

		sTmp.ysize = CurSize.y;

		// change timer back to 30 seconds...
		SetTimer( hWnd, ID_CLKTIMER, TIMER_30SEC, NULL );
	}
	else	// create a wide view for watching sr activity...
	{
		showflag = SW_SHOW;
		rWin.left -= TxtInfo.tmAveCharWidth*30;
		rWin.right += TxtInfo.tmAveCharWidth*30;
		if( rWin.right > scrnsize.x - 1 )
		{
			rWin.left = scrnsize.x - (rWin.right-rWin.left) - 1;
			rWin.right = scrnsize.x - 1;
		}
		if( rWin.left < 0 )
		{
			rWin.right = rWin.right-rWin.left;
			rWin.left = 0;
		}
		sTmp.x     = rWin.left;
		sTmp.y     = rWin.top;
		sTmp.xsize = CurSize.x = rWin.right-rWin.left;
		sTmp.ysize = CurSize.y;

		// change the timer to every second...
		SetTimer( hWnd, ID_CLKTIMER, TIMER_1SEC, NULL );
	}
	bWidthFlag = !bWidthFlag;
	MoveWindow( hWnd, sTmp.x, sTmp.y, sTmp.xsize, sTmp.ysize, TRUE );
	ShowWindow( SROptions, showflag );
	ShowWindow( ClkOptions, showflag );
	InvalidateRect( hWnd, NULL, TRUE );
}

/*
 Draw the view meter based on the level of incoming signal.

 Input:

 wLevel - magnitude of the audio level range = 0-0xffff
*/
void PaintVMeter( WORD wLevel )
{
	if( !bWidthFlag ) return;

	int ysize = rVMeter.bottom-rVMeter.top;
	double fVsize = (double)wLevel/65535.0*(double)ysize;
	int iVsize = (int)fVsize, ysize3=ysize/3;
	HDC hdc = GetDC( ghWndMain );

 	// erase the old view...
	SelectObject( hdc, nulpen );
	SelectObject( hdc, ltbrush );
	Rectangle( hdc, rVMeter.left+1, rVMeter.top+1, rVMeter.right, rVMeter.top+ysize-iVsize );

	// green (lower signal)...
	SelectObject( hdc, grnbrush );
	Rectangle( hdc, rVMeter.left+1, rVMeter.bottom-iVsize+1, rVMeter.right, rVMeter.bottom );

	// yellow (midrange signal)...
	if( iVsize > ysize3 )
	{
		SelectObject( hdc, ylwbrush );
		Rectangle( hdc, rVMeter.left+1, rVMeter.bottom-iVsize+1, rVMeter.right, rVMeter.bottom-ysize3 );

		// red (high/peak signal)...
		if( iVsize > ysize3*2 )
		{
			SelectObject( hdc, redbrush );
			Rectangle( hdc, rVMeter.left+1, rVMeter.bottom-iVsize+1, rVMeter.right, rVMeter.top+ysize3 );
		}
	}

	ReleaseDC( ghWndMain, hdc );
}
