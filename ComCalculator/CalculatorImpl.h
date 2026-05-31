#pragma once
// CalculatorImpl.h - ICalculator インターフェースの実装クラス

#pragma once

#include "Calculator_h.h"
#include <atomic>

// ICalculator インターフェースを実装する具体的なクラス
class CalculatorImpl : public ICalculator
{
public:
    // コンストラクタ
    CalculatorImpl();

    // デストラクタ
    ~CalculatorImpl();

    // IUnknown メソッド（すべての COM オブジェクトが実装する必要がある）
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;
    ULONG __stdcall AddRef(void) override;
    ULONG __stdcall Release(void) override;

    // ICalculator メソッド（計算機能）
    HRESULT __stdcall Add(int a, int b, int* result) override;
    HRESULT __stdcall Subtract(int a, int b, int* result) override;
    HRESULT __stdcall Multiply(int a, int b, int* result) override;
    HRESULT __stdcall Divide(int a, int b, int* result) override;

private:
    // 参照カウント（COM オブジェクトが何個の参照を持っているか）
    std::atomic<ULONG> m_refCount;
};