/******************************************************************************\
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/

void AboutDialog( HWND hwnd );
void ClockOptionsDialog( HWND hwnd );
void ToggleTitleBar( HWND hWnd, BOOL bFlag );

extern int   TimeContOpt;
extern int   SRFlag;
extern BOOL  bWndOnTop;
extern BOOL  bWndTitleBar;
extern POINT OrigSize, CurSize;
