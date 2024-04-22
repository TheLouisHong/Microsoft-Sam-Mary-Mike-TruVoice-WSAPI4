/* TelCtl.h*/

#ifndef _TELCTL_H_
#define _TELCTL_H_



/* CYesNo */

class CYesNo : public CTelControlFramework {
   public:
      // virtual functions
      virtual PCWSTR GetControlName (void);
      virtual DWORD GetDefaultTextResID (LANGID langID);
      virtual void GetCLSID (CLSID *pCLSID);
      virtual void OnState (DWORD dwStateID);
      virtual void OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                               DWORD dwParseMemSize,
                                               PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult);
};

typedef CYesNo * PCYesNo;

#define  MAXDIGITS   24

class CExtension : public CTelControlFramework {
   public:
      // virtual functions
      virtual PCWSTR GetControlName (void);
      virtual DWORD GetDefaultTextResID (LANGID langID);
      virtual void GetCLSID (CLSID *pCLSID);
      virtual void OnState (DWORD dwStateID);
      virtual void OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                               DWORD dwParseMemSize,
                                               PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult);
      virtual void OnNoAnswer (void);

      // member variables
      WCHAR    m_szCurrentDigits[MAXDIGITS];
      WCHAR    m_szSpaceDigits[MAXDIGITS*2];
};

typedef CExtension * PCExtension;

class CPhoneNum : public CTelControlFramework {
   public:
      // virtual functions
      virtual PCWSTR GetControlName (void);
      virtual DWORD GetDefaultTextResID (LANGID langID);
      virtual void GetCLSID (CLSID *pCLSID);
      virtual void OnState (DWORD dwStateID);
      virtual void OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                               DWORD dwParseMemSize,
                                               PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult);

      // member variables
      WCHAR    m_szCurrentDigits[MAXDIGITS];
      WCHAR    m_szSpaceDigits[MAXDIGITS*2];
      WCHAR    m_szCombinedDigits[MAXDIGITS];
};

typedef CPhoneNum * PCPhoneNum;

class CGrammar : public CTelControlFramework {
   public:
      CGrammar();
      ~CGrammar();

      // virtual functions
      virtual PCWSTR GetControlName (void);
      virtual DWORD GetDefaultTextResID (LANGID langID);
      virtual void GetCLSID (CLSID *pCLSID);
      virtual void OnState (DWORD dwStateID);
      virtual void OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                               DWORD dwParseMemSize,
                                               PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult);

      DWORD    m_dwRecog;     // recognition result
      DWORD    m_dwParseSize; // number of bytes in the parse
      PWSTR    m_pszParse;    // parse string pointer. allocated with malloc
};

typedef CGrammar * PCGrammar;



class CDate : public CTelControlFramework {
public:
   // virtual functions
   virtual PCWSTR GetControlName (void);
   virtual DWORD GetDefaultTextResID (LANGID langID);
   virtual void GetCLSID (CLSID *pCLSID);
   virtual void OnState (DWORD dwStateID);
   virtual void OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                               DWORD dwParseMemSize, PSRPHRASEW pSRPhrase,
                               LPUNKNOWN lpUnkResult);

   DWORD    m_dwRecog;     // recognition result
   WCHAR    m_szParse[256];   // parse results

   DWORD MungeDateFromParse(DWORD dwRecog, WCHAR *sz);
   void CalcRelativeDate(SYSTEMTIME *psystime,
                         DWORD dwDirection,
                         int iNumTimeUnits,
                         DWORD dwTimeUnit);
   int GetNumFromStart(WCHAR *sz);
   int GetNumFromEnd(WCHAR *sz);
   DWORD GetRelativeToTodayDirection(DWORD dwRecog);
   int GetRelativeToTodayNumDays(DWORD dwRecog);
   int WeekdayDaysAway(DWORD dwToday, DWORD dwWeekday, DWORD dwDirection);
};

typedef CDate * PCDate;



class CTime : public CTelControlFramework {
   public:
      // virtual functions
      virtual PCWSTR GetControlName (void);
      virtual DWORD GetDefaultTextResID (LANGID langID);
      virtual void GetCLSID (CLSID *pCLSID);
      virtual void OnState (DWORD dwStateID);
      virtual void OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                               DWORD dwParseMemSize,
                                               PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult);

      DWORD    m_dwRecog;     // recognition result
      WCHAR    m_szParse[64];   // parse results

};

