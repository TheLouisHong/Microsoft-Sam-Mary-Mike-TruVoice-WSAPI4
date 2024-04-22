/***********************************************************************
LogFile.Cpp - Code that opens a log file.


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <objbase.h>
#include <objerror.h>
#include <speech.h>
#include "ctools.h"
#include "logfile.h"


/***********************************************************************
CLogFile::CLogFile - Constructor. This does nothing.

*/
CLogFile::CLogFile(void)
{
   //intentially left blank
   return;
}

/***********************************************************************
CLogFile::~CLogFile - Frees up memory allocated
*/
CLogFile::~CLogFile(void)
{
   //intentially left blank
   return;
}


/***********************************************************************
CLogFile::FileOpen - Opens a file and reads in the log entries. Returns
   FALSE if it can't open, perhaps because its not a valid log file.

inputs
   char  *pszFile - file name
returns
   BOOL - TRUE if successful
*/
BOOL CLogFile::FileOpen (char *pszFile)
{
   CMem  mem;

   if (m_list.GetNumElems())
      return FALSE;  // can't open when already have something openend

   FILE  *f;
   f = fopen (pszFile, "rb");
   if (!f)
      return FALSE;  // can't find

   // make sure it has the right header
   CLSID cls;
   size_t   ret;
   ret = fread (&cls, sizeof(cls), 1, f);
   if (!ret || !IsEqualCLSID(cls, CLSID_STLog)) {
      fclose (f);
      return FALSE;
   }

   DWORD dwSize;
   while (TRUE) {
      // start reading in
      ret = fread (&dwSize, sizeof(dwSize), 1, f);
      if (!ret || (dwSize < sizeof(DWORD)) )
         break;   // end of file
      dwSize -= sizeof(DWORD);

      // make sure have enough memory
      if (!mem.Required(dwSize))
         break;   // error, but can't do much about it

      // load in data
      ret = fread (mem.Pointer(), 1, dwSize, f);
      if (ret != dwSize)
         break;   // another error

      // add it
      m_list.AddElem (mem.Pointer(), dwSize);
   }

   fclose (f);

   return TRUE;
}


/***********************************************************************
CLogFile::GetNumElems - Returns the number fo elements.

returns
   DWORD dwNum
*/
DWORD CLogFile::GetNumElems (void)
{
   return m_list.GetNumElems();
}


/***********************************************************************
CLogFile::GetSource - Returns a pointer to the source string.

inputs
   DWORD dwElem
returns
   PCWSTR - String. NULL if eror
*/
PCWSTR CLogFile::GetSource (DWORD dwElem)
{
   PBYTE pElem;

   pElem = (PBYTE) m_list.GetElem(dwElem);
   if (!pElem)
      return NULL;


   return (PCWSTR) (pElem + (sizeof(DWORD) + sizeof(FILETIME) + sizeof(WORD)));
}



/***********************************************************************
CLogFile::GetMessage - Returns a pointer to the source string.

inputs
   DWORD dwElem
returns
   PCWSTR - String. NULL if eror
*/
PCWSTR CLogFile::GetMessage (DWORD dwElem)
{
   PCWSTR   pSource;

   pSource = GetSource (dwElem);
   if (!pSource)
      return NULL;

   return pSource + (wcslen(pSource) + 1);
}



/***********************************************************************
CLogFile::GetDetail - Returns the detail level of an element. 0xffff if error

inputs
   DWORD dwElem
returns
   WORD
*/
WORD CLogFile::GetDetail (DWORD dwElem)
{
   PBYTE pElem;

   pElem = (PBYTE) m_list.GetElem(dwElem);
   if (!pElem)
      return 0xffff;


   return *((WORD*) (pElem + (sizeof(DWORD) + sizeof(FILETIME))));
}




/***********************************************************************
CLogFile::GetData - Returns a pointer to the data of an element. NULL
   if error

inputs
   DWORD dwElem
   DWORD *pdwSize - Filled with the size
returns
   PVOID
*/
PVOID CLogFile::GetData (DWORD dwElem, DWORD *pdwSize)
{
   PBYTE pElem;

   pElem = (PBYTE) m_list.GetElem(dwElem);
   if (!pElem)
      return NULL;


   *pdwSize = *((DWORD*) pElem);

   // find where the memory begins
   PCWSTR   psz;
   psz = GetMessage(dwElem);
   if (!psz)
      return NULL;

   return (PVOID) (psz + (wcslen(psz)+1));
}




/***********************************************************************
CLogFile::GetFileTime - Returns the file time of an element.

inputs
   DWORD dwElem
   FILETIME *pft - Filled in with the filetime
returns
   BOOL - TRUE if succeded, FALSE if fail
*/
BOOL CLogFile::GetFileTime (DWORD dwElem, FILETIME *pft)
{
   PBYTE pElem;

   pElem = (PBYTE) m_list.GetElem(dwElem);
   if (!pElem)
      return FALSE;


   *pft = *((FILETIME*) (pElem + sizeof(DWORD) ));
   return TRUE;
}


