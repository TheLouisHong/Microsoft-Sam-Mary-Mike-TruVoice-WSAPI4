#ifndef __REDITCB__
#define __REDITCB__

class CRichEditOleCallback : public IRichEditOleCallback 
{
   private:

   public:
      CRichEditOleCallback (void);
      ~CRichEditOleCallback (void);

      // IUnkown members that delegate to m_punkOuter
      // Non-delegating object IUnknown
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      STDMETHODIMP GetNewStorage(LPSTORAGE FAR *);
      STDMETHODIMP GetInPlaceContext(LPOLEINPLACEFRAME FAR *,
                                     LPOLEINPLACEUIWINDOW FAR *,
                                     LPOLEINPLACEFRAMEINFO);
      STDMETHODIMP ShowContainerUI(BOOL);
      STDMETHODIMP QueryInsertObject(LPCLSID, 
                                     LPSTORAGE, 
                                     LONG);
      STDMETHODIMP DeleteObject(LPOLEOBJECT);
      STDMETHODIMP QueryAcceptData(LPDATAOBJECT,	
                                   CLIPFORMAT FAR *,	
                                   DWORD,
                                   BOOL,
                                   HGLOBAL);
      STDMETHODIMP ContextSensitiveHelp(BOOL);
      STDMETHODIMP GetClipboardData(CHARRANGE FAR *,
                                    DWORD,
                                    LPDATAOBJECT FAR *);
      STDMETHODIMP GetDragDropEffect(BOOL,
                                     DWORD,
                                     LPDWORD);
      STDMETHODIMP GetContextMenu(WORD,
                                  LPOLEOBJECT,
                                  CHARRANGE FAR *,
                                  HMENU FAR *);
};

#endif