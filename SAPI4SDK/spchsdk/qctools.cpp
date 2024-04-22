/*********************************************************************
CTools.Cpp - Misc tools for C++ wrapper objects.

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
#include "spchwrap.h"


#define  NOLINE            0xffffffff

#define  LINE_SECTION      1
#define  LINE_VALUE        2
#define  LINE_COMMENT      3
#define  LINE_UNK          4
#define  LINE_MULTILINE    5

typedef struct {
   PWSTR    pStart;  // start of the line
   DWORD    dwType;  // type of line
} TEXTLINEINFO, *PTEXTLINEINFO;

// ignore first character with fffe
#define  ISCR(x)  (((x) == L'\n') || ((x) == L'\r') || ((x) == 0xfeff))

/************************************************************************
CInfParse::CInfParse - Constructire. Takes a string of memory with the inf
   text. Null terminated.

inputs
   LPCWSTR  pszText - text to parse
returns
   none
*/
CInfParse::CInfParse (LPCWSTR pszText, DWORD dwChars)
{
   // initializes variables to 0
   m_pszInf = NULL;
   m_dwCurSection = m_dwCurValue = NOLINE;

   // copy the text to an internal buffer
   DWORD dwLen = dwChars;
   m_pszInf = (LPWSTR) malloc((dwLen + 1) * sizeof(WCHAR));
   if (!m_pszInf)
      return;  // error
   wcsncpy (m_pszInf, pszText, dwChars);
   m_pszInf[dwChars] = 0;

   // loop through the strucuture and find all the new lines. Also,
   // put NULLs where the CR are.
   

   WCHAR *pCur;
   pCur = m_pszInf;

   // start
   while (TRUE) {
      // skip past any initial lines
      while (*pCur && ISCR(*pCur))
         pCur++;
      if (!(*pCur))
         break;   // no real data here

      // we know we're at the beginning of a line.
      // Find the end
      WCHAR *pEnd;
      BOOL  fFoundEquals, fMultiLine;;
      DWORD dwNumParen = 0;
      fFoundEquals = fMultiLine = FALSE;
      for (pEnd = pCur; *pEnd; pEnd++) {
         // if multiple-line mark then end only if find 3 paren in a row
         // followed by a CR, else, end if find a CR
         if ((fMultiLine ? (dwNumParen >= 3): TRUE) && ISCR(*pEnd))
            break;

         // if it's a paren remember
         if (pEnd[0] == L')')
            dwNumParen++;
         else
            dwNumParen = 0;

         if (*pEnd == L'=') {
            fFoundEquals = TRUE;

            // see if it's multiple lines
            if (!fMultiLine && pEnd[1] == L'(' && pEnd[2] == L'(' && pEnd[3] == L'(')
               fMultiLine = TRUE;
         }
      }

      // what type of line is it?
      TEXTLINEINFO li;
      li.pStart = pCur;
      if (*pCur == L'[')
         li.dwType = LINE_SECTION;
      else if (*pCur == L';')
         li.dwType = LINE_COMMENT;
      else if ((pCur[0] == L'/') && (pCur[1] == L'/'))
         li.dwType = LINE_COMMENT;
      else if (fFoundEquals) {
         li.dwType = fMultiLine ? LINE_MULTILINE : LINE_VALUE;
      }
      else
         li.dwType = LINE_UNK;

      // add to the list
      m_lines.AddElem (&li, sizeof(li));

      // null terminate line and go on
      if (!(*pEnd))
         break;   // already at end of data

      // if multiline then clear out 3 previous values, since they're
      // ending parenthesis
      if (fMultiLine)
         pEnd[-1] = pEnd[-2] = pEnd[-3] = 0;

      *pEnd = 0;
      pCur = pEnd + 1;
   }

   SectionReset();

   // done

}


/************************************************************************
CInfParse::~CInfParse - Destructur
*/
CInfParse::~CInfParse ()
{
   if (m_pszInf)
      free (m_pszInf);
}


