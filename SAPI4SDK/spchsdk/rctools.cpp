/********************************************************************
CTools.Cpp - Class tools


Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>
#include "ctools.h"

/********************************************************************
CMem - This is a memory-allocation calls. The main thing that it
   does is supply a pointer to memory and an easy re-alloc.

CMem->dwCurPosn - Current CMem::malloc() position to allocate from
   and return a value. This can be read/written. It's only as an aid.
CMem::Pointer() - This returns a PVOID pointer to the current memory
   location. This value may change when Required is called.
CMem::Required(dwSize) - This makes sure that the allocated memory
   is at least this large. It will do nothing if the allocated memory
   is already that large, and may make it larger. If it returns
   FALSE then an error has occured
CMem::Malloc(dwSize) - This makes sure that the memory has
   dwCurPosn + dwSize memory, and then returns a pointer to
   (PBYTE) CMem->Pointer() + dwCurPosn. dwCurPosn is increased by dwSize.
   Note that this pointer can change as soon as CMem->Required() is
   called again. It keeps things DWORD aligned.
*/


CMem::CMem (void)
{
m_pMem = NULL;
m_dwAllocated = NULL;
dwCurPosn = 0;
}

CMem::~CMem (void)
{
if (m_pMem)
   free (m_pMem);
}

BOOL CMem::Required (DWORD dwSize)
{
PVOID pTemp;

if (dwSize <= m_dwAllocated)
   return TRUE;

// make sure to allocate more than they ask for
dwSize += (dwSize / 2) + 256;

if (m_pMem) {
   pTemp = realloc (m_pMem, dwSize);
   if (!pTemp)
      return FALSE;  // cant allocate
   m_pMem = pTemp;
   }
else {
   m_pMem = malloc(dwSize);
   };

m_dwAllocated = dwSize;
return TRUE;
}

PVOID CMem::Malloc (DWORD dwSize)
{
PVOID p;

if (dwCurPosn & 0x03)
   dwCurPosn += (4 - (dwCurPosn & 0x03));

if (!this->Required(dwSize + dwCurPosn))
   return NULL;
p = (PVOID) ((PBYTE) m_pMem + dwCurPosn);
dwCurPosn += dwSize;

if (dwCurPosn & 0x03)
   dwCurPosn += (4 - (dwCurPosn & 0x03));

return p;
}



/********************************************************************
CSmallMem - This memory is good for small mallocs whicha re all to
   be freed up at once (when the object is destroyed).It's good for
   this because it allocates a 1K buffer and takes out of it until
   the 1 K buffer is full. Once that is done it remembeers the buffer
   and allocates another 1K buffer, using it until it is full. This
   way, it effectively does a realloc() when the buffer is too large
   but pointers dont change.

CSmallMem::CSmallMem - No inpits
PVOID CSmallMem::Malloc (dwSize) - Allocate that many bytes. This
   kepps it DWORD aligned..
*/
CSmallMem::CSmallMem (void)
{
m_pMemList = NULL;
m_dwMemAlloc = 0;
m_pCurMem = NULL;
m_dwCurMemUsed = 0;
m_dwCurMemSize = 0;
}

CSmallMem::~CSmallMem (void)
{
PVOID * ppv;
DWORD i;

if (m_pMemList) {
   ppv = (PVOID *) m_pMemList->Pointer();

   for (i = 0; i < m_dwMemAlloc; i++)
      if (ppv[i])
         free (ppv[i]);

   delete m_pMemList;
   };

}

