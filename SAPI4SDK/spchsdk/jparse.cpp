/********************************************************************
Parse.Cpp - C++ object that reads a word at a time from a file.
   This loads the whole file in memory for speed.


Copyright c. 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"


CSharedMem::CSharedMem (void)
{
   m_dwCount = 1;
   m_pMem = NULL;
}

CSharedMem::~CSharedMem (void)
{
   if (m_pMem)
      free (m_pMem);
}

BOOL CSharedMem::Init (DWORD dwSize)
{
   if (m_pMem)
      return FALSE;
   m_pMem = malloc (dwSize);
   return m_pMem ? TRUE : FALSE;
}

void CSharedMem::AddRef (void)
{
   m_dwCount++;
}

void CSharedMem::Release (void)
{
   m_dwCount--;
   if (!m_dwCount)
      delete this;
}


CParse::CParse (void)
{
   m_dwFileSize = 0;
   m_dwFilePosn = 0;
   m_fWordMode = FALSE;
   m_pSharedFile = NULL;
}

CParse::~CParse (void)
{
   if (m_pSharedFile)
      m_pSharedFile->Release();
}

BOOL CParse::Init (char *pszFile)
{
   if (m_pSharedFile)
      return FALSE;

   FILE *f;
   f = fopen(pszFile, "rb");
   if (!f)
      return FALSE;
   fseek (f, 0, SEEK_END);
   m_dwFileSize = (DWORD) ftell(f);
   fseek (f, 0, SEEK_SET);

   // allocate the memory
   m_pSharedFile = new CSharedMem;
   if (!m_pSharedFile) {
      fclose (f);
      return FALSE;
   }
   if (!m_pSharedFile->Init(m_dwFileSize+1)) {
      fclose (f);
      return FALSE;
   }

   memset (m_pSharedFile->m_pMem, 0, m_dwFileSize+1);
   fread (m_pSharedFile->m_pMem, 1, m_dwFileSize, f);

   fclose (f);

   return TRUE;
}

BOOL CParse::MyIsSpace (char cTemp)
{
   if (m_fWordMode)
      return (BOOL) isspace(cTemp);
   else
      return ((cTemp == '\n') || (cTemp == '\r'));
}

BOOL CParse::Next (char *szWord, DWORD dwWordSize)
{
   if (!m_pSharedFile)
      return FALSE;

   // wait till we're in a word
   char  cTemp;
   while (TRUE) {
      if (m_dwFilePosn >= m_dwFileSize)
         return FALSE;  // EOF
      cTemp = ((char*)m_pSharedFile->m_pMem)[m_dwFilePosn++];
      if (!cTemp) return FALSE;  // EOF
      if (!MyIsSpace(cTemp))
         break;   // letter
   }

   // add this
   if (dwWordSize > 1) {
      *(szWord++) = cTemp;
      dwWordSize--;
   }

   // wait till we're out
   while (TRUE) {
      if (m_dwFilePosn >= m_dwFileSize)
         break;  // EOF
      cTemp = ((char*)m_pSharedFile->m_pMem)[m_dwFilePosn++];
      if (!cTemp) break;   // EOF
      if (MyIsSpace(cTemp))
         break;   // space

      // add it
      if (dwWordSize > 1) {
         *(szWord++) = cTemp;
         dwWordSize--;
      }
   }

   // null terminate
   *szWord = 0;

   return TRUE;
}

CParse *CParse::Clone (void)
{
   if (!m_pSharedFile)
      return NULL;

   CParse   *newParse;

   newParse = new CParse();
   if (!newParse)
      return NULL;

   // copy the file contents
   newParse->m_dwFileSize = m_dwFileSize;
   newParse->m_fWordMode = m_fWordMode;
   newParse->m_dwFilePosn = m_dwFilePosn;
   newParse->m_pSharedFile = m_pSharedFile;
   m_pSharedFile->AddRef();

   // done
   return newParse;
}

