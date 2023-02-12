#include "StructuredBuffer.h"
#include "GraphicsEngine.h"
#include <Windows.h>


//�f�t�H���g �R���X�g���N�^
StructuredBuffer::StructuredBuffer():
    buffers_On_GPU_(),
    buffers_On_CPU_(),

    element_Number_(0),
    element_Size_(0),

    is_Inited_(false)
{
}

//�f�t�H���g �f�X�g���N�^
StructuredBuffer::~StructuredBuffer()
{
}

//������
void StructuredBuffer::Init(GraphicsEngine*& graphicsEngine, int elementSize, int elementNumber, void* initData)
{
    
    //�G�������g�T�C�Y �擾
    this->element_Size_ = elementSize;
    //�G�������g�v�f�� �擾
    this->element_Number_ = elementNumber;

    //�o�b�t�@�ԍ�
    int buffer_number = 0;
    //�q�[�v���
    auto heap_prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    //���\�[�X�f�X�N�̃o�b�t�@�@�擾
    auto resource_desc = CD3DX12_RESOURCE_DESC::Buffer(this->element_Size_ * this->element_Number_);

    for (auto& buffer : this->buffers_On_GPU_)
    {
        //���\�[�X ����
        graphicsEngine->CreateCommittedResource(
            heap_prop,
            D3D12_HEAP_FLAG_NONE,
            resource_desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            buffer
        );

        ////�����@�`�F�b�N
        //if (FAILED(hr))
        //{
        //    MessageBox(nullptr, TEXT("GraphicsEngine::CreateSynchronizationWithGPUObject�̐����Ɏ��s"), TEXT("�G���["), MB_OK);
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

//SRV�ɓo�^
void StructuredBuffer::RegistShaderResourceView(GraphicsEngine*& graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNumber)
{
    //�������`�F�b�N
    if (!this->is_Inited_)return;

    D3D12_SHADER_RESOURCE_VIEW_DESC shader_recouce_view_desc;
    //�������u���b�N���[���Ŗ��߂�
    //ZeroMemory(&shader_recouce_view_desc, sizeof(shader_recouce_view_desc));
    SecureZeroMemory(&shader_recouce_view_desc, sizeof(shader_recouce_view_desc));

    //�V�F�[�_�[���\�[�X�r���[ �ݒ�
    shader_recouce_view_desc.Format = DXGI_FORMAT_UNKNOWN;
    shader_recouce_view_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    shader_recouce_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shader_recouce_view_desc.Buffer.FirstElement = 0;
    shader_recouce_view_desc.Buffer.NumElements = static_cast<UINT>(this->element_Number_);
    shader_recouce_view_desc.Buffer.StructureByteStride = this->element_Size_;
    shader_recouce_view_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

    //�V�F�[�_�[���\�[�X�r���[ ����
    //device.CreateShaderResourceView(
    //    this->buffers_On_GPU_[bufferNumber].Get(), 
    //    &shader_recouce_view_desc, 
    //    descriptorHandle
    //);

    graphicsEngine->CreateShaderResourceView(this->buffers_On_GPU_[bufferNumber], shader_recouce_view_desc, descriptorHandle);

}

//�\�����o�b�t�@�̓��e�X�V
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

