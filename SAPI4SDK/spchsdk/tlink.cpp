/************************************************************************
link.c - Linked list code.

begun 6/1/94 by Larry Israel
*/

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "link.h"

/* 
 *  Helper function prototypes
 */
PLINKELEM FindElem(PVOID, DWORD, DWORD, PLINKELEM);


typedef unsigned _int64 QWORD, * PQWORD;



/*
 *
 *   Wide character functions to run on Chicago
 *
 */

/************************************************************************
MyWStrCmp - Wide string compare.

inputs
   WCHAR *    pA - A string
   WCHAR *    pB - B string
returns
   int - Positive is A > B, negative if A < B, 0 if A == B
*/

int MyWStrCmp (WCHAR * pA, WCHAR * pB)
{
    while ((*pA && *pB) && (*pA == *pB)) {
        pA++;
        pB++;
    }

    return (int) *pA - (int) *pB;
}


/************************************************************************
MyWStrLen - Wide string length.

inputs
   WCHAR *    pA - A string
returns
   int - length of string (excluding null terminator)
*/

int MyWStrLen (WCHAR * pA)
{
    int cnt=0;

    while (*pA) {
        pA++;
        cnt++;
    }

    return cnt;
}


/************************************************************************
MyWStrCpy - Wide string copy.

inputs
   WCHAR *    pA - A string
   WCHAR *    pB - B string
returns
   int - number of characters copied
*/

int MyWStrCpy (WCHAR * pA, WCHAR * pB)
{
    int cnt = 0;

    while (*pB) {
        *pA = *pB;
        pA++;
        pB++;
        cnt++;
    }
    *pA = L'\0';

    return cnt;
}


/************************************************************************
MyWStrCat - Wide string concatenate.

inputs
   WCHAR *    pA - A string
   WCHAR *    pB - B string
returns
   number of characters concatenated
*/

int MyWStrCat (WCHAR * pA, WCHAR * pB)
{
    int cnt = 0;

    while (*pA)
        pA++;

    cnt = MyWStrCpy(pA, pB);

    return cnt;
}



/*
 *
 *   ANSI functions to run on UNICODE version
 *
 */

/************************************************************************
MyAStrCmp - ANSI string compare.

inputs
   PSTR    pA - A string
   PSTR    pB - B string
returns
   int - Positive is A > B, negative if A < B, 0 if A == B
*/

int MyAStrCmp (PSTR pA, PSTR pB)
{
    while ((*pA && *pB) && (*pA == *pB)) {
        pA++;
        pB++;
    }

    return (int) *pA - (int) *pB;
}


/************************************************************************
MyAStrLen - ANSI string length.

inputs
   PSTR    pA - A string
returns
   int - length of string (excluding null terminator)
*/

int MyAStrLen (PSTR pA)
{
    int cnt=0;

    while (*pA) {
        pA++;
        cnt++;
    }

    return cnt;
}


/************************************************************************
MyAStrCpy - ANSI string copy.

inputs
   PSTR    pA - A string
   PSTR    pB - B string
returns
   int - number of characters copied
*/

int MyAStrCpy (PSTR pA, PSTR pB)
{
    int cnt = 0;

    while (*pB) {
        *pA = *pB;
        pA++;
        pB++;
        cnt++;
    }
    *pA = '\0';

    return cnt;
}


/************************************************************************
MyAStrCat - ANSI string concatenate.

inputs
   PSTR    pA - A string
   PSTR    pB - B string
returns
   number of characters concatenated
*/

int MyAStrCat (PSTR pA, PSTR pB)
{
    int cnt = 0;

    while (*pA)
        pA++;

    cnt = MyAStrCpy(pA, pB);

    return cnt;
}



/*
 *  Link List
 */

/************************************************************************
CLink constructur
*/
CLink::CLink (void)
{
	dwNumElems = 0;
	m_pFirst = 0;
	m_pLast = 0;
}

/************************************************************************
CLink destructor - This frees up all of the memory.
*/
CLink::~CLink (void)
{
    FreeAllElems();
}



/************************************************************************
CLink  - This frees up all of the elements in the list.
*/
void CLink::FreeAllElems (void)
{
	PLINKELEM pTemp, pTemp2;

	if (m_pFirst) {
		pTemp = m_pFirst;
		do {
			pTemp2 = (PLINKELEM)(pTemp->pNext);
			FREE (pTemp->pElem);
			FREE (pTemp);
			pTemp = pTemp2;
		}
		while (pTemp);
	}

    m_pFirst   = NULL;
    m_pLast    = NULL;
    dwNumElems = 0;
}



/************************************************************************
GetNumElems - Returns the number of elements in the list.
*/
DWORD CLink::GetNumElems (void)
{
	return dwNumElems;
}


/************************************************************************
AddElem - Add an element onto the end of the list. pData is a pointer to
   memory containing the elemnt information (which is copied into internal memory)
   and dwSize is the size of the element information. This returns TRUE if
   it was successful.
*/
BOOL CLink::AddElem (PVOID pData, DWORD dwSize)
{
	PVOID pTemp;
	PLINKELEM pTempElem;

	if (!pData || !dwSize)
   		return FALSE;

    // allocate a list element
    pTempElem = (PLINKELEM) ALLOC(sizeof(LINKELEM));
	if (!pTempElem) 
		return FALSE;

    // get memory for the list entry and copy it
	pTemp = ALLOC(dwSize);
	if (!pTemp) 
		return FALSE;
	memcpy (pTemp, pData, dwSize);

    // fill in the list element
	pTempElem->pElem = pTemp;
	pTempElem->dwElemSize = dwSize;
	pTempElem->pNext = 0;
	pTempElem->pPrev = 0;

    // if this is the first entry, indicate so
    // (list entries are added on to the end)
	if (!m_pFirst)
		m_pFirst = pTempElem;

    // update the links
	if (m_pLast) {
		m_pLast->pNext = pTempElem;
		pTempElem->pPrev = m_pLast;
	}
	m_pLast = pTempElem;

	dwNumElems++;
	return TRUE;
}