typedef CTime * PCTime;

class CRecord;

class CCtlSTRecordNotify : public ISTRecordNotifySink {
   private:

   public:
      CCtlSTRecordNotify (void);
      ~CCtlSTRecordNotify (void);

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // ISRNotifySink
	   STDMETHODIMP AutoStop       (void);
	   STDMETHODIMP VU         (WORD);

      CRecord*    m_pRecord;
   };
typedef CCtlSTRecordNotify * PCCtlSTRecordNotify;

class CRecord : public CTelControlFramework {
   friend class CCtlSTRecordNotify;
   public:

      CRecord(void);
      ~CRecord(void);

      // virtual functions
      virtual PCWSTR GetControlName (void);
      virtual DWORD GetDefaultTextResID (LANGID langID);
      virtual void GetCLSID (CLSID *pCLSID);
      virtual void OnState (DWORD dwStateID);
      virtual void OnAbort (void);
      virtual void OnDTMF (WORD wDTMF);
      virtual void OnTTSStop (void);
      STDMETHODIMP Start          (PITELCONTROLNOTIFYSINK);


      PVOID    m_pWAV;        // wave file. Allocated with CoTaskmemAlloc
      DWORD    m_dwWAVSize;   // wave file size
      BOOL     m_fRecording;  // true if recording
      CCtlSTRecordNotify m_NS;   // notification sink
      PISTRECORD  m_pISTRecord;  // record object
};

typedef CRecord * PCRecord;



class CSpelling : public CTelControlFramework {
      bool m_bNewAction;
      bool m_bVerifying;

      int m_iThisCmd;
      bool m_bIsCorrection;
      WCHAR m_chThis;
      WCHAR *m_szThisPronunciation;

      DWORD m_dwRecog;     // recognition result
      DWORD m_dwParseSize; // number of bytes in the parse

      PWSTR m_szParse;     // parse string pointer. allocated with malloc
      WCHAR **m_rgszPronunciations;

      // variables for undo
      int m_iUndoCmd;
      WCHAR m_chUndoData;
      WCHAR *m_szUndoData;
      WCHAR **m_rgszUndoData;
      int m_iUndoArrayLen;

      // variables for misrecognition
      int m_iNumMisrecognized;
      int *m_rgiMisrecognizedCmds;
      WCHAR *m_rgchMisrecognizedChars;

      void ClearEnteredData();

      bool IsDTMFString(WCHAR *szThisPhrase);
      void ParseDTMFString(WCHAR *szThisPhrase);
      void ParsePhrase(WCHAR *szThisPhrase);
      void ParseAsInPhrase(WCHAR *szThisPhrase);
      void ParseNumOrPunc(WCHAR *szThisPhrase);
      void ParseAlphabeticChar(WCHAR *szThisPhrase);
      void DoAlphabeticChar(WCHAR ch, bool bCapsFlag);

      void ApplyCommand(DWORD dwStateID);
      void DoConcatenateChar(WCHAR ch, WCHAR *szThisPronunciation);
      void DoDelete(bool bSayWhatIsBeingDeleted);
      void ListWords();
      void ListPunctuation();
      void DoRecap();
      void DoRestart();

      void InitMisrecognizedList();
      bool WasAlreadyMisrecognized(int iCmd, WCHAR ch);
      void AddToMisrecognizedList(int iCmd, WCHAR ch);
      void ClearMisrecognizedList();

      void InitUndo();
      void SaveUndoData();
      void Undo();
      void ClearUndoData();

   public:
      CSpelling();
      ~CSpelling();

      // virtual functions
      virtual PCWSTR GetControlName (void);
      virtual DWORD GetDefaultTextResID (LANGID langID);
      virtual void GetCLSID (CLSID *pCLSID);
      virtual void OnState (DWORD dwStateID);
      virtual void OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                  DWORD dwParseMemSize,
                                  PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult);
      virtual void OnVerify();
      virtual void OnAskBack();
};

typedef CSpelling * PCSpelling;

template<class T> struct SLinkedListElement;
template<class T> class CLinkedListIterator;

template<class T>
class CLinkedList {
  
protected:
  SLinkedListElement<T> *m_pleHead, *m_pleTail;
  int m_iNumElems;
  
public:
  CLinkedList() { m_pleHead = m_pleTail = 0; m_iNumElems = 0; }
  ~CLinkedList() { (void)EDestroyAll(); }
  
