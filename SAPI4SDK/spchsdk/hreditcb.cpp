/*
Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

  #include "cmnhdr.h"
#include "REditCB.h"

/*************************************************************************
CRichEditOleCallback - Source Notification object. 
*/

CRichEditOleCallback::CRichEditOleCallback (void)
{
}

CRichEditOleCallback::~CRichEditOleCallback (void)
{
// this space intentionally left blank
}

STDMETHODIMP CRichEditOleCallback::QueryInterface (REFIID riid, LPVOID *ppv)
{
   *ppv = NULL;

   if (IsEqualIID (riid, IID_IUnknown) || IsEqualIID(riid, IID_IRichEditOleCallback)) {
      *ppv = (LPVOID) this;
      return NOERROR;
   }

   return ResultFromScode (E_NOINTERFACE);
}

STDMETHODIMP_ (ULONG) CRichEditOleCallback::AddRef (void)
{
// normally this increases a reference count, but this object
// is going to be freed as soon as the app is freed, so it doesn't
// matter
   return 1;
}

STDMETHODIMP_(ULONG) CRichEditOleCallback::Release (void)
{
// normally this releases a reference count, but this object
// is going to be freed when the application is freed so it doesnt
// matter
   return 1;
}

STDMETHODIMP CRichEditOleCallback::GetNewStorage(LPSTORAGE FAR *)
{
   return E_NOTIMPL;
}

STDMETHODIMP CRichEditOleCallback::GetInPlaceContext(LPOLEINPLACEFRAME FAR *,
                                                     LPOLEINPLACEUIWINDOW FAR *,
                                                     LPOLEINPLACEFRAMEINFO)
{
   return E_NOTIMPL;
}

STDMETHODIMP CRichEditOleCallback::ShowContainerUI(BOOL)
{
   return E_NOTIMPL;
}

STDMETHODIMP CRichEditOleCallback::QueryInsertObject(LPCLSID, 
                                                     LPSTORAGE, 
                                                     LONG)
{
   return E_NOTIMPL;
}

STDMETHODIMP CRichEditOleCallback::DeleteObject(LPOLEOBJECT)
{
   return E_NOTIMPL;
}

STDMETHODIMP CRichEditOleCallback::QueryAcceptData(LPDATAOBJECT,	
                                                   CLIPFORMAT FAR *,	
                                                   DWORD reco,
                                                   BOOL,
                                                   HGLOBAL)
{
   // disable drop
   if (RECO_DROP == reco)
      return E_FAIL; 

   return S_OK;
}

STDMETHODIMP CRichEditOleCallback::ContextSensitiveHelp(BOOL)
{
   return E_NOTIMPL;
}

STDMETHODIMP CRichEditOleCallback::GetClipboardData(CHARRANGE FAR *,
                                                    DWORD,
                                                    LPDATAOBJECT FAR *)
{
   return E_NOTIMPL;
}

STDMETHODIMP CRichEditOleCallback::GetDragDropEffect(BOOL,
                                                     DWORD,
                                                     LPDWORD)
{
   return E_NOTIMPL;
}

STDMETHODIMP CRichEditOleCallback::GetContextMenu(WORD,
                                                  LPOLEOBJECT,
                                                  CHARRANGE FAR *,
                                                  HMENU FAR *)
{
   return E_NOTIMPL;
}
   