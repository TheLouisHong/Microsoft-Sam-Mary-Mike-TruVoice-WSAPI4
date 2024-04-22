/******************************************************************************\
*       This module contains functions for handling the clock's About and 
*       Options dialog boxes.
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
#include "srclkdlg.h"
#include "resource.h"


LONG CALLBACK ClockOptionsMain( HWND hDlg, unsigned message, WORD wparam, LONG lparam );
LONG CALLBACK AboutMain( HWND hDlg, unsigned message, WORD wparam, LONG lparam );


/*
 Show the about box.
*/
void ClockOptionsDialog( HWND hwnd )
{
	DialogBox( hinst, MAKEINTRESOURCE(IDD_CLKOPTDLG), hwnd, (DLGPROC)ClockOptionsMain );
}

/*
 Process the about box messages, use the program icon for the ok button.
*/
LONG CALLBACK ClockOptionsMain( HWND hDlg, unsigned message, WORD wparam, LONG lparam )
{
	static int TimeOptLocal=0, SRFlagLocal=1;
	static bWndOnTopLocal=FALSE, bWndTitleLocal=TRUE;
	
	switch ( message )
	{
			// convert globals to locals...
		case WM_INITDIALOG:
			TimeOptLocal   = TimeContOpt;
			SRFlagLocal    = SRFlag;
			bWndOnTopLocal = bWndOnTop;
			bWndTitleLocal = bWndTitleBar;
			CheckRadioButton( hDlg, IDC_RADIOTIMEOFF, IDC_RADIO1MIN, TimeOptLocal );
			CheckRadioButton( hDlg, IDC_RECOGGLOBAL, IDC_RECOGOFF, SRFlagLocal+IDC_RECOGGLOBAL );
			CheckDlgButton( hDlg, IDC_WNDONTOP, bWndOnTopLocal );
			CheckDlgButton( hDlg, IDC_WNDTITLEBAR, bWndTitleLocal );
			return TRUE;

		case WM_COMMAND:
		{
			wparam = LOWORD(wparam);
			switch( wparam )
			{
					// convert locals to globals...
				case IDOK:
					TimeContOpt = TimeOptLocal;
					if( SRFlagLocal != SRFlag ) ActivateSR( SRFlag=SRFlagLocal );
					if( bWndTitleLocal != bWndTitleBar ) ToggleTitleBar( GetParent(hDlg), bWndTitleBar=bWndTitleLocal );
					if( bWndOnTopLocal != bWndOnTop )
						SetWindowPos( ghWndMain, (bWndOnTop=bWndOnTopLocal)?HWND_TOPMOST:HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE );

					// if user canceled then no need to change anything...
				case IDCANCEL:
					EndDialog(hDlg, TRUE);
					return TRUE;
					break;

				case IDC_WNDTITLEBAR:
					bWndTitleLocal = !bWndTitleLocal;
					break;

				case IDC_WNDONTOP:
					bWndOnTopLocal = !bWndOnTopLocal;
					break;

				case IDC_RADIOTIMEOFF:
				case IDC_RADIOHOUR:
				case IDC_RADIOHALFHR:
				case IDC_RADIOQUARTHR:
				case IDC_RADIO1MIN:
					TimeOptLocal = wparam;
					break;

				case IDC_RECOGGLOBAL:
					SRFlagLocal = RECOG_GLOBAL;
					break;

				case IDC_RECOGWINDOW:
					SRFlagLocal = RECOG_WINDOW;
					break;

				case IDC_RECOGOFF:
					SRFlagLocal = RECOG_OFF;
					break;
			}
		}
	}
	return( FALSE );
}

/*
 Activate the About dialog.
*/
void AboutDialog( HWND hwnd )
{
	DialogBox( hinst, MAKEINTRESOURCE(IDD_ABOUTDLG), hwnd, (DLGPROC)AboutMain );
}

/*
 Message handler for the About dialog.
*/
LONG CALLBACK AboutMain( HWND hDlg, unsigned message, WORD wparam, LONG lparam )
{
	switch ( message )
	{
		case WM_COMMAND:
		{
			switch( wparam )
			{
				case IDOK:
					EndDialog(hDlg, TRUE);
					return TRUE;
			}
		}
      case WM_CLOSE:
			EndDialog(hDlg, TRUE);
         return TRUE;
	}
	return( FALSE );
}

/*
 Remove/Restore title bar of main clock window.  When the title bar is removed then
 the window may be moved by dragging.
*/
void ToggleTitleBar( HWND hWnd, BOOL bFlag )
{
	long lStyle = GetWindowLong( hWnd, GWL_STYLE );

	if( bFlag )
	{
		SetWindowLong( hWnd, GWL_STYLE, lStyle | WS_CAPTION );
		CurSize.y = OrigSize.y;
		CurSize.x += BorderSize.x - 2;
	}
	else
	{
		SetWindowLong( hWnd, GWL_STYLE, lStyle & ~WS_CAPTION | WS_BORDER);
		CurSize.y -= BorderSize.y - 2;
		CurSize.x -= BorderSize.x - 2;
	}

	SetWindowPos( hWnd, NULL, 0, 0, CurSize.x, CurSize.y, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
	ShowWindow( hWnd, SW_SHOWNA );
}
