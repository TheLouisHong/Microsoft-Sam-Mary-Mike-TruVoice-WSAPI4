/**********************************************************************
Paint.cpp - Paints the mouth in any shape to an HDC. This is not cached.
   There is another set of functions that cache the bitmaps

Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <mmsystem.h>
#include <objbase.h>
#include <objerror.h>
#include <ole2ver.h>
#include <speech.h>
#include "paint.h"

int   iCenterX;
int   iCenterY;
int   iMaxWidthX;
int   iMinWidthX;
int   iMaxHeightY;
int   iMaxTopLipHeight;
int   iMaxBottomLipHeight;
int   iMaxTopTeethHeight;
int   iMaxBottomTeethHeight;
BOOL  gfMale;

/**********************************************************************
Bezier - Takes an array of 4 points and fills another buffer in
   with an array of N points (depending upon the depth). Note that
   it fill never fill in the last point, so the application will need
   to do that.

inputs
   POINT    *paIn - Pointer to an array of 4 points
   POINT    *paOut - Points to fill in
   DWORD    dwDepth - If 0, only 3 points are filled in. If 1, 7 are.
               If 2, 15 are. Etc.
returns
   DWORD    - Number of points
*/
DWORD Bezier (POINT *paIn, POINT *paOut, DWORD dwDepth)
{
   if (!dwDepth) {
      // fill in 3 points and we're done
      memcpy (paOut, paIn, 3 * sizeof(POINT));
      return 3;
   }

   // else, subdivide
   DWORD dwTotal = 0;
   POINT Mid;
   POINT aNew[4];
   Mid.x = (paIn[1].x + paIn[2].x) / 2;
   Mid.y = (paIn[1].y + paIn[2].y) / 2;

   // first half
   aNew[0] = paIn[0];
   aNew[1].x = (paIn[0].x + paIn[1].x) / 2;
   aNew[1].y = (paIn[0].y + paIn[1].y) / 2;
   aNew[2].x = (Mid.x + paIn[1].x) / 2;
   aNew[2].y = (Mid.y + paIn[1].y) / 2;
   aNew[3] = Mid;
   dwTotal = Bezier (aNew, paOut, dwDepth-1);

   // second half
   aNew[0] = Mid;
   aNew[1].x = (Mid.x + paIn[2].x) / 2;
   aNew[1].y = (Mid.y + paIn[2].y) / 2;
   aNew[2].x = (paIn[3].x + paIn[2].x) / 2;
   aNew[2].y = (paIn[3].y + paIn[2].y) / 2;
   aNew[3] = paIn[3];
   dwTotal += Bezier (aNew, paOut + dwTotal, dwDepth-1);

   return dwTotal;
}

/**********************************************************************
Mirror - Takes a set of pointers and mirrors them about a vertical
   axis. The best way to send in points is to start at the middle top
   of the object, work around clockwise, and end at the middle bottom.

inputs
   int      iMirrorX - X line to mirror around
   POINT    *paBuf - Pointer to an array of points
   DWORD    dwNum - Number of points. This writes another dwNum points
               into the buffer
returns
   none
*/
void Mirror (int iMirrorX, POINT *paBuf, DWORD dwNum)
{
   DWORD i;

   for (i = 0; i < dwNum; i++) {
      paBuf[dwNum+i].x = iMirrorX - (paBuf[dwNum-i-1].x - iMirrorX);
      paBuf[dwNum+i].y = paBuf[dwNum-i-1].y;
   }
}

/**********************************************************************
CalcMaxParams - Given the client rect, this calculates some
   important max & min params
*/
void CalcMaxParams (RECT *pRect)
{
   iCenterX = (pRect->right + pRect->left) / 2;
   iCenterY = (pRect->top * 2 + pRect->bottom) / 3;
   iMaxWidthX = (int) ((pRect->right - pRect->left) * .45f);
   iMinWidthX = iMaxWidthX * 2 / 3;
   iMaxHeightY = iMaxWidthX * 2 / 3;
   iMaxTopLipHeight = iMaxWidthX / (gfMale ? 7 : 5);
   iMaxBottomLipHeight = iMaxWidthX / (gfMale ? 6 : 4);
   iMaxTopTeethHeight = iMaxWidthX / 6;
   iMaxBottomTeethHeight = iMaxWidthX / 8;

}