/***********************************************************************
CLogFile::MakeWave - Given a start poisition, this makes a wave file
   (in memory) out of it. It does this by:
      1) Look from the current position back for a "WAVEFORMATEX"
            and use that.
      2) Look from the current position back for an "UtteranceBegin".
            If it's found, it goes about two seconds before that.
            It not found, it goes to the first data after WAVEFORMATEX
      3) Look from the current position to the end for the
            next "WAVEFORMATEX" and stop before then. If had found an
            utterance begin then stop at an "UtteranceEnd"
      4) Combine all of the "DataGet" data between the start & end to
            make the wave data
      5) Construct the wave file and write to disk

inputs
   DWORD dwCur - Current position to start looking at
   char  *pszFile - File name
returns
   BOOL - TRUE if successful, FALSE if fail
*/
BOOL CLogFile::MakeWave (DWORD dwCur, char *pszFile)
{
   // number of elements
   DWORD dwNum;
   dwNum = GetNumElems();

   // loop from the current position to the beginning looking for WFEX
   PCWSTR   psz;
   DWORD    i;
   DWORD    dwWFEX = (DWORD)-1L;
   for (i = dwCur; i && (i < (DWORD)-1L); i--) {
      psz = GetMessage(i);
      if (!_wcsicmp(psz, L"WAVEFORMATEX")) {
         dwWFEX = i;
         break;
      }
   }
   if (dwWFEX == (DWORD)-1L)
      return FALSE;  // can't find WFEX
   WAVEFORMATEX   *pWFEX;
   DWORD          dwWFEXSize;
   pWFEX = (WAVEFORMATEX*) GetData (dwWFEX, &dwWFEXSize);
   if (!pWFEX)
      return FALSE;

   // loop and look for utterance begin
   DWORD dwUttBegin = 0;
   for (i = dwCur; i > dwWFEX; i--) {
      psz = GetMessage(i);
      if (!_wcsicmp(psz, L"UtteranceBegin")) {
         dwUttBegin = i;
         break;
      }
   }

   // if we have an utterance begin then find a start
   DWORD dwStartDigital;
   PBYTE pData;
   DWORD dwData;
   if (dwUttBegin) {
      int   iTwoSec;
      iTwoSec = (int) pWFEX->nAvgBytesPerSec * 2;

      for (i = dwUttBegin-1; i > dwWFEX; i--) {
         psz = GetMessage(i);

         // if find an utterance end then abort here
         // Don't want one utterance running into another
         if (!_wcsicmp(psz, L"UtteranceEnd"))
            break;

         if (_wcsicmp(psz, L"DataGet"))
            continue;

         // see how much data
         pData = (PBYTE) GetData (i, &dwData);
         iTwoSec -= (int) dwData;

         if (iTwoSec <= 0)
            break;
      }

      // if got here then this is the start of the digital
      // no matter what
      dwStartDigital = i;

   }
   else
      dwStartDigital = dwWFEX+1;
   
   // Find the end of the data
   DWORD dwEndDigital;
   for (i = dwUttBegin ? dwUttBegin : max(dwCur,dwStartDigital); i < dwNum; i++) {
      psz = GetMessage(i);

      // if find a WFEX then end
      if (!_wcsicmp(psz, L"WAVEFORMATEX"))
         break;

      // if find an utterance end then break
      if (dwUttBegin && !_wcsicmp(psz, L"UtteranceEnd"))
         break;
   }
   dwEndDigital = i;

   // write it out
   HMMIO hFileWrite;
   hFileWrite = mmioOpen( pszFile,
	   NULL, MMIO_CREATE | MMIO_WRITE | MMIO_EXCLUSIVE | MMIO_ALLOCBUF );
   if(!hFileWrite) return FALSE;   // cant create

   // Write the main chunk
   MMCKINFO ckMain;
   memset (&ckMain, 0, sizeof(ckMain));
   ckMain.fccType = mmioFOURCC('W','A','V','E');
   mmioCreateChunk (hFileWrite, &ckMain, MMIO_CREATERIFF);

   // Get the wave format and write it
   MMCKINFO ckFmt;
   memset(&ckFmt, 0, sizeof(ckFmt));
   ckFmt.ckid = mmioFOURCC('f','m','t',' ');
   mmioCreateChunk (hFileWrite, &ckFmt,0);
   mmioWrite (hFileWrite, (char*) pWFEX, dwWFEXSize);
   mmioAscend (hFileWrite, &ckFmt, 0);

   // Start the data section
   MMCKINFO ckFileWrite;
   memset(&ckFileWrite, 0, sizeof(ckFileWrite));
   ckFileWrite.ckid = mmioFOURCC('d','a','t','a');
   mmioCreateChunk (hFileWrite, &ckFileWrite,0);

   // loop through all the data and write out
   for (i = dwStartDigital; i < dwEndDigital; i++) {
      psz = GetMessage(i);

      if (_wcsicmp(psz, L"DataGet"))
         continue;

      // see how much data
      pData = (PBYTE) GetData (i, &dwData);

      // if we got here then we have a wave file, so write
      mmioWrite (hFileWrite, (char*) pData, dwData);
   }

   // finish up
	mmioAscend (hFileWrite, &ckFileWrite, 0);
	mmioAscend (hFileWrite, &ckMain, 0);
	mmioClose (hFileWrite, 0);

   return TRUE;
}




