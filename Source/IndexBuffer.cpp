#include "IndexBuffer.h"

//デフォルト コンストラクタ
IndexBuffer::IndexBuffer():
    index_Buffer_(),
    index_Buffer_View_(),
    index_Count_(0),
    stride_In_Bytes_(0),
    size_In_Bytes_(0)
{
}

//デフォルト デストラクタ
IndexBuffer::~IndexBuffer()
{
}

//初期化
void IndexBuffer::Init(GraphicsEngine*& graphicsEngine,int size, int stride)
{
    //サイズを設定
    if (stride == 2)
    {
        this->size_In_Bytes_ = size * 2;
    }
    else
    {
        this->size_In_Bytes_ = size;
    }
    //ヒープの設定 取得
    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    //リソースのサイズ 取得
    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(this->size_In_Bytes_);
    

    //RESOURCE 生成
    graphicsEngine->CreateCommittedResource(
        heapProp,
        D3D12_HEAP_FLAG_NONE,
        resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        this->index_Buffer_
    );

    //生成チェック
    //if (FAILED(hr))
    //{
    //    //TODO :MYASSERTを実装する
    //    std::abort();
    //}

    //インデックスバッファのビュー 生成
    this->index_Buffer_View_.BufferLocation = this->index_Buffer_->GetGPUVirtualAddress();

    //ストライドば4byte固定
    this->stride_In_Bytes_ = 4;
    this->index_Buffer_View_.Format = DXGI_FORMAT_R32_UINT;
    this->index_Buffer_View_.SizeInBytes = this->size_In_Bytes_;

    //インデックス数を設定
    this->index_Count_ = this->size_In_Bytes_ / this->stride_In_Bytes_;
}

//インデックスデータをインデックスバッファにコピー
void IndexBuffer::Copy(uint16_t* srcIndecies)
{

    uint32_t* data = nullptr;

    //インデックスバッファにコピー(map)する
    this->index_Buffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    for (int i = 0; i < this->index_Count_; i++)
    {
        data[i] = srcIndecies[i];
    }

    //mapを解除
    this->index_Buffer_->Unmap(0, nullptr);
}

//インデックスデータをインデックスバッファにコピー
void IndexBuffer::Copy(uint32_t* srcIndecies)
{
    uint32_t* data = nullptr;

    //インデックスバッファにコピー(map)する
    this->index_Buffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    for (int i = 0; i < this->index_Count_; i++)
    {
        data[i] = srcIndecies[i];
    }

    //mapを解除
    this->index_Buffer_->Unmap(0, nullptr);

}