int   iTopLipAboveTeeth, iBottomLipBelowTeeth;
int   iMouthWidthX, iMouthHeightY;
int   iTopLipHeight, iBottomLipHeight;
int   iMidLipWidthX;
int   iUpturn;
double fTension;
double fTongueDown, fTongueUp;

/**********************************************************************
CalcCurParams - Given the mouth shape, this calcaultes current parameters.
*/
void CalcCurParams (PTTSMOUTH pMouth)
{
   // how much are the teeth showing
   iTopLipAboveTeeth = iMaxTopTeethHeight * pMouth->bTeethUpperVisible / 256;
   iBottomLipBelowTeeth = iMaxBottomTeethHeight * pMouth->bTeethLowerVisible / 256;

   // how wide and high are the teeth
   iMouthWidthX = iMinWidthX + (iMaxWidthX - iMinWidthX) * pMouth->bMouthWidth / 256;
   iMouthHeightY = iMaxHeightY * pMouth->bMouthHeight / 256;
   if (pMouth->bMouthUpturn >= 0x80)
      iUpturn = -(iMouthWidthX * ((int) pMouth->bMouthUpturn - 0x80) / 128 / 10);
   else
      iUpturn = -(iMouthWidthX * ((int) pMouth->bMouthUpturn - 0x80) / 128 / 20);

   // how thick are the lips. Assume are of lip is constant?
   iTopLipHeight = iMaxTopLipHeight;
   iBottomLipHeight = iMaxBottomLipHeight;
   iMidLipWidthX = iMouthWidthX / 2;
   fTension = pMouth->bLipTension / 255.0;

   // tongue
   fTongueDown = fTongueUp = 0;
   if (pMouth->bTonguePosn < 0x80)
      fTongueDown = (0x80 - (double) pMouth->bTonguePosn) / 128.0;
   else // tongue > 0x80
      fTongueUp = ((double)pMouth->bTonguePosn - 0x80) / 128.0;
}