/************************************************************************
RemoveElem - This removes a specific element from the list.
*/
BOOL CLink::RemoveElem (PVOID pKey, DWORD dwType, DWORD dwOffset)
{

	PLINKELEM pTemp, pTemp2;

	if ((pTemp = FindElem(pKey, dwType, dwOffset, m_pFirst)) == 0)
   		return FALSE;

	if (pTemp->pNext) {
		pTemp2 = (PLINKELEM)(pTemp->pNext);
		pTemp2->pPrev = pTemp->pPrev;
	}
	else if (pTemp == m_pLast)
		m_pLast = (PLINKELEM)(pTemp->pPrev);

	if (pTemp->pPrev) {
		pTemp2 = (PLINKELEM)(pTemp->pPrev);
		pTemp2->pNext = pTemp->pNext;
	}
	else if (pTemp == m_pFirst)
		m_pFirst = (PLINKELEM)(pTemp->pNext);

	FREE(pTemp->pElem);
	FREE(pTemp);

	dwNumElems--;
	return TRUE;
}


/************************************************************************
GetElemSize - This gets the size of a specific element number, defined
   by pKey
*/
DWORD CLink::GetElemSize (PVOID pKey, DWORD dwType, DWORD dwOffset)
{
	PLINKELEM pTemp;

	if ((pTemp = FindElem(pKey, dwType, dwOffset, m_pFirst)) == 0)
   		return FALSE;

	return pTemp->dwElemSize;
}


/************************************************************************
GetElem - This returns a pointer to memory containing the element's data.
*/
PVOID CLink::GetElem (PVOID pKey, DWORD dwType, DWORD  dwOffset)
{
	PLINKELEM pTemp;

	if ((pTemp = FindElem(pKey, dwType, dwOffset, m_pFirst)) == 0)
   		return FALSE;

	return pTemp->pElem;
}


/************************************************************************
SetElem - This sets the element to the new data.
*/
BOOL CLink::SetElem (PVOID pKey, DWORD dwType, DWORD  dwOffset, PVOID pData,
                     DWORD dwSize)
{
	PLINKELEM pTemp;
    PVOID     pTempData;

	if ((pTemp = FindElem(pKey, dwType, dwOffset, m_pFirst)) == 0)
   		return FALSE;

    // free the old data
    FREE(pTemp->pElem);

    // get memory for the list entry and copy it
	pTempData = ALLOC(dwSize);
	if (!pTempData) 
		return FALSE;
	memcpy (pTempData, pData, dwSize);

    // point to new data
    pTemp->pElem = pTempData;

	return TRUE;
}


/************************************************************************
GetElem - This returns a pointer to memory containing the element's data.
*/
PVOID CLink::GetEntryElem (DWORD dwEntry)
{
	PLINKELEM pTemp;
	int i;

	if (dwEntry > dwNumElems)
		return 0;

    // if the entry requested is in the second half of the list,
    // search backwards from the end
	if (dwEntry > (dwNumElems/2)) {
		pTemp = m_pLast;
		i = dwNumElems - dwEntry;
		while (i--)
			pTemp = (PLINKELEM)pTemp->pPrev;
	}
    // if the entry is in the first half, search forwards from beginning
	else {
		pTemp = m_pFirst;
		i = dwEntry;
		while (--i)
			pTemp = (PLINKELEM)pTemp->pNext;
	}

	return pTemp->pElem;
}


/************************************************************************
UpdateElem - This updates the entry specified by dwEntry
*/
BOOL CLink::UpdateElem (PVOID pData, DWORD dwEntry)
{
	PLINKELEM pTemp;
    PVOID pTempData;
	int i;

	if (dwEntry > dwNumElems)
		return FALSE;

    // if the entry requested is in the second half of the list,
    // search backwards from the end
	if (dwEntry > (dwNumElems/2)) {
		pTemp = m_pLast;
		i = dwNumElems - dwEntry;
		while (i--)
			pTemp = (PLINKELEM)pTemp->pPrev;
	}
    // if the entry is in the first half, search forwards from beginning
	else {
		pTemp = m_pFirst;
		i = dwEntry;
		while (--i)
			pTemp = (PLINKELEM)pTemp->pNext;
	}

    pTempData = (PVOID)pTemp->pElem;
    // copy the new data to the entry
    memcpy(pTempData, pData, pTemp->dwElemSize);
	return TRUE;
}


/************************************************************************
GetNextElem - This will get the next element in the linked list based
              on pLink
*/
PVOID CLink::GetNextElem(PVOID pLink, PVOID * ppData)
{
    PLINKELEM pLinkElem;

    if (pLink == NULL)
        pLinkElem = m_pFirst;
    else
        pLinkElem = (PLINKELEM)((PLINKELEM)pLink)->pNext;

    if (!pLinkElem)
        return NULL;

    *ppData = pLinkElem->pElem;
    return (PVOID)pLinkElem;
}



/************************************************************************
GetPrevElem - This will get the previous element in the linked list based
              on pLink
*/
PVOID CLink::GetPrevElem(PVOID pLink, PVOID * ppData)
{
    PLINKELEM pLinkElem;

    if (pLink == NULL)
        pLinkElem = m_pLast;
    else
        pLinkElem = (PLINKELEM)((PLINKELEM)pLink)->pPrev;

    if (!pLinkElem)
        return NULL;

    *ppData = pLinkElem->pElem;
    return (PVOID)pLinkElem;
}



