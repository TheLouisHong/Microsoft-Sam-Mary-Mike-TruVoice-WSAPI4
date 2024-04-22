/************************************************************************
FakeTTSC.Cpp - Fake TTS class factory.

   This includes the interface:
      IClassFactory
      IUnknown

Copyright (c) 1994-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <mmsystem.h>

#include <objbase.h>
#include <objerror.h>
#include <speech.h>
#include "fakeTTS.h"
#include "fakeTTSE.h"


/************************************************************************
globals */

DWORD       gEngObjectCount = 0;       // number of objects existing
   // This includes mode object, enumeration object
DWORD       gEngLockCount   = 0;       // number of times that this is locked
HINSTANCE   ghInstance;                // DLL library instance

/************************************************************************
*/

BOOL WINAPI DllMain (HINSTANCE hInst, DWORD fdwReason, LPVOID lpvXX)
{
   switch(fdwReason)
      {
      case DLL_PROCESS_ATTACH:

         ghInstance = hInst;
         break;

      case DLL_PROCESS_DETACH:

         break;
      }

   return(TRUE);
}


/************************************************************************
DllGetClassObject - This is the external entry point for the OLE DLL,
   It follows standard OLE interface.
*/

HRESULT FAR PASCAL DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR * ppv)
{
   if (!IsEqualCLSID (rclsid, CLSID_FakeTTSEngineEnum))

      return ResultFromScode(E_FAIL);

// check that we can provide the interface

   if (!IsEqualIID (riid, IID_IUnknown)
    && !IsEqualIID (riid, IID_IClassFactory))

      return ResultFromScode(E_NOINTERFACE);

// return our IClassFactory for the Manager object

   *ppv = (LPVOID) new CEngineClassFactory();

   if (NULL==*ppv) return ResultFromScode(E_OUTOFMEMORY);

// dont forget to addred the object through any interfaces we return

   ((LPUNKNOWN) *ppv)->AddRef();

   return NOERROR;
}


/************************************************************************
DllCanUnloadNow - This is standard OLE entry.
*/

STDAPI DllCanUnloadNow (void)
{
   SCODE sc;

// our answer is whether there are any objects or locks.

   sc = (gEngObjectCount == 0 && gEngLockCount == 0) ? S_OK : S_FALSE;

   return ResultFromScode (sc);
}


/************************************************************************
EngObjectDestroyed - This is called when an object gets destroyed.

Inputs
   none
Outputs
   none
*/

void PASCAL EngObjectDestroyed (void)
{
   gEngObjectCount--;
}


/************************************************************************
CEngineClassFactory - Class factory object for the engine.
*/

CEngineClassFactory::CEngineClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CEngineClassFactory::~CEngineClassFactory(void)
{
   return;
}


STDMETHODIMP CEngineClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

// Any intercae on this object is the obvject pointer

   if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory))

      *ppv = (LPVOID) this;

// If we assign an interface then addrred

   if (NULL != *ppv)
      {
      ((LPUNKNOWN)*ppv)->AddRef();
      return NOERROR;
      }

   return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_ (ULONG) CEngineClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CEngineClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;

   if (0L == m_cRef) delete this;

   return cRefT;
}


STDMETHODIMP CEngineClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCEngineEnum   pObj;
   HRESULT        hr;

   *ppvObj = NULL;
   hr      = ResultFromScode(E_OUTOFMEMORY);

// Verify that if there is a controlling unknown that it's asking for IUnknown

   if (NULL != punkOuter)

      return ResultFromScode(CLASS_E_NOAGGREGATION);

// Create the object, telling it a function to notify us when it's gone

   pObj = new CEngineEnum(punkOuter, EngObjectDestroyed);

   if (NULL == pObj)  return hr;
   if (pObj->FInit()) hr = pObj->QueryInterface(riid, ppvObj);

// Kill the object if initial creation or FInit failed

   if (FAILED(hr))

      delete pObj;

   else
		{
      gEngObjectCount++;
		}

   return hr;
}


STDMETHODIMP CEngineClassFactory::LockServer(BOOL fLock)
{
   if (fLock)
		{
      gEngLockCount++;
		}
   else
		{
      if (gEngLockCount > 0)

         gEngLockCount--;

      else

        return ResultFromScode(E_FAIL);

		}

   return NOERROR;
}