/**********************************************************************
DrawLips - Draws the lips on the HDC (after all the basic parameters
   are caclulated

inputs
   HDC      hdc - DC
returns
   none
*/
void DrawLips (HDC hdc)
{
   static HBRUSH redBrushMale = NULL;
   static HBRUSH redBrushFemale = NULL;
   if (!redBrushMale){
      redBrushMale = CreateSolidBrush (COLORREF(
         // RGB(184,80,80) )); //a solid skin brush - too red
         RGB(199,100,90) ));
      redBrushFemale = CreateSolidBrush (COLORREF(
         RGB(206,28, 28) )); //a solid skin brush
   }

   HBRUSH oldBrush = (HBRUSH) SelectObject(hdc, gfMale ? redBrushMale : redBrushFemale);

   int nLeft = iCenterX - iMouthWidthX;

   POINT aBuf[128];
   POINT aBez[4];
   DWORD dwCur;

#define  SETPOINT(a,b,c)   aBez[a].x = (b); aBez[a].y = (c)
#define  INTERPF(f,a,b)     ((1-(f)) * (a) + (f) * (b))
#define  INTERP(f,a,b)     ((int) INTERPF(f,a,b))
   // bottom lip
   int   iBottomLip = iCenterY + iMouthHeightY + iBottomLipBelowTeeth ;
   int   iBottomLipBottom = iCenterY + iMouthHeightY + iBottomLipBelowTeeth+ iBottomLipHeight;
   iBottomLip = INTERP(INTERPF(fTension, 0, .9),
      iBottomLip,
      iCenterY);
   iBottomLipBottom = INTERP(INTERPF(fTension, 0, .8),
      iBottomLipBottom,
      iCenterY+ iBottomLipHeight);
   dwCur = 0;

   // top of the bottom lip
   SETPOINT (0,
      iCenterX,
      iBottomLip);
   SETPOINT (1,
      INTERP(INTERPF(fTension, .6, .8), nLeft, iCenterX),
      INTERP(1.0, iCenterY, iBottomLip) + iUpturn / 3);
   SETPOINT (2,
      INTERP(INTERPF(fTension, .2, .5), nLeft, iCenterX),
      INTERP(INTERPF(fTension, .33, .1), iCenterY, iBottomLip) + iUpturn * 2 / 3);
   SETPOINT (3,
      nLeft,
      INTERP(0, iCenterY, iBottomLip) + iUpturn);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // bottom of the bottom lip
   SETPOINT (0,
      nLeft,
      INTERP(0, iCenterY, iBottomLipBottom) + iUpturn);
   SETPOINT (1,
      INTERP(INTERPF(fTension, .2, .4), nLeft, iCenterX),
      INTERP(INTERPF(fTension, .33, .2), iCenterY, iBottomLipBottom) + iUpturn * 2 / 3);
   SETPOINT (2,
      INTERP(INTERPF(fTension, .6, .8), nLeft, iCenterX),
      INTERP(1.0, iCenterY, iBottomLipBottom) + iUpturn / 3);
   SETPOINT (3,
      iCenterX,
      iBottomLipBottom);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // mirror & draw bottom lip
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);

   // top lip
   dwCur = 0;
   int iTopLip = iCenterY - iTopLipAboveTeeth - iTopLipHeight;
   int iTopLipBottom = iCenterY - iTopLipAboveTeeth;

   // bend in upper lip
   SETPOINT (0,
      iCenterX,
      iTopLip);
   SETPOINT (1,
      INTERP(.95, nLeft, iCenterX),
      iTopLip);
   SETPOINT (2,
      INTERP(.9, nLeft, iCenterX),
      iTopLip-iTopLipHeight/10);
   SETPOINT (3,
      INTERP(.8, nLeft, iCenterX),
      iTopLip-iTopLipHeight/8);
   dwCur += Bezier (aBez, aBuf + dwCur, 2);

   // from bend to left edge
   SETPOINT (0,
      INTERP(.8, nLeft, iCenterX),
      iTopLip-iTopLipHeight/8);
   SETPOINT (1,
      INTERP(INTERPF(fTension,.5,.6), nLeft, iCenterX),
      INTERP(.6, iCenterY,iTopLip) + iUpturn / 3);
   SETPOINT (2,
      INTERP(INTERPF(fTension,.2,.3), nLeft, iCenterX),
      INTERP(.3, iCenterY, iTopLip) + iUpturn * 2 / 3);
   SETPOINT (3,
      nLeft,
      iCenterY + iUpturn);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // from left edge to center lower
   SETPOINT (0,
      nLeft,
      iCenterY + iUpturn);
   SETPOINT (1,
      INTERP (INTERPF(fTension, .33, .5), nLeft, iCenterX),
      INTERP (INTERPF(fTension, .5, 0), iCenterY, iTopLipBottom) + iTopLipHeight / 8 +
      iUpturn * 2 / 3);
   SETPOINT (2,
      INTERP (INTERPF(fTension, .66, .8), nLeft, iCenterX),
      INTERP (INTERPF(fTension, .75, 0), iCenterY, iTopLipBottom) + iUpturn / 3);
   SETPOINT (3,
      iCenterX,
      INTERP (INTERPF(fTension, 1.0, .4), iCenterY, iTopLipBottom) + iTopLipHeight / 6);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // mirror & draw top lip
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);

   SelectObject (hdc, oldBrush);

   DeleteObject (redBrushMale);
   DeleteObject (redBrushFemale);
   redBrushMale = 0;
   redBrushFemale = 0;
}


