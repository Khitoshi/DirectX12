#include "IndexBuffer.h"

//�f�t�H���g �R���X�g���N�^
IndexBuffer::IndexBuffer():
    index_Buffer_(),
    index_Buffer_View_(),
    index_Count_(0),
    stride_In_Bytes_(0),
    size_In_Bytes_(0)
{
}

//�f�t�H���g �f�X�g���N�^
IndexBuffer::~IndexBuffer()
{
}

//������
void IndexBuffer::Init(GraphicsEngine*& graphicsEngine,int size, int stride)
{
    //�T�C�Y��ݒ�
    if (stride == 2)
    {
        this->size_In_Bytes_ = size * 2;
    }
    else
    {
        this->size_In_Bytes_ = size;
    }
    //�q�[�v�̐ݒ� �擾
    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    //���\�[�X�̃T�C�Y �擾
    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(this->size_In_Bytes_);
    

    //RESOURCE ����
    graphicsEngine->CreateCommittedResource(
        heapProp,
        D3D12_HEAP_FLAG_NONE,
        resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        this->index_Buffer_
    );

    //�����`�F�b�N
    //if (FAILED(hr))
    //{
    //    //TODO :MYASSERT����������
    //    std::abort();
    //}

    //�C���f�b�N�X�o�b�t�@�̃r���[ ����
    this->index_Buffer_View_.BufferLocation = this->index_Buffer_->GetGPUVirtualAddress();

    //�X�g���C�h��4byte�Œ�
    this->stride_In_Bytes_ = 4;
    this->index_Buffer_View_.Format = DXGI_FORMAT_R32_UINT;
    this->index_Buffer_View_.SizeInBytes = this->size_In_Bytes_;

    //�C���f�b�N�X����ݒ�
    this->index_Count_ = this->size_In_Bytes_ / this->stride_In_Bytes_;
}

//�C���f�b�N�X�f�[�^���C���f�b�N�X�o�b�t�@�ɃR�s�[
void IndexBuffer::Copy(uint16_t* srcIndecies)
{

    uint32_t* data = nullptr;

    //�C���f�b�N�X�o�b�t�@�ɃR�s�[(map)����
    this->index_Buffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    for (int i = 0; i < this->index_Count_; i++)
    {
        data[i] = srcIndecies[i];
    }

    //map������
    this->index_Buffer_->Unmap(0, nullptr);
}

//�C���f�b�N�X�f�[�^���C���f�b�N�X�o�b�t�@�ɃR�s�[
void IndexBuffer::Copy(uint32_t* srcIndecies)
{
    uint32_t* data = nullptr;

    //�C���f�b�N�X�o�b�t�@�ɃR�s�[(map)����
    this->index_Buffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    for (int i = 0; i < this->index_Count_; i++)
    {
        data[i] = srcIndecies[i];
    }

    //map������
    this->index_Buffer_->Unmap(0, nullptr);

}