/************************************************************************
CInfParse::SectionReset - Resets the section search to the beginning
of the text file, and finds the first section.

inputs
returns
   BOOL - TRUE if found a section
*/
BOOL CInfParse::SectionReset (void)
{
   DWORD i;
   PTEXTLINEINFO pli;

   m_dwCurSection = m_dwCurValue = NOLINE;


   for (i = 0; pli = (PTEXTLINEINFO) m_lines.GetElem(i); i++) {
      if (pli->dwType == LINE_SECTION) {
         // found it
         m_dwCurSection = i;
         ValueReset();
         return TRUE;
      }
   }

   // cant find any sections at all
   return FALSE;
}



/************************************************************************
CInfParse::SectionQuery - Returns the current section name.

inputs
   LPWSTR   pszSectionName - Filled with the sction name. If NULL,
               iL's not filled.
   DWORD    *pdwSectionNameSize - Start the variable out with the
               number of characters that can fill (includeing the null).
               Will be filled with the number of characters needed.
returns
   BOOL - TRUE if found a section and filled in a name.
*/
BOOL CInfParse::SectionQuery (LPWSTR pszSectionName, DWORD *pdwSectionNameSize)
{
   // are we in a sction
   PTEXTLINEINFO   pli;
   pli = (PTEXTLINEINFO) m_lines.GetElem (m_dwCurSection);
   if (!pli) {
      *pdwSectionNameSize = 0;
      return FALSE;
   }

   // how long is it
   WCHAR *pStart;
   WCHAR *pEnd;
   pStart = pli->pStart + 1;  // skip the left bracket
   for (pEnd = pStart; *pEnd && (*pEnd != L']'); pEnd++);
   DWORD dwLen;
   dwLen  = (DWORD) (pEnd - pStart) + 1;
   if (dwLen > *pdwSectionNameSize) {
      *pdwSectionNameSize = dwLen;
      return FALSE;
   }

   // copy
   if (pszSectionName) {
      for (pEnd = pStart; *pEnd && (*pEnd != L']'); pEnd++)
         *(pszSectionName++) = *pEnd;
      *pszSectionName = 0;
   }

   // done
   return TRUE;
}



/************************************************************************
CInfParse::SectionNext - Moves onto the next section.

inputs
returns
   BOOL - TRUE if found a section and filled in a name.
*/
BOOL CInfParse::SectionNext (void)
{
   if (m_dwCurSection == NOLINE)
      return FALSE;  // end of sections

   // find the next one
   PTEXTLINEINFO   pli;
   m_dwCurSection++;
   for (; pli = (PTEXTLINEINFO) m_lines.GetElem (m_dwCurSection); m_dwCurSection++) {
      if (pli->dwType == LINE_SECTION) {
         ValueReset();
         return TRUE;
      }
   }

   // couldn't find anything
   return FALSE;
}




/************************************************************************
CInfParse::SectionFind - Finds and moves to a section with the given
   name, or returns FALSE if it can't find it. Case insensative.

inputs
   LPWSTR   pszSectionName - Stion name.

returns
   BOOL - TRUE if found a section and filled in a name.
*/
BOOL CInfParse::SectionFind (LPCWSTR pszSectionName)
{
   // reset
   if (!SectionReset())
      return FALSE;

   // allocate enough memory for a section name
   WCHAR *pMem;
   DWORD dwLen;
   dwLen = wcslen(pszSectionName) + 2;
   pMem = (WCHAR*) malloc(dwLen * sizeof(WCHAR));
   if (!pMem)
      return FALSE;

   // repeat
   while (TRUE) {
      DWORD dwSize;
      dwSize = dwLen;
      if (SectionQuery(pMem, &dwSize)) {
         // found something, does it match
         if (!_wcsicmp(pszSectionName, pMem)) {
            free (pMem);
            return TRUE;
         }
      };
      // else it was either the wrong size, or nothing left

      // go forward
      if (!SectionNext())
         break;
   }

   // not found
   free (pMem);
   return FALSE;
}


