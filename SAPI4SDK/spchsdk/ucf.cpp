/************************************************************************
CF.cpp - Class factory code

   This includes the interface:
      IClassFactory
      IUnknown

Copyright (c) 1995-1998 by Microsoft Corporation

 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
*/

#include <windows.h>
#include <mmsystem.h>
#include <initguid.h>
#include <objbase.h>
#include <objerror.h>
#include <dsound.h>
#include <spchwrap.h>
#include "telctl.h"

/*************************************************************************
Classes */

class CYesNoClassFactory : public IClassFactory {

   protected:

      ULONG          m_cRef;  // Reference count on class object

   public:

      CYesNoClassFactory(void);
      ~CYesNoClassFactory(void);

      // IUnknown menebers
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // SRManagerClassFactory members
      STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
      STDMETHODIMP         LockServer (BOOL);
   };

typedef CYesNoClassFactory FAR * PCYesNoClassFactory;


class CExtensionClassFactory : public IClassFactory {

   protected:

      ULONG          m_cRef;  // Reference count on class object

   public:

      CExtensionClassFactory(void);
      ~CExtensionClassFactory(void);

      // IUnknown menebers
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // SRManagerClassFactory members
      STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
      STDMETHODIMP         LockServer (BOOL);
   };

typedef CExtensionClassFactory FAR * PCExtensionClassFactory;


class CPhoneNumClassFactory : public IClassFactory {

   protected:

      ULONG          m_cRef;  // Reference count on class object

   public:

      CPhoneNumClassFactory(void);
      ~CPhoneNumClassFactory(void);

      // IUnknown menebers
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // SRManagerClassFactory members
      STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
      STDMETHODIMP         LockServer (BOOL);
   };

typedef CPhoneNumClassFactory FAR * PCPhoneNumClassFactory;


class CGrammarClassFactory : public IClassFactory {

   protected:

      ULONG          m_cRef;  // Reference count on class object

   public:

      CGrammarClassFactory(void);
      ~CGrammarClassFactory(void);

      // IUnknown menebers
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // SRManagerClassFactory members
      STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
      STDMETHODIMP         LockServer (BOOL);
   };

typedef CGrammarClassFactory FAR * PCGrammarClassFactory;



class CDateClassFactory : public IClassFactory {

   protected:

      ULONG          m_cRef;  // Reference count on class object

   public:

      CDateClassFactory(void);
      ~CDateClassFactory(void);

      // IUnknown menebers
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // SRManagerClassFactory members
      STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
      STDMETHODIMP         LockServer (BOOL);
   };

typedef CDateClassFactory FAR * PCDateClassFactory;


class CTimeClassFactory : public IClassFactory {

   protected:

      ULONG          m_cRef;  // Reference count on class object

   public:

      CTimeClassFactory(void);
      ~CTimeClassFactory(void);

      // IUnknown menebers
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // SRManagerClassFactory members
      STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
      STDMETHODIMP         LockServer (BOOL);
   };

typedef CTimeClassFactory FAR * PCTimeClassFactory;




class CRecordClassFactory : public IClassFactory {

   protected:

      ULONG          m_cRef;  // Reference count on class object

   public:

      CRecordClassFactory(void);
      ~CRecordClassFactory(void);

      // IUnknown menebers
      STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      // SRManagerClassFactory members
      STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
      STDMETHODIMP         LockServer (BOOL);
   };

typedef CRecordClassFactory FAR * PCRecordClassFactory;



class CSpellingClassFactory : public IClassFactory {

    protected:
        ULONG          m_cRef;  // Reference count on class object

    public:
        CSpellingClassFactory(void);
        ~CSpellingClassFactory(void);

        // IUnknown members
        STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
        STDMETHODIMP_(ULONG) AddRef(void);
        STDMETHODIMP_(ULONG) Release(void);

        // SRManagerClassFactory members
        STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
        STDMETHODIMP         LockServer (BOOL);
};

typedef CSpellingClassFactory FAR * PCSpellingClassFactory;



class CNameClassFactory : public IClassFactory {

    protected:
        ULONG          m_cRef;  // Reference count on class object

    public:
        CNameClassFactory(void);
        ~CNameClassFactory(void);

        // IUnknown members
        STDMETHODIMP         QueryInterface (REFIID, LPVOID FAR *);
        STDMETHODIMP_(ULONG) AddRef(void);
        STDMETHODIMP_(ULONG) Release(void);

        // SRManagerClassFactory members
        STDMETHODIMP         CreateInstance (LPUNKNOWN, REFIID, LPVOID FAR *);
        STDMETHODIMP         LockServer (BOOL);
};

