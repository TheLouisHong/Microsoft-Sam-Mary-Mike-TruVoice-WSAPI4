/******************************************************************************\
*       This code and information is provided "as is" without warranty
*       of any kind, either expressed or implied, including but not
*       limited to the implied warranties of merchantability and/or
*       fitness for a particular purpose.
*
*       Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
*
\******************************************************************************/

// dib functions called from wndproc...
int LoadDIB( HWND hwnd );
int OnPaletteChanged( HWND hwnd, WPARAM wparam );
void PaintClock( HWND hwnd, HDC hdc );
void FreeDIB();