/**********************************************************************
DrawSkin - Draws the lips on the HDC (after all the basic parameters
   are caclulated

inputs
   HDC      hdc - DC
returns
   none
*/
void DrawSkin (HDC hdc)
{
   static HBRUSH skinBrush = NULL;
   if (!skinBrush) {
      skinBrush = CreateSolidBrush (COLORREF(RGB(199,143,103))); //a solid skin brush
   }

   HBRUSH oldBrush = (HBRUSH) SelectObject(hdc, skinBrush);
   HPEN   oldPen = (HPEN) SelectObject(hdc, GetStockObject(NULL_PEN));

   int nLeft = iCenterX - iMouthWidthX;

   POINT aBuf[128];
   POINT aBez[4];
   DWORD dwCur;

   // bottom lip
   int   iBottomLip = iCenterY + iMouthHeightY + iBottomLipBelowTeeth ;
   int   iBottomLipBottom = iCenterY + iMouthHeightY + iBottomLipBelowTeeth+ iBottomLipHeight;
   int iTopLip = iCenterY - iTopLipAboveTeeth - iTopLipHeight;
   int iTopLipBottom = iCenterY - iTopLipAboveTeeth;
   iBottomLip = INTERP(INTERPF(fTension, 0, .9),
      iBottomLip,
      iCenterY);
   iBottomLipBottom = INTERP(INTERPF(fTension, 0, .8),
      iBottomLipBottom,
      iCenterY+ iBottomLipHeight);
   dwCur = 0;

   // bottom of the bottom lip
   SETPOINT (0,
      iCenterX,
      iBottomLipBottom);
   SETPOINT (1,
      INTERP(INTERPF(fTension, .6, .8), nLeft, iCenterX),
      INTERP(1.0, iCenterY, iBottomLipBottom) + iUpturn / 3);
   SETPOINT (2,
      INTERP(INTERPF(fTension, .2, .4), nLeft, iCenterX),
      INTERP(INTERPF(fTension, .33, .2), iCenterY, iBottomLipBottom) + iUpturn * 2 / 3);
   SETPOINT (3,
      nLeft,
      INTERP(0, iCenterY, iBottomLipBottom) + iUpturn);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // from bend to left edge
   SETPOINT (0,
      nLeft,
      iCenterY + iUpturn);
   SETPOINT (1,
      INTERP(INTERPF(fTension,.2,.3), nLeft, iCenterX),
      INTERP(.3, iCenterY, iTopLip) + iUpturn * 2 / 3);
   SETPOINT (2,
      INTERP(INTERPF(fTension,.5,.6), nLeft, iCenterX),
      INTERP(.6, iCenterY,iTopLip) + iUpturn / 3);
   SETPOINT (3,
      INTERP(.8, nLeft, iCenterX),
      iTopLip-iTopLipHeight/8);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // bend in upper lip
   SETPOINT (0,
      INTERP(.8, nLeft, iCenterX),
      iTopLip-iTopLipHeight/8);
   SETPOINT (1,
      INTERP(.9, nLeft, iCenterX),
      iTopLip);
   SETPOINT (2,
      INTERP(.95, nLeft, iCenterX),
      iTopLip);
   SETPOINT (3,
      iCenterX,
      iTopLip);
   dwCur += Bezier (aBez, aBuf + dwCur, 2);

   // put the last point in & then draw a large box around
   aBuf[dwCur] = aBez[3];
   dwCur++;
   aBuf[dwCur].x = iCenterX;
   aBuf[dwCur].y = -10;
   dwCur++;
   aBuf[dwCur].x = -10;
   aBuf[dwCur].y = -10;
   dwCur++;
   aBuf[dwCur].x = -10;
   aBuf[dwCur].y = 1000;
   dwCur++;

   // mirror & draw bottom lip
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);


   SelectObject (hdc, oldBrush);
   SelectObject (hdc, oldPen);

   DeleteObject (skinBrush);
   skinBrush = 0;
}