/************************************************************************
FindElem - This will find an element in the linked list based on the key
	information passed to the function.
*/
PLINKELEM FindElem(PVOID pKey, DWORD  dwType, DWORD dwOffset, PLINKELEM pFirst)
{
	PLINKELEM pTemp;
	PCHAR pCompare;

	pTemp = pFirst;
	while (pTemp) {
		pCompare = (PCHAR)pTemp->pElem;
		pCompare += dwOffset;
		switch (dwType) {
			case TYPE_QWORD:
				if (*(QWORD *)pCompare == *(QWORD *)pKey)
					return pTemp;
                break;
			case TYPE_DWORD:
				if (*(DWORD *)pCompare == *(DWORD *)pKey)
					return pTemp;
                break;
			case TYPE_WORD:
				if (*(WORD *)pCompare == *(WORD *)pKey)
					return pTemp;
                break;
			case TYPE_CHAR:
				if (MyAStrCmp((PSTR)pCompare, (PSTR)pKey) == 0)
					return pTemp;
                break;
			case TYPE_WCHAR:
				if (MyWStrCmp((PWCHAR)pCompare, (PWCHAR)pKey) == 0)
					return pTemp;
                break;
		}
		pTemp = (PLINKELEM)(pTemp->pNext);
	}

return 0;
}



/*
 *  Queue
 */

/************************************************************************
CQueue constructur
*/
CQueue::CQueue (void)
{
	m_dwNumElems    = 0;
    m_pHeap         = NULL;
    m_pQueueTail    = NULL;
    m_pQueueHead    = NULL;
    m_pQueueStart   = NULL;
    m_pQueueEnd     = NULL;
}

/************************************************************************
CQueue destructor - This frees up all of the memory.
*/
CQueue::~CQueue (void)
{
    if (m_pHeap)
        HeapDestroy(m_pHeap);
}


/************************************************************************
FInit - Initialize the tree object. This will allocate a heap from the
    Windows memory manager from which all nodes of the tree will be 
    allocated.
*/
BOOL CQueue::FInit(void)
{
    m_pHeap = HeapCreate(0, 16*K, 0);
    if (!m_pHeap)
        return FALSE;

    m_pQueueStart = HeapAlloc(m_pHeap, HEAP_ZERO_MEMORY, sizeof(QUEUEELEM)*K);
    if (!m_pQueueStart) {
        HeapDestroy(m_pHeap);
        return FALSE;
    }

    m_pQueueEnd = (PBYTE)m_pQueueStart + sizeof(QUEUEELEM)*K;
    m_pQueueHead = (PQUEUEELEM)m_pQueueStart;
    m_pQueueTail = (PQUEUEELEM)m_pQueueStart;
    m_dwMaxElems = K;

    return TRUE;
}


/************************************************************************
GetNumElems - Returns the number of elements in the list.
*/
DWORD CQueue::GetNumElems (void)
{
	return m_dwNumElems;
}


/************************************************************************
FreeAllElems - Free everything in the queue.
*/
void CQueue::FreeAllElems (void)
{
    while (m_pQueueHead != m_pQueueTail) {
        HeapFree(m_pHeap, NULL, m_pQueueHead->pElem);
        if (++m_pQueueHead >= (PQUEUEELEM)m_pQueueEnd)
            m_pQueueHead = (PQUEUEELEM)m_pQueueStart;
    }
    m_dwNumElems = 0;
}


/************************************************************************
QueueElem - Add an element onto the end of the queue. pData is a pointer to
   memory containing the elemnt information (which is copied into internal memory)
   and dwSize is the size of the element information. This returns TRUE if
   it was successful.
*/
BOOL CQueue::QueueElem (PVOID pData, DWORD dwSize)
{
    PVOID pTemp;
    PQUEUEELEM pTempElem;

    if (!pData || !dwSize)
        return FALSE;

    if (m_dwNumElems >= m_dwMaxElems)
        return FALSE;

    // get memory for the list entry and copy it
	pTemp = HeapAlloc (m_pHeap, HEAP_ZERO_MEMORY, dwSize);
	if (!pTemp) 
		return FALSE;
	memcpy (pTemp, pData, dwSize);

    // point to the correct element and update the queue tail pointer
    pTempElem = m_pQueueTail;
    if (++m_pQueueTail >= (PQUEUEELEM)m_pQueueEnd)
        m_pQueueTail = (PQUEUEELEM)m_pQueueStart;

    // fill in the list element
    pTempElem->pElem = pTemp;
//    pTempElem->pElem = pData;
    pTempElem->dwElemSize = dwSize;
    
    m_dwNumElems++;
    return TRUE;
}


/************************************************************************
QueueElemToHead - Add an element onto the head of the queue. pData is a pointer to
   memory containing the elemnt information (which is copied into internal memory)
   and dwSize is the size of the element information. This returns TRUE if
   it was successful.
*/
BOOL CQueue::QueueElemToHead (PVOID pData, DWORD dwSize)
{
    PVOID pTemp;
    PQUEUEELEM pTempElem;

    if (!pData || !dwSize)
        return FALSE;

    if (m_dwNumElems >= m_dwMaxElems)
        return FALSE;

    // get memory for the list entry and copy it
    pTemp = HeapAlloc (m_pHeap, HEAP_ZERO_MEMORY, dwSize);
    if (!pTemp) 
        return FALSE;
    memcpy (pTemp, pData, dwSize);

    if (m_dwNumElems == 0) {
        // point to the correct element
        pTempElem = m_pQueueTail;
        if (++m_pQueueTail >= (PQUEUEELEM)m_pQueueEnd)
            m_pQueueTail = (PQUEUEELEM)m_pQueueStart;
    }
    else {
        if (--m_pQueueHead <= (PQUEUEELEM)m_pQueueStart)
            m_pQueueHead = (PQUEUEELEM)((PBYTE)m_pQueueEnd-sizeof(QUEUEELEM));
        pTempElem = m_pQueueHead;
    }

    // fill in the list element
    pTempElem->pElem = pTemp;
//    pTempElem->pElem = pData;
    pTempElem->dwElemSize = dwSize;
    
    m_dwNumElems++;
    return TRUE;
}


