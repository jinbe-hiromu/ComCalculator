# Stage 1: 最小限の COM サーバー構築 - 学習記録

## 🎯 Stage 1 で達成したこと

最初の COM サーバーを完全に実装し、テストに成功した。

### テスト結果
```
[1] Object created (RefCount = 1)         ✓
[2] QueryInterface OK (RefCount = 2)      ✓
[3] Add(10, 20) = 30                      ✓ PASS
[4] Subtract(50, 20) = 30                 ✓ PASS
[5] Multiply(5, 6) = 30                   ✓ PASS
[6] Divide(60, 2) = 30                    ✓ PASS
[7] Divide(10, 0) = Error                 ✓ PASS (Error handling)
[8] After Release, RefCount = 1           ✓
[9] Final Release, RefCount = 0 (deleted) ✓
```

---

## 📚 重要な学習ポイント

### 1. IDL ファイルと MIDL コンパイル

**IDL（Interface Definition Language）**
- COM インターフェースを定義する言語
- 言語中立的（C++, C#, VB.NET など対応）
- MIDL コンパイラで自動的に C++ コード生成

**MIDL コンパイル流れ**
```
Calculator.idl
    ↓ MIDL コンパイル
    ├→ Calculator_h.h    (インターフェース定義)
    ├→ Calculator_i.c    (GUID 定義)
    └→ Calculator_p.c    (プロキシ/スタブ)
```

### 2. __stdcall について

**重要：IDL をコンパイルすると、自動的に `__stdcall` になる**

```cpp
// IDL では指定しない
[object, uuid(...)]
interface ICalculator : IUnknown
{
    HRESULT Add([in] int a, [in] int b, [out, retval] int *result);
};

// MIDL が自動生成する時に __stdcall を挿入
virtual HRESULT __stdcall Add(...) = 0;
```

**理由：**
- 複数言語からの呼び出しに対応
- マーシャリング（別プロセス・別マシン）に対応
- 32bit ↔ 64bit の一貫性

### 3. IUnknown インターフェース

すべての COM オブジェクトが実装する必須メソッド

#### QueryInterface
```cpp
HRESULT QueryInterface(REFIID riid, void **ppvObject)
```
- 要求されたインターフェースが利用可能か確認
- 利用可能なら、そのインターフェースのポインタを返す
- 例：`IID_ICalculator` → `ICalculator *` を返す

#### AddRef
```cpp
ULONG AddRef()
```
- 参照カウントをインクリメント
- オブジェクトが「参照されている」ことを記録
- 戻り値：更新後の参照カウント

#### Release
```cpp
ULONG Release()
```
- 参照カウントをデクリメント
- 参照カウントが 0 になったら `delete this` でオブジェクト削除
- 戻り値：更新後の参照カウント

### 4. 参照カウント管理

COM オブジェクトのライフサイクル

```
1. new CalculatorImpl()        → RefCount = 1
2. QueryInterface() 成功       → RefCount = 2（AddRef() 呼ばれる）
3. Release()                  → RefCount = 1
4. Release()                  → RefCount = 0 → delete this
```

**重要ルール：**
- `new` したら最初は RefCount = 1
- インターフェースを取得するたびに `AddRef()` を呼ぶ
- 不要になったら `Release()` を呼ぶ
- RefCount が 0 になったら自動削除

### 5. CalculatorImpl の実装パターン

```cpp
class CalculatorImpl : public ICalculator
{
private:
    std::atomic<ULONG> m_refCount;

public:
    // IUnknown メソッド
    HRESULT QueryInterface(REFIID riid, void **ppvObject) override;
    ULONG AddRef() override;
    ULONG Release() override;

    // ICalculator メソッド
    HRESULT Add(int a, int b, int *result) override;
    // ...
};
```

### 6. エラーハンドリング

すべてのメソッドが `HRESULT` を返す

```cpp
HRESULT Divide(int a, int b, int *result)
{
    if (!result) return E_INVALIDARG;           // null チェック
    if (b == 0) return E_INVALIDARG;            // ゼロ除算チェック
    *result = a / b;
    return S_OK;                                // 成功
}
```

**よく使う値：**
- `S_OK` (0x00000000) - 成功
- `E_INVALIDARG` - 無効な引数
- `E_NOINTERFACE` - インターフェース未対応
- `E_OUTOFMEMORY` - メモリ不足

---

## 🔑 キーポイント

| 項目 | 学んだこと |
|------|-----------|
| **IDL** | インターフェース仕様を言語中立的に定義 |
| **MIDL** | IDL → C++ コード自動生成 |
| **__stdcall** | MIDL が自動挿入、多言語対応・マーシャリング対応 |
| **IUnknown** | すべての COM の基本、参照カウント管理 |
| **QueryInterface** | インターフェース取得の安全な方法 |
| **参照カウント** | COM オブジェクトのライフサイクル管理 |
| **HRESULT** | エラーハンドリング |

---

## 📖 コード構成

```
Calculator.idl
  ↓ MIDL コンパイル
  ├→ Calculator_h.h (自動生成)
  ├→ Calculator_i.c (自動生成)
  └→ Calculator_p.c (自動生成)
  
CalculatorImpl.h
  → ICalculator を継承した具体実装クラス

CalculatorImpl.cpp
  → QueryInterface, AddRef, Release 実装
  → Add, Subtract, Multiply, Divide 実装

CalculatorClient.cpp
  → テストコード
```

---

## 🚀 次のステップ（Stage 2）

- [ ] サーバーをレジストリに登録
- [ ] CoCreateInstance() での動的生成
- [ ] 32bit / 64bit 対応確認

---

## 📝 注記

- 参照カウントはスレッドセーフに実装（std::atomic）
- すべてのエラーケースをチェック（null ポインタ、ゼロ除算など）
- HRESULT は必ず呼び出し側で SUCCEEDED() / FAILED() でチェック