/************************************************************************
CInfParse::ValueReset - Resets the Value search to the beginning
of the section, and finds the first Value.

inputs
returns
   BOOL - TRUE if found a Value and filled in a name/value.
*/
BOOL CInfParse::ValueReset (void)
{
   PTEXTLINEINFO pli;

   if (m_dwCurSection == NOLINE)
      return FALSE;
   for (m_dwCurValue = m_dwCurSection + 1;
      pli = (PTEXTLINEINFO) m_lines.GetElem(m_dwCurValue); m_dwCurValue++) {
         // if iL's a new value then we're done (or if unknown)
         if ((pli->dwType == LINE_VALUE) || (pli->dwType == LINE_MULTILINE) || (pli->dwType == LINE_UNK))
            return TRUE;

         // if iL's a new section we're at the end
         if (pli->dwType == LINE_SECTION)
            break;
      }

   // if we got here we didn't find anything
   m_dwCurValue = NOLINE;
   return FALSE;
}


/************************************************************************
CInfParse::ValueQuery - Returns the current Value name and value.

inputs
   LPWSTR   pszValueName - Filled with the Value name. If NULL,
               iL's not filled.
   DWORD    *pdwValueNameSize - Start the variable out with the
               number of characters that can fill (includeing the null).
               Will be filled with the number of characters needed.
   LPWSTR   pszValueValue - Filled with the Value value. If NULL,
               iL's not filled.
   DWORD    *pdwValueNameValue - Start the variable out with the
               number of characters that can fill (includeing the null).
               Will be filled with the number of characters needed.
returns
   BOOL - TRUE if found a Value and filled in a name.
*/
BOOL CInfParse::ValueQuery (LPWSTR pszValueName, DWORD *pdwValueNameSize,
                            LPWSTR pszValueValue, DWORD *pdwValueValueSize)
{
   // are we on a value?
   PTEXTLINEINFO   pli;
   pli = (PTEXTLINEINFO) m_lines.GetElem (m_dwCurValue);
   if (!pli) {
      if (pdwValueNameSize)
         *pdwValueNameSize = 0;
      if (pdwValueValueSize)
         *pdwValueValueSize = 0;
      return FALSE;
   }

   // isolate start, equals, and end
   WCHAR *pStart;
   WCHAR *pEquals;
   WCHAR *pEnd;
   BOOL  fMultiLine;
   fMultiLine = (pli->dwType == LINE_MULTILINE);
   pStart = pli->pStart;
   for (pEquals = pStart; *pEquals && (*pEquals != L'='); pEquals++);
   if (*pEquals) {
      for (pEnd = pEquals + (fMultiLine ? 4 : 1); *pEnd; pEnd++);
   }
   else
      pEnd = NULL;

   // how long are both parts?
   DWORD dwLenLeft, dwLenRight;
   BOOL  fCouldCopy = TRUE;
   dwLenLeft = (DWORD) (pEquals - pStart) + 1;
   if (pdwValueNameSize) {
      if (dwLenLeft <= *pdwValueNameSize) {
         if (pszValueName) {
            wcsncpy (pszValueName, pStart, dwLenLeft - 1);
            pszValueName[dwLenLeft-1] = 0;

            // eliminate extra spaces from the end
            int iPosn;
            iPosn = (int) dwLenLeft - 2;
            for (;iPosn; iPosn--)
               if (pszValueName[iPosn] == L' ')
                  pszValueName[iPosn] = 0;
               else
                  break;
         }
      }
      else
         fCouldCopy = FALSE;
      *pdwValueNameSize = dwLenLeft;
   }
   
   // if multiple lines then up pEquals
   if (fMultiLine && pEquals)
      pEquals += 3;

   dwLenRight = pEnd ? (DWORD) (pEnd - pEquals) : 0;
   if (pdwValueValueSize) {
      if (dwLenRight <= *pdwValueValueSize) {
         if (pszValueValue)
            pszValueValue[0] = 0;
         if (pszValueValue && pEnd) {
            // eleminate spacces from the beginning
            pEquals++;
            for (; (*pEquals == L' ') && (dwLenRight >= 2); pEquals++, dwLenRight--);

            wcsncpy (pszValueValue, pEquals, dwLenRight- 1);
            pszValueValue[dwLenRight-1] = 0;
         }
      }
      else
         fCouldCopy = FALSE;
      *pdwValueValueSize = dwLenRight;
   }

   // done
   return fCouldCopy;
}