/************************************************************************
DequeueElem - This removes a specific element from the list.
*/
BOOL CQueue::DequeueElem ()
{

    PQUEUEELEM pTemp;

    // anything in queue?
    if (m_pQueueHead == m_pQueueTail)
        return FALSE;
    if (m_dwNumElems == 0)
        return FALSE;

    pTemp = m_pQueueHead;
    HeapFree(m_pHeap, NULL, pTemp->pElem);

    if (++m_pQueueHead >= (PQUEUEELEM)m_pQueueEnd)
        m_pQueueHead = (PQUEUEELEM)m_pQueueStart;

    m_dwNumElems--;
    return TRUE;
}


/************************************************************************
GetQueueHead - This returns a pointer to memory containing the queue heads data.
*/
PVOID CQueue::GetQueueHead (void)
{
    // anything in queue?
    if (m_pQueueHead == m_pQueueTail)
        return NULL;
    if (m_dwNumElems == 0)
        return NULL;

    return m_pQueueHead->pElem;
}





/************************************************************************
  Binary tree objects
 ************************************************************************/

/************************************************************************
GUID BTree
*************************************************************************/
/************************************************************************
BTree constructor
*/
BTree::BTree (void)
{
	m_dwNumElems = 0;
	m_pRoot = NULL;
    m_pHeap = NULL;
}

/************************************************************************
BTree destructor - This frees up all of the memory.
*/
BTree::~BTree (void)
{
    if (m_pHeap)
        HeapDestroy(m_pHeap);
}


/************************************************************************
FInit - Initialize the tree object. This will allocate a heap from the
    Windows memory manager from which all nodes of the tree will be 
    allocated.
*/
BOOL BTree::FInit(void)
{
    m_pHeap = HeapCreate(0, 8*K, 0);
    if (!m_pHeap)
        return FALSE;

    return TRUE;
}


/************************************************************************
GetNumElems - Returns the number of elements in the list.
*/
DWORD BTree::GetNumElem (void)
{
    return m_dwNumElems;
}


/************************************************************************
AddElem - Add an element onto the tree. pData is a pointer to
   memory containing the element information (which is copied into internal
   memory). dwSize is the size in bytes of the element information.
   This returns the element number of either the newly added element, or in
   the case of a duplicate entry, it will return the element number of the
   existing element. It will also set pdwFlag to indicate it was a duplicate
   or not.
*/
DWORD BTree::AddElem (PVOID pData, DWORD dwSize, DWORD *pdwFlag)
{
    PNODE pNode, pNew, *ppChild;
    GUID  *pgID;
    int   iRet;

    *pdwFlag = TREE_OUTOFMEM;  //default to error
    pNode = m_pRoot;

    // if there is no root yet, we will fall out of the while, otherwise
    // every path leads to a return from the while loop
    while (pNode) {
        pgID = (GUID *)pNode->Data.pElem;
        iRet = memcmp(pData, pgID, sizeof(GUID));

        if (iRet  == 0) {
            *pdwFlag = TREE_DUPLICATE;
            return pNode->dwElemNum;
        }
        if (iRet > 0)
            ppChild = (PNODE *)&pNode->pRight;
        else
            ppChild = (PNODE *)&pNode->pLeft;

        if ((pNode = *ppChild) == NULL) {
            pNew = MakeNode(pData, dwSize);
            if (!pNew)
                return 0;

            *ppChild = pNew;
            *pdwFlag = TREE_NEW;
            return pNew->dwElemNum;
        }
	}

    // it will only get here if m_pRoot is NULL
    pNew = MakeNode(pData, dwSize);
    if (!pNew)
        return FALSE;

    m_pRoot = pNew;

    *pdwFlag = TREE_NEW;
    return pNew->dwElemNum;
}


/************************************************************************
MakeNode - This will make a new node to add onto the tree. Called by
    AddElem.
*/
PNODE BTree::MakeNode(PVOID pData, DWORD dwSize)
{
    PNODE pNew;
    PVOID pTemp;

    pNew = (PNODE)HeapAlloc(m_pHeap, HEAP_ZERO_MEMORY, sizeof(NODE));
    if (!pNew)
        return NULL;

    pTemp = HeapAlloc(m_pHeap, 0, dwSize);
    if (!pTemp) {
        HeapFree(m_pHeap, 0, pNew);
        return NULL;
    }

    memcpy(pTemp, pData, dwSize);

    pNew->Data.pElem = pTemp;
    pNew->dwElemNum = ++m_dwNumElems;

    return pNew;
}


/************************************************************************
FindElem - This will find an element in the tree based on the key
	information passed to the function. It will return the will return
    the data in the tree node.
*/
PVOID BTree::FindElem (GUID riid)
{
    PNODE pTemp;
    GUID  *pgID;
    int   iRet;

    pTemp = m_pRoot;
    while (pTemp) {
        pgID = (GUID *)pTemp->Data.pElem;
        iRet = memcmp(&riid, pgID, sizeof(GUID));

        if (iRet == 0)
            return pTemp->Data.pElem;
        if (iRet > 0)
		    pTemp = (PNODE)(pTemp->pRight);
        else
		    pTemp = (PNODE)(pTemp->pLeft);
	}

    return 0;
}






