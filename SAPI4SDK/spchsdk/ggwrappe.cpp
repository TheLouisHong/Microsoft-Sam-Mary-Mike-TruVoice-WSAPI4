// Wrapper.cpp -- implementation of the DLL exports provided by the sample 
// wrapper control.

// This code is copyright (c) Microsoft Corporation, 1998.  All rights reserved.

#include "stdafx.h"
#include "resource.h"

// Define the CLSID of the wrapper control object.
// You should change this GUID, along with the one
// in the file WrapObj.rgs.  Since this is the file
// in which the CLSID will actually be defined (rather
// than referenced), we need to define "initguid.h"
// so that the GUID will be instantiated.

#include "initguid.h"
#include "Wrapper.h"

// {9DF54EF1-EE69-11D1-AA96-00C04FA34D72}

const CLSID CLSID_WrapObj = 
	{0x9DF54EF1,0xEE69,0x11d1,{0xAA,0x96,0x00,0xC0,0x4F,0xA3,0x4D,0x72}};

// The class will be instantiated through the creation of 
// objects of C++ class type CWrapObj.

#include "WrapObj.h"

// ATL requires that an object called _Module actually
// implement some of the functions in the DLL.

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_WrapObj, CWrapObj)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		_Module.Init(ObjectMap, hInstance);
		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
		_Module.Term();
	return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// registers object.  Does NOT register contents of
	// the typelib -- in this case, there isn't one.

	return _Module.RegisterServer(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer();

	return S_OK;
}


