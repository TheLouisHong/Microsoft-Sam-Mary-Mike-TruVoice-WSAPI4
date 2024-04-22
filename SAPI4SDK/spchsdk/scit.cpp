/******************************************************************************\
*       This module contains functions for initializing the OLE components and 
*       window position and description characteristics.
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
#include "vcmd.h"

BOOL InitApplication( HANDLE hInstance );
BOOL InitInstance( HANDLE hInstance, int nCmdShow );
void Windowpos( int *x, int *y, int *xsize, int *ysize );

/*
 Initialize OLE, app instance and dispatch messages until the user terminates the app, then
 terminate OLE.
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   // Set the current working directory
   char  szModule[256];
   char *pSlash;
   GetModuleFileName (hInstance, szModule, sizeof(szModule));
   for (pSlash = strchr(szModule,'\\'); pSlash && strchr(pSlash+1,'\\');
      pSlash = strchr(pSlash+1,'\\'));
   if (pSlash)
      *pSlash = 0;
   SetCurrentDirectory (szModule);


   MSG msg;

	if( !hPrevInstance )
		if( !InitApplication(hInstance) ) return( FALSE );

	// try to begin ole
	if( !BeginOLE() ) return 1;

	if( !InitInstance(hInstance, nCmdShow) ) return( FALSE );

	while( GetMessage(&msg, 0, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if( !EndOLE() ) MessageBox (NULL, "Failed to release interfaces or shut down OLE.", NULL, MB_OK);

	return( msg.wParam );
}

/*
 Initialize and register the clock the window class.
*/
BOOL InitApplication( HANDLE hInstance )
{
	WNDCLASS  wc;

	wc.style			= CS_DBLCLKS;
	wc.lpfnWndProc		= MainWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= (HINSTANCE) hInstance;
	wc.hIcon			= LoadIcon( (HINSTANCE) hInstance, "IDI_SRCLKICON" );
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH) GetStockObject(LTGRAY_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "ClkClass";

	return( RegisterClass(&wc) );
}


/*
 This function is called at initialization time for every instance of 
 this application.  This function performs initialization tasks that 
 cannot be shared by multiple instances.  
*/        
BOOL InitInstance( HANDLE hInstance, int nCmdShow )
{
	HWND hwnd;
	HDC hdc;
	RECT rTmp;
	DWORD dwStyle = WS_OVERLAPPED|WS_SYSMENU|WS_BORDER|WS_CAPTION|WS_MINIMIZEBOX;
	int xsize,ysize;

	hinst = (HINSTANCE) hInstance;

 	// get video mode and text characteristics...
	hdc = GetDC( NULL );
	scrnsize.x = GetDeviceCaps( hdc, HORZRES );
	scrnsize.y = GetDeviceCaps( hdc, VERTRES );
	BitsPerPixel= GetDeviceCaps( hdc, BITSPIXEL );
	GetTextMetrics( hdc, &TxtInfo );
	ReleaseDC( NULL, hdc );

	// compute the clock window size based on a 1:1 dib size (actual dib size).
	rTmp.left = rTmp.top = 0;
	rTmp.right = rTmp.bottom = DIBSIZE;
	AdjustWindowRect( &rTmp, dwStyle, FALSE );
	xsize = rTmp.right - rTmp.left;
	ysize = rTmp.bottom - rTmp.top;
	BorderSize.x = xsize - DIBSIZE;
	BorderSize.y = ysize - DIBSIZE;
	
	// Create the main window...
	hwnd = CreateWindowEx( 0, "ClkClass", szWinTitle,	dwStyle,
				(scrnsize.x-xsize)/2, (scrnsize.y-ysize)/2,
				xsize, ysize, NULL, NULL, (HINSTANCE) hinst, NULL );

	if( !hwnd ) return( FALSE );

	ShowWindow( hwnd, nCmdShow );

	UpdateWindow( hwnd );

	return( TRUE );
}