typedef CNameClassFactory FAR * PCNameClassFactory;


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
   if (
      !IsEqualCLSID (rclsid, CLSID_SampleYesNoControl) &&
      !IsEqualCLSID (rclsid, CLSID_SamplePhoneNumControl) &&
      !IsEqualCLSID (rclsid, CLSID_SampleGrammarControl) &&
      !IsEqualCLSID (rclsid, CLSID_SampleDateControl) &&
      !IsEqualCLSID (rclsid, CLSID_SampleTimeControl) &&
      !IsEqualCLSID (rclsid, CLSID_SampleRecordControl) &&
      !IsEqualCLSID (rclsid, CLSID_SampleSpellingControl) &&
      !IsEqualCLSID (rclsid, CLSID_SampleNameControl) &&
      !IsEqualCLSID (rclsid, CLSID_SampleExtensionControl)
      )
      return ResultFromScode(E_FAIL);

   // check that we can provide the interface
   if (!IsEqualIID (riid, IID_IUnknown)
   && !IsEqualIID (riid, IID_IClassFactory))
      return ResultFromScode(E_NOINTERFACE);

   // return our IClassFactory for the object
   if (IsEqualCLSID (rclsid, CLSID_SampleYesNoControl))
      *ppv = (LPVOID) new CYesNoClassFactory();
   else if (IsEqualCLSID (rclsid, CLSID_SamplePhoneNumControl))
      *ppv = (LPVOID) new CPhoneNumClassFactory();
   else if (IsEqualCLSID (rclsid, CLSID_SampleGrammarControl))
      *ppv = (LPVOID) new CGrammarClassFactory();
   else if (IsEqualCLSID (rclsid, CLSID_SampleDateControl))
      *ppv = (LPVOID) new CDateClassFactory();
   else if (IsEqualCLSID (rclsid, CLSID_SampleTimeControl))
      *ppv = (LPVOID) new CTimeClassFactory();
   else if (IsEqualCLSID (rclsid, CLSID_SampleRecordControl))
      *ppv = (LPVOID) new CRecordClassFactory();
   else if (IsEqualCLSID (rclsid, CLSID_SampleSpellingControl))
      *ppv = (LPVOID) new CSpellingClassFactory();
   else if (IsEqualCLSID (rclsid, CLSID_SampleNameControl))
      *ppv = (LPVOID) new CNameClassFactory();
   else if (IsEqualCLSID (rclsid, CLSID_SampleExtensionControl))
      *ppv = (LPVOID) new CExtensionClassFactory();

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
CYesNoClassFactory - Class factory object.
*/

CYesNoClassFactory::CYesNoClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CYesNoClassFactory::~CYesNoClassFactory(void)
{
   return;
}


STDMETHODIMP CYesNoClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
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


STDMETHODIMP_ (ULONG) CYesNoClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CYesNoClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0L == m_cRef) 
      delete this;

   return cRefT;
}


STDMETHODIMP CYesNoClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCYesNo        pObj;
   HRESULT     hr = E_FAIL;

   *ppvObj = NULL;

   // Verify that if there is a controlling unknown that it's asking for IUnknown

   if (NULL != punkOuter)
      return ResultFromScode(CLASS_E_NOAGGREGATION);

   // Create the object, telling it a function to notify us when it's gone
   pObj = new CYesNo;
   if (NULL == pObj) 
      return ResultFromScode(E_FAIL);

   if (!pObj->Init(&gObjectCount, ghInstance))
      hr = pObj->QueryInterface(riid, ppvObj);

   // Kill the object if initial creation or FInit failed
   if (FAILED(hr))
      delete pObj;
   //else
   //   InterlockedIncrement((LONG *)&gObjectCount);

   return hr;
}


STDMETHODIMP CYesNoClassFactory::LockServer(BOOL fLock)
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
CExtensionClassFactory - Class factory object.
*/

CExtensionClassFactory::CExtensionClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CExtensionClassFactory::~CExtensionClassFactory(void)
{
   return;
}


STDMETHODIMP CExtensionClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
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


STDMETHODIMP_ (ULONG) CExtensionClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CExtensionClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0L == m_cRef) 
      delete this;

   return cRefT;
}


STDMETHODIMP CExtensionClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCExtension        pObj;
   HRESULT     hr = E_FAIL;

   *ppvObj = NULL;

   // Verify that if there is a controlling unknown that it's asking for IUnknown

   if (NULL != punkOuter)
      return ResultFromScode(CLASS_E_NOAGGREGATION);

   // Create the object, telling it a function to notify us when it's gone
   pObj = new CExtension;
   if (NULL == pObj) 
      return ResultFromScode(E_FAIL);

   if (!pObj->Init(&gObjectCount, ghInstance))
      hr = pObj->QueryInterface(riid, ppvObj);

   // Kill the object if initial creation or FInit failed
   if (FAILED(hr))
      delete pObj;
   //else
   //   InterlockedIncrement((LONG *)&gObjectCount);

   return hr;
}


