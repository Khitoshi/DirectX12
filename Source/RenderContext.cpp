#include "RenderContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "DescriptorHeap.h"
#include "GraphicsEngine.h"

//デフォルト コンストラクタ
RenderContext::RenderContext():
    command_List_(),
    current_Viewport_(),
    descriptor_Heap_(),
    scratch_Resource_List_(),
    constant_Buffer_(),
    shader_Resources_()
{
}

//デフォルト デストラクタ
RenderContext::~RenderContext()
{
    for (auto& dh : descriptor_Heap_)
    {
        if(dh)dh->Release();
    }

    if (command_List_)command_List_->Release();
    

    for (auto& resouce : shader_Resources_)
    {
        if (resouce)delete resouce;
    }

    for (auto& buffer : constant_Buffer_)
    {
        if (buffer)delete buffer;
    }

}

//初期化
void RenderContext::Init(ID3D12GraphicsCommandList4* commandList)
{
    this->command_List_ = commandList;
}

//頂点バッファ 設定
void RenderContext::SetVertexBuffer(VertexBuffer& vertexBuffer)
{
    //頂点バッファビューに情報を セット
    this->command_List_->IASetVertexBuffers(0, 1, &vertexBuffer.GetVertexBufferView());
}

//インデックスバッファを設定
void RenderContext::SetIndexBuffer(IndexBuffer& indexBuffer)
{
    this->command_List_->IASetIndexBuffer(&indexBuffer.GetIndexBufferView());
}

//プリミティブのトポロジー 設定
void RenderContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
    this->command_List_->IASetPrimitiveTopology(topology);
}

//コマンドリスト 設定
void RenderContext::SetCommandList(ID3D12GraphicsCommandList5* commandList)
{
    this->command_List_ = commandList;
}

void RenderContext::SetViewportAndScissor(D3D12_VIEWPORT& viewport)
{
    //シザリング矩形
    D3D12_RECT scissorRect;
    scissorRect.bottom = static_cast<LONG>(viewport.Height);
    scissorRect.top = 0;
    scissorRect.left = 0;
    scissorRect.right = static_cast<LONG>(viewport.Width);
    SetScissorRect(scissorRect);

    this->command_List_->RSSetViewports(1, &viewport);
    this->current_Viewport_ = viewport;

}

//シザリング矩形 設定
void RenderContext::SetScissorRect(D3D12_RECT& rect)
{
    this->command_List_->RSSetScissorRects(1, &rect);
}


//ルートシグネチャ　設定
void RenderContext::SetRootSignature(ID3D12RootSignature* rootSignature)
{
    this->command_List_->SetGraphicsRootSignature(rootSignature);
}

//ルートシグネチャ　設定
void RenderContext::SetRootSignature(RootSignature& rootSignature)
{
    this->command_List_->SetGraphicsRootSignature(rootSignature.GetRootSignature());
}

//ルートシグネチャ　設定
void RenderContext::SetComputeRootSignature(ID3D12RootSignature* rootSignature)
{
    this->command_List_->SetComputeRootSignature(rootSignature);
}

//ルートシグネチャ　設定
void RenderContext::SetComputeRootSignature(RootSignature& rootSignature)
{
    this->command_List_->SetComputeRootSignature(rootSignature.GetRootSignature());
}

//パイプラインステート 設定
void RenderContext::SetPipelineState(ID3D12PipelineState* pipelineState)
{
    this->command_List_->SetPipelineState(pipelineState);
}

//パイプラインステート 設定
void RenderContext::SetPipelineState(PipelineState& pipelineState)
{
    this->command_List_->SetPipelineState(pipelineState.GetPipelineState());
}

void RenderContext::SetComputeRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
{
    this->command_List_->SetComputeRootDescriptorTable(RootParameterIndex, BaseDescriptor);
}