/************************************************************************
CInfParse::ValueQuery (overloaded)- Returns the current Value name and value.

inputs
   LPWSTR   pszValueName - Filled with the Value name. If NULL,
               iL's not filled.
   DWORD    *pdwValueNameSize - Start the variable out with the
               number of characters that can fill (includeing the null).
               Will be filled with the number of characters needed.
   LONG     *plValue - Filled with the value
returns
   BOOL - TRUE if found a Value and filled in a name.
*/
BOOL CInfParse::ValueQuery (LPWSTR pszValueName, DWORD *pdwValueNameSize,
                            LONG *plValue)
{
   WCHAR szTemp[64];
   DWORD dwLen;
   dwLen = sizeof(szTemp) / sizeof(WCHAR);

   *plValue = 0;
   if (ValueQuery(pszValueName, pdwValueNameSize, szTemp, &dwLen)) {
      *plValue = wcstol (szTemp, NULL, 10);
      return TRUE;
   }
   else
      return FALSE;
      
}


/************************************************************************
CInfParse::ValueNext - Moves onto the next Value.

inputs
returns
   BOOL - TRUE if found a section.
*/
BOOL CInfParse::ValueNext (void)
{
   PTEXTLINEINFO pli;

   if (m_dwCurValue == NOLINE)
      return FALSE;
   for (++m_dwCurValue;
      pli = (PTEXTLINEINFO) m_lines.GetElem(m_dwCurValue); m_dwCurValue++) {
         // if iL's a new value then we're done (or if unknown)
         if ((pli->dwType == LINE_VALUE) || (pli->dwType == LINE_MULTILINE) || (pli->dwType == LINE_UNK))
            return TRUE;

         // if iL's a new section we're at the end
         if (pli->dwType == LINE_SECTION)
            break;
      }

   // if we got here we didn't find anything
   m_dwCurValue = NOLINE;
   return FALSE;
}



/************************************************************************
CInfParse::ValueFind - Finds and moves to a Value with the given
   name, or returns FALSE if it can't find it. Case insensative.

inputs
   LPWSTR   pszValueName - Value name.

returns
   BOOL - TRUE if found a section.
*/
BOOL CInfParse::ValueFind (LPCWSTR pszValueName)
{
   // reset
   if (!ValueReset())
      return FALSE;

   // allocate enough memory for a value name
   WCHAR *pMem;
   DWORD dwLen;
   dwLen = wcslen(pszValueName) + 2;
   pMem = (WCHAR*) malloc(dwLen * sizeof(WCHAR));
   if (!pMem)
      return FALSE;

   // repeat
   while (TRUE) {
      DWORD dwSize;
      dwSize = dwLen;
      if (ValueQuery(pMem, &dwSize, NULL, NULL)) {
         // found something, does it match
         if (!_wcsicmp(pszValueName, pMem)) {
            free (pMem);
            return TRUE;
         }
      };
      // else it was either the wrong size, or nothing left

      // go forward
      if (!ValueNext())
         break;
   }

   // not found
   free (pMem);
   return FALSE;
}




/*************************************************************************
NextToken - Parses the next token in the string and returns values.
   Tokens are separarated by white-space

inputs
   WCHAR    *pCur - Current positiion
   WCHAR    **ppStart - Filled with the start of the token
   WCHAR    *pdwSize - Filled with the size of the token, in characters
returns
   WCHAR * - Pointing to just after the token. NULL if no more tokens
               were found
*/
WCHAR * NextToken (WCHAR *pCur, WCHAR **ppStart, DWORD *pdwSize)
{
   // find the start of the token
   WCHAR *pStart;
   for (pStart = pCur; *pStart && iswspace(*pStart); pStart++);
   if (!(*pStart)) {
      *ppStart = NULL;
      *pdwSize = 0;
      return NULL;
   }

   // find the end of the token. We make a special exception if there's
   // quotes, in which case the end is another quote which is not preceeded
   // by a backslash
   BOOL  fQuote;
   fQuote = (*pStart == L'"');
   WCHAR *pEnd;
   for (pEnd = pStart+1; *pEnd; pEnd++) {
      if (fQuote) {
         // quote, so break at end of quote
         // IMPORTANT: This might have some parse problems is combine
         // back-slashes and quotes in a string
         if ((pEnd[0] == L'"') && (pEnd[-1] != L'\\')) {
            pEnd++;
            break;
         }
      }
      else {
         // not a quoute, so break when white space
         if (iswspace(*pEnd))
            break;
      }
   }

   // fill in the paremeters
   *ppStart = pStart;
   *pdwSize = ((DWORD) pEnd - (DWORD) pStart) / 2;
   return pEnd;
}