  int EAddToHead(T *data);
  int EAddToTail(T *data);
  
  CLinkedListIterator<T> GetHead() { return CLinkedListIterator<T>(this, m_pleHead); }
  CLinkedListIterator<T> GetTail() { return CLinkedListIterator<T>(this, m_pleTail); }
  CLinkedListIterator<T> GetIterFor(T *data);
  int GetNumElems() { return m_iNumElems; }
  
  int ERemoveAt(CLinkedListIterator<T>& li);
  int ERemoveAll();
  int EDestroyAt(CLinkedListIterator<T>& li);
  int EDestroyAll();
  
  friend CLinkedListIterator<T>;
};


template<class T>
struct SLinkedListElement {
  SLinkedListElement<T> *m_plePrev, *m_pleNext;
  T *m_data;
};


template<class T>
class CLinkedListIterator {
  
protected:
  CLinkedList<T> *m_plist;
  SLinkedListElement<T> *m_pleCurr;
  
  CLinkedListIterator(CLinkedList<T> *plist, SLinkedListElement<T> *ple)
  { m_plist = plist; m_pleCurr = ple; }
  
public:
  CLinkedListIterator() { m_plist = 0; m_pleCurr = 0; }
  CLinkedListIterator(CLinkedListIterator<T>& li)
  { m_plist = li.m_plist; m_pleCurr = li.m_pleCurr; }
  ~CLinkedListIterator() {}
  void Next() { if(m_pleCurr) m_pleCurr = m_pleCurr->m_pleNext; }
  void Prev() { if(m_pleCurr) m_pleCurr = m_pleCurr->m_plePrev; }
  void Rewind() { m_pleCurr = m_plist->m_pleHead; }
  void FastForward() { m_pleCurr = m_plist->m_pleTail; }
  bool IsValid() { return (m_pleCurr != 0) ? true : false; }
  T *GetData() { return (m_pleCurr != 0) ? m_pleCurr->m_data : 0; }
  CLinkedListIterator<T>& operator=(CLinkedListIterator<T> li)
  { m_plist = li.m_plist; m_pleCurr = li.m_pleCurr; return *this; }
  
  friend CLinkedList<T>;
};




template<class T>
int CLinkedList<T>::EAddToHead(T *data) {
  
  SLinkedListElement<T> *ple;
  
  if((ple = new SLinkedListElement<T>) == 0)
	 return -1;
  ple->m_data = data;
  ple->m_plePrev = 0;
  ple->m_pleNext = m_pleHead;
  if(m_pleHead)
    m_pleHead->m_plePrev = ple;
  m_pleHead = ple;
  if(m_pleTail == 0)
    m_pleTail = ple;
  m_iNumElems++;
  return 0;
}


template<class T>
int CLinkedList<T>::EAddToTail(T *data) {
  
  SLinkedListElement<T> *ple;
  
  if((ple = new SLinkedListElement<T>) == 0)
	 return -1;
  ple->m_data = data;
  ple->m_plePrev = m_pleTail;
  ple->m_pleNext = 0;
  if(m_pleTail)
    m_pleTail->m_pleNext = ple;
  m_pleTail = ple;
  if(m_pleHead == 0)
    m_pleHead = ple;
  m_iNumElems++;
  return 0;
}


template<class T>
CLinkedListIterator<T> CLinkedList<T>::GetIterFor(T *data) {
  
  SLinkedListElement<T> *ple = m_pleHead;
  
  while((ple != 0) && (ple->m_data != data))
	 ple = ple->m_pleNext;
  if(ple)
	 return CLinkedListIterator<T>(this, ple);
  else
	 return CLinkedListIterator<T>();
}


template<class T>
int CLinkedList<T>::ERemoveAt(CLinkedListIterator<T>& li) {
  
  if(!li.IsValid())
	 return -1;
  
  if(li.m_pleCurr == m_pleHead)
	 m_pleHead = (li.m_pleCurr)->m_pleNext;
  if(li.m_pleCurr == m_pleTail)
	 m_pleTail = (li.m_pleCurr)->m_plePrev;
  if((li.m_pleCurr)->m_plePrev != 0)
	 ((li.m_pleCurr)->m_plePrev)->m_pleNext = (li.m_pleCurr)->m_pleNext;
  if((li.m_pleCurr)->m_pleNext != 0)
	 ((li.m_pleCurr)->m_pleNext)->m_plePrev = (li.m_pleCurr)->m_plePrev;
  
  delete li.m_pleCurr;
  li.m_pleCurr = 0;
  
  m_iNumElems--;
  return 0;
}


