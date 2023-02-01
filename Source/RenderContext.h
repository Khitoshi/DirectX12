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
    /// �f�t�H���g �R���X�g���N�^
    /// </summary>
    RenderContext();

    /// <summary>
    /// �f�t�H���g �f�X�g���N�^
    /// </summary>
    ~RenderContext();

    /// <summary>
    /// ������
    /// </summary>
    /// <param name="commandList">�R�}���h���X�g</param>
    void Init(ID3D12GraphicsCommandList4* commandList);

    /// <summary>
    /// �C���X�^���V���O�`��
    /// </summary>
    /// <param name="indexCount">�C���f�b�N�X��</param>
    void DrawIndexed(UINT indexCount)
    {
        this->command_List_->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
    }

    /// <summary>
    /// �C���X�^���V���O�`��
    /// </summary>
    /// <param name="indexCount">�C���f�b�N�X��</param>
    /// <param name="numInstance">�C���X�^���X��</param>
    void DrawIndexedInstanced(UINT indexCount, UINT numInstance)
    {
        this->command_List_->DrawIndexedInstanced(indexCount, numInstance, 0, 0, 0);
    }

    void Reset(ID3D12CommandAllocator* commandAllocator, ID3D12PipelineState* pipelineState)
    {
        this->command_List_->Reset(commandAllocator, pipelineState);
        //�X�N���b�`���\�[�X���N���A
        this->scratch_Resource_List_.clear();
    }

    /// <summary>
    /// �R�}���h���X�g�����
    /// </summary>
    void Close()
    {
        this->command_List_->Close();
    }

