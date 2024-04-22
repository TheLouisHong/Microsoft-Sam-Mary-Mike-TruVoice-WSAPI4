/************************************************************************
CF.cpp - Class factory code

   This includes the interface:
      IClassFactory
      IUnknown

Copyright (c) 1995-199 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <mmsystem.h>
#include <dbt.h>
#include <objbase.h>
#include <objerror.h>
#include <speech.h>
#include "audio.h"

/*************************************************************************
Classes */

class CAudioDestClassFactory : public IClassFactory {

   protected:

      ULONG          m_cRef;  // Reference count on class object

   public:

      CAudioDestClassFactory(void);
      ~CAudioDestClassFactory(void);

      // IUnknown menebers
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // SRManagerClassFactory members
      STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
      STDMETHODIMP         LockServer (BOOL);
   };

typedef CAudioDestClassFactory FAR * PCAudioDestClassFactory;

class CAudioSourceClassFactory : public IClassFactory {

   protected:

      ULONG          m_cRef;  // Reference count on class object

   public:

      CAudioSourceClassFactory(void);
      ~CAudioSourceClassFactory(void);

      // IUnknown members
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // SRManagerClassFactory members
      STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
      STDMETHODIMP         LockServer (BOOL);
   };

typedef CAudioSourceClassFactory FAR * PCAudioSourceClassFactory;




/************************************************************************
globals */

LONG        gObjectCount = -1;      // number of objects existing
LONG        gLockCount   = -1;      // number of times that this is locked
HINSTANCE   ghInstance;                // DLL library instance

/************************************************************************
*/

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpvXX)
{
   switch(fdwReason) {
      case DLL_PROCESS_ATTACH:
         ghInstance = hInst;
         break;

      case DLL_PROCESS_DETACH:
         break;
   }
   return(TRUE);
} /* End of DllMain() */


/************************************************************************
DllGetClassObject - This is the external entry point for the OLE DLL,
   It follows standard OLE interface.
*/

HRESULT FAR PASCAL DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR * ppv)
{
   if (!IsEqualCLSID (rclsid, CLSID_MyAudioDest) &&
       !IsEqualCLSID (rclsid, CLSID_MyAudioSource))
      return ResultFromScode(E_FAIL);

   // check that we can provide the interface
   if (!IsEqualIID (riid, IID_IUnknown)
   && !IsEqualIID (riid, IID_IClassFactory))
      return ResultFromScode(E_NOINTERFACE);

   // return our IClassFactory for the object
   if (IsEqualCLSID (rclsid, CLSID_MyAudioDest))
      *ppv = (LPVOID) new CAudioDestClassFactory();
   else if (IsEqualCLSID (rclsid, CLSID_MyAudioSource))
      *ppv = (LPVOID) new CAudioSourceClassFactory();

   if (NULL == *ppv) 
      return ResultFromScode(E_OUTOFMEMORY);

   // don't forget to addref the object through any interfaces we return
   ((LPUNKNOWN) *ppv)->AddRef();

   return NOERROR;
}


/************************************************************************
DllCanUnloadNow - This is standard OLE entry.
*/

STDAPI DllCanUnloadNow (void)
{
   BOOL  fAnyInstances, fAnyLocks;

   // our answer is whether there are any objects or locks.
   fAnyInstances = InterlockedIncrement(&gObjectCount);
   InterlockedDecrement(&gObjectCount);
 
   fAnyLocks = InterlockedIncrement(&gLockCount);
   InterlockedDecrement(&gLockCount);
 
   if (fAnyInstances || fAnyLocks)
       return S_FALSE;
   else {
       Sleep(0);   // try yielding to any thread that might be post
                   // InterlockedDecrement() but still mid-Release()
       return S_OK;
   }
}


/************************************************************************
ObjectDestroyed - This is called when an object gets destroyed.

Inputs
   none
Outputs
   none
*/

void PASCAL ObjectDestroyed (void)
{
   InterlockedDecrement((LONG *)&gObjectCount);
}


