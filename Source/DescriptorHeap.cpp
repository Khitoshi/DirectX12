#include "DescriptorHeap.h"
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "IShaderResource.h"
#include "IUnorderAccessResrouce.h"

//�f�t�H���g �R���X�g���N�^
DescriptorHeap::DescriptorHeap():
    descriptor_Heap_(),
    shader_Resources_(MAX_SHADER_RESOURCE),
    uav_Resources_(MAX_SHADER_RESOURCE),
    constant_Buffers_(MAX_CONSTANT_BUFFER),
    sampler_Descs_(),
    const_Buffer_Gpu_Descriptor_Start_(),
    shader_Resource_Gpu_Descriptor_Start_(),
    uav_Gpu_Descriptor_Start_(),
    sampler_Gpu_Descriptor_Start_(),

    num_Shader_Resource_(0),
    num_Constant_Buffer_(0),
    num_Uav_Resource_(0),
    num_Sampler_Desc_(0),
    num_Descriptor_Heap_(0)
{
}

//�f�t�H���g �f�X�g���N�^
DescriptorHeap::~DescriptorHeap()
{
}

//�V�F�[�_�[���\�[�X���f�B�X�N���v�^�q�[�v�ɓo�^
void DescriptorHeap::RegistShaderResource(int registerNo, IShaderResource& shaderResource)
{
    RegistResource(
        registerNo,
        &shaderResource,
        &this->shader_Resources_.front(),
        this->num_Shader_Resource_,
        MAX_SHADER_RESOURCE,
        L"DescriptorHeap::RegistShaderResource() ���W�X�^�ԍ����͈͊O�ł��B"
    );
}

//�A���I�[�_�[�A�N�Z�X���\�[�X��o�^
void DescriptorHeap::RegistUnorderAccessResource(int registerIndex, IUnorderAccessResrouce& unorderAccessResrouce)
{
    RegistResource(
        registerIndex,
        &unorderAccessResrouce,
        &this->uav_Resources_.front(),
        this->num_Uav_Resource_,
        MAX_SHADER_RESOURCE,
        L"DescriptorHeap::RegistUnorderAccessResource() ���W�X�^�ԍ����͈͊O�ł��B"
    );
}

//�萔�o�b�t�@���f�B�X�N���v�^�q�[�v�ɓo�^
void DescriptorHeap::RegistConstantBuffer(int registerIndex, ConstantBuffer& constantBuffer)
{
    RegistResource(
        registerIndex,
        &constantBuffer,
        &this->constant_Buffers_.front(),
        this->num_Constant_Buffer_,
        MAX_CONSTANT_BUFFER,
        L"DescriptorHeap::RegistConstantBuffer() ���W�X�^�ԍ����͈͊O�ł��B"
    );
}

//�T���v����`���f�B�X�N���v�^�q�[�v�ɒǉ�
void DescriptorHeap::RegistSamplerDesc(int registerIndex, const D3D12_SAMPLER_DESC& samplerDesc)
{
    RegistResource(
        registerIndex,
        samplerDesc,
        this->sampler_Descs_,
        this->num_Sampler_Desc_,
        MAX_SAMPLER_STATE,
        L"DescriptorHeap::RegistSamplerDesc() ���W�X�^�ԍ����͈͊O�ł��B"
    );
}

//�f�B�X�N���v�^�q�[�v�ւ̓o�^���m��
void DescriptorHeap::Commit(GraphicsEngine* graphicsEngine)
{
    //�f�o�C�X���擾
    const auto& device = graphicsEngine->GetD3DDevice();

    //�V�F�[�_�[���\�[�X�r���[�̃f�B�X�N���v�^�q�[�v �f�X�N
    D3D12_DESCRIPTOR_HEAP_DESC srv_Heap_Desc = {};
    //�V�F�[�_�[���\�[�X�� + �萔�o�b�t�@�� + �A���I�[�_�[�A�N�Z�X���\�[�X��
    //���f�B�X�N���v�^���ɐݒ�
    srv_Heap_Desc.NumDescriptors = num_Shader_Resource_ + num_Constant_Buffer_+ num_Uav_Resource_;
    srv_Heap_Desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srv_Heap_Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    //�f�B�X�N���v�^ ���� loop
    for (auto& descriptor_Heap : this->descriptor_Heap_) 
    {
        //�f�B�X�N���v�^�q�[�v ����
        HRESULT hr = device.CreateDescriptorHeap(&srv_Heap_Desc, IID_PPV_ARGS(&descriptor_Heap));
        //�f�B�X�N���v�^�� �C���N�������g
        this->num_Descriptor_Heap_++;

        //�����`�F�b�N
        if (FAILED(hr))
        {
            //�������s
            MessageBox(nullptr, L"DescriptorHeap::Commit �f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B", L"�G���[", MB_OK);
            std::abort();
            //�����I��
            std::abort();
        }
    }

    //�萔�o�b�t�@��V�F�[�_�[���\�[�X�̃f�B�X�N���v�^���q�[�v�ɏ�������
    int buffer_Index = 0;
    for (auto& descriptor_heap : this->descriptor_Heap_)
    {
        //CPU �n���h���擾
        auto cpu_Handle = descriptor_heap->GetCPUDescriptorHandleForHeapStart();
        //GPU �n���h������
        auto gpu_Handle = descriptor_heap->GetGPUDescriptorHandleForHeapStart();


        //�萔�o�b�t�@��o�^
        for (auto& constant_buffer:this->constant_Buffers_)
        {
            if (constant_buffer != nullptr)
            {
                constant_buffer->RegistConstantBufferView(graphicsEngine, cpu_Handle, buffer_Index);
            }
            //���ɐi�߂�B
            cpu_Handle.ptr += graphicsEngine->GetCbrSrvDescriptorSize();
        }

        //�����ăV�F�[�_�[���\�[�X�B
        for (int i = 0; i < this->num_Shader_Resource_; i++) 
        {
            if (this->shader_Resources_[i] != nullptr) 
            {
                this->shader_Resources_[i]->RegistShaderResourceView(*graphicsEngine,cpu_Handle, buffer_Index);
            }
            //���ɐi�߂�B
            cpu_Handle.ptr += graphicsEngine->GetCbrSrvDescriptorSize();
        }

        //������UAV�B
        for (int i = 0; i < this->num_Uav_Resource_; i++) {
            if (this->uav_Resources_[i] != nullptr) {
                this->uav_Resources_[i]->RegistUnorderAccessView(cpu_Handle, buffer_Index);
            }
            //���ɐi�߂�B
            cpu_Handle.ptr += graphicsEngine->GetCbrSrvDescriptorSize();
        }

        //�萔�o�b�t�@�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h�����v�Z�B
        this->const_Buffer_Gpu_Descriptor_Start_[buffer_Index] = gpu_Handle;
        //�V�F�[�_�[���\�[�X�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h�����v�Z�B
        this->shader_Resource_Gpu_Descriptor_Start_[buffer_Index] = gpu_Handle;
        this->uav_Gpu_Descriptor_Start_[buffer_Index].ptr += (UINT64)graphicsEngine->GetCbrSrvDescriptorSize() * this->num_Constant_Buffer_;
        //UAV���\�[�X�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h�����v�Z�B
        this->uav_Gpu_Descriptor_Start_[buffer_Index] = gpu_Handle;
        this->uav_Gpu_Descriptor_Start_[buffer_Index].ptr += (UINT64)graphicsEngine->GetCbrSrvDescriptorSize() * (this->num_Shader_Resource_ + this->num_Constant_Buffer_);

        //�|�C���^��i�߂�
        gpu_Handle.ptr += (UINT64)graphicsEngine->GetCbrSrvDescriptorSize() * (this->num_Shader_Resource_ + this->num_Constant_Buffer_ + this->num_Uav_Resource_);

        //�o�b�t�@�����C���N�������g
        buffer_Index++;
    }
}

//�T���v���X�e�[�g�p�̃f�B�X�N���v�^�q�[�v�ւ̓o�^
void DescriptorHeap::CommitSamplerHeap(GraphicsEngine* graphicsEngine)
{
    //�f�o�C�X
    const auto& device = graphicsEngine->GetD3DDevice();

    //�V�F�[�_�[���\�[�X�r���[�@�q�[�v�f�X�N
    D3D12_DESCRIPTOR_HEAP_DESC srv_Heap_Desc = {};
    
    //�V�F�[�_�[���\�[�X�r���[�@�q�[�v�f�X�N�ݒ�
    srv_Heap_Desc.NumDescriptors = this->num_Sampler_Desc_;
    srv_Heap_Desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
    srv_Heap_Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    //�f�B�X�N���v�^�q�[�v�@���� loop
    for (auto& descriptor_Heap : this->descriptor_Heap_) {
        //����
        HRESULT hr = device.CreateDescriptorHeap(&srv_Heap_Desc, IID_PPV_ARGS(&descriptor_Heap));

        //���� �`�F�b�N
        if (FAILED(hr)) {
            //���s
            MessageBox(nullptr, L"DescriptorHeap::Commit �f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B", L"�G���[", MB_OK);
            //�ُ�I��
            std::abort();
        }
    }

    int buffer_Index = 0;
    //�T���v���X�e�[�g���f�B�X�N���v�^�q�[�v�ɓo�^���Ă���loop
    for (auto& descriptorHeap : this->descriptor_Heap_) {
        //CPU�̃n���h�����擾
        auto cpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
        //GPU�̃n���h�����擾
        auto gpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();

        for (int i = 0; i < this->num_Sampler_Desc_; i++) {
            //�T���v���X�e�[�g���f�B�X�N���v�^�q�[�v�ɓo�^
            device.CreateSampler(&this->sampler_Descs_[i], cpuHandle);
            cpuHandle.ptr += graphicsEngine->GetSapmerDescriptorSize();
        }
        this->sampler_Gpu_Descriptor_Start_[buffer_Index] = gpuHandle;
        //�o�b�t�@�̐����C���N�������g
        buffer_Index++;
    }
}

//���\�[�X���f�B�X�N���v�^�q�[�v�ɓo�^
template<class T>
void DescriptorHeap::RegistResource(int registerIndex, T res, T resTable[], int& numRes, const int MAX_RESOURCE, const wchar_t* errorMessage)
{
    if (registerIndex == -1)
    {
        //-1���w�肳��Ă���̂ŁC
        //���ݓo�^����Ă��郊�\�[�X�̈�Ԍ��ɓo�^
        registerIndex = numRes;
    }

    //�o�^�ԍ������\�[�X�ő�l�ȉ��̏ꍇ
    if (registerIndex < MAX_RESOURCE)
    {
        //�o�^
        resTable[registerIndex] = res;

        if(numRes < registerIndex + 1)
        {
            //�o�^����Ă��郊�\�[�X���𑝂₷
            numRes = registerIndex + 1;
        }
    }
    else
    {
        //���\�[�X�̍ő�l��葽���ꍇ
        //�G���[���b�Z�[�W���o��
        MessageBox(nullptr, errorMessage, L"DescriptorHeap::RegistResource�Ŏ��s", MB_OK);
        std::abort();
        //�����I��
        std::abort();
    }
}
