#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include <Windows.h>


//デフォルト コンストラクタ
ConstantBuffer::ConstantBuffer() :
    constant_Buffer_(),
    const_Buffer_CPU_(),
    constant_Buffer_Size_(0),
    alloc_Size_(0),
    is_Valid_(false)
{
}

//ムーブコンストラクタ
ConstantBuffer::ConstantBuffer(ConstantBuffer&& cb)
{
    //cb から thisに譲渡
    //cb から thisに定数バッファの譲渡
    this->constant_Buffer_[0] = cb.constant_Buffer_[0];
    this->constant_Buffer_[1] = cb.constant_Buffer_[1];
    //cb から thisにCPUからアクセスできる定数バッファの譲渡
    this->const_Buffer_CPU_[0] = cb.const_Buffer_CPU_[0];
    this->const_Buffer_CPU_[1] = cb.const_Buffer_CPU_[1];
    //cb から 定数バッファのサイズを譲渡
    this->constant_Buffer_Size_ = cb.constant_Buffer_Size_;
    //cb から allocのサイズを譲渡
    this->alloc_Size_ = cb.alloc_Size_;
    //cb から　is_Valid_を譲渡
    this->is_Valid_ = cb.is_Valid_;

    //コピー元の削除
    //定数バッファ 削除
    cb.constant_Buffer_[0] = nullptr;
    cb.constant_Buffer_[1] = nullptr;
    //CPUからアクセスできる定数バッファ 削除
    cb.const_Buffer_CPU_[0] = nullptr;
    cb.const_Buffer_CPU_[1] = nullptr;
}

//デフォルト デストラクタ
ConstantBuffer::~ConstantBuffer()
{
}

//初期化
void ConstantBuffer::Init(GraphicsEngine* graphicsEngine, int constantBufferSize, void* srcData)
{
    //デバイス取得
    auto device = graphicsEngine->GetD3DDevice();

    //定数バッファの初期化
    this->constant_Buffer_Size_ = constantBufferSize;

    //定数バッファは256バイトアライメントが要求されるので,256の倍率に切り上げる
    this->alloc_Size_ = (constantBufferSize + 256) & 0xFFFFFF00;
    //バッファ番号
    int buffer_Index = 0;
    //ヒープ設定
    auto heap_Prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    //リソースサイズ設定
    auto resource_Desc = CD3DX12_RESOURCE_DESC::Buffer(this->alloc_Size_);

    //定数バッファ 生成
    for (auto& constant_Buffer : this->constant_Buffer_)
    {
        //生成
        HRESULT hr = device.CreateCommittedResource(
            &heap_Prop,
            D3D12_HEAP_FLAG_NONE,
            &resource_Desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&constant_Buffer)
        );

        //生成 チェック
        /*
        if (FAILED(hr))
        {
            //生成 失敗
            //メッセージボックス生成
            MessageBox(nullptr, TEXT("ConstantBuffer::Initで生成に失敗しました"), L"エラー", MB_OK);
            //プログラムを異常終了させる。
            std::abort();
        }*/

        //定数バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
        //マップ、アンマップのオーバーヘッドを軽減するためにはこのインスタンスが生きている間は行わない。
        {
            CD3DX12_RANGE read_Range(0, 0);
            constant_Buffer->Map(0, &read_Range, reinterpret_cast<void**>(&this->const_Buffer_CPU_[buffer_Index]));
        }
        if (srcData != nullptr) {
            memcpy(this->const_Buffer_CPU_[buffer_Index], srcData, this->constant_Buffer_Size_);
        }
        //現在の番号の生成が終了したのでインクリメントする
        buffer_Index++;
    }
    //利用を可能にする
    this->is_Valid_ = true;
}

//データをVRAMにコピーする
void ConstantBuffer::CopyToVRAM(GraphicsEngine* graphicsEngine, void* data)
{
    auto backBufferIndex = graphicsEngine->GetBackBufferIndex();
    memcpy(this->const_Buffer_CPU_[backBufferIndex], data, this->constant_Buffer_Size_);
}

//データをVRAMにコピーする
/*
template<class T>
void ConstantBuffer::CopyToVRAM(GraphicsEngine* graphicsEngine, T& data)
{
    CopyToVRAM(graphicsEngine, &data);
}
*/

//ディスクリプタヒープに定数バッファビューを登録
void ConstantBuffer::RegistConstantBufferView(GraphicsEngine* graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
{
    //バックバッファ数を取得
    auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
    //ディスクリプタヒープに定数バッファビューを登録
    RegistConstantBufferView(graphicsEngine, descriptorHandle, back_Buffer_Index);
}

//ディスクリプタヒープに定数バッファビューを登録
void ConstantBuffer::RegistConstantBufferView(GraphicsEngine* graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
{
    //デバイスを取得。
    auto device = graphicsEngine->GetD3DDevice();
    D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
    desc.BufferLocation = this->constant_Buffer_[bufferNo]->GetGPUVirtualAddress();
    desc.SizeInBytes = this->alloc_Size_;
    device.CreateConstantBufferView(&desc, descriptorHandle);
}

//ディスクリプタヒープに定数バッファビューを登録
D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGPUVirtualAddress(GraphicsEngine* graphicsEngine)
{
    auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
    return this->constant_Buffer_[back_Buffer_Index]->GetGPUVirtualAddress();
}