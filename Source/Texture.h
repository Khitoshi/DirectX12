#pragma once

#include "IShaderResource.h"
#include <wrl.h>

using namespace Microsoft::WRL;

class Texture :public IShaderResource
{
public:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    Texture();

    /// <summary>
    /// �����I �R���X�g���N�^
    /// </summary>
    /// <param name = "filePath">���[�h����e�N�X�`���̃t�@�C���p�X</param>
    explicit Texture(GraphicsEngine* graphicsEngine,const wchar_t* filePath);

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~Texture();

    /// <summary>
    /// DDS�t�@�C������e�N�X�`��������������B
    /// </summary>
    /// <param name="filePath">���[�h����e�N�X�`���̃t�@�C���p�X�B</param>
    void InitFromDDSFile(GraphicsEngine* graphicsEngine,const wchar_t* filePath);

    /// <summary>
    /// ����������e�N�X�`��������������B
    /// </summary>
    /// <param name="memory">�e�N�X�`���f�[�^���i�[����Ă��郁�����̐擪�A�h���X</param>
    /// <param name="size">�e�N�X�`���̃T�C�Y�B</param>
    void InitFromMemory(GraphicsEngine* graphicsEngine,const char* memory, unsigned int size);
    
    /// <summary>
    /// D3D���\�[�X����e�N�X�`��������������B
    /// </summary>
    /// <param name="resrouce">D3D���\�[�X�B</param>
    void InitFromD3DResource(ID3D12Resource* texture);
    
    /// <summary>
    /// SRV�ɓo�^�B
    /// </summary>
    /// <param name="descriptorHandle"></param>
    void RegistShaderResourceView(GraphicsEngine* graphicsEngine,D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

private:

    /// <summary>
    /// DDS�t�@�C������e�N�X�`�������[�h�B
    /// </summary>
    /// <param name="filePath">�t�@�C���p�X�B</param>
    void LoadTextureFromDDSFile(GraphicsEngine* graphicsEngine,const wchar_t* filePath);

    /// <summary>
    /// ����������e�N�X�`�������[�h�B
    /// </summary>
    /// <param name="memory">�e�N�X�`���f�[�^���i�[����Ă��郁�����̐擪�A�h���X�B</param>
    /// <param name="size">�e�N�X�`���̃T�C�Y�B</param>
    /// <param name="ge12">Dx12�ł̃O���t�B�b�N�X�G���W��</param>
    /// <param name="device">D3D�f�o�C�X</param>
    void LoadTextureFromMemory(GraphicsEngine* graphicsEngine,const char* memory, unsigned int size);


public:
#pragma region Get method
    
    /// <summary>
    /// �e�N�X�`�����L��������B
    /// </summary>
    /// <returns>true���Ԃ��Ă�����L���B</returns>
    bool IsValid() const
    {
        return this->texture_ != nullptr;
    }

    /// <summary>
    /// �e�N�X�`���擾
    /// </summary>
    /// <returns></returns>
    ID3D12Resource* Get()
    {
        return this->texture_.Get();
    }

    /// <summary>
    /// �e�N�X�`���̕����擾�B
    /// </summary>
    /// <returns></returns>
    int GetWeight()const
    {
        return static_cast<int>(this->texture_Desc_.Width); 
    }
    
    /// <summary>
    /// �e�N�X�`���̍������擾�B
    /// </summary>
    /// <returns></returns>
    int GetHeight() const
    {
        return static_cast<int>(this->texture_Desc_.Height);
    }

    /// <summary>
    /// �e�N�X�`���̃t�H�[�}�b�g���擾�B
    /// </summary>
    /// <returns></returns>
    DXGI_FORMAT GetFormat() const
    {
        return this->texture_Desc_.Format;
    }

#pragma endregion

private:
    //�e�N�X�`���B
    ComPtr<ID3D12Resource> texture_ = nullptr;	
    //�e�N�X�`�����
    D3D12_RESOURCE_DESC texture_Desc_;	
};

