// WrapObj.h : Declaration of CWrapObj, the object that is instantiated 
// as a control inside IE.

// This code is copyright (c) Microsoft Corporation, 1998.  All rights reserved.

#ifndef __WRAPOBJ_H_
#define __WRAPOBJ_H_

#include "resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// CWrapObj
class ATL_NO_VTABLE CWrapObj :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapObj,&CLSID_WrapObj>,
	public CComControl<CWrapObj>,
	public IPersistStreamInitImpl<CWrapObj>,
	public IOleControlImpl<CWrapObj>,
	public IOleObjectImpl<CWrapObj>,
	public IOleInPlaceActiveObjectImpl<CWrapObj>,
	public IOleInPlaceObjectWindowlessImpl<CWrapObj>,
#if _MSC_VER >= 1200
	public IObjectSafetyImpl<CWrapObj, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
//	, public IUnknown
#else
	public IObjectSafetyImpl<CWrapObj>,
	public IUnknown
#endif
{
public:
	CWrapObj()
	{
		// This definition makes the object
		// set its own size -- in this case, to 
		// zero.

		m_bAutoSize = TRUE;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPOBJ)

BEGIN_COM_MAP(CWrapObj) 
#if _MSC_VER < 1200
	COM_INTERFACE_ENTRY(IUnknown)
#endif
	COM_INTERFACE_ENTRY_IMPL_IID(IID_IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY_IMPL_IID(IID_IOleInPlaceObject, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY_IMPL(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY_IMPL(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY_IMPL(IOleControl)
	COM_INTERFACE_ENTRY_IMPL(IOleObject)
	COM_INTERFACE_ENTRY_IMPL(IPersistStreamInit)
#if _MSC_VER >= 1200
	COM_INTERFACE_ENTRY(IObjectSafety)
#else
	COM_INTERFACE_ENTRY_IMPL(IObjectSafety)
#endif
   END_COM_MAP()

BEGIN_PROPERTY_MAP(CWrapObj)
	// In ATL 2.1, omitting this map can cause
	// trouble under certain circumstances.
END_PROPERTY_MAP()


BEGIN_MSG_MAP(CWrapObj)
	// In ATL 2.1, omitting this map can cause
	// trouble under certain circumstances.
END_MSG_MAP()


// IOleObjectImpl

	STDMETHOD(GetExtent)(DWORD dwDrawAspect, SIZEL *psizel)
	{
		// We auto-size here; we set our extent
		// to 0.

		ZeroMemory(&m_sizeExtent, sizeof(SIZE));

		// And then defer to the default implementation
		// inside ATL.

		return IOleObjectImpl<CWrapObj>::GetExtent(dwDrawAspect, psizel);
	}

// IWrapObj
public:
	HRESULT OnDraw(ATL_DRAWINFO& di);

};

#endif //__WRAPOBJ_H_