/*************************************************************************
ExtractToken - Given a token beginning and ending, this identifies
   the token class, and copies the meat (without the brackets or quotes)
   of the token into another buffer.

inputs
   WCHAR    *pStart - start
   DWORD    dwSize - number of characters
   WCHAR    *pszCopyTo - Where it gets copied to. It will be copied
               with NULL termination
returns
   WCHAR - class. The classes right now are:
      NULL - no class. (Or doesn't consistently being and end with cahracter
      '<' - begins and ends with chevrons
      '[' - begins and ends with brackets
      '{' - begins and ends with curly braces
      '(' - begins and ends with parenthesis
      '"' - begins and ends with quotes
*/
WCHAR ExtractToken (WCHAR *pStart, DWORD dwSize, WCHAR *pszCopyTo)
{
   // if dwSize == 1 then it's obviously null
   if (dwSize == 1) {
      pszCopyTo[0] = pStart[0];
      pszCopyTo[1] = 0;
      return 0;
   }

   WCHAR wType;
   wType = 0;  // assume at first
   switch (pStart[0]) {
   case '<':
      if (pStart[dwSize-1] == '>')
         wType = '<';
      break;
   case '[':
      if (pStart[dwSize-1] == ']')
         wType = '[';
      break;
   case '{':
      if (pStart[dwSize-1] == '}')
         wType = '{';
      break;
   case '(':
      if (pStart[dwSize-1] == ')')
         wType = '(';
      break;
   case '"':
      // special since we convert magic characters
      if (pStart[dwSize-1] != '"')
         break;   // not a quote
      pStart++; // skip first quote
      dwSize -= 2;   // remove to quites
      while (dwSize) {
         if ((pStart[0] == '\\') && (dwSize >= 2)) {
            WCHAR cWrite = pStart[1];
            if ((pStart[1] >= L'A') && (pStart[1] <= L'Z'))
               cWrite = pStart[1] - L'A' + 1;
            else if ((pStart[1] >= L'a') && (pStart[1] <= L'z'))
               cWrite = pStart[1] - L'a' + 1;
            *pszCopyTo = cWrite;
            pStart += 2;
            pszCopyTo++;
            dwSize -= 2;
         }
         else {
            // normal character
            *pszCopyTo = *pStart;
            pStart++;
            pszCopyTo++;
            dwSize--;
         }
      }
      *pszCopyTo = 0;   // null terminate
      return '"';
   }

   DWORD i;
   for (i = 0; i < (wType ? (dwSize-2) : dwSize); i++)
      pszCopyTo[i] = pStart[i + (wType ? 1 : 0)];
   pszCopyTo[i] = 0;

   return wType;
}


/************************************************************************
CSWList constructur
*/
CSWList::CSWList (void)
{
dwNumElems = 0;
paElems = 0;
dwBufSize = 0;
}

/************************************************************************
CSWList destructor - This frees up all of the memory.
*/
CSWList::~CSWList (void)
{
DWORD i;

if (paElems) {
   for (i = 0; i < dwNumElems; i++)
      free (paElems[i].pElem);
   free ((PVOID) paElems);
   };
}


