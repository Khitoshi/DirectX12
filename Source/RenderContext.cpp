#include "RenderContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "DescriptorHeap.h"
#include "GraphicsEngine.h"

//�f�t�H���g �R���X�g���N�^
RenderContext::RenderContext():
    command_List_(),
    current_Viewport_(),
    descriptor_Heap_(),
    scratch_Resource_List_(),
    constant_Buffer_(),
    shader_Resources_()
{
}

//�f�t�H���g �f�X�g���N�^
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

//������
void RenderContext::Init(ID3D12GraphicsCommandList4* commandList)
{
    this->command_List_ = commandList;
}

//���_�o�b�t�@ �ݒ�
void RenderContext::SetVertexBuffer(VertexBuffer& vertexBuffer)
{
    //���_�o�b�t�@�r���[�ɏ��� �Z�b�g
    this->command_List_->IASetVertexBuffers(0, 1, &vertexBuffer.GetVertexBufferView());
}

//�C���f�b�N�X�o�b�t�@��ݒ�
void RenderContext::SetIndexBuffer(IndexBuffer& indexBuffer)
{
    this->command_List_->IASetIndexBuffer(&indexBuffer.GetIndexBufferView());
}

//�v���~�e�B�u�̃g�|���W�[ �ݒ�
void RenderContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
    this->command_List_->IASetPrimitiveTopology(topology);
}

//�R�}���h���X�g �ݒ�
void RenderContext::SetCommandList(ID3D12GraphicsCommandList5* commandList)
{
    this->command_List_ = commandList;
}

void RenderContext::SetViewportAndScissor(D3D12_VIEWPORT& viewport)
{
    //�V�U�����O��`
    D3D12_RECT scissorRect;
    scissorRect.bottom = static_cast<LONG>(viewport.Height);
    scissorRect.top = 0;
    scissorRect.left = 0;
    scissorRect.right = static_cast<LONG>(viewport.Width);
    SetScissorRect(scissorRect);

    this->command_List_->RSSetViewports(1, &viewport);
    this->current_Viewport_ = viewport;

}

//�V�U�����O��` �ݒ�
void RenderContext::SetScissorRect(D3D12_RECT& rect)
{
    this->command_List_->RSSetScissorRects(1, &rect);
}


//���[�g�V�O�l�`���@�ݒ�
void RenderContext::SetRootSignature(ID3D12RootSignature* rootSignature)
{
    this->command_List_->SetGraphicsRootSignature(rootSignature);
}

//���[�g�V�O�l�`���@�ݒ�
void RenderContext::SetRootSignature(RootSignature& rootSignature)
{
    this->command_List_->SetGraphicsRootSignature(rootSignature.GetRootSignature());
}

//���[�g�V�O�l�`���@�ݒ�
void RenderContext::SetComputeRootSignature(ID3D12RootSignature* rootSignature)
{
    this->command_List_->SetComputeRootSignature(rootSignature);
}

//���[�g�V�O�l�`���@�ݒ�
void RenderContext::SetComputeRootSignature(RootSignature& rootSignature)
{
    this->command_List_->SetComputeRootSignature(rootSignature.GetRootSignature());
}

//�p�C�v���C���X�e�[�g �ݒ�
void RenderContext::SetPipelineState(ID3D12PipelineState* pipelineState)
{
    this->command_List_->SetPipelineState(pipelineState);
}

//�p�C�v���C���X�e�[�g �ݒ�
void RenderContext::SetPipelineState(PipelineState& pipelineState)
{
    this->command_List_->SetPipelineState(pipelineState.GetPipelineState());
}

void RenderContext::SetComputeRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
{
    this->command_List_->SetComputeRootDescriptorTable(RootParameterIndex, BaseDescriptor);
}
