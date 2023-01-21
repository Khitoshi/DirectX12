#pragma once

#include <d3dx12.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class GraphicsEngine;
class VertexBuffer;
class IndexBuffer;
class RootSignature;
class PipelineState;
class DescriptorHeap;


class RenderContext
{
private:
public:
    /// <summary>
    /// デフォルト コンストラクタ
    /// </summary>
    RenderContext();

    /// <summary>
    /// デフォルト デストラクタ
    /// </summary>
    ~RenderContext();


    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="commandList">コマンドリスト</param>
    void Init(ID3D12GraphicsCommandList4* commandList);

public://set method
#pragma region Set Method
    /// <summary>
    /// 頂点バッファ 設定
    /// </summary>
    /// <param name="vertexBuffer">頂点バッファ</param>
    void SetVertexBuffer(VertexBuffer& vertexBuffer);

    /// <summary>
    /// インデックスバッファ 設定
    /// </summary>
    /// <param name="indexbuffer">インデックス バッファ</param>
    void SetIndexBuffer(IndexBuffer& indexbuffer);

    /// <summary>
    /// プリミティブトポロジー 設定
    /// </summary>
    /// <param name="topology">プリミティブ トポロジー</param>
    void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology);

    /// <summary>
    /// コマンドリスト 設定
    /// </summary>
    /// <param name="commandList">コマンド リスト</param>
    void SetCommandList(ID3D12GraphicsCommandList5* commandList);

    /// <summary>
    /// ビューポートとシザリング矩形をセットで設定
    /// </summary>
    /// <param name="viewport">ビューポート</param>
    void SetViewportAndScissor(D3D12_VIEWPORT& viewport);

    /// <summary>
    /// シザリング矩形 設定
    /// </summary>
    /// <param name="rect"></param>
    void SetScissorRect(D3D12_RECT& rect);


    /// <summary>
    /// ルートシグネチャを設定。
    /// </summary>
    void SetRootSignature(ID3D12RootSignature* rootSignature);
    void SetRootSignature(RootSignature& rootSignature);
    void SetComputeRootSignature(ID3D12RootSignature* rootSignature);
    void SetComputeRootSignature(RootSignature& rootSignature);
    
    /// <summary>
    /// パイプラインステート 設定
    /// </summary>
    /// <param name="pipelineState">パイプラインステート</param>
    void SetPipelineState(ID3D12PipelineState* pipelineState);
    void SetPipelineState(PipelineState& pipelineState);
    
    /// <summary>
    /// ディスクリプタヒープを設定。
    /// </summary>
    void SetDescriptorHeap(ID3D12DescriptorHeap* descHeap)
    {
        this->descriptor_Heap_[0] = descHeap;
        this->command_List_->SetDescriptorHeaps(1, this->descriptor_Heap_->GetAddressOf());
    }

    void SetDescriptorHeap(GraphicsEngine* graphicsEngine,DescriptorHeap& descHeap);
    void SetComputeDescriptorHeap(GraphicsEngine* graphicsEngine,DescriptorHeap& descHeap);
    /// <summary>
    /// ディスクリプタテーブルを設定。
    /// </summary>
    /// <param name="RootParameterIndex"></param>
    /// <param name="BaseDescriptor"></param>
    void SetComputeRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor);

#pragma endregion

public://get method
    D3D12_VIEWPORT GetViewport()const { return this->current_Viewport_; }

private:
    void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
    {
        this->command_List_->SetGraphicsRootDescriptorTable(RootParameterIndex, BaseDescriptor);
    }

private:
    //ディスクリプタヒープの最大数。
    enum { MAX_DESCRIPTOR_HEAP = 4 };	
    //定数バッファの最大数。増量可
    enum { MAX_CONSTANT_BUFFER = 8 };	
    //シェーダーリソースの最大数。増量可
    enum { MAX_SHADER_RESOURCE = 16 };	

private:
    //コマンド リスト
    ComPtr<ID3D12GraphicsCommandList4> command_List_;
    //現在のビューポート。
    D3D12_VIEWPORT current_Viewport_;
    //ディスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descriptor_Heap_[MAX_DESCRIPTOR_HEAP];

};