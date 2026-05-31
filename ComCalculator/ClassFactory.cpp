#include "ClassFactory.h"
#include "CalculatorImpl.h"
#include <atomic>
#include <new>

extern std::atomic<ULONG> g_lockCount;

HRESULT __stdcall CalculatorClassFactory::QueryInterface(REFIID riid, void** ppv)
{
    if (!ppv) return E_INVALIDARG;

    if (riid == IID_IUnknown || riid == IID_IClassFactory)
    {
        *ppv = static_cast<IClassFactory*>(this);
        AddRef();
        return S_OK;
    }

    *ppv = nullptr;
    return E_NOINTERFACE;
}

ULONG __stdcall CalculatorClassFactory::AddRef()
{
    return ++m_refCount;
}

ULONG __stdcall CalculatorClassFactory::Release()
{
    ULONG count = --m_refCount;
    if (count == 0) delete this;
    return count;
}

HRESULT __stdcall CalculatorClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
    if (!ppv) return E_INVALIDARG;
    if (pUnkOuter) return CLASS_E_NOAGGREGATION;

    CalculatorImpl* pObj = new (std::nothrow) CalculatorImpl();
    if (!pObj) return E_OUTOFMEMORY;

    HRESULT hr = pObj->QueryInterface(riid, ppv);
    pObj->Release();
    return hr;
}

HRESULT __stdcall CalculatorClassFactory::LockServer(BOOL fLock)
{
    if (fLock) ++g_lockCount;
    else --g_lockCount;
    return S_OK;
}
