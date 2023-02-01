#pragma once

#include <d3dx12.h>
#include <wrl.h>
#include "RenderTarget.h"
#include "DescriptorHeap_inline.h"
#include "ConstantBuffer.h"
#include "Texture.h"
using namespace Microsoft::WRL;

class GraphicsEngine;
class VertexBuffer;
class IndexBuffer;
class RootSignature;
class PipelineState;
class RenderTarget;
//class DescriptorHeap;


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

    /// <summary>
    /// インスタンシング描画
    /// </summary>
    /// <param name="indexCount">インデックス数</param>
    void DrawIndexed(UINT indexCount)
    {
        this->command_List_->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
    }

    /// <summary>
    /// インスタンシング描画
    /// </summary>
    /// <param name="indexCount">インデックス数</param>
    /// <param name="numInstance">インスタンス数</param>
    void DrawIndexedInstanced(UINT indexCount, UINT numInstance)
    {
        this->command_List_->DrawIndexedInstanced(indexCount, numInstance, 0, 0, 0);
    }

    void Reset(ID3D12CommandAllocator* commandAllocator, ID3D12PipelineState* pipelineState)
    {
        this->command_List_->Reset(commandAllocator, pipelineState);
        //スクラッチリソースをクリア
        this->scratch_Resource_List_.clear();
    }

    /// <summary>
    /// コマンドリストを閉じる
    /// </summary>
    void Close()
    {
        this->command_List_->Close();
    }

#pragma region Clear Method

    /// <summary>
    /// 複数枚のレンダリングターゲットをクリア。
    /// </summary>
    /// <remarks>
    /// クリアカラーはレンダリングターゲットの初期化時に指定したカラーです。
    /// </remarks>
    /// <param name="numRt">レンダリングターゲットの数</param>
    /// <param name="renderTargets">レンダリングターゲットの数</param>
    inline void ClearRenderTargetViews(
        int numRt,
        RenderTarget* renderTargets[]
    )
    {
        if (renderTargets[0]->IsExsitDepthStencilBuffer()) {
            //深度バッファがある。
            ClearDepthStencilView(renderTargets[0]->GetDSVCpuDescriptorHandle(), renderTargets[0]->GetDSVClearValue());
        }
        for (int i = 0; i < numRt; i++) {
            ClearRenderTargetView(renderTargets[i]->GetRTVCpuDescriptorHandle(), renderTargets[i]->GetRTVClearColor());
        }
    }

    /// <summary>
    /// レンダリングターゲットのクリア。
    /// </summary>
    /// <param name="rtvHandle">CPUのレンダリングターゲットビューのディスクリプタハンドル</param>
    /// <param name="clearColor">クリアカラー</param>
    void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* clearColor)
    {
        this->command_List_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    }

    /// <summary>
    /// レンダリングターゲットのクリア。
    /// </summary>
    /// <param name="renderTarget"></param>
    void ClearRenderTargetView(RenderTarget& renderTarget)
    {
        RenderTarget* rtArray[] = { &renderTarget };
        ClearRenderTargetViews(1, rtArray);
    }

    /// <summary>
    /// デプスステンシルビューをクリア
    /// </summary>
    /// <param name="renderTarget">レンダリングターゲット</param>
    /// <param name="clearValue">クリア値</param>
    void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float clearValue)
    {
        this->command_List_->ClearDepthStencilView(
            dsvHandle,
            D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
            clearValue,
            0,
            0,
            nullptr);
    }

#pragma endregion

#pragma region Wait Until Method
    /// <summary>
    /// レンダリングターゲットへの描き込み待ち
    /// </summary>
    /// <remarks>
    /// レンダリングターゲットとして使われているテクスチャをシェーダーリソースビューとして
    /// 使用したい場合は、この関数を使って描き込み完了待ちを行う必要があります
    /// </remarks>
    /// <param name="renderTarget">レンダリングターゲット</param>
    void WaitUntilFinishDrawingToRenderTargets(int numRt, RenderTarget* renderTargets[])
    {
        for (int i = 0; i < numRt; i++) {
            WaitUntilFinishDrawingToRenderTarget(*renderTargets[i]);
        }
    }
    void WaitUntilFinishDrawingToRenderTarget(RenderTarget& renderTarget)
    {
        WaitUntilFinishDrawingToRenderTarget(renderTarget.GetRenderTargetTexture().GetTexture());
    }
    void WaitUntilFinishDrawingToRenderTarget(ID3D12Resource* renderTarget)
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            renderTarget,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT);
        this->command_List_->ResourceBarrier(1, &barrier);
    }
    
    /// <summary>
    /// レンダリングターゲットとして使用可能になるまで待機
    /// </summary>
    /// <remarks>
    /// レンダリングターゲットとして設定したい場合は
    /// 本関数を使って使用可能になるまで待機する必要
    /// </remarks>
    void WaitUntilToPossibleSetRenderTargets(int numRt, RenderTarget* renderTargets[])
    {
        for (int i = 0; i < numRt; i++) {
            WaitUntilToPossibleSetRenderTarget(*renderTargets[i]);
        }
    }
    void WaitUntilToPossibleSetRenderTarget(RenderTarget& renderTarget)
    {
        WaitUntilToPossibleSetRenderTarget(renderTarget.GetRenderTargetTexture().GetTexture());
    }
    void WaitUntilToPossibleSetRenderTarget(ID3D12Resource* renderTarget)
    {
        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        this->command_List_->ResourceBarrier(1, &barrier);
    }