/************************************************************************
MakeListMemoryThisBig - This makes sure that the list memory is large
   neough to handle spe specified number of elements (in dwNUmElems)
   It returns an error if that fails.
*/
BOOL CSWList::MakeListMemoryThisBig (DWORD dwNumElems)
{
DWORD    dwSizeNeeded = dwNumElems * sizeof(LISTELEM);
PVOID    pTemp;

if (dwSizeNeeded <= dwBufSize) return TRUE;

// Just to be safe, add a bit extra onto what's needed, so we
// dont have to continually realloc
dwSizeNeeded += (dwSizeNeeded / 2);

if (paElems) {
   pTemp = realloc (paElems, dwSizeNeeded);
   if (!pTemp)
      return FALSE;  // cant do it
   paElems = (PLISTELEM) pTemp;
   dwBufSize = dwSizeNeeded;
   return TRUE;   // done
   };

// else its a job for malloc
paElems = (PLISTELEM) malloc (dwSizeNeeded);
if (paElems)
   dwBufSize = dwSizeNeeded;
return (paElems != NULL);
}


/************************************************************************
GetNumElems - Returns the number of elements in the list.
*/
DWORD CSWList::GetNumElems (void)
{
return dwNumElems;
}


/************************************************************************
AddElem - Add an element onto the end of the list. pData is a pointer to
   memory containing the elemnt information (which is copied into internal memory)
   and dwSize is the size of the element information. This returns TRUE if
   it was successful.
*/
BOOL CSWList::AddElem (PVOID pData, DWORD dwSize)
{
PVOID pTemp;

if (!pData || !dwSize)
   return FALSE;

if (!this->MakeListMemoryThisBig (dwNumElems+1))
   return FALSE;       

pTemp = malloc (dwSize);
if (!pTemp) return FALSE;
memcpy (pTemp, pData, dwSize);

paElems[dwNumElems].pElem = pTemp;
paElems[dwNumElems].dwElemSize = dwSize;
dwNumElems++;

return TRUE;
}


/************************************************************************
InsertElem - This inserts an element into the middle of the list. dwElemNum
   is the element to insert before, and the rest is just like AddElem.
*/
BOOL CSWList::InsertElem (DWORD dwElemNum, PVOID pData, DWORD dwSize)
{
PVOID pTemp;

if (dwElemNum >= dwNumElems)
   return this->AddElem(pData, dwSize);

if (!pData || !dwSize)
   return FALSE;

if (!this->MakeListMemoryThisBig (dwNumElems+1))
   return FALSE;

pTemp = malloc (dwSize);
if (!pTemp) return FALSE;
memcpy (pTemp, pData, dwSize);

paElems[dwNumElems].pElem = pTemp;
paElems[dwNumElems].dwElemSize = dwSize;
memmove (paElems + (1+dwElemNum), paElems + dwElemNum,
   (dwNumElems - dwElemNum) * sizeof(LISTELEM));
dwNumElems++;
paElems[dwElemNum].pElem = pTemp;
paElems[dwElemNum].dwElemSize = dwSize;

return TRUE;
}

/************************************************************************
RemoveElem - This removes a specific element number from the list.
*/
BOOL CSWList::RemoveElem (DWORD dwElemNum)
{
if (dwElemNum >= dwNumElems)
   return FALSE;

// free the memory
if (paElems[dwElemNum].pElem)
   free (paElems[dwElemNum].pElem);

memmove (paElems + dwElemNum, paElems + (dwElemNum+1),
   (dwNumElems - dwElemNum - 1) * sizeof(LISTELEM));
dwNumElems--;
return TRUE;
}


/************************************************************************
GetElemSize - This gets the size of a specific element number, defined
   by dwElemNum
*/
DWORD CSWList::GetElemSize (DWORD dwElemNum)
{
if (dwElemNum >= dwNumElems)
   return 0;

return paElems[dwElemNum].dwElemSize;
}


/************************************************************************
GetElem - This returns a pointer to memory containing the element's data.
   The element # is defined by dwElemNum. The memory is the actual memory
   used by the list for storage, so the memory should not be re-sized or
   freed. It can be safely altered. If the element is ever freed then the
   memory will become invalid.
*/
PVOID CSWList::GetElem (DWORD dwElemNum)
{
if (dwElemNum >= dwNumElems)
   return NULL;
return paElems[dwElemNum].pElem;
}

/************************************************************************
SetElem - This replaces the memory of an element with new data. The element
   numbeer is defined by dwElemNum. If GetElem() was called for this element
   then the pointer will become invalid.
*/
BOOL CSWList::SetElem (DWORD dwElemNum, PVOID pData, DWORD dwSize)
{
if (dwElemNum >= dwNumElems)
   return FALSE;

if (!this->RemoveElem(dwElemNum))
   return FALSE;
return this->InsertElem(dwElemNum, pData, dwSize);
}

