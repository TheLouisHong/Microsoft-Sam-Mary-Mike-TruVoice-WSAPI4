/************************************************************************
Date.cpp - Simple control to ask for a Date

   This includes the interface:
      IClassFactory
      IUnknown

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
#include <stdio.h>
#include <objbase.h>
#include <objerror.h>
#include <spchwrap.h>
#include "telctl.h"
#include "resource.h"

static WCHAR gszStart[] = L"AskDate";


PCWSTR CDate::GetControlName (void)
{
   return L"Date";
}


DWORD CDate::GetDefaultTextResID (LANGID langID)
{
   // since only have one, return the english one no matter what
   return IDR_DATE;
}

void CDate::GetCLSID (CLSID *pCLSID)
{
#ifdef BUILDREALCONTROL
   *pCLSID = CLSID_DateControl;
#else
   *pCLSID = CLSID_SampleDateControl;
#endif
}


void CDate::OnPhraseParse(DWORD dwParseID, PVOID pParseMem,
                          DWORD dwParseMemSize, PSRPHRASEW pSRPhrase,
                          LPUNKNOWN lpUnkResult) {
   m_dwRecog = dwParseID;
   if (pParseMem)
      wcscpy (m_szParse, (WCHAR*) pParseMem);
   else
      m_szParse[0] = L'\0';

   DoState(1); // verify
}


#define MASK_NEXTLAST                          0x000001
#define VAL_NEXTLAST_NEXT                      0x000000
#define VAL_NEXTLAST_LAST                      0x000001

#define MASK_WEEKDAY                           0x00000e

#define MASK_TIMEUNIT                          0x000070
#define VAL_TIMEUNIT_DAY                       0x000010
#define VAL_TIMEUNIT_WEEK                      0x000020
#define VAL_TIMEUNIT_FORTNIGHT                 0x000030
#define VAL_TIMEUNIT_MONTH                     0x000040
#define VAL_TIMEUNIT_YEAR                      0x000050

#define MASK_RELATIVETOTODAY                   0x000380
#define VAL_RELATIVETOTODAY_TODAY              0x000080
#define VAL_RELATIVETOTODAY_YESTERDAY          0x000100
#define VAL_RELATIVETOTODAY_DAYBEFOREYESTERDAY 0x000180
#define VAL_RELATIVETOTODAY_TOMORROW           0x000200
#define VAL_RELATIVETOTODAY_DAYAFTERTOMORROW   0x000280

#define MASK_THISNEXTLAST                      0x000c00
#define VAL_THISNEXTLAST_THIS                  0x000400
#define VAL_THISNEXTLAST_NEXT                  0x000800
#define VAL_THISNEXTLAST_LAST                  0x000c00

#define MASK_NTH                               0x01f000

#define MASK_MONTH                             0x1e0000


DWORD CDate::MungeDateFromParse(DWORD dwRecog, WCHAR *sz) {

   SYSTEMTIME systime;
   DWORD wThisYear;
   int i;

   // don't even think about munging non-existent data
   if(dwRecog <= 0)
      return dwRecog;

   // find out the local time
   GetLocalTime(&systime);
   wThisYear = systime.wYear;

   // <StartsWithNothing>=<a> (TimeUnit) <from> (RelativeToToday)
   // <StartsWithNothing>=<Natural> (TimeUnit) <from> (RelativeToToday)
   if((dwRecog & MASK_TIMEUNIT) && (dwRecog & MASK_RELATIVETOTODAY)) {
      CalcRelativeDate(&systime,
                       GetRelativeToTodayDirection(dwRecog),
                       GetRelativeToTodayNumDays(dwRecog),
                       VAL_TIMEUNIT_DAY);
      i = GetNumFromStart(sz);
      CalcRelativeDate(&systime,
                       VAL_NEXTLAST_NEXT,
                       i,
                       dwRecog & MASK_TIMEUNIT);
   }

   // <StartsWithNothing>=<a> (TimeUnit) <from> (Weekday)
   // <StartsWithNothing>=<a> (TimeUnit) <from> (NextLast) (Weekday)
   // <StartsWithNothing>=<Natural> (TimeUnit) <from> (Weekday)
   // <StartsWithNothing>=<Natural> (TimeUnit) <from> (NextLast) (Weekday)
   else if((dwRecog & MASK_TIMEUNIT) && (dwRecog & MASK_WEEKDAY)) {
      i = WeekdayDaysAway(systime.wDayOfWeek + 1, dwRecog & MASK_WEEKDAY, dwRecog & MASK_NEXTLAST);
      CalcRelativeDate(&systime,
                       dwRecog & MASK_NEXTLAST,
                       i,
                       VAL_TIMEUNIT_DAY);
      i = GetNumFromStart(sz);
      CalcRelativeDate(&systime,
                       VAL_NEXTLAST_NEXT,
                       i,
                       dwRecog & MASK_TIMEUNIT);
   }

   // <StartsWithNothing>=(RelativeToToday)
   else if(dwRecog & MASK_RELATIVETOTODAY) {
      CalcRelativeDate(&systime,
                       GetRelativeToTodayDirection(dwRecog),
                       GetRelativeToTodayNumDays(dwRecog),
                       VAL_TIMEUNIT_DAY);
   }

   // <StartsWithNothing>=<Natural> (TimeUnit) <ago>
   // <StartsWithIn>=<Natural> (TimeUnit)
   else if(dwRecog & MASK_TIMEUNIT) {
      i = wcslen(sz);
      if((i > 5) && !wcscmp((WCHAR *)(sz + i - 5), L" ago "))
         dwRecog = (dwRecog & ~MASK_NEXTLAST) | VAL_NEXTLAST_LAST;
      i = GetNumFromStart(sz);
      CalcRelativeDate(&systime,
                       dwRecog & MASK_NEXTLAST,
                       i,
                       dwRecog & MASK_TIMEUNIT);
   }

   // <StartsWithNothing>=(Month) [opt] <the> (Nth)
   // <StartsWithOn>=(Nth) <of> (Month)
   else if((dwRecog & MASK_MONTH) && (dwRecog & MASK_NTH)) {
      systime.wDay = (WORD)((dwRecog & (DWORD)MASK_NTH) >> 12);
      systime.wMonth = (WORD)((dwRecog & (DWORD)MASK_MONTH) >> 17);
   }

   // <StartsWithOn>=(Nth) <of> (ThisNextLast) <month>
   else if((dwRecog & MASK_NTH) && (dwRecog & MASK_THISNEXTLAST)) {
      systime.wDay = (WORD)((dwRecog & (DWORD)MASK_NTH) >> 12);
      switch(dwRecog & MASK_THISNEXTLAST) {
      case VAL_THISNEXTLAST_THIS:
         systime.wMonth = systime.wMonth;
         break;
      case VAL_THISNEXTLAST_NEXT:
         systime.wMonth = systime.wMonth + 1;
         if(systime.wMonth == 13)
            systime.wMonth = 1, systime.wYear++;
         break;
      case VAL_THISNEXTLAST_LAST:
         systime.wMonth = systime.wMonth - 1;
         if(systime.wMonth == 0)
            systime.wMonth = 12, systime.wYear--;
         break;
      }
   }

   // <StartsWithOn>=(NextLast) (Weekday)
   // <StartsWithOn>=(Weekday)
   else if(dwRecog & MASK_WEEKDAY) {
      i = WeekdayDaysAway(systime.wDayOfWeek + 1, dwRecog & MASK_WEEKDAY, dwRecog & MASK_NEXTLAST);
      CalcRelativeDate(&systime,
                       dwRecog & MASK_NEXTLAST,
                       i,
                       VAL_TIMEUNIT_DAY);
   }

   // <StartsWithNothing>=(Month) <Natural>
   else if(dwRecog & MASK_MONTH) {
      systime.wDay = GetNumFromEnd(sz);
      systime.wMonth = (WORD)((dwRecog & (DWORD)MASK_MONTH) >> 17);
   }

   // <StartsWithOn>=(Nth) <of> <the> <month>
   // <StartsWithOn>=(Nth)
   else if(dwRecog & MASK_NTH) {
      systime.wDay = (WORD)((dwRecog & (DWORD)MASK_NTH) >> 12);
      systime.wMonth = systime.wMonth;
   }

   else {
      // doh! shouldn't get here!
      return dwRecog;
   }

   // now actually put the date in sz
   if(wThisYear == systime.wYear)
      swprintf(sz, L"%ld/%ld", systime.wMonth, systime.wDay);
   else
      swprintf(sz, L"%ld/%ld/%ld", systime.wMonth, systime.wDay, systime.wYear);

   return 0;
}


void CDate::CalcRelativeDate(SYSTEMTIME *psystime,
                      DWORD dwDirection,
                      int iNumTimeUnits,
                      DWORD dwTimeUnit) {

   // weed out the case that we actually don't want to change anything
   if(iNumTimeUnits == 0)
      return;

   // handle days, weeks, and fortnights
   if((dwTimeUnit == VAL_TIMEUNIT_DAY) ||
      (dwTimeUnit == VAL_TIMEUNIT_WEEK) ||
      (dwTimeUnit == VAL_TIMEUNIT_FORTNIGHT)) {
      QWORD liTime, liOneDay = (QWORD)10000000 * 60 * 60 * 24, liAdjust;
      FILETIME filetime;

      SystemTimeToFileTime(psystime, &filetime);
      memcpy(&liTime, &filetime, sizeof(filetime));

      switch(dwTimeUnit) {
      case VAL_TIMEUNIT_DAY:        liAdjust = liOneDay * iNumTimeUnits;      break;
      case VAL_TIMEUNIT_WEEK:       liAdjust = 7 * liOneDay * iNumTimeUnits;  break;
      case VAL_TIMEUNIT_FORTNIGHT:  liAdjust = 14 * liOneDay * iNumTimeUnits; break;
      }
      if(dwDirection == VAL_NEXTLAST_NEXT)
         liTime += liAdjust;
      else
         liTime -= liAdjust;

      memcpy(&filetime, &liTime, sizeof(filetime));
      FileTimeToSystemTime(&filetime, psystime);
   }

   else if(dwTimeUnit == VAL_TIMEUNIT_MONTH) {
      if(dwDirection == VAL_NEXTLAST_NEXT)
         psystime->wMonth += iNumTimeUnits;
      else
         psystime->wMonth -= iNumTimeUnits;
      // in case the months are now out of the range 1-12, fix that
      if(psystime->wMonth < 32768) { // since words are unsigned, this is how we test for "positivity"
         psystime->wYear += (psystime->wMonth - 1) / 12;
         psystime->wMonth = ((psystime->wMonth - 1) % 12) + 1;
      }
      else {
         psystime->wYear -= (WORD)(12 - psystime->wMonth) / 12;
         // subtract five instead of one from psystime->wMonth because
         //   WORDs are unsigned, and so the modular arithmetic of
         //   "negative" numbers gets shifted by (2^16)%12 = 4
         // trust me, the following line is correct!
         psystime->wMonth = ((psystime->wMonth - 5) % 12) + 1;
      }
   }

   else if(dwTimeUnit == VAL_TIMEUNIT_YEAR) {
      if(dwDirection == VAL_NEXTLAST_NEXT)
         psystime->wYear += iNumTimeUnits;
      else
         psystime->wYear -= iNumTimeUnits;
   }

   // check for the ultra-special case that today is Feb. 29 but the target
   //   year is not a leap year
   if((psystime->wDay == 29) &&
      (psystime->wMonth == 2) &&
      ((((psystime->wYear % 4) == 0) && ((psystime->wYear % 400) != 0)) || ((psystime->wYear % 2000) == 0)))
      psystime->wDay = 28;
}


int CDate::GetNumFromStart(WCHAR *sz) {
   int i;

   // weed out the case that sz begins with " a "
   if(*sz == L' ')
      return 1;

   // get the number
   swscanf(sz, L"%d", &i);
   return i;
}


int CDate::GetNumFromEnd(WCHAR *sz) {
   int i;
   WCHAR *pch;

   // get the number
   pch = sz + wcslen(sz) - 1;
   while(*pch != L' ') pch--;
   pch++;
   swscanf(pch, L"%d", &i);
   return i;
}


DWORD CDate::GetRelativeToTodayDirection(DWORD dwRecog) {
   switch(dwRecog & MASK_RELATIVETOTODAY) {
   case VAL_RELATIVETOTODAY_TODAY:
   case VAL_RELATIVETOTODAY_TOMORROW:
   case VAL_RELATIVETOTODAY_DAYAFTERTOMORROW:
      return VAL_NEXTLAST_NEXT;
   case VAL_RELATIVETOTODAY_YESTERDAY:
   case VAL_RELATIVETOTODAY_DAYBEFOREYESTERDAY:
      return VAL_NEXTLAST_LAST;
   default:
      // doh! shouldn't get here!
      return VAL_NEXTLAST_NEXT;
   }
}


int CDate::GetRelativeToTodayNumDays(DWORD dwRecog) {
   switch(dwRecog & MASK_RELATIVETOTODAY) {
   case VAL_RELATIVETOTODAY_TODAY:
      return 0;
   case VAL_RELATIVETOTODAY_YESTERDAY:
   case VAL_RELATIVETOTODAY_TOMORROW:
      return 1;
   case VAL_RELATIVETOTODAY_DAYBEFOREYESTERDAY:
   case VAL_RELATIVETOTODAY_DAYAFTERTOMORROW:
      return 2;
   default:
      // doh! shouldn't get here!
      return 0;
   }
}


int CDate::WeekdayDaysAway(DWORD dwToday, DWORD dwWeekday, DWORD dwDirection) {
   int iDiff;
   DWORD dwThen = dwWeekday >> 1;

   if(dwDirection == VAL_NEXTLAST_NEXT)
      iDiff = (int)(dwThen - dwToday);
   else
      iDiff = (int)(dwToday - dwThen);
   if(iDiff <= 0)
      iDiff += 7;
   return iDiff;
}


void CDate::OnState (DWORD dwStateID) {

   switch (dwStateID) {

   case 0: // initialization
      TTSSpeak(L"Prompts", L"Main");
      ReadyToListenBeep();
      StartSRWhenReady(gszStart);
      break;
      
   case 1: // verification
      if(m_fVerify && m_szParse[0])
         DoVerify(L"VerifyPre", L"VerifyPost", m_szParse, 2, 0);
      else
         DoState(2); // don't need to verify, or shouldn't verify
      break;

   case 2: // called when everything has been verified
      m_dwRecog = MungeDateFromParse(m_dwRecog, m_szParse);
      DoFinish (m_dwRecog, m_szParse, (wcslen(m_szParse)+1)*2);
      break;

   }
}