/**********************************************************************
DrawTeeth - Draws the lips on the HDC (after all the basic parameters
   are caclulated

inputs
   HDC      hdc - DC
returns
   none
*/
void DrawTeeth (HDC hdc)
{
   static HBRUSH whiteBrush = NULL;
   if (!whiteBrush) {
      whiteBrush = CreateSolidBrush (COLORREF(RGB(244,245,216))); //a solid skin brush
   }

   HBRUSH oldBrush = (HBRUSH) SelectObject(hdc, whiteBrush);

   int nLeft = iCenterX - iMaxWidthX;

   POINT aBuf[128];
   POINT aBez[4];
   DWORD dwCur;

   // bottom teeth
   int   iBottomTeeth = iCenterY + iMouthHeightY ;
   int   iBottomTeethBottom = iCenterY + iMouthHeightY + iMaxBottomTeethHeight;
   dwCur = 0;

   // top of the bottom tooth
   SETPOINT (0,
      iCenterX,
      iBottomTeeth);
   SETPOINT (1,
      INTERP(.6, nLeft, iCenterX),
      INTERP(1.0, iCenterY, iBottomTeeth));
   SETPOINT (2,
      INTERP(.2, nLeft, iCenterX),
      INTERP(.33, iCenterY, iBottomTeeth) );
   SETPOINT (3,
      nLeft,
      INTERP(0, iCenterY, iBottomTeeth) );
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // switch over
   SETPOINT (0,
      nLeft,
      INTERP(0, iCenterY, iBottomTeeth) );
   SETPOINT (1,
      nLeft,
      INTERP(0, iCenterY, iBottomTeeth) );
   SETPOINT (2,
      nLeft,
      iCenterY + iMaxBottomTeethHeight );
   SETPOINT (3,
      nLeft,
      iCenterY + iMaxBottomTeethHeight );
   dwCur += Bezier (aBez, aBuf + dwCur, 2);

   // bottom of the bottom lip
   SETPOINT (0,
      nLeft,
      iCenterY + iMaxBottomTeethHeight);
   SETPOINT (1,
      INTERP(.2, nLeft, iCenterX),
      INTERP(.33, iCenterY + iMaxBottomTeethHeight, iBottomTeethBottom) );
   SETPOINT (2,
      INTERP(.6, nLeft, iCenterX),
      INTERP(1.0, iCenterY + iMaxBottomTeethHeight, iBottomTeethBottom) );
   SETPOINT (3,
      iCenterX,
      iBottomTeethBottom);
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // mirror & draw bottom teeth
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);




   // top teeth
   dwCur = 0;

   // front teeth
   SETPOINT (0,
      iCenterX,
      iCenterY + iMaxTopTeethHeight / 6);
   SETPOINT (1,
      INTERP(.85, nLeft, iCenterX),
      iCenterY + iMaxTopTeethHeight / 6);
   SETPOINT (2,
      INTERP(.8, nLeft, iCenterX),
      iCenterY);
   SETPOINT (3,
      INTERP(.8, nLeft, iCenterX),
      iCenterY);
   dwCur += Bezier (aBez, aBuf + dwCur, 1);

   // to edge of mouth
   SETPOINT (0,
      INTERP(.75, nLeft, iCenterX),
      iCenterY);
   SETPOINT (1,
      INTERP(.5, nLeft, iCenterX),
      iCenterY - iMaxTopTeethHeight / 10);
   SETPOINT (2,
      INTERP(.25, nLeft, iCenterX),
      iCenterY - iMaxTopTeethHeight / 6);
   SETPOINT (3,
      nLeft,
      iCenterY - iMaxTopTeethHeight / 3);
   dwCur += Bezier (aBez, aBuf + dwCur, 1);

   // up
   SETPOINT (0,
      nLeft,
      iCenterY - iMaxTopTeethHeight / 3);
   SETPOINT (1,
      nLeft,
      iCenterY - iMaxTopTeethHeight / 3);
   SETPOINT (2,
      nLeft,
      iCenterY - iMaxTopTeethHeight);
   SETPOINT (3,
      nLeft,
      iCenterY - iMaxTopTeethHeight);
   dwCur += Bezier (aBez, aBuf + dwCur, 1);

   // back over
   SETPOINT (0,
      nLeft,
      iCenterY - iMaxTopTeethHeight);
   SETPOINT (1,
      nLeft,
      iCenterY - iMaxTopTeethHeight);
   SETPOINT (2,
      iCenterX,
      iCenterY - iMaxTopTeethHeight);
   SETPOINT (3,
      iCenterX,
      iCenterY - iMaxTopTeethHeight);
   dwCur += Bezier (aBez, aBuf + dwCur, 1);

   // mirror & draw top teeth
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);


   SelectObject (hdc, oldBrush);

   DeleteObject (whiteBrush);
   whiteBrush = 0;
}


