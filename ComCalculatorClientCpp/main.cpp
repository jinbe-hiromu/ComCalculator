// main.cpp - C++ クライアント
// CoCreateInstance で COM サーバーを呼び出す

#include <windows.h>
#include <stdio.h>

// COM サーバーのインターフェース定義をインクルード
#include "..\ComCalculator\Calculator_h.h"
#include "..\ComCalculator\Calculator_i.c"

int main()
{
    printf("=== C++ Client - CoCreateInstance Test ===\n\n");

    // COM ランタイムを初期化
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        printf("CoInitialize failed: 0x%08X\n", hr);
        return 1;
    }
    printf("[1] CoInitialize OK\n");

    // レジストリから COM オブジェクトを生成
    ICalculator* pCalc = nullptr;
    hr = CoCreateInstance(
        CLSID_Calculator,       // レジストリで登録した CLSID
        nullptr,
        CLSCTX_INPROC_SERVER,  // 同一プロセス内 DLL
        IID_ICalculator,        // 取得するインターフェース
        (void**)&pCalc
    );

    if (FAILED(hr))
    {
        printf("CoCreateInstance failed: 0x%08X\n", hr);
        CoUninitialize();
        return 1;
    }
    printf("[2] CoCreateInstance OK\n");
    printf("    -> レジストリから DLL を探して生成しました\n\n");

    // 計算テスト
    int result = 0;

    pCalc->Add(10, 20, &result);
    printf("[3] Add(10, 20)      = %d  %s\n", result, result == 30 ? "PASS" : "FAIL");

    pCalc->Subtract(50, 20, &result);
    printf("[4] Subtract(50, 20) = %d  %s\n", result, result == 30 ? "PASS" : "FAIL");

    pCalc->Multiply(5, 6, &result);
    printf("[5] Multiply(5, 6)   = %d  %s\n", result, result == 30 ? "PASS" : "FAIL");

    pCalc->Divide(60, 2, &result);
    printf("[6] Divide(60, 2)    = %d  %s\n", result, result == 30 ? "PASS" : "FAIL");

    // 解放
    pCalc->Release();
    printf("\n[7] Release OK\n");

    CoUninitialize();
    printf("[8] CoUninitialize OK\n");

    printf("\n=== Test Complete ===\n");
    return 0;
}