/************************************************************************
String BTree using offsets into big heap instead of pointers to small
    memory blocks.
*************************************************************************/
/************************************************************************
BTreeHeap constructor
*/
BTreeHeap::BTreeHeap (void)
{
	m_dwNumElems = 0;
	m_dwRoot = 0;
    m_pHeap = NULL;
    m_fUnicode = TRUE;

    m_pData = NULL;
    m_dwAvail = 0;
    m_dwSize = 0;
}

/************************************************************************
BTreeHeap destructor - This frees up all of the memory.
*/
BTreeHeap::~BTreeHeap (void)
{
    if (m_pHeap)
        HeapDestroy(m_pHeap);
}


/************************************************************************
FInit - Initialize the tree object. This will allocate a heap from the
    Windows memory manager from which all nodes of the tree will be 
    allocated.
*/
BOOL BTreeHeap::FInit(void)
{
    m_pHeap = HeapCreate(0, 0, 0);
    if (!m_pHeap)
        return FALSE;

    m_pData = (PBYTE) HeapAlloc(m_pHeap, HEAP_ZERO_MEMORY, 8*K);
    if (!m_pData) {
        HeapDestroy(m_pHeap);
        return FALSE;
    }

    // save first DWORD in heap
    m_dwAvail = 8*K - sizeof(DWORD);
    m_dwSize = 8*K;

    return TRUE;
}


/************************************************************************
SetCharMode - Sets the character mode to UNICODE or ANSI.
*/
void BTreeHeap::SetCharMode(BOOL fUnicode)
{
    m_fUnicode = fUnicode;
}


/************************************************************************
GetNumElems - Returns the number of elements in the list.
*/
DWORD BTreeHeap::GetNumElem (void)
{
    return m_dwNumElems;
}


/************************************************************************
GetTree - Return the address of the array memory
*/
HRESULT BTreeHeap::GetTree(PVOID *ppMem, DWORD *pdwSize)
{
    *ppMem = m_pData;
    *pdwSize = (m_dwSize - m_dwAvail);
    return NOERROR;
}


/************************************************************************
GetMemAddress - Return the address of the array memory
*/
HRESULT BTreeHeap::SetTree(PVOID pMem, DWORD dwSize)
{
    DWORD dwCnt;

    dwCnt = dwSize + 4*K;
    HeapFree(m_pHeap, NULL, m_pData);
    m_pData = (PBYTE) HeapAlloc(m_pHeap, HEAP_ZERO_MEMORY, dwCnt);
    if (!m_pData)
        return E_OUTOFMEMORY;

    // save first DWORD in heap
    m_dwAvail = 4*K;
    m_dwSize = dwCnt;
    CopyMemory(m_pData, pMem, dwSize);
    m_dwRoot = sizeof(DWORD);
    return NOERROR;
}


/************************************************************************
AddElem - Add an element onto the tree. pData is a pointer to
   memory containing the element information (which is copied into internal
   memory). dwSize is the size in bytes of the element information.
   This returns the element number of either the newly added element, or in
   the case of a duplicate entry, it will return the element number of the
   existing element. It will also set pdwFlag to indicate it was a duplicate
   or not.
*/
DWORD BTreeHeap::AddElem (PVOID pData, DWORD dwSize, DWORD *pdwFlag)
{
    PNODEHEAP   pNode, pNew;
    DWORD       dwNew, dwChild, dwOffsetChild;
    PWSTR       pWord;
    int         iRet;

    *pdwFlag = TREE_OUTOFMEM;  //default to error
    pNode = (PNODEHEAP)(m_pData + m_dwRoot);
    if (m_dwRoot == 0)
        goto NoRootYet;

    // if there is no root yet, we will fall out of the while, otherwise
    // every path leads to a return from the while loop
    while ((PBYTE)pNode != m_pData) {
        pWord = (PWSTR)(m_pData + pNode->dwData);

        if (m_fUnicode)
            iRet = MyWStrCmp((PWSTR)pData, pWord);
        else
            iRet = MyAStrCmp((PSTR)pData, (PSTR)pWord);

        if (iRet  == 0) {
            *pdwFlag = TREE_DUPLICATE;
            return pNode->dwElemNum;
        }
        if (iRet > 0) {
            dwChild = pNode->dwRight;
            dwOffsetChild = (PBYTE)(&pNode->dwRight) - m_pData;
        }
        else {
            dwChild = pNode->dwLeft;
            dwOffsetChild = (PBYTE)(&pNode->dwLeft) - m_pData;
        }

        pNode = (PNODEHEAP)(m_pData + dwChild);
        if (dwChild == 0) {
            dwNew = MakeNode(pData, dwSize);
            if (!dwNew)
                return 0;

            *(DWORD *)(m_pData + dwOffsetChild) = dwNew;
            *pdwFlag = TREE_NEW;
            pNew = (PNODEHEAP)(m_pData + dwNew);
            return pNew->dwElemNum;
        }
	}

NoRootYet:
    // it will only get here if m_pRoot is NULL
    dwNew = MakeNode(pData, dwSize);
    if (!dwNew)
        return FALSE;

    m_dwRoot = dwNew;

    *pdwFlag = TREE_NEW;
    pNew = (PNODEHEAP)(m_pData + dwNew);
    return pNew->dwElemNum;
}