/**********************************************************************
DrawTongue - Draws the lips on the HDC (after all the basic parameters
   are caclulated

inputs
   HDC      hdc - DC
returns
   none
*/
void DrawTongue (HDC hdc)
{
   static HBRUSH tongueBrush = NULL;
   if (!tongueBrush) {
      tongueBrush = CreateSolidBrush (COLORREF(RGB(180,79,61))); //a solid skin brush
   }

   HBRUSH oldBrush = (HBRUSH) SelectObject(hdc, tongueBrush);

   int nLeft = iCenterX - iMaxWidthX * 3 / 5;

   POINT aBuf[128];
   POINT aBez[4];
   DWORD dwCur;

   // bottom teeth
   int   iBottomTeeth = iCenterY + iMouthHeightY ;
   int   iBottomTeethBottom = iCenterY + iMouthHeightY + iMaxBottomTeethHeight;
   int   iTongueTop = iCenterY + iMaxTopTeethHeight / 2;
   int   iTongueBottom = iTongueTop + iMaxTopTeethHeight * 5 / 2;
   int   iTopTeeth = iCenterY - 2* iMaxTopTeethHeight;
   dwCur = 0;

   // top of the tongue. This has a little bump
   SETPOINT (0,
      iCenterX,
      INTERP(fTongueUp, iTongueTop, iTopTeeth) );
   SETPOINT (1,
      INTERP(.8, nLeft, iCenterX),
      INTERP(fTongueUp, INTERP(1.1, iTongueBottom, iTongueTop), iTopTeeth));
   SETPOINT (2,
      nLeft,
      INTERP(.8, iTongueBottom, iTongueTop) );
   SETPOINT (3,
      nLeft,
      INTERP(.5, iTongueBottom, iTongueTop) );
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // bottom of tongue
   SETPOINT (0,
      nLeft,
      INTERP(.5, iTongueBottom, iTongueTop) );
   SETPOINT (1,
      nLeft,
      INTERP(.3, iTongueBottom, iTongueTop) );
   int   iInterp = INTERP(fTongueDown, iTongueBottom, iBottomTeethBottom);
   SETPOINT (2,
      INTERP(.5, nLeft, iCenterX),
      max(iTongueBottom, iInterp) );
   SETPOINT (3,
      iCenterX,
      max(iTongueBottom, iInterp) );
   dwCur += Bezier (aBez, aBuf + dwCur, 3);

   // mirror & draw tongue
   Mirror (iCenterX, aBuf, dwCur);
   Polygon(hdc, aBuf, dwCur * 2);


   SelectObject (hdc, oldBrush);

   DeleteObject (tongueBrush);
   tongueBrush = 0;
}


/**********************************************************************
PaintMouth - Given a mouth shape and HDC, this draws it.

inputs
   TTSMOUTH    *pTTSMouth - mouth shape
   HDC         hdc - drawing context
   RECT        *pRect - rectangle to draw in
returns
   none
*/
void PaintMouth (TTSMOUTH *pTTSMouth, HDC hdc, RECT *pRect)
{
   static HBRUSH blackBrush = NULL;
   if (!blackBrush){
      blackBrush = CreateSolidBrush (COLORREF(RGB(101,37,39))); //a solid skin brush
   }

   CalcMaxParams (pRect);
   CalcCurParams (pTTSMouth);

   // back of mouth
   FillRect(hdc, pRect, blackBrush);
   DrawTongue (hdc);
   DrawTeeth (hdc);
   DrawSkin (hdc);
   DrawLips (hdc);

   DeleteObject (blackBrush);
   blackBrush = 0;

   return;
}

/**********************************************************************
PaintGender - Specified the Gender
*/
void PaintGender (BOOL fMale)
{
   gfMale = fMale;
}
