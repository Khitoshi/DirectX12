#include "RenderContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "PipelineState.h"
//�f�t�H���g �R���X�g���N�^
RenderContext::RenderContext():
    command_List()
{
}

//�f�t�H���g �f�X�g���N�^
RenderContext::~RenderContext()
{
}

//������
void RenderContext::Init(ID3D12GraphicsCommandList4* commandList)
{
    this->command_List = commandList;
}

//���_�o�b�t�@ �ݒ�
void RenderContext::SetVertexBuffer(VertexBuffer& vertexBuffer)
{
    //���_�o�b�t�@�r���[�ɏ��� �Z�b�g
    this->command_List->IASetVertexBuffers(0, 1, &vertexBuffer.GetVertexBufferView());
}

//�C���f�b�N�X�o�b�t�@��ݒ�
void RenderContext::SetIndexBuffer(IndexBuffer& indexBuffer)
{
    this->command_List->IASetIndexBuffer(&indexBuffer.GetIndexBufferView());
}

//�v���~�e�B�u�̃g�|���W�[ �ݒ�
void RenderContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
    this->command_List->IASetPrimitiveTopology(topology);
}

//�R�}���h���X�g �ݒ�
void RenderContext::SetCommandList(ID3D12GraphicsCommandList5* commandList)
{
    this->command_List = commandList;
}

void RenderContext::SetViewportAndScissor(D3D12_VIEWPORT& viewport)
{
    //�V�U�����O��`
    D3D12_RECT scissorRect;
    scissorRect.bottom = static_cast<LONG>(viewport.MaxDepth);
    scissorRect.top = 0;
    scissorRect.left = 0;
    scissorRect.right = static_cast<LONG>(viewport.MaxDepth);
    SetScissorRect(scissorRect);

    this->command_List->RSSetViewports(1, &viewport);
    this->current_Viewport = viewport;

}

//�V�U�����O��` �ݒ�
void RenderContext::SetScissorRect(D3D12_RECT& rect)
{
    this->command_List->RSSetScissorRects(1, &rect);
}


//���[�g�V�O�l�`���@�ݒ�
void RenderContext::SetRootSignature(ID3D12RootSignature* rootSignature)
{
    this->command_List->SetGraphicsRootSignature(rootSignature);
}

//���[�g�V�O�l�`���@�ݒ�
void RenderContext::SetRootSignature(RootSignature& rootSignature)
{
    this->command_List->SetGraphicsRootSignature(rootSignature.GetRootSignature());
}

//���[�g�V�O�l�`���@�ݒ�
void RenderContext::SetComputeRootSignature(ID3D12RootSignature* rootSignature)
{
    this->command_List->SetComputeRootSignature(rootSignature);
}

//���[�g�V�O�l�`���@�ݒ�
void RenderContext::SetComputeRootSignature(RootSignature& rootSignature)
{
    this->command_List->SetComputeRootSignature(rootSignature.GetRootSignature());
}

//�p�C�v���C���X�e�[�g �ݒ�
void RenderContext::SetPipelineState(ID3D12PipelineState* pipelineState)
{
    this->command_List->SetPipelineState(pipelineState);
}

//�p�C�v���C���X�e�[�g �ݒ�
void RenderContext::SetPipelineState(PipelineState* pipelineState)
{
    this->command_List->SetPipelineState(pipelineState->GetPipelineState());
}

//�f�B�X�N���v�^�q�[�v �ݒ�
void RenderContext::SetDescriptorHeap(ID3D12DescriptorHeap* descriptorHeap)
{
    this->descriptor_Heap_[0] = descriptorHeap;
    this->command_List->SetDescriptorHeaps(1, this->descriptor_Heap_->GetAddressOf());
}