#pragma endregion

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
    void SetDescriptorHeap(GraphicsEngine* graphicsEngine,DescriptorHeap& descriptorHeap)
    {
        this->descriptor_Heap_[0] = descriptorHeap.GetDescriptorHeap(graphicsEngine);
        this->command_List_->SetDescriptorHeaps(1, this->descriptor_Heap_->GetAddressOf());

        //ディスクリプタテーブルに登録する。
        if (descriptorHeap.IsRegistConstantBuffer()) {
            SetGraphicsRootDescriptorTable(0, descriptorHeap.GetConstantBufferGpuDescriptorStartHandle(graphicsEngine));
        }
        if (descriptorHeap.IsRegistShaderResource()) {
            SetGraphicsRootDescriptorTable(1, descriptorHeap.GetShaderResourceGpuDescriptorStartHandle(graphicsEngine));
        }
        if (descriptorHeap.IsRegistUavResource()) {
            SetGraphicsRootDescriptorTable(2, descriptorHeap.GetUavResourceGpuDescriptorStartHandle(graphicsEngine));
        }
    }
    void SetComputeDescriptorHeap(GraphicsEngine* graphicsEngine, DescriptorHeap& descriptorHeap)
    {
        this->descriptor_Heap_[0] = descriptorHeap.GetDescriptorHeap(graphicsEngine);
        this->command_List_->SetDescriptorHeaps(1, this->descriptor_Heap_->GetAddressOf());

        //ディスクリプタテーブルに登録する。
        if (descriptorHeap.IsRegistConstantBuffer()) {
            SetComputeRootDescriptorTable(0, descriptorHeap.GetConstantBufferGpuDescriptorStartHandle(graphicsEngine));
        }
        if (descriptorHeap.IsRegistShaderResource()) {
            SetComputeRootDescriptorTable(1, descriptorHeap.GetShaderResourceGpuDescriptorStartHandle(graphicsEngine));
        }
        if (descriptorHeap.IsRegistUavResource()) {
            SetComputeRootDescriptorTable(2, descriptorHeap.GetUavResourceGpuDescriptorStartHandle(graphicsEngine));
        }
    }
    /// <summary>
    /// ディスクリプタテーブルを設定。
    /// </summary>
    /// <param name="RootParameterIndex"></param>
    /// <param name="BaseDescriptor"></param>
    void SetComputeRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor);


    /// <summary>
    /// 複数枚のレンダリングターゲットを設定する。
    /// </summary>
    /// <remarks>
    /// MRTを利用したレンダリングを行いたい場合に利用してください。
    /// </remarks>
    /// <param name="numRT">レンダリングターゲットの数</param>
    /// <param name="renderTarget">レンダリングターゲットの配列。</param>
    void SetRenderTargets(UINT numRT, RenderTarget* renderTargets[])
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtDSHandleTbl[32];
        int rtNo = 0;
        for (UINT rtNo = 0; rtNo < numRT; rtNo++) {
            rtDSHandleTbl[rtNo] = renderTargets[rtNo]->GetRTVCpuDescriptorHandle();
        }
        if (renderTargets[0]->IsExsitDepthStencilBuffer()) {
            //深度バッファがある。
            D3D12_CPU_DESCRIPTOR_HANDLE dsDS = renderTargets[0]->GetDSVCpuDescriptorHandle();
            this->command_List_->OMSetRenderTargets(numRT, rtDSHandleTbl, FALSE, &dsDS);
        }
        else {
            //深度バッファがない。
            this->command_List_->OMSetRenderTargets(numRT, rtDSHandleTbl, FALSE, nullptr);
        }
    }
    /// <summary>
    /// レンダリングターゲット 設定
    /// </summary>
    /// <param name="renderTarget"></param>
    void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle)
    {
        this->command_List_->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
    }

#pragma endregion

public://get method
    D3D12_VIEWPORT GetViewport()const { return this->current_Viewport_; }

private:
    void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
    {
        this->command_List_->SetGraphicsRootDescriptorTable(RootParameterIndex, BaseDescriptor);
    }

private:
#pragma region enum
    //ディスクリプタヒープの最大数。
    enum { MAX_DESCRIPTOR_HEAP = 4 };

    //定数バッファの最大数。増量可
    enum { MAX_CONSTANT_BUFFER = 8 };

    //シェーダーリソースの最大数。増量可
    enum { MAX_SHADER_RESOURCE = 16 };	

#pragma endregion


private:
    //コマンド リスト
    ComPtr<ID3D12GraphicsCommandList4> command_List_;
    //現在のビューポート。
    D3D12_VIEWPORT current_Viewport_;
    //ディスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descriptor_Heap_[MAX_DESCRIPTOR_HEAP];
    std::vector<ComPtr<ID3D12Resource>> scratch_Resource_List_;

    //定数バッファ
    ConstantBuffer* constant_Buffer_[MAX_CONSTANT_BUFFER];

    //シェーダーリソース
    Texture* shader_Resources_[MAX_SHADER_RESOURCE];
};