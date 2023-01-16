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

public://set method
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
    void SetPipelineState(PipelineState* pipelineState);
    
    /// <summary>
    /// �f�B�X�N���v�^�q�[�v �ݒ�
    /// </summary>
    /// <param name="descriptorHeap">�f�B�X�N���v�^�q�[�v</param>
    void SetDescriptorHeap(ID3D12DescriptorHeap* descriptorHeap);

    //TODO:�@�����܂łł����B�����DescriptorHeap.h�쐬����

public://get method
    D3D12_VIEWPORT GetViewport()const { return this->current_Viewport; }

private:
    //�f�B�X�N���v�^�q�[�v�̍ő吔�B
    enum { MAX_DESCRIPTOR_HEAP = 4 };	
    //�萔�o�b�t�@�̍ő吔�B���ʉ�
    enum { MAX_CONSTANT_BUFFER = 8 };	
    //�V�F�[�_�[���\�[�X�̍ő吔�B���ʉ�
    enum { MAX_SHADER_RESOURCE = 16 };	

private:
    //�R�}���h ���X�g
    ComPtr<ID3D12GraphicsCommandList4> command_List;
    //���݂̃r���[�|�[�g�B
    D3D12_VIEWPORT current_Viewport;
    //�f�B�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descriptor_Heap_[MAX_DESCRIPTOR_HEAP];

};