#include <windows.h>
#include <atomic>
#include <new>
#include "ClassFactory.h"

// g_hModule: DLL itself handle saved in DllMain, used to get DLL path in DllRegisterServer
static HMODULE g_hModule = NULL;

// g_lockCount: lock count for DllCanUnloadNow / IClassFactory::LockServer
std::atomic<ULONG> g_lockCount{ 0 };

extern "C" const CLSID CLSID_Calculator;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        g_hModule = hModule;
    return TRUE;
}

extern "C"
{
    // Called by regsvr32: registers CLSID under HKCR\CLSID
    HRESULT __stdcall DllRegisterServer(void)
    {
        wchar_t szModulePath[MAX_PATH];
        // Pass g_hModule (the DLL itself), not NULL (which would give regsvr32.exe path)
        GetModuleFileNameW(g_hModule, szModulePath, MAX_PATH);

        HKEY hKey = NULL;
        LONG lResult = RegCreateKeyExW(HKEY_CLASSES_ROOT,
            L"CLSID\\{87654321-4321-4321-4321-098765432100}",
            0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

        if (lResult != ERROR_SUCCESS)
            return HRESULT_FROM_WIN32(lResult);

        RegSetValueExW(hKey, NULL, 0, REG_SZ,
            (const BYTE*)L"Calculator Class",
            (DWORD)(wcslen(L"Calculator Class") + 1) * sizeof(wchar_t));

        HKEY hInproc = NULL;
        if (RegCreateKeyExW(hKey, L"InprocServer32", 0, NULL,
            REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hInproc, NULL) == ERROR_SUCCESS)
        {
            RegSetValueExW(hInproc, NULL, 0, REG_SZ,
                (const BYTE*)szModulePath,
                (DWORD)(wcslen(szModulePath) + 1) * sizeof(wchar_t));
            RegSetValueExW(hInproc, L"ThreadingModel", 0, REG_SZ,
                (const BYTE*)L"Apartment",
                (DWORD)(wcslen(L"Apartment") + 1) * sizeof(wchar_t));
            RegCloseKey(hInproc);
        }
        RegCloseKey(hKey);
        return S_OK;
    }

    // Called by regsvr32 /u: removes CLSID entries from registry
    HRESULT __stdcall DllUnregisterServer(void)
    {
        // Delete child key before parent
        RegDeleteKeyW(HKEY_CLASSES_ROOT,
            L"CLSID\\{87654321-4321-4321-4321-098765432100}\\InprocServer32");
        RegDeleteKeyW(HKEY_CLASSES_ROOT,
            L"CLSID\\{87654321-4321-4321-4321-098765432100}");
        return S_OK;
    }

    // Called by CoCreateInstance / CoGetClassObject
    HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
    {
        if (!ppv) return E_INVALIDARG;
        if (!IsEqualCLSID(rclsid, CLSID_Calculator))
            return CLASS_E_CLASSNOTAVAILABLE;

        CalculatorClassFactory* pFactory = new (std::nothrow) CalculatorClassFactory();
        if (!pFactory) return E_OUTOFMEMORY;

        HRESULT hr = pFactory->QueryInterface(riid, ppv);
        pFactory->Release();
        return hr;
    }

    // Called by COM runtime to check if DLL can be unloaded
    HRESULT __stdcall DllCanUnloadNow(void)
    {
        return (g_lockCount == 0) ? S_OK : S_FALSE;
    }
}
