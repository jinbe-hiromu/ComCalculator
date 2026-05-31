// CalculatorImpl.cpp - CalculatorImpl クラスの実装

#include "CalculatorImpl.h"

// コンストラクタ：参照カウントを 1 で初期化
CalculatorImpl::CalculatorImpl() : m_refCount(1)
{
}

// デストラクタ：リソースの解放
CalculatorImpl::~CalculatorImpl()
{
}

// QueryInterface: 要求されたインターフェースが使用可能か確認
// 使用可能なら、そのインターフェースのポインタを返す
HRESULT __stdcall CalculatorImpl::QueryInterface(REFIID riid, void** ppvObject)
{
    if (!ppvObject)
    {
        return E_INVALIDARG;
    }

    // ICalculator インターフェースが要求された場合
    if (riid == IID_ICalculator)
    {
        *ppvObject = static_cast<ICalculator*>(this);
        AddRef();  // 参照カウントをインクリメント
        return S_OK;
    }

    // IUnknown インターフェースが要求された場合
    if (riid == IID_IUnknown)
    {
        *ppvObject = static_cast<IUnknown*>(this);
        AddRef();  // 参照カウントをインクリメント
        return S_OK;
    }

    // サポートされていないインターフェース
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

// AddRef: 参照カウントをインクリメント
// 戻り値：更新後の参照カウント
ULONG __stdcall CalculatorImpl::AddRef(void)
{
    return ++m_refCount;
}

// Release: 参照カウントをデクリメント
// 参照カウントが 0 になったら、オブジェクトを削除
ULONG __stdcall CalculatorImpl::Release(void)
{
    ULONG count = --m_refCount;

    if (count == 0)
    {
        delete this;  // 自分自身を削除
    }

    return count;
}

// Add: 2 つの数値を足す
HRESULT __stdcall CalculatorImpl::Add(int a, int b, int* result)
{
    if (!result)
    {
        return E_INVALIDARG;
    }

    *result = a + b;
    return S_OK;
}

// Subtract: 2 つの数値を引く
HRESULT __stdcall CalculatorImpl::Subtract(int a, int b, int* result)
{
    if (!result)
    {
        return E_INVALIDARG;
    }

    *result = a - b;
    return S_OK;
}

// Multiply: 2 つの数値を掛ける
HRESULT __stdcall CalculatorImpl::Multiply(int a, int b, int* result)
{
    if (!result)
    {
        return E_INVALIDARG;
    }

    *result = a * b;
    return S_OK;
}

// Divide: 2 つの数値を割る（ゼロ除算チェック付き）
HRESULT __stdcall CalculatorImpl::Divide(int a, int b, int* result)
{
    if (!result)
    {
        return E_INVALIDARG;
    }

    // ゼロで割られないかチェック
    if (b == 0)
    {
        return E_INVALIDARG;  // ゼロで割るのは無効
    }

    *result = a / b;
    return S_OK;
}