#pragma region Clear Method

    /// <summary>
    /// �������̃����_�����O�^�[�Q�b�g���N���A�B
    /// </summary>
    /// <remarks>
    /// �N���A�J���[�̓����_�����O�^�[�Q�b�g�̏��������Ɏw�肵���J���[�ł��B
    /// </remarks>
    /// <param name="numRt">�����_�����O�^�[�Q�b�g�̐�</param>
    /// <param name="renderTargets">�����_�����O�^�[�Q�b�g�̐�</param>
    inline void ClearRenderTargetViews(
        int numRt,
        RenderTarget* renderTargets[]
    )
    {
        if (renderTargets[0]->IsExsitDepthStencilBuffer()) {
            //�[�x�o�b�t�@������B
            ClearDepthStencilView(renderTargets[0]->GetDSVCpuDescriptorHandle(), renderTargets[0]->GetDSVClearValue());
        }
        for (int i = 0; i < numRt; i++) {
            ClearRenderTargetView(renderTargets[i]->GetRTVCpuDescriptorHandle(), renderTargets[i]->GetRTVClearColor());
        }
    }

    /// <summary>
    /// �����_�����O�^�[�Q�b�g�̃N���A�B
    /// </summary>
    /// <param name="rtvHandle">CPU�̃����_�����O�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�n���h��</param>
    /// <param name="clearColor">�N���A�J���[</param>
    void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* clearColor)
    {
        this->command_List_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    }

    /// <summary>
    /// �����_�����O�^�[�Q�b�g�̃N���A�B
    /// </summary>
    /// <param name="renderTarget"></param>
    void ClearRenderTargetView(RenderTarget& renderTarget)
    {
        RenderTarget* rtArray[] = { &renderTarget };
        ClearRenderTargetViews(1, rtArray);
    }

    /// <summary>
    /// �f�v�X�X�e���V���r���[���N���A
    /// </summary>
    /// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
    /// <param name="clearValue">�N���A�l</param>
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
    /// �����_�����O�^�[�Q�b�g�ւ̕`�����ݑ҂�
    /// </summary>
    /// <remarks>
    /// �����_�����O�^�[�Q�b�g�Ƃ��Ďg���Ă���e�N�X�`�����V�F�[�_�[���\�[�X�r���[�Ƃ���
    /// �g�p�������ꍇ�́A���̊֐����g���ĕ`�����݊����҂����s���K�v������܂�
    /// </remarks>
    /// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
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
    /// �����_�����O�^�[�Q�b�g�Ƃ��Ďg�p�\�ɂȂ�܂őҋ@
    /// </summary>
    /// <remarks>
    /// �����_�����O�^�[�Q�b�g�Ƃ��Đݒ肵�����ꍇ��
    /// �{�֐����g���Ďg�p�\�ɂȂ�܂őҋ@����K�v
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
    /// ���_�o�b�t�@ �ݒ�
    /// </summary>
    /// <param name="vertexBuffer">���_�o�b�t�@</param>
    void SetVertexBuffer(VertexBuffer& vertexBuffer);

    /// <summary>
    /// �C���f�b�N�X�o�b�t�@ �ݒ�
    /// </summary>
    /// <param name="indexbuffer">�C���f�b�N�X �o�b�t�@</param>
    void SetIndexBuffer(IndexBuffer& indexbuffer);

    /// <summary>
    /// �v���~�e�B�u�g�|���W�[ �ݒ�
    /// </summary>
    /// <param name="topology">�v���~�e�B�u �g�|���W�[</param>
    void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology);

    /// <summary>
    /// �R�}���h���X�g �ݒ�
    /// </summary>
    /// <param name="commandList">�R�}���h ���X�g</param>
    void SetCommandList(ID3D12GraphicsCommandList5* commandList);

    /// <summary>
    /// �r���[�|�[�g�ƃV�U�����O��`���Z�b�g�Őݒ�
    /// </summary>
    /// <param name="viewport">�r���[�|�[�g</param>
    void SetViewportAndScissor(D3D12_VIEWPORT& viewport);

    /// <summary>
    /// �V�U�����O��` �ݒ�
    /// </summary>
    /// <param name="rect"></param>
    void SetScissorRect(D3D12_RECT& rect);


    /// <summary>
    /// ���[�g�V�O�l�`����ݒ�B
    /// </summary>
    void SetRootSignature(ID3D12RootSignature* rootSignature);
    void SetRootSignature(RootSignature& rootSignature);
    void SetComputeRootSignature(ID3D12RootSignature* rootSignature);
    void SetComputeRootSignature(RootSignature& rootSignature);
    
    /// <summary>
    /// �p�C�v���C���X�e�[�g �ݒ�
    /// </summary>
    /// <param name="pipelineState">�p�C�v���C���X�e�[�g</param>
    void SetPipelineState(ID3D12PipelineState* pipelineState);
    void SetPipelineState(PipelineState& pipelineState);
    
    /// <summary>
    /// �f�B�X�N���v�^�q�[�v��ݒ�B
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

        //�f�B�X�N���v�^�e�[�u���ɓo�^����B
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

        //�f�B�X�N���v�^�e�[�u���ɓo�^����B
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
    /// �f�B�X�N���v�^�e�[�u����ݒ�B
    /// </summary>
    /// <param name="RootParameterIndex"></param>
    /// <param name="BaseDescriptor"></param>
    void SetComputeRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor);


    /// <summary>
    /// �������̃����_�����O�^�[�Q�b�g��ݒ肷��B
    /// </summary>
    /// <remarks>
    /// MRT�𗘗p���������_�����O���s�������ꍇ�ɗ��p���Ă��������B
    /// </remarks>
    /// <param name="numRT">�����_�����O�^�[�Q�b�g�̐�</param>
    /// <param name="renderTarget">�����_�����O�^�[�Q�b�g�̔z��B</param>
    void SetRenderTargets(UINT numRT, RenderTarget* renderTargets[])
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtDSHandleTbl[32];
        int rtNo = 0;
        for (UINT rtNo = 0; rtNo < numRT; rtNo++) {
            rtDSHandleTbl[rtNo] = renderTargets[rtNo]->GetRTVCpuDescriptorHandle();
        }
        if (renderTargets[0]->IsExsitDepthStencilBuffer()) {
            //�[�x�o�b�t�@������B
            D3D12_CPU_DESCRIPTOR_HANDLE dsDS = renderTargets[0]->GetDSVCpuDescriptorHandle();
            this->command_List_->OMSetRenderTargets(numRT, rtDSHandleTbl, FALSE, &dsDS);
        }
        else {
            //�[�x�o�b�t�@���Ȃ��B
            this->command_List_->OMSetRenderTargets(numRT, rtDSHandleTbl, FALSE, nullptr);
        }
    }
    /// <summary>
    /// �����_�����O�^�[�Q�b�g �ݒ�
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
    //�f�B�X�N���v�^�q�[�v�̍ő吔�B
    enum { MAX_DESCRIPTOR_HEAP = 4 };

    //�萔�o�b�t�@�̍ő吔�B���ʉ�
    enum { MAX_CONSTANT_BUFFER = 8 };

    //�V�F�[�_�[���\�[�X�̍ő吔�B���ʉ�
    enum { MAX_SHADER_RESOURCE = 16 };	

#pragma endregion


private:
    //�R�}���h ���X�g
    ComPtr<ID3D12GraphicsCommandList4> command_List_;
    //���݂̃r���[�|�[�g�B
    D3D12_VIEWPORT current_Viewport_;
    //�f�B�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descriptor_Heap_[MAX_DESCRIPTOR_HEAP];
    std::vector<ComPtr<ID3D12Resource>> scratch_Resource_List_;

    //�萔�o�b�t�@
    ConstantBuffer* constant_Buffer_[MAX_CONSTANT_BUFFER];

    //�V�F�[�_�[���\�[�X
    Texture* shader_Resources_[MAX_SHADER_RESOURCE];
};