/****************************************************************8
CSWList::Clone - Clone the current list into a new list. Returns a new list
*/
CSWList * CSWList::Clone (void)
{
CSWList * pNew;
DWORD	dwNum, i;
DWORD	dwSize;
PVOID	pMem;

pNew = new CSWList;
if (!pNew)
	return NULL;
dwNum = this->GetNumElems();

for (i = 0; i < dwNum; i++) {
	pMem = this->GetElem(i);
	dwSize = this->GetElemSize(i);
	if (!pMem) {
		// error
		delete pNew;
		return NULL;
		};

	// add this
	if (!pNew->AddElem (pMem, dwSize)) {
		// error
		delete pNew;
		return NULL;
		};

	// OK, its added
	};

// if we got here we're OK
return pNew;
}


/**************************************************************************
CTreeList - List that's accessable by either a string or by number.

DWORD NameToNumber (PCWSTR pszName) - Takes a unicode string and returns
   a list item number, starting with 1. A 0 indicates its not found.
   Case insensative.

PCWSTR NumberToName (DWORD dwElemNum) - Takes an element number (1 based),
   and returns a pointer to the string. The string is invalid if
   SetElem() is called. Returns NULL if not found.

DWORD GetNumElems () - Returns the number of elements.

BOOL AddElem (PCWSTR pszName, PVOID pData, DWORD dwSize) - Adds an element.
   If the element already exists this does a SetElem(). pData is element
   specific data. dwSize is the size of it. Returns TRUE if successful.

DWORD GetElemSize (DWORD dwElemNum) - Returns the number of bytes ot
   element data given a number (1 based).

PVOID GetElem (DWORD dwElemNum) - Given an element number (1 based),
   returns a pointer to the data. NULL if can't find.

BOOL SetElem (DWORD dwElemNum, PVOID pData, DWORD dwSize) - Sets an
   element value.

NOTES on implementation: This uses a CSWList object. The block structure
   follows:
         dwLeft - left branch. (1 based)
         dwRight - right branch (1 based)
         dwSizeName - Size of the name in bytes (rounded to nearest DWORD)
         <string> & padding - Name
         <element data>
*/
typedef struct {
   DWORD    dwLeft;
   DWORD    dwRight;
   DWORD    dwSizeName;
} TREELIST, *PTREELIST;

CTreeList::CTreeList (void)
{
   m_fCaseSens = TRUE;

   // nothing else to do
   return;
}

CTreeList::~CTreeList (void)
{
   // nothing else to do
   return;
}

DWORD CTreeList::NameToNumber (PCWSTR pszName)
{
   DWORD    dwNode;  // 1 based
   int      iRes;
   PTREELIST pTL;

   // search for a match, or for where to put it
   dwNode = 1;
   while (dwNode) {
      pTL = (PTREELIST) m_list.GetElem(dwNode - 1);
      if (!pTL)
         return 0;   // can't find
      iRes = (m_fCaseSens ? wcscmp : _wcsicmp) (pszName, (WCHAR *) (pTL + 1));
      if (!iRes) {
         // they're the same
         return dwNode;
         };
   
      dwNode = ((iRes > 0) ? pTL->dwRight : pTL->dwLeft);
      };

   // cant find
   return 0;
}

PCWSTR CTreeList::NumberToName (DWORD dwElemNum)
{
   if (!dwElemNum)
      return NULL;

   PTREELIST pTL;
   pTL = (PTREELIST) m_list.GetElem (dwElemNum-1);
   if (!pTL)
      return NULL;

   return (WCHAR*) (pTL+1);
}


DWORD CTreeList::GetNumElems (void)
{
   return m_list.GetNumElems();
}