PVOID CSmallMem::Malloc (DWORD dwSize)
{
PVOID pTemp;

if (!dwSize)
   return NULL;

// Create memory
if (!m_pMemList)
   m_pMemList = new CMem();
if (!m_pMemList)
   return NULL;   // error

// DWORD alignt
if (dwSize & 0x03)
   dwSize += (4 - (dwSize & 0x03));

if ((dwSize + m_dwCurMemUsed) > m_dwCurMemSize) {
   // allocate more memory
   PVOID * ppv;

   ppv = (PVOID *) m_pMemList->Malloc (sizeof(PVOID));
   if (!ppv)
      return NULL;
   m_dwMemAlloc++;
   m_dwCurMemSize = max (1024, dwSize);
   m_dwCurMemUsed = 0;
   m_pCurMem = *ppv = malloc (m_dwCurMemSize);
   if (!ppv) {
      m_dwCurMemSize = 0;
      return NULL; 
      };
   };

pTemp = (PVOID) ((PBYTE) m_pCurMem + m_dwCurMemUsed);
m_dwCurMemUsed += dwSize;

return pTemp;
}



/********************************************************************
CAddBTree - An add-only binary tree.

CAddBTree::Add (char *pszKey, DWORD dwValue) - This adds pszKey
   to the binary tree and stored dwValue along with it. if pszKey
   (case insensative) already exists in the tree then its old value
   is replaced. Returns TRUE if successful.

CAddBTree::Find (char *pszKey) - This finds pszKey and returns the
   value stored with it, or 0 if not found.
*/
CAddBTree::CAddBTree (void)
{
m_pMem = NULL;
m_pFirst = NULL;
}

CAddBTree::~CAddBTree (void)
{
if (m_pMem)
   delete m_pMem;
}

BOOL CAddBTree::Add (char *pszKey, DWORD dwValue)
{
PADDBTREENODE pN;
DWORD          dwLen;
PADDBTREENODE * pChange;
int            iRes;

if (!m_pMem)
   m_pMem = new CSmallMem();
if (!m_pMem)
   return FALSE;   // error

dwLen = strlen (pszKey) + 1;

// search for a match, or for where to put it
pN = m_pFirst;
pChange = &m_pFirst;
while (pN) {
   iRes = strcmp (pszKey, (char *) (pN + 1));
   if (!iRes) {
      // they're the same
      pN->dwValue = dwValue;
      return TRUE;
      };
   
   pChange = (PADDBTREENODE*)
      ( (iRes > 0) ? &(pN->pRight) : &(pN->pLeft) );
   pN = *pChange;
   };

// we got to the end so add it.
*pChange = pN = (PADDBTREENODE) m_pMem->Malloc (sizeof(*pN) + dwLen);
if (!pN)
   return FALSE;  //error
pN->pLeft = pN->pRight = NULL;
pN->dwValue = dwValue;
strcpy ((char *) (pN + 1), pszKey);

return TRUE;
}


DWORD CAddBTree::Find (char *pszKey)
{
PADDBTREENODE pN;
int            iRes;

// search for a match, or for where to put it
pN = m_pFirst;
while (pN) {
   iRes = strcmp (pszKey, (char *) (pN + 1));
   if (!iRes) {
      // they're the same
      return pN->dwValue;
      };
   
   pN = (PADDBTREENODE) ((iRes > 0) ? pN->pRight : pN->pLeft);
   };

// cant find
return 0;
}

/********************************************************************
CAddBNumTree - An add-only binary tree.  The binary tree uses numbers
   for its keys.

CAddBNumTree::Add (DWORD dwKey, DWORD dwValue) - This adds dwKey
   to the binary tree and stored dwValue along with it. if dwKey
   (case insensative) already exists in the tree then its old value
   is replaced. Returns TRUE if successful.

CAddBNumTree::Find (DWORD dwKey) - This finds dwKey and returns the
   value stored with it, or 0 if not found.
*/
CAddBNumTree::CAddBNumTree (void)
{
m_pMem = NULL;
m_pFirst = NULL;
}

CAddBNumTree::~CAddBNumTree (void)
{
if (m_pMem)
   delete m_pMem;
}

