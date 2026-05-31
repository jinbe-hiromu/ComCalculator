#pragma once
#include <windows.h>
#include <objbase.h>
#include "Calculator_h.h"
#include <atomic>

class CalculatorClassFactory : public IClassFactory
{
public:
    // IUnknown
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

    // IClassFactory
    HRESULT __stdcall CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv) override;
    HRESULT __stdcall LockServer(BOOL fLock) override;

private:
    std::atomic<ULONG> m_refCount{ 1 };
};