BOOL CTreeList::AddElem (PCWSTR szName, PVOID pData, DWORD dwSize)
{
   BYTE        *abData;
   abData = new BYTE[dwSize + sizeof(TREELIST) + (wcslen(szName)+1)*2 + 8];   // 8 for round-off
   PTREELIST   pTLNew = (PTREELIST) abData;
   DWORD       dwNewSize;

   // create the structure
   pTLNew->dwLeft = pTLNew->dwRight = 0;
   pTLNew->dwSizeName = (wcslen(szName)+1) * sizeof(WCHAR);
   if (pTLNew->dwSizeName % 4)
      pTLNew->dwSizeName += 2;
   wcscpy ((WCHAR*) (pTLNew+1), szName);
   if (pData && dwSize)
      memcpy (abData + (sizeof(TREELIST) + pTLNew->dwSizeName),
         pData, dwSize);
   dwNewSize = sizeof(TREELIST) + pTLNew->dwSizeName + dwSize;

   // if this is the first element then just add it
   DWORD dwElemsInList;
   dwElemsInList = m_list.GetNumElems();
   if (!dwElemsInList) {
      BOOL fRet;
      fRet = m_list.AddElem (pTLNew, dwNewSize);
      delete abData;
      return fRet;
   }


   DWORD          dwN;
   DWORD          *pdwChange;
   int            iRes;
   PTREELIST      pTLN;

   // search for a match, or for where to put it
   dwN = 1;
   pdwChange = NULL;
   while (dwN) {
      pTLN = (PTREELIST) m_list.GetElem (dwN-1);
      if (!pTLN) {
         delete abData;
         return FALSE;  //error
      }
      iRes = (m_fCaseSens ? wcscmp : _wcsicmp) (szName, (WCHAR *) (pTLN + 1));
      if (!iRes) {
         // they're the same
         BOOL fRet;
         fRet = SetElem (dwN, pData, dwSize);
         delete abData;
         return fRet;
         };
   
      pdwChange = ( (iRes > 0) ? &(pTLN->dwRight) : &(pTLN->dwLeft) );
      dwN = *pdwChange;
      };

   // we got to the end so add it.
   if (!m_list.AddElem (pTLNew, dwNewSize)) {
      delete abData;
      return FALSE;
   }
   *pdwChange = dwElemsInList + 1;

   delete abData;
   return TRUE;
}

DWORD CTreeList::GetElemSize (DWORD dwElemNum)
{
   PTREELIST   pTL;

   if (!dwElemNum)
      return 0;
   DWORD dwSize;
   dwSize = m_list.GetElemSize (dwElemNum-1);
   if (!dwSize)
      return 0;
   pTL = (PTREELIST) m_list.GetElem (dwElemNum-1);
   if (!pTL)
      return 0;

   return dwSize - sizeof(TREELIST) - pTL->dwSizeName;
}

PVOID CTreeList::GetElem (DWORD dwElemNum)
{
   PTREELIST   pTL;

   if (!dwElemNum)
      return 0;
   pTL = (PTREELIST) m_list.GetElem (dwElemNum-1);
   if (!pTL)
      return 0;

   return (PVOID) ((PBYTE) pTL + (pTL->dwSizeName + sizeof(TREELIST)));
}

BOOL CTreeList::SetElem (DWORD dwElemNum, PVOID pData, DWORD dwSize)
{
   BYTE        *abData;
   DWORD       dwNewSize;
   PTREELIST   pTLOld;

   // get the old one
   if (!dwElemNum) {
      return FALSE;
   }
   pTLOld = (PTREELIST) m_list.GetElem (dwElemNum-1);
   if (!pTLOld) {
      return FALSE;
   }

   abData = new BYTE[dwSize + sizeof(TREELIST) + pTLOld->dwSizeName + 8];   // 8 for round-off
   PTREELIST   pTLNew = (PTREELIST) abData;

   // create the structure
   memcpy (pTLNew, pTLOld, sizeof(TREELIST) + pTLOld->dwSizeName);
   memcpy (abData + (sizeof(TREELIST) + pTLNew->dwSizeName),
      pData, dwSize);
   dwNewSize = sizeof(TREELIST) + pTLNew->dwSizeName + dwSize;

   // set it
   BOOL  fRet;
   fRet = m_list.SetElem (dwElemNum-1, pTLNew, dwNewSize);
   delete abData;
   return fRet;
}



