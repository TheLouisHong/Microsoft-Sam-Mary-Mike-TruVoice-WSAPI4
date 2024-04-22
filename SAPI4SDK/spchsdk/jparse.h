/********************************************************************
Parse.h - C++ object that reads a word at a time from a file.

begun 2/20/96 by Mike Rozak
*/

#ifndef _PARSE_H_
#define _PARSE_H_

/*
CSharedMem::Init - Initalize shared memory so it has the specified
   memory size. It starts out with a reference count of 1. Returnes
   FALSE if error.
CSharedMem::AddRef - Adds one to the reference count.
CSharedMem::Release - Releases the reference count by 1. If this
   goes to 0 it releases all memory.
*/
class CSharedMem {
   private:
      DWORD m_dwCount;

   public:
      CSharedMem (void);
      ~CSharedMem (void);

      BOOL Init (DWORD dwSize);
      void AddRef (void);
      void Release (void);
      PVOID m_pMem;     // memory
   };

typedef CSharedMem * PCSharedMem;

/*
CParse::Init - Initialize the parse to read from the specified file.

CParse::Next - Read the next word from the file. This fills in szWord
   with the next word. dwWordSize is the maximum size. Returns TRUE
   if a word is gotten, FALSE if the file has ended.

CParse::Clone - Clones the parse object so another instance is parsing
   at the same location.
*/

class CParse {
   private:
      CSharedMem *m_pSharedFile;   // file contents, shared
      DWORD m_dwFileSize; // number of bytes in the file
      BOOL MyIsSpace (char cTemp);

   public:
      CParse (void);
      ~CParse (void);

      BOOL Init (char *pszFile);
      BOOL Next (char *szWord, DWORD dwWordSize);
      CParse *Clone ();
      BOOL  m_fWordMode;
      DWORD m_dwFilePosn; // current file position
   };

typedef CParse * PCParse;

#endif   // _PARSE_H_