template<class T>
int CLinkedList<T>::ERemoveAll() {
  
  SLinkedListElement<T> *ple = m_pleHead, *pleTemp;
  
  while(ple) {
	 pleTemp = ple->m_pleNext;
	 delete ple;
	 ple = pleTemp;
  }
  
  m_pleHead = m_pleTail = 0;
  m_iNumElems = 0;
  return 0;
}


template<class T>
int CLinkedList<T>::EDestroyAt(CLinkedListIterator<T>& li) {
  
  T *data;
  int iRet;
  
  data = (li.m_pleCurr)->m_data;
  if((iRet = ERemoveAt(li)) != 0)
	 return -1;
  delete data;
  //m_iNumElems--; // don't do this here--it's already done in ERemoveAt()
  return 0;
}


template<class T>
int CLinkedList<T>::EDestroyAll() {
  
  SLinkedListElement<T> *ple = m_pleHead, *pleTemp;
  
  while(ple) {
	 delete ple->m_data;
	 pleTemp = ple->m_pleNext;
	 delete ple;
	 ple = pleTemp;
  }
  
  m_pleHead = m_pleTail = 0;

  m_iNumElems = 0;
  return 0;
}


struct SNameAuxPerson {
   WCHAR *m_szPreTitle;
   WCHAR *m_szFirstName;
   WCHAR *m_szLastName;
   WCHAR *m_szPostTitle;
   WCHAR *m_szFullName;
   SNameAuxPerson() { Clear(); }
   SNameAuxPerson(WCHAR *szName);
   void GetPreTitle(WCHAR **ppchRestOfName);
   void GetFirstName(WCHAR **ppchRestOfName);
   void GetLastName(WCHAR **ppchRestOfName);
   void GetPostTitle(WCHAR **ppchRestOfName);
   ~SNameAuxPerson() { if(m_szPreTitle)  free(m_szPreTitle);
                       if(m_szFirstName) free(m_szFirstName);
                       if(m_szLastName)  free(m_szLastName);
                       if(m_szPostTitle) free(m_szPostTitle);
                       if(m_szFullName)  free(m_szFullName); }
   void Clear() { m_szPreTitle = m_szFirstName = m_szLastName = m_szPostTitle = m_szFullName = 0; }


   bool operator==(SNameAuxPerson& nap);
   bool operator!=(SNameAuxPerson& nap) { return !(*this == nap); }
};

class CName : public CTelControlFramework {
      bool m_bNewAction;
      bool m_bVerifying;
      bool m_bIsFirstPrompt;

      WCHAR *m_szSpecialGrammar;
      CLinkedList<SNameAuxPerson> m_llPeople;

      WCHAR *m_szParse;
      DWORD m_dwRecog;

      void FillInAllNames();
      void GrammarForTwo(WCHAR **pszPrompt);
      void GrammarForMany(WCHAR **pszPrompt);
      void ConcatGrammarFirstPossLast(WCHAR *szBuffer, SNameAuxPerson *pnap, int iPersonNum);
      void ConcatGrammarPossFirstLast(WCHAR *szBuffer, SNameAuxPerson *pnap, int iPersonNum);
      void ConcatGrammarLast(WCHAR *szBuffer, SNameAuxPerson *pnap, int iPersonNum);
      void ConcatGrammarAuxData(WCHAR *szBuffer, SNameAuxPerson *pnap, int iPersonNum);

   public:
      CName();
      ~CName();

      // virtual functions
      virtual PCWSTR GetControlName (void);
      virtual DWORD GetDefaultTextResID (LANGID langID);
      virtual void GetCLSID (CLSID *pCLSID);
      virtual void OnState (DWORD dwStateID);
      virtual void OnPhraseParse (DWORD dwParseID, PVOID pParseMem,
                                  DWORD dwParseMemSize,
                                  PSRPHRASEW pSRPhrase, LPUNKNOWN lpUnkResult);
      virtual STDMETHODIMP Compile (LPWSTR *ppszError, DWORD *pdwSize);
      virtual void OnAskBack();
      //virtual void OnAskBack();
};

typedef CName * PCName;



#endif // _TELCTL_H_