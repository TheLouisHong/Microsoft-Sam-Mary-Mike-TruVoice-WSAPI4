/******************************************************************************\
*       This module contains functions for reading and displaying the clock's 
*       bitmap file.
*
*       The dib that SRCLK uses contains 2 clock faces: the 256-color one and 
*       the 16-color one positioned in memory immediately after the 256-color
*       one.
*
*       Note that this module was written to read this particular clock bitmap,
*       it should not be used as a template for reading/displaying other bitmaps. 
*       Understanding this code is not pertinent to the real sample which is to 
*       use the speech API.
*
*
Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include "showdib.h"
#include "srclk.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))

void CreateClkPalette();
BYTE *ReadDib( LPSTR szFileName );
DWORD GetDibInfoHeaderSize( BYTE * lpDib );
WORD GetDibWidth( BYTE *lpDib );
WORD GetDibHeight( BYTE * lpDib );
BYTE *GetDibBitsAddr( BYTE *lpDib );

LPSTR szFileName="srclk.bmp";    // name of the dib file containing the clock face
BYTE *lpDib;
BYTE *lpDibBits;
LOGPALETTE *pPalette;
HPALETTE hPal=NULL;
HDC	hdcmem;

/*
 Read the dib file and create the corresponding clock's palette.  The clock uses 24 custom
 palette indices: 20 for the 256-color clock face, and 4 for the 16-color clock face.
*/
int LoadDIB( HWND hwnd )
{
	lpDib = ReadDib( szFileName );

	if (lpDib == NULL)
	{
		char buf[MAXSTRLEN];
		sprintf( buf, "Error finding or reading DIB file %s.", szFileName );
		MessageBox (hwnd, buf, "Error", MB_ICONEXCLAMATION | MB_OK) ;
		return -1;
	}

	CreateClkPalette();
		
	return 0;
}

/*
 Draw the clock face in the client window.
*/
void PaintClock( HWND hwnd, HDC hdc )
{
	static WORD wDisplay = IDM_ACTUAL;
//	static WORD wDisplay = IDM_STRETCH;
	short cxDib, cyDib;
	
	if( lpDib == NULL ) return;

	// realize the clock's palette...
	if( hPal )
	{
		SelectPalette( hdc, hPal, 0 );
		RealizePalette( hdc );
	}
	
	lpDibBits = GetDibBitsAddr( lpDib );
	cxDib     = GetDibWidth( lpDib );
	cyDib     = GetDibHeight( lpDib );

	SetStretchBltMode( hdc, COLORONCOLOR );

	if( wDisplay == IDM_ACTUAL )
	{
		SetDIBitsToDevice (hdc, 0, 0, cxDib, cyDib, 0, 0,
				0, cyDib, lpDibBits, (LPBITMAPINFO) lpDib, DIB_RGB_COLORS);
	}
	else
	{
		StretchDIBits( hdc, 0, 0, rClient.right, rClient.bottom, 0, 0, cxDib, cyDib, (LPSTR) lpDibBits,
				(LPBITMAPINFO) lpDib, DIB_RGB_COLORS, SRCCOPY);
	}
}

/*
 Select and realize the clock's palette, invalidate the client window if needed.
*/
int OnPaletteChanged( HWND hwnd, WPARAM wparam )
{
	HPALETTE hPalTmp;

	// if 16 colors, then don't use the palette...
	if( BitsPerPixel < 8 ) return 0;

	HDC hdc = GetDC( hwnd );
	hPalTmp = SelectPalette( hdc, hPal, 0 );
	int i = RealizePalette( hdc );
	SelectPalette( hdc, hPalTmp, 0 );
	ReleaseDC( hwnd,  hdc );

	if( i > 0 ) InvalidateRect( hwnd, NULL, TRUE );
	
	return i;
}

/*
 Free the clock dib memory.
*/
void FreeDIB()
{
	if( pPalette ) LocalFree( (HLOCAL)pPalette );
	if( hPal ) DeleteObject( hPal );
	if( lpDib) free( lpDib );
}

/*
 Read the clock's dib file and create a palette from the color info.
*/
void CreateClkPalette()
{
	int hFile, i;
	UINT uNumColors = (UINT)( ((BITMAPINFOHEADER *) lpDib)->biClrUsed );
	PALETTEENTRY *pQuad;
	BYTE tmpbyte;
	
	pPalette = (LOGPALETTE *)LocalAlloc( LPTR, sizeof(LOGPALETTE) + uNumColors * sizeof(PALETTEENTRY) );
	if( pPalette == NULL )
	{
		MessageBox( NULL, "Not enough memory for DIB palette.", "Error", MB_OK );
		return;
	}
	
	pPalette->palNumEntries = (WORD)uNumColors;
	pPalette->palVersion = 0x300;
	pQuad = (PALETTEENTRY *)pPalette->palPalEntry;
	
	hFile = _lopen( szFileName, OF_READ | OF_SHARE_DENY_WRITE );
   if (hFile !=  HFILE_ERROR) {
	   _llseek( hFile, GetDibInfoHeaderSize(lpDib)+14, 0 );
	   
	   for( i=0; i<(int)uNumColors; i++, pQuad++ )
	   {
		   _lread( hFile, (LPSTR)pQuad, sizeof(PALETTEENTRY) );
		   tmpbyte = pQuad->peRed;
		   pQuad->peRed   = pQuad->peBlue;
		   pQuad->peBlue  = tmpbyte;
	   }
	   
	   _lclose (hFile);
   }
	
	hPal = CreatePalette( pPalette );
}