BOOL CAddBNumTree::Add (DWORD dwKey, DWORD dwValue)
{
PADDBNUMTREENODE pN;
PADDBNUMTREENODE * pChange;

if (!m_pMem)
   m_pMem = new CSmallMem();
if (!m_pMem)
   return FALSE;   // error

// search for a match, or for where to put it
pN = m_pFirst;
pChange = &m_pFirst;
while (pN) {
   if (dwKey == pN->dwKey) {
      // they're the same
      pN->dwValue = dwValue;
      return TRUE;
      };
   
   pChange = (PADDBNUMTREENODE*)
      ( (dwKey > pN->dwKey) ? &(pN->pRight) : &(pN->pLeft) );
   pN = *pChange;
   };

// we got to the end so add it.
*pChange = pN = (PADDBNUMTREENODE) m_pMem->Malloc (sizeof(*pN));
if (!pN)
   return FALSE;  //error
pN->pLeft = pN->pRight = NULL;
pN->dwValue = dwValue;
pN->dwKey = dwKey;

return TRUE;
}


DWORD CAddBNumTree::Find (DWORD dwKey)
{
PADDBNUMTREENODE pN;

// search for a match, or for where to put it
pN = m_pFirst;
while (pN) {
   if (dwKey == pN->dwKey) {
      // they're the same
      return pN->dwValue;
      };
   
   pN = (PADDBNUMTREENODE) ((dwKey > pN->dwKey) ? pN->pRight : pN->pLeft);
   };

// cant find
return 0;
}



/********************************************************************
CNumList - This is a list which contains numeric values. At the moment
   it can only be added to.

GetNumElems - Return the number of elements
AddElem - Add a new element to the end of this list.
    dwValue is the value to add.
GetElem - Gets the value of an element, dwElemNum. pdwValue is
   filled in with the value of the element. Returns TRUE if OK,
   FALSE if error.
SetElem - Sets the value of an element, dwElemNum. dwValue is
   the value to re-write. Returns TRUE if OK,
   FALSE if error.
*/
CNumList::CNumList (void)
{
m_dwNumElems = 0;
m_pMem = NULL;
}

CNumList::~CNumList (void)
{
if (m_pMem)
   delete m_pMem;
}

DWORD CNumList::GetNumElems (void)
{
return m_dwNumElems;
}

BOOL CNumList::AddElem (DWORD dwValue)
{
DWORD *pdw;

if (!m_pMem) {
   m_pMem = new CMem();
   if (!m_pMem)
      return FALSE;
   };
if (!m_pMem->Required((m_dwNumElems+1) * sizeof(DWORD)) )
   return FALSE;

pdw = (DWORD*) m_pMem->Pointer();
if (!pdw)
   return FALSE;
pdw[m_dwNumElems] = dwValue;
m_dwNumElems++;

return TRUE;
}


BOOL CNumList::GetElem (DWORD dwElemNum, DWORD *pdwValue)
{
DWORD *pdw;

if (dwElemNum >= m_dwNumElems)
   return FALSE;

pdw = (DWORD*) m_pMem->Pointer();
if (!pdw)
   return FALSE;
*pdwValue = pdw[dwElemNum];

return TRUE;
}


BOOL CNumList::SetElem (DWORD dwElemNum, DWORD dwValue)
{
DWORD *pdw;

if (dwElemNum >= m_dwNumElems)
   return FALSE;

pdw = (DWORD*) m_pMem->Pointer();
if (!pdw)
   return FALSE;
pdw[dwElemNum] = dwValue;

return TRUE;
}


/************************************************************************
link.c - Linked list code.

begun 4/19/94 by Mike Rozak
*/


/************************************************************************
CList constructur
*/
CList::CList (void)
{
dwNumElems = 0;
paElems = 0;
dwBufSize = 0;
}

/************************************************************************
CList destructor - This frees up all of the memory.
*/
CList::~CList (void)
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
BOOL CList::MakeListMemoryThisBig (DWORD dwNumElems)
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
DWORD CList::GetNumElems (void)
{
return dwNumElems;
}


