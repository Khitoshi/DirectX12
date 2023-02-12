#include "StructuredBuffer.h"
#include "GraphicsEngine.h"
#include <Windows.h>


//デフォルト コンストラクタ
StructuredBuffer::StructuredBuffer():
    buffers_On_GPU_(),
    buffers_On_CPU_(),

    element_Number_(0),
    element_Size_(0),

    is_Inited_(false)
{
}

//デフォルト デストラクタ
StructuredBuffer::~StructuredBuffer()
{
}

//初期化
void StructuredBuffer::Init(GraphicsEngine*& graphicsEngine, int elementSize, int elementNumber, void* initData)
{
    
    //エレメントサイズ 取得
    this->element_Size_ = elementSize;
    //エレメント要素数 取得
    this->element_Number_ = elementNumber;

    //バッファ番号
    int buffer_number = 0;
    //ヒープ情報
    auto heap_prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    //リソースデスクのバッファ　取得
    auto resource_desc = CD3DX12_RESOURCE_DESC::Buffer(this->element_Size_ * this->element_Number_);

    for (auto& buffer : this->buffers_On_GPU_)
    {
        //リソース 生成
        graphicsEngine->CreateCommittedResource(
            heap_prop,
            D3D12_HEAP_FLAG_NONE,
            resource_desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            buffer
        );

        ////生成　チェック
        //if (FAILED(hr))
        //{
        //    MessageBox(nullptr, TEXT("GraphicsEngine::CreateSynchronizationWithGPUObjectの生成に失敗"), TEXT("エラー"), MB_OK);
        //    std::abort();
        //}

        CD3DX12_RANGE read_range(0, 0);
        buffer->Map(0, &read_range, reinterpret_cast<void**>(this->buffers_On_CPU_[buffer_number]));
        
        if (initData != nullptr)
        {
            memcpy(this->buffers_On_CPU_[buffer_number], initData, this->element_Size_ * this->element_Number_);
        }
        buffer_number++;
    }
    this->is_Inited_ = true;
}

//SRVに登録
void StructuredBuffer::RegistShaderResourceView(GraphicsEngine*& graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNumber)
{
    //初期化チェック
    if (!this->is_Inited_)return;

    D3D12_SHADER_RESOURCE_VIEW_DESC shader_recouce_view_desc;
    //メモリブロックをゼロで埋める
    //ZeroMemory(&shader_recouce_view_desc, sizeof(shader_recouce_view_desc));
    SecureZeroMemory(&shader_recouce_view_desc, sizeof(shader_recouce_view_desc));

    //シェーダーリソースビュー 設定
    shader_recouce_view_desc.Format = DXGI_FORMAT_UNKNOWN;
    shader_recouce_view_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    shader_recouce_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shader_recouce_view_desc.Buffer.FirstElement = 0;
    shader_recouce_view_desc.Buffer.NumElements = static_cast<UINT>(this->element_Number_);
    shader_recouce_view_desc.Buffer.StructureByteStride = this->element_Size_;
    shader_recouce_view_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

    //シェーダーリソースビュー 生成
    //device.CreateShaderResourceView(
    //    this->buffers_On_GPU_[bufferNumber].Get(), 
    //    &shader_recouce_view_desc, 
    //    descriptorHandle
    //);

    graphicsEngine->CreateShaderResourceView(this->buffers_On_GPU_[bufferNumber], shader_recouce_view_desc, descriptorHandle);

}

//構造化バッファの内容更新
void StructuredBuffer::Update(GraphicsEngine*& graphicsEngine, void* data)
{
    auto back_baffer_index = graphicsEngine->GetBackBufferIndex();
    memcpy(this->buffers_On_CPU_[back_baffer_index], data, this->element_Number_ * this->element_Size_);
}

ID3D12Resource* StructuredBuffer::GetResouce(GraphicsEngine*& graphicsEngine)
{

    auto buck_buffer_index = graphicsEngine->GetBackBufferIndex();
    return this->buffers_On_GPU_[buck_buffer_index].Get();
}