/************************************************************************
GetOffset - This will get the next available memory in the block, and update
    the record-keeping fields to reflect this memory is used.
*/
BOOL BTreeHeap::GetOffset(DWORD *pdwOffset, DWORD dwSize)
{
    PVOID pTemp;

    while (dwSize > m_dwAvail) {
        m_dwSize += 4*K;
        pTemp = HeapReAlloc(m_pHeap, HEAP_ZERO_MEMORY, m_pData, m_dwSize);
        if (!pTemp)
            return FALSE;
        m_pData = (PBYTE)pTemp;

        m_dwAvail += 4*K;
    }

    *pdwOffset = (m_dwSize - m_dwAvail);
    m_dwAvail -= dwSize;

    return TRUE;
}


/************************************************************************
MakeNode - This will make a new node to add onto the tree. Called by
    AddElem.
*/
DWORD BTreeHeap::MakeNode(PVOID pData, DWORD dwSize)
{
    PNODEHEAP   pNew;
    PBYTE       pTemp;
    DWORD       dwOffsetNode, dwOffsetData;

    // get both offsets before trying to use them because m_pData could be
    // changed from the HeapReAlloc call in GetOffset
    if (GetOffset(&dwOffsetNode, sizeof(NODEHEAP)) == FALSE)
        return NULL;
    if (GetOffset(&dwOffsetData, dwSize) == FALSE)
        return NULL;

    pNew = (PNODEHEAP)(m_pData + dwOffsetNode);
    pTemp = m_pData + dwOffsetData;

    memcpy(pTemp, pData, dwSize);

    pNew->dwData = dwOffsetData;
    pNew->dwElemNum = ++m_dwNumElems;

    return dwOffsetNode;
}


/************************************************************************
FindElem - This will find an element in the tree based on the key
	information passed to the function. It will return the will return
    the data in the tree node.
*/
PVOID BTreeHeap::FindElem (PVOID pKey)
{
    PNODEHEAP   pTemp;
    PWSTR       pWord;
    int         iRet;

    if (m_dwRoot == 0)
        return 0;

    pTemp = (PNODEHEAP)(m_pData + m_dwRoot);
    while ((PBYTE)pTemp != m_pData) {
        pWord = (PWSTR)(m_pData + pTemp->dwData);

        if (m_fUnicode)
            iRet = MyWStrCmp((PWSTR)pKey, pWord);
        else
            iRet = MyAStrCmp((PSTR)pKey, (PSTR)pWord);

        if (iRet == 0)
            return m_pData + pTemp->dwData;
        if (iRet > 0)
		    pTemp = (PNODEHEAP)(m_pData + pTemp->dwRight);
        else
		    pTemp = (PNODEHEAP)(m_pData + pTemp->dwLeft);
	}

    return 0;
}


/************************************************************************
FindElemNumber - This will find an element in the tree based on the key
	information passed to the function. It will return element number.
*/
DWORD BTreeHeap::FindElemNumber (PVOID pKey)
{
    PNODEHEAP   pTemp;
    PWSTR       pWord;
    int         iRet;

    if (m_dwRoot == 0)
        return 0;

    pTemp = (PNODEHEAP)(m_pData + m_dwRoot);
    while ((PBYTE)pTemp != m_pData) {
        pWord = (PWSTR)(m_pData + pTemp->dwData);

        if (m_fUnicode)
            iRet = MyWStrCmp((PWSTR)pKey, pWord);
        else
            iRet = MyAStrCmp((PSTR)pKey, (PSTR)pWord);

        if (iRet == 0)
            return pTemp->dwElemNum;
        if (iRet > 0)
		    pTemp = (PNODEHEAP)(m_pData + pTemp->dwRight);
        else
		    pTemp = (PNODEHEAP)(m_pData + pTemp->dwLeft);
	}

    // couldn't find it, return 0
    return 0;
}




/************************************************************************
DWORD BTree
*************************************************************************/
/************************************************************************
BTreeDWord constructor
*/
BTreeDWord::BTreeDWord (void)
{
	m_dwNumElems = 0;
	m_pRoot = NULL;
    m_pHeap = NULL;
}

/************************************************************************
BTreeDWord destructor - This frees up all of the memory.
*/
BTreeDWord::~BTreeDWord (void)
{
    if (m_pHeap)
        HeapDestroy(m_pHeap);
}


/************************************************************************
FInit - Initialize the tree object. This will allocate a heap from the
    Windows memory manager from which all nodes of the tree will be 
    allocated.
*/
BOOL BTreeDWord::FInit(void)
{
    m_pHeap = HeapCreate(0, 8*K, 0);
    if (!m_pHeap)
        return FALSE;

    return TRUE;
}


/************************************************************************
GetNumElems - Returns the number of elements in the list.
*/
DWORD BTreeDWord::GetNumElem (void)
{
    return m_dwNumElems;
}


/************************************************************************
AddElem - Add an element onto the tree. pdwData is a pointer to
   memory containing the element information (which is copied into internal
   memory). All elements are DWORD's
   .
   This returns the element number of either the newly added element, or in
   the case of a duplicate entry, it will return the element number of the
   existing element. It will also set pdwFlag to indicate it was a duplicate
   or not.
*/
DWORD BTreeDWord::AddElem (DWORD dwData, DWORD *pdwFlag)
{
    PNODE pNode, pNew, *ppChild;
    DWORD dwKey;

    *pdwFlag = TREE_OUTOFMEM;  //default to error
    pNode = m_pRoot;

    // if there is no root yet, we will fall out of the while, otherwise
    // every path leads to a return from the while loop
    while (pNode) {
        dwKey = pNode->Data.dwData;
        if (dwData == dwKey) {
            *pdwFlag = TREE_DUPLICATE;
            return pNode->dwElemNum;
        }
        if (dwData > dwKey)
            ppChild = (PNODE *)&pNode->pRight;
        else
            ppChild = (PNODE *)&pNode->pLeft;

        if ((pNode = *ppChild) == NULL) {
            pNew = MakeNode(dwData);
            if (!pNew)
                return 0;

            *ppChild = pNew;
            *pdwFlag = TREE_NEW;
            return pNew->dwElemNum;
        }
	}

    // it will only get here if m_pRoot is NULL
    pNew = MakeNode(dwData);
    if (!pNew)
        return FALSE;

    m_pRoot = pNew;

    *pdwFlag = TREE_NEW;
    return pNew->dwElemNum;
}


/************************************************************************
MakeNode - This will make a new node to add onto the tree. Called by
    AddElem.
*/
PNODE BTreeDWord::MakeNode(DWORD dwData)
{
    PNODE pNew;

    pNew = (PNODE)HeapAlloc(m_pHeap, HEAP_ZERO_MEMORY, sizeof(NODE));
    if (!pNew)
        return NULL;

    pNew->Data.dwData = dwData;
    pNew->dwElemNum = ++m_dwNumElems;

    return pNew;
}



/************************************************************************
BTree/List combination
*************************************************************************/

/*
 *  helper function prototype
 */
void FreeLists(PLISTNODE);

/************************************************************************
BTreeList constructor
*/
BTreeList::BTreeList (void)
{
	m_pRoot = NULL;
    m_pHeap = NULL;
}

/************************************************************************
BTreeList destructor - This frees up all of the memory.
*/
BTreeList::~BTreeList (void)
{
    if (m_pRoot)
        FreeLists(m_pRoot);

    if (m_pHeap)
        HeapDestroy(m_pHeap);
}


void FreeLists(PLISTNODE pNode)
{
    if (pNode->pLeft)
        FreeLists((PLISTNODE)pNode->pLeft);
    if (pNode->pRight)
        FreeLists((PLISTNODE)pNode->pRight);
    delete pNode->paList;
}


/************************************************************************
FInit - Initialize the tree object. This will allocate a heap from the
    Windows memory manager from which all nodes of the tree will be 
    allocated.
*/
BOOL BTreeList::FInit(void)
{
    m_pHeap = HeapCreate(0, 4*K, 0);
    if (!m_pHeap)
        return FALSE;

    return TRUE;
}


/************************************************************************
AddElem - Finds a node, and if its a new one it creates a new heap array
 object. The data is put into the heap array.
*/
BOOL BTreeList::AddElem (PVOID pData, DWORD dwSize)
{
    PLISTNODE pNode, pNew, *ppChild;
    WORD  iNum, iRet;

    pNode = m_pRoot;

    // if there is no root yet, we will fall out of the while, otherwise
    // every path leads to a return from the while loop
    while (pNode) {
        iNum = pNode->iKey;
        if ((iRet = (*(WORD *)pData - iNum)) == 0) {
            if (pNode->paList->AddElem(pData, dwSize) == FALSE)
                return FALSE;
            return TRUE;
        }
        if (iRet > 0)
            ppChild = (PLISTNODE *)&pNode->pRight;
        else
            ppChild = (PLISTNODE *)&pNode->pLeft;

        if ((pNode = *ppChild) == NULL) {
            pNew = MakeNode(pData, dwSize);
            if (!pNew)
                return FALSE;

            *ppChild = pNew;
            return TRUE;
        }
	}

    // it will only get here if m_pRoot is NULL
    pNew = MakeNode(pData, dwSize);
    if (!pNew)
        return FALSE;

    m_pRoot = pNew;

    return TRUE;
}


/************************************************************************
MakeNode - This will make a new node to add onto the tree. Called by
    AddElem.
*/
PLISTNODE BTreeList::MakeNode(PVOID pData, DWORD dwSize)
{
    PLISTNODE pNew;

    pNew = (PLISTNODE)HeapAlloc(m_pHeap, HEAP_ZERO_MEMORY, sizeof(LISTNODE));
    if (!pNew)
        return NULL;

    pNew->paList = new CHeapArray();
    if (!pNew->paList) {
        HeapFree(m_pHeap, 0, pNew);
        return NULL;
    }

    if (pNew->paList->FInit() == FALSE) {
        HeapFree(m_pHeap, 0, pNew);
        return NULL;
    }

    if (pNew->paList->AddElem(pData, dwSize) == FALSE) {
        HeapFree(m_pHeap, 0, pNew);
        return NULL;
    }

    pNew->iKey = *(WORD *)pData;
    return pNew;
}


/************************************************************************
GetListAddress - This will find an element in the tree based on the key
	information passed to the function. It will return the address of the
    list associated with this element
*/
PVOID BTreeList::GetListAddress (PVOID pKey)
{
    PLISTNODE pNode;
    WORD iRet, iNum;

    pNode = m_pRoot;
    while (pNode) {
        iNum = pNode->iKey;
        if ((iRet = (*(WORD *)pKey - iNum)) == 0)
            return pNode->paList->GetMemAddress();
        if (iRet > 0)
		    pNode = (PLISTNODE)(pNode->pRight);
        else
		    pNode = (PLISTNODE)(pNode->pLeft);
	}

    return 0;
}


/************************************************************************
GetListSize - This will find an element in the tree based on the key
	information passed to the function. It will return the size of the
    list associated with this element
*/
DWORD BTreeList::GetListSize (PVOID pKey)
{
    PLISTNODE pNode;
    WORD iRet, iNum;

    pNode = m_pRoot;
    while (pNode) {
        iNum = pNode->iKey;
        if ((iRet = (*(WORD *)pKey - iNum)) == 0)
            return pNode->paList->GetSize();
        if (iRet > 0)
		    pNode = (PLISTNODE)(pNode->pRight);
        else
		    pNode = (PLISTNODE)(pNode->pLeft);
	}

    return 0;
}


BOOL Traverse(PLISTNODE pNode, PCHeapArray paLists)
{
    if (!pNode)
        return TRUE;

    if (Traverse((PLISTNODE)pNode->pLeft, paLists) == FALSE)
        return FALSE;

    if (paLists->AddElem(&pNode->paList, sizeof(PCHeapArray)) == FALSE)
        return FALSE;

    if (Traverse((PLISTNODE)pNode->pRight, paLists) == FALSE)
        return FALSE;

    return TRUE;
}


BOOL BTreeList::GetAllLists(PCHeapArray paLists)
{
    if (!m_pRoot)
        return FALSE;
    if (Traverse(m_pRoot, paLists) == FALSE)
        return FALSE;
    return TRUE;
}




/************************************************************************
Heap Array object
*************************************************************************/

/************************************************************************
Constructor
*/ 
CHeapArray::CHeapArray(void)
{
    m_pHeap = NULL;
    m_pData = NULL;
    m_dwHeapSize = 0;
    m_dwAvail    = 0;
}


/************************************************************************
Destructor
*/ 
CHeapArray::~CHeapArray(void)
{
    if (m_pHeap)
        HeapDestroy(m_pHeap);
}


/************************************************************************
FInit - Initialize the heap manager object. This will allocate a heap from
 the Windows memory manager from which all array elements will be allocated.
*/
BOOL CHeapArray::FInit(void)
{
    m_pHeap = HeapCreate(0, 0, 0);
    if (!m_pHeap)
        return FALSE;

    return TRUE;
}


/************************************************************************
GetMemAddress - Return the address of the array memory
*/
PVOID CHeapArray::GetMemAddress(void)
{
    return m_pData;
}


/************************************************************************
GetMemAddress - Return the address of the array memory
*/
DWORD CHeapArray::GetSize(void)
{
    return (m_dwHeapSize - m_dwAvail);
}


/************************************************************************
Clean - Frees all the memory in the object
*/
void CHeapArray::Clean(void)
{
    if (m_pData) {
        HeapFree(m_pHeap, NULL, m_pData);
        m_pData = NULL;
        m_dwHeapSize = 0;
        m_dwAvail    = 0;
    }
}


/************************************************************************
AddElem - Add an array element onto the end of the array
*/
BOOL CHeapArray::AddElem(PVOID pData, DWORD dwSize)
{
    PBYTE pNew;

    if (!m_pData) {
        m_pData = HeapAlloc(m_pHeap, HEAP_ZERO_MEMORY, 4*K);
        if (!m_pData)
            return FALSE;
        m_dwHeapSize = 4*K;
        m_dwAvail = m_dwHeapSize;
    }

    while (dwSize > m_dwAvail) {
        m_pData = HeapReAlloc(m_pHeap, HEAP_ZERO_MEMORY, m_pData,
                              m_dwHeapSize+4*K);
        if (!m_pData)
            return FALSE;
        m_dwHeapSize += 4*K;
        m_dwAvail += 4*K;
    }

    pNew = (PBYTE)m_pData + (m_dwHeapSize-m_dwAvail);
    memcpy (pNew, pData, dwSize);
    m_dwAvail -= dwSize;

    return TRUE;
}




/************************************************************************
Stack object
*************************************************************************/

/************************************************************************
Constructor
*/ 
CStack::CStack(void)
{
    m_pHeap = NULL;
    m_pMem  = NULL;
    m_dwHeapSize = 0;
    m_dwAvail    = 0;
}


/************************************************************************
Destructor
*/ 
CStack::~CStack(void)
{
    if (m_pHeap)
        HeapDestroy(m_pHeap);
}


/************************************************************************
FInit - Initialize the heap manager object. This will allocate a heap from
 the Windows memory manager from which all array elements will be allocated.
*/
BOOL CStack::FInit(void)
{
    m_pHeap = HeapCreate(0, 8*K, 0);
    if (!m_pHeap)
        return FALSE;

    m_pMem = HeapAlloc(m_pHeap, HEAP_ZERO_MEMORY, 8*K);
    if (!m_pMem) {
        HeapDestroy(m_pHeap);
        return FALSE;
    }

    m_dwHeapSize = 8*K;

    m_pStackEnd = (PBYTE)m_pMem;
    m_pStack = (PBYTE)m_pMem + m_dwHeapSize;
    m_pStackStart = m_pStack;

    return TRUE;
}


/************************************************************************
Push - Add an element onto the stack
*/
BOOL CStack::Push(SDATA dData)
{
    if ((m_pStack -= sizeof(SDATA)) < m_pStackEnd)
        return FALSE;

    *(PSDATA)m_pStack = dData;
    return TRUE;
}


/************************************************************************
Pop - Take an element off the stack
*/
BOOL CStack::Pop(PSDATA pdData)
{
    SDATA dData;

    pdData->pData  = NULL;
    pdData->dwSize = 0;

    if (m_pStack >= m_pStackStart)
        return FALSE;

    dData = *(PSDATA)m_pStack;
    m_pStack += sizeof(SDATA);
    *pdData = dData;
    return TRUE;
}


/************************************************************************
GetNumElems - returns the number of elements on the stack
*/
DWORD CStack::GetNumElems()
{
    return ((m_pStackStart - m_pStack) / sizeof(SDATA));
}


/************************************************************************
Reset - Positions the stack back to the beginning
*/
void CStack::Reset()
{
    m_pStack = m_pStackStart;
}