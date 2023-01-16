#pragma once

#include <d3dx12.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class VertexBuffer;
class IndexBuffer;
class RootSignature;
class PipelineState;

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
    void SetPipelineState(PipelineState* pipelineState);
    
    /// <summary>
    /// ディスクリプタヒープ 設定
    /// </summary>
    /// <param name="descriptorHeap">ディスクリプタヒープ</param>
    void SetDescriptorHeap(ID3D12DescriptorHeap* descriptorHeap);

    //TODO:　ここまでできた。次回はDescriptorHeap.h作成する

public://get method
    D3D12_VIEWPORT GetViewport()const { return this->current_Viewport; }

private:
    //ディスクリプタヒープの最大数。
    enum { MAX_DESCRIPTOR_HEAP = 4 };	
    //定数バッファの最大数。増量可
    enum { MAX_CONSTANT_BUFFER = 8 };	
    //シェーダーリソースの最大数。増量可
    enum { MAX_SHADER_RESOURCE = 16 };	

private:
    //コマンド リスト
    ComPtr<ID3D12GraphicsCommandList4> command_List;
    //現在のビューポート。
    D3D12_VIEWPORT current_Viewport;
    //ディスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descriptor_Heap_[MAX_DESCRIPTOR_HEAP];

};