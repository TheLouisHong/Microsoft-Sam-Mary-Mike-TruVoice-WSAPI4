/******************************************************************************\
*       gramfunc.cpp
*
*       This module contains utility functions for extracting information
*       from a grammar file.
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
#include "stdafx.h"

#include "srtest.h"
#include "gramfunc.h"


typedef struct tagWORDENUM {
   DWORD dwWordNum;
   PSTR  pszWordName;
   DWORD dwSize;
   BOOL fFound;
} WORDENUM, *PWORDENUM;

typedef struct tagRULEENUM {
   DWORD dwRuleNum;
   PSTR  pszRuleName;
   DWORD dwSize;
   BOOL fFound;
} RULEENUM, *PRULEENUM;

typedef struct tagLISTENUM {
   DWORD dwListNum;
   PSTR  pszListName;
   DWORD dwSize;
   BOOL fFound;
} LISTENUM, *PLISTENUM;


HGRAMMAR GrammarOpen( PTCHAR pszGrammarFile )
{
   HGRAMMAR hGrammar;
   HANDLE   hGrammarFile;
   DWORD    dwBytesRead;
   SRCHUNK  srChunk;
   BYTE     *pBytes;
   DWORD    dwAlignedChunkSize;

   // Allocate the grammar object
   if ((hGrammar = (HGRAMMAR) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(GRAMMAR))) == NULL)
      return (NULL);

   // Open the grammar file for reading
   if ((hGrammarFile = CreateFile(pszGrammarFile, GENERIC_READ, FILE_SHARE_READ, 
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
      HeapFree(GetProcessHeap(), 0, hGrammar);
      return (NULL);
   }

   // Initialize the grammar object
   // First read the grammar header
   ReadFile(hGrammarFile, &hGrammar->srHeader, sizeof(SRHEADER), &dwBytesRead, NULL);

   // Now read in each and every chunk
   for (;;)
   {
      ReadFile(hGrammarFile, &srChunk, sizeof(SRCHUNK), &dwBytesRead, NULL);
      if (dwBytesRead != sizeof(SRCHUNK))
         break;
      dwAlignedChunkSize = (srChunk.dwChunkSize + 3) & ~3;
      pBytes = (BYTE *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(SRCHUNK) + dwAlignedChunkSize);
      memcpy(pBytes, &srChunk, sizeof(SRCHUNK));
      ReadFile(hGrammarFile, pBytes + sizeof(SRCHUNK), dwAlignedChunkSize, &dwBytesRead, NULL);
      switch (srChunk.dwChunkID)
      {
         case SRCK_LANGUAGE:
            hGrammar->psrckLanguage = (PSRCHUNK) pBytes;
            break;

         case SRCKCFG_WORDS:
            hGrammar->psrckCFGWords = (PSRCHUNK) pBytes;
            break;

         case SRCKCFG_RULES:
            hGrammar->psrckCFGRules = (PSRCHUNK) pBytes;
            break;

         case SRCKCFG_EXPORTRULES:
            hGrammar->psrckCFGExportRules = (PSRCHUNK) pBytes;
            break;

         case SRCKCFG_IMPORTRULES:
            hGrammar->psrckCFGImportRules = (PSRCHUNK) pBytes;
            break;

         case SRCKCFG_LISTS:
            hGrammar->psrckCFGLists = (PSRCHUNK) pBytes;
            break;

         case SRCKLD_WORDS:
            hGrammar->psrckLDWords = (PSRCHUNK) pBytes;
            break;

         case SRCKLD_GROUP:
            hGrammar->psrckLDGroup = (PSRCHUNK) pBytes;
            break;

         case SRCKLD_SAMPLE:
            hGrammar->psrckLDSample = (PSRCHUNK) pBytes;
            break;

         case SRCKD_TOPIC:
            hGrammar->psrckDTopic = (PSRCHUNK) pBytes;
            break;

         case SRCKD_COMMON:
            hGrammar->psrckDCommon = (PSRCHUNK) pBytes;
            break;

         case SRCKD_GROUP:
            hGrammar->psrckDGroup = (PSRCHUNK) pBytes;
            break;

         case SRCKD_SAMPLE:
            hGrammar->psrckDSample = (PSRCHUNK) pBytes;
            break;

         default:
            // Ignore unknown chunk types
            HeapFree(GetProcessHeap(), 0, pBytes);
            break;
      }

   } /* End of processing all chunks */
   
   // Close grammar file
   CloseHandle(hGrammarFile);

   // Return handle to grammar
   return (hGrammar);
}

void GrammarClose( HGRAMMAR hGrammar )
{
   if (hGrammar == NULL)
      return;
   if (hGrammar->psrckLanguage)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckLanguage);
   if (hGrammar->psrckCFGWords)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckCFGWords);
   if (hGrammar->psrckCFGRules)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckCFGRules);
   if (hGrammar->psrckCFGExportRules)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckCFGExportRules);
   if (hGrammar->psrckCFGImportRules)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckCFGImportRules);
   if (hGrammar->psrckCFGLists)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckCFGLists);
   if (hGrammar->psrckLDWords)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckLDWords);
   if (hGrammar->psrckLDGroup)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckLDGroup);
   if (hGrammar->psrckLDSample)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckLDSample);
   if (hGrammar->psrckDTopic)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckDTopic);
   if (hGrammar->psrckDCommon)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckDCommon);
   if (hGrammar->psrckDGroup)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckDGroup);
   if (hGrammar->psrckDSample)
      HeapFree(GetProcessHeap(), 0, hGrammar->psrckDSample);
   HeapFree(GetProcessHeap(), 0, hGrammar);
}