STDMETHODIMP CExtensionClassFactory::LockServer(BOOL fLock)
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
CPhoneNumClassFactory - Class factory object.
*/

CPhoneNumClassFactory::CPhoneNumClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CPhoneNumClassFactory::~CPhoneNumClassFactory(void)
{
   return;
}


STDMETHODIMP CPhoneNumClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
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


STDMETHODIMP_ (ULONG) CPhoneNumClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CPhoneNumClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0L == m_cRef) 
      delete this;

   return cRefT;
}


STDMETHODIMP CPhoneNumClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCPhoneNum        pObj;
   HRESULT     hr = E_FAIL;

   *ppvObj = NULL;

   // Verify that if there is a controlling unknown that it's asking for IUnknown

   if (NULL != punkOuter)
      return ResultFromScode(CLASS_E_NOAGGREGATION);

   // Create the object, telling it a function to notify us when it's gone
   pObj = new CPhoneNum;
   if (NULL == pObj) 
      return ResultFromScode(E_FAIL);

   if (!pObj->Init(&gObjectCount, ghInstance))
      hr = pObj->QueryInterface(riid, ppvObj);

   // Kill the object if initial creation or FInit failed
   if (FAILED(hr))
      delete pObj;
   //else
   //   InterlockedIncrement((LONG *)&gObjectCount);

   return hr;
}


STDMETHODIMP CPhoneNumClassFactory::LockServer(BOOL fLock)
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
CGrammarClassFactory - Class factory object.
*/

CGrammarClassFactory::CGrammarClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CGrammarClassFactory::~CGrammarClassFactory(void)
{
   return;
}


STDMETHODIMP CGrammarClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
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


STDMETHODIMP_ (ULONG) CGrammarClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CGrammarClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0L == m_cRef) 
      delete this;

   return cRefT;
}


STDMETHODIMP CGrammarClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCGrammar        pObj;
   HRESULT     hr = E_FAIL;

   *ppvObj = NULL;

   // Verify that if there is a controlling unknown that it's asking for IUnknown

   if (NULL != punkOuter)
      return ResultFromScode(CLASS_E_NOAGGREGATION);

   // Create the object, telling it a function to notify us when it's gone
   pObj = new CGrammar;
   if (NULL == pObj) 
      return ResultFromScode(E_FAIL);

   if (!pObj->Init(&gObjectCount, ghInstance))
      hr = pObj->QueryInterface(riid, ppvObj);

   // Kill the object if initial creation or FInit failed
   if (FAILED(hr))
      delete pObj;
   //else
   //   InterlockedIncrement((LONG *)&gObjectCount);

   return hr;
}


STDMETHODIMP CGrammarClassFactory::LockServer(BOOL fLock)
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
CDateClassFactory - Class factory object.
*/

CDateClassFactory::CDateClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CDateClassFactory::~CDateClassFactory(void)
{
   return;
}


STDMETHODIMP CDateClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
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


STDMETHODIMP_ (ULONG) CDateClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CDateClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0L == m_cRef) 
      delete this;

   return cRefT;
}


STDMETHODIMP CDateClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCDate        pObj;
   HRESULT     hr = E_FAIL;

   *ppvObj = NULL;

   // Verify that if there is a controlling unknown that it's asking for IUnknown

   if (NULL != punkOuter)
      return ResultFromScode(CLASS_E_NOAGGREGATION);

   // Create the object, telling it a function to notify us when it's gone
   pObj = new CDate;
   if (NULL == pObj) 
      return ResultFromScode(E_FAIL);

   if (!pObj->Init(&gObjectCount, ghInstance))
      hr = pObj->QueryInterface(riid, ppvObj);

   // Kill the object if initial creation or FInit failed
   if (FAILED(hr))
      delete pObj;
   //else
   //   InterlockedIncrement((LONG *)&gObjectCount);

   return hr;
}


STDMETHODIMP CDateClassFactory::LockServer(BOOL fLock)
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
CTimeClassFactory - Class factory object.
*/

CTimeClassFactory::CTimeClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CTimeClassFactory::~CTimeClassFactory(void)
{
   return;
}


STDMETHODIMP CTimeClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
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


STDMETHODIMP_ (ULONG) CTimeClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CTimeClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0L == m_cRef) 
      delete this;

   return cRefT;
}