/************************************************************************
CAudioDestClassFactory - Class factory object.
*/

CAudioDestClassFactory::CAudioDestClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CAudioDestClassFactory::~CAudioDestClassFactory(void)
{
   return;
}


STDMETHODIMP CAudioDestClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   // Any interface on this object is the object pointer
   if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory))
      *ppv = (LPVOID) this;

   // If we assign an interface then addref
   if (NULL != *ppv) {
      ((LPUNKNOWN)*ppv)->AddRef();
      return NOERROR;
   }
   return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_ (ULONG) CAudioDestClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CAudioDestClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0L == m_cRef) 
      delete this;

   return cRefT;
}


STDMETHODIMP CAudioDestClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCAOut      pObj;
   HRESULT     hr = E_FAIL;

   *ppvObj = NULL;

   // Verify that if there is a controlling unknown that it's asking for IUnknown

   if (NULL != punkOuter)
      return ResultFromScode(CLASS_E_NOAGGREGATION);

   // Create the object, telling it a function to notify us when it's gone
   pObj = new CAOut(punkOuter, ObjectDestroyed);
   if (NULL == pObj) 
      return ResultFromScode(E_FAIL);

   if (pObj->FInit(WAVE_MAPPER, NULL, 0))
      hr = pObj->QueryInterface(riid, ppvObj);

   // Kill the object if initial creation or FInit failed
   if (FAILED(hr))
      delete pObj;
   else
      InterlockedIncrement((LONG *)&gObjectCount);

   return hr;
}


STDMETHODIMP CAudioDestClassFactory::LockServer(BOOL fLock)
{
   if (fLock) {
      InterlockedIncrement((LONG *)&gLockCount);
   }
   else {
      if (gLockCount > (-1))
         InterlockedDecrement((LONG *)&gLockCount);
      else
         return ResultFromScode(E_FAIL);
   }

   return NOERROR;
}


/************************************************************************
CAudioSourceClassFactory - Class factory object.
*/

CAudioSourceClassFactory::CAudioSourceClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CAudioSourceClassFactory::~CAudioSourceClassFactory(void)
{
   return;
}


STDMETHODIMP CAudioSourceClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
{
   *ppv = NULL;

   // Any interface on this object is the object pointer
   if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory))
      *ppv = (LPVOID) this;

   // If we assign an interface then addref
   if (NULL != *ppv) {
      ((LPUNKNOWN)*ppv)->AddRef();
      return NOERROR;
   }

   return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_ (ULONG) CAudioSourceClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CAudioSourceClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0L == m_cRef) 
      delete this;

   return cRefT;
}


STDMETHODIMP CAudioSourceClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCAIn    pObj;
   HRESULT  hr = E_FAIL;

   *ppvObj = NULL;

   // Verify that if there is a controlling unknown that it's asking for IUnknown
   if (NULL != punkOuter)
      return ResultFromScode(CLASS_E_NOAGGREGATION);

   // Create the object, telling it a function to notify us when it's gone
   pObj = new CAIn(punkOuter, ObjectDestroyed);
   if (NULL == pObj)  
      return ResultFromScode(E_FAIL);

   if (pObj->FInit(WAVE_MAPPER, NULL, 0))
      hr = pObj->QueryInterface(riid, ppvObj);

   // Kill the object if initial creation or FInit failed
   if (FAILED(hr))
      delete pObj;
   else
      InterlockedIncrement((LONG *)&gObjectCount);

   return hr;
}


STDMETHODIMP CAudioSourceClassFactory::LockServer(BOOL fLock)
{
   if (fLock) {
      InterlockedIncrement((LONG *)&gLockCount);
   }
   else {
      if (gLockCount > (-1))
         InterlockedDecrement((LONG *)&gLockCount);
      else
        return ResultFromScode(E_FAIL);
   }

   return NOERROR;
}

/* End of cf.cpp */
