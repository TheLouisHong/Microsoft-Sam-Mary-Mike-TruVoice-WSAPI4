/************************************************************************
link.h - Linked list, and BTree header file.

*/

#ifndef _LINK_H_
#define _LINK_H_

#include <speech.h>

#define K 1024

#define TYPE_WORD	0x1
#define TYPE_DWORD	0x2
#define TYPE_CHAR	0x4
#define TYPE_WCHAR	0x8
#define TYPE_QWORD  0x10

#define TREE_NEW        1
#define TREE_DUPLICATE  2
#define TREE_OUTOFMEM   4

#define ALLOC(x) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, x);
#define FREE(x)  HeapFree(GetProcessHeap(), NULL, x);

int MyWStrCmp (WCHAR * pA, WCHAR * pB);
int MyWStrLen (WCHAR * pA);
int MyWStrCpy (WCHAR * pA, WCHAR * pB);
int MyWStrCat (WCHAR * pA, WCHAR * pB);
int MyAStrCmp (PSTR pA, PSTR pB);
int MyAStrLen (PSTR pA);
int MyAStrCpy (PSTR pA, PSTR pB);
int MyAStrCat (PSTR pA, PSTR pB);


/**********************************************************************
  Link List
 **********************************************************************/
typedef struct {
   PVOID        pElem;      // pointer to the element
   DWORD        dwElemSize; // size of the element in bytes
   PVOID 		pNext;		// pointer to next in link
   PVOID		pPrev;		// pointer to previous in link
   } LINKELEM, * PLINKELEM;

class CLink {
   private:
      DWORD dwNumElems;			    // number of elements stored away
	  PLINKELEM m_pFirst;			// first element in linked list
	  PLINKELEM m_pLast;			// last element in linked list

   public:
      CLink (void);
      ~CLink (void);

      void  FreeAllElems();

      DWORD GetNumElems(void);
      BOOL  AddElem (PVOID pData, DWORD dwSize);
      BOOL  RemoveElem (PVOID pKey, DWORD dwType, DWORD dwOffset);
      DWORD GetElemSize (PVOID pKey, DWORD dwType, DWORD dwOffset);
      PVOID GetElem (PVOID pKey, DWORD dwType, DWORD dwOffset);
      BOOL  SetElem (PVOID pKey, DWORD dwType, DWORD dwOffset, PVOID pData,
                     DWORD dwSize);
	  PVOID GetEntryElem (DWORD dwEntry);
      BOOL  UpdateElem (PVOID pData, DWORD dwEntry);
      PVOID GetNextElem (PVOID, PVOID *);
      PVOID GetPrevElem (PVOID, PVOID *);
   };
typedef CLink * PCLink;





/**********************************************************************
  Queue
 **********************************************************************/
typedef struct {
   PVOID        pElem;      // pointer to the element
   DWORD        dwElemSize; // size of the element in bytes
   } QUEUEELEM, * PQUEUEELEM;

class CQueue {
   private:
      DWORD m_dwNumElems;       // number of elements stored away
      DWORD m_dwMaxElems;       // max number of elements allowed in queue
      PVOID m_pHeap;            // pointer to heap
      PVOID m_pQueueStart;      // begin of queue memory
      PVOID m_pQueueEnd;        // end of queue memory
      PQUEUEELEM m_pQueueTail;  // queue tail
      PQUEUEELEM m_pQueueHead;  // queue head

   public:
      CQueue (void);
      ~CQueue (void);

      BOOL FInit();

      BOOL  QueueElem       (PVOID pData, DWORD dwSize);
      BOOL  QueueElemToHead (PVOID pData, DWORD dwSize);
      BOOL  DequeueElem     (void);
      PVOID GetQueueHead    (void);
      DWORD GetNumElems     (void);
      void  FreeAllElems    (void);
   };
typedef CQueue * PCQueue;





typedef struct {
    union {
        PVOID        pElem;      // pointer to the element for string tree
        DWORD        dwData;     // data for DWORD tree
    } Data;
    DWORD        dwElemNum;  // element number
    PVOID        pLeft;      // pointer to left child
    PVOID        pRight;     // pointer to right child
    } NODE, * PNODE;

/**********************************************************************
  BTree stuff (GUID key)
 **********************************************************************/
class BTree {
   private:
      DWORD m_dwNumElems;   // number of elements stored away
	  PNODE m_pRoot;        // tree root
      PVOID m_pHeap;        // pointer to heap

      PNODE MakeNode(PVOID, DWORD);

   public:
      BTree (void);
      ~BTree (void);

      BOOL FInit();