STDMETHODIMP CTimeClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCTime        pObj;
   HRESULT     hr = E_FAIL;

   *ppvObj = NULL;

   // Verify that if there is a controlling unknown that it's asking for IUnknown

   if (NULL != punkOuter)
      return ResultFromScode(CLASS_E_NOAGGREGATION);

   // Create the object, telling it a function to notify us when it's gone
   pObj = new CTime;
   if (NULL == pObj) 
      return ResultFromScode(E_FAIL);

   if (!pObj->Init(&gObjectCount, ghInstance))
      hr = pObj->QueryInterface(riid, ppvObj);

   // Kill the object if initial creation or FInit failed
   if (FAILED(hr))
      delete pObj;
   //else
   //   InterlockedIncrement((LONG *)&gObjectCount);

   return hr;
}


STDMETHODIMP CTimeClassFactory::LockServer(BOOL fLock)
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
CRecordClassFactory - Class factory object.
*/

CRecordClassFactory::CRecordClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CRecordClassFactory::~CRecordClassFactory(void)
{
   return;
}


STDMETHODIMP CRecordClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
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


STDMETHODIMP_ (ULONG) CRecordClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CRecordClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0L == m_cRef) 
      delete this;

   return cRefT;
}


STDMETHODIMP CRecordClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCRecord        pObj;
   HRESULT     hr = E_FAIL;

   *ppvObj = NULL;

   // Verify that if there is a controlling unknown that it's asking for IUnknown

   if (NULL != punkOuter)
      return ResultFromScode(CLASS_E_NOAGGREGATION);

   // Create the object, telling it a function to notify us when it's gone
   pObj = new CRecord;
   if (NULL == pObj) 
      return ResultFromScode(E_FAIL);

   if (!pObj->Init(&gObjectCount, ghInstance))
      hr = pObj->QueryInterface(riid, ppvObj);

   // Kill the object if initial creation or FInit failed
   if (FAILED(hr))
      delete pObj;
   //else
   //   InterlockedIncrement((LONG *)&gObjectCount);

   return hr;
}


STDMETHODIMP CRecordClassFactory::LockServer(BOOL fLock)
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
CSpellingClassFactory - Class factory object.
*/

CSpellingClassFactory::CSpellingClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CSpellingClassFactory::~CSpellingClassFactory(void)
{
   return;
}


STDMETHODIMP CSpellingClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
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


STDMETHODIMP_ (ULONG) CSpellingClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CSpellingClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0L == m_cRef) 
      delete this;

   return cRefT;
}


STDMETHODIMP CSpellingClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCSpelling  pObj;
   HRESULT     hr = E_FAIL;

   *ppvObj = NULL;

   // Verify that if there is a controlling unknown that it's asking for IUnknown

   if (NULL != punkOuter)
      return ResultFromScode(CLASS_E_NOAGGREGATION);

   // Create the object, telling it a function to notify us when it's gone
   pObj = new CSpelling;
   if (NULL == pObj) 
      return ResultFromScode(E_FAIL);

   if (!pObj->Init(&gObjectCount, ghInstance))
      hr = pObj->QueryInterface(riid, ppvObj);

   // Kill the object if initial creation or FInit failed
   if (FAILED(hr))
      delete pObj;
   //else
   //   InterlockedIncrement((LONG *)&gObjectCount);

   return hr;
}


STDMETHODIMP CSpellingClassFactory::LockServer(BOOL fLock)
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
CNameClassFactory - Class factory object.
*/

CNameClassFactory::CNameClassFactory(void)
{
   m_cRef = 0L;
   return;
}


CNameClassFactory::~CNameClassFactory(void)
{
   return;
}


STDMETHODIMP CNameClassFactory::QueryInterface(REFIID riid, LPVOID FAR * ppv)
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


STDMETHODIMP_ (ULONG) CNameClassFactory::AddRef(void)
{
   return ++m_cRef;
}


STDMETHODIMP_ (ULONG) CNameClassFactory::Release(void)
{
   ULONG cRefT;

   cRefT = --m_cRef;
   if (0L == m_cRef) 
      delete this;

   return cRefT;
}


STDMETHODIMP CNameClassFactory::CreateInstance(LPUNKNOWN punkOuter, REFIID riid, LPVOID * ppvObj)
{
   PCName  pObj;
   HRESULT     hr = E_FAIL;

   *ppvObj = NULL;

   // Verify that if there is a controlling unknown that it's asking for IUnknown

   if (NULL != punkOuter)
      return ResultFromScode(CLASS_E_NOAGGREGATION);

   // Create the object, telling it a function to notify us when it's gone
   pObj = new CName;
   if (NULL == pObj) 
      return ResultFromScode(E_FAIL);

   if (!pObj->Init(&gObjectCount, ghInstance))
      hr = pObj->QueryInterface(riid, ppvObj);

   // Kill the object if initial creation or FInit failed
   if (FAILED(hr))
      delete pObj;
   //else
   //   InterlockedIncrement((LONG *)&gObjectCount);

   return hr;
}


STDMETHODIMP CNameClassFactory::LockServer(BOOL fLock)
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
