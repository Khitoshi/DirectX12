#include "Texture.h"
#include "GraphicsEngine.h"
#include "d3dx12.h"
#include <d3d12.h>
#include <ResourceUploadBatch.h>
#include <DDSTextureLoader.h>


using namespace DirectX;

//�R���X�g���N�^
Texture::Texture():
    texture_(nullptr),
    texture_Desc_()
{
}

//�����I �R���X�g���N�^
Texture::Texture(GraphicsEngine*& graphicsEngine,const wchar_t* filePath):
    texture_(nullptr),
    texture_Desc_()
{
    //������
    InitFromDDSFile(graphicsEngine, filePath);
}

//�f�X�g���N�^
Texture::~Texture()
{
}

//.DDS file ����e�N�X�`��������������
void Texture::InitFromDDSFile(GraphicsEngine*& graphicsEngine,const wchar_t* filePath)
{
    //DDS�t�@�C������e�N�X�`�������[�h�B
    LoadTextureFromDDSFile(graphicsEngine, filePath);
}

//����������e�N�X�`�� ������
void Texture::InitFromMemory(GraphicsEngine*& graphicsEngine,const char* memory, unsigned int size)
{
    //DDS�t�@�C������e�N�X�`�������[�h�B
    LoadTextureFromMemory(graphicsEngine,memory, size);
}

//D3D���\�[�X����e�N�X�`��������������
void Texture::InitFromD3DResource(ID3D12Resource*& texture)
{
    //���łɑ��݂��Ă���ꍇ�͉������
    if (this->texture_)texture->Release();

    //������
    this->texture_ = texture;
    this->texture_->AddRef();
    this->texture_Desc_ = this->texture_->GetDesc();
}

//SRV�ɓo�^
void Texture::RegistShaderResourceView(GraphicsEngine*& graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
{
    //�e�N�X�`�������݂��Ȃ��ꍇ
    if (!this->texture_) return;

    //�V�F�[�_�[���\�[�X�r���[�@�ݒ�
    D3D12_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
    shader_resource_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shader_resource_view_desc.Format = this->texture_Desc_.Format;
    shader_resource_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = this->texture_Desc_.MipLevels;
    
    ////�V�F�[�_�[���\�[�X�r���[ ����
    auto device = graphicsEngine->GetDevice();
    device->CreateShaderResourceView(
        this->texture_, 
        &shader_resource_view_desc, 
        descriptorHandle);
    
    /*
    graphicsEngine->GetDevice()->CreateShaderResourceView(
        this->texture_,
        &shader_resource_view_desc,
        descriptorHandle);

    
    graphicsEngine->CreateShaderResourceView(
        this->texture_,
        shader_resource_view_desc,
        descriptorHandle);
        */
    return;
}

//.DDS file ����e�N�X�`�������[�h
void Texture::LoadTextureFromDDSFile(GraphicsEngine*& graphicsEngine, const wchar_t* filePath)
{
    //ResourceUploadBatch resouce_upload_batch(device);
    ResourceUploadBatch resouce_upload_batch(graphicsEngine->GetDevice());
    resouce_upload_batch.Begin();

    ComPtr<ID3D12Resource> texture;
    
    //DDS�e�N�X�`���𐶐�����
    HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
        //device,
        graphicsEngine->GetDevice(),
        resouce_upload_batch,
        filePath,
        0,
        D3D12_RESOURCE_FLAG_NONE,
        0,
        &texture
    );

    //ResourceUploadBatch���I������
    resouce_upload_batch.End(graphicsEngine->GetCommandQueue());
    
    //�����m�F
    if (FAILED(hr))
    {
        //���s�����̂ňُ�I������
        MessageBoxA(nullptr, "CreateDDSTextureFromFileEx�Ɏ��s���܂���", "�G���[", MB_OK);
        std::abort();
    }

    this->texture_ = texture.Get();
    this->texture_Desc_ = this->texture_->GetDesc();

    //texture->Release();
}

//����������e�N�X�`�������[�h
void Texture::LoadTextureFromMemory(GraphicsEngine*& graphicsEngine, const char* memory, unsigned int size)
{
    
    ResourceUploadBatch resouce_upload_batch(graphicsEngine->GetDevice());
    resouce_upload_batch.Begin();
    ComPtr<ID3D12Resource> texture;
    
    //DDS�e�N�X�`�������� ����
    HRESULT hr = CreateDDSTextureFromMemoryEx(
        graphicsEngine->GetDevice(),
        resouce_upload_batch,
        (const uint8_t*)memory,
        size,
        0,
        D3D12_RESOURCE_FLAG_NONE,
        0,
        &texture
    );

    //ResourceUploadBatch���I������
    resouce_upload_batch.End(graphicsEngine->GetCommandQueue());

    //�����m�F
    if (FAILED(hr))
    {
        //���s�����̂ňُ�I������
        MessageBoxA(nullptr, "CreateDDSTextureFromMemoryEx�Ɏ��s���܂���", "�G���[", MB_OK);
        std::abort();
    }

    this->texture_ = texture.Get();
    this->texture_Desc_ = this->texture_->GetDesc();
}
