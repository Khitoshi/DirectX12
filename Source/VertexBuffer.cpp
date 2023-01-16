#include "VertexBuffer.h"

VertexBuffer::VertexBuffer():
    vertex_Buffer_(),
    vertex_Buffer_View_()
{
}

VertexBuffer::~VertexBuffer()
{
}

//������
void VertexBuffer::Init(GraphicsEngine* graphicsEngine, int size, int stride)
{
    //�f�o�C�X �擾
    auto device = graphicsEngine->GetD3DDevice();
    //�q�[�v�̐ݒ� �擾
    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    //���\�[�X�̃T�C�Y �擾
    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

    //RESOURCE ����
    HRESULT hr = device->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertex_Buffer_)
    );
    
    //���� �`�F�b�N
    if (FAILED(hr))
    {
        //TODO:MYASSERT����������
        exit(-1);
    }

    //���_�o�b�t�@�̖��O �Z�b�g
    vertex_Buffer_->SetName(L"VertexBuffer");

    //GPU�̉��z�A�h���X �Z�b�g
    vertex_Buffer_View_.BufferLocation = vertex_Buffer_->GetGPUVirtualAddress();
    //�o�b�t�@�[�̃T�C�Y �Z�b�g
    vertex_Buffer_View_.SizeInBytes = size;
    //�e���_�G���g���̃T�C�Y �Z�b�g
    vertex_Buffer_View_.StrideInBytes = stride;
}

void VertexBuffer::Copy(void* strVertices)
{
    uint8_t* date = nullptr;
    
    //map�J�n
    vertex_Buffer_->Map(0, nullptr, (void**)date);

    //���������R�s�[�@(map���Ă���̂�date��vertexbuffer�ɓ���)
    memcpy(date, strVertices, this->vertex_Buffer_View_.SizeInBytes);
    
    //map����
    this->vertex_Buffer_->Unmap(0,nullptr);
}
