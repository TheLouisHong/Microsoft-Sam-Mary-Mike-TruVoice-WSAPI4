// CWrapObj.cpp : Implementation of the object that is instantiated 
// as a control inside IE.

// This code is copyright (c) Microsoft Corporation, 1998.  All rights reserved.

#include "stdafx.h"
#include "Wrapper.h"
#include "WrapObj.h"

/////////////////////////////////////////////////////////////////////////////
// CWrapObj

HRESULT CWrapObj::OnDraw(ATL_DRAWINFO& di)
{
	// In an ordinary ATL object, this actually draws something.
	// Here, it deliberately does nothing; we don't ever want to
	// have a visual representation.

	return S_OK;
}
