#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include <Windows.h>


//�f�t�H���g �R���X�g���N�^
ConstantBuffer::ConstantBuffer() :
    constant_Buffer_(),
    const_Buffer_CPU_(),
    constant_Buffer_Size_(0),
    alloc_Size_(0),
    is_Valid_(false)
{
}

//���[�u�R���X�g���N�^
ConstantBuffer::ConstantBuffer(ConstantBuffer&& cb)
{
    //cb ���� this�ɏ��n
    //cb ���� this�ɒ萔�o�b�t�@�̏��n
    this->constant_Buffer_[0] = cb.constant_Buffer_[0];
    this->constant_Buffer_[1] = cb.constant_Buffer_[1];
    //cb ���� this��CPU����A�N�Z�X�ł���萔�o�b�t�@�̏��n
    this->const_Buffer_CPU_[0] = cb.const_Buffer_CPU_[0];
    this->const_Buffer_CPU_[1] = cb.const_Buffer_CPU_[1];
    //cb ���� �萔�o�b�t�@�̃T�C�Y�����n
    this->constant_Buffer_Size_ = cb.constant_Buffer_Size_;
    //cb ���� alloc�̃T�C�Y�����n
    this->alloc_Size_ = cb.alloc_Size_;
    //cb ����@is_Valid_�����n
    this->is_Valid_ = cb.is_Valid_;

    //�R�s�[���̍폜
    //�萔�o�b�t�@ �폜
    cb.constant_Buffer_[0] = nullptr;
    cb.constant_Buffer_[1] = nullptr;
    //CPU����A�N�Z�X�ł���萔�o�b�t�@ �폜
    cb.const_Buffer_CPU_[0] = nullptr;
    cb.const_Buffer_CPU_[1] = nullptr;
}

//�f�t�H���g �f�X�g���N�^
ConstantBuffer::~ConstantBuffer()
{
}

//������
void ConstantBuffer::Init(GraphicsEngine* graphicsEngine, int constantBufferSize, void* srcData)
{
    //�f�o�C�X�擾
    auto device = graphicsEngine->GetD3DDevice();

    //�萔�o�b�t�@�̏�����
    this->constant_Buffer_Size_ = constantBufferSize;

    //�萔�o�b�t�@��256�o�C�g�A���C�����g���v�������̂�,256�̔{���ɐ؂�グ��
    this->alloc_Size_ = (constantBufferSize + 256) & 0xFFFFFF00;
    //�o�b�t�@�ԍ�
    int buffer_Index = 0;
    //�q�[�v�ݒ�
    auto heap_Prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    //���\�[�X�T�C�Y�ݒ�
    auto resource_Desc = CD3DX12_RESOURCE_DESC::Buffer(this->alloc_Size_);

    //�萔�o�b�t�@ ����
    for (auto& constant_Buffer : this->constant_Buffer_)
    {
        //����
        HRESULT hr = device.CreateCommittedResource(
            &heap_Prop,
            D3D12_HEAP_FLAG_NONE,
            &resource_Desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&constant_Buffer)
        );

        //���� �`�F�b�N
        /*
        if (FAILED(hr))
        {
            //���� ���s
            //���b�Z�[�W�{�b�N�X����
            MessageBox(nullptr, TEXT("ConstantBuffer::Init�Ő����Ɏ��s���܂���"), L"�G���[", MB_OK);
            //�v���O�������ُ�I��������B
            std::abort();
        }*/

        //�萔�o�b�t�@��CPU����A�N�Z�X�\�ȉ��z�A�h���X��ԂɃ}�b�s���O����B
        //�}�b�v�A�A���}�b�v�̃I�[�o�[�w�b�h���y�����邽�߂ɂ͂��̃C���X�^���X�������Ă���Ԃ͍s��Ȃ��B
        {
            CD3DX12_RANGE read_Range(0, 0);
            constant_Buffer->Map(0, &read_Range, reinterpret_cast<void**>(&this->const_Buffer_CPU_[buffer_Index]));
        }
        if (srcData != nullptr) {
            memcpy(this->const_Buffer_CPU_[buffer_Index], srcData, this->constant_Buffer_Size_);
        }
        //���݂̔ԍ��̐������I�������̂ŃC���N�������g����
        buffer_Index++;
    }
    //���p���\�ɂ���
    this->is_Valid_ = true;
}

//�f�[�^��VRAM�ɃR�s�[����
void ConstantBuffer::CopyToVRAM(GraphicsEngine* graphicsEngine, void* data)
{
    auto backBufferIndex = graphicsEngine->GetBackBufferIndex();
    memcpy(this->const_Buffer_CPU_[backBufferIndex], data, this->constant_Buffer_Size_);
}

//�f�[�^��VRAM�ɃR�s�[����
/*
template<class T>
void ConstantBuffer::CopyToVRAM(GraphicsEngine* graphicsEngine, T& data)
{
    CopyToVRAM(graphicsEngine, &data);
}
*/

//�f�B�X�N���v�^�q�[�v�ɒ萔�o�b�t�@�r���[��o�^
void ConstantBuffer::RegistConstantBufferView(GraphicsEngine* graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
{
    //�o�b�N�o�b�t�@�����擾
    auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
    //�f�B�X�N���v�^�q�[�v�ɒ萔�o�b�t�@�r���[��o�^
    RegistConstantBufferView(graphicsEngine, descriptorHandle, back_Buffer_Index);
}

//�f�B�X�N���v�^�q�[�v�ɒ萔�o�b�t�@�r���[��o�^
void ConstantBuffer::RegistConstantBufferView(GraphicsEngine* graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
{
    //�f�o�C�X���擾�B
    auto device = graphicsEngine->GetD3DDevice();
    D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
    desc.BufferLocation = this->constant_Buffer_[bufferNo]->GetGPUVirtualAddress();
    desc.SizeInBytes = this->alloc_Size_;
    device.CreateConstantBufferView(&desc, descriptorHandle);
}

//�f�B�X�N���v�^�q�[�v�ɒ萔�o�b�t�@�r���[��o�^
D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGPUVirtualAddress(GraphicsEngine* graphicsEngine)
{
    auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
    return this->constant_Buffer_[back_Buffer_Index]->GetGPUVirtualAddress();
}