      DWORD AddElem (PVOID pData, DWORD dwSize, DWORD *pdwFlag);
      PVOID FindElem (GUID riid);
      DWORD GetNumElem(void);
   };
typedef BTree * PBTree;



typedef struct {
    DWORD        dwData;     // offset of data
    DWORD        dwElemNum;  // element number
    DWORD        dwLeft;     // offset of left child
    DWORD        dwRight;    // offset of right child
    } NODEHEAP, * PNODEHEAP;

/**********************************************************************
  BTree stuff (text string key and offsets into big heap rather than
  pointers to small memory blocks
 **********************************************************************/
class BTreeHeap {
   private:
      DWORD m_dwNumElems;   // number of elements stored away
	  DWORD m_dwRoot;       // offset of tree root
      PVOID m_pHeap;        // pointer to heap
      BOOL  m_fUnicode;     // UNICODE or ANSI string indicator

      PBYTE m_pData;        // pointer to memory block that has the data
      DWORD m_dwAvail;      // number of bytes available
      DWORD m_dwSize;       // total size of memory block

      DWORD MakeNode(PVOID, DWORD);
      BOOL  GetOffset(DWORD *, DWORD);

   public:
      BTreeHeap (void);
      ~BTreeHeap (void);

      BOOL FInit();
      void SetCharMode(BOOL);

      HRESULT GetTree(PVOID *, DWORD *);
      HRESULT SetTree(PVOID, DWORD);

      DWORD AddElem (PVOID pData, DWORD dwSize, DWORD *pdwFlag);
      PVOID FindElem (PVOID pKey);
      DWORD FindElemNumber (PVOID pKey);
      DWORD GetNumElem(void);
   };
typedef BTreeHeap * PBTreeHeap;



/**********************************************************************
  BTreeDword stuff (DWord Key)
 **********************************************************************/
class BTreeDWord {
   private:
      DWORD m_dwNumElems;   // number of elements stored away
	  PNODE m_pRoot;        // tree root
      PVOID m_pHeap;        // pointer to heap

      PNODE MakeNode(DWORD);

   public:
      BTreeDWord (void);
      ~BTreeDWord (void);

      BOOL FInit();

      DWORD AddElem (DWORD dwData,     // tree key
                     DWORD *pdwFlag);
      PVOID FindElem (PVOID pKey);
      DWORD GetNumElem(void);
   };
typedef BTreeDWord * PBTreeDWord;



/**********************************************************************
Heap array object  
 **********************************************************************/
class CHeapArray {
   private:
      PVOID m_pHeap;        // pointer to heap
      PVOID m_pData;        // pointer to memory
      DWORD m_dwHeapSize;   // current heap size in bytes
      DWORD m_dwAvail;      // number of bytes avail in the heap memory

   public:
      CHeapArray (void);
      ~CHeapArray (void);

      BOOL FInit();

      BOOL  AddElem(PVOID pData, DWORD dwSize);
      PVOID GetMemAddress();
      DWORD GetSize();
      void  Clean();
   };
typedef CHeapArray * PCHeapArray;



/**********************************************************************
  BTree/List combination
 **********************************************************************/
typedef struct {
   PCHeapArray  paList;     // pointer to the heap array object
   PVOID        pLeft;      // pointer to left child
   PVOID        pRight;     // pointer to right child
   WORD         iKey;       // element number
   } LISTNODE, * PLISTNODE;

class BTreeList {
   private:
      PLISTNODE m_pRoot;        // tree root
      PVOID     m_pHeap;        // pointer to heap

      PLISTNODE MakeNode(PVOID, DWORD);

   public:
      BTreeList (void);
      ~BTreeList (void);

      BOOL FInit();

      BOOL  AddElem (PVOID pData, DWORD dwSize);
      PVOID GetListAddress(PVOID);
      DWORD GetListSize(PVOID);
      BOOL  GetAllLists(PCHeapArray);
   };
typedef BTreeList * PBTreeList;



/***************************************************************
Stack object
*/
class CStack {
    private:
        PVOID m_pHeap;        // pointer to heap
        PVOID m_pMem;         // pointer to memory
        PBYTE m_pStack;       // pointer to current stack position
        PBYTE m_pStackEnd;    // pointer to the top of the stack
        PBYTE m_pStackStart;  // pointer to bottom of stack

        DWORD m_dwHeapSize;   // current heap size in bytes
        DWORD m_dwAvail;      // number of bytes avail in the heap memory

    public:
        CStack (void);
        ~CStack (void);

        BOOL FInit();

        BOOL    Push(SDATA);
        BOOL    Pop(PSDATA);
        DWORD   GetNumElems(void);
        void    Reset(void);
};
typedef CStack *PCSTACK;

#endif // _LINK_H_
