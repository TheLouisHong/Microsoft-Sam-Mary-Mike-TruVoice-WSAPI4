/******************************************************************************\
*       This module contains the functions for drawing the clock hands.
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
#include <math.h>

const int MAX_POINTS=5;
const int Center = (DIBSIZE/2-1);


typedef struct
{
	double x, y;
} FPoint_t;

void CalcHand( double position, FPoint_t *fHand, POINT *iHand );

int RadiusMin = DIBSIZE / 10 * 7;
int RadiusHr  = DIBSIZE / 10 * 5;
FPoint_t MinHandf[MAX_POINTS], HourHandf[MAX_POINTS], SecHandf[MAX_POINTS];
POINT    MinHand[MAX_POINTS], HourHand[MAX_POINTS], SecHand[MAX_POINTS];

/*
 Draw the clock hands based on the current time.  If wide view then draw the second hand too.
*/
void DrawClkHands( HDC hdc, ClkTime_t *clktime, BOOL bSecFlag )
{
	int releaseflag=1, i;
	double hour;

	if( hdc == NULL ) hdc = GetDC( ghWndMain );
	else releaseflag = 0;

	SelectObject( hdc, dkpen );

	// draw minute hand...
	CalcHand( (double)clktime->minutes, MinHandf, MinHand );
	MoveToEx( hdc, MinHand[0].x, MinHand[0].y, NULL );
	for( i=1; i<MAX_POINTS; i++ ) LineTo( hdc, MinHand[i].x, MinHand[i].y );

	// draw hour hand...
	hour = ((double)clktime->hours + (double)clktime->minutes*0.016667 ) * 5.0;
	CalcHand( hour, HourHandf, HourHand );
	MoveToEx( hdc, HourHand[0].x, HourHand[0].y, NULL );
	for( i=1; i<MAX_POINTS; i++ ) LineTo( hdc, HourHand[i].x, HourHand[i].y );

	// draw second hand, if desired...
	if( bSecFlag )
	{
		CalcHand( (double)clktime->seconds, SecHandf, SecHand );
		MoveToEx( hdc, SecHand[0].x, SecHand[0].y, NULL );
		for( i=1; i<MAX_POINTS; i++ ) LineTo( hdc, SecHand[i].x, SecHand[i].y );
	}

	if( releaseflag ) ReleaseDC( ghWndMain, hdc );
}

/*
 Calculate the points information of a single hand based on the current position.
*/
void CalcHand( double position, FPoint_t *fHand, POINT *iHand )
{
	int i;
	double sintheta,costheta;

	position *= 0.10471976;

	sintheta = sin( position );
	costheta = cos( position );
	for( i=0; i<MAX_POINTS; i++ )
	{
		iHand[i].x = (int)(fHand[i].x*costheta - fHand[i].y*sintheta) + Center;
		iHand[i].y = (int)(fHand[i].x*sintheta + fHand[i].y*costheta) + Center;
	}
}

/*
 Initialize the cartesian point arrays of each clock hand.  For simplicity I use an sufficient
 size scale for accurate resolution then scale it to the logical clock size.
*/
void InitClkHands()
{
	int i;

	MinHandf[0].x =   0.;
	MinHandf[0].y =  -RadiusMin;
	MinHandf[1].x =   4.;
	MinHandf[1].y =   0.;
	MinHandf[2].x =   0.;
	MinHandf[2].y =   4.;
	MinHandf[3].x =  -4.;
	MinHandf[3].y =   0.;
	MinHandf[4].x =   0.;
	MinHandf[4].y =  -RadiusMin;
			 
	HourHandf[0].x =   0.;
	HourHandf[0].y =  -RadiusHr;
	HourHandf[1].x =   5.;
	HourHandf[1].y =   0.;
	HourHandf[2].x =   0.;
	HourHandf[2].y =   5.;
	HourHandf[3].x =  -5.;
	HourHandf[3].y =   0.;
	HourHandf[4].x =   0.;
	HourHandf[4].y =  -RadiusHr;

	SecHandf[0].x =   0.;
	SecHandf[0].y =  -RadiusMin;
	SecHandf[1].x =   1.;
	SecHandf[1].y =   0.;
	SecHandf[2].x =   0.;
	SecHandf[2].y =   1.;
	SecHandf[3].x =  -1.;
	SecHandf[3].y =   0.;
	SecHandf[4].x =   0.;
	SecHandf[4].y =  -RadiusMin;

	// scale them down...
	for( i=0; i<MAX_POINTS; i++ )
	{
		MinHandf[i].x /= 2;
		MinHandf[i].y /= 2;
		HourHandf[i].x /= 2;
		HourHandf[i].y /= 2;
		SecHandf[i].x /= 2;
		SecHandf[i].y /= 2;
	}
}			 