/*
 Read the dib file, allocating memory for the bitmap data.
*/
BYTE *ReadDib( LPSTR szFileName )
{
	BITMAPFILEHEADER bmfh;
	BYTE *           lpDib;
	DWORD            dwDibSize, dwOffset=0, dwHeaderSize;
	int              hFile;
	WORD             wDibRead;
	char             buf[MAXFILENAME];

	if( (hFile = _lopen (szFileName, OF_READ | OF_SHARE_DENY_WRITE)) == -1 )
	{
		// ok, try looking in the directory immediately above...
		wsprintf( buf, "..\\%s", szFileName );
		if( (hFile = _lopen (buf, OF_READ | OF_SHARE_DENY_WRITE)) == -1 )
			return NULL;
	}

	if( _lread (hFile, (LPSTR) &bmfh, sizeof (BITMAPFILEHEADER)) != sizeof (BITMAPFILEHEADER) )
	{
		_lclose (hFile);
		return NULL;
	}

	if( bmfh.bfType != * (WORD *)"BM" )
	{
		_lclose (hFile);
		return NULL;
	}

	dwDibSize = bmfh.bfSize - sizeof (BITMAPFILEHEADER);

	lpDib = (BYTE * ) malloc( dwDibSize );

	if( lpDib == NULL )
	{
		_lclose (hFile);
		return NULL;
	}

	while( dwDibSize > 0 )
	{
		wDibRead = (WORD) MIN (32768ul, dwDibSize);

		if (wDibRead != _lread (hFile, (LPSTR) (lpDib + dwOffset), wDibRead))
		{
			_lclose (hFile);
			free (lpDib);
			return NULL;
		}

		dwDibSize -= wDibRead;
		dwOffset  += wDibRead;
	}

	_lclose (hFile);

	dwHeaderSize = GetDibInfoHeaderSize( lpDib );

	if (dwHeaderSize < 12 || (dwHeaderSize > 12 && dwHeaderSize < 16))
	{
		free( lpDib );
		return NULL;
	}

	return lpDib;
}

/*
 Compute the address of the bitmap bits.
*/
BYTE *GetDibBitsAddr( BYTE *lpDib )
{
	DWORD dwNumColors, dwColorTableSize ;
	WORD  wBitCount ;

	if( GetDibInfoHeaderSize (lpDib) == sizeof (BITMAPCOREHEADER) )
	{
		wBitCount = ((BITMAPCOREHEADER *) lpDib)->bcBitCount ;

		if (wBitCount != 24)
			dwNumColors = 1L << wBitCount ;
		else
			dwNumColors = 0 ;

		dwColorTableSize = dwNumColors * sizeof (RGBTRIPLE) ;
	}
	else
	{
		wBitCount = ((BITMAPINFOHEADER *) lpDib)->biBitCount ;

		if (GetDibInfoHeaderSize (lpDib) >= 36)
			dwNumColors = ((BITMAPINFOHEADER *) lpDib)->biClrUsed ;
		else
			dwNumColors = 0 ;

		if (dwNumColors == 0)
		{
			if (wBitCount != 24)
				dwNumColors = 1L << wBitCount ;
			else
				dwNumColors = 0 ;
		}

		dwColorTableSize = dwNumColors * sizeof (RGBQUAD);
	}

	// this bmp file actually contains 2 bitmaps, use the appropriate one...
	if( BitsPerPixel < 8 )
		return lpDib + GetDibInfoHeaderSize (lpDib) + dwColorTableSize - ((BITMAPINFOHEADER *)lpDib)->biSizeImage/2;
	else
		return lpDib + GetDibInfoHeaderSize (lpDib) + dwColorTableSize ;
}

/*
 Compute the dib info header size.
*/
DWORD GetDibInfoHeaderSize( BYTE *lpDib )
{
	return ((BITMAPINFOHEADER *) lpDib)->biSize;
}

/*
 Compute the width of the bitmap.
*/
WORD GetDibWidth( BYTE *lpDib )
{
	if( GetDibInfoHeaderSize( lpDib ) == sizeof (BITMAPCOREHEADER) )
		return (WORD) (((BITMAPCOREHEADER *) lpDib)->bcWidth);
	else
		return (WORD) (((BITMAPINFOHEADER *) lpDib)->biWidth) ;
}

/*
 Compute the height of the bitmap.
*/
WORD GetDibHeight( BYTE *lpDib )
{
	if( GetDibInfoHeaderSize( lpDib ) == sizeof(BITMAPCOREHEADER) )
		return (WORD) (((BITMAPCOREHEADER *) lpDib)->bcHeight);
	else
		return (WORD) (((BITMAPINFOHEADER *) lpDib)->biHeight);
}