/************************************************************************
AddElem - Add an element onto the end of the list. pData is a pointer to
   memory containing the elemnt information (which is copied into internal memory)
   and dwSize is the size of the element information. This returns TRUE if
   it was successful.
*/
BOOL CList::AddElem (PVOID pData, DWORD dwSize)
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
BOOL CList::InsertElem (DWORD dwElemNum, PVOID pData, DWORD dwSize)
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
BOOL CList::RemoveElem (DWORD dwElemNum)
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
DWORD CList::GetElemSize (DWORD dwElemNum)
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
PVOID CList::GetElem (DWORD dwElemNum)
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
BOOL CList::SetElem (DWORD dwElemNum, PVOID pData, DWORD dwSize)
{
if (dwElemNum >= dwNumElems)
   return FALSE;

if (!this->RemoveElem(dwElemNum))
   return FALSE;
return this->InsertElem(dwElemNum, pData, dwSize);
}

/****************************************************************8
CList::Clone - Clone the current list into a new list. Returns a new list
*/
CList * CList::Clone (void)
{
CList * pNew;
DWORD	dwNum, i;
DWORD	dwSize;
PVOID	pMem;

pNew = new CList;
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

NOTES on implementation: This uses a CList object. The block structure
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
   // HACKHACK - I'm just having a 1K buffer on the stack
   // so I don't have to continually realloc.
   BYTE        abData[1024];
   PTREELIST   pTLNew = (PTREELIST) abData;
   DWORD       dwNewSize;

   // create the structure
   pTLNew->dwLeft = pTLNew->dwRight = 0;
   pTLNew->dwSizeName = (wcslen(szName)+1) * sizeof(WCHAR);
   if (pTLNew->dwSizeName % 4)
      pTLNew->dwSizeName += 2;
   wcscpy ((WCHAR*) (pTLNew+1), szName);
   memcpy (abData + (sizeof(TREELIST) + pTLNew->dwSizeName),
      pData, dwSize);
   dwNewSize = sizeof(TREELIST) + pTLNew->dwSizeName + dwSize;

   // if this is the first element then just add it
   DWORD dwElemsInList;
   dwElemsInList = m_list.GetNumElems();
   if (!dwElemsInList)
      return m_list.AddElem (pTLNew, dwNewSize);


   DWORD          dwN;
   DWORD          *pdwChange;
   int            iRes;
   PTREELIST      pTLN;

   // search for a match, or for where to put it
   dwN = 1;
   pdwChange = NULL;
   while (dwN) {
      pTLN = (PTREELIST) m_list.GetElem (dwN-1);
      if (!pTLN)
         return FALSE;  //error
      iRes = (m_fCaseSens ? wcscmp : _wcsicmp) (szName, (WCHAR *) (pTLN + 1));
      if (!iRes) {
         // they're the same
         return SetElem (dwN, pData, dwSize);
         };
   
      pdwChange = ( (iRes > 0) ? &(pTLN->dwRight) : &(pTLN->dwLeft) );
      dwN = *pdwChange;
      };

   // we got to the end so add it.
   if (!m_list.AddElem (pTLNew, dwNewSize))
      return FALSE;
   *pdwChange = dwElemsInList + 1;

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
   // HACKHACK - I'm just having a 1K buffer on the stack
   // so I don't have to continually realloc.
   BYTE        abData[1024];
   PTREELIST   pTLNew = (PTREELIST) abData;
   DWORD       dwNewSize;
   PTREELIST   pTLOld;

   // get the old one
   if (!dwElemNum)
      return FALSE;
   pTLOld = (PTREELIST) m_list.GetElem (dwElemNum-1);
   if (!pTLOld)
      return FALSE;

   // create the structure
   memcpy (pTLNew, pTLOld, sizeof(TREELIST) + pTLOld->dwSizeName);
   memcpy (abData + (sizeof(TREELIST) + pTLNew->dwSizeName),
      pData, dwSize);
   dwNewSize = sizeof(TREELIST) + pTLNew->dwSizeName + dwSize;

   // set it
   return m_list.SetElem (dwElemNum-1, pTLNew, dwNewSize);
}



