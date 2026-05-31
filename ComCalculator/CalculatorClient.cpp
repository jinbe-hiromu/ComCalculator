#include "CalculatorImpl.h"
#include <stdio.h>

#define CHECK_HRESULT(hr) \
    if (FAILED(hr)) { printf("Error: 0x%08X\n", hr); return 1; }

int main()
{
    printf("=== Calculator COM Test ===\n\n");

    // Create object
    CalculatorImpl* pCalc = new CalculatorImpl();
    if (!pCalc) { printf("Memory error\n"); return 1; }
    printf("[1] Object created (RefCount = 1)\n");

    // Get ICalculator interface
    ICalculator* pICalc = nullptr;
    HRESULT hr = pCalc->QueryInterface(IID_ICalculator, (void**)&pICalc);
    CHECK_HRESULT(hr);
    printf("[2] QueryInterface OK (RefCount = 2)\n");

    // Test Add
    int result = 0;
    hr = pICalc->Add(10, 20, &result);
    CHECK_HRESULT(hr);
    printf("[3] Add(10, 20) = %d (Expected: 30) %s\n", result, result == 30 ? "PASS" : "FAIL");

    // Test Subtract
    hr = pICalc->Subtract(50, 20, &result);
    CHECK_HRESULT(hr);
    printf("[4] Subtract(50, 20) = %d (Expected: 30) %s\n", result, result == 30 ? "PASS" : "FAIL");

    // Test Multiply
    hr = pICalc->Multiply(5, 6, &result);
    CHECK_HRESULT(hr);
    printf("[5] Multiply(5, 6) = %d (Expected: 30) %s\n", result, result == 30 ? "PASS" : "FAIL");

    // Test Divide
    hr = pICalc->Divide(60, 2, &result);
    CHECK_HRESULT(hr);
    printf("[6] Divide(60, 2) = %d (Expected: 30) %s\n", result, result == 30 ? "PASS" : "FAIL");

    // Test error case: divide by zero
    hr = pICalc->Divide(10, 0, &result);
    printf("[7] Divide(10, 0) = %s (Expected: Error) %s\n",
        FAILED(hr) ? "Error" : "Success", FAILED(hr) ? "PASS" : "FAIL");

    // Release interface
    ULONG refCount = pICalc->Release();
    printf("[8] After Release, RefCount = %u\n", refCount);

    // Release object
    refCount = pCalc->Release();
    printf("[9] Final Release, RefCount = %u (Object deleted)\n", refCount);

    printf("\n=== All tests completed ===\n");
    return 0;
}