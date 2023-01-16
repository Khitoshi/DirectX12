#include "VertexBuffer.h"

VertexBuffer::VertexBuffer():
    vertex_Buffer_(),
    vertex_Buffer_View_()
{
}

VertexBuffer::~VertexBuffer()
{
}

//初期化
void VertexBuffer::Init(GraphicsEngine* graphicsEngine, int size, int stride)
{
    //デバイス 取得
    auto device = graphicsEngine->GetD3DDevice();
    //ヒープの設定 取得
    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    //リソースのサイズ 取得
    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

    //RESOURCE 生成
    HRESULT hr = device->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertex_Buffer_)
    );
    
    //生成 チェック
    if (FAILED(hr))
    {
        //TODO:MYASSERTを実装する
        exit(-1);
    }

    //頂点バッファの名前 セット
    vertex_Buffer_->SetName(L"VertexBuffer");

    //GPUの仮想アドレス セット
    vertex_Buffer_View_.BufferLocation = vertex_Buffer_->GetGPUVirtualAddress();
    //バッファーのサイズ セット
    vertex_Buffer_View_.SizeInBytes = size;
    //各頂点エントリのサイズ セット
    vertex_Buffer_View_.StrideInBytes = stride;
}

void VertexBuffer::Copy(void* strVertices)
{
    uint8_t* date = nullptr;
    
    //map開始
    vertex_Buffer_->Map(0, nullptr, (void**)date);

    //メモリをコピー　(mapしているのでdateがvertexbufferに入る)
    memcpy(date, strVertices, this->vertex_Buffer_View_.SizeInBytes);
    
    //map解除
    this->vertex_Buffer_->Unmap(0,nullptr